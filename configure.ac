#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])

m4_define([MAJOR_VERSION], 0)
m4_define([MINOR_VERSION], 3)
m4_define([MICRO_VERSION], 3)
m4_define([BLOCKEM_PACKAGE_NAME], [blockem])

# replace %H below with %h to get the short version of the SSH hash
define([gitcommit], esyscmd([sh -c "git show -s --pretty=\"format:%H\""]))

AC_INIT(
    [BLOCKEM_PACKAGE_NAME], 
    [MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION], 
    [frechilla -at- gmail -dot- com],
    [BLOCKEM_PACKAGE_NAME])
AM_INIT_AUTOMAKE([BLOCKEM_PACKAGE_NAME], [MAJOR_VERSION.MINOR_VERSION.MICRO_VERSION])
AC_CONFIG_SRCDIR([src/game1v1.cpp])
AC_CONFIG_HEADERS([config.h])
AC_DEFINE(GIT_COMMIT, "gitcommit", [git commit SSH])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# http://www.gnu.org/software/gettext/manual/gettext.html#configure_002eac
# you need to add an explicit invocation of ‘AC_GNU_SOURCE’ in the 
# configure.ac file - after ‘AC_PROG_CC’ but before ‘AM_GNU_GETTEXT’, 
# most likely very close to the ‘AC_PROG_CC’ invocation. This is necessary 
# because of ordering restrictions imposed by GNU autoconf
AC_GNU_SOURCE

# AM_CONDITIONAL(HAVE_OBJCOPY_WEAKEN, ["$OBJCOPY" -W malloc /bin/ls /dev/null])
AC_CHECK_TOOL([OBJCOPY], [objcopy], [])
AC_CHECK_TOOL([LD], [ld], [])

# i18n. See http://www.gnu.org/software/gettext/manual/gettext.html#Maintainers
GETTEXT_PACKAGE=BLOCKEM_PACKAGE_NAME
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [gettext package])
AC_SUBST(GETTEXT_PACKAGE)
#AM_GLIB_GNU_GETTEXT

# http://www.gnu.org/software/hello/manual/gettext/AM_005fGNU_005fGETTEXT.html
# This call is purposely simple, even if it generates a lot of configure time
# checking and actions. 
# AM_GNU_GETTEXT([intlsymbol], [needsymbol], [intldir])
# intlsymbol can be ‘external’ or ‘no-libtool’. The default (if it is not 
# specified or empty) is ‘no-libtool’. intlsymbol should be ‘external’ for 
# packages with no intl/ directory. For packages with an intl/ directory, you 
# can either use an intlsymbol equal to ‘no-libtool’, or you can use ‘external’
# and override by using the macro AM_GNU_GETTEXT_INTL_SUBDIR elsewhere. The two
# ways to specify the existence of an intl/ directory are equivalent. At build
# time, a static library $(top_builddir)/intl/libintl.a will then be created
AM_GNU_GETTEXT
# Please use AM_GNU_GETTEXT([external]) in order to cause autoconfiguration
# to look for an external libintl or in case you have suppressed the intl/ 
# subdirectory by calling gettextize without '--intl' option
# src/Makefile.am should be using LTLIBINTL instead of LIBINTL if this option
# is the chosen one
#AM_GNU_GETTEXT([external])



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
# xml parsing needs libxml. version dependency:
# $ pkg-config --modversion libxml-2.0
# 2.6.26
PKG_CHECK_MODULES([LIBXML], [libxml-2.0 >= 2.6.26])
# if only glib is needed (it is contained in gtkmm). 
# 2.16 Hasn't been really checked. Just a guess
# PKG_CHECK_MODULES([GLIB], [glib-2.0 >= 2.16])
# contained in gtkmm
# PKG_CHECK_MODULES([SIGCPP], [sigc++-2.0 >= 1.0.0])

AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)
AC_SUBST(GTHREAD_CFLAGS)
AC_SUBST(GTHREAD_LIBS)
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

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
LDFLAGS=
#CXXFLAGS="${USER_CFLAGS} -Wall -O3 -g3 -DNDEBUG"
# no debug symbols by default (decreases final binary size)
CXXFLAGS="${USER_CFLAGS} -Wall -O3 -g0 -DNDEBUG"

# back in the days debug symbols were not added to the final executable 
# only in win32 platforms
#if test "$os_win32" = "yes"; then
#    CXXFLAGS="-Wall -O3 -g0 -DNDEBUG"
#fi

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

# from: http://www.cs.utah.edu/dept/old/texinfo/as/gprof.html
# In addition, you would probably want to specify the profiling C library, 
# `/usr/lib/libc_p.a', by writing `-lc_p' instead of the usual `-lc'. 
# This is not absolutely necessary, but doing this gives you number-of-calls 
# information for standard library functions such as read and open
AC_ARG_ENABLE(gprof,
    AC_HELP_STRING([--enable-gprof], [Enable code profiling. You might get better results enabling debugging code too]),
    [ if test x$enableval = xyes; then        
        # check if debug info should be enabled in the binary. This
        # happens when DEBUG hasn't been enabled by the user
        if test "$debug" != "yes"; then
            CXXFLAGS="${USER_CFLAGS} -Wall -O3 -g3 -DNDEBUG"
            if test "$debugprinting" = "yes"; then
                CXXFLAGS+=" -DDEBUG_PRINT"
            fi
        fi
        
        debugprof=yes
        CXXFLAGS+=" -pg"
        LDFLAGS+=" -lc"
    fi])

# prevent gcov and gprof from being activated at the same time
AC_ARG_ENABLE(gcov,
    AC_HELP_STRING([--enable-gcov], [Enable code coverage support. You'll get better code coverage results enabling debugging code too. gprof and gcov can't be activated at the same time]),
    [ if test x$enableval = xyes; then
        if test "$debugprof" = "yes"; then
            AC_MSG_ERROR([Invalid options: gprof and gcov can't be activated at the same time. Use --help to see good help stuff])
        else
            # check if debug info should be enabled in the binary. This
            # happens when DEBUG hasn't been enabled by the user
            if test "$debug" != "yes"; then
                CXXFLAGS="${USER_CFLAGS} -Wall -O3 -g3 -DNDEBUG"
                if test "$debugprinting" = "yes"; then
                    CXXFLAGS+=" -DDEBUG_PRINT"
                fi
            fi
            
            debugcov=yes
            CXXFLAGS+=" -ftest-coverage -fprofile-arcs"
            LDFLAGS=" -lgcov"
        fi
    fi])

# if in win32 add these flags to remove the extra terminal window and avoid strange errors
# http://live.gnome.org/GTK%2B/Win32/Apps
if test "$os_win32" = "yes"; then
    CXXFLAGS+=" -mms-bitfields -mwindows"
fi
 
AC_OUTPUT([ 
intl/Makefile 
po/Makefile.in
Makefile
src/Makefile
src/tests/Makefile
doc/Makefile
pixmaps/Makefile
challenges/Makefile
])
