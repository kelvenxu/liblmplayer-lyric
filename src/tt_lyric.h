/* vi: set sw=4 ts=4: */
/*
 * tt_lyric.h
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

#ifndef __TT_LYRIC_H__
#define __TT_LYRIC_H__  1

typedef struct _LyricId
{
	int no;
	char *id;
	char *artist;
	char *title;
} LyricId;

typedef struct _LyricId TTLyric;

gchar* lmp_lyric_download( gchar* filename, gchar* artist, gchar* title); 
void lmp_lyric_download_with_thread( gchar* filename, gchar* artist, gchar* title);

gboolean tt_get_lyrics_content_and_save(unsigned int id, gchar* artist, gchar* title, const gchar* lyric_filename);
gchar* tt_get_lyrics_list(const gchar* artist, const gchar* title);
gchar* tt_get_lyrics_content(gint id, gchar* artist, gchar* title);
GSList* tt_parse_lyricslist(gchar* xml);

//void tt_lyrics_list_selection_dialog( GSList* list, const gchar* lyric_filename);
#endif /*__TT_LYRIC_H__ */
