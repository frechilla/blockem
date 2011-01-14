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
/// @file game_4players_widget.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 17-Nov-2010  Original development
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#include <cstdio>    // printf/snprintf (needed for better i18n)
#include <iomanip>   // setw
#endif

#include "gettext.h" // i18n
#include "gui/game_4players_widget.h"
#include "gui/game_4players_config.h"
#include "rules.h"

/// size of the winner message
static const uint32_t WINNER_MESSAGE_LENGTH = 128;
/// maximum size of the string to notify the end of the game
static const uint32_t GAME_FINISHED_BUFFER_LENGTH = 512;

Game4PlayersWidget::Game4PlayersWidget():
    Gtk::VBox(), //Calls the base class constructor
    m_currentGameFinished(false),
    m_currentMovingPlayer(Game4Players::e_Game4_Player1),
    m_the4PlayersGame(),
    m_boardDrawingArea(m_the4PlayersGame.GetBoard()),
    m_pickPiecesDrawingArea(     // player1 shown as current player
        m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1),
        DrawingAreaShowPieces::eOrientation_leftToRight),
    m_showPiecesDrawingAreaLeft( // player2 shown on the left
        m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2),
        DrawingAreaShowPieces::eOrientation_topToBottom),
    m_showPiecesDrawingAreaTop(  // player3 shown on top
        m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3),
        DrawingAreaShowPieces::eOrientation_rightToLeft),
    m_showPiecesDrawingAreaRight( // player4 shown on the right
        m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4),
        DrawingAreaShowPieces::eOrientation_bottomToTop),
    m_editPieceTable(),
    m_gameTable(2, 3),    // 2 rows, 3 columns
    m_statusBar(4, false) // 4 players. Without progress bar (for now)
{
    // build up the GUI!
    BuildGUI();

    // connect the signal coming fromt the board drawing area to process when
    //  the user clicks on the board
    m_boardDrawingArea.signal_boardPicked().connect(
            sigc::mem_fun(*this, &Game4PlayersWidget::BoardDrawingArea_BoardClicked));

    // initialise the list of players of the board drawing area
    m_boardDrawingArea.AddPlayerToList(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
    m_boardDrawingArea.AddPlayerToList(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
    m_boardDrawingArea.AddPlayerToList(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
    m_boardDrawingArea.AddPlayerToList(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));

    // player is a human and he/she will put down a piece
    m_boardDrawingArea.SetCurrentPlayer(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));

    // set piece colour to player1's in the edit piece area
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
    m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1).GetColour(red, green, blue);
    m_editPieceTable.SetPieceRGB(
        static_cast<float>(red)   / 255,
        static_cast<float>(green) / 255,
        static_cast<float>(blue)  / 255);

    // human beings are allowed to edit pieces
    m_editPieceTable.set_sensitive(true);

    // launch the game!!
    LaunchNewGame();
}

Game4PlayersWidget::~Game4PlayersWidget()
{
}

void Game4PlayersWidget::hide_all()
{
    // stop all glib timers before hiding the widget
    m_boardDrawingArea.CancelLatestPieceDeployedEffect();
    m_statusBar.StopAllStopwatches();

    // hide the widget
    Gtk::VBox::hide_all();
}

