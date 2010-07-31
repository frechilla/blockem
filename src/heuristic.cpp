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
/// @file  heuristic.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 31-Mar-2009  Original development
///           Faustino Frechilla 23-Jul-2010  i18n
/// @endhistory
///
// ============================================================================

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "gettext.h" // i18n
#include "heuristic.h"
#include "player.h"
#include "rules.h"
#include "bitwise.h"

/// Pieces that will be put down while trying to take over the centre of the board
/// see CalculateNKWeightedv1 for an example
static const int32_t N_PIECES_TO_TAKE_OVER_THE_CENTRE = 3;

// instantiate the const heuristic data array. Heuristics must be defined here
// in the same order they are described in Heuristic::eHeuristicType_t in heuristic.h
const Heuristic::sHeuristicData_t Heuristic::m_heuristicData[e_heuristicCount] =
{
    {e_heuristicInfluenceArea,
     Heuristic::CalculateInfluenceAreaWeighted,
     // i18n TRANSLATORS: This is the name given to a type of heuristic. Should be as small as possible
     N_("Influence Area"),
     // i18n TRANSLATORS: This is a quick explanation of what the "Influence Area" heuristic is supposed
     // i18n to do. Shouldn't be too long since it will have to fit nicely on the GUI widgets
     N_("Uses the influence areas that a user's pieces create on the board to determine the quality of a move")
    },
    {e_heuristicInfluenceAreaEastwood,
     Heuristic::CalculateInfluenceAreaWeightedEastwood,
     // i18n TRANSLATORS: This is the name given to a type of heuristic. Should be as small as possible
     N_("Mr. Eastwood"),
     // i18n TRANSLATORS: This is a quick explanation of what the "Mr. Eastwood" heuristic is supposed
     // i18n to do. Shouldn't be too long since it will have to fit nicely on the GUI widgets
     N_("Modified version of \"Influence Area\". When in doubt it will try to block you")
    },
    {e_heuristicNKWeightedv1,
     Heuristic::CalculateNKWeightedv1,
     // i18n TRANSLATORS: This is the name given to a type of heuristic. Should be as small as possible
     // i18n NK stands for Nucleation point
     N_("NK weighted"),
     // i18n TRANSLATORS: This is a quick explanation of what the "NK weighted" heuristic is supposed
     // i18n to do. Shouldn't be too long since it will have to fit nicely on the GUI widgets
     // i18n NK stands for Nucleation point
     N_("The more Nucleation points the better. NK points are more important in the middle of the board at the beginning")
    },
//    {e_heuristicNKWeightedv2,
//     Heuristic::CalculateNKWeightedv2,
//     // i18n TRANSLATORS: This is the name given to a type of heuristic. Should be as small as possible
//     N_("NK weighted v2"),
//     // i18n TRANSLATORS: This is a quick explanation of what the "NK weighted v2" heuristic is supposed
//     // i18n to do. Shouldn't be too long since it will have to fit nicely on the GUI widgets
//     N_("Evolution of v1. The more pieces you can put down per nk point the better")
//    },
    {e_heuristicCentreFocused,
     Heuristic::CalculateCentreFocused,
     // i18n TRANSLATORS: This is the name given to a type of heuristic. Should be as small as possible
     N_("Centre focused"),
     // i18n TRANSLATORS: This is a quick explanation of what the "Centre focused" heuristic is supposed
     // i18n to do. Shouldn't be too long since it will have to fit nicely on the GUI widgets
     N_("It has a tendency to create nucleation points over the centre of the board")
    },
    {e_heuristicSimple,
     Heuristic::CalculateSimple,
     // i18n TRANSLATORS: This is the name given to a type of heuristic. Should be as small as possible
     N_("Simple"),
     // i18n TRANSLATORS: This is a quick explanation of what the "Simple" heuristic is supposed
     // i18n to do. Shouldn't be too long since it will have to fit nicely on the GUI widgets
     N_("Takes into account only the amount of squares of the deployed pieces")
    },
    {e_heuristicRandom,
     Heuristic::CalculateRandom,
     // i18n TRANSLATORS: This is the name given to a type of heuristic. Should be as small as possible
     N_("Random"),
     // i18n TRANSLATORS: This is a quick explanation of what the "Random" heuristic is supposed
     // i18n to do. Shouldn't be too long since it will have to fit nicely on the GUI widgets
     N_("Random heuristic. Evaluation function returns a random value so any heuristic can be checked against it")
    },
//    {e_heuristicAmountOfPieces,
//     Heuristic::CalculateNPieces,
//     // i18n TRANSLATORS: This is the name given to a type of heuristic. Should be as small as possible
//     N_("Amount of pieces"),
//     // i18n TRANSLATORS: This is a quick explanation of what the "Amount of pieces" heuristic is supposed
//     // i18n to do. Shouldn't be too long since it will have to fit nicely on the GUI widgets
//     N_("The more pieces can be put down per NK point, the better. It's slow and a bit crazy sometimes")
//    }
};

