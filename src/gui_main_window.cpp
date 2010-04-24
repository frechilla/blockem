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

// static easy to use mutex for shared data across threads
// http://tadeboro.blogspot.com/2009/06/multi-threaded-gtk-applications.html
static float s_computingCurrentProgress = 0.0;
G_LOCK_DEFINE_STATIC(s_computingCurrentProgress);

// this queue is used to communicate the worker thread with the main thread
// in a thread-safe manner. It's a bit dirty but it will do it for now
static std::queue< std::pair<Piece, Coordinate> > s_computerMoveQueue = std::queue< std::pair<Piece, Coordinate> >();
G_LOCK_DEFINE_STATIC(s_computerMoveQueue);


void MainWindow::ProgressUpdate(float a_progress)
{
#ifdef DEBUG
    assert(MainWindow::GetPtr());
#endif

    G_LOCK(s_computingCurrentProgress);
        s_computingCurrentProgress = a_progress;
    G_UNLOCK(s_computingCurrentProgress);

    MainWindow::Instance().m_signal_computingProgressUpdated.emit();
}

MainWindow::MainWindow() :
    Singleton<MainWindow>(),
    m_refXml(NULL),
    m_theWindow(NULL),
    m_the1v1Game(),
    m_lastCoord(COORD_UNITIALISED, COORD_UNITIALISED),
    m_workerThread(NULL),
    m_aboutDialog(NULL),
    m_pickPiecesDrawingArea(m_the1v1Game.GetPlayer1(), DrawingAreaShowPieces::eOrientation_leftToRight),
    m_showComputerPiecesDrawingArea(m_the1v1Game.GetPlayer2(), DrawingAreaShowPieces::eOrientation_bottomToTop),
    m_boardDrawingArea(m_the1v1Game.GetBoard()),
    m_editPieceTable(NULL),
    m_stopWatchLabelUser(STOPWATCH_UPDATE_PERIOD_MILLIS, m_the1v1Game.GetPlayer1().GetName() + std::string("'s elapsed time ")),
    m_stopWatchLabelComputer(STOPWATCH_UPDATE_PERIOD_MILLIS, m_the1v1Game.GetPlayer2().GetName() + std::string("'s elapsed time "))
{
}

MainWindow::~MainWindow()
{
    delete(m_aboutDialog);
    delete(m_workerThread);
    delete(m_editPieceTable);

    // deleting this object prints out a lot of error messages on the screen
    // or even dumps a core.
    // There's a big memory leak running the app with valgrin, but all of it is
    // caused by gui code, and I don't know why it happens. It also happens if
    // m_theWindow is deleted, so I chose not to show those ugly wrror lines printed
    // on the screen
    //delete(m_theWindow);
}

