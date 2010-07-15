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
/// @file  game1v1_test.h
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

#ifndef GAME1V1TEST_H_
#define GAME1V1TEST_H_

#include "game1v1.h"
#include "coordinate.h"

/// @brief regression testing for the Game1v1
class Game1v1Test :
    public Game1v1
{
public:
    Game1v1Test();
    virtual ~Game1v1Test();

    /// launches the Game1v1 test!
    void DoTest();
    
private:
    /// tests if the (faster) way Game1v1 puts down and removes pieces
    /// has the same results as the basic methid that consists on setting
    /// a piece on the board coordinate by coordinate and then cumputing
    /// every coordinate of the board looking for nk points. Both boards
    /// (the game1v1 one and the special for test) have to have the same
    /// nk points in the same places
    void TestGame1v1PutDownAndRemovePiece();
    
    /// Tests if Game1v1::GetAllNucleationPoints works properly
    void TestGame1v1GetAllNucleationPoints(eGame1v1Player_t who);

    void PutDownPiece(Piece &a_piece)
    {
        Game1v1::PutDownPiece(
            a_piece,
            Coordinate(7, 7),
            Game1v1::e_Game1v1Player1);
    }

    void RemovePiece(Piece &a_piece)
    {
        Game1v1::RemovePiece(
            a_piece,
            Coordinate(7, 7),
            Game1v1::e_Game1v1Player1);
    }

};

#endif /* GAME1V1TEST_H_ */
