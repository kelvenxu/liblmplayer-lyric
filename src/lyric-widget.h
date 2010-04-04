/* vi: set sw=4 ts=4: */
/*
 * skinlyric.h
 *
 * This file is part of libskin.
 *
 * Copyright (C) 2008 - kelvenxu <kelvenxu@gmail.com>.
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

#ifndef __SKINLYRIC_H__
#define __SKINLYRIC_H__  1

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS
#define LMPLAYER_TYPE_LYRIC_WIDGET (lmplayer_lyric_widget_get_type())
#define LMPLAYER_LYRIC_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), LMPLAYER_TYPE_LYRIC_WIDGET, LmplayerLyricWidget))
#define LMPLAYER_IS_LYRIC_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), LMPLAYER_TYPE_LYRIC_WIDGET))
#define LMPLAYER_LYRIC_WIDGET_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE((obj), LMPLAYER_TYPE_LYRIC_WIDGET, LmplayerLyricWidgetInterface))

typedef struct _LmplayerLyricWidget LmplayerLyricWidget; /* dummy object */
typedef struct _LmplayerLyricWidgetInterface      LmplayerLyricWidgetInterface;

typedef struct _LmplayerLyricWidgetInterface
{
	GTypeInterface parent_iface;

	void (*get_size) (LmplayerLyricWidget *lyric, gint *width, gint *height);
	void (*set_size) (LmplayerLyricWidget *lyric, gint width, gint height);
  gboolean (*add_file) (LmplayerLyricWidget *lyric, const gchar *file);
	void (*set_current_second) (LmplayerLyricWidget *lyric, gint sec);
	void (*set_bg_color) (LmplayerLyricWidget *lyric, const GdkColor *color);
	void (*set_text_color) (LmplayerLyricWidget *lyric, const GdkColor *color);
	void (*set_highlight_color) (LmplayerLyricWidget *lyric, const GdkColor *color);

	void (*set_color) (LmplayerLyricWidget *lyric, const GdkColor *bg, const GdkColor *text, const GdkColor *hilight);
};

GType lmplayer_lyric_widget_get_type();

void lmplayer_lyric_widget_get_size(LmplayerLyricWidget *lyric, gint *width, gint *height);
void lmplayer_lyric_widget_set_size(LmplayerLyricWidget *lyric, gint width, gint height);
gboolean lmplayer_lyric_widget_add_file(LmplayerLyricWidget *lyric, const gchar *file);
void lmplayer_lyric_widget_set_current_second(LmplayerLyricWidget *lyric, gint sec);
void lmplayer_lyric_widget_set_bg_color(LmplayerLyricWidget *lyric, const GdkColor *color);
void lmplayer_lyric_widget_set_text_color(LmplayerLyricWidget *lyric, const GdkColor *color);
void lmplayer_lyric_widget_set_highlight_color(LmplayerLyricWidget *lyric, const GdkColor *color);

void lmplayer_lyric_widget_set_color(LmplayerLyricWidget *lyric, const GdkColor *bg, const GdkColor *text, const GdkColor *hilight);

G_END_DECLS

#endif /*__SKINLYRIC_H__ */

