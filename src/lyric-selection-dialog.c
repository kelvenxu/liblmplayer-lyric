/* vi: set sw=4 ts=4: */
/*
 * lyric-selection-dialog.c
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

#include "lyric-selection-dialog.h"
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include "tt_lyric.h"


G_DEFINE_TYPE (LmplayerLyricSelectionDialog, lmplayer_lyric_selection_dialog, GTK_TYPE_DIALOG)

#define LMPLAYER_LYRIC_SELECTION_DIALOG_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), LMPLAYER_TYPE_LYRIC_SELECTION_DIALOG, LmplayerLyricSelectionDialogPrivate))

struct _LmplayerLyricSelectionDialogPrivate 
{
	GtkWidget *container;
	GtkWidget *widget;
	GtkWidget *first_btn;

	GList *btn_list;
	//gint id;
	gint index;

	GSList *lyrics_list;
};

static void
lmplayer_lyric_selection_dialog_dispose(LmplayerLyricSelectionDialog *self)
{
	LmplayerLyricSelectionDialogPrivate *priv;

	priv = LMPLAYER_LYRIC_SELECTION_DIALOG_GET_PRIVATE (self);

	if(priv->btn_list)
	{
		g_list_free(priv->btn_list);
		priv->btn_list = NULL;
	}
}

static void
lmplayer_lyric_selection_dialog_finalize(LmplayerLyricSelectionDialog *self)
{
}

static void
lmplayer_lyric_selection_dialog_init(LmplayerLyricSelectionDialog *self)
{
	LmplayerLyricSelectionDialogPrivate *priv;

	priv = LMPLAYER_LYRIC_SELECTION_DIALOG_GET_PRIVATE (self);

	priv->container = NULL;
	priv->widget = NULL;
	priv->first_btn = NULL;

	priv->btn_list = NULL;
	priv->index = 0;

	priv->lyrics_list = NULL;
}

static void
lmplayer_lyric_selection_dialog_class_init(LmplayerLyricSelectionDialogClass *self_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (self_class);

	g_type_class_add_private (self_class, sizeof (LmplayerLyricSelectionDialogPrivate));
	object_class->dispose = (void (*) (GObject *object)) lmplayer_lyric_selection_dialog_dispose;
	object_class->finalize = (void (*) (GObject *object)) lmplayer_lyric_selection_dialog_finalize;
}

#if 0
static void
dialog_response_cb(GtkDialog *dialog, gint response_id, gpointer user_data)
{
	if(response_id == GTK_RESPONSE_ACCEPT)
	{
		g_print("accept\n");
	}

	gtk_widget_hide(GTK_WIDGET(dialog));
}
#endif

static void
lmplayer_lyric_selection_dialog_construct(LmplayerLyricSelectionDialog *dlg)
{
	LmplayerLyricSelectionDialogPrivate *priv;
	priv = LMPLAYER_LYRIC_SELECTION_DIALOG_GET_PRIVATE(dlg);

	GtkWidget *root_vbox = GTK_DIALOG(dlg)->vbox;

	GtkWidget *vbox = gtk_vbox_new(FALSE, 6);
 
	GtkWidget *label = gtk_label_new(_("Select lyric below for download:"));
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 6);
 
	GtkWidget *hbox1 = gtk_hbox_new(FALSE, 6);
	GtkWidget *placehold = gtk_label_new(" ");
	gtk_box_pack_start(GTK_BOX(hbox1), placehold, FALSE, FALSE, 0);

	priv->container = gtk_vbox_new(FALSE, 6);
	gtk_box_pack_start(GTK_BOX(hbox1), priv->container, FALSE, FALSE, 0);
 
	gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(root_vbox), vbox, FALSE, FALSE, 6);
 
	g_signal_connect(dlg, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
}

GtkWidget *
lmplayer_lyric_selection_dialog_new()
{
	LmplayerLyricSelectionDialog *dlg;
	
	dlg = g_object_new(LMPLAYER_TYPE_LYRIC_SELECTION_DIALOG, NULL);

	gtk_dialog_add_buttons(GTK_DIALOG(dlg), 
			GTK_STOCK_OK,
			GTK_RESPONSE_ACCEPT,
			GTK_STOCK_CANCEL,
			GTK_RESPONSE_REJECT, 
			NULL);

	gtk_window_set_title(GTK_WINDOW(dlg), _("Select lyric"));

	lmplayer_lyric_selection_dialog_construct(dlg);

	return GTK_WIDGET(dlg);
}

static void
radio_button_toggled_cb(GtkToggleButton *button, LmplayerLyricSelectionDialog *dlg)
{
	LmplayerLyricSelectionDialogPrivate *priv;

	g_return_if_fail(dlg);

	priv = LMPLAYER_LYRIC_SELECTION_DIALOG_GET_PRIVATE(dlg);

	priv->index = g_list_index(priv->btn_list, button);
}

static void
create_radio_button_and_pack(TTLyric *lyric, LmplayerLyricSelectionDialog *dlg)
{
	LmplayerLyricSelectionDialogPrivate *priv;

	g_return_if_fail(lyric && dlg);

	priv = LMPLAYER_LYRIC_SELECTION_DIALOG_GET_PRIVATE(dlg);

	GtkWidget *btn = NULL;
	if(priv->first_btn)
	{
		btn = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(priv->first_btn), lyric->title);
	}
	else
	{
		priv->first_btn = gtk_radio_button_new_with_label(NULL, lyric->title);
		btn = priv->first_btn;
	}

	gtk_box_pack_start(GTK_BOX(priv->widget), btn, FALSE, FALSE, 6);
	g_signal_connect(btn, "toggled", G_CALLBACK(radio_button_toggled_cb), dlg);

	gtk_widget_show(btn);
	priv->btn_list = g_list_append(priv->btn_list, btn);
}

/*
 * Note: list is TTLyric list
 */
void
lmplayer_lyric_selection_dialog_set_list(LmplayerLyricSelectionDialog *dlg, GSList *list)
{
	LmplayerLyricSelectionDialogPrivate *priv;

	g_return_if_fail(dlg && list);

	priv = LMPLAYER_LYRIC_SELECTION_DIALOG_GET_PRIVATE(dlg);

	if(priv->btn_list)
	{
		g_list_foreach(priv->btn_list, gtk_widget_destroy, NULL);
		g_list_free(priv->btn_list);
		priv->btn_list = NULL;
		priv->first_btn = NULL;
	}

	if(GTK_IS_WIDGET(priv->widget))
	{
		gtk_widget_destroy(priv->widget);
		priv->widget = NULL;
		priv->index = 0;
	}

	priv->widget = gtk_vbox_new(FALSE, 6);
	priv->lyrics_list = list;

	g_slist_foreach(list, (GFunc)create_radio_button_and_pack, dlg);
	gtk_box_pack_start(GTK_BOX(priv->container), priv->widget, FALSE, FALSE, 6);
}

gint
lmplayer_lyric_selection_dialog_get_selected_index(LmplayerLyricSelectionDialog *dlg)
{
	LmplayerLyricSelectionDialogPrivate *priv;

	g_return_if_fail(dlg);

	priv = LMPLAYER_LYRIC_SELECTION_DIALOG_GET_PRIVATE(dlg);

	return priv->index;
}

