# SUBDIRS = tests

bin_PROGRAMS = blockem

INCLUDES = -I$(srcdir)

AM_CXXFLAGS = $(GTKMM_CFLAGS) $(GLADEMM_CFLAGS)

gui_glade_obj.o: gui.glade
	@echo 'turn the xml gui file into an object file to be embedded into the final binary'
	ld -r -b binary -o $@ $<
	objcopy --rename-section .data=.rodata,alloc,load,readonly,data,contents $@ $@
	@echo ' '

CLEANFILES = gui_glade_obj.o

blockem_DEPENDENCIES = gui_glade_obj.o

blockem_LDADD = gui_glade_obj.o $(GTKMM_LIBS) $(GLADEMM_LIBS)

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

