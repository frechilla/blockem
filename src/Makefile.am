# SUBDIRS = tests

bin_PROGRAMS = blockem

INCLUDES = -I$(srcdir)

AM_CXXFLAGS = $(GTKMM_CFLAGS) $(GLADEMM_CFLAGS) $(GTHREAD_CFLAGS) -DDATADIR=\"$(datadir)/@PACKAGE@\"
AM_LDFLAGS = 

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
Board.cpp \
Board.h \
Coordinate.h \
CoordinateSet.h \
Game1v1.cpp \
Game1v1.h \
GameTotalAllocation.cpp \
GameTotalAllocation.h \
GUIException.h \
gui_drawing_area_show_pieces.cpp \
gui_drawing_area_show_pieces.h   \
Heuristic.cpp \
Heuristic.h \
Piece.cpp \
Piece.h \
Player.cpp \
Player.h \
Rules.cpp \
Rules.h \
AboutDialog.cpp \
AboutDialog.h \
gui_glade.h \
main.cpp \
MainWindow.cpp \
MainWindow.h \
MainWindowWorkerThread.cpp \
MainWindowWorkerThread.h 

