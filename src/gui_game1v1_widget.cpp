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
/// @file gui_game1v1_widget.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 05-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#include <cstdio>    // printf/snprintf (needed for better i18n)
#include <iomanip>   // setw
#endif

#include "gettext.h" // i18n
#include "gui_game1v1_widget.h"
#include "gui_game1v1_config.h"
#include "gui_glade_defs.h"

/// maximum size of the string to notify the end of the game
static const uint32_t GAME_FINISHED_BUFFER_LENGTH = 256;


/// static methods to communicate Game1v1 progress with Game1v1Widget
/// this float is used to communicate the worker thread with the main thread
/// in a thread-safe manner. It's a bit dirty but it will do it for now
static float s_computingCurrentProgress = 0.0;
// http://tadeboro.blogspot.com/2009/06/multi-threaded-gtk-applications.html
/// static easy to use mutex for shared data across threads
G_LOCK_DEFINE_STATIC(s_computingCurrentProgress);

/// this is a pointer to the Game1v1Widget itself so it can be used from the
/// static method/ Game1v1Widget::ProgressUpdate. It is dirty, but it works
/// (and it is enough for now)
/// WARNING: it won't work if Game1v1Widget is instantiated twice
static Game1v1Widget *g_pGame1v1Widget = NULL;

void Game1v1Widget::ProgressUpdate(float a_progress)
{
    // WARNING: this method is run by another thread.
    // once the m_signal_moveComputed signal is emited the
    // main thread will update the GUI widgets

    G_LOCK(s_computingCurrentProgress);
        s_computingCurrentProgress = a_progress;
    G_UNLOCK(s_computingCurrentProgress);

    if (g_pGame1v1Widget)
    {
        g_pGame1v1Widget->m_signal_computingProgressUpdated.emit();
    }
}

Game1v1Widget::Game1v1Widget():
    Gtk::VBox(), //Calls the base class constructor
    m_currentGameFinished(false),
    m_currentMovingPlayer(Game1v1::e_Game1v1Player1),
    m_the1v1Game(
        Game1v1Config::Instance().GetPlayer1StartingCoord(),
        Game1v1Config::Instance().GetPlayer2StartingCoord()),
    m_lastCoord(COORD_UNINITIALISED, COORD_UNINITIALISED),
    m_workerThread(),
    m_pickPiecesDrawingArea(
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1),
        DrawingAreaShowPieces::eOrientation_leftToRight),
    m_showOpponentPiecesDrawingArea(
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2),
        DrawingAreaShowPieces::eOrientation_bottomToTop),
    m_boardDrawingArea(m_the1v1Game.GetBoard()),
    m_editPieceTable(),
    m_statusBar(2, true) // 2 players, include progress bar
{
    //TODO this is dirty (even though it works) the way Game1v1Widget::ProgressUpdate
    // access the Game1v1Widget Instance should be fixed in some way
#ifdef DEBUG
    // assert there's no more than 1 instance of Game1v1Widget
    assert(g_pGame1v1Widget == NULL);
#endif
    g_pGame1v1Widget = this;

    // create and initialise the randomizer using now time as the seed
    GTimeVal timeNow;
    g_get_current_time(&timeNow);
    m_randomizer = g_rand_new_with_seed(static_cast<guint32>(timeNow.tv_sec ^ timeNow.tv_usec));

    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();

    // retrieve the default colour from the config class to apply it to the players
    uint8_t red, green, blue;
    Game1v1Config::Instance().GetPlayer1Colour(red, green, blue);
    m_the1v1Game.SetPlayerColour(Game1v1::e_Game1v1Player1, red, green, blue);
    Game1v1Config::Instance().GetPlayer2Colour(red, green, blue);
    m_the1v1Game.SetPlayerColour(Game1v1::e_Game1v1Player2, red, green, blue);

    // configure the widgets
    m_hBoxEditPieces.set_spacing(10);
    m_hBoxEditPieces.set_size_request(-1, 200);
    m_hBoxGameStatus.set_spacing(1);
    m_hBoxGameStatus.set_homogeneous(false);
    m_hBoxOpponentPieces.set_size_request(100, -1);
    m_hBoxOpponentPieces.set_homogeneous(false);

    // pack_start (Widget& child, bool expand, bool fill, guint padding=0)
    m_hBoxGameStatus.pack_start(m_boardDrawingArea, true, true);     // expand and fill
    m_hBoxGameStatus.pack_start(m_hBoxOpponentPieces, false, true);  // don't expand but fill
    m_hBoxOpponentPieces.pack_start(m_opponentSeparator, false, true, 5); // don't expand but fill
    m_hBoxOpponentPieces.pack_start(m_showOpponentPiecesDrawingArea, true, true); // expand and fill
    // pick pieces drawing area, edit pieces table and show opponent's pieces
    m_hBoxEditPieces.pack_start(m_pickPiecesDrawingArea, true, true);
    m_hBoxEditPieces.pack_start(m_editPieceTable, false, false);

    this->pack_start(m_hBoxGameStatus, true, true);  // expand and fill
    this->pack_start(m_hBoxEditPieces, false, true); // not expand but fill
    this->pack_start(m_statusBar, false, true);      // not expand but fill

    // update the score shown in the status bar before setting them up as visible
    // so the widgets take their proper size at GUI startup
    m_statusBar.SetScoreStatus(1, m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    m_statusBar.SetScoreStatus(2, m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2));

    // stopwatch must be initialised also
    m_statusBar.SetStopwatchPrefix(1, m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    m_statusBar.SetStopwatchPrefix(2, m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2));

    // progress handler for the computing process of the MinMax algorithm
    m_the1v1Game.SetProgressHandler(&Game1v1Widget::ProgressUpdate);

    // connect the interthread communication (GLib::Dispatcher) to invalidate the
    // board drawing area
    m_signal_moveComputed.connect(
            sigc::mem_fun(*this, &Game1v1Widget::NotifyMoveComputed));

    // connect the interthread communication to update the progress bar
    m_signal_computingProgressUpdated.connect(
            sigc::mem_fun(*this, &Game1v1Widget::NotifyProgressUpdate));

    // connect the worker thread signal
    m_workerThread.signal_computingFinished().connect(
            sigc::mem_fun(*this, &Game1v1Widget::WorkerThread_computingFinished));
            //sigc::ptr_fun(f) );

    // connect the signal coming from the pickPiecesDrawingArea to update TableEditPiece
    m_pickPiecesDrawingArea.signal_piecePicked().connect(
            sigc::mem_fun(m_editPieceTable, &TableEditPiece::SetPiece));

    // connect the signal coming from the editing piece table to process the change in the
    // currently editing piece
    m_editPieceTable.signal_pieceChanged().connect(
            sigc::mem_fun(m_boardDrawingArea, &DrawingAreaBoard::SetCurrentPiece));

    // connect the signal coming fromt he board drawing area to process when the user clicks
    // on the board
    m_boardDrawingArea.signal_boardPicked().connect(
            sigc::mem_fun(*this, &Game1v1Widget::BoardDrawingArea_BoardClicked));


    // initialise the list of players of the board drawing area
    m_boardDrawingArea.AddPlayerToList(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    m_boardDrawingArea.AddPlayerToList(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2));

    // launch the game!!
    LaunchNewGame();
}

