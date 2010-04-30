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
/// @file  board.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2009  Original development
/// @endhistory
///
// ============================================================================

#include "board.h"
#include <iostream>
#include <iomanip> // setw

Board::Board(int32_t a_rows, int32_t a_columns, char a_emptyChar, char** a_board):
    m_emptyChar(a_emptyChar),
    m_nRows(a_rows),
    m_nColumns(a_columns)
{
    // allocate the space for the board and set all of it to the emptyChar
	m_theBoard = new char* [m_nRows];
    for (int32_t i = 0; i < m_nRows; i++)
    {
    	m_theBoard[i] = new char[m_nColumns];
        for (int32_t j = 0; j < m_nColumns; j++)
        {
            if (a_board != NULL)
            {
                // in case we have a board to copy do so
                m_theBoard[i][j] = a_board[i][j];
            }
            else
            {
                m_theBoard[i][j] = m_emptyChar;
            }
        }
    }
}

Board::~Board()
{
    // delete the array that saves the board
	for (int32_t i = 0; i < m_nRows; i++)
	{
		delete [] m_theBoard[i];
	}

	delete [] m_theBoard;
}

Board::Board(const Board &a_src)
{
    // allocate the memory needed before copying from object
    this->m_theBoard = new char* [a_src.m_nRows];
    for (int32_t i = 0; i < a_src.m_nRows; i++)
    {
        this->m_theBoard[i] = new char[a_src.m_nColumns];
    }

    CopyFromObject(a_src);
}

Board& Board::operator= (const Board &a_src)
{
    if (this != &a_src)
    {
        // only delete the memory already allocated if the size of the
        // new board is different from "this"
        if ( (this->m_nRows    != a_src.m_nRows)  ||
             (this->m_nColumns != a_src.m_nColumns) )
        {
            // delete the array that currently saves the board configuration
            for (int32_t i = 0; i < this->m_nRows; i++)
            {
                delete [] this->m_theBoard[i];
            }
            delete [] this->m_theBoard;

            // once the old memory is deleted allocate the new memory
            // needed for the new size before copying from object
            this->m_theBoard = new char* [a_src.m_nRows];
            for (int32_t i = 0; i < a_src.m_nRows; i++)
            {
                this->m_theBoard[i] = new char[a_src.m_nColumns];
            }
        }

        // copy the new info from the source object
        CopyFromObject(a_src);
    }

    return *this;
}

void Board::CopyFromObject(const Board &a_src)
{
    this->m_emptyChar = a_src.m_emptyChar;
    this->m_nRows     = a_src.m_nRows;
    this->m_nColumns  = a_src.m_nColumns;

    // set all the coords of the board to the src's
    for (int32_t i = 0; i < a_src.m_nRows; i++)
    {
        for (int32_t j = 0; j < a_src.m_nColumns; j++)
        {
            this->m_theBoard[i][j] = a_src.m_theBoard[i][j];
        }
    }
}

void Board::BitwiseBoardCalculate(
        const Coordinate &a_coord, 
        const Player     &a_player,
        uint64_t         &out_bitwiseBoard,
        uint64_t         &out_bitwisePlayerBoard) const
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < GetNRows());
    assert(a_coord.m_col < GetNColumns());
#endif

    out_bitwiseBoard       = static_cast<uint64_t>(0xffffffffffffffffull);
    out_bitwisePlayerBoard = static_cast<uint64_t>(0xffffffffffffffffull);
    for (int32_t i = (a_coord.m_row - 3); i <= (a_coord.m_row + 3); i++)
    {
        if ( (i < 0) || (i >= GetNRows()) )
        {
            // the whole row is out of the board
            out_bitwiseBoard <<= 7;
            out_bitwiseBoard |= static_cast<uint64_t>(0x7f);
            
            out_bitwisePlayerBoard <<= 7;
            out_bitwisePlayerBoard |= static_cast<uint64_t>(0x7f);
        }
        else
        {
            for (int32_t j = (a_coord.m_col - 3); j <= (a_coord.m_col + 3); j++)
            {
                out_bitwiseBoard       <<= 1;
                out_bitwisePlayerBoard <<= 1;
                if ( (j < 0) || (j >= GetNColumns()))
                {
                    // out of the board
                    out_bitwiseBoard       |= static_cast<uint64_t>(0x01);
                    out_bitwisePlayerBoard |= static_cast<uint64_t>(0x01);
                }
                else if (IsPlayerInCoord(i, j, a_player))
                {
                    // coord occupied by a_player
                    out_bitwiseBoard       |= static_cast<uint64_t>(0x01);
                    out_bitwisePlayerBoard |= static_cast<uint64_t>(0x01);
                }
                else if (IsCoordEmpty(i, j) == false)
                {
                    // someone else is occupying this coordinate
                    out_bitwiseBoard       |= static_cast<uint64_t>(0x01);
                }
            }
        }
    }
}

