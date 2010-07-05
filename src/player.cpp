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
/// @file  player.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2009  Original development
/// @endhistory
///
// ============================================================================

#include "player.h"
#include <ostream>
#include <iomanip> // setw

Player::Player(
    std::string a_name,
    char a_presentationChar,
    int32_t a_rowsInBoard,
    int32_t a_columnsInBoard,
    const Coordinate &a_startingCoordinate):
        m_name(a_name),
        m_presentationChar(a_presentationChar),
        m_nRowsInBoard(a_rowsInBoard),
        m_nColumnsInBoard(a_columnsInBoard),
        m_nkPointsCount(0),
        m_influencedCoordsCount(0),
        m_startingCoordinate(a_startingCoordinate)
{
    // load the pieces
    LoadPieces();

    // all the pieces are available
    for (int8_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        m_piecesPresent[i] = true;
    }
    m_nPiecesAvailable = e_numberOfPieces;

	// allocate memory for the nucleation point array and set all the nk points to 0
    m_coordinateProperties = new uint8_t* [m_nRowsInBoard];
	for (int32_t i = 0 ; i < m_nRowsInBoard; i++)
	{
	    m_coordinateProperties[i] = new uint8_t[m_nColumnsInBoard];
		for (int32_t j = 0; j < m_nColumnsInBoard; j++)
		{
		    // set all the properties to "not present" (false)
		    m_coordinateProperties[i][j] = ~COORD_PROPERTY_MASK;
		}
	}
}

Player::~Player()
{
    // delete the array that saves the nk points of this player
	for (int32_t i = 0 ; i < m_nRowsInBoard; i++)
	{
		delete [] m_coordinateProperties[i];
	}
	delete [] m_coordinateProperties;
}

Player::Player(const Player &a_src)
{
    // allocate the memory needed before copying from object
    this->m_coordinateProperties = new uint8_t* [a_src.m_nRowsInBoard];
    for (int32_t i = 0; i < a_src.m_nRowsInBoard; i++)
    {
        this->m_coordinateProperties[i] = new uint8_t[a_src.m_nColumnsInBoard];
    }

    CopyFromObject(a_src);
}

Player& Player::operator= (const Player &a_src)
{
    if (this != &a_src)
    {
        // only delete the memory already allocated if the size of the
        // new board is different from "this"
        if ( (this->m_nRowsInBoard    != a_src.m_nRowsInBoard)  ||
             (this->m_nColumnsInBoard != a_src.m_nColumnsInBoard) )
        {
            // delete the array that saves the nk points of this player
            for (int32_t i = 0 ; i < this->m_nRowsInBoard; i++)
            {
                delete [] this->m_coordinateProperties[i];
            }
            delete [] m_coordinateProperties;

            // once the old memory is deleted allocate the new memory
            // needed for the new size before copying from object
            this->m_coordinateProperties   = new uint8_t* [a_src.m_nRowsInBoard];
            for (int32_t i = 0; i < a_src.m_nRowsInBoard; i++)
            {
                m_coordinateProperties[i] = new uint8_t[a_src.m_nColumnsInBoard];
            }
        }

        // copy the new info from the source object
        CopyFromObject(a_src);
    }

    return *this;
}

void Player::CopyFromObject(const Player &a_src)
{
    this->m_name                  = a_src.m_name;
    this->m_presentationChar      = a_src.m_presentationChar;
    this->m_nPiecesAvailable      = a_src.m_nPiecesAvailable;
    this->m_nRowsInBoard          = a_src.m_nRowsInBoard;
    this->m_nColumnsInBoard       = a_src.m_nColumnsInBoard;
    this->m_nkPointsCount         = a_src.m_nkPointsCount;
    this->m_influencedCoordsCount = a_src.m_influencedCoordsCount;
    this->m_startingCoordinate    = a_src.m_startingCoordinate;
    this->m_colourRed             = a_src.m_colourRed;
    this->m_colourGreen           = a_src.m_colourGreen;
    this->m_colourBlue            = a_src.m_colourBlue;

    // copy the state of the pieces
    for (int8_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        this->m_pieces[i] = a_src.m_pieces[i];
        this->m_piecesPresent[i] = a_src.m_piecesPresent[i];
    }

    // set all the nk points to the src's
    for (int32_t i = 0; i < a_src.m_nRowsInBoard; i++)
    {
        for (int32_t j = 0 ; j < a_src.m_nColumnsInBoard; j++)
        {
            m_coordinateProperties[i][j] = a_src.m_coordinateProperties[i][j];
        }
    }
}

