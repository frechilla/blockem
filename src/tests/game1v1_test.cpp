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

void Game1v1Test::TestPieces()
{
    int32_t possibleConfigurations = 0;

#ifdef NDEBUG
    std::cout << "Warning: asserts in Game1v1Test will fail silently. "
              << "Compile without the -DNDEBUG flag for a proper test."
              << std::endl;
#endif

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(6, 6));
    assert(GetPlayer1().IsNucleationPoint(6, 8));
    assert(GetPlayer1().IsNucleationPoint(8, 6));
    assert(GetPlayer1().IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(6, 8));
    assert(!GetPlayer1().IsNucleationPoint(8, 6));
    assert(!GetPlayer1().IsNucleationPoint(8, 8));


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
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(6, 6));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 6));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 6));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    // X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(6, 6));
    assert(GetPlayer1().IsNucleationPoint(6, 8));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(6, 8));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 8));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 8));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 8));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 6));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 6));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 6));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(10, 6));
    assert(GetPlayer1().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(10, 6));
    assert(!GetPlayer1().IsNucleationPoint(10, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(6, 6));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 8));
    assert(GetPlayer1().IsNucleationPoint(9, 5));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(6, 8));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(6, 8));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 8));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 8));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(6, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));

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
	assert(GetPlayer1().NumberOfNucleationPoints() == 5);
	assert(GetPlayer1().IsNucleationPoint(5, 5));
	assert(GetPlayer1().IsNucleationPoint(5, 8));
	assert(GetPlayer1().IsNucleationPoint(7, 5));
	assert(GetPlayer1().IsNucleationPoint(9, 6));
	assert(GetPlayer1().IsNucleationPoint(9, 8));

	RemovePiece(thisPiece);
	assert(GetBoard().IsCoordEmpty(7, 7));
	assert(GetBoard().IsCoordEmpty(6, 7));
	assert(GetBoard().IsCoordEmpty(6, 6));
	assert(GetBoard().IsCoordEmpty(8, 7));
	assert(GetPlayer1().NumberOfNucleationPoints() == 0);
	assert(!GetPlayer1().IsNucleationPoint(5, 5));
	assert(!GetPlayer1().IsNucleationPoint(5, 8));
	assert(!GetPlayer1().IsNucleationPoint(7, 5));
	assert(!GetPlayer1().IsNucleationPoint(9, 6));
	assert(!GetPlayer1().IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //     X
    // X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(6, 6));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 6));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(6, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(6, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 7);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 8));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 8));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(4, 6));
    assert(GetPlayer1().IsNucleationPoint(4, 8));
    assert(GetPlayer1().IsNucleationPoint(10, 6));
    assert(GetPlayer1().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(4, 6));
    assert(!GetPlayer1().IsNucleationPoint(4, 8));
    assert(!GetPlayer1().IsNucleationPoint(10, 6));
    assert(!GetPlayer1().IsNucleationPoint(10, 8));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X X X
    PutDownPiece(thisPiece);
    assert(!GetBoard().IsCoordEmpty(7, 5));
    assert(!GetBoard().IsCoordEmpty(7, 6));
    assert(!GetBoard().IsCoordEmpty(7, 7));
    assert(!GetBoard().IsCoordEmpty(7, 8));
    assert(!GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer1().NumberOfNucleationPoints() == 4);
    assert(GetPlayer1().IsNucleationPoint(6, 4));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(8, 4));
    assert(GetPlayer1().IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 4));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(8, 4));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 8);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(4, 6));
    assert(GetPlayer1().IsNucleationPoint(4, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(8, 10));
    assert(GetPlayer1().IsNucleationPoint(8, 6));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(4, 6));
    assert(!GetPlayer1().IsNucleationPoint(4, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));
    assert(!GetPlayer1().IsNucleationPoint(8, 6));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(10, 6));
    assert(GetPlayer1().IsNucleationPoint(10, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(8, 10));
    assert(GetPlayer1().IsNucleationPoint(6, 6));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(10, 6));
    assert(!GetPlayer1().IsNucleationPoint(10, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));
    assert(!GetPlayer1().IsNucleationPoint(6, 6));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 4));
    assert(GetPlayer1().IsNucleationPoint(8, 4));
    assert(GetPlayer1().IsNucleationPoint(10, 6));
    assert(GetPlayer1().IsNucleationPoint(10, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 4));
    assert(!GetPlayer1().IsNucleationPoint(8, 4));
    assert(!GetPlayer1().IsNucleationPoint(10, 6));
    assert(!GetPlayer1().IsNucleationPoint(10, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 4));
    assert(GetPlayer1().IsNucleationPoint(8, 4));
    assert(GetPlayer1().IsNucleationPoint(4, 6));
    assert(GetPlayer1().IsNucleationPoint(4, 8));
    assert(GetPlayer1().IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 4));
    assert(!GetPlayer1().IsNucleationPoint(8, 4));
    assert(!GetPlayer1().IsNucleationPoint(4, 6));
    assert(!GetPlayer1().IsNucleationPoint(4, 8));
    assert(!GetPlayer1().IsNucleationPoint(8, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(4, 6));
    assert(GetPlayer1().IsNucleationPoint(4, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(4, 6));
    assert(!GetPlayer1().IsNucleationPoint(4, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(8, 10));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));

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
	assert(GetPlayer1().NumberOfNucleationPoints() == 5);
	assert(GetPlayer1().IsNucleationPoint(5, 5));
	assert(GetPlayer1().IsNucleationPoint(5, 8));
	assert(GetPlayer1().IsNucleationPoint(7, 5));
	assert(GetPlayer1().IsNucleationPoint(10, 6));
	assert(GetPlayer1().IsNucleationPoint(10, 8));

	RemovePiece(thisPiece);
	assert(GetBoard().IsCoordEmpty(7, 7));
	assert(GetBoard().IsCoordEmpty(6, 7));
	assert(GetBoard().IsCoordEmpty(6, 6));
	assert(GetBoard().IsCoordEmpty(8, 7));
	assert(GetBoard().IsCoordEmpty(9, 7));
	assert(GetPlayer1().NumberOfNucleationPoints() == 0);
	assert(!GetPlayer1().IsNucleationPoint(5, 5));
	assert(!GetPlayer1().IsNucleationPoint(5, 8));
	assert(!GetPlayer1().IsNucleationPoint(7, 5));
	assert(!GetPlayer1().IsNucleationPoint(10, 6));
	assert(!GetPlayer1().IsNucleationPoint(10, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 4));
    assert(GetPlayer1().IsNucleationPoint(8, 4));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 4));
    assert(!GetPlayer1().IsNucleationPoint(8, 4));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(4, 6));
    assert(GetPlayer1().IsNucleationPoint(4, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(4, 6));
    assert(!GetPlayer1().IsNucleationPoint(4, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(10, 6));
    assert(GetPlayer1().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(10, 6));
    assert(!GetPlayer1().IsNucleationPoint(10, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 4));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 4));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 4));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 4));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(9, 8));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(9, 8));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

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
	assert(GetPlayer1().NumberOfNucleationPoints() == 5);
	assert(GetPlayer1().IsNucleationPoint(5, 5));
	assert(GetPlayer1().IsNucleationPoint(5, 8));
	assert(GetPlayer1().IsNucleationPoint(8, 5));
	assert(GetPlayer1().IsNucleationPoint(9, 6));
	assert(GetPlayer1().IsNucleationPoint(9, 8));

	RemovePiece(thisPiece);
	assert(GetBoard().IsCoordEmpty(7, 7));
	assert(GetBoard().IsCoordEmpty(7, 6));
	assert(GetBoard().IsCoordEmpty(6, 7));
	assert(GetBoard().IsCoordEmpty(6, 6));
	assert(GetBoard().IsCoordEmpty(8, 7));
	assert(GetPlayer1().NumberOfNucleationPoints() == 0);
	assert(!GetPlayer1().IsNucleationPoint(5, 5));
	assert(!GetPlayer1().IsNucleationPoint(5, 8));
	assert(!GetPlayer1().IsNucleationPoint(8, 5));
	assert(!GetPlayer1().IsNucleationPoint(9, 6));
	assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 5);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 9));
    assert(GetPlayer1().IsNucleationPoint(10, 6));
    assert(GetPlayer1().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));
    assert(!GetPlayer1().IsNucleationPoint(10, 6));
    assert(!GetPlayer1().IsNucleationPoint(10, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(6, 4));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 4));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 4));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 4));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(4, 6));
    assert(GetPlayer1().IsNucleationPoint(4, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(4, 6));
    assert(!GetPlayer1().IsNucleationPoint(4, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(6, 4));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 4));
    assert(GetPlayer1().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(6, 4));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 4));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));

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
	assert(GetPlayer1().NumberOfNucleationPoints() == 6);
	assert(GetPlayer1().IsNucleationPoint(4, 6));
	assert(GetPlayer1().IsNucleationPoint(4, 8));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
	assert(GetPlayer1().IsNucleationPoint(7, 9));
	assert(GetPlayer1().IsNucleationPoint(9, 6));
	assert(GetPlayer1().IsNucleationPoint(9, 8));

	RemovePiece(thisPiece);
	assert(GetBoard().IsCoordEmpty(7, 7));
	assert(GetBoard().IsCoordEmpty(8, 7));
	assert(GetBoard().IsCoordEmpty(6, 7));
	assert(GetBoard().IsCoordEmpty(6, 8));
	assert(GetBoard().IsCoordEmpty(5, 7));
	assert(GetPlayer1().NumberOfNucleationPoints() == 0);
	assert(!GetPlayer1().IsNucleationPoint(4, 6));
	assert(!GetPlayer1().IsNucleationPoint(4, 8));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
	assert(!GetPlayer1().IsNucleationPoint(7, 9));
	assert(!GetPlayer1().IsNucleationPoint(9, 6));
	assert(!GetPlayer1().IsNucleationPoint(9, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 10));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(10, 6));
    assert(GetPlayer1().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(10, 6));
    assert(!GetPlayer1().IsNucleationPoint(10, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 6));
    assert(GetPlayer1().IsNucleationPoint(8, 9));
    assert(GetPlayer1().IsNucleationPoint(10, 6));
    assert(GetPlayer1().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(8, 9));
    assert(!GetPlayer1().IsNucleationPoint(10, 6));
    assert(!GetPlayer1().IsNucleationPoint(10, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(6, 4));
    assert(GetPlayer1().IsNucleationPoint(6, 8));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 4));
    assert(GetPlayer1().IsNucleationPoint(9, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 5));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(6, 8));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 4));
    assert(!GetPlayer1().IsNucleationPoint(9, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(4, 6));
    assert(GetPlayer1().IsNucleationPoint(4, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 8));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 5));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(4, 6));
    assert(!GetPlayer1().IsNucleationPoint(4, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 8));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 6));
    assert(GetPlayer1().IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 6));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 5));
    assert(GetPlayer1().IsNucleationPoint(5, 7));
    assert(GetPlayer1().IsNucleationPoint(6, 8));
    assert(GetPlayer1().IsNucleationPoint(8, 5));
    assert(GetPlayer1().IsNucleationPoint(10, 6));
    assert(GetPlayer1().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(9, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(6, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 5));
    assert(!GetPlayer1().IsNucleationPoint(5, 7));
    assert(!GetPlayer1().IsNucleationPoint(6, 8));
    assert(!GetPlayer1().IsNucleationPoint(8, 5));
    assert(!GetPlayer1().IsNucleationPoint(10, 6));
    assert(!GetPlayer1().IsNucleationPoint(10, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(5, 6));
    assert(GetPlayer1().IsNucleationPoint(5, 9));
    assert(GetPlayer1().IsNucleationPoint(6, 4));
    assert(GetPlayer1().IsNucleationPoint(7, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 4));
    assert(GetPlayer1().IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 6));
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(6, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(5, 6));
    assert(!GetPlayer1().IsNucleationPoint(5, 9));
    assert(!GetPlayer1().IsNucleationPoint(6, 4));
    assert(!GetPlayer1().IsNucleationPoint(7, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 4));
    assert(!GetPlayer1().IsNucleationPoint(8, 8));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(4, 6));
    assert(GetPlayer1().IsNucleationPoint(4, 8));
    assert(GetPlayer1().IsNucleationPoint(6, 9));
    assert(GetPlayer1().IsNucleationPoint(8, 6));
    assert(GetPlayer1().IsNucleationPoint(9, 7));
    assert(GetPlayer1().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(6, 7));
    assert(GetBoard().IsCoordEmpty(5, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(8, 8));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(4, 6));
    assert(!GetPlayer1().IsNucleationPoint(4, 8));
    assert(!GetPlayer1().IsNucleationPoint(6, 9));
    assert(!GetPlayer1().IsNucleationPoint(8, 6));
    assert(!GetPlayer1().IsNucleationPoint(9, 7));
    assert(!GetPlayer1().IsNucleationPoint(9, 9));

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
    assert(GetPlayer1().NumberOfNucleationPoints() == 6);
    assert(GetPlayer1().IsNucleationPoint(6, 6));
    assert(GetPlayer1().IsNucleationPoint(6, 10));
    assert(GetPlayer1().IsNucleationPoint(7, 5));
    assert(GetPlayer1().IsNucleationPoint(8, 10));
    assert(GetPlayer1().IsNucleationPoint(9, 5));
    assert(GetPlayer1().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(GetBoard().IsCoordEmpty(7, 7));
    assert(GetBoard().IsCoordEmpty(7, 8));
    assert(GetBoard().IsCoordEmpty(7, 9));
    assert(GetBoard().IsCoordEmpty(8, 7));
    assert(GetBoard().IsCoordEmpty(8, 6));
    assert(GetPlayer1().NumberOfNucleationPoints() == 0);
    assert(!GetPlayer1().IsNucleationPoint(6, 6));
    assert(!GetPlayer1().IsNucleationPoint(6, 10));
    assert(!GetPlayer1().IsNucleationPoint(7, 5));
    assert(!GetPlayer1().IsNucleationPoint(8, 10));
    assert(!GetPlayer1().IsNucleationPoint(9, 5));
    assert(!GetPlayer1().IsNucleationPoint(9, 8));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
