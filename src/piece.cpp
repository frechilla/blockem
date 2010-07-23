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
/// @file  piece.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2009  Original development
///           Faustino Frechilla 23-Jul-2010  i18n
/// @endhistory
///
// ============================================================================

#include <glib/gi18n.h> // i18n
#include "piece.h"
#include "stdio.h"

/// a piece has to fit in a square (2*2) + 1
static const uint8_t PIECE_MAX_RADIUS = 2;

// bear in mind the description the pieces must match the order in which
// pieces are added to the enum ePieceType_t in piece.h
const std::string Piece::PieceDescription[e_numberOfPieces] =
{
    N_("1Piece_BabyPiece"),
    N_("2Piece_TwoPiece"),
    N_("3Piece_LongPiece"),
    N_("3Piece_Triangle"),
    N_("4Piece_FullSquare"),
    N_("4Piece_LongPiece"),
    N_("4Piece_LittleL"),
    N_("4Piece_LittleS"),
    N_("4Piece_LittleT"),
    N_("5Piece_BigPeni"),
    N_("5Piece_SquarePlus"),
    N_("5Piece_HalfSquare"),
    N_("5Piece_CuntPiece"),
    N_("5Piece_BigL"),
    N_("5Piece_BoringPiece"),
    N_("5Piece_BigS"),
    N_("5Piece_MrT"),
    N_("5Piece_TheUltimate"),
    N_("5Piece_SafPiece"),
    N_("5Piece_WPiece"),
    N_("5Piece_Cross")
};

// instantiate the load piece map.
Piece::LoadPieceFunction_t Piece::m_loadFunctionMap[] =
{
    Piece::LoadPiece_1BabyPiece,    // e_1Piece_BabyPiece
    Piece::LoadPiece_2TwoPiece,	    // e_2Piece_TwoPiece
    Piece::LoadPiece_3LongPiece,    // e_3Piece_LongPiece
    Piece::LoadPiece_3Triangle,     // e_3Piece_Triangle
    Piece::LoadPiece_4FullSquare,   // e_4Piece_FullSquare
    Piece::LoadPiece_4LongPiece,    // e_4Piece_LongPiece
    Piece::LoadPiece_4littleL,      // e_4Piece_LittleL
    Piece::LoadPiece_4LittleS,      // e_4Piece_LittleS
    Piece::LoadPiece_4LittleT,      // e_4Piece_LittleT
    Piece::LoadPiece_5BigPenis,     // e_5Piece_BigPenis
    Piece::LoadPiece_5SquareAppen,  // e_5Piece_SquareAppen
    Piece::LoadPiece_5HalfSquare,   // e_5Piece_HalfSquare
    Piece::LoadPiece_5CuntPiece,    // e_5Piece_CuntPiece
    Piece::LoadPiece_5BigL,         // e_5Piece_BigL
    Piece::LoadPiece_5BoringPiece,  // e_5Piece_BoringPiece
    Piece::LoadPiece_5BigS,         // e_5Piece_BigS
    Piece::LoadPiece_5MrT,          // e_5Piece_MrT
    Piece::LoadPiece_5TheUltimate,  // e_5Piece_TheUltimate
    Piece::LoadPiece_5SafPiece,     // e_5Piece_SafPiece
    Piece::LoadPiece_5WPiece,       // e_5Piece_WPiece
    Piece::LoadPiece_5Cross         // e_5Piece_Cross
};

Piece::Piece(ePieceType_t a_type):
	m_type(a_type)
#ifdef DEBUG
    ,m_initialised(false)
#endif
{
	if (a_type != e_noPiece)
	{
#ifdef DEBUG
	    assert(a_type >= e_minimumPieceIndex);
	    assert(a_type < e_numberOfPieces);
	    assert(a_type != e_noPiece);
#endif
	    // load the legacy piece representation
        (m_loadFunctionMap[a_type])(*this);

        // build up the precalculated configuration too
        BuildUpPrecalculatedRepresentations();
	}
	else
	{
		// no piece loaded
	    std::vector<Coordinate> dummyVector(0);
	    SetPiece(dummyVector, dummyVector, dummyVector, false, 0, 0);
	}
}

Piece::~Piece()
{
}

void Piece::SetPiece(
    const std::vector<Coordinate> &a_pieceCoords,
    const std::vector<Coordinate> &a_nkPoints,
    const std::vector<Coordinate> &a_forbiddenArea,
    bool                           a_mirror,
    uint8_t                        a_nRotations,
    uint8_t                        a_radius)
{
#ifdef DEBUG
    assert(a_pieceCoords.size() <= PIECE_MAX_SQUARES);
    assert(a_radius <= PIECE_MAX_RADIUS);
#endif

    m_origMirror    = a_mirror;
    m_origRotations = a_nRotations;
    m_radius        = a_radius;

    m_nMirrors = 0;

    m_originalConf.m_pieceSquares.resize(a_pieceCoords.size());
    m_currentConf.m_pieceSquares.resize(a_pieceCoords.size());

    m_originalConf.m_nkPoints.resize(a_nkPoints.size());
    m_currentConf.m_nkPoints.resize(a_nkPoints.size());

    m_originalConf.m_forbiddenArea.resize(a_forbiddenArea.size());
    m_currentConf.m_forbiddenArea.resize(a_forbiddenArea.size());

    for (std::size_t i = 0; i < a_pieceCoords.size(); i++)
    {
        m_currentConf.m_pieceSquares[i].Set(
                a_pieceCoords[i].m_row,
                a_pieceCoords[i].m_col);

        m_originalConf.m_pieceSquares[i].Set(
                a_pieceCoords[i].m_row,
                a_pieceCoords[i].m_col);
    }

    for (std::size_t i = 0; i < a_nkPoints.size(); i++)
    {
        m_currentConf.m_nkPoints[i].Set(
                a_nkPoints[i].m_row,
                a_nkPoints[i].m_col);

        m_originalConf.m_nkPoints[i].Set(
                a_nkPoints[i].m_row,
                a_nkPoints[i].m_col);
    }

    for (std::size_t i = 0; i < a_forbiddenArea.size(); i++)
    {
        m_currentConf.m_forbiddenArea[i].Set(
                a_forbiddenArea[i].m_row,
                a_forbiddenArea[i].m_col);

        m_originalConf.m_forbiddenArea[i].Set(
                a_forbiddenArea[i].m_row,
                a_forbiddenArea[i].m_col);
    }

#ifdef DEBUG
    m_initialised = true;
#endif
}