void Player::SetColour(uint8_t a_colourRed, uint8_t a_colourGreen, uint8_t a_colourBlue)
{
    m_colourRed   = a_colourRed;
    m_colourGreen = a_colourGreen;
    m_colourBlue  = a_colourBlue;
}

void Player::GetColour(uint8_t &out_colourRed, uint8_t &out_colourGreen, uint8_t &out_colourBlue) const
{
    out_colourRed   = m_colourRed;
    out_colourGreen = m_colourGreen;
    out_colourBlue  = m_colourBlue;
}

void Player::Reset(const Coordinate &a_startingCoordinate)
{
	// reset all the pieces owned by the player
	for (int8_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
	{
		m_pieces[i].Reset();

		if (IsPieceAvailable(static_cast<ePieceType_t>(i)) == false)
		{
			SetPiece(static_cast<ePieceType_t>(i));
		}
	}

	// Player will have all properties unset (false)
	Coordinate thisCoord(0, 0);
	for (thisCoord.m_row = 0 ; thisCoord.m_row < m_nRowsInBoard; thisCoord.m_row++)
	{
		for (thisCoord.m_col = 0 ; thisCoord.m_col < m_nColumnsInBoard; thisCoord.m_col++)
		{
		    //m_coordinateProperties[thisCoord.m_row][thisCoord.m_col] = ~COORD_PROPERTY_MASK;
		    UnsetInfluencedCoord(thisCoord);
		    UnsetNucleationPoint(thisCoord);
		}
	}

    SetStartingCoordinate(a_startingCoordinate);

#ifdef DEBUG
	assert(m_nkPointsCount == 0);
	assert(m_influencedCoordsCount == 0);
	assert(m_nPiecesAvailable == e_numberOfPieces);

	for (int8_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
	{
		assert(IsPieceAvailable(static_cast<ePieceType_t>(i)));
	}

    for (thisCoord.m_row = 0 ; thisCoord.m_row < m_nRowsInBoard; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0 ; thisCoord.m_col < m_nColumnsInBoard; thisCoord.m_col++)
        {
            assert(IsNucleationPoint(thisCoord) == false);
            assert(IsCoordInfluencedByPlayer(thisCoord) == false);
        }
    }
#endif
}

void Player::LoadPieces()
{
    m_pieces[e_1Piece_BabyPiece]    = Piece(e_1Piece_BabyPiece);
    m_pieces[e_2Piece_TwoPiece]     = Piece(e_2Piece_TwoPiece);
    m_pieces[e_3Piece_LongPiece]    = Piece(e_3Piece_LongPiece);
    m_pieces[e_3Piece_Triangle]     = Piece(e_3Piece_Triangle);
    m_pieces[e_4Piece_LongPiece]    = Piece(e_4Piece_LongPiece);
    m_pieces[e_4Piece_LittleS]      = Piece(e_4Piece_LittleS);
    m_pieces[e_4Piece_LittleT]      = Piece(e_4Piece_LittleT);
    m_pieces[e_4Piece_LittleL]      = Piece(e_4Piece_LittleL);
    m_pieces[e_4Piece_FullSquare]   = Piece(e_4Piece_FullSquare);
    m_pieces[e_5Piece_BigS]         = Piece(e_5Piece_BigS);
    m_pieces[e_5Piece_SafPiece]     = Piece(e_5Piece_SafPiece);
    m_pieces[e_5Piece_WPiece]       = Piece(e_5Piece_WPiece);
    m_pieces[e_5Piece_CuntPiece]    = Piece(e_5Piece_CuntPiece);
    m_pieces[e_5Piece_BigPenis]     = Piece(e_5Piece_BigPenis);
    m_pieces[e_5Piece_Cross]        = Piece(e_5Piece_Cross);
    m_pieces[e_5Piece_HalfSquare]   = Piece(e_5Piece_HalfSquare);
    m_pieces[e_5Piece_BigL]         = Piece(e_5Piece_BigL);
    m_pieces[e_5Piece_MrT]          = Piece(e_5Piece_MrT);
    m_pieces[e_5Piece_SquarePlus]   = Piece(e_5Piece_SquarePlus);
    m_pieces[e_5Piece_BoringPiece]  = Piece(e_5Piece_BoringPiece);
    m_pieces[e_5Piece_TheUltimate]  = Piece(e_5Piece_TheUltimate);
}

void Player::PrintAvailablePieces(std::ostream& a_outStream) const
{
    for (uint32_t i = e_minimumPieceIndex; i < e_numberOfPieces ; i++)
    {
        if (m_piecesPresent[i])
        {
            a_outStream << "1";
        }
        else
        {
            a_outStream << "0";
        }
    }
    a_outStream << std::endl;
}

void Player::PrintNucleationPoints(std::ostream& a_outStream) const
{
    a_outStream << "  ";
    for (int32_t j = 0; j < m_nColumnsInBoard; j++)
    {
        a_outStream << " " << std::setfill(' ') << std::setw(3) << j;
    }
    a_outStream << std::endl;

    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < m_nRowsInBoard; thisCoord.m_row++)
    {
        a_outStream << "   +";
        for (thisCoord.m_col = 0; thisCoord.m_col < m_nColumnsInBoard; thisCoord.m_col++)
        {
            a_outStream << "---+";
        }
        a_outStream << std::endl;

        a_outStream << std::setfill(' ') << std::setw(2) << thisCoord.m_row << " |";
        for (thisCoord.m_col = 0; thisCoord.m_col < m_nColumnsInBoard; thisCoord.m_col++)
        {
            a_outStream << " ";
            if (IsNucleationPoint(thisCoord))
            {
                a_outStream << m_presentationChar;
            }
            else
            {
                a_outStream << " ";
            }
            a_outStream << " |";
        }
        a_outStream << std::endl;
    }

    a_outStream << "   +";
    for (thisCoord.m_col = 0; thisCoord.m_col < m_nColumnsInBoard; thisCoord.m_col++)
    {
        a_outStream << "---+";
    }
    a_outStream << std::endl;
}

