#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([blockem], [0.1], [frechilla@gmail.com])
AM_INIT_AUTOMAKE(blockem, 0.1)
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AC_CHECK_TOOL([OBJCOPY], [objcopy], [])
# AM_CONDITIONAL(HAVE_OBJCOPY_WEAKEN, ["$OBJCOPY" -W malloc /bin/ls /dev/null])

AC_CHECK_TOOL([LD], [ld], [])


# Checks for libraries.
PKG_CHECK_MODULES([GTKMM], [gtkmm-2.4 >= 1.0.0])
PKG_CHECK_MODULES([GLADEMM], [libglademm-2.4 >= 1.0.0])

AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)
AC_SUBST(GLADEMM_CFLAGS)
AC_SUBST(GLADEMM_LIBS)

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

# Checks for library functions.

AC_OUTPUT([
Makefile
src/Makefile
])
