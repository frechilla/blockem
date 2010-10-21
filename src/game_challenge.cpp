// ============================================================================
// Copyright 2010 Faustino Frechilla
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
/// @file  game_challenge.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 08-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#include <stdlib.h>
#include "gettext.h" // i18n
#include "game_challenge.h"
#include "rules.h"

/// challenger's name
static const char PLAYER_CHALLENGER_NAME[] = N_("Mr Green");
/// opponents's name 
/// (this player won't be externally visible. No need to translate its name)
static const char PLAYER_DISTURBER_NAME[] = "disturber";

/// it will be used as an empty space in the board
static const char CHAR_EMPTY = ' ';
/// it will be used as the character for the challenger on the board
static const char CHAR_CHALLENGER = 'X';
/// it will be used as the character for 'opponent' in the board
static const char CHAR_DISTURBER = 'O';


GameChallenge::GameChallenge(
    const BlockemChallenge& a_challenge) :
    m_board(
            a_challenge.GetBoardRows(), 
            a_challenge.GetBoardColumns(), 
            CHAR_EMPTY),
    m_challenger(
            _(PLAYER_CHALLENGER_NAME),
            CHAR_CHALLENGER,
            a_challenge.GetBoardRows(),
            a_challenge.GetBoardColumns(),
            (a_challenge.IsChallengerStartingCoordSet()) ? 
                a_challenge.GetChallengerStartingCoord() : Coordinate()),
    m_disturber(
            PLAYER_DISTURBER_NAME,
            CHAR_DISTURBER,
            a_challenge.GetBoardRows(),
            a_challenge.GetBoardColumns(),
            Coordinate())
{
    // load challenge data into this object attributes
    LoadChallenge(a_challenge);
}

GameChallenge::~GameChallenge()
{
}

void GameChallenge::SetChallengerColour(
        uint8_t a_red,
        uint8_t a_green,
        uint8_t a_blue)
{
    m_challenger.SetColour(a_red, a_green, a_blue);
}

void GameChallenge::SetOpponentTakenSquaresColour(
        uint8_t a_red,
        uint8_t a_green,
        uint8_t a_blue)
{
    m_disturber.SetColour(a_red, a_green, a_blue);
}

void GameChallenge::LoadChallenge(const BlockemChallenge& a_challenge)
{
    STLCoordinateSet_t::iterator takenCoordIt;
    Coordinate thisCoord;
    
    for (thisCoord.m_row = 0; 
         thisCoord.m_row < m_board.GetNRows(); 
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; 
             thisCoord.m_col < m_board.GetNColumns(); 
             thisCoord.m_col++)
        {
            takenCoordIt = 
                a_challenge.GetOpponentTakenSquares().find(thisCoord);
            if (takenCoordIt != a_challenge.GetOpponentTakenSquares().end())
            {
                m_board.SetPlayerInCoord(thisCoord, m_disturber);
#ifdef DEBUG
                // this coordinate can't be taken by the challenger!!
                takenCoordIt = 
                    a_challenge.GetChallengerTakenSquares().find(thisCoord);
                assert (takenCoordIt == 
                            a_challenge.GetChallengerTakenSquares().end());
#endif
            }
            else
            {
                takenCoordIt = 
                    a_challenge.GetChallengerTakenSquares().find(thisCoord);
                if (takenCoordIt != a_challenge.GetChallengerTakenSquares().end())
                {
                    m_board.SetPlayerInCoord(thisCoord, m_challenger);
                }
                else
                {
                    // empty square!
                    m_board.BlankCoord(thisCoord);
                }
            }
        }
    }
    
    for (int32_t i = e_minimumPieceIndex; 
         i < e_numberOfPieces;
         i++)
    {
        ePieceType_t thisPiece = static_cast<ePieceType_t>(i);
        
        // disturber has no pieces at all. it doesn't need them
        if (m_disturber.IsPieceAvailable(thisPiece))
        {
            m_disturber.UnsetPiece(thisPiece);
        }
        
        // is this piece avilable for the challenger?
        if (a_challenge.IsChallengerPieceAvailable(thisPiece) && 
            !m_challenger.IsPieceAvailable(thisPiece))
        {
            m_challenger.SetPiece(thisPiece);
        }
        else if (!a_challenge.IsChallengerPieceAvailable(thisPiece) && 
                 m_challenger.IsPieceAvailable(thisPiece))
        {
            m_challenger.UnsetPiece(thisPiece);
        }
    }
    
    RecalculateNKInAllBoard();
}

