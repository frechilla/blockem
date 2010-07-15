// ============================================================================
// Copyright 2009 Faustino Frechilla
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
/// @file  game1v1_test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Sep-2009  Original development
///           Faustino Frechilla 06-May-2010  Bitwise configuration check
///           Faustino Frechilla 15-Jul-2010  Bitwise stuff moved to bitwise_test.[cpp|h]
/// @endhistory
///
// ============================================================================

#include "game1v1_test.h"
#include "bitwise.h"
#ifdef DEBUG_PRINT
#include <iostream>
#endif

Game1v1Test::Game1v1Test() :
    Game1v1()
{
}

Game1v1Test::~Game1v1Test()
{
}

void Game1v1Test::DoTest()
{
    // all other test functions are called from this one
    TestGame1v1PutDownAndRemovePiece();
}

void Game1v1Test::TestGame1v1PutDownAndRemovePiece()
{
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << std::endl;
#endif

    // game1v1 must be reset before starting (otherwise it might fail)
    this->Reset();

    Board  extraBoard(m_board);
    Player extraPlayer1(m_player1);
    Player extraPlayer2(m_player2);

    std::vector<Coordinate> validCoords(PIECE_MAX_SQUARES);
    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        // both m_player1 and m_player2 should have all the pieces available anyway
        const std::list<PieceConfiguration> &pieceConfList =
            m_player1.m_pieces[i].GetPrecalculatedConfs();

        std::list<PieceConfiguration>::const_iterator pieceConfIt;
        for (pieceConfIt  = pieceConfList.begin();
             pieceConfIt != pieceConfList.end();
             pieceConfIt++)
        {
            // sets&iterators to retrieve nk points
            STLCoordinateSet_t nkPointsSetPlayer1;
            STLCoordinateSet_t nkPointsSetPlayer2;
            STLCoordinateSet_t nkPointsSetExtraPlayer1;
            STLCoordinateSet_t nkPointsSetExtraPlayer2;
            STLCoordinateSet_t::const_iterator nkIt;
            STLCoordinateSet_t::const_iterator extraNKIt;

            // put down both pieces on the real game1v1 board using the game1v1 method
            Game1v1::PutDownPiece(
                m_board,
                m_player1.GetStartingCoordinate(),
                *pieceConfIt,
                m_player1,
                m_player2);

            TestGame1v1GetAllNucleationPoints(Game1v1::e_Game1v1Player1);
            TestGame1v1GetAllNucleationPoints(Game1v1::e_Game1v1Player2);

            Game1v1::PutDownPiece(
                m_board,
                m_player2.GetStartingCoordinate(),
                *pieceConfIt,
                m_player2,
                m_player1);

            TestGame1v1GetAllNucleationPoints(Game1v1::e_Game1v1Player1);
            TestGame1v1GetAllNucleationPoints(Game1v1::e_Game1v1Player2);

            // put down the very same pieces on the extraBoard using the extra players
            // using directly m_pieceSquares attribute
            PieceConfigurationContainer_t::const_iterator it;
            for (it  = pieceConfIt->m_pieceSquares.begin();
                 it != pieceConfIt->m_pieceSquares.end();
                 it++)
            {
                Coordinate player1Coord(
                    m_player1.GetStartingCoordinate().m_row + it->m_row,
                    m_player1.GetStartingCoordinate().m_col + it->m_col);

                Coordinate player2Coord(
                    m_player2.GetStartingCoordinate().m_row + it->m_row,
                    m_player2.GetStartingCoordinate().m_col + it->m_col);

                assert(extraBoard.IsCoordEmpty(player1Coord));
                assert(extraBoard.IsCoordEmpty(player2Coord));

                extraBoard.SetPlayerInCoord(player1Coord, extraPlayer1);
                extraBoard.SetPlayerInCoord(player2Coord, extraPlayer2);
            } // for (it = a_pieceConf.m_pieceSquares.begin();

            // recalculate all the nk points for both extra players on the board
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
                    if (rules::IsNucleationPointCompute(extraBoard, extraPlayer1, thisCoord))
                    {
                        extraPlayer1.SetNucleationPoint(thisCoord);
                    }
                    else
                    {
                        extraPlayer1.UnsetNucleationPoint(thisCoord);
                    }

                    // "extra" player2
                    if (rules::IsNucleationPointCompute(extraBoard, extraPlayer2, thisCoord))
                    {
                        extraPlayer2.SetNucleationPoint(thisCoord);
                    }
                    else
                    {
                        extraPlayer2.UnsetNucleationPoint(thisCoord);
                    }

                    assert (m_board.IsCoordEmpty(thisCoord) ==
                            extraBoard.IsCoordEmpty(thisCoord));

                    assert (m_board.IsPlayerInCoord(thisCoord, m_player1) ==
                            extraBoard.IsPlayerInCoord(thisCoord, extraPlayer1));

                    assert (m_board.IsPlayerInCoord(thisCoord, m_player2) ==
                            extraBoard.IsPlayerInCoord(thisCoord, extraPlayer2));
                }
            }

            // GetAllNucleationPoints method must retrieve the same nucleation points
            m_player1.GetAllNucleationPoints(nkPointsSetPlayer1);
            m_player2.GetAllNucleationPoints(nkPointsSetPlayer2);
            extraPlayer1.GetAllNucleationPoints(nkPointsSetExtraPlayer1);
            extraPlayer2.GetAllNucleationPoints(nkPointsSetExtraPlayer2);

            assert(nkPointsSetPlayer1.size() == nkPointsSetExtraPlayer1.size());
            assert(nkPointsSetPlayer2.size() == nkPointsSetExtraPlayer2.size());

            nkIt= nkPointsSetPlayer1.begin();
            extraNKIt = nkPointsSetExtraPlayer1.begin();
            while (nkIt != nkPointsSetPlayer1.end() &&
                   extraNKIt != nkPointsSetExtraPlayer1.end())
            {
                assert (*nkIt == *extraNKIt);
                assert (m_player1.IsNucleationPoint(*nkIt) ==
                        extraPlayer1.IsNucleationPoint(*extraNKIt));
                nkIt++;
                extraNKIt++;
            }
            assert (nkIt == nkPointsSetPlayer1.end() &&
                   extraNKIt == nkPointsSetExtraPlayer1.end());

            nkIt= nkPointsSetPlayer2.begin();
            extraNKIt = nkPointsSetExtraPlayer2.begin();
            while (nkIt != nkPointsSetPlayer2.end() &&
                   extraNKIt != nkPointsSetExtraPlayer2.end())
            {
                assert (*nkIt == *extraNKIt);
                assert (m_player2.IsNucleationPoint(*nkIt) ==
                        extraPlayer2.IsNucleationPoint(*extraNKIt));
                nkIt++;
                extraNKIt++;
            }
            assert (nkIt == nkPointsSetPlayer2.end() &&
                   extraNKIt == nkPointsSetExtraPlayer2.end());



            // remove pieces and do it again
            Game1v1::RemovePiece(
                m_board,
                m_player1.GetStartingCoordinate(),
                *pieceConfIt,
                m_player1,
                m_player2);

            TestGame1v1GetAllNucleationPoints(Game1v1::e_Game1v1Player2);
            TestGame1v1GetAllNucleationPoints(Game1v1::e_Game1v1Player1);

            Game1v1::RemovePiece(
                m_board,
                m_player2.GetStartingCoordinate(),
                *pieceConfIt,
                m_player2,
                m_player1);

            TestGame1v1GetAllNucleationPoints(Game1v1::e_Game1v1Player2);
            TestGame1v1GetAllNucleationPoints(Game1v1::e_Game1v1Player1);

            // remove the very same pieces from the extraBoard using the extra players
            // using directly m_pieceSquares attribute
            for (it  = pieceConfIt->m_pieceSquares.begin();
                 it != pieceConfIt->m_pieceSquares.end();
                 it++)
            {
                Coordinate player1Coord(
                    m_player1.GetStartingCoordinate().m_row + it->m_row,
                    m_player1.GetStartingCoordinate().m_col + it->m_col);

                Coordinate player2Coord(
                    m_player2.GetStartingCoordinate().m_row + it->m_row,
                    m_player2.GetStartingCoordinate().m_col + it->m_col);

                assert(extraBoard.IsPlayerInCoord(player1Coord, extraPlayer1));
                assert(extraBoard.IsPlayerInCoord(player2Coord, extraPlayer2));

                extraBoard.BlankCoord(player1Coord);
                extraBoard.BlankCoord(player2Coord);
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
                    if (rules::IsNucleationPointCompute(extraBoard, extraPlayer1, thisCoord))
                    {
                        extraPlayer1.SetNucleationPoint(thisCoord);
                    }
                    else
                    {
                        extraPlayer1.UnsetNucleationPoint(thisCoord);
                    }

                    // "extra" player2
                    if (rules::IsNucleationPointCompute(extraBoard, extraPlayer2, thisCoord))
                    {
                        extraPlayer2.SetNucleationPoint(thisCoord);
                    }
                    else
                    {
                        extraPlayer2.UnsetNucleationPoint(thisCoord);
                    }

                    assert (m_board.IsCoordEmpty(thisCoord) ==
                            extraBoard.IsCoordEmpty(thisCoord));

                    assert (m_board.IsPlayerInCoord(thisCoord, m_player1) ==
                            extraBoard.IsPlayerInCoord(thisCoord, extraPlayer1));

                    assert (m_board.IsPlayerInCoord(thisCoord, m_player2) ==
                            extraBoard.IsPlayerInCoord(thisCoord, extraPlayer2));
                }
            }

            // GetAllNucleationPoints method must retrieve the same nucleation points
            nkPointsSetPlayer1.clear();
            m_player1.GetAllNucleationPoints(nkPointsSetPlayer1);
            nkPointsSetPlayer2.clear();
            m_player2.GetAllNucleationPoints(nkPointsSetPlayer2);
            nkPointsSetExtraPlayer1.clear();
            extraPlayer1.GetAllNucleationPoints(nkPointsSetExtraPlayer1);
            nkPointsSetExtraPlayer2.clear();
            extraPlayer2.GetAllNucleationPoints(nkPointsSetExtraPlayer2);

            assert(nkPointsSetPlayer1.size() == nkPointsSetExtraPlayer1.size());
            assert(nkPointsSetPlayer2.size() == nkPointsSetExtraPlayer2.size());

            nkIt= nkPointsSetPlayer1.begin();
            extraNKIt = nkPointsSetExtraPlayer1.begin();
            while (nkIt != nkPointsSetPlayer1.end() &&
                   extraNKIt != nkPointsSetExtraPlayer1.end())
            {
                assert (*nkIt == *extraNKIt);
                assert (m_player1.IsNucleationPoint(*nkIt) ==
                        extraPlayer1.IsNucleationPoint(*extraNKIt));
                nkIt++;
                extraNKIt++;
            }
            assert (nkIt == nkPointsSetPlayer1.end() &&
                   extraNKIt == nkPointsSetExtraPlayer1.end());

            nkIt= nkPointsSetPlayer2.begin();
            extraNKIt = nkPointsSetExtraPlayer2.begin();
            while (nkIt != nkPointsSetPlayer2.end() &&
                   extraNKIt != nkPointsSetExtraPlayer2.end())
            {
                assert (*nkIt == *extraNKIt);
                assert (m_player2.IsNucleationPoint(*nkIt) ==
                        extraPlayer2.IsNucleationPoint(*extraNKIt));
                nkIt++;
                extraNKIt++;
            }
            assert (nkIt == nkPointsSetPlayer2.end() &&
                   extraNKIt == nkPointsSetExtraPlayer2.end());

        } // for (pieceCoordIt = coordConfList.begin()
    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
}

