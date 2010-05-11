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
/// @file gui_main_window.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Nov-2009  Original development
///           Faustino Frechilla 01-Apr-2010  Add drawing board with computer's pieces left
///           Faustino Frechilla 06-Apr-2010  StopWatch for human and computer
///           Faustino Frechilla 25-Apr-2010  libglade dependency removed. Code migrated to GtkBuilder
///           Faustino Frechilla 27-Apr-2010  2 human players allowed. Computer can start from both player1 and 2
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif

#include <queue>   // queue computer's moves
#include <iomanip> // setw
#include "gui_main_window.h"
#include "gui_game1v1_config.h"
#include "gui_glade_defs.h"

/// message to be shown to the user when he/she requested the
/// application to be closed, and the worker thread is busy computing
static const char MESSAGE_ASK_BEFORE_CLOSE[] =
        "The game is computing the next move. Are you sure do you want to close the application?";

/// length of the custom messages to be shown to the user
static const int32_t MESSAGE_LENGTH = 100;

/// how often stopwatches are updated
static const uint32_t STOPWATCH_UPDATE_PERIOD_MILLIS = 500; // 1000 = 1 second


// contains the info that stores a move (piece + where + who)
typedef struct
{
    Piece               piece;
    Coordinate          coord;
    Game1v1::eGame1v1Player_t playerToMove;
} sCalculatedMove_t;

// this queue is used to communicate the worker thread with the main thread
// in a thread-safe manner. It's a bit dirty but it will do it for now
static std::queue< sCalculatedMove_t > s_computerMoveQueue = std::queue< sCalculatedMove_t >();
// static easy to use mutex for shared data across threads
// http://tadeboro.blogspot.com/2009/06/multi-threaded-gtk-applications.html
G_LOCK_DEFINE_STATIC(s_computerMoveQueue);

// static methods to communicate Game1v1 progress with MainWindow
static float s_computingCurrentProgress = 0.0;
G_LOCK_DEFINE_STATIC(s_computingCurrentProgress);

// this is a pointer to the MainWindow itself so it can be used from the static method
// MainWindow::ProgressUpdate. It is dirty, but it works (and it is enough for now)
// WARNING: it won't work if MainWindow is instantiated twice
static MainWindow *g_pMainWindow = NULL;

