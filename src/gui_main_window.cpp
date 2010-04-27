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
#include "gui_glade_defs.h"

/// @brief uninitialised coord value
static const int32_t COORD_UNITIALISED = 0xffffffff;

/// message to be shown to the user when he/she requested the
/// application to be closed, and the worker thread is busy computing
static const char MESSAGE_ASK_BEFORE_CLOSE[] =
        "The game is computing the next move. Are you sure do you want to close the application?";

/// length of the custom messages to be shown to the user
static const int32_t MESSAGE_LENGTH = 100;

// drawing area options
static const float BOARD_BORDER_RED        = 0.8;
static const float BOARD_BORDER_GREEN      = 0.0;
static const float BOARD_BORDER_BLUE       = 0.0;
static const float BOARD_BORDER_LINE_WIDTH = 5.0;

static const float BOARD_RED        = BOARD_BORDER_RED;
static const float BOARD_GREEN      = BOARD_BORDER_GREEN;
static const float BOARD_BLUE       = BOARD_BORDER_BLUE;
static const float BOARD_LINE_WIDTH = 1.0;

static const float PLAYER_ME_RED   = 0.0;
static const float PLAYER_ME_GREEN = 0.0;
static const float PLAYER_ME_BLUE  = 0.9;

static const float PLAYER_OPPONENT_RED   = 0.3;
static const float PLAYER_OPPONENT_GREEN = 0.8;
static const float PLAYER_OPPONENT_BLUE  = 0.3;

static const float GHOST_PIECE_RIGHT_RED   = PLAYER_OPPONENT_RED;
static const float GHOST_PIECE_RIGHT_GREEN = PLAYER_OPPONENT_GREEN;
static const float GHOST_PIECE_RIGHT_BLUE  = PLAYER_OPPONENT_BLUE;

static const float GHOST_PIECE_WRONG_RED          = 0.9;
static const float GHOST_PIECE_WRONG_GREEN        = 0.0;
static const float GHOST_PIECE_WRONG_BLUE         = 0.0;
static const float GHOST_PIECE_ALPHA_TRANSPARENCY = 0.2;

static const uint32_t STOPWATCH_UPDATE_PERIOD_MILLIS = 500; // 1000 = 1 second

//TODO remove this testing shit whenever code is ready
#define PLAYER1_IS_COMPUTER false
#define PLAYER2_IS_COMPUTER true

// move calculated by the worker thread
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

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder)  throw (GUIException):
    Gtk::Window(cobject), //Calls the base class constructor
    m_gtkBuilder(a_gtkBuilder),
    m_the1v1Game(),
    m_lastCoord(COORD_UNITIALISED, COORD_UNITIALISED),
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
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1).GetName() + std::string("'s elapsed time ")),
    m_stopWatchLabelPlayer2(
        STOPWATCH_UPDATE_PERIOD_MILLIS,
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2).GetName() + std::string("'s elapsed time "))
{
    //TODO this is dirty (even though it works) the way MainWindow::ProgressUpdate
    // access the MainWindow Instance should be fixed in some way
    // assert there's no more than 1 instance of MainWindow
#ifdef DEBUG
    assert(g_pMainWindow == NULL);
#endif
    g_pMainWindow = this;

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


    //TODO the colours should be handled by some kind of dialog. This will do it now
    m_the1v1Game.SetPlayerColour(
            Game1v1::e_Game1v1Player2,
            PLAYER_ME_RED*255,
            PLAYER_ME_GREEN*255,
            PLAYER_ME_BLUE*255);

    m_the1v1Game.SetPlayerColour(
                Game1v1::e_Game1v1Player1,
                PLAYER_OPPONENT_RED*255,
                PLAYER_OPPONENT_GREEN*255,
                PLAYER_OPPONENT_BLUE*255);

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

    // http://library.gnome.org/devel/gtkmm/stable/classGtk_1_1Builder.html#ab8c6679c1296d6c4d8590ef907de4d5a
    // Note that you are responsible for deleting top-level widgets (windows and dialogs) instantiated
    // by the Builder object. Other widgets are instantiated as managed so they will be deleted
    // automatically if you add them to a container widget
    delete(m_aboutDialog);
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

    // kill the worker thread
    m_workerThread.Join();

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

    m_workerThread.Join();

    // exit the app
    this->hide();
}

