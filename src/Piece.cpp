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
/// @file  Piece.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2009  Original development
/// @endhistory
///
// ============================================================================

#include "Piece.h"
#include "stdio.h"

const std::string Piece::PieceDescription[e_numberOfPieces] =
{
	"1Piece_BabyPiece",
	"2Piece_TwoPiece",
	"3Piece_LongPiece",
	"3Piece_Triangle",
	"4Piece_LongPiece",
	"4Piece_LittleS",
	"4Piece_LittleT",
	"4Piece_LittleL",
	"4Piece_FullSquare",
	"5Piece_BigS",
	"5Piece_SafPiece",
	"5Piece_WPiece",
	"5Piece_CuntPiece",
	"5Piece_BigPenis",
	"5Piece_Cross",
	"5Piece_HalfSquare",
	"5Piece_BigL",
	"5Piece_MrT",
	"5Piece_SquareAppen",
	"5Piece_BoringPiece",
	"5Piece_TheUltimate"
};

// load the load piece map
Piece::LoadPieceFunction_t Piece::m_loadFunctionMap[] =
{
    Piece::LoadPiece_1BabyPiece,    // e_1Piece_BabyPiece
    Piece::LoadPiece_2TwoPiece,	    // e_2Piece_TwoPiece
    Piece::LoadPiece_3LongPiece,    // e_3Piece_LongPiece
    Piece::LoadPiece_3Triangle,     // e_3Piece_Triangle
    Piece::LoadPiece_4LongPiece,    // e_4Piece_LongPiece
    Piece::LoadPiece_4LittleS,      // e_4Piece_LittleS
    Piece::LoadPiece_4LittleT,      // e_4Piece_LittleT
    Piece::LoadPiece_4littleL,      // e_4Piece_LittleL
    Piece::LoadPiece_4FullSquare,   // e_4Piece_FullSquare
    Piece::LoadPiece_5BigS,         // e_5Piece_BigS
    Piece::LoadPiece_5SafPiece,     // e_5Piece_SafPiece
    Piece::LoadPiece_5WPiece,       // e_5Piece_WPiece
    Piece::LoadPiece_5CuntPiece,    // e_5Piece_CuntPiece
    Piece::LoadPiece_5BigPenis,     // e_5Piece_BigPenis
    Piece::LoadPiece_5Cross,        // e_5Piece_Cross
    Piece::LoadPiece_5HalfSquare,   // e_5Piece_HalfSquare
    Piece::LoadPiece_5BigL,         // e_5Piece_BigL
    Piece::LoadPiece_5MrT,          // e_5Piece_MrT
    Piece::LoadPiece_5SquareAppen,  // e_5Piece_SquareAppen
    Piece::LoadPiece_5BoringPiece,  // e_5Piece_BoringPiece
    Piece::LoadPiece_5TheUltimate   // e_5Piece_TheUltimate
};


Piece::Piece():
	m_type(e_noPiece),
    m_initialised(false)
{
    Coordinate coords[0];
    SetPiece(coords, 0, false, 0, 0);
}

Piece::Piece(ePieceType_t a_type):
	m_type(a_type),
    m_initialised(false)
{
	if (a_type != e_noPiece)
	{
		LoadPieceData(a_type);
	}
	else
	{
		// no piece loaded
	    Coordinate coords[0];
	    SetPiece(coords, 0, false, 0, 0);
	}
}

Piece::~Piece()
{
}

void Piece::SetPiece(Coordinate a_coords[PIECE_MAX_SQUARES], uint8_t a_nSquares, bool a_mirror, int8_t a_nRotations, uint8_t a_squareSideHalfSize)
{
#ifdef DEBUF
    assert(nSquares <= PIECE_MAX_SQUARES);
    assert(squareSideHalfSize <= PIECE_MAX_SQUARESIDEHALFSIZE);
#endif

    m_nSquares           = a_nSquares;
    m_origMirror         = a_mirror;
    m_origRotations      = a_nRotations;
    m_squareSideHalfSize = a_squareSideHalfSize;

    m_nMirrors = 0;
    m_nRotations = 0;

    uint8_t i;
    for (i = 0; i < a_nSquares ; i++)
    {
        m_coords[i].m_row = m_origCoords[i].m_row = a_coords[i].m_row;
        m_coords[i].m_col = m_origCoords[i].m_col = a_coords[i].m_col;
    }
    for (i = a_nSquares; i < PIECE_MAX_SQUARES ; i++)
    {
        m_coords[i].m_row = m_origCoords[i].m_row = 0;
        m_coords[i].m_col = m_origCoords[i].m_col = 0;
    }

    m_initialised = true;
}