Game1v1Widget::~Game1v1Widget()
{
    //TODO this is dirty too. See comment at the beginning of the constructor
    // it might cause a lot of trouble if there are more than 1 instance of
    // Game1v1Widget
    g_pGame1v1Widget = NULL;

    // delete the worker thread
    m_workerThread.Join();

    // delete the randomizer too
    g_rand_free(m_randomizer);
}

void Game1v1Widget::CancelComputing()
{
    // cancel the worker thread
    m_workerThread.CancelComputing();
}

bool Game1v1Widget::IsComputingMove()
{
    return m_workerThread.IsThreadComputingMove();
}

DrawingAreaBoard& Game1v1Widget::BoardDrawingArea()
{
    return m_boardDrawingArea;
}

void Game1v1Widget::ShowInfluenceAreaInBoard(Game1v1::eGame1v1Player_t a_game1v1Player)
{
    switch (a_game1v1Player)
    {
    case Game1v1::e_Game1v1Player1: // let it fall down
    case Game1v1::e_Game1v1Player2:
        m_boardDrawingArea.ShowPlayerInfluenceArea(
            m_the1v1Game.GetPlayer(a_game1v1Player));
        break;

    case Game1v1::e_Game1v1NoPlayer:
        m_boardDrawingArea.HidePlayerInfluenceArea();
        break;
#ifdef DEBUG
    default:
        assert(0);
#endif
    }
}

void Game1v1Widget::ShowForbiddenAreaInBoard(Game1v1::eGame1v1Player_t a_game1v1Player)
{
    switch (a_game1v1Player)
    {
    case Game1v1::e_Game1v1Player1: // let it fall down
    case Game1v1::e_Game1v1Player2:
        m_boardDrawingArea.ShowPlayerForbiddenArea(
            m_the1v1Game.GetPlayer(a_game1v1Player));
        break;

    case Game1v1::e_Game1v1NoPlayer:
        m_boardDrawingArea.HidePlayerForbiddenArea();
        break;
#ifdef DEBUG
    default:
        assert(0);
#endif
    }
}

