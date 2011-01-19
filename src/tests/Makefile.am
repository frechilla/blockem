noinst_PROGRAMS = blockemtest
# If the test program shouldn't run automatically (e.g. because it's 
# interactive), omit the TESTS line
TESTS = blockemtest

INCLUDES = -I$(srcdir)/..

# blockem_challenge_test needs libxml 
AM_CXXFLAGS = $(GTKMM_CFLAGS) $(GTHREAD_CFLAGS) $(LIBXML_CFLAGS)
AM_LDFLAGS = 

# files to run tests on challenges
EXTRA_DIST = \
challengetest/challenge_opponent_taken_row_invalid.xml \
challengetest/challenge_board_nrows_invalid.xml \
challengetest/challenge_challenger_starting_row_no.xml \
challengetest/challenge_challenger_taken_col_toobig.xml \
challengetest/challenge_challenger_piece_available_invalid_name_bogus.xml \
challengetest/challenge_opponent_taken_duplicated_bogus.xml \
challengetest/challenge_board_no_ncolumns.xml \
challengetest/challenge_opponent_taken_col_no.xml \
challengetest/emptyfile.xml \
challengetest/challenge_challenger_taken_col_invalid.xml \
challengetest/challenge_board_nrows_negative.xml \
challengetest/challenge_root_has_empty_name_property.xml \
challengetest/challenge_challenger_piece_available_invalid.xml \
challengetest/challenge_challenger_starting_col_invalid.xml \
challengetest/challenge_challenger_starting_coord_taken_challenger.xml \
challengetest/challenge_challenger_taken_row_no.xml \
challengetest/challenge_challenger_starting_row_invalid.xml \
challengetest/challenge_root_bogus.xml \
challengetest/challenge_challenger_starting_row_negative.xml \
challengetest/challenge_challenger_taken_row_negative.xml \
challengetest/challenge_board_ncolumns_negative.xml \
challengetest/challenge_syntax_bogus.xml \
challengetest/challenge_opponent_taken_col_negative.xml \
challengetest/challenge_challenger_no.xml \
challengetest/challenge_challenger_starting_col_toobig.xml \
challengetest/challenge_challenger_taken_row_invalid.xml \
challengetest/challenge_opponent_taken_row_no.xml \
challengetest/challenge_opponent_taken_row_toobig.xml \
challengetest/challenge_opponent_taken_row_negative.xml \
challengetest/challenge_root_has_no_name_property.xml \
challengetest/challenge_challenger_taken_col_negative.xml \
challengetest/challenge_opponent_no.xml \
challengetest/challenge_taken_duplicated_opponent_challenger.xml \
challengetest/challenge_board_no_nrows.xml \
challengetest/challenge_valid1.xml \
challengetest/challenge_challenger_starting_col_negative.xml \
challengetest/challenge_challenger_taken_row_toobig.xml \
challengetest/challenge_valid2.xml \
challengetest/challenge_challenger_taken_col_no.xml \
challengetest/challenge_challenger_starting_col_row_no.xml \
challengetest/challenge_board_ncolumns_invalid.xml \
challengetest/challenge_opponent_taken_col_invalid.xml \
challengetest/challenge_challenger_piece_available_no.xml \
challengetest/challenge_challenger_starting_coord_taken_opponent.xml \
challengetest/challenge_challenger_starting_row_toobig.xml \
challengetest/challenge_opponent_taken_col_toobig.xml \
challengetest/challenge_board_no.xml \
challengetest/challenge_challenger_starting_col_no.xml \
challengetest/challenge_challenger_taken_duplicated_bogus.xml

blockemtest_SOURCES = \
bitwise_test.cpp \
bitwise_test.h \
blockem_challenge_test.cpp \
blockem_challenge_test.h \
board_test.cpp \
board_test.h \
game1v1_test.cpp \
game1v1_test.h \
game_total_allocation_test.cpp \
game_total_allocation_test.h \
g_blocking_queue_test.cpp \
g_blocking_queue_test.h \
main_test.cpp \
piece_test.cpp \
piece_test.h \
player_test.cpp \
player_test.h \
rules_test.cpp \
rules_test.h

# this should be changed at some point. we should be creating a library with the base blockem classes
# and then add that library to blockemtest_LIBADD
BLOCKEM_OBJS = \
../blockem_challenge.o \
../board.o \
../game1v1.o \
../game_total_allocation.o \
../heuristic.o \
../piece.o \
../player.o \
../rules.o \
../bitwise.o

blockemtest_LDADD = $(BLOCKEM_OBJS) @LIBINTL@ \
    $(GTKMM_LIBS) $(GTHREAD_LIBS) $(LIBXML_LIBS)

