#bin_PROGRAMS = lmplayer-lyric-test
lib_LTLIBRARIES = liblmplayer-lyric.la

headers = \
	lyric-downloader.h

sources = \
	lyric-downloader.c

liblmplayer_lyric_la_SOURCES = $(sources) $(headers)
liblmplayer_lyric_la_LIBADD = $(PACKAGE_LIBS)
liblmplayer_lyric_la_CPPFLAGS = -I$(top_builddir) $(PACKAGE_CFLAGS)