void Game1v1Widget::LaunchNewGame()
{
    if (m_workerThread.IsThreadComputingMove())
    {
        // stop worker thread. Current calculation won't be needed in the new game
        // this call blocks the current thread until the worker thread is ready to
        // calculate next move
        m_workerThread.CancelComputing();
    }

    // clear old moves from the move queue. They belong to the old game
    while (!m_moveQueue.IsEmpty())
    {
        CalculatedMove_t dummy;
        m_moveQueue.Pop(dummy);
    }

    // reset the current game
    m_the1v1Game.Reset(
        Game1v1Config::Instance().GetPlayer1StartingCoord(),
        Game1v1Config::Instance().GetPlayer2StartingCoord());

    // update the board view
    m_boardDrawingArea.CancelLatestPieceDeployedEffect();
    m_boardDrawingArea.Invalidate();
    // player1 will be picking next piece
    m_pickPiecesDrawingArea.Invalidate(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    // player2's pieces will be shown on the right side
    m_showOpponentPiecesDrawingArea.Invalidate(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2));

    // it will be player1's go next. set piece colour to player1's
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
    m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1).GetColour(red, green, blue);
    m_editPieceTable.SetPieceRGB(
        static_cast<float>(red)   / 255,
        static_cast<float>(green) / 255,
        static_cast<float>(blue)  / 255);

    UpdateScoreStatus();

    // reset the cursor (even if it's been already done)
    ResetCursor();

    // stopwatches must be restarted.
    m_statusBar.ResetAllStopwatches();

    // restart the progress bar
    G_LOCK(s_computingCurrentProgress);
        s_computingCurrentProgress = 0.0;
    G_UNLOCK(s_computingCurrentProgress);
    m_statusBar.SetFraction(0.0);

    // Start player1's timer
    m_statusBar.ContinueStopwatch(1);

    // reset and force redraw editPieceTable. It'll be set to sensitive
    // or unsensitive depending on the type of player1
    m_editPieceTable.SetPiece(e_noPiece);

    // new game just started. It can't be finished!
    m_currentGameFinished = false;
    // player1 is the one who's got to put a piece next
    m_currentMovingPlayer = Game1v1::e_Game1v1Player1;

    if (Game1v1Config::Instance().IsPlayer1Computer())
    {
        // next player is the computer. Disallow editing the edit piece
        // area while computer is processing next move
        m_editPieceTable.set_sensitive(false);

        RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player1, false);
    }
    else
    {
        // a human being is allowed to edit next piece
        m_editPieceTable.set_sensitive(true);

        // player1 is a human and he/she will put down the next piece (player1 goes always first)
        m_boardDrawingArea.SetCurrentPlayer(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    }
}

