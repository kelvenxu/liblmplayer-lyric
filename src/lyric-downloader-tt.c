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
#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include "lyric-downloader-tt.h"
#include "tt_lyric.h"

G_DEFINE_TYPE (LmplayerLyricDownloaderTT, lmplayer_lyric_downloader_tt, LMPLAYER_TYPE_LYRIC_DOWNLOADER)

#define LMPLAYER_LYRIC_DOWNLOADER_TT_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT, LmplayerLyricDownloaderTTPrivate))

struct _LmplayerLyricDownloaderTTPrivate 
{
	int placehold;
};

static void tt_lyric_download(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist, const char *output);
static GSList * tt_lyric_get_list(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist);

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
	parent_class->get_list = tt_lyric_get_list;
}

static void
tt_lyric_download(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist, const char *output)
{
	g_print("really start to download tt lyric\n");
}

static GSList *
tt_lyric_get_list(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist)
{
}

static void
tt_lyric_download_func(TTLyric *lyric)
{
	gboolean ret;
	ret = tt_get_lyric_content_and_save(lyric->id, lyric->artist, lyric->title, lyric->filename);
	if(ret)
	{
		// emit signal
	}
}

static void
tt_lyric_get_list_func(TTLyric *lyric)
{
	GSList *list = NULL;
	gchar *xml = tt_get_lyric_list(lyric->artist, lyric->title);
	if(xml)
	{
		list = tt_parse_lyricslist(xml);
		if(list)
		{
			// emit signal
		}
		else
		{
		}
	}

	// failed
}


