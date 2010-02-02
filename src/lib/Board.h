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
/// @file  Board.h
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
#include <ostream>
#include "assert.h"
#include "Piece.h"
#include "Player.h"

class Board
{
public:
    /// @param a_emptyChar. Character to be used to represent an empty place in the board
    /// @param a_board. array that should be a_rRows wide X a_nColumns height which will
    ///        initialize the board status
    Board(int32_t a_rows, int32_t a_columns, char a_emptyChar = ' ', char** a_board = NULL);
    virtual ~Board();

    /// @brief copy constructor. Needed because there's some dynamic memory
    Board(const Board &a_src);
    /// @brief operator=. Needed because there's some dynamic memory
    Board& operator= (const Board &a_src);

    /// @return true if the position specified by a_coordX is empty
    inline bool IsCoordEmpty(Coordinate a_coord) const
    {
    	return IsCoordEmpty(a_coord.m_X, a_coord.m_Y);
    }
    /// @return true if the position (a_coordX, a_coordY) is empty
    inline bool IsCoordEmpty(int32_t a_coordX, int32_t a_coordY) const
    {
#ifdef DEBUG
    	assert(a_coordX >= 0);
    	assert(a_coordY >= 0);
    	assert(a_coordX < m_nRows);
    	assert(a_coordY < m_nColumns);
#endif
    	return (m_theBoard[a_coordX][a_coordY] == m_emptyChar);
    }

    /// @brief set the coord passed as parameter to empty in the board
    inline void BlankCoord(int32_t a_coordX, int32_t a_coordY)
    {
#ifdef DEBUG
    	assert(a_coordX >= 0);
    	assert(a_coordY >= 0);
    	assert(a_coordX < m_nRows);
    	assert(a_coordY < m_nColumns);
#endif
        m_theBoard[a_coordX][a_coordY] = m_emptyChar;
    }

    /// @return true if the player is in the coords (a_coordX, a_coordY)
    inline bool IsPlayerInCoord(int32_t a_coordX, int32_t a_coordY, const Player &a_player) const
    {
#ifdef DEBUG
    	assert(a_coordX >= 0);
    	assert(a_coordY >= 0);
    	assert(a_coordX < m_nRows);
    	assert(a_coordY < m_nColumns);
#endif
    	return (m_theBoard[a_coordX][a_coordY] == a_player.PresentationChar());
    }

    /// @brief set the coord passed as paramater to be occupied by the player 'a_player'
    inline void SetPlayerInCoord(int32_t a_coordX, int32_t a_coordY, const Player &a_player)
    {
#ifdef DEBUG
    	assert(a_coordX >= 0);
    	assert(a_coordY >= 0);
    	assert(a_coordX < m_nRows);
    	assert(a_coordY < m_nColumns);
#endif
    	m_theBoard[a_coordX][a_coordY] = a_player.PresentationChar();
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
    void SetSquare(char a_char, int32_t a_coordX, int32_t a_coordY);

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
