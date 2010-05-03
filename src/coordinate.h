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

/// uninitialised coord value
const int32_t COORD_UNINITIALISED = 0xf0000000; // (-268435456)

/// @brief represents a coordinate in the board
class Coordinate
{
public:
	Coordinate():
	    m_row(COORD_UNINITIALISED),
	    m_col(COORD_UNINITIALISED)
	{};
	Coordinate(int32_t a_row, int32_t a_column):
	    m_row(a_row),
	    m_col(a_column)
	{};
	virtual ~Coordinate()
	{}

	/// @brief sets the value of the coordinate
	void Set(int32_t a_row, int32_t a_column)
	{
	    m_row = a_row;
	    m_col = a_column;
	};

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

#endif /* COORDINATE_H_ */
