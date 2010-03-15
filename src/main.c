/* vi: set sw=4 ts=4: */
/*
 * main.c
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

static GtkWidget *list_vbox = NULL;
static GtkWidget *child = NULL;
static GtkWidget *dialog = NULL;
static GSList *group = NULL;
static GtkWidget *first_btn = NULL;
static gint selected_id = 0;

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
	g_signal_connect(dialog, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
}

static void
list_toggled_cb(GtkToggleButton *button, gpointer *user_data)
{
}

static void
create_radio_button_and_pack(const char* label, GtkWidget *box)
{
	GtkWidget *btn = NULL;
	if(first_btn)
	{
		btn = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(first_btn), label);
	}
	else
	{
		first_btn = gtk_radio_button_new_with_label(NULL, label);
		btn = first_btn;
	}

	gtk_box_pack_start(GTK_BOX(box), btn, FALSE, FALSE, 6);
	g_signal_connect(btn, "toggled", G_CALLBACK(list_toggled_cb), NULL);
}

static void
list_setup(GSList *list)
{
	if(GTK_IS_WIDGET(child))
	{
		gtk_widget_destroy(child);
		child = NULL;
		first_btn = NULL;
	}

	child = gtk_vbox_new(FALSE, 6);

	g_slist_foreach(list, (GFunc)create_radio_button_and_pack, child);

	gtk_box_pack_start(GTK_BOX(list_vbox), child, FALSE, FALSE, 6);
}

static gboolean
timer_cb(gpointer user_data)
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

	list_setup(list);
	g_slist_foreach(list, (GFunc)g_free, NULL);
	g_slist_free(list);

	gtk_widget_show_all(dialog);
	return TRUE;
}

int main()
{
	gtk_init(NULL, NULL);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	dialog_setup();
	g_timeout_add(2000, timer_cb, NULL);

	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show(win);

	gtk_main();
}

