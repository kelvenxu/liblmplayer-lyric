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

#include "lyric-widget-da.h"
#include "lyric-widget.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glib/gstdio.h>

static void lmplayer_lyric_widget_interface_init(LmplayerLyricWidgetInterface *iface);

G_DEFINE_TYPE_WITH_CODE (LmplayerLyricWidgetDa, lmplayer_lyric_widget_da, GTK_TYPE_LAYOUT,
		G_IMPLEMENT_INTERFACE(LMPLAYER_TYPE_LYRIC_WIDGET, lmplayer_lyric_widget_interface_init))

#define LMPLAYER_LYRIC_WIDGET_DA_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), LMPLAYER_TYPE_LYRIC_WIDGET_DA, LmplayerLyricWidgetDaPrivate))

typedef struct LyricLine
{
	// 在da中的位置 
	gint x;
	gint y;

	// 时间标签
	gint sec;
	gint ms;

	// 歌词文本
	gchar *text;
} LyricLine;

struct _LmplayerLyricWidgetDaPrivate 
{
	GtkWidget *alignment;
	GtkWidget *da;

	GdkPixmap *pixmap;

	// 为在da上输出字符串
	GdkGC *da_gc;
	PangoContext *pango_context;
	PangoLayout *pango_layout;

	GdkColor bg;
	GdkColor fg;
	GdkColor current;

	gint da_width;
	gint da_height;
	gboolean da_size_changed;

	gint width;
	gint height;

	// 存储时间，其中的text指向texts中存储的歌词
	GList *lines;

	// 当前指示的时间
	gint current_second;

	// 只存储歌词文本本身
	GList *texts;
	gint nlines;
	gint index;

	// 指示是否读入了歌词
	gboolean loaded;
	// 指示歌词是否改变了
	gboolean changed;

	gint inter; // FIXME: 字高 ＋ 间隙

	gchar *songname;
	gchar *lyricname;

	gchar *title;
	gchar *artist;
	gchar *album;
	gchar *author;
	gint offset;
};


static void
lmplayer_lyric_widget_da_dispose (LmplayerLyricWidgetDa *self)
{
}

static void
lmplayer_lyric_widget_da_finalize (LmplayerLyricWidgetDa *self)
{
}

static void
lmplayer_lyric_widget_da_init (LmplayerLyricWidgetDa *self)
{
	LmplayerLyricWidgetDaPrivate *priv;
	priv = LMPLAYER_LYRIC_WIDGET_DA_GET_PRIVATE(self);
	self->priv = priv;

	priv->alignment = NULL;
	priv->da = NULL;
	priv->pixmap = NULL;

	priv->da_gc = NULL;
	priv->pango_context = NULL;
	priv->pango_layout = NULL;

	priv->songname = NULL;
	priv->lyricname = NULL;

	priv->inter = 14;

	priv->bg.red = 0;
	priv->bg.green = 0;
	priv->bg.blue = 0;

	priv->fg.red = 54740;
	priv->fg.green = 54740;
	priv->fg.blue = 54740;

	priv->current.red = 65535;
	priv->current.green = 65535;
	priv->current.blue = 65535;

	priv->lines = NULL;
	priv->nlines = 0;
	priv->index = 0;

	priv->loaded = FALSE;
	priv->changed = FALSE;

	priv->current_second = -1;
	priv->da_size_changed = FALSE;
}

static void
lmplayer_lyric_widget_da_class_init (LmplayerLyricWidgetDaClass *self_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (self_class);

	g_type_class_add_private (self_class, sizeof (LmplayerLyricWidgetDaPrivate));
	object_class->dispose = (void (*) (GObject *object)) lmplayer_lyric_widget_da_dispose;
	object_class->finalize = (void (*) (GObject *object)) lmplayer_lyric_widget_da_finalize;
}

