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
/// @file  rules_test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 15-Jul-2010  Original development
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif

#include "rules_test.h"


// ' ' -> empty char
// 'X' -> player1
// 'O' -> player2
// '+' -> player3
// '=' -> player4
char RulesTest::s_preloadedBoardArray[RULES_TEST_ROWS][RULES_TEST_COLUMNS] =
{
/*         0   1   2   3   4   5   6   7   8   9  10  11  12  13 */
/*  0 */ {'X',' ',' ',' ','X',' ',' ',' ',' ',' ',' ','O','O','O'},
/*  1 */ {'X','X',' ',' ','X','X','X',' ',' ',' ','O','O',' ',' '},
/*  2 */ {' ','X','X',' ','X','O','O','O','O','O',' ',' ',' ',' '},
/*  3 */ {' ',' ',' ','X',' ',' ',' ',' ',' ',' ','O','O',' ',' '},
/*  4 */ {' ',' ',' ','X',' ',' ',' ',' ',' ',' ','O',' ',' ',' '},
/*  5 */ {' ',' ',' ','X','X','X',' ',' ',' ',' ','O',' ',' ',' '},
/*  6 */ {' ','=','=','=',' ',' ',' ',' ',' ',' ','O',' ',' ',' '},
/*  7 */ {' ','=',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
/*  8 */ {' ','=',' ',' ',' ',' ',' ',' ','+',' ',' ',' ',' ',' '},
/*  9 */ {'=',' ',' ',' ',' ',' ',' ',' ','+','+',' ',' ',' ',' '},
/* 10 */ {'=',' ',' ',' ',' ',' ',' ',' ',' ','+','+',' ',' ',' '},
/* 11 */ {'=',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','+','+',' '},
/* 12 */ {'=',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','+',' '},
/* 13 */ {'=',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','+','+'}
};

RulesTest::RulesTest() :
    m_baseBoard(RULES_TEST_ROWS, RULES_TEST_COLUMNS, ' '),
    m_player1(std::string("player1"), 'X', RULES_TEST_ROWS, RULES_TEST_COLUMNS, Coordinate(0, 0)),
    m_player2(std::string("player2"), 'O', RULES_TEST_ROWS, RULES_TEST_COLUMNS, Coordinate(0, 13)),
    m_player3(std::string("player3"), '+', RULES_TEST_ROWS, RULES_TEST_COLUMNS, Coordinate(13, 0)),
    m_player4(std::string("player4"), '=', RULES_TEST_ROWS, RULES_TEST_COLUMNS, Coordinate(13, 13))
{
    // this is quite dirty, but it is the fastest way to set
    // m_baseBoard to the values stored in s_preloadedBoardArray
    Board* pBaseBoard = const_cast<Board*> (&m_baseBoard);
    
    // unset pieces from players. Have a look at the s_preloadedBoardArray
    m_player1.UnsetPiece(e_5Piece_WPiece);
    m_player1.UnsetPiece(e_5Piece_HalfSquare);
    m_player1.UnsetPiece(e_5Piece_MrT);
    m_player2.UnsetPiece(e_5Piece_BigL);
    m_player2.UnsetPiece(e_5Piece_BigPenis);
    m_player2.UnsetPiece(e_5Piece_TheUltimate);
    m_player3.UnsetPiece(e_5Piece_BigS);
    m_player3.UnsetPiece(e_5Piece_WPiece);
    m_player4.UnsetPiece(e_5Piece_BigPenis);
    m_player4.UnsetPiece(e_5Piece_HalfSquare);

    // load s_preloadedBoardArray into m_baseBoard using non-const pointer pBaseBoard
    Coordinate thisCoord;
    for (thisCoord.m_row = 0; thisCoord.m_row < RULES_TEST_ROWS; thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; thisCoord.m_col < RULES_TEST_COLUMNS; thisCoord.m_col++)
        {
            switch(s_preloadedBoardArray[thisCoord.m_row][thisCoord.m_col])
            {
            case 'X':
                pBaseBoard->SetPlayerInCoord(thisCoord, m_player1);
                break;

            case 'O':
                pBaseBoard->SetPlayerInCoord(thisCoord, m_player2);
                break;

            case '+':
                pBaseBoard->SetPlayerInCoord(thisCoord, m_player3);
                break;

            case '=':
                pBaseBoard->SetPlayerInCoord(thisCoord, m_player4);
                break;

            case ' ':
                pBaseBoard->BlankCoord(thisCoord);
                break;
            default:
                // unexpected char in preloaded board
                assert(0);
            } // switch
        } // for (thisCoord.m_col
    } // for (thisCoord.m_row
}

RulesTest::~RulesTest()
{
}

void RulesTest::DoTest()
{
    // test all functions defined in rules, one by one
    TestIsCoordTouchingPlayerCompute();
    TestIsPieceDeployableCompute();
    TestIsNucleationPointCompute();
    TestIsPieceDeployableInCoord();
    TestIsPieceDeployableInStartingPoint();
    TestCalculateValidCoordsInNucleationPoint();
    TestCalculateNextValidCoordInNucleationPoint();
    TestHasValidCoordInNucleationPoint();
    TestCalculateValidCoordsInStartingPoint();
    TestRecalculateNKAroundCoord();
    TestRecalculateInfluenceAreaAroundPieceFast();
    TestIsCoordInfluencedByPlayerFastCompute();
    TestRecalculateInfluenceAreaInBoard();
    TestCanPlayerGo();
}

void RulesTest::TestIsCoordTouchingPlayerCompute()
{
    // we'll run a test on IsCoordTouchingPlayerCompute
    // for the 4 players on 4 different coordiantes
    // (5, 2), (3, 4), (7, 9), (8, 0)
    
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(5, 2),
        m_player1) == true);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(5, 2),
        m_player2) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(5, 2),
        m_player3) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(5, 2),
        m_player4) == true);
        

    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(3, 4),
        m_player1) == true);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(3, 4),
        m_player2) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(3, 4),
        m_player3) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(3, 4),
        m_player4) == false);
        

    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(7, 9),
        m_player1) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(7, 9),
        m_player2) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(7, 9),
        m_player3) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(7, 9),
        m_player4) == false);
        

    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(8, 0),
        m_player1) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(8, 0),
        m_player2) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(8, 0),
        m_player3) == false);
        
    assert(rules::IsCoordTouchingPlayerCompute(
        m_baseBoard,
        Coordinate(8, 0),
        m_player4) == true);
     
    
    // now go through all the coordinates in the board
    // if a coord is taken by a player it has to be touching itself
    // since no baby pieces are deployed
    Coordinate thisCoord;
    for (thisCoord.m_row = 0; 
         thisCoord.m_row < m_baseBoard.GetNRows(); 
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; 
             thisCoord.m_col < m_baseBoard.GetNColumns(); 
             thisCoord.m_col++)
        {
            if (m_baseBoard.IsPlayerInCoord(thisCoord, m_player1))
            {
                assert(rules::IsCoordTouchingPlayerCompute(
                            m_baseBoard,
                            thisCoord,
                            m_player1) == true);
            }
            
            if (m_baseBoard.IsPlayerInCoord(thisCoord, m_player2))
            {
                assert(rules::IsCoordTouchingPlayerCompute(
                            m_baseBoard,
                            thisCoord,
                            m_player2) == true);
            }
            
            if (m_baseBoard.IsPlayerInCoord(thisCoord, m_player3))
            {
                assert(rules::IsCoordTouchingPlayerCompute(
                            m_baseBoard,
                            thisCoord,
                            m_player3) == true);
            }
            
            if (m_baseBoard.IsPlayerInCoord(thisCoord, m_player4))
            {
                assert(rules::IsCoordTouchingPlayerCompute(
                            m_baseBoard,
                            thisCoord,
                            m_player4) == true);
            }
        } // for (thisCoord.m_col
    } // for (thisCoord.m_row
}