void MainWindow::Initialize(Glib::RefPtr<Gnome::Glade::Xml> a_refXml) throw (GUIException)
{
    m_refXml         = a_refXml;
    m_workerThread   = new MainWindowWorkerThread();
    m_aboutDialog    = new AboutDialog(a_refXml);
    m_editPieceTable = new TableEditPiece(a_refXml);

    // first of all retrieve the Gtk::window object and set its icon
    m_refXml->get_widget(GUI_MAIN_WINDOW_NAME, m_theWindow);
    if (m_theWindow == NULL)
    {
        throw new GUIException(std::string("couldn't retrieve the MainWindow from the .glade file"));
    }

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
            m_theWindow->set_icon(icon);
        }
    }

    // retrieve the objects from the GUI design
    m_refXml->get_widget(GUI_MENU_ITEM_GAME_NEW, m_newMenuItem);
    if (m_newMenuItem == NULL)
    {
        throw new GUIException(std::string("new menu item retrieval failed"));
    }

    m_refXml->get_widget(GUI_MENU_ITEM_GAME_QUIT, m_quitMenuItem);
    if (m_quitMenuItem == NULL)
    {
        throw new GUIException(std::string("quit menu item retrieval failed"));
    }

    m_refXml->get_widget(GUI_MENU_ITEM_HELP_ABOUT, m_helpAboutMenuItem);
    if (m_helpAboutMenuItem == NULL)
    {
        throw new GUIException(std::string("help->about menu item retrieval failed"));
    }

    m_refXml->get_widget(GUI_VBOX_DRAWING_NAME, m_vBoxDrawing);
	if (m_vBoxDrawing == NULL)
	{
		throw new GUIException(std::string("Drawing area vbox retrieval failed"));
	}

    m_refXml->get_widget(GUI_HBOX_GAME_STATUS_NAME, m_hBoxGameStatus);
    if (m_hBoxGameStatus == NULL)
    {
    	throw new GUIException(std::string("Game status hbox retrieval failed"));
    }

    m_refXml->get_widget(GUI_HBOX_COMPUTER_PIECES_NAME, m_hBoxComputerPieces);
    if (m_hBoxComputerPieces == NULL)
    {
    	throw new GUIException(std::string("Computer pieces hbox retrieval failed"));
    }

    m_refXml->get_widget(GUI_HBOX_PIECES_AREA_NAME, m_hBoxEditPieces);
    if (m_hBoxEditPieces == NULL)
    {
        throw new GUIException(std::string("PiecesHBox retrieval failed"));
    }

    m_refXml->get_widget(GUI_HBOX_STATUSBAR_NAME, m_hBoxStatusBar);
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
    m_hBoxEditPieces->pack_start(m_editPieceTable->Table(), false, false);

	m_hBoxComputerPieces->pack_start(
			m_showComputerPiecesDrawingArea,
			true,
			true);

    // if we don't show them, nobody will be able to see them
    // set_visible doesn't work in windows. use show!
    m_pickPiecesDrawingArea.show();
    m_showComputerPiecesDrawingArea.show();

    // update the score shown in the status bar
    UpdateScoreStatus();

	// the custom status bar
	m_hBoxStatusBar->pack_start(m_userScoreLabel, true, true);
	m_hBoxStatusBar->pack_start(m_arrayStatusBarSeparator[0], false, true);
	m_hBoxStatusBar->pack_start(m_stopWatchLabelUser, true, true);
	m_hBoxStatusBar->pack_start(m_arrayStatusBarSeparator[1], false, true);
	m_hBoxStatusBar->pack_start(m_computerScoreLabel, true, true);
	m_hBoxStatusBar->pack_start(m_arrayStatusBarSeparator[2], false, true);
	m_hBoxStatusBar->pack_start(m_stopWatchLabelComputer, true, true);
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

    // connect the interthread communication (GLib::Dispatcher) to notify the
    // user that the game is finished
    m_signal_gameFinished.connect(
            sigc::mem_fun(*this, &MainWindow::NotifyGameFinished));

    // connect the interthread communication to update the progress bar
    m_signal_computingProgressUpdated.connect(
            sigc::mem_fun(*this, &MainWindow::NotifyProgressUpdate));

    // connect the worker thread signal
    m_workerThread->signal_computingFinished().connect(
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
	m_theWindow->signal_delete_event().connect(
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

	m_editPieceTable->SetPieceRGB(
	        PLAYER_OPPONENT_RED,
	        PLAYER_OPPONENT_GREEN,
	        PLAYER_OPPONENT_BLUE);

    // initialise the list of players of the board drawing area
    m_boardDrawingArea.AddPlayerToList(&(m_the1v1Game.GetPlayer1()));
    m_boardDrawingArea.AddPlayerToList(&(m_the1v1Game.GetPlayer2()));

    // human's go. Set the player in the board drawing area and start counting
    m_boardDrawingArea.SetCurrentPlayer(m_the1v1Game.GetPlayer1());
    m_stopWatchLabelUser.Continue();
}

void MainWindow::WorkerThread_computingFinished(
        const Piece      &a_piece,
        const Coordinate &a_coord,
        int32_t           a_returnValue)
{
    // WARNING: this method is run by another thread.
    // once the m_signal_moveComputed signal is emited the
    // main thread will update the GUI widgets

    G_LOCK(s_computerMoveQueue);
        //TODO moves should be queued. A move can be lost if the main thread
        // is too slow to draw the computer thread calculations

    s_computerMoveQueue.push(std::pair<Piece, Coordinate>(a_piece, a_coord));

    G_UNLOCK(s_computerMoveQueue);

    // this signal is being issued from the MainWindowWorkerThread
    // GTK is not thread safe, so every GUI function should be called from the same thread
    // we need inter-thread communication to force the main thread to invalidate the
    // board drawing area
    // have a look at:
    //     http://library.gnome.org/devel/glibmm/stable/thread_2dispatcher_8cc-example.html
    m_signal_moveComputed.emit();

    if (a_piece.GetType() == e_noPiece)
    {
        if (Rules::CanPlayerGo(m_the1v1Game.GetBoard(), m_the1v1Game.GetPlayer1()) == false)
        {
            // the game is over. Computing returned e_noPiece and
            // the opponent can't go
            m_signal_gameFinished.emit();
        }
#ifdef DEBUG_PRINT
        else
        {
            std::cout << "Computer can't move" << std::endl;
        }
#endif
    }
}

bool MainWindow::MainWindow_DeleteEvent(GdkEventAny*)
{
    if (m_workerThread->IsThreadComputingMove())
    {
        Gtk::MessageDialog::MessageDialog exitingMessage(
                *m_theWindow,
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
    m_workerThread->Join();

    // continue with delete event
    return false;
}

void MainWindow::MenuItemGameQuit_Activate()
{
    if (m_workerThread->IsThreadComputingMove())
    {
        Gtk::MessageDialog::MessageDialog exitingMessage(
                *m_theWindow,
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

    m_workerThread->Join();

    // exit the app
    m_theWindow->hide();
}

void MainWindow::MenuItemGameNew_Activate()
{
    if (m_workerThread->IsThreadComputingMove())
    {
        Gtk::MessageDialog::MessageDialog infoMessage(
                *m_theWindow,
                "There's a movement being calculated now. Please, wait until the process is finished to start a new game",
                true,
                Gtk::MESSAGE_QUESTION,
                Gtk::BUTTONS_OK,
                true);

        infoMessage.run();
        return;
    }

    char theMessage[MESSAGE_LENGTH];
    if ( (m_the1v1Game.GetPlayer1().NumberOfPiecesAvailable() == e_numberOfPieces) &&
         (m_the1v1Game.GetPlayer2().NumberOfPiecesAvailable() == e_numberOfPieces) )
    {
        // the game never started. Restart the user timer
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "This will restart your elapsed time stopwatch. Are you sure?");
    }
    else if ( (Rules::CanPlayerGo(m_the1v1Game.GetBoard(), m_the1v1Game.GetPlayer1()) == false) &&
              (Rules::CanPlayerGo(m_the1v1Game.GetBoard(), m_the1v1Game.GetPlayer2()) == false) )
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
            *m_theWindow,
            theMessage,
            true,
            Gtk::MESSAGE_QUESTION,
            Gtk::BUTTONS_YES_NO,
            true);

    if (cancelCurrentGameMessage.run() == Gtk::RESPONSE_YES)
    {
        // reset the current game, and update the view
        m_the1v1Game.Reset();
        m_boardDrawingArea.CancelLatestPieceDeployedEffect();
        m_boardDrawingArea.Invalidate();
        m_pickPiecesDrawingArea.Invalidate();
        m_showComputerPiecesDrawingArea.Invalidate();
        m_editPieceTable->SetPiece(e_noPiece);

        UpdateScoreStatus();

        // stopwatches must be restarted.
        m_stopWatchLabelUser.Reset();
        m_stopWatchLabelComputer.Reset();

        // restart the progress bar
        m_progressBar.set_fraction(0.0);

        // human user will put down the next piece
        m_boardDrawingArea.SetCurrentPlayer(m_the1v1Game.GetPlayer1());

        // it will be human's go next. Start his/her timer
        m_stopWatchLabelUser.Continue();
    }
}

void MainWindow::MenuItemHelpAbout_Activate()
{
    m_aboutDialog->dialog().run();
    m_aboutDialog->dialog().hide();
}

void MainWindow::BoardDrawingArea_BoardClicked(const Coordinate &a_coord, const Piece &a_piece, const Player &a_player)
{
#ifdef DEBUG_PRINT
    std::cout << a_player.GetName() <<  " clicked in (" << a_coord.m_row << ", " << a_coord.m_col << ")" << std::endl;
#endif

	if (m_workerThread->IsThreadComputingMove())
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
        std::cout << "Cheeky you! Don't try to put a piece where it's not allowed" << std::endl;
#endif
        return;
    }

    // set the cursor to busy in the board drawing area, so the user
    // won't get (very) nervous while the computer is processing the next move
    Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea.get_window();
    if (window)
    {
        window->set_cursor(Gdk::Cursor(Gdk::WATCH));
    }

    //TODO this has to be changed. We should only use the arguments of the signal handler!!!!
    // we are positive the move is valid
    m_the1v1Game.PutDownPiece(
        a_piece,
        a_coord,
        Game1v1::e_Game1v1Player1);

    // update the score on the status bar
    UpdateScoreStatus();

    // stop the stopwatch that counts the user time
    m_stopWatchLabelUser.Stop();
    // start the one that counts the computer time
    m_stopWatchLabelComputer.Continue();
    // show the progress bar
    m_progressBar.show();

    // remove the actual piece being edited from the edit piece drawing area
    // and force the edit piece drawing area to be redraw
    m_editPieceTable->SetPiece(e_noPiece);

    // force the pick pieces drawing area to redraw because a piece has just been deployed
    m_pickPiecesDrawingArea.Invalidate();

    // change the current player and force the board to be redraw to update it with the brand new move
    //TODO If the other user is a human being currentPlayer should be updated accordingly
    //m_boardDrawingArea.SetCurrentPlayer(m_the1v1Game.GetPlayerMe());
    m_boardDrawingArea.UnsetCurrentPlayer();
    //TODO if the other player is a human being the latest piece deployed effect shouldn't be cancelled
    // should only be updated
    m_boardDrawingArea.CancelLatestPieceDeployedEffect();
    m_boardDrawingArea.Invalidate();

    if (!m_workerThread->ComputeMove(m_the1v1Game, a_piece, a_coord))
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
            *m_theWindow,
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
        m_workerThread->Join();

        // exit the app
        m_theWindow->hide();
    }

    return;
}

void MainWindow::NotifyGameFinished()
{
    // reset the cursor (even if it's been already done)
    Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea.get_window();
    if (window)
    {
        window->set_cursor();
    }

    // stop computer's stopwatch (even if it was already done)
    m_stopWatchLabelComputer.Stop();

    // restart the progress bar
    m_progressBar.set_fraction(0.0);

    int32_t squaresLeftHuman = 0;
    int32_t squaresLeftComputer = 0;
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (m_the1v1Game.GetPlayer1().IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftHuman +=
                    m_the1v1Game.GetPlayer1().m_pieces[i].GetNSquares();
        }

        if (m_the1v1Game.GetPlayer2().IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftComputer +=
                    m_the1v1Game.GetPlayer2().m_pieces[i].GetNSquares();
        }
    }

    char theMessage[MESSAGE_LENGTH];
    if (squaresLeftHuman > squaresLeftComputer)
    {
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "<b>Computer</b> has beaten you!\n\tYou have <b>%d</b> squares left and the computer <b>%d</b>",
                squaresLeftHuman,
                squaresLeftComputer);
    }
    else if (squaresLeftComputer > squaresLeftHuman)
    {
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "<b>YOU</b> have beaten the computer!\n\tYou have <b>%d</b> squares left and the computer <b>%d</b>",
                squaresLeftHuman,
                squaresLeftComputer);
    }
    else // squaresLeftComputer == squaresLeftHuman
    {
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "That was a <b>DRAW</b>!\n\tBoth the computer and you have <b>%d</b> squares left",
                squaresLeftHuman);
    }

    //MessageDialog (
    //        Gtk::Window& parent,
    //        const Glib::ustring& message,
    //        bool use_markup=false,
    //        MessageType type=MESSAGE_INFO,
    //        ButtonsType buttons=BUTTONS_OK,
    //        bool modal=false)
    Gtk::MessageDialog::MessageDialog gameOverMessage(
            *m_theWindow,
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

void MainWindow::NotifyMoveComputed()
{
    // after the lock protected loop these variables will contain
    // the latest piece and latest coord deployed
    Piece latestPiece(e_noPiece);
    Coordinate latestCoord(COORD_UNITIALISED, COORD_UNITIALISED);

    G_LOCK(s_computerMoveQueue);
        // save the latest moves on the board
        while (!s_computerMoveQueue.empty())
        {
            latestPiece = s_computerMoveQueue.front().first;
            latestCoord = s_computerMoveQueue.front().second;

            if (latestPiece.GetType() != e_noPiece)
            {
                m_the1v1Game.PutDownPiece(
                        latestPiece,
                        latestCoord,
                        Game1v1::e_Game1v1Player2);
            }

            s_computerMoveQueue.pop();
        }
    G_UNLOCK(s_computerMoveQueue);

    // invalidate the board drawing area to show the new move
    // activating the latest piece deployed effect
    m_boardDrawingArea.Invalidate(
            latestPiece,
            latestCoord,
            m_the1v1Game.GetPlayer2());

	// update computer's squares left
	UpdateScoreStatus();

	// update the computer's pieces left too
    m_showComputerPiecesDrawingArea.Invalidate();

    // there's a few things that have to be done if the user can put down a piece
    if (Rules::CanPlayerGo(m_the1v1Game.GetBoard(), m_the1v1Game.GetPlayer1()))
    {
        //user's go
        // stop computer stopwatch
        m_stopWatchLabelComputer.Stop();
        // resume the user stopwatch
        m_stopWatchLabelUser.Continue();
        // restart the progress bar
        m_progressBar.set_fraction(0.0);

        // human player will be putting down pieces on the board
        m_boardDrawingArea.SetCurrentPlayer(m_the1v1Game.GetPlayer1());

        // restore the mouse cursor
        Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea.get_window();
        if (window)
        {
            window->set_cursor();
        }
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
    int32_t squaresLeftUser = 0;
    int32_t squaresLeftComputer = 0;
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (m_the1v1Game.GetPlayer2().IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftComputer +=
                    m_the1v1Game.GetPlayer2().m_pieces[i].GetNSquares();
        }

        if (m_the1v1Game.GetPlayer1().IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftUser +=
                    m_the1v1Game.GetPlayer1().m_pieces[i].GetNSquares();
        }
    }

    // update the GUI widgets
    std::stringstream theMessage;
    theMessage << m_the1v1Game.GetPlayer2().GetName()
               << ": "
               << std::setfill(' ') << std::setw(2)
               << static_cast<int32_t>(squaresLeftComputer)
               << " left";

    m_computerScoreLabel.set_text(theMessage.str().c_str());

    theMessage.str("");
    theMessage << m_the1v1Game.GetPlayer1().GetName()
               << ": "
               << std::setfill(' ') << std::setw(2)
               << static_cast<int32_t>(squaresLeftUser)
               << " left";

    m_userScoreLabel.set_text(theMessage.str().c_str());
}
