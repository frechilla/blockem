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
// with Foobar. If not, see http://www.gnu.org/licenses/.
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

#include "Player.h"
#include "Board.h"
#include "Coordinate.h"

/// @brief A Game in which 1 player tries to allocate all the pieces in the board
///        No opponent. Just one player, one board and the pieces
class GameTotalAllocation
{
public:
	/// @brief builds the game
	/// It creates a board and 1 players, 'me' and the board
	/// using a_rows and a_columns as size
    GameTotalAllocation(int32_t a_rows, int32_t a_columns);
    virtual ~GameTotalAllocation();

	/// @returns the board of the game
	inline const Board& GetBoard() const
	{
	    return m_board;
	}

    /// @brief The player will try to put all the pieces in the board.
    /// It will modify the player and board objects inside this instance
    /// They will save how the calculations ended
    /// @param Coordinate with the starting row and the starting column
    /// @return true if the game was solved. False if there is no solution to the problem
    bool Solve(const Coordinate &a_startingCoord);

private:
    /// The blokus board where the game will be played
    Board m_board;

    /// The player who is supposed to allocate all the pieces in the board
    Player m_player;

    /// remove a piece from the board. The user is supposed to check if the piece was there
    /// before calling this function since it just will set to empty the squares
    /// @param the piece
    /// @param the ABSOLUTE coord
    void RemovePiece(
            const Piece      &a_piece,
            const Coordinate &a_coord);

    /// put down a piece on the board. The user is supposed to check if there is space for
    /// it before calling
    /// @param the piece
    /// @param the ABSOLUTE coord
    void PutDownPiece(
            const Piece      &a_piece,
            const Coordinate &a_coord);

    /// once there is at least one piece put down in the board this function
    /// will try to put down all the pieces in the existing nk points in the board
    /// @param an array with pointers to the latest pieces set in previous
    ///        iterations of the game
    /// @param an array with pointers to sets with the nucleation points of the previous
    ///        itearations of the game
    /// @return true if it succeded. False otherwise
    bool AllocateAllPieces(
            Piece*              a_lastPieces[e_numberOfPieces],
            STLCoordinateSet_t* a_oldNkPoints[e_numberOfPieces]);

    // prevent this class to be instantiated without the proper arguments
    GameTotalAllocation();
};

#endif /* GAMETOTALALLOCATION_H_ */

