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
/// @file  Rules.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 08-Sept-2009 Original development
/// @endhistory
///
// ============================================================================

#include "Rules.h"

Rules::Rules()
{
}

Rules::~Rules()
{
}

bool Rules::IsPieceDeployable(
        const Board      &a_board,
        const Piece      &a_piece,
        const Coordinate &a_coord,
        const Player     &a_player)
{
	bool touchingNKPoint = false;

    for (uint8_t i = 0 ; i < a_piece.GetNSquares() ; i++)
    {
        int32_t row    = a_coord.m_row + a_piece.m_coords[i].m_row;
        int32_t column = a_coord.m_col + a_piece.m_coords[i].m_col;

        if ( (row    < 0) || (row    >= a_board.GetNRows())    ||
             (column < 0) || (column >= a_board.GetNColumns()) ||
             (a_board.IsCoordEmpty(row, column) == false) )
        {
            // this square is out of the board or it's not empty
            return false;
        }

        if ( (row    > 0) && a_board.IsPlayerInCoord(row - 1, column, a_player) )
        {
        	// touching a piece not in a corner
        	return false;
        }

        if ( (column > 0) && a_board.IsPlayerInCoord(row, column - 1, a_player) )
		{
        	// touching a piece not in a corner
        	return false;
		}

        if ( (row < a_board.GetNRows() - 1) &&
                a_board.IsPlayerInCoord(row + 1, column, a_player))
        {
        	// touching a piece not in a corner
        	return false;
        }

		if ( (column < a_board.GetNColumns() - 1) &&
                a_board.IsPlayerInCoord(row, column + 1, a_player) )
        {
			// touching a piece not in a corner
        	return false;
        }

        if ( (touchingNKPoint == false) &&
        	 (IsNucleationPointCompute(a_board, a_player, row, column)) )
        {
        	// the piece will have to be occupying at least one nucleation point to
        	// be deployed in this place. Once it occupies one nk point we don't need to
        	// calculate if it occupies more
        	touchingNKPoint = true;
        }
    }

    return touchingNKPoint;
}

bool Rules::IsPieceDeployableInNKPoint(
        const Board      &a_board,
        const Piece      &a_piece,
        const Coordinate &a_coord,
        const Coordinate &a_nkPoint,
        const Player     &a_player)
{
#ifdef DEBUG
    assert(a_player.IsNucleationPoint(a_nkPoint));
#endif

    bool touchesNKPoint = false;

    for (uint8_t i = 0 ; i < a_piece.GetNSquares() ; i++)
    {
        int32_t row    = a_coord.m_row + a_piece.m_coords[i].m_row;
        int32_t column = a_coord.m_col + a_piece.m_coords[i].m_col;

        if ( (row    < 0) || (row    >= a_board.GetNRows())    ||
             (column < 0) || (column >= a_board.GetNColumns()) ||
             (a_board.IsCoordEmpty(row, column) == false) )
        {
            // this square is out of the board or it's not empty
            return false;
        }

        if ( (column != 0) &&
             (a_board.IsPlayerInCoord(row, column-1, a_player)) )
        {
            // touching a piece not in a corner
            return false;
        }

        if ( (column != (a_board.GetNColumns() - 1)) &&
             (a_board.IsPlayerInCoord(row, column+1, a_player)) )
        {
        	// touching a piece not in a corner
            return false;
        }

        if ( (row != 0) &&
             (a_board.IsPlayerInCoord(row-1, column, a_player)) )
		{
        	// touching a piece not in a corner
			return false;
		}

        if ( (row != (a_board.GetNRows() - 1)) &&
             (a_board.IsPlayerInCoord(row+1, column, a_player)) )
		{
        	// touching a piece not in a corner
			return false;
		}

        if ( (row == a_nkPoint.m_row) && (column == a_nkPoint.m_col) )
        {
        	touchesNKPoint = true;
        }
    }

    return touchesNKPoint;
}

bool Rules::IsPieceDeployableInStartingPoint(
        const Board      &a_board,
        const Piece      &a_piece,
        const Coordinate &a_coord,
        const Coordinate &a_startingPoint)
{
    bool touchesStartingPoint = false;

    for (uint8_t i = 0 ; i < a_piece.GetNSquares() ; i++)
    {
        int32_t row    = a_coord.m_row + a_piece.m_coords[i].m_row;
        int32_t column = a_coord.m_col + a_piece.m_coords[i].m_col;

        if ( (row    < 0) || (row    >= a_board.GetNRows())    ||
             (column < 0) || (column >= a_board.GetNColumns()) ||
             (a_board.IsCoordEmpty(row, column) == false) )
        {
            // this square is out of the board or it's not empty
            return false;
        }

        if ( (row == a_startingPoint.m_row) && (column == a_startingPoint.m_col) )
        {
        	touchesStartingPoint = true;
        }
    }

    return touchesStartingPoint;
}

