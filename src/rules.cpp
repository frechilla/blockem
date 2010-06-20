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
///           Faustino Frechilla 16-Jun-2010  Influence area support
/// @endhistory
///
// ============================================================================

#include "rules.h"

static const int32_t INFLUENCE_AREA_AROUND_SQUARE_SIZE = 2;

bool rules::IsCoordTouchingPlayer(
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

bool rules::IsPieceDeployable(
        const Board                &a_board,
        const pieceConfiguration_t &a_pieceConf,
        const Coordinate           &a_coord,
        const Player               &a_player)
{
	bool touchingNKPoint = false;

    for (pieceConfiguration_t::const_iterator it = a_pieceConf.begin();
        it != a_pieceConf.end();
        it++)
    {
        Coordinate currentCoord(
                a_coord.m_row + it->m_row,
                a_coord.m_col + it->m_col);

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

bool rules::IsPieceDeployableInNKPoint(
        const Board                &a_board,
        const pieceConfiguration_t &a_pieceConf,
        const Coordinate           &a_coord,
        const Coordinate           &a_nkPoint,
        const Player               &a_player)
{
#ifdef DEBUG
    assert(a_player.IsNucleationPoint(a_nkPoint));
#endif

    bool touchesNKPoint = false;

    for (pieceConfiguration_t::const_iterator it = a_pieceConf.begin();
        it != a_pieceConf.end();
        it++)
    {
        Coordinate currentCoord(
                a_coord.m_row + it->m_row,
                a_coord.m_col + it->m_col);

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

bool rules::IsPieceDeployableInStartingPoint(
        const Board                &a_board,
        const pieceConfiguration_t &a_pieceConf,
        const Coordinate           &a_coord,
        const Coordinate           &a_startingPoint)
{
    bool touchesStartingPoint = false;

    for (pieceConfiguration_t::const_iterator it = a_pieceConf.begin();
        it != a_pieceConf.end();
        it++)
    {
        Coordinate currentCoord(
                a_coord.m_row + it->m_row,
                a_coord.m_col + it->m_col);

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

bool rules::IsNucleationPointCompute(
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

int32_t rules::CalculateValidCoordsInNucleationPoint(
        const Board                &a_board,
        const Player               &a_player,
        const Coordinate           &a_nkPointCoord,
        const pieceConfiguration_t &a_pieceConf,
        int32_t                     a_pieceRadius,
        std::vector<Coordinate>    &out_validCoords)
{
    // It is true that every piece fits in a square 5x5 pieces.
    // But most of the pieces need a smaller square (3x3). The baby piece only needs 1 little square
    // the increment variable will save the amount of squares to be checked. The size of the square
    // will be (increment x 2) + 1

    int32_t nValidCoords = 0;

    if (a_pieceRadius > 0)
    {
        for (int32_t i = -a_pieceRadius ; i <= a_pieceRadius ; i++)
        {
            for (int32_t j = -a_pieceRadius ; j <= a_pieceRadius ; j++)
            {
            	Coordinate thisCoord(
            			a_nkPointCoord.m_row + i,
            			a_nkPointCoord.m_col + j);

				if (IsPieceDeployableInNKPoint(
						a_board, a_pieceConf, thisCoord, a_nkPointCoord, a_player))
				{
#ifdef DEBUG
				    assert(static_cast<int32_t>(out_validCoords.size()) > nValidCoords);
#endif
                    out_validCoords[nValidCoords] = thisCoord;
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
        assert(static_cast<int32_t>(out_validCoords.size()) > nValidCoords);
#endif
        out_validCoords[nValidCoords] = a_nkPointCoord;
		nValidCoords++;
    }

    return nValidCoords;
}

bool rules::HasValidCoordInNucleationPoint(
        const Board                &a_board,
        const Player               &a_player,
        const Coordinate           &a_nkPointCoord,
        const pieceConfiguration_t &a_pieceConf,
        int32_t                     a_pieceRadius)
{
    if (a_pieceRadius > 0)
    {
        for (int32_t i = -a_pieceRadius ; i <= a_pieceRadius ; i++)
        {
            for (int32_t j = -a_pieceRadius ; j <= a_pieceRadius ; j++)
            {
                Coordinate thisCoord(
                        a_nkPointCoord.m_row + i,
                        a_nkPointCoord.m_col + j);

                if (IsPieceDeployableInNKPoint(
                        a_board, a_pieceConf, thisCoord, a_nkPointCoord, a_player))
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

int32_t rules::CalculateValidCoordsInStartingPoint(
        const Board                &a_board,
        const Player               &a_player,
        const Coordinate           &a_startingPointCoord,
        const pieceConfiguration_t &a_pieceConf,
        int32_t                     a_pieceRadius,
        std::vector<Coordinate>    &out_validCoords)
{
    // It is true that every piece fits in a square 5x5 pieces.
    // But most of the pieces need a smaller square (3x3). The baby piece only needs 1 little square
    // the increment variable will save the amount of squares to be checked. The size of the square
    // will be (increment x 2) + 1

    int32_t nValidCoords = 0;

    if (a_pieceRadius > 0)
    {
		for (int32_t i = -a_pieceRadius ; i <= a_pieceRadius ; i++)
		{
			for (int32_t j = -a_pieceRadius ; j <= a_pieceRadius ; j++)
			{
				bool validCoord = false;
			    for (pieceConfiguration_t::const_iterator it = a_pieceConf.begin();
			        it != a_pieceConf.end();
			        it++)
				{
					Coordinate thisCoord(
							a_startingPointCoord.m_row + i + it->m_row,
							a_startingPointCoord.m_col + j + it->m_col);

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
				} // for (pieceConfiguration_t::iterator it = a_pieceConf.begin();

				if (validCoord && a_board.IsCoordEmpty(a_startingPointCoord))
				{
#ifdef DEBUG
                    assert(static_cast<int32_t>(out_validCoords.size()) > nValidCoords);
#endif
                    out_validCoords[nValidCoords] = Coordinate(
                            a_startingPointCoord.m_row + i,
                            a_startingPointCoord.m_col + j);
                    nValidCoords++;
				}
			}
		}
    }
    else if (a_board.IsCoordEmpty(a_startingPointCoord))
    {
#ifdef DEBUG
        assert(static_cast<int32_t>(out_validCoords.size()) > nValidCoords);
#endif
    	// starting with the baby piece. It doesn't seem to be the most sensible thing to do
        out_validCoords[nValidCoords] = a_startingPointCoord;
		nValidCoords++;
    }

    return nValidCoords;
}

void rules::RecalculateNKAroundCoord(
        const Board      &a_board,
        const Coordinate &a_coord,
        int32_t           a_radiusToCheck,
        Player           &a_player)
{
    int32_t endRow = std::min(a_coord.m_row + a_radiusToCheck, a_board.GetNRows() - 1);
    int32_t endCol = std::min(a_coord.m_col + a_radiusToCheck, a_board.GetNColumns() - 1);

    Coordinate thisCoord(0, 0);
    for (thisCoord.m_row  = std::max(0, (a_coord.m_row - a_radiusToCheck)) ;
         thisCoord.m_row <= endRow;
         thisCoord.m_row++)
    {
		for (thisCoord.m_col  = std::max(0, (a_coord.m_col - a_radiusToCheck));
		     thisCoord.m_col <= endCol;
		     thisCoord.m_col++)
		{
			if (rules::IsNucleationPointCompute(a_board, a_player, thisCoord))
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

void rules::RecalculateInfluenceAreaAroundPiece(
        const Board                &a_board,
        const Coordinate           &a_coord,
        const pieceConfiguration_t &a_pieceConf,
        Player                     &a_player)
{
    int32_t startCol, endCol, startRow, endRow;

    // pieces are described in such a way that the 1st coordinate
    // that describes them is right in the middle, that is,
    // relative (0, 0). Calculate the influence area around
    // a_coord because, which will always be occupied by the relative
    // (0, 0) square of a_pieceConf

    startRow = std::max(0, a_coord.m_row - INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    startCol = std::max(0, a_coord.m_col - INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    endRow = std::min(a_board.GetNRows() - 1,    a_coord.m_row + INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    endCol = std::min(a_board.GetNColumns() - 1, a_coord.m_col + INFLUENCE_AREA_AROUND_SQUARE_SIZE);

    Coordinate tmpCoord;
    for (tmpCoord.m_row  = startRow; tmpCoord.m_row <= endRow; tmpCoord.m_row++)
    {
        for (tmpCoord.m_col  = startCol; tmpCoord.m_col <= endCol; tmpCoord.m_col++)
        {
            if (a_board.IsCoordEmpty(tmpCoord) &&
                (IsCoordTouchingPlayer(a_board, tmpCoord, a_player) == false) )
            {
                a_player.SetInfluencedCoord(tmpCoord);
            }
            else
            {
                a_player.UnsetInfluencedCoord(tmpCoord);
            }
        }
    } // for (tmpCoord.m_row  = a_coord.m_row - INFLUENCE_AREA_AROUND_SQUARE_SIZE

    // now go through the borders of the influence area per each one
    // of squares that make up the piece configuration to check if
    // this influence area should be bigger
    pieceConfiguration_t::const_iterator it = a_pieceConf.begin();
    // 1st square is always relative (0, 0) - read comment above -
    // it has been checked in the previous loop. Also note that
    // incrementing this iterator is always correct because all
    // piece must have at least one coordinate
    it++;
    while (it != a_pieceConf.end())
    {
        // go for the borders on the top and bottom of the influence area
        startCol = std::max(0, a_coord.m_col - INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_col);
        endCol   = std::min(a_board.GetNColumns() - 1,
                            a_coord.m_col + INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_col);

        // upper most row of the influence area. Check it only if the squares to be
        // checked are expected to be outside the "main" influence area checked
        // in the 1st loop of the function
        if (it->m_row < 0)
        {
            tmpCoord.m_row = a_coord.m_row - INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_row;
            if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
            {
                for (tmpCoord.m_col  = startCol;
                     tmpCoord.m_col <= endCol;
                     tmpCoord.m_col++)
                {
                    if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
                    {
                        if (a_board.IsCoordEmpty(tmpCoord) &&
                            (IsCoordTouchingPlayer(a_board, tmpCoord, a_player) == false) )
                        {
                            a_player.SetInfluencedCoord(tmpCoord);
                        }
                        else
                        {
                            a_player.UnsetInfluencedCoord(tmpCoord);
                        }
                    } // if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
                }
            } // if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
        } // if (it->m_row < 0)

        // lowest most row of the influence area
        if (it->m_row > 0)
        {
            tmpCoord.m_row = a_coord.m_row + INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_row;
            if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
            {
                for (tmpCoord.m_col  = startCol;
                     tmpCoord.m_col <= endCol;
                     tmpCoord.m_col++)
                {
                    if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
                    {
                        if (a_board.IsCoordEmpty(tmpCoord) &&
                            (IsCoordTouchingPlayer(a_board, tmpCoord, a_player) == false) )
                        {
                            a_player.SetInfluencedCoord(tmpCoord);
                        }
                        else
                        {
                            a_player.UnsetInfluencedCoord(tmpCoord);
                        }
                    } // if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
                }
            } // if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
        } // if (it->m_row > 0)

        // go for the borders on the sides (thay haven't been checked yet)
        // upper and down most rows have already been checked, but some coordinates
        // could have been discarded because they weren't expected to go outside the
        // the influence area checked in the 1st loop of the function
        startRow = std::max(0, a_coord.m_row - INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_row);
        endRow   = std::min(a_board.GetNRows() - 1,
                            a_coord.m_row + INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_row);

        // left side
        if (it->m_col < 0)
        {
            tmpCoord.m_col = a_coord.m_col - INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_col;
            if ( (tmpCoord.m_col >= 0) && (tmpCoord.m_col < a_board.GetNColumns()) )
            {
                for (tmpCoord.m_row  = startRow;
                     tmpCoord.m_row <= endRow;
                     tmpCoord.m_row++)
                {
                    if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
                    {
                        if (a_board.IsCoordEmpty(tmpCoord) &&
                            (IsCoordTouchingPlayer(a_board, tmpCoord, a_player) == false) )
                        {
                            a_player.SetInfluencedCoord(tmpCoord);
                        }
                        else
                        {
                            a_player.UnsetInfluencedCoord(tmpCoord);
                        }
                    } // if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
                }
            } // if ( (tmpCoord.m_col >= 0) && (tmpCoord.m_col < a_board.GetNColumns()) )
        } // if (it->m_col < 0)

        // right side
        if (it->m_col > 0)
        {
            tmpCoord.m_col = a_coord.m_col + INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_col;
            if ( (tmpCoord.m_col >= 0) && (tmpCoord.m_col < a_board.GetNColumns()) )
            {
                for (tmpCoord.m_row  = startRow;
                     tmpCoord.m_row <= endRow;
                     tmpCoord.m_row++)
                {
                    if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
                    {
                        if (a_board.IsCoordEmpty(tmpCoord) &&
                            (IsCoordTouchingPlayer(a_board, tmpCoord, a_player) == false) )
                        {
                            a_player.SetInfluencedCoord(tmpCoord);
                        }
                        else
                        {
                            a_player.UnsetInfluencedCoord(tmpCoord);
                        }
                    } // if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
                }
            } // if ( (tmpCoord.m_col >= 0) && (tmpCoord.m_col < a_board.GetNColumns()) )
        }// if (it->m_col > 0)

        // go for the next square of the piece!
        it++;
    } // while (it != a_pieceConf.end())

}

bool rules::IsCoordInfluencedByPlayerCompute(
        const Board       &a_board,
        const Coordinate  &a_coord,
        const Player      &a_player)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0 && a_coord.m_col >= 0);
    assert(a_coord.m_row < a_board.GetNRows());
    assert(a_coord.m_col < a_board.GetNColumns());

#endif // DEBUG

    // if a_coord is not empty or is touching a_player cannot be part
    // of the influence area
    if ( (a_board.IsCoordEmpty(a_coord) == false) ||
         IsCoordTouchingPlayer(a_board, a_coord, a_player))
    {
        return false;
    }

    // go through the influence area around a_coord to check if there
    // at least one square taken over by a_player
    int32_t startRow = std::max(0, a_coord.m_row - INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    int32_t startCol = std::max(0, a_coord.m_col - INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    int32_t endRow = std::min(a_board.GetNRows() - 1,    a_coord.m_row + INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    int32_t endCol = std::min(a_board.GetNColumns() - 1, a_coord.m_col + INFLUENCE_AREA_AROUND_SQUARE_SIZE);

    Coordinate tmpCoord;
    for (tmpCoord.m_row  = startRow; tmpCoord.m_row <= endRow; tmpCoord.m_row++)
    {
        for (tmpCoord.m_col  = startCol; tmpCoord.m_col <= endCol; tmpCoord.m_col++)
        {
            if (a_board.IsPlayerInCoord(tmpCoord, a_player))
            {
                // there is another square taken over by this player
                // inside INFLUENCE_AREA_AROUND_SQUARE_SIZE, so this
                // point belongs to his/her influence area
                return true;
            }
        }
    } // for (tmpCoord.m_row  = a_coord.m_row - INFLUENCE_AREA_AROUND_SQUARE_SIZE

    return false;
}

bool rules::CanPlayerGo(const Board &a_board, const Player &a_player)
{
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

        const std::list<pieceConfiguration_t> &coordConfList =
            a_player.m_pieces[i].GetPrecalculatedConfs();
        std::list<pieceConfiguration_t>::const_iterator pieceCoordIt;

        for (pieceCoordIt = coordConfList.begin();
             pieceCoordIt != coordConfList.end();
             pieceCoordIt++)
        {
            if (a_player.NumberOfPiecesAvailable() == e_numberOfPieces)
            {
                std::vector<Coordinate> validCoords(VALID_COORDS_SIZE);

                int32_t nValidCoords = CalculateValidCoordsInStartingPoint(
                                        a_board,
                                        a_player,
                                        a_player.GetStartingCoordinate(),
                                        (*pieceCoordIt),
                                        a_player.m_pieces[i].GetRadius(),
                                        validCoords);

                if (nValidCoords > 0)
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
                // retrieve if there is a valid coord of this piece in the current nk point
                bool nValidCoords = rules::HasValidCoordInNucleationPoint(
                                            a_board,
                                            a_player,
                                            thisNkPoint,
                                            (*pieceCoordIt),
                                            a_player.m_pieces[i].GetRadius());

                if (nValidCoords)
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
void rules::GetNKCorrespondingToPiece(
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

