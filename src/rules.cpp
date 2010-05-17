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
/// @file  rules.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 08-Sept-2009 Original development
/// @endhistory
///
// ============================================================================

#include "rules.h"


Rules::Rules()
{
}

Rules::~Rules()
{
}

bool Rules::IsCoordTouchingPlayer(
            const Board      &a_board,
            const Coordinate &a_coord,
            const Player     &a_player)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_row < a_board.GetNRows());
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_col < a_board.GetNColumns());
#endif

    Coordinate thisCoord(a_coord.m_row - 1, a_coord.m_col);
    if ( (thisCoord.m_row >= 0) &&
          a_board.IsPlayerInCoord(thisCoord, a_player) )
    {
        // touching a a_player's piece
        return true;
    }

    thisCoord.m_row = a_coord.m_row + 1;
    if ( (thisCoord.m_row < a_board.GetNRows()) &&
          a_board.IsPlayerInCoord(thisCoord, a_player))
    {
        // touching a a_player's piece
        return true;
    }

    thisCoord.m_row = a_coord.m_row;
    thisCoord.m_col = a_coord.m_col - 1;
    if ( (thisCoord.m_col >= 0) &&
          a_board.IsPlayerInCoord(thisCoord, a_player) )
    {
        // touching a a_player's piece
        return true;
    }

    thisCoord.m_col = a_coord.m_col + 1;
    if ( (thisCoord.m_col < a_board.GetNColumns()) &&
          a_board.IsPlayerInCoord(thisCoord, a_player) )
    {
        // touching a a_player's piece
        return true;
    }

    return false;
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
        Coordinate currentCoord(
                a_coord.m_row + a_piece.GetCoord(i).m_row,
                a_coord.m_col + a_piece.GetCoord(i).m_col);

        if ( (currentCoord.m_row < 0) || (currentCoord.m_row >= a_board.GetNRows())    ||
             (currentCoord.m_col < 0) || (currentCoord.m_col >= a_board.GetNColumns()) ||
             (a_board.IsCoordEmpty(currentCoord) == false) )
        {
            // this square is out of the board or it's not empty
            return false;
        }

        if (IsCoordTouchingPlayer(a_board, currentCoord, a_player))
        {
            return false;
        }

        //TODO IsNucleationPointCompute performs again same checks done by IsCoordTouchingPlayer
        if ( (touchingNKPoint == false) &&
        	 (IsNucleationPointCompute(a_board, a_player, currentCoord)) )
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
        Coordinate currentCoord(
                a_coord.m_row + a_piece.GetCoord(i).m_row,
                a_coord.m_col + a_piece.GetCoord(i).m_col);

        if ( (currentCoord.m_row < 0) || (currentCoord.m_row >= a_board.GetNRows())    ||
             (currentCoord.m_col < 0) || (currentCoord.m_col >= a_board.GetNColumns()) ||
             (a_board.IsCoordEmpty(currentCoord) == false) )
        {
            // this square is out of the board or it's not empty
            return false;
        }

        if (IsCoordTouchingPlayer(a_board, currentCoord, a_player))
        {
            return false;
        }

        if ( (currentCoord.m_row == a_nkPoint.m_row) &&
             (currentCoord.m_col == a_nkPoint.m_col) )
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
        Coordinate currentCoord(
                a_coord.m_row + a_piece.GetCoord(i).m_row,
                a_coord.m_col + a_piece.GetCoord(i).m_col);

        if ( (currentCoord.m_row < 0) || (currentCoord.m_row >= a_board.GetNRows())    ||
             (currentCoord.m_col < 0) || (currentCoord.m_col >= a_board.GetNColumns()) ||
             (a_board.IsCoordEmpty(currentCoord) == false) )
        {
            // this square is out of the board or it's not empty
            return false;
        }

        if ( (currentCoord.m_row == a_startingPoint.m_row) &&
             (currentCoord.m_col == a_startingPoint.m_col) )
        {
        	touchesStartingPoint = true;
        }
    }

    return touchesStartingPoint;
}

bool Rules::IsNucleationPointCompute(
        const Board      &a_board,
        const Player     &a_player,
        const Coordinate &a_coord)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < a_board.GetNRows());
    assert(a_coord.m_col < a_board.GetNColumns());
