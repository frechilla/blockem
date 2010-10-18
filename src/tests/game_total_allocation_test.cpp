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
/// @file  game_total_allocation_test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 18-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#include "game_total_allocation_test.h"
#include "bitwise.h"
#include "rules.h"
#ifdef DEBUG_PRINT
#include <iostream>
#endif

static const int32_t TEST_NROWS = 14;
static const int32_t TEST_NCOLS = 14;
static const Coordinate TEST_STARTING_COORD = Coordinate();

GameTotalAllocationTest::GameTotalAllocationTest() :
    GameTotalAllocation(TEST_NROWS, TEST_NCOLS, TEST_STARTING_COORD)
{
}

GameTotalAllocationTest::~GameTotalAllocationTest()
{
}

void GameTotalAllocationTest::DoTest()
{
    TestGameTotalAllocationPutDownAndRemovePiece();
    TestSolve();
}

bool GameTotalAllocationTest::IsBoardEmpty()
{
    for (int32_t i = 0; i < GetBoard().GetNRows(); i++)
    {
        for (int32_t j = 0; j < GetBoard().GetNColumns(); j++)
        {
            if (GetBoard().IsCoordEmpty(Coordinate(i, j)) == false)
            {
                return false;
            }
        }
    } 
    
    return true;
}

void GameTotalAllocationTest::TestSolve()
{
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << std::endl;
#endif

    // Impossible to solve starting from a specific coordinate
    Reset(2, 2, Coordinate(0, 0));    
    assert(IsBoardEmpty());
    assert(m_startingCoord.Initialised() == true);
    assert(Solve() == false);
    
    // Impossible to solve starting from anywhere
    Reset(3, 3, Coordinate());
    assert(IsBoardEmpty());
    assert(m_startingCoord.Initialised() == false);
    assert(Solve() == false);
    
    // Solvable game starting from anywhere
    Reset(14, 14, Coordinate());
    assert(IsBoardEmpty());
    assert(m_startingCoord.Initialised() == false);
    assert(Solve() == true);
    
    // Solvable game starting from a specific coordinate
    Reset(15, 14, Coordinate(4, 4));
    assert(IsBoardEmpty());
    assert(m_startingCoord.Initialised() == true);
    assert(Solve() == true);
    
    // put down manually the cross on 4, 4. Remove it.
    // put it down again and try to solve starting from
    // anywhere
    Piece crossPiece(e_5Piece_Cross);
    Reset(15, 15, Coordinate());
    assert(m_startingCoord.Initialised() == false);
    PutDownPiece(crossPiece, Coordinate(4, 4));
    assert(m_player.IsPieceAvailable(e_5Piece_Cross) == false);
    assert(IsBoardEmpty() == false);
    RemovePiece(crossPiece, Coordinate(4, 4));
    assert(m_player.IsPieceAvailable(e_5Piece_Cross) == true);
    assert(IsBoardEmpty() == true);
    PutDownPiece(crossPiece, Coordinate(4, 4));
    assert(m_player.IsPieceAvailable(e_5Piece_Cross) == false);
    assert(IsBoardEmpty() == false);    
    assert(Solve() == true);    
}

