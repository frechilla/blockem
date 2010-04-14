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
/// @file  Game1v1.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 28-Aug-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef GAME1V1_H_
#define GAME1V1_H_

#include <istream>
#include <ostream>
#include <csignal> // atomic_t
#include "Piece.h"
#include "Player.h"
#include "Board.h"
#include "heuristic.h"
#include "Rules.h"
#include "Coordinate.h"
#include "CoordinateSet.h"

/// our own version of INFINITE
const int32_t INFINITE = 2147483647;

/// standard number of rows for a 1vs1 game
const int32_t BOARD_1VS1_ROWS    = 14;
/// standard number of columns for a 1vs1 game
const int32_t BOARD_1VS1_COLUMNS = 14;

/// @brief contains the definition of a game 1vs1 of blokus
/// This class is meant to contain all the rules about blokus 1vs1
class Game1v1
{
public:
    /// This coordinate set needs to know the size of the boards,
    /// since it is a static data structure
    typedef CoordinateSet<BOARD_1VS1_ROWS, BOARD_1VS1_COLUMNS> CoordSetGame1v1_t;

    /// function pointer that will be called each time there's a progress update
    /// float will be a value between 0.0 and 1.0
    typedef void(*ProgressFunctor_t)(float);

	/// @brief builds the game
	/// It creates a board and two players,
    ///   * 'me' (computer) which the MinMax algorithm will maximise to win
    ///   * the 'opponent' (user), which the MinMax algorithm will try to beat
    Game1v1();
	virtual ~Game1v1();

	/// @returns the board of the game
	inline const Board& GetBoard() const
	{
	    return m_board;
	}

    /// @returns the player 'me' (the one that the MinMax algorithm will try to make win)
	inline Player& GetPlayerMe()
	{
		return m_playerMe;
	}

	/// @returns the player 'opponent' (the one that the MinMax algorithm will try to beat)
	inline Player& GetPlayerOpponent()
	{
		return m_playerOpponent;
	}

	/// @brief sets the progress functor
	inline void SetProgressHandler(ProgressFunctor_t a_progressFunctor)
	{
	    m_progressFunctor = a_progressFunctor;
	}

	/// puts down the computer piece on the board and sets it to used in player me (computer)
	inline void PutDownPieceMe(
			const Piece      &a_piece,
			const Coordinate &a_coord)
	{
	    Game1v1::PutDownPiece(m_board, a_piece, a_coord, m_playerMe, m_playerOpponent);
	    m_playerMe.UnsetPiece(a_piece.GetType());
	}

	/// remove piece from the board and sets it to unused in player me (computer)
	inline void RemovePieceMe(
			const Piece      &a_piece,
			const Coordinate &a_coord)
	{
	    Game1v1::RemovePiece(m_board, a_piece, a_coord, m_playerMe, m_playerOpponent);
	    m_playerMe.SetPiece(a_piece.GetType());
	}

	/// puts down the computer piece on the board and sets it to used in player opponent (user)
	inline void PutDownPieceOpponent(
			const Piece      &a_piece,
			const Coordinate &a_coord)
	{
	    Game1v1::PutDownPiece(m_board, a_piece, a_coord, m_playerOpponent, m_playerMe);
	    m_playerOpponent.UnsetPiece(a_piece.GetType());
	}

	/// remove piece from the board and sets it to unused in player opponent (user)
	inline void RemovePieceOpponent(
			const Piece      &a_piece,
			const Coordinate &a_coord)
	{
	    Game1v1::RemovePiece(m_board, a_piece, a_coord, m_playerOpponent, m_playerMe);
	    m_playerOpponent.SetPiece(a_piece.GetType());
	}

    /// @brief calculates the next piece to be put down by player 'me' using the heuristic passed as parameter
    /// It uses the minimax algorithm with the alpha beta pruning
	/// @param the heuristic method for the min max algorithm
	/// @param maximum depth for the search tree
	/// @param a reference to a piece where the result will be stored
	/// @param a reference to a Coordinate where the result will be stored
    /// @param parameter with a reference to a variable that could be set
    ///        to non-zero by a different thread indicating to the thread currently stuck
    ///        in the calculation algorithm to stop. If the current thread notices
    ///        this flag variable is set to non-zero it will stop immediately, which means
    ///        every reference used as an output or returned value will have unexpected
    ///        undescribed values.
    /// @param optional parameter with the coordinate where the
    ///        opponent put down the latest piece
	/// @param optional parameter with the latest piece put down by the opponent
	///        if this param is set to Piece(e_noPiece) MinMax won't be able to take into
	///        account the latest opponent's move to compute the next move. it can also
	///        be used to add a bit of randomness to the AI behaviour
    int32_t MinMax(
            Heuristic::calculateMethod_t a_heuristicMethod,
            int32_t                      depth,
            Piece                        &out_resultPiece,
            Coordinate                   &out_coord,
            const volatile sig_atomic_t  &stopProcessingFlag,
            const Coordinate             &a_lastOpponentPieceCoord = Coordinate(),
            const Piece                  &a_lastOpponentPiece = Piece(e_noPiece));

