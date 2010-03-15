/* vi: set sw=4 ts=4: */
/*
 * lyric-downloader.c
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

#include "lyric-downloader.h"

G_DEFINE_TYPE (LmplayerLyricDownloader, lmplayer_lyric_downloader, G_TYPE_OBJECT)

#define LMPLAYER_LYRIC_DOWNLOADER_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), LMPLAYER_TYPE_LYRIC_DOWNLOADER, LmplayerLyricDownloaderPrivate))

struct _LmplayerLyricDownloaderPrivate 
{
	int i;
};

enum {
	DOWNLOAD_FINISHED,
	DOWNLOAD_FAILED,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

static void
lmplayer_lyric_downloader_dispose (LmplayerLyricDownloader *self)
{
}

static void
lmplayer_lyric_downloader_finalize (LmplayerLyricDownloader *self)
{
}

static void
lmplayer_lyric_downloader_init(LmplayerLyricDownloader *self)
{
	LmplayerLyricDownloaderPrivate *priv;

	priv = LMPLAYER_LYRIC_DOWNLOADER_GET_PRIVATE (self);
}

static void
lmplayer_lyric_downloader_class_init (LmplayerLyricDownloaderClass *self_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (self_class);

	g_type_class_add_private (self_class, sizeof (LmplayerLyricDownloaderPrivate));
	object_class->dispose = (void (*) (GObject *object)) lmplayer_lyric_downloader_dispose;
	object_class->finalize = (void (*) (GObject *object)) lmplayer_lyric_downloader_finalize;

	self_class->download = (void (*)) lmplayer_lyric_downloader_download;

	self_class->download_finished = NULL;
	self_class->download_failed = NULL;

  signals[DOWNLOAD_FINISHED] = g_signal_new(
			"download_finished",
		  G_TYPE_FROM_CLASS(object_class),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET(LmplayerLyricDownloaderClass, download_finished),
		  NULL, 
			NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 
			0);

  signals[DOWNLOAD_FAILED] = g_signal_new(
			"download_failed",
		  G_TYPE_FROM_CLASS(object_class),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET(LmplayerLyricDownloaderClass, download_failed),
		  NULL, 
			NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 
			0);
}

void
lmplayer_lyric_downloader_download(LmplayerLyricDownloader *downloader, const char *title, const char *artist)
{
	LmplayerLyricDownloaderClass *self_class = LMPLAYER_LYRIC_DOWNLOADER_GET_CLASS(downloader);
	if(self_class->download)
		self_class->download(downloader, title, artist);
	//g_print("empty start to download lyric\n");
}