void MainWindow::ProgressUpdate(float a_progress)
{
    // WARNING: this method is run by another thread.
    // once the m_signal_moveComputed signal is emited the
    // main thread will update the GUI widgets

    G_LOCK(s_computingCurrentProgress);
        s_computingCurrentProgress = a_progress;
    G_UNLOCK(s_computingCurrentProgress);

    if (g_pMainWindow)
    {
        g_pMainWindow->m_signal_computingProgressUpdated.emit();
    }
}

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException):
    Gtk::Window(cobject), //Calls the base class constructor
    m_currentGameFinished(false),
    m_currentMovingPlayer(Game1v1::e_Game1v1Player1),
    m_gtkBuilder(a_gtkBuilder),
    m_the1v1Game(
        Game1v1Config::Instance().GetPlayer1StartingCoord(),
        Game1v1Config::Instance().GetPlayer2StartingCoord()),
    m_lastCoord(COORD_UNINITIALISED, COORD_UNINITIALISED),
    m_workerThread(),
    m_aboutDialog(NULL),
    m_pickPiecesDrawingArea(
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1),
        DrawingAreaShowPieces::eOrientation_leftToRight),
    m_showOpponentPiecesDrawingArea(
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2),
        DrawingAreaShowPieces::eOrientation_bottomToTop),
    m_boardDrawingArea(m_the1v1Game.GetBoard()),
    m_editPieceTable(NULL),
    m_stopWatchLabelPlayer1(
        STOPWATCH_UPDATE_PERIOD_MILLIS,
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1).GetName() + std::string(" elapsed time ")),
    m_stopWatchLabelPlayer2(
        STOPWATCH_UPDATE_PERIOD_MILLIS,
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2).GetName() + std::string(" elapsed time "))
{
    //TODO this is dirty (even though it works) the way MainWindow::ProgressUpdate
    // access the MainWindow Instance should be fixed in some way
#ifdef DEBUG
    // assert there's no more than 1 instance of MainWindow
    assert(g_pMainWindow == NULL);
#endif
    g_pMainWindow = this;

    // create and initialise the randomizer using now time as the seed
    GTimeVal timeNow;
    g_get_current_time(&timeNow);
    m_randomizer = g_rand_new_with_seed(static_cast<guint32>(timeNow.tv_sec ^ timeNow.tv_usec));

    // icon of the window
    Glib::RefPtr< Gdk::Pixbuf > icon;
    // gboolean g_file_test(const gchar *filename, GFileTest test);
    if (g_file_test(GUI_PATH_TO_16PICTURE, G_FILE_TEST_IS_REGULAR))
    {
        try
        {
            icon = Gdk::Pixbuf::create_from_file(GUI_PATH_TO_16PICTURE);
        } catch(...)
        {
            icon.reset();
#ifdef DEBUG_PRINT
            std::cerr
               << "WARNING: Exception occurred when setting the 16x16 icon into the Main Window from "
               << GUI_PATH_TO_16PICTURE
               << std::endl;
#endif
        }

        if (icon)
        {
            this->set_icon(icon);
        }
    }

    // retrieve the config dialog. It must be retrieved calling get_widget_derived
    // otherwise app will core
    m_gtkBuilder->get_widget_derived(GUI_CONFIG_DIALOG_NAME, m_configDialog);
    if (m_configDialog == NULL)
    {
        throw new GUIException(std::string("ConfigDialog retrieval failed"));
    }

    // retrieve the about dialog. It must be retrieved calling get_widget_derived
    // otherwise app will core
    m_gtkBuilder->get_widget_derived(GUI_ABOUT_DIALOG_NAME, m_aboutDialog);
    if (m_aboutDialog == NULL)
    {
        throw new GUIException(std::string("AboutDialog retrieval failed"));
    }

    // retrieve the editing piece table. It must be retrieved calling get_widget_derived
    // otherwise app will core
    m_gtkBuilder->get_widget_derived(GUI_TABLE_EDITING_PIECE_NAME, m_editPieceTable);
    if (m_editPieceTable == NULL)
    {
        throw new GUIException(std::string("Edit pieces table retrieval failed"));
    }

    // retrieve the rest of objects from the GUI design
    m_gtkBuilder->get_widget(GUI_MENU_ITEM_GAME_NEW, m_newMenuItem);
    if (m_newMenuItem == NULL)
    {
        throw new GUIException(std::string("new menu item retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_MENU_ITEM_GAME_QUIT, m_quitMenuItem);
    if (m_quitMenuItem == NULL)
    {
        throw new GUIException(std::string("quit menu item retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_MENU_ITEM_SETTINGS_NKPOINTS, m_settingsNKPointsMenuItem);
    if (m_settingsNKPointsMenuItem == NULL)
    {
        throw new GUIException(std::string("view nk points menu item retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_MENU_ITEM_SETTINGS_FORBIDDENAREA, m_settingsForbiddenAreaMenuItem);
    if (m_settingsForbiddenAreaMenuItem == NULL)
    {
        throw new GUIException(std::string("show forbidden area menu item retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_MENU_ITEM_SETTINGS_PREFS, m_settingsPrefsMenuItem);
    if (m_settingsPrefsMenuItem == NULL)
    {
        throw new GUIException(std::string("settings->preferences menu item retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_MENU_ITEM_HELP_ABOUT, m_helpAboutMenuItem);
    if (m_helpAboutMenuItem == NULL)
    {
        throw new GUIException(std::string("help->about menu item retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_VBOX_DRAWING_NAME, m_vBoxDrawing);
    if (m_vBoxDrawing == NULL)
    {
        throw new GUIException(std::string("Drawing area vbox retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_HBOX_GAME_STATUS_NAME, m_hBoxGameStatus);
    if (m_hBoxGameStatus == NULL)
    {
        throw new GUIException(std::string("Game status hbox retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_HBOX_COMPUTER_PIECES_NAME, m_hBoxComputerPieces);
    if (m_hBoxComputerPieces == NULL)
    {
        throw new GUIException(std::string("Computer pieces hbox retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_HBOX_PIECES_AREA_NAME, m_hBoxEditPieces);
    if (m_hBoxEditPieces == NULL)
    {
        throw new GUIException(std::string("PiecesHBox retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_HBOX_STATUSBAR_NAME, m_hBoxStatusBar);
    if (m_hBoxStatusBar == NULL)
    {
        throw new GUIException(std::string("status bar hbox retrieval failed"));
    }


    // place the custom widgets where they are expected to be
    // pack_start (Widget& child, bool expand, bool fill, guint padding=0)
    // first the board drawing area to the game status hbox and display it
    m_hBoxGameStatus->pack_start(m_boardDrawingArea, true, true);
    m_hBoxGameStatus->reorder_child(m_boardDrawingArea, 0);
    m_boardDrawingArea.show();

    // pick pieces drawing area, edit pieces table and show opponent's pieces
    m_hBoxEditPieces->pack_start(m_pickPiecesDrawingArea, true, true);
    m_hBoxEditPieces->pack_start(*m_editPieceTable, false, false);

    m_hBoxComputerPieces->pack_start(
            m_showOpponentPiecesDrawingArea,
            true,
            true);

    // if we don't show them, nobody will be able to see them
    // set_visible doesn't work in 2.16 (which is used in windows). use show!
    m_pickPiecesDrawingArea.show();
    m_showOpponentPiecesDrawingArea.show();

    // update the score shown in the status bar
    UpdateScoreStatus();

    // the custom status bar
    m_hBoxStatusBar->pack_start(m_player1ScoreLabel, true, true);
    m_hBoxStatusBar->pack_start(m_arrayStatusBarSeparator[0], false, true);
    m_hBoxStatusBar->pack_start(m_stopWatchLabelPlayer1, true, true);
    m_hBoxStatusBar->pack_start(m_arrayStatusBarSeparator[1], false, true);
    m_hBoxStatusBar->pack_start(m_player2ScoreLabel, true, true);
    m_hBoxStatusBar->pack_start(m_arrayStatusBarSeparator[2], false, true);
    m_hBoxStatusBar->pack_start(m_stopWatchLabelPlayer2, true, true);
    m_progressBar.set_orientation(Gtk::PROGRESS_LEFT_TO_RIGHT);
    m_progressBar.set_fraction(0.0);
    m_hBoxStatusBar->pack_start(m_progressBar, true, true);
    m_hBoxStatusBar->show_all();

    // progress handler for the computing process of the MinMax algorithm
    m_the1v1Game.SetProgressHandler(&MainWindow::ProgressUpdate);

    // connect the interthread communication (GLib::Dispatcher) to invalidate the
    // board drawing area
    m_signal_moveComputed.connect(
            sigc::mem_fun(*this, &MainWindow::NotifyMoveComputed));

    // connect the interthread communication to update the progress bar
    m_signal_computingProgressUpdated.connect(
            sigc::mem_fun(*this, &MainWindow::NotifyProgressUpdate));

    // connect the worker thread signal
    m_workerThread.signal_computingFinished().connect(
            sigc::mem_fun(*this, &MainWindow::WorkerThread_computingFinished));
            //sigc::ptr_fun(f) );

    // connect the signal coming from the pickPiecesDrawingArea to update TableEditPiece
    m_pickPiecesDrawingArea.signal_piecePicked().connect(
            sigc::mem_fun(*m_editPieceTable, &TableEditPiece::SetPiece));

    // connect the signal coming from the editing piece table to process the change in the
    // currently editing piece
    m_editPieceTable->signal_pieceChanged().connect(
            sigc::mem_fun(m_boardDrawingArea, &DrawingAreaBoard::SetCurrentPiece));

    // connect the signal coming fromt he board drawing area to process when the user clicks
    // on the board
    m_boardDrawingArea.signal_boardPicked().connect(
            sigc::mem_fun(*this, &MainWindow::BoardDrawingArea_BoardClicked));


    // connect the rest of the signals to the handlers
    // if the handler is not part of an object use sigc::ptr_fun
    this->signal_delete_event().connect(
            sigc::mem_fun(*this, &MainWindow::MainWindow_DeleteEvent));
    m_newMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemGameNew_Activate));
    m_quitMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemGameQuit_Activate));
    m_helpAboutMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemHelpAbout_Activate));
    m_settingsPrefsMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsPreferences_Activate));
    m_settingsNKPointsMenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsViewNKPoints_Toggled));
    m_settingsForbiddenAreaMenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowForbiddenArea_Toggled));

    // retrieve the default colour from the config class to apply it to the players
    uint8_t red, green, blue;
    Game1v1Config::Instance().GetPlayer1Colour(red, green, blue);
    m_the1v1Game.SetPlayerColour(Game1v1::e_Game1v1Player1, red, green, blue);
    Game1v1Config::Instance().GetPlayer2Colour(red, green, blue);
    m_the1v1Game.SetPlayerColour(Game1v1::e_Game1v1Player2, red, green, blue);

    // initialise the list of players of the board drawing area
    m_boardDrawingArea.AddPlayerToList(&(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1)));
    m_boardDrawingArea.AddPlayerToList(&(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2)));

    // launch the game!!
    LaunchNewGame();
}

MainWindow::~MainWindow()
{
    //TODO this is dirty too. See comment at the beginning of the constructor
    // it might cause a lot of trouble if there are more than 1 instance of MainWindow
    g_pMainWindow = NULL;

    // delete the worker thread
    m_workerThread.Join();

    // delete the randomizer too
    g_rand_free(m_randomizer);

    // http://library.gnome.org/devel/gtkmm/stable/classGtk_1_1Builder.html#ab8c6679c1296d6c4d8590ef907de4d5a
    // Note that you are responsible for deleting top-level widgets (windows and dialogs) instantiated
    // by the Builder object. Other widgets are instantiated as managed so they will be deleted
    // automatically if you add them to a container widget
    delete(m_aboutDialog);
    delete(m_configDialog);
    //delete(m_editPieceTable);
}

bool MainWindow::MainWindow_DeleteEvent(GdkEventAny*)
{
    if (m_workerThread.IsThreadComputingMove())
    {
        Gtk::MessageDialog::MessageDialog exitingMessage(
                *this,
                MESSAGE_ASK_BEFORE_CLOSE,
                true,
                Gtk::MESSAGE_QUESTION,
                Gtk::BUTTONS_YES_NO,
                true);

        if (exitingMessage.run() == Gtk::RESPONSE_NO)
        {
            // cancel delete event
            return true;
        }
    }

    // cancel the worker thread
    m_workerThread.CancelComputing();

    // continue with delete event
    return false;
}

void MainWindow::MenuItemGameQuit_Activate()
{
    if (m_workerThread.IsThreadComputingMove())
    {
        Gtk::MessageDialog::MessageDialog exitingMessage(
                *this,
                MESSAGE_ASK_BEFORE_CLOSE,
                true,
                Gtk::MESSAGE_QUESTION,
                Gtk::BUTTONS_YES_NO,
                true);

        if (exitingMessage.run() == Gtk::RESPONSE_NO)
        {
            // cancel delete event
            return;
        }
    }

    m_workerThread.CancelComputing();

    // exit the app
    this->hide();
}

void MainWindow::MenuItemGameNew_Activate()
{
    m_configDialog->set_title("New 1vs1 game");
    // starting coords can always be edited when a new game is launched
    m_configDialog->SetStartingCoordEditionSensitive(true);

    Gtk::ResponseType result = static_cast<Gtk::ResponseType>(m_configDialog->run());
    if (result == Gtk::RESPONSE_OK)
    {
        // save configuration shown by the dialog into global config singleton
        m_configDialog->SaveCurrentConfigIntoGlobalSettings();

        // go for the brand new game!!
        LaunchNewGame();
    }
#if defined(DEBUG_PRINTING) || defined (DEBUG)
    else if ( (result == Gtk::RESPONSE_CANCEL) || (result == Gtk::RESPONSE_DELETE_EVENT))
    {
        // config dialog cancelled
        ;
#ifdef DEBUG_PRINTING
        std::cout << "Config Dialog cancelled" << std::endl;
#endif // DEBUG_PRINTING
    }
#ifdef DEBUG
    else
    {
        // unexpected
        assert(0);
    }
#endif // DEBUG
#endif // defined(DEBUG_PRINTING) || defined (DEBUG)

    m_configDialog->hide();
}

void MainWindow::MenuItemSettingsViewNKPoints_Toggled()
{
    if (m_settingsNKPointsMenuItem->property_active())
    {
        m_boardDrawingArea.ShowNucleationPoints();
    }
    else
    {
        m_boardDrawingArea.HideNucleationPoints();
    }
}

void MainWindow::MenuItemSettingsShowForbiddenArea_Toggled()
{
    if (m_settingsForbiddenAreaMenuItem->property_active())
    {
        m_boardDrawingArea.ShowCurrentPlayerForbiddenArea();
    }
    else
    {
        m_boardDrawingArea.HideCurrentPlayerForbiddenArea();
    }
}

void MainWindow::MenuItemSettingsPreferences_Activate()
{
    Gtk::ResponseType result;
    m_configDialog->set_title("Configure current game");

    //TODO starting coords cannot be edited through the configuration dialog yet
    m_configDialog->SetStartingCoordEditionSensitive(false);

    result = static_cast<Gtk::ResponseType>(m_configDialog->run());
    if (result == Gtk::RESPONSE_OK)
    {
        // if current player is a human being and it's been set to computer
        // next move will have to be requested to the worker thread.
        // otherwise new settings will be automatically applied once current move
        // is finished

        if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1)     &&
               (Game1v1Config::Instance().IsPlayer1Computer() == false) &&
               (m_configDialog->IsPlayer1TypeComputer() == true)        )
             ||
             ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2)     &&
               (Game1v1Config::Instance().IsPlayer2Computer() == false) &&
               (m_configDialog->IsPlayer2TypeComputer() == true)        )
           )
        {
            // current player is not the computer and it has been
            // requested in the config dialog to change it to be the computer
            // save configuration shown by the dialog into global config singleton
            // and request the processing thread to compute next player1 move
            m_configDialog->SaveCurrentConfigIntoGlobalSettings();

            // next move was being calculated by a human being, but now it has
            // been swapped to a computer player. Humans are not allowed to edit
            // pieces while computer is thinking
            m_editPieceTable->set_sensitive(false);

            // requesting the thread!!
            RequestThreadToComputeNextMove(m_currentMovingPlayer, true);
        }
        else
        {
            // an info message might be needed to inform the user changes will be
            // applied later
            bool showInfoMessage = false;
            if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1)     &&
                   (Game1v1Config::Instance().IsPlayer1Computer() == true) )
                 ||
                 ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2)     &&
                 (Game1v1Config::Instance().IsPlayer2Computer() == true) )
               )
            {
                // current moving player is the computer. Changes will be applied
                // once current move is calculated
                showInfoMessage = true;
            }

            // save configuration shown by the dialog into global config singleton
            // new configuratio will be applied once current move is calculated
            // no changes need to be applied to the editPieceTable
            m_configDialog->SaveCurrentConfigIntoGlobalSettings();

            // show the info message after global settings have been applied
            if (showInfoMessage)
            {
                Gtk::MessageDialog::MessageDialog infoMessage(
                    *this,
                    "Changes will be applied once current move is calculated by the computer",
                    true,
                    Gtk::MESSAGE_INFO,
                    Gtk::BUTTONS_OK,
                    true);

                if (infoMessage.run())
                {
                    ; // the dialog only has 1 button
                }
            }
        }
    }

    m_configDialog->hide();
}

void MainWindow::MenuItemHelpAbout_Activate()
{
    m_aboutDialog->run();
    m_aboutDialog->hide();
}

void MainWindow::LaunchNewGame()
{
    if (m_workerThread.IsThreadComputingMove())
    {
        // stop worker thread. Current calculation won't be needed in the new game
        // this call blocks the current thread until the worker thread is ready to
        // calculate next move
        m_workerThread.CancelComputing();
    }

    // clear old moves from the move queue. They belong to the old game
    G_LOCK(s_computerMoveQueue);
        while (!s_computerMoveQueue.empty())
        {
            s_computerMoveQueue.pop();
        }
    G_UNLOCK(s_computerMoveQueue);

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
    m_editPieceTable->SetPieceRGB(
        static_cast<float>(red)   / 255,
        static_cast<float>(green) / 255,
        static_cast<float>(blue)  / 255);

    UpdateScoreStatus();

    // reset the cursor (even if it's been already done)
    ResetCursor();

    // stopwatches must be restarted.
    m_stopWatchLabelPlayer1.Reset();
    m_stopWatchLabelPlayer2.Reset();

    // restart the progress bar
    m_progressBar.set_fraction(0.0);

    // Start player1's timer
    m_stopWatchLabelPlayer1.Continue();

    // reset and force redraw editPieceTable. It'l be set to sensitive
    // or unsensitive depending on the type of player1
    m_editPieceTable->SetPiece(e_noPiece);

    // new game just started. It can't be finished!
    m_currentGameFinished = false;
    // player1 is the one who's got to put a piece next
    m_currentMovingPlayer = Game1v1::e_Game1v1Player1;

    if (Game1v1Config::Instance().IsPlayer1Computer())
    {
        // next player is the computer. Disallow editing the edit piece
        // area while computer is processing next move
        m_editPieceTable->set_sensitive(false);

        RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player1, false);
    }
    else
    {
        // a human b eing is allowed to edit next piece
        m_editPieceTable->set_sensitive(true);

        // player1 is a human and he/she will put down the next piece (player1 goes always first)
        m_boardDrawingArea.SetCurrentPlayer(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    }
}

void MainWindow::RequestThreadToComputeNextMove(
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
             (Rules::CanPlayerGo(m_the1v1Game.GetBoard(), opponent) ) )
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
            << "Error while telling the thread to start computing."
            << std::endl
            << "The worker thread seems to be busy"
            << std::endl;
#endif
        std::stringstream theMessage;
        theMessage << "<b>Fatal Error</b> telling the computing to start computing (seems to be busy)."
                   << std::endl
                   << "Application will try to exit now!";

        Gtk::MessageDialog::MessageDialog fatalErrorMessage(
            *this,
            theMessage.str().c_str(),
            true,
            Gtk::MESSAGE_INFO,
            Gtk::BUTTONS_OK,
            true);

        if (fatalErrorMessage.run())
        {
            ; // the dialog only has 1 button
        }

        // kill the worker thread
        m_workerThread.CancelComputing();

        // exit the app
        this->hide();
    }
}

void MainWindow::WorkerThread_computingFinished(
        const Piece              &a_piece,
        const Coordinate         &a_coord,
        Game1v1::eGame1v1Player_t a_playerToMove,
        int32_t                   a_returnValue)
{
    // WARNING: this method is run by another thread.
    // once the m_signal_moveComputed signal is emited the
    // main thread will update the GUI widgets

    G_LOCK(s_computerMoveQueue);
        sCalculatedMove_t data;
        data.piece = a_piece;
        data.coord = a_coord;
        data.playerToMove = a_playerToMove;
        s_computerMoveQueue.push(data);
    G_UNLOCK(s_computerMoveQueue);

#ifdef DEBUG_PRINT
    if (a_piece.GetType() == e_noPiece)
    {
        std::cout << "Computer can't move" << std::endl;
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

void MainWindow::BoardDrawingArea_BoardClicked(const Coordinate &a_coord, const Piece &a_piece, const Player &a_player)
{
#ifdef DEBUG_PRINT
    std::cout << a_player.GetName() <<  " clicked in (" << a_coord.m_row << ", " << a_coord.m_col << ")" << std::endl;
#endif

	if (m_workerThread.IsThreadComputingMove())
	{
#ifdef DEBUG_PRINT
	    std::cout
            << "The worker thread is busy. Please be patient (a polite way to say: 'Fuck off')"
            << std::endl;
#endif
	    return;
	}

    if( ( (a_player.NumberOfPiecesAvailable() == e_numberOfPieces) &&
          (!Rules::IsPieceDeployableInStartingPoint(
                    m_the1v1Game.GetBoard(),
                    a_piece,
                    a_coord,
                    a_player.GetStartingCoordinate())) ) ||
        ( (a_player.NumberOfPiecesAvailable() < e_numberOfPieces) &&
          (!Rules::IsPieceDeployable(
                    m_the1v1Game.GetBoard(),
                    a_piece,
                    a_coord,
                    a_player)) ) )
    {
#ifdef DEBUG_PRINT
        std::cout << "Cheeky you! Don't try to put a piece where it's not allowed to" << std::endl;
#endif
        return;
    }

    // don't save the move directly in the board. Use the same interface as the worker thread
    // Game 1v1 board will be only be modified in NotifyMoveComputed
    G_LOCK(s_computerMoveQueue);
        sCalculatedMove_t data;
        data.piece = a_piece;
        data.coord = a_coord;
        data.playerToMove = m_the1v1Game.GetPlayerType(a_player);
        s_computerMoveQueue.push(data);
    G_UNLOCK(s_computerMoveQueue);

    // this method is not expected to be run by another thread, but if both worker thread and human
    // user use the same interface, things are much easier to understand
    // so, there's no need to emit any signal (which adds a bit of overhead), but
    // interfaces must be followed, so NotifyMoveComputed is caled directly instead of emiting interthread
    // signals
    //m_signal_moveComputed.emit();
    NotifyMoveComputed();

    return;
}

void MainWindow::NotifyMoveComputed()
{
    // after the lock protected loop these variables will contain
    // the latest piece and latest coord deployed
    Piece latestPiece(e_noPiece);
    Coordinate latestCoord(COORD_UNINITIALISED, COORD_UNINITIALISED);
    Game1v1::eGame1v1Player_t latestPlayerToMove;

    G_LOCK(s_computerMoveQueue);

        if (s_computerMoveQueue.empty())
        {
            G_UNLOCK(s_computerMoveQueue);
            return;
        }

        // save the latest moves on the board
        do
        {
            latestPiece        = s_computerMoveQueue.front().piece;
            latestCoord        = s_computerMoveQueue.front().coord;
            latestPlayerToMove = s_computerMoveQueue.front().playerToMove;

            if (latestPiece.GetType() != e_noPiece)
            {
                m_the1v1Game.PutDownPiece(
                        latestPiece,
                        latestCoord,
                        latestPlayerToMove);
            }

            s_computerMoveQueue.pop();

        } while (!s_computerMoveQueue.empty());

    G_UNLOCK(s_computerMoveQueue);


    // actions will depend on who put down the latest piece
    const Player &latestPlayer   = m_the1v1Game.GetPlayer(latestPlayerToMove);
    const Player &latestOpponent = m_the1v1Game.GetOpponent(latestPlayerToMove);

    Game1v1::eGame1v1Player_t followingPlayer; // player who didn't put a piece o the board this time
    StopWatchLabel* stopWatchPlayer;   // pointer to the stopwatch of the latest player to move
    StopWatchLabel* stopWatchOpponent; // pointer to the stopwatch of the opponent of latest player to move
    switch (latestPlayerToMove)
    {
    case Game1v1::e_Game1v1Player1:
    {
        stopWatchPlayer   = &m_stopWatchLabelPlayer1;
        stopWatchOpponent = &m_stopWatchLabelPlayer2;
        followingPlayer   = Game1v1::e_Game1v1Player2;
        break;
    }
    case Game1v1::e_Game1v1Player2:
    {
        stopWatchPlayer   = &m_stopWatchLabelPlayer2;
        stopWatchOpponent = &m_stopWatchLabelPlayer1;
        followingPlayer   = Game1v1::e_Game1v1Player1;
        break;
    }
#ifdef DEBUG
    default:
    {
        assert(0);
    }
#endif
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
    if ( Rules::CanPlayerGo(m_the1v1Game.GetBoard(), latestOpponent) )
    {
        // next player to move is the opponent
        m_boardDrawingArea.SetCurrentPlayer(latestOpponent);

        // stop current player's stopwatch
        stopWatchPlayer->Stop();
        // resume the opponent's stopwatch
        stopWatchOpponent->Continue();
        // restart the progress bar
        m_progressBar.set_fraction(0.0);

        // it will be platestOpponent's go next. set piece colour to latestOpponent's
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        latestOpponent.GetColour(red, green, blue);
        m_editPieceTable->SetPieceRGB(
            static_cast<float>(red)   / 255,
            static_cast<float>(green) / 255,
            static_cast<float>(blue)  / 255);

        // remove the actual piece being edited from the edit piece drawing area
        // and force the edit piece drawing area to be redraw
        m_editPieceTable->SetPiece(e_noPiece);

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
            m_editPieceTable->set_sensitive(false);

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
            m_editPieceTable->set_sensitive(true);

            // restore the mouse cursor so the human being who has to put down next piece
            // can do it
            ResetCursor();
        }
    }
    else
    {
        // remove the actual piece being edited from the edit piece drawing area
        // and force the edit piece drawing area to be redraw
        m_editPieceTable->SetPiece(e_noPiece);

        // latestPlayer can put down more pieces. Game is still on
        m_pickPiecesDrawingArea.Invalidate();

        if ( !Rules::CanPlayerGo(m_the1v1Game.GetBoard(), latestPlayer) )
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

void MainWindow::GameFinished()
{
    // once this function is called, game is supposed to e finished
    if (m_currentGameFinished == true)
    {
        return;
    }
    m_currentGameFinished = true;

    // reset the cursor (even if it's been already done)
    ResetCursor();

    // stop stopwatches (even if it was already done)
    m_stopWatchLabelPlayer1.Stop();
    m_stopWatchLabelPlayer2.Stop();

    // no player is expected to put down any piece on the board now
    m_boardDrawingArea.UnsetCurrentPlayer();

    // allow a possible human user to play with the edit pieces area
    m_editPieceTable->set_sensitive(true);

    // restart the progress bar
    m_progressBar.set_fraction(0.0);

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

    std::stringstream theMessage;
    if (squaresLeftPlayer1 == squaresLeftPlayer2)
    {
        theMessage << "That was a <b>DRAW</b>!"
                   << std::endl
                   << "    Both "
                   << player1.GetName()
                   << " and "
                   << player2.GetName()
                   << " have <b>"
                   << squaresLeftPlayer1
                   << "</b> squares left";
    }
    else //(squaresLeftPlayer1 != squaresLeftPlayer2)
    {
        theMessage << "<b>"
                   << ((squaresLeftPlayer1 < squaresLeftPlayer2) ? player1.GetName() : player2.GetName())
                   << "</b> won!"
                   << std::endl
                   << "    "
                   << player1.GetName()
                   << " has <b>"
                   << squaresLeftPlayer1
                   << "</b> squares left and "
                   << player2.GetName()
                   << " <b>"
                   << squaresLeftPlayer2
                   << "</b>";
    }

    //MessageDialog (
    //        Gtk::Window& parent,
    //        const Glib::ustring& message,
    //        bool use_markup=false,
    //        MessageType type=MESSAGE_INFO,
    //        ButtonsType buttons=BUTTONS_OK,
    //        bool modal=false)
    Gtk::MessageDialog::MessageDialog gameOverMessage(
            *this,
            theMessage.str().c_str(),
            true,
            Gtk::MESSAGE_INFO,
            Gtk::BUTTONS_OK,
            true);

    if (gameOverMessage.run())
    {
        ; // the dialog only has 1 button
    }
}

void MainWindow::NotifyProgressUpdate()
{
    float progress;

    if (m_currentGameFinished)
    {
        // do not update the progress bar if the current game is finished
        return;
    }

    G_LOCK(s_computingCurrentProgress);
        progress = s_computingCurrentProgress;
    G_UNLOCK(s_computingCurrentProgress);

    m_progressBar.set_fraction(progress);
}

void MainWindow::UpdateScoreStatus()
{
    // calculate the number of squares left
    int32_t squaresLeftPLayer1 = 0;
    int32_t squaresLeftPlayer2 = 0;
    const Player &player1 = m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1);
    const Player &player2 = m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2);
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (player1.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPLayer1 += player1.m_pieces[i].GetNSquares();
        }

        if (player2.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer2 += player2.m_pieces[i].GetNSquares();
        }
    }

    // update the GUI widgets
    std::stringstream theMessage;
    theMessage << player2.GetName()
               << ": "
               << std::setfill(' ') << std::setw(2)
               << static_cast<int32_t>(squaresLeftPlayer2)
               << " left";

    m_player2ScoreLabel.set_text(theMessage.str().c_str());

    theMessage.str("");
    theMessage << player1.GetName()
               << ": "
               << std::setfill(' ') << std::setw(2)
               << static_cast<int32_t>(squaresLeftPLayer1)
               << " left";

    m_player1ScoreLabel.set_text(theMessage.str().c_str());
}

void MainWindow::SetWaitCursor()
{
    Glib::RefPtr<Gdk::Window> window;
    window = m_editPieceTable->get_window();
    if (window)
    {
        get_window()->set_cursor(Gdk::Cursor(Gdk::WATCH));
    }
}
void MainWindow::ResetCursor()
{
    Glib::RefPtr<Gdk::Window> window;
    window= m_editPieceTable->get_window();
    if (window)
    {
        window->set_cursor();
    }
}
