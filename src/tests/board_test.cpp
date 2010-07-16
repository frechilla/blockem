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

    TestCreateBoardFromArray();
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

void BoardTest::TestCreateBoardFromArray()
{
    // allocate array with the board
    char ** m_preloadedBoard = new char* [ROWS];
    for (int32_t i = 0; i < ROWS; i++)
    {
        m_preloadedBoard[i] = new char[COLUMNS];

        for (int32_t j = 0; j < COLUMNS; j++)
        {
            m_preloadedBoard[i][j] = ' '; // ' ' is the empty coordinate char
        }
    }

    Coordinate thisCoord;
    Board testBoard1(ROWS, COLUMNS, ' ', m_preloadedBoard);
    for (thisCoord.m_row = 0; thisCoord.m_row < ROWS; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < COLUMNS; thisCoord.m_col++)
        {
            assert(testBoard1.IsCoordEmpty(thisCoord));
            m_preloadedBoard[thisCoord.m_row][thisCoord.m_col] = 'X'; // any char different from ' ' will do it
        }
    }

    Board testBoard2(ROWS, COLUMNS, ' ', m_preloadedBoard);
    testBoard1 = testBoard2;
    for (thisCoord.m_row = 0; thisCoord.m_row < ROWS; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < COLUMNS; thisCoord.m_col++)
        {
            assert(!testBoard2.IsCoordEmpty(thisCoord));
            assert(!testBoard1.IsCoordEmpty(thisCoord));
            if (thisCoord.m_row & 0x01)
            {
                m_preloadedBoard[thisCoord.m_row][thisCoord.m_col] = 'X';
            }
            else
            {
                m_preloadedBoard[thisCoord.m_row][thisCoord.m_col] = ' ';
            }
        }
    }

    Board testBoard3(ROWS, COLUMNS, ' ', m_preloadedBoard);
    testBoard1 = testBoard2 = testBoard3;
    for (thisCoord.m_row = 0; thisCoord.m_row < ROWS; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < COLUMNS; thisCoord.m_col++)
        {
            if (thisCoord.m_row & 0x01)
            {
                assert(!testBoard3.IsCoordEmpty(thisCoord));
                assert(!testBoard2.IsCoordEmpty(thisCoord));
                assert(!testBoard1.IsCoordEmpty(thisCoord));
            }
            else
            {
                assert(testBoard3.IsCoordEmpty(thisCoord));
                assert(testBoard2.IsCoordEmpty(thisCoord));
                assert(testBoard1.IsCoordEmpty(thisCoord));
            }
        }
    }

    // free heap memory
    for (int32_t i = 0; i < ROWS; i++)
    {
        delete [] m_preloadedBoard[i];
    }
    delete [] m_preloadedBoard;
}
