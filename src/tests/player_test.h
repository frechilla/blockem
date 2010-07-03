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
/// @file  player_test.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 3-Jul-2010   Original development
/// @endhistory
///
// ============================================================================

#ifndef PLAYER_TEST_H_
#define PLAYER_TEST_H_

#include "coordinate.h"
#include "board.h"  // need a board and...
#include "player.h" // ...a player to test shape of pieces

/// @brief regression testing for the Player class
class PlayerTest
{
public:
    PlayerTest();
    virtual ~PlayerTest();

    void DoTest();

private:
    // we need a board to test players properly
    Board m_board;
    // we'll use this player for testing
    Player m_player;

    /// tests copy constructor and operator=
    void TestCopyPlayer();

    /// test nk points
    void TestNKPoints();

    /// Tests if Get[First|Next]NucleationPointSpiral works properly
    void TestNKSpiralAlgorithm(const Player &a_player);

    // m_player puts a piece on m_board. nk points are updated with IsCoordNKPointCompute
    void PutDownPiece(const Piece &a_piece, const Coordinate &a_coord, Player &a_player);
    // m_player removes a piece from m_board. nk points are updated with IsCoordNKPointCompute
    void RemovePiece(const Piece &a_piece, const Coordinate &a_coord, Player &a_player);

};

#endif // PLAYER_TEST_H_
