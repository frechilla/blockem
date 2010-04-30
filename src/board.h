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
/// @file  board.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef BOARD_H_
#define BOARD_H_

#include <stdio.h>
#include <stdint.h> // types
#include <ostream>
#include "assert.h"
#include "piece.h"
#include "player.h"

class Board
{
public:
    /// @param a_emptyChar. Character to be used to represent an empty place in the board
    /// @param a_board. array that should be a_nRows height X a_nColumns wide which will
    ///        initialize the board status
    Board(int32_t a_rows, int32_t a_columns, char a_emptyChar = ' ', char** a_board = NULL);
    virtual ~Board();

    /// @brief copy constructor. Needed because there's some dynamic memory
    Board(const Board &a_src);
    /// @brief operator=. Needed because there's some dynamic memory
    Board& operator= (const Board &a_src);

    /// @return true if the position specified by a_coord is empty
    inline bool IsCoordEmpty(Coordinate a_coord) const
    {
    	return IsCoordEmpty(a_coord.m_row, a_coord.m_col);
    }
    /// @return true if the position (a_row, a_col) is empty
    inline bool IsCoordEmpty(int32_t a_row, int32_t a_col) const
    {
#ifdef DEBUG
    	assert(a_row >= 0);
    	assert(a_col >= 0);
    	assert(a_row < m_nRows);
    	assert(a_col < m_nColumns);
#endif
    	return (m_theBoard[a_row][a_col] == m_emptyChar);
    }

    /// @brief set the coord passed as parameter to empty in the board
    inline void BlankCoord(int32_t a_row, int32_t a_col)
    {
#ifdef DEBUG
    	assert(a_row >= 0);
    	assert(a_col >= 0);
    	assert(a_row < m_nRows);
    	assert(a_col < m_nColumns);
#endif
        m_theBoard[a_row][a_col] = m_emptyChar;
    }

    /// @return true if the player is in the coords (a_row, a_col)
    inline bool IsPlayerInCoord(int32_t a_row, int32_t a_col, const Player &a_player) const
    {
#ifdef DEBUG
    	assert(a_row >= 0);
    	assert(a_col >= 0);
    	assert(a_row < m_nRows);
    	assert(a_col < m_nColumns);
#endif
    	return (m_theBoard[a_row][a_col] == a_player.PresentationChar());
    }

    /// @brief set the coord passed as paramater to be occupied by the player 'a_player'
    inline void SetPlayerInCoord(int32_t a_row, int32_t a_col, const Player &a_player)
    {
#ifdef DEBUG
    	assert(a_row >= 0);
    	assert(a_col >= 0);
    	assert(a_row < m_nRows);
    	assert(a_col < m_nColumns);
#endif
    	m_theBoard[a_row][a_col] = a_player.PresentationChar();
    }

    /// Get the number of rows of the board
    inline int32_t GetNRows() const
    {
    	return m_nRows;
    }

    /// Get the number of columns of the board
    inline int32_t GetNColumns() const
    {
    	return m_nColumns;
    }

    /// Print board into the out_stream
    void PrintBoard(std::ostream& a_outStream) const;

    /// blank out the board
    void Reset();

    /// set a square of the board to a particular value
    /// should be used as little as possible. Better to use SetPlayerInCoord
    void SetSquare(char a_char, int32_t a_row, int32_t a_col);

    /// Calculate a 7x7 bitwise representation of the board with center
    /// wherever the parameter a_coord says. It has 1 where the board is taken/busy
    /// and 0 wherever is empty
    /// This 7x7 representation can be used with the bitwise representations
    /// of pieces (have a look at Piece:BuildUpBitwiseRepresentation)
    /// Basically takes a picture of the current state of the board and saves
    /// it in an uint64_t (which is returned)
    /// The bitwise representation of the board is surrounded by 3 rows and 3 cols
    /// which will be always set to 1 (non available) in order to easily (and quickly)
    /// check whether a piece is inside or outside the board. For example, if the board is 14x14
    /// a 7x7 bitwise representation of the coordinate 0,0 is (note that the coordinate 0,0 is
    /// written by an 'x'. 1 means taken and 0 empty)
    /// 1 1 1 1 1 1 1
    /// 1 1 1 1 1 1 1
    /// 1 1 1 1 1 1 1
    /// 1 1 1 x 0 0 0
    /// 1 1 1 0 0 0 0
    /// 1 1 1 0 0 0 0
    /// 1 1 1 0 0 0 0
    ///
    /// which would be: 1111111 1111111 1111111 111x000 1110000 1110000 1110000 = fffffffffe1c3870 (if x is 0)
    /// @param coordinate to be used as center of the 7x7 square
    /// @param player whose pieces will be represented in out_bitwisePlayerBoard
    /// @param 7x7 representation of the whole board (1 means coord is not empty, 0 it is empty)
    /// @param 7x7 representation of the pieces of 'a_player'. 1 means the player is in that specific coord
    ///        0 means player is not in that position
    void BitwiseBoardCalculate(
        const Coordinate &a_coord, 
        const Player     &a_player,
        uint64_t         &out_bitwiseBoard,
        uint64_t         &out_bitwisePlayerBoard) const;

    /// Move right the 7x7 bitwise representaions of the board saved in in_out_bitwiseBoard and
    /// bitwise representation of 'a_player' pieces in in_out_bitwisePlayerBoard
    /// a_coord contains the old coord i.e. before moving to the right
    void BitwiseBoardMoveRight(
        const Coordinate &a_coord, 
        const Player     &a_player,
        uint64_t         &in_out_bitwiseBoard,
        uint64_t         &in_out_bitwisePlayerBoard) const;

    /// Move left the 7x7 bitwise representaions of the board saved in in_out_bitwiseBoard and
    /// bitwise representation of 'a_player' pieces in in_out_bitwisePlayerBoard
    /// a_coord contains the old coord i.e. before moving to the left
    void BitwiseBoardMoveLeft(
        const Coordinate &a_coord, 
        const Player     &a_player,
        uint64_t         &in_out_bitwiseBoard,
        uint64_t         &in_out_bitwisePlayerBoard) const;

    /// Move down the the 7x7 bitwise representaions of the board saved in in_out_bitwiseBoard and
    /// bitwise representation of 'a_player' pieces in in_out_bitwisePlayerBoard
    /// a_coord contains the old coord i.e. before moving downwards
    void BitwiseBoardMoveDown(
        const Coordinate &a_coord, 
        const Player     &a_player,
        uint64_t         &in_out_bitwiseBoard,
        uint64_t         &in_out_bitwisePlayerBoard) const;

private:
    /// the actual board
    char** m_theBoard;
    /// the character that represents an empty place in the board
    char m_emptyChar;
    /// Rows of the board
    int32_t m_nRows;
    /// Columns of the board
    int32_t m_nColumns;

    /// @brief copy the source Board argument into the current object
    /// It doesn't delete or allocate any memory, so when used in
    /// operator= or copy constructor that should be taken care of
    /// before calling to the function
    void CopyFromObject(const Board &a_src);

    // prevent boards to be created without the proper arguments
    Board();
};

#endif /* BOARD_H_ */
