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
/// @file  Game1v1.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 28-Aug-2009  Original development
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif

#include "Game1v1.h"

/// name for the player me. The one that MinMax tries to maximise (and win)
static const char PLAYER_ME_NAME[] = "Computer";
/// name fot the opponent (the human player. Me is the computer's)
static const char PLAYER_OPPONENT_NAME[] = "TheOpponent";

/// it will be used as an empty space in the board
static const char CHAR_EMPTY    = ' ';
/// it will be used as the character for 'me' in the board
static const char CHAR_ME       = 'X';
/// it will be used as the character for 'opponent' in the board
static const char CHAR_OPPONENT = 'O';

/// initial X coordinate for player opponent (user)
static const int32_t STARTING_COORD_X_OPPONENT = 4;
/// initial Y coordinate for player opponent (user)
static const int32_t STARTING_COORD_Y_OPPONENT = 4;

/// initial X coordinate for player Me (computer)
static const int32_t STARTING_COORD_X_ME = 9;
/// initial Y coordinate for player Me (computer)
static const int32_t STARTING_COORD_Y_ME = 9;

/// To be used in the MinMax Algorithm as the size of the valid coords arrays
static const int8_t VALID_COORDS_SIZE = 5;

/// Assume makes an ass out of u and me, but sometimes assumptions must be made
/// This constant represents how many 5-square pieces will be put in-a-row before even
/// considering to put a 4,3,2 or 1 square piece on the board
/// the goal is improvimg the speed of the MinMax algorithm
static const int8_t MIN_5SQUARE_PIECES_AT_START = 5;


Game1v1::Game1v1() :
	m_board(BOARD_1VS1_ROWS, BOARD_1VS1_COLUMNS, CHAR_EMPTY),
    m_playerMe(std::string(PLAYER_ME_NAME), CHAR_ME, BOARD_1VS1_ROWS, BOARD_1VS1_COLUMNS, Coordinate(STARTING_COORD_X_ME, STARTING_COORD_Y_ME)),
    m_playerOpponent(std::string(PLAYER_OPPONENT_NAME), CHAR_OPPONENT, BOARD_1VS1_ROWS, BOARD_1VS1_COLUMNS, Coordinate(STARTING_COORD_X_OPPONENT, STARTING_COORD_Y_OPPONENT)),
    m_progressFunctor(NULL)
{
	// initialise the players' nk according to the current state of the blokus board
    Rules::RecalculateNKInAllBoard(m_board, m_playerMe, m_playerOpponent);
}

Game1v1::~Game1v1()
{
}

void Game1v1::RemovePiece(
        Board            &a_theBoard,
        const Piece      &a_piece,
        const Coordinate &a_coord,
        Player           &a_playerMe,
        Player           &a_playerOpponent)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_row < a_theBoard.GetNRows());

    assert(a_coord.m_col >= 0);
    assert(a_coord.m_col < a_theBoard.GetNColumns());
#endif

    for (uint8_t i = 0 ; i < a_piece.GetNSquares() ; i++)
    {
#ifdef DEBUG
        assert( ((a_coord.m_row + a_piece.m_coords[i].m_row) >= 0) &&
        		((a_coord.m_row + a_piece.m_coords[i].m_row) < a_theBoard.GetNRows()) );
        assert( ((a_coord.m_col + a_piece.m_coords[i].m_col) >= 0) &&
        		((a_coord.m_col + a_piece.m_coords[i].m_col) < a_theBoard.GetNColumns()) );

        assert(a_theBoard.IsPlayerInCoord(
        		a_coord.m_row + a_piece.m_coords[i].m_row,
        		a_coord.m_col + a_piece.m_coords[i].m_col,
        		a_playerMe));
#endif

        a_theBoard.BlankCoord(
        		a_coord.m_row + a_piece.m_coords[i].m_row,
        		a_coord.m_col + a_piece.m_coords[i].m_col);

        // (a_coordX + pieceX, a_coordY + pieceY) is now empty
        // is it now a nucleation point for the opponent? (it couldn't be before, as it was occupied by 'me')
        if (Rules::IsNucleationPointCompute(
                a_theBoard,
        		a_playerOpponent,
        		a_coord.m_row + a_piece.m_coords[i].m_row,
        		a_coord.m_col + a_piece.m_coords[i].m_col))
        {
        	a_playerOpponent.SetNucleationPoint(
        			a_coord.m_row + a_piece.m_coords[i].m_row,
        			a_coord.m_col + a_piece.m_coords[i].m_col);
        }
    }

    // recalculate all the nk points around the piece we just removed (player 'me')
    Rules::RecalculateNKAroundPiece(a_theBoard, a_piece, a_coord, a_playerMe);
}

