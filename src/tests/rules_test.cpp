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
/// @file  rules_test.cpp
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

#include "rules_test.h"


// ' ' -> empty char
// 'X' -> player1
// 'O' -> player2
// '+' -> player3
// '=' -> player4
char RulesTest::s_preloadedBoardArray[RULES_TEST_ROWS][RULES_TEST_COLUMNS] =
{
/*         0    1    2    3    4    5    6    7    8    9   10   11   12   13 */
/*  0 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  1 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  2 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  3 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  4 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  5 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  6 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  7 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  8 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  9 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/* 10 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/* 11 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/* 12 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/* 13 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};

RulesTest::RulesTest() :
    m_baseBoard(RULES_TEST_ROWS, RULES_TEST_COLUMNS, ' '),
    m_player1(std::string("player1"), 'X', RULES_TEST_ROWS, RULES_TEST_COLUMNS),
    m_player2(std::string("player2"), 'O', RULES_TEST_ROWS, RULES_TEST_COLUMNS),
    m_player3(std::string("player3"), '+', RULES_TEST_ROWS, RULES_TEST_COLUMNS),
    m_player4(std::string("player4"), '=', RULES_TEST_ROWS, RULES_TEST_COLUMNS)
{
    // this is quite dirty, but it is the fastest way to set
    // m_baseBoard to the values stored in s_preloadedBoardArray
    Board* pBaseBoard = const_cast<Board*> (&m_baseBoard);

    // load s_preloadedBoardArray into m_baseBoard using non-const pointer pBaseBoard
    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < RULES_TEST_ROWS; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < RULES_TEST_COLUMNS; thisCoord.m_col++)
        {
            switch(s_preloadedBoardArray[thisCoord.m_row][thisCoord.m_col])
            {
            case 'X':
                pBaseBoard->SetPlayerInCoord(thisCoord, m_player1);
                break;

            case 'O':
                pBaseBoard->SetPlayerInCoord(thisCoord, m_player2);
                break;

            case '+':
                pBaseBoard->SetPlayerInCoord(thisCoord, m_player3);
                break;

            case '=':
                pBaseBoard->SetPlayerInCoord(thisCoord, m_player4);
                break;

            case ' ':
                pBaseBoard->BlankCoord(thisCoord);
                break;
            default:
                // unexpected char in preloaded board
                assert(0);
            } // switch
        } // for (thisCoord.m_col
    } // for (thisCoord.m_row
}

RulesTest::~RulesTest()
{
}

void RulesTest::DoTest()
{
    // test all functions defined in rules, one by one
    TestIsCoordTouchingPlayerCompute();
    TestIsPieceDeployableCompute();
    TestsNucleationPointCompute();
    TestIsPieceDeployableInNKPoint();
    TestIsPieceDeployableInStartingPoint();
    TestCalculateValidCoordsInNucleationPoint();
    TestCalculateNextValidCoordInNucleationPoint();
    TestHasValidCoordInNucleationPoint();
    TestCalculateValidCoordsInStartingPoint();
    TestRecalculateNKAroundCoord();
    TestRecalculateInfluenceAreaAroundPieceFast();
    TestIsCoordInfluencedByPlayerFastCompute();
    TestRecalculateInfluenceAreaInBoard();
    TestCanPlayerGo();
}

void RulesTest::TestIsCoordTouchingPlayerCompute()
{
}

void RulesTest::TestIsPieceDeployableCompute()
{
}

void RulesTest::TestsNucleationPointCompute()
{
}

void RulesTest::TestIsPieceDeployableInNKPoint()
{
}

void RulesTest::TestIsPieceDeployableInStartingPoint()
{
}

void RulesTest::TestCalculateValidCoordsInNucleationPoint()
{
}

void RulesTest::TestCalculateNextValidCoordInNucleationPoint()
{
}

void RulesTest::TestHasValidCoordInNucleationPoint()
{
}

void RulesTest::TestCalculateValidCoordsInStartingPoint()
{
}

void RulesTest::TestRecalculateNKAroundCoord()
{
}

void RulesTest::TestRecalculateInfluenceAreaAroundPieceFast()
{
}

void RulesTest::TestIsCoordInfluencedByPlayerFastCompute()
{
}

void RulesTest::TestRecalculateInfluenceAreaInBoard()
{
}

void RulesTest::TestCanPlayerGo()
{
}

