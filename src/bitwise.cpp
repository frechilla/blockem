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
/// @file  bitwise.cpp
/// @brief Contains operations to handle bitwise representations of board and pieces
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 6-May-2010  Original development
/// @endhistory
///
// ============================================================================

#include "bitwise.h"

/// while using bitwise representations of the board only the 49 (7x7) least
/// significant bits are used. Therefore the 1st 15 (64 - 49) are the unused bits
/// which can be set to 1 by this flag (or to 0 using ~BITWISE_UNUSED_BITS_FLAG)
static const uint64_t BITWISE_UNUSED_BITS_FLAG = 0xfffe000000000000ull;

void bitwise::BoardCalculate(
        const Board      &a_board,
        const Coordinate &a_coord,
        const Player     &a_player,
        uint64_t         &out_bitwiseBoard,
        uint64_t         &out_bitwisePlayerBoard)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < a_board.GetNRows());
    assert(a_coord.m_col < a_board.GetNColumns());
#endif

    out_bitwiseBoard       = static_cast<uint64_t>(0xffffffffffffffffull);
    out_bitwisePlayerBoard = static_cast<uint64_t>(0x0000000000000000ull);

    Coordinate thisCoord(0, 0);
    for (thisCoord.m_row = (a_coord.m_row - 3);
         thisCoord.m_row <= (a_coord.m_row + 3);
         thisCoord.m_row++)
    {
        if ( (thisCoord.m_row < 0) || (thisCoord.m_row >= a_board.GetNRows()) )
        {
            // the whole row is out of the board
            out_bitwiseBoard <<= 7;
            out_bitwiseBoard |= static_cast<uint64_t>(0x7f);

            // bitwise player board has 0s where the player has no piece. Even if
            // it is outside the board
            out_bitwisePlayerBoard <<= 7;
            //out_bitwisePlayerBoard |= static_cast<uint64_t>(0x7f);
        }
        else
        {
            for (thisCoord.m_col = (a_coord.m_col - 3);
                 thisCoord.m_col <= (a_coord.m_col + 3);
                 thisCoord.m_col++)
            {
                out_bitwiseBoard       <<= 1;
                out_bitwisePlayerBoard <<= 1;
                if ( (thisCoord.m_col < 0) || (thisCoord.m_col >= a_board.GetNColumns()))
                {
                    // out of the board
                    out_bitwiseBoard       |= static_cast<uint64_t>(0x01);
                    //out_bitwisePlayerBoard |= static_cast<uint64_t>(0x01);
                }
                else if (a_board.IsPlayerInCoord(thisCoord, a_player))
                {
                    // coord occupied by a_player
                    out_bitwiseBoard       |= static_cast<uint64_t>(0x01);
                    out_bitwisePlayerBoard |= static_cast<uint64_t>(0x01);
                }
                else if (a_board.IsCoordEmpty(thisCoord) == false)
                {
                    // someone else is occupying this coordinate
                    out_bitwiseBoard       |= static_cast<uint64_t>(0x01);
                }
            } // for (thisCoord.m_col = (a_coord.m_col - 3);
        } // else
    } // for (thisCoord.m_row = (a_coord.m_row - 3);

    // no need to use BITWISE_UNUSED_BITS_FLAG for out_bitwiseBoard when the representation
    // of the board is first created. It serves no purpuse and can save a couple of cycles ;)
    out_bitwisePlayerBoard |= BITWISE_UNUSED_BITS_FLAG;
}

void bitwise::BoardMoveRight(
        const Board      &a_board,
        const Coordinate &a_coord,
        const Player     &a_player,
        uint64_t         &in_out_bitwiseBoard,
        uint64_t         &in_out_bitwisePlayerBoard)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < a_board.GetNRows());
    assert(a_coord.m_col < a_board.GetNColumns());
