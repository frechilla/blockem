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
/// @file  player_test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 3-Jul-2010   Original development
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif

#include "player_test.h"
#include "rules.h"

#define ROWS    14
#define COLUMNS 14

PlayerTest::PlayerTest() :
    m_board(ROWS, COLUMNS),
    m_player(std::string("the player"), 'X', ROWS, COLUMNS, Coordinate())
{
}

PlayerTest::~PlayerTest()
{
}

void PlayerTest::DoTest()
{
    //std::cout << __FUNCTION__ << " Started" << std::endl << std::endl;

    // tests copy constructor and operator=
    TestCopyPlayer();

    // test nk points
    TestNKPoints();
}

void PlayerTest::TestCopyPlayer()
{
#ifdef DEBUG_PRINT
    std::cout << "Player copy constructor and operator= test started... ";
    std::cout.flush();
#endif

    Player p1(std::string("tester1"), 'T', 5, 5, Coordinate());
    Player p2(p1);
    Player p3(std::string("tester3"), 'T', 6, 6, Coordinate(1, 1));
    assert(p2.IsNucleationPoint(Coordinate(0, 0)) == p1.IsNucleationPoint(Coordinate(0, 0)));

    p2.SetNucleationPoint(Coordinate(0, 0));
    assert(p2.IsNucleationPoint(Coordinate(0, 0)) != p1.IsNucleationPoint(Coordinate(0, 0)));

    p1 = p2;
    assert(p2.IsNucleationPoint(Coordinate(0, 0)) == p1.IsNucleationPoint(Coordinate(0, 0)));

    p3 = p1;
    assert(p3.IsNucleationPoint(Coordinate(0, 0)) == p1.IsNucleationPoint(Coordinate(0, 0)));

    p3.UnsetNucleationPoint(Coordinate(0, 0));
    assert(p3.IsNucleationPoint(Coordinate(0, 0)) != p1.IsNucleationPoint(Coordinate(0, 0)));

#ifdef DEBUG_PRINT
    std::cout << "  [Passed]" << std::endl;
#endif
}