int32_t Player::GetAllNucleationPoints(
		Coordinate out_nucleationPoints[], int32_t a_size) const
{
	int32_t nNucleationPoints = 0;

	Coordinate thisCoord(0, 0);
    for (thisCoord.m_row = 0; thisCoord.m_row < m_nRowsInBoard ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < m_nColumnsInBoard ; thisCoord.m_col++)
        {
        	if (IsNucleationPoint(thisCoord))
            {
                if (nNucleationPoints < a_size)
                {
                    out_nucleationPoints[nNucleationPoints] = thisCoord;
                }
                nNucleationPoints++;
            }
        }
    }

    return nNucleationPoints;
}

bool Player::GetFirstNucleationPointSpiral(
        SpiralIterator &iterator, Coordinate &out_coord) const
{
	int32_t rowCount    = m_nRowsInBoard / 2;
	int32_t columnCount = m_nColumnsInBoard / 2;
    int32_t startRow, startCol, endRow, endCol;
	Coordinate thisCoord(0, 0);
    iterator = 0;

    int32_t maxRotationalIndex = 1 + std::max(m_nRowsInBoard / 2, m_nColumnsInBoard / 2);
    while (iterator <= maxRotationalIndex)
    {
        startRow = std::max(0, rowCount - iterator);
        startCol = std::max(0, columnCount - iterator);
        endRow   = std::min(m_nRowsInBoard - 1, rowCount + iterator);
        endCol   = std::min(m_nColumnsInBoard - 1, columnCount + iterator);

        for (thisCoord.m_row = startRow; thisCoord.m_row <= endRow; thisCoord.m_row++)
        {
            for (thisCoord.m_col = startCol; thisCoord.m_col <= endCol; thisCoord.m_col++)
            {
                if ( (thisCoord.m_col == (columnCount - iterator)) ||
                     (thisCoord.m_col == (columnCount + iterator)) ||
                     (thisCoord.m_row == (rowCount - iterator))    ||
                     (thisCoord.m_row == (rowCount + iterator))    )
                {
                    if (IsNucleationPoint(thisCoord))
                    {
                        out_coord = thisCoord;
                        return true;
                    }
                }
            } // for (thisCoord.m_col = (columnCount - iterator) ;
        } // for (thisCoord.m_row = (rowCount - iterator) ;

        iterator++;
    } // while (iterator <= maxRotationalIndex)

    return false;
}