#endif

    // 1111111 1111111 1111111 111x000 1110000 1110000 1110000
    // will be:
    // 111111y 111111y 111111y 11x000y 110000y 110000y 110000y

    // a_coord contains the current coordinate (before moving to the right)
    // we'll have to calculate a_coord.m_col + 4

    // .... 1111110 1111110 1111110 1111110 1111110 1111110 1111110
    static const uint64_t BLANK_OUT_MOVE_RIGHT = static_cast<uint64_t>(0xfffffbf7efdfbf7eull);

    in_out_bitwiseBoard       <<= 1;
    in_out_bitwisePlayerBoard <<= 1;
    in_out_bitwiseBoard       &= BLANK_OUT_MOVE_RIGHT;
    in_out_bitwisePlayerBoard &= BLANK_OUT_MOVE_RIGHT;

    Coordinate thisCoord(0, a_coord.m_col + 4);
    for (thisCoord.m_row = (a_coord.m_row - 3); thisCoord.m_row <= (a_coord.m_row + 3); thisCoord.m_row++)
    {
        if ( (thisCoord.m_row < 0) ||
             (thisCoord.m_row >= a_board.GetNRows()) ||
             (thisCoord.m_col >= a_board.GetNColumns()) )
        {
            // out of the board
            in_out_bitwiseBoard |=
                static_cast<uint64_t>(1) << ( (3 - (thisCoord.m_row - a_coord.m_row)) * 7);
        }
        else if (a_board.IsPlayerInCoord(thisCoord, a_player))
        {
            // 'a_player' is occupying this coord
            in_out_bitwiseBoard |=
                static_cast<uint64_t>(1) << ( (3 - (thisCoord.m_row - a_coord.m_row)) * 7);
            in_out_bitwisePlayerBoard |=
                static_cast<uint64_t>(1) << ( (3 - (thisCoord.m_row - a_coord.m_row)) * 7);
        }
        else if (a_board.IsCoordEmpty(thisCoord) == false)
        {
            // someone else is occupying this coord
            in_out_bitwiseBoard |=
                static_cast<uint64_t>(1) << ( (3 - (thisCoord.m_row - a_coord.m_row)) * 7);
        }
    }

    // set unused bits to 1
    in_out_bitwiseBoard       |= BITWISE_UNUSED_BITS_FLAG;
    in_out_bitwisePlayerBoard |= BITWISE_UNUSED_BITS_FLAG;
}

void bitwise::BoardMoveLeft(
        const Board      &a_board,
        const Coordinate &a_coord,
        const Player     &a_player,
        uint64_t         &in_out_bitwiseBoard,
        uint64_t         &in_out_bitwisePlayerBoard)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < a_board.GetNRows());
    assert(a_coord.m_col < a_board.GetNColumns());
