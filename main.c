/* vi: set sw=4 ts=4: */
/*
 * main.c
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

#include <gtk/gtk.h>

GtkWidget *list_vbox;
GtkWidget *child;
GtkWidget *dialog;

static void 
dialog_response_cb(GtkDialog *dialog, gint response_id, gpointer user_data) 
{
	if(response_id == GTK_RESPONSE_ACCEPT)
	{
		g_print("accept\n");
		gtk_widget_hide(GTK_WIDGET(dialog));
	}
}

GtkWidget *
dialog_setup()
{
	dialog = gtk_dialog_new_with_buttons("My dialog",
			NULL,
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_STOCK_OK,
			GTK_RESPONSE_ACCEPT,
			GTK_STOCK_CANCEL,
			GTK_RESPONSE_REJECT,
			NULL);

	GtkWidget *vbox = gtk_vbox_new(FALSE, 6);

	GtkWidget *label = gtk_label_new("Select item below for download:");
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);

	GtkWidget *hbox1 = gtk_hbox_new(FALSE, 6);
	GtkWidget *placehold = gtk_label_new("    ");
	gtk_box_pack_start(GTK_BOX(hbox1), placehold, FALSE, FALSE, 0);
	list_vbox = gtk_vbox_new(FALSE, 6);
	gtk_box_pack_start(GTK_BOX(hbox1), list_vbox, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), vbox, FALSE, FALSE, 6);

	g_signal_connect(dialog, "response", G_CALLBACK(dialog_response_cb), NULL);
}

static void
list_toggled_cb(GtkToggleButton *button, gpointer *user_data)
{
}

void
list_setup(GSList *list)
{
	GSList *iter = NULL;
	GSList *group = NULL;

	if(child)
		gtk_container_remove(GTK_CONTAINER(list_vbox), child);

	child = gtk_vbox_new(FALSE, 6);

	for(iter = list; iter; iter = iter->next)
	{
		GtkWidget *btn = gtk_radio_button_new_with_label(group, (const char *)iter->data);

		if(!group)
			group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(btn));

		gtk_box_pack_start(GTK_BOX(child), btn, FALSE, FALSE, 6);
		g_signal_connect(btn, "toggled", G_CALLBACK(list_toggled_cb), NULL);
	}

	gtk_box_pack_start(GTK_BOX(list_vbox), child, FALSE, FALSE, 6);
}

static gboolean
timer_cb(gpointer user_data)
{
	static int i = 0;
	GSList *list = NULL;

	gchar buf[64];

	sprintf(buf, "item %d", i++);
	list = g_slist_append(list, buf);
	
	sprintf(buf, "item %d", i++);
	list = g_slist_append(list, buf);

	sprintf(buf, "item %d", i++);
	list = g_slist_append(list, buf);

	list_setup(list);

	gtk_widget_show_all(dialog);
	return TRUE;
}

int main()
{
	gtk_init(NULL, NULL);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	dialog_setup();
	g_timeout_add(2000, timer_cb, NULL);
	gtk_widget_show(win);
	gtk_main();
}

