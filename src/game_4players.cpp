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
/// @file  game_4players.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 15-Nov-2010  Original development
/// @endhistory
///
// ============================================================================

#include <stdlib.h>
#include "gettext.h" // i18n
#include "game_4players.h"
#include "rules.h"

/// player1's name
static const char PLAYER_1_NAME[] = N_("Mr Green");
/// player2's name
static const char PLAYER_2_NAME[] = N_("Mr Blue");
/// player3's name
static const char PLAYER_3_NAME[] = N_("Mr Yellow");
/// player4's name
static const char PLAYER_4_NAME[] = N_("Mr Red");

/// it will be used as an empty space in the board
static const char CHAR_EMPTY   = ' ';
/// it will be used as the character for 'player1' in the board
static const char CHAR_PLAYER1 = 'X';
/// it will be used as the character for 'player2' in the board
static const char CHAR_PLAYER2 = 'O';
/// it will be used as the character for 'player3' in the board
static const char CHAR_PLAYER3 = '+';
/// it will be used as the character for 'player4' in the board
static const char CHAR_PLAYER4 = '-';

static const int32_t GAME_4_PLAYERS_DEFAULT_BOARD_ROWS = 20;
static const int32_t GAME_4_PLAYERS_DEFAULT_BOARD_COLS = 20;

static const Coordinate PLAYER1_DEFAULT_STARTING_COORD = Coordinate(
    0, 0);
static const Coordinate PLAYER2_DEFAULT_STARTING_COORD = Coordinate(
    0, GAME_4_PLAYERS_DEFAULT_BOARD_COLS - 1);
static const Coordinate PLAYER3_DEFAULT_STARTING_COORD = Coordinate(
    GAME_4_PLAYERS_DEFAULT_BOARD_COLS - 1, GAME_4_PLAYERS_DEFAULT_BOARD_COLS - 1);
static const Coordinate PLAYER4_DEFAULT_STARTING_COORD = Coordinate(
    GAME_4_PLAYERS_DEFAULT_BOARD_COLS - 1, 0);
    
// this is a workaround to initialise an array in a constructor
// gcc says when trying ot properly initialise m_players: 
//     "extended initializer lists is only available with -std=c++0x o -std=gnu++0x"
static const Player DEFAULT_PLAYERS_ARRAY[Game4Players::e_Game4_PlayersCount] = 
{
    Player(_(PLAYER_1_NAME),
             CHAR_PLAYER1,
             GAME_4_PLAYERS_DEFAULT_BOARD_ROWS,
             GAME_4_PLAYERS_DEFAULT_BOARD_COLS,
             PLAYER1_DEFAULT_STARTING_COORD),
    Player(_(PLAYER_2_NAME),
             CHAR_PLAYER2,
             GAME_4_PLAYERS_DEFAULT_BOARD_ROWS,
             GAME_4_PLAYERS_DEFAULT_BOARD_COLS,
             PLAYER2_DEFAULT_STARTING_COORD),
    Player(_(PLAYER_3_NAME),
             CHAR_PLAYER3,
             GAME_4_PLAYERS_DEFAULT_BOARD_ROWS,
             GAME_4_PLAYERS_DEFAULT_BOARD_COLS,
             PLAYER3_DEFAULT_STARTING_COORD),
    Player(_(PLAYER_4_NAME),
             CHAR_PLAYER4,
             GAME_4_PLAYERS_DEFAULT_BOARD_ROWS,
             GAME_4_PLAYERS_DEFAULT_BOARD_COLS,
             PLAYER4_DEFAULT_STARTING_COORD),
};


Game4Players::Game4Players() :
    m_board(
            GAME_4_PLAYERS_DEFAULT_BOARD_ROWS, 
            GAME_4_PLAYERS_DEFAULT_BOARD_COLS, 
            CHAR_EMPTY),
    m_players(DEFAULT_PLAYERS_ARRAY)
{
    ResetOpponentLists();
}

Game4Players::~Game4Players()
{
}