bool Game1v1Widget::ProcessChangeInCurrentGame(Game1v1ConfigDialog& a_configDialog)
{
    bool currentProcessingCancelled = false;

    // if current player is a human being and it's been set to computer
    // next move will have to be requested to the worker thread.
    // if current player is the computer and its settings has been changed
    // current move will be cancelled and new settings will be applied
    // (both if player has been set up to be human or computer)

    if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1)     &&
           (Game1v1Config::Instance().IsPlayer1Computer() == false) &&
           (a_configDialog.IsPlayer1TypeComputer() == true)        )
         ||
         ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2)     &&
           (Game1v1Config::Instance().IsPlayer2Computer() == false) &&
           (a_configDialog.IsPlayer2TypeComputer() == true)        )
       )
    {
        // current player is not the computer and it has been
        // requested in the config dialog to change it to be the computer
        // save configuration shown by the dialog into global config singleton
        // and request the processing thread to compute next player1 move
        a_configDialog.SaveCurrentConfigIntoGlobalSettings();

        // next move was being calculated by a human being, but now it has
        // been swapped to a computer player. Humans are not allowed to edit
        // pieces while computer is thinking
        m_editPieceTable.set_sensitive(false);

        // requesting the thread!!
        RequestThreadToComputeNextMove(m_currentMovingPlayer, true);
    }
    else if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1)    &&
                (Game1v1Config::Instance().IsPlayer1Computer() == true) )
            ||
              ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2)    &&
                (Game1v1Config::Instance().IsPlayer2Computer() == true) )
            )
    {
        if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1) &&
               (a_configDialog.IsPlayer1TypeComputer() == false) )
             ||
             ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2) &&
               (a_configDialog.IsPlayer2TypeComputer() == false) ) )
        {
            // current moving player is the computer and it has been set to be a human user
            // cancel current calculation and launch the new move

            // current move is going to be cancelled
            currentProcessingCancelled = true;

            // cancel worker thread current computing process
            m_workerThread.CancelComputing();

            // empty out move queue
            while (!m_moveQueue.IsEmpty())
            {
                CalculatedMove_t dummy;
                m_moveQueue.Pop(dummy);
            }

            // save new settings into global config
            a_configDialog.SaveCurrentConfigIntoGlobalSettings();

            // restart the progress bar (computer is not thinking this move anymore)
            G_LOCK(s_computingCurrentProgress);
                s_computingCurrentProgress = 0.0;
            G_UNLOCK(s_computingCurrentProgress);
            m_statusBar.SetFraction(0.0);

            // allow the new human user to put down pieces on the board
            m_editPieceTable.set_sensitive(true);

            // restore the mouse cursor so the human being who has to put down next piece
            // can do it
            ResetCursor();
        } // if (a_configDialog.IsPlayer1TypeComputer() == false)
        else if (
              ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1)      &&
                ( (Game1v1Config::Instance().GetMinimaxDepthPlayer1() !=
                      a_configDialog.GetPlayer1SearchTreeDepth()        )
                  ||
                  (Game1v1Config::Instance().GetHeuristicTypePlayer1() !=
                      a_configDialog.GetPlayer1Heuristic()              ) ) )
              ||
              ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2)      &&
                ( (Game1v1Config::Instance().GetMinimaxDepthPlayer2() !=
                       a_configDialog.GetPlayer2SearchTreeDepth()        )
                   ||
                   (Game1v1Config::Instance().GetHeuristicTypePlayer2() !=
                       a_configDialog.GetPlayer2Heuristic()              ) ) ) )
        {
            // current player was the computer and it still is.
            // cancel current move process if there has been a change
            // in any of the rest of the settings (all but the
            // type of player: type of heuristic, depth of
            // search tree)

            // current move is going to be cancelled
            currentProcessingCancelled = true;

            // cancel worker thread current computing process
            m_workerThread.CancelComputing();

            // empty out move queue
            while (!m_moveQueue.IsEmpty())
            {
                CalculatedMove_t dummy;
                m_moveQueue.Pop(dummy);
            }

            // restart the progress bar (computer is not thinking this move anymore)
            G_LOCK(s_computingCurrentProgress);
                s_computingCurrentProgress = 0.0;
            G_UNLOCK(s_computingCurrentProgress);
            m_statusBar.SetFraction(0.0);

            // save current config for it to be applied in the future
            a_configDialog.SaveCurrentConfigIntoGlobalSettings();

            // requesting the thread!!
            RequestThreadToComputeNextMove(m_currentMovingPlayer, true);
        }
        else
        {
            // current player settings weren't modified. Save them
            // but do not cancel current calculation
            a_configDialog.SaveCurrentConfigIntoGlobalSettings();
        }
    }
    else
    {
        // changes to the config dialog don't affect current move
        // save configuration shown by the dialog into global config singleton
        // new configuratio will be applied once current move is calculated
        a_configDialog.SaveCurrentConfigIntoGlobalSettings();
    }

    return currentProcessingCancelled;
}

