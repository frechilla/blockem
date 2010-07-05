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
/// @file  coordinate_set.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 06-Nov-2009  Original development
///           Faustino Frechilla 23-Jun-2010  Set of sets
/// @endhistory
///
// ============================================================================

#ifndef COORDINATESET_H_
#define COORDINATESET_H_

#include <set> // for the STL based coordinate set
#include "coordinate.h"

/// @brief A coordinate set based on STL containers
typedef std::set<Coordinate, Coordinate::comparator> STLCoordinateSet_t;
#if 0
/// @brief class which adds the methid isPresent so STLSets and
///        customised sets are interchangeable
class STLCoordinateSet_t :
    public std::set<Coordinate, Coordinate::comparator>
{
public:
    STLCoordinateSet_t() :
        std::set<Coordinate, Coordinate::comparator>()
    {};

    ~STLCoordinateSet_t()
    {};

    inline bool isPresent(const Coordinate& a_coord) const
    {
        std::set<Coordinate, Coordinate::comparator>::const_iterator it;
        it = std::set<Coordinate, Coordinate::comparator>::find(a_coord);
        return (it != std::set<Coordinate, Coordinate::comparator>::end());
    }
};
#endif

/// @brief set of coordinates for a 16x16 board (maximum)
class CoordinateSet16x16
{
public:
    CoordinateSet16x16()
    {
        clear();
    };
    ~CoordinateSet16x16()
    {
    };

    /// returns true if the coordinate exists in the CoordinateSet
    inline bool isPresent(const Coordinate &a_coord) const
    {
#ifdef DEBUG
        assert( (a_coord.m_row >= 0) && (a_coord.m_row < 16));
        assert( (a_coord.m_col >= 0) && (a_coord.m_col < 16));
#endif
        uint16_t flag = 0x0001 << a_coord.m_col;
        return ( (flag & m_theSet[a_coord.m_row]) == flag );
    }

    /// insert items into a set
    inline void insert(const Coordinate &a_coord)
    {
#ifdef DEBUG
        assert( (a_coord.m_row >= 0) && (a_coord.m_row < 16));
        assert( (a_coord.m_col >= 0) && (a_coord.m_col < 16));
#endif
        uint16_t flag = 0x0001 << a_coord.m_col;
        if ( (flag & m_theSet[a_coord.m_row]) != flag)
        {
            m_theSet[a_coord.m_row] |= flag;
            m_nElems++;
        }
    }

    /// removes all elements from the set
    void clear()
    {
        for (int32_t i = 0; i < 16; i++)
        {
            m_theSet[i] = 0x0000;
        }

        m_nElems = 0;
    }

    /// returns the number of items in the set
    std::size_t size()
    {
        return m_nElems;
    }

    /// true if the set has no elements
    bool empty()
    {
        return (m_nElems == 0);
    }

private:
    /// this array contains if an element is in the set or not
    uint16_t m_theSet[16];

    /// number of elements in the set
    std::size_t m_nElems;
};


// this coordinate set has been commented out because the one above is a bit faster
// (around 1-2%)
#if 0

/// constant to represent present coordinates in the CoordinateSet
static const int8_t COORDINATESET_PRESENT = 1;
/// constant to represent not present coordinates in the CoordinateSet
static const int8_t COORDINATESET_NOT_PRESENT = 0;

/// @brief set of coordinates
/// This class represents a set of coordinates. It has to be instantiated with the number of ROWS and COLUMNS
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
        assert( (a_coord.m_row >= 0) && (a_coord.m_row < ROWS));
        assert( (a_coord.m_col >= 0) && (a_coord.m_col < COLUMNS));