void Game4Players::ResetOpponentLists()
{
    m_playerOpponentLists[e_Game4_Player1].clear();
    m_playerOpponentLists[e_Game4_Player1].push_back(&m_players[e_Game4_Player2]);
    m_playerOpponentLists[e_Game4_Player1].push_back(&m_players[e_Game4_Player3]);
    m_playerOpponentLists[e_Game4_Player1].push_back(&m_players[e_Game4_Player4]);    

    m_playerOpponentLists[e_Game4_Player2].clear();
    m_playerOpponentLists[e_Game4_Player2].push_back(&m_players[e_Game4_Player1]);
    m_playerOpponentLists[e_Game4_Player2].push_back(&m_players[e_Game4_Player3]);
    m_playerOpponentLists[e_Game4_Player2].push_back(&m_players[e_Game4_Player4]);
    
    m_playerOpponentLists[e_Game4_Player3].clear();
    m_playerOpponentLists[e_Game4_Player3].push_back(&m_players[e_Game4_Player1]);
    m_playerOpponentLists[e_Game4_Player3].push_back(&m_players[e_Game4_Player2]);
    m_playerOpponentLists[e_Game4_Player3].push_back(&m_players[e_Game4_Player4]);
    
    m_playerOpponentLists[e_Game4_Player4].clear();
    m_playerOpponentLists[e_Game4_Player4].push_back(&m_players[e_Game4_Player1]);
    m_playerOpponentLists[e_Game4_Player4].push_back(&m_players[e_Game4_Player2]);
    m_playerOpponentLists[e_Game4_Player4].push_back(&m_players[e_Game4_Player3]);
}

const Player& Game4Players::GetPlayer(eGame4_Player_t a_player) const
{
#ifdef DEBUG
    assert(a_player >= e_Game4_Player1 && a_player <= e_Game4_Player4);        
#endif

    return m_players[a_player];
}

Game4Players::eGame4_Player_t Game4Players::GetPlayerType(const Player &a_player) const
{
    eGame4_Player_t rValue = Game4Players::e_Game4_NoPlayer;

    if (a_player.PresentationChar() == m_players[e_Game4_Player1].PresentationChar())
    {
        rValue = e_Game4_Player1;
    }
    else if (a_player.PresentationChar() == m_players[e_Game4_Player2].PresentationChar())
    {
        rValue = e_Game4_Player2;
    }
    else if (a_player.PresentationChar() == m_players[e_Game4_Player3].PresentationChar())
    {
        rValue = e_Game4_Player3;
    }
    else if (a_player.PresentationChar() == m_players[e_Game4_Player4].PresentationChar())
    {
        rValue = e_Game4_Player4;
    }
    else
    {
#ifdef DEBUG
        assert(0);
#endif
    }

    return rValue;
}

void Game4Players::SetPlayerColour(
        eGame4_Player_t a_player,
        uint8_t a_red,
        uint8_t a_green,
        uint8_t a_blue)
{
#ifdef DEBUG
    assert(a_player >= e_Game4_Player1 && a_player <= e_Game4_Player4);        
#endif

    m_players[a_player].SetColour(a_red, a_green, a_blue);
}

void Game4Players::RecalculateNKInAllBoard()
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
            if (rules::IsNucleationPointCompute(m_board, m_players[e_Game4_Player1], thisCoord))
            {
                m_players[e_Game4_Player1].SetNucleationPoint(thisCoord);
            }
            else
            {
                m_players[e_Game4_Player1].UnsetNucleationPoint(thisCoord);
            }
            
            if (rules::IsNucleationPointCompute(m_board, m_players[e_Game4_Player2], thisCoord))
            {
                m_players[e_Game4_Player2].SetNucleationPoint(thisCoord);
            }
            else
            {
                m_players[e_Game4_Player2].UnsetNucleationPoint(thisCoord);
            }
            
            if (rules::IsNucleationPointCompute(m_board, m_players[e_Game4_Player3], thisCoord))
            {
                m_players[e_Game4_Player3].SetNucleationPoint(thisCoord);
            }
            else
            {
                m_players[e_Game4_Player3].UnsetNucleationPoint(thisCoord);
            }
            
            if (rules::IsNucleationPointCompute(m_board, m_players[e_Game4_Player4], thisCoord))
            {
                m_players[e_Game4_Player4].SetNucleationPoint(thisCoord);
            }
            else
            {
                m_players[e_Game4_Player4].UnsetNucleationPoint(thisCoord);
            }
        }
    }
}