void Game4PlayersWidget::BuildGUI()
{
    // retrieve colours from 4players game config singleton
    uint8_t red, green, blue;

    Game4PlayersConfig::Instance().GetPlayerColour(
        Game4Players::e_Game4_Player1,
        red, green, blue);
    m_the4PlayersGame.SetPlayerColour(
        Game4Players::e_Game4_Player1,
        red, green, blue);

    Game4PlayersConfig::Instance().GetPlayerColour(
        Game4Players::e_Game4_Player2,
        red, green, blue);
    m_the4PlayersGame.SetPlayerColour(
        Game4Players::e_Game4_Player2,
        red, green, blue);

    Game4PlayersConfig::Instance().GetPlayerColour(
        Game4Players::e_Game4_Player3,
        red, green, blue);
    m_the4PlayersGame.SetPlayerColour(
        Game4Players::e_Game4_Player3,
        red, green, blue);

    Game4PlayersConfig::Instance().GetPlayerColour(
        Game4Players::e_Game4_Player4,
        red, green, blue);
    m_the4PlayersGame.SetPlayerColour(
        Game4Players::e_Game4_Player4,
        red, green, blue);


    // configure hbox edit pieces
    m_hBoxEditPieces.set_spacing(10);
    m_hBoxEditPieces.set_size_request(-1, 200);
    m_hBoxEditPieces.pack_start(m_pickPiecesDrawingArea, true, true);
    m_hBoxEditPieces.pack_start(m_editPieceTable, false, false);

    // configure drawing areas where the pieces of players waiting for
    // their turns are shown
    m_hBoxOpponentLeft.set_size_request(100, -1);
    m_hBoxOpponentLeft.set_homogeneous(false);
    m_hBoxOpponentLeft.pack_start(m_showPiecesDrawingAreaLeft, true, true, 5); // expand and fill

    m_hBoxOpponentRight.set_size_request(100, -1);
    m_hBoxOpponentRight.set_homogeneous(false);
    m_hBoxOpponentRight.pack_start(m_showPiecesDrawingAreaRight, true, true, 5); // expand and fill

    m_vBoxOpponentTop.set_size_request(-1, 100);
    m_vBoxOpponentTop.set_homogeneous(false);
    m_vBoxOpponentTop.pack_start(m_showPiecesDrawingAreaTop, true, true, 5); // expand and fill

    // configure table which has the board and the drawing areas where
    // the pieces of players waiting for their turns are shown
    // attach ( Widget& child,
    //          guint left_attach,
    //          guint right_attach,
    //          guint top_attach,
    //          guint bottom_attach,
    //          AttachOptions xoptions=FILL|EXPAND,
    //          AttachOptions yoptions=FILL|EXPAND,
    //          guint xpadding=0,
    //          guint ypadding=0)
    m_gameTable.attach(m_vBoxOpponentTop,   1, 2, 0, 1, Gtk::FILL, Gtk::FILL);
    m_gameTable.attach(m_hBoxOpponentLeft,  0, 1, 1, 2, Gtk::FILL, Gtk::FILL);
    m_gameTable.attach(m_boardDrawingArea,  1, 2, 1, 2);
    m_gameTable.attach(m_hBoxOpponentRight, 2, 3, 1, 2, Gtk::FILL, Gtk::FILL);

    // add widgets to this table
    // pack_start (Widget& child, bool expand, bool fill, guint padding=0)
    this->pack_start(m_gameTable, true, true);
    this->pack_start(m_hBoxEditPieces, false, true);
    this->pack_start(m_statusBar, false, true);

    // update the score shown in the status bar before setting them up as visible
    // so the widgets take their proper size at GUI startup
    m_statusBar.SetScoreStatus(1, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
    m_statusBar.SetScoreStatus(2, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
    m_statusBar.SetScoreStatus(3, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
    m_statusBar.SetScoreStatus(4, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));

    // stopwatches must be initialised also
    m_statusBar.SetStopwatchPrefix(1, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
    m_statusBar.SetStopwatchPrefix(2, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
    m_statusBar.SetStopwatchPrefix(3, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
    m_statusBar.SetStopwatchPrefix(4, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));

    // connect the signal coming from the pickPiecesDrawingArea to update TableEditPiece
    m_pickPiecesDrawingArea.signal_piecePicked().connect(
            sigc::mem_fun(m_editPieceTable, &TableEditPiece::SetPiece));

    // connect the signal coming from the editing piece table to process the change in the
    // currently editing piece
    m_editPieceTable.signal_pieceChanged().connect(
            sigc::mem_fun(m_boardDrawingArea, &DrawingAreaBoard::SetCurrentPiece));
}

DrawingAreaBoard& Game4PlayersWidget::BoardDrawingArea()
{
    return m_boardDrawingArea;
}

void Game4PlayersWidget::ShowInfluenceAreaInBoard(
    Game4Players::eGame4_Player_t a_playerType)
{
    switch (a_playerType)
    {
    case Game4Players::e_Game4_Player1: // let it fall down
    case Game4Players::e_Game4_Player2:
    case Game4Players::e_Game4_Player3:
    case Game4Players::e_Game4_Player4:
        m_boardDrawingArea.ShowPlayerInfluenceArea(
            m_the4PlayersGame.GetPlayer(a_playerType));
        break;

    case Game4Players::e_Game4_NoPlayer:
        m_boardDrawingArea.HidePlayerInfluenceArea();
        break;

#ifdef DEBUG
    default:
        assert(0);
#endif
    }
}

void Game4PlayersWidget::ShowForbiddenAreaInBoard(
    Game4Players::eGame4_Player_t a_playerType)
{
    switch (a_playerType)
    {
    case Game4Players::e_Game4_Player1: // let it fall down
    case Game4Players::e_Game4_Player2:
    case Game4Players::e_Game4_Player3:
    case Game4Players::e_Game4_Player4:
        m_boardDrawingArea.ShowPlayerForbiddenArea(
            m_the4PlayersGame.GetPlayer(a_playerType));
        break;

    case Game4Players::e_Game4_NoPlayer:
        m_boardDrawingArea.HidePlayerForbiddenArea();
        break;

#ifdef DEBUG
    default:
        assert(0);
#endif
    }
}

void Game4PlayersWidget::LaunchNewGame()
{
    m_the4PlayersGame.Reset(
        Game4PlayersConfig::Instance().GetNRows(),
        Game4PlayersConfig::Instance().GetNColumns(),
        Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player1),
        Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player2),
        Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player3),
        Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player4));

    // reset board drawing area settings
    m_boardDrawingArea.ResetBoard(m_the4PlayersGame.GetBoard());
    m_boardDrawingArea.ResetPlayerList();
    m_boardDrawingArea.AddPlayerToList(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
    m_boardDrawingArea.AddPlayerToList(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
    m_boardDrawingArea.AddPlayerToList(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
    m_boardDrawingArea.AddPlayerToList(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));
    m_boardDrawingArea.SetCurrentPlayer(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));

    // update the board view. Latest piece deployed effect has already
    // been cancelled
    m_boardDrawingArea.Invalidate();

    // player1 will be picking next piece
    m_pickPiecesDrawingArea.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
    // the rest of players' pieces will be shown in their corresponding widgets
    m_showPiecesDrawingAreaLeft.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));
    m_showPiecesDrawingAreaTop.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
    m_showPiecesDrawingAreaRight.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));

    // it will be player1's go next. set piece colour to player1's
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
    m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1).GetColour(red, green, blue);
    m_editPieceTable.SetPieceRGB(
        static_cast<float>(red)   / 255,
        static_cast<float>(green) / 255,
        static_cast<float>(blue)  / 255);

    // players have been reset, score will be reset too
    UpdateScoreStatus();

    // stopwatches must be restarted.
    m_statusBar.ResetAllStopwatches();

    // Start player's timer
    m_statusBar.ContinueStopwatch(1);

    // reset and force redraw editPieceTable. It'll be set to sensitive
    // or unsensitive depending on the type of player1
    m_editPieceTable.SetPiece(e_noPiece);

    // player1 is the one who's got to put a piece next
    m_currentMovingPlayer = Game4Players::e_Game4_Player1;

    // new game just started. It can't be finished!
    m_currentGameFinished = false;
}

