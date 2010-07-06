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
/// @file  piece_impl.h
/// @brief Implementation details of inlined functions of the Piece class
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 6-Jul-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef __PIECE_IMPL_H__
#define __PIECE_IMPL_H__

inline
uint8_t Piece::GetNRotations() const
{
#ifdef DEBUG
    assert(m_initialised);
#endif
    return m_origRotations;
}

inline
bool Piece::CanMirror() const
{
#ifdef DEBUG
    assert(m_initialised);
#endif
    return m_origMirror;
}

inline
bool Piece::IsMirrored() const
{
#ifdef DEBUG
    assert(m_initialised);
#endif
    return ( m_origMirror && (m_nMirrors & 0x01) );
}

inline
ePieceType_t Piece::GetType() const
{
    return m_type;
}

inline
std::size_t Piece::GetNSquares() const
{
    return m_currentConf.m_pieceSquares.size();
}

inline
uint8_t Piece::GetRadius() const
{
    return m_radius;
}

inline
const std::list<PieceConfiguration>& Piece::GetPrecalculatedConfs() const
{
    return m_precalculatedConfsList;
}

inline
const std::list<uint64_t>& Piece::GetBitwiseList() const
{
    return m_bitwiseRepresentationList;
}

inline
void Piece::SetCurrentConfiguration(const PieceConfiguration &a_newConf)
{
#ifdef DEBUG
    assert(a_newConf.m_pieceSquares.size()  == m_currentConf.m_pieceSquares.size());
    assert(a_newConf.m_nkPoints.size()      == m_currentConf.m_nkPoints.size());
    assert(a_newConf.m_forbiddenArea.size() == m_currentConf.m_forbiddenArea.size());
#endif
    for (std::size_t i = 0; i < m_currentConf.m_pieceSquares.size(); i++)
    {
        m_currentConf.m_pieceSquares[i] = a_newConf.m_pieceSquares[i];
    }
    for (std::size_t i = 0; i < m_currentConf.m_nkPoints.size(); i++)
    {
        m_currentConf.m_nkPoints[i] = a_newConf.m_nkPoints[i];
    }
    for (std::size_t i = 0; i < m_currentConf.m_forbiddenArea.size(); i++)
    {
        m_currentConf.m_forbiddenArea[i] = a_newConf.m_forbiddenArea[i];
    }
}

inline
const PieceConfiguration& Piece::GetCurrentConfiguration() const
{
    return m_currentConf;
}

inline
const Coordinate& Piece::GetCoord(int32_t a_squareIndex) const
{
#ifdef DEBUG
    assert(a_squareIndex < static_cast<int32_t>(GetNSquares()));
#endif
    return m_currentConf.m_pieceSquares[a_squareIndex];
}

#endif // __PIECE_IMPL_H__
