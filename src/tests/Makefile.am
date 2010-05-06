noinst_PROGRAMS = blockemtest
# If the test program shouldn't run automatically (e.g. because it's 
# interactive), omit the TESTS line
TESTS = blockemtest

INCLUDES = -I$(srcdir)/..

AM_CXXFLAGS =

blockemtest_SOURCES = \
main_test.cpp \
game1v1_test.cpp \
game1v1_test.h

# this should be changed at some point. we should be crating a library with the base blockem classes
# and then add that library to blockemtest_LIBADD
blockemtest_LDADD = \
../board.o \
../game1v1.o \
../game_total_allocation.o \
../heuristic.o \
../piece.o \
../player.o \
../rules.o \
../bitwise.o