const char* Piece::GetPieceDescription(ePieceType_t a_pieceType)
{
    return _(Piece::m_pieceDescription[a_pieceType]);
}

void Piece::Reset()
{
#ifdef DEBUG
    assert(m_initialised);
#endif

    for (std::size_t i = 0; i < m_originalConf.m_pieceSquares.size(); i++)
    {
        m_currentConf.m_pieceSquares[i].Set(
            m_originalConf.m_pieceSquares[i].m_row,
            m_originalConf.m_pieceSquares[i].m_col);
    }

    for (std::size_t i = 0; i < m_originalConf.m_nkPoints.size(); i++)
    {
        m_currentConf.m_nkPoints[i].Set(
            m_originalConf.m_nkPoints[i].m_row,
            m_originalConf.m_nkPoints[i].m_col);
    }

    for (std::size_t i = 0; i < m_originalConf.m_forbiddenArea.size(); i++)
    {
        m_currentConf.m_forbiddenArea[i].Set(
            m_originalConf.m_forbiddenArea[i].m_row,
            m_originalConf.m_forbiddenArea[i].m_col);
    }

    m_nMirrors   = 0;
}

void Piece::RotateRight()
{
#ifdef DEBUG
    assert(m_initialised);
#endif

    if (m_origRotations <= 1)
    {
    	// the piece can't be rotated. Do nothing and return
    	return;
    }

    std::vector<Coordinate>::iterator it;

    // change all the properties saved into the current configuration
    // (squares, nk points and forbidden areas)
    for (it  = m_currentConf.m_pieceSquares.begin();
         it != m_currentConf.m_pieceSquares.end();
         it++)
    {
        RotateCoordRight(*it);
    }

    for (it  = m_currentConf.m_nkPoints.begin();
         it != m_currentConf.m_nkPoints.end();
         it++)
    {
        RotateCoordRight(*it);
    }

    for (it  = m_currentConf.m_forbiddenArea.begin();
         it != m_currentConf.m_forbiddenArea.end();
         it++)
    {
        RotateCoordRight(*it);
    }

    return;
}

void Piece::RotateLeft()
{
#ifdef DEBUG
    assert(m_initialised);
#endif

    if (m_origRotations <= 1)
    {
        // the piece can't be rotated. Do nothing and return
        return;
    }

    std::vector<Coordinate>::iterator it;

    // change all the properties saved into the current configuration
    // (squares, nk points and forbidden areas)
    for (it  = m_currentConf.m_pieceSquares.begin();
         it != m_currentConf.m_pieceSquares.end();
         it++)
    {
        RotateCoordLeft(*it);
    }

    for (it  = m_currentConf.m_nkPoints.begin();
         it != m_currentConf.m_nkPoints.end();
         it++)
    {
        RotateCoordLeft(*it);
    }

    for (it  = m_currentConf.m_forbiddenArea.begin();
         it != m_currentConf.m_forbiddenArea.end();
         it++)
    {
        RotateCoordLeft(*it);
    }

    return;
}

bool Piece::MirrorYAxis()
{
#ifdef DEBUG
    assert(m_initialised);
#endif

    std::vector<Coordinate>::iterator it;

    // change all the properties saved into the current configuration
    // (squares, nk points and forbidden areas)
    for (it  = m_currentConf.m_pieceSquares.begin();
         it != m_currentConf.m_pieceSquares.end();
         it++)
    {
        MirrorCoordYAxis(*it);
    }

    for (it  = m_currentConf.m_nkPoints.begin();
         it != m_currentConf.m_nkPoints.end();
         it++)
    {
        MirrorCoordYAxis(*it);
    }

    for (it  = m_currentConf.m_forbiddenArea.begin();
         it != m_currentConf.m_forbiddenArea.end();
         it++)
    {
        MirrorCoordYAxis(*it);
    }

    if (!m_origMirror)
    {
    	// if it can't be mirrored. Don't increment m_nMirrors
    	return false;
    }

	m_nMirrors++;
    return IsMirrored();
}

bool Piece::MirrorXAxis()
{
#ifdef DEBUG
    assert(m_initialised);
#endif

    std::vector<Coordinate>::iterator it;

    // change all the properties saved into the current configuration
    // (squares, nk points and forbidden areas)
    for (it  = m_currentConf.m_pieceSquares.begin();
         it != m_currentConf.m_pieceSquares.end();
         it++)
    {
        MirrorCoordXAxis(*it);
    }

    for (it  = m_currentConf.m_nkPoints.begin();
         it != m_currentConf.m_nkPoints.end();
         it++)
    {
        MirrorCoordXAxis(*it);
    }

    for (it  = m_currentConf.m_forbiddenArea.begin();
         it != m_currentConf.m_forbiddenArea.end();
         it++)
    {
        MirrorCoordXAxis(*it);
    }

    if (!m_origMirror)
    {
        // if it can't be mirrored. Don't increment m_nMirrors
        return false;
    }

    m_nMirrors++;
    return IsMirrored();
}

