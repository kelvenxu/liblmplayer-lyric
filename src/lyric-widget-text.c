/* vi: set sw=4 ts=4: */
/*
 * lyric-widget-text.c
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

#include "lyric-widget-text.h"
#include "lyric-widget.h"
#include "lyric-loader.h"
#include <gtk/gtk.h>

static void lmplayer_lyric_widget_interface_init(LmplayerLyricWidgetInterface *iface);

G_DEFINE_TYPE_WITH_CODE (LmplayerLyricWidgetText, lmplayer_lyric_widget_text, GTK_TYPE_SCROLLED_WINDOW,
		G_IMPLEMENT_INTERFACE(LMPLAYER_TYPE_LYRIC_WIDGET, lmplayer_lyric_widget_interface_init))

#define LMPLAYER_LYRIC_WIDGET_TEXT_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), LMPLAYER_TYPE_LYRIC_WIDGET_TEXT, LmplayerLyricWidgetTextPrivate))

struct _LmplayerLyricWidgetTextPrivate 
{
	LmplayerLyricLoader *lyric_loader;
	GtkWidget *text_view;
	GtkTextBuffer *text_buffer;
	gint current_line;
	gint current_sec;
};

static void
lmplayer_lyric_widget_text_dispose (LmplayerLyricWidgetText *self)
{
	LmplayerLyricWidgetTextPrivate *priv;

	priv = LMPLAYER_LYRIC_WIDGET_TEXT_GET_PRIVATE(self);
}

static void
lmplayer_lyric_widget_text_finalize (LmplayerLyricWidgetText *self)
{
}

static void
lmplayer_lyric_widget_text_init (LmplayerLyricWidgetText *self)
{
	LmplayerLyricWidgetTextPrivate *priv;

	priv = LMPLAYER_LYRIC_WIDGET_TEXT_GET_PRIVATE(self);

	priv->lyric_loader = g_object_new(LMPLAYER_TYPE_LYRIC_LOADER, NULL);
	
	priv->text_view = gtk_text_view_new();
	priv->text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(priv->text_view));
	gtk_text_view_set_editable(GTK_TEXT_VIEW(priv->text_view), FALSE);

	gtk_text_buffer_create_tag(priv->text_buffer, "center", 
			"justification",GTK_JUSTIFY_CENTER, 
			NULL);
	gtk_text_buffer_create_tag(priv->text_buffer, "current", 
			"scale", PANGO_SCALE_LARGE,
			"scale-set", TRUE,
			"foreground", "blue", 
			"weight", PANGO_WEIGHT_BOLD,
			NULL);
	gtk_text_buffer_create_tag(priv->text_buffer, "normal", 
			"scale", PANGO_SCALE_SMALL,
			"scale-set", TRUE,
			"foreground", "black", 
			"weight", PANGO_WEIGHT_NORMAL,
			NULL);

	gtk_container_add(GTK_CONTAINER(self), priv->text_view);

	priv->current_line = -1;
	priv->current_sec = -1;
}

static void
lmplayer_lyric_widget_text_class_init (LmplayerLyricWidgetTextClass *self_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (self_class);

	g_type_class_add_private (self_class, sizeof (LmplayerLyricWidgetTextPrivate));
	object_class->dispose = (void (*) (GObject *object)) lmplayer_lyric_widget_text_dispose;
	object_class->finalize = (void (*) (GObject *object)) lmplayer_lyric_widget_text_finalize;
}

static void
lmplayer_lyric_widget_text_prepare(LmplayerLyricWidget *self)
{
	GList *iter;
	GtkTextIter buffer_iter;

	g_return_if_fail(self);

	LmplayerLyricWidgetTextPrivate *priv = LMPLAYER_LYRIC_WIDGET_TEXT_GET_PRIVATE(self);

	GList *lines_list = lmplayer_lyric_loader_get_lines(priv->lyric_loader);

	g_return_if_fail(lines_list);

	gtk_text_buffer_get_start_iter(priv->text_buffer, &buffer_iter);

	for(iter = lines_list; iter; iter = iter->next)
	{
		LmplayerLyricLine *line = (LmplayerLyricLine *)iter->data;
		gtk_text_buffer_insert_with_tags_by_name(priv->text_buffer, &buffer_iter, line->text, -1, "center", NULL);
		gtk_text_buffer_insert(priv->text_buffer, &buffer_iter, "\n", -1);
	}
}

static void 
lmplayer_lyric_widget_text_get_size(LmplayerLyricWidget *self, gint *width, gint *height)
{
}

static void 
lmplayer_lyric_widget_text_set_size(LmplayerLyricWidget *lyric, gint width, gint height)
{
}

static gboolean 
lmplayer_lyric_widget_text_add_file(LmplayerLyricWidget *self, const gchar *file)
{
	g_return_val_if_fail(self, FALSE);

	LmplayerLyricWidgetTextPrivate *priv = LMPLAYER_LYRIC_WIDGET_TEXT_GET_PRIVATE(self);

	gboolean ret = lmplayer_lyric_loader_add_file(priv->lyric_loader, file);

	if(ret)
	{
		lmplayer_lyric_widget_text_prepare(self);
	}

	return ret;
}

static void 
lmplayer_lyric_widget_text_set_current_second(LmplayerLyricWidget *self, gint sec)
{
	GList *iter = NULL;
	LmplayerLyricLine *line = NULL;
	gint line_no = -1;
	gboolean found = FALSE;

	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET_TEXT(self));

	LmplayerLyricWidgetTextPrivate *priv = LMPLAYER_LYRIC_WIDGET_TEXT_GET_PRIVATE(self);
	GList *lines = lmplayer_lyric_loader_get_lines(priv->lyric_loader);

	if(priv->current_sec == sec)
		return;

	g_print("sec: %d\n", sec);
	for(iter = lines; iter; iter = iter->next)
	{
		line = iter->data;
		g_print("line sec: %d\n", line->sec);
		g_print("line nth: %d\n", line->nth);
		if(line && line->sec == sec)
		{
			line_no = line->nth;
			found = TRUE;
			break;
		}
		else if(line && line->sec > sec)
		{
			found = FALSE;
			break;
		}
	}

	if(!found)
		return;
	
	GtkTextIter start;
	GtkTextIter end;
	
	if(priv->current_line >= 0)
	{
		g_print("normal it %d\n", priv->current_line);
		gtk_text_buffer_get_iter_at_line(priv->text_buffer, &start, priv->current_line);
		gtk_text_buffer_get_iter_at_line(priv->text_buffer, &end, priv->current_line + 1);
		gtk_text_buffer_apply_tag_by_name(priv->text_buffer, "normal", &start, &end);
	}

	//priv->current_line ++;
	if(line_no >= 0)
	{
		//gtk_text_buffer_get_iter_at_line(priv->text_buffer, &start, priv->current_line);
		//gtk_text_buffer_get_iter_at_line(priv->text_buffer, &end, priv->current_line + 1);
		//gtk_text_buffer_apply_tag_by_name(priv->text_buffer, "normal", &start, &end);
		g_print("high light it %d\n", line_no);
		gtk_text_buffer_get_iter_at_line(priv->text_buffer, &start, line_no);
		gtk_text_buffer_get_iter_at_line(priv->text_buffer, &end, line_no + 1);
		gtk_text_buffer_apply_tag_by_name(priv->text_buffer, "current", &start, &end);
	}

	priv->current_sec = sec;
	priv->current_line = line_no;
}

static void 
lmplayer_lyric_widget_text_set_bg_color(LmplayerLyricWidget *lyric, const GdkColor *color)
{
}
static void 
lmplayer_lyric_widget_text_set_text_color(LmplayerLyricWidget *lyric, const GdkColor *color)
{
}

static void 
lmplayer_lyric_widget_text_set_highlight_color(LmplayerLyricWidget *lyric, const GdkColor *color)
{
}

static void 
lmplayer_lyric_widget_text_set_color(LmplayerLyricWidget *lyric, const GdkColor *bg, const GdkColor *text, const GdkColor *hilight)
{
}

static void 
lmplayer_lyric_widget_interface_init(LmplayerLyricWidgetInterface *iface)
{
	iface->get_size = lmplayer_lyric_widget_text_get_size;
	iface->set_size = lmplayer_lyric_widget_text_set_size;
  iface->add_file = lmplayer_lyric_widget_text_add_file;
	iface->set_current_second = lmplayer_lyric_widget_text_set_current_second;
	iface->set_bg_color = lmplayer_lyric_widget_text_set_bg_color;
	iface->set_text_color = lmplayer_lyric_widget_text_set_text_color;
	iface->set_highlight_color = lmplayer_lyric_widget_text_set_highlight_color;
	iface->set_color = lmplayer_lyric_widget_text_set_color;
}

GtkWidget *
lmplayer_lyric_widget_text_new()
{
	return g_object_new(LMPLAYER_TYPE_LYRIC_WIDGET_TEXT, 
			"hadjustment", NULL, 
			"hscrollbar-policy", GTK_POLICY_AUTOMATIC,
			"vadjustment", NULL,
			"vscrollbar-policy", GTK_POLICY_AUTOMATIC,
			NULL);
}