bool Player::GetNextNucleationPointSpiral(
        SpiralIterator &iterator, Coordinate &in_out_coord) const
{
#ifdef DEBUG
    assert(in_out_coord.m_row >= 0);
    assert(in_out_coord.m_row <  m_nRowsInBoard);
    assert(in_out_coord.m_col >= 0);
    assert(in_out_coord.m_col <  m_nColumnsInBoard);
#endif

	int32_t rowCount    = m_nRowsInBoard / 2;
	int32_t columnCount = m_nColumnsInBoard / 2;
    int32_t startRow, startCol, endRow, endCol;
	Coordinate thisCoord(0, 0);

    // check the current line before incrementing this row
	thisCoord.m_row = in_out_coord.m_row;
    endCol = std::min(m_nColumnsInBoard - 1, columnCount + iterator);
    for (thisCoord.m_col = (in_out_coord.m_col + 1);
         thisCoord.m_col <= endCol;
         thisCoord.m_col++)
    {
        if ( (thisCoord.m_col == (columnCount - iterator)) || (thisCoord.m_col == (columnCount + iterator)) ||
             (thisCoord.m_row == (rowCount - iterator))    || (thisCoord.m_row == (rowCount + iterator)) )
        {
            if (IsNucleationPoint(thisCoord))
            {
                in_out_coord = thisCoord;
                return true;
            }
        }
    }// for (thisCoord.m_col = (in_out_coord.m_col + 1);

    // check all the possibilities for next row before incrementing the rotational index
    startCol = std::max(0, columnCount - iterator);
    endRow   = std::min(m_nRowsInBoard - 1, rowCount + iterator);
    endCol   = std::min(m_nColumnsInBoard - 1, columnCount + iterator);
    for (thisCoord.m_row  = (in_out_coord.m_row + 1) ;
         thisCoord.m_row <= endRow;
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = startCol; thisCoord.m_col <= endCol; thisCoord.m_col++)
        {
            if ( (thisCoord.m_col == (columnCount - iterator)) || (thisCoord.m_col == (columnCount + iterator)) ||
                 (thisCoord.m_row == (rowCount - iterator))    || (thisCoord.m_row == (rowCount + iterator)) )
            {
                if (IsNucleationPoint(thisCoord))
                {
                    in_out_coord = thisCoord;
                    return true;
                }
            }
        } // for (thisCoord.m_col= (columnCount - iterator) ;
    } // for (thisCoord.m_row= (in_out_coord.m_row + 1) ;

    // go on with the next rotational index
    iterator++;

    int maxRotationalIndex = 1 + std::max(m_nRowsInBoard / 2, m_nColumnsInBoard / 2);
    while (iterator <= maxRotationalIndex)
    {
        startRow = std::max(0, rowCount - iterator);
        startCol = std::max(0, columnCount - iterator);
        endRow   = std::min(m_nRowsInBoard - 1, rowCount + iterator);
        endCol   = std::min(m_nColumnsInBoard - 1, columnCount + iterator);

        for (thisCoord.m_row = startRow; thisCoord.m_row <= endRow; thisCoord.m_row++)
        {
            for (thisCoord.m_col = startCol; thisCoord.m_col <= endCol; thisCoord.m_col++)
            {
                if ( (thisCoord.m_col == (columnCount - iterator)) || (thisCoord.m_col == (columnCount + iterator)) ||
                     (thisCoord.m_row == (rowCount - iterator))    || (thisCoord.m_row == (rowCount + iterator)) )
                {
                    if (IsNucleationPoint(thisCoord))
                    {
                        in_out_coord = thisCoord;
                        return true;
                    }
                }
            } // for (thisCoord.m_col= (columnCount - iterator) ;
        } // for (thisCoord.m_row= (rowCount - iterator) ;
        iterator++;
    } // while (iterator <= maxRotationalIndex)

    return false;
}