void MainWindow::MenuItemGameNew_Activate()
{
    if (m_workerThread.IsThreadComputingMove())
    {
        Gtk::MessageDialog::MessageDialog infoMessage(
                *this,
                "There's a movement being calculated now. Please, wait until the process is finished to start a new game",
                true,
                Gtk::MESSAGE_QUESTION,
                Gtk::BUTTONS_OK,
                true);

        infoMessage.run();
        return;
    }

    char theMessage[MESSAGE_LENGTH];
    if ( (m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1).NumberOfPiecesAvailable() == e_numberOfPieces) &&
         (m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2).NumberOfPiecesAvailable() == e_numberOfPieces) )
    {
        // the game never started. Restart the user timer
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "This will restart your elapsed time stopwatch. Are you sure?");
    }
    else if ( (Rules::CanPlayerGo(m_the1v1Game.GetBoard(), m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1)) == false) &&
              (Rules::CanPlayerGo(m_the1v1Game.GetBoard(), m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2)) == false) )
    {
        // game is finished
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "If a new game starts the board will be wiped out. Are you sure?");
    }
    else
    {
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "Are you sure you want to cancel the current Game?");
    }

    Gtk::MessageDialog::MessageDialog cancelCurrentGameMessage(
            *this,
            theMessage,
            true,
            Gtk::MESSAGE_QUESTION,
            Gtk::BUTTONS_YES_NO,
            true);

    if (cancelCurrentGameMessage.run() == Gtk::RESPONSE_YES)
    {
        LaunchNewGame();
    }
}

void MainWindow::MenuItemHelpAbout_Activate()
{
    m_aboutDialog->run();
    m_aboutDialog->hide();
}

void MainWindow::LaunchNewGame()
{
    // reset the current game
    m_the1v1Game.Reset();
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
    // reset and force redraw editPieceTable
    m_editPieceTable->SetPiece(e_noPiece);

    UpdateScoreStatus();

    // stopwatches must be restarted.
    m_stopWatchLabelPlayer1.Reset();
    m_stopWatchLabelPlayer2.Reset();

    // restart the progress bar
    m_progressBar.set_fraction(0.0);

    // Start player1's timer
    m_stopWatchLabelPlayer1.Continue();

    //TODO this check using the #define is temporary
    if (PLAYER1_IS_COMPUTER)
    {
        // next player is the computer. Disallow editing the board while
        // computer is processing next move
        m_boardDrawingArea.UnsetCurrentPlayer();

        RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player1);
    }
    else
    {
        // player1 is a human and he/she will put down the next piece (player1 goes always first)
        m_boardDrawingArea.SetCurrentPlayer(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    }
}

