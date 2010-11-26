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

glade_obj.o: gui/gui.glade
	@echo 'turn the xml gui file into an object file to be embedded into the final binary'
	$(LD) -r -b binary -o $@ $<
	$(OBJCOPY) --rename-section .data=.rodata,alloc,load,readonly,data,contents $@ $@
	@echo ' '

main.o: compiletime.h

blockem_DEPENDENCIES = glade_obj.o

# there is no need for @LIBICONV@:
# libiconv is used to convert between a given text encoding and the user's 
# encoding (...) for use on systems which don't have one, or whose implementation 
# cannot convert from/to Unicode
blockem_LDADD = glade_obj.o @LIBINTL@ \
    $(GTKMM_LIBS) $(GTHREAD_LIBS) $(LIBXML_LIBS)

CLEANFILES = glade_obj.o compiletime.h

# win32 specific commands
if OS_WIN32
blockem_res.o: blockem.rc
	$(WINDRES) -i blockem.rc --input-format=rc -o blockem_res.o -O coff

CLEANFILES += blockem_res.o
blockem_DEPENDENCIES += blockem_res.o
blockem_LDADD += blockem_res.o
endif


EXTRA_DIST =  \
gui/gui.glade \
blockem.rc

blockem_SOURCES = \
gui/dialog_about.cpp \
gui/dialog_about.h \
gui/dialog_newgame.cpp \
gui/dialog_newgame.h \
gui/dialog_newgame_widget.h \
gui/dialog_newgame_widget_1v1.cpp \
gui/dialog_newgame_widget_1v1.h \
gui/dialog_newgame_widget_4players.cpp \
gui/dialog_newgame_widget_4players.h \
gui/dialog_newgame_widget_challenge.cpp \
gui/dialog_newgame_widget_challenge.h \
gui/dialog_newgame_widget_totalallocation.cpp \
gui/dialog_newgame_widget_totalallocation.h \
gui/drawing_area_board.h \
gui/drawing_area_board.cpp \
gui/drawing_area_show_pieces.cpp \
gui/drawing_area_show_pieces.h   \
gui/game_1v1_config.cpp \
gui/game_1v1_config.h \
gui/game_1v1_config_dialog.cpp \
gui/game_1v1_config_dialog.h \
gui/game_1v1_widget.cpp \
gui/game_1v1_widget.h \
gui/game_4players_config.cpp \
gui/game_4players_config.h \
gui/game_4players_widget.cpp \
gui/game_4players_widget.h \
gui/game_challenge_config.cpp \
gui/game_challenge_config.h \
gui/game_challenge_widget.cpp \
gui/game_challenge_widget.h \
gui/game_total_allocation_config.cpp \
gui/game_total_allocation_config.h \
gui/game_total_allocation_widget.cpp \
gui/game_total_allocation_widget.h \
gui/glade_defs.h \
gui/gui_exception.cpp \
gui/gui_exception.h \
gui/main_window.cpp \
gui/main_window.h \
gui/main_window_worker_thread.cpp \
gui/main_window_worker_thread.h \
gui/statusbar_game.cpp \
gui/statusbar_game.h \
gui/table_edit_piece.cpp \
gui/table_edit_piece.h   \
gui/stop_watch_label.cpp \
gui/stop_watch_label.h \
impl/board_impl.h \
impl/coordinate_impl.h \
impl/g_blocking_queue_impl.h \
impl/game1v1_impl.h \
impl/piece_impl.h \
impl/player_impl.h \
bitwise.cpp \
bitwise.h \
blockem_challenge.cpp \
blockem_challenge.h \
blockem_config.cpp \
blockem_config.h \
board.cpp \
board.h \
coordinate.h \
coordinate_set.h \
game1v1.cpp \
game1v1.h \
game_4players.cpp \
game_4players.h \
game_challenge.cpp \
game_challenge.h \
game_total_allocation.cpp \
game_total_allocation.h \
g_blocking_queue.h \
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