void Game4Players::Reset(
    int32_t a_nRows,
    int32_t a_nCols,
    const Coordinate &a_startingCoordPlayer1,
    const Coordinate &a_startingCoordPlayer2,
    const Coordinate &a_startingCoordPlayer3,
    const Coordinate &a_startingCoordPlayer4)
{
    if ( (a_nRows != m_board.GetNRows()) ||
         (a_nCols != m_board.GetNColumns()) )
    {
        // size of the new board is different from the currently loaded one
        // boards and players must be adapted to this new size before loading
        // its data on the objects
        m_board = Board(
            a_nRows, 
            a_nCols, 
            CHAR_EMPTY);
        
        // save current colour to apply it to the new player created
        uint8_t red, green, blue;
        m_players[e_Game4_Player1].GetColour(red, green, blue);        
        m_players[e_Game4_Player1] = Player(
            _(PLAYER_1_NAME),
            CHAR_PLAYER1,
            a_nRows,
            a_nCols,
            a_startingCoordPlayer1);
        m_players[e_Game4_Player1].SetColour(red, green, blue);
        
        m_players[e_Game4_Player2].GetColour(red, green, blue);        
        m_players[e_Game4_Player2] = Player(
            _(PLAYER_2_NAME),
            CHAR_PLAYER2,
            a_nRows,
            a_nCols,
            a_startingCoordPlayer2);
        m_players[e_Game4_Player2].SetColour(red, green, blue);
        
        m_players[e_Game4_Player3].GetColour(red, green, blue);        
        m_players[e_Game4_Player3] = Player(
            _(PLAYER_3_NAME),
            CHAR_PLAYER3,
            a_nRows,
            a_nCols,
            a_startingCoordPlayer3);
        m_players[e_Game4_Player3].SetColour(red, green, blue);        
        
        m_players[e_Game4_Player4].GetColour(red, green, blue);
        m_players[e_Game4_Player4] = Player(
            _(PLAYER_4_NAME),
            CHAR_PLAYER4,
            a_nRows,
            a_nCols,
            a_startingCoordPlayer4);
        m_players[e_Game4_Player4].SetColour(red, green, blue);
        
        // opponent lists must be updated since m_player[1-4] holds new 
        // Player instances
        ResetOpponentLists();
    }
    
    m_board.Reset();
    m_players[e_Game4_Player1].Reset(a_startingCoordPlayer1);
    m_players[e_Game4_Player2].Reset(a_startingCoordPlayer2);
    m_players[e_Game4_Player3].Reset(a_startingCoordPlayer3);
    m_players[e_Game4_Player4].Reset(a_startingCoordPlayer4);
}

void Game4Players::RemovePiece(
        const Piece      &a_piece,
        const Coordinate &a_coord,
        eGame4_Player_t   a_player)
{
#ifdef DEBUG
    assert(a_player >= e_Game4_Player1 && a_player <= e_Game4_Player4);        
#endif
    
    Game4Players::RemovePiece(
        m_board,
        a_coord,
        a_piece.GetCurrentConfiguration(),
        m_players[a_player],
        m_playerOpponentLists[a_player]);

    // piece is again available
    m_players[a_player].SetPiece(a_piece.GetType());
}

void Game4Players::PutDownPiece(        
        const Piece      &a_piece,
        const Coordinate &a_coord,
        eGame4_Player_t   a_player)
{
#ifdef DEBUG
    assert(a_player >= e_Game4_Player1 && a_player <= e_Game4_Player4);        
#endif
    
    Game4Players::PutDownPiece(
        m_board,
        a_coord,
        a_piece.GetCurrentConfiguration(),
        m_players[a_player],
        m_playerOpponentLists[a_player]);

    // piece is not available now
    m_players[a_player].UnsetPiece(a_piece.GetType());
}

