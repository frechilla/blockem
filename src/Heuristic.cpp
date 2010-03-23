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
/// @file  Heuristic.cpp
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
#include <iostream>
#include "Heuristic.h"
#include "Player.h"
#include "Rules.h"

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

    for (uint8_t rowCount = 0; rowCount < a_board.GetNRows() ; rowCount++)
    {
        for (uint8_t columnCount = 0; columnCount <  a_board.GetNColumns() ; columnCount++)
        {
            if (a_board.IsPlayerInCoord(rowCount, columnCount, a_playerMe))
            {
                squaresMe++;
            }
            else if (a_board.IsPlayerInCoord(rowCount, columnCount, a_playerOpponent))
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

int32_t Heuristic::CalculateNKWeighted(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t rv = 0;

    int32_t valueNkMe = 0;
    int32_t valueNkOpponent = 0;
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;


    for (uint8_t rowCount = 0; rowCount < a_board.GetNRows() ; rowCount++)
    {
        for (uint8_t columnCount = 0; columnCount < a_board.GetNColumns() ; columnCount++)
        {
            if (a_board.IsCoordEmpty(rowCount, columnCount))
            {
                bool opponentNKPoint = false;
                int32_t weightedValue = 0;

                if (a_playerOpponent.IsNucleationPoint(rowCount, columnCount))
                {
              		weightedValue = std::max(1, CalculateCircularWeight(a_board, rowCount, columnCount)/2);
                    opponentNKPoint = true;

                    valueNkOpponent += weightedValue;
                    /*
                    if (Rules::IsPieceDeployable(a_board, m_FullSquare, rowCount, columnCount, a_playerOpponent) ||
                        Rules::IsPieceDeployable(a_board, m_FullSquare, rowCount - 1, columnCount, a_playerOpponent) ||
                        Rules::IsPieceDeployable(a_board, m_FullSquare, rowCount, columnCount - 1, a_playerOpponent) ||
                        Rules::IsPieceDeployable(a_board, m_FullSquare, rowCount - 1, columnCount - 1, a_playerOpponent) )
                    {
                        valueNkOpponent += weightedValue * 2;
                    }
                    else
                    {
                        valueNkOpponent += weightedValue;
                    }
                    */
                } // if (a_playerOpponent->IsNucleationPoint(rowCount, columnCount))

                if (a_playerMe.IsNucleationPoint(rowCount, columnCount))
                {
                    if (!opponentNKPoint)
                    {
                        weightedValue = std::max(1, CalculateCircularWeight(a_board, rowCount, columnCount)/2);
                    	if (IsPointTouchingPlayer(a_board, rowCount, columnCount, a_playerOpponent))
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

                    /*
                    if (Rules::IsPieceDeployable(a_board, m_FullSquare, rowCount, columnCount, a_playerMe) ||
                        Rules::IsPieceDeployable(a_board, m_FullSquare, rowCount - 1, columnCount, a_playerMe) ||
                        Rules::IsPieceDeployable(a_board, m_FullSquare, rowCount, columnCount - 1, a_playerMe) ||
                        Rules::IsPieceDeployable(a_board, m_FullSquare, rowCount - 1, columnCount - 1, a_playerMe) )
                    {
                        valueNkMe += weightedValue * 2 ;
                    }
                    else
                    {
                        valueNkMe += weightedValue;
                    }
                    */
                } // if (a_playerMe->IsNucleationPoint(rowCount, columnCount))
            }
            else if (a_board.IsPlayerInCoord(rowCount, columnCount, a_playerMe))
            {
                squaresMe++;
            }
            else // if (a_board.IsPlayerInCoord(rowCount, columnCount, a_playeOpponent)
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

int32_t Heuristic::CalculateCircularWeight(
		const Board &a_board, int32_t a_row, int32_t a_column)
{
    int32_t tmpValue =
        std::max(
                abs(a_row + 1    - (a_board.GetNRows() / 2)),
                abs(a_column + 1 - (a_board.GetNColumns() / 2)) );

    int32_t weightedValue = ( ( (a_board.GetNRows() / 2) - tmpValue ) / 2) + 1;

    return weightedValue;
}

bool Heuristic::IsPointTouchingPlayer(
            const Board &a_board,
            int32_t a_row,
            int32_t a_col,
            const Player &a_player)
{
    if ( (a_col > 0) &&
         (a_board.IsPlayerInCoord(a_row, a_col-1, a_player)))
    {
        return true;
    }
    if ( (a_col < a_board.GetNColumns() - 1) &&
         (a_board.IsPlayerInCoord(a_row, a_col+1, a_player)))
    {
        return true;
    }

    if ( (a_row > 0) &&
         (a_board.IsPlayerInCoord(a_row-1, a_col, a_player)) )
    {
        return true;
    }

    if ( (a_row < a_board.GetNRows() - 1) &&
         (a_board.IsPlayerInCoord(a_row+1, a_col, a_player)) )
    {
        return true;
    }

    return false;
}

