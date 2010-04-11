/* vi: set sw=4 ts=4: */
/*
 * test-downloader.c
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
#include <glib/gi18n.h>
#include "lyric-downloader-tt.h"
#include "lyric-downloader.h"
#include "lyric-selection-dialog.h"

struct MusicList
{
	char *title;
	char *artist;
} music_list[] = {
#if 0
	{"不只有缘", 	"张学友"}, 	
	{"幸福糖", 	"王澜霏"}, 	
	{"把我的爱放在你手...", 	"林耿贤"}, 	
	{"一直看见天使", 	"容祖儿"}, 	
	{"恋上你背面", 	"张学友"}, 	
	{"恋爱涟漪", 	"林一峰"}, 	
	{"无缘", 	"范逸臣"}, 	
#endif
	{"生理时钟", 	"罗志祥"}, 	
	{"热辣辣", 	"张学友"}, 	
	{"新鲜爱", 	"周楚霏"},
	{"花甲", 	"后弦"}, 	
	{"爱,复活", 	"李小璐"}, 	
	{"你还欠我一个拥抱", 	"后弦"}, 	
	{"天亮后分手", 	"尚雯婕"}, 	
	{"谜", 	"陈坤"}, 	
	{"罗生门", 	"罗志祥"}, 	
	{"easy", 	"彭坦"}, 	
	{"幽兰操", 	"王菲"}, 	
	{"最初", 	"赵传"}, 	
	{"如梦令", 	"蔡琴"},
	{"背对背拥抱", 	"林俊杰"}, 	
	{"心电心", 	"王心凌"}, 	
	{"大家一起喜羊羊", 	"周笔畅"}, 	
	{"无尽透明的思念", 	"庾澄庆"}, 	
	{"娃娃脸", 	"后弦"}, 	
	{"新的家", 	"曾轶可"}, 	
	{"哥哥", 	"常石磊"}, 	
	{"第几个100天", 	"林俊杰"}, 	
	{"答案", 	"沙宝亮"}, 	
	{"故乡山川", 	"李健"},
	{0, 0}
};

static gboolean
timer_cb(LmplayerLyricDownloader *lyric_downloader)
{
	static int i = 0;
	if(!music_list[i].title)
		return FALSE;

	g_print("start to donwload: %d\n", i);
	gchar *filename = g_strdup_printf("/tmp/%d.txt", i);
	lmplayer_lyric_downloader_download(lyric_downloader, music_list[i].title, music_list[i].artist, filename);

	g_free(filename);
	++i;
	return TRUE;
}

download_finished_cb(LmplayerLyricDownloader *downloader, gpointer user_data)
{
	g_print("download finished\n");
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_thread_init(NULL);

	LmplayerLyricDownloader *lyric_downloader = g_object_new(LMPLAYER_TYPE_LYRIC_DOWNLOADER_TT, NULL);

	g_signal_connect(lyric_downloader, "download_finished", G_CALLBACK(download_finished_cb), NULL);
	g_timeout_add_seconds(5, (GSourceFunc)timer_cb, lyric_downloader);

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
};
