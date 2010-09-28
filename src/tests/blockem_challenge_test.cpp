// ============================================================================
// Copyright 2010 Faustino Frechilla
//
// This file is part of Blockem.
//
// Blockem is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// Blockem is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with Blockem. If not, see http://www.gnu.org/licenses/.
//
/// @file  blockem_challenge_test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 28-Sep-2010  Original development
/// @endhistory
///
// ============================================================================
#include <iostream>
#include <algorithm> // std::set_union
#include "assert.h"
#include "blockem_challenge_test.h"

#define CHALLENGE_DIR "challengetest"

BlockemChallengeTest::BlockemChallengeTest()
{
}

BlockemChallengeTest::~BlockemChallengeTest()
{
}

void BlockemChallengeTest::DoTest()
{
    // build an empty challenge
    BlockemChallenge theChallenge();

    // try to load a challenge from a non existent .xml file into it
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/this_file_does_not_exist.xml"));

    // try out an empty file
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/emptyfile.xml"));

    // malformed XML file (bogus syntax)
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_syntax_bogus.xml"));

    // "blockem_challenge" root tag does not exist
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_root_bogus.xml"));
    // "blockem_challenge" property "name" does not exist
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_root_has_no_name_property.xml"));
    // "blockem_challenge" property "name" is empty
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_root_has_empty_name_property.xml"));

    // "blockem_challenge" doesn't have "board" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_board_no.xml"));
    // "blockem_challenge" doesn't have "board"->"nrows" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_board_no_nrows.xml"));
    // "blockem_challenge" doesn't have "board"->"ncolumns" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_board_no_ncolumns.xml"));
    // "blockem_challenge" bogus "board"->"nrows" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_board_nrows_invalid.xml"));
    // "blockem_challenge" bogus "board"->"nrows" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_board_nrows_negative.xml"));
    // "blockem_challenge" bogus "board"->"ncolumns" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_board_ncolumns_invalid.xml"));
    // "blockem_challenge" bogus "board"->"ncolumns" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_board_ncolumns_negative.xml"));

    // "blockem_challenge" doesn't have "opponent" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_no.xml"));
    // "blockem_challenge" doesn't have "opponent" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_activetag_no.xml"));
    // "blockem_challenge" doesn't have "opponent" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_activetag_invalid.xml"));
    // "blockem_challenge" bogus "opponent"->"taken" no row
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_taken_row_no.xml"));
    // "blockem_challenge" bogus "opponent"->"taken" no col
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_taken_col_no.xml"));
    // "blockem_challenge" bogus "opponent"->"taken"->"row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_taken_row_invalid.xml"));
    // "blockem_challenge" bogus "opponent"->"taken"->"row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_taken_row_negative.xml"));
    // "blockem_challenge" bogus "opponent"->"taken"->"row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_taken_row_toobig.xml"));
    // "blockem_challenge" bogus "opponent"->"taken"->"col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_taken_col_invalid.xml"));
    // "blockem_challenge" bogus "opponent"->"taken"->"col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_taken_col_negative.xml"));
    // "blockem_challenge" bogus "opponent"->"taken"->"col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_taken_col_toobig.xml"));
    // "blockem_challenge" bogus "opponent"->"taken" duplicated and bogus
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_taken_duplicated_bogus.xml"));
    //"blockem_challenge" bogus "opponent"->"piece". available tag missing
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_piece_available_no.xml"));
    //"blockem_challenge" bogus "opponent"->"piece". available tag bogus
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_piece_available_invalid.xml"));
    //"blockem_challenge" bogus "opponent"->"piece". Piece name invalid + available tag bogus
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_piece_available_invalid_name_bogus.xml"));
    // "blockem_challenge" bogus "opponent"->"starting_row" no (col set)
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_starting_row_no.xml"));
    // "blockem_challenge" bogus "opponent"->"starting_col" no (row set)
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_starting_col_no.xml"));
    // "blockem_challenge" bogus "opponent"->"starting_row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_starting_row_invalid.xml"));
    // "blockem_challenge" bogus "opponent"->"starting_row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_starting_row_negative.xml"));
    // "blockem_challenge" bogus "opponent"->"starting_row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_starting_row_toobig.xml"));
    // "blockem_challenge" bogus "opponent"->"starting_col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_starting_col_invalid.xml"));
    // "blockem_challenge" bogus "opponent"->"starting_col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_starting_col_negative.xml"));
    // "blockem_challenge" bogus "opponent"->"starting_col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_opponent_starting_col_toobig.xml"));

    // "blockem_challenge" doesn't have "challenger" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_no.xml"));
    // "blockem_challenge" bogus "challenger"->"taken" no row
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_taken_row_no.xml"));
    // "blockem_challenge" bogus "challenger"->"taken" no col
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_taken_col_no.xml"));
    // "blockem_challenge" bogus "challenger"->"taken"->"row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_taken_row_invalid.xml"));
    // "blockem_challenge" bogus "challenger"->"taken"->"row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_taken_row_negative.xml"));
    // "blockem_challenge" bogus "challenger"->"taken"->"row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_taken_row_toobig.xml"));
    // "blockem_challenge" bogus "challenger"->"taken"->"col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_taken_col_invalid.xml"));
    // "blockem_challenge" bogus "challenger"->"taken"->"col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_taken_col_negative.xml"));
    // "blockem_challenge" bogus "challenger"->"taken"->"col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_taken_col_toobig.xml"));
    // "blockem_challenge" bogus "challenger"->"taken" duplicated and bogus
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_taken_duplicated_bogus.xml"));
    //"blockem_challenge" bogus "challenger"->"piece". available tag missing
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_piece_available_no.xml"));
    //"blockem_challenge" bogus "challenger"->"piece". available tag bogus
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_piece_available_invalid.xml"));
    //"blockem_challenge" bogus "challenger"->"piece". Piece name invalid + available tag bogus
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_piece_available_invalid_name_bogus.xml"));
    // "blockem_challenge" bogus "challenger"->"starting_row" no
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_starting_row_no.xml"));
    // "blockem_challenge" bogus "challenger"->"starting_col" no
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_starting_col_no.xml"));
    // "blockem_challenge" bogus "challenger"->"starting_col" and "starting_row" not set
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_starting_col_row_no.xml"));
    // "blockem_challenge" bogus "challenger"->"starting_row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_starting_row_invalid.xml"));
    // "blockem_challenge" bogus "challenger"->"starting_row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_starting_row_negative.xml"));
    // "blockem_challenge" bogus "challenger"->"starting_row" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_starting_row_toobig.xml"));
    // "blockem_challenge" bogus "challenger"->"starting_col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_starting_col_invalid.xml"));
    // "blockem_challenge" bogus "challenger"->"starting_col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_starting_col_negative.xml"));
    // "blockem_challenge" bogus "challenger"->"starting_col" tag
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_challenger_starting_col_toobig.xml"));

    // "blockem_challenge" bogus challenger and opponent have same coordinate as taken
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_taken_duplicated_opponent_challenger.xml"));

    // opponent is active and its starting coords are the same as the challenger's
    assert(!TryLoadXMLChallenge(CHALLENGE_DIR"/challenge_starting_coords_equal.xml"));

    // go for a valid challenge, and ensure it loads what the xml file says
    LoadAndCheckValidChallenge();
}

