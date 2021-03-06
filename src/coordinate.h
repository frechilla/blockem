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
/// @file  coordinate.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 11-Sep-2009  Original development
///           Faustino Frechilla 02-May-2010  Uninitialised value
/// @endhistory
///
// ============================================================================

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <stdint.h> // for types

const int32_t COORD_MAX_VALUE     =  2147483646; // 2147483647
const int32_t COORD_MIN_VALUE     = -2147483646; // -2147483647
const int32_t COORD_UNINITIALISED = -2147483647; // -2147483648

/// @brief represents a coordinate in the board
class Coordinate
{
public:
    /// @brief builds an uninitialised coordinate
    Coordinate():
        m_row(COORD_UNINITIALISED),
        m_col(COORD_UNINITIALISED)
    {}

    /// @brief builds a coordiante and initialises it with parameters
    /// @param a_row
    /// @param a_column
    Coordinate(int32_t a_row, int32_t a_column):
        m_row(a_row),
        m_col(a_column)
    {}

    virtual ~Coordinate()
    {}

    /// @brief sets the value of the coordinate
    inline void Set(int32_t a_row, int32_t a_column);

    /// @return true if both coords have been initialised
    inline bool Initialised() const;

    inline bool operator== (const Coordinate &a_src) const;

    inline bool operator!= (const Coordinate &a_src) const;

    inline bool operator>= (const Coordinate &a_src) const;

    inline bool operator> (const Coordinate &a_src) const;

    inline bool operator<= (const Coordinate &a_src) const;

    inline bool operator< (const Coordinate &a_src) const;

    ///@brief struct to be used as a comparator.
    /// Implements an operator() which returns true if its first argument is less than its second argument,
    /// and false otherwise
    /// It can be used with std::set like this:
    /// std::set<Coordinate, Coordinate::comparator> thisSet;
    struct comparator
    {
        ///@brief returns true if its first argument is less than its second argument, and false otherwise
        bool operator()(const Coordinate& coord1, const Coordinate& coord2) const
        {
            if ( (coord1.m_row < coord2.m_row) ||
                 ( (coord1.m_row == coord2.m_row) && (coord1.m_col < coord2.m_col) ) )
            {
                return true;
            }

            return false;
        }
    };

    /// row of this coordinate
    int32_t m_row;
    /// column of this coordinate
    int32_t m_col;
};

// include implementation details of inline functions
#include "impl/coordinate_impl.h"


#if 0
/// @brief a magical array of coordinates to be used with STL
template<std::size_t NCOORDS>
class CoordinateArray
{
public:

    inline Coordinate& operator[] (std::size_t n)
    {
        return m_coords[n];
    }

    inline const Coordinate& operator[] (std::size_t n) const
    {
        return m_coords[n];
    }

    inline CoordinateArray<NCOORDS>& operator=(const CoordinateArray<NCOORDS> &a_src)
    {
        for (std::size_t i = 0; i < NCOORDS; i++)
        {
            m_coords[i] = a_src.m_coords[i];
        }

        return *this;
    }

private:
    Coordinate m_coords[NCOORDS];
};
#endif // #if 0


#endif /* COORDINATE_H_ */
