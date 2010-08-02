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
///           Faustino Frechilla  9-MAy-2010  Precalculated representations
/// @endhistory
///
// ============================================================================

#ifndef __PIECE_H__
#define __PIECE_H__

#include <string>
#include <stdint.h> // for types
#include <list>
#include <vector>
#include "assert.h"
#include "coordinate.h"

/// maximum number of squares a piece can have
const std::size_t PIECE_MAX_SQUARES = 5;

/// @brief available blockem pieces
typedef enum
{
    // WARNING:
    // if this enum is modified, ensure all the constants (values) are updated accordingly
    // ensure also Piece::PieceDescription and Piece::m_loadFunctionMap
    // are updated with the correct order described in this enum
    //
    // pieces are described here from less complex to more complex. This ordering might
    // be used in a minimax algorithm to try to prune out useless branches early
    // using the alphabeta prunning

    e_minimumPieceIndex = 0,

    e_1Piece_BabyPiece   = e_minimumPieceIndex,
    e_2Piece_TwoPiece    = 1,
    e_3Piece_LongPiece   = 2,
    e_3Piece_Triangle    = 3,
    e_4Piece_FullSquare  = 4,
    e_4Piece_LongPiece   = 5,
    e_4Piece_LittleL     = 6,  // aka little brother
    e_4Piece_LittleS     = 7,
    e_4Piece_LittleT     = 8,
    e_5Piece_BigPenis    = 9,
    e_5Piece_SquarePlus  = 10, // (square++). formerly known as squareAppen (square + appendix)
    e_5Piece_HalfSquare  = 11, // aka big L, aka Mike's piece
    e_5Piece_CuntPiece   = 12,
    e_5Piece_BigL        = 13, // aka big brother
    e_5Piece_BoringPiece = 14,
    e_5Piece_BigS        = 15,  // aka big Z
    e_5Piece_MrT         = 16,
    e_5Piece_TheUltimate = 17,
    e_5Piece_SafPiece    = 18,
    e_5Piece_WPiece      = 19,
    e_5Piece_Cross       = 20,

    e_numberOfPieces = 21,
    e_noPiece = 22,

} ePieceType_t;

/// container used for all the attributes that make up class PieceConfiguration
typedef std::vector<Coordinate> PieceConfigurationContainer_t;

/// A piece configuration represents what the piece is like at the moment
/// Once a piece is rotated/mirrored the piece configuration changes
class PieceConfiguration
{
public:
    PieceConfiguration():
        m_pieceSquares(0),
        m_nkPoints(0),
        m_forbiddenArea(0)
    {};
    ~PieceConfiguration() {};

    // Stores one coordinate per square occupied by the piece
    PieceConfigurationContainer_t m_pieceSquares;
    // 1 coordinate per nk point created by the piece
    PieceConfigurationContainer_t m_nkPoints;
    // 1 coordinate per square which will be forbidden for the player
    // once he/she deploys the piece
    PieceConfigurationContainer_t m_forbiddenArea;
};

/// @brief represents a piece
class Piece
{
public:
	/// @brief constructor
	/// @param a_type type of blokus piece
	Piece(ePieceType_t a_type = e_noPiece);
    virtual ~Piece();

    /// @return a string of characters that describes in some sort of human 
    ///         language (internationalised) the piece 'a_pieceType'
    static const char* GetPieceDescription(ePieceType_t a_pieceType);

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

    /// @return the number of possible different rotations the piece has originally
    uint8_t GetNRotations() const;

    /// @return true if the piece can be mirrored
    bool CanMirror() const;

    /// @return true if the actual configuration of the piece corresponds to the mirror of the original
    bool IsMirrored() const;

    /// @return the type of piece
    ePieceType_t GetType() const;

    /// @return the amount of squares the piece is made of
    std::size_t GetNSquares() const;

    /// @return "radius" of the square where the piece fits
    /// the real size of the square is: ( (squareSideSize * 2) + 1)
    /// For example for fullSquare will be 1, and for baby piece should be 0
    /// and for the bigpenis 2
    uint8_t GetRadius() const;

    /// @return the list of precalculated configurations of the piece
    const std::list<PieceConfiguration>& GetPrecalculatedConfs() const;

    /// @return the list of bitwise representations of the piece
    /// have a look at the documentation for BuildUpBitwiseRepresentation to learn
    /// more about this way of representing pieces
    const std::list<uint64_t>& GetBitwiseList() const;

    /// set current configuration of the piece to the configuration specified by a_newCoords
    /// The user of this function must ensure size of std::vectors in PieceConfiguration "a_newConf"
    /// is the same as the vectors of the piece
    void SetCurrentConfiguration(const PieceConfiguration &a_newConf);