void Piece::RotateCoordRight(Coordinate &in_out_coord)
{
    int32_t aux;
    if ((in_out_coord.m_row >= 0) && (in_out_coord.m_col >= 0))
    {
        // X will be positive. Y negative
        aux = in_out_coord.m_row;
        in_out_coord.m_row = in_out_coord.m_col;
        in_out_coord.m_col = -aux;
    }
    else if ((in_out_coord.m_row >= 0) && (in_out_coord.m_col < 0))
    {
        // both will be negative
        aux = in_out_coord.m_row;
        in_out_coord.m_row = in_out_coord.m_col;
        in_out_coord.m_col = -aux;
    }
    else if ((in_out_coord.m_row < 0) && (in_out_coord.m_col < 0))
    {
        // X will be negative. Y positive
        aux = in_out_coord.m_row;
        in_out_coord.m_row = in_out_coord.m_col;
        in_out_coord.m_col = -aux;
    }
    else // ((in_out_coord.m_row < 0) && (in_out_coord.m_col >= 0))
    {
        // both will be positive
        aux = in_out_coord.m_row;
        in_out_coord.m_row = in_out_coord.m_col;
        in_out_coord.m_col = -aux;
    }
}

void Piece::RotateCoordLeft(Coordinate &in_out_coord)
{
    int32_t aux;
    if ((in_out_coord.m_row >= 0) && (in_out_coord.m_col >= 0))
    {
        // X will be negative. Y positive
        aux = in_out_coord.m_row;
        in_out_coord.m_row = -in_out_coord.m_col;
        in_out_coord.m_col = aux;
    }
    else if ((in_out_coord.m_row >= 0) && (in_out_coord.m_col < 0))
    {
        // both will be positive
        aux = in_out_coord.m_row;
        in_out_coord.m_row = -in_out_coord.m_col;
        in_out_coord.m_col = aux;
    }
    else if ((in_out_coord.m_row < 0) && (in_out_coord.m_col < 0))
    {
        // X will be positive. Y negative
        aux = in_out_coord.m_row;
        in_out_coord.m_row = -in_out_coord.m_col;
        in_out_coord.m_col = aux;
    }
    else // ((in_out_coord.m_row < 0) && (in_out_coord.m_col >= 0))
    {
        // both will be negative
        aux = in_out_coord.m_row;
        in_out_coord.m_row = -in_out_coord.m_col;
        in_out_coord.m_col = aux;
    }
}

void Piece::MirrorCoordXAxis(Coordinate &in_out_coord)
{
    in_out_coord.m_row = -in_out_coord.m_row;
}

void Piece::MirrorCoordYAxis(Coordinate &in_out_coord)
{
    in_out_coord.m_col = -in_out_coord.m_col;
}

