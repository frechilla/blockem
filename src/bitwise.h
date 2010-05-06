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
/// @file  bitwise.h
/// @brief Contains operations to handle bitwise representations of board and pieces
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 6-May-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef _BITWISE_H_
#define _BITWISE_H_

#include <stdint.h> // for types
#include "board.h"
#include "player.h"
#include "piece.h"

/// this namespace contains several methods to create, check and handle
/// bitwise representations of the board and pieces. The final aim is to improve
/// performance of traversing the search tree
namespace bitwise
{
    /// Calculate a 7x7 bitwise representation of 'a_board' with center
    /// wherever the parameter 'a_coord' says. It has 1 where the board is occupied
    /// and 0 wherever it is empty
    /// This 7x7 representation can be used with the rest of bitwise representations
    /// of pieces (have a look at namespace bitwise)
    /// Basically it takes a picture of the current state of a 7x7 square
    /// sourrounding 'a_coord' and saves it in an uint64_t (which is saved in out_bitwiseBoard)
    ///
    /// The hypothetical bitwise representation of the whole board would be surrounded by
    /// 3 rows and 3 cols always set to 1 (non available) in order to easily (and quickly)
    /// check whether a piece is inside or outside the board. For example, if the board is 14x14
    /// a 7x7 bitwise representation of the coordinate 0,0 is (note that the coordinate 0,0 is
    /// written by an 'x'. 1 means taken and 0 empty)
    /// 1 1 1 1 1 1 1
    /// 1 1 1 1 1 1 1
    /// 1 1 1 1 1 1 1
    /// 1 1 1 x 0 0 0
    /// 1 1 1 0 0 0 0
    /// 1 1 1 0 0 0 0
    /// 1 1 1 0 0 0 0
    ///
    /// which would be: 1111111 1111111 1111111 111x000 1110000 1110000 1110000 = fffffffffe1c3870 (if x is 0)
    ///
    /// In the case of out_bitwisePlayerBoard it only has 1s where 'a_player' has pieces deployed
    /// on the board. Any other bit will be set to 0, except for the bits which are not used at the
    /// beginning of the uint64_t, which are always set to 1
    /// @param a const reference to the board which will be used to calculate bitwise representations
    /// @param coordinate to be used as center of the 7x7 square
    /// @param player whose pieces will be represented in out_bitwisePlayerBoard
    /// @param 7x7 representation of the whole board (1 means coord is not empty, 0 it is empty)
    /// @param 7x7 representation of the pieces of 'a_play
    void BoardCalculate(
            const Board      &a_board,
            const Coordinate &a_coord,
            const Player     &a_player,
            uint64_t         &out_bitwiseBoard,
            uint64_t         &out_bitwisePlayerBoard);

    /// Move right (row = old_row. column = old_column + 1) the 7x7 bitwise representations of the board saved
    /// in in_out_bitwiseBoard and the bitwise representation of 'a_player' pieces in in_out_bitwisePlayerBoard
    /// @param const reference to the board which will be used to calculate bitwise representations
    /// @param const reference to the the old coordinate, i.e. before moving to the right
    /// @param const reference to the player whose bitwise representation will be calculated in in_out_bitwisePlayerBoard
    /// @param current bitwise representation of the board. It will be overwritten with the representation
    ///        of the board moved 1 position to the right
    /// @param current bitwise representation of a_player's pieces on the board. It will be overwritten with
    ///        the representation of a_player's pieces moved 1 position to the right
    void BoardMoveRight(
            const Board      &a_board,
            const Coordinate &a_coord,
            const Player     &a_player,
            uint64_t         &in_out_bitwiseBoard,
            uint64_t         &in_out_bitwisePlayerBoard);