void Piece::Reset()
{
#ifdef DEBUF
    assert(m_initialised);
#endif

    uint8_t i;
    for (i = 0; i < m_nSquares ; i++)
    {
        m_coords[i].m_row = m_origCoords[i].m_row;
        m_coords[i].m_col = m_origCoords[i].m_col;
    }
    for (i = m_nSquares; i < PIECE_MAX_SQUARES ; i++)
    {
        m_coords[i].m_row = m_origCoords[i].m_row;
        m_coords[i].m_col = m_origCoords[i].m_col;
    }

    m_nMirrors   = 0;
    m_nRotations = 0;
}

int8_t Piece::Rotate()
{
#ifdef DEBUF
    assert(m_initialised);
#endif

    if (m_origRotations <= 1)
    {
    	// the piece can't be rotated. Do nothing and return there is no rotations left
    	return 0;
    }

    for (uint8_t i = 0; i < m_nSquares; i++)
    {
    	int8_t aux;
        if ((m_coords[i].m_row >= 0) && (m_coords[i].m_col >= 0))
        {
            // X will be positive. Y negative
            aux = m_coords[i].m_row;
            m_coords[i].m_row = m_coords[i].m_col;
            m_coords[i].m_col = -aux;
        }
        else if ((m_coords[i].m_row >= 0) && (m_coords[i].m_col < 0))
        {
            // both will be negative
            aux = m_coords[i].m_row;
            m_coords[i].m_row = m_coords[i].m_col;
            m_coords[i].m_col = -aux;
        }
        else if ((m_coords[i].m_row < 0) && (m_coords[i].m_col < 0))
        {
            // X will be negative. Y positive
            aux = m_coords[i].m_row;
            m_coords[i].m_row = m_coords[i].m_col;
            m_coords[i].m_col = -aux;
        }
        else // ((m_coords[i].m_row < 0) && (m_coords[i].m_col >= 0))
        {
            // both will be positive
            aux = m_coords[i].m_row;
            m_coords[i].m_row = m_coords[i].m_col;
            m_coords[i].m_col = -aux;
        }
    }

    m_nRotations++;
    if (m_origRotations == m_nRotations)
    {
        m_nRotations = 0;
        return 0;
    }

    return (m_origRotations - m_nRotations);
}

bool Piece::Mirror()
{
#ifdef DEBUF
    assert(m_initialised);
#endif

    if (!m_origMirror)
    {
    	// if it can't be mirrored do nothing
    	return false;
    }

	for (uint8_t i = 0; i < m_nSquares; i++)
	{
		m_coords[i].m_col = -m_coords[i].m_col;
	}

	m_nMirrors++;

    return IsMirrored();
}

