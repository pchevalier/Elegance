AC_INIT(elegance, 0.0.1, enlightenment-devel@lists.sourceforge.net)
AC_PREREQ(2.52)
AC_CONFIG_SRCDIR(configure.ac)

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AM_CONFIG_HEADER(config.h)

AC_C_BIGENDIAN
AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST
AC_C___ATTRIBUTE__
AC_FUNC_ALLOCA

AC_LIBTOOL_WIN32_DLL
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

PKG_CHECK_MODULES([ELEMENTARY], [
   elementary
])

EFL_WITH_BIN([eet], [eet-eet], [eet])
EFL_WITH_BIN([edje], [edje-cc], [edje_cc])

AC_OUTPUT([
Makefile
src/Makefile
src/bin/Makefile
data/Makefile
data/desktop/Makefile
data/themes/Makefile
data/tools/Makefile
])