void Game1v1Test::TestGame1v1GetAllNucleationPoints(eGame1v1Player_t who)
{
#ifndef NDEBUG
    // this ifndef prevents a warning when compiling in release mode
    Player::SpiralIterator iterator;
#endif
    Coordinate tmpCoord;

    Game1v1::Game1v1CoordinateSet_t coordSetCustom;
    Game1v1::GetAllNucleationPoints(GetPlayer(who), coordSetCustom);
    assert(static_cast<int32_t>(coordSetCustom.size()) == GetPlayer(who).NumberOfNucleationPoints());

    STLCoordinateSet_t coordSet;
    GetPlayer(who).GetAllNucleationPoints(coordSet);
    assert(static_cast<int32_t>(coordSet.size()) == GetPlayer(who).NumberOfNucleationPoints());

    if (GetPlayer(who).NumberOfNucleationPoints() > 0)
    {
        // check GetFirstNucleationPointSpiral works fine
        assert(
            GetPlayer(who).GetFirstNucleationPointSpiral(iterator, tmpCoord));

        STLCoordinateSet_t::iterator setIt = coordSet.find(tmpCoord);
        assert(setIt != coordSet.end());
        assert(coordSetCustom.isPresent(tmpCoord));

        // assert any other nk point is found by the spiral algorithm
        for (int32_t i = 1; i < GetPlayer(who).NumberOfNucleationPoints(); i++)
        {
            assert(
                GetPlayer(who).GetNextNucleationPointSpiral(iterator, tmpCoord));

            STLCoordinateSet_t::iterator setIt = coordSet.find(tmpCoord);
            assert(setIt != coordSet.end());
            assert(coordSetCustom.isPresent(tmpCoord));
        }

        // assert there is no more nk points left
        assert(
            GetPlayer(who).GetNextNucleationPointSpiral(iterator, tmpCoord) == false);
    }
    else
    {
        // assert spiral algorithm also says there is no nk points
        assert(
            GetPlayer(who).GetFirstNucleationPointSpiral(iterator, tmpCoord) == false);
    }
}