void GameChallenge::RecalculateNKInAllBoard()
{
    Coordinate thisCoord;
    for (thisCoord.m_row = 0 ;
         thisCoord.m_row < m_board.GetNRows();
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0 ;
             thisCoord.m_col < m_board.GetNColumns();
             thisCoord.m_col++)
        {
            if (rules::IsNucleationPointCompute(m_board, m_challenger, thisCoord))
            {
                m_challenger.SetNucleationPoint(thisCoord);
            }
            else
            {
                m_challenger.UnsetNucleationPoint(thisCoord);
            }
        }
    }
}

void GameChallenge::Reset(const BlockemChallenge& a_challenge)
{
    if ( (a_challenge.GetBoardRows() != m_board.GetNRows()) ||
         (a_challenge.GetBoardColumns() != m_board.GetNColumns()) )
    {
        // size of the new challenge is different from the currently loaded one
        // boards and players must be adapted to this new size before loading
        // its data on the objects
        m_board = Board(
            a_challenge.GetBoardRows(), 
            a_challenge.GetBoardColumns(), 
            CHAR_EMPTY);
        
        uint8_t red, green, blue;
        m_challenger.GetColour(red, green, blue);        
        m_challenger = Player(
            _(PLAYER_CHALLENGER_NAME),
            CHAR_CHALLENGER,
            a_challenge.GetBoardRows(),
            a_challenge.GetBoardColumns(),
            (a_challenge.IsChallengerStartingCoordSet()) ? 
                a_challenge.GetChallengerStartingCoord() : Coordinate() );
                
        // this is a new player instance. colour must be reset
        m_challenger.SetColour(red, green, blue);
        
        m_disturber.GetColour(red, green, blue);
        m_disturber = Player(
            PLAYER_DISTURBER_NAME,
            CHAR_DISTURBER,
            a_challenge.GetBoardRows(),
            a_challenge.GetBoardColumns(),
            Coordinate());
        m_disturber.SetColour(red, green, blue);
    }
    
    // load challenge data into this object attributes
    LoadChallenge(a_challenge);
}

void GameChallenge::RemovePiece(
        const Piece      &a_piece,
        const Coordinate &a_coord)
{
    RemovePiece(
        a_coord,
        a_piece.GetCurrentConfiguration());

    // piece is again available
    m_challenger.SetPiece(a_piece.GetType());
}

void GameChallenge::PutDownPiece(
        const Piece      &a_piece,
        const Coordinate &a_coord)
{
    PutDownPiece(
        a_coord,
        a_piece.GetCurrentConfiguration());

    // piece is not available now
    m_challenger.UnsetPiece(a_piece.GetType());
}

void GameChallenge::RemovePiece(
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

    // go through the list of squares of the piece first
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

        assert(m_board.IsPlayerInCoord(thisCoord, m_challenger));
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
        		m_challenger,
        		thisCoord))
        {
        	m_challenger.SetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_pieceSquares.begin();

    // now check the nk points of the piece. Are they still nk points for m_challenger?
    for (it  = a_pieceConf.m_nkPoints.begin();
         it != a_pieceConf.m_nkPoints.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        if ( (thisCoord.m_row < 0) ||(thisCoord.m_col < 0)    ||
             (thisCoord.m_row >= m_board.GetNRows())          ||
             (thisCoord.m_col >= m_board.GetNColumns())       ||
             (m_challenger.IsNucleationPoint(thisCoord) == false) )
        {
            // this point is out of the board (or is not an nk
            // point). Try next one
            continue;
        }

        if (!rules::IsNucleationPointCompute(
                m_board,
        		m_challenger,
        		thisCoord))
        {
            // this nk point is not nk point any more since the piece has been
            // removed
        	m_challenger.UnsetNucleationPoint(thisCoord);
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
        assert(m_challenger.IsNucleationPoint(thisCoord) == false);
#endif

        if (rules::IsNucleationPointCompute(
                m_board,
        		m_challenger,
        		thisCoord))
        {
            // this forbidden coord is now a nk point since the piece was removed
        	m_challenger.SetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_forbiddenArea.begin();
}

void GameChallenge::PutDownPiece(
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

        m_board.SetPlayerInCoord(thisCoord,	m_challenger);
        m_challenger.UnsetNucleationPoint(thisCoord);
    } // for (it  = a_pieceConf.m_pieceSquares.begin();

    // now check the nk points of the piece. If they are inside the board they will 
    // be checked in case they are touching challenger's pieces
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
             (!rules::IsCoordTouchingPlayerCompute(m_board, thisCoord, m_challenger)))
        {
            // this coord is now a valid nk point (if it already was a nk point
            // it's ok -and faster- to set it twice
        	m_challenger.SetNucleationPoint(thisCoord);
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
            m_challenger.UnsetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_forbiddenArea.begin();
}
