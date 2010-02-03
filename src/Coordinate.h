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
/// @file  Coordinate.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 11-Sep-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef COORDINATE_H_
#define COORDINATE_H_

/// @brief represents a coordinate in the board
class Coordinate
{
public:
	Coordinate():
		m_X(0),
		m_Y(0)
	{};
	Coordinate(int32_t a_coordX, int32_t a_coordY):
		m_X(a_coordX),
		m_Y(a_coordY)
	{};
	virtual ~Coordinate()
	{}

	/// @brief sets the value of the coordinate
	void Set(int32_t a_coordX, int32_t a_coordY)
	{
		m_X = a_coordX;
		m_Y = a_coordY;
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
			if ( (coord1.m_X < coord2.m_X) ||
				 ( (coord1.m_X == coord2.m_X) && (coord1.m_Y < coord2.m_Y) ) )
			{
				return true;
			}

			return false;
		}
	};

	int32_t m_X;
	int32_t m_Y;
};

#endif /* COORDINATE_H_ */