int32_t Heuristic::CalculateSimple(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;
    int32_t rv = 0;

    Coordinate thisCoord(0, 0);
    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col <  a_board.GetNColumns() ; thisCoord.m_col++)
        {
            if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
            {
                squaresMe++;
            }
            else if (a_board.IsPlayerInCoord(thisCoord, a_playerOpponent))
            {
                squaresOpponent++;
            }
        }
    }

    rv += (squaresMe << 2); // times 4
    rv += (a_playerMe.NumberOfNucleationPoints() << 1); // times 2
    rv -= (squaresOpponent << 2); // times 4
    rv -= a_playerOpponent.NumberOfNucleationPoints();

    return rv;
}

int32_t Heuristic::CalculateRandom(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    static bool runOnceFlag = false;

    if (runOnceFlag == false)
    {
        runOnceFlag = true;
        // initialise seed. Not a very random way to initialise the seed
        // but security is not an issue while playing blockem
        srand (time(NULL));
    }

    return rand();
}

int32_t Heuristic::CalculateCentreFocused(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t rv = 0;
    int32_t valueNkMe = 0;
    int32_t squaresMe = 0;
    int32_t valueNkOpponent = 0;
    int32_t squaresOpponent = 0;

    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
        {
            if (a_board.IsCoordEmpty(thisCoord))
            {
                bool opponentNKPoint = false;
                int32_t weightedValue = 0;

                if (a_playerOpponent.IsNucleationPoint(thisCoord))
                {
              		weightedValue = CalculateCircularWeight(a_board, thisCoord);
                    opponentNKPoint = true;

                    valueNkOpponent += weightedValue;
                } // if (a_playerOpponent->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))

                if (a_playerMe.IsNucleationPoint(thisCoord))
                {
                    if (!opponentNKPoint)
                    {
                        weightedValue = CalculateCircularWeight(a_board, thisCoord);
                    	if (rules::IsCoordTouchingPlayerCompute(a_board, thisCoord, a_playerOpponent))
                    	{
                    	    // an nk point that is touching the other player is unblockable by the opponent
                    	    // (it might get bloked, but not directly)
                    		weightedValue += 1;
                    	}
                    }
                    else
                    {
                        // the weighted value of this nk point is half the value of the opponent
                        // because putting down a piece which is sharing nk points is bad
                        // since the next go will be for the opponent
                        weightedValue /= 2;
                    }

                    valueNkMe += weightedValue;
                } // if (a_playerMe->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))
            }
            else if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
            {
                squaresMe++;
            }
            else // if (a_board.IsPlayerInCoord(thisCoord.m_row, thisCoord.m_col, a_playeOpponent)
            {
                squaresOpponent++;
            }
        }
    }

    rv += valueNkMe;
    rv += squaresMe;
    rv -= valueNkOpponent;
    rv -= squaresOpponent;

    return rv;
}

int32_t Heuristic::CalculateNKWeightedv1(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    if (a_playerOpponent.NumberOfPiecesAvailable() >=
           (e_numberOfPieces - N_PIECES_TO_TAKE_OVER_THE_CENTRE))
    {
        return CalculateCentreFocused(a_board, a_playerMe, a_playerOpponent);
    }

    int32_t rv = 0;

    int32_t valueNkMe = 0;
    int32_t valueNkOpponent = 0;
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;

    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
        {
            if (a_board.IsCoordEmpty(thisCoord))
            {
                bool opponentNKPoint = false;

                if (a_playerOpponent.IsNucleationPoint(thisCoord))
                {
                    opponentNKPoint = true;
                    valueNkOpponent += 1;
                }

                if (a_playerMe.IsNucleationPoint(thisCoord))
                {
                    if (!opponentNKPoint)
                    {
                        valueNkMe += 1;

                        if (rules::IsCoordTouchingPlayerCompute(a_board, thisCoord, a_playerOpponent))
                        {
                            // an nk point that is touching the other player is unblockable by the opponent
                            // (it might get blocked, but not directly)
                            valueNkMe += 1;
                        }
                    }
                    /*
                    else
                    {
                        // the weighted value of this nk point is none
                        // because putting down a piece which is sharing nk points is bad
                        // since the next go will be for the opponent
                        valueNkMe += 0;
                    }*/

                } // if (a_playerMe->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))
            }
            else if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
            {
                squaresMe++;
            }
            else // if (a_board.IsPlayerInCoord(thisCoord, a_playeOpponent))
            {
                squaresOpponent++;
            }
        } // for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
    } // for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)

    rv += (valueNkMe);
    rv += (squaresMe << 1);       // times 2
    rv -= (valueNkOpponent);
    rv -= (squaresOpponent << 1); // times 2

    return rv;
}

