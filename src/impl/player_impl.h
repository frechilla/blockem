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
/// @file  player_impl.h
/// @brief Implementation details of inlined functions of the Player class
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 6-Jul-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef PLAYER_IMPL_H_
#define PLAYER_IMPL_H_

inline
void Player::SetColour(uint8_t a_colourRed, uint8_t a_colourGreen, uint8_t a_colourBlue)
{
    m_colourRed   = a_colourRed;
    m_colourGreen = a_colourGreen;
    m_colourBlue  = a_colourBlue;
}

inline
void Player::GetColour(uint8_t &out_colourRed, uint8_t &out_colourGreen, uint8_t &out_colourBlue) const
{
    out_colourRed   = m_colourRed;
    out_colourGreen = m_colourGreen;
    out_colourBlue  = m_colourBlue;
}

inline
char Player::PresentationChar() const
{
    return m_presentationChar;
}

inline
const std::string& Player::GetName() const
{
    return m_name;
}

inline
void Player::SetName(std::string& a_newName)
{
    m_name = a_newName;
}

inline
const Coordinate& Player::GetStartingCoordinate() const
{
    return m_startingCoordinate;
}

inline
void Player::SetStartingCoordinate(const Coordinate &a_startingCoordinate)
{
    m_startingCoordinate = a_startingCoordinate;
}

inline
uint8_t Player::NumberOfPiecesAvailable() const
{
    return m_nPiecesAvailable;
}

inline
int32_t Player::NumberOfNucleationPoints() const
{
    return m_nkPointsCount;
}

inline
int32_t Player::InfluenceAreaSize() const
{
    return m_influencedCoordsCount;
}

inline
void Player::UnsetPiece(ePieceType_t a_piece)
{
#ifdef DEBUG
    assert((a_piece >= e_minimumPieceIndex) && (a_piece < e_numberOfPieces));
    assert(m_piecesPresent[a_piece] == true);
#endif
    m_piecesPresent[a_piece] = false;
    m_nPiecesAvailable--;
}


inline
void Player::SetPiece(ePieceType_t a_piece)
{
#ifdef DEBUG
    assert((a_piece >= e_minimumPieceIndex) && (a_piece < e_numberOfPieces));
    assert(m_piecesPresent[a_piece] == false);
#endif
    m_piecesPresent[a_piece] = true;
    m_nPiecesAvailable++;
}

inline
bool Player::IsPieceAvailable(ePieceType_t a_piece) const
{
#ifdef DEBUG
    assert((a_piece >= e_minimumPieceIndex) && (a_piece < e_numberOfPieces));
#endif
    return m_piecesPresent[a_piece];
}

inline
bool Player::IsCoordInfluencedByPlayer(const Coordinate &a_coord) const
{
#ifdef DEBUG
    assert((a_coord.m_row >= 0) && (a_coord.m_row < m_nRowsInBoard));
    assert((a_coord.m_col >= 0) && (a_coord.m_col < m_nColumnsInBoard));
#endif
    uint8_t coordProperty = m_coordinateProperties[a_coord.m_row][a_coord.m_col];
    return static_cast<bool>(coordProperty & COORD_PROPERTY_INFLUENCE_AREA);
}

inline
void Player::SetInfluencedCoord(const Coordinate &a_coord)
{
#ifdef DEBUG
    assert((a_coord.m_row >= 0) && (a_coord.m_row < m_nRowsInBoard));
    assert((a_coord.m_col >= 0) && (a_coord.m_col < m_nColumnsInBoard));
#endif
    uint8_t coordProperty = m_coordinateProperties[a_coord.m_row][a_coord.m_col];
    if ((coordProperty & COORD_PROPERTY_INFLUENCE_AREA) == 0)
    {
#ifdef DEBUG
        assert(m_influencedCoordsCount < (m_nRowsInBoard * m_nColumnsInBoard));
#endif
        m_coordinateProperties[a_coord.m_row][a_coord.m_col] |= COORD_PROPERTY_INFLUENCE_AREA;
        m_influencedCoordsCount++;
    }
}

