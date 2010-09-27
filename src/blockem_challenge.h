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
#include "coordinate_set.h" // STLCoordinateSet_t
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
    /// WARNING: This method is NOT thread safe. If several threads are parsing
    /// different .xml files they should do so with different instances of this
    /// class
    void LoadXMLChallenge(const std::string &a_path)  throw (std::runtime_error);

    /// resets the challenge stored in this object
    void Reset();

    int32_t GetBoardRows()
    {
        return m_nRows;
    }

    int32_t GetBoardColumns()
    {
        return m_nColumns;
    }

private:
    /// XML parsing internal pointer. It is set to NULL after an XML file has
    /// been completely parsed (or a fatal error is given)
    xmlDocPtr m_xmlDoc;

    /// Name of the challenge
    std::string m_challengeName;

    /// Number of rows of the board
    int32_t m_nRows;
    /// Number of columns of the board
    int32_t m_nColumns;

    /// Is the opponent active (is it expected to put down pieces?)
    bool m_opponentActive;
    /// Squares taken by the opponent
    STLCoordinateSet_t m_opponentTakenSquares;
    /// Available pieces of the opponent
    bool m_opponentPiecesPresent[e_numberOfPieces];
    /// Starting coordinate of the opponent (it is only used
    /// if it is active)
    Coordinate m_opponentStartingCoord;

    /// Squares taken by the challenger
    /// m_challengerTakenSquares INTERSECTION m_challengerTakenSquares
    ///     MUST be the empty set
    STLCoordinateSet_t m_challengerTakenSquares;
    /// Available pieces of the challenger
    bool m_challengerPiecesPresent[e_numberOfPieces];
    /// Starting coordinate of the challenger
    Coordinate m_challengerStartingCoord;

    /// resets internal XML variables and throws a std::runtime_exception
    /// that contains a_xmlFile and a_errorMsg
    void XMLParsingFatalError (
        const std::string &a_xmlFile,
        const std::string &a_errorMsg) throw (std::runtime_error);

    /// parses the contents of "board" tag of a challenge .xml file
    /// throws a std::runtime_error if there is any kind of error in
    /// the parsing process
    /// @param xml file being parsed
    /// @param board_node MUST point to the XML point which holds the "board" node
    void XMLParseTagBoard(
            const std::string &a_xmlFile,
            xmlNode* board_node) throw (std::runtime_error);

    /// parses the contents of "opponent" tag of a challenge .xml file
    /// throws a std::runtime_error if there is any kind of error in
    /// the parsing process
    /// @param xml file being parsed
    /// @param opponent_node MUST point to the XML point which holds the "opponent" node
    void XMLParseTagOpponent(
            const std::string &a_xmlFile,
            xmlNode* opponent_node) throw (std::runtime_error);

    /// parses the contents of "challenger" tag of a challenge .xml file
    /// throws a std::runtime_error if there is any kind of error in
    /// the parsing process
    /// @param xml file being parsed
    /// @param opponent_node MUST point to the XML point which holds the "challenger" node
    void XMLParseTagChallenger(
            const std::string &a_xmlFile,
            xmlNode* challenger_node) throw (std::runtime_error);

    void SetChallengeName(const std::string &a_name)
    {
        m_challengeName = a_name;
    }

    void SetNRows(int32_t a_nRows)
    {
        m_nRows = a_nRows;
    }

    void SetNColumns(int32_t a_nRows)
    {
        m_nRows = a_nRows;
    }

    void SetOpponentActive(bool a_isOpponentActive)
    {
        m_opponentActive = a_isOpponentActive;
    }

    void SetOpponentStartingCoord(const Coordinate &a_coord)
    {
        m_opponentStartingCoord = a_coord;
    }
};

#endif // __BLOCKEM_CHALLENGE__