void Game4PlayersWidget::BoardDrawingArea_BoardClicked(
    const Coordinate &a_coord,
    const Piece &a_piece,
    const Player &a_player)
{
    if (a_player.NumberOfPiecesAvailable() == e_numberOfPieces)
    {
        if ( ( (!a_player.GetStartingCoordinate().Initialised()) &&
               (!rules::IsPieceDeployableInStartingPoint(
                    m_the4PlayersGame.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_coord)) ) ||
             ( (a_player.GetStartingCoordinate().Initialised()) &&
               (!rules::IsPieceDeployableInStartingPoint(
                    m_the4PlayersGame.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_player.GetStartingCoordinate())) ) )
        {
#ifdef DEBUG_PRINT
            std::cout << "Starting coordinate deployment failure: Cheeky you! Don't try to deploy a piece where it's not allowed to"
                      << std::endl;
#endif
            return;
        }
    }
    else
    {
        if (!rules::IsPieceDeployableCompute(
                    m_the4PlayersGame.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_player))
        {
#ifdef DEBUG_PRINT
            std::cout << "Deployment failure: Cheeky you! Don't try to deploy a piece where it's not allowed to"
                      << std::endl;
#endif
            return;
        }
    }

    // get type of player who clicked on the board
    Game4Players::eGame4_Player_t thisPlayerType =
        m_the4PlayersGame.GetPlayerType(a_player);
#ifdef DEBUG
    assert(thisPlayerType == m_currentMovingPlayer);
#endif

    // put down current piece before anything else
    m_the4PlayersGame.PutDownPiece(
        a_piece,
        a_coord,
        thisPlayerType);

    // invalidate the board drawing area to show the new moves
    // activating the latest piece deployed glowing effect
    m_boardDrawingArea.Invalidate(a_piece, a_coord, a_player);

	// update score after this piece has been put down
    UpdateScoreStatus();

    // remove the actual piece being edited from the edit piece drawing area
    // and force the edit piece drawing area to be redraw
    m_editPieceTable.SetPiece(e_noPiece);

    // decide which player will move next.
    // Player 2 moves after 1, 3 after 2, 4 after 1 and finally, 1 after 4
    m_currentMovingPlayer = Game4Players::GetNextPlayerType(thisPlayerType);
    int32_t nPlayersChecked = 1;
    while ((nPlayersChecked <= Game4Players::e_Game4_PlayersCount) &&
           (rules::CanPlayerGo(
               m_the4PlayersGame.GetBoard(),
               m_the4PlayersGame.GetPlayer(m_currentMovingPlayer)) == false) )
    {
        m_currentMovingPlayer = Game4Players::GetNextPlayerType(m_currentMovingPlayer);
        nPlayersChecked++;
    }

    if (nPlayersChecked > Game4Players::e_Game4_PlayersCount)
    {
#ifdef DEBUG
        // we checked all players, m_currentMovingPlayer has been changed
        // 4 times and it came back to the original value
        assert(thisPlayerType == m_currentMovingPlayer);
#endif

        // game is finished. No player can put down a piece
        m_statusBar.StopAllStopwatches();

        // current moving player's pieces will be shown on the pick piece area
        // widget must be updated beforehand
        m_pickPiecesDrawingArea.Invalidate(m_the4PlayersGame.GetPlayer(thisPlayerType));

        // update also the rest of widgets which show pieces
        switch (thisPlayerType)
        {
        case Game4Players::e_Game4_Player1:
        {
            m_showPiecesDrawingAreaLeft.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));
            m_showPiecesDrawingAreaTop.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
            m_showPiecesDrawingAreaRight.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
            break;
        }
        case Game4Players::e_Game4_Player2:
        {
            m_showPiecesDrawingAreaLeft.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));
            m_showPiecesDrawingAreaTop.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
            m_showPiecesDrawingAreaRight.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
            break;
        }
        case Game4Players::e_Game4_Player3:
        {
            m_showPiecesDrawingAreaLeft.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));
            m_showPiecesDrawingAreaTop.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
            m_showPiecesDrawingAreaRight.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
            break;
        }
        case Game4Players::e_Game4_Player4:
        {
            m_showPiecesDrawingAreaLeft.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
            m_showPiecesDrawingAreaTop.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
            m_showPiecesDrawingAreaRight.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
            break;
        }
        default:
        {
            // WTF??
#ifdef DEBUG
            assert(0);
#endif
        }
        } // switch (thisPlayerType)

        // notify end of the game
        GameFinished();

        // no further processing
        return;
    }

    // notify the board widget who is the next player to move
    m_boardDrawingArea.SetCurrentPlayer(m_the4PlayersGame.GetPlayer(m_currentMovingPlayer));

    for (int32_t i = 0; i < nPlayersChecked; i++)
    {
        // stop current player's stopwatch and start the next one
        m_statusBar.SwapStopwatches();
    }

    // update edit piece's colour to next player's
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
    m_the4PlayersGame.GetPlayer(m_currentMovingPlayer).GetColour(red, green, blue);
    m_editPieceTable.SetPieceRGB(
        static_cast<float>(red)   / 255,
        static_cast<float>(green) / 255,
        static_cast<float>(blue)  / 255);

    // swap pieces being shown in pickPiecesDrawingArea
    // current moving player's pieces will be shown in there
    m_pickPiecesDrawingArea.Invalidate(m_the4PlayersGame.GetPlayer(m_currentMovingPlayer));

    // update the rest of widgets which show pieces too
    switch (m_currentMovingPlayer)
    {
    case Game4Players::e_Game4_Player1:
    {
        m_showPiecesDrawingAreaLeft.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));
        m_showPiecesDrawingAreaTop.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
        m_showPiecesDrawingAreaRight.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
        break;
    }
    case Game4Players::e_Game4_Player2:
    {
        m_showPiecesDrawingAreaLeft.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));
        m_showPiecesDrawingAreaTop.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
        m_showPiecesDrawingAreaRight.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
        break;
    }
    case Game4Players::e_Game4_Player3:
    {
        m_showPiecesDrawingAreaLeft.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));
        m_showPiecesDrawingAreaTop.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
        m_showPiecesDrawingAreaRight.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
        break;
    }
    case Game4Players::e_Game4_Player4:
    {
        m_showPiecesDrawingAreaLeft.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
        m_showPiecesDrawingAreaTop.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
        m_showPiecesDrawingAreaRight.Invalidate(m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
        break;
    }
    default:
    {
        // WTF??
#ifdef DEBUG
        assert(0);
#endif
    }
    } // switch (thisPlayerType)
}

