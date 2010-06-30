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
/// @file  piece_test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Jun-2010  Copied most of content from game1v1_test
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include<iostream>
#endif

#include "piece_test.h"
#include "rules.h"

#define ROWS    14
#define COLUMNS 14

PieceTest::PieceTest() :
    m_board(ROWS, COLUMNS),
    m_player(std::string("the player"), 'X', ROWS, COLUMNS)
{
}

PieceTest::~PieceTest()
{
}

void PieceTest::PutDownPiece(const Piece &a_piece)
{
    // piece will always be deployed in 7,7 for the test
    Coordinate a_coord(7, 7);
    const PieceConfiguration &a_pieceConf = a_piece.GetCurrentConfiguration();

    PieceConfigurationContainer_t::const_iterator it;
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        assert( ((a_coord.m_row + it->m_row) >= 0) &&
                ((a_coord.m_row + it->m_row) < m_board.GetNRows()) );
        assert( ((a_coord.m_col + it->m_col) >= 0) &&
                ((a_coord.m_col + it->m_col) < m_board.GetNColumns()) );

        assert(m_board.IsCoordEmpty(thisCoord));

        m_board.SetPlayerInCoord(thisCoord, m_player);
    } // for (it = a_pieceConf.m_pieceSquares.begin();

    // recalculate nk points in the whole board after this piece was put down
    Coordinate thisCoord(0, 0);
    for (thisCoord.m_row = 0 ;
         thisCoord.m_row < m_board.GetNRows();
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0 ;
             thisCoord.m_col < m_board.GetNColumns();
             thisCoord.m_col++)
        {
            if (rules::IsNucleationPointCompute(m_board, m_player, thisCoord))
            {
                m_player.SetNucleationPoint(thisCoord);
            }
            else
            {
                m_player.UnsetNucleationPoint(thisCoord);
            }
        }
    }
}

void PieceTest::RemovePiece(const Piece &a_piece)
{
    // piece will always be deployed in 7,7 for the test
    Coordinate a_coord(7, 7);
    const PieceConfiguration &a_pieceConf = a_piece.GetCurrentConfiguration();

    PieceConfigurationContainer_t::const_iterator it;
    for (it  = a_pieceConf.m_pieceSquares.begin();
         it != a_pieceConf.m_pieceSquares.end();
         it++)
    {
        Coordinate thisCoord(a_coord.m_row + it->m_row,
                             a_coord.m_col + it->m_col);

        assert( ((a_coord.m_row + it->m_row) >= 0) &&
                ((a_coord.m_row + it->m_row) < m_board.GetNRows()) );
        assert( ((a_coord.m_col + it->m_col) >= 0) &&
                ((a_coord.m_col + it->m_col) < m_board.GetNColumns()) );

        assert(m_board.IsPlayerInCoord(thisCoord, m_player));

        // remove the current square of the piece from the board
        m_board.BlankCoord(thisCoord);
    }

    // recalculate nk points in the whole board after this piece was removed
    Coordinate thisCoord(0, 0);
    for (thisCoord.m_row = 0 ;
         thisCoord.m_row < m_board.GetNRows();
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0 ;
             thisCoord.m_col < m_board.GetNColumns();
             thisCoord.m_col++)
        {
            if (rules::IsNucleationPointCompute(m_board, m_player, thisCoord))
            {
                m_player.SetNucleationPoint(thisCoord);
            }
            else
            {
                m_player.UnsetNucleationPoint(thisCoord);
            }
        }
    }
}

