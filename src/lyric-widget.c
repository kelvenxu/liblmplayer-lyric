/* vi: set sw=4 ts=4: */
/*
 * lyric-widget.c
 *
 * This file is part of liblmplayer-lyric.
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

#include "lyric-widget.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glib/gstdio.h>

static void
lmplayer_lyric_widget_base_init(LmplayerLyricWidget *self)
{
	static gboolean is_initialized = FALSE;

	if(!is_initialized)
	{

		is_initialized = TRUE;
	}
}

GType
lmplayer_lyric_widget_get_type (void)
{
	static GType iface_type = 0;
	if(iface_type == 0)
	{
		static const GTypeInfo info = 
		{
			sizeof(LmplayerLyricWidgetInterface),
			(GBaseInitFunc)lmplayer_lyric_widget_base_init,   /* base_init */
			NULL,   /* base_finalize */
		};

		iface_type = g_type_register_static(G_TYPE_INTERFACE, "LmplayerLyricWidget", &info, 0);
	}

	return iface_type;
}

void lmplayer_lyric_widget_get_size(LmplayerLyricWidget *lyric, gint *width, gint *height)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET(lyric));

	LMPLAYER_LYRIC_WIDGET_GET_INTERFACE(lyric)->get_size(lyric, width, height);
}

void
lmplayer_lyric_widget_set_size(LmplayerLyricWidget *lyric, gint width, gint height)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET(lyric));

	LMPLAYER_LYRIC_WIDGET_GET_INTERFACE(lyric)->set_size(lyric, width, height);
}

gboolean
lmplayer_lyric_widget_add_file(LmplayerLyricWidget *lyric, const gchar *file)
{
	g_return_val_if_fail(LMPLAYER_IS_LYRIC_WIDGET(lyric), FALSE);
	g_return_val_if_fail(file != NULL, FALSE);

	return LMPLAYER_LYRIC_WIDGET_GET_INTERFACE(lyric)->add_file(lyric, file);
}

void 
lmplayer_lyric_widget_set_current_second(LmplayerLyricWidget *lyric, gint sec)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET(lyric));
	g_return_if_fail(sec >= 0);

	LMPLAYER_LYRIC_WIDGET_GET_INTERFACE(lyric)->set_current_second(lyric, sec);
}

void 
lmplayer_lyric_widget_set_bg_color(LmplayerLyricWidget *lyric, const GdkColor *color)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET(lyric));
	g_return_if_fail(color != NULL);
	
	LMPLAYER_LYRIC_WIDGET_GET_INTERFACE(lyric)->set_bg_color(lyric, color);
}

void 
lmplayer_lyric_widget_set_text_color(LmplayerLyricWidget *lyric, const GdkColor *color)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET(lyric));
	g_return_if_fail(color != NULL);
	
	LMPLAYER_LYRIC_WIDGET_GET_INTERFACE(lyric)->set_text_color(lyric, color);
}

void 
lmplayer_lyric_widget_set_highlight_color(LmplayerLyricWidget *lyric, const GdkColor *color)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET(lyric));
	g_return_if_fail(color != NULL);
	
	LMPLAYER_LYRIC_WIDGET_GET_INTERFACE(lyric)->set_highlight_color(lyric, color);
}

void
lmplayer_lyric_widget_set_color(LmplayerLyricWidget *lyric, const GdkColor *bg,
									   const GdkColor *text,
									   const GdkColor *hilight)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET(lyric));

	LMPLAYER_LYRIC_WIDGET_GET_INTERFACE(lyric)->set_color(lyric, bg, text, hilight);
}