void Game4PlayersWidget::GameFinished()
{
    // once this function is called, game is supposed to be finished
    if (m_currentGameFinished == true)
    {
        // game was already notified to be finished
        return;
    }
    m_currentGameFinished = true;

    // stop stopwatches (even if it was already done)
    m_statusBar.StopAllStopwatches();

    // no player is expected to put down any piece on the board now
    m_boardDrawingArea.UnsetCurrentPlayer();

    // allow a possible human user to play with the edit pieces area
    m_editPieceTable.set_sensitive(true);

    int32_t squaresLeftPlayer1 = 0;
    int32_t squaresLeftPlayer2 = 0;
    int32_t squaresLeftPlayer3 = 0;
    int32_t squaresLeftPlayer4 = 0;
    const Player &player1 = m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1);
    const Player &player2 = m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2);
    const Player &player3 = m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3);
    const Player &player4 = m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4);
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (player1.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer1 += player1.m_pieces[i].GetNSquares();
        }
        if (player2.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer2 += player2.m_pieces[i].GetNSquares();
        }
        if (player3.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer3 += player3.m_pieces[i].GetNSquares();
        }
        if (player4.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer4 += player4.m_pieces[i].GetNSquares();
        }
    }

    const Player* winner = NULL;
    if ( (squaresLeftPlayer1 < squaresLeftPlayer2) &&
         (squaresLeftPlayer1 < squaresLeftPlayer3) &&
         (squaresLeftPlayer1 < squaresLeftPlayer4) )
    {
        winner = &player1;
    }
    else if ( (squaresLeftPlayer2 < squaresLeftPlayer1) &&
              (squaresLeftPlayer2 < squaresLeftPlayer3) &&
              (squaresLeftPlayer2 < squaresLeftPlayer4) )

    {
        winner = &player2;
    }
    else if ( (squaresLeftPlayer3 < squaresLeftPlayer1) &&
              (squaresLeftPlayer3 < squaresLeftPlayer2) &&
              (squaresLeftPlayer3 < squaresLeftPlayer4) )

    {
        winner = &player3;
    }
    else if ( (squaresLeftPlayer4 < squaresLeftPlayer1) &&
              (squaresLeftPlayer4 < squaresLeftPlayer2) &&
              (squaresLeftPlayer4 < squaresLeftPlayer3) )

    {
        winner = &player4;
    }

    // it will contain the game finished message
    char theMessage[GAME_FINISHED_BUFFER_LENGTH];
    // winner message. It will be empty if there is no winner
    char winnerMessage[WINNER_MESSAGE_LENGTH];
    winnerMessage[0] = '\0'; // set winnerMessage to empty string
    if (winner != NULL)
    {
        uint8_t red, green, blue;
        winner->GetColour(red, green, blue);

        snprintf(winnerMessage,
                WINNER_MESSAGE_LENGTH,
                // i18n TRANSLATORS: Please, respect the HTML formatting as much as possible
                // i18n Bear in mind that ' color=\"#%02X%02X%02X\" ' will be replaced by the winner's colour,
                // i18n and the first %s will be replaced by the winner's name
                // i18n Please, leave the \n\n characters at the end of the string so this string
                // i18n can preceed the one which contains the final score
                // i18n Thank you for contributing to this project
                _("<b><span color=\"#%02X%02X%02X\">%s</span></b> won!\n\n"),
                red,
                green,
                blue,
                winner->GetName().c_str());
    }
    else
    {
        // find out who won. There must have been a draw, but who made it?

        // 1st of all calculate the winer score
        int32_t winnerScore;
        winnerScore = std::min(squaresLeftPlayer1, squaresLeftPlayer2);
        winnerScore = std::min(winnerScore, squaresLeftPlayer3);
        winnerScore = std::min(winnerScore, squaresLeftPlayer4);

        // find out whose score is equal to this minimum
        snprintf(winnerMessage,
                WINNER_MESSAGE_LENGTH,
                // i18n TRANSLATORS: Please, respect the HTML formatting as much as possible
                // i18n Bear in mind the final %d will be replaced by the winner score
                // i18n Thank you for contributing to this project
                _("It ended in a <b>draw</b>! Two or more player's score is %d\n\n"),
                winnerScore);
    }

    uint8_t red1, green1, blue1;
    uint8_t red2, green2, blue2;
    uint8_t red3, green3, blue3;
    uint8_t red4, green4, blue4;
    player1.GetColour(red1, green1, blue1);
    player2.GetColour(red2, green2, blue2);
    player3.GetColour(red3, green3, blue3);
    player4.GetColour(red4, green4, blue4);

    snprintf(theMessage,
            GAME_FINISHED_BUFFER_LENGTH,
            // i18n TRANSLATORS: Please, respect formatting as much as possible
            // i18n Each line contains the name of the player surrounded by
            // i18n <span> HTML tags to set up its colour plus the score
            // i18n Thank you for contributing to this project
            _("Final score: \n"
              " <b><span color=\"#%02X%02X%02X\">%s</span></b> %d |"
              " <b><span color=\"#%02X%02X%02X\">%s</span></b> %d |"
              " <b><span color=\"#%02X%02X%02X\">%s</span></b> %d |"
              " <b><span color=\"#%02X%02X%02X\">%s</span></b> %d"),
            red1, green1, blue1, player1.GetName().c_str(), squaresLeftPlayer1,
            red2, green2, blue2, player2.GetName().c_str(), squaresLeftPlayer2,
            red3, green3, blue3, player3.GetName().c_str(), squaresLeftPlayer3,
            red4, green4, blue4, player4.GetName().c_str(), squaresLeftPlayer4);


    // notify this game is finished sending the final score message
    signal_gameFinished().emit(std::string(winnerMessage) + std::string(theMessage));
}

void Game4PlayersWidget::UpdateScoreStatus()
{
    m_statusBar.SetScoreStatus(1, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player1));
    m_statusBar.SetScoreStatus(2, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player2));
    m_statusBar.SetScoreStatus(3, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player3));
    m_statusBar.SetScoreStatus(4, m_the4PlayersGame.GetPlayer(Game4Players::e_Game4_Player4));
}