//int32_t Heuristic::CalculateNKWeightedv2(
//        const Board  &a_board,
//        const Player &a_playerMe,
//        const Player &a_playerOpponent)
//{
//    // see if we should only try to take over the centre of the board
//    // (this happens at the beginning)
//    if (a_playerOpponent.NumberOfPiecesAvailable() >=
//           (e_numberOfPieces - N_PIECES_TO_TAKE_OVER_THE_CENTRE))
//    {
//        return CalculateCentreFocused(a_board, a_playerMe, a_playerOpponent);
//    }
//
//    int32_t rv = 0;
//    int32_t valueNkMe = 0;
//    int32_t squaresMe = 0;
//    int32_t valueNkOpponent = 0;
//    int32_t squaresOpponent = 0;
//
//    Coordinate thisCoord;
//    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
//    {
//        for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
//        {
//            if (a_board.IsCoordEmpty(thisCoord))
//            {
//                bool opponentNKPoint = false;
//
//                if (a_playerOpponent.IsNucleationPoint(thisCoord))
//                {
//                    valueNkOpponent += BiggestPieceDeployableInNKPointSize(a_board, a_playerOpponent, thisCoord);
//                    opponentNKPoint = true;
//
//                    if (rules::IsCoordTouchingPlayer(a_board, thisCoord, a_playerMe))
//                    {
//                        // an nk point that is touching the other player is unblockable by the opponent
//                        // (it might get blocked, but not directly)
//                        valueNkOpponent += 1;
//                    }
//                } // if (a_playerOpponent->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))
//
//                if (a_playerMe.IsNucleationPoint(thisCoord))
//                {
//                    valueNkMe += BiggestPieceDeployableInNKPointSize(a_board, a_playerMe, thisCoord);
//
//                    if (!opponentNKPoint)
//                    {
//                        if (rules::IsCoordTouchingPlayer(a_board, thisCoord, a_playerOpponent))
//                        {
//                            // an nk point that is touching the other player is unblockable by the opponent
//                            // (it might get blocked, but not directly)
//                            valueNkMe += 1;
//                        }
//                    }
//                    else
//                    {
//                        // the weighted value of this nk point is half the value cos it is shared
//                        // and the opponent could block it in the next move
//                        valueNkMe -= 1;
//                    }
//                } // if (a_playerMe->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))
//            }
//            else if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
//            {
//                squaresMe++;
//            }
//            else // if (a_board.IsPlayerInCoord(thisCoord, a_playeOpponent))
//            {
//                squaresOpponent++;
//            }
//        } // for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
//    } // for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
//
//    // times 4 so NK point values don't get much more importance
//    rv += (squaresMe << 2);       // times 4
//    rv -= (squaresOpponent << 2); // times 4
//
//    // NK point values
//    rv += (valueNkMe << 1);  // times 2
//    rv -= (valueNkOpponent);
//
//    return rv;
//}