bool Rules::IsNucleationPointCompute(
        const Board  &a_board,
        const Player &a_player,
        int32_t      a_coordX,
        int32_t      a_coordY)
{
#ifdef DEBUG
    assert(a_coordX >= 0);
    assert(a_coordY >= 0);
    assert(a_coordX < a_board.GetNRows());
    assert(a_coordY < a_board.GetNColumns());
#endif

    if (!a_board.IsCoordEmpty(a_coordX, a_coordY))
    {
        return false;
    }

    bool isNucleationPoint = false;

    if ( (a_coordY != 0) &&
         (a_board.IsPlayerInCoord(a_coordX, a_coordY-1, a_player)) )
    {
        // not a corner
        return false;
    }

    if ( (a_coordY != (a_board.GetNColumns() - 1)) &&
         (a_board.IsPlayerInCoord(a_coordX, a_coordY+1, a_player)) )
    {
        // not a corner
        return false;
    }

    if (a_coordX != 0)
    {
        if ( a_board.IsPlayerInCoord(a_coordX-1, a_coordY, a_player) )
        {
            // not a corner
            return false;
        }

        if ( (a_coordY != 0) &&
             (a_board.IsPlayerInCoord(a_coordX-1, a_coordY-1, a_player)) )
        {
            // nucleation point detected
            isNucleationPoint = true;
        }
        else if ( (a_coordY != (a_board.GetNColumns() - 1)) &&
                  (a_board.IsPlayerInCoord(a_coordX-1, a_coordY+1, a_player)) )
        {
            // nucleation point detected
            isNucleationPoint = true;
        }
    }

    if (a_coordX != (a_board.GetNRows() - 1))
    {
        if ( a_board.IsPlayerInCoord(a_coordX+1, a_coordY, a_player))
        {
            // not a corner
            return false;
        }

        if ( (a_coordY != 0) &&
             (a_board.IsPlayerInCoord(a_coordX+1, a_coordY-1, a_player)) )
        {
            // nucleation point detected
            isNucleationPoint = true;
        }
        else if ( (a_coordY != (a_board.GetNColumns() - 1)) &&
                  (a_board.IsPlayerInCoord(a_coordX+1, a_coordY+1, a_player)) )
        {
            // nucleation point detected
            isNucleationPoint = true;
        }
    }

    return isNucleationPoint;
}

int32_t Rules::CalculateValidCoordsInNucleationPoint(
        const Board      &a_board,
        const Piece      &a_piece,
        const Coordinate &a_nkPointCoord,
        const Player     &a_player,
        Coordinate       out_validCoords[],
        int32_t          a_size)
{
    // It is true that every piece fits in a square 5x5 pieces.
    // But most of the pieces need a smaller square (3x3). The baby piece only needs 1 little square
    // the increment variable will save the amount of squares to be checked. The size of the square
    // will be (increment x 2) + 1

    int16_t increment = static_cast<int16_t>(a_piece.GetSquareSideHalfSize());
    int32_t nValidCoords = 0;

    if (increment > 0)
    {
        for (int16_t i = -increment ; i <= increment ; i++)
        {
            for (int16_t j = -increment ; j <= increment ; j++)
            {
            	Coordinate thisCoord(
            			a_nkPointCoord.m_row + i,
            			a_nkPointCoord.m_col + j);

				if (IsPieceDeployableInNKPoint(
						a_board, a_piece, thisCoord, a_nkPointCoord, a_player))
				{
					if (nValidCoords < a_size)
					{
					    out_validCoords[nValidCoords] = thisCoord;
					}

					nValidCoords++;
				}
            }
        }
    }
    else
    {
        // the baby piece. No need to use IsDeployableInNKPoint because the piece
    	// only has one square. It will only fit in a known nucleation point
#ifdef DEBUG
        assert(a_player.IsNucleationPoint(a_nkPointCoord));
    	assert(0 < a_size);
#endif
		out_validCoords[nValidCoords] = a_nkPointCoord;

		nValidCoords++;
    }

    return nValidCoords;
}

