/* vi: set sw=4 ts=4: */
/*
 * lyric-downloader-tt.h
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

#ifndef _LMP_LYRIC_DOWNLOAD_H_
#define _LMP_LYRIC_DOWNLOAD_H_

#define MAXPATHLEN 1024

#include <glib-object.h>
#include "lyric-downloader.h"

G_BEGIN_DECLS
#define LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT (lmplayer_lyric_downloader_tt_get_type())
#define LMPLAYER_LYRIC_DOWNLOADER_TT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT, LmplayerLyricDownloaderTt))
#define LMPLAYER_LYRIC_DOWNLOADER_TT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT, LmplayerLyricDownloaderTtClass))
#define LMPLAYER_IS_LYRIC_DOWNLOADER_TT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT))
#define LMPLAYER_IS_LYRIC_DOWNLOADER_TT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT))
#define LMPLAYER_LYRIC_DOWNLOADER_TT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT, LmplayerLyricDownloaderTtClass))

typedef struct _LmplayerLyricDownloaderTTPrivate LmplayerLyricDownloaderTTPrivate;

typedef struct 
{
	LmplayerLyricDownloader parent;

	LmplayerLyricDownloaderTTPrivate *priv;
} LmplayerLyricDownloaderTT;

typedef struct
{
	LmplayerLyricDownloaderClass parent;
} LmplayerLyricDownloaderTTClass;


GType lmplayer_lyric_downloader_tt_get_type();

G_END_DECLS
#endif // _LMP_LYRIC_DOWNLOAD_H_

