# SUBDIRS = tests

bin_PROGRAMS = blockem

INCLUDES = -I$(srcdir)

AM_CXXFLAGS = $(GTKMM_CFLAGS) $(GLADEMM_CFLAGS) $(GTHREAD_CFLAGS)
AM_LDFLAGS = 

#DATADIR in windows is the current directory
# otherwise autotools "datadir" is used
if OS_WIN32    
    AM_CXXFLAGS += -DDATADIR=.
else
    AM_CXXFLAGS += -DDATADIR=\"$(datadir)/@PACKAGE@\"
endif

# if uncommented compiletime.h will be overwritten everytime the `make` command is run
#.PHONY: compiletime.h

compiletime.h:
	@echo "Saving the current date into compiletime.h"
	date '+#define COMPILETIME "%x %X"' > compiletime.h
	@echo ' '

gui_glade_obj.o: gui.glade
	@echo 'turn the xml gui file into an object file to be embedded into the final binary'
	$(LD) -r -b binary -o $@ $<
	$(OBJCOPY) --rename-section .data=.rodata,alloc,load,readonly,data,contents $@ $@
	@echo ' '

main.o: compiletime.h

blockem_DEPENDENCIES = gui_glade_obj.o

blockem_LDADD = gui_glade_obj.o $(GTKMM_LIBS) $(GLADEMM_LIBS) $(GTHREAD_LIBS)

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
bitwise.cpp \
bitwise.h \
board.cpp \
board.h \
coordinate.h \
coordinate_set.h \
game1v1.cpp \
game1v1.h \
game_total_allocation.cpp \
game_total_allocation.h \
g_blocking_queue.h \
g_blocking_queue_impl.h \
gui_exception.h \
gui_about_dialog.cpp \
gui_about_dialog.h \
gui_config_dialog.cpp \
gui_config_dialog.h \
gui_drawing_area_board.h \
gui_drawing_area_board.cpp \
gui_drawing_area_show_pieces.cpp \
gui_drawing_area_show_pieces.h   \
gui_game1v1_config.cpp \
gui_game1v1_config.h \
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