void Piece::LoadPiece_1BabyPiece(Piece &thisPiece)
{
    // + . +
    // . X .
    // + . +
    std::vector<Coordinate> coords(1);
    coords[0] = Coordinate(0, 0);

    std::vector<Coordinate> nkPoints(4);
    nkPoints[0] = Coordinate(-1, -1);
    nkPoints[1] = Coordinate(-1,  1);
    nkPoints[2] = Coordinate( 1, -1);
    nkPoints[3] = Coordinate( 1,  1);

    std::vector<Coordinate> forbiddenArea(4);
    forbiddenArea[0] = Coordinate(-1,  0);
    forbiddenArea[1] = Coordinate( 0,  1);
    forbiddenArea[2] = Coordinate( 0, -1);
    forbiddenArea[3] = Coordinate( 1,  0);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 1, 0);
}
void Piece::LoadPiece_2TwoPiece(Piece &thisPiece)
{
    // + . . +
    // . X X .
    // + . . +
    std::vector<Coordinate> coords(2);
    coords[0] = Coordinate(0, 0);
    coords[1] = Coordinate(0, 1);

    std::vector<Coordinate> nkPoints(4);
    nkPoints[0] = Coordinate(-1, -1);
    nkPoints[1] = Coordinate(-1,  2);
    nkPoints[2] = Coordinate( 1, -1);
    nkPoints[3] = Coordinate( 1,  2);

    std::vector<Coordinate> forbiddenArea(6);
    forbiddenArea[0] = Coordinate(-1,  0);
    forbiddenArea[1] = Coordinate(-1,  1);
    forbiddenArea[2] = Coordinate( 0, -1);
    forbiddenArea[3] = Coordinate( 0,  2);
    forbiddenArea[4] = Coordinate( 1,  0);
    forbiddenArea[5] = Coordinate( 1,  1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 2, 1);
}
void Piece::LoadPiece_3LongPiece(Piece &thisPiece)
{
    // + . . . +
    // . X X X .
    // + . . . +
    std::vector<Coordinate> coords(3);
    coords[0] = Coordinate(0,  0);
    coords[1] = Coordinate(0, -1);
    coords[2] = Coordinate(0,  1);

    std::vector<Coordinate> nkPoints(4);
    nkPoints[0] = Coordinate(-1, -2);
    nkPoints[1] = Coordinate(-1,  2);
    nkPoints[2] = Coordinate( 1, -2);
    nkPoints[3] = Coordinate( 1,  2);

    std::vector<Coordinate> forbiddenArea(8);
    forbiddenArea[0] = Coordinate(-1, -1);
    forbiddenArea[1] = Coordinate(-1,  0);
    forbiddenArea[2] = Coordinate(-1,  1);
    forbiddenArea[3] = Coordinate( 0, -2);
    forbiddenArea[4] = Coordinate( 0,  2);
    forbiddenArea[5] = Coordinate( 1, -1);
    forbiddenArea[6] = Coordinate( 1,  0);
    forbiddenArea[7] = Coordinate( 1,  1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 2, 1);
}
void Piece::LoadPiece_3Triangle(Piece &thisPiece)
{
    // + . . +
    // . X X .
    // + . X .
    //   + . +
    std::vector<Coordinate> coords(3);
    coords[0] = Coordinate(0,  0);
    coords[1] = Coordinate(0, -1);
    coords[2] = Coordinate(1,  0);

    std::vector<Coordinate> nkPoints(5);
    nkPoints[0] = Coordinate(-1, -2);
    nkPoints[1] = Coordinate(-1,  1);
    nkPoints[2] = Coordinate( 1, -2);
    nkPoints[3] = Coordinate( 2, -1);
    nkPoints[4] = Coordinate( 2,  1);

    std::vector<Coordinate> forbiddenArea(7);
    forbiddenArea[0] = Coordinate(-1, -1);
    forbiddenArea[1] = Coordinate(-1,  0);
    forbiddenArea[2] = Coordinate( 0, -2);
    forbiddenArea[3] = Coordinate( 0,  1);
    forbiddenArea[4] = Coordinate( 1, -1);
    forbiddenArea[5] = Coordinate( 1,  1);
    forbiddenArea[6] = Coordinate( 2,  0);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 4, 1);
}
void Piece::LoadPiece_4LongPiece(Piece &thisPiece)
{
    // + . . . . +
    // . X X X X .
    // + . . . . +
    std::vector<Coordinate> coords(4);
    coords[0] = Coordinate(0,  0);
    coords[1] = Coordinate(0, -1);
    coords[2] = Coordinate(0,  2);
    coords[3] = Coordinate(0,  1);

    std::vector<Coordinate> nkPoints(4);
    nkPoints[0] = Coordinate(-1, -2);
    nkPoints[1] = Coordinate(-1,  3);
    nkPoints[2] = Coordinate( 1, -2);
    nkPoints[3] = Coordinate( 1,  3);

    std::vector<Coordinate> forbiddenArea(10);
    forbiddenArea[0] = Coordinate(-1, -1);
    forbiddenArea[1] = Coordinate(-1,  0);
    forbiddenArea[2] = Coordinate(-1,  1);
    forbiddenArea[3] = Coordinate(-1,  2);
    forbiddenArea[4] = Coordinate( 0, -2);
    forbiddenArea[5] = Coordinate( 0,  3);
    forbiddenArea[6] = Coordinate( 1, -1);
    forbiddenArea[7] = Coordinate( 1,  0);
    forbiddenArea[8] = Coordinate( 1,  1);
    forbiddenArea[9] = Coordinate( 1,  2);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 2, 2);
}
void Piece::LoadPiece_4LittleS(Piece &thisPiece)
{
    //   + . . +
    // + . X X .
    // . X X . +
    // + . . +
    std::vector<Coordinate> coords(4);
    coords[0] = Coordinate(0,  0);
    coords[1] = Coordinate(1, -1);
    coords[2] = Coordinate(1,  0);
    coords[3] = Coordinate(0,  1);

    std::vector<Coordinate> nkPoints(6);
    nkPoints[0] = Coordinate(-1, -1);
    nkPoints[1] = Coordinate(-1,  2);
    nkPoints[2] = Coordinate( 0, -2);
    nkPoints[3] = Coordinate( 1,  2);
    nkPoints[4] = Coordinate( 2, -2);
    nkPoints[5] = Coordinate( 2,  1);

    std::vector<Coordinate> forbiddenArea(8);
    forbiddenArea[0] = Coordinate(-1,  0);
    forbiddenArea[1] = Coordinate(-1,  1);
    forbiddenArea[2] = Coordinate( 0, -1);
    forbiddenArea[3] = Coordinate( 0,  2);
    forbiddenArea[4] = Coordinate( 1, -2);
    forbiddenArea[5] = Coordinate( 1,  1);
    forbiddenArea[6] = Coordinate( 2, -1);
    forbiddenArea[7] = Coordinate( 2,  0);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, true, 2, 1);
}
void Piece::LoadPiece_4LittleT(Piece &thisPiece)
{
    //   + . +
    // + . X . +
    // . X X X .
    // + . . . +
    std::vector<Coordinate> coords(4);
    coords[0] = Coordinate( 0,  0);
    coords[1] = Coordinate( 0, -1);
    coords[2] = Coordinate( 0,  1);
    coords[3] = Coordinate(-1,  0);

    std::vector<Coordinate> nkPoints(6);
    nkPoints[0] = Coordinate(-2, -1);
    nkPoints[1] = Coordinate(-2,  1);
    nkPoints[2] = Coordinate(-1, -2);
    nkPoints[3] = Coordinate(-1,  2);
    nkPoints[4] = Coordinate( 1, -2);
    nkPoints[5] = Coordinate( 1,  2);

    std::vector<Coordinate> forbiddenArea(8);
    forbiddenArea[0] = Coordinate(-1, -1);
    forbiddenArea[1] = Coordinate(-2,  0);
    forbiddenArea[2] = Coordinate(-1,  1);
    forbiddenArea[3] = Coordinate( 0, -2);
    forbiddenArea[4] = Coordinate( 0,  2);
    forbiddenArea[5] = Coordinate( 1, -1);
    forbiddenArea[6] = Coordinate( 1,  0);
    forbiddenArea[7] = Coordinate( 1,  1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 4, 1);
}
void Piece::LoadPiece_4littleL(Piece &thisPiece)
{
    // + . . . +
    // . X X X .
    // + . . X .
    //     + . +
    std::vector<Coordinate> coords(4);
    coords[0] = Coordinate(0,  0);
    coords[1] = Coordinate(1,  1);
    coords[2] = Coordinate(0, -1);
    coords[3] = Coordinate(0,  1);

    std::vector<Coordinate> nkPoints(5);
    nkPoints[0] = Coordinate(-1, -2);
    nkPoints[1] = Coordinate(-1,  2);
    nkPoints[2] = Coordinate( 1, -2);
    nkPoints[3] = Coordinate( 2,  0);
    nkPoints[4] = Coordinate( 2,  2);

    std::vector<Coordinate> forbiddenArea(9);
    forbiddenArea[0] = Coordinate(-1, -1);
    forbiddenArea[1] = Coordinate(-1,  0);
    forbiddenArea[2] = Coordinate(-1,  1);
    forbiddenArea[3] = Coordinate( 0, -2);
    forbiddenArea[4] = Coordinate( 0,  2);
    forbiddenArea[5] = Coordinate( 1, -1);
    forbiddenArea[6] = Coordinate( 1,  0);
    forbiddenArea[7] = Coordinate( 1,  2);
    forbiddenArea[8] = Coordinate( 2,  1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, true, 4, 1);
}
void Piece::LoadPiece_4FullSquare(Piece &thisPiece)
{
    // + . . +
    // . X X .
    // . X X .
    // + . . +
    std::vector<Coordinate> coords(4);
    coords[0] = Coordinate(0, 0);
    coords[1] = Coordinate(0, 1);
    coords[2] = Coordinate(1, 0);
    coords[3] = Coordinate(1, 1);

    std::vector<Coordinate> nkPoints(4);
    nkPoints[0] = Coordinate(-1, -1);
    nkPoints[1] = Coordinate(-1,  2);
    nkPoints[2] = Coordinate( 2, -1);
    nkPoints[3] = Coordinate( 2,  2);

    std::vector<Coordinate> forbiddenArea(8);
    forbiddenArea[0] = Coordinate(-1,  0);
    forbiddenArea[1] = Coordinate(-1,  1);
    forbiddenArea[2] = Coordinate( 0, -1);
    forbiddenArea[3] = Coordinate( 0,  2);
    forbiddenArea[4] = Coordinate( 1, -1);
    forbiddenArea[5] = Coordinate( 1,  2);
    forbiddenArea[6] = Coordinate( 2,  0);
    forbiddenArea[7] = Coordinate( 2,  1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 1, 1);
}
void Piece::LoadPiece_5BigS(Piece &thisPiece)
{
    //   + . . +
    //   . X X .
    // + . X . +
    // . X X .
    // + . . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0,  0);
    coords[1] = Coordinate(-1,  1);
    coords[2] = Coordinate( 1, -1);
    coords[3] = Coordinate(-1,  0);
    coords[4] = Coordinate( 1,  0);

    std::vector<Coordinate> nkPoints(6);
    nkPoints[0] = Coordinate(-2, -1);
    nkPoints[1] = Coordinate(-2,  2);
    nkPoints[2] = Coordinate( 0, -2);
    nkPoints[3] = Coordinate( 0,  2);
    nkPoints[4] = Coordinate( 2, -2);
    nkPoints[5] = Coordinate( 2,  1);

    std::vector<Coordinate> forbiddenArea(10);
    forbiddenArea[0] = Coordinate(-2,  0);
    forbiddenArea[1] = Coordinate(-2,  1);
    forbiddenArea[2] = Coordinate(-1, -1);
    forbiddenArea[3] = Coordinate(-1,  2);
    forbiddenArea[4] = Coordinate( 0, -1);
    forbiddenArea[5] = Coordinate( 0,  1);
    forbiddenArea[6] = Coordinate( 1, -2);
    forbiddenArea[7] = Coordinate( 1,  1);
    forbiddenArea[8] = Coordinate( 2, -1);
    forbiddenArea[9] = Coordinate( 2,  0);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, true, 2, 1);
}
void Piece::LoadPiece_5SafPiece(Piece &thisPiece)
{
    //   + . +
    //   . X . +
    // + . X X .
    // . X X . +
    // + . . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0,  0);
    coords[1] = Coordinate( 1, -1);
    coords[2] = Coordinate( 0,  1);
    coords[3] = Coordinate( 1,  0);
    coords[4] = Coordinate(-1,  0);

    std::vector<Coordinate> nkPoints(7);
    nkPoints[0] = Coordinate(-2, -1);
    nkPoints[1] = Coordinate(-2,  1);
    nkPoints[2] = Coordinate(-1,  2);
    nkPoints[3] = Coordinate( 0, -2);
    nkPoints[4] = Coordinate( 1,  2);
    nkPoints[5] = Coordinate( 2, -2);
    nkPoints[6] = Coordinate( 2,  1);

    std::vector<Coordinate> forbiddenArea(9);
    forbiddenArea[0] = Coordinate(-2,  0);
    forbiddenArea[1] = Coordinate(-1, -1);
    forbiddenArea[2] = Coordinate(-1,  1);
    forbiddenArea[3] = Coordinate( 0, -1);
    forbiddenArea[4] = Coordinate( 0,  2);
    forbiddenArea[5] = Coordinate( 1, -2);
    forbiddenArea[6] = Coordinate( 1,  1);
    forbiddenArea[7] = Coordinate( 2,  0);
    forbiddenArea[8] = Coordinate( 2, -1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, true, 4, 1);
}
void Piece::LoadPiece_5WPiece(Piece &thisPiece)
{
    // + . . +
    // . X X . +
    // + . X X .
    //   + . X .
    //     + . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0,  0);
    coords[1] = Coordinate(-1, -1);
    coords[2] = Coordinate( 1,  1);
    coords[3] = Coordinate(-1,  0);
    coords[4] = Coordinate( 0,  1);

    std::vector<Coordinate> nkPoints(7);
    nkPoints[0] = Coordinate(-2, -2);
    nkPoints[1] = Coordinate(-2,  1);
    nkPoints[2] = Coordinate(-1,  2);
    nkPoints[3] = Coordinate( 0, -2);
    nkPoints[4] = Coordinate( 1, -1);
    nkPoints[5] = Coordinate( 2,  0);
    nkPoints[6] = Coordinate( 2,  2);

    std::vector<Coordinate> forbiddenArea(9);
    forbiddenArea[0] = Coordinate(-2,  0);
    forbiddenArea[1] = Coordinate(-2, -1);
    forbiddenArea[2] = Coordinate(-1, -2);
    forbiddenArea[3] = Coordinate(-1,  1);
    forbiddenArea[4] = Coordinate( 0, -1);
    forbiddenArea[5] = Coordinate( 0,  2);
    forbiddenArea[6] = Coordinate( 1,  0);
    forbiddenArea[7] = Coordinate( 1,  2);
    forbiddenArea[8] = Coordinate( 2,  1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 4, 1);
}
void Piece::LoadPiece_5CuntPiece(Piece &thisPiece)
{
    // + . . . +
    // . X X X .
    // . X . X .
    // + . + . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate(0,  0);
    coords[1] = Coordinate(1, -1);
    coords[2] = Coordinate(1,  1);
    coords[3] = Coordinate(0, -1);
    coords[4] = Coordinate(0,  1);

    std::vector<Coordinate> nkPoints(5);
    nkPoints[0] = Coordinate(-1, -2);
    nkPoints[1] = Coordinate(-1,  2);
    nkPoints[2] = Coordinate( 2, -2);
    nkPoints[3] = Coordinate( 2,  0);
    nkPoints[4] = Coordinate( 2,  2);

    std::vector<Coordinate> forbiddenArea(10);
    forbiddenArea[0] = Coordinate(-1, -1);
    forbiddenArea[1] = Coordinate(-1,  0);
    forbiddenArea[2] = Coordinate(-1,  1);
    forbiddenArea[3] = Coordinate( 0, -2);
    forbiddenArea[4] = Coordinate( 0,  2);
    forbiddenArea[5] = Coordinate( 1, -2);
    forbiddenArea[6] = Coordinate( 1,  0);
    forbiddenArea[7] = Coordinate( 1,  2);
    forbiddenArea[8] = Coordinate( 2, -1);
    forbiddenArea[9] = Coordinate( 2,  1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 4, 1);
}
void Piece::LoadPiece_5BigPenis(Piece &thisPiece)
{
    // + . +
    // . X .
    // . X .
    // . X .
    // . X .
    // . X .
    // + . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0, 0);
    coords[1] = Coordinate(-2, 0);
    coords[2] = Coordinate( 2, 0);
    coords[3] = Coordinate(-1, 0);
    coords[4] = Coordinate( 1, 0);

    std::vector<Coordinate> nkPoints(4);
    nkPoints[0] = Coordinate(-3, -1);
    nkPoints[1] = Coordinate(-3,  1);
    nkPoints[2] = Coordinate( 3, -1);
    nkPoints[3] = Coordinate( 3,  1);

    std::vector<Coordinate> forbiddenArea(12);
    forbiddenArea[0] = Coordinate(-3,  0);
    forbiddenArea[1] = Coordinate(-2, -1);
    forbiddenArea[2] = Coordinate(-2,  1);
    forbiddenArea[3] = Coordinate(-1, -1);
    forbiddenArea[4] = Coordinate(-1,  1);
    forbiddenArea[5] = Coordinate( 0, -1);
    forbiddenArea[6] = Coordinate( 0,  1);
    forbiddenArea[7] = Coordinate( 1, -1);
    forbiddenArea[8] = Coordinate( 1,  1);
    forbiddenArea[9] = Coordinate( 2, -1);
    forbiddenArea[10]= Coordinate( 2,  1);
    forbiddenArea[11]= Coordinate( 3,  0);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 2, 2);
}
void Piece::LoadPiece_5Cross(Piece &thisPiece)
{
    //   + . +
    // + . X . +
    // . X X X .
    // + . X . +
    //   + . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0,  0);
    coords[1] = Coordinate(-1,  0);
    coords[2] = Coordinate( 0,  1);
    coords[3] = Coordinate( 0, -1);
    coords[4] = Coordinate( 1,  0);

    std::vector<Coordinate> nkPoints(8);
    nkPoints[0] = Coordinate(-2, -1);
    nkPoints[1] = Coordinate(-2,  1);
    nkPoints[2] = Coordinate(-1, -2);
    nkPoints[3] = Coordinate(-1,  2);
    nkPoints[4] = Coordinate( 1, -2);
    nkPoints[5] = Coordinate( 1,  2);
    nkPoints[6] = Coordinate( 2, -1);
    nkPoints[7] = Coordinate( 2,  1);

    std::vector<Coordinate> forbiddenArea(8);
    forbiddenArea[0] = Coordinate(-2,  0);
    forbiddenArea[1] = Coordinate(-1, -1);
    forbiddenArea[2] = Coordinate(-1,  1);
    forbiddenArea[3] = Coordinate( 0, -2);
    forbiddenArea[4] = Coordinate( 0,  2);
    forbiddenArea[5] = Coordinate( 1, -1);
    forbiddenArea[6] = Coordinate( 1,  1);
    forbiddenArea[7] = Coordinate( 2,  0);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 1, 1);
}
void Piece::LoadPiece_5HalfSquare(Piece &thisPiece)
{
    // + . +
    // . X .
    // . X . . +
    // . X X X .
    // + . . . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0, 0);
    coords[1] = Coordinate( 0, 2);
    coords[2] = Coordinate(-2, 0);
    coords[3] = Coordinate(-1, 0);
    coords[4] = Coordinate( 0, 1);

    std::vector<Coordinate> nkPoints(5);
    nkPoints[0] = Coordinate(-3, -1);
    nkPoints[1] = Coordinate(-3,  1);
    nkPoints[2] = Coordinate(-1,  3);
    nkPoints[3] = Coordinate( 1, -1);
    nkPoints[4] = Coordinate( 1,  3);

    std::vector<Coordinate> forbiddenArea(11);
    forbiddenArea[0] = Coordinate(-3,  0);
    forbiddenArea[1] = Coordinate(-2, -1);
    forbiddenArea[2] = Coordinate(-2,  1);
    forbiddenArea[3] = Coordinate(-1, -1);
    forbiddenArea[4] = Coordinate(-1,  1);
    forbiddenArea[5] = Coordinate(-1,  2);
    forbiddenArea[6] = Coordinate( 0, -1);
    forbiddenArea[7] = Coordinate( 0,  3);
    forbiddenArea[8] = Coordinate( 1,  0);
    forbiddenArea[9] = Coordinate( 1,  1);
    forbiddenArea[10]= Coordinate( 1,  2);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 4, 2);
}
void Piece::LoadPiece_5BigL(Piece &thisPiece)
{
    // + . +
    // . X .
    // . X .
    // . X . +
    // . X X .
    // + . . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0, 0);
    coords[1] = Coordinate(-2, 0);
    coords[2] = Coordinate( 1, 1);
    coords[3] = Coordinate(-1, 0);
    coords[4] = Coordinate( 1, 0);

    std::vector<Coordinate> nkPoints(5);
    nkPoints[0] = Coordinate(-3, -1);
    nkPoints[1] = Coordinate(-3,  1);
    nkPoints[2] = Coordinate( 0,  2);
    nkPoints[3] = Coordinate( 2, -1);
    nkPoints[4] = Coordinate( 2,  2);

    std::vector<Coordinate> forbiddenArea(11);
    forbiddenArea[0] = Coordinate(-3,  0);
    forbiddenArea[1] = Coordinate(-2, -1);
    forbiddenArea[2] = Coordinate(-2,  1);
    forbiddenArea[3] = Coordinate(-1, -1);
    forbiddenArea[4] = Coordinate(-1,  1);
    forbiddenArea[5] = Coordinate( 0, -1);
    forbiddenArea[6] = Coordinate( 0,  1);
    forbiddenArea[7] = Coordinate( 1, -1);
    forbiddenArea[8] = Coordinate( 1,  2);
    forbiddenArea[9] = Coordinate( 2,  0);
    forbiddenArea[10]= Coordinate( 2,  1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, true,  4, 2);
}
void Piece::LoadPiece_5MrT(Piece &thisPiece)
{
    //     + . +
    // + . . X .
    // . X X X .
    // + . . X .
    //     + . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0,  0);
    coords[1] = Coordinate(-1,  1);
    coords[2] = Coordinate( 1,  1);
    coords[3] = Coordinate( 0, -1);
    coords[4] = Coordinate( 0,  1);

    std::vector<Coordinate> nkPoints(6);
    nkPoints[0] = Coordinate(-2,  0);
    nkPoints[1] = Coordinate(-2,  2);
    nkPoints[2] = Coordinate(-1, -2);
    nkPoints[3] = Coordinate( 1, -2);
    nkPoints[4] = Coordinate( 2,  0);
    nkPoints[5] = Coordinate( 2,  2);

    std::vector<Coordinate> forbiddenArea(10);
    forbiddenArea[0] = Coordinate(-2,  1);
    forbiddenArea[1] = Coordinate(-1, -1);
    forbiddenArea[2] = Coordinate(-1,  0);
    forbiddenArea[3] = Coordinate(-1,  2);
    forbiddenArea[4] = Coordinate( 0, -2);
    forbiddenArea[5] = Coordinate( 0,  2);
    forbiddenArea[6] = Coordinate( 1, -1);
    forbiddenArea[7] = Coordinate( 1,  0);
    forbiddenArea[8] = Coordinate( 1,  2);
    forbiddenArea[9] = Coordinate( 2,  1);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, false, 4, 1);
}
void Piece::LoadPiece_5SquareAppen(Piece &thisPiece)
{
    // + . . +
    // . X X .
    // . X X .
    // . X . +
    // + . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0, 0);
    coords[1] = Coordinate(-1, 1);
    coords[2] = Coordinate( 1, 0);
    coords[3] = Coordinate(-1, 0);
    coords[4] = Coordinate( 0, 1);

    std::vector<Coordinate> nkPoints(5);
    nkPoints[0] = Coordinate(-2, -1);
    nkPoints[1] = Coordinate(-2,  2);
    nkPoints[2] = Coordinate( 1,  2);
    nkPoints[3] = Coordinate( 2, -1);
    nkPoints[4] = Coordinate( 2,  1);

    std::vector<Coordinate> forbiddenArea(9);
    forbiddenArea[0] = Coordinate(-2,  0);
    forbiddenArea[1] = Coordinate(-2,  1);
    forbiddenArea[2] = Coordinate(-1, -1);
    forbiddenArea[3] = Coordinate(-1,  2);
    forbiddenArea[4] = Coordinate( 0, -1);
    forbiddenArea[5] = Coordinate( 0,  2);
    forbiddenArea[6] = Coordinate( 1, -1);
    forbiddenArea[7] = Coordinate( 1,  1);
    forbiddenArea[8] = Coordinate( 2,  0);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, true,  4, 1);
}
void Piece::LoadPiece_5BoringPiece(Piece &thisPiece)
{
    //     + . +
    // + . . X . +
    // . X X X X .
    // + . . . . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0,  0);
    coords[1] = Coordinate( 0,  2);
    coords[2] = Coordinate(-1,  1);
    coords[3] = Coordinate( 0, -1);
    coords[4] = Coordinate( 0,  1);

    std::vector<Coordinate> nkPoints(6);
    nkPoints[0] = Coordinate(-2,  0);
    nkPoints[1] = Coordinate(-2,  2);
    nkPoints[2] = Coordinate(-1, -2);
    nkPoints[3] = Coordinate(-1,  3);
    nkPoints[4] = Coordinate( 1, -2);
    nkPoints[5] = Coordinate( 1,  3);

    std::vector<Coordinate> forbiddenArea(10);
    forbiddenArea[0] = Coordinate(-2,  1);
    forbiddenArea[1] = Coordinate(-1, -1);
    forbiddenArea[2] = Coordinate(-1,  0);
    forbiddenArea[3] = Coordinate(-1,  2);
    forbiddenArea[4] = Coordinate( 0, -2);
    forbiddenArea[5] = Coordinate( 0,  3);
    forbiddenArea[6] = Coordinate( 1, -1);
    forbiddenArea[7] = Coordinate( 1,  0);
    forbiddenArea[8] = Coordinate( 1,  1);
    forbiddenArea[9] = Coordinate( 1,  2);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, true,  4, 2);
}
void Piece::LoadPiece_5TheUltimate(Piece &thisPiece)
{
    //   + . +
    // + . X .
    // . X X .
    // . X . +
    // . X .
    // + . +
    std::vector<Coordinate> coords(5);
    coords[0] = Coordinate( 0, 0);
    coords[1] = Coordinate( 2, 0);
    coords[2] = Coordinate(-1, 1);
    coords[3] = Coordinate( 0, 1);
    coords[4] = Coordinate( 1, 0);

    std::vector<Coordinate> nkPoints(6);
    nkPoints[0] = Coordinate(-2,  0);
    nkPoints[1] = Coordinate(-2,  2);
    nkPoints[2] = Coordinate(-1, -1);
    nkPoints[3] = Coordinate( 1,  2);
    nkPoints[4] = Coordinate( 3, -1);
    nkPoints[5] = Coordinate( 3,  1);

    std::vector<Coordinate> forbiddenArea(10);
    forbiddenArea[0] = Coordinate(-2,  1);
    forbiddenArea[1] = Coordinate(-1,  0);
    forbiddenArea[2] = Coordinate(-1,  2);
    forbiddenArea[3] = Coordinate( 0, -1);
    forbiddenArea[4] = Coordinate( 0,  2);
    forbiddenArea[5] = Coordinate( 1, -1);
    forbiddenArea[6] = Coordinate( 1,  1);
    forbiddenArea[7] = Coordinate( 2, -1);
    forbiddenArea[8] = Coordinate( 2,  1);
    forbiddenArea[9] = Coordinate( 3,  0);

    thisPiece.SetPiece(coords, nkPoints, forbiddenArea, true,  4, 2);
}

