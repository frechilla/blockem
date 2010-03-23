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
/// @file  Game1v1Test.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Sep-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef GAME1V1TEST_H_
#define GAME1V1TEST_H_

#include "Game1v1.h"
#include "Coordinate.h"

/// @brief regression testing for the Game1v1
class Game1v1Test
{
public:
    Game1v1Test();
    virtual ~Game1v1Test();

    void TestPieces();

private:
    Game1v1 m_1v1Game;

    void PutDownPiece(Piece &a_piece)
    {
        m_1v1Game.PutDownPieceMe(a_piece, Coordinate(7, 7));
    }

    void RemovePiece(Piece &a_piece)
    {
        m_1v1Game.RemovePieceMe(a_piece, Coordinate(7, 7));
    }

    void TestPiece_1BabyPiece();
    void TestPiece_2TwoPiece();
    void TestPiece_3LongPiece();
    void TestPiece_3Triangle();
    void TestPiece_4LongPiece();
    void TestPiece_4LittleS();
    void TestPiece_4LittleT();
    void TestPiece_4littleL();
    void TestPiece_4FullSquare();
    void TestPiece_5BigS();
    void TestPiece_5SafPiece();
    void TestPiece_5WPiece();
    void TestPiece_5CuntPiece();
    void TestPiece_5BigPenis();
    void TestPiece_5Cross();
    void TestPiece_5HalfSquare();
    void TestPiece_5BigL();
    void TestPiece_5MrT();
    void TestPiece_5SquareAppen();
    void TestPiece_5BoringPiece();
    void TestPiece_5TheUltimate();
};

#endif /* GAME1V1TEST_H_ */
