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
/// @file  GameTotalAllocation.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Sep-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef GAMETOTALALLOCATION_H_
#define GAMETOTALALLOCATION_H_

#include "player.h"
#include "board.h"
#include "coordinate.h"

/// @brief A Game in which 1 player tries to allocate all the pieces in the board
///        No opponent. Just one player, one board and the pieces
class GameTotalAllocation
{
public:
	/// @brief builds the game
	/// It creates a board and 1 players, 'me' and the board
	/// using a_rows and a_columns as size
    /// @param number of rows
    /// @param number of columns
    /// @param starting coordinate. If it is an uninitialised coord player
    ///        will be able to start from any place on the board
    GameTotalAllocation(
        int32_t a_rows, 
        int32_t a_columns,
        const Coordinate &a_startingCoord);
    virtual ~GameTotalAllocation();

	/// @returns the board of the game
	inline const Board& GetBoard() const
	{
	    return m_board;
	}
    
    inline const Player& GetPlayer() const
    {
        return m_player;
    }
    
    inline void SetPlayerColour(
	        uint8_t a_red,
	        uint8_t a_green,
	        uint8_t a_blue)
    {
        m_player.SetColour(a_red, a_green, a_blue);
    }
    
    /// @brief reset current total allocation game
    /// @param number of rows
    /// @param number of columns
    /// @param starting coordinate. If it isn't initialised player will be 
    ///        able to start from any place on the board 
    void Reset(int32_t a_rows, int32_t a_columns, const Coordinate &a_startingCoord);

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
    
    /// @brief a backtracking algorithm will try to put all the pieces in the board.
    /// It will modify the player and board objects inside this instance
    /// They will contain how the calculations ended
    /// @return true if the game was solved. False if there is no solution to the problem
    bool Solve();
    
private:
    /// The blokus board where the game will be played
    Board m_board;

    /// The player who is supposed to allocate all the pieces in the board
    Player m_player;
    
    /// starting coordinate. If it isn't initialised player will be able to 
    /// start from any coordinate on the board 
    Coordinate m_startingCoord;

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
    
    /// once there is at least one piece put down in the board this function
    /// will try to put down all the pieces in the existing nk points in the board
    /// @param an array with piece types of the latest pieces set in previous
    ///        iterations of the game
    /// @param an array with pointers to sets with the nucleation points of the previous
    ///        itearations of the game
    /// @return true if it succeeded. False otherwise
    bool AllocateAllPieces(
            ePieceType_t        a_lastPieces[e_numberOfPieces],
            STLCoordinateSet_t* a_oldNkPoints[e_numberOfPieces]);

    // prevent this class to be instantiated without the proper arguments
    GameTotalAllocation();
};

#endif /* GAMETOTALALLOCATION_H_ */

