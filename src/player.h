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
/// @file  player.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2009  Original development
///           Faustino Frechilla 16-Jun-2010  Influence area support
/// @endhistory
///
// ============================================================================

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include "assert.h"
#include "piece.h"
#include "coordinate.h"
#include "coordinate_set.h"

// masks for the coordinate properties saved in Player::m_coordinateProperties
// each bit says if that specific property is present (0 false, 1 true)
#define COORD_PROPERTY_NKPOINTS       0x01  // bit 1 (lowest significant bit)
#define COORD_PROPERTY_INFLUENCE_AREA 0x02  // bit 2
#define COORD_PROPERTY_MASK           0x03  // 2 lowest significant bits

class Player
{
public:
	/// type of the iterator used to retrieve the nk points of this player in a spiral shape
    typedef int32_t SpiralIterator;

    /// @param name is the name of the player
    /// @param presenChar is the character that represents the player
	/// @param a_rowsInBoard
	/// @param a_columnsInBoard
    /// @param a_startingCoordinate
    Player(
            std::string a_name,
            char a_presentationChar,
            int32_t a_rowsInBoard,
            int32_t a_columnsInBoard,
            const Coordinate &a_startingCoordinate);
    virtual ~Player();

    /// @brief copy constructor. Needed because there's some dynamic memory
    Player(const Player &a_src);
    /// @brief operator=. Needed because there's some dynamic memory
    Player& operator= (const Player &a_src);

    /// array with this player's pieces
    Piece m_pieces[e_numberOfPieces];

    /// Reset the player to its original configuration
    /// that is, no nucleation points saved and all the pieces present are reset too
    /// @param player's starting coordinate
    void Reset(const Coordinate &a_startingCoordinate);

    /// Sets the colour which represents this player
    /// values are uint8_t, that is, from 0 to 255
    void SetColour(uint8_t a_colourRed, uint8_t a_colourGreen, uint8_t a_colourBlue);

    /// Gets the colour which represents this player
    /// values are uint8_t, that is, from 0 to 255
    void GetColour(uint8_t &out_colourRed, uint8_t &out_colourGreen, uint8_t &out_colourBlue) const;

    /// Returns the character that represents the player
    char PresentationChar() const;

    /// Returns the name of the player
    const std::string& GetName() const;

    /// modifies the name of the player
    void SetName(std::string& a_newName);

    /// Returns the starting coordinate of this player
    const Coordinate& GetStartingCoordinate() const;

    /// @brief sets the starting coordinate of this player
    void SetStartingCoordinate(const Coordinate &a_startingCoordinate);

    /// @return the number of pieces available
    uint8_t NumberOfPiecesAvailable() const;

    /// @return number of nucleation points of this player
    int32_t NumberOfNucleationPoints() const;

    /// @return size of the influence area (in squares)
    int32_t InfluenceAreaSize() const;

    /// Set the piece as not present
    void UnsetPiece(ePieceType_t a_piece);

    /// Set the piece as present
    void SetPiece(ePieceType_t a_piece);

    /// returns true if the player still has the piece
    bool IsPieceAvailable(ePieceType_t a_piece) const;

    /// returns true if a_coord is influenced by the player. Definition of "influenced"
    /// is a bit vague. Have a look at rules::RecalculateInfluenceAreaAroundPiece
    /// to know what "influence" in blockem
    bool IsCoordInfluencedByPlayer(const Coordinate &a_coord) const;

    /// sets a specific coord as "influenced" by this player
    void SetInfluencedCoord(const Coordinate &a_coord);

    /// set a specific coord as not influenced by this player
    void UnsetInfluencedCoord(const Coordinate &a_coord);

    /// returns true if this player has a nucleation point in the coords specified
    bool IsNucleationPoint(const Coordinate &a_coord) const;

    /// save a nucleation point in the coords passed as parameters
    void SetNucleationPoint(const Coordinate &a_coord);

    /// delete a nucleation point in the coords passed as parameters
    void UnsetNucleationPoint(const Coordinate &a_coord);

