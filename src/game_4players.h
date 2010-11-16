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
/// @file  game_4players.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 15-Nov-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef __GAME_4PLAYERS_H__
#define __GAME_4PLAYERS_H__

#include "player.h"
#include "board.h"
#include "coordinate.h"
#include "blockem_challenge.h"

/// @brief A Game in which 4 players try to allocate all their pieces
class Game4Players
{
public:

    /// This enum represents a player of a 4player game
    typedef enum
    {
        e_Game4_Player1 = 0,
        e_Game4_Player2,
        e_Game4_Player3,
        e_Game4_Player4,

        e_Game4_NoPlayer,
        
        e_Game4_PlayersCount = e_Game4_NoPlayer,
    } eGame4_Player_t;

	/// @brief builds the game
	/// It creates a board and a 4 players
    Game4Players();
    virtual ~Game4Players();

	/// @returns the board of the game
	inline const Board& GetBoard() const
	{
	    return m_board;
	}

    /// @return a const reference to the requested player
    /// WARNING: ensure a_player is e_Game4_Player[1-4]. Any other value will
    /// lead to unexpected behaviour
    const Player& GetPlayer(eGame4_Player_t a_player) const;
    
    /// @return the type of the player passed as a parameter.
    /// if that player doesn't belong to the Game, player type returned will be undefined
    eGame4_Player_t GetPlayerType(const Player &a_player) const;
    
    /// @brief set colour of a player's pieces
    /// WARNING: ensure a_player is e_Game4_Player[1-4]. Any other value will
    /// lead to unexpected behaviour
    void SetPlayerColour(
            eGame4_Player_t a_player,
	        uint8_t a_red,
	        uint8_t a_green,
	        uint8_t a_blue);
    
    /// @brief reset current blockem challenge game
    /// @param description of the new blockem challenge
    void Reset(
        int32_t a_nRows,
        int32_t a_nCols,
        const Coordinate &a_startingCoordPlayer1,
        const Coordinate &a_startingCoordPlayer2,
        const Coordinate &a_startingCoordPlayer3,
        const Coordinate &a_startingCoordPlayer4);

    /// put down a piece on the board. The user is supposed to check if there is space for it before calling
    /// It updates the internal player contained in the 4players game 
    /// WARNING: ensure a_player is e_Game4_Player[1-4]. Any other value will
    /// lead to unexpected behaviour
    /// @param the piece
    /// @param the ABSOLUTE coord
    /// @param the player whose piece will be put down
	void PutDownPiece(
			const Piece      &a_piece,
			const Coordinate &a_coord,
            eGame4_Player_t   a_player);

    /// remove a piece from the board. The user is supposed to check if the piece was there
    /// before calling this function since it will just set to empty the squares
    /// It updates the internal player contained in the 4players game
    /// WARNING: ensure a_player is e_Game4_Player[1-4]. Any other value will
    /// lead to unexpected behaviour
    /// @param the piece
    /// @param the ABSOLUTE coord
    /// @param the player whose piece will be removed
	void RemovePiece(
			const Piece      &a_piece,
			const Coordinate &a_coord,
            eGame4_Player_t   a_player);

private:
    /// The blockem board where the game will be played
    Board m_board;

    /// player 1, 2, 3 and 4. Have a look at the eGame4_Player_t to know
    /// what e_Game4_PlayersCount is defined to
    Player m_players[e_Game4_PlayersCount];
    
    /// list of opponents for each one of the players
    std::list<Player*> m_playerOpponentLists[4];
    
    /// @brief recalculate the nucleation points using the whole board
    /// Save each nk point found into the corresponding player
    void RecalculateNKInAllBoard();
    
    /// @brief loads opponent lists with pointers to player instances
    void ResetOpponentLists();

    /// remove a piece from the board. The user is supposed to check if the piece was there
    /// before calling this function since it just will set to empty the squares
	/// @param the board
    /// @param the ABSOLUTE coord
    /// @param the piece configuration
    /// @param the player that owns the piece to be removed
	/// @param the list of opponents to a_playerMe
    static void RemovePiece(
            Board                    &a_theBoard,
            const Coordinate         &a_coord,
            const PieceConfiguration &a_pieceConf,
            Player                   &a_playerMe,
            std::list<Player*>       &a_opponentList);
     
    /// put down a piece on the board. The user is supposed to check if there 
    /// is space for it before calling
    /// @param the board
    /// @param the ABSOLUTE coord
    /// @param the piece configuration
    /// @param the player that owns the piece to be removed
	/// @param the list of opponents to a_playerMe
    static void PutDownPiece(
            Board                    &a_theBoard,
            const Coordinate         &a_coord,
            const PieceConfiguration &a_pieceConf,
            Player                   &a_playerMe,
            std::list<Player*>       &a_opponentList);
};

#endif /* __GAME_4PLAYERS_H__ */

