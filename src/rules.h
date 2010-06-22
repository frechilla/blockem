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
const uint8_t VALID_COORDS_SIZE = PIECE_MAX_SQUARES;

/// @brief this namespace contains all the methods that define any blockem game
namespace rules
{
    /// return true if 'a_coord' is touching one of a_player's pieces as
    /// that coord would be an illegal place to put one of a_player's pieces
    /// as it is touching one of its pieces.
    /// note that touching a corner is not 'touching' for this function
    /// @param the blokus board
    /// @param coord (x, y) of the piece
    /// @param the player that owns the pieces
    bool IsCoordTouchingPlayer(
            const Board      &a_board,
            const Coordinate &a_coord,
            const Player     &a_player);

    /// return true if the piece can be deployed in the position defined by the 3th parameter
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
    /// @param the piece configuration
    /// @param coord (x, y) of the piece
    /// @param the player that owns the pieces
    bool IsPieceDeployable(
            const Board                &a_board,
            const pieceConfiguration_t &a_pieceConf,
            const Coordinate           &a_coord,
            const Player               &a_player);

    /// return true if the piece can be deployed in the position defined by the 3rdparameter
    /// it checks:
    ///   1) if the space that the piece will occupy is empty (and inside the board)
    ///   2) if the piece is not touching another piece of 'a_player'
    ///   3) if the piece will be occupying the nucleation point defined by the 4th
    /// it doesn't rotate or mirror the piece
    /// @param the blokus board
    /// @param the piece
    /// @param coord where the piece is going to be deployed
    /// @param coord of the nucleation point the piece must make use of
    /// @param the player that owns the pieces
    bool IsPieceDeployableInNKPoint(
            const Board                &a_board,
            const pieceConfiguration_t &a_pieceConf,
            const Coordinate           &a_coord,
            const Coordinate           &a_nkPoint,
            const Player               &a_player);

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
    /// @param coord of the starting point the piece must make use of
    bool IsPieceDeployableInStartingPoint(
            const Board                &a_board,
            const pieceConfiguration_t &a_pieceConf,
            const Coordinate           &a_coord,
            const Coordinate           &a_startingPoint);

    /// Calculates using the current state of the board if a point in the board is
    /// a nucleation point for a specific player
    /// Note that it is called Compute because it doesn't read the value from a variable
    /// It has to calculate it reading from the actual board
    /// This function should only be called when the blokus board changes (putting down
    /// or removing a piece). If you want to know if a specific point of the board
    /// is a nk for a determined player use the accesor IsNucleationPoint in the class Player
    /// @param the player whose nucleation points are being calculated
    /// @param the ABSOLUTE coord of the place to be checked
    /// @returns true if it is, false otherwise
    bool IsNucleationPointCompute(
            const Board      &a_board,
            const Player     &a_player,
            const Coordinate &a_coord);

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
    /// The result will be saved in the 6th parameter. The user should have initialised the vector to a big enough size
    /// so that inserting elements in vector won't step on random memory locations
    /// A size of PIECE_MAX_SQUARES will do it, since a piece can only be deployed in as many ways as squares it has
    /// @param the blokus board
    /// @param the player who needs the valid coords
    /// @param coord of the nucleation point
    /// @param the piece configuration
    /// @param radius of the piece that will be checked looking for NK points
    /// @param vector where the list of absolute coords where the piece can be deployed in that nucleation point will be saved
    /// @return the number of nucleation points saved into the vector
    int32_t CalculateValidCoordsInNucleationPoint(
            const Board                &a_board,
            const Player               &a_player,
            const Coordinate           &a_nkPointCoord,
            const pieceConfiguration_t &a_pieceConf,
            int32_t                     a_pieceRadius,
            std::vector<Coordinate>    &out_validCoords);

    /// @brief Retrieve next valid coord to deploy a_pieceConf in a_nkPointCoord
    /// It is based on CalculateValidCoordsInNucleationPoint (have a look at it)
    /// It starts from the next position that the one indicated by in_out_validCoord
    /// if in_out_validCoord is uninitialised it will start searching from coordinate
    /// (0, 0). it doesn't rotate or mirror the piece configuration.
    /// If the function returns true the next valid coord is saved into in_out_validCoord
    /// @return true if the function found a valid nk point. False otherwise
    bool CalculateNextValidCoordInNucleationPoint(
            const Board                &a_board,
            const Player               &a_player,
            const Coordinate           &a_nkPointCoord,
            const pieceConfiguration_t &a_pieceConf,
            int32_t                     a_pieceRadius,
            Coordinate                 &in_out_validCoord);

