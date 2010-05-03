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
/// @file  piece.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2009  Original development
///           Faustino Frechilla 19-Apr-2010  Bitwise representations
/// @endhistory
///
// ============================================================================

#ifndef __PIECE_H__
#define __PIECE_H__

#include <string>
#include <stdint.h> // for types
#include <list>
#include "assert.h"
#include "coordinate.h"

/// maximum number of squares a piece can have
const uint8_t PIECE_MAX_SQUARES = 5;

/// @brief available blokus pieces
typedef enum
{
    // if this enum is modified, ensure all the constants are updated accordingly
	e_minimumPieceIndex = 0,

	e_1Piece_BabyPiece   = e_minimumPieceIndex,
	e_2Piece_TwoPiece    = 1,
	e_3Piece_LongPiece   = 2,
	e_3Piece_Triangle    = 3,
	e_4Piece_LongPiece   = 4,
	e_4Piece_LittleS     = 5,
	e_4Piece_LittleT     = 6,
	e_4Piece_LittleL     = 7,  // aka little brother
	e_4Piece_FullSquare  = 8,
	e_5Piece_BigS        = 9,  // aka big Z
	e_5Piece_SafPiece    = 10,
	e_5Piece_WPiece      = 11,
	e_5Piece_CuntPiece   = 12,
	e_5Piece_BigPenis    = 13,
	e_5Piece_Cross       = 14,
	e_5Piece_HalfSquare  = 15, // aka big L, aka Mike's piece
	e_5Piece_BigL        = 16, // aka big brother
	e_5Piece_MrT         = 17,
	e_5Piece_SquarePlus  = 18, // (square++). formerly known as squareAppen (square + appendix)
	e_5Piece_BoringPiece = 19,
	e_5Piece_TheUltimate = 20,

	e_numberOfPieces = 21,
	e_noPiece = 22,

} ePieceType_t;

class Piece
{
public:
    // @brief an uninitialised piece of type e_noPiece. It's the same as calling Piece(e_noPiece)
    Piece();
	/// @brief constructor
	/// @param a_type type of blokus piece
	Piece(ePieceType_t a_type);
    virtual ~Piece();

    /// @brief array of strings which describe every possible piece
    static const std::string PieceDescription[e_numberOfPieces];

    /// list of coords that describe the shape of the piece
    /// Stores one coordinate per square
    Coordinate m_coords[PIECE_MAX_SQUARES];

    /// Reset the piece to the original configuration
    void Reset();

    /// rotates this piece 90 degrees to the right. It will save the result in the current object
    /// @return times the piece has been rotated to the right
    void RotateRight();

    /// rotates this piece 90 degrees tot he left. It will save the result in the current object
    void RotateLeft();

    /// calculates the mirror of the current piece and save the result in this same object
    /// makes the mirror the image in the Y axis
    /// @return true if the actual configuration of the piece corresponds to the mirror of the original
    ///         false if the actual configuration is the same as the original
    bool MirrorYAxis();

    /// calculates the mirror of the current piece and save the result in this same object
    /// makes the mirror the image in the X axis
    /// @return true if the actual configuration of the piece corresponds to the mirror of the original
    ///         false if the actual configuration is the same as the original
    bool MirrorXAxis();

    /// @returns the number of possible different rotations the piece has originally
    inline int8_t GetNRotations() const
    {
#ifdef DEBUF
        assert(m_initialised);
#endif
        return m_origRotations;
    }

    /// @returns true if the piece can be mirrored
    inline bool CanMirror() const
    {
#ifdef DEBUF
        assert(m_initialised);
#endif
        return m_origMirror;
    }

    /// @return true if the actual configuration of the piece corresponds to the mirror of the original
    inline bool IsMirrored() const
    {
#ifdef DEBUF
        assert(m_initialised);
#endif
        return ( m_origMirror && (m_nMirrors & 0x01) );
    }

    /// returns the type of piece
    inline ePieceType_t GetType() const
    {
        return m_type;
    }

    /// returns the amount of squares the piece is made of
    inline uint8_t GetNSquares() const
    {
    	return m_nSquares;
    }

    /// returns the half of the size of the side of the square where the piece fits
    /// the real size of the square is: ( (squareSideSize * 2) + 1)
    /// For example the fullSquare will be 1, and the baby piece should be 0
    inline uint8_t GetSquareSideHalfSize() const
    {
        return m_squareSideHalfSize;
    }
    
    /// returns the list of bitwise representations of the piece
    /// have a look at the documentation for BuildUpBitwiseRepresentation to learn
    /// more about this way of representing pieces
    inline const std::list<uint64_t>& GetBitwiseList() const
    {
        return m_bitwiseRepresentationList;
    }

private:
    /// saves the type of piece
    ePieceType_t m_type;
    /// true if the piece has been initialised
    bool m_initialised;
    /// number of square the piece is made of
    uint8_t m_nSquares;
    /// original coords
    Coordinate m_origCoords[PIECE_MAX_SQUARES];
    /// true if the piece has a different mirror image
    bool m_origMirror;
    /// number of possible different rotations
    int8_t  m_origRotations;
    /// save the number of times the function Mirror has been called
    int8_t m_nMirrors;
    ///  half of the size of the side of the square where the piece fits (see comment for SetPiece)
    uint8_t m_squareSideHalfSize;
    /// bitwise representation of al the possible configurations of the piece
    /// have a look at BuildUpBitwiseRepresentation to learn more
    std::list<uint64_t> m_bitwiseRepresentationList;