void Piece::LoadPiece_1BabyPiece(Piece &thisPiece)
{
    // X
    Coordinate coords[1];

    coords[0].Set(0, 0);

    thisPiece.SetPiece(coords, 1, false, 1, 0);
}
void Piece::LoadPiece_2TwoPiece(Piece &thisPiece)
{
    // X X
    Coordinate coords[2];

	coords[0].Set(0, 0);
	coords[1].Set(0, 1);

    thisPiece.SetPiece(coords, 2, false, 2, 1);
}
void Piece::LoadPiece_3LongPiece(Piece &thisPiece)
{
    // X X X
    Coordinate coords[3];

	coords[0].Set(0, 0);
	coords[1].Set(0, -1);
	coords[2].Set(0, 1);

    thisPiece.SetPiece(coords, 3, false, 2, 1);
}
void Piece::LoadPiece_3Triangle(Piece &thisPiece)
{
    // X X
    //   X
    Coordinate coords[3];

	coords[0].Set(0, 0);
	coords[1].Set(0, -1);
	coords[2].Set(1, 0);

    thisPiece.SetPiece(coords, 3, false, 4, 1);
}
void Piece::LoadPiece_4LongPiece(Piece &thisPiece)
{
    // X X X X
    Coordinate coords[4];

	coords[0].Set(0, 0);
	coords[1].Set(0, -1);
	coords[2].Set(0, 2);
	coords[3].Set(0, 1);

    thisPiece.SetPiece(coords, 4, false, 2, 2);
}
void Piece::LoadPiece_4LittleS(Piece &thisPiece)
{
    //   X X
    // X X
    Coordinate coords[4];

	coords[0].Set(0, 0);
	coords[1].Set(1, -1);
	coords[2].Set(1, 0);
	coords[3].Set(0, 1);

    thisPiece.SetPiece(coords, 4, true, 2, 1);
}
void Piece::LoadPiece_4LittleT(Piece &thisPiece)
{
    //   X
    // X X X
    Coordinate coords[4];

	coords[0].Set(0, 0);
	coords[1].Set(0, -1);
	coords[2].Set(0, 1);
	coords[3].Set(-1, 0);

    thisPiece.SetPiece(coords, 4, false, 4, 1);
}
void Piece::LoadPiece_4littleL(Piece &thisPiece)
{
    // X X X
    //     X
    Coordinate coords[4];

	coords[0].Set(0, 0);
	coords[1].Set(1, 1);
	coords[2].Set(0, -1);
	coords[3].Set(0, 1);

    thisPiece.SetPiece(coords, 4, true, 4, 1);
}
void Piece::LoadPiece_4FullSquare(Piece &thisPiece)
{
    // X X
    // X X
    Coordinate coords[4];

	coords[0].Set(0, 0);
	coords[1].Set(0, 1);
	coords[2].Set(1, 0);
	coords[3].Set(1, 1);

    thisPiece.SetPiece(coords, 4, false, 1, 1);
}
void Piece::LoadPiece_5BigS(Piece &thisPiece)
{
    //   X X
    //   X
    // X X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(-1, 1);
	coords[2].Set(1, -1);
	coords[3].Set(-1, 0);
	coords[4].Set(1, 0);

    thisPiece.SetPiece(coords, 5, true, 2, 1);
}
void Piece::LoadPiece_5SafPiece(Piece &thisPiece)
{
    //   X
    //   X X
    // X X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(1, -1);
	coords[2].Set(0, 1);
	coords[3].Set(1, 0);
	coords[4].Set(-1, 0);

    thisPiece.SetPiece(coords, 5, true, 4, 1);
}
void Piece::LoadPiece_5WPiece(Piece &thisPiece)
{
    // X X
    //   X X
    //     X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(-1, -1);
	coords[2].Set(1, 1);
	coords[3].Set(-1, 0);
	coords[4].Set(0, 1);

    thisPiece.SetPiece(coords, 5, false, 4, 1);
}
void Piece::LoadPiece_5CuntPiece(Piece &thisPiece)
{
    // X X X
    // X   X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(1, -1);
	coords[2].Set(1, 1);
	coords[3].Set(0, -1);
	coords[4].Set(0, 1);

    thisPiece.SetPiece(coords, 5, false, 4, 1);
}
void Piece::LoadPiece_5BigPenis(Piece &thisPiece)
{
    // X
    // X
    // X
    // X
    // X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(-2, 0);
	coords[2].Set(2, 0);
	coords[3].Set(-1, 0);
	coords[4].Set(1, 0);

    thisPiece.SetPiece(coords, 5, false, 2, 2);
}
void Piece::LoadPiece_5Cross(Piece &thisPiece)
{
    //   X
    // X X X
    //   X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(-1, 0);
	coords[2].Set(0, 1);
	coords[3].Set(0, -1);
	coords[4].Set(1, 0);

    thisPiece.SetPiece(coords, 5, false, 1, 1);
}
void Piece::LoadPiece_5HalfSquare(Piece &thisPiece)
{
    // X
    // X
    // X X X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(0, 2);
	coords[2].Set(-2, 0);
	coords[3].Set(-1, 0);
	coords[4].Set(0, 1);

    thisPiece.SetPiece(coords, 5, false, 4, 2);
}
void Piece::LoadPiece_5BigL(Piece &thisPiece)
{
    // X
    // X
    // X
    // X X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(-2, 0);
	coords[2].Set(1, 1);
	coords[3].Set(-1, 0);
	coords[4].Set(1, 0);

    thisPiece.SetPiece(coords, 5, true,  4, 2);
}
void Piece::LoadPiece_5MrT(Piece &thisPiece)
{
    //     X
    // X X X
    //     X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(-1, 1);
	coords[2].Set(1, 1);
	coords[3].Set(0, -1);
	coords[4].Set(0, 1);

    thisPiece.SetPiece(coords, 5, false, 4, 1);
}
void Piece::LoadPiece_5SquareAppen(Piece &thisPiece)
{
    // X X
    // X X
    // X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(-1, 1);
	coords[2].Set(1, 0);
	coords[3].Set(-1, 0);
	coords[4].Set(0, 1);

    thisPiece.SetPiece(coords, 5, true,  4, 1);
}
void Piece::LoadPiece_5BoringPiece(Piece &thisPiece)
{
    //     X
    // X X X X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(0, 2);
	coords[2].Set(-1, 1);
	coords[3].Set(0, 1);
	coords[4].Set(0, -1);

    thisPiece.SetPiece(coords, 5, true,  4, 2);
}
void Piece::LoadPiece_5TheUltimate(Piece &thisPiece)
{
    //   X
    // X X
    // X
    // X
    Coordinate coords[5];

	coords[0].Set(0, 0);
	coords[1].Set(2, 0);
	coords[2].Set(-1, 1);
	coords[3].Set(0, 1);
	coords[4].Set(1, 0);

    thisPiece.SetPiece(coords, 5, true,  4, 2);
}