int32_t Rules::CalculateValidCoordsInStartingPoint(
        const Board      &a_board,
        const Piece      &a_piece,
        const Coordinate &a_startingPointCoord,
        const Player     &a_player,
        Coordinate       out_validCoords[],
        int32_t          a_size)
{
    // It is true that every piece fits in a square 5x5 pieces.
    // But most of the pieces need a smaller square (3x3). The baby piece only needs 1 little square
    // the increment variable will save the amount of squares to be checked. The size of the square
    // will be (increment x 2) + 1

    int16_t increment = static_cast<int16_t>(a_piece.GetSquareSideHalfSize());
    int32_t nValidCoords = 0;

    if (increment > 0)
    {
		for (int16_t i = -increment ; i <= increment ; i++)
		{
			for (int16_t j = -increment ; j <= increment ; j++)
			{
				bool validCoord = false;
				for (uint8_t k = 0 ; k < a_piece.GetNSquares() ; k++)
				{
					Coordinate thisCoord(
							a_startingPointCoord.m_row + i + a_piece.m_coords[k].m_row,
							a_startingPointCoord.m_col + j + a_piece.m_coords[k].m_col);

					if ( (thisCoord.m_row < 0) || (thisCoord.m_row >= a_board.GetNRows()) ||
						 (thisCoord.m_col < 0) || (thisCoord.m_col >= a_board.GetNColumns()) )
					{
						validCoord = false;
						break; // exit this fucking loop. The piece can't be place there 'cos it will be out of the board
					}
					else if ( (thisCoord.m_row == a_startingPointCoord.m_row) &&
							  (thisCoord.m_col == a_startingPointCoord.m_col) )
					{
						// this square of this piece would be touching the selected starting point
						validCoord = true;
					}
				}

				if (validCoord)
				{
#ifdef DEBUG
                    assert(a_board.IsCoordEmpty(a_startingPointCoord));
#endif
					// check if there's too many valid coords to be saved in this array
					if (nValidCoords < a_size)
					{
                        out_validCoords[nValidCoords].m_row = a_startingPointCoord.m_row + i;
                        out_validCoords[nValidCoords].m_col = a_startingPointCoord.m_col + j;
					}

					nValidCoords++;
				}
			}
		}
    }
    else
    {
    	// starting with the baby piece. It doesn't seem to be the most sensible thing to do
#ifdef DEBUG
        assert(a_board.IsCoordEmpty(a_startingPointCoord));
    	assert(0 < a_size);
#endif
		out_validCoords[nValidCoords].m_row = a_startingPointCoord.m_row;
		out_validCoords[nValidCoords].m_col = a_startingPointCoord.m_col;

		nValidCoords++;
    }

    return nValidCoords;
}

void Rules::RecalculateNKInAllBoard(
		const Board &a_board,
		Player      &a_playerMe,
		Player      &a_playerOpponent)
{
	for (int32_t i = 0 ; i < a_board.GetNRows(); i++)
	{
		for (int32_t j = 0 ; j < a_board.GetNColumns(); j++)
		{
			if (Rules::IsNucleationPointCompute(a_board, a_playerMe, i, j))
			{
				a_playerMe.SetNucleationPoint(i, j);
			}
			else
			{
				a_playerMe.UnsetNucleationPoint(i, j);
			}

			if (Rules::IsNucleationPointCompute(a_board, a_playerOpponent, i, j))
			{
				a_playerOpponent.SetNucleationPoint(i, j);
			}
			else
			{
				a_playerOpponent.UnsetNucleationPoint(i, j);
			}
		}
	}
}

void Rules::RecalculateNKAroundPiece(
		const Board      &a_board,
        const Piece      &a_piece,
        const Coordinate &a_coord,
        Player           &a_player)
{
    // It is true that every piece fits in a square 5x5 pieces. We've got to check a square 7x7
	// but that 7x7 square is smaller depending on the piece to check
	// the increment variable will save the amount of squares to be checked. The size of the square
	// will be (increment x 2) + 1
    int32_t increment = a_piece.GetSquareSideHalfSize() + 1;

    int32_t startX = std::max(0, (a_coord.m_row - increment));
    int32_t startY = std::max(0, (a_coord.m_col - increment));
    int32_t endX   = std::min(a_coord.m_row + increment, a_board.GetNRows() - 1);
    int32_t endY   = std::min(a_coord.m_col + increment, a_board.GetNColumns() - 1);

    for (int32_t x = startX ; x <= endX; x++)
    {
		for (int32_t y = startY; y <= endY; y++)
		{
			if (Rules::IsNucleationPointCompute(a_board, a_player, x, y))
			{
				a_player.SetNucleationPoint(x, y);
			}
			else
			{
				a_player.UnsetNucleationPoint(x, y);
			}
		}
    }
}


//TODO this might help to know which nk points corresponds to the latest piece
/*
void Rules::GetNKCorrespondingToPiece(
		const Board &a_board,
        const BlokusPiece* a_piece,
        int32_t a_coordX,
        int32_t a_coordY,
        const Player& a_player,
        STLCoordinateSet &a_setOfCoords)
{
	for (uint8_t i = 0 ; i < a_piece->GetNSquares() ; i++)
	{
		int8_t pieceX = a_coordX + a_piece->m_coords[i][0];
		int8_t pieceY = a_coordY + a_piece->m_coords[i][1];

		if ( (pieceX > 0) && (pieceY > 0) )
		{
			if (a_player.IsNucleationPoint(pieceX-1, pieceY-1))
			{

			}
			else
			{

			}
		}

		if ( (pieceX > 0) && (pieceY < (a_board.GetNColumns() - 1)) )
		{
			if (a_player.IsNucleationPoint(pieceX-1, pieceY+1))
			{

			}
			else
			{

			}
		}

		if ( (pieceX < (a_board.GetNRows() - 1)) && (pieceY > 0) )
		{
			if (a_player.IsNucleationPoint(pieceX+1, pieceY-1))
			{

			}
			else
			{

			}
		}

		if ( (pieceX < (a_board.GetNRows() - 1)) &&
			 (pieceY < (a_board.GetNColumns() - 1)) )
		{
			if (a_player.IsNucleationPoint(pieceX+1, pieceY+1))
			{

			}
			else
			{

			}
		}
	} // for (uint8_t i = 0 ; i < a_piece->GetNSquares() ; i++)
}
*/