void MainWindow::RequestThreadToComputeNextMove(
        Game1v1::eGame1v1Player_t a_whoMoves,
        const Coordinate         &a_coordinate,
        const Piece              &a_piece)
{
    // set the cursor to busy in the board drawing area, so the user
    // won't get (very) nervous while the computer is processing the next move
    Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea.get_window();
    if (window)
    {
        window->set_cursor(Gdk::Cursor(Gdk::WATCH));
    }

    if (!m_workerThread.ComputeMove(m_the1v1Game, a_whoMoves, a_coordinate, a_piece))
    {
#ifdef DEBUG_PRINT
        std::cout
            << "Error while telling the thread to start computing."
            << std::endl
            << "The worker thread seems to be busy"
            << std::endl;
#endif
        char theMessage[MESSAGE_LENGTH];
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "<b>Fatal Error</b> telling the computing to start computing (seems to be busy).\n Application will try to exit now!");

        Gtk::MessageDialog::MessageDialog fatalErrorMessage(
            *this,
            theMessage,
            true,
            Gtk::MESSAGE_INFO,
            Gtk::BUTTONS_OK,
            true);

        if (fatalErrorMessage.run())
        {
            ; // the dialog only has 1 button
        }

        // kill the worker thread
        m_workerThread.Join();

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
    Coordinate latestCoord(COORD_UNITIALISED, COORD_UNITIALISED);
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

        }while (!s_computerMoveQueue.empty());

    G_UNLOCK(s_computerMoveQueue);


    // actions will depend on who put down the latest piece
    const Player &latestPlayer   = m_the1v1Game.GetPlayer(latestPlayerToMove);
    const Player &latestOpponent = m_the1v1Game.GetOpponent(latestPlayerToMove);

    StopWatchLabel* stopWatchPlayer;   // pointer to the stopwatch of the latest player to move
    StopWatchLabel* stopWatchOpponent; // pointer to the stopwatch of the opponent of latest player to move
    switch (latestPlayerToMove)
    {
    case Game1v1::e_Game1v1Player1:
    {
        stopWatchPlayer   = &m_stopWatchLabelPlayer1;
        stopWatchOpponent = &m_stopWatchLabelPlayer2;
        break;
    }
    case Game1v1::e_Game1v1Player2:
    {
        stopWatchPlayer   = &m_stopWatchLabelPlayer2;
        stopWatchOpponent = &m_stopWatchLabelPlayer1;
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

        // --it checks if the next player to put down a piece is a computer--
        if ( ( (latestPlayerToMove == Game1v1::e_Game1v1Player2) && PLAYER1_IS_COMPUTER ) ||
             ( (latestPlayerToMove == Game1v1::e_Game1v1Player1) && PLAYER2_IS_COMPUTER ) )
        {
            // next player is the computer. Disallow editing the board while
            // computer is processing next move
            m_boardDrawingArea.UnsetCurrentPlayer();

            // tell the worker thread to compute next move
            if (latestPlayerToMove == Game1v1::e_Game1v1Player1)
            {
                RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player2);
            }
            else
            {
                RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player1);
            }
        }
        else
        {
            // allow next player to edit the board 'cos it's a human being
            m_boardDrawingArea.SetCurrentPlayer(latestOpponent);

            // restore the mouse cursor so the human being who has to put down next piece
            // can do it
            Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea.get_window();
            if (window)
            {
                window->set_cursor();
            }
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
        // game is still on. Current player can put down more pieces
    }
}

void MainWindow::GameFinished()
{
    // reset the cursor (even if it's been already done)
    Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea.get_window();
    if (window)
    {
        window->set_cursor();
    }

    // stop stopwatches (even if it was already done)
    m_stopWatchLabelPlayer1.Stop();
    m_stopWatchLabelPlayer2.Stop();

    // no player is expected to put down any piece on the board now
    m_boardDrawingArea.UnsetCurrentPlayer();

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

    char theMessage[MESSAGE_LENGTH];
    if (squaresLeftPlayer1 == squaresLeftPlayer2)
    {
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "That was a <b>DRAW</b>!\n\tBoth %s and %s have <b>%d</b> squares left",
                player1.GetName().c_str(),
                player2.GetName().c_str(),
                squaresLeftPlayer1);
    }
    else //(squaresLeftPlayer1 != squaresLeftPlayer2)
    {
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "<b>%s</b> won!\n\t%s has <b>%d</b> squares left and %s <b>%d</b>",
                (squaresLeftPlayer1 < squaresLeftPlayer2) ?
                    player1.GetName().c_str() : player2.GetName().c_str(),
                player1.GetName().c_str(),
                squaresLeftPlayer1,
                player2.GetName().c_str(),
                squaresLeftPlayer2);
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
            theMessage,
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
