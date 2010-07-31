AUTOMAKE_OPTIONS = foreign
# SUBDIRS = src doc examples man scripts
# add intl if AM_GNU_GETTEXT is used in configure.ac
SUBDIRS = intl po src src/tests doc

# other stuff to be distributed without compiling
EXTRA_DIST = \
ABOUT-NLS \
config.rpath \
gettext.h \
m4/ChangeLog \
pixmaps/blockemicon.ico    \
pixmaps/blockemlogo128.png \
pixmaps/blockemlogo16.png  \
pixmaps/blockemlogo48.png  \
pixmaps/blockemlogo.png

# Note pixmapsdir contains @PACKAGE@. This must be taken into account
# by the binary sources (see gui_glade_defs.h)
pixmapsdir = $(datadir)/@PACKAGE@/pixmaps

# note that 'pixmaps/' must be added in front because it is the directory
# where these files are stored (relative path from the root of the package)
pixmaps_DATA = \
pixmaps/blockemicon.ico    \
pixmaps/blockemlogo128.png \
pixmaps/blockemlogo16.png  \
pixmaps/blockemlogo48.png  \
pixmaps/blockemlogo.png


ACLOCAL_AMFLAGS = -I m4