void Piece::BuildUpPrecalculatedRepresentations()
{
    do
    {
        uint8_t nRotations = 0;
        while(nRotations < GetNRotations())
        {
            // save current configuration into precalculated list of coords
            m_precalculatedConfsList.push_back(m_currentConf);

            // calculate and save bitwise representation of this configuration
            uint64_t bitwisePiece = 0x0000000000000000ull;
            std::vector<Coordinate>::const_iterator it;
            for (it  = m_currentConf.m_pieceSquares.begin();
                 it != m_currentConf.m_pieceSquares.end();
                 it++)
            {
                // this piece of magic converts a piece into a string of bits
                bitwisePiece |= static_cast<uint64_t>(1) << ( ((3 - it->m_row) * 7) + (3 - it->m_col) );
            }
            m_bitwiseRepresentationList.push_back(bitwisePiece);

            RotateRight();
            nRotations++;
        }

        if ( (GetType() == e_4Piece_LittleS) &&
             (IsMirrored() == false) )
        {
            // For this piece the maximum number or rotations is 2
            // and the piece is not symmetric, the configuration after
            // the 3rd rotation is the shame shape as the original, but
            // the coords moved. Reset the piece before mirroring to
            // avoid unexpected results
            //
            // it also happens with 2piece and 4longPiece, but those pieces
            // don't have mirror, so there's no need for this extra check
            Reset();
        }

    } while (MirrorYAxis());

    // leave the piece as it was originally described
    Reset();
}
