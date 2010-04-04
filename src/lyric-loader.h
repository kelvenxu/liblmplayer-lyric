/* vi: set sw=4 ts=4: */
/*
 * lyric-loader.h
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

#ifndef __LYRIC_LOADER_H__
#define __LYRIC_LOADER_H__  1
#include <glib-object.h>

G_BEGIN_DECLS
#define LMPLAYER_TYPE_LYRIC_LOADER (lmplayer_lyric_loader_get_type())
#define LMPLAYER_LYRIC_LOADER(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LMPLAYER_LYRIC_LOADER_TYPE, LmplayerLyricLoader))
#define LMPLAYER_LYRIC_LOADER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LMPLAYER_LYRIC_LOADER_TYPE, LmplayerLyricLoaderClass))
#define LMPLAYER_IS_LYRIC_LOADER(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LMPLAYER_LYRIC_LOADER_TYPE))
#define LMPLAYER_IS_LYRIC_LOADER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LMPLAYER_LYRIC_LOADER_TYPE))
#define LMPLAYER_LYRIC_LOADER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LMPLAYER_LYRIC_LOADER_TYPE, LmplayerLyricLoaderClass))

typedef struct _LmplayerLyricLoaderPrivate LmplayerLyricLoaderPrivate;

typedef struct 
{
	GObject parent;

	LmplayerLyricLoaderPrivate *priv;
} LmplayerLyricLoader;

typedef struct
{
	GObjectClass parent;
} LmplayerLyricLoaderClass;

typedef struct _LmplayerLyricLine
{
	// 时间标签
	gint sec;
	gint ms;

	// 第几句
	gint nth;

	// 歌词文本
	gchar *text;

} LmplayerLyricLine;

GType lmplayer_lyric_loader_get_type();

gboolean lmplayer_lyric_loader_add_file(LmplayerLyricLoader *self, const gchar *filename);

const gchar * lmplayer_lyric_loader_get_artist(LmplayerLyricLoader *self);
const gchar * lmplayer_lyric_loader_get_album(LmplayerLyricLoader *self);
const gchar * lmplayer_lyric_loader_get_author(LmplayerLyricLoader *self);
const gchar * lmplayer_lyric_loader_get_title(LmplayerLyricLoader *self);
GList * lmplayer_lyric_loader_get_lines(LmplayerLyricLoader *self);

gint lmplayer_lyric_line_compare(LmplayerLyricLine *line1, LmplayerLyricLine *line2);

G_END_DECLS
#endif /*__LYRIC_LOADER_H__ */
