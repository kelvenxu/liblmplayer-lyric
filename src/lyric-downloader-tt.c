/* vi: set sw=4 ts=4: */
/*
 * lyric-downloader-tt.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "lyric-downloader-tt.h"
#include "tt_codefunc.h"
#include "http_fetcher.h"
#include "lmp_debug.h"

static gboolean is_downloading = FALSE;


G_DEFINE_TYPE (LmplayerLyricDownloaderTT, lmplayer_lyric_downloader_tt, LMPLAYER_TYPE_LYRIC_DOWNLOADER)

#define LMPLAYER_LYRIC_DOWNLOADER_TT_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT, LmplayerLyricDownloaderTTPrivate))

struct _LmplayerLyricDownloaderTTPrivate 
{
	int placehold;
};

static void tt_lyric_download(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist);

static void
lmplayer_lyric_downloader_tt_dispose (LmplayerLyricDownloaderTT *self)
{
}

static void
lmplayer_lyric_downloader_tt_finalize (LmplayerLyricDownloaderTT *self)
{
}

static void
lmplayer_lyric_downloader_tt_init (LmplayerLyricDownloaderTT *self)
{
	LmplayerLyricDownloaderTTPrivate *priv;

	priv = LMPLAYER_LYRIC_DOWNLOADER_TT_GET_PRIVATE (self);
}

static void
lmplayer_lyric_downloader_tt_class_init (LmplayerLyricDownloaderTTClass *self_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (self_class);

	g_type_class_add_private (self_class, sizeof (LmplayerLyricDownloaderTTPrivate));
	object_class->dispose = (void (*) (GObject *object)) lmplayer_lyric_downloader_tt_dispose;
	object_class->finalize = (void (*) (GObject *object)) lmplayer_lyric_downloader_tt_finalize;

	LmplayerLyricDownloaderClass *parent_class = LMPLAYER_LYRIC_DOWNLOADER_CLASS(self_class);
	
	parent_class->download = tt_lyric_download;
}

static void
tt_lyric_download(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist)
{
	g_print("really start to download tt lyric\n");
}

static gchar* 
tt_http_request(char* url)
{
	lmp_debug(" ");
	int ret;
	char *fileBuf = NULL;				/* Pointer to downloaded data */

	ret = http_fetch(url, &fileBuf);	/* Downloads page */
	lmp_debug(" ");
	if(ret == -1)						/* All HTTP Fetcher functions return */
	{
		http_perror("http_fetch");		/*	-1 on error. */
		return NULL;
	}
	
	lmp_debug(" ");
	return fileBuf;
}

//g_free when no use again.
static gchar* 
tt_encode_artist_title(const char* str)
{
	lmp_debug(" ");
	int i, j;
	char temp[64];

	assert(str != NULL);
	int str_len = strlen(str);
	int utf16_len = g_utf8_strlen(str, str_len) * 2;

	lmp_debug(" ");
	char* re = g_new0(char, str_len * 6 + 1); //FIXME:这里应是多大才好呢？

	lmp_debug(" ");
	gunichar2* utf16_str = g_utf8_to_utf16(str, 
			str_len,
			NULL,
			NULL,
			NULL);

	lmp_debug(" ");
	char* p = (char*)utf16_str;

	lmp_debug("str: %s", str);
	for(i = 0, j = 0; i < utf16_len; ++i)
	{
		sprintf(temp, "%02x", p[i]);
		//lmp_debug("i = %d, temp = %s", i, temp);
		int n = strlen(temp);
		re[j] = temp[n - 2];
		++j;
		re[j] = temp[n - 1];
		++j;
	}
	re[j] = '\0';

	lmp_debug(" ");
	g_free(utf16_str);
	return re;
}

//g_free when no use again.
static gchar* 
tt_remove_blankspace_lower(const gchar* src)
{
	lmp_debug(" ");

	gchar* s = NULL;
	int len = strlen(src);

	if(len == 0)
	{
		s = g_strdup(" ");
		return s;
	}

	lmp_debug("g_new0 begin: len(src) = %d", len);

	s = g_new0(gchar, len + 1);

	lmp_debug("g_new0 end");

	int i, j;
	for(i = 0, j = 0; i < len; ++i)
	{
		if(src[i] != ' ')
		{
			s[j] = g_ascii_tolower(src[i]);
			++j;
		}
	}
	s[j] = '\0';

	lmp_debug(" ");
	return s;
}

gchar* 
tt_get_lyrics_list(const gchar* artist, const gchar* title)
{
	lmp_debug("artist: %s, title: %s", artist, title);
	assert(artist != NULL);
	assert(title != NULL);
	gchar* art1 = tt_remove_blankspace_lower(artist);
	gchar* art2 = tt_encode_artist_title(art1);

	lmp_debug("g_free(art1)");
	g_free(art1); //FIXME:

	gchar* tit1 = tt_remove_blankspace_lower(title);
	gchar* tit2 = tt_encode_artist_title(tit1);
	g_free(tit1); //FIXME:

	gchar* url = g_strdup_printf("http://lrcct2.ttplayer.com/dll/lyricsvr.dll?sh?Artist=%s&Title=%s&Flags=0", art2, tit2);

	g_free(art2); //FIXME:
	g_free(tit2); //FIXME:
	gchar* xml = tt_http_request(url);
	g_free(url);

	return xml;
}

