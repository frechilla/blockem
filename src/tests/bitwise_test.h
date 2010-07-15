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
/// @file  bitwise_test.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 15-Jul-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef BITWISETEST_H_
#define BITWISETEST_H_

#include "game1v1.h"
#include "coordinate.h"

/// @brief regression testing for the Bitwise methods
/// this class inherits from Game1v1 to be able to use its players and methods
/// and compare them against the bitwise ones. game1v1_test should be run too
///  to ensure those methods work properly
class BitwiseTest :
    public Game1v1
{
public:
    BitwiseTest();
    virtual ~BitwiseTest();

    /// tests if the (faster) way Game1v1 puts down and removes pieces
    /// has the same results as the basic method that consists on setting
    /// a piece on the board coordinate by coordinate and then cumputing
    /// every coordinate of the board looking for nk points. Both boards
    /// (the game1v1 one and the special for test) have to have the same
    /// nk points in the same places
    void TestGame1v1PutDownAndRemovePiece();

    /// test the bitwise representation against he legacy well tested way
    void DoTest();

private:
    /// checks if a piece can be deployed in thisCoord using both legacy and bitwise representations
    /// and asserts if results match. It also checks if a brand new calculation of bitwise configuration
    /// of the board in thisCoord matches the paramaters bitwiseBoard, bitwiseBoardPlayer1, which are
    /// supposed to have been calculated using Board::BitwiseBoardMoveRight, Board::BitwiseBoardMoveLeft
    /// and Board::BitwiseBoardMoveDown
    void TestBitwiseCheckConfiguration(
            const Coordinate         &thisCoord,
            const PieceConfiguration &piece,
            uint64_t                  bPiece,
            uint64_t                  bitwiseBoard,
            uint64_t                  bitwiseBoardPlayer1);

    /// Test that legacy and bitwise representation of board and pieces match. It uses the board
    /// which is part of the Game1v1, so the caller must modify it before calling to this function
    /// it also uses m_player1 to check whether its pieces can be deployed in each coord of the board
    /// @param number of pieces that were picked to be put down on the board so they can't be checked
    ///        in the this function (player1 already used them)
    /// @param piece type that has been put on the board before calling to the function if a_nUsedPieces is 1
    ///        function will assert it is the only piece which is not checked.
    ///        iot can be set to e_noPiece to prevent the assertion from being run
    void TestBoardAndPiecesBitwise(int32_t a_nUsedPieces, ePieceType_t a_pieceMissing = e_noPiece);
};

#endif /* BITWISETEST_H_ */
