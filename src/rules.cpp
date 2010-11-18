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

#ifdef DEBUG_PRINT
#include <iostream>
#endif

static const int32_t INFLUENCE_AREA_AROUND_SQUARE_SIZE = 2;

bool rules::IsCoordTouchingPlayerCompute(
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

bool rules::IsPieceDeployableCompute(
        const Board              &a_board,
        const PieceConfiguration &a_pieceConf,
        const Coordinate         &a_coord,
        const Player             &a_player)
{
	bool touchingNKPoint = false;

    PieceConfigurationContainer_t::const_iterator it;
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
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

        if ( (touchingNKPoint == false) &&
        	 (IsNucleationPointCompute(a_board, a_player, currentCoord)) )
        {
        	// the piece will have to be occupying at least one nucleation point to
        	// be deployed in this place. Once it occupies one nk point we don't need to
        	// calculate if it occupies more
        	touchingNKPoint = true;
        }
    }

    if (touchingNKPoint == false)
    {
        // if no square of the piece is touching any nk point
        // the piece can't be deployed in a_coord
        return false;
    }

    // check if a_player has already deployed a piece which would be touching
    // the forbidden area of the piece (represented by a_pieceConf)
    for (it  = a_pieceConf.m_forbiddenArea.begin();
         it != a_pieceConf.m_forbiddenArea.end();
         it++)
    {
        Coordinate currentCoord(
                a_coord.m_row + it->m_row,
                a_coord.m_col + it->m_col);

        if ( (currentCoord.m_row >= 0) && (currentCoord.m_col >= 0) &&
             (currentCoord.m_row < a_board.GetNRows())              &&
             (currentCoord.m_col < a_board.GetNColumns())           &&
             (a_board.IsPlayerInCoord(currentCoord, a_player)) )
        {
            // this square is already occupied by a_player. No piece of
            // 'a_player' can already be in the forbidden area of the current
            // piece for this move to be valid
            return false;
        }
    } // for (it  = a_pieceConf.m_forbiddenArea.begin();

    // if it got to here piece can be deployed in a_coord touching at least 1 nk point
    return true;
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

bool rules::IsPieceDeployableInCoord(
        const Board              &a_board,
        const PieceConfiguration &a_pieceConf,
        const Coordinate         &a_coord,
        const Coordinate         &a_mustTouchCoord,
        const Player             &a_player)
{
#ifdef DEBUG
    assert(a_board.IsCoordEmpty(a_mustTouchCoord));
#endif

    bool touchesCoord = false;

    PieceConfigurationContainer_t::const_iterator it;
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
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

        if ( (currentCoord.m_row == a_mustTouchCoord.m_row) &&
             (currentCoord.m_col == a_mustTouchCoord.m_col) )
        {
        	touchesCoord = true;
        }
    } // for (it  = a_pieceConf.m_pieceSquares.begin();

    if (touchesCoord == false)
    {
        // if no square of the piece is touching the nk point
        // the piece can't be deployed in a_coord
        return false;
    }

    // check if a_player has already deployed a piece which would be touching
    // the forbidden area of the piece (represented by a_pieceConf)
    for (it  = a_pieceConf.m_forbiddenArea.begin();
         it != a_pieceConf.m_forbiddenArea.end();
         it++)
    {
        Coordinate currentCoord(
                a_coord.m_row + it->m_row,
                a_coord.m_col + it->m_col);

        if ( (currentCoord.m_row >= 0) && (currentCoord.m_col >= 0) &&
             (currentCoord.m_row < a_board.GetNRows())              &&
             (currentCoord.m_col < a_board.GetNColumns())           &&
             (a_board.IsPlayerInCoord(currentCoord, a_player)) )
        {
            // this square is already occupied by a_player. No piece of
            // 'a_player' can already be in the forbidden area of the current
            // piece for this move to be valid
            return false;
        }
    } // for (it  = a_pieceConf.m_forbiddenArea.begin();

    // if it got to here piece can be deployed in a_coord touching a_nkPoint
    return true;
}

