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
/// @file  Heuristic.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 31-Mar-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include "Board.h"
#include "Player.h"

/// @brief class used as a container of methods to calculate the heuristic.
/// All those methods have to have the following prototype:
///   static int32_t Calculate(
///          const Board &a_board,
///          const Player &a_playerMe,
///          const Player &a_playerOpponent)
///
/// Note that all of this is a kind of inheritance without virtual functions
/// Abstract classes + inheritance is easier to understand, but virtual functions
/// are more expensive for calculations, and the heuristic function has to be
/// run lots of times. That is why the model changed into pointers to static functions
/// instead of typical parent + children classes.
class Heuristic
{
public:
    /// pointer to the heuristic calculator function
    typedef int32_t (*calculateMethod_t)(const Board &, const Player &, const Player &);

    virtual ~Heuristic();

    /// Calculate the heuristic value taking into account only the amount
    /// Of squares of the pieces put down
    static int32_t CalculateSimple(
            const Board  &a_board,
            const Player &a_playerMe,
            const Player &a_playerOpponent);

    /// Calculate the heuristic value taking into account the weight nucleation points
    /// The more in the middle in the board a NK point is the more important it is
    static int32_t CalculateNKWeighted(
            const Board  &a_board,
            const Player &a_playerMe,
            const Player &a_playerOpponent);

protected:
    /// @brief calculates the circular weight of a coord in the board
    /// The output in a 14x14 board:
    ///    1 1 1 1 1 1 1 1 1 1 1 1 1 1
    ///    1 2 2 2 2 2 2 2 2 2 2 2 1 1
    ///    1 2 2 2 2 2 2 2 2 2 2 2 1 1
    ///    1 2 2 3 3 3 3 3 3 3 2 2 1 1
    ///    1 2 2 3 3 3 3 3 3 3 2 2 1 1
    ///    1 2 2 3 3 4 4 4 3 3 2 2 1 1
    ///    1 2 2 3 3 4 4 4 3 3 2 2 1 1
    ///    1 2 2 3 3 4 4 4 3 3 2 2 1 1
    ///    1 2 2 3 3 3 3 3 3 3 2 2 1 1
    ///    1 2 2 3 3 3 3 3 3 3 2 2 1 1
    ///    1 2 2 2 2 2 2 2 2 2 2 2 1 1
    ///    1 2 2 2 2 2 2 2 2 2 2 2 1 1
    ///    1 1 1 1 1 1 1 1 1 1 1 1 1 1
    ///    1 1 1 1 1 1 1 1 1 1 1 1 1 1
    static int32_t CalculateCircularWeight(
            const Board &a_board, int32_t a_row, int32_t a_column);

    /// @returns true if the point (x, y) in the board is touching any player piece
    ///          note that touching a corner is not 'touching' for this function
    static bool IsPointTouchingPlayer(
            const Board &a_board,
            int32_t x,
            int32_t y,
            const Player &a_player);

private:
    /// @brief constructor
    /// The constructor of the class does nothing. This class is used as a container for
    /// static methods
    Heuristic();
    Heuristic(const Heuristic &src);
    Heuristic& operator=(const Heuristic &src);
};

#endif /* HEURISTIC_H_ */

