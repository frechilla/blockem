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
/// @endhistory
///
// ============================================================================

#include "game1v1_test.h"
#include <iostream>

Game1v1Test::Game1v1Test() :
    Game1v1()
{
}

Game1v1Test::~Game1v1Test()
{
}

void Game1v1Test::TestBitwise()
{
    // ensure the game1v1 hasn't been modified
    Reset();

    // it'll be used to save if piece is deployable using the legacy (old) way
    bool isDeployableLegacy;
    // it'll be used to save if piece is deployable using the bitwise way
    bool isDeployableBitwise;

    // 0: empty
    // +: player2
    // X: player1
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 0 0 0 0 0
    // 0 0 0 + + + 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 X 0 0 0 0 0 0
    // 0 0 0 0 0 X X X 0 0 0 0 0
    // 0 0 0 0 0 0 X 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    Game1v1::PutDownPiece(m_player1.m_pieces[e_5Piece_Cross], Coordinate(6, 6), Game1v1::e_Game1v1Player1);
    Game1v1::PutDownPiece(m_player2.m_pieces[e_5Piece_Cross], Coordinate(4, 4), Game1v1::e_Game1v1Player2);

    // neither player1 nor player2 have the cross piece now, it won't be tested in this first stage
    // of the test, but they will be tested a bit later (when the only pieces deployed are the
    // baby pieces)

    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        if (!m_player1.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            continue;
        }

        const std::list<uint64_t> &pieceConfigurations = m_player1.m_pieces[i].GetBitwiseList();
        std::list<uint64_t>::const_iterator it = pieceConfigurations.begin();

        do
        {
            int8_t nRotations = 0;
            while(nRotations < m_player1.m_pieces[i].GetNRotations())
            {
                // current coordinate being studied
                Coordinate thisCoord(0, 0);

                uint64_t bitwiseBoard;   // place in the board is empty or not
                uint64_t bitwiseBoardPlayer1; // place in the board is occupied by player me
                m_board.BitwiseBoardCalculate(thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                // bitwise representation of current piece
                uint64_t bPiece = (*it);

                // the following loop goes trough the board doing a S-like movement.
                // it goes from left to right, then down, then right to left, then down,
                // then left to right...
                // checking how many pieces can be put down per position on the board
                while (true)
                {
                    for (thisCoord.m_col = 0;
                         thisCoord.m_col < (m_board.GetNColumns() - 1);
                         thisCoord.m_col += 1)
                    {
                        // check
                        isDeployableLegacy =
                            Rules::IsPieceDeployable(m_board,
                                              m_player1.m_pieces[i],
                                              thisCoord,
                                              m_player1);
                        isDeployableBitwise =
                            Rules::IsDeployableBitwise(bPiece,
                                                bitwiseBoard,
                                                bitwiseBoardPlayer1);
                        std::cout << isDeployableLegacy << " " << isDeployableBitwise << std::endl;
                        assert(isDeployableLegacy == isDeployableBitwise);

                        m_board.BitwiseBoardMoveRight(thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                    }
                    // latest configuration wasn't checked
                    // check
                    isDeployableLegacy =
                        Rules::IsPieceDeployable(m_board,
                                          m_player1.m_pieces[i],
                                          thisCoord,
                                          m_player1);
                    isDeployableBitwise =
                        Rules::IsDeployableBitwise(bPiece,
                                            bitwiseBoard,
                                            bitwiseBoardPlayer1);
                    std::cout << isDeployableLegacy << " " << isDeployableBitwise << std::endl;
                    assert(isDeployableLegacy == isDeployableBitwise);

                    if ((thisCoord.m_row + 1) >= m_board.GetNRows())
                    {
                        break; // got to the latest row of the board
                    }

                    // next row
                    m_board.BitwiseBoardMoveDown(thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                    thisCoord.m_row++;

                    // check moving to the left
                    for (thisCoord.m_col = (m_board.GetNColumns() - 1);
                         thisCoord.m_col > 0;
                         thisCoord.m_col -= 1)
                    {
                        // check
                        isDeployableLegacy =
                            Rules::IsPieceDeployable(m_board,
                                              m_player1.m_pieces[i],
                                              thisCoord,
                                              m_player1);
                        isDeployableBitwise =
                            Rules::IsDeployableBitwise(bPiece,
                                                bitwiseBoard,
                                                bitwiseBoardPlayer1);
                        std::cout << isDeployableLegacy << " " << isDeployableBitwise << std::endl;
                        assert(isDeployableLegacy == isDeployableBitwise);

                        m_board.BitwiseBoardMoveLeft(thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                    }
                    // latest configuration wasn't checked
                    //check
                    isDeployableLegacy =
                        Rules::IsPieceDeployable(m_board,
                                          m_player1.m_pieces[i],
                                          thisCoord,
                                          m_player1);
                    isDeployableBitwise =
                        Rules::IsDeployableBitwise(bPiece,
                                            bitwiseBoard,
                                            bitwiseBoardPlayer1);
                    std::cout << isDeployableLegacy << " " << isDeployableBitwise << std::endl;
                    assert(isDeployableLegacy == isDeployableBitwise);

                    if ((thisCoord.m_row + 1) >= m_board.GetNRows())
                    {
                        break; // got to the latest row of the board
                    }

                    // next row
                    m_board.BitwiseBoardMoveDown(thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                    thisCoord.m_row++;
                }

                // next bitwise representation (rotate right)
                it++;
                // rotate right the old way representation
                nRotations++;
                m_player1.m_pieces[i].RotateRight();

            } // while (nOrigRotations > a_playerMe.m_pieces[i].GetNRotationsRight())

            if ( (m_player1.m_pieces[i].GetType() == e_4Piece_LittleS) &&
                 (m_player1.m_pieces[i].IsMirrored() == false) )
            {
                // For this piece the maximum number or rotations is 2
                // and the piece is not symmetric, the configuration after
                // the 3rd rotation is the same shape as the original, but
                // the coords moved. Reset the piece before mirroring to
                // avoid unexpected results
                //
                // it also happens with 2piece and 4longPiece, but those pieces
                // don't have mirror, so there's no need for this extra check
                m_player1.m_pieces[i].Reset();
            }

            if (m_player1.m_pieces[i].CanMirror())
            {
                // next bitwise representation (mirror)
                it++;
            }

        } while (m_player1.m_pieces[i].MirrorYAxis());

        assert(it == pieceConfigurations.end());

        // leave the piece as it was!!!
        m_player1.m_pieces[i].Reset();

    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)


    // reset the board to launch next test (where only 2 baby pieces are deployed)
    Reset();

    // 0: empty
    // +: player2
    // X: player1
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 X 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0
    Game1v1::PutDownPiece(m_player1.m_pieces[e_1Piece_BabyPiece], Coordinate(6, 6), Game1v1::e_Game1v1Player1);
    Game1v1::PutDownPiece(m_player2.m_pieces[e_1Piece_BabyPiece], Coordinate(4, 4), Game1v1::e_Game1v1Player2);
}

void Game1v1Test::TestPieces()
{
    int32_t possibleConfigurations = 0;

    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        do
        {
            int16_t nRotations = 0;
            while(nRotations < GetPlayer(e_Game1v1Player1).m_pieces[i].GetNRotations())
            {
                m_player1.m_pieces[i].RotateRight();
                possibleConfigurations++;
                nRotations++;
            }
            // reset the amount of rotations to right before mirroring the piece
            nRotations = 0;
        } while (m_player1.m_pieces[i].MirrorYAxis());

        m_player1.m_pieces[i].Reset();
    }

    // this is a magic number, but you've got to trust me it is right
    assert(possibleConfigurations == 91);

    TestPiece_1BabyPiece();
    TestPiece_2TwoPiece();
    TestPiece_3LongPiece();
    TestPiece_3Triangle();
    TestPiece_4LongPiece();
    TestPiece_4LittleS();
    TestPiece_4LittleT();
    TestPiece_4littleL();
    TestPiece_4FullSquare();
    TestPiece_5BigS();
    TestPiece_5SafPiece();
    TestPiece_5WPiece();
    TestPiece_5CuntPiece();
    TestPiece_5BigPenis();
    TestPiece_5Cross();
    TestPiece_5HalfSquare();
    TestPiece_5BigL();
    TestPiece_5MrT();
    TestPiece_5SquareAppen();
    TestPiece_5BoringPiece();
    TestPiece_5TheUltimate();
}

void Game1v1Test::TestPiece_1BabyPiece()
{
    // X
    Piece &thisPiece = m_player1.m_pieces[e_1Piece_BabyPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));


    nRotations++;
    thisPiece.RotateRight();
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}

void Game1v1Test::TestPiece_2TwoPiece()
{
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_2Piece_TwoPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    // X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}

void Game1v1Test::TestPiece_3LongPiece()
{
    // X X X
    Piece &thisPiece = m_player1.m_pieces[e_3Piece_LongPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_3Triangle()
{
    Piece &thisPiece = m_player1.m_pieces[e_3Piece_Triangle];
    int8_t nRotations = 0;

    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4LongPiece()
{
    // X X X X
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_LongPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4LittleS()
{
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_LittleS];
    int8_t nRotations = 0;

    //   X X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    thisPiece.Reset(); // the piece is not symmetrical. It must be reset before mirroring
                       // to ensure is has the same shape once it has been mirrored
    assert(thisPiece.MirrorYAxis() == true);
    nRotations = 0;

    // X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    //   X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4LittleT()
{
    //   X
    // X X X
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_LittleT];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4littleL()
{
    // X X X
    //     X
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_LittleL];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X
    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis());

    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

	// X X
	//   X
	//   X
	PutDownPiece(thisPiece);
	assert(!GetBoard().IsCoordEmpty(7, 7));
	assert(!GetBoard().IsCoordEmpty(6, 7));
	assert(!GetBoard().IsCoordEmpty(6, 6));
	assert(!GetBoard().IsCoordEmpty(8, 7));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

	RemovePiece(thisPiece);
	assert(GetBoard().IsCoordEmpty(7, 7));
	assert(GetBoard().IsCoordEmpty(6, 7));
	assert(GetBoard().IsCoordEmpty(6, 6));
	assert(GetBoard().IsCoordEmpty(8, 7));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //     X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X
    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4FullSquare()
{
    // X X
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_FullSquare];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BigS()
{
    //   X X
    //   X
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_BigS];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);

    // X X
    //   X
    //   X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //     X
    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5SafPiece()
{
    //   X
    //   X X
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_SafPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //   X X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    // X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                   // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);

    //   X
    // X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X
    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X
    //   X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //     X
    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5WPiece()
{
    // X X
    //   X X
    //     X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_WPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //     X
    //   X X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}


void Game1v1Test::TestPiece_5CuntPiece()
{
    // X X X
    // X   X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_CuntPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X
    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X   X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BigPenis()
{
    // X
    // X
    // X
    // X
    // X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_BigPenis];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(5, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 5));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5Cross()
{
    //   X
    // X X X
    //   X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_Cross];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 8);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5HalfSquare()
{
    // X
    // X
    // X X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_HalfSquare];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(5, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(9, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X
    //     X
    //     X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 5));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //     X
    //     X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 5));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BigL()
{
    // X
    // X
    // X
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_BigL];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(5, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

	// X X
	//   X
	//   X
    //   X
	PutDownPiece(thisPiece);
	assert(!GetBoard().IsCoordEmpty(7, 7));
	assert(!GetBoard().IsCoordEmpty(6, 7));
	assert(!GetBoard().IsCoordEmpty(6, 6));
	assert(!GetBoard().IsCoordEmpty(8, 7));
	assert(!GetBoard().IsCoordEmpty(9, 7));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

	RemovePiece(thisPiece);
	assert(GetBoard().IsCoordEmpty(7, 7));
	assert(GetBoard().IsCoordEmpty(6, 7));
	assert(GetBoard().IsCoordEmpty(6, 6));
	assert(GetBoard().IsCoordEmpty(8, 7));
	assert(GetBoard().IsCoordEmpty(9, 7));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //       X
    // X X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 5));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                   // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);

    //   X
    //   X
    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(5, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X
    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X X X
    //       X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 5));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5MrT()
{
    //     X
    // X X X
    //     X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_MrT];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X
    //   X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X X
    //   X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5SquareAppen()
{
    // X X
    // X X
    // X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_SquarePlus];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //   X
    // X X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);


	// X X
	// X X
	//   X
	PutDownPiece(thisPiece);
	assert(!GetBoard().IsCoordEmpty(7, 7));
	assert(!GetBoard().IsCoordEmpty(7, 6));
	assert(!GetBoard().IsCoordEmpty(6, 7));
	assert(!GetBoard().IsCoordEmpty(6, 6));
	assert(!GetBoard().IsCoordEmpty(8, 7));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

	RemovePiece(thisPiece);
	assert(GetBoard().IsCoordEmpty(7, 7));
	assert(GetBoard().IsCoordEmpty(7, 6));
	assert(GetBoard().IsCoordEmpty(6, 7));
	assert(GetBoard().IsCoordEmpty(6, 6));
	assert(GetBoard().IsCoordEmpty(8, 7));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}

void Game1v1Test::TestPiece_5BoringPiece()
{
    //     X
    // X X X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_BoringPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(9, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 5));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    // X X
    //   X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(5, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);


    //   X
    // X X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 5));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

	// X
	// X X
	// X
    // X
	PutDownPiece(thisPiece);
	assert(!GetBoard().IsCoordEmpty(7, 7));
	assert(!GetBoard().IsCoordEmpty(8, 7));
	assert(!GetBoard().IsCoordEmpty(6, 7));
	assert(!GetBoard().IsCoordEmpty(6, 8));
	assert(!GetBoard().IsCoordEmpty(5, 7));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

	RemovePiece(thisPiece);
	assert(GetBoard().IsCoordEmpty(7, 7));
	assert(GetBoard().IsCoordEmpty(8, 7));
	assert(GetBoard().IsCoordEmpty(6, 7));
	assert(GetBoard().IsCoordEmpty(6, 8));
	assert(GetBoard().IsCoordEmpty(5, 7));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    //   X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(9, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5TheUltimate()
{
    //   X
    // X X
    // X
    // X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_TheUltimate];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(9, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X
    //     X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //   X
    //   X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(5, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    //   X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                   // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);

    // X
    // X X
    //   X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(9, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(10, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //     X X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(5, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 4));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(!GetBoard().IsCoordEmpty(5, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(4, 8));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 9));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 7));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 9));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X X X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(!GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 6));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(6, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(7, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(8, 10));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 5));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