bool rules::IsPieceDeployableInStartingPoint(
        const Board              &a_board,
        const PieceConfiguration &a_pieceConf,
        const Coordinate         &a_coord,
        const Coordinate         &a_startingPoint)
{
    bool touchesStartingPoint = false;

    PieceConfigurationContainer_t::const_iterator it;
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
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

bool rules::CalculateNextValidCoordInNucleationPoint(
        const Board              &a_board,
        const Player             &a_player,
        const Coordinate         &a_nkPointCoord,
        const PieceConfiguration &a_pieceConf,
        int32_t                   a_pieceRadius,
        Coordinate               &in_out_validCoord)
{
#ifdef DEBUG
    assert(rules::IsNucleationPointCompute(a_board, a_player, a_nkPointCoord));
#endif

    if (a_pieceRadius > 0)
    {
        // the baby piece is a special case. This is code for the rest of pieces
        int32_t endRow = std::min(a_nkPointCoord.m_row + a_pieceRadius, a_board.GetNRows() - 1);
        int32_t endCol = std::min(a_nkPointCoord.m_col + a_pieceRadius, a_board.GetNColumns() - 1);
        int32_t startRow = std::max(0, a_nkPointCoord.m_row - a_pieceRadius);
        int32_t startCol = std::max(0, a_nkPointCoord.m_col - a_pieceRadius);

        // thisCoord will point to the first place that will be checked out for valid coords
        // to deploy the piece in a_nkPointCoord
        Coordinate thisCoord(startRow, startCol);
        if (in_out_validCoord.Initialised())
        {
            // we don't have to start from (0, 0)
            // try to start from next column
            thisCoord.m_row = in_out_validCoord.m_row;
            thisCoord.m_col = in_out_validCoord.m_col + 1;
            if (thisCoord.m_col > endCol)
            {
                // we were on the last column. must start from next row
                thisCoord.m_row = in_out_validCoord.m_row + 1;
                thisCoord.m_col = startCol;

                if (thisCoord.m_row > endRow)
                {
                    // got to the end of the board
                    return false;
                }
            }
        }

        // check the actual row saved in thisCoord.m_row first
        while(thisCoord.m_col <= endCol)
        {
            if (IsPieceDeployableInCoord(
                    a_board, a_pieceConf, thisCoord, a_nkPointCoord, a_player))
            {
                in_out_validCoord.m_row = thisCoord.m_row;
                in_out_validCoord.m_col = thisCoord.m_col;
                return true;
            }

            thisCoord.m_col++;
        }

        // now continue with the rest of the loop, starting off the next row
        for (thisCoord.m_row++; thisCoord.m_row <= endRow; thisCoord.m_row++)
        {
            for (thisCoord.m_col = startCol; thisCoord.m_col <= endCol; thisCoord.m_col++)
            {
				if (IsPieceDeployableInCoord(
						a_board, a_pieceConf, thisCoord, a_nkPointCoord, a_player))
				{
                    in_out_validCoord.m_row = thisCoord.m_row;
                    in_out_validCoord.m_col = thisCoord.m_col;
                    return true;
				}
            }
        }
    }
    else
    {
        // Baby piece!. No need to use IsDeployableInNKPoint because the piece
    	// only has one square. It will only fit in 1 known nucleation point
        // we must ensure in_out_validCoord is not initialised or is smaller
        // than a_nkPointCoord because this might be the 2nd time this function
        // is called to retrieve the valid coords of this baby piece
        if ( (!in_out_validCoord.Initialised())               ||
             (in_out_validCoord.m_row < a_nkPointCoord.m_row) ||
             ( (in_out_validCoord.m_row == a_nkPointCoord.m_row) &&
                 (in_out_validCoord.m_col <  a_nkPointCoord.m_col) ) )
        {
            in_out_validCoord.m_row = a_nkPointCoord.m_row;
            in_out_validCoord.m_col = a_nkPointCoord.m_col;
            return true;
        }
    }

    return false;
}

int32_t rules::CalculateValidCoordsInNucleationPoint(
        const Board              &a_board,
        const Player             &a_player,
        const Coordinate         &a_nkPointCoord,
        const PieceConfiguration &a_pieceConf,
        std::vector<Coordinate>  &out_validCoords)
{
#ifdef DEBUG
    assert(rules::IsNucleationPointCompute(a_board, a_player, a_nkPointCoord));
#endif

    int32_t nValidCoords = 0;

    // go through all the squares that make up the piece and try to allocate
    // the piece so those squares touch a_nkPointCoord (the nucleation
    // point described by the user)
    PieceConfigurationContainer_t::const_iterator it;
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(
            a_nkPointCoord.m_row - it->m_row,
            a_nkPointCoord.m_col - it->m_col);

        if (IsPieceDeployableInCoord(
                a_board, a_pieceConf, thisCoord, a_nkPointCoord, a_player))
        {
#ifdef DEBUG
            assert(static_cast<int32_t>(out_validCoords.size()) > nValidCoords);
#endif
            out_validCoords[nValidCoords] = thisCoord;
            nValidCoords++;
        }
    }

    return nValidCoords;
}

