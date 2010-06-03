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
/// @endhistory
///
// ============================================================================

#include <iostream>
#include <stdlib.h>
#include "game_total_allocation.h"
#include "rules.h"

/// it will be used as an empty space in the board
const char CHAR_EMPTY    = ' ';
/// it will be used as the character for the player in the board
const char CHAR_PLAYER   = 'X';


GameTotalAllocation::GameTotalAllocation(int32_t a_rows, int32_t a_columns) :
    m_board(a_rows, a_columns, CHAR_EMPTY),
    m_player(std::string("ThePlayer"), CHAR_PLAYER, a_rows, a_columns, Coordinate(0, 0))
{
}

GameTotalAllocation::~GameTotalAllocation()
{
}

void GameTotalAllocation::RemovePiece(
        const Coordinate           &a_coord,
        const pieceConfiguration_t &a_pieceConf,
        int32_t                     a_pieceRadius)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_row < m_board.GetNRows());

    assert(a_coord.m_col >= 0);
    assert(a_coord.m_col < m_board.GetNColumns());
#endif

    for (pieceConfiguration_t::const_iterator it = a_pieceConf.begin();
         it != a_pieceConf.end();
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

    // recalculate all the nk points around the piece we just put down
    rules::RecalculateNKAroundCoord(m_board, a_coord, a_pieceRadius + 1, m_player);
}

void GameTotalAllocation::PutDownPiece(
        const Coordinate           &a_coord,
        const pieceConfiguration_t &a_pieceConf,
        int32_t                     a_pieceRadius)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_row < m_board.GetNRows());

    assert(a_coord.m_col >= 0);
    assert(a_coord.m_col < m_board.GetNColumns());
#endif

    for (pieceConfiguration_t::const_iterator it = a_pieceConf.begin();
         it != a_pieceConf.end();
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
    }

    // recalculate all the nk points around the piece we just put down
    rules::RecalculateNKAroundCoord(m_board, a_coord, a_pieceRadius + 1, m_player);
}

bool GameTotalAllocation::Solve(const Coordinate &a_startingCoord)
{
    STLCoordinateSet_t emptySet;

	// declare the array of last pieces and old NK points and clear them out
	ePieceType_t lastPieces[e_numberOfPieces];
	// pointers to the stack, it is a bad idea, but they won't be used badly I promise
	STLCoordinateSet_t* oldNkPoints[e_numberOfPieces];

	for (int32_t i = e_minimumPieceIndex ; i < e_numberOfPieces ; i++)
	{
		lastPieces[i]  = e_noPiece;
		oldNkPoints[i] = NULL;
	}

	//update the starting point of the player
	m_player.SetStartingCoordinate(a_startingCoord);

    for (int32_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
    	m_player.UnsetPiece(static_cast<ePieceType_t>(i));

    	std::vector<Coordinate> validCoords(PIECE_MAX_SQUARES);

        const std::list<pieceConfiguration_t> &coordConfList =
            m_player.m_pieces[i].GetPrecalculatedConfs();
        std::list<pieceConfiguration_t>::const_iterator pieceCoordIt;

        for (pieceCoordIt = coordConfList.begin();
             pieceCoordIt != coordConfList.end();
             pieceCoordIt++)
        {
            int32_t nValidCoords = rules::CalculateValidCoordsInStartingPoint(
                                              m_board,
                                              m_player,
                                              a_startingCoord,
                                              *pieceCoordIt,
                                              m_player.m_pieces[i].GetRadius(),
                                              validCoords);

            if (nValidCoords > VALID_COORDS_SIZE)
            {
                std::cerr << "Jaysus" << std::endl << std::endl;
                exit(-1);
            }

            for (int32_t k = 0 ; k < nValidCoords ; k++)
            {
                PutDownPiece(validCoords[k], *pieceCoordIt, m_player.m_pieces[i].GetRadius());

                if (AllocateAllPieces(lastPieces, oldNkPoints))
                {
                    return true;
                }

                RemovePiece(validCoords[k], *pieceCoordIt, m_player.m_pieces[i].GetRadius());
            }
        } // for (pieceCoordIt = coordConfList.begin()

        m_player.SetPiece(static_cast<ePieceType_t>(i));
    } // for (int32_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)

    // if we got here we couldn't put down the pieces in the board. Pity
    return false;
}

