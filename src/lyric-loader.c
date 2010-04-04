/* vi: set sw=4 ts=4: */
/*
 * lyric-loader.c
 *
 * This file is part of liblmplayer-lyric.
 *
 * Copyright (C) 2010 - kelvenxu <kelvenxu@gmail.com>.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 * */

#include "lyric-loader.h"
#include <glib.h>
#include <string.h>
#include <stdio.h>


G_DEFINE_TYPE (LmplayerLyricLoader, lmplayer_lyric_loader, G_TYPE_OBJECT)

#define LMPLAYER_LYRIC_LOADER_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE((o), LMPLAYER_TYPE_LYRIC_LOADER, LmplayerLyricLoaderPrivate))


struct _LmplayerLyricLoaderPrivate 
{
	gchar *songname;
	gchar *lyricname;

	gchar *title;
	gchar *artist;
	gchar *album;
	gchar *author;
	gint offset;

	// 只存储歌词文本本身
	GList *texts;

	// 存储时间，其中的text指向texts中存储的歌词
	GList *lines;
	gint nlines;
};

static void lmplayer_lyric_loader_free(LmplayerLyricLoader *self);

static void
lmplayer_lyric_loader_dispose (LmplayerLyricLoader *self)
{
	lmplayer_lyric_loader_free(self);
}


static void
lmplayer_lyric_loader_finalize (LmplayerLyricLoader *self)
{
}

static void
lmplayer_lyric_loader_init (LmplayerLyricLoader *self)
{
	LmplayerLyricLoaderPrivate *priv;

	priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE (self);
	
	priv->songname = NULL;
	priv->lyricname = NULL;
	priv->title = NULL;
	priv->artist = NULL;
	priv->album = NULL;
	priv->author = NULL;
	priv->texts = NULL;
	priv->lines = NULL;
	priv->nlines = 0;
	priv->offset = 0;
}

static void
lmplayer_lyric_loader_class_init (LmplayerLyricLoaderClass *self_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (self_class);

	g_type_class_add_private (self_class, sizeof (LmplayerLyricLoaderPrivate));
	object_class->dispose = (void (*) (GObject *object)) lmplayer_lyric_loader_dispose;
	object_class->finalize = (void (*) (GObject *object)) lmplayer_lyric_loader_finalize;
}


//NOTE: str: [mm:ss.ss
static void 
str2time(const gchar* str, int* sec, int* msec)
{
	//str: [mm:ss.ss
	int m = 0, s = 0, ms = 0;
	int len = strlen(str);
	gchar p[3];

	if(len >= 3)
	{
		p[0] = str[1];
		p[1] = str[2];
		p[2] = '\0';
		m = atoi(p);
	}

	if(len >= 6)
	{
		p[0] = str[4];
		p[1] = str[5];
		p[2] = '\0';
		s = atoi(p);
	}

	if(len >= 9)
	{
		p[0] = str[7];
		p[1] = str[8];
		p[2] = '\0';
		ms = atoi(p);
	}

	*sec = m * 60 + s;
	*msec = ms;
}

static gboolean
parse_lyric_line(LmplayerLyricLoader *self, const gchar *line, gint line_no_)
{
	char** p = NULL;
	char* text = NULL;
	int sec, ms; 
	int old_sec = -1;
	gboolean flag = FALSE;

	g_return_if_fail(self);
	LmplayerLyricLoaderPrivate *priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE(self);

	char** parray = g_strsplit(line, "]", 5);

	//find lyric text string
	for(p = parray; *p; ++p)
	{
		if((*p)[0] == '[' && (*p)[3] == ':')
			continue;
		else
		{
			text = g_strdup(*p);
			priv->texts = g_list_prepend(priv->texts, text);
			break;
		}
	}

	for(p = parray; *p; ++p)
	{ 
		if((*p)[0] == '[' && (*p)[3] == ':')
		{
			flag = TRUE;
			str2time(*p, &sec, &ms);

			if(old_sec == sec) //FIXME: 解决两个歌词时间相同的问题
				++sec;
			else
				old_sec = sec;

			LmplayerLyricLine* data = g_new0(LmplayerLyricLine, 1);
			data->text = text;
			data->sec = sec;
			data->ms = ms;
			// 这里没有顺序
			priv->lines = g_list_prepend(priv->lines, data);
		}
	}

	g_strfreev(parray);

	return flag;
}

gint
lmplayer_lyric_line_compare(LmplayerLyricLine *line1, LmplayerLyricLine *line2)
{
	if(line1->sec == line2->sec)
		return 0;
	if(line1->sec > line2->sec)
		return 1;

	return -1;
}

static void
lmplayer_lyric_loader_parse_post(LmplayerLyricLoader *self)
{
	int i;
	GList *iter;

	g_return_if_fail(self);
	LmplayerLyricLoaderPrivate *priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE(self);

	priv->lines = g_list_sort(priv->lines, (GCompareFunc)lmplayer_lyric_line_compare);

	// fill nth 
	for(iter = priv->lines, i = 0; iter; iter = iter->next, ++i)
	{
		LmplayerLyricLine *ll = (LmplayerLyricLine *)iter->data;
		ll->nth = i;
	}
}

