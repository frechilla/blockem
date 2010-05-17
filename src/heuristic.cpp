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
/// @file  heuristic.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 31-Mar-2009  Original development
/// @endhistory
///
// ============================================================================

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "heuristic.h"
#include "player.h"
#include "rules.h"
#include "bitwise.h"

// instantiate the const heuristic data array. Heuristics must be defined here
// in the same order they are described in Heuristic::eHeuristicType_t in heuristic.h
const Heuristic::sHeuristicData_t Heuristic::m_heuristicData[e_heuristicCount] =
{
    {e_heuristicNKWeightedv2,
     Heuristic::CalculateNKWeightedv2,
     std::string("NK weighted v2"),
     std::string("Evolution of the old NK weighted heuristic. Nucleation points are picked in the middle of the board at the beginning")
    },
    {e_heuristicNKWeightedv1,
     Heuristic::CalculateNKWeightedv1,
     std::string("NK weighted v1"),
     std::string("The more in the middle of the board a nucleation point is, the better")
    },
    {e_heuristicSimple,
     Heuristic::CalculateSimple,
     std::string("Simple"),
     std::string("Takes into account only the amount of squares of the deployed pieces")
    },
    {e_heuristicRandom,
     Heuristic::CalculateRandom,
     std::string("Random"),
     std::string("Random heuristic. Evaluation function returns a random value so any heuristic can be checked against it")
    }
//    ,{e_heuristicAmountOfPieces,
//     Heuristic::CalculateNPieces,
//     std::string("Amount of pieces"),
//     std::string("The more pieces can be put down, the better")
//    }
};

Heuristic::Heuristic()
{
}

Heuristic::~Heuristic()
{
}

int32_t Heuristic::CalculateSimple(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;
    int32_t rv = 0;

    Coordinate thisCoord(0, 0);
    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col <  a_board.GetNColumns() ; thisCoord.m_col++)
        {
            if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
            {
                squaresMe++;
            }
            else if (a_board.IsPlayerInCoord(thisCoord, a_playerOpponent))
            {
                squaresOpponent++;
            }
        }
    }

    rv += squaresMe*4;
    rv += a_playerMe.NumberOfNucleationPoints()*2;
    rv -= squaresOpponent*4;
    rv -= a_playerOpponent.NumberOfNucleationPoints();

    return rv;
}

int32_t Heuristic::CalculateRandom(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    static bool runOnceFlag = false;

    if (runOnceFlag == false)
    {
        runOnceFlag = true;
        // initialise seed. Not a very random way to initialise the seed
        // but security is not an issue while playing blockem
        srand (time(NULL));
    }

    return rand();
}

int32_t Heuristic::CalculateNKWeightedv1(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t rv = 0;

    int32_t valueNkMe = 0;
    int32_t valueNkOpponent = 0;
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;


    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
        {
            if (a_board.IsCoordEmpty(thisCoord))
            {
                bool opponentNKPoint = false;
                int32_t weightedValue = 0;

                if (a_playerOpponent.IsNucleationPoint(thisCoord))
                {
              		weightedValue = std::max(1, CalculateCircularWeight(a_board, thisCoord)/2);
                    opponentNKPoint = true;

                    valueNkOpponent += weightedValue;
                } // if (a_playerOpponent->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))

                if (a_playerMe.IsNucleationPoint(thisCoord))
                {
                    if (!opponentNKPoint)
                    {
                        weightedValue = std::max(1, CalculateCircularWeight(a_board, thisCoord)/2);
                    	if (Rules::IsCoordTouchingPlayer(a_board, thisCoord, a_playerOpponent))
                    	{
                    	    // an nk point that is touching the other player is unblockable by the opponent
                    	    // (it might get bloked, but not directly)
                    		weightedValue += 1;
                    	}
                    }
                    else
                    {
                        // the weighted value of this nk point is half the value of the opponent
                        // because putting down a piece which is sharing nk points is bad
                        // since the next go will be for the opponent
                        weightedValue /= 2;
                    }

                    valueNkMe += weightedValue;
                } // if (a_playerMe->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))
            }
            else if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
            {
                squaresMe++;
            }
            else // if (a_board.IsPlayerInCoord(thisCoord.m_row, thisCoord.m_col, a_playeOpponent)
            {
                squaresOpponent++;
            }
        }
    }

    rv += (valueNkMe);
    rv += (squaresMe * 4);
    rv -= (valueNkOpponent);
    rv -= (squaresOpponent * 4);

    return rv;
}

int32_t Heuristic::CalculateNKWeightedv2(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t rv = 0;

    int32_t valueNkMe = 0;
    int32_t valueNkOpponent = 0;
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;


    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
        {
            if (a_board.IsCoordEmpty(thisCoord))
            {
                bool opponentNKPoint = false;
                int32_t weightedValue = 0;

                if (a_playerOpponent.IsNucleationPoint(thisCoord))
                {
                    weightedValue = (a_playerOpponent.NumberOfPiecesAvailable() > (e_numberOfPieces - 3)) ?
                                        CalculateCircularWeight(a_board, thisCoord) : 1;
                    opponentNKPoint = true;

                    valueNkOpponent += weightedValue;

                } // if (a_playerOpponent->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))

                if (a_playerMe.IsNucleationPoint(thisCoord))
                {
                    if (!opponentNKPoint)
                    {
                        weightedValue = (a_playerMe.NumberOfPiecesAvailable() > (e_numberOfPieces - 3)) ?
                                            CalculateCircularWeight(a_board, thisCoord) : 1;

                        if (Rules::IsCoordTouchingPlayer(a_board, thisCoord, a_playerOpponent))
                        {
                            // an nk point that is touching the other player is unblockable by the opponent
                            // (it might get blocked, but not directly)
                            weightedValue += 1;
                        }
                    }
                    else
                    {
                        // the weighted value of this nk point is half the value of the opponent
                        // because putting down a piece which is sharing nk points is bad
                        // since the next go will be for the opponent
                        weightedValue = (a_playerMe.NumberOfPiecesAvailable() > (e_numberOfPieces - 3)) ?
                                            (weightedValue / 2) : 0;
                    }

                    valueNkMe += weightedValue;

                } // if (a_playerMe->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))
            }
            else if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
            {
                squaresMe++;
            }
            else // if (a_board.IsPlayerInCoord(thisCoord, a_playeOpponent))
            {
                squaresOpponent++;
            }
        }
    }

    rv += (valueNkMe);
    rv += (squaresMe);
    rv -= (valueNkOpponent);
    rv -= (squaresOpponent);

    return rv;
}

