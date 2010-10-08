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
/// @file  rules_test.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 15-Jul-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef RULES_TEST_H_
#define RULES_TEST_H_

#include "rules.h"

#define RULES_TEST_ROWS    14
#define RULES_TEST_COLUMNS 14

/// @brief regression testing for the rules namespace
class RulesTest
{
public:
    RulesTest();
    virtual ~RulesTest();

    /// launch the test!!
    void DoTest();

private:
    
    /// board loaded with s_preloadedBoard so it can be used as a base for the test
    /// is shouldn't be written to as it can screw someone else's tests
    const Board m_baseBoard;
    
    /// 4 players. Needed for testing
    Player m_player1;
    Player m_player2;
    Player m_player3;
    Player m_player4;
    
    
    /// a preloaded board to perform tests easily
    static char s_preloadedBoardArray[RULES_TEST_ROWS][RULES_TEST_COLUMNS];

    void TestIsCoordTouchingPlayerCompute();
    void TestIsPieceDeployableCompute();
    void TestsNucleationPointCompute();
    void TestIsPieceDeployableInCoord();
    void TestIsPieceDeployableInStartingPoint();
    void TestCalculateValidCoordsInNucleationPoint();
    void TestCalculateNextValidCoordInNucleationPoint();
    void TestHasValidCoordInNucleationPoint();
    void TestCalculateValidCoordsInStartingPoint();
    void TestRecalculateNKAroundCoord();
    void TestRecalculateInfluenceAreaAroundPieceFast();
    void TestIsCoordInfluencedByPlayerFastCompute();
    void TestRecalculateInfluenceAreaInBoard();
    void TestCanPlayerGo();
};

#endif // RULES_TEST_H_
