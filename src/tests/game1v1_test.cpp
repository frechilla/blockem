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
/// @file  Game1v1Test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Sep-2009  Original development
///           Faustino Frechilla 06-May-2010  Bitwise configuration check
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

            TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
            TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

            Game1v1::PutDownPiece(
                m_board,
                m_player2.GetStartingCoordinate(),
                *pieceConfIt,
                m_player2,
                m_player1);

            TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
            TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

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

            TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
            TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);

            Game1v1::RemovePiece(
                m_board,
                m_player2.GetStartingCoordinate(),
                *pieceConfIt,
                m_player2,
                m_player1);

            TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
            TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);

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

void Game1v1Test::TestBitwiseCheckConfiguration(
    const Coordinate         &thisCoord,
    const PieceConfiguration &piece,
    uint64_t                  bPiece,
    uint64_t                  bitwiseBoard,
    uint64_t                  bitwiseBoardPlayer1)
{
    bool isDeployableLegacy;          // is piece deployable using the legacy (old) way
    bool isDeployableBitwise;         // is piece deployable using the bitwise way?
    uint64_t bitwiseBoardTest;        // recalculated locally here to check if it is the same as the
    uint64_t bitwiseBoardPlayer1Test; // bitwise representations calculated moving left, right or downwards

    // recalculate bitwise representations using BitwiseBoardCalculate which goes trhough the whole
    // board to calculate the bitwise representation. Check calculated values against bitwiseBoard
    // and bitwiseBoardPlayer1 which are supposed to have been calculated using Board::BitwiseBoardMoveRight
    // Board::BitwiseBoardMoveLeft or Board::BitwiseBoardMoveDown
    bitwise::BoardCalculate(m_board, thisCoord, m_player1, bitwiseBoardTest, bitwiseBoardPlayer1Test);
    assert(bitwiseBoardTest == bitwiseBoard);
    assert(bitwiseBoardPlayer1Test == bitwiseBoardPlayer1);

    // check if piece is deployable using the legacy old way and bitwise way
    // assert both ways gives same result
    isDeployableLegacy =
        rules::IsPieceDeployableCompute(
                m_board,
                piece,
                thisCoord,
                m_player1);

    isDeployableBitwise =
        bitwise::IsPieceDeployable(
                    bPiece,
                    bitwiseBoard,
                    bitwiseBoardPlayer1);

    //std::cout << isDeployableLegacy << " " << isDeployableBitwise << " "
    //          << std::hex << bPiece << " " << bitwiseBoard << " " << bitwiseBoardPlayer1 << std::endl;
    assert(isDeployableLegacy == isDeployableBitwise);
}