void PieceTest::DoTest()
{
#ifdef DEBUG_PRINT
    std::cout << "PieceTest Started..." << std::endl;
    std::cout << __FUNCTION__ << std::endl;
#endif

    int32_t possibleConfigurationsPrecalc = 0;
    int32_t possibleConfigurations = 0;

    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        const std::list<PieceConfiguration> &pieceConfList =
            m_player.m_pieces[i].GetPrecalculatedConfs();

        std::list<PieceConfiguration>::const_iterator pieceConfIt;
        pieceConfIt = pieceConfList.begin();
        do
        {
            int16_t nRotations = 0;
            while(nRotations < m_player.m_pieces[i].GetNRotations())
            {
                m_player.m_pieces[i].RotateRight();
                possibleConfigurations++;
                nRotations++;

                // next precalculated configuration
                pieceConfIt++;
                possibleConfigurationsPrecalc++;
            }
            // reset the amount of rotations to right before mirroring the piece
            nRotations = 0;

        } while (m_player.m_pieces[i].MirrorYAxis());

        assert(pieceConfIt == pieceConfList.end());

        m_player.m_pieces[i].Reset();
    }

    // this is a magic number, but you've got to trust me it is right
    assert(possibleConfigurations == 91);
    assert(possibleConfigurationsPrecalc == 91);

    // test expected configuration of the pieces

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

void PieceTest::TestPiece_1BabyPiece()
{
    // X
    Piece &thisPiece = m_player.m_pieces[e_1Piece_BabyPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 8)));

    nRotations++;
    thisPiece.RotateRight();
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}

