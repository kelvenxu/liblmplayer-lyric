/* vi: set sw=4 ts=4: */
/*
 * main.c
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


#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <stdio.h>
#include <stdlib.h>
#include "lyric-widget.h"
#include "lyric-widget-da.h"
#include "lyric-widget-text.h"

#define TICKS 1000 //ms

static gboolean
timer_cb(LmplayerLyricWidget *lyric_widget)
{
	static int i = 0;
	static int f = 1;

	lmplayer_lyric_widget_set_current_second(lyric_widget, i * TICKS / 1000);

	++i;

	if(i > 30)
	{
		if(f % 3 == 1)
		{
			lmplayer_lyric_widget_add_file(LMPLAYER_LYRIC_WIDGET(lyric_widget), "test1.lrc");
		}
		else if( f % 3 == 2)
		{
			lmplayer_lyric_widget_add_file(LMPLAYER_LYRIC_WIDGET(lyric_widget), "test2.lrc");
		}
		else
		{
			lmplayer_lyric_widget_add_file(LMPLAYER_LYRIC_WIDGET(lyric_widget), "test.lrc");
		}

		i = 0;
		++f;
	}

	return TRUE;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	//GtkWidget *lyric_widget = lmplayer_lyric_widget_da_new();
	GtkWidget *lyric_widget = lmplayer_lyric_widget_text_new();

	lmplayer_lyric_widget_set_size(LMPLAYER_LYRIC_WIDGET(lyric_widget), 300, 500);
	gtk_widget_show(lyric_widget);

	GtkStyle *style = gtk_widget_get_style(win);
	
	lmplayer_lyric_widget_set_bg_color(LMPLAYER_LYRIC_WIDGET(lyric_widget), &(style->bg[0]));
	lmplayer_lyric_widget_set_text_color(LMPLAYER_LYRIC_WIDGET(lyric_widget), &(style->fg[0]));

	GdkColor light;
	light.red = 65535;
	light.green = 0;
	light.blue = 0;
	//lmplayer_lyric_widget_set_highlight_color(lyric_widget, &(style->light[1]));
	lmplayer_lyric_widget_set_highlight_color(LMPLAYER_LYRIC_WIDGET(lyric_widget), &light);

	gtk_container_add(GTK_CONTAINER(win), lyric_widget);

	lmplayer_lyric_widget_add_file(LMPLAYER_LYRIC_WIDGET(lyric_widget), "test.lrc");
	g_timeout_add(TICKS, (GSourceFunc)timer_cb, lyric_widget);

	g_signal_connect(win, "destroy", gtk_main_quit, NULL);
	gtk_widget_show_all(win);

	gtk_main();
	return 0;
}

