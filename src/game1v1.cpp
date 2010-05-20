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
/// @file  game1v1.cpp
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

#include "game1v1.h"

/// player1's name
static const char PLAYER_1_NAME[] = "Player1";
/// player2's name
static const char PLAYER_2_NAME[] = "Player2";

/// it will be used as an empty space in the board
static const char CHAR_EMPTY   = ' ';
/// it will be used as the character for 'me' in the board
static const char CHAR_PLAYER1 = 'X';
/// it will be used as the character for 'opponent' in the board
static const char CHAR_PLAYER2 = 'O';

/// Assume makes an ass out of u and me, but sometimes assumptions must be made
/// This constant represents how many 5-square pieces will be put in-a-row before even
/// considering to put a 4,3,2 or 1 square piece on the board
/// the goal is improvimg the speed of the MinMax algorithm
static const int8_t MIN_5SQUARE_PIECES_AT_START = 5;


Game1v1::Game1v1(
    const Coordinate &a_player1StartingCoord,
    const Coordinate &a_player2StartingCoord) :
	m_board(BOARD_1VS1_ROWS, BOARD_1VS1_COLUMNS, CHAR_EMPTY),
    m_player1(std::string(PLAYER_1_NAME), CHAR_PLAYER1, BOARD_1VS1_ROWS, BOARD_1VS1_COLUMNS, a_player1StartingCoord),
    m_player2(std::string(PLAYER_2_NAME), CHAR_PLAYER2, BOARD_1VS1_ROWS, BOARD_1VS1_COLUMNS, a_player2StartingCoord),
    m_progressFunctor(NULL)
{
	// initialise the players' nk according to the current state of the blokus board
    Rules::RecalculateNKInAllBoard(m_board, m_player1, m_player2);
}

Game1v1::~Game1v1()
{
}

void Game1v1::SetProgressHandler(ProgressFunctor_t a_progressFunctor)
{
    m_progressFunctor = a_progressFunctor;
}

const Player& Game1v1::GetPlayer(eGame1v1Player_t a_playerType) const
{
    switch (a_playerType)
    {
    case Game1v1::e_Game1v1Player1:
    {
        return m_player1;
    }
    case Game1v1::e_Game1v1Player2:
    {
        return m_player2;
    }
#ifdef DEBUG
    default:
    {
        assert(0);
    }
#endif
    } // switch (a_playerType)
}

const Player& Game1v1::GetOpponent(eGame1v1Player_t a_playerType) const
{
    switch (a_playerType)
    {
    case Game1v1::e_Game1v1Player1:
    {
        return m_player2;
    }
    case Game1v1::e_Game1v1Player2:
    {
        return m_player1;
    }
#ifdef DEBUG
    default:
    {
        assert(0);
    }
#endif
    } // switch (a_playerType)
}

Game1v1::eGame1v1Player_t Game1v1::GetPlayerType(const Player &a_player) const
{
    eGame1v1Player_t rValue = Game1v1::e_Game1v1Player1;

    if (a_player.PresentationChar() == m_player1.PresentationChar())
    {
        rValue = Game1v1::e_Game1v1Player1;
    }
    else if (a_player.PresentationChar() == m_player2.PresentationChar())
    {
        rValue = Game1v1::e_Game1v1Player2;
    }
#ifdef DEBUG
    else
    {
        assert(0);
    }
#endif

    return rValue;
}

void Game1v1::SetPlayerColour(
        eGame1v1Player_t a_player,
        uint8_t a_red,
        uint8_t a_green,
        uint8_t a_blue)
{
    switch (a_player)
    {
    case e_Game1v1Player1:
    {
        m_player1.SetColour(a_red, a_green, a_blue);
        break;
    }
    case e_Game1v1Player2:
    {
        m_player2.SetColour(a_red, a_green, a_blue);
        break;
    }
#ifdef DEBUG
    default:
        assert(0);
#endif
    } // switch (a_player)
}