void Game1v1Widget::RequestThreadToComputeNextMove(
        Game1v1::eGame1v1Player_t a_whoMoves,
        bool                      a_blockCall,
        const Coordinate         &a_coordinate,
        const Piece              &a_piece)
{
    // set the cursor to busy in the board drawing area, so the user
    // won't get (very) nervous while the computer is processing the next move
    SetWaitCursor();

    // retrieve eval function pointer and depth from global configuration
    Heuristic::eHeuristicType_t heuristicType = Heuristic::e_heuristicStartCount;
    int32_t searchTreeDepth = GAME1V1CONFIG_DEPTH_AUTOADJUST;
    switch(a_whoMoves)
    {
    case Game1v1::e_Game1v1Player1:
        heuristicType   = Game1v1Config::Instance().GetHeuristicTypePlayer1();
        searchTreeDepth = Game1v1Config::Instance().GetMinimaxDepthPlayer1();
        break;
    case Game1v1::e_Game1v1Player2:
        heuristicType   = Game1v1Config::Instance().GetHeuristicTypePlayer2();
        searchTreeDepth = Game1v1Config::Instance().GetMinimaxDepthPlayer2();
        break;
#ifdef DEBUG
    default:
        assert(0);
#endif // DEBUG
    } // switch(a_whoMoves)

    const Heuristic::sHeuristicData_t &heuristicData = Heuristic::m_heuristicData[heuristicType];
    const Player &currentPlayer = m_the1v1Game.GetPlayer(a_whoMoves);
    const Player &opponent = m_the1v1Game.GetOpponent(a_whoMoves);

    // search tree depth. If it is set to GAME1V1CONFIG_DEPTH_AUTOADJUST
    // its value will be set here to a sensible value
    if (searchTreeDepth == GAME1V1CONFIG_DEPTH_AUTOADJUST)
    {
        searchTreeDepth = 3;

        //TODO 14 is a magic number
        if ( (currentPlayer.NumberOfPiecesAvailable() < 14) &&
             (rules::CanPlayerGo(m_the1v1Game.GetBoard(), opponent) ) )
        {
            // if the opponent cannot go there is no point on setting the depth to 5.
            // 3 should be good enough in that case
            searchTreeDepth = 5;
        }
    }

    bool computeMoveSucceded;
    if ( (currentPlayer.NumberOfPiecesAvailable() == e_numberOfPieces) &&
         (g_rand_int_range(m_randomizer, 0, 2) == 0) )
    {
        // pass the move made by the opponent to the minmax algorithm half of the times
        // it will show a bit of randomness at the start to a potencial human user
        computeMoveSucceded = m_workerThread.ComputeMove(
                                    m_the1v1Game,
                                    a_whoMoves,
                                    heuristicData.m_evalFunction,
                                    searchTreeDepth,
                                    a_blockCall);
    }
    else
    {
        // use the latest coord and piece deployed by the opponent for half the starting moves
        // and for all the rest of the moves
        computeMoveSucceded = m_workerThread.ComputeMove(
                                    m_the1v1Game,
                                    a_whoMoves,
                                    heuristicData.m_evalFunction,
                                    searchTreeDepth,
                                    a_blockCall,
                                    a_coordinate,
                                    a_piece);
    }

    if (!computeMoveSucceded)
    {
#ifdef DEBUG_PRINT
        std::cout
            << "Error while telling the thread to start computing. Worker thread is busy"
            << std::endl;
#endif
        std::stringstream theMessage;
        // i18n TRANSLATORS: Please, leave <b> and </b> tags sorrounding the translation of "Fatal error"
        // i18n Thank you for contributing to this project
        theMessage << _("<b>Fatal Error:</b> Could not communicate with worker thread. Application will exit now!");

        // stop worker thread
        m_workerThread.CancelComputing();

        // Notify the fatal error. They can't be handled
        signal_fatalError().emit(theMessage.str());
    }
}

void Game1v1Widget::WorkerThread_computingFinished(
        const Piece              &a_piece,
        const Coordinate         &a_coord,
        Game1v1::eGame1v1Player_t a_playerToMove,
        int32_t                   a_returnValue)
{
    // WARNING: this method is run by another thread.
    // once the m_signal_moveComputed signal is emited the
    // main thread will update the GUI widgets
    CalculatedMove_t thisMove;
    thisMove.piece = a_piece;
    thisMove.coord = a_coord;
    thisMove.playerToMove = a_playerToMove;

    // if the queue is full this call might block, but there aren't that many moves really...
    m_moveQueue.Push(thisMove);

#ifdef DEBUG_PRINT
    if (a_piece.GetType() == e_noPiece)
    {
        std::cout << _("Computer can't move") << std::endl;
    }
#endif

    // this signal is being issued from the MainWindowWorkerThread
    // GTK is not thread safe, so every GUI function should be called from the same thread
    // we need inter-thread communication to force the main thread to invalidate the
    // board drawing area
    // have a look at:
    //     http://library.gnome.org/devel/glibmm/stable/thread_2dispatcher_8cc-example.html
    m_signal_moveComputed.emit();
}

void Game1v1Widget::BoardDrawingArea_BoardClicked(
    const Coordinate &a_coord,
    const Piece &a_piece,
    const Player &a_player)
{
	if (m_workerThread.IsThreadComputingMove())
	{
#ifdef DEBUG_PRINT
	    std::cout
            << "Worker thread is busy. Please be patient while it is calculating next move"
            << std::endl;
#endif
	    return;
	}

    if( ( (a_player.NumberOfPiecesAvailable() == e_numberOfPieces) &&
          (!rules::IsPieceDeployableInStartingPoint(
                    m_the1v1Game.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_player.GetStartingCoordinate())) ) ||
        ( (a_player.NumberOfPiecesAvailable() < e_numberOfPieces) &&
          (!rules::IsPieceDeployableCompute(
                    m_the1v1Game.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_player)) ) )
    {
#ifdef DEBUG_PRINT
        std::cout << "Cheeky you! Don't try to deploy a piece where it's not allowed to"
                  << std::endl;
#endif
        return;
    }

    // don't save the move directly in the board. Use the same interface as the worker thread
    // Game 1v1 board will be only be modified in NotifyMoveComputed
    CalculatedMove_t thisMove;
    thisMove.piece = a_piece;
    thisMove.coord = a_coord;
    thisMove.playerToMove =  m_the1v1Game.GetPlayerType(a_player);

    // main thread cannot get blocked, if it fails inserting the element in
    // the queue just do nothing
    bool result = m_moveQueue.TryPush(thisMove);

    if (result == false)
    {
#ifdef DEBUG_PRINT
        std::cout << "Human move could not be added to the queue" << std::endl;
#endif

        Gtk::Window* topLevelWindow = NULL;
        topLevelWindow = static_cast<Gtk::Window*>(this->get_toplevel());

        if (topLevelWindow)
        {
            Gtk::MessageDialog::MessageDialog errorMsg(
                    *topLevelWindow,
                    _("Internal Error: Move could not be processed. Please click on the board normally to try again"),
                    true,
                    Gtk::MESSAGE_ERROR,
                    Gtk::BUTTONS_OK,
                    true);

            if (errorMsg.run())
            {
                ; // the dialog has only 1 button
            }
        }
        else
        {
            // emit a fatal error message
            signal_fatalError().emit(
                std::string(_("Main Window could not be retrieved from a child GUI element")));
        }

        // main thread cannot get blocked. It failed inserting the element so
        // get back and do nothing (do not notify this move)
        return;
    }

    // this method is not expected to be run by another thread, but if both worker thread and human
    // user use the same interface, things are much easier to understand
    // so, there's no need to emit any signal (which adds a bit of overhead), but
    // interfaces must be followed, so NotifyMoveComputed is caled directly instead of emiting interthread
    // signals
    //m_signal_moveComputed.emit();
    NotifyMoveComputed();

    return;
}