void RulesTest::TestIsPieceDeployableCompute()
{
    // deployable
    assert(rules::IsPieceDeployableCompute(
        m_baseBoard,
        m_player1.m_pieces[e_5Piece_Cross].GetCurrentConfiguration(),
        Coordinate(6, 7),
        m_player1) == true);
    
    // not deployable. Touching the side of another player1's piece
    assert(rules::IsPieceDeployableCompute(
        m_baseBoard,
        m_player1.m_pieces[e_5Piece_Cross].GetCurrentConfiguration(),
        Coordinate(6, 6),
        m_player1) == false);
    
    // not deployable. Not touching any nucleation point
    assert(rules::IsPieceDeployableCompute(
        m_baseBoard,
        m_player1.m_pieces[e_5Piece_Cross].GetCurrentConfiguration(),
        Coordinate(11, 5),
        m_player1) == false);
        
    // not deployable. Touching someone else's nk point (and not a valid
    // player1's one)
    assert(rules::IsPieceDeployableCompute(
        m_baseBoard,
        m_player1.m_pieces[e_5Piece_Cross].GetCurrentConfiguration(),
        Coordinate(8, 4),
        m_player1) == false);
    
    // deployable. Touching a shared NK point (shared by player2 and 3)
    assert(rules::IsPieceDeployableCompute(
        m_baseBoard,
        m_player2.m_pieces[e_1Piece_BabyPiece].GetCurrentConfiguration(),
        Coordinate(7, 9),
        m_player2) == true);
    
    // deployable. Touching a shared NK point (shared by player2 and 3)
    assert(rules::IsPieceDeployableCompute(
        m_baseBoard,
        m_player3.m_pieces[e_1Piece_BabyPiece].GetCurrentConfiguration(),
        Coordinate(7, 9),
        m_player3) == true);
        
}

