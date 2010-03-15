/* vi: set sw=4 ts=4: */
/*
 * lyric-downloader.h
 *
 * This file is part of ________.
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

#ifndef __LYRIC_DOWNLOADER_H__
#define __LYRIC_DOWNLOADER_H__  1

#include <glib-object.h>

G_BEGIN_DECLS

#define LMPLAYER_LYRIC_DOWNLOADER_TYPE (lmplayer_lyric_downloader_get_type())
#define LMPLAYER_LYRIC_DOWNLOADER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LMPLAYER_LYRIC_DOWNLOADER_TYPE, LmplayerLyricDownloader))
#define LMPLAYER_LYRIC_DOWNLOADER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LMPLAYER_LYRIC_DOWNLOADER_TYPE, LmplayerLyricDownloaderClass))
#define LMPLAYER_IS_LYRIC_DOWNLOADER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LMPLAYER_LYRIC_DOWNLOADER_TYPE))
#define LMPLAYER_IS_LYRIC_DOWNLOADER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LMPLAYER_LYRIC_DOWNLOADER_TYPE))
#define LMPLAYER_LYRIC_DOWNLOADER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LMPLAYER_LYRIC_DOWNLOADER_TYPE, LmplayerLyricDownloaderClass))

typedef struct _LmplayerLyricDownloaderPrivate LmplayerLyricDownloaderPrivate;

typedef struct 
{
	GObject parent;

	LmplayerLyricDownloaderPrivate *priv;
} LmplayerLyricDownloader;

typedef struct
{
	GObjectClass parent;
} LmplayerLyricDownloaderClass;


G_END_DECLS
#endif /*__LYRIC_DOWNLOADER_H__ */