static void 
update_pixmap(LmplayerLyricWidgetDa *lyric)
{
	GList *iter;
	LyricLine *line;
	GdkGC *gc;
	PangoContext *context;
	PangoLayout *layout;
	LmplayerLyricWidgetDaPrivate *priv = lyric->priv;
	gint n;

	if(priv->loaded == FALSE)
		return; //FIXME:

	if(priv->lines == NULL)
		return;

	//gint real_width = priv->da_width;
	//gint real_height = priv->da_height;
	//gdk_drawable_get_size(priv->da->window, &real_width, &real_height);

	//if(!(priv->changed) || (real_width == priv->da_width && real_height == priv->da_height))
	if(!priv->changed && !priv->da_size_changed)
	{
		g_print("no changed\n");
		return;
	}

	if(priv->pixmap != NULL)
	{
		gdk_pixmap_unref(priv->pixmap);
	}

	n = g_list_length(priv->lines);

	priv->da_height = (n + 4) * priv->inter;

	priv->pixmap = gdk_pixmap_new(priv->da->window,
			priv->da_width, 
			priv->da_height,
			-1); 

	if(priv->pixmap == NULL)
		return;

	gc = gdk_gc_new(priv->da->window);
	gdk_gc_set_rgb_fg_color(gc, &priv->bg);
	gdk_draw_rectangle(priv->pixmap, gc, TRUE,
			0, 0,
			priv->da_width, 
			priv->da_height);

	context = gtk_widget_get_pango_context(priv->da);
	layout = pango_layout_new(context);
	pango_layout_set_width(layout, priv->da_width * PANGO_SCALE);
	pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
	gdk_gc_set_rgb_fg_color(gc, &priv->fg);

	for(iter = priv->lines; iter; iter = iter->next)
	{
		line = (LyricLine*)iter->data;
		if(line->text == NULL) 
			continue;
		pango_layout_set_text(layout, line->text, -1);
		gdk_draw_layout(priv->pixmap, gc, line->x, line->y, layout);
	}

	priv->changed = FALSE;
	gtk_widget_set_size_request(priv->da, priv->da_width, priv->da_height);
}

static gboolean
layout_expose_cb(GtkWidget *widget, GdkEventExpose *event, LmplayerLyricWidgetDa *lyric)
{
	LmplayerLyricWidgetDaPrivate *priv = lyric->priv;
	GdkGC *gc;

	gc = gdk_gc_new(GTK_LAYOUT(widget)->bin_window);
	gdk_gc_set_rgb_fg_color(gc, &priv->bg);
	gdk_draw_rectangle(GTK_LAYOUT(widget)->bin_window, gc, TRUE,
			0, 0,
			widget->allocation.width, widget->allocation.height);

	return TRUE;
}

static gboolean
da_expose_cb(GtkWidget *widget, GdkEventExpose *event, LmplayerLyricWidgetDa *lyric)
{ 
	LmplayerLyricWidgetDaPrivate *priv = lyric->priv;
	
	if(priv->da_gc == NULL)
		priv->da_gc = gdk_gc_new(widget->window);

	g_print("expose cb pixmap = %p\n", priv->pixmap);
	if(priv->pixmap == NULL)
	{
		gdk_gc_set_rgb_fg_color(priv->da_gc, &priv->bg);
		gdk_draw_rectangle(widget->window, priv->da_gc, TRUE,
				0, 0,
				widget->allocation.width, 
				widget->allocation.height);
	}
	else
	{
		gdk_draw_drawable(widget->window,
				widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
				lyric->priv->pixmap,
				0, 0,
				0, 0,
				lyric->priv->da_width,
				lyric->priv->da_height);
	}
	return TRUE;
}

static gboolean
da_configure_cb(GtkWidget *widget, GdkEventExpose *event, LmplayerLyricWidgetDa *lyric)
{
	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	g_print("da configure cbw: %d h: %d\n", allocation.width, allocation.height);
	update_pixmap(lyric);
	g_print("da configure cb done\n");
}

GtkWidget *
lmplayer_lyric_widget_da_new()
{
	LmplayerLyricWidgetDa *lyric;
	LmplayerLyricWidgetDaPrivate *priv;

	lyric = g_object_new(LMPLAYER_TYPE_LYRIC_WIDGET_DA, NULL);

	priv = lyric->priv;
	priv->alignment = gtk_alignment_new(0, 0, 1, 1);
	gtk_widget_show(priv->alignment);

	priv->da = gtk_drawing_area_new();
	gtk_widget_show(priv->da);

	//priv->da_gc = gdk_gc_new(priv->da->window);
	priv->pango_context = gtk_widget_get_pango_context(priv->da);
	priv->pango_layout = pango_layout_new(priv->pango_context);

	gtk_container_add(GTK_CONTAINER(priv->alignment), priv->da);
	gtk_container_add(GTK_CONTAINER(lyric), priv->alignment);

	gtk_widget_set_events(priv->da, GDK_EXPOSURE_MASK);

	g_signal_connect(G_OBJECT(lyric), "expose-event", G_CALLBACK(layout_expose_cb), lyric);
	g_signal_connect(G_OBJECT(priv->da), "expose-event", G_CALLBACK(da_expose_cb), lyric);
	g_signal_connect(G_OBJECT(priv->da), "configure-event", G_CALLBACK(da_configure_cb), lyric);

	return GTK_WIDGET(lyric);
}