int32_t Heuristic::CalculateInfluenceAreaWeighted(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t rv = 0;

    int32_t valueNkMe = 0;
    int32_t valueNkOpponent = 0;
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;

    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
        {
            if (a_board.IsCoordEmpty(thisCoord))
            {
                bool opponentNKPoint = false;

                if (a_playerOpponent.IsNucleationPoint(thisCoord))
                {
                    opponentNKPoint = true;
                    valueNkOpponent += 1;
                }
                else if(a_playerOpponent.IsCoordInfluencedByPlayer(thisCoord))
                {
                    rv -= 1;
                }

                if (a_playerMe.IsNucleationPoint(thisCoord))
                {
                    if (!opponentNKPoint)
                    {
                        valueNkMe += 1;

                        if (rules::IsCoordTouchingPlayerCompute(a_board, thisCoord, a_playerOpponent))
                        {
                            // an nk point that is touching the other player is unblockable by the opponent
                            // (it might get blocked, but not directly)
                            valueNkMe += 1;
                        }
                    }
                }
                else if(a_playerMe.IsCoordInfluencedByPlayer(thisCoord))
                {
                    rv += 1;
                }
            }
            else if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
            {
                squaresMe++;
            }
            else // if (a_board.IsPlayerInCoord(thisCoord, a_playeOpponent))
            {
                squaresOpponent++;
            }
        } // for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
    } // for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)

    rv += (valueNkMe);
    rv += (squaresMe << 1);       // times 2
    rv -= (valueNkOpponent);
    rv -= (squaresOpponent << 1); // times 2

    return rv;
}

int32_t Heuristic::CalculateInfluenceAreaWeightedEastwood(
        const Board  &a_board,
        const Player &a_playerMe,
        const Player &a_playerOpponent)
{
    int32_t rv = 0;

    int32_t valueNkMe = 0;
    int32_t valueNkOpponent = 0;
    int32_t squaresMe = 0;
    int32_t squaresOpponent = 0;

    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
        {
            if (a_board.IsCoordEmpty(thisCoord))
            {
                bool opponentNKPoint = false;

                if (a_playerOpponent.IsNucleationPoint(thisCoord))
                {
                    opponentNKPoint = true;
                    valueNkOpponent += 1;
                }
                else if(a_playerOpponent.IsCoordInfluencedByPlayer(thisCoord))
                {
                    rv -= 2;
                }

                if (a_playerMe.IsNucleationPoint(thisCoord))
                {
                    if (!opponentNKPoint)
                    {
                        valueNkMe += 1;

                        if (rules::IsCoordTouchingPlayerCompute(a_board, thisCoord, a_playerOpponent))
                        {
                            // an nk point that is touching the other player is unblockable by the opponent
                            // (it might get blocked, but not directly)
                            valueNkMe += 1;
                        }
                    }
                }
                else if(a_playerMe.IsCoordInfluencedByPlayer(thisCoord))
                {
                    rv += 1;
                }
            }
            else if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
            {
                squaresMe++;
            }
            else // if (a_board.IsPlayerInCoord(thisCoord, a_playeOpponent))
            {
                squaresOpponent++;
            }
        } // for (thisCoord.m_col = 0; thisCoord.m_col < a_board.GetNColumns() ; thisCoord.m_col++)
    } // for (thisCoord.m_row = 0; thisCoord.m_row < a_board.GetNRows() ; thisCoord.m_row++)

    rv += (valueNkMe);
    rv += (squaresMe << 1);       // times 2
    rv -= (valueNkOpponent << 1); // times 2
    rv -= (squaresOpponent << 1); // times 2

    return rv;
}

