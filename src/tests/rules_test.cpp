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

#define ROWS    14
#define COLUMNS 14

RulesTest::RulesTest()
{
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