#endif

    // 1111111 1111111 1111111 111x000 1110000 1110000 1110000
    // will be:
    // y111111 y111111 y111111 y111x00 y111000 y111000 y111000

    // a_coord contains the current coordinate (before moving to the left)
    // we'll have to calculate a_coord.m_col - 4

    // .... 0111111 0111111 0111111 0111111 0111111 0111111 0111111
    static const uint64_t BLANK_OUT_MOVE_LEFT = static_cast<uint64_t>(0xfffefdfbf7efdfbfull);

    in_out_bitwiseBoard       >>= 1;
    in_out_bitwisePlayerBoard >>= 1;
    in_out_bitwiseBoard       &= BLANK_OUT_MOVE_LEFT;
    in_out_bitwisePlayerBoard &= BLANK_OUT_MOVE_LEFT;

    Coordinate thisCoord(0, a_coord.m_col - 4);
    for (thisCoord.m_row = (a_coord.m_row - 3); thisCoord.m_row <= (a_coord.m_row + 3); thisCoord.m_row++)
    {
        if ( (thisCoord.m_row < 0) ||
             (thisCoord.m_row >= a_board.GetNRows()) ||
             (thisCoord.m_col < 0) )
        {
            // out of the board
            in_out_bitwiseBoard |=
                static_cast<uint64_t>(1) << (((3 - (thisCoord.m_row - a_coord.m_row)) * 7) + 6);
        }
        else if (a_board.IsPlayerInCoord(thisCoord, a_player))
        {
            // 'a_player' is occupying this coord
            in_out_bitwiseBoard       |=
                static_cast<uint64_t>(1) << (((3 - (thisCoord.m_row - a_coord.m_row)) * 7) + 6);
            in_out_bitwisePlayerBoard |=
                static_cast<uint64_t>(1) << (((3 - (thisCoord.m_row - a_coord.m_row)) * 7) + 6);
        }
        else if (a_board.IsCoordEmpty(thisCoord) == false)
        {
            // someone else is occupying this coord
            in_out_bitwiseBoard |= static_cast<uint64_t>(1) << (((3 - (thisCoord.m_row - a_coord.m_row)) * 7) + 6);
        }
    }

    // set unused bits to 1
    in_out_bitwiseBoard       |= BITWISE_UNUSED_BITS_FLAG;
    in_out_bitwisePlayerBoard |= BITWISE_UNUSED_BITS_FLAG;
}

void bitwise::BoardMoveDown(
        const Board      &a_board,
        const Coordinate &a_coord,
        const Player     &a_player,
        uint64_t         &in_out_bitwiseBoard,
        uint64_t         &in_out_bitwisePlayerBoard)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_col >= 0);
    assert(a_coord.m_row < a_board.GetNRows());
    assert(a_coord.m_col < a_board.GetNColumns());
#endif

    // 1111111 1111111 1111111 111x000 1110000 1110000 1110000
    // will be:
    // 1111111 1111111 111x000 1110000 1110000 1110000 yyyyyyy

    // a_coord contains the current coordinate (before moving downwards)
    // we'll have to calculate a_coord.m_row + 4
    if ((a_coord.m_row + 4) >= a_board.GetNRows()) // || ((a_coord.m_row + 4) < 0) )
    {
        // row out of the board
        in_out_bitwiseBoard <<= 7;
        in_out_bitwiseBoard |= static_cast<uint64_t>(0x7f);

        // bitwise player board has 0s where the player has no piece. Even if
        // it is outside the board
        in_out_bitwisePlayerBoard <<= 7;
        //in_out_bitwisePlayerBoard |= static_cast<uint64_t>(0x7f);
    }
    else
    {
        Coordinate thisCoord(a_coord.m_row + 4, 0);
        for (thisCoord.m_col = (a_coord.m_col - 3);
             thisCoord.m_col <= (a_coord.m_col + 3);
             thisCoord.m_col++)
        {
            in_out_bitwiseBoard       <<= 1;
            in_out_bitwisePlayerBoard <<= 1;
            if ( (thisCoord.m_col < 0) ||
                 (thisCoord.m_col >= a_board.GetNColumns()) ||
                 (thisCoord.m_row >= a_board.GetNRows()) )
            {
                // out of the board
                in_out_bitwiseBoard |= static_cast<uint64_t>(0x01);
            }
            else if (a_board.IsPlayerInCoord(thisCoord, a_player))
            {
                // 'a_player' is occupying this coord
                in_out_bitwiseBoard       |= static_cast<uint64_t>(0x01);
                in_out_bitwisePlayerBoard |= static_cast<uint64_t>(0x01);
            }
            else if (a_board.IsCoordEmpty(thisCoord) == false)
            {
                // someone else is occupying this coord
                in_out_bitwiseBoard |= static_cast<uint64_t>(0x01);
            }
        }
    }

    // set unused bits to 1
    in_out_bitwiseBoard       |= BITWISE_UNUSED_BITS_FLAG;
    in_out_bitwisePlayerBoard |= BITWISE_UNUSED_BITS_FLAG;
}