void PieceTest::TestPiece_2TwoPiece()
{
    // X X
    Piece &thisPiece = m_player.m_pieces[e_2Piece_TwoPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}

void PieceTest::TestPiece_3LongPiece()
{
    // X X X
    Piece &thisPiece = m_player.m_pieces[e_3Piece_LongPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_3Triangle()
{
    Piece &thisPiece = m_player.m_pieces[e_3Piece_Triangle];
    int8_t nRotations = 0;

    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_4LongPiece()
{
    // X X X X
    Piece &thisPiece = m_player.m_pieces[e_4Piece_LongPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_4LittleS()
{
    Piece &thisPiece = m_player.m_pieces[e_4Piece_LittleS];
    int8_t nRotations = 0;

    //   X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

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
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 1);

    //   X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_4LittleT()
{
    //   X
    // X X X
    Piece &thisPiece = m_player.m_pieces[e_4Piece_LittleT];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_4littleL()
{
    // X X X
    //     X
    Piece &thisPiece = m_player.m_pieces[e_4Piece_LittleL];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X
    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis());

    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

	// X X
	//   X
	//   X
	PutDownPiece(thisPiece);
	assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
	assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
	assert(m_player.NumberOfNucleationPoints() == 5);
	assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
	assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
	assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
	assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
	assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

	RemovePiece(thisPiece);
	assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
	assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
	assert(m_player.NumberOfNucleationPoints() == 0);
	assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
	assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
	assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
	assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
	assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //     X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X
    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_4FullSquare()
{
    // X X
    // X X
    Piece &thisPiece = m_player.m_pieces[e_4Piece_FullSquare];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5BigS()
{
    //   X X
    //   X
    // X X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_BigS];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);

    // X X
    //   X
    //   X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //     X
    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5SafPiece()
{
    //   X
    //   X X
    // X X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_SafPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //   X X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    // X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

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
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X
    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X
    //   X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //     X
    // X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5WPiece()
{
    // X X
    //   X X
    //     X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_WPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //     X
    //   X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 7);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}


void PieceTest::TestPiece_5CuntPiece()
{
    // X X X
    // X   X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_CuntPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X
    //   X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X   X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    // X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5BigPenis()
{
    // X
    // X
    // X
    // X
    // X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_BigPenis];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_player.NumberOfNucleationPoints() == 4);
    assert(m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5Cross()
{
    //   X
    // X X X
    //   X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_Cross];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 8);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5HalfSquare()
{
    // X
    // X
    // X X X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_HalfSquare];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 6)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 6)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X
    //     X
    //     X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //     X
    //     X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5BigL()
{
    // X
    // X
    // X
    // X X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_BigL];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

	// X X
	//   X
	//   X
    //   X
	PutDownPiece(thisPiece);
	assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
	assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
	assert(m_player.NumberOfNucleationPoints() == 5);
	assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
	assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
	assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
	assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
	assert(m_player.IsNucleationPoint(Coordinate(10, 8)));

	RemovePiece(thisPiece);
	assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
	assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
	assert(m_player.NumberOfNucleationPoints() == 0);
	assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
	assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
	assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
	assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
	assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //       X
    // X X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

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
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X
    // X
    // X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X X X
    //       X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5MrT()
{
    //     X
    // X X X
    //     X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_MrT];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X
    //   X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X X
    //   X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5SquareAppen()
{
    // X X
    // X X
    // X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_SquarePlus];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X
    //   X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //   X
    // X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);


	// X X
	// X X
	//   X
	PutDownPiece(thisPiece);
	assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
	assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
	assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
	assert(m_player.NumberOfNucleationPoints() == 5);
	assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
	assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
	assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
	assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
	assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

	RemovePiece(thisPiece);
	assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
	assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
	assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
	assert(m_player.NumberOfNucleationPoints() == 0);
	assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
	assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
	assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
	assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
	assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //   X X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X
    // X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 5);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}

void PieceTest::TestPiece_5BoringPiece()
{
    //     X
    // X X X X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_BoringPiece];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X
    // X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    // X X
    //   X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    nRotations = 0;                    // nRotations is 0 again to rotate the piece
                                      // as many times as it was originally configured to
    assert(thisPiece.MirrorYAxis() == true);


    //   X
    // X X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

	// X
	// X X
	// X
    // X
	PutDownPiece(thisPiece);
	assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
	assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
	assert(!m_board.IsCoordEmpty(Coordinate(5, 7)));
	assert(m_player.NumberOfNucleationPoints() == 6);
	assert(m_player.IsNucleationPoint(Coordinate(4, 6)));
	assert(m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
	assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
	assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
	assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

	RemovePiece(thisPiece);
	assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
	assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
	assert(m_board.IsCoordEmpty(Coordinate(5, 7)));
	assert(m_player.NumberOfNucleationPoints() == 0);
	assert(!m_player.IsNucleationPoint(Coordinate(4, 6)));
	assert(!m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
	assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
	assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
	assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    // X X X X
    //     X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    //   X
    //   X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
void PieceTest::TestPiece_5TheUltimate()
{
    //   X
    // X X
    // X
    // X
    Piece &thisPiece = m_player.m_pieces[e_5Piece_TheUltimate];
    int8_t nRotations = 0;

    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    // X X X
    //     X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 5)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 2);

    //   X
    //   X
    // X X
    // X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 3);

    // X X
    //   X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));

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
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(10, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(9, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(10, 8)));

    thisPiece.RotateRight();nRotations++;
    assert(nRotations == 1);

    //     X X
    // X X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 6)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(5, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(5, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 4)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 2);

    // X
    // X
    // X X
    //   X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 9)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(6, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(5, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 8)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(4, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(4, 8)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 9)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 7)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 9)));

    thisPiece.RotateRight();
    nRotations++;
    assert(nRotations == 3);

    //   X X X
    // X X
    PutDownPiece(thisPiece);
    assert(!m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(!m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(!m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 6);
    assert(m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(m_player.IsNucleationPoint(Coordinate(9, 8)));

    RemovePiece(thisPiece);
    assert(m_board.IsCoordEmpty(Coordinate(7, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 8)));
    assert(m_board.IsCoordEmpty(Coordinate(7, 9)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 7)));
    assert(m_board.IsCoordEmpty(Coordinate(8, 6)));
    assert(m_player.NumberOfNucleationPoints() == 0);
    assert(!m_player.IsNucleationPoint(Coordinate(6, 6)));
    assert(!m_player.IsNucleationPoint(Coordinate(6, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(7, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(8, 10)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 5)));
    assert(!m_player.IsNucleationPoint(Coordinate(9, 8)));

    thisPiece.RotateRight();
    nRotations++;
    assert(thisPiece.GetNRotations() == nRotations);
    assert(thisPiece.MirrorYAxis() == false);

    thisPiece.Reset();
}
