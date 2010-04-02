
noinst_PROGRAMS = test-downloader test-dialog test-widget

lib_LTLIBRARIES = liblmplayer-lyric.la

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

liblmplayer_lyricincdir = $(pkgincludedir)
liblmplayer_lyricinc_HEADERS = $(headers)

liblmplayer_lyric_la_SOURCES = $(sources)
liblmplayer_lyric_la_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)
liblmplayer_lyric_la_LDFLAGS = \
	-version-info $(LIBLMPLAYER_LYRIC_CURRENT):$(LIBLMPLAYER_LYRIC_REVISION):$(LIBLMPLAYER_LYRIC_AGE)\
	-no-undefined\
	-export-symbols-regex "^[^_].*"
liblmplayer_lyric_la_LIBADD = $(PACKAGE_LIBS)

test_downloader_SOURCES = test-downloader.c
test_downloader_LDADD = $(PACKAGE_LIBS) $(top_builddir)/src/liblmplayer-lyric.la
test_downloader_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)

test_dialog_SOURCES = test-dialog.c
test_dialog_LDADD = $(PACKAGE_LIBS) $(top_builddir)/src/liblmplayer-lyric.la
test_dialog_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)

test_widget_SOURCES = test-widget.c
test_widget_LDADD = $(PACKAGE_LIBS) $(top_builddir)/src/liblmplayer-lyric.la
test_widget_CFLAGS = -I$(top_srcdir) $(CFLAGS) $(PACKAGE_CFLAGS)

EXTRA_DIST = \
	test-widget.c \
	test-downloader.c \
	test-dialog.c