gchar* 
tt_get_lyrics_content(gint id, gchar* artist, gchar* title)
{
	lmp_debug(" ");
	gchar* art_tit = g_strconcat(artist, title, NULL);
	gchar* url = g_strdup_printf(
			"http://lrcct2.ttplayer.com/dll/lyricsvr.dll?dl?Id=%d&Code=%d&uid=01&mac=%012x",
			id, 
			tt_CodeFunc(id, art_tit), 
			g_random_int_range(0, 0x7FFFFFFF));

	//printf("url = %s\n", url);

	gchar* buf = tt_http_request(url);
	lmp_debug("++%s", buf);

	g_free(url);

	return buf;
}

GSList* 
tt_parse_lyricslist(gchar* xml)
{
	lmp_debug(" ");
	xmlDoc* doc;
	xmlNode* root_node;
	xmlNode* cur_node;

	GSList* list = NULL;
	int i = 0;

	if(xml == NULL)
		return NULL;

	doc = xmlReadDoc((const xmlChar*)xml, NULL, "UTF-8", 0);
	if(doc == NULL)
	{
		lmp_debug("can't parse xml string\n");
		return NULL;
	}

	root_node = xmlDocGetRootElement(doc);
	if(root_node == NULL)
	{
		g_print("parse xml string failed\n");
		return NULL;
	}

	for(cur_node = root_node->children; cur_node; cur_node = cur_node->next)
	{
		LyricId* item = g_new0(LyricId, 1);
		item->no = i;
		item->id = (gchar*)xmlGetProp(cur_node, (xmlChar*)"id");
		if(item->id == NULL) //FIXME: 为什么中间会有一个NULL元素
			continue;
		item->artist = (gchar*)xmlGetProp(cur_node, (xmlChar*)"artist");
		item->title = (gchar*)xmlGetProp(cur_node, (xmlChar*)"title");
		list = g_slist_append(list, item);
		++i;
	}

	xmlFreeDoc(doc);

	return list;
}

typedef struct _thread_arg
{
	gchar filename[MAXPATHLEN];
	gchar artist[MAXPATHLEN];
	gchar title[MAXPATHLEN];
} ThreadArg;

static gchar* 
lmp_lyric_download1(ThreadArg* arg)
{
	lmp_debug("in thread: filename: %s, artist: %s, title: %s", 
			arg->filename, arg->artist, arg->title);
	lmp_lyric_download(arg->filename,
			arg->artist,
			arg->title);

	return NULL;
}

void 
lmp_lyric_download_with_thread( gchar* filename, gchar* artist, gchar* title)
{
	GError* error = NULL;
	static ThreadArg arg;

	if(is_downloading)
		return;

	is_downloading = TRUE;

	g_strlcpy(arg.filename ,filename, MAXPATHLEN);
	g_strlcpy(arg.artist, artist, MAXPATHLEN);
	g_strlcpy(arg.title, title, MAXPATHLEN);

	//lmp_debug("before thread: filename: %s, artist: %s, title: %s", 
	//		arg.filename, arg.artist, arg.title);
	g_thread_create((GThreadFunc)lmp_lyric_download1,
			&arg, TRUE, &error);

	if(error)
	{
		lmp_debug("error: %s", error->message);
	}
}

gchar* 
lmp_lyric_download( gchar* lyric_filename, gchar* artist, gchar* title)
{
	if(lyric_filename == NULL)
	{
		fprintf(stderr, _("no path or filename given.\n"));
		return NULL;
	}

	if(artist == NULL || title == NULL)
	{
		fprintf(stderr, _("no artist or title given.\n"));
		return NULL;
	}

	lmp_debug("artist: %s, title: %s", artist, title);
	gchar* xml = tt_get_lyrics_list(artist, title);

	if(xml == NULL)
		return NULL;

	GSList* lyrics_list = tt_parse_lyricslist(xml);
	g_free(xml);

	if(lyrics_list != NULL)
	{
		guint num = g_slist_length(lyrics_list);
		if(num == 1)
		{
			lmp_debug("只有一条，没得选择了！");
			LyricId* item = g_slist_nth_data(lyrics_list, 0);
			unsigned int id = atoi(item->id);
			tt_get_lyrics_content_and_save(id, item->artist, item->title, lyric_filename);
		}
		else
		{
			lmp_debug("有多条选择，弹出选择对话框！");
			//tt_lyrics_list_selection_dialog(lyrics_list, lyric_filename);
		}
	}
	else
	{
		lmp_debug("从网络下载歌词失败！");
		return NULL;
	}

	lmp_debug("歌词下载成功，并保存： %s", lyric_filename);
	g_slist_free(lyrics_list);
	return lyric_filename;
}

gboolean 
tt_get_lyrics_content_and_save( unsigned int id, gchar* artist, gchar* title, const gchar* lyric_filename)
{
	FILE* fp;
	gchar* buf = tt_get_lyrics_content(id, artist, title);

	fp = fopen(lyric_filename, "w+");
	if(fp != NULL)
	{
		fwrite(buf, strlen(buf), 1, fp);
		fclose(fp);
		return TRUE;
	}

	return FALSE;
}