//int32_t Heuristic::CalculateNPieces(
//        const Board  &a_board,
//        const Player &a_playerMe,
//        const Player &a_playerOpponent)
//{
//    int32_t rv = 0;
//
//    int32_t nSquaresCanDeployMe = 0;
//    //int32_t nSquaresCanDeployOpponent = 0;
//
//    // current coordiante being studied
//    Coordinate thisCoord(0, 0);
//    uint64_t bitwiseBoard;   // place in the board is empty or not
//    uint64_t bitwiseBoardMe; // place in the board is occupied by player me
//    bitwise::BoardCalculate(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
//
//    // the following loop goes trough the board doing a S-like movement.
//    // it goes from left to right, then down, then right to left, then down,
//    // then left to right...
//    // checking how many pieces can be put down per position on the board
//    while (true)
//    {
//        for (thisCoord.m_col = 0;
//             thisCoord.m_col < (a_board.GetNColumns() - 1);
//             thisCoord.m_col += 1)
//        {
//            if (a_board.IsCoordEmpty(thisCoord))
//            {
//                nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
//                                            a_board,
//                                            a_playerMe,
//                                            bitwiseBoard,
//                                            bitwiseBoardMe);
//            }
//
//            rv += NKBasedHeuristicThisCoord(
//                    a_board,
//                    a_playerMe,
//                    a_playerOpponent,
//                    thisCoord);
//
//            bitwise::BoardMoveRight(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
//        }
//        // latest configuration wasn't checked
//        if (a_board.IsCoordEmpty(thisCoord))
//        {
//            nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
//                                        a_board,
//                                        a_playerMe,
//                                        bitwiseBoard,
//                                        bitwiseBoardMe);
//        }
//
//        rv += NKBasedHeuristicThisCoord(
//                    a_board,
//                    a_playerMe,
//                    a_playerOpponent,
//                    thisCoord);
//
//
//        if ((thisCoord.m_row + 1) >= a_board.GetNRows())
//        {
//            break; // got to the latest row of the board
//        }
//
//        // next row
//        bitwise::BoardMoveDown(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
//        thisCoord.m_row++;
//
//        // check moving to the left
//        for (thisCoord.m_col = (a_board.GetNColumns() - 1);
//             thisCoord.m_col > 0;
//             thisCoord.m_col -= 1)
//        {
//            if (a_board.IsCoordEmpty(thisCoord))
//            {
//                nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
//                                            a_board,
//                                            a_playerMe,
//                                            bitwiseBoard,
//                                            bitwiseBoardMe);
//            }
//
//            rv += NKBasedHeuristicThisCoord(
//                    a_board,
//                    a_playerMe,
//                    a_playerOpponent,
//                    thisCoord);
//
//            bitwise::BoardMoveLeft(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
//        }
//        // latest configuration wasn't checked
//        if (a_board.IsCoordEmpty(thisCoord))
//        {
//            nSquaresCanDeployMe += CountSquaresCanBeDeployedBitwise(
//                                        a_board,
//                                        a_playerMe,
//                                        bitwiseBoard,
//                                        bitwiseBoardMe);
//        }
//
//        rv += NKBasedHeuristicThisCoord(
//                    a_board,
//                    a_playerMe,
//                    a_playerOpponent,
//                    thisCoord);
//
//
//        if ((thisCoord.m_row + 1) >= a_board.GetNRows())
//        {
//            break; // got to the latest row of the board
//        }
//
//        // next row
//        bitwise::BoardMoveDown(a_board, thisCoord, a_playerMe, bitwiseBoard, bitwiseBoardMe);
//        thisCoord.m_row++;
//    }
//
//    int32_t squaresAvailableMe = 0;
//    //int32_t squaresOpponent = 0;
//    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
//    {
//        if (a_playerMe.IsPieceAvailable(static_cast<ePieceType_t>(i)))
//        {
//            squaresAvailableMe += a_playerMe.m_pieces[i].GetNSquares();
//        }
////        if (!a_playerOpponent.IsPieceAvailable(static_cast<ePieceType_t>(i)))
////        {
////            squaresOpponent += a_playerOpponent.m_pieces[i].GetNSquares();
////        }
//    }
//
////    rv += (squaresMe);
////    rv -= (squaresOpponent);
//      //rv -= (nSquaresCanDeployOpponent * 4);
//
//    rv <<= 3;
//    int32_t nTotalUsableSquares = squaresAvailableMe * a_board.GetNRows() * a_board.GetNColumns();
//    if ( (nTotalUsableSquares - nSquaresCanDeployMe) > (nTotalUsableSquares << 2) )
//    {
//        // number of squares that can be deployed is less than 3 quarters of the total amount of squares
//        // use the average amount of deployable squares in the heuristic. It forces moves to touch
//        // the opponent (which is a common thing human beings do)
//        rv += (nSquaresCanDeployMe / (a_board.GetNRows() * a_board.GetNColumns()));
//    }
//
//    return rv;
//}

//int32_t Heuristic::NKBasedHeuristicThisCoord(
//        const Board  &a_board,
//        const Player &a_playerMe,
//        const Player &a_playerOpponent,
//        const Coordinate &thisCoord)
//{
//    int32_t valueNkMe = 0;
//    int32_t valueNkOpponent = 0;
//    int32_t squaresMe = 0;
//    int32_t squaresOpponent = 0;
//
//    if (a_board.IsCoordEmpty(thisCoord))
//    {
//        if (a_playerOpponent.IsNucleationPoint(thisCoord))
//        {
//            valueNkOpponent = 2;
//        }
//
//        if (a_playerMe.IsNucleationPoint(thisCoord))
//        {
//            if (valueNkOpponent == 0)
//            {
//                valueNkMe = 2;
//                if (rules::IsCoordTouchingPlayerCompute(a_board, thisCoord, a_playerOpponent))
//                {
//                    // an nk point that is touching the other player is unblockable by the opponent
//                    // (it might get blocked, but not directly)
//                   valueNkMe += 1;
//                }
//            }
//            else
//            {
//                // the weighted value of this nk point is half the value of the opponent
//                // because putting down a piece which is sharing nk points is bad
//                // since the next go will be for the opponent
//                valueNkMe = 1;
//            }
//        } // if (a_playerMe->IsNucleationPoint(thisCoord.m_row, thisCoord.m_col))
//    }
//    else if (a_board.IsPlayerInCoord(thisCoord, a_playerMe))
//    {
//        squaresMe++;
//    }
//    else // if (a_board.IsPlayerInCoord(thisCoord, a_playeOpponent))
//    {
//        squaresOpponent++;
//    }
//
//    int32_t rv = 0;
//    rv += squaresMe + valueNkMe;
//    rv -= squaresOpponent + valueNkOpponent;
//
//    return rv;
//}

int32_t Heuristic::CalculateCircularWeight(
		const Board &a_board, const Coordinate &a_coord)
{
#if 0
//#ifdef DEBUG
    // this piece of code tried to improve performance using
    // a precalculated static 14x14 array.
    // it might not make a lot of sense, but it is slower
    // than the code below, which runs a calculation
    // instead of just accessing an array
    assert(a_board.GetNRows() == 14);
    assert(a_board.GetNColumns() == 14);
//#endif
    // values precalculated for a 14x14 board
    static const int8_t s_circularWeightValues[14][14] =
    {
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 4, 4, 4, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 4, 4, 4, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 4, 4, 4, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 },
        { 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
    };

    return static_cast<int32_t>(s_circularWeightValues[a_coord.m_row][a_coord.m_col]);
#endif

    int32_t tmpValue =
        std::max(
                abs(a_coord.m_row + 1 - (a_board.GetNRows() / 2)),
                abs(a_coord.m_col + 1 - (a_board.GetNColumns() / 2)) );

    int32_t weightedValue = ( ( (a_board.GetNRows() / 2) - tmpValue ) / 2) + 1;

    return weightedValue;
}

//int32_t Heuristic::CountSquaresCanBeDeployedBitwise(
//        const Board  &a_board,
//        const Player &a_player,
//        uint64_t      a_bitwiseBoard,
//        uint64_t      a_bitwisePlayerBoard)
//{
//    int32_t rValue = 0;
//
//    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
//    {
//        if (a_player.IsPieceAvailable(static_cast<ePieceType_t>(i)))
//        {
//            const std::list<uint64_t> &pieceConfigurations = a_player.m_pieces[i].GetBitwiseList();
//            std::list<uint64_t>::const_iterator it = pieceConfigurations.begin();
//            while (it != pieceConfigurations.end())
//            {
//                uint64_t bPiece = (*it);
//
//                if (bitwise::IsPieceDeployable(bPiece, a_bitwiseBoard, a_bitwisePlayerBoard))
//                {
//                    rValue += a_player.m_pieces[i].GetNSquares();
//                    break;
//                }
//
//                it++;
//            }
//        }
//    }
//
//    return rValue;
//}

//int32_t Heuristic::BiggestPieceDeployableInNKPointSize(
//        const Board      &a_board,
//        const Player     &a_player,
//        const Coordinate &a_NKPointCoord)
//{
//    // Player used as a container for all the pieces
//    const static Player sPlayer(
//            std::string("tmpPlayer"), '_', 1, 1);
//
//    for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
//    {
//        if (a_player.IsPieceAvailable(static_cast<ePieceType_t>(i)))
//        {
//            const std::list<PieceConfiguration> &pieceConfList =
//                                sPlayer.m_pieces[i].GetPrecalculatedConfs();
//
//            std::list<PieceConfiguration>::const_iterator pieceConfIt;
//            for (pieceConfIt  = pieceConfList.begin();
//                 pieceConfIt != pieceConfList.end();
//                 pieceConfIt++)
//            {
//                if (rules::HasValidCoordInNucleationPoint(
//                        a_board,
//                        a_player,
//                        a_NKPointCoord,
//                        *(pieceConfIt),
//                        sPlayer.m_pieces[i].GetRadius()) )
//                {
//                    return sPlayer.m_pieces[i].GetNSquares();
//                }
//            } // for (pieceConfIt  = coordConfList.begin()
//        } // if (a_player.IsPieceAvailable(static_cast<ePieceType_t>(i)))
//    } // for (int8_t i = e_numberOfPieces - 1 ; i >= e_minimumPieceIndex ; i--)
//
//    // no piece can be deployed
//    return 0;
//}