    /// @returns true if the player 'me' (the computer) can put down a piece
    bool CanPlayerMeGo() const;

    /// @returns true if the player 'opponent' (the user) can put down a piece
    bool CanPlayerOpponentGo() const;

    /// @brief Blanks out the board and reset the players so they have all the pieces available
    /// it can be used to restart the game (to start in a new blank game)
    void Reset();

    /// @brief load a game from an in_stream
    /// the in_stream has to contain the output of a game as it is done by Blokus1v1Game::SaveGame
    ///
    /// it doesn't check for dodgy games to be loaded, so the function might have unexpected
    /// results if the game saved in a_inStream is not in the expected format
    /// @param the instream that represents the input to be loaded
    /// @returns true if the loading was correct. False otherwise
    bool LoadGame(std::istream& a_inStream);

    /// @brief saves the game into an out_stream
    /// the data saves into the outStream will be the following:
    /// Board::PrintBoard (BOARD_1VS1_ROWSxBOARD_1VS1_COLUMNS long)
    /// + Player::PrintAvailablePieces() for the player that represents 'me'
    /// + Player::PrintAvailablePieces() for the player that the opponent
    /// @returns true if the game was saved succesfully. False otherwise
    bool SaveGame(std::ostream& a_outStream);

private:
    /// The blokus board where the game will be played
	Board m_board;

	/// The player that represents me (the guy MinMax will try to make win)
	Player m_playerMe;

	/// The player that represents the opponent (the guy MinMax will try to beat)
	Player m_playerOpponent;

	/// functor to notify the progress made by the MinMax algorithm
	ProgressFunctor_t m_progressFunctor;

    /// remove a piece from the board. The user is supposed to check if the piece was there
    /// before calling this function since it just will set to empty the squares
	/// @param the board
    /// @param the piece
    /// @param the ABSOLUTE coord
    /// @param the player that owns the piece to be removed
	/// @param the opponent of the player that owns the piece
    static void RemovePiece(
            Board            &a_theBoard,
            const Piece      &a_piece,
            const Coordinate &a_coord,
            Player           &a_playerMe,
            Player           &a_playerOpponent);

    /// put down a piece on the board. The user is supposed to check if there is space for it before calling
    /// @param the board
    /// @param the piece
    /// @param the ABSOLUTE coord
    /// @param the player that owns the piece to be put down
    /// @param the opponent of the player that owns the piece
    static void PutDownPiece(
            Board            &a_theBoard,
            const Piece      &a_piece,
            const Coordinate &a_coord,
            Player           &a_playerMe,
            Player           &a_playerOpponent);

    /// @brief calculate the first piece to be put in the board
    static int32_t ComputeFirstPiece(
            const Board      &a_board,
            const Player     &a_playerMe,
            const Player     &a_playerOpponent,
            const Coordinate &a_lastOpponentPieceCoord,
            const Piece      &a_lastOpponentPiece,
            Piece            &out_resultPiece,
            Coordinate       &out_coord);

    /// @brief calculates the next piece to be put down by player 'me' using the heuristic passed as parameter
    /// It's an implementation of the minimax algorithm using the alpha beta pruning
    /// The parameters alpha and beta must be set to -INFINITE and INFINITE respectively (in the 1st call)
    /// or the algorithm won't work correctly (those are the default values for the call)
    /// It'll call iself recursively until depth is lower or equal 0
    ///
    /// stopProcessingFlag is a reference to a variable that could be set
    /// to non-zero by a different thread indicating to the thread currently stuck
    /// in the calculation algorithm to stop. If the current thread notices
    /// this flag variable is set to non-zero it will stop immediately, which means
    /// every reference used as an output or returned value will have unexpected
    /// undescribed values.
    static int32_t MinMaxAlphaBetaCompute(
            Board                        &a_board,
            Player                       &a_playerMe,
            CoordSetGame1v1_t*           a_oldNkPointsMe[e_numberOfPieces],
            Piece*                       a_lastPiecesMe[e_numberOfPieces],
            Player                       &a_playerOpponent,
            CoordSetGame1v1_t*           a_oldNkPointsOpponent[e_numberOfPieces],
            Piece*                       a_lastPiecesOpponent[e_numberOfPieces],
            Heuristic::calculateMethod_t a_heuristicMethod,
            int32_t                      originalDepth,
            int32_t                      depth,
            int32_t                      alpha,  //  = -INFINITE (in the 1st call)
            int32_t                      beta,   //  = INFINITE  (in the 1st call)
            const volatile sig_atomic_t  &stopProcessingFlag
#ifdef DEBUG_PRINT
            ,int32_t                      &times
#endif
    );

    /// @returns true if the player 'a_player' can put down at least 1 piece in a_board
    static bool CanPlayerGo(
            const Board  &a_board,
            const Player &a_player);
};

#endif /* GAME1V1_H_ */