void GameTotalAllocationTest::TestGameTotalAllocationPutDownAndRemovePiece()
{
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << std::endl;
#endif

    // game total allocation must be reset before starting 
    // (otherwise it might fail)
    this->Reset(TEST_NROWS, TEST_NCOLS, Coordinate(4, 4));

    Board  extraBoard(m_board);
    Player extraPlayer(m_player);

    std::vector<Coordinate> validCoords(PIECE_MAX_SQUARES);
    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        // m_player should have all the pieces available
        const std::list<PieceConfiguration> &pieceConfList =
            m_player.m_pieces[i].GetPrecalculatedConfs();

        std::list<PieceConfiguration>::const_iterator pieceConfIt;
        for (pieceConfIt  = pieceConfList.begin();
             pieceConfIt != pieceConfList.end();
             pieceConfIt++)
        {
            // sets&iterators to retrieve nk points
            STLCoordinateSet_t nkPointsSetPlayer;
            STLCoordinateSet_t nkPointsSetExtraPlayer;
            STLCoordinateSet_t::const_iterator nkIt;
            STLCoordinateSet_t::const_iterator extraNKIt;

            // put down current piece on the game
            PutDownPiece(
                m_player.GetStartingCoordinate(),
                *pieceConfIt);

            // put down the very same piece on the extraBoard using the extra 
            // players using directly m_pieceSquares attribute
            PieceConfigurationContainer_t::const_iterator it;
            for (it  = pieceConfIt->m_pieceSquares.begin();
                 it != pieceConfIt->m_pieceSquares.end();
                 it++)
            {
                Coordinate playerCoord(
                    m_player.GetStartingCoordinate().m_row + it->m_row,
                    m_player.GetStartingCoordinate().m_col + it->m_col);

                assert(extraBoard.IsCoordEmpty(playerCoord));

                extraBoard.SetPlayerInCoord(playerCoord, extraPlayer);
            } // for (it = a_pieceConf.m_pieceSquares.begin();

            // recalculate all the nk points for the extra player on the board
            // using the "compute" method (it's slow but it's been well tested)
            Coordinate thisCoord(0, 0);
            for (thisCoord.m_row = 0 ;
                 thisCoord.m_row < extraBoard.GetNRows();
                 thisCoord.m_row++)
            {
                for (thisCoord.m_col = 0 ;
                     thisCoord.m_col < extraBoard.GetNColumns();
                     thisCoord.m_col++)
                {
                    // "extra" player1
                    if (rules::IsNucleationPointCompute(extraBoard, extraPlayer, thisCoord))
                    {
                        extraPlayer.SetNucleationPoint(thisCoord);
                    }
                    else
                    {
                        extraPlayer.UnsetNucleationPoint(thisCoord);
                    }

                    assert (m_board.IsCoordEmpty(thisCoord) ==
                            extraBoard.IsCoordEmpty(thisCoord));

                    assert (m_board.IsPlayerInCoord(thisCoord, m_player) ==
                            extraBoard.IsPlayerInCoord(thisCoord, extraPlayer));
                }
            }

            // GetAllNucleationPoints method must retrieve the same nucleation points
            m_player.GetAllNucleationPoints(nkPointsSetPlayer);
            extraPlayer.GetAllNucleationPoints(nkPointsSetExtraPlayer);

            assert(nkPointsSetPlayer.size() == nkPointsSetExtraPlayer.size());

            nkIt= nkPointsSetPlayer.begin();
            extraNKIt = nkPointsSetExtraPlayer.begin();
            while (nkIt != nkPointsSetPlayer.end() &&
                   extraNKIt != nkPointsSetExtraPlayer.end())
            {
                assert (*nkIt == *extraNKIt);
                assert (m_player.IsNucleationPoint(*nkIt) ==
                        extraPlayer.IsNucleationPoint(*extraNKIt));
                nkIt++;
                extraNKIt++;
            }
            assert (nkIt == nkPointsSetPlayer.end() &&
                   extraNKIt == nkPointsSetExtraPlayer.end());


            // remove pieces and do it again
            RemovePiece(
                m_player.GetStartingCoordinate(),
                *pieceConfIt);

            // remove the very same pieces from the extraBoard using the extra players
            // using directly m_pieceSquares attribute
            for (it  = pieceConfIt->m_pieceSquares.begin();
                 it != pieceConfIt->m_pieceSquares.end();
                 it++)
            {
                Coordinate playerCoord(
                    m_player.GetStartingCoordinate().m_row + it->m_row,
                    m_player.GetStartingCoordinate().m_col + it->m_col);

                assert(extraBoard.IsPlayerInCoord(playerCoord, extraPlayer));

                extraBoard.BlankCoord(playerCoord);
            } // for (it = a_pieceConf.m_pieceSquares.begin();

            for (thisCoord.m_row = 0 ;
                 thisCoord.m_row < extraBoard.GetNRows();
                 thisCoord.m_row++)
            {
                for (thisCoord.m_col = 0 ;
                     thisCoord.m_col < extraBoard.GetNColumns();
                     thisCoord.m_col++)
                {
                    // "extra" player1
                    if (rules::IsNucleationPointCompute(extraBoard, extraPlayer, thisCoord))
                    {
                        extraPlayer.SetNucleationPoint(thisCoord);
                    }
                    else
                    {
                        extraPlayer.UnsetNucleationPoint(thisCoord);
                    }

                    assert (m_board.IsCoordEmpty(thisCoord) ==
                            extraBoard.IsCoordEmpty(thisCoord));

                    assert (m_board.IsPlayerInCoord(thisCoord, m_player) ==
                            extraBoard.IsPlayerInCoord(thisCoord, extraPlayer));
                }
            }

            // GetAllNucleationPoints method must retrieve the same nucleation points
            nkPointsSetPlayer.clear();
            m_player.GetAllNucleationPoints(nkPointsSetPlayer);
            nkPointsSetExtraPlayer.clear();
            extraPlayer.GetAllNucleationPoints(nkPointsSetExtraPlayer);

            assert(nkPointsSetPlayer.size() == nkPointsSetExtraPlayer.size());

            nkIt= nkPointsSetPlayer.begin();
            extraNKIt = nkPointsSetExtraPlayer.begin();
            while (nkIt != nkPointsSetPlayer.end() &&
                   extraNKIt != nkPointsSetExtraPlayer.end())
            {
                assert (*nkIt == *extraNKIt);
                assert (m_player.IsNucleationPoint(*nkIt) ==
                        extraPlayer.IsNucleationPoint(*extraNKIt));
                nkIt++;
                extraNKIt++;
            }
            assert (nkIt == nkPointsSetPlayer.end() &&
                   extraNKIt == nkPointsSetExtraPlayer.end());

        } // for (pieceCoordIt = coordConfList.begin()
    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
}
