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
/// @file  board_test.cpp
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

#include "board_test.h"
#include "player.h"

#define ROWS    14
#define COLUMNS 14

BoardTest::BoardTest()
{
}

BoardTest::~BoardTest()
{
}

void BoardTest::DoTest()
{
    //std::cout << __FUNCTION__ << " Started" << std::endl << std::endl;

    // tests copy constructor and operator=
    TestCopyBoard();
}

void BoardTest::TestCopyBoard()
{
    Player testPlayer(std::string("the test player"), 'X', ROWS, COLUMNS);

    Board b1(5, 5, ' ');
    Board b2(b1);
    Board b3(6, 6, ' ');

    assert(b1.GetNRows() == b2.GetNRows());
    assert(b1.GetNColumns() == b2.GetNColumns());
    assert(b1.GetNRows() != b3.GetNRows());
    assert(b1.GetNColumns() != b3.GetNColumns());

    b1.SetPlayerInCoord(Coordinate(0, 0), testPlayer);
    assert(b2.IsCoordEmpty(Coordinate(0, 0)) != b1.IsCoordEmpty(Coordinate(0, 0)));
    assert(b2.IsPlayerInCoord(Coordinate(0, 0), testPlayer) !=
           b1.IsPlayerInCoord(Coordinate(0, 0), testPlayer) );

    b2.SetPlayerInCoord(Coordinate(0, 0), testPlayer);
    assert(b2.IsCoordEmpty(Coordinate(0, 0)) == b1.IsCoordEmpty(Coordinate(0, 0)));
    assert(b2.IsPlayerInCoord(Coordinate(0, 0), testPlayer) ==
           b1.IsPlayerInCoord(Coordinate(0, 0), testPlayer) );

    // b3 has to be resized
    b3 = b1;
    assert(b1.GetNRows() == b3.GetNRows());
    assert(b1.GetNColumns() == b3.GetNColumns());

    assert(b3.IsCoordEmpty(Coordinate(0, 0)) == b1.IsCoordEmpty(Coordinate(0, 0)));
    assert(b3.IsPlayerInCoord(Coordinate(0, 0), testPlayer) ==
           b1.IsPlayerInCoord(Coordinate(0, 0), testPlayer) );

    b3.BlankCoord(Coordinate(0, 0));
    assert(b3.IsCoordEmpty(Coordinate(0, 0)) != b1.IsCoordEmpty(Coordinate(0, 0)));
    assert(b3.IsPlayerInCoord(Coordinate(0, 0), testPlayer) !=
           b1.IsPlayerInCoord(Coordinate(0, 0), testPlayer) );


    b1 = Board(10, 10, ' ');
    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < b1.GetNRows();thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < b1.GetNColumns(); thisCoord.m_col++)
        {
            assert(b1.IsCoordEmpty(thisCoord));
            b1.SetPlayerInCoord(thisCoord, testPlayer);
        }
    }

    for (thisCoord.m_row = 0; thisCoord.m_row < b1.GetNRows(); thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < b1.GetNColumns(); thisCoord.m_col++)
        {
            assert(!b1.IsCoordEmpty(thisCoord));
        }
    }

    b1.Reset();
    for (thisCoord.m_row = 0; thisCoord.m_row < b1.GetNRows();thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < b1.GetNColumns(); thisCoord.m_col++)
        {
            assert(b1.IsCoordEmpty(thisCoord));
        }
    }
}