void Game1v1Test::TestBoardAndPiecesBitwise(int32_t a_nUsedPieces, ePieceType_t a_pieceMissing)
{
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << std::endl;
#endif

    // assert params are correct
    if (a_pieceMissing != e_noPiece)
    {
        assert(a_nUsedPieces == 1);
    }

    int32_t nUsedPiecesChecked = 0;

    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        if (!m_player1.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            // this is the only piece that should be missing
            if (a_pieceMissing != e_noPiece)
            {
                assert(a_pieceMissing == static_cast<ePieceType_t>(i));
            }
            nUsedPiecesChecked++;
            continue;
        }

        const std::list<uint64_t> &pieceConfigurations = m_player1.m_pieces[i].GetBitwiseList();
        std::list<uint64_t>::const_iterator it = pieceConfigurations.begin();

        // retrieve legacy precalculated coords
        const std::list<PieceConfiguration> &pieceConfList =
            m_player1.m_pieces[i].GetPrecalculatedConfs();

        std::list<PieceConfiguration>::const_iterator pieceConfIt;
        for (pieceConfIt  = pieceConfList.begin();
             pieceConfIt != pieceConfList.end();
             pieceConfIt++)
        {
            // current coordinate being studied
            Coordinate thisCoord(0, 0);

            // bitwise representation of the board. Coord is empty or not
            uint64_t bitwiseBoard;
            // bitwise representation of the board. Coord is occuppied by player1
            uint64_t bitwiseBoardPlayer1;
            bitwise::BoardCalculate(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
            // bitwise representation of current piece
            uint64_t bPiece = (*it);

            // the following loop goes trough the board doing a S-like movement.
            // it goes from left to right, then down, then right to left, then down,
            // then left to right...
            while (true)
            {
                for (thisCoord.m_col = 0;
                     thisCoord.m_col < (m_board.GetNColumns() - 1);
                     thisCoord.m_col += 1)
                {
                    // check
                    TestBitwiseCheckConfiguration(
                        thisCoord,
                        *pieceConfIt,
                        bPiece,
                        bitwiseBoard,
                        bitwiseBoardPlayer1);

                    bitwise::BoardMoveRight(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                }
                // latest configuration wasn't checked
                // check
                TestBitwiseCheckConfiguration(
                    thisCoord,
                    *pieceConfIt,
                    bPiece,
                    bitwiseBoard,
                    bitwiseBoardPlayer1);

                if ((thisCoord.m_row + 1) >= m_board.GetNRows())
                {
                    break; // got to the latest row of the board
                }

                // next row
                bitwise::BoardMoveDown(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                thisCoord.m_row++;

                // check moving to the left
                for (thisCoord.m_col = (m_board.GetNColumns() - 1);
                     thisCoord.m_col > 0;
                     thisCoord.m_col -= 1)
                {
                    // check
                    TestBitwiseCheckConfiguration(
                        thisCoord,
                        *pieceConfIt,
                        bPiece,
                        bitwiseBoard,
                        bitwiseBoardPlayer1);

                    bitwise::BoardMoveLeft(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                }
                // latest configuration wasn't checked
                //check
                TestBitwiseCheckConfiguration(
                    thisCoord,
                    *pieceConfIt,
                    bPiece,
                    bitwiseBoard,
                    bitwiseBoardPlayer1);

                if ((thisCoord.m_row + 1) >= m_board.GetNRows())
                {
                    break; // got to the latest row of the board
                }

                // next row
                bitwise::BoardMoveDown(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                thisCoord.m_row++;
            } // while (true)

            // next bitwise representation (rotate right)
            // if it is the latest rotation and the piece can be mirrored
            // it'll represent the piece mirrored
            it++;

        } // for (pieceCoordIt = coordConfList.begin()

        // asserting we checked all possible configurations
        assert(it == pieceConfigurations.end());

    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)

    // assert we checked as many pieces as we were expected to
    assert (nUsedPiecesChecked == a_nUsedPieces);
}
void Game1v1Test::TestBitwise()
{
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << std::endl;
#endif

    // ensure the game1v1 hasn't been modified
    Reset();

    // 0: empty
    // +: player2
    // X: player1
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
    Game1v1::PutDownPiece(m_player1.m_pieces[e_5Piece_Cross], Coordinate(6, 6), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
    Game1v1::PutDownPiece(m_player2.m_pieces[e_5Piece_Cross], Coordinate(4, 4), Game1v1::e_Game1v1Player2);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // neither player1 nor player2 have the cross piece now, it won't be tested in this first stage
    // of the test, but they will be tested a bit later, in the 2 following tests
    TestBoardAndPiecesBitwise(1, e_5Piece_Cross);

    // reset the board to launch next test (where only 2 baby pieces are deployed)
    Reset();

    // 0: empty
    // +: player2
    // X: player1
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
    Game1v1::PutDownPiece(m_player1.m_pieces[e_1Piece_BabyPiece], Coordinate(6, 6), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
    Game1v1::PutDownPiece(m_player2.m_pieces[e_1Piece_BabyPiece], Coordinate(4, 4), Game1v1::e_Game1v1Player2);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // go for the test!
    TestBoardAndPiecesBitwise(1, e_1Piece_BabyPiece);

    // reset the board to launch next test (where only two 3-square triangle pieces are deployed)
    Reset();

    // 0: empty
    // +: player2
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
    m_player1.m_pieces[e_3Piece_Triangle].RotateLeft();
    Game1v1::PutDownPiece(m_player1.m_pieces[e_3Piece_Triangle], Coordinate(6, 6), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
    m_player2.m_pieces[e_3Piece_Triangle].RotateRight();
    Game1v1::PutDownPiece(m_player2.m_pieces[e_3Piece_Triangle], Coordinate(4, 4), Game1v1::e_Game1v1Player2);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // go for the test!
    TestBoardAndPiecesBitwise(1, e_3Piece_Triangle);


    // 0: empty
    // +: player2
    // X: player1
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
    Game1v1::PutDownPiece(m_player1.m_pieces[e_5Piece_MrT], Coordinate(4, 7), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
    m_player2.m_pieces[e_5Piece_MrT].RotateRight();
    m_player2.m_pieces[e_5Piece_MrT].RotateRight();
    Game1v1::PutDownPiece(m_player2.m_pieces[e_5Piece_MrT], Coordinate(6, 3), Game1v1::e_Game1v1Player2);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // go for the test! (2 pieces were deployed on the board before launching the test)
    TestBoardAndPiecesBitwise(2);


    // 0: empty
    // +: player2
    // X: player1
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
    Game1v1::PutDownPiece(m_player1.m_pieces[e_5Piece_BigS], Coordinate(1, 10), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // go for the test! (3 pieces were deployed on the board by m_player1 before launching the test)
    TestBoardAndPiecesBitwise(3);
}

void Game1v1Test::TestNKSpiralAlgorithm(eGame1v1Player_t who)
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

        // easy to check now if CalculateNextValidCoordInNucleationPoint works as
        // CalculateValidCoordsInNucleationPoint does
        TestCalculateValidCoordsInNKPoint(who, tmpCoord);

        // assert any other nk point is found by the spiral algorithm
        for (int32_t i = 1; i < GetPlayer(who).NumberOfNucleationPoints(); i++)
        {
            assert(
                GetPlayer(who).GetNextNucleationPointSpiral(iterator, tmpCoord));

            STLCoordinateSet_t::iterator setIt = coordSet.find(tmpCoord);
            assert(setIt != coordSet.end());
            assert(coordSetCustom.isPresent(tmpCoord));

            // easy to check now if CalculateNextValidCoordInNucleationPoint works as
            // CalculateValidCoordsInNucleationPoint does
            TestCalculateValidCoordsInNKPoint(who, tmpCoord);
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

void Game1v1Test::TestCalculateValidCoordsInNKPoint(eGame1v1Player_t who, const Coordinate &where)
{
    assert(GetPlayer(who).IsNucleationPoint(where));
    assert(GetBoard().IsCoordEmpty(where));

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
                                    GetBoard(),
                                    GetPlayer(who),
                                    where,
                                    tmpPiece.GetCurrentConfiguration(),
                                    validCoordVector);

        validCoord.m_row = validCoord.m_col = COORD_UNINITIALISED;
        nNKPointsSecondMethod = 0;
        while(rules::CalculateNextValidCoordInNucleationPoint(
                                    GetBoard(),
                                    GetPlayer(who),
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
