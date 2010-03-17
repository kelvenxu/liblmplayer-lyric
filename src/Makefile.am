noinst_PROGRAMS = test-downloader test-dialog test-widget
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
liblmplayer_lyric_la_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)

test_downloader_SOURCES = test-downloader.c
test_downloader_LDADD = $(PACKAGE_LIBS) $(top_builddir)/src/liblmplayer-lyric.la
test_downloader_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)

test_dialog_SOURCES = test-dialog.c
test_dialog_LDADD = $(PACKAGE_LIBS) $(top_builddir)/src/liblmplayer-lyric.la
test_dialog_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)

test_widget_SOURCES = test-widget.c
test_widget_LDADD = $(PACKAGE_LIBS) $(top_builddir)/src/liblmplayer-lyric.la
test_widget_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)


