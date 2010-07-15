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
/// @file  bitwise_test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 15-Jul-2010  Original development
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif

#include "bitwise_test.h"
#include "bitwise.h"

BitwiseTest::BitwiseTest() :
    Game1v1()
{
}

BitwiseTest::~BitwiseTest()
{
}

void BitwiseTest::TestBitwiseCheckConfiguration(
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

void BitwiseTest::TestBoardAndPiecesBitwise(int32_t a_nUsedPieces, ePieceType_t a_pieceMissing)
{
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
void BitwiseTest::DoTest()
{
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
    Game1v1::PutDownPiece(m_player2.m_pieces[e_5Piece_Cross], Coordinate(4, 4), Game1v1::e_Game1v1Player2);

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
    Game1v1::PutDownPiece(m_player2.m_pieces[e_1Piece_BabyPiece], Coordinate(4, 4), Game1v1::e_Game1v1Player2);

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
    m_player2.m_pieces[e_3Piece_Triangle].RotateRight();
    Game1v1::PutDownPiece(m_player2.m_pieces[e_3Piece_Triangle], Coordinate(4, 4), Game1v1::e_Game1v1Player2);

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
    m_player2.m_pieces[e_5Piece_MrT].RotateRight();
    m_player2.m_pieces[e_5Piece_MrT].RotateRight();
    Game1v1::PutDownPiece(m_player2.m_pieces[e_5Piece_MrT], Coordinate(6, 3), Game1v1::e_Game1v1Player2);

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

    // go for the test! (3 pieces were deployed on the board by m_player1 before launching the test)
    TestBoardAndPiecesBitwise(3);
}
