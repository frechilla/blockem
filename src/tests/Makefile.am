noinst_PROGRAMS = blockemtest
# If the test program shouldn't run automatically (e.g. because it's 
# interactive), omit the TESTS line
TESTS = blockemtest

INCLUDES = -I$(srcdir)/..

AM_CXXFLAGS = $(GTHREAD_CFLAGS)
AM_LDFLAGS = 

blockemtest_SOURCES = \
bitwise_test.cpp \
bitwise_test.h \
board_test.cpp \
board_test.h \
game1v1_test.cpp \
game1v1_test.h \
g_blocking_queue_test.cpp \
g_blocking_queue_test.h \
main_test.cpp \
piece_test.cpp \
piece_test.h \
player_test.cpp \
player_test.h \
rules_test.cpp \
rules_test.h

# this should be changed at some point. we should be crating a library with the base blockem classes
# and then add that library to blockemtest_LIBADD
BLOCKEM_OBJS = \
../board.o \
../game1v1.o \
../game_total_allocation.o \
../heuristic.o \
../piece.o \
../player.o \
../rules.o \
../bitwise.o

blockemtest_LDADD = \
@LIBINTL@ \
$(GTHREAD_LIBS) \
$(BLOCKEM_OBJS)
