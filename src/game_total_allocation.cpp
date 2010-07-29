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
/// @file  GameTotalAllocation.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Sep-2009  Original development
///           Faustino Frechilla 21-Jul-2010  i18n
/// @endhistory
///
// ============================================================================

#include <stdlib.h>
#include "gettext.h" // i18n
#include "game_total_allocation.h"
#include "rules.h"

/// it will be used as an empty space in the board
const char CHAR_EMPTY    = ' ';
/// it will be used as the character for the player in the board
const char CHAR_PLAYER   = 'X';


GameTotalAllocation::GameTotalAllocation(int32_t a_rows, int32_t a_columns) :
    m_board(a_rows, a_columns, CHAR_EMPTY),
    m_player(std::string(_("The player")),
             CHAR_PLAYER,
             a_rows,
             a_columns,
             Coordinate(0, 0))
{
}

GameTotalAllocation::~GameTotalAllocation()
{
}

void GameTotalAllocation::RemovePiece(
        const Coordinate         &a_coord,
        const PieceConfiguration &a_pieceConf)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_row < m_board.GetNRows());

    assert(a_coord.m_col >= 0);
    assert(a_coord.m_col < m_board.GetNColumns());
#endif

    PieceConfigurationContainer_t::const_iterator it;

    // go through the list of squares of the piecee first
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);
#ifdef DEBUG
        assert( ((a_coord.m_row + it->m_row) >= 0) &&
        		((a_coord.m_row + it->m_row) < m_board.GetNRows())    );
        assert( ((a_coord.m_col + it->m_col) >= 0) &&
        		((a_coord.m_col + it->m_col) < m_board.GetNColumns()) );

        assert(m_board.IsPlayerInCoord(thisCoord, m_player));
