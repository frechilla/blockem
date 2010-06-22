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
/// @file  Game1v1Test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Sep-2009  Original development
///           Faustino Frechilla 06-May-2010  Bitwise configuration check
/// @endhistory
///
// ============================================================================

#include "game1v1_test.h"
#include "bitwise.h"
#ifdef DEBUG_PRINT
#include <iostream>
#endif

Game1v1Test::Game1v1Test() :
    Game1v1()
{
}

Game1v1Test::~Game1v1Test()
{
}

void Game1v1Test::TestBitwiseCheckConfiguration(
    const Coordinate           &thisCoord,
    const pieceConfiguration_t &piece,
    uint64_t                    bPiece,
    uint64_t                    bitwiseBoard,
    uint64_t                    bitwiseBoardPlayer1)
{
    bool isDeployableLegacy;          // is piece deployable using the legacy (old) way
    bool isDeployableBitwise;         // is piece deployable using the bitwise way?
    uint64_t bitwiseBoardTest;        // recalculated locally here to check if it is the same as the
    uint64_t bitwiseBoardPlayer1Test; // bitwise representations calculated moving left, right or downwards

    // recalculate bitwise representations using BitwiseBoardCalculate which goes trhough the whole
    // board to calculate the bitwise representation. Check calculated values against bitwiseBoard
    // and bitwiseBoardPlayer1 which are supposed to have been calculated using Board::BitwiseBoardMoveRight
    // Board::BitwiseBoardMoveLeft or Board::BitwiseBoardMoveDown
    bitwise::BoardCalculate(m_board, thisCoord, m_player1, bitwiseBoardTest, bitwiseBoardPlayer1Test);
    assert(bitwiseBoardTest == bitwiseBoard);
    assert(bitwiseBoardPlayer1Test == bitwiseBoardPlayer1);

    // check if piece is deployable using the legacy old way and bitwise way
    // assert both ways gives same result
    isDeployableLegacy =
        rules::IsPieceDeployable(
                m_board,
                piece,
                thisCoord,
                m_player1);

    isDeployableBitwise =
        bitwise::IsPieceDeployable(
                    bPiece,
                    bitwiseBoard,
                    bitwiseBoardPlayer1);

    //std::cout << isDeployableLegacy << " " << isDeployableBitwise << " "
    //          << std::hex << bPiece << " " << bitwiseBoard << " " << bitwiseBoardPlayer1 << std::endl;
    assert(isDeployableLegacy == isDeployableBitwise);
}

