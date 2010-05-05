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
/// @file  main_test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 27-Nov-2009  Original development
/// @endhistory
///
// ============================================================================

#include <iostream>
#include "game1v1_test.h"
#include "game_total_allocation.h"

/// @brief a simple test with a well-known configuration
void simpleTest();

int main(int argc, char** argv)
{
#ifdef NDEBUG
    std::cout << "Warning: asserts will fail silently. "
              << "Compile without the -DNDEBUG flag for a proper test."
              << std::endl;
#endif

    // Game1v1 test
    std::cout << "Game1v1 test started... ";
    std::cout.flush();

    Game1v1Test the1v1Test;
    the1v1Test.TestPieces();
    the1v1Test.TestBitwise();

    std::cout << "  [Passed]" << std::endl << std::endl;

    // tests for copy constructor and operator= for classes with dynamic memory
    std::cout << "Board copy constructor and operator= test started... ";
    std::cout.flush();

    Board b1(5, 5, ' ');
    Board b2(b1);
    Board b3(6, 6, ' ');

    b1.SetSquare('1', 0, 0);
    b3 = b1;
    assert(b3.IsCoordEmpty(0, 0) == b1.IsCoordEmpty(0, 0));

    b3.BlankCoord(0, 0);
    assert(b3.IsCoordEmpty(0, 0) != b1.IsCoordEmpty(0, 0));

    std::cout << "  [Passed]" << std::endl << std::endl;

    std::cout << "Player copy constructor and operator= test started... ";
    std::cout.flush();

    Player p1(std::string("tester1"), 'T', 5, 5);
    Player p2(p1);
    Player p3(std::string("tester3"), 'T', 6, 6);
    assert(p2.IsNucleationPoint(0, 0) == p1.IsNucleationPoint(0, 0));

    p2.SetNucleationPoint(0, 0);
    assert(p2.IsNucleationPoint(0, 0) != p1.IsNucleationPoint(0, 0));

    p1 = p2;
    assert(p2.IsNucleationPoint(0, 0) == p1.IsNucleationPoint(0, 0));

    p3 = p1;
    assert(p3.IsNucleationPoint(0, 0) == p1.IsNucleationPoint(0, 0));

    p3.UnsetNucleationPoint(0, 0);
    assert(p3.IsNucleationPoint(0, 0) != p1.IsNucleationPoint(0, 0));

    std::cout << "  [Passed]" << std::endl << std::endl;


    // run a total allocation test
    std::cout << "GameTotalAllocation test started... ";

    GameTotalAllocation totalAllocation1PlayerGame(14, 14);
    totalAllocation1PlayerGame.Solve(Coordinate(8, 8));

    //totalAllocation1PlayerGame.GetBoard().PrintBoard(std::cout);

    std::cout << "  [Passed]" << std::endl << std::endl;


    return 0;
}