#endif

    if (!a_board.IsCoordEmpty(a_coord))
    {
        return false;
    }

    bool isNucleationPoint = false;

    Coordinate thisCoord(a_coord.m_row, a_coord.m_col - 1);
    if ( (thisCoord.m_col >= 0) &&
         (a_board.IsPlayerInCoord(thisCoord, a_player)) )
    {
        // not a corner
        return false;
    }

    thisCoord.m_col = a_coord.m_col + 1;
    if ( (thisCoord.m_col < a_board.GetNColumns()) &&
         (a_board.IsPlayerInCoord(thisCoord, a_player)) )
    {
        // not a corner
        return false;
    }

    if (a_coord.m_row != 0)
    {
        thisCoord.m_row = a_coord.m_row - 1;
        thisCoord.m_col = a_coord.m_col;
        if ( a_board.IsPlayerInCoord(thisCoord, a_player) )
        {
            // not a corner
            return false;
        }

        thisCoord.m_col = a_coord.m_col - 1;
        if ( (thisCoord.m_col >= 0) &&
             (a_board.IsPlayerInCoord(thisCoord, a_player)) )
        {
            // nucleation point detected
            isNucleationPoint = true;
        }
        else
        {
            thisCoord.m_col = a_coord.m_col + 1;
            if ( (thisCoord.m_col < a_board.GetNColumns() ) &&
                 (a_board.IsPlayerInCoord(thisCoord, a_player)) )
            {
                // nucleation point detected
                isNucleationPoint = true;
            }
        }
    }

    if (a_coord.m_row != (a_board.GetNRows() - 1))
    {
        thisCoord.m_row = a_coord.m_row + 1;
        thisCoord.m_col = a_coord.m_col;
        if (a_board.IsPlayerInCoord(thisCoord, a_player))
        {
            // not a corner
            return false;
        }

        thisCoord.m_col = a_coord.m_col - 1;
        if ( (thisCoord.m_col >= 0) &&
             (a_board.IsPlayerInCoord(thisCoord, a_player)) )
        {
            // nucleation point detected
            isNucleationPoint = true;
        }
        else
        {
            thisCoord.m_col = a_coord.m_col + 1;
            if ( (thisCoord.m_col < a_board.GetNColumns()) &&
                 (a_board.IsPlayerInCoord(thisCoord, a_player)) )
            {
                // nucleation point detected
                isNucleationPoint = true;
            }
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

    int32_t increment = static_cast<int32_t>(a_piece.GetSquareSideHalfSize());
    int32_t nValidCoords = 0;

    if (increment > 0)
    {
        for (int32_t i = -increment ; i <= increment ; i++)
        {
            for (int32_t j = -increment ; j <= increment ; j++)
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
#ifdef DEBUG
                    else
                    {
                        assert(0);
                    }
#endif
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

bool Rules::HasValidCoordInNucleationPoint(
        const Board      &a_board,
        const Piece      &a_piece,
        const Coordinate &a_nkPointCoord,
        const Player     &a_player)
{
    // It is true that every piece fits in a square 5x5 pieces.
    // But most of the pieces need a smaller square (3x3). The baby piece only needs 1 little square
    // the increment variable will save the amount of squares to be checked. The size of the square
    // will be (increment x 2) + 1

    int32_t increment = static_cast<int32_t>(a_piece.GetSquareSideHalfSize());

    if (increment > 0)
    {
        for (int32_t i = -increment ; i <= increment ; i++)
        {
            for (int32_t j = -increment ; j <= increment ; j++)
            {
                Coordinate thisCoord(
                        a_nkPointCoord.m_row + i,
                        a_nkPointCoord.m_col + j);

                if (IsPieceDeployableInNKPoint(
                        a_board, a_piece, thisCoord, a_nkPointCoord, a_player))
                {
                    return true;
                }
            }
        }
    }
    else
    {
        // the baby piece. No need to use IsDeployableInNKPoint because the piece
        // only has one square. It will fit in a known nucleation point
#ifdef DEBUG
        assert(a_player.IsNucleationPoint(a_nkPointCoord));
#endif
        return true;
    }

    return false;
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

    int32_t increment = static_cast<int32_t>(a_piece.GetSquareSideHalfSize());
    int32_t nValidCoords = 0;

    if (increment > 0)
    {
		for (int32_t i = -increment ; i <= increment ; i++)
		{
			for (int32_t j = -increment ; j <= increment ; j++)
			{
				bool validCoord = false;
				for (uint8_t k = 0 ; k < a_piece.GetNSquares() ; k++)
				{
					Coordinate thisCoord(
							a_startingPointCoord.m_row + i + a_piece.GetCoord(k).m_row,
							a_startingPointCoord.m_col + j + a_piece.GetCoord(k).m_col);

					if ( (thisCoord.m_row < 0) || (thisCoord.m_row >= a_board.GetNRows())    ||
						 (thisCoord.m_col < 0) || (thisCoord.m_col >= a_board.GetNColumns()) ||
						 (a_board.IsCoordEmpty(thisCoord) == false) )
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

				if (validCoord && a_board.IsCoordEmpty(a_startingPointCoord))
				{
					// check if there's too many valid coords to be saved in this array
					if (nValidCoords < a_size)
					{
                        out_validCoords[nValidCoords].m_row = a_startingPointCoord.m_row + i;
                        out_validCoords[nValidCoords].m_col = a_startingPointCoord.m_col + j;
					}
#ifdef DEBUG
					else
					{
					    assert(0);
					}
#endif
					nValidCoords++;
				}
			}
		}
    }
    else if (a_board.IsCoordEmpty(a_startingPointCoord))
    {
    	// starting with the baby piece. It doesn't seem to be the most sensible thing to do
#ifdef DEBUG
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
    Coordinate thisCoord(0, 0);
	for (thisCoord.m_row = 0 ;
	     thisCoord.m_row < a_board.GetNRows();
	     thisCoord.m_row++)
	{
		for (thisCoord.m_col = 0 ;
		     thisCoord.m_col < a_board.GetNColumns();
		     thisCoord.m_col++)
		{
			if (Rules::IsNucleationPointCompute(a_board, a_playerMe, thisCoord))
			{
				a_playerMe.SetNucleationPoint(thisCoord);
			}
			else
			{
				a_playerMe.UnsetNucleationPoint(thisCoord);
			}

			if (Rules::IsNucleationPointCompute(a_board, a_playerOpponent, thisCoord))
			{
				a_playerOpponent.SetNucleationPoint(thisCoord);
			}
			else
			{
				a_playerOpponent.UnsetNucleationPoint(thisCoord);
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
    int32_t endX   = std::min(a_coord.m_row + increment, a_board.GetNRows() - 1);
    int32_t endY   = std::min(a_coord.m_col + increment, a_board.GetNColumns() - 1);

    Coordinate thisCoord(0, 0);
    for (thisCoord.m_row = std::max(0, (a_coord.m_row - increment)) ;
         thisCoord.m_row <= endX;
         thisCoord.m_row++)
    {
		for (thisCoord.m_col = std::max(0, (a_coord.m_col - increment));
		     thisCoord.m_col <= endY;
		     thisCoord.m_col++)
		{
			if (Rules::IsNucleationPointCompute(a_board, a_player, thisCoord))
			{
				a_player.SetNucleationPoint(thisCoord);
			}
			else
			{
				a_player.UnsetNucleationPoint(thisCoord);
			}
		}
    }
}

bool Rules::CanPlayerGo(const Board &a_board, const Player &a_player)
{
    Coordinate validCoords[VALID_COORDS_SIZE];

    if (a_player.NumberOfPiecesAvailable() == 0)
    {
        return false;
    }

    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        if (a_player.IsPieceAvailable(static_cast<ePieceType_t>(i)) == false)
        {
            continue;
        }

        // copy the piece so the original is not modified
        Piece thisPiece(a_player.m_pieces[i].GetType());

        const std::list< CoordinateArray<PIECE_MAX_SQUARES> > &coordConfList =
            thisPiece.GetPrecalculatedConfs();
        std::list< CoordinateArray<PIECE_MAX_SQUARES> >::const_iterator pieceCoordIt;

        for (pieceCoordIt = coordConfList.begin();
             pieceCoordIt != coordConfList.end();
             pieceCoordIt++)
        {
            // update piece with current precalculated configuration
            thisPiece.SetCurrentCoords(*pieceCoordIt);

            if (a_player.NumberOfPiecesAvailable() == e_numberOfPieces)
            {
                int32_t nValidCoords = CalculateValidCoordsInStartingPoint(
                                        a_board,
                                        thisPiece,
                                        a_player.GetStartingCoordinate(),
                                        a_player,
                                        validCoords,
                                        VALID_COORDS_SIZE);

                if (nValidCoords)
                {
                    return true;
                }
            } // if (a_player.NumberOfPiecesAvailable() == e_numberOfPieces)

            bool nkExists;
            Coordinate thisNkPoint;
            Player::SpiralIterator nkIterator;

            nkExists = a_player.GetFirstNucleationPointSpiral(nkIterator, thisNkPoint);
            while(nkExists)
            {
                // retrieve the valid coords of this piece in the current nk point
                int32_t nValidCoords = Rules::CalculateValidCoordsInNucleationPoint(
                                            a_board,
                                            thisPiece,
                                            thisNkPoint,
                                            a_player,
                                            validCoords,
                                            VALID_COORDS_SIZE);

                if (nValidCoords > 0)
                {
                    return true;
                }

                nkExists = a_player.GetNextNucleationPointSpiral(nkIterator, thisNkPoint);

            } // while(nkExists)
        } // for (pieceCoordIt = coordConfList.begin()
    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)

    return false;
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
		int8_t pieceX = a_coordX + a_piece->GetCoord(i)[0];
		int8_t pieceY = a_coordY + a_piece->GetCoord(i)[1];

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

