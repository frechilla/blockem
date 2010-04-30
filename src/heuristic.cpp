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
#include "heuristic.h"
#include "player.h"
#include "rules.h"


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

int32_t Heuristic::CalculatePiecesPerNKPoint(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t rv = 0;

    int32_t nSquaresCanDeployMe = 0;
    int32_t nSquaresCanDeployOpponent = 0;
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;

    // current coordiante being studied
    Coordinate thisCoord(0, 0);
    uint64_t bitwiseBoard;   // place in the board is empty or not
    uint64_t bitwiseBoardMe; // place in the board is occupied by player me
    a_board.BitwiseBoardCalculate(thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);

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

            a_board.BitwiseBoardMoveRight(thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
        }
        // latest configuration wasn't checked
        nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
                                    a_board,
                                    a_playerMe,
                                    bitwiseBoard,
                                    bitwiseBoardMe);

        thisCoord.m_row++;
        if (thisCoord.m_row >= a_board.GetNRows())
        {
            break; // got to the latest row of the board
        }

        // next row
        a_board.BitwiseBoardMoveDown(thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);

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

            a_board.BitwiseBoardMoveLeft(thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
        }
        // latest configuration wasn't checked
        nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
                                    a_board,
                                    a_playerMe,
                                    bitwiseBoard,
                                    bitwiseBoardMe);

        thisCoord.m_row++;
        if (thisCoord.m_row >= a_board.GetNRows())
        {
            break; // got to the latest row of the board
        }

        // next row
        a_board.BitwiseBoardMoveDown(thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
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

    rv += (squaresMe * 4);
    rv += (nSquaresCanDeployMe);
    rv -= (squaresOpponent);
    rv -= (nSquaresCanDeployOpponent * 4);

    return rv;
}

int32_t Heuristic::CalculateCircularWeight(
		const Board &a_board, int32_t a_row, int32_t a_column)
{
#ifdef DEBUG
    // this function has been modified to use a static
    // 14x14 array for performance reasons. if the size of the
    // board is not 14 it won't work properly. Use the code
    // between #if 0 ... #endif at the end of this very same function
    // if board is not expected to be 14x14
    assert(a_board.GetNRows() == 14);
    assert(a_board.GetNColumns() == 14);
#endif
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

    return static_cast<int32_t>(s_circularWeightValues[a_row][a_column]);

#if 0
    //WARNING this is the code for any board size. It has been replaced
    // by specific code for 14x14 boards since it is the only board
    // available, but it might be needed in the future
    int32_t tmpValue =
        std::max(
                abs(a_row + 1    - (a_board.GetNRows() / 2)),
                abs(a_column + 1 - (a_board.GetNColumns() / 2)) );

    int32_t weightedValue = ( ( (a_board.GetNRows() / 2) - tmpValue ) / 2) + 1;

    return weightedValue;
#endif
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
            // does the piece fit in the board?
            if ( (bPiece ^ (bPiece & a_bitwiseBoard)) == bPiece )
            {
                // there's space for this piece in the board. Check if this
                // place is valid

                // does the piece touch the side of another a_player's piece?
                if ( (((bPiece << 7) ^ ((bPiece << 7) & a_bitwisePlayerBoard)) == (bPiece << 7)) && // move down
                     (((bPiece >> 7) ^ ((bPiece >> 7) & a_bitwisePlayerBoard)) == (bPiece >> 7)) && // move up
                     (((bPiece << 1) ^ ((bPiece << 1) & a_bitwisePlayerBoard)) == (bPiece << 1)) && // move right
                     (((bPiece >> 1) ^ ((bPiece >> 1) & a_bitwisePlayerBoard)) == (bPiece >> 1)) )  // move left
                {
                    // does it touch a corner (aka nk point)?
                    if ( (((bPiece << 8) ^ ((bPiece << 8) & a_bitwisePlayerBoard)) == (bPiece << 8)) || // down-right corner
                         (((bPiece << 6) ^ ((bPiece << 6) & a_bitwisePlayerBoard)) == (bPiece << 6)) || // down-left corner
                         (((bPiece >> 8) ^ ((bPiece >> 8) & a_bitwisePlayerBoard)) == (bPiece >> 8)) || // up-right corner
                         (((bPiece >> 6) ^ ((bPiece >> 6) & a_bitwisePlayerBoard)) == (bPiece >> 6)) )  // up-left corner
                    {
                        rValue += a_player.m_pieces[i].GetNSquares();
                        break; // exit the ineer most while. This piece can be put down here
                    }
                }
            }
            it++;
        }
    }

    return rValue;
}