void Board::BitwiseBoardMoveRight(
        const Coordinate &a_coord, 
        const Player     &a_player,
        uint64_t         &in_out_bitwiseBoard,
        uint64_t         &in_out_bitwisePlayerBoard) const
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < GetNRows());
    assert(a_coord.m_col < GetNColumns());
#endif

    // 1111111 1111111 1111111 111x000 1110000 1110000 1110000
    // will be:
    // 111111y 111111y 111111y 11x000y 110000y 110000y 110000y

    // a_coord contains the current coordinate (before moving to the right)
    // we'll have to calculate a_coord.m_col + 4

    // .... 1111110 1111110 1111110 1111110 1111110 1111110 1111110
    static const uint64_t BLANK_OUT_MOVE_RIGHT = static_cast<uint64_t>(0xfffffbf7efdfbf7eull);

    in_out_bitwiseBoard       <<= 1;
    in_out_bitwisePlayerBoard <<= 1;
    in_out_bitwiseBoard       &= BLANK_OUT_MOVE_RIGHT;
    in_out_bitwisePlayerBoard &= BLANK_OUT_MOVE_RIGHT;
    for (int32_t i = (a_coord.m_row - 3); i <= (a_coord.m_row + 3); i++)
    {
        if ( (i < 0) || (i >= GetNRows()) ||
             ((a_coord.m_col + 4) <= GetNColumns()) )
        {
            // out of the board
            in_out_bitwiseBoard       |= static_cast<uint64_t>(1) << ( (3 - (i - a_coord.m_row)) * 7);
            in_out_bitwisePlayerBoard |= static_cast<uint64_t>(1) << ( (3 - (i - a_coord.m_row)) * 7);
        }
        else if (IsPlayerInCoord(i, a_coord.m_col + 4, a_player))
        {
            // 'a_player' is occupying this coord
            in_out_bitwiseBoard       |= static_cast<uint64_t>(1) << ( (3 - (i - a_coord.m_row)) * 7);
            in_out_bitwisePlayerBoard |= static_cast<uint64_t>(1) << ( (3 - (i - a_coord.m_row)) * 7);
        }
        else if (IsCoordEmpty(i, a_coord.m_col + 4) == false)
        {
            // someone else is occupying this coord
            in_out_bitwiseBoard       |= static_cast<uint64_t>(1) << ( (3 - (i - a_coord.m_row)) * 7);
        }
    }
}

void Board::BitwiseBoardMoveLeft(
        const Coordinate &a_coord, 
        const Player     &a_player,
        uint64_t         &in_out_bitwiseBoard,
        uint64_t         &in_out_bitwisePlayerBoard) const
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < GetNRows());
    assert(a_coord.m_col < GetNColumns());
#endif

    // 1111111 1111111 1111111 111x000 1110000 1110000 1110000
    // will be:
    // y111111 y111111 y111111 y111x00 y111000 y111000 y111000

    // a_coord contains the current coordinate (before moving to the left)
    // we'll have to calculate a_coord.m_col - 4

    // .... 0111111 0111111 0111111 0111111 0111111 0111111 0111111
    static const uint64_t BLANK_OUT_MOVE_LEFT = static_cast<uint64_t>(0xfff7fdfbf7efdfbfull);

    in_out_bitwiseBoard       >>= 1;
    in_out_bitwisePlayerBoard >>= 1;
    in_out_bitwiseBoard       &= BLANK_OUT_MOVE_LEFT;
    in_out_bitwisePlayerBoard &= BLANK_OUT_MOVE_LEFT;
    for (int32_t i = (a_coord.m_row - 3); i <= (a_coord.m_row + 3); i++)
    {
        if ( (i < 0) || (i >= GetNRows()) ||
             ((a_coord.m_col - 4) > 0) )
        {
            // out of the board
            in_out_bitwiseBoard       |= static_cast<uint64_t>(1) << (((3 - (i - a_coord.m_row)) * 7) + 6);
            in_out_bitwisePlayerBoard |= static_cast<uint64_t>(1) << (((3 - (i - a_coord.m_row)) * 7) + 6);
        }
        else if (IsPlayerInCoord(i, a_coord.m_col - 4, a_player))
        {
            // 'a_player' is occupying this coord
            in_out_bitwiseBoard       |= static_cast<uint64_t>(1) << (((3 - (i - a_coord.m_row)) * 7) + 6);
            in_out_bitwisePlayerBoard |= static_cast<uint64_t>(1) << (((3 - (i - a_coord.m_row)) * 7) + 6);
        }
        else if (IsCoordEmpty(i, a_coord.m_col - 4) == false)
        {
            // someone else is occupying this coord
            in_out_bitwiseBoard       |= static_cast<uint64_t>(1) << (((3 - (i - a_coord.m_row)) * 7) + 6);
        }
    }
}