void Game1v1Widget::NotifyMoveComputed()
{
    // after the loop on the locked queue these variables will contain
    // the latest piece and latest coord deployed
    Piece latestPiece(e_noPiece);
    Coordinate latestCoord;
    Game1v1::eGame1v1Player_t latestPlayerToMove = Game1v1::e_Game1v1Player1;

    CalculatedMove_t currentMove;
    bool moveQueueEmpty = true;
    while(m_moveQueue.TryPop(currentMove))
    {
        if (moveQueueEmpty == true)
        {
            moveQueueEmpty = false;
        }

        latestPiece        = currentMove.piece;
        latestCoord        = currentMove.coord;
        latestPlayerToMove = currentMove.playerToMove;

        if (latestPiece.GetType() != e_noPiece)
        {
            m_the1v1Game.PutDownPiece(
                    latestPiece,
                    latestCoord,
                    latestPlayerToMove);
        }
    } // while(m_moveQueue.TryPop(currentMove))

    if (moveQueueEmpty)
    {
        // there were no moves stored in the queue. Nothing can be done
        return;
    }

    // actions will depend on who put down the latest piece
    const Player &latestPlayer   = m_the1v1Game.GetPlayer(latestPlayerToMove);
    const Player &latestOpponent = m_the1v1Game.GetOpponent(latestPlayerToMove);

    // player who didn't put a piece on the board this time
    Game1v1::eGame1v1Player_t followingPlayer;

    switch (latestPlayerToMove)
    {
    case Game1v1::e_Game1v1Player1:
    {
        followingPlayer   = Game1v1::e_Game1v1Player2;
        break;
    }
    case Game1v1::e_Game1v1Player2:
    {
        followingPlayer   = Game1v1::e_Game1v1Player1;
        break;
    }

    case Game1v1::e_Game1v1NoPlayer: // let it fall down
    default:
    {
        // this is impossible. latestPlayerToMove should always be player1 or player2
        // return here avoids compiling warnings
#ifdef DEBUG
        assert(0);
#endif
        return;
    }
    } // switch (latestPlayerToMove)


    // invalidate the board drawing area to show the new moves
    // activating the latest piece deployed glowing effect
    m_boardDrawingArea.Invalidate(
            latestPiece,
            latestCoord,
            latestPlayer);

	UpdateScoreStatus();

    // there's a few things that have to be done if the opponent of the latest
    // player to put down a piece can move
    if ( rules::CanPlayerGo(m_the1v1Game.GetBoard(), latestOpponent) )
    {
        // next player to move is the opponent
        m_boardDrawingArea.SetCurrentPlayer(latestOpponent);

        // stop current player's stopwatch and start the opponent's
        m_statusBar.SwapStopwatches();

        G_LOCK(s_computingCurrentProgress);
            s_computingCurrentProgress = 0.0;
        G_UNLOCK(s_computingCurrentProgress);
        m_statusBar.SetFraction(0.0);

        // it will be platestOpponent's go next. set piece colour to latestOpponent's
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        latestOpponent.GetColour(red, green, blue);
        m_editPieceTable.SetPieceRGB(
            static_cast<float>(red)   / 255,
            static_cast<float>(green) / 255,
            static_cast<float>(blue)  / 255);

        // remove the actual piece being edited from the edit piece drawing area
        // and force the edit piece drawing area to be redraw
        m_editPieceTable.SetPiece(e_noPiece);

        // swap pieces being shown in pickPiecesDrawingArea and showOpponentPiecesDrawingArea
        // opponent will select pieces while currentPlayer's will be only shown
        m_pickPiecesDrawingArea.Invalidate(latestOpponent);
        m_showOpponentPiecesDrawingArea.Invalidate(latestPlayer);

        // current player to move will have to be updated
        m_currentMovingPlayer = followingPlayer;

        // --it checks if the next player to put down a piece is a computer--
        if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1) &&
                    Game1v1Config::Instance().IsPlayer1Computer() ) ||
             ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2) &&
                    Game1v1Config::Instance().IsPlayer2Computer() ) )
        {
            // next player is the computer. Disallow editing the edit piece
            // area while computer is processing next move
            m_editPieceTable.set_sensitive(false);

            // tell the worker thread to compute next move
            if (latestPlayerToMove == Game1v1::e_Game1v1Player1)
            {
                // block till the job is transfered to the worker thread
                RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player2, true, latestCoord, latestPiece);
            }
            else
            {
                // block till the job is transfered to the worker thread
                RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player1, true, latestCoord, latestPiece);
            }
        }
        else
        {
            // allow next player to edit the piece to put down on the board
            // 'cos it's a human being
            m_editPieceTable.set_sensitive(true);

            // restore the mouse cursor so the human being who has to put down next piece
            // can do it
            ResetCursor();
        }
    }
    else
    {
        // remove the actual piece being edited from the edit piece drawing area
        // and force the edit piece drawing area to be redraw
        m_editPieceTable.SetPiece(e_noPiece);

        // latestPlayer can put down more pieces. Game is still on
        m_pickPiecesDrawingArea.Invalidate();

        if ( !rules::CanPlayerGo(m_the1v1Game.GetBoard(), latestPlayer) )
        {
            // neither player1 nor player2 can move. Game is over
            // piece calculated by computer is noPiece which means, no move, or
            // human player can't put down any piece after the latest one
            GameFinished();
        }
        // else
        // game is still on. Current player can put down more pieces.
        // Do nothing while current player thinks of the next move
    }
}