void Game1v1::PutDownPiece(
        Board            &a_theBoard,
        const Piece      &a_piece,
        const Coordinate &a_coord,
        Player           &a_playerMe,
        Player           &a_playerOpponent)
{
#ifdef DEBUG
    assert(a_coord.m_row >= 0);
    assert(a_coord.m_row < a_theBoard.GetNRows());

    assert(a_coord.m_col >= 0);
    assert(a_coord.m_col < a_theBoard.GetNColumns());
#endif

    for (int i = 0 ; i < a_piece.GetNSquares() ; i++)
    {
#ifdef DEBUG

        assert( ((a_coord.m_row + a_piece.m_coords[i].m_row) >= 0) &&
        		((a_coord.m_row + a_piece.m_coords[i].m_row) < a_theBoard.GetNRows()) );
        assert( ((a_coord.m_col + a_piece.m_coords[i].m_col) >= 0) &&
        		((a_coord.m_col + a_piece.m_coords[i].m_col) < a_theBoard.GetNColumns()) );

        assert(a_theBoard.IsCoordEmpty(
        		a_coord.m_row + a_piece.m_coords[i].m_row,
        		a_coord.m_col + a_piece.m_coords[i].m_col));
#endif

        a_theBoard.SetPlayerInCoord(
        		a_coord.m_row + a_piece.m_coords[i].m_row,
        		a_coord.m_col + a_piece.m_coords[i].m_col,
        		a_playerMe);

        // this new point is being occupied by the player 'me'. It can't be a nucleation point
        // for the opponent any more
        a_playerOpponent.UnsetNucleationPoint(
        		a_coord.m_row + a_piece.m_coords[i].m_row,
        		a_coord.m_col + a_piece.m_coords[i].m_col);
    }

    // recalculate all the nk points around the piece we just put down (player 'me')
    Rules::RecalculateNKAroundPiece(a_theBoard, a_piece, a_coord, a_playerMe);
}

bool Game1v1::CanPlayerMeGo() const
{
    return Game1v1::CanPlayerGo(m_board, m_playerMe);
}

bool Game1v1::CanPlayerOpponentGo() const
{
    return Game1v1::CanPlayerGo(m_board, m_playerOpponent);
}

bool Game1v1::CanPlayerGo(
        const Board  &a_board,
        const Player &a_player)
{
    if (a_player.NumberOfPiecesAvailable() == 0)
    {
        return false;
    }

    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        if (a_player.IsPieceAvailable(static_cast<ePieceType_t>(i)) == false)
        {
            continue;
        }

        // copy the piece so the original is not modified
        Piece thisPiece(a_player.m_pieces[i].GetType());

        do
        {
            int8_t nRotations = 0;
            while(nRotations < thisPiece.GetNRotations())
            {
                bool nkExists;
                Coordinate thisNkPoint;
                Player::SpiralIterator nkIterator;

                nkExists = a_player.GetFirstNucleationPointSpiral(nkIterator, thisNkPoint);
                while(nkExists)
                {
                    // retrieve the valid coords of this piece in the current nk point
                    Coordinate validCoords[VALID_COORDS_SIZE];
                    int32_t nValidCoords = Rules::CalculateValidCoordsInNucleationPoint(
                                                a_board,
                                                thisPiece,
                                                thisNkPoint,
                                                a_player,
                                                validCoords,
                                                VALID_COORDS_SIZE);

                    if (nValidCoords > 0)
                    {
                        return true;
                    }

                    nkExists = a_player.GetNextNucleationPointSpiral(nkIterator, thisNkPoint);

                } // while(nkExists)

                nRotations++;
                thisPiece.RotateRight();
            }

            // reset the amount of rotations done before mirroring
            nRotations = 0;

            if ( (thisPiece.GetType() == e_4Piece_LittleS) &&
                 (thisPiece.IsMirrored() == false) )
            {
                // For this piece the maximum number or rotations is 2
                // and the piece is not symmetric, the configuration after
                // the 3rd rotation is the shame shape as the original, but
                // the coords moved. Reset the piece before mirroring to
                // avoid unexpected results
                //
                // it also happens with 2piece and 4longPiece, but those pieces
                // don't have mirror, so there's no need for this extra check
                thisPiece.Reset();
            }

        } while (thisPiece.MirrorYAxis());

    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)

    return false;
}

int32_t Game1v1::MinMax(
        Heuristic::calculateMethod_t a_heuristicMethod,
        int32_t                      depth,
        Piece                        &out_resultPiece,
        Coordinate                   &out_coord,
        const volatile sig_atomic_t  &stopProcessingFlag,
        const Coordinate             &a_lastOpponentPieceCoord,
        const Piece                  &a_lastOpponentPiece)
{
	// putting down the 1st piece is a special case
	if (m_playerMe.NumberOfPiecesAvailable() == e_numberOfPieces)
	{
		return ComputeFirstPiece(
				m_board,
				m_playerMe,
				m_playerOpponent,
				a_lastOpponentPieceCoord,
				a_lastOpponentPiece,
				out_resultPiece,
				out_coord);
	}
#ifdef DEBUG_PRINT
    int32_t timesCalled = 1;
#endif
    int32_t alpha = -INFINITE;
    int32_t beta  = INFINITE;

    // best piece + coord in this particular call to the function
    Piece bestPiece(e_noPiece);
    Coordinate bestCoord;

    // this set will be used not to place the same piece in the same place more than once
    // it will be reset every time the piece is rotated/mirrored or the piece is changed
    CoordSetGame1v1_t testedCoords;

    // this set will save the current nucleation points. It will be used in future calls not to
    // check configurations more than once
    CoordSetGame1v1_t nkPointSetMe;
    m_playerMe.GetAllNucleationPoints(nkPointSetMe);

    // declare the array of last pieces and old NK points for me and opponent
    // and clear them out
    Piece* lastPiecesMe[e_numberOfPieces];
    Piece* lastPiecesOpponent[e_numberOfPieces];
    CoordSetGame1v1_t* oldNkPointsMe[e_numberOfPieces];
    CoordSetGame1v1_t* oldNkPointsOpponent[e_numberOfPieces];

    for (int32_t i = e_minimumPieceIndex ; i < e_numberOfPieces ; i++)
    {
        lastPiecesMe[i]  = NULL;
        oldNkPointsMe[i] = NULL;
        lastPiecesOpponent[i]  = NULL;
        oldNkPointsOpponent[i] = NULL;
    }

    // save a pointer to this set in the place (index) reserved for it
    // in this case is 0 (the current depth starting from lower to higher)
    oldNkPointsMe[0] = &nkPointSetMe;

    // lastPiecesOpponent[0] needs to be set to an empty piece for the algorithm
    // to work even if the depth is set to an even number
    // it will affect the performance because adding an extra if check per
    // calling to the MinMaxAlphaBetaCompute function, which is completely
    // insignificant compared to the heuristic function weight
    // in the case of calling the function with an odd depth, it won't affect
    // performance at all
    Piece noPiece(e_noPiece);
    lastPiecesOpponent[0] = &noPiece;

    // no pieces available. Return the current value of the heuristic and 'bestPiece' (which
    // at this time doesn't represent any piece yet
    if (m_playerMe.NumberOfPiecesAvailable() == 0)
    {
        // bestPiece doesn't represent any piece yet
        out_resultPiece = bestPiece;

        // calculate the heuristic of the board to return it, even though the function calculated fuck-all
        return a_heuristicMethod(m_board, m_playerMe, m_playerOpponent);
    }

#ifdef DEBUG_PRINT
    std::cout << "NK: " << m_playerMe.NumberOfNucleationPoints() << std::endl;
#endif


    // number of pieces successfully put down
    // if no pieces could be put down the recursive function wasn't called even once,
    // which means the search tree was broken.
    int32_t nBranchesSearchTree = 0;

    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    {
        if (m_progressFunctor)
        {
            m_progressFunctor(static_cast<float>(e_numberOfPieces - i) / e_numberOfPieces);
        }
#ifdef DEBUG_PRINT
        std::cout <<".";
        std::cout.flush(); // fflush(stdout);
        if (i == 0)
        {
            std::cout << std::endl;
        }
#endif

        if ( (m_playerMe.m_pieces[i].GetNSquares() < 5) &&
             (m_playerMe.NumberOfPiecesAvailable() > (e_numberOfPieces - MIN_5SQUARE_PIECES_AT_START)) &&
             (nBranchesSearchTree > 0))
        {
            // if at least one 5-square piece has been put down before this non 5-square piece
            // do not try to put down the piece
            continue;
        }

        if (m_playerMe.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            m_playerMe.UnsetPiece(static_cast<ePieceType_t>(i));
            do
            {
                int8_t nRotations = 0;
                while(nRotations < m_playerMe.m_pieces[i].GetNRotations())
                {
                    bool nkExists;
                    Coordinate thisNkPoint;
                    Player::SpiralIterator nkIterator;

                    nkExists = m_playerMe.GetFirstNucleationPointSpiral(nkIterator, thisNkPoint);
                    while(nkExists)
                    {
                        // retrieve the valid coords of this piece in the current nk point
                        Coordinate validCoords[VALID_COORDS_SIZE];
                        int32_t nValidCoords = Rules::CalculateValidCoordsInNucleationPoint(
                                                    m_board,
                                                    m_playerMe.m_pieces[i],
                                                    thisNkPoint,
                                                    m_playerMe,
                                                    validCoords,
                                                    VALID_COORDS_SIZE);

                        for (uint8_t k = 0 ; k < nValidCoords ; k++)
                        {
                            if (!testedCoords.isPresent(validCoords[k]))
                            {
                                testedCoords.insert(validCoords[k]);

                                Game1v1::PutDownPiece(
                                        m_board,
                                        m_playerMe.m_pieces[i],
                                        validCoords[k],
                                        m_playerMe,
                                        m_playerOpponent);

                                // save a pointer to this piece in the place (index) reserved for it
                                lastPiecesMe[0] = &m_playerMe.m_pieces[i];

                                // number of branches searched at this level of the tree
                                nBranchesSearchTree++;

                                int32_t maxValue = -Game1v1::MinMaxAlphaBetaCompute(
                                                        m_board,
                                                        m_playerOpponent,
                                                        oldNkPointsOpponent,
                                                        lastPiecesOpponent,
                                                        m_playerMe,
                                                        oldNkPointsMe,
                                                        lastPiecesMe,
                                                        a_heuristicMethod,
                                                        depth,
                                                        depth - 1,
                                                        -beta,
                                                        -alpha,
                                                        stopProcessingFlag
#ifdef DEBUG_PRINT
                                                        ,timesCalled
#endif
                                                        );

                                if (stopProcessingFlag)
                                {
                                    // something happened and we were told to stop
                                    // processing (that is probably why MinMaxAlphaBetaCompute
                                    // returned. This function just returns as it is described on
                                    // the description of the function:
                                    // " (...) output or returned value will have unexpected
                                    //   undescribed values "
                                    return 0;
                                }

                                if (maxValue > alpha)
                                {
                                    bestPiece = m_playerMe.m_pieces[i];
                                    bestCoord.m_row = validCoords[k].m_row;
                                    bestCoord.m_col = validCoords[k].m_col;

                                    alpha = maxValue;
                                }

                                Game1v1::RemovePiece(
                                        m_board,
                                        m_playerMe.m_pieces[i],
                                        validCoords[k],
                                        m_playerMe,
                                        m_playerOpponent);


#ifdef DEBUG
                                assert(beta > alpha);
#endif
                            } // if (it == testedCoords.end())
                        } // for (uint8_t k = 0 ; k < nValidCoords ; k++)

                        nkExists = m_playerMe.GetNextNucleationPointSpiral(nkIterator, thisNkPoint);

                    } // while(nkExists)

                    testedCoords.clear();

                    nRotations++;
                    m_playerMe.m_pieces[i].RotateRight();

                }// while (nOrigRotations > m_playerMe.m_pieces[i].GetNRotationsRight())

                // reset the amount of rotations done before mirroring
                nRotations = 0;

                if ( (m_playerMe.m_pieces[i].GetType() == e_4Piece_LittleS) &&
                     (m_playerMe.m_pieces[i].IsMirrored() == false) )
                {
                    // For this piece the maximum number or rotations is 2
                    // and the piece is not symmetric, the configuration after
                    // the 3rd rotation is the shame shape as the original, but
                    // the coords moved. Reset the piece before mirroring to
                    // avoid unexpected results
                    //
                    // it also happens with 2piece and 4longPiece, but those pieces
                    // don't have mirror, so there's no need for this extra check
                    m_playerMe.m_pieces[i].Reset();
                }

            } while (m_playerMe.m_pieces[i].MirrorYAxis());

            m_playerMe.SetPiece(static_cast<ePieceType_t>(i));
            m_playerMe.m_pieces[i].Reset();

        } // if (me->IsPieceAvailable(i))
    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)

    // only the highest node (root node) of the tree saves the result of the algorithm
    out_resultPiece = bestPiece;
    out_coord.m_row   = bestCoord.m_row;
    out_coord.m_col   = bestCoord.m_col;

#ifdef DEBUG_PRINT
    std::cout << "Times called " << timesCalled << std::endl;
#endif

    return alpha;
}

int32_t Game1v1::ComputeFirstPiece(
        const Board      &a_board,
        const Player     &a_playerMe,
        const Player     &a_playerOpponent,
        const Coordinate &a_lastOpponentPieceCoord,
        const Piece      &a_lastOpponentPiece,
        Piece            &out_resultPiece,
        Coordinate       &out_coord)
{
    if ( (a_playerMe.NumberOfPiecesAvailable() == e_numberOfPieces) &&
         (a_lastOpponentPiece.GetType() != e_noPiece) &&
         (a_lastOpponentPiece.GetNSquares() == 5) &&
         (a_playerOpponent.NumberOfNucleationPoints() > 5) )
    {
        // this is the 1st move. The opponent put down a 5-square piece
        // creating more than 5 nk points. We'll be cheeky and copy the move
        out_resultPiece = a_lastOpponentPiece;
        out_resultPiece.RotateRight();
        out_resultPiece.RotateRight();

        out_coord.m_row = a_board.GetNRows()    - a_lastOpponentPieceCoord.m_row - 1;
        out_coord.m_col = a_board.GetNColumns() - a_lastOpponentPieceCoord.m_col - 1;
    }
    else
    {
        // we shouldn't copy the 1st user's move, because it was shit
        // temporary solution. Put the cross in 9,8 (acceptable first move)
        out_resultPiece = Piece(e_5Piece_Cross);
        out_coord = Coordinate(9, 8);
    }

	return 0;
}

