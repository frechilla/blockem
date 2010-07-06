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
/// @file  game1v1_impl.h
/// @brief Implementation details of inlined functions of the Game1v1 class
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 6-Jul-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef GAME1V1_IMPL_H_
#define GAME1V1_IMPL_H_

inline 
const Board& Game1v1::GetBoard() const
{
    return m_board;
}

inline 
int32_t Game1v1::GetAllNucleationPoints(
        const Player       &a_player,
        CoordinateSet16x16 &a_set)
{
#ifdef DEBUG
    assert(16 >= BOARD_1VS1_ROWS);
    assert(16 >= BOARD_1VS1_COLUMNS);
#endif
    int32_t nNucleationPoints = 0;
    Coordinate thisCoord;

    for (thisCoord.m_row = 0; thisCoord.m_row < BOARD_1VS1_ROWS ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < BOARD_1VS1_COLUMNS ; thisCoord.m_col++)
        {
            if (a_player.IsNucleationPoint(thisCoord))
            {
                a_set.insert(thisCoord);
                nNucleationPoints++;
            }
        }
    }

    return nNucleationPoints;
}

#if 0
/// Get all the nucleation points in the board. Save the results in a special kind of set
/// without memory allocation on the heap
/// this method is slower (around 2%) than the method above
/// @param set to save the nucleation points
/// @return the number of nucleation points saved into the output set
template<int32_t ROWS, int32_t COLUMNS>
inline 
int32_t Game1v1::GetAllNucleationPoints(
        const Player                 &a_player,
        CoordinateSet<ROWS, COLUMNS> &a_set)
{
#ifdef DEBUG
    assert(ROWS    == BOARD_1VS1_ROWS);
    assert(COLUMNS == BOARD_1VS1_COLUMNS);
#endif
    int32_t nNucleationPoints = 0;
    Coordinate thisCoord;

    for (thisCoord.m_row = 0; thisCoord.m_row < m_nRowsInBoard ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < m_nColumnsInBoard ; thisCoord.m_col++)
        {
            if (a_player.IsNucleationPoint(thisCoord))
            {
                a_set.insert(thisCoord);
                nNucleationPoints++;
            }
        }
    }

    return nNucleationPoints;
}
#endif

#endif // GAME1V1_IMPL_H_