bool rules::HasValidCoordInNucleationPoint(
        const Board              &a_board,
        const Player             &a_player,
        const Coordinate         &a_nkPointCoord,
        const PieceConfiguration &a_pieceConf,
        int32_t                   a_pieceRadius)
{
    // create an uninitialised tmp coordinate.
    // If CalculateNextValidCoordInNucleationPoint finds a valid coord
    // (returns true) there is at least a valid coord in this nucleation
    // point
    Coordinate tmpCoord; // uninitialised coord
    return rules::CalculateNextValidCoordInNucleationPoint(
                    a_board,
                    a_player,
                    a_nkPointCoord,
                    a_pieceConf,
                    a_pieceRadius,
                    tmpCoord);
}

int32_t rules::CalculateValidCoordsInStartingPoint(
        const Board              &a_board,
        const Coordinate         &a_startingPointCoord,
        const PieceConfiguration &a_pieceConf,
        std::vector<Coordinate>  &out_validCoords)
{
    int32_t nValidCoords = 0;

    // go through all the squares that make up the piece and try to allocate
    // the piece so those squares touch a_startingPointCoord (the starting
    // point described by the user)
    PieceConfigurationContainer_t::const_iterator it;
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(
            a_startingPointCoord.m_row - it->m_row,
            a_startingPointCoord.m_col - it->m_col);

        if (IsPieceDeployableInStartingPoint(
                a_board, a_pieceConf, thisCoord, a_startingPointCoord))
        {
#ifdef DEBUG
            assert(static_cast<int32_t>(out_validCoords.size()) > nValidCoords);
#endif
            out_validCoords[nValidCoords] = thisCoord;
            nValidCoords++;
        }
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
    int32_t startRow = std::max(0, a_coord.m_row - a_radiusToCheck);
    int32_t startCol = std::max(0, a_coord.m_col - a_radiusToCheck);

    Coordinate thisCoord;
    for (thisCoord.m_row = startRow; thisCoord.m_row <= endRow; thisCoord.m_row++)
    {
		for (thisCoord.m_col = startCol; thisCoord.m_col <= endCol; thisCoord.m_col++)
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

void rules::RecalculateInfluenceAreaAroundPieceFast(
        const Board              &a_board,
        const Coordinate         &a_coord,
        const PieceConfiguration &a_pieceConf,
        Player                   &a_player)
{
    int32_t startCol, endCol, startRow, endRow;

    // pieces are described in such a way that the 1st coordinate
    // that describes them is right in the middle, that is,
    // relative (0, 0). Calculate the influence area around
    // a_coord because, which will always be occupied by the relative
    // (0, 0) square of a_pieceConf

    startRow = std::max(0, a_coord.m_row - INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    startCol = std::max(0, a_coord.m_col - INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    endRow   = std::min(a_board.GetNRows() - 1,    a_coord.m_row + INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    endCol   = std::min(a_board.GetNColumns() - 1, a_coord.m_col + INFLUENCE_AREA_AROUND_SQUARE_SIZE);

    Coordinate tmpCoord;
    for (tmpCoord.m_row  = startRow; tmpCoord.m_row <= endRow; tmpCoord.m_row++)
    {
        for (tmpCoord.m_col  = startCol; tmpCoord.m_col <= endCol; tmpCoord.m_col++)
        {
            if (a_board.IsCoordEmpty(tmpCoord) &&
                (IsCoordTouchingPlayerCompute(a_board, tmpCoord, a_player) == false) )
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
    PieceConfigurationContainer_t::const_iterator it =
        a_pieceConf.m_pieceSquares.begin();
    // 1st square is always relative (0, 0) - read comment above -
    // it has been checked in the previous loop. Also note that
    // incrementing this iterator is always correct because all
    // piece must have at least one coordinate
    it++;
    while (it != a_pieceConf.m_pieceSquares.end())
    {
        // go for the borders on the top and bottom of the influence area
        startCol = std::max(0, a_coord.m_col - INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_col);
        endCol   = std::min(a_board.GetNColumns() - 1,
                            a_coord.m_col + INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_col);

        if (it->m_row < 0)
        {
            // upper most row of the influence area. Check it only if the squares to be
            // checked are expected to be outside the "main" influence area checked
            // in the 1st loop of the function
            tmpCoord.m_row = a_coord.m_row - INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_row;
            if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
            {
                for (tmpCoord.m_col  = startCol;
                     tmpCoord.m_col <= endCol;
                     tmpCoord.m_col++)
                {
                    if (a_board.IsCoordEmpty(tmpCoord) &&
                        (IsCoordTouchingPlayerCompute(a_board, tmpCoord, a_player) == false) )
                    {
                        a_player.SetInfluencedCoord(tmpCoord);
                    }
                    else
                    {
                        a_player.UnsetInfluencedCoord(tmpCoord);
                    }
                }
            } // if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
        } // if (it->m_row < 0)
        else if (it->m_row > 0)
        {
            // lowest most row of the influence area
            tmpCoord.m_row = a_coord.m_row + INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_row;
            if ( (tmpCoord.m_row >= 0) && (tmpCoord.m_row < a_board.GetNRows()) )
            {
                for (tmpCoord.m_col  = startCol;
                     tmpCoord.m_col <= endCol;
                     tmpCoord.m_col++)
                {
                    if (a_board.IsCoordEmpty(tmpCoord) &&
                        (IsCoordTouchingPlayerCompute(a_board, tmpCoord, a_player) == false) )
                    {
                        a_player.SetInfluencedCoord(tmpCoord);
                    }
                    else
                    {
                        a_player.UnsetInfluencedCoord(tmpCoord);
                    }
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

        if (it->m_col < 0)
        {
            // left side
            tmpCoord.m_col = a_coord.m_col - INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_col;
            if ( (tmpCoord.m_col >= 0) && (tmpCoord.m_col < a_board.GetNColumns()) )
            {
                for (tmpCoord.m_row  = startRow;
                     tmpCoord.m_row <= endRow;
                     tmpCoord.m_row++)
                {
                    if (a_board.IsCoordEmpty(tmpCoord) &&
                        (IsCoordTouchingPlayerCompute(a_board, tmpCoord, a_player) == false) )
                    {
                        a_player.SetInfluencedCoord(tmpCoord);
                    }
                    else
                    {
                        a_player.UnsetInfluencedCoord(tmpCoord);
                    }
                }
            } // if ( (tmpCoord.m_col >= 0) && (tmpCoord.m_col < a_board.GetNColumns()) )
        } // if (it->m_col < 0)
        else if (it->m_col > 0)
        {
            // right side
            tmpCoord.m_col = a_coord.m_col + INFLUENCE_AREA_AROUND_SQUARE_SIZE + it->m_col;
            if ( (tmpCoord.m_col >= 0) && (tmpCoord.m_col < a_board.GetNColumns()) )
            {
                for (tmpCoord.m_row  = startRow;
                     tmpCoord.m_row <= endRow;
                     tmpCoord.m_row++)
                {
                    if (a_board.IsCoordEmpty(tmpCoord) &&
                        (IsCoordTouchingPlayerCompute(a_board, tmpCoord, a_player) == false) )
                    {
                        a_player.SetInfluencedCoord(tmpCoord);
                    }
                    else
                    {
                        a_player.UnsetInfluencedCoord(tmpCoord);
                    }
                }
            } // if ( (tmpCoord.m_col >= 0) && (tmpCoord.m_col < a_board.GetNColumns()) )
        }// if (it->m_col > 0)

        // go for the next square of the piece!
        it++;
    } // while (it != a_pieceConf.end())

}

bool rules::IsCoordInfluencedByPlayerFastCompute(
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
          IsCoordTouchingPlayerCompute(a_board, a_coord, a_player))
    {
        return false;
    }

    // go through the influence area around a_coord to check if there
    // at least one square taken over by a_player
    int32_t startRow = std::max(0, a_coord.m_row - INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    int32_t startCol = std::max(0, a_coord.m_col - INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    int32_t endRow   = std::min(a_board.GetNRows() - 1, a_coord.m_row + INFLUENCE_AREA_AROUND_SQUARE_SIZE);
    int32_t endCol   = std::min(a_board.GetNColumns() - 1, a_coord.m_col + INFLUENCE_AREA_AROUND_SQUARE_SIZE);

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

void rules::RecalculateInfluenceAreaInBoard(
        const Board &a_board,
        Player      &a_player)
{
    // remove current influence area state
    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows(); thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns(); thisCoord.m_col++)
        {
            a_player.UnsetInfluencedCoord(thisCoord);
        }
    }

    if (a_player.NumberOfPiecesAvailable() == 0)
    {
        return;
    }

    STLCoordinateSet_t nkPointsSet;
    std::vector<Coordinate> validCoords(PIECE_MAX_SQUARES);
    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        if (a_player.IsPieceAvailable(static_cast<ePieceType_t>(i)) == false)
        {
            continue;
        }

        const std::list<PieceConfiguration> &pieceConfList =
            a_player.m_pieces[i].GetPrecalculatedConfs();

        std::list<PieceConfiguration>::const_iterator pieceConfIt;
        for (pieceConfIt  = pieceConfList.begin();
             pieceConfIt != pieceConfList.end();
             pieceConfIt++)
        {
            a_player.GetAllNucleationPoints(nkPointsSet);
            for (STLCoordinateSet_t::const_iterator nkIt = nkPointsSet.begin();
                 nkIt != nkPointsSet.end();
                 nkIt++)
            {
                // retrieve if there is a valid coord of this piece in the current nk point
                int32_t nValidCoords = rules::CalculateValidCoordsInNucleationPoint(
                                            a_board,
                                            a_player,
                                            (*nkIt),
                                            (*pieceConfIt),
                                            validCoords);

                for (int32_t k = 0; k < nValidCoords; k++)
                {
                    PieceConfigurationContainer_t::const_iterator it;
                    for (it  = pieceConfIt->m_pieceSquares.begin();
                         it != pieceConfIt->m_pieceSquares.end();
                         it++)
                    {
                        Coordinate influeceAreaCoord(
                            validCoords[k].m_row + it->m_row,
                            validCoords[k].m_col + it->m_col);

                        if ( (influeceAreaCoord.m_row >= 0) &&
                             (influeceAreaCoord.m_row <  a_board.GetNRows()) &&
                             (influeceAreaCoord.m_col >= 0) &&
                             (influeceAreaCoord.m_col <  a_board.GetNColumns()) )
                        {
                            a_player.SetInfluencedCoord(influeceAreaCoord);
                        }
                    }
                }
            } // for (STLCoordinateSet_t::const_iterator it = nkPointsSet.begin();
        } // for (pieceCoordIt = coordConfList.begin()
    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
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

        const std::list<PieceConfiguration> &pieceConfList =
            a_player.m_pieces[i].GetPrecalculatedConfs();

        std::list<PieceConfiguration>::const_iterator pieceConfIt;
        for (pieceConfIt  = pieceConfList.begin();
             pieceConfIt != pieceConfList.end();
             pieceConfIt++)
        {
            if (a_player.GetStartingCoordinate().Initialised() &&
                a_board.IsCoordEmpty(a_player.GetStartingCoordinate()))
            {
                std::vector<Coordinate> validCoords(VALID_COORDS_SIZE);
                int32_t nValidCoords = CalculateValidCoordsInStartingPoint(
                                        a_board,
                                        a_player.GetStartingCoordinate(),
                                        (*pieceConfIt),
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
                                            (*pieceConfIt),
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
        const Piece* a_piece,
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

