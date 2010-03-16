/* vi: set sw=4 ts=4: */
/*
 * test-dialog.c
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

#include <gtk/gtk.h>
#include "lyric-selection-dialog.h"

static gboolean
timer_cb(GtkWidget *dlg)
{
	static int i = 0;
	GSList *list = NULL;

	gchar buf[64];

	sprintf(buf, "item %d", i++);
	list = g_slist_append(list, g_strdup(buf));

	sprintf(buf, "item %d", i++);
	list = g_slist_append(list, g_strdup(buf));

	sprintf(buf, "item %d", i++);
	list = g_slist_append(list, g_strdup(buf));

	lmplayer_lyric_selection_dialog_set_list(LMPLAYER_LYRIC_SELECTION_DIALOG(dlg), list);
	g_slist_foreach(list, (GFunc)g_free, NULL);
	g_slist_free(list);

	gtk_widget_show_all(dlg);
	return TRUE;
}
 
int main()
{
	gtk_init(NULL, NULL);
 
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 
	GtkWidget *dlg = lmplayer_lyric_selection_dialog_new();

	g_timeout_add(2000, (GSourceFunc)timer_cb, dlg);
 
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show(win);
 
	gtk_main();
}