static void 
lmplayer_lyric_widget_da_get_size(LmplayerLyricWidget *lyric, gint *width, gint *height)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET_DA(lyric));
	LmplayerLyricWidgetDaPrivate *priv = LMPLAYER_LYRIC_WIDGET_DA(lyric)->priv;

	*width = priv->da_width;
	*height = priv->da_height;
}

static void
lmplayer_lyric_widget_da_set_size(LmplayerLyricWidget *lyric, gint width, gint height)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET_DA(lyric));
	LmplayerLyricWidgetDaPrivate *priv = LMPLAYER_LYRIC_WIDGET_DA(lyric)->priv;

	if(priv->da_width != width || priv->da_height != height)
	{
		gtk_widget_set_size_request(GTK_WIDGET(lyric), width, height);
		gtk_widget_set_size_request(priv->da, width, height);

		priv->da_width = width;
		priv->da_height = height;

		priv->da_size_changed = TRUE;
		update_pixmap(LMPLAYER_LYRIC_WIDGET_DA(lyric));
	}
}

//NOTE: str: [mm:ss.ss
static void 
str2time(const gchar* str, int* sec, int* msec)
{
	//str: [mm:ss.ss
	int m = 0, s = 0, ms = 0;
	int len = strlen(str);
	gchar p[3];

	if(len >= 3)
	{
		p[0] = str[1];
		p[1] = str[2];
		p[2] = '\0';
		m = atoi(p);
	}

	if(len >= 6)
	{
		p[0] = str[4];
		p[1] = str[5];
		p[2] = '\0';
		s = atoi(p);
	}

	if(len >= 9)
	{
		p[0] = str[7];
		p[1] = str[8];
		p[2] = '\0';
		ms = atoi(p);
	}

	*sec = m * 60 + s;
	*msec = ms;
}

static gboolean
parse_lyric_line(LmplayerLyricWidgetDa *lyric, const gchar *line, gint line_no_)
{
	char** p = NULL;
	char* text = NULL;
	int sec, ms; 
	int old_sec = -1;
	gboolean flag = FALSE;

	LmplayerLyricWidgetDaPrivate *priv = lyric->priv;
	char** parray = g_strsplit(line, "]", 5);

	//find lyric text string
	for(p = parray; *p; ++p)
	{
		if((*p)[0] == '[' && (*p)[3] == ':')
			continue;
		else
		{
			text = g_strdup(*p);
			priv->texts = g_list_prepend(priv->texts, text);
			break;
		}
	}

	for(p = parray; *p; ++p)
	{ 
		if((*p)[0] == '[' && (*p)[3] == ':')
		{
			flag = TRUE;
			str2time(*p, &sec, &ms);

			if(old_sec == sec) //FIXME: 解决两个歌词时间相同的问题
				++sec;
			else
				old_sec = sec;

			LyricLine* data = g_new0(LyricLine, 1);
			data->text = text;
			data->sec = sec;
			data->ms = ms;
			//data->x = 0;
			//data->y = priv->inter * line_no;
			// 这里没有顺序
			priv->lines = g_list_prepend(priv->lines, data);
		}
	}

	g_strfreev(parray);

	return flag;
}

static void
update_xy_coordinate(LmplayerLyricWidgetDa *lyric)
{
	GList *iter;
	LyricLine *line;
	gint n;

	for(iter = lyric->priv->lines, n = 0; iter; iter = iter->next, ++n)
	{
		line = (LyricLine*)iter->data;
		line->x = 0;
		line->y = n * lyric->priv->inter;
	}
}

static gint
lyric_line_compare(LyricLine *line1, LyricLine *line2)
{
	if(line1->sec == line2->sec)
		return 0;
	if(line1->sec > line2->sec)
		return 1;

	return -1;
}