void RulesTest::TestIsNucleationPointCompute()
{    
    Coordinate thisCoord;
    for (thisCoord.m_row = 0; 
         thisCoord.m_row < m_baseBoard.GetNRows(); 
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; 
             thisCoord.m_col < m_baseBoard.GetNColumns(); 
             thisCoord.m_col++)
        {
            // player1's nk points: (0, 2) (0, 7) (3, 0) (3, 5) (4, 6) (6, 6)
            if (thisCoord != Coordinate(0, 2) &&
                thisCoord != Coordinate(0, 7) &&
                thisCoord != Coordinate(3, 0) &&
                thisCoord != Coordinate(3, 5) &&
                thisCoord != Coordinate(4, 6) &&
                thisCoord != Coordinate(6, 6) )
            {
                assert(rules::IsNucleationPointCompute(
                            m_baseBoard,
                            m_player1,
                            thisCoord) == false);
            }
            else
            {
                assert(rules::IsNucleationPointCompute(
                            m_baseBoard,
                            m_player1,
                            thisCoord) == true);
            }
            
            // player2's nk points: (0, 9) (2, 12) (3, 4) (4, 12) (7, 9) (7, 11)
            if (thisCoord != Coordinate(0, 9)  &&
                thisCoord != Coordinate(2, 12) &&
                thisCoord != Coordinate(3, 4)  &&
                thisCoord != Coordinate(4, 12) &&
                thisCoord != Coordinate(7, 9)  &&
                thisCoord != Coordinate(7, 11) )
            {
                assert(rules::IsNucleationPointCompute(
                            m_baseBoard,
                            m_player2,
                            thisCoord) == false);
            }
            else
            {
                assert(rules::IsNucleationPointCompute(
                            m_baseBoard,
                            m_player2,
                            thisCoord) == true);
            }
            
            // player3's nk points: (7, 7) (7, 9) (8, 10) (9, 11) (10, 7) (10, 13) (11, 8) (12, 10)
            if (thisCoord != Coordinate(7, 7)   &&
                thisCoord != Coordinate(7, 9)   &&
                thisCoord != Coordinate(8, 10)  &&
                thisCoord != Coordinate(9, 11)  &&
                thisCoord != Coordinate(10, 7)  &&
                thisCoord != Coordinate(10, 13) &&
                thisCoord != Coordinate(11, 8)  &&
                thisCoord != Coordinate(12, 10) )
            {
                assert(rules::IsNucleationPointCompute(
                            m_baseBoard,
                            m_player3,
                            thisCoord) == false);
            }
            else
            {
                assert(rules::IsNucleationPointCompute(
                            m_baseBoard,
                            m_player3,
                            thisCoord) == true);
            }
            
            // player4's nk points: (5, 0) (7, 4) (9, 2)
            if (thisCoord != Coordinate(5, 0) &&
                thisCoord != Coordinate(7, 4) &&
                thisCoord != Coordinate(9, 2) )
            {
                assert(rules::IsNucleationPointCompute(
                            m_baseBoard,
                            m_player4,
                            thisCoord) == false);
            }
            else
            {
                assert(rules::IsNucleationPointCompute(
                            m_baseBoard,
                            m_player4,
                            thisCoord) == true);
            }
            
        } // for (thisCoord.m_col
    } // for (thisCoord.m_row
}