    /// @brief returns true if there's a way to deploy the piece in this coord, even if the piece has to be moved around
    ///        it doesn't rotate or mirror the piece
    ///        it is based on CalculateValidCoordsInNucleationPoint, but it doesn't save the valid coords, it just
    ///        returns once it finds a valid coord for the piece to be deployed
    bool HasValidCoordInNucleationPoint(
            const Board                &a_board,
            const Player               &a_player,
            const Coordinate           &a_nkPointCoord,
            const pieceConfiguration_t &a_pieceConf,
            int32_t                     a_pieceRadius);

    /// see comment in CalculateValidCoordsInNucleationPoint
    /// The result will be saved in the 6th parameter. The user should have initialised the vector to a big enough size
    /// so that inserting elements in vector won't step on random memory locations
    /// A size of PIECE_MAX_SQUARES will do it, since a piece can only be deployed in as many ways as squares it has
    /// the user must check a_startingPointCoord is empty before calling the function
    /// @param the blokus board
    /// @param the player who needs the valid coords
    /// @param coord of the starting point
    /// @param the piece
    /// @param radius of the piece that will be checked looking for NK points
    /// @param vector where the list of absolute coords where the piece can be deployed in that nucleation point will be saved
    /// @return the number of nucleation points saved into the output array
    int32_t CalculateValidCoordsInStartingPoint(
            const Board                &a_board,
            const Player               &a_player,
            const Coordinate           &a_startingPointCoord,
            const pieceConfiguration_t &a_pieceConf,
            int32_t                     a_pieceRadius,
            std::vector<Coordinate>    &out_validCoords);

    /// recalculate the nucleation points around a specific coord.
    /// If the function is to be used to recalculate the NK points after a piece has been deployed
    /// user must take into account that all the pieces fit in a square of 5x5 pieces, but in the case
    /// the piece does need a 5x5 square the fucntion should recalculate a 7x7 square around a_coorf
    /// (1 more row and column at each side: up, down, left and right)
    /// It will update the matrix saved in a_player with the nk points with the new configuration
    /// @param the board
    /// @param the coord
    /// @param the rows/columns at left/right up/down to be recalculated.
    ///        for example, if a square of 5x5 is to be recalculated this param should be 2
    ///        in the case of a square of 7x7, it should be 3.
    /// @param the player that owns the piece
    void RecalculateNKAroundCoord(
    		const Board      &a_board,
            const Coordinate &a_coord,
            int32_t           a_radiusToCheck,
            Player           &a_player);

    /// recalculate the change influence area of a player caused by a piece been deployed
    /// on a specific coordinate
    /// "influence area" of a player "John" is the set of coordinates of the board
    /// that are empty, valid for a possible deployment and are "at reach" of  "John".
    /// The proper way to calculate this should be going through every nucleation point
    /// of the player and try to allocate all his pieces left. This is too slow for what
    /// we need (the influence area was born as a way to improve heuristics), so it
    /// is calculated taking the constant INFLUENCE_AREA_AROUND_SQUARE_SIZE which defines
    /// how many squares of the board  are under the influence of a specific coordinate
    /// (which is supposed to have been taken by out beloved player "John")
    ///
    /// Let's say we deploy the saf piece and the const INFLUENCE_AREA_AROUND_SQUARE_SIZE
    /// is 2. The influece area would be: ('X' is the piece. '#' the influence area)
    ///
    /// +---+---+---+---+---+---+---+
    /// | # | # | # | # | # |   |   |
    /// +---+---+---+---+---+---+---+
    /// | # | # |   | # | # | # | # |
    /// +---+---+---+---+---+---+---+
    /// | # |   | X |   |   | # | # |
    /// +---+---+---+---+---+---+---+
    /// | # |   | X | X | X |   | # |
    /// +---+---+---+---+---+---+---+
    /// | # | # |   | X |   | # | # |
    /// +---+---+---+---+---+---+---+
    /// | # | # | # |   | # | # | # |
    /// +---+---+---+---+---+---+---+
    /// |   | # | # | # | # | # |   |
    /// +---+---+---+---+---+---+---+
    /// @param the board
    /// @param the coord
    /// @param configuration of the piece deployed
    /// @param reference to the player whose indluence area will be changed
    void RecalculateInfluenceAreaAroundPiece(
            const Board                &a_board,
            const Coordinate           &a_coord,
            const pieceConfiguration_t &a_pieceConf,
            Player                     &a_player);

    /// computes if a_coord belongs to the influence area of a_player
    /// @return true if a_coord belongs to the influence area of a_player
    bool IsCoordInfluencedByPlayerCompute(
            const Board       &a_board,
            const Coordinate  &a_coord,
            const Player      &a_player);


    /// @return true if the 'a_player' can put down at least one piece on the board
    bool CanPlayerGo(const Board &a_board, const Player &a_player);
};

#endif /* RULES_H_ */