void Game4Players::RemovePiece(
        Board                    &a_theBoard,
        const Coordinate         &a_coord,
        const PieceConfiguration &a_pieceConf,
        Player                   &a_playerMe,
        std::list<Player*>       &a_opponentList)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_row < a_theBoard.GetNRows());

    assert(a_coord.m_col >= 0);
    assert(a_coord.m_col < a_theBoard.GetNColumns());
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
        		((a_coord.m_row + it->m_row) < a_theBoard.GetNRows()) );
        assert( ((a_coord.m_col + it->m_col) >= 0) &&
        		((a_coord.m_col + it->m_col) < a_theBoard.GetNColumns()) );

        assert(a_theBoard.IsPlayerInCoord(
                thisCoord,
        		a_playerMe));
#endif

        // remove the current square of the piece from the board
        a_theBoard.BlankCoord(thisCoord);
    }

    // go through the list of squares of the piece again now that
    // the piece has been removed
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        // is it now a nucleation point for me or the opponents? 
        // (it couldn't be before, as it was occupied by 'me')
        if (rules::IsNucleationPointCompute(
                a_theBoard,
        		a_playerMe,
        		thisCoord))
        {
        	a_playerMe.SetNucleationPoint(thisCoord);
        } // if (rules::IsNucleationPointCompute
        
        for (std::list<Player*>::iterator player_it = a_opponentList.begin();
             player_it != a_opponentList.end();
             player_it++)
        {
            if (rules::IsNucleationPointCompute(
                    a_theBoard,
                    *(*player_it),
                    thisCoord))
            {
                (*player_it)->SetNucleationPoint(thisCoord);
            }
            
            // is the current coord influenced by this opponent 
            // (it couldn't be before as this square was occupied by me)
            if (rules::IsCoordInfluencedByPlayerFastCompute(
                    a_theBoard,
                    thisCoord,
                    *(*player_it)))
            {
                (*player_it)->SetInfluencedCoord(thisCoord);
            }
        } // for std::list<Player*>::iterator player_it

    } // for (it  = a_pieceConf.m_pieceSquares.begin();

    // now check the nk points of the piece. Are they still nk points for player 'me'?
    for (it  = a_pieceConf.m_nkPoints.begin();
         it != a_pieceConf.m_nkPoints.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        if ( (thisCoord.m_row < 0) || (thisCoord.m_col < 0)     ||
             (thisCoord.m_row >= a_theBoard.GetNRows())         ||
             (thisCoord.m_col >= a_theBoard.GetNColumns())      ||
             (a_playerMe.IsNucleationPoint(thisCoord) == false) )
        {
            // this point is out of the board (or is not an nk
            // point). Try next one
            continue;
        }

        if (!rules::IsNucleationPointCompute(
                a_theBoard,
        		a_playerMe,
        		thisCoord))
        {
            // this nk point is not nk point any more since the piece has been
            // removed
        	a_playerMe.UnsetNucleationPoint(thisCoord);
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
             (thisCoord.m_row >= a_theBoard.GetNRows())     ||
             (thisCoord.m_col >= a_theBoard.GetNColumns())  ||
             (a_theBoard.IsCoordEmpty(thisCoord) == false)  )
        {
            // this forbidden coord is out of the board (or occupied). Try next one
            continue;
        }

#ifdef DEBUG
        assert(a_playerMe.IsNucleationPoint(thisCoord) == false);
#endif

        if (rules::IsNucleationPointCompute(
                a_theBoard,
        		a_playerMe,
        		thisCoord))
        {
            // this forbidden coord is now a nk point since the piece was removed
        	a_playerMe.SetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_forbiddenArea.begin();

    // recalculate the influence area around the piece that was just removed
    rules::RecalculateInfluenceAreaAroundPieceFast(a_theBoard, a_coord, a_pieceConf, a_playerMe);
}

void Game4Players::PutDownPiece(
        Board                    &a_theBoard,
        const Coordinate         &a_coord,
        const PieceConfiguration &a_pieceConf,
        Player                   &a_playerMe,
        std::list<Player*>       &a_opponentList)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_row < a_theBoard.GetNRows());

    assert(a_coord.m_col >= 0);
    assert(a_coord.m_col < a_theBoard.GetNColumns());
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
        		((a_coord.m_row + it->m_row) < a_theBoard.GetNRows()) );
        assert( ((a_coord.m_col + it->m_col) >= 0) &&
        		((a_coord.m_col + it->m_col) < a_theBoard.GetNColumns()) );

        assert(a_theBoard.IsCoordEmpty(thisCoord));
