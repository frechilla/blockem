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
/// @file  game_challenge.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 08-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef __GAME_CHALLENGE_H__
#define __GAME_CHALLENGE_H__

#include "player.h"
#include "board.h"
#include "coordinate.h"
#include "blockem_challenge.h"

/// @brief A Game in which 1 player tries to allocate all the pieces on a board
///        Described by a challenge (see blockem challenge doc)
class GameChallenge
{
public:
	/// @brief builds the game
	/// It creates a board and a player (challenger), 'me' and the board
	/// using a_rows and a_columns as size
    /// @param description of this blockem challenge
    GameChallenge(const BlockemChallenge& a_challenge);
    virtual ~GameChallenge();

	/// @returns the board of the game
	inline const Board& GetBoard() const
	{
	    return m_board;
	}

    /// The player who is supposed to allocate all the pieces in the board
    inline const Player& GetChallenger() const
    {
        return m_challenger;
    }
    
    /// This player isn't supposed to put down any pieces, it just "takes"
    /// a few squares on the board to make it difficult for the challenger to
    /// allocate all its pieces
    inline const Player& GetOpponent() const
    {
        return m_disturber;
    }

    /// @brief set colour of challenger's pieces
    void SetChallengerColour(
	        uint8_t a_red,
	        uint8_t a_green,
	        uint8_t a_blue);
    
    /// @brief set colour of squares taken by "opponent"
    /// opponent is a virtual player, it owns spaces on the board but it does
    /// not put down any pieces on the board
    void SetOpponentTakenSquaresColour(
	        uint8_t a_red,
	        uint8_t a_green,
	        uint8_t a_blue);

    /// @brief reset current blockem challenge game
    /// @param description of the new blockem challenge
    void Reset(const BlockemChallenge& a_blockemChallenge);

    /// put down a piece on the board. The user is supposed to check if there is space for it before calling
    /// It updates the internal player contained in the total allocation game
    /// @param the piece
    /// @param the ABSOLUTE coord
	void PutDownPiece(
			const Piece      &a_piece,
			const Coordinate &a_coord);

    /// remove a piece from the board. The user is supposed to check if the piece was there
    /// before calling this function since it will just set to empty the squares
    /// It updates the internal player contained in the total allocation game
    /// @param the piece
    /// @param the ABSOLUTE coord
	void RemovePiece(
			const Piece      &a_piece,
			const Coordinate &a_coord);

private:
    /// The blokus board where the game will be played
    Board m_board;

    /// The player who is supposed to allocate all the pieces in the board
    Player m_challenger;
    
    /// This player isn't supposed to put down any pieces, it just "takes"
    /// a few squares on the board to make it difficult for the challenger to
    /// allocate all its pieces
    Player m_disturber;
    
    /// @brief load challenge data into this object attributes
    /// @param a const reference to the challenge
    void LoadChallenge(const BlockemChallenge& a_challenge);
    
    /// @brief recalculate the nucleation points using the whole board and save them into the challenger
    void RecalculateNKInAllBoard();

    /// remove a piece from the board. The user is supposed to check if the piece was there
    /// before calling this function since it just will set to empty the squares
    /// It doesn't set the piece back as available to the player
    /// @param the ABSOLUTE coord
    /// @param the piece configuration
    void RemovePiece(
            const Coordinate         &a_coord,
            const PieceConfiguration &a_pieceConf);

    /// put down a piece on the board. The user is supposed to check if there is space for
    /// it before calling. It doesn't unset the piece from player
    /// @param the ABSOLUTE coord
    /// @param the piece configuration
    void PutDownPiece(
            const Coordinate         &a_coord,
            const PieceConfiguration &a_pieceConf);

    // prevent this class to be instantiated without the proper arguments
    GameChallenge();
};

#endif /* __GAME_CHALLENGE_H__ */