#endif

        m_board.BlankCoord(thisCoord);
    }

    // go through the list of squares of the piece again, now that
    // the piece has been removed
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        // thisCoord is now empty
        // is it now a nucleation point for player? (it couldn't be before, as it was occupied)
        if (rules::IsNucleationPointCompute(
                m_board,
        		m_player,
        		thisCoord))
        {
        	m_player.SetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_pieceSquares.begin();

    // now check the nk points of the piece. Are they still nk points for m_player?
    for (it  = a_pieceConf.m_nkPoints.begin();
         it != a_pieceConf.m_nkPoints.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        if ( (thisCoord.m_row < 0) ||(thisCoord.m_col < 0)    ||
             (thisCoord.m_row >= m_board.GetNRows())          ||
             (thisCoord.m_col >= m_board.GetNColumns())       ||
             (m_player.IsNucleationPoint(thisCoord) == false) )
        {
            // this point is out of the board (or is not an nk
            // point). Try next one
            continue;
        }

        if (!rules::IsNucleationPointCompute(
                m_board,
        		m_player,
        		thisCoord))
        {
            // this nk point is not nk point any more since the piece has been
            // removed
        	m_player.UnsetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_nkPoints.begin();

    // forbidden areas around the piece that was just removed might also be nk points
    for (it  = a_pieceConf.m_forbiddenArea.begin();
         it != a_pieceConf.m_forbiddenArea.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        if ( (thisCoord.m_row < 0) || (thisCoord.m_col < 0) ||
             (thisCoord.m_row >= m_board.GetNRows())        ||
             (thisCoord.m_col >= m_board.GetNColumns())     ||
             (m_board.IsCoordEmpty(thisCoord) == false)     )
        {
            // this forbidden coord is out of the board. Try next one
            continue;
        }

#ifdef DEBUG
        assert(m_player.IsNucleationPoint(thisCoord) == false);
#endif

        if (rules::IsNucleationPointCompute(
                m_board,
        		m_player,
        		thisCoord))
        {
            // this forbidden coord is now a nk point since the piece was removed
        	m_player.SetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_forbiddenArea.begin();
}

void GameTotalAllocation::PutDownPiece(
        const Coordinate         &a_coord,
        const PieceConfiguration &a_pieceConf)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_row < m_board.GetNRows());

    assert(a_coord.m_col >= 0);
    assert(a_coord.m_col < m_board.GetNColumns());
#endif

    PieceConfigurationContainer_t::const_iterator it;

    // go through the list of squares of the piecee first
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);
#ifdef DEBUG
        assert( ((a_coord.m_row + it->m_row) >= 0) &&
        		((a_coord.m_row + it->m_row) < m_board.GetNRows())    );
        assert( ((a_coord.m_col + it->m_col) >= 0) &&
        		((a_coord.m_col + it->m_col) < m_board.GetNColumns()) );

        assert(m_board.IsCoordEmpty(thisCoord));
#endif

        m_board.SetPlayerInCoord(thisCoord,	m_player);
        m_player.UnsetNucleationPoint(thisCoord);
    } // for (it  = a_pieceConf.m_pieceSquares.begin();

    // now check the nk points of the piece. If they are inside the board they will be set as
    // nucleation point. No need to compute
    for (it  = a_pieceConf.m_nkPoints.begin();
         it != a_pieceConf.m_nkPoints.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        if ( (thisCoord.m_row >= 0)                    &&
             (thisCoord.m_col >= 0)                    &&
             (thisCoord.m_row < m_board.GetNRows())    &&
             (thisCoord.m_col < m_board.GetNColumns()) &&
             (m_board.IsCoordEmpty(thisCoord))         &&
             (!rules::IsCoordTouchingPlayerCompute(m_board, thisCoord, m_player)))
        {
            // this coord is now a valid nk point (if it already was a nk point
            // it's ok -and faster- to set it twice
        	m_player.SetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_nkPoints.begin();

    // forbidden areas around the piece can't be a valid nk point any longer
    for (it  = a_pieceConf.m_forbiddenArea.begin();
         it != a_pieceConf.m_forbiddenArea.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        if ( (thisCoord.m_row >= 0)                    &&
             (thisCoord.m_col >= 0)                    &&
             (thisCoord.m_row < m_board.GetNRows())    &&
             (thisCoord.m_col < m_board.GetNColumns()) )
        {
            // this coord is not a nk point any longer since it belongs
            // to the forbidden area of the deployed piece
            m_player.UnsetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_forbiddenArea.begin();
}

bool GameTotalAllocation::Solve(const Coordinate &a_startingCoord)
{
    std::vector<Coordinate> validCoords(PIECE_MAX_SQUARES);

	//update the starting point of the player
	m_player.SetStartingCoordinate(a_startingCoord);

	// declare the array of last pieces and old NK points and clear them out
	ePieceType_t lastPieces[e_numberOfPieces];
	// pointers to the stack, it is a bad idea, but they won't be used badly I promise
	STLCoordinateSet_t* oldNkPoints[e_numberOfPieces];

	for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
	{
		lastPieces[i]  = e_noPiece;
		oldNkPoints[i] = NULL;
	}

	STLCoordinateSet_t emptySet;
	oldNkPoints[0] = &emptySet;

    for (int32_t currentPiece = e_numberOfPieces - 1;
         currentPiece >= e_minimumPieceIndex;
         currentPiece--)
    {
		if (!m_player.IsPieceAvailable(static_cast<ePieceType_t>(currentPiece)))
		{
		    continue;
		}

    	m_player.UnsetPiece(static_cast<ePieceType_t>(currentPiece));

        // save current deployed piece in the lastPieces array
        lastPieces[0] = m_player.m_pieces[currentPiece].GetType();

    	const std::list<PieceConfiguration> &pieceConfList =
            m_player.m_pieces[currentPiece].GetPrecalculatedConfs();

        std::list<PieceConfiguration>::const_iterator pieceConfIt;
        for (pieceConfIt  = pieceConfList.begin();
             pieceConfIt != pieceConfList.end();
             pieceConfIt++)
        {
            int32_t nValidCoords = rules::CalculateValidCoordsInStartingPoint(
                                              m_board,
                                              a_startingCoord,
                                              *pieceConfIt,
                                              validCoords);

            for (int32_t k = 0 ; k < nValidCoords ; k++)
            {
                PutDownPiece(validCoords[k], *pieceConfIt);

                if (AllocateAllPieces(lastPieces, oldNkPoints))
                {
                    return true;
                }

                RemovePiece(validCoords[k], *pieceConfIt);
            }
        } // for (pieceConfIt = pieceConfList.begin()

        m_player.SetPiece(static_cast<ePieceType_t>(currentPiece));
    } // for (int32_t currentPiece = e_numberOfPieces - 1;

    // if we got here we couldn't put down the pieces in the board. Pity
    return false;
}

bool GameTotalAllocation::AllocateAllPieces(
        ePieceType_t        a_lastPieces[e_numberOfPieces],
		STLCoordinateSet_t* a_oldNkPoints[e_numberOfPieces])
{
    // vector to be sued to save the valid coords in each nk point
    std::vector<Coordinate> validCoords(PIECE_MAX_SQUARES);
	// this set will be used not to place the same piece in the same place more than once
    // it will be reset every time the piece is rotated/mirrored or the piece is changed
    STLCoordinateSet_t testedCoords;
    // this set will save the current nucleation points
    STLCoordinateSet_t nkPointSet;

    if (m_player.NumberOfPiecesAvailable() == 0)
    {
        return true;
    }

    // retrieve current nk points. It will be used in future calls not to check configurations more than once
    m_player.GetAllNucleationPoints(nkPointSet);

    // save a pointer to this set in the place (index) reserved for it
    // the index is the level of depth in the search tree, so if 1 piece has been set
    // the current level will be 1
    a_oldNkPoints[e_numberOfPieces - m_player.NumberOfPiecesAvailable()] = &nkPointSet;

	for (int32_t currentPiece = e_numberOfPieces - 1;
         currentPiece >= e_minimumPieceIndex;
         currentPiece--)
	{
		if (!m_player.IsPieceAvailable(static_cast<ePieceType_t>(currentPiece)))
		{
		    continue;
		}

		m_player.UnsetPiece(static_cast<ePieceType_t>(currentPiece));

        // save current deployed piece in the lastPieces array
        a_lastPieces[e_numberOfPieces - m_player.NumberOfPiecesAvailable()] =
            m_player.m_pieces[currentPiece].GetType();

    	const std::list<PieceConfiguration> &pieceConfList =
            m_player.m_pieces[currentPiece].GetPrecalculatedConfs();

        std::list<PieceConfiguration>::const_iterator pieceConfIt;
        for (pieceConfIt = pieceConfList.begin();
             pieceConfIt != pieceConfList.end();
             pieceConfIt++)
        {
            STLCoordinateSet_t::iterator nkIterator = nkPointSet.begin();
            while(nkIterator != nkPointSet.end())
            {
                // before putting down the piece test if the future configuration
                // could be tested by some other level in the backtrack tree
                // using (e_numberOfPieces - m_player.NumberOfPiecesAvailable() - 2)
                // because m_player.NumberOfPiecesAvailable() - 1 is this current level in the tree
                // and we don't want to use it. No need for using the level 0 either
                int32_t thisLevel;
                for (thisLevel = e_numberOfPieces - m_player.NumberOfPiecesAvailable() - 2;
                     thisLevel > 0;
                     thisLevel--)
                {
                    // if the current piece's index is bigger than the old piece deployed on
                    // the board, put down the piece only if this nucleation doesn't belong to the
                    // old nucleation point's set
                    // both old piece and old nk points set correspond to this particular
                    // level of the backtrack tree
                    if (currentPiece >= a_lastPieces[thisLevel])
                    {
                        STLCoordinateSet_t::iterator it;
                        it = a_oldNkPoints[thisLevel]->find(*nkIterator);
                        if (it != a_oldNkPoints[thisLevel]->end())
                        {
                            nkIterator++;
                            break;
                        }
                    }
                }

                if (thisLevel > 0)
                {
                    // the previous loop called break before the end
                    continue;
                }

                // retrieve the valid coords of this piece in the current nk point
                int32_t nValidCoords = rules::CalculateValidCoordsInNucleationPoint(
                                                    m_board,
                                                    m_player,
                                                    *nkIterator,
                                                    *pieceConfIt,
                                                    validCoords);

                for (int32_t k = 0; k < nValidCoords; k++)
                {
                    STLCoordinateSet_t::iterator it =
                        testedCoords.find(validCoords[k]);
                    if (it == testedCoords.end())
                    {
                        testedCoords.insert(validCoords[k]);

                        PutDownPiece(validCoords[k], *pieceConfIt);

                        if (AllocateAllPieces(a_lastPieces, a_oldNkPoints))
                        {
                            return true;
                        }

                        RemovePiece(validCoords[k], *pieceConfIt);
                    }
                } // for (int32_t k = 0; k < nValidCoords; k++)

                nkIterator++;
            } // while(nkIterator != nkPointSet.end())

            testedCoords.clear();
        } // for (pieceConfIt = pieceConfList.begin()

        m_player.SetPiece(static_cast<ePieceType_t>(currentPiece));

	} // for (int32_t currentPiece = e_numberOfPieces - 1;

    return false;
}