bool GameTotalAllocation::AllocateAllPieces(
        ePieceType_t        a_lastPieces[e_numberOfPieces],
		STLCoordinateSet_t* a_oldNkPoints[e_numberOfPieces])
{
	// this set will be used not to place the same piece in the same place more than once
    // it will be reset every time the piece is rotated/mirrored or the piece is changed
    STLCoordinateSet_t testedCoords;

    if (m_player.NumberOfPiecesAvailable() == 0)
    {
        return true;
    }

    // this set will save the current nucleation points. It will be used in future calls not to
    // check configurations more than once
    STLCoordinateSet_t nkPointSet;
    m_player.GetAllNucleationPoints(nkPointSet);

    // save a pointer to this set in the place (index) reserved for it
    // the index is the level of depth in the search tree, so if 1 piece has been set
    // the current level will be 1
    a_oldNkPoints[e_numberOfPieces - m_player.NumberOfPiecesAvailable()] = &nkPointSet;

	for (int32_t i = e_numberOfPieces - 1; i >= e_minimumPieceIndex; i--)
	{
		if (!m_player.IsPieceAvailable(static_cast<ePieceType_t>(i)))
		{
		    continue;
		}

		m_player.UnsetPiece(static_cast<ePieceType_t>(i));

		std::vector<Coordinate> validCoords(PIECE_MAX_SQUARES);

        const std::list<pieceConfiguration_t> &coordConfList =
            m_player.m_pieces[i].GetPrecalculatedConfs();
        std::list<pieceConfiguration_t>::const_iterator pieceCoordIt;

        for (pieceCoordIt = coordConfList.begin();
             pieceCoordIt != coordConfList.end();
             pieceCoordIt++)
        {
            STLCoordinateSet_t::iterator nkIterator = nkPointSet.begin();
            while(nkIterator != nkPointSet.end())
            {
                // before putting down the piece test if the future configuration
                // could be tested by some other level in the backtrack tree
                // using (e_numberOfPieces - m_player.NumberOfPiecesAvailable() - 2)
                // because m_player.NumberOfPiecesAvailable() - 1 is this current level in the tree
                // and we don't want to use it. No need for using the level 0 either, because
                // a_lastPieces[0] and  a_oldNkPoints[0] is set to NULL
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
                    if (i >= a_lastPieces[thisLevel])
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
                                                                          *pieceCoordIt,
                                                                          m_player.m_pieces[i].GetRadius(),
                                                                          validCoords);

                for (int32_t k = 0; k < nValidCoords; k++)
                {
                    STLCoordinateSet_t::iterator it =
                        testedCoords.find(validCoords[k]);
                    if (it == testedCoords.end())
                    {
                        testedCoords.insert(validCoords[k]);

                        PutDownPiece(validCoords[k], *pieceCoordIt, m_player.m_pieces[i].GetRadius());

                        // it's m_player.NumberOfPiecesAvailable()-1 because current piece
                        // has already being removed from the player
                        a_lastPieces[e_numberOfPieces - m_player.NumberOfPiecesAvailable() - 1]  =
                            m_player.m_pieces[i].GetType();

                        if (AllocateAllPieces(a_lastPieces, a_oldNkPoints))
                        {
                            return true;
                        }

                        RemovePiece(validCoords[k], *pieceCoordIt, m_player.m_pieces[i].GetRadius());
                    }
                } // for (int32_t k = 0; k < nValidCoords; k++)

                nkIterator++;
            } // while(nkIterator != nkPointSet.end())

            testedCoords.clear();
        } // for (pieceCoordIt = coordConfList.begin()

        m_player.SetPiece(static_cast<ePieceType_t>(i));

	} // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)

    return false;
}