void PlayerTest::TestNKPoints()
{
#ifdef DEBUG_PRINT
    std::cout << "Player NK points test started... ";
    std::cout.flush();
#endif

    m_board.Reset();
    m_player.Reset(Coordinate());
    Player a_tmpPlayer(std::string("someone else"), 'O', ROWS, COLUMNS, Coordinate());

    // 0: empty
    // +: a_tmpPlayer
    // X: m_player
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 0 0 0 0 0 0
    // 0 0 0 + + + 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 X X X 0 0 0 0 0 0
    // 0 0 0 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    PutDownPiece(m_player.m_pieces[e_5Piece_Cross], Coordinate(6, 6), m_player);
    RecalculateNKPointsInWholeBoard(a_tmpPlayer);
    TestNKSpiralAlgorithm(m_player);
    TestNKSpiralAlgorithm(a_tmpPlayer);
    PutDownPiece(a_tmpPlayer.m_pieces[e_5Piece_Cross], Coordinate(4, 4), a_tmpPlayer);
    RecalculateNKPointsInWholeBoard(m_player);
    TestNKSpiralAlgorithm(m_player);
    TestNKSpiralAlgorithm(a_tmpPlayer);

    // reset the board to launch next test (where only 2 baby pieces are deployed)
    m_board.Reset();
    m_player.Reset(Coordinate());
    a_tmpPlayer.Reset(Coordinate());

    // 0: empty
    // +: a_tmpPlayer
    // X: m_player
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    PutDownPiece(m_player.m_pieces[e_1Piece_BabyPiece], Coordinate(6, 6), m_player);
    RecalculateNKPointsInWholeBoard(a_tmpPlayer);
    TestNKSpiralAlgorithm(m_player);
    TestNKSpiralAlgorithm(a_tmpPlayer);
    PutDownPiece(a_tmpPlayer.m_pieces[e_1Piece_BabyPiece], Coordinate(4, 4), a_tmpPlayer);
    RecalculateNKPointsInWholeBoard(m_player);
    TestNKSpiralAlgorithm(m_player);
    TestNKSpiralAlgorithm(a_tmpPlayer);


    // reset the board to launch next test (where only 2 baby pieces are deployed)
    m_board.Reset();
    m_player.Reset(Coordinate());
    a_tmpPlayer.Reset(Coordinate());

    // 0: empty
    // +: m_player
    // X: player1
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 0 0 0 0 0 0
    // 0 0 0 + + 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 X X 0 0 0 0 0 0
    // 0 0 0 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0

    // e_3Piece_Triangle is originaly described as:
    //  X(X)
    //    X
    m_player.m_pieces[e_3Piece_Triangle].RotateLeft();
    PutDownPiece(m_player.m_pieces[e_3Piece_Triangle], Coordinate(6, 6), m_player);
    RecalculateNKPointsInWholeBoard(a_tmpPlayer);
    TestNKSpiralAlgorithm(m_player);
    TestNKSpiralAlgorithm(a_tmpPlayer);
    a_tmpPlayer.m_pieces[e_3Piece_Triangle].RotateRight();
    PutDownPiece(a_tmpPlayer.m_pieces[e_3Piece_Triangle], Coordinate(4, 4), a_tmpPlayer);
    RecalculateNKPointsInWholeBoard(m_player);
    TestNKSpiralAlgorithm(m_player);
    TestNKSpiralAlgorithm(a_tmpPlayer);


    // 0: empty
    // +: a_tmpPlayer
    // X: m_player
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 X 0 0 0 0 0
    // 0 0 0 + + 0 X X X 0 0 0 0 0
    // 0 0 + 0 0 0 0 0 X 0 0 0 0 0
    // 0 0 + + + 0 X X 0 0 0 0 0 0
    // 0 0 + 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0

    // put down an extra Mr. T piece. e_5Piece_MrT is originally described as:
    //     X
    // X(X)X
    //     X
    PutDownPiece(m_player.m_pieces[e_5Piece_MrT], Coordinate(4, 7), m_player);
    RecalculateNKPointsInWholeBoard(a_tmpPlayer);
    TestNKSpiralAlgorithm(m_player);
    TestNKSpiralAlgorithm(a_tmpPlayer);
    a_tmpPlayer.m_pieces[e_5Piece_MrT].RotateRight();
    a_tmpPlayer.m_pieces[e_5Piece_MrT].RotateRight();
    PutDownPiece(a_tmpPlayer.m_pieces[e_5Piece_MrT], Coordinate(6, 3), a_tmpPlayer);
    RecalculateNKPointsInWholeBoard(m_player);
    TestNKSpiralAlgorithm(m_player);
    TestNKSpiralAlgorithm(a_tmpPlayer);


    // 0: empty
    // +: a_tmpPlayer
    // X: m_player
    // 0 0 0 0 0 0 0 0 0 0 X X 0 0
    // 0 0 0 0 0 0 0 0 0 0 X 0 0 0
    // 0 0 0 0 0 0 0 0 0 X X 0 0 0
    // 0 0 0 0 + 0 0 0 X 0 0 0 0 0
    // 0 0 0 + + 0 X X X 0 0 0 0 0
    // 0 0 + 0 0 0 0 0 X 0 0 0 0 0
    // 0 0 + + + 0 X X 0 0 0 0 0 0
    // 0 0 + 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0

    // put down an extra Big S piece. e_5Piece_BigS is originally described as:
    //   X X
    //  (X)
    // X X
    PutDownPiece(m_player.m_pieces[e_5Piece_BigS], Coordinate(1, 10), m_player);
    RecalculateNKPointsInWholeBoard(a_tmpPlayer);
    TestNKSpiralAlgorithm(m_player);
    TestNKSpiralAlgorithm(a_tmpPlayer);

#ifdef DEBUG_PRINT
    std::cout << "  [Passed]" << std::endl;
#endif
}

void PlayerTest::TestNKSpiralAlgorithm(const Player &a_player)
{
    // variables to test Get[First|Next]NucleationPointSpiral
    Player::SpiralIterator iterator;
    Coordinate tmpCoord;

    STLCoordinateSet_t coordSet;
    a_player.GetAllNucleationPoints(coordSet);
    assert(static_cast<int32_t>(coordSet.size()) == a_player.NumberOfNucleationPoints());

    if (a_player.NumberOfNucleationPoints() > 0)
    {
        // check GetFirstNucleationPointSpiral works fine
        bool rv = a_player.GetFirstNucleationPointSpiral(iterator, tmpCoord);
        assert(rv == true);

        STLCoordinateSet_t::iterator setIt = coordSet.find(tmpCoord);
        assert(setIt != coordSet.end());

        // easy to check now if CalculateNextValidCoordInNucleationPoint works as
        // CalculateValidCoordsInNucleationPoint does
        TestCalculateValidCoordsInNKPoint(a_player, tmpCoord);

        // assert any other nk point is found by the spiral algorithm
        for (int32_t i = 1; i < a_player.NumberOfNucleationPoints(); i++)
        {
            rv = a_player.GetNextNucleationPointSpiral(iterator, tmpCoord);
            assert(rv == true);

            setIt = coordSet.find(tmpCoord);
            assert(setIt != coordSet.end());

            // easy to check now if CalculateNextValidCoordInNucleationPoint works as
            // CalculateValidCoordsInNucleationPoint does
            TestCalculateValidCoordsInNKPoint(a_player, tmpCoord);
        }

        // assert there is no more nk points left
        assert(
                a_player.GetNextNucleationPointSpiral(iterator, tmpCoord) == false);
    }
    else
    {
        // assert spiral algorithm also says there is no nk points
        assert(
                a_player.GetFirstNucleationPointSpiral(iterator, tmpCoord) == false);
    }
}

