#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])

m4_define([MAJOR_VERSION], 0)
m4_define([MINOR_VERSION], 2)
m4_define([MICRO_VERSION], 0)

define([svnversion], esyscmd([sh -c "svnversion|tr -d '\n'"]))

AC_INIT([blockem], [MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION], [frechilla -at- gmail -dot- com])
AM_INIT_AUTOMAKE(blockem, [MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION])
AC_CONFIG_SRCDIR([src/game1v1.cpp])
AC_CONFIG_HEADERS([config.h])
AC_DEFINE(SVN_REVISION, "svnversion", [SVN Revision])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# AM_CONDITIONAL(HAVE_OBJCOPY_WEAKEN, ["$OBJCOPY" -W malloc /bin/ls /dev/null])
AC_CHECK_TOOL([OBJCOPY], [objcopy], [])
AC_CHECK_TOOL([LD], [ld], [])

# OS dependant checking. http://www.airs.com/ian/configure/configure_5.html
# This macro will arrange to define a few shell variables...
AC_CANONICAL_HOST

# win32 check from: http://git.gnome.org/browse/gedit/tree/configure.ac
AC_MSG_CHECKING([for native win32])
case "$host" in
    *-*-mingw*)
        os_win32=yes
        ;;
    *)
        os_win32=no
        ;;
esac
AC_MSG_RESULT([$os_win32])
AM_CONDITIONAL(OS_WIN32, test "$os_win32" = "yes")
if test "$os_win32" = "yes"; then
    AC_CHECK_TOOL([WINDRES], [windres], [])
fi

# Checks for libraries.
# .glade file requiers Gtk+ >= 2.16 to work
PKG_CHECK_MODULES([GTKMM], [gtkmm-2.4 >= 2.16])
# GTHREAD 2.16 hasn't been really checked. Just a guess
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0 >= 2.16])
# contained in gtkmm
# PKG_CHECK_MODULES([SIGCPP], [sigc++-2.0 >= 1.0.0])

AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)
AC_SUBST(GTHREAD_CFLAGS)
AC_SUBST(GTHREAD_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.

USER_CFLAGS=
CXXFLAGS="-Wall -O3 -g3 -DNDEBUG"

# if in win32 debug symbols won't be added to executable to decrease its size
if test "$os_win32" = "yes"; then
    CXXFLAGS="-Wall -O3 -g0 -DNDEBUG"
fi

AC_ARG_ENABLE(debug,
    AC_HELP_STRING([--enable-debug], [Enable debugging code]),
    [ if test x$enableval = xyes; then
        debug=yes
        CXXFLAGS="${USER_CFLAGS} -Wall -O0 -ggdb -DDEBUG"
    fi])

AC_ARG_ENABLE(debug-printing,
    AC_HELP_STRING([--enable-debug-printing], [Enable debugging printing to standard out]),
    [ if test x$enableval = xyes; then
        debugprinting=yes
        CXXFLAGS+=" -DDEBUG_PRINT"
    fi])

# if in win32 add these flags to remove the extra terminal window and avoid strange errors
# http://live.gnome.org/GTK%2B/Win32/Apps
if test "$os_win32" = "yes"; then
    CXXFLAGS+=" -mms-bitfields -mwindows"
fi

AC_OUTPUT([
Makefile
src/Makefile
src/tests/Makefile
doc/Makefile
])
