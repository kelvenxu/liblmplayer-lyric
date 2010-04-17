/* vi: set sw=4 ts=4: */
/*
 * lyric-downloader-tt.c
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

#include <stdio.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include "lyric-downloader-tt.h"
#include "tt_lyric.h"
#include "lyric-selection-dialog.h"

G_DEFINE_TYPE (LmplayerLyricDownloaderTT, lmplayer_lyric_downloader_tt, LMPLAYER_TYPE_LYRIC_DOWNLOADER)

#define LMPLAYER_LYRIC_DOWNLOADER_TT_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT, LmplayerLyricDownloaderTTPrivate))

struct _LmplayerLyricDownloaderTTPrivate 
{
	GtkWidget *dlg;

	gboolean dlg_showing; // 指示dlg是否弹出

	gchar *artist;
	gchar *title;
	gchar *filename;

	GSList *list;

	gint index; // 指示list中被选中的节点
};

static void tt_lyric_download(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist, const char *output);
static GSList * tt_lyric_get_list(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist);
static void dialog_response_cb(GtkDialog *dialog, gint response_id, LmplayerLyricDownloader *downloader);
static void tt_lyric_get_list_func(LmplayerLyricDownloaderTT *downloader);
static void tt_lyric_download_func(LmplayerLyricDownloaderTT *downloader);

static void
lmplayer_lyric_downloader_tt_dispose (LmplayerLyricDownloaderTT *self)
{
}

static void
lmplayer_lyric_downloader_tt_finalize (LmplayerLyricDownloaderTT *self)
{
}

static void
lmplayer_lyric_downloader_tt_init (LmplayerLyricDownloaderTT *self)
{
	LmplayerLyricDownloaderTTPrivate *priv;

	priv = LMPLAYER_LYRIC_DOWNLOADER_TT_GET_PRIVATE (self);
	priv->dlg = (GtkWidget *)lmplayer_lyric_selection_dialog_new();

	gtk_widget_realize(priv->dlg);

	g_signal_connect(priv->dlg, "response", G_CALLBACK(dialog_response_cb), self);

	priv->artist = NULL;
	priv->title = NULL;
	priv->filename = NULL;
	priv->dlg_showing = FALSE;
}

static void
lmplayer_lyric_downloader_tt_class_init(LmplayerLyricDownloaderTTClass *self_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (self_class);

	g_type_class_add_private (self_class, sizeof (LmplayerLyricDownloaderTTPrivate));
	object_class->dispose = (void (*) (GObject *object)) lmplayer_lyric_downloader_tt_dispose;
	object_class->finalize = (void (*) (GObject *object)) lmplayer_lyric_downloader_tt_finalize;

	LmplayerLyricDownloaderClass *parent_class = LMPLAYER_LYRIC_DOWNLOADER_CLASS(self_class);
	
	parent_class->download = tt_lyric_download;
	parent_class->get_list = tt_lyric_get_list;
}

static void
tt_lyric_download(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist, const char *filename)
{
	LmplayerLyricDownloaderTTPrivate *priv;

	priv = LMPLAYER_LYRIC_DOWNLOADER_TT_GET_PRIVATE(downloader);

	if(priv->title)
	{
		g_free(priv->title);
	}

	priv->title = g_strdup(title);

	if(priv->artist)
	{
		g_free(priv->artist);
	}

	priv->artist = g_strdup(artist);

	if(priv->filename)
	{
		g_free(priv->filename);
	}

	priv->filename = g_strdup(filename);

	g_thread_create((GThreadFunc)tt_lyric_get_list_func, downloader, FALSE, NULL);
}

static GSList *
tt_lyric_get_list(LmplayerLyricDownloaderTT *downloader, const char *title, const char *artist)
{
}

static void
tt_lyric_download_func(LmplayerLyricDownloaderTT *downloader)
{
	gboolean ret;
	LmplayerLyricDownloaderTTPrivate *priv;

	priv = LMPLAYER_LYRIC_DOWNLOADER_TT_GET_PRIVATE(downloader);

	TTLyric *lyric = (TTLyric *)g_slist_nth_data(priv->list, priv->index);

	if(lyric)
	{
		ret = tt_get_lyrics_content_and_save(atoi(lyric->id), 
																				lyric->artist, 
																				lyric->title, 
																				priv->filename);
		if(ret)
		{
			g_signal_emit_by_name(downloader, "download_finished", NULL);
		}
	}
}

static void
tt_lyric_get_list_func(LmplayerLyricDownloaderTT *downloader)
{
	LmplayerLyricDownloaderTTPrivate *priv;

	priv = LMPLAYER_LYRIC_DOWNLOADER_TT_GET_PRIVATE(downloader);

	gchar *xml = tt_get_lyrics_list(priv->artist, priv->title);
	if(!xml)
	{
		return;
	}

	priv->list = tt_parse_lyricslist(xml);
	if(!priv->list)
	{
		return;
	}

	// 多于一个时，要选择
	if(g_slist_length(priv->list) > 1)
	{
		lmplayer_lyric_selection_dialog_set_list(LMPLAYER_LYRIC_SELECTION_DIALOG(priv->dlg), priv->list);
		gtk_widget_show_all(priv->dlg);
		priv->dlg_showing = TRUE;
		return;
	}
	else
	{
		if(priv->dlg_showing)
		{
			gtk_widget_hide(priv->dlg);
			priv->dlg_showing = FALSE;
		}

		priv->index = 0;
		tt_lyric_download_func(downloader);
	}
}

static void
dialog_response_cb(GtkDialog *dialog, gint response_id, LmplayerLyricDownloader *downloader)
{
	LmplayerLyricDownloaderTTPrivate *priv;

	priv = LMPLAYER_LYRIC_DOWNLOADER_TT_GET_PRIVATE(downloader);

	if(response_id == GTK_RESPONSE_ACCEPT)
	{
		priv->index = lmplayer_lyric_selection_dialog_get_selected_index(LMPLAYER_LYRIC_SELECTION_DIALOG(dialog));
		g_thread_create((GThreadFunc)tt_lyric_download_func, downloader, FALSE, NULL);
	}

	priv->dlg_showing = FALSE;
	gtk_widget_hide(GTK_WIDGET(dialog));
}