void Game1v1::RemovePiece(
            const Piece      &a_piece,
            const Coordinate &a_coord,
            eGame1v1Player_t a_player)
{
    switch (a_player)
    {
    case e_Game1v1Player1:
    {
        Game1v1::RemovePiece(m_board, a_piece, a_coord, m_player1, m_player2);
        m_player1.SetPiece(a_piece.GetType());
        break;
    }
    case e_Game1v1Player2:
    {
        Game1v1::RemovePiece(m_board, a_piece, a_coord, m_player2, m_player1);
        m_player2.SetPiece(a_piece.GetType());
        break;
    }
#ifdef DEBUG
    default:
        assert(0);
#endif
    } // switch (a_player)
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
        Coordinate thisCoord(a_coord.m_row + a_piece.GetCoord(i).m_row,
                             a_coord.m_col + a_piece.GetCoord(i).m_col);
#ifdef DEBUG
        assert( ((a_coord.m_row + a_piece.GetCoord(i).m_row) >= 0) &&
        		((a_coord.m_row + a_piece.GetCoord(i).m_row) < a_theBoard.GetNRows()) );
        assert( ((a_coord.m_col + a_piece.GetCoord(i).m_col) >= 0) &&
        		((a_coord.m_col + a_piece.GetCoord(i).m_col) < a_theBoard.GetNColumns()) );

        assert(a_theBoard.IsPlayerInCoord(
                thisCoord,
        		a_playerMe));
#endif

        a_theBoard.BlankCoord(thisCoord);

        // (a_coordX + pieceX, a_coordY + pieceY) is now empty
        // is it now a nucleation point for the opponent? (it couldn't be before, as it was occupied by 'me')
        if (Rules::IsNucleationPointCompute(
                a_theBoard,
        		a_playerOpponent,
        		thisCoord))
        {
        	a_playerOpponent.SetNucleationPoint(thisCoord);
        }
    }

    // recalculate all the nk points around the piece we just removed (player 'me')
    Rules::RecalculateNKAroundPiece(a_theBoard, a_piece, a_coord, a_playerMe);
}

void Game1v1::PutDownPiece(
            const Piece      &a_piece,
            const Coordinate &a_coord,
            eGame1v1Player_t a_player)
{
    switch (a_player)
    {
    case e_Game1v1Player1:
    {
        Game1v1::PutDownPiece(m_board, a_piece, a_coord, m_player1, m_player2);
        m_player1.UnsetPiece(a_piece.GetType());
        break;
    }
    case e_Game1v1Player2:
    {
        Game1v1::PutDownPiece(m_board, a_piece, a_coord, m_player2, m_player1);
        m_player2.UnsetPiece(a_piece.GetType());
        break;
    }
#ifdef DEBUG
    default:
        assert(0);
#endif
    } // switch (a_player)
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
        Coordinate thisCoord(a_coord.m_row + a_piece.GetCoord(i).m_row,
                             a_coord.m_col + a_piece.GetCoord(i).m_col);
#ifdef DEBUG

        assert( ((a_coord.m_row + a_piece.GetCoord(i).m_row) >= 0) &&
        		((a_coord.m_row + a_piece.GetCoord(i).m_row) < a_theBoard.GetNRows()) );
        assert( ((a_coord.m_col + a_piece.GetCoord(i).m_col) >= 0) &&
        		((a_coord.m_col + a_piece.GetCoord(i).m_col) < a_theBoard.GetNColumns()) );

        assert(a_theBoard.IsCoordEmpty(thisCoord));
#endif

        a_theBoard.SetPlayerInCoord(
                thisCoord,
        		a_playerMe);

        // this new point is being occupied by the player 'me'. It can't be a nucleation point
        // for the opponent any more
        a_playerOpponent.UnsetNucleationPoint(thisCoord);
    }

    // recalculate all the nk points around the piece we just put down (player 'me')
    Rules::RecalculateNKAroundPiece(a_theBoard, a_piece, a_coord, a_playerMe);
}

int32_t Game1v1::MinMax(
        Heuristic::EvalFunction_t    a_heuristicMethod,
        int32_t                      depth,
        eGame1v1Player_t             a_player,
        Piece                        &out_resultPiece,
        Coordinate                   &out_coord,
        const volatile sig_atomic_t  &stopProcessingFlag,
        const Coordinate             &a_lastOpponentPieceCoord,
        const Piece                  &a_lastOpponentPiece)
{
    Player* playerMe       = NULL;
    Player* playerOpponent = NULL;

    switch (a_player)
    {
    case e_Game1v1Player1:
    {
        playerMe       = &m_player1;
        playerOpponent = &m_player2;
        break;
    }
    case e_Game1v1Player2:
    {
        playerMe       = &m_player2;
        playerOpponent = &m_player1;
        break;
    }
#ifdef DEBUG
    default:
        assert(0);
#endif
    } // switch (a_player)

	// putting down the 1st piece is a special case
	if (playerMe->NumberOfPiecesAvailable() == e_numberOfPieces)
	{
		return ComputeFirstPiece(
				m_board,
				*playerMe,
				*playerOpponent,
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
    playerMe->GetAllNucleationPoints(nkPointSetMe);

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
    if (playerMe->NumberOfPiecesAvailable() == 0)
    {
        // bestPiece doesn't represent any piece yet
        out_resultPiece = bestPiece;

        // calculate the heuristic of the board to return it, even though the function calculated fuck-all
        return a_heuristicMethod(m_board, *playerMe, *playerOpponent);
    }

#ifdef DEBUG_PRINT
    std::cout << "NK: " << playerMe->NumberOfNucleationPoints() << std::endl;
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

        if ( (playerMe->IsPieceAvailable(static_cast<ePieceType_t>(i)) == false) ||
             ( (playerMe->m_pieces[i].GetNSquares() < 5) &&
               (playerMe->NumberOfPiecesAvailable() > (e_numberOfPieces - MIN_5SQUARE_PIECES_AT_START)) &&
               (nBranchesSearchTree > 0) ) )
        {
            // piece is not available OR it should be ignored
            // if at least one 5-square piece has been put down before this non 5-square piece
            // do not try to put down the piece
            continue;
        }

        playerMe->UnsetPiece(static_cast<ePieceType_t>(i));

        const std::list< CoordinateArray<PIECE_MAX_SQUARES> > &coordConfList =
            playerMe->m_pieces[i].GetPrecalculatedConfs();
        std::list< CoordinateArray<PIECE_MAX_SQUARES> >::const_iterator pieceCoordIt;

        for (pieceCoordIt = coordConfList.begin();
             pieceCoordIt != coordConfList.end();
             pieceCoordIt++)
        {
            // update piece with current precalculated configuration
            playerMe->m_pieces[i].SetCurrentCoords(*pieceCoordIt);

            bool nkExists;
            Coordinate thisNkPoint;
            Player::SpiralIterator nkIterator;

            nkExists = playerMe->GetFirstNucleationPointSpiral(nkIterator, thisNkPoint);
            while(nkExists)
            {
                // retrieve the valid coords of this piece in the current nk point
                Coordinate validCoords[VALID_COORDS_SIZE];
                int32_t nValidCoords = Rules::CalculateValidCoordsInNucleationPoint(
                                            m_board,
                                            playerMe->m_pieces[i],
                                            thisNkPoint,
                                            *playerMe,
                                            validCoords,
                                            VALID_COORDS_SIZE);

                for (uint8_t k = 0 ; k < nValidCoords ; k++)
                {
                    if (!testedCoords.isPresent(validCoords[k]))
                    {
                        testedCoords.insert(validCoords[k]);

                        Game1v1::PutDownPiece(
                                m_board,
                                playerMe->m_pieces[i],
                                validCoords[k],
                                *playerMe,
                                *playerOpponent);

                        // save a pointer to this piece in the place (index) reserved for it
                        lastPiecesMe[0] = &(playerMe->m_pieces[i]);

                        // number of branches searched at this level of the tree
                        nBranchesSearchTree++;

                        int32_t maxValue = -Game1v1::MinMaxAlphaBetaCompute(
                                                m_board,
                                                *playerOpponent,
                                                oldNkPointsOpponent,
                                                lastPiecesOpponent,
                                                *playerMe,
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

                        if (maxValue > alpha)
                        {
                            bestPiece = playerMe->m_pieces[i];
                            bestCoord.m_row = validCoords[k].m_row;
                            bestCoord.m_col = validCoords[k].m_col;

                            alpha = maxValue;
                        }

                        Game1v1::RemovePiece(
                                m_board,
                                playerMe->m_pieces[i],
                                validCoords[k],
                                *playerMe,
                                *playerOpponent);

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

#ifdef DEBUG
                        assert(beta > alpha);
#endif
                    } // if (it == testedCoords.end())
                } // for (uint8_t k = 0 ; k < nValidCoords ; k++)

                nkExists = playerMe->GetNextNucleationPointSpiral(nkIterator, thisNkPoint);

            } // while(nkExists)

            testedCoords.clear();
        } // for (pieceCoordIt = coordConfList.begin()

        playerMe->SetPiece(static_cast<ePieceType_t>(i));
        playerMe->m_pieces[i].Reset();
    } // for (int i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)

    // only the highest node (root node) of the tree saves the result of the algorithm
    out_resultPiece = bestPiece;
    out_coord.m_row = bestCoord.m_row;
    out_coord.m_col = bestCoord.m_col;

#ifdef DEBUG_PRINT
    std::cout << "Times called " << timesCalled << std::endl;
#endif

    return alpha;
}

int32_t Game1v1::ComputeFirstPiece(
        Board            &a_board,
        Player           &a_playerMe,
        Player           &a_playerOpponent,
        const Coordinate &a_lastOpponentPieceCoord,
        const Piece      &a_lastOpponentPiece,
        Piece            &out_resultPiece,
        Coordinate       &out_coord)
{
    // try to calculate a 'smart' first move quickly
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
        // Do an acceptable 1st move (quick and dirty solution).
        // The folloing piece of code will put the cross in an 'good'
        // place, for example in 9,8 -if starting coord is (9,9)-
        // or 5,6 if starting coord is (4,4). Which are the standard
        // starting coords
        out_resultPiece = Piece(e_5Piece_Cross);

        Coordinate startingCoord = a_playerMe.GetStartingCoordinate();
        if ((a_board.GetNColumns() / 2) > startingCoord.m_col)
        {
            out_coord.m_col = startingCoord.m_col + 1;
        }
        else if ((a_board.GetNColumns() / 2) < startingCoord.m_col)
        {
            out_coord.m_col = startingCoord.m_col - 1;
        }
        else
        {
            out_coord.m_col = startingCoord.m_col;
        }

        out_coord.m_row = startingCoord.m_row;
    }

    // check out now if the move calculated in the previous step is valid
    if (!Rules::IsPieceDeployableInStartingPoint(
            a_board,
            out_resultPiece,
            out_coord,
            a_playerMe.GetStartingCoordinate()) )
    {
        // move wasn't valid. calculate a new one (if it is possible)

        // new move. Restart variables which save the calculated move
        out_resultPiece = Piece(e_noPiece);
        out_coord = Coordinate();

        int32_t heuristicValue = -INFINITE;

        for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
        {
            // go trough the list of pieces to find out what move is the best one
            if (!a_playerMe.IsPieceAvailable(static_cast<ePieceType_t>(i)))
            {
                // it is the 1st piece to be deployed. It shouldn't be needed
                continue;
            }

            const std::list< CoordinateArray<PIECE_MAX_SQUARES> > &coordConfList =
                a_playerMe.m_pieces[i].GetPrecalculatedConfs();
            std::list< CoordinateArray<PIECE_MAX_SQUARES> >::const_iterator pieceCoordIt;

            for (pieceCoordIt = coordConfList.begin();
                 pieceCoordIt != coordConfList.end();
                 pieceCoordIt++)
            {
                // update piece with current precalculated configuration
                a_playerMe.m_pieces[i].SetCurrentCoords(*pieceCoordIt);

                Coordinate validCoords[VALID_COORDS_SIZE];
                int32_t nValidCoords = Rules::CalculateValidCoordsInStartingPoint(
                                            a_board,
                                            a_playerMe.m_pieces[i],
                                            a_playerMe.GetStartingCoordinate(),
                                            a_playerMe,
                                            validCoords,
                                            VALID_COORDS_SIZE);
                for (uint8_t k = 0 ; k < nValidCoords ; k++)
                {
                    Game1v1::PutDownPiece(
                            a_board,
                            a_playerMe.m_pieces[i],
                            validCoords[k],
                            a_playerMe,
                            a_playerOpponent);

                    // taking over the center of the board is pretty important. Use
                    // nk weighted heuristic to try to calculate alternative 1st move
                    int32_t currentValue =
                        Heuristic::CalculateNKWeightedv1(a_board, a_playerMe, a_playerOpponent);

                    if (currentValue > heuristicValue)
                    {
                        // this is the best move so far
                        heuristicValue = currentValue;
                        out_resultPiece = a_playerMe.m_pieces[i];
                        out_coord = validCoords[k];
                    }

                    Game1v1::RemovePiece(
                            a_board,
                            a_playerMe.m_pieces[i],
                            validCoords[k],
                            a_playerMe,
                            a_playerOpponent);
                } // for (uint8_t k = 0 ; k < nValidCoords ; k++)

                a_playerMe.m_pieces[i].RotateRight();
            } // for (pieceCoordIt = coordConfList.begin()

            // leave the piece as it was!!
            a_playerMe.m_pieces[i].Reset();
        } // for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
    } // if (!Rules::IsPieceDeployableInStartingPoint(

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
        Heuristic::EvalFunction_t    a_heuristicMethod,
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
    // to the pieces put by 'me'
    a_oldNkPointsMe[(originalDepth / 2) - (depth / 2)] = &nkPointSetMe;

    // number of pieces successfully put down
    // if no pieces could be put down the recursive function wasn't called even once,
    // which means the search tree was broken.
    int32_t nBranchesSearchTree = 0;

	for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
	{
        if ( (a_playerMe.IsPieceAvailable(static_cast<ePieceType_t>(i)) == false) ||
             ( (a_playerMe.m_pieces[i].GetNSquares() < 5) &&
               (a_playerMe.NumberOfPiecesAvailable() > (e_numberOfPieces - MIN_5SQUARE_PIECES_AT_START)) &&
               (nBranchesSearchTree > 0) ) )
        {
            // piece is not available OR it should be ignored
            // if at least one 5-square piece has been put down before this non 5-square piece
            // do not try to put down the piece
            continue;
        }

        a_playerMe.UnsetPiece(static_cast<ePieceType_t>(i));

        const std::list< CoordinateArray<PIECE_MAX_SQUARES> > &coordConfList =
            a_playerMe.m_pieces[i].GetPrecalculatedConfs();
        std::list< CoordinateArray<PIECE_MAX_SQUARES> >::const_iterator pieceCoordIt;

        for (pieceCoordIt = coordConfList.begin();
             pieceCoordIt != coordConfList.end();
             pieceCoordIt++)
        {
            // update piece with current precalculated configuration
            a_playerMe.m_pieces[i].SetCurrentCoords(*pieceCoordIt);

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
                } // for (thisLevel = (originalDepth / 2) - (depth / 2) - 1;

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
        } // for (pieceCoordIt = coordConfList.begin()

        a_playerMe.SetPiece(static_cast<ePieceType_t>(i));
        a_playerMe.m_pieces[i].Reset();
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

void Game1v1::Reset(
    const Coordinate &a_player1StartingCoord,
    const Coordinate &a_player2StartingCoord)
{
    m_board.Reset();
    m_player1.Reset(a_player1StartingCoord);
    m_player2.Reset(a_player2StartingCoord);
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
            switch(line.at(charInLine))
            {
            case CHAR_EMPTY:
                m_board.BlankCoord(Coordinate(i, j));
                break;
            case CHAR_PLAYER1:
                m_board.SetPlayerInCoord(Coordinate(i, j), m_player1);
                break;
            case CHAR_PLAYER2:
                m_board.SetPlayerInCoord(Coordinate(i, j), m_player2);
                break;
            default:
                m_board.BlankCoord(Coordinate(i, j));
#ifdef DEBUG
                assert(0);
#endif
            } // switch(line.at(charInLine))
            charInLine += 4;
        }
    }
    getline(a_inStream, line); // don't need it

    getline(a_inStream, line); // this line contains me's pieces left
    for (int i = 0; i < e_numberOfPieces; i++)
    {
        if (line.at(i) == '0')
        {
        	m_player1.UnsetPiece(static_cast<ePieceType_t>(i));
        }
    }
    getline(a_inStream, line); // this line contains opponent's pieces left
    for (int i = 0; i < e_numberOfPieces; i++)
    {
        if (line.at(i) == '0')
        {
        	m_player2.UnsetPiece(static_cast<ePieceType_t>(i));
        }
    }

    Rules::RecalculateNKInAllBoard(m_board, m_player1, m_player2);

    return true;
}

bool Game1v1::SaveGame(std::ostream& a_outStream)
{
    m_board.PrintBoard(a_outStream);
    m_player1.PrintAvailablePieces(a_outStream);
    m_player2.PrintAvailablePieces(a_outStream);

    a_outStream << std::endl;

    return true;
}

