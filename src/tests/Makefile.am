noinst_PROGRAMS = blockemtest
# If the test program shouldn't run automatically (e.g. because it's 
# interactive), omit the TESTS line
TESTS = blockemtest

INCLUDES = -I$(srcdir)/..

AM_CXXFLAGS =

blockemtest_SOURCES = \
main_test.cpp \
Game1v1Test.cpp \
Game1v1Test.h

# this should be changed at some point. we should be crating a library with the base blockem classes
# and then add that library to blockemtest_LIBADD
blockemtest_LDADD = ../Board.o ../Game1v1.o ../GameTotalAllocation.o ../heuristic.o ../Piece.o ../Player.o ../Rules.o