    /// Move left (row = old_row. column = old_column - 1) the 7x7 bitwise representations of the board saved
    /// in in_out_bitwiseBoard and the bitwise representation of 'a_player' pieces in in_out_bitwisePlayerBoard
    /// @param const reference to the board which will be used to calculate bitwise representations
    /// @param const reference to the the old coordinate, i.e. before moving to the left
    /// @param const reference to the player whose bitwise representation will be calculated in in_out_bitwisePlayerBoard
    /// @param current bitwise representation of the board. It will be overwritten with the representation
    ///        of the board moved 1 position to the left
    /// @param current bitwise representation of a_player's pieces on the board. It will be overwritten with
    ///        the representation of a_player's pieces moved 1 position to the left
    void BoardMoveLeft(
            const Board      &a_board,
            const Coordinate &a_coord,
            const Player     &a_player,
            uint64_t         &in_out_bitwiseBoard,
            uint64_t         &in_out_bitwisePlayerBoard);

    /// Move down (row = old_row + 1. column = old_column) the 7x7 bitwise representations of the board
    /// saved in in_out_bitwiseBoard and the bitwise representation of 'a_player' pieces
    /// in in_out_bitwisePlayerBoard
    /// @param const reference to the board which will be used to calculate bitwise representations
    /// @param const reference to the the old coordinate, i.e. before moving downwards
    /// @param const reference to the player whose bitwise representation will be calculated in in_out_bitwisePlayerBoard
    /// @param current bitwise representation of the board. It will be overwritten with the representation
    ///        of the board moved 1 position downwards
    /// @param current bitwise representation of a_player's pieces on the board. It will be overwritten with
    ///        the representation of a_player's pieces moved 1 position downwards
    void BoardMoveDown(
            const Board      &a_board,
            const Coordinate &a_coord,
            const Player     &a_player,
            uint64_t         &in_out_bitwiseBoard,
            uint64_t         &in_out_bitwisePlayerBoard);

    /// @brief checks if a_bPiece can be put down in the board
    /// have a look at bitwise:BoardCalculate if you want to know more about bitwise operations
    /// @param bitwise representation of the piece
    /// @param bitwise representation of the whole board
    /// @param bitwise representation of the a_bPiece's owner's (player) pieces on the board
    /// @return true if a_bPiece can be put down on the board
    inline bool IsPieceDeployable(uint64_t a_bPiece, uint64_t a_bitwiseBoard, uint64_t a_bitwisePlayerBoard)
    {
        // does the piece fit in the board?
        if ( (a_bPiece ^ (a_bPiece & a_bitwiseBoard)) == a_bPiece )
        {
            // there's space for this piece in the board. Check if this
            // place is valid

            // does the piece touch the side of another a_player's piece?
            if ( (((a_bPiece << 7) ^ ((a_bPiece << 7) & a_bitwisePlayerBoard)) == (a_bPiece << 7)) && // move up
                 (((a_bPiece >> 7) ^ ((a_bPiece >> 7) & a_bitwisePlayerBoard)) == (a_bPiece >> 7)) && // move down
                 (((a_bPiece << 1) ^ ((a_bPiece << 1) & a_bitwisePlayerBoard)) == (a_bPiece << 1)) && // move left
                 (((a_bPiece >> 1) ^ ((a_bPiece >> 1) & a_bitwisePlayerBoard)) == (a_bPiece >> 1)) )  // move right
            {
                // does it touch a corner (a.k.a. nk point)?
                if ( (((a_bPiece << 8) ^ ((a_bPiece << 8) & a_bitwisePlayerBoard)) != (a_bPiece << 8)) || // up-left corner
                     (((a_bPiece << 6) ^ ((a_bPiece << 6) & a_bitwisePlayerBoard)) != (a_bPiece << 6)) || // up-right corner
                     (((a_bPiece >> 8) ^ ((a_bPiece >> 8) & a_bitwisePlayerBoard)) != (a_bPiece >> 8)) || // down-left corner
                     (((a_bPiece >> 6) ^ ((a_bPiece >> 6) & a_bitwisePlayerBoard)) != (a_bPiece >> 6)) )  // down-right corner
                {
                    return true;
                }
            }
        }

        return false;
    }
}

#endif // _BITWISE_H_
