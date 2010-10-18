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
/// @file  game_total_allocation_test.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 18-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef GAME_TOTAL_ALLOCATION_TEST_H_
#define GAME_TOTAL_ALLOCATION_TEST_H_

#include "game_total_allocation.h"
#include "coordinate.h"

/// @brief regression testing for the Game1v1
class GameTotalAllocationTest :
    public GameTotalAllocation
{
public:
    GameTotalAllocationTest();
    virtual ~GameTotalAllocationTest();

    /// launches the game total allocation test!
    void DoTest();
    
private:

    /// test the solve function in 3 different cases:
    ///   1) Impossible to solve starting from a specific coordinate
    ///   2) Impossible to solve starting from anywhere
    ///   3) Solvable game starting from anywhere
    ///   4) Solvable game starting from a specific coordinate
    void TestSolve();

    /// tests if the (faster) way GameTotalAllocation puts down and removes
    /// pieces has the same results as the basic method that consists on setting
    /// a piece on the board coordinate by coordinate and then cumputing
    /// every coordinate of the board looking for nk points. Both boards
    /// (the gameTotalAllocation one and the special for test) have to have 
    /// the same nk points in the same places
    void TestGameTotalAllocationPutDownAndRemovePiece();

    /// @return true if the game board is empty. False otherwise
    bool IsBoardEmpty();

};

#endif /* GAME_TOTAL_ALLOCATION_TEST_H_ */
