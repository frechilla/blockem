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
/// @file  board_impl.h
/// @brief Implementation details of inlined functions of the Board class
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 6-Jul-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef BOARD_IMPL_H_
#define BOARD_IMPL_H_

inline 
bool Board::IsCoordEmpty(const Coordinate &a_coord) const
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < m_nRows);
    assert(a_coord.m_col < m_nColumns);
#endif
    return (m_theBoard[a_coord.m_row][a_coord.m_col] == m_emptyChar);
}

inline 
void Board::BlankCoord(const Coordinate &a_coord)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < m_nRows);
    assert(a_coord.m_col < m_nColumns);
#endif
    m_theBoard[a_coord.m_row][a_coord.m_col] = m_emptyChar;
}

inline 
bool Board::IsPlayerInCoord(const Coordinate &a_coord, const Player &a_player) const
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < m_nRows);
    assert(a_coord.m_col < m_nColumns);
#endif
    return (m_theBoard[a_coord.m_row][a_coord.m_col] == a_player.PresentationChar());
}

inline 
void Board::SetPlayerInCoord(const Coordinate &a_coord, const Player &a_player)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < m_nRows);
    assert(a_coord.m_col < m_nColumns);
#endif
    m_theBoard[a_coord.m_row][a_coord.m_col] = a_player.PresentationChar();
}

inline 
int32_t Board::GetNRows() const
{
    return m_nRows;
}

inline 
int32_t Board::GetNColumns() const
{
    return m_nColumns;
}

#endif // BOARD_IMPL_H_