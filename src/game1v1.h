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
/// @file  game1v1.h
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
#include <csignal>         // atomic_t
#include <sigc++/signal.h> // sigc++-2.0/sigc++
#include "piece.h"
#include "player.h"
#include "board.h"
#include "heuristic.h"
#include "rules.h"
#include "coordinate.h"
#include "coordinate_set.h"

/// our own version of INFINITE
const int32_t INFINITE = 2147483647;

/// standard number of rows for a 1vs1 game
const int32_t BOARD_1VS1_ROWS    = 14;
/// standard number of columns for a 1vs1 game
const int32_t BOARD_1VS1_COLUMNS = 14;

/// initial coordinate for player 2
static const int32_t DEFAULT_STARTING_ROW_PLAYER2 = 9;
static const int32_t DEFAULT_STARTING_COL_PLAYER2 = 9;

/// initial coordinate for player 1
static const int32_t DEFAULT_STARTING_ROW_PLAYER1 = 4;
static const int32_t DEFAULT_STARTING_COL_PLAYER1 = 4;


/// @brief contains the definition of a game 1vs1 of blockem
/// This class is meant to contain all the rules about blockem 1vs1
class Game1v1
{
public:

    /// This enum represents a player of a 1v1 game
    typedef enum
    {
        e_Game1v1Player1 = 0,
        e_Game1v1Player2,

        e_Game1v1NoPlayer,

    } eGame1v1Player_t;

    /// @brief the customised set that is used in Game1v1. It is a typedef so it can
    ///        be swapped easily if a new faster method is found
    typedef CoordinateSet16x16 Game1v1CoordinateSet_t; // 1-2% faster than the one below
    //typedef CoordinateSet<BOARD_1VS1_ROWS, BOARD_1VS1_COLUMNS> Game1v1CoordinateSet_t; // 1-2% faster than the one below
    //typedef STLCoordinateSet_t Game1v1CoordinateSet_t;

	/// @brief builds the game
	/// It creates a board where the game will be played
    /// @param player1 starting coordinate
    /// @param player2 starting coordinate
    Game1v1(
        const Coordinate &a_player1StartingCoord = Coordinate(
            DEFAULT_STARTING_ROW_PLAYER1,
            DEFAULT_STARTING_COL_PLAYER1),
        const Coordinate &a_player2StartingCoord = Coordinate(
            DEFAULT_STARTING_ROW_PLAYER2,
            DEFAULT_STARTING_COL_PLAYER2));
	virtual ~Game1v1();

	/// @returns the board of the game
	const Board& GetBoard() const;

    /// access to the signal that will issued each time there's a progress
    /// update in the minimax algorithm
    /// float will be a value between 0.0 and 1.0
    /// instantiating an object that will be notified by this signal is as easy
    /// as this:
    ///     SignalProgress().connect(sigc::mem_fun(
    ///         your_instance,
    ///         &YourClass::YourMethod));
    ///
    /// YourClass::YourMethod must be declared like this:
    ///     void YourClass::YourMethod(float)
    sigc::signal<void, float>& SignalProgressUpdate();

    /// @returns a const reference to the player passed as parameter
	/// If a_playerType is invalid Player returned is undefined
    const Player& GetPlayer(eGame1v1Player_t a_playerType) const;

    /// @returns a const reference to the opponent of the player passed as parameter
    /// If a_playerType is invalid Player returned is undefined
    const Player& GetOpponent(eGame1v1Player_t a_playerType) const;

    /// @return the type of the player passed as a parameter.
    /// if that player doesn't belong to the Game, player type returned will be undefined
    eGame1v1Player_t GetPlayerType(const Player &a_player) const;

	/// @brief sets the player colour
	void SetPlayerColour(
	        eGame1v1Player_t a_player,
	        uint8_t a_red,
	        uint8_t a_green,
	        uint8_t a_blue);

    /// put down a piece on the board. The user is supposed to check if there is space for it before calling
    /// @param the piece
    /// @param the ABSOLUTE coord
    /// @param the player that owns the piece to be put down
	void PutDownPiece(
			const Piece      &a_piece,
			const Coordinate &a_coord,
			eGame1v1Player_t a_player);

    /// remove a piece from the board. The user is supposed to check if the piece was there
    /// before calling this function since it will just set to empty the squares
    /// @param the piece
    /// @param the ABSOLUTE coord
    /// @param the player that owns the piece to be removed
	void RemovePiece(
			const Piece      &a_piece,
			const Coordinate &a_coord,
			eGame1v1Player_t a_player);

    /// @brief calculates the next piece to be put down by player 'me' using the heuristic passed as parameter
    /// It uses the minimax algorithm with the alpha beta pruning
	/// @param the heuristic method for the min max algorithm
	/// @param maximum depth for the search tree
    /// @param Pplayer whose move will be calculated by MinMAx algorithm
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
	/// @returns value calculated by the evaluation function for the selected configuration
    int32_t MinMax(
            Heuristic::EvalFunction_t    a_heuristicMethod,
            int32_t                      depth,
            eGame1v1Player_t             a_player,
            Piece                        &out_resultPiece,
            Coordinate                   &out_coord,
            const volatile sig_atomic_t  &stopProcessingFlag,
            const Coordinate             &a_lastOpponentPieceCoord = Coordinate(),
            const Piece                  &a_lastOpponentPiece = Piece(e_noPiece));

