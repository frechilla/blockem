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
            const Coordinate &a_startingCoordinate = Coordinate(0,0));
    virtual ~Player();

    /// @brief copy constructor. Needed because there's some dynamic memory
    Player(const Player &a_src);
    /// @brief operator=. Needed because there's some dynamic memory
    Player& operator= (const Player &a_src);

    /// array with this player's pieces
    Piece m_pieces[e_numberOfPieces];

    /// Reset the player to its original configuration
    /// that is, no nucleation points saved and all the pieces present and reset too
    /// @param player's starting coordinate
    void Reset(const Coordinate &a_startingCoordinate = Coordinate(0,0));

    /// Sets the colour which represents this player
    /// values are uint8_t, that is, from 0 to 255
    void SetColour(uint8_t a_colourRed, uint8_t a_colourGreen, uint8_t a_colourBlue);

    /// Gets the colour which represents this player
    /// values are uint8_t, that is, from 0 to 255
    void GetColour(uint8_t &out_colourRed, uint8_t &out_colourGreen, uint8_t &out_colourBlue) const;

    /// Returns the character that represents the player
    inline char PresentationChar() const
    {
    	return m_presentationChar;
    }

    /// Returns the name of the player
    inline const std::string& GetName() const
	{
    	return m_name;
	}

    /// modifies the name of the player
    inline void SetName(std::string& a_newName)
    {
        m_name = a_newName;
    }

    /// Returns the starting coordinate of this player
    inline const Coordinate& GetStartingCoordinate() const
    {
        return m_startingCoordinate;
    }

    inline void SetStartingCoordinate(const Coordinate &a_startingCoordinate)
    {
        m_startingCoordinate = a_startingCoordinate;
    }

    /// returns the number of pieces available
    inline uint8_t NumberOfPiecesAvailable() const
    {
        return m_nPiecesAvailable;
    }

    inline int32_t NumberOfNucleationPoints() const
    {
    	return m_nkPointsCount;
    }

    /// Set the piece as not present
    inline void UnsetPiece(ePieceType_t a_piece)
    {
#ifdef DEBUG
        assert((a_piece >= e_minimumPieceIndex) && (a_piece < e_numberOfPieces));
        assert(m_piecesPresent[a_piece] == true);
#endif
        m_piecesPresent[a_piece] = false;
        m_nPiecesAvailable--;
    }

    /// Set the piece as present
    inline void SetPiece(ePieceType_t a_piece)
    {
#ifdef DEBUG
        assert((a_piece >= e_minimumPieceIndex) && (a_piece < e_numberOfPieces));
        assert(m_piecesPresent[a_piece] == false);
#endif
        m_piecesPresent[a_piece] = true;
        m_nPiecesAvailable++;
    }

    /// returns true if the player still has the piece
    inline bool IsPieceAvailable(ePieceType_t a_piece) const
    {
#ifdef DEBUG
        assert((a_piece >= e_minimumPieceIndex) && (a_piece < e_numberOfPieces));
#endif
        return m_piecesPresent[a_piece];
    }

    /// returns true if this player has a nucleation point in the coords specified
    inline bool IsNucleationPoint(const Coordinate &a_coord) const
    {
#ifdef DEBUG
        assert((a_coord.m_row >= 0) && (a_coord.m_row < m_nRowsInBoard));
        assert((a_coord.m_col >= 0) && (a_coord.m_col < m_nColumnsInBoard));
#endif
        return static_cast<bool>(m_nkPoints[a_coord.m_row][a_coord.m_col]);
    }

    /// save a nucleation point in the coords passed as parameters
    inline void SetNucleationPoint(const Coordinate &a_coord)
    {
#ifdef DEBUG
    	assert((a_coord.m_row >= 0) && (a_coord.m_row < m_nRowsInBoard));
    	assert((a_coord.m_col >= 0) && (a_coord.m_col < m_nColumnsInBoard));
#endif
        if (m_nkPoints[a_coord.m_row][a_coord.m_col] == 0)
        {
#ifdef DEBUG
            assert(m_nkPointsCount < (m_nRowsInBoard * m_nColumnsInBoard));
#endif
            m_nkPoints[a_coord.m_row][a_coord.m_col] = 1;
            m_nkPointsCount++;
        }
    }

    /// delete a nucleation point in the coords passed as parameters
    inline void UnsetNucleationPoint(const Coordinate &a_coord)
    {
#ifdef DEBUG
    	assert((a_coord.m_row >= 0) && (a_coord.m_row < m_nRowsInBoard));
    	assert((a_coord.m_col >= 0) && (a_coord.m_col < m_nColumnsInBoard));
#endif
        if (m_nkPoints[a_coord.m_row][a_coord.m_col])
        {
#ifdef DEBUG
             assert(m_nkPointsCount > 0);
#endif
            m_nkPoints[a_coord.m_row][a_coord.m_col] = 0;
            m_nkPointsCount--;
        }
    }

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
    int32_t GetAllNucleationPoints(
    		STLCoordinateSet_t &a_set) const;

    /// Get all the nucleation points in the board. Save the results in a special kind of set
    /// without memory allocation on the heap
    /// @param set to save the nucleation points
    /// @return the number of nucleation points saved into the output set
    template<int32_t ROWS, int32_t COLUMNS>
    int32_t GetAllNucleationPoints(
            CoordinateSet<ROWS, COLUMNS> &a_set) const
    {
#ifdef DEBUG
        assert(ROWS    == m_nRowsInBoard);
        assert(COLUMNS == m_nColumnsInBoard);
#endif
        int32_t nNucleationPoints = 0;
        Coordinate thisCoord;

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
    /// Nucleation points in the board corresponding to this player
    uint8_t** m_nkPoints;
    /// Number of nucleation points saved in the nkPoints array
    int32_t m_nkPointsCount;
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

#endif /* PLAYER_H_ */
