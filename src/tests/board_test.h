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
/// @file  board_test.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 15-Jul-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef BOARD_TEST_H_
#define BOARD_TEST_H_

#include "board.h"

/// @brief regression testing for the Board class
class BoardTest
{
public:
    BoardTest();
    virtual ~BoardTest();

    /// launch the test!!
    void DoTest();

private:

    /// tests copy constructor and operator=
    void TestCopyBoard();

    /// Test creation of boards loading data from arrays
    void TestCreateBoardFromArray();
};

#endif // BOARD_TEST_H_