int32_t Heuristic::CalculateNPieces(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t rv = 0;

    int32_t nSquaresCanDeployMe = 0;
    //int32_t nSquaresCanDeployOpponent = 0;
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;

    // current coordiante being studied
    Coordinate thisCoord(0, 0);
    uint64_t bitwiseBoard;   // place in the board is empty or not
    uint64_t bitwiseBoardMe; // place in the board is occupied by player me
    bitwise::BoardCalculate(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);

    // the following loop goes trough the board doing a S-like movement.
    // it goes from left to right, then down, then right to left, then down,
    // then left to right...
    // checking how many pieces can be put down per position on the board
    while (true)
    {
        for (thisCoord.m_col = 0;
             thisCoord.m_col < (a_board.GetNColumns() - 1);
             thisCoord.m_col += 1)
        {
            nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
                                        a_board,
                                        a_playerMe,
                                        bitwiseBoard,
                                        bitwiseBoardMe);

            bitwise::BoardMoveRight(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
        }
        // latest configuration wasn't checked
        nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
                                    a_board,
                                    a_playerMe,
                                    bitwiseBoard,
                                    bitwiseBoardMe);

        if ((thisCoord.m_row + 1) >= a_board.GetNRows())
        {
            break; // got to the latest row of the board
        }

        // next row
        bitwise::BoardMoveDown(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
        thisCoord.m_row++;

        // check moving to the left
        for (thisCoord.m_col = (a_board.GetNColumns() - 1);
             thisCoord.m_col > 0;
             thisCoord.m_col -= 1)
        {
            nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
                                        a_board,
                                        a_playerMe,
                                        bitwiseBoard,
                                        bitwiseBoardMe);

            bitwise::BoardMoveLeft(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
        }
        // latest configuration wasn't checked
        nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
                                    a_board,
                                    a_playerMe,
                                    bitwiseBoard,
                                    bitwiseBoardMe);

        if ((thisCoord.m_row + 1) >= a_board.GetNRows())
        {
            break; // got to the latest row of the board
        }

        // next row
        bitwise::BoardMoveDown(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
        thisCoord.m_row++;
    }

    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        if (!a_playerMe.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresMe += a_playerMe.m_pieces[i].GetNSquares();
        }
        if (!a_playerOpponent.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresOpponent += a_playerOpponent.m_pieces[i].GetNSquares();
        }
    }

    rv += (squaresMe * 10);
    rv += (nSquaresCanDeployMe);
    //rv -= (squaresOpponent);
    //rv -= (nSquaresCanDeployOpponent * 4);

    return rv;
}

int32_t Heuristic::CalculateCircularWeight(
		const Board &a_board, const Coordinate &a_coord)
{
#if 0
//#ifdef DEBUG
    // this piece of code tried to improve performance using
    // a precalculated static 14x14 array.
    // it might not make a lot of sense, but it is slower
    // than the code below, which runs a calculation
    // instead of just accessing an array
    assert(a_board.GetNRows() == 14);
    assert(a_board.GetNColumns() == 14);
//#endif
    // values precalculated for a 14x14 board
    static const int8_t s_circularWeightValues[14][14] =
    {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 4, 4, 4, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 4, 4, 4, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 4, 4, 4, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };

    return static_cast<int32_t>(s_circularWeightValues[a_coord.m_row][a_coord.m_col]);
#endif

    int32_t tmpValue =
        std::max(
                abs(a_coord.m_row + 1 - (a_board.GetNRows() / 2)),
                abs(a_coord.m_col + 1 - (a_board.GetNColumns() / 2)) );

    int32_t weightedValue = ( ( (a_board.GetNRows() / 2) - tmpValue ) / 2) + 1;

    return weightedValue;
}

int32_t Heuristic::CountSquaresCanBeDeployedBitwise(
        const Board  &a_board,
        const Player &a_player,
        uint64_t      a_bitwiseBoard,
        uint64_t      a_bitwisePlayerBoard)
{
    int32_t rValue = 0;

    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        if (!a_player.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            continue;
        }

        const std::list<uint64_t> &pieceConfigurations = a_player.m_pieces[i].GetBitwiseList();
        std::list<uint64_t>::const_iterator it = pieceConfigurations.begin();
        while (it != pieceConfigurations.end())
        {
            uint64_t bPiece = (*it);

            if (bitwise::IsPieceDeployable(bPiece, a_bitwiseBoard, a_bitwisePlayerBoard))
            {
                rValue += a_player.m_pieces[i].GetNSquares();
                break;
            }

            it++;
        }
    }

    return rValue;
}
