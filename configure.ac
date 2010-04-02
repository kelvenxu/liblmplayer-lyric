AC_PREREQ(2.52)

# 制作一个发行版：
#   micro_version += 1;
#   interface_age += 1;
# 如果任何函数被加入，设置interface_age为0，
# 如果后向兼容性被破坏，
# 设置binary_age和interface_age为0.
#
m4_define(lmplayer_version_major, 0)
m4_define(lmplayer_version_minor, 10)
m4_define(lmplayer_version_micro, 0)
m4_define(liblmplayer_lyric_interface_age, 0)

AC_INIT([liblmplayer-lyric],
        [lmplayer_version_major.lmplayer_version_minor.lmplayer_version_micro],
        [http://www.lmplayer.org])

AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE(liblmplayer-lyric, 0.10.0)

AM_MAINTAINER_MODE

AC_PROG_INTLTOOL([0.21])

LIBLMPLAYER_LYRIC_VERSION_MAJOR=lmplayer_version_major
LIBLMPLAYER_LYRIC_VERSION_MINOR=lmplayer_version_minor
LIBLMPLAYER_LYRIC_VERSION_MICRO=lmplayer_version_micro

AC_SUBST(LIBLMPLAYER_LYRIC_VERSION_MAJOR)
AC_SUBST(LIBLMPLAYER_LYRIC_VERSION_MINOR)
AC_SUBST(LIBLMPLAYER_LYRIC_VERSION_MICRO)

# If you need a modifier for the version number, 
# Normally empty, but can be used to make "fixup" releases.
m4_define([liblmplayer_lyric_extraversion], [])

m4_define([liblmplayer_lyric_current], 
					[m4_eval(100 * lmplayer_version_minor + lmplayer_version_micro - liblmplayer_lyric_interface_age)])
m4_define([liblmplayer_lyric_binary_age], [m4_eval(100 * lmplayer_version_minor + lmplayer_version_micro)])
m4_define([liblmplayer_lyric_revision], [liblmplayer_lyric_interface_age])
m4_define([liblmplayer_lyric_age], [m4_eval(liblmplayer_lyric_binary_age - liblmplayer_lyric_interface_age)])
m4_define([liblmplayer_lyric_version], 
					[lmplayer_version_major.lmplayer_version_minor.lmplayer_version_micro()liblmplayer_lyric_extraversion()])

LIBLMPLAYER_LYRIC_CURRENT=liblmplayer_lyric_current
LIBLMPLAYER_LYRIC_REVISION=liblmplayer_lyric_revision
LIBLMPLAYER_LYRIC_AGE=liblmplayer_lyric_age

AC_SUBST(LIBLMPLAYER_LYRIC_INTERFACE_AGE)
AC_SUBST(LIBLMPLAYER_LYRIC_CURRENT)
AC_SUBST(LIBLMPLAYER_LYRIC_REVISION)
AC_SUBST(LIBLMPLAYER_LYRIC_AGE)
AC_SUBST(LIBLMPLAYER_LYRIC_BINARY_VERSION)

AC_PROG_CC
AC_PROG_CPP
AM_PROG_CC_C_O
AC_ISC_POSIX
AC_HEADER_STDC
AC_PROG_INSTALL

AM_PROG_LIBTOOL

GTK_REQUIRED=2.18.0
GLIB_REQUIRED=2.22.0
GTHREAD_REQUIRED=2.22.0

PKG_CHECK_MODULES(PACKAGE, 
									gtk+-2.0 >= $GTK_REQUIRED 
									glib-2.0 >= $GLIB_REQUIRED 
									gthread-2.0 >= $GTHREAD_REQUIRED 
									libxml-2.0)

AC_SUBST(PACKAGE_CFLAGS)
AC_SUBST(PACKAGE_LIBS)

#ALL_LINGUAS="zh_CN"

#GETTEXT_PACKAGE=gtk-fx-1.0
#AC_SUBST(GETTEXT_PACKAGE)
#AM_GLIB_GNU_GETTEXT
#AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", "definition of GETTEXT_PACKAGE")

AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

AC_OUTPUT([
	Makefile
	src/Makefile
	liblmplayer-lyric.pc
	po/Makefile.in
])

