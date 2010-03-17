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

#define TICKS 200 //ms

static gboolean
timer_cb(SkinLyric *lyric_widget)
{
	static int i = 0;

	skin_lyric_set_current_second(lyric_widget, i * TICKS / 1000);

	++i;
	return TRUE;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *lyric_widget = (GtkWidget *)skin_lyric_new();

	skin_lyric_set_size(lyric_widget, 300, 500);
	gtk_widget_show(lyric_widget);

	gtk_container_add(GTK_CONTAINER(win), lyric_widget);

	skin_lyric_add_file(lyric_widget, "test.lrc");
	g_timeout_add(200, (GSourceFunc)timer_cb, lyric_widget);

	gtk_widget_show_all(win);

	gtk_main();
	return 0;
}
