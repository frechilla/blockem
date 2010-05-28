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
/// @file  heuristic.h
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

#include "board.h"
#include "player.h"

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
    typedef int32_t (*EvalFunction_t)(const Board &, const Player &, const Player &);

    /// types of heuristic. If you add a heuristic here you MUST add a proper description
    /// to Game1v1Config in the constructor using a Game1v1Config::sHeuristicConfig_t
    typedef enum
    {
        e_heuristicStartCount = 0, // this element must be always 0 and must be at the start

        e_heuristicNKWeightedv1 = e_heuristicStartCount,
        e_heuristicAdvanced,
        e_heuristicCentreFocused,
        e_heuristicSimple,
        e_heuristicRandom,
        e_heuristicAmountOfPieces,

        e_heuristicCount, // stores the amount of heuristics. Must be always at the end
    } eHeuristicType_t;

    /// heuristic data to be used in the heuristics config array
    typedef struct
    {
        eHeuristicType_t m_type;
        EvalFunction_t m_evalFunction;
        std::string m_name;
        std::string m_description;
    } sHeuristicData_t;

    static const sHeuristicData_t m_heuristicData[e_heuristicCount];

    virtual ~Heuristic();

    /// Calculate the heuristic value taking into account only the amount
    /// Of squares of the pieces put down
    static int32_t CalculateSimple(
            const Board  &a_board,
            const Player &a_playerMe,
            const Player &a_playerOpponent);

    /// return a random integer value between 0 and RAND_MAX
    static int32_t CalculateRandom(
            const Board  &a_board,
            const Player &a_playerMe,
            const Player &a_playerOpponent);

    /// Calculate the heuristic value taking into account the weight nucleation points
    /// The more in the middle in the board a NK point is the more important it is
    static int32_t CalculateCentreFocused(
            const Board  &a_board,
            const Player &a_playerMe,
            const Player &a_playerOpponent);

    static int32_t CalculateNKWeightedv1(
            const Board  &a_board,
            const Player &a_playerMe,
            const Player &a_playerOpponent);

    static int32_t CalculateNKWeightedv3(
            const Board  &a_board,
            const Player &a_playerMe,
            const Player &a_playerOpponent);

    /// Calculate the heurisitc value based on the amount of pieces that can be put down
    /// on the board. It uses bitwise representation of pieces and board to improve
    /// calculation times
    static int32_t CalculateNPieces(
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
            const Board &a_board, const Coordinate &a_coord);

    /// @returns the amount of squares (the ones that make up the pieces) that can be deployed
    ///          in a specific nk point
    /// @param the board
    /// @param the player whose pieces will be checked
    /// @param bitwise representation of the board (which places are available)
    /// @param bitwise represntation of 'a_player' pieces on the board
    static int32_t CountSquaresCanBeDeployedBitwise(
            const Board  &a_board,
            const Player &a_player,
            uint64_t      a_bitwiseBoard,
            uint64_t      a_bitwisePlayerBoard);


    /// @calculates a nk based heuristic value of thisCoord
    static int32_t NKBasedHeuristicThisCoord(
            const Board  &a_board,
            const Player &a_playerMe,
            const Player &a_playerOpponent,
            const Coordinate &thisCoord);

    /// @return the size of the maximum piece deployable in a particular NK point
    // it assumes pieces are defined in piece.h in reversed order from 5 to 1 squares (and they are)
    static int32_t BiggestPieceDeployableInNKPointSize(
            const Board      &a_board,
            const Player     &a_player,
            const Coordinate &a_NKPointCoord);

private:
    /// @brief constructor
    /// The constructor of the class does nothing. This class is used as a container for
    /// static methods
    Heuristic();
    Heuristic(const Heuristic &src);
    Heuristic& operator=(const Heuristic &src);
};

#endif /* HEURISTIC_H_ */