void Game1v1Test::TestBoardAndPiecesBitwise(int32_t a_nUsedPieces, ePieceType_t a_pieceMissing)
{
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << std::endl;
#endif

    // assert params are correct
    if (a_pieceMissing != e_noPiece)
    {
        assert(a_nUsedPieces == 1);
    }

    int32_t nUsedPiecesChecked = 0;

    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        if (!m_player1.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            // this is the only piece that should be missing
            if (a_pieceMissing != e_noPiece)
            {
                assert(a_pieceMissing == static_cast<ePieceType_t>(i));
            }
            nUsedPiecesChecked++;
            continue;
        }

        const std::list<uint64_t> &pieceConfigurations = m_player1.m_pieces[i].GetBitwiseList();
        std::list<uint64_t>::const_iterator it = pieceConfigurations.begin();

        // retrieve legacy precalculated coords
        const std::list<pieceConfiguration_t> &coordConfList =
            m_player1.m_pieces[i].GetPrecalculatedConfs();
        std::list<pieceConfiguration_t>::const_iterator pieceCoordIt;

        for (pieceCoordIt  = coordConfList.begin();
             pieceCoordIt != coordConfList.end();
             pieceCoordIt++)
        {
            // current coordinate being studied
            Coordinate thisCoord(0, 0);

            // bitwise representation of the board. Coord is empty or not
            uint64_t bitwiseBoard;
            // bitwise representation of the board. Coord is occuppied by player1
            uint64_t bitwiseBoardPlayer1;
            bitwise::BoardCalculate(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
            // bitwise representation of current piece
            uint64_t bPiece = (*it);

            // the following loop goes trough the board doing a S-like movement.
            // it goes from left to right, then down, then right to left, then down,
            // then left to right...
            while (true)
            {
                for (thisCoord.m_col = 0;
                     thisCoord.m_col < (m_board.GetNColumns() - 1);
                     thisCoord.m_col += 1)
                {
                    // check
                    TestBitwiseCheckConfiguration(
                        thisCoord,
                        *pieceCoordIt,
                        bPiece,
                        bitwiseBoard,
                        bitwiseBoardPlayer1);

                    bitwise::BoardMoveRight(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                }
                // latest configuration wasn't checked
                // check
                TestBitwiseCheckConfiguration(
                    thisCoord,
                    *pieceCoordIt,
                    bPiece,
                    bitwiseBoard,
                    bitwiseBoardPlayer1);

                if ((thisCoord.m_row + 1) >= m_board.GetNRows())
                {
                    break; // got to the latest row of the board
                }

                // next row
                bitwise::BoardMoveDown(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                thisCoord.m_row++;

                // check moving to the left
                for (thisCoord.m_col = (m_board.GetNColumns() - 1);
                     thisCoord.m_col > 0;
                     thisCoord.m_col -= 1)
                {
                    // check
                    TestBitwiseCheckConfiguration(
                        thisCoord,
                        *pieceCoordIt,
                        bPiece,
                        bitwiseBoard,
                        bitwiseBoardPlayer1);

                    bitwise::BoardMoveLeft(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                }
                // latest configuration wasn't checked
                //check
                TestBitwiseCheckConfiguration(
                    thisCoord,
                    *pieceCoordIt,
                    bPiece,
                    bitwiseBoard,
                    bitwiseBoardPlayer1);

                if ((thisCoord.m_row + 1) >= m_board.GetNRows())
                {
                    break; // got to the latest row of the board
                }

                // next row
                bitwise::BoardMoveDown(m_board, thisCoord, m_player1, bitwiseBoard, bitwiseBoardPlayer1);
                thisCoord.m_row++;
            } // while (true)

            // next bitwise representation (rotate right)
            // if it is the latest rotation and the piece can be mirrored
            // it'll represent the piece mirrored
            it++;

        } // for (pieceCoordIt = coordConfList.begin()

        // asserting we checked all possible configurations
        assert(it == pieceConfigurations.end());

    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)

    // assert we checked as many pieces as we were expected to
    assert (nUsedPiecesChecked == a_nUsedPieces);
}
void Game1v1Test::TestBitwise()
{
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << std::endl;
#endif

    // ensure the game1v1 hasn't been modified
    Reset();

    // 0: empty
    // +: player2
    // X: player1
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 0 0 0 0 0 0
    // 0 0 0 + + + 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 X X X 0 0 0 0 0 0
    // 0 0 0 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Game1v1::PutDownPiece(m_player1.m_pieces[e_5Piece_Cross], Coordinate(6, 6), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
    Game1v1::PutDownPiece(m_player2.m_pieces[e_5Piece_Cross], Coordinate(4, 4), Game1v1::e_Game1v1Player2);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // neither player1 nor player2 have the cross piece now, it won't be tested in this first stage
    // of the test, but they will be tested a bit later, in the 2 following tests
    TestBoardAndPiecesBitwise(1, e_5Piece_Cross);

    // reset the board to launch next test (where only 2 baby pieces are deployed)
    Reset();

    // 0: empty
    // +: player2
    // X: player1
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Game1v1::PutDownPiece(m_player1.m_pieces[e_1Piece_BabyPiece], Coordinate(6, 6), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
    Game1v1::PutDownPiece(m_player2.m_pieces[e_1Piece_BabyPiece], Coordinate(4, 4), Game1v1::e_Game1v1Player2);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // go for the test!
    TestBoardAndPiecesBitwise(1, e_1Piece_BabyPiece);

    // reset the board to launch next test (where only two 3-square triangle pieces are deployed)
    Reset();

    // 0: empty
    // +: player2
    // X: player1
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 0 0 0 0 0 0
    // 0 0 0 + + 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 X X 0 0 0 0 0 0
    // 0 0 0 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0

    // e_3Piece_Triangle is originaly described as:
    //  X(X)
    //    X
    m_player1.m_pieces[e_3Piece_Triangle].RotateLeft();
    Game1v1::PutDownPiece(m_player1.m_pieces[e_3Piece_Triangle], Coordinate(6, 6), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
    m_player2.m_pieces[e_3Piece_Triangle].RotateRight();
    Game1v1::PutDownPiece(m_player2.m_pieces[e_3Piece_Triangle], Coordinate(4, 4), Game1v1::e_Game1v1Player2);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // go for the test!
    TestBoardAndPiecesBitwise(1, e_3Piece_Triangle);


    // 0: empty
    // +: player2
    // X: player1
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 + 0 0 0 X 0 0 0 0 0
    // 0 0 0 + + 0 X X X 0 0 0 0 0
    // 0 0 + 0 0 0 0 0 X 0 0 0 0 0
    // 0 0 + + + 0 X X 0 0 0 0 0 0
    // 0 0 + 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0

    // put down an extra Mr. T piece. e_5Piece_MrT is originally described as:
    //     X
    // X(X)X
    //     X
    Game1v1::PutDownPiece(m_player1.m_pieces[e_5Piece_MrT], Coordinate(4, 7), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);
    m_player2.m_pieces[e_5Piece_MrT].RotateRight();
    m_player2.m_pieces[e_5Piece_MrT].RotateRight();
    Game1v1::PutDownPiece(m_player2.m_pieces[e_5Piece_MrT], Coordinate(6, 3), Game1v1::e_Game1v1Player2);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // go for the test! (2 pieces were deployed on the board before launching the test)
    TestBoardAndPiecesBitwise(2);


    // 0: empty
    // +: player2
    // X: player1
    // 0 0 0 0 0 0 0 0 0 0 X X 0 0
    // 0 0 0 0 0 0 0 0 0 0 X 0 0 0
    // 0 0 0 0 0 0 0 0 0 X X 0 0 0
    // 0 0 0 0 + 0 0 0 X 0 0 0 0 0
    // 0 0 0 + + 0 X X X 0 0 0 0 0
    // 0 0 + 0 0 0 0 0 X 0 0 0 0 0
    // 0 0 + + + 0 X X 0 0 0 0 0 0
    // 0 0 + 0 0 0 X 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0

    // put down an extra Big S piece. e_5Piece_BigS is originally described as:
    //   X X
    //  (X)
    // X X
    Game1v1::PutDownPiece(m_player1.m_pieces[e_5Piece_BigS], Coordinate(1, 10), Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player2);

    // go for the test! (3 pieces were deployed on the board by m_player1 before launching the test)
    TestBoardAndPiecesBitwise(3);
}

void Game1v1Test::TestPieces()
{
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << std::endl;
#endif

    int32_t possibleConfigurations = 0;

    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        do
        {
            int16_t nRotations = 0;
            while(nRotations < GetPlayer(e_Game1v1Player1).m_pieces[i].GetNRotations())
            {
                m_player1.m_pieces[i].RotateRight();
                possibleConfigurations++;
                nRotations++;
            }
            // reset the amount of rotations to right before mirroring the piece
            nRotations = 0;
        } while (m_player1.m_pieces[i].MirrorYAxis());

        m_player1.m_pieces[i].Reset();
    }

    // this is a magic number, but you've got to trust me it is right
    assert(possibleConfigurations == 91);

    // check again with precalculated coords
    possibleConfigurations = 0;
    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        // retrieve legacy precalculated coords
        const std::list<pieceConfiguration_t> &coordConfList =
            m_player1.m_pieces[i].GetPrecalculatedConfs();
        std::list<pieceConfiguration_t>::const_iterator pieceCoordIt;

        for (pieceCoordIt = coordConfList.begin();
             pieceCoordIt != coordConfList.end();
             pieceCoordIt++)
        {
            possibleConfigurations++;
        }
    }

    // this is a magic number, but you've got to trust me it is right
    assert(possibleConfigurations == 91);

#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_1BabyPiece" << std::endl;
#endif
    TestPiece_1BabyPiece();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_2TwoPiece" << std::endl;
#endif
    TestPiece_2TwoPiece();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_3LongPiece" << std::endl;
#endif
    TestPiece_3LongPiece();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_3Triangle" << std::endl;
#endif
    TestPiece_3Triangle();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_4LongPiece" << std::endl;
#endif
    TestPiece_4LongPiece();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_4LittleS" << std::endl;
#endif
    TestPiece_4LittleS();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_4LittleT" << std::endl;
#endif
    TestPiece_4LittleT();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_4littleL" << std::endl;
#endif
    TestPiece_4littleL();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_4FullSquare" << std::endl;
#endif
    TestPiece_4FullSquare();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5BigS" << std::endl;
#endif
    TestPiece_5BigS();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5SafPiece" << std::endl;
#endif
    TestPiece_5SafPiece();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5WPiece" << std::endl;
#endif
    TestPiece_5WPiece();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5CuntPiece" << std::endl;
#endif
    TestPiece_5CuntPiece();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5BigPenis" << std::endl;
#endif
    TestPiece_5BigPenis();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5Cross" << std::endl;
#endif
    TestPiece_5Cross();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5HalfSquare" << std::endl;
#endif
    TestPiece_5HalfSquare();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5BigL" << std::endl;
#endif
    TestPiece_5BigL();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5MrT" << std::endl;
#endif
    TestPiece_5MrT();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5SquareAppen" << std::endl;
#endif
    TestPiece_5SquareAppen();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5BoringPiece" << std::endl;
#endif
    TestPiece_5BoringPiece();
#ifdef DEBUG_PRINT
    std::cout << __FUNCTION__ << ": TestPiece_5TheUltimate" << std::endl;
#endif
    TestPiece_5TheUltimate();
}

void Game1v1Test::TestNKSpiralAlgorithm(eGame1v1Player_t who)
{
// this ifdef is needed to avoid warnings about things not being used because
// they are used inside asserts (which do nothing if NDEBUG is defined)
#ifndef NDEBUG
    // variables to test Get[First|Next]NucleationPointSpiral
    Player::SpiralIterator iterator;
    Coordinate tmpCoord;

    if (GetPlayer(who).NumberOfNucleationPoints() > 0)
    {
        // check GetFirstNucleationPointSpiral works fine
        assert(
            GetPlayer(who).GetFirstNucleationPointSpiral(iterator, tmpCoord));

        // easy to check now if CalculateNextValidCoordInNucleationPoint works as
        // CalculateValidCoordsInNucleationPoint does
        TestCalculateValidCoordsInNKPoint(who, tmpCoord);

        // assert any other nk point is found by the spiral algorithm
        for (int32_t i = 1; i < GetPlayer(who).NumberOfNucleationPoints(); i++)
        {
            assert(
                GetPlayer(who).GetNextNucleationPointSpiral(iterator, tmpCoord));

            // easy to check now if CalculateNextValidCoordInNucleationPoint works as
            // CalculateValidCoordsInNucleationPoint does
            TestCalculateValidCoordsInNKPoint(who, tmpCoord);
        }

        // assert there is no more nk points left
        assert(
            GetPlayer(who).GetNextNucleationPointSpiral(iterator, tmpCoord) == false);
    }
    else
    {
        // assert spiral algorithm also says there is no nk points
        assert(
            GetPlayer(who).GetFirstNucleationPointSpiral(iterator, tmpCoord) == false);
    }
#endif // NDEBUG
}

void Game1v1Test::TestCalculateValidCoordsInNKPoint(eGame1v1Player_t who, const Coordinate &where)
{
    assert(GetPlayer(who).IsNucleationPoint(where));

    std::vector<Coordinate> validCoordVector(PIECE_MAX_SQUARES);
    Coordinate validCoord;
    int32_t nNKPointsSecondMethod;
    int32_t nNKPoints;

    // test is done using a baby piece, a 3square triangle, a 4squares S piece and a saf piece
    Piece pieceArray[4];
    pieceArray[0] = Piece(e_1Piece_BabyPiece); // baby piece
    pieceArray[1] = Piece(e_3Piece_Triangle); // 3 squares triangle
    pieceArray[2] = Piece(e_4Piece_LittleS); // 4 squares S Piece
    pieceArray[3] = Piece(e_5Piece_SafPiece); // 5 squares safPiece

    for (int32_t i = 0; i < 4; i++)
    {
        nNKPoints = rules::CalculateValidCoordsInNucleationPoint(
                                    GetBoard(),
                                    GetPlayer(who),
                                    where,
                                    pieceArray[i].GetCurrentConfiguration(),
                                    pieceArray[i].GetRadius(),
                                    validCoordVector);

        validCoord.m_row = validCoord.m_col = COORD_UNINITIALISED;
        nNKPointsSecondMethod = 0;
        while(rules::CalculateNextValidCoordInNucleationPoint(
                                    GetBoard(),
                                    GetPlayer(who),
                                    where,
                                    pieceArray[i].GetCurrentConfiguration(),
                                    pieceArray[i].GetRadius(),
                                    validCoord))
        {
            nNKPointsSecondMethod++;
        }

        // assert both methods found the same amount of valid coords
        assert(nNKPoints == nNKPointsSecondMethod);
    }
}

void Game1v1Test::TestPiece_1BabyPiece()
{
    // X
    Piece &thisPiece = m_player1.m_pieces[e_1Piece_BabyPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));

    nRotations++;
    thisPiece.RotateRight();
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}

void Game1v1Test::TestPiece_2TwoPiece()
{
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_2Piece_TwoPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    // X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}

void Game1v1Test::TestPiece_3LongPiece()
{
    // X X X
    Piece &thisPiece = m_player1.m_pieces[e_3Piece_LongPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    // X
    // X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_3Triangle()
{
    Piece &thisPiece = m_player1.m_pieces[e_3Piece_Triangle];
    int8_t nRotations = 0;

    // X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    //   X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4LongPiece()
{
    // X X X X
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_LongPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X
    // X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4LittleS()
{
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_LittleS];
    int8_t nRotations = 0;

    //   X X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    thisPiece.Reset(); // the piece is not symmetrical. It must be reset before mirroring
                       // to ensure is has the same shape once it has been mirrored
    assert(thisPiece.MirrorYAxis() == true);
    nRotations = 0;

    // X X
    //   X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    //   X
    // X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4LittleT()
{
    //   X
    // X X X
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_LittleT];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    // X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4littleL()
{
    // X X X
    //     X
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_LittleL];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X
    //   X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis());

    // X X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

	// X X
	//   X
	//   X
	PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
	assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

	RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
	assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
	assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //     X
    // X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X
    // X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_4FullSquare()
{
    // X X
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_4Piece_FullSquare];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BigS()
{
    //   X X
    //   X
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_BigS];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X X
    //     X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);

    // X X
    //   X
    //   X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //     X
    // X X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5SafPiece()
{
    //   X
    //   X X
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_SafPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //   X X
    // X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    // X X X
    //     X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                   // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);

    //   X
    // X X
    //   X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X
    // X X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X
    //   X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //     X
    // X X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5WPiece()
{
    // X X
    //   X X
    //     X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_WPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //     X
    //   X X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    //   X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X X
    // X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 7);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}


void Game1v1Test::TestPiece_5CuntPiece()
{
    // X X X
    // X   X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_CuntPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X
    //   X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X   X
    // X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BigPenis()
{
    // X
    // X
    // X
    // X
    // X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_BigPenis];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 4);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5Cross()
{
    //   X
    // X X X
    //   X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_Cross];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 8);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5HalfSquare()
{
    // X
    // X
    // X X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_HalfSquare];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X
    // X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X
    //     X
    //     X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //     X
    //     X
    // X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5BigL()
{
    // X
    // X
    // X
    // X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_BigL];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

	// X X
	//   X
	//   X
    //   X
	PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
	assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

	RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
	assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
	assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //       X
    // X X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                   // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);

    //   X
    //   X
    //   X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X
    // X
    // X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X X X
    //       X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5MrT()
{
    //     X
    // X X X
    //     X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_MrT];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X
    //   X
    // X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X X
    //   X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5SquareAppen()
{
    // X X
    // X X
    // X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_SquarePlus];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X
    //   X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //   X
    // X X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    // X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);


	// X X
	// X X
	//   X
	PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
	assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

	RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
	assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
	assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
	assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X X
    // X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 5);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}

void Game1v1Test::TestPiece_5BoringPiece()
{
    //     X
    // X X X X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_BoringPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X
    // X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    // X X
    //   X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);


    //   X
    // X X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

	// X
	// X X
	// X
    // X
	PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
	assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
	assert(!GetBoard().IsCoordEmpty(Coordinate(5, 7)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

	RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
	assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
	assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
	assert(GetBoard().IsCoordEmpty(Coordinate(5, 7)));
	assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
	assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X X
    //     X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    //   X
    // X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void Game1v1Test::TestPiece_5TheUltimate()
{
    //   X
    // X X
    // X
    // X
    Piece &thisPiece = m_player1.m_pieces[e_5Piece_TheUltimate];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X
    //     X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //   X
    //   X
    // X X
    // X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    //   X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                   // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);

    // X
    // X X
    //   X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(9, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //     X X
    // X X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 6)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(5, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 4)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X
    // X X
    //   X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(6, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(5, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 8)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(4, 8)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 9)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 7)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X X X
    // X X
    PutDownPiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(!GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 6);
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    TestNKSpiralAlgorithm(Game1v1::e_Game1v1Player1);
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 8)));
    assert(GetBoard().IsCoordEmpty(Coordinate(7, 9)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 7)));
    assert(GetBoard().IsCoordEmpty(Coordinate(8, 6)));
    assert(GetPlayer(Game1v1::e_Game1v1Player1).NumberOfNucleationPoints() == 0);
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 6)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(6, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(7, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(8, 10)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 5)));
    assert(!GetPlayer(Game1v1::e_Game1v1Player1).IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
