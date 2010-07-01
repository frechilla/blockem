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

#include<iostream>
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
    PutDownPiece(a_piece.GetCurrentConfiguration());
}

void PieceTest::PutDownPiece(const PieceConfiguration &a_pieceConf)
{
    // piece will always be deployed in 7,7 for the test
    Coordinate a_coord(7, 7);

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
    RemovePiece(a_piece.GetCurrentConfiguration());
}

void PieceTest::RemovePiece(const PieceConfiguration &a_pieceConf)
{
    // piece will always be deployed in 7,7 for the test
    Coordinate a_coord(7, 7);

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
    std::cout << __FUNCTION__ << " Started" << std::endl << std::endl;

    // test expected shape of the pieces
    std::cout << __FUNCTION__ << ": TestPiece_1BabyPiece";
    std::cout.flush();

    TestPiece_1BabyPiece();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_2TwoPiece";
    std::cout.flush();

    TestPiece_2TwoPiece();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_3LongPiece";
    std::cout.flush();

    TestPiece_3LongPiece();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_3Triangle";
    std::cout.flush();

    TestPiece_3Triangle();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_4LongPiece";
    std::cout.flush();

    TestPiece_4LongPiece();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_4LittleS";
    std::cout.flush();

    TestPiece_4LittleS();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_4LittleT";
    std::cout.flush();

    TestPiece_4LittleT();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_4littleL";
    std::cout.flush();

    TestPiece_4littleL();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_4FullSquare";
    std::cout.flush();

    TestPiece_4FullSquare();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5BigS";
    std::cout.flush();

    TestPiece_5BigS();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5SafPiece";
    std::cout.flush();

    TestPiece_5SafPiece();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5WPiece";
    std::cout.flush();

    TestPiece_5WPiece();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5CuntPiece";
    std::cout.flush();

    TestPiece_5CuntPiece();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5BigPenis";
    std::cout.flush();

    TestPiece_5BigPenis();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5Cross";
    std::cout.flush();

    TestPiece_5Cross();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5HalfSquare";
    std::cout.flush();

    TestPiece_5HalfSquare();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5BigL";
    std::cout.flush();

    TestPiece_5BigL();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5MrT";
    std::cout.flush();

    TestPiece_5MrT();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5SquareAppen";
    std::cout.flush();

    TestPiece_5SquareAppen();

    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5BoringPiece";
    std::cout.flush();

    TestPiece_5BoringPiece();


    std::cout << " [ OK ]" << std::endl;
    std::cout << __FUNCTION__ << ": TestPiece_5TheUltimate";
    std::cout.flush();

    TestPiece_5TheUltimate();

    std::cout << " [ OK ]" << std::endl;


    std::cout << __FUNCTION__ << ": Testing all piece configurations...";
    std::cout.flush();

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
    std::cout << " [ OK ]" << std::endl;

    std::cout << __FUNCTION__ << ": Testing precalculated piece configurations...";
    std::cout.flush();

    m_board.Reset();
    m_player.Reset();
    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        const std::list<PieceConfiguration> &pieceConfList =
            m_player.m_pieces[i].GetPrecalculatedConfs();

        std::list<PieceConfiguration>::const_iterator pieceConfIt;
        for (pieceConfIt = pieceConfList.begin();
             pieceConfIt != pieceConfList.end();
             pieceConfIt++)
        {
            // pieces are all put down in 7,7 (have a look at
            // PutDownPiece and RemovePiece
            Coordinate a_coord(7, 7);

            PutDownPiece(*pieceConfIt);

            PieceConfigurationContainer_t::const_iterator it;
            for (it  = pieceConfIt->m_pieceSquares.begin();
                 it != pieceConfIt->m_pieceSquares.end();
                 it++)
            {
                Coordinate thisCoord(a_coord.m_row + it->m_row,
                                     a_coord.m_col + it->m_col);

                assert( (thisCoord.m_row >= 0) &&
                        (thisCoord.m_row < m_board.GetNRows()) );
                assert( (thisCoord.m_col >= 0) &&
                        (thisCoord.m_col < m_board.GetNColumns()) );
                // this configuration must fit inside the radius specified
                assert(thisCoord.m_row <= (a_coord.m_row + m_player.m_pieces[i].GetRadius()));
                assert(thisCoord.m_row >= (a_coord.m_row - m_player.m_pieces[i].GetRadius()));
                assert(thisCoord.m_col <= (a_coord.m_col + m_player.m_pieces[i].GetRadius()));
                assert(thisCoord.m_col >= (a_coord.m_col - m_player.m_pieces[i].GetRadius()));

                assert(!m_board.IsCoordEmpty(thisCoord));
            }

            for (it  = pieceConfIt->m_nkPoints.begin();
                 it != pieceConfIt->m_nkPoints.end();
                 it++)
            {
                Coordinate thisCoord(a_coord.m_row + it->m_row,
                                     a_coord.m_col + it->m_col);

                if ( (thisCoord.m_row >= 0) &&
                     (thisCoord.m_row < m_board.GetNRows()) &&
                     (thisCoord.m_col >= 0) &&
                     (thisCoord.m_col < m_board.GetNColumns()) )
                {
                    assert(m_player.IsNucleationPoint(thisCoord));
                }
            }

            for (it  = pieceConfIt->m_forbiddenArea.begin();
                 it != pieceConfIt->m_forbiddenArea.end();
                 it++)
            {
                Coordinate thisCoord(a_coord.m_row + it->m_row,
                                     a_coord.m_col + it->m_col);

                if ( (thisCoord.m_row >= 0) &&
                     (thisCoord.m_row < m_board.GetNRows()) &&
                     (thisCoord.m_col >= 0) &&
                     (thisCoord.m_col < m_board.GetNColumns()) )
                {
                    assert(rules::IsCoordTouchingPlayerCompute(m_board, thisCoord, m_player));
                }
            }

            RemovePiece(*pieceConfIt);

            for (it  = pieceConfIt->m_pieceSquares.begin();
                 it != pieceConfIt->m_pieceSquares.end();
                 it++)
            {
                Coordinate thisCoord(a_coord.m_row + it->m_row,
                                     a_coord.m_col + it->m_col);

                assert( (thisCoord.m_row >= 0) &&
                        (thisCoord.m_row < m_board.GetNRows()) );
                assert( (thisCoord.m_col >= 0) &&
                        (thisCoord.m_col < m_board.GetNColumns()) );

                assert(m_board.IsCoordEmpty(thisCoord));
            }

            for (it  = pieceConfIt->m_nkPoints.begin();
                 it != pieceConfIt->m_nkPoints.end();
                 it++)
            {
                Coordinate thisCoord(a_coord.m_row + it->m_row,
                                     a_coord.m_col + it->m_col);

                if ( (thisCoord.m_row >= 0) &&
                     (thisCoord.m_row < m_board.GetNRows()) &&
                     (thisCoord.m_col >= 0) &&
                     (thisCoord.m_col < m_board.GetNColumns()) )
                {
                    assert(!m_player.IsNucleationPoint(thisCoord));
                }
            }

            for (it  = pieceConfIt->m_forbiddenArea.begin();
                 it != pieceConfIt->m_forbiddenArea.end();
                 it++)
            {
                Coordinate thisCoord(a_coord.m_row + it->m_row,
                                     a_coord.m_col + it->m_col);

                if ( (thisCoord.m_row >= 0) &&
                     (thisCoord.m_row < m_board.GetNRows()) &&
                     (thisCoord.m_col >= 0) &&
                     (thisCoord.m_col < m_board.GetNColumns()) )
                {
                    assert(!rules::IsCoordTouchingPlayerCompute(m_board, thisCoord, m_player));
                }
            }

        } // for (pieceConfIt = pieceConfList.begin();
    } // for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)

    std::cout << " [ OK ]" << std::endl;

    std::cout << __FUNCTION__ << ": Testing rotation and mirroring of pieces...";
    std::cout.flush();

    m_player.Reset();
    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        Piece tmpPiece(e_noPiece);
        tmpPiece = m_player.m_pieces[i];
        if (m_player.m_pieces[i].CanMirror())
        {
            tmpPiece.MirrorXAxis();
            tmpPiece.MirrorXAxis();
            m_player.m_pieces[i].MirrorYAxis();
            m_player.m_pieces[i].MirrorYAxis();
        }

        assert(tmpPiece.GetCurrentConfiguration().m_pieceSquares ==
            m_player.m_pieces[i].GetCurrentConfiguration().m_pieceSquares);

        assert(tmpPiece.GetCurrentConfiguration().m_nkPoints ==
            m_player.m_pieces[i].GetCurrentConfiguration().m_nkPoints);

        assert(tmpPiece.GetCurrentConfiguration().m_forbiddenArea ==
            m_player.m_pieces[i].GetCurrentConfiguration().m_forbiddenArea);

        tmpPiece = m_player.m_pieces[i];
        for(int8_t nRots = 0; nRots < m_player.m_pieces[i].GetNRotations(); nRots++)
        {
            tmpPiece.RotateLeft();
            m_player.m_pieces[i].RotateRight();
        }

        assert(tmpPiece.GetCurrentConfiguration().m_pieceSquares ==
            m_player.m_pieces[i].GetCurrentConfiguration().m_pieceSquares);

        assert(tmpPiece.GetCurrentConfiguration().m_nkPoints ==
            m_player.m_pieces[i].GetCurrentConfiguration().m_nkPoints);

        assert(tmpPiece.GetCurrentConfiguration().m_forbiddenArea ==
            m_player.m_pieces[i].GetCurrentConfiguration().m_forbiddenArea);

    } // for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)

    std::cout << " [ OK ]" << std::endl;
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
