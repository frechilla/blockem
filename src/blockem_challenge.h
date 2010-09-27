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
/// @file  blockem_challenge.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 26-Sep-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef __BLOCKEM_CHALLENGE__
#define __BLOCKEM_CHALLENGE__

#include <set>
#include "coordinate.h"
#include "piece.h"

/// @brief This class represents a blockem challenge
/// Challenges are supposed to be loaded from .xml files
class BlockemChallenge
{
public:
    /// @brief instantiates an object with a default challenge
    /// a default challenge is an empty board of 14x14 in which
    /// only one player tries to put down all its pieces
    BlockemChallenge();
    /// @brief instantiates an object loading the blockem challenge from a_path
    /// loads the .xml file 'a_path' content into the new object
    /// throws a new std::runtime_error exception if the xml file can't be loaded
    BlockemChallenge(const std::string &a_path) throw (std::runtime_error);
    ~BlockemChallenge();

    /// @brief loads the .xml file 'a_path' content into the current object
    /// deletes the current challenge loaded in 'this'
    /// throws a new std::runtime_error exception if the xml file can't be loaded
    void LoadXMLChallenge(const std::string &a_path)  throw (std::runtime_error);

private:
    /// Name of the challenge
    std::string m_challengeName;

    /// Number of rows of the board
    int32_t m_nRows;
    /// Number of columns of the board
    int32_t m_nColumns;

    /// Is the opponent active (is it expected to put down pieces?)
    bool m_opponentActive;
    /// Squares taken by the opponent
    std::set<Coordinate> m_opponentTakenSquares;
    /// Available pieces of the opponent
    bool m_opponentPiecesPresent[e_numberOfPieces];
    /// Starting coordinate of the opponent (it is only used
    /// if it is active)
    Coordinate m_opponentStartingCoord;

    /// Squares taken by the challenger
    /// m_challengerTakenSquares INTERSECTION m_challengerTakenSquares
    ///     MUST be the empty set
    std::set<Coordinate> m_challengerTakenSquares;
    /// Available pieces of the challenger
    bool m_challengerPiecesPresent[e_numberOfPieces];
    /// Starting coordinate of the challenger
    Coordinate m_challengerStartingCoord;

    void SetChallengeName(const std::string &a_name)
    {
        m_challengeName = a_name;
    }
};

#endif // __BLOCKEM_CHALLENGE__