    /// @brief Blanks out the board and reset the players so they have all the pieces available
    /// it can be used to restart the game (to start in a new blank game)
    /// @param player1 starting coordinate for the new game
    /// @param player2 starting coordinate for the new game
    void Reset(
        const Coordinate &a_player1StartingCoord = Coordinate(
            DEFAULT_STARTING_ROW_PLAYER1,
            DEFAULT_STARTING_COL_PLAYER1),
        const Coordinate &a_player2StartingCoord = Coordinate(
            DEFAULT_STARTING_ROW_PLAYER2,
            DEFAULT_STARTING_COL_PLAYER2));

    /// @brief load a game from an in_stream
    /// the in_stream has to contain the output of a game as it is done by Game1v1::SaveGame
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

protected:
    /// The blockem board where the game will be played
	Board m_board;

	/// one of the players that take part in the game. It's called 1, but it's not more (or less) important
	Player m_player1;

	/// one of the players that take part in the game. It's called 2, but it's not more (or less) important
	Player m_player2;

    /// signal that will issued each time there's a progress update in the
    /// minimax algorithm
    /// float will be a value between 0.0 and 1.0
    sigc::signal<void, float> m_signal_progress;

    /// signal to be sent when the computing is finished
    sigc::signal<void, const Piece&, const Coordinate&, Game1v1::eGame1v1Player_t, int32_t> m_signal_computingFinished;

    /// @brief recalculate the nucleation points using the whole blockem board and save them into the players
    void RecalculateNKInAllBoard();

    /// @brief recalculate the influence area (for both player1 and 2) in the wholse board
    void RecalculateInflueceAreasInAllBoard();

    /// remove a piece from the board. The user is supposed to check if the piece was there
    /// before calling this function since it just will set to empty the squares
	/// @param the board
    /// @param the ABSOLUTE coord
    /// @param the piece configuration
    /// @param the player that owns the piece to be removed
	/// @param the opponent of the player that owns the piece
    static void RemovePiece(
            Board                    &a_theBoard,
            const Coordinate         &a_coord,
            const PieceConfiguration &a_pieceConf,
            Player                   &a_playerMe,
            Player                   &a_playerOpponent);

    /// put down a piece on the board. The user is supposed to check if there is space for it before calling
    /// @param the board
    /// @param the ABSOLUTE coord
    /// @param the piece configuration
    /// @param the player that owns the piece to be removed
    /// @param the opponent of the player that owns the piece
    static void PutDownPiece(
            Board                    &a_theBoard,
            const Coordinate         &a_coord,
            const PieceConfiguration &a_pieceConf,
            Player                   &a_playerMe,
            Player                   &a_playerOpponent);

    /// @brief calculate the first piece to be put in the board
    /// a_board, a_playerMe and a_playerOpponent are not const references, but they
    /// are not modified after this method is run
    static int32_t ComputeFirstPiece(
            Board            &a_board,
            Player           &a_playerMe,
            Player           &a_playerOpponent,
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
            Board                       &a_board,
            Player                      &a_playerMe,
            Game1v1CoordinateSet_t*      a_oldNkPointsMe[e_numberOfPieces],
            ePieceType_t                 a_lastPiecesMe[e_numberOfPieces],
            Player                      &a_playerOpponent,
            Game1v1CoordinateSet_t*      a_oldNkPointsOpponent[e_numberOfPieces],
            ePieceType_t                 a_lastPiecesOpponent[e_numberOfPieces],
            Heuristic::EvalFunction_t    a_heuristicMethod,
            int32_t                      originalDepth,
            int32_t                      depth,
            int32_t                      alpha,  //  = -INFINITE (in the 1st call)
            int32_t                      beta,   //  = INFINITE  (in the 1st call)
            const volatile sig_atomic_t &stopProcessingFlag
#ifdef DEBUG_PRINT
            ,int32_t                      &times
#endif
    );

    /// Get all the nucleation points in the board. Save the results in a special kind of set
    /// without memory allocation on the heap.
    /// WARNING: This method won't work if there are more than 16 rows or columns in the board
    /// @param set to save the nucleation points
    /// @return the number of nucleation points saved into the output set
    inline static int32_t GetAllNucleationPoints(
            const Player       &a_player,
            CoordinateSet16x16 &a_set);

#if 0
    /// Get all the nucleation points in the board. Save the results in a special kind of set
    /// without memory allocation on the heap
    /// this method is slower (around 2%) than the method above
    /// @param set to save the nucleation points
    /// @return the number of nucleation points saved into the output set
    template<int32_t ROWS, int32_t COLUMNS>
    inline static int32_t GetAllNucleationPoints(
            const Player                 &a_player,
            CoordinateSet<ROWS, COLUMNS> &a_set);
#endif
};

// include implementation details of inline functions
#include "impl/game1v1_impl.h"

#endif /* GAME1V1_H_ */