void Game1v1Widget::GameFinished()
{
    // once this function is called, game is supposed to be finished
    if (m_currentGameFinished == true)
    {
        // game was already notified to be finished
        return;
    }
    m_currentGameFinished = true;

    // reset the cursor (even if it's been already done)
    ResetCursor();

    // stop stopwatches (even if it was already done)
    m_statusBar.StopAllStopwatches();

    // no player is expected to put down any piece on the board now
    m_boardDrawingArea.UnsetCurrentPlayer();

    // allow a possible human user to play with the edit pieces area
    m_editPieceTable.set_sensitive(true);

    // restart the progress bar
    G_LOCK(s_computingCurrentProgress);
        s_computingCurrentProgress = 0.0;
    G_UNLOCK(s_computingCurrentProgress);
    m_statusBar.SetFraction(0.0);

    int32_t squaresLeftPlayer1 = 0;
    int32_t squaresLeftPlayer2 = 0;
    const Player &player1 = m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1);
    const Player &player2 = m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2);
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
    }

    uint8_t red1, green1, blue1;
    uint8_t red2, green2, blue2;
    player1.GetColour(red1, green1, blue1);
    player2.GetColour(red2, green2, blue2);

    char theMessage[GAME_FINISHED_BUFFER_LENGTH];
    if (squaresLeftPlayer1 == squaresLeftPlayer2)
    {
        snprintf(theMessage,
                GAME_FINISHED_BUFFER_LENGTH,
                // i18n TRANSLATORS: Please, leave the translated word DRAW between the <b> and </b> tags
                // i18n Bear in mind the first ' color=\"#%02X%02X%02X\" ' will be replaced by player1's colour,
                // i18n and the first %s will be replaced by player1's name
                // i18n The 2nd ' color=\"#%02X%02X%02X\" ' and 2nd %s will be replaced by player2's colour and
                // i18n player2's name respectively.
                // i18n The final %d will be replaced by the final score
                // i18n The order of these tags must be maintained in the translation
                // i18n A typical complete message:
                // i18n That was a <b>DRAW</b>!\nBoth <span color="#012345">Eddie</span> and <span color="#678901">John</span> have <b>5</b> squares left
                // i18n Thank you for contributing to this project
                _("That was a <b>DRAW</b>!\nBoth <span color=\"#%02X%02X%02X\">%s</span> and <span color=\"#%02X%02X%02X\">%s</span> have <b>%d</b> squares left"),
                static_cast<int32_t>(red1),
                static_cast<int32_t>(green1),
                static_cast<int32_t>(blue1),
                player1.GetName().c_str(),
                static_cast<int32_t>(red2),
                static_cast<int32_t>(green2),
                static_cast<int32_t>(blue2),
                player2.GetName().c_str(),
                squaresLeftPlayer1);

        // for the record. This was the old way before i18n
        /*
        std::stringstream theMessage;
        theMessage
            << "That was a <b>DRAW</b>!"
            << std::endl
            << "    Both "
            << "<span color=\"#"
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(red1)
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(green1)
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(blue1)
            << "\">"
            << player1.GetName()
            << "</span>"
            << " and "
            << "<span color=\"#"
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(red2)
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(green2)
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(blue2)
            << "\">"
            << player2.GetName()
            << "</span>"
            << " have <b>"
            << std::setbase(10) << squaresLeftPlayer1
            << "</b> squares left";
        */
    }
    else //(squaresLeftPlayer1 != squaresLeftPlayer2)
    {
        snprintf(theMessage,
                GAME_FINISHED_BUFFER_LENGTH,
                // i18n TRANSLATORS: Please, respect the HTML formatting as much as possible
                // i18n Bear in mind that the 1st ' color=\"#%02X%02X%02X\" ' will be replaced by the winner's colour,
                // i18n and the first %s will be replaced by the winner's name
                // i18n The 2nd ' color=\"#%02X%02X%02X\" ' and 2nd %s will be replaced by player1's colour and
                // i18n player1's name respectively. the following %d would be player1's score
                // i18n The 3rd ' color=\"#%02X%02X%02X\" ' and 3rd %s will be replaced by player2's colour and
                // i18n player2's name respectively. the following %d would be player2's score
                // i18n The order of these tags must be taken into account for the translation
                // i18n A typical complete message:
                // i18n <b><span color="#012345">Eddie</span></b>!\n<span color="#012345">Eddie</span> has <b>5</b> squares left and <span color="#678901">John</span> has <b>7</b>
                // i18n Thank you for contributing to this project
                _("<b><span color=\"#%02X%02X%02X\">%s</span></b> won!\n<span color=\"#%02X%02X%02X\">%s</span> has <b>%d</b> squares left and <span color=\"#%02X%02X%02X\">%s</span> has <b>%d</b>"),
                (squaresLeftPlayer1 < squaresLeftPlayer2) ? red1   : red2,
                (squaresLeftPlayer1 < squaresLeftPlayer2) ? green1 : green2,
                (squaresLeftPlayer1 < squaresLeftPlayer2) ? blue1  : blue2,
                (squaresLeftPlayer1 < squaresLeftPlayer2) ? player1.GetName().c_str() : player2.GetName().c_str(),
                red1,
                green1,
                blue1,
                player1.GetName().c_str(),
                squaresLeftPlayer1,
                red2,
                green2,
                blue2,
                player2.GetName().c_str(),
                squaresLeftPlayer2);
    }

    // notify this game is finished sending the final score message
    signal_gameFinished().emit(std::string(theMessage));
}

