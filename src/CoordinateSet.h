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
/// @file  CoordinateSet.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 06-Nov-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef COORDINATESET_H_
#define COORDINATESET_H_

#include <set> // for the STL based coordinate set
#include "Coordinate.h"

/// @brief A coordinate set based on the STL containers
typedef std::set<Coordinate, Coordinate::comparator> STLCoordinateSet_t;

/// constant to represent present coordinates in the CoordinateSet
const int8_t COORD_SET_PRESENT = 1;
/// constant to represent not present coordinates in the CoordinateSet
const int8_t COORD_SET_NOT_PRESENT = 0;


/// @brief set of coordinates
/// This class represents a set of coordinates. It has to be instantiated with the number of ROWS nad COLUMNS
/// it will create an array[ROWS][COLUMNS] to represent whether a coordinate is contained or not in the set
template<int32_t ROWS, int32_t COLUMNS>
class CoordinateSet
{
public:
    CoordinateSet()
    {
        clear();
    };
    ~CoordinateSet()
    {
    };

    /// returns true if the coordinate exists in the CoordinateSet
    inline bool isPresent(const Coordinate &a_coord) const
    {
#ifdef DEBUG
        assert( (a_coord.m_X >= 0) && (a_coord.m_X < ROWS));
        assert( (a_coord.m_Y >= 0) && (a_coord.m_Y < COLUMNS));
#endif
        return (m_theSet[a_coord.m_X][a_coord.m_Y] == COORD_SET_PRESENT);
    }

    /// insert items into a set
    inline void insert(const Coordinate &a_coord)
    {
#ifdef DEBUG
        assert( (a_coord.m_X >= 0) && (a_coord.m_X < ROWS));
        assert( (a_coord.m_Y >= 0) && (a_coord.m_Y < COLUMNS));
#endif
        if (m_theSet[a_coord.m_X][a_coord.m_Y] == COORD_SET_NOT_PRESENT)
        {
            m_theSet[a_coord.m_X][a_coord.m_Y] = COORD_SET_PRESENT;
            m_nElems++;
        }
    }

    /// removes all elements from the set
    void clear()
    {
        for (int32_t i = 0; i < ROWS; i++)
        {
            for (int32_t j = 0; j < ROWS; j++)
            {
                m_theSet[i][j] = COORD_SET_NOT_PRESENT;
            }
        }

        m_nElems = 0;
    }

    /// returns the number of items in the set
    int32_t size()
    {
        return m_nElems;
    }

    /// true if the set has no elements
    bool empty()
    {
        return (m_nElems == 0);
    }

private:
    int8_t m_theSet[ROWS][COLUMNS];

    int32_t m_nElems;
};

#endif /* COORDINATESET_H_ */