static gboolean
parse_lyric_file_without_check(LmplayerLyricWidgetDa *lyric, const gchar *filename)
{
	gchar buf[1024];
	gchar *pbuf;
	gint n;
	gboolean flag = FALSE;
	LmplayerLyricWidgetDaPrivate *priv = lyric->priv;

	FILE* fp = fopen(filename, "r");

	if(fp == NULL) return FALSE;

	n = 0;
	while(fgets(buf, 1024, fp))
	{
		g_strstrip(buf);
		if(strlen(buf) == 0 || buf[0] != '[')
			continue;

		if(g_utf8_validate(buf, -1, NULL))
			pbuf = g_strdup(buf);
		else 
			pbuf = g_convert(buf, -1, "utf-8", "gb18030", NULL, NULL, NULL);

		//title:
		if(strncmp(pbuf, "[ti:", 4) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->title = (gchar*)g_strdup(&pbuf[4]);
			flag = TRUE;
			g_free(pbuf);
			continue;
		}

		//artist:
		if(strncmp(pbuf, "[ar:", 4) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->artist = g_strdup(&pbuf[4]);
			flag = TRUE;
			g_free(pbuf);
			continue; 
		}

		//album:
		if(strncmp(pbuf, "[al:", 4) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->album = g_strdup(&pbuf[4]);
			flag = TRUE;
			g_free(pbuf);
			continue;
		}

		//author
		if(strncmp(pbuf, "[by:", 4) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->author = g_strdup(&pbuf[4]);
			flag = TRUE;
			g_free(pbuf);
			continue;
		}

		//offset:
		if(strncmp(pbuf, "[offset:", 8) == 0)
		{
			pbuf[strlen(pbuf) - 1] = '\0';
			priv->offset = atoi(&pbuf[8]);
			flag = TRUE;
			g_free(pbuf);
			continue;
		}

		flag = parse_lyric_line(lyric, pbuf, n);
		++n;
		g_free(pbuf);
	}

	fclose(fp);

	priv->lines = g_list_sort(priv->lines, (GCompareFunc)lyric_line_compare);
	update_xy_coordinate(lyric);

	return flag;
}


static gboolean
lmplayer_lyric_widget_da_add_file(LmplayerLyricWidget *lyric, const gchar *file)
{
	g_return_val_if_fail(LMPLAYER_IS_LYRIC_WIDGET_DA(lyric), FALSE);
	g_return_val_if_fail(file != NULL, FALSE);

	LmplayerLyricWidgetDaPrivate *priv = LMPLAYER_LYRIC_WIDGET_DA(lyric)->priv;

	if(g_access(file, R_OK) != 0) return FALSE;

	if(priv->lines != NULL) 
	{
		g_list_free(priv->lines);
		priv->lines = NULL;
		priv->da_height = 0;
	}

	priv->loaded = parse_lyric_file_without_check(LMPLAYER_LYRIC_WIDGET_DA(lyric), file);
	if(priv->loaded)
	{
		gtk_layout_move(GTK_LAYOUT(lyric), priv->alignment, 0, 0);
		
		gdk_window_invalidate_rect(priv->da->window, NULL, FALSE);
	}

	priv->changed = TRUE;
	priv->current_second = -1;

	update_pixmap(LMPLAYER_LYRIC_WIDGET_DA(lyric));

	return priv->loaded;
}

static void 
lmplayer_lyric_widget_da_set_current_second(LmplayerLyricWidget *lyric, gint sec)
{
	gint w, h;
	gint n;
	gboolean find = FALSE;
	GList *iter = NULL;
	LyricLine *line = NULL;

	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET_DA(lyric));
	g_return_if_fail(sec >= 0);

	LmplayerLyricWidgetDaPrivate *priv = LMPLAYER_LYRIC_WIDGET_DA(lyric)->priv;

	if(priv->current_second == sec)
		return;

	priv->current_second = sec;

	gtk_widget_get_size_request(GTK_WIDGET(lyric), &w, &h);

	for(iter = priv->lines, n = 0; iter; iter = iter->next, ++n)
	{
		line = iter->data;
		if(line && line->sec == sec)
		{
			find = TRUE;
			break;
		}
		else if(line && line->sec > sec)
		{
			find = FALSE;
			break;
		}

		find = FALSE;
	}
	
	if(find && line)
	{
		if(line->text != NULL)
		{
			gdk_gc_set_rgb_fg_color(priv->da_gc, &priv->current);
			pango_layout_set_text(priv->pango_layout, line->text, -1);
			pango_layout_set_width(priv->pango_layout, priv->da_width * PANGO_SCALE);
			pango_layout_set_alignment(priv->pango_layout, PANGO_ALIGN_CENTER);

			// 注意：这里是在da上直接绘，而不是在pixmap上
			gdk_draw_layout(priv->da->window, priv->da_gc, line->x, line->y, priv->pango_layout);
			//gdk_draw_layout(priv->pixmap, priv->da_gc, line->x, line->y, priv->pango_layout);
		}
		gtk_layout_move(GTK_LAYOUT(lyric), priv->alignment, 0, h / 2 - line->y);

		if(n > 0)
		{
			line = (LyricLine*)g_list_nth_data(priv->lines, n - 1);

			if(line->text != NULL)
			{
				gdk_gc_set_rgb_fg_color(priv->da_gc, &priv->fg);
				pango_layout_set_text(priv->pango_layout, line->text, -1);
				pango_layout_set_width(priv->pango_layout, priv->da_width * PANGO_SCALE);
				pango_layout_set_alignment(priv->pango_layout, PANGO_ALIGN_CENTER);

				// 注意：这里是在da上直接绘，而不是在pixmap上
				gdk_draw_layout(priv->da->window, priv->da_gc, line->x, line->y, priv->pango_layout);
				//gdk_draw_layout(priv->pixmap, priv->da_gc, line->x, line->y, priv->pango_layout);
			}
		}
	}
}

static void 
lmplayer_lyric_widget_da_set_bg_color(LmplayerLyricWidget *lyric, const GdkColor *color)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET_DA(lyric));
	g_return_if_fail(color != NULL);

	LmplayerLyricWidgetDaPrivate *priv = LMPLAYER_LYRIC_WIDGET_DA(lyric)->priv;
	
	priv->bg.red = color->red;
	priv->bg.green = color->green;
	priv->bg.blue = color->blue;
	priv->bg.pixel = color->pixel;

	if(GDK_IS_WINDOW(priv->da->window))
		gdk_window_invalidate_rect(priv->da->window, NULL, FALSE);
}

static void 
lmplayer_lyric_widget_da_set_text_color(LmplayerLyricWidget *lyric, const GdkColor *color)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET_DA(lyric));
	g_return_if_fail(color != NULL);

	LmplayerLyricWidgetDaPrivate *priv = LMPLAYER_LYRIC_WIDGET_DA(lyric)->priv;
	
	priv->fg.red = color->red;
	priv->fg.green = color->green;
	priv->fg.blue = color->blue;
	priv->fg.pixel = color->pixel;

	if(GDK_IS_WINDOW(priv->da->window))
		gdk_window_invalidate_rect(priv->da->window, NULL, FALSE);
}

static void 
lmplayer_lyric_widget_da_set_highlight_color(LmplayerLyricWidget *lyric, const GdkColor *color)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET_DA(lyric));
	g_return_if_fail(color != NULL);

	LmplayerLyricWidgetDaPrivate *priv = LMPLAYER_LYRIC_WIDGET_DA(lyric)->priv;
	
	priv->current.red = color->red;
	priv->current.green = color->green;
	priv->current.blue = color->blue;
	priv->current.pixel = color->pixel;

	if(GDK_IS_WINDOW(priv->da->window))
		gdk_window_invalidate_rect(priv->da->window, NULL, FALSE);
}

static void
lmplayer_lyric_widget_da_set_color(LmplayerLyricWidget *lyric, const GdkColor *bg,
									   const GdkColor *text,
									   const GdkColor *hilight)
{
	g_return_if_fail(LMPLAYER_IS_LYRIC_WIDGET_DA(lyric));

	LmplayerLyricWidgetDaPrivate *priv = LMPLAYER_LYRIC_WIDGET_DA(lyric)->priv;

	if(bg)
	{
		priv->bg.red = bg->red;
		priv->bg.green = bg->green;
		priv->bg.blue = bg->blue;
		priv->bg.pixel = bg->pixel;
	}

	if(text)
	{
		priv->fg.red = text->red;
		priv->fg.green = text->green;
		priv->fg.blue = text->blue;
		priv->fg.pixel = text->pixel;
	}

	if(hilight)
	{
		priv->current.red = hilight->red;
		priv->current.green = hilight->green;
		priv->current.blue = hilight->blue;
		priv->current.pixel = hilight->pixel;
	}

	update_pixmap(LMPLAYER_LYRIC_WIDGET_DA(lyric));
}

static void 
lmplayer_lyric_widget_interface_init(LmplayerLyricWidgetInterface *iface)
{
	iface->get_size = lmplayer_lyric_widget_da_get_size;
	iface->set_size = lmplayer_lyric_widget_da_set_size;
  iface->add_file = lmplayer_lyric_widget_da_add_file;
	iface->set_current_second = lmplayer_lyric_widget_da_set_current_second;
	iface->set_bg_color = lmplayer_lyric_widget_da_set_bg_color;
	iface->set_text_color = lmplayer_lyric_widget_da_set_text_color;
	iface->set_highlight_color = lmplayer_lyric_widget_da_set_highlight_color;
	iface->set_color = lmplayer_lyric_widget_da_set_color;
}