int32_t Game1v1::MinMaxAlphaBetaCompute(
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
        int32_t                      alpha,
        int32_t                      beta,
        const volatile sig_atomic_t  &stopProcessingFlag
#ifdef DEBUG_PRINT
        ,int32_t                      &times
#endif
        )
{
#ifdef DEBUG_PRINT
    times++;
#endif

    if ( (depth <= 0) || (a_playerMe.NumberOfPiecesAvailable() == 0) )
    {
        // If the heuristic is to be calculated, it should be done using the opponent as 'me'
        // (and me as the 'opponent') for the call to Calculate
        // Why is this? This is because when this call was made 'me' and 'opponent' were
        // interchanged for the next iteration in the MiniMax algorithm, but we want
        // to calculate the heuristic based on the latest player to put a piece, that is
        // the 'opponent' in this call, 'me' in the previous node of the minimax tree
        return -a_heuristicMethod(a_board, a_playerOpponent, a_playerMe);
    }
    // this set will be used not to place the same piece in the same place more than once
    // it will be reset every time the piece is rotated/mirrored or the piece is changed
    CoordSetGame1v1_t testedCoords;

    // this set will save the current nucleation points. It will be used in future calls not to
    // check configurations more than once
    CoordSetGame1v1_t nkPointSetMe;
    a_playerMe.GetAllNucleationPoints(nkPointSetMe);

    // save a pointer to this set in the place (index) reserved for it
    // (originalDepth / 2) - (depth / 2) represents this level of relative depth
    // to the pieces put by 'me'. originalDepth should be odd for this
    // calculation to work properly
    a_oldNkPointsMe[(originalDepth / 2) - (depth / 2)] = &nkPointSetMe;

    // number of pieces successfully put down
    // if no pieces could be put down the recursive function wasn't called even once,
    // which means the search tree was broken.
    int32_t nBranchesSearchTree = 0;

	for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
	{
        if ( (a_playerMe.m_pieces[i].GetNSquares() < 5) &&
             (a_playerMe.NumberOfPiecesAvailable() > (e_numberOfPieces - MIN_5SQUARE_PIECES_AT_START)) &&
             (nBranchesSearchTree > 0))
        {
            // if at least one 5-square piece has been put down before this non 5-square piece
            // do not try to put down the piece
            continue;
        }

		if (a_playerMe.IsPieceAvailable(static_cast<ePieceType_t>(i)))
		{
			a_playerMe.UnsetPiece(static_cast<ePieceType_t>(i));
			do
			{
                int8_t nRotations = 0;
                while(nRotations < a_playerMe.m_pieces[i].GetNRotations())
				{
					bool nkExists;
					Coordinate thisNkPoint;
					Player::SpiralIterator nkIterator;

					nkExists = a_playerMe.GetFirstNucleationPointSpiral(nkIterator, thisNkPoint);
				    while(nkExists)
				    {
                        // before putting down the piece test if the future configuration
                        // could be tested by some other level in the backtrack tree
				        // using originalDepth/2 and depth/2 because there are 2 arrays, one
				        // for pieces set by 'me' and one for pieces set by the opponent
				        // -1 is used because (originalDepth / 2) - (depth / 2) represents
				        // the current level
                        int32_t thisLevel;
                        for (thisLevel = (originalDepth / 2) - (depth / 2) - 1;
                             thisLevel >= 0;
                             thisLevel--)
                        {
                            // if the current piece's index is bigger than the old piece deployed on
                            // the board, put down the piece only if this nucleation doesn't belong to the
                            // old nucleation point's set
                            // both old piece and old nk points set correspond to this particular
                            // level of the backtrack tree
                            if ( (a_lastPiecesMe[thisLevel] != NULL) &&
                                 (i >= a_lastPiecesMe[thisLevel]->GetType()) )
                            {
                                if (a_oldNkPointsMe[thisLevel]->isPresent(thisNkPoint))
                                {
                                    nkExists = a_playerMe.GetNextNucleationPointSpiral(nkIterator, thisNkPoint);
                                    break;
                                }
                            }
                        }

                        if (thisLevel >= 0)
                        {
                            // the previous loop called break before the end
                            continue;
                        }

                        // retrieve the valid coords of this piece in the current nk point
                        Coordinate validCoords[VALID_COORDS_SIZE];
                        int32_t nValidCoords = Rules::CalculateValidCoordsInNucleationPoint(
                                                    a_board,
                                                    a_playerMe.m_pieces[i],
                                                    thisNkPoint,
                                                    a_playerMe,
                                                    validCoords,
                                                    VALID_COORDS_SIZE);

                        for (uint8_t k = 0 ; k < nValidCoords ; k++)
                        {
                            if (!testedCoords.isPresent(validCoords[k]))
                            {
                                testedCoords.insert(validCoords[k]);

                                Game1v1::PutDownPiece(
                                        a_board,
                                        a_playerMe.m_pieces[i],
                                        validCoords[k],
                                        a_playerMe,
                                        a_playerOpponent);

                                // save a pointer to this piece in the place (index) reserved for it
                                a_lastPiecesMe[(originalDepth / 2) - (depth / 2)] = &(a_playerMe.m_pieces[i]);

                                // number of branches searched at this level of the tree
                                nBranchesSearchTree++;

                                int32_t maxValue = -Game1v1::MinMaxAlphaBetaCompute(
                                                        a_board,
                                                        a_playerOpponent,
                                                        a_oldNkPointsOpponent,
                                                        a_lastPiecesOpponent,
                                                        a_playerMe,
                                                        a_oldNkPointsMe,
                                                        a_lastPiecesMe,
                                                        a_heuristicMethod,
                                                        originalDepth,
                                                        depth - 1,
                                                        -beta,
                                                        -alpha,
                                                        stopProcessingFlag
#ifdef DEBUG_PRINT
                                                        ,times
#endif
                                                        );

                                if (stopProcessingFlag)
                                {
                                    // something happened and we were told to stop
                                    // processing (that is probably why MinMaxAlphaBetaCompute
                                    // returned. This function just returns as it is described on
                                    // the description of the function:
                                    // " (...) output or returned value will have unexpected
                                    //   undescribed values "
                                    return 0;
                                }

                                if (maxValue > alpha)
                                {
                                    alpha = maxValue;
                                }

                                Game1v1::RemovePiece(
                                        a_board,
                                        a_playerMe.m_pieces[i],
                                        validCoords[k],
                                        a_playerMe,
                                        a_playerOpponent);

                                if (beta <= alpha)
                                {
                                    // this branch can be safely be pruned
                                    // set the piece again to available and reset it
                                    a_playerMe.SetPiece(static_cast<ePieceType_t>(i));
                                    a_playerMe.m_pieces[i].Reset();

                                    return alpha;
                                }
                            } // if (it == testedCoords.end())
                        } // for (uint8_t k = 0 ; k < nValidCoords ; k++)

                        nkExists = a_playerMe.GetNextNucleationPointSpiral(nkIterator, thisNkPoint);
				    } // while(nkExists)

					testedCoords.clear();
					nRotations++;
					a_playerMe.m_pieces[i].RotateRight();

				} // while(nOrigRotations > a_playerMe.m_pieces[i].GetNRotationsRight())

	            // reset the amount of rotations done before mirroring
                nRotations = 0;

	            if ( (a_playerMe.m_pieces[i].GetType() == e_4Piece_LittleS) &&
	                 (a_playerMe.m_pieces[i].IsMirrored() == false) )
	            {
	                // For this piece the maximum number or rotations is 2
	                // and the piece is not symmetric, the configuration after
	                // the 3rd rotation is the shame shape as the original, but
	                // the coords moved. Reset the piece before mirroring to
	                // avoid unexpected results
	                //
	                // it also happens with 2piece and 4longPiece, but those pieces
	                // don't have mirror, so there's no need for this extra check
	                a_playerMe.m_pieces[i].Reset();
	            }

			} while (a_playerMe.m_pieces[i].MirrorYAxis());

			a_playerMe.SetPiece(static_cast<ePieceType_t>(i));
			a_playerMe.m_pieces[i].Reset();

		} // if (me->IsPieceAvailable(i))
	} // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)

	if (nBranchesSearchTree == 0)
	{
	    // no pieces were put down (the player can't put down any piece at this level
	    // of the search tree). Call the algorithm with no piece put down
	    a_lastPiecesMe[(originalDepth / 2) - (depth / 2)] = NULL;
        int32_t maxValue = -Game1v1::MinMaxAlphaBetaCompute(
                                a_board,
                                a_playerOpponent,
                                a_oldNkPointsOpponent,
                                a_lastPiecesOpponent,
                                a_playerMe,
                                a_oldNkPointsMe,
                                a_lastPiecesMe,
                                a_heuristicMethod,
                                originalDepth,
                                depth - 1,
                                -beta,
                                -alpha,
                                stopProcessingFlag
#ifdef DEBUG_PRINT
                                ,times
#endif
                                );

        if (stopProcessingFlag)
        {
            // something happened and we were told to stop
            // processing (that is probably why MinMaxAlphaBetaCompute
            // returned. This function just returns as it is described on
            // the description of the function:
            // " (...) output or returned value will have unexpected
            //   undescribed values "
            return 0;
        }

        if (maxValue > alpha)
        {
            alpha = maxValue;
        }
	}

    return alpha;
}