#endif
        return (m_theSet[a_coord.m_row][a_coord.m_col] == COORDINATESET_PRESENT);
    }

    /// insert items into a set
    inline void insert(const Coordinate &a_coord)
    {
#ifdef DEBUG
        assert( (a_coord.m_row >= 0) && (a_coord.m_row < ROWS));
        assert( (a_coord.m_col >= 0) && (a_coord.m_col < COLUMNS));
#endif
        if (m_theSet[a_coord.m_row][a_coord.m_col] == COORDINATESET_NOT_PRESENT)
        {
            m_theSet[a_coord.m_row][a_coord.m_col] = COORDINATESET_PRESENT;
            m_nElems++;
        }
    }

    /// removes all elements from the set
    void clear()
    {
        for (int32_t i = 0; i < ROWS; i++)
        {
            for (int32_t j = 0; j < COLUMNS; j++)
            {
                m_theSet[i][j] = COORDINATESET_NOT_PRESENT;
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
    std::size_t m_nElems;
};
#endif // ifdef 0

// this set is an idea which hasn't been tested yet
#if 0
/// @brief set of 32 sets of coordinates
/// This class represents a set of sets of coordinates. It has to be instantiated with the number of ROWS and COLUMNS
/// it will create an array[ROWS][COLUMNS], where each bit of each position in the array represent whether a coordinate
/// is contained or not in the set
/// The aim of this class is to save space in memory to make cache fails difficult
template<int32_t ROWS, int32_t COLUMNS>
class SetOf32CoordinateSets
{
public:
    SetOf32CoordinateSets()
    {
        clear();
    };
    ~SetOf32CoordinateSets()
    {
    };

    /// returns true if the coordinate exists in the set identified by a_SetId
    inline bool isPresent(const Coordinate &a_coord, int32_t a_setId) const
    {
#ifdef DEBUG
        assert( (a_coord.m_row >= 0) && (a_coord.m_row < ROWS));
        assert( (a_coord.m_col >= 0) && (a_coord.m_col < COLUMNS));
        assert(a_setId < 32);
#endif
        int32_t flag = (1 << a_setId);
        return ((m_theSet[a_coord.m_row][a_coord.m_col] & flag) == flag);
    }

    /// insert items into the set identified by a_setId
    inline void insert(const Coordinate &a_coord, int32_t a_setId)
    {
#ifdef DEBUG
        assert( (a_coord.m_row >= 0) && (a_coord.m_row < ROWS));
        assert( (a_coord.m_col >= 0) && (a_coord.m_col < COLUMNS));
        assert(a_setId < 32);
#endif
        int32_t flag = (1 << a_setId);
        if ((m_theSet[a_coord.m_row][a_coord.m_col] & a_setId) == 0)
        {
            m_theSet[a_coord.m_row][a_coord.m_col] |= flag;
            m_nElems[a_setId]++;
        }
    }

    /// removes all elements from all the sets
    void clear()
    {
        for (int32_t i = 0; i < ROWS; i++)
        {
            for (int32_t j = 0; j < COLUMNS; j++)
            {
                m_theSet[i][j] = 0x00000000;
            }
        }

        for (int32_t i = 0; i < 32; i++)
        {
            m_nElems[i] = 0;
        }
    }

    // removes all elementes from the set identified by a_setId
    void clear (int32_t a_setId)
    {
#ifdef DEBUG
        assert(a_setId < 32);
#endif
        int32_t flag = (1 << a_setId);

        for (int32_t i = 0; i < ROWS; i++)
        {
            for (int32_t j = 0; j < COLUMNS; j++)
            {
                m_theSet[i][j] &= ~flag;
            }
        }

        m_nElems[a_setId] = 0;
    }

    /// returns the number of items in the set
    int32_t size(int32_t a_setId)
    {
#ifdef DEBUG
        assert(a_setId < 32);
#endif
        return m_nElems[a_setId];
    }

    /// true if the set has no elements
    bool empty(int32_t a_setId)
    {
#ifdef DEBUG
        assert(a_setId < 32);
#endif
        return (m_nElems[a_setId] == 0);
    }

private:
    uint32_t m_theSet[ROWS][COLUMNS];

    int32_t m_nElems[32]; // one per set
};
#endif

#endif /* COORDINATESET_H_ */