    /// @brief set original configuration of the piece.
    /// @param array with the coordinates which make up the piece
    /// @param nSquares number of squares of the piece
    /// @param mirror set to true if the piece has a mirror image
    /// @param nRotations number of possible rotations (from 1 to 4)
    /// @param squareSideHalfSize is the half of the size of the side of the square where the piece fits
    ///        the real size of the square is: ( (squareSideSize * 2) + 1)
    ///        For example the fullSquare will be 1, and the baby piece should be 0
    void SetPiece(Coordinate a_coords[PIECE_MAX_SQUARES], uint8_t a_nSquares, bool a_mirror, int8_t a_nRotations, uint8_t a_squareSideHalfSize);

    /// sets the properties of *this* object to represent the a_piece
    /// @param a_piece the piece type to be loaded in this object
    inline void LoadPieceData(ePieceType_t a_piece)
    {
#ifdef DEBUG
        assert( ((a_piece >= e_minimumPieceIndex) && (a_piece < e_numberOfPieces)) && (a_piece != e_noPiece) );
#endif
		(m_loadFunctionMap[a_piece])(*this);
        // build up the bitwise representation too
        BuildUpBitwiseRepresentation();
    }

    // static functions to load the description of each piece in the object passed as parameter
    static void LoadPiece_1BabyPiece  (Piece &thisPiece);
    static void LoadPiece_2TwoPiece   (Piece &thisPiece);
    static void LoadPiece_3LongPiece  (Piece &thisPiece);
    static void LoadPiece_3Triangle   (Piece &thisPiece);
    static void LoadPiece_4LongPiece  (Piece &thisPiece);
    static void LoadPiece_4LittleS    (Piece &thisPiece);
    static void LoadPiece_4LittleT    (Piece &thisPiece);
    static void LoadPiece_4littleL    (Piece &thisPiece);
    static void LoadPiece_4FullSquare (Piece &thisPiece);
    static void LoadPiece_5BigS       (Piece &thisPiece);
    static void LoadPiece_5SafPiece   (Piece &thisPiece);
    static void LoadPiece_5WPiece     (Piece &thisPiece);
    static void LoadPiece_5CuntPiece  (Piece &thisPiece);
    static void LoadPiece_5BigPenis   (Piece &thisPiece);
    static void LoadPiece_5Cross      (Piece &thisPiece);
    static void LoadPiece_5HalfSquare (Piece &thisPiece);
    static void LoadPiece_5BigL       (Piece &thisPiece);
    static void LoadPiece_5MrT        (Piece &thisPiece);
    static void LoadPiece_5SquareAppen(Piece &thisPiece);
    static void LoadPiece_5BoringPiece(Piece &thisPiece);
    static void LoadPiece_5TheUltimate(Piece &thisPiece);

    // mapping the type of pieces to the corresponding Load function
    typedef void (*LoadPieceFunction_t) (Piece &);
    static LoadPieceFunction_t m_loadFunctionMap[e_numberOfPieces];
    
    /// build up the bitwise representation of all the configurations of the piece
    /// this methid must be called once the m_origCoords array and the rest of 
    /// attributes of the object has been set
    /// it rotates and mirrors the piece as many times as possible and then
    /// calls Reset to restore the original status of it. DO NOT CALL TO THIS
    /// FUNCTION ONCE THE PIECE COULD HAVE BEEN EDITED BY AN EXTERNAL USER
    /// BECAUSE IT RESETS THE PIECE
    ///
    /// the bitwise represenation of a piece constist of a "string" of 49 bits
    /// that hold the piece configuration.
    /// Those 49 bits represent 7 rows of 7 bits where the bit in the coordinate
    /// (4, 4) is the center of that piece (given that cords are valid from 1 to 7)
    /// for example, the saf piece would be:
    /// 0 0 0 0 0 0 0
    /// 0 0 0 0 0 0 0
    /// 0 0 0 1 0 0 0
    /// 0 0 0 1 1 0 0
    /// 0 0 1 1 0 0 0
    /// 0 0 0 0 0 0 0
    /// 0 0 0 0 0 0 0
    ///
    /// which is: 000000000000000 0000000 0000000 0001000 0001100 0011000 0000000 0000000
    /// (0x0000000081860000)
    ///
    /// every piece could fit in a square 5x5 but a square 7x7 has been chosen to be able to move
    /// a piece up/down/right/left easily using the same representation (the uint64_t)
    /// Using this representation moving a piece to the right is a matter of (>>1) and to the 
    /// left (<< 1). Moving up and down is (<<7) and (>>7) respectively
    ///
    /// Thses binary representations are saved into the list m_bitwiseRepresentations, which holds
    /// a maximum of 8 (4 rotations X 2 mirrors) confgurations
    void BuildUpBitwiseRepresentation();
};

#endif /* __PIECE_H__ */