void Game1v1::Reset()
{
    m_board.Reset();
    m_playerMe.Reset();
    m_playerOpponent.Reset();
}

bool Game1v1::LoadGame(std::istream& a_inStream)
{
    std::string line;

    if(!a_inStream)
    {
        return false;
    }

    getline(a_inStream, line); // don't need it
    for (int i = 0; i < m_board.GetNRows() ; i++)
    {
        getline(a_inStream, line); // don't need this one
        getline(a_inStream, line); // the line is there

        int charInLine = 5;
        for (int j = 0; j < m_board.GetNColumns() ; j++)
        {
        	m_board.SetSquare(line.at(charInLine), i, j);
             charInLine += 4;
        }
    }
    getline(a_inStream, line); // don't need it

    getline(a_inStream, line); // this line contains me's pieces left
    for (int i = 0; i < e_numberOfPieces; i++)
    {
        if (line.at(i) == '0')
        {
        	m_playerMe.UnsetPiece(static_cast<ePieceType_t>(i));
        }
    }
    getline(a_inStream, line); // this line contains opponent's pieces left
    for (int i = 0; i < e_numberOfPieces; i++)
    {
        if (line.at(i) == '0')
        {
        	m_playerOpponent.UnsetPiece(static_cast<ePieceType_t>(i));
        }
    }

    Rules::RecalculateNKInAllBoard(m_board, m_playerMe, m_playerOpponent);

    return true;
}

bool Game1v1::SaveGame(std::ostream& a_outStream)
{
    m_board.PrintBoard(a_outStream);
    m_playerMe.PrintAvailablePieces(a_outStream);
    m_playerOpponent.PrintAvailablePieces(a_outStream);

    a_outStream << std::endl;

    return true;
}

