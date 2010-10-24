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
/// @file  coordinate_impl.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 24-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef COORDINATE_IMPL_H_
#define COORDINATE_IMPL_H_


inline
void Coordinate::Set(int32_t a_row, int32_t a_column)
{
    m_row = a_row;
    m_col = a_column;
}

inline
bool Coordinate::Initialised() const
{
    return ( (m_row != COORD_UNINITIALISED) &&
             (m_col != COORD_UNINITIALISED) );
}

inline
bool Coordinate::operator== (const Coordinate &a_src) const
{
    return ((this->m_row == a_src.m_row) &&
            (this->m_col == a_src.m_col) );
}

inline
bool Coordinate::operator!= (const Coordinate &a_src) const
{
    return ((this->m_row != a_src.m_row) ||
            (this->m_col != a_src.m_col) );
}

inline
bool Coordinate::operator>= (const Coordinate &a_src) const
{
    return ( (this->m_row > a_src.m_row) ||
             ((this->m_row == a_src.m_row) && (this->m_col >= a_src.m_col)) );
}

inline
bool Coordinate::operator> (const Coordinate &a_src) const
{
    return ( (this->m_row > a_src.m_row) ||
             ((this->m_row == a_src.m_row) && (this->m_col > a_src.m_col)) );
}

inline
bool Coordinate::operator<= (const Coordinate &a_src) const
{
    return (! (*this > a_src));
}

inline
bool Coordinate::operator< (const Coordinate &a_src) const
{
    return (! (*this >= a_src));
}

#endif /* COORDINATE_IMPL_H_ */
