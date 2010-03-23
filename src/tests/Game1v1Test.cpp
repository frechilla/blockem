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

#include "Game1v1Test.h"
#include <iostream>

Game1v1Test::Game1v1Test() :
    m_1v1Game()
{
}

Game1v1Test::~Game1v1Test()
{
}

void Game1v1Test::TestPieces()
{
    int32_t possibleConfigurations = 0;
    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        do
        {
            do
            {
                possibleConfigurations++;
            } while (m_1v1Game.GetPlayerMe().m_pieces[i].Rotate() > 0);

        } while (m_1v1Game.GetPlayerMe().m_pieces[i].Mirror());

        m_1v1Game.GetPlayerMe().m_pieces[i].Reset();
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
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_1Piece_BabyPiece];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}

void Game1v1Test::TestPiece_2TwoPiece()
{
    // X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_2Piece_TwoPiece];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 1);

    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}

void Game1v1Test::TestPiece_3LongPiece()
{
    // X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_3Piece_LongPiece];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 1);

    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_3Triangle()
{
    // X
    // X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_3Piece_Triangle];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 3);

    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 2);

    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 1);

    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4LongPiece()
{
    // X X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_4Piece_LongPiece];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));

    assert(thisPiece.Rotate() == 1);

    // X
    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4LittleS()
{
    // X X
    //   X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_4Piece_LittleS];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 1);

    //   X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);
    thisPiece.Reset(); // the piece is not symmetrical. It must be reset before mirroring
    assert(thisPiece.Mirror() == true);

    //   X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));

    assert(thisPiece.Rotate() == 1);

    // X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4LittleT()
{
    //   X
    // X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_4Piece_LittleT];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 3);

    // X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 2);

    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 1);

    //   X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4littleL()
{
    // X
    // X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_4Piece_LittleL];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 3);

    // X X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 2);


    // X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 1);

    //   X
    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror());

    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    assert(thisPiece.Rotate() == 3);

	// X X
	//   X
	//   X
	PutDownPiece(thisPiece);
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
	assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

	RemovePiece(thisPiece);
	assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
	assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

	assert(thisPiece.Rotate() == 2);

    //     X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 1);

    // X
    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4FullSquare()
{
    // X X
    // X X

    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_4Piece_FullSquare];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));


    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BigS()
{
    // X X
    //   X
    //   X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_BigS];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 1);

    //     X
    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == true);

    //   X X
    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 1);

    // X
    // X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5SafPiece()
{
    // X X
    //   X X
    //   X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_SafPiece];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 3);

    //     X
    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 2);


    //   X
    // X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 1);

    //   X
    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror());

    //   X
    //   X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 3);

    // X
    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 2);

    //   X X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 1);

    //   X
    // X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5WPiece()
{
    // X X
    //   X X
    //     X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_WPiece];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 3);

    //     X
    //   X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 2);

    // X
    // X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 1);

    //   X X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 7);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);


    thisPiece.Reset();
}


void Game1v1Test::TestPiece_5CuntPiece()
{
    // X   X
    // X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_CuntPiece];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 3);

    // X X
    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 2);

    // X X X
    // X   X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));

    assert(thisPiece.Rotate() == 1);

    // X X
    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BigPenis()
{
    // X X X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_BigPenis];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));

    assert(thisPiece.Rotate() == 1);

    // X
    // X
    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 4);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5Cross()
{
    //   X
    // X X X
    //   X

    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_Cross];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 8);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5HalfSquare()
{
    // X
    // X
    // X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_HalfSquare];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 6));

    assert(thisPiece.Rotate() == 3);

    // X X X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 6));

    assert(thisPiece.Rotate() == 2);

    // X X X
    //     X
    //     X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 8));

    assert(thisPiece.Rotate() == 1);

    //     X
    //     X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BigL()
{
    // X
    // X X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_BigL];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));

    assert(thisPiece.Rotate() == 3);

    // X X
    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));

    assert(thisPiece.Rotate() == 2);


    // X X X X
    //       X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 1);

    //   X
    //   X
    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror());

    // X X X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    assert(thisPiece.Rotate() == 3);

	// X X
	//   X
	//   X
    //   X
	PutDownPiece(thisPiece);
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
	assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));

	RemovePiece(thisPiece);
	assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(9, 7));
	assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 6));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(10, 8));

	assert(thisPiece.Rotate() == 2);

    //       X
    // X X X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 5));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 4));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 1);

    // X
    // X
    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(5, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(4, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5MrT()
{
    // X
    // X X X
    // X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_MrT];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));

    assert(thisPiece.Rotate() == 3);

    // X X X
    //   X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));

    assert(thisPiece.Rotate() == 2);

    //     X
    // X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 1);

    //   X
    //   X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(7, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5SquareAppen()
{
    // X X
    // X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_SquarePlus];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 3);

    // X X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 2);


    // X X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 1);

    //   X
    // X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror());

    // X X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 3);

	// X X
	// X X
	//   X
	PutDownPiece(thisPiece);
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
	assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

	RemovePiece(thisPiece);
	assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
	assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

	assert(thisPiece.Rotate() == 2);

    //   X X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 1);

    // X
    // X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BoringPiece()
{
    //     X
    // X X X X
    Piece &thisPiece = m_1v1Game.GetPlayerMe().m_pieces[e_5Piece_BoringPiece];

    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 6);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 9));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 7));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 10));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 10));

    assert(thisPiece.Rotate() == 3);

    // X X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 2);


    // X X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 1);

    //   X
    // X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror());

    // X X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

    assert(thisPiece.Rotate() == 3);

	// X X
	// X X
	//   X
	PutDownPiece(thisPiece);
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
	assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
	assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
	assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

	RemovePiece(thisPiece);
	assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 6));
	assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
	assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 5));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
	assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 8));

	assert(thisPiece.Rotate() == 2);

    //   X X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 6));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 5));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(8, 9));

    assert(thisPiece.Rotate() == 1);

    // X
    // X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(!m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 5);
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    RemovePiece(thisPiece);
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(7, 8));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(6, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 7));
    assert(m_1v1Game.GetBoard().IsCoordEmpty(8, 8));
    assert(m_1v1Game.GetPlayerMe().NumberOfNucleationPoints() == 0);
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(5, 8));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(6, 9));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 6));
    assert(!m_1v1Game.GetPlayerMe().IsNucleationPoint(9, 9));

    assert(thisPiece.Rotate() == 0);
    assert(thisPiece.Mirror() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5TheUltimate()
{
    // X X
    //   X X X
    int8_t coords[5][2] =
    {
        {0, 0},
        {0, 2},
        {-1, -1},
        {-1, 0},
        {0, 1}
    };

}
