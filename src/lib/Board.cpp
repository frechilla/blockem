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
/// @file  Board.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2009  Original development
/// @endhistory
///
// ============================================================================

#include "Board.h"
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

void Board::SetSquare(char a_char, int32_t a_coordX, int32_t a_coordY)
{
#ifdef DEBUG
    assert(a_coordX >= 0);
    assert(a_coordX < m_nRows);
    assert(a_coordY >= 0);
    assert(a_coordY < m_nColumns);
#endif
    m_theBoard[a_coordX][a_coordY] = a_char;
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