void Game1v1Widget::NotifyProgressUpdate()
{
    if (m_currentGameFinished)
    {
        // do not update the progress bar if the current game is finished
        return;
    }

    float progress;

    G_LOCK(s_computingCurrentProgress);
        progress = s_computingCurrentProgress;
    G_UNLOCK(s_computingCurrentProgress);

    m_statusBar.SetFraction(progress);
}

void Game1v1Widget::UpdateScoreStatus()
{
    m_statusBar.SetScoreStatus(1, m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    m_statusBar.SetScoreStatus(2, m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2));
}

void Game1v1Widget::SetWaitCursor()
{
    Glib::RefPtr<Gdk::Window> topLevelWindow;
    topLevelWindow = this->get_window();
    if (topLevelWindow)
    {
        // set the cursor to busy
        topLevelWindow->set_cursor(Gdk::Cursor(Gdk::WATCH));
    }
}
void Game1v1Widget::ResetCursor()
{
    Glib::RefPtr<Gdk::Window> topLevelWindow;
    topLevelWindow = this->get_window();
    if (topLevelWindow)
    {
        // set the cursor to default
        topLevelWindow->set_cursor();
    }
}

#ifdef WIN32
void Game1v1Widget::ForceTranslationOfWidgets()
{
    // in win32 systems gettext fails when the string is static and marked as
    // translatable with N_() but _() is never called explicitely. Basically
    // there are 2 kinds of strings that are not translated:
    //  + Those included in the GOptionEntry list, which show the available
    //    options that can be passed to the program through command line
    //  + Strings included in the .glade file that never change during the
    //    execution of the application, for example a menu called "Game", or a
    //    label that contains the word "rotate"
    //
    // We'll be calling here to _() for every string found in the .glade file
    // so it gets properly translated into the current domain (the 2nd case
    // described above)

}
#else
void Game1v1Widget::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
