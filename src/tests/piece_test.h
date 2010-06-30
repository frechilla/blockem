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
/// @file  piece_test.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Jun-2010  Copied most of content from game1v1_test
/// @endhistory
///
// ============================================================================

#ifndef PIECETEST_H_
#define PIECETEST_H_

#include "coordinate.h"
#include "board.h"  // need a board and...
#include "player.h" // ...a player to test shape of pieces

/// @brief regression testing for the Piece class
class PieceTest
{
public:
    PieceTest();
    virtual ~PieceTest();
    
    void DoTest();

private:
    Board  m_board;
    Player m_player;
    
    void PutDownPiece(const Piece &a_piece);
    void RemovePiece(const Piece &a_piece);
    
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

#endif // PIECETEST_H_
