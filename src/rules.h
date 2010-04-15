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
/// @file  rules.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 07-Sept-2009 Original development
/// @endhistory
///
// ============================================================================

#ifndef RULES_H_
#define RULES_H_

#include "board.h"
#include "piece.h"
#include "player.h"
#include "coordinate.h"

/// Size of the valid coords arrays
const int8_t VALID_COORDS_SIZE = 5;

/// @brief this class contains all the methods that define any blokus game
class Rules
{
public:
    Rules();
    virtual ~Rules();

    /// return true if the piece can be deployed in the position defined by the 3rd and 4th parameters
    /// it checks:
    ///   1) if the space that the piece will occupy is empty (and inside the board)
    ///   2) if the piece is touching at least one nucleation point
    ///   3) if the piece is not touching one of a_player's pieces in an illegal way
    /// this function will check if every square occupied by the piece is a nucleation point
    /// If the user is trying to deploy a piece in a specific nucleation point he/she might
    /// want to try to use IsDeployableInNKPoint, which takes the nk point as parameter
    /// and it doesn't check if every square occupied by the piece is a nk point
    /// it doesn't rotate or mirror the piece
    /// @param the blokus board
    /// @param the piece
    /// @param coord x of the piece
    /// @param coord y of the piece
    /// @param the player that owns the pieces
    static bool IsPieceDeployable(
            const Board      &a_board,
            const Piece      &a_piece,
            const Coordinate &a_coord,
            const Player     &a_player);

    /// return true if the piece can be deployed in the position defined by the 3rdparameter
    /// it checks:
    ///   1) if the space that the piece will occupy is empty (and inside the board)
    ///   2) if the piece is not touching another piece of 'a_player'
    ///   3) if the piece will be occupying the nucleation point defined by the 4th
    /// it doesn't rotate or mirror the piece
    /// @param the blokus board
    /// @param the piece
    /// @param coord of the piece
    /// @param coord of the nucleation point
    /// @param the player that owns the pieces
    static bool IsPieceDeployableInNKPoint(
            const Board      &a_board,
            const Piece      &a_piece,
            const Coordinate &a_coord,
            const Coordinate &a_nkPoint,
            const Player     &a_player);

    /// return true if the piece can be deployed in the position defined by the 3rdparameter
    /// it checks:
    ///   1) if the space that the piece will occupy is empty (and inside the board)
    ///   2) if the piece will be occupying the starting point defined by the 4th
    /// it doesn't
    ///   1) check if the piece is not touching another piece of 'a_player' (board should be empty)
    ///   2) rotate or mirror the piece
    /// @param the blokus board
    /// @param the piece
    /// @param coord of the piece
    /// @param coord of the starting point
    static bool IsPieceDeployableInStartingPoint(
            const Board      &a_board,
            const Piece      &a_piece,
            const Coordinate &a_coord,
            const Coordinate &a_startingPoint);

    /// Calculates using the current state of the board if a point in the board is
    /// a nucleation point for a specific player
    /// Note that it is called Compute because it doesn't read the value from a variable
    /// It has to calculate it reading from the actual board
    /// This function should only be called when the blokus board changes (putting down
    /// or removing a piece). If you want to know if a specific point of the board
    /// is a nk for a determined player use the accesor IsNucleationPoint in the class Player
    /// @param the player whose nucleation points are being calculated
    /// @param the ABSOLUTE coord X of the place to be checked
    /// @param the ABSOLUTE coord Y of the place to be checked
    /// @returns true if it is, false otherwise
    static bool IsNucleationPointCompute(
            const Board  &a_board,
            const Player &a_player,
            int32_t      a_coordX,
            int32_t      a_coordY);