#endif
        // put down this square of the piece
        a_theBoard.SetPlayerInCoord(
                thisCoord,
        		a_playerMe);

        // can't be a nucleation point of player 'me' any more
        a_playerMe.UnsetNucleationPoint(thisCoord);

        // this new point is being occupied by the player 'me'. It can't be a nucleation point
        // or belong to the influence area of the opponents any more
        for (std::list<Player*>::iterator player_it = a_opponentList.begin();
             player_it != a_opponentList.end();
             player_it++)
        {
            (*player_it)->UnsetNucleationPoint(thisCoord);
            (*player_it)->UnsetInfluencedCoord(thisCoord);
        } // for std::list<Player*>::iterator player_it
        
    } // for (it  = a_pieceConf.m_pieceSquares.begin();

    // now check the nk points of the piece. If they are inside the board they will be set as
    // nucleation point. No need to compute
    for (it  = a_pieceConf.m_nkPoints.begin();
         it != a_pieceConf.m_nkPoints.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        if ( (thisCoord.m_row >= 0)                       &&
             (thisCoord.m_col >= 0)                       &&
             (thisCoord.m_row < a_theBoard.GetNRows())    &&
             (thisCoord.m_col < a_theBoard.GetNColumns()) &&
             (a_theBoard.IsCoordEmpty(thisCoord))         &&
             (!rules::IsCoordTouchingPlayerCompute(a_theBoard, thisCoord, a_playerMe)))
        {
            // this coord is now a valid nk point (if it already was a nk point
            // it's ok -and faster- to set it twice
        	a_playerMe.SetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_nkPoints.begin();

    // forbidden areas around the piece can't be a valid nk point any longer
    for (it  = a_pieceConf.m_forbiddenArea.begin();
         it != a_pieceConf.m_forbiddenArea.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        if ( (thisCoord.m_row >= 0) &&
             (thisCoord.m_col >= 0) &&
             (thisCoord.m_row < a_theBoard.GetNRows()) &&
             (thisCoord.m_col < a_theBoard.GetNColumns()) )
        {
            // this coord is not a nk point any longer since it belongs
            // to the forbidden area of the deployed piece
            a_playerMe.UnsetNucleationPoint(thisCoord);
        }
    } // for (it  = a_pieceConf.m_forbiddenArea.begin();

    // recalculate the influence area around the piece that was just put down
    rules::RecalculateInfluenceAreaAroundPieceFast(a_theBoard, a_coord, a_pieceConf, a_playerMe);
}

Game4Players::eGame4_Player_t Game4Players::GetNextPlayerType(eGame4_Player_t &a_player)
{
#ifdef DEBUG
    assert(a_player >= e_Game4_Player1 && a_player <= e_Game4_Player4);        
#endif
    
    if (a_player == e_Game4_Player1)
    {
        return e_Game4_Player2;
    }
    else if (a_player == e_Game4_Player2)
    {
        return e_Game4_Player3;
    }
    else if (a_player == e_Game4_Player3)
    {
        return e_Game4_Player4;
    }
    else if (a_player == e_Game4_Player4)
    {
        return e_Game4_Player1;
    }
    else
    {
#ifdef DEBUG
        assert(0);
#endif
        // avoid warnings. This does nothing really
        return e_Game4_NoPlayer;
    }
}
