# SUBDIRS = tests

bin_PROGRAMS = blockem

# gettext.h used to be in top/lib
#INCLUDES = -I$(top_srcdir)/lib

# DATADIR in the windows OS is set to the current directory
if OS_WIN32    
    BLOCKEM_DATADIR ?= .
    # This is not used (it is hardcoded in main.cpp) but it's
    # set to the same value
    BLOCKEM_LOCALEDIR ?= ./share/locale
endif

# DATADIR is set by default using the autotools "datadir" variable
# ?= sets the var only if it wasn't set before
BLOCKEM_DATADIR ?= $(datadir)
BLOCKEM_LOCALEDIR ?= $(localedir)

AM_CXXFLAGS  = $(GTKMM_CFLAGS) $(GTHREAD_CFLAGS) $(LIBXML_CFLAGS)
AM_CXXFLAGS += -DDATADIR=\"$(BLOCKEM_DATADIR)\" 
AM_CXXFLAGS += -DLOCALEDIR=\"$(BLOCKEM_LOCALEDIR)\"
AM_LDFLAGS = 

# if uncommented compiletime.h will be overwritten everytime the `make` command is run
#.PHONY: compiletime.h

compiletime.h:
	@echo "Saving current date into compiletime.h"
	date '+#define COMPILETIME "%x %X"' > compiletime.h
	@echo ' '

gui_glade_obj.o: gui.glade
	@echo 'turn the xml gui file into an object file to be embedded into the final binary'
	$(LD) -r -b binary -o $@ $<
	$(OBJCOPY) --rename-section .data=.rodata,alloc,load,readonly,data,contents $@ $@
	@echo ' '

main.o: compiletime.h

blockem_DEPENDENCIES = gui_glade_obj.o

# there is no need for @LIBICONV@:
# libiconv is used to convert between a given text encoding and the user's 
# encoding (...) for use on systems which don't have one, or whose implementation 
# cannot convert from/to Unicode
blockem_LDADD = gui_glade_obj.o @LIBINTL@ \
    $(GTKMM_LIBS) $(GTHREAD_LIBS) $(LIBXML_LIBS)

CLEANFILES = gui_glade_obj.o compiletime.h

# win32 specific commands
if OS_WIN32
blockem_res.o: blockem.rc
	$(WINDRES) -i blockem.rc --input-format=rc -o blockem_res.o -O coff

CLEANFILES += blockem_res.o
blockem_DEPENDENCIES += blockem_res.o
blockem_LDADD += blockem_res.o
endif


EXTRA_DIST = \
gui.glade    \
blockem.rc

blockem_SOURCES = \
impl/board_impl.h \
impl/g_blocking_queue_impl.h \
impl/game1v1_impl.h \
impl/piece_impl.h \
impl/player_impl.h \
bitwise.cpp \
bitwise.h \
blockem_config.cpp \
blockem_config.h \
board.cpp \
board.h \
coordinate.h \
coordinate_set.h \
game1v1.cpp \
game1v1.h \
game_total_allocation.cpp \
game_total_allocation.h \
g_blocking_queue.h \
gui_exception.cpp \
gui_exception.h \
gui_about_dialog.cpp \
gui_about_dialog.h \
gui_drawing_area_board.h \
gui_drawing_area_board.cpp \
gui_drawing_area_show_pieces.cpp \
gui_drawing_area_show_pieces.h   \
gui_game1v1_config.cpp \
gui_game1v1_config.h \
gui_game1v1_config_dialog.cpp \
gui_game1v1_config_dialog.h \
gui_glade_defs.h \
gui_main_window.cpp \
gui_main_window.h \
gui_main_window_worker_thread.cpp \
gui_main_window_worker_thread.h \
gui_table_edit_piece.cpp \
gui_table_edit_piece.h   \
gui_stop_watch_label.cpp \
gui_stop_watch_label.h \
heuristic.cpp \
heuristic.h \
piece.cpp \
piece.h \
player.cpp \
player.h \
rules.cpp \
rules.h \
singleton.h \
main.cpp