    /// the pieces are described in such a way that the middle of the piece (0,0) is always a square and
    /// the maximum number of squares to the left/right or up/down from that point is 2, so every piece
    /// fits in a square defined in the interval [-2,2] in both x and Y coords
    ///
    /// knowing this, if we know a nucleation point the piece must fit (if it does) in that interval
    /// i.e. the little L piece
    ///
    /// X
    /// X X X X
    ///   |
    /// (0,0)
    ///
    /// the point (0,0) is in the middle of the piece. In the case we want to deploy this piece by a full square
    ///
    ///     X
    ///     X X X X
    /// X X
    /// X X
    ///
    /// so, if the nucleation point is (2,2) we'll have to move the piece 1 coord to the right for the deployment
    /// in the most extreme case:
    ///
    /// X
    /// X X X X
    ///         X X
    ///         X X
    ///
    /// the piece will have to be moved 2 points to the left
    ///
    /// The result will be saved in the 5th parameter. The user has to ensure the size of the array is big enough (for example
    /// 5, since it is the size of the piece) If the size of the array wasn't big enough this function will save a_size
    /// elements in out_validCoords, but it will return a number bigger than a_size
    /// the user must check a_nkPointCoord is a nk point before calling the function
    /// @param the blokus board
    /// @param the piece
    /// @param coord of the nucleation point
    /// @param the player who needs the valid coords
    /// @param array where the list of absolute coords where the piece can be deployed in that nucleation point will be saved
    /// @param size of the array
    /// @return the number of nucleation points saved into the output array
    static int32_t CalculateValidCoordsInNucleationPoint(
            const Board      &a_board,
            const Piece      &a_piece,
            const Coordinate &a_nkPointCoord,
            const Player     &a_player,
            Coordinate       out_validCoords[],
            int32_t          a_size);

    /// @brief returns true if there's a way to deploy the piece in this coord, even if the piece has to be moved around
    ///        it doesn't rotate or mirror the piece
    ///        it is based on CalculateValidCoordsInNucleationPoint, but it doesn't save the valid coords, it just
    ///        returns once it finds a valid coord for the piece to be deployed
    static bool HasValidCoordInNucleationPoint(
            const Board      &a_board,
            const Piece      &a_piece,
            const Coordinate &a_nkPointCoord,
            const Player     &a_player);

    /// see comment in CalculateValidCoordsInNucleationPoint
    /// The result will be saved in the 5th parameter. The user has to ensure the size of the array is big enough (for example
    /// 5, since it is the size of the piece) If the size of the array wasn't big enough this function will overwrite random memory
    /// running in release mode since the check has been deleted to improve performance
    /// the user must check a_startingPointCoord is empty before calling the function
    /// @param the blokus board
    /// @param the piece
    /// @param coord of the starting point
    /// @param the player who needs the valid coords
    /// @param array where the list of absolute coords where the piece can be deployed in that starting point will be saved
    /// @param size of the array
    /// @return the number of nucleation points saved into the output array
    static int32_t CalculateValidCoordsInStartingPoint(
        const Board      &a_board,
        const Piece      &a_piece,
        const Coordinate &a_startingPointCoord,
        const Player     &a_player,
        Coordinate       out_validCoords[],
        int32_t          a_size);

    /// @brief recalculate the nucleation points using the whole blokus board and save them into the players
    static void RecalculateNKInAllBoard(
    		const Board &a_board,
    		Player      &a_playerMe,
    		Player      &a_playerOpponent);

    /// recalculate the nucleation points around the piece.
    /// all the pieces fit in a square 5x5 pieces. We've got to check a maximum square of 7x7
    /// (1 more row and column at each side: up, down, left and right)
    /// It will update the matrix saved in a_player with the nk points with the new configuration
    /// @param the board
    /// @param the piece
    /// @param the coords
    /// @param the player that owns the piece
    static void RecalculateNKAroundPiece(
    		const Board      &a_board,
            const Piece      &a_piece,
            const Coordinate &a_coord,
            Player           &a_player);

    /// @returns true if the 'a_player' can put down at least one piece on the board
    static bool CanPlayerGo(const Board &a_board, const Player &a_player);
};

#endif /* RULES_H_ */