    const PieceConfiguration& GetCurrentConfiguration() const;

    /// gives access to the current coords of the squares that make up the piece
    /// @return the coordinate saved in a_squareIndex in the current piece configuration
    const Coordinate& GetCoord(int32_t a_squareIndex) const;

private:
    /// type of piece
    ePieceType_t m_type;
    /// configuration that describe the current shape of the piece
    PieceConfiguration m_currentConf;
    /// original configuration. To be used in the reset function
    PieceConfiguration m_originalConf;
    /// true if PieceConfiguration piece has a different mirror image
    bool m_origMirror;
    /// number of possible different rotations
    uint8_t m_origRotations;
    /// save the number of times the function Mirror has been called
    int8_t m_nMirrors;
    /// "radius" of the square where the piece fits (see comment for SetPiece)
    uint8_t m_radius;
    /// list of precaculated configurations of the piece
    std::list<PieceConfiguration> m_precalculatedConfsList;
    /// bitwise representation of al the possible configurations of the piece
    /// have a look at BuildUpBitwiseRepresentation to learn more
    std::list<uint64_t> m_bitwiseRepresentationList;

#ifdef DEBUG
    /// true if the piece has been initialised
    bool m_initialised;
#endif

    /// rotates in_out_coord coord 90 degrees to the right. It will save the result
    /// in the current in_out_coord object
    void RotateCoordRight(Coordinate &in_out_coord);
    /// rotates in_out_coord coord 90 degrees to the left. It will save the result
    /// in the current in_out_coord object
    void RotateCoordLeft(Coordinate &in_out_coord);
    /// makes the mirror of in_out_coord in the X axis
    void MirrorCoordXAxis(Coordinate &in_out_coord);
    /// makes the mirror of in_out_coord in the Y axis
    void MirrorCoordYAxis(Coordinate &in_out_coord);

    /// @brief set original configuration of the piece.
    /// @param vector with the coordinates which make up the piece
    /// @param vector with the nk points generated by this piece
    ///        (when in the configuration specified by a_pieceCoords)
    /// @param vector with the forbidden area around this piece (those
    ///        coords which will be touching a piece -not a corner-)
    /// @param mirror set to true if the piece has a mirror image
    /// @param nRotations number of possible rotations (from 1 to 4)
    /// @param radius is half the size of the side of the square where the piece fits
    ///        the real size of the square is: ( (squareSideSize * 2) + 1)
    ///        For example the fullSquare shall be 1, and the baby piece should be 0
    ///        For that piece called bigpenis it is 2
    void SetPiece(
            const std::vector<Coordinate> &a_pieceCoords,
            const std::vector<Coordinate> &a_nkPoints,
            const std::vector<Coordinate> &a_forbiddenArea,
            bool                           a_mirror,
            uint8_t                        a_nRotations,
            uint8_t                        a_radius);

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

    /// @brief type of function used to load the values of the function
    typedef void (*LoadPieceFunction_t) (Piece &);
    /// array to map type of pieces to the corresponding Load function
    static LoadPieceFunction_t m_loadFunctionMap[e_numberOfPieces];

    /// @brief array of strings which describe every possible piece
    static const char* m_pieceDescription[e_numberOfPieces];

    /// @brief build up precalculated list of configurations of this piece
    /// Saves into m_precalculatedConfsList a list of PieceConfiguration
    /// which represents the piece using (its squares + nk points + forbidden area)
    ///
    /// it also build up the bitwise representation of all the configurations of
    /// the piece
    ///
    /// this method must be called once the m_origCoords array and the rest of
    /// attributes of the object has been set (have a look at the constructor of
    /// this class)
    /// it rotates and mirrors the piece as many times as possible and then
    /// calls Reset to restore the original status of it. DO NOT CALL TO THIS
    /// FUNCTION ONCE THE PIECE COULD HAVE BEEN EDITED BY AN EXTERNAL USER
    /// BECAUSE IT RESETS THE PIECE
    ///
    /// the bitwise representation of a piece consists of a "string" of 49 bits
    /// that hold the piece configuration.
    /// Those 49 bits represent 7 rows of 7 bits where the bit in the coordinate
    /// (4, 4) is the centre of that piece (given that cords are valid from 1 to 7)
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
    /// These binary representations are saved into the list m_bitwiseRepresentations, which holds
    /// a maximum of 8 (4 rotations X 2 mirrors) configurations
    void BuildUpPrecalculatedRepresentations();
};

// include implementation details of inline functions
#include "impl/piece_impl.h"

#endif /* __PIECE_H__ */
