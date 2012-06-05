AC_INIT(elegance, 0.0.1, enlightenment-devel@lists.sourceforge.net)
AM_INIT_AUTOMAKE([1.6 dist-bzip2])
AC_PROG_LIBTOOL
AC_PROG_CC

PKG_PROG_PKG_CONFIG

AC_CONFIG_FILES([
Makefile
src/Makefile
src/bin/Makefile
data/Makefile
data/desktop/Makefile
data/themes/Makefile
data/tools/Makefile
])

AC_OUTPUT

PKG_CHECK_MODULES([EFL], [eet edje emotion evas eina ecore ecore-evas ecore-x ecore-con])
PKG_CHECK_MODULES([ELEMENTARY], [elementary])

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
   CFLAGS="$CFLAGS $EFL_CFLAGS"
fi

AC_OUTPUT

echo \
"-------------------------------------------------
${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Configuration Options Summary:

  * Debug mode  : '${debug}'

-------------------------------------------------"
