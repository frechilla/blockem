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
///           Faustino Frechilla 28-Sep-2010  Blockem challenge test
/// @endhistory
///
// ============================================================================

#include <iostream>
#include "bitwise_test.h"
#include "blockem_challenge_test.h"
#include "board_test.h"
#include "g_blocking_queue_test.h"
#include "game1v1_test.h"
#include "piece_test.h"
#include "player_test.h"
#include "rules_test.h"

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

    // blocking queue test
    std::cout << "glib blocking queue test started... ";
    std::cout.flush();
    BlockingQueueTest blockingQTest;
    blockingQTest.DoTest();
    std::cout << "  [Passed]" << std::endl << std::endl;

    // board test
    std::cout << "Board test started...";
    std::cout.flush();
    BoardTest boardTest;
    boardTest.DoTest();
    std::cout << "  [Passed]" << std::endl << std::endl;

    // Piece test
    std::cout << "Piece test started...";
    std::cout.flush();
    PieceTest pieceTest;
    pieceTest.DoTest();
    std::cout << "  [Passed]" << std::endl << std::endl;

    // Player test
    std::cout << "Player test started...";
    std::cout.flush();
    PlayerTest playerTest;
    playerTest.DoTest();
    std::cout << "  [Passed]" << std::endl << std::endl;

    // rules test
    std::cout << "Rules test started...";
    std::cout.flush();
    RulesTest rulesTest;
    rulesTest.DoTest();
    std::cout << "  [Passed]" << std::endl << std::endl;

    // bitwise test
    std::cout << "Bitwise test started...";
    std::cout.flush();
    BitwiseTest bitwiseTest;
    bitwiseTest.DoTest();
    std::cout << "  [Passed]" << std::endl << std::endl;

    // Game1v1 test
    std::cout << "Game1v1 test started... ";
    std::cout.flush();
    Game1v1Test the1v1Test;
    the1v1Test.DoTest();
    std::cout << "  [Passed]" << std::endl << std::endl;

    // Blockem challenge test
    std::cout << "Blockem challenge test started... ";
    std::cout.flush();
    BlockemChallengeTest theChallengeTest;
    theChallengeTest.DoTest();
    std::cout << "  [Passed]" << std::endl << std::endl;


    // run a total allocation test
    std::cout << "GameTotalAllocation test started... ";

    // start from anywhere in the board
    GameTotalAllocation totalAllocation1PlayerGame(14, 14, Coordinate());
    assert(totalAllocation1PlayerGame.Solve());
    //totalAllocation1PlayerGame.GetBoard().PrintBoard(std::cout);

    std::cout << "  [Passed]" << std::endl << std::endl;


    return 0;
}