void PlayerTest::TestCalculateValidCoordsInNKPoint(const Player &a_player, const Coordinate &where)
{
    assert(a_player.IsNucleationPoint(where));
    assert(m_board.IsCoordEmpty(where));

    // no need to instantiate a bigger vector
    std::vector<Coordinate> validCoordVector(PIECE_MAX_SQUARES);
    Coordinate validCoord;
    int32_t nNKPointsSecondMethod;
    int32_t nNKPoints;

    // test is done using all pieces available (no rotate or mirror though)
    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
    {
        Piece tmpPiece(static_cast<ePieceType_t>(i));

        nNKPoints = rules::CalculateValidCoordsInNucleationPoint(
                                    m_board,
                                    a_player,
                                    where,
                                    tmpPiece.GetCurrentConfiguration(),
                                    validCoordVector);

        validCoord.m_row = validCoord.m_col = COORD_UNINITIALISED;
        nNKPointsSecondMethod = 0;
        while(rules::CalculateNextValidCoordInNucleationPoint(
                                    m_board,
                                    a_player,
                                    where,
                                    tmpPiece.GetCurrentConfiguration(),
                                    tmpPiece.GetRadius(),
                                    validCoord))
        {
            nNKPointsSecondMethod++;
        }

        // assert both methods found the same amount of valid coords
        assert(nNKPoints == nNKPointsSecondMethod);
    }
}

void PlayerTest::RecalculateNKPointsInWholeBoard(Player &a_player)
{
    Coordinate thisCoord(0, 0);
    for (thisCoord.m_row = 0 ;
         thisCoord.m_row < m_board.GetNRows();
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0 ;
             thisCoord.m_col < m_board.GetNColumns();
             thisCoord.m_col++)
        {
            if (rules::IsNucleationPointCompute(m_board, a_player, thisCoord))
            {
                a_player.SetNucleationPoint(thisCoord);
            }
            else
            {
                a_player.UnsetNucleationPoint(thisCoord);
            }
        }
    }
}

void PlayerTest::PutDownPiece(const Piece &a_piece, const Coordinate &a_coord, Player &a_player)
{
    const PieceConfiguration &a_pieceConf =
        a_piece.GetCurrentConfiguration();

    PieceConfigurationContainer_t::const_iterator it;
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        assert( ((a_coord.m_row + it->m_row) >= 0) &&
                ((a_coord.m_row + it->m_row) < m_board.GetNRows()) );
        assert( ((a_coord.m_col + it->m_col) >= 0) &&
                ((a_coord.m_col + it->m_col) < m_board.GetNColumns()) );

        assert(m_board.IsCoordEmpty(thisCoord));

        m_board.SetPlayerInCoord(thisCoord, a_player);
    } // for (it = a_pieceConf.m_pieceSquares.begin();

    // recalculate nk points in the whole board after this piece was put down
    RecalculateNKPointsInWholeBoard(a_player);
}

void PlayerTest::RemovePiece(const Piece &a_piece, const Coordinate &a_coord, Player &a_player)
{
    const PieceConfiguration &a_pieceConf =
        a_piece.GetCurrentConfiguration();

    PieceConfigurationContainer_t::const_iterator it;
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        assert( ((a_coord.m_row + it->m_row) >= 0) &&
                ((a_coord.m_row + it->m_row) < m_board.GetNRows()) );
        assert( ((a_coord.m_col + it->m_col) >= 0) &&
                ((a_coord.m_col + it->m_col) < m_board.GetNColumns()) );

        assert(m_board.IsPlayerInCoord(thisCoord, a_player));

        // remove the current square of the piece from the board
        m_board.BlankCoord(thisCoord);
    }

    // recalculate nk points in the whole board after this piece was removed
    RecalculateNKPointsInWholeBoard(a_player);
}
