/* vi: set sw=4 ts=4: */
/*
 * lyric-widget-text.h
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

#ifndef __LYRIC_WIDGET_TEXT_H__
#define __LYRIC_WIDGET_TEXT_H__  1

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define LMPLAYER_TYPE_LYRIC_WIDGET_TEXT (lmplayer_lyric_widget_text_get_type())
#define LMPLAYER_LYRIC_WIDGET_TEXT(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LMPLAYER_TYPE_LYRIC_WIDGET_TEXT, LmplayerLyricWidgetText))
#define LMPLAYER_LYRIC_WIDGET_TEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), LMPLAYER_TYPE_LYRIC_WIDGET_TEXT, LmplayerLyricWidgetTextClass))
#define LMPLAYER_IS_LYRIC_WIDGET_TEXT(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LMPLAYER_TYPE_LYRIC_WIDGET_TEXT))
#define IS_LMPLAYER_LYRIC_WIDGET_TEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), LMPLAYER_TYPE_LYRIC_WIDGET_TEXT))
#define LMPLAYER_LYRIC_WIDGET_TEXT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), LMPLAYER_TYPE_LYRIC_WIDGET_TEXT, LmplayerLyricWidgetTextClass))

typedef struct _LmplayerLyricWidgetTextPrivate LmplayerLyricWidgetTextPrivate;

typedef struct 
{
	GtkScrolledWindow parent;

	LmplayerLyricWidgetTextPrivate *priv;
} LmplayerLyricWidgetText;

typedef struct
{
	GtkScrolledWindowClass parent;
} LmplayerLyricWidgetTextClass;

GType lmplayer_lyric_widget_text_get_type();

GtkWidget * lmplayer_lyric_widget_text_new();

G_END_DECLS

#endif /*__LYRIC_WIDGET_TEXT_H__ */