bool BlockemChallengeTest::TryLoadXMLChallenge(const char* fileName)
{
    bool successfullyLoaded = true;
    try
    {
        BlockemChallenge theChallenge(fileName);
    }
    catch (const std::runtime_error &ex)
    {
        // print what the exception was
        std::cout << "Exception caught: " << ex.what()
                  << std::endl << std::endl;
        successfullyLoaded = false;
    }

    return successfullyLoaded;
}

void BlockemChallengeTest::LoadAndCheckValidChallenge()
{
    BlockemChallenge theChallenge;
    try
    {
        theChallenge.LoadXMLChallenge(CHALLENGE_DIR"/challenge_valid1.xml");
    }
    catch (const std::runtime_error &ex)
    {
        // error!
        std::cout << "Exception caught: " << ex.what() << std::endl;
        assert(0);
    }

    // You've got to trust me about this. The .xml file contains
    // exactly these parameters
    assert (theChallenge.GetChallengeName() == "tst: Valid challenge 1");

    assert (theChallenge.GetBoardRows() == 14);
    assert (theChallenge.GetBoardColumns() == 14);

    assert (theChallenge.IsOpponentActive() == true);

    assert (theChallenge.GetOpponentStartingCoord() ==
            Coordinate(5, 5));
    assert (theChallenge.GetChallengerStartingCoord() ==
            Coordinate(8, 8));

    const STLCoordinateSet_t &opponentTakenSet =
        theChallenge.GetOpponentTakenSquares();

    assert (opponentTakenSet.size() == 4);
    assert (opponentTakenSet.isPresent(Coordinate(0,  0)));
    assert (opponentTakenSet.isPresent(Coordinate(0, 13)));
    assert (opponentTakenSet.isPresent(Coordinate(13, 0)));
    assert (opponentTakenSet.isPresent(Coordinate(13, 13)));

    const STLCoordinateSet_t &challengerTakenSet =
        theChallenge.GetChallengerTakenSquares();

    assert (challengerTakenSet.size() == 8);
    assert (challengerTakenSet.isPresent(Coordinate(1,  1)));
    assert (challengerTakenSet.isPresent(Coordinate(1, 12)));
    assert (challengerTakenSet.isPresent(Coordinate(12, 1)));
    assert (challengerTakenSet.isPresent(Coordinate(12, 12)));
    assert (challengerTakenSet.isPresent(Coordinate(6,  6)));
    assert (challengerTakenSet.isPresent(Coordinate(6,  7)));
    assert (challengerTakenSet.isPresent(Coordinate(7,  6)));
    assert (challengerTakenSet.isPresent(Coordinate(7,  7)));

    // m_challengerTakenSquares INTERSECTION m_opponentTakenSquares MUST
    // be an empty set
    STLCoordinateSet_t takenSquaresIntersectionSet;
    std::set_intersection(
        opponentTakenSet.begin(),
        opponentTakenSet.end(),
        challengerTakenSet.begin(),
        challengerTakenSet.end(),
        std::inserter(takenSquaresIntersectionSet, takenSquaresIntersectionSet.begin()));
    assert(takenSquaresIntersectionSet.empty() == true);

    assert (theChallenge.IsOpponentPieceAvailable(e_1Piece_BabyPiece) == true);
    assert (theChallenge.IsOpponentPieceAvailable(e_5Piece_Cross)     == true);
    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
    {
        if ((i != e_1Piece_BabyPiece) && (i != e_5Piece_Cross))
        {
            assert (theChallenge.IsOpponentPieceAvailable(static_cast<ePieceType_t>(i)) == false);
        }
    }

    assert (theChallenge.IsChallengerPieceAvailable(e_1Piece_BabyPiece) == false);
    assert (theChallenge.IsChallengerPieceAvailable(e_5Piece_SafPiece) == false);
    assert (theChallenge.IsChallengerPieceAvailable(e_5Piece_WPiece) == false);
    assert (theChallenge.IsChallengerPieceAvailable(e_5Piece_Cross) == false);
    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
    {
        if ( (i != e_1Piece_BabyPiece) &&
             (i != e_5Piece_SafPiece)  &&
             (i != e_5Piece_WPiece)    &&
             (i != e_5Piece_Cross)     )
        {
            assert (theChallenge.IsChallengerPieceAvailable(static_cast<ePieceType_t>(i)) == true);
        }
    }
}
