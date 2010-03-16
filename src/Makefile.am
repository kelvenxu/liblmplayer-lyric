noinst_PROGRAMS = lmplayer-lyric-test
lib_LTLIBRARIES = liblmplayer-lyric.la

INCLUDES = -I$(top_srcdir) $(PACKAGE_CFLAGS)

headers = \
	lyric-downloader.h\
	http_error_codes.h\
	http_fetcher.h\
	lmp_debug.h\
	lyric-downloader.h\
	lyric-widget.h\
	lyric-selection-dialog.h\
	tt_codefunc.h\
	tt_lyric.h

sources = \
	http_error_codes.c\
	http_fetcher.c\
	lmp_debug.c\
	lyric-downloader.c\
	lyric-widget.c\
	lyric-selection-dialog.c\
	tt_codefunc.c\
	tt_lyric.c

headers += lyric-downloader-tt.h
sources += lyric-downloader-tt.c

liblmplayer_lyric_la_SOURCES = $(sources) $(headers)
liblmplayer_lyric_la_LIBADD = $(PACKAGE_LIBS)
#liblmplayer_lyric_la_CPPFLAGS = $(CPPFLAGS) $(PACKAGE_CFLAGS) $(CFLAGS)
liblmplayer_lyric_la_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)

lmplayer_lyric_test_SOURCES = main.c $(sources) $(headers)
lmplayer_lyric_test_LDADD = $(PACKAGE_LIBS) $(top_builddir)/src/liblmplayer-lyric.la
#lmplayer_lyric_test_CPPFLAGS = -I$(top_srcdir) $(CPPFLAGS) $(PACKAGE_CFLAGS)
lmplayer_lyric_test_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)