void RulesTest::TestIsPieceDeployableInCoord()
{
    // coordinate (0, 0) of a piece is ALWAYS taken. A piece that is deployable
    // in a specific coordinate using the function IsPieceDeployableCompute MUST
    // be deployable using IsPieceDeployableInCoord using as a_mustTouchCoord 
    // any coordinate that will be taken by the piece
    Coordinate thisCoord;
    for (thisCoord.m_row = 0; 
         thisCoord.m_row < m_baseBoard.GetNRows(); 
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; 
             thisCoord.m_col < m_baseBoard.GetNColumns(); 
             thisCoord.m_col++)
        {

            for (int32_t thisPiece = e_minimumPieceIndex;
                 thisPiece < e_numberOfPieces;
                 thisPiece++)
            {
                if (m_baseBoard.IsCoordEmpty(thisCoord) == false)
                {
                    continue;
                }
                
                if (m_player1.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                {
                    const std::list<PieceConfiguration>& precalcConfs = 
                        m_player1.m_pieces[thisPiece].GetPrecalculatedConfs();
                    std::list<PieceConfiguration>::const_iterator it;
                    for (it  = precalcConfs.begin();
                         it != precalcConfs.end();
                         it++)
                    {
                        bool deployableCompute = 
                            rules::IsPieceDeployableCompute(
                                m_baseBoard,
                                *it,
                                thisCoord,
                                m_player1);                      
                        
                        if (deployableCompute == true)
                        {
                            PieceConfigurationContainer_t::const_iterator squareIt;
                            for (squareIt  = it->m_pieceSquares.begin();
                                 squareIt != it->m_pieceSquares.end();
                                 squareIt++)
                            {
                                Coordinate deployCoord(
                                    thisCoord.m_row + squareIt->m_row,
                                    thisCoord.m_col + squareIt->m_col);
                                bool deployableInCoord =
                                    rules::IsPieceDeployableInCoord(
                                        m_baseBoard,
                                        *it,
                                        thisCoord,
                                        deployCoord,
                                        m_player1);     
                                    
                                assert(deployableCompute == deployableInCoord);
                            }
                        }
                    }
                } // if (m_player1.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                
                if (m_player2.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                {
                    const std::list<PieceConfiguration>& precalcConfs = 
                        m_player2.m_pieces[thisPiece].GetPrecalculatedConfs();
                    std::list<PieceConfiguration>::const_iterator it;
                    for (it  = precalcConfs.begin();
                         it != precalcConfs.end();
                         it++)
                    {
                        bool deployableCompute = 
                            rules::IsPieceDeployableCompute(
                                m_baseBoard,
                                *it,
                                thisCoord,
                                m_player2);
                        
                        if (deployableCompute == true)
                        {
                            PieceConfigurationContainer_t::const_iterator squareIt;
                            for (squareIt  = it->m_pieceSquares.begin();
                                 squareIt != it->m_pieceSquares.end();
                                 squareIt++)
                            {
                                Coordinate deployCoord(
                                    thisCoord.m_row + squareIt->m_row,
                                    thisCoord.m_col + squareIt->m_col);
                                bool deployableInCoord =
                                    rules::IsPieceDeployableInCoord(
                                        m_baseBoard,
                                        *it,
                                        thisCoord,
                                        deployCoord,
                                        m_player2);     
                                    
                                assert(deployableCompute == deployableInCoord);
                            }
                        }
                    }
                } // if (m_player2.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                
                if (m_player3.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                {
                    const std::list<PieceConfiguration>& precalcConfs = 
                        m_player3.m_pieces[thisPiece].GetPrecalculatedConfs();
                    std::list<PieceConfiguration>::const_iterator it;
                    for (it  = precalcConfs.begin();
                         it != precalcConfs.end();
                         it++)
                    {
                        bool deployableCompute = 
                            rules::IsPieceDeployableCompute(
                                m_baseBoard,
                                *it,
                                thisCoord,
                                m_player3);
                        
                                
                        if (deployableCompute == true)
                        {
                            PieceConfigurationContainer_t::const_iterator squareIt;
                            for (squareIt  = it->m_pieceSquares.begin();
                                 squareIt != it->m_pieceSquares.end();
                                 squareIt++)
                            {
                                Coordinate deployCoord(
                                    thisCoord.m_row + squareIt->m_row,
                                    thisCoord.m_col + squareIt->m_col);
                                bool deployableInCoord =
                                    rules::IsPieceDeployableInCoord(
                                        m_baseBoard,
                                        *it,
                                        thisCoord,
                                        deployCoord,
                                        m_player3);     
                                    
                                assert(deployableCompute == deployableInCoord);
                            }
                        }
                    }
                } // if (m_player3.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                
                if (m_player4.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                {
                    const std::list<PieceConfiguration>& precalcConfs = 
                        m_player4.m_pieces[thisPiece].GetPrecalculatedConfs();
                    std::list<PieceConfiguration>::const_iterator it;
                    for (it  = precalcConfs.begin();
                         it != precalcConfs.end();
                         it++)
                    {
                        bool deployableCompute = 
                            rules::IsPieceDeployableCompute(
                                m_baseBoard,
                                *it,
                                thisCoord,
                                m_player4);
                                
                        if (deployableCompute == true)
                        {
                            PieceConfigurationContainer_t::const_iterator squareIt;
                            for (squareIt  = it->m_pieceSquares.begin();
                                 squareIt != it->m_pieceSquares.end();
                                 squareIt++)
                            {
                                Coordinate deployCoord(
                                    thisCoord.m_row + squareIt->m_row,
                                    thisCoord.m_col + squareIt->m_col);
                                bool deployableInCoord =
                                    rules::IsPieceDeployableInCoord(
                                        m_baseBoard,
                                        *it,
                                        thisCoord,
                                        deployCoord,
                                        m_player4);     
                                    
                                assert(deployableCompute == deployableInCoord);
                            }
                        }
                    }
                } // if (m_player4.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
            } // for (int32t thisPiece = e_minimumPieceIndex;        
        } // for (thisCoord.m_col
    } // for (thisCoord.m_row
}

void RulesTest::TestIsPieceDeployableInStartingPoint()
{
    Board localBoard(14, 14);
    Player localPlayer(std::string("local player"), 'X', 14, 14, Coordinate(0, 0));
    
    // localBoard should be empty (it is emptied out at instantiation time)
    // a baby piece must be deployable in starting point at all coordinates 
    Coordinate thisCoord;
    for (thisCoord.m_row = 0; 
         thisCoord.m_row < m_baseBoard.GetNRows(); 
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; 
             thisCoord.m_col < m_baseBoard.GetNColumns(); 
             thisCoord.m_col++)
        {
            assert(rules::IsPieceDeployableInStartingPoint(
                    localBoard,
                    localPlayer.m_pieces[e_1Piece_BabyPiece].GetCurrentConfiguration(),
                    thisCoord,
                    thisCoord) == true);
                    
            for (int32_t thisPiece = e_minimumPieceIndex;
                 thisPiece < e_numberOfPieces;
                 thisPiece++)
            {
                const std::list<PieceConfiguration>& precalcConfs = 
                    localPlayer.m_pieces[thisPiece].GetPrecalculatedConfs();
                std::list<PieceConfiguration>::const_iterator it;
                for (it  = precalcConfs.begin();
                     it != precalcConfs.end();
                     it++)
                {
                    // manually check if the piece can be put down
                    PieceConfigurationContainer_t::const_iterator squareIt;
                    for (squareIt  = it->m_pieceSquares.begin();
                         squareIt != it->m_pieceSquares.end();
                         squareIt++)
                    {
                        Coordinate deployCoord(
                            thisCoord.m_row + squareIt->m_row,
                            thisCoord.m_col + squareIt->m_col);
                        if ( (deployCoord.m_row < 0) ||
                             (deployCoord.m_col < 0) ||
                             (deployCoord.m_row >= localBoard.GetNRows()) ||
                             (deployCoord.m_col >= localBoard.GetNColumns()) )
                        {
                            break;
                        }
                    }
                    
                    bool outOfTheBoard = (squareIt != it->m_pieceSquares.end());
                    
                    // assert what is expected to happen
                    for (squareIt  = it->m_pieceSquares.begin();
                         squareIt != it->m_pieceSquares.end();
                         squareIt++)
                    {
                        Coordinate deployCoord(
                            thisCoord.m_row + squareIt->m_row,
                            thisCoord.m_col + squareIt->m_col);
                        
                        if (!outOfTheBoard)
                        {
                            // piece MUST be deployable in starting point
                            assert(rules::IsPieceDeployableInStartingPoint(
                                    localBoard,
                                    *it,
                                    thisCoord,
                                    deployCoord) == true);
                        }
                        else
                        {
                            // piece is NOT deployable in starting point
                            assert(rules::IsPieceDeployableInStartingPoint(
                                    localBoard,
                                    *it,
                                    thisCoord,
                                    deployCoord) == false);
                        }
                    }
                }
            }
        } // for (thisCoord.m_col
    } // for (thisCoord.m_row
}

void RulesTest::TestCalculateValidCoordsInNucleationPoint()
{
}

void RulesTest::TestCalculateNextValidCoordInNucleationPoint()
{
}

void RulesTest::TestHasValidCoordInNucleationPoint()
{
    // copy members
    Board localBoard(m_baseBoard);
    Player localPlayer1(m_player1);
    Player localPlayer2(m_player2);
    Player localPlayer3(m_player3);
    Player localPlayer4(m_player4);
    
    // retrieve all nucleation points from the board
    Coordinate thisCoord;
    STLCoordinateSet_t coordSet[4]; // one per player
    STLCoordinateSet_t::iterator nkCoordIt;
    for (thisCoord.m_row = 0; 
         thisCoord.m_row < localBoard.GetNRows(); 
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; 
             thisCoord.m_col < localBoard.GetNColumns(); 
             thisCoord.m_col++)
        {
            if (rules::IsNucleationPointCompute(
                        localBoard,
                        localPlayer1,
                        thisCoord))
            {
                localPlayer1.SetNucleationPoint(thisCoord);
                coordSet[0].insert(thisCoord);
            }
            else
            {
                localPlayer1.UnsetNucleationPoint(thisCoord);
            }
            
            if (rules::IsNucleationPointCompute(
                        localBoard,
                        localPlayer2,
                        thisCoord))
            {
                localPlayer2.SetNucleationPoint(thisCoord);
                coordSet[1].insert(thisCoord);
            }
            else
            {
                localPlayer2.UnsetNucleationPoint(thisCoord);
            }
            
            if (rules::IsNucleationPointCompute(
                        localBoard,
                        localPlayer3,
                        thisCoord))
            {
                localPlayer3.SetNucleationPoint(thisCoord);
                coordSet[2].insert(thisCoord);
            }
            else
            {
                localPlayer3.UnsetNucleationPoint(thisCoord);
            }
            
            if (rules::IsNucleationPointCompute(
                        localBoard,
                        localPlayer4,
                        thisCoord))
            {
                localPlayer4.SetNucleationPoint(thisCoord);
                coordSet[3].insert(thisCoord);
            }
            else
            {
                localPlayer4.UnsetNucleationPoint(thisCoord);
            }
        }
    }
    
    // keep going for the test
    for (thisCoord.m_row = 0; 
         thisCoord.m_row < localBoard.GetNRows(); 
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; 
             thisCoord.m_col < localBoard.GetNColumns(); 
             thisCoord.m_col++)
        {

            for (int32_t thisPiece = e_minimumPieceIndex;
                 thisPiece < e_numberOfPieces;
                 thisPiece++)
            {
                if (localBoard.IsCoordEmpty(thisCoord) == false)
                {
                    continue;
                }
                
                if (localPlayer1.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                {
                    const std::list<PieceConfiguration>& precalcConfs = 
                        localPlayer1.m_pieces[thisPiece].GetPrecalculatedConfs();
                    std::list<PieceConfiguration>::const_iterator it;
                    for (it  = precalcConfs.begin();
                         it != precalcConfs.end();
                         it++)
                    {
                        bool deployableCompute = 
                            rules::IsPieceDeployableCompute(
                                localBoard,
                                *it,
                                thisCoord,
                                localPlayer1);
                                
                        if (deployableCompute == true)
                        {
                            // must be deployable in at least one nk point
                            bool deployableInANKPoint = false;
                            for (nkCoordIt  = coordSet[0].begin();
                                 nkCoordIt != coordSet[0].end();
                                 nkCoordIt++)
                            {
                                if (deployableInANKPoint == false)
                                {
                                    deployableInANKPoint = 
                                        rules::HasValidCoordInNucleationPoint(
                                                localBoard,
                                                localPlayer1,
                                                *nkCoordIt,
                                                *it,
                                                localPlayer1.m_pieces[thisPiece].GetRadius());
                                }
                            }                            
                            assert(deployableInANKPoint == true);                                
                        }
                    } // for (it  = precalcConfs.begin();
                } // if (localPlayer1.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                
                if (localPlayer2.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                {
                    const std::list<PieceConfiguration>& precalcConfs = 
                        localPlayer2.m_pieces[thisPiece].GetPrecalculatedConfs();
                    std::list<PieceConfiguration>::const_iterator it;
                    for (it  = precalcConfs.begin();
                         it != precalcConfs.end();
                         it++)
                    {
                        bool deployableCompute = 
                            rules::IsPieceDeployableCompute(
                                localBoard,
                                *it,
                                thisCoord,
                                localPlayer2);
                                
                        if (deployableCompute == true)
                        {
                            // must be deployable in at least one nk point
                            bool deployableInANKPoint = false;
                            for (nkCoordIt  = coordSet[1].begin();
                                 nkCoordIt != coordSet[1].end();
                                 nkCoordIt++)
                            {
                                if (deployableInANKPoint == false)
                                {
                                    deployableInANKPoint = 
                                        rules::HasValidCoordInNucleationPoint(
                                                localBoard,
                                                localPlayer2,
                                                *nkCoordIt,
                                                *it,
                                                localPlayer2.m_pieces[thisPiece].GetRadius());
                                }
                            }                            
                            assert(deployableInANKPoint == true);                                
                        }
                    } // for (it  = precalcConfs.begin();
                } // if (localPlayer2.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                
                if (localPlayer3.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                {
                    const std::list<PieceConfiguration>& precalcConfs = 
                        localPlayer3.m_pieces[thisPiece].GetPrecalculatedConfs();
                    std::list<PieceConfiguration>::const_iterator it;
                    for (it  = precalcConfs.begin();
                         it != precalcConfs.end();
                         it++)
                    {
                        bool deployableCompute = 
                            rules::IsPieceDeployableCompute(
                                localBoard,
                                *it,
                                thisCoord,
                                localPlayer3);
                                
                        if (deployableCompute == true)
                        {
                            // must be deployable in at least one nk point
                            bool deployableInANKPoint = false;
                            for (nkCoordIt  = coordSet[2].begin();
                                 nkCoordIt != coordSet[2].end();
                                 nkCoordIt++)
                            {
                                if (deployableInANKPoint == false)
                                {
                                    deployableInANKPoint = 
                                        rules::HasValidCoordInNucleationPoint(
                                                localBoard,
                                                localPlayer3,
                                                *nkCoordIt,
                                                *it,
                                                localPlayer3.m_pieces[thisPiece].GetRadius());
                                }
                            }                            
                            assert(deployableInANKPoint == true);                                
                        }
                    } // for (it  = precalcConfs.begin();
                } // if (localPlayer3.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                
                if (localPlayer4.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
                {
                    const std::list<PieceConfiguration>& precalcConfs = 
                        localPlayer4.m_pieces[thisPiece].GetPrecalculatedConfs();
                    std::list<PieceConfiguration>::const_iterator it;
                    for (it  = precalcConfs.begin();
                         it != precalcConfs.end();
                         it++)
                    {
                        bool deployableCompute = 
                            rules::IsPieceDeployableCompute(
                                localBoard,
                                *it,
                                thisCoord,
                                localPlayer4);
                                
                        if (deployableCompute == true)
                        {
                            // must be deployable in at least one nk point
                            bool deployableInANKPoint = false;
                            for (nkCoordIt  = coordSet[3].begin();
                                 nkCoordIt != coordSet[3].end();
                                 nkCoordIt++)
                            {
                                if (deployableInANKPoint == false)
                                {
                                    deployableInANKPoint = 
                                        rules::HasValidCoordInNucleationPoint(
                                                localBoard,
                                                localPlayer4,
                                                *nkCoordIt,
                                                *it,
                                                localPlayer4.m_pieces[thisPiece].GetRadius());
                                }
                            }                            
                            assert(deployableInANKPoint == true);                                
                        }
                    } // for (it  = precalcConfs.begin();
                } // if (localPlayer4.IsPieceAvailable(static_cast<ePieceType_t>(thisPiece)))
            } // for (int32t thisPiece = e_minimumPieceIndex;        
        } // for (thisCoord.m_col
    } // for (thisCoord.m_row
}

void RulesTest::TestCalculateValidCoordsInStartingPoint()
{
}

void RulesTest::TestRecalculateNKAroundCoord()
{
}

void RulesTest::TestRecalculateInfluenceAreaAroundPieceFast()
{
}

void RulesTest::TestIsCoordInfluencedByPlayerFastCompute()
{
}

void RulesTest::TestRecalculateInfluenceAreaInBoard()
{
}

void RulesTest::TestCanPlayerGo()
{
    // copy members
    Board copiedBoard(m_baseBoard);
    Player localPlayer1(m_player1);
    Player localPlayer2(m_player2);
    Player localPlayer3(m_player3);
    Player localPlayer4(m_player4);
    
    // retrieve all nucleation points from the board
    Coordinate thisCoord;
    for (thisCoord.m_row = 0; 
         thisCoord.m_row < copiedBoard.GetNRows(); 
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; 
             thisCoord.m_col < copiedBoard.GetNColumns(); 
             thisCoord.m_col++)
        {
            if (rules::IsNucleationPointCompute(
                        copiedBoard,
                        localPlayer1,
                        thisCoord))
            {
                localPlayer1.SetNucleationPoint(thisCoord);
            }
            else
            {
                localPlayer1.UnsetNucleationPoint(thisCoord);
            }
            
            if (rules::IsNucleationPointCompute(
                        copiedBoard,
                        localPlayer2,
                        thisCoord))
            {
                localPlayer2.SetNucleationPoint(thisCoord);
            }
            else
            {
                localPlayer2.UnsetNucleationPoint(thisCoord);
            }
            
            if (rules::IsNucleationPointCompute(
                        copiedBoard,
                        localPlayer3,
                        thisCoord))
            {
                localPlayer3.SetNucleationPoint(thisCoord);
            }
            else
            {
                localPlayer3.UnsetNucleationPoint(thisCoord);
            }
            
            if (rules::IsNucleationPointCompute(
                        copiedBoard,
                        localPlayer4,
                        thisCoord))
            {
                localPlayer4.SetNucleationPoint(thisCoord);
            }
            else
            {
                localPlayer4.UnsetNucleationPoint(thisCoord);
            }
        }
    }

    // all players should be able to put down a piece
    assert(rules::CanPlayerGo(copiedBoard, localPlayer1));
    assert(rules::CanPlayerGo(copiedBoard, localPlayer2));
    assert(rules::CanPlayerGo(copiedBoard, localPlayer3));
    assert(rules::CanPlayerGo(copiedBoard, localPlayer4));
    
    // a brand new player should be able to put down a piece on an empty board
    Board localBoard(14, 14);
    Player localPlayer(std::string("local player"), 'X', 15, 15, Coordinate(7, 7));
    assert(rules::CanPlayerGo(localBoard, localPlayer));
    
    // a player without pieces available can't go
    for (int32_t thisPiece = e_minimumPieceIndex;
         thisPiece < e_numberOfPieces;
         thisPiece++)
    {
        localPlayer.UnsetPiece(static_cast<ePieceType_t>(thisPiece));
    }
    assert(rules::CanPlayerGo(localBoard, localPlayer) == false);
    
    // give localPlayer his/her pieces back
    localPlayer.Reset(Coordinate(7, 7));
    // fill the board 
    for (thisCoord.m_row = 0; 
         thisCoord.m_row < localBoard.GetNRows(); 
         thisCoord.m_row++)
    {
        for (thisCoord.m_col = 0; 
             thisCoord.m_col < localBoard.GetNColumns(); 
             thisCoord.m_col++)
        {
            localBoard.SetPlayerInCoord(thisCoord, localPlayer);
        }
    }
    // local player can't go
    assert(rules::CanPlayerGo(localBoard, localPlayer) == false);
}