static gboolean
lmplayer_lyric_loader_parse_file(LmplayerLyricLoader *self, const gchar *filename)
{
	gchar buf[1024];
	gchar *pbuf;
	gint n;
	gboolean flag = FALSE;

	LmplayerLyricLoaderPrivate *priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE(self);

	g_return_val_if_fail(self, FALSE);
	g_return_val_if_fail(filename, FALSE);

	FILE* fp = fopen(filename, "r");

	if(fp == NULL) return FALSE;

	n = 0;
	while(fgets(buf, 1024, fp))
	{
		g_strstrip(buf);
		if(strlen(buf) == 0 || buf[0] != '[')
			continue;

		if(g_utf8_validate(buf, -1, NULL))
			pbuf = g_strdup(buf);
		else 
			pbuf = g_convert(buf, -1, "utf-8", "gb18030", NULL, NULL, NULL);

		//title:
		if(strncmp(pbuf, "[ti:", 4) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->title = (gchar*)g_strdup(&pbuf[4]);
			flag = TRUE;
			g_free(pbuf);
			continue;
		}

		//artist:
		if(strncmp(pbuf, "[ar:", 4) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->artist = g_strdup(&pbuf[4]);
			flag = TRUE;
			g_free(pbuf);
			continue; 
		}

		//album:
		if(strncmp(pbuf, "[al:", 4) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->album = g_strdup(&pbuf[4]);
			flag = TRUE;
			g_free(pbuf);
			continue;
		}

		//author
		if(strncmp(pbuf, "[by:", 4) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->author = g_strdup(&pbuf[4]);
			flag = TRUE;
			g_free(pbuf);
			continue;
		}

		//offset:
		if(strncmp(pbuf, "[offset:", 8) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->offset = atoi(&pbuf[8]);
			flag = TRUE;
			g_free(pbuf);
			continue;
		}

		flag = parse_lyric_line(self, pbuf, n);
		++n;
		g_free(pbuf);
	}

	fclose(fp);

	return flag;
}

static void
lmplayer_lyric_loader_free(LmplayerLyricLoader *self)
{
	g_return_if_fail(self);

	LmplayerLyricLoaderPrivate *priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE (self);

	g_return_if_fail(priv);

	if(priv->songname)
	{
		g_free(priv->songname);
		priv->songname = NULL;
	}
	if(priv->title)
	{
		g_free(priv->title);
		priv->title = NULL;
	}
	if(priv->artist)
	{
		g_free(priv->artist);
		priv->artist = NULL;
	}
	if(priv->album)
	{
		g_free(priv->album);
		priv->album = NULL;
	}
	if(priv->author)
	{
		g_free(priv->author);
		priv->author = NULL;
	}

	if(priv->texts)
	{
		g_list_foreach(priv->texts, (GFunc)g_free, NULL);
		g_list_free(priv->texts);
		priv->texts = NULL;
	}

	if(priv->lines)
	{
		g_list_foreach(priv->lines, (GFunc)g_free, NULL);
		g_list_free(priv->lines);
		priv->lines = NULL;
	}

	priv->nlines = 0;
	priv->offset = 0;
}

gboolean
lmplayer_lyric_loader_add_file(LmplayerLyricLoader *self, const gchar *filename)
{
	g_return_val_if_fail(self, FALSE);
	g_return_val_if_fail(filename, FALSE);

	lmplayer_lyric_loader_free(self);
	gboolean ret = lmplayer_lyric_loader_parse_file(self, filename);
	if(ret)
	{
		lmplayer_lyric_loader_parse_post(self);
	}

	return ret;
}

const gchar *
lmplayer_lyric_loader_get_artist(LmplayerLyricLoader *self)
{
	g_return_val_if_fail(self, NULL);

	LmplayerLyricLoaderPrivate *priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE (self);

	g_return_val_if_fail(priv, NULL);

	return priv->artist;
}

const gchar *
lmplayer_lyric_loader_get_album(LmplayerLyricLoader *self)
{
	g_return_val_if_fail(self, NULL);

	LmplayerLyricLoaderPrivate *priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE (self);

	g_return_val_if_fail(priv, NULL);

	return priv->album;
}

const gchar *
lmplayer_lyric_loader_get_author(LmplayerLyricLoader *self)
{
	g_return_val_if_fail(self, NULL);

	LmplayerLyricLoaderPrivate *priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE (self);

	g_return_val_if_fail(priv, NULL);

	return priv->author;
}

const gchar *
lmplayer_lyric_loader_get_title(LmplayerLyricLoader *self)
{
	g_return_val_if_fail(self, NULL);

	LmplayerLyricLoaderPrivate *priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE (self);

	g_return_val_if_fail(priv, NULL);

	return priv->title;
}

GList *
lmplayer_lyric_loader_get_lines(LmplayerLyricLoader *self)
{
	g_return_val_if_fail(self, NULL);

	LmplayerLyricLoaderPrivate *priv = LMPLAYER_LYRIC_LOADER_GET_PRIVATE (self);

	g_return_val_if_fail(priv, NULL);

	return priv->lines;
}