inline
void Player::UnsetInfluencedCoord(const Coordinate &a_coord)
{
#ifdef DEBUG
    assert((a_coord.m_row >= 0) && (a_coord.m_row < m_nRowsInBoard));
    assert((a_coord.m_col >= 0) && (a_coord.m_col < m_nColumnsInBoard));
#endif
    uint8_t coordProperty = m_coordinateProperties[a_coord.m_row][a_coord.m_col];
    if ((coordProperty & COORD_PROPERTY_INFLUENCE_AREA) == COORD_PROPERTY_INFLUENCE_AREA)
    {
#ifdef DEBUG
         assert(m_influencedCoordsCount > 0);
#endif
         m_coordinateProperties[a_coord.m_row][a_coord.m_col] &= ~COORD_PROPERTY_INFLUENCE_AREA;
         m_influencedCoordsCount--;
    }
}

inline
bool Player::IsNucleationPoint(const Coordinate &a_coord) const
{
#ifdef DEBUG
    assert((a_coord.m_row >= 0) && (a_coord.m_row < m_nRowsInBoard));
    assert((a_coord.m_col >= 0) && (a_coord.m_col < m_nColumnsInBoard));
#endif
    uint8_t coordProperty = m_coordinateProperties[a_coord.m_row][a_coord.m_col];
    return static_cast<bool>(coordProperty & COORD_PROPERTY_NKPOINTS);
}

inline
void Player::SetNucleationPoint(const Coordinate &a_coord)
{
#ifdef DEBUG
    assert((a_coord.m_row >= 0) && (a_coord.m_row < m_nRowsInBoard));
    assert((a_coord.m_col >= 0) && (a_coord.m_col < m_nColumnsInBoard));
#endif
    uint8_t coordProperty = m_coordinateProperties[a_coord.m_row][a_coord.m_col];
    if ((coordProperty & COORD_PROPERTY_NKPOINTS) == 0)
    {
#ifdef DEBUG
        assert(m_nkPointsCount < (m_nRowsInBoard * m_nColumnsInBoard));
#endif
        m_coordinateProperties[a_coord.m_row][a_coord.m_col] |= COORD_PROPERTY_NKPOINTS;
        m_nkPointsCount++;
    }
}

inline
void Player::UnsetNucleationPoint(const Coordinate &a_coord)
{
#ifdef DEBUG
    assert((a_coord.m_row >= 0) && (a_coord.m_row < m_nRowsInBoard));
    assert((a_coord.m_col >= 0) && (a_coord.m_col < m_nColumnsInBoard));
#endif
    uint8_t coordProperty = m_coordinateProperties[a_coord.m_row][a_coord.m_col];
    if ((coordProperty & COORD_PROPERTY_NKPOINTS) == COORD_PROPERTY_NKPOINTS)
    {
#ifdef DEBUG
        assert(m_nkPointsCount > 0);
#endif
        m_coordinateProperties[a_coord.m_row][a_coord.m_col] &= ~COORD_PROPERTY_NKPOINTS;
        m_nkPointsCount--;
    }
}

inline
int32_t Player::GetAllNucleationPoints(STLCoordinateSet_t &a_set) const
{
    int32_t nNucleationPoints = 0;

    Coordinate thisCoord(0, 0);
    for (thisCoord.m_row = 0; thisCoord.m_row < m_nRowsInBoard ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < m_nColumnsInBoard ; thisCoord.m_col++)
        {
            if (IsNucleationPoint(thisCoord))
            {
                a_set.insert(thisCoord);
                nNucleationPoints++;
            }
        }
    }

    return nNucleationPoints;
}

inline
bool Player::GetNextNucleationPoint(Coordinate &in_out_coord) const
{
    Coordinate thisCoord(0, 0);
    if (in_out_coord.Initialised())
    {
        thisCoord.m_row = in_out_coord.m_row;
        thisCoord.m_col = in_out_coord.m_col + 1;
        if (thisCoord.m_col >= m_nColumnsInBoard)
        {
            thisCoord.m_col = 0;
            thisCoord.m_row = in_out_coord.m_row + 1;
            if (thisCoord.m_row >= m_nRowsInBoard)
            {
                // the end of the board;
                return false;
            }
        }
    }

    while (thisCoord.m_col < m_nColumnsInBoard)
    {
        if (IsNucleationPoint(thisCoord))
        {
            in_out_coord = thisCoord;
            return true;
        }

        thisCoord.m_col++;
    }

    for (thisCoord.m_row++; thisCoord.m_row < m_nRowsInBoard; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < m_nColumnsInBoard; thisCoord.m_col++)
        {
            if (IsNucleationPoint(thisCoord))
            {
                in_out_coord = thisCoord;
                return true;
            }
        }
    }

    return false;
}

#endif // PLAYER_IMPL_H_
