# initial information about the project
AC_INIT(elegance, 0.0.1, enlightenment-devel@lists.sourceforge.net)

# adding var to check the host OS
AC_CANONICAL_HOST

# check autoconf version
AC_PREREQ(2.67)
AC_CONFIG_SRCDIR(configure.ac)

# automake initialisation (mandatory)
AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

AC_C_BIGENDIAN
AC_ISC_POSIX
# check for C compiler and library compiler
AC_PROG_CC
AM_PROG_CC_STDC
# Checks for header files
AC_HEADER_STDC
AC_C_CONST
AC_C___ATTRIBUTE__
AC_FUNC_ALLOCA
# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

AC_LIBTOOL_WIN32_DLL
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

PKG_CHECK_MODULES([EFL], [eet edje emotion evas eina ecore ecore-evas ecore-x ecore-con])

PKG_CHECK_MODULES([ELEMENTARY], [elementary])

EFL_WITH_BIN([eet], [eet-eet], [eet])
EFL_WITH_BIN([edje], [edje-cc], [edje_cc])

AC_CONFIG_FILES([
Makefile
src/Makefile
src/bin/Makefile
data/Makefile
data/desktop/Makefile
data/themes/Makefile
data/tools/Makefile
])

AC_ARG_ENABLE([debug],
     [  --enable-debug          turn on debugging],
     [case "${enableval}" in
       yes) debug=true ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
      esac],
     [debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])
AC_SUBST([debug])

# distribute additional compiler and linker flags
if test "x${debug}" = "xtrue" ; then
   CFLAGS="$CFLAGS -Wall -Wextra -Werror=unused-variable -Winit-self -g -ggdb -DDEBUG $EFL_CFLAGS"
else
   CFLAGS="$CFLAGS -DDEBUG $EFL_CFLAGS"
fi

AC_OUTPUT

echo \
"-------------------------------------------------
${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Configuration Options Summary:

  * Debug mode  : '${debug}'

-------------------------------------------------"