void Board::BitwiseBoardMoveDown(
        const Coordinate &a_coord, 
        const Player     &a_player,
        uint64_t         &in_out_bitwiseBoard,
        uint64_t         &in_out_bitwisePlayerBoard) const
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < GetNRows());
    assert(a_coord.m_col < GetNColumns());
#endif

    // 1111111 1111111 1111111 111x000 1110000 1110000 1110000
    // will be:
    // 1111111 1111111 111x000 1110000 1110000 1110000 yyyyyyy

    // a_coord contains the current coordinate (before moving downwards)
    // we'll have to calculate a_coord.m_row + 4
    if ((a_coord.m_row + 4) >= GetNRows()) // || ((a_coord.m_row + 4) < 0) )
    {
        // row out of the board
        in_out_bitwiseBoard       <<= 7;
        in_out_bitwisePlayerBoard <<= 7;
        in_out_bitwiseBoard       |= static_cast<uint64_t>(0x7f);
        in_out_bitwisePlayerBoard |= static_cast<uint64_t>(0x7f);
    }
    else
    {
        for (int32_t i = (a_coord.m_col - 3); i <= (a_coord.m_col + 3); i++)
        {
            in_out_bitwiseBoard       <<= 1;
            in_out_bitwisePlayerBoard <<= 1;
            if ( (i < 0) || (i >= GetNColumns()) )
            {
                // out of the board
                in_out_bitwiseBoard       |= static_cast<uint64_t>(0x01);
                in_out_bitwisePlayerBoard |= static_cast<uint64_t>(0x01);
            }
            else if (IsPlayerInCoord(a_coord.m_col + 4, i, a_player))
            {
                // 'a_player' is occupying this coord
                in_out_bitwiseBoard       |= static_cast<uint64_t>(0x01);
                in_out_bitwisePlayerBoard |= static_cast<uint64_t>(0x01);
            }
            else if (IsCoordEmpty(a_coord.m_row + 4, i) == false)
            {
                // someone else is occupying this coord
                in_out_bitwiseBoard |= static_cast<uint64_t>(0x01);
            }
        }
    }
}

void Board::SetSquare(char a_char, int32_t a_row, int32_t a_col)
{
#ifdef DEBUG
    assert(a_row >= 0);
    assert(a_row < m_nRows);
    assert(a_col >= 0);
    assert(a_col < m_nColumns);
#endif
    m_theBoard[a_row][a_col] = a_char;
}

void Board::PrintBoard(std::ostream& a_outStream) const
{
    a_outStream << "  ";
    for (int32_t j = 0; j < m_nColumns; j++)
    {
        a_outStream << " " << std::setfill(' ') << std::setw(3) << j;
    }
    a_outStream << std::endl;

    for (int32_t i = 0; i < m_nRows; i++)
    {
        a_outStream << "   +";
        for (int32_t j = 0; j < m_nColumns; j++)
        {
            a_outStream << "---+";
        }
        a_outStream << std::endl;

        a_outStream << std::setfill(' ') << std::setw(2) << i << " |";
        for (int32_t j = 0; j < m_nColumns; j++)
        {
            a_outStream << " " << m_theBoard[i][j] << " |";
        }
        a_outStream << std::endl;
    }

    a_outStream << "   +";
    for (int32_t j = 0; j < m_nColumns; j++)
    {
        a_outStream << "---+";
    }
    a_outStream << std::endl;
}

void Board::Reset()
{
    for (int32_t i = 0; i < m_nRows; i++)
    {
        for (int32_t j = 0; j < m_nColumns; j++)
        {
            m_theBoard[i][j] = m_emptyChar;
        }
    }
}