    /// Get all the nucleation points in the board. The user has to ensure the size of the array is big enough (for example
    /// 2/5 the size of the board) If the size of the array wasn't big enough this function will save a_size
    /// into the out_nucleationPoints array and return the total number of nk points (bigger than a_size)
    /// @param array to be declared as 'Coordinate values[size]'. It will save the nucleation points
    /// @param size of the array.
    /// @return the number of nucleation points saved into the output array
    int32_t GetAllNucleationPoints(
            Coordinate out_nucleationPoints[], int32_t a_size) const;

    /// Get all the nucleation points in the board.
    /// @param set to save the nucleation points
    /// @return the number of nucleation points saved into the output set
    int32_t GetAllNucleationPoints(STLCoordinateSet_t &a_set) const;

    /// Get the first nucleation point in the board starting from the center of the board
    /// and keep on going on a spiral shape
    /// The result will be saved in the variables out_coordX and out_coordY
    /// @param iterator used to go through the board rotating from the center to the borders. It is reseted in this function
    ///     It is used also in GetNextNucleationPoint
    /// @param out_coord
    /// @return true if a nk was found. False otherwise
    bool GetFirstNucleationPointSpiral(
            SpiralIterator &iterator, Coordinate &out_coord) const;

    /// Get Next nucleation point starting from the next point to the one represented by in_out_coord.
    /// if the user wants the function to start from the beginning he/she should use GetFirstNucleationPoint
    /// IT STARTS CHECKING FROM THE NEXT POINT TO THE COORDS SPECIFIED BY in_out_coord
    /// @param iterator used to go through the board rotating from the center to the borders.
    ///     the function GetFirstNucleationPoint must be called first to initialiase the iterator
    ///     if GetNextNucleationPoint is called before GetFirstNucleationPoint the algorithm won't work as expected
    /// @param in_out_coordX
    /// @return true if a nk was found. False otherwise
    bool GetNextNucleationPointSpiral(
            SpiralIterator &iterator, Coordinate &in_out_coord) const;

    /// @brief gets next nucleation point (starts from the following coord of in_out_coord)
    /// If in_out_coord is uninitialised it starts from (0, 0)
    /// @param in_out_coord must contain current coordinate from which the algorithm will start
    ///        looking for nk points. It must be uninitialised first time is called to start off (0, 0)
    ///        it will contain the next nk point found if the function returns true. It will be unmodified
    ///        if no nk point was found (false is returned in this case)
    /// @return true if a new nk point was found. False otherwise
    bool GetNextNucleationPoint(Coordinate &in_out_coord) const;

    /// print a list of bits indicating which pieces are present and which not
    void PrintAvailablePieces(std::ostream& a_outStream) const;

    /// print the nucleation points of this player in a board
    void PrintNucleationPoints(std::ostream& a_outStream) const;

private:
    /// the name of the player
    std::string m_name;
    /// represents the pieces available
    bool m_piecesPresent[e_numberOfPieces];
    /// the character that represents the player
    char m_presentationChar;
    /// amount of pieces available
    uint8_t m_nPiecesAvailable;
    /// number of rows of the board where the player is playing
    int32_t m_nRowsInBoard;
    /// number of columns of the board where the player is playing
    int32_t m_nColumnsInBoard;
    /// Array with the properties of this player related to every place
    /// in the board (every coordinate)
    uint8_t** m_coordinateProperties;
    /// Number of nucleation points of this player (saved in m_coordinateProperties)
    int32_t m_nkPointsCount;
    /// Number of coords influenced by this player
    int32_t m_influencedCoordsCount;
    /// Starting coordinate of this player
    Coordinate m_startingCoordinate;
    /// Red channel of the Colour representation of the player
    uint8_t m_colourRed;
    /// Green channel of the Colour representation of the player
    uint8_t m_colourGreen;
    /// Blue channel of the Colour representation of the player
    uint8_t m_colourBlue;

    /// load all pieces into the m_pieces array
    void LoadPieces();

    /// @brief copy the source player argument into the current object
    /// It doesn't delete or allocate any memory, so when used in
    /// operator= or copy constructor that should be taken care of
    /// before calling to the function
    void CopyFromObject(const Player &a_src);

    // prevent players to be created without the proper arguments
    Player();
};

// include implementation details of inline functions
#include "impl/player_impl.h"

#endif /* PLAYER_H_ */
