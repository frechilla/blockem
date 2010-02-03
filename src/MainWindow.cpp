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
// with Foobar. If not, see http://www.gnu.org/licenses/.
//
/// @file MainWindow.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Nov-2009  Original development
/// @endhistory
///
// ============================================================================

#include <iostream>
#include "MainWindow.h"
#include "gui_glade.h"

/// @brief uninitialised coord value
static const int32_t COORD_UNITIALISED = 0xffffffff;

/// @brief number of squares of the edit piece board
static const int32_t NSQUARES_EDIT_PIECES_BOARD = 5;

/// message to be shown to the user when he/she requested the
/// application to be closed, and the worker thread is busy computing
static const char MESSAGE_ASK_BEFORE_CLOSE[] =
        "The game is computing the next move. Are you sure do you want to close the application?";

/// length of the custom messages to be shown to the user
static const int32_t MESSAGE_LENGTH = 100;

// drawing area options
static const float DEFAULT_RED   = 0.5;
static const float DEFAULT_GREEN = 0.5;
static const float DEFAULT_BLUE  = 0.5;

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


MainWindow::MainWindow(
		Game1v1 &a_theGame,
		Glib::RefPtr<Gnome::Glade::Xml> a_refXml) throw (GUIException) :
			m_the1v1Game(a_theGame),
			m_editPiece(e_noPiece),
			m_lastCoord(COORD_UNITIALISED, COORD_UNITIALISED),
			m_workerThread(),
			m_aboutDialog(a_refXml),
			m_refXml(a_refXml)
{
    // first of all retrieve the Gtk::window object and set its icon
    m_refXml->get_widget(GUI_MAIN_WINDOW_NAME, m_theWindow);
    if (m_theWindow == NULL)
    {
        throw new GUIException(std::string("couldn't retrieve the MainWindow from the .glade file"));
    }

    Glib::RefPtr< Gdk::Pixbuf > icon;
    try
    {
        icon = Gdk::Pixbuf::create_from_file(GUI_PATH_TO_16PICTURE);
    } catch(...)
    {
        icon.reset();
       std::cerr
           << "WARNING: Exception occurred when setting the 16x16 icon into the Main Window"
           << std::endl;
    }
    if (icon)
    {
        m_theWindow->set_icon(icon);
    }


    // connect the interthread communication (GLib::Dispatcher) to invalidate the
    // board drawing area
    m_signal_moveComputed.connect(
            sigc::mem_fun(*this, &MainWindow::NotifyMoveComputed));

    // connect the interthread communication (GLib::Dispatcher) to notify the
    // user that the game is finished
    m_signal_gameFinished.connect(
            sigc::mem_fun(*this, &MainWindow::NotifyGameFinished));

    // connect the worker thread signal
    m_workerThread.signal_computingFinished().connect(
            sigc::mem_fun(*this, &MainWindow::WorkerThread_computingFinished));
            //sigc::ptr_fun(f) );


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

	m_refXml->get_widget(GUI_DRAWINGAREA_BOARD_NAME, m_boardDrawingArea);
	if (m_boardDrawingArea == NULL)
	{
		throw new GUIException(std::string("Board drawing area retrieval failed"));
	}

	m_refXml->get_widget(GUI_DRAWINGAREA_EDITING_PIECE, m_editPiecesDrawingArea);
	if (m_editPiecesDrawingArea == NULL)
	{
		throw new GUIException(std::string("Edit pieces drawing area retrieval failed"));
	}

	m_refXml->get_widget(GUI_BUTTON_ROTATE_NAME, m_rotateButton);
	if (m_rotateButton == NULL)
	{
		throw new GUIException(std::string("rotate button retrieval failed"));
	}

	m_refXml->get_widget(GUI_BUTTON_MIRROR_NAME, m_mirrorButton);
	if (m_mirrorButton == NULL)
	{
		throw new GUIException(std::string("mirror button retrieval failed"));
	}

	// toggle buttons to select the pieces
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_1BABY, m_selectPieceToggleButtons[e_1Piece_BabyPiece]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_2LONG, m_selectPieceToggleButtons[e_2Piece_TwoPiece]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_3LONG, m_selectPieceToggleButtons[e_3Piece_LongPiece]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_3TRIANGLE, m_selectPieceToggleButtons[e_3Piece_Triangle]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_4LONG, m_selectPieceToggleButtons[e_4Piece_LongPiece]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_4LITTEL_S, m_selectPieceToggleButtons[e_4Piece_LittleS]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_4LITTLE_T, m_selectPieceToggleButtons[e_4Piece_LittleT]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_4LITTLE_L, m_selectPieceToggleButtons[e_4Piece_LittleL]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_4FULLSQUARE, m_selectPieceToggleButtons[e_4Piece_FullSquare]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5BIG_S, m_selectPieceToggleButtons[e_5Piece_BigS]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5SAF, m_selectPieceToggleButtons[e_5Piece_SafPiece]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5W, m_selectPieceToggleButtons[e_5Piece_WPiece]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5CUNT, m_selectPieceToggleButtons[e_5Piece_CuntPiece]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5LONG, m_selectPieceToggleButtons[e_5Piece_BigPenis]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5CROSS, m_selectPieceToggleButtons[e_5Piece_Cross]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5HALFSQUARE, m_selectPieceToggleButtons[e_5Piece_HalfSquare]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5BIG_L, m_selectPieceToggleButtons[e_5Piece_BigL]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5MR_T, m_selectPieceToggleButtons[e_5Piece_MrT]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5SQUAREPLUS, m_selectPieceToggleButtons[e_5Piece_SquarePlus]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5BORING, m_selectPieceToggleButtons[e_5Piece_BoringPiece]);
	m_refXml->get_widget(GUI_TOGGBTN_PIECE_5ULTIMATE, m_selectPieceToggleButtons[e_5Piece_TheUltimate]);

	for (int8_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
	{
		if (m_selectPieceToggleButtons[i] == NULL)
		{
			throw new GUIException(std::string("toggle button retrieval failed for piece " + i));
		}
	}

	// connect the signals to the handlers
	// if the handler is not part of an object use sigc::ptr_fun
	m_theWindow->signal_delete_event().connect(
	        sigc::mem_fun(*this, &MainWindow::MainWindow_DeleteEvent));
	m_newMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemGameNew_Activate));
	m_quitMenuItem->signal_activate().connect(
	        sigc::mem_fun(*this, &MainWindow::MenuItemGameQuit_Activate));
	m_helpAboutMenuItem->signal_activate().connect(
	            sigc::mem_fun(*this, &MainWindow::MenuItemHelpAbout_Activate));
	m_boardDrawingArea->signal_expose_event().connect(
			sigc::mem_fun(*this, &MainWindow::BoardDrawingArea_ExposeEvent));
	m_boardDrawingArea->add_events(Gdk::BUTTON_PRESS_MASK);
	m_boardDrawingArea->signal_button_press_event().connect(
			sigc::mem_fun(*this, &MainWindow::BoardDrawingArea_ButtonPressed));
	m_boardDrawingArea->add_events(Gdk::POINTER_MOTION_MASK);
	m_boardDrawingArea->signal_motion_notify_event().connect(
			sigc::mem_fun(*this, &MainWindow::BoardDrawingArea_MotionNotify));

	m_editPiecesDrawingArea->signal_expose_event().connect(
			sigc::mem_fun(*this, &MainWindow::EditPiecesDrawingArea_ExposeEvent));

	m_rotateButton->signal_clicked().connect(
			sigc::mem_fun(*this, &MainWindow::RotateButton_ButtonPressed));
	m_mirrorButton->signal_clicked().connect(
			sigc::mem_fun(*this, &MainWindow::MirrorButton_ButtonPressed));


	m_selectPieceToggleButtons[e_1Piece_BabyPiece]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_1baby));
	m_selectPieceToggleButtons[e_2Piece_TwoPiece]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_2long));
	m_selectPieceToggleButtons[e_3Piece_LongPiece]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_3long));
	m_selectPieceToggleButtons[e_3Piece_Triangle]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_3triangle));
	m_selectPieceToggleButtons[e_4Piece_LongPiece]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_4long));
	m_selectPieceToggleButtons[e_4Piece_LittleS]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_4littleS));
	m_selectPieceToggleButtons[e_4Piece_LittleT]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_4littleT));
	m_selectPieceToggleButtons[e_4Piece_LittleL]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_4littleL));
	m_selectPieceToggleButtons[e_4Piece_FullSquare]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_4fullSquare));
	m_selectPieceToggleButtons[e_5Piece_BigS]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5bigS));
	m_selectPieceToggleButtons[e_5Piece_SafPiece]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5saf));
	m_selectPieceToggleButtons[e_5Piece_WPiece]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5w));
	m_selectPieceToggleButtons[e_5Piece_CuntPiece]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5cunt));
	m_selectPieceToggleButtons[e_5Piece_BigPenis]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5penis));
	m_selectPieceToggleButtons[e_5Piece_Cross]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5cross));
	m_selectPieceToggleButtons[e_5Piece_HalfSquare]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5halfSquare));
	m_selectPieceToggleButtons[e_5Piece_BigL]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5bigL));
	m_selectPieceToggleButtons[e_5Piece_MrT]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5mrT));
	m_selectPieceToggleButtons[e_5Piece_SquarePlus]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5squarePlusPlus));
	m_selectPieceToggleButtons[e_5Piece_BoringPiece]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5boring));
	m_selectPieceToggleButtons[e_5Piece_TheUltimate]->signal_toggled().connect(
			sigc::mem_fun(*this, &MainWindow::Togglebtn_5ultimate));

	// set the rotate and mirror button to not sensitive, since at the beginning
	// there's no piece loaded in the edit piece drawing area
	m_rotateButton->set_sensitive(false);
	m_mirrorButton->set_sensitive(false);
}

MainWindow::~MainWindow()
{
    // MainWindow_DeleteEvent or MenuItemGameQuit_Activate take care of killing
    /// the thread when necessary.
    // All MainWindow needs is waiting until the worker thread is properly dead
    // Joining the thread suspends the execution of this thread to wait for the other
    // If pthread_join is called twice over the same thread, it will fail,
    // but the app won't core
    m_workerThread.Join();

    delete (m_theWindow);
}

void MainWindow::WorkerThread_computingFinished(
        const Piece      &a_piece,
        const Coordinate &a_coord,
        int32_t           a_returnValue)
{
    if (a_piece.GetType() != e_noPiece)
    {
        m_the1v1Game.PutDownPieceMe(a_piece, a_coord);
    }

    // this signal is being issued from the MainWindowWorkerThread
    // GTK is not thread safe, so every GUI function should be called from the same thread
    // we need inter-thread communication to force the main thread to invalidate the
    // board drawing area
    // have a look at:
    //     http://library.gnome.org/devel/glibmm/stable/thread_2dispatcher_8cc-example.html
    m_signal_moveComputed.emit();

    if (a_piece.GetType() == e_noPiece)
    {
        if (m_the1v1Game.CanPlayerOpponentGo() == false)
        {
            // the game is over. Computing returned e_noPiece and
            // the opponent can't go
            m_signal_gameFinished.emit();
        }
        else
        {
            std::cout << "Computer can't move" << std::endl;
        }
    }
}

bool MainWindow::MainWindow_DeleteEvent(GdkEventAny*)
{
    if (m_workerThread.IsThreadComputingMove())
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

        // the thread is computing a new move, but the user wants to exit.
        // we don't know how long it will take to join, so we "killed" the thread
        m_workerThread.Terminate();
    }
    else
    {
        m_workerThread.Join();
    }

    // continue with delete event
    return false;
}

void MainWindow::MenuItemGameQuit_Activate()
{
    if (m_workerThread.IsThreadComputingMove())
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

        // the thread is computing a new move, but the user wants to exit.
        // we don't know how long it will take to join, so we "killed" the thread
        m_workerThread.Terminate();
    }
    else
    {
        m_workerThread.Join();
    }

    // exit the app
    m_theWindow->hide();
}

void MainWindow::MenuItemGameNew_Activate()
{
    if (m_workerThread.IsThreadComputingMove())
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
    if ( (m_the1v1Game.GetPlayerMe().NumberOfPiecesAvailable() == e_numberOfPieces) &&
         (m_the1v1Game.GetPlayerOpponent().NumberOfPiecesAvailable() == e_numberOfPieces) )
    {
        // the game never started. Do nothing
        return;
    }
    else if ( (m_the1v1Game.CanPlayerOpponentGo() == false) &&
              (m_the1v1Game.CanPlayerMeGo() == false) )
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
        InvalidateDrawingArea(m_boardDrawingArea);
        InvalidateDrawingArea(m_editPiecesDrawingArea);
        m_rotateButton->set_sensitive(false);
        m_mirrorButton->set_sensitive(false);

        for (int8_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
        {
            if (m_selectPieceToggleButtons[i]->sensitive() == false)
            {
                m_selectPieceToggleButtons[i]->set_sensitive(true);
            }
        }

        UpdateSelectedPiece(e_noPiece);
    }
}

void MainWindow::MenuItemHelpAbout_Activate()
{
    m_aboutDialog.dialog().run();
    m_aboutDialog.dialog().hide();
}

bool MainWindow::BoardDrawingArea_ExposeEvent(GdkEventExpose* event)
{
	//std::cout << "in BoardDrawingArea_ExposeEvent" << std::endl;

	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea->get_window();
	if(window)
	{
		Gtk::Allocation allocation = m_boardDrawingArea->get_allocation();

		int32_t width  = allocation.get_width();
		int32_t height = allocation.get_height();
		int32_t squareSize = std::min(width, height);

		int32_t littleSquareHeight = squareSize / m_the1v1Game.GetBoard().GetNColumns();
		int32_t littleSquareWidth  = squareSize / m_the1v1Game.GetBoard().GetNRows();

		int32_t squareHeight = littleSquareHeight * m_the1v1Game.GetBoard().GetNColumns();
		int32_t squareWidth  = littleSquareWidth  * m_the1v1Game.GetBoard().GetNRows();

		// coordinates for the centre of the window
		int32_t xc = width  / 2;
		int32_t yc = height / 2;

		// get the pen to draw
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

		// clip to the area indicated by the expose event so that we only redraw
		// the portion of the window that needs to be redrawn
		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);
		cr->clip();

		// if there's no pieces on the board now, draw a small circle in the starting point
        if (m_the1v1Game.GetPlayerOpponent().NumberOfPiecesAvailable() == e_numberOfPieces)
        {
            cr->set_source_rgba(
                    PLAYER_OPPONENT_RED,
                    PLAYER_OPPONENT_GREEN,
                    PLAYER_OPPONENT_BLUE,
                    GHOST_PIECE_ALPHA_TRANSPARENCY);

            cr->arc(xc - squareWidth/2  +
                        littleSquareWidth*STARTING_COORD_X_OPPONENT + littleSquareWidth/2,
                    yc - squareHeight/2 +
                        littleSquareHeight*STARTING_COORD_Y_OPPONENT + littleSquareHeight/2,
                    std::min(littleSquareHeight, littleSquareWidth) / 2 -
                            std::min(littleSquareHeight, littleSquareWidth) / 4,
                    0.0,
                    2 * M_PI);

            cr->fill();
        }

		if (m_the1v1Game.GetPlayerMe().NumberOfPiecesAvailable() == e_numberOfPieces)
		{
	        cr->set_source_rgba(
	                PLAYER_ME_RED,
	                PLAYER_ME_GREEN,
	                PLAYER_ME_BLUE,
	                GHOST_PIECE_ALPHA_TRANSPARENCY);

            cr->arc(xc - squareWidth/2  +
                        littleSquareWidth*STARTING_COORD_X_ME + littleSquareWidth/2,
                    yc - squareHeight/2 +
                        littleSquareHeight*STARTING_COORD_Y_ME + littleSquareHeight/2,
                    std::min(littleSquareHeight, littleSquareWidth) / 2 -
                            std::min(littleSquareHeight, littleSquareWidth) / 4,
                    0.0,
                    2 * M_PI);

	        cr->fill();
		}

		// fill the squares where the player 'me' is in right now
		cr->set_source_rgb(
				PLAYER_ME_RED,
				PLAYER_ME_GREEN,
				PLAYER_ME_BLUE);
	    for (int32_t rowCount = 0;
                     rowCount < m_the1v1Game.GetBoard().GetNRows() ;
                     rowCount++)
	    {
	        for (int32_t columnCount = 0;
                         columnCount <  m_the1v1Game.GetBoard().GetNColumns() ;
                         columnCount++)
	        {
	            if (m_the1v1Game.GetBoard().IsPlayerInCoord(
	            		rowCount,
	            		columnCount,
	            		m_the1v1Game.GetPlayerMe()))
	            {
	            	SetSquareInBoard(Coordinate(rowCount, columnCount), cr);
	            }
	        }
	    }


		// fill the squares where the player 'opponent' is in right now
		cr->set_source_rgb(
				PLAYER_OPPONENT_RED,
				PLAYER_OPPONENT_GREEN,
				PLAYER_OPPONENT_BLUE);
	    for (int32_t rowCount = 0;
                     rowCount < m_the1v1Game.GetBoard().GetNRows() ;
                     rowCount++)
	    {
	        for (int32_t columnCount = 0;
                         columnCount <  m_the1v1Game.GetBoard().GetNColumns() ;
                         columnCount++)
	        {
	            if (m_the1v1Game.GetBoard().IsPlayerInCoord(
	            		rowCount,
	            		columnCount,
	            		m_the1v1Game.GetPlayerOpponent()))
	            {
	            	SetSquareInBoard(Coordinate(rowCount, columnCount), cr);
	            }
	        }
	    }

        // line width and colour for the border of the board
        cr->set_line_width(BOARD_BORDER_LINE_WIDTH);
        cr->set_source_rgb(
                BOARD_BORDER_RED,
                BOARD_BORDER_GREEN,
                BOARD_BORDER_BLUE);

        // draw the border of the board
        cr->move_to(xc - squareWidth/2, yc - squareHeight/2);
        cr->line_to(xc + squareWidth/2, yc - squareHeight/2);
        cr->line_to(xc + squareWidth/2, yc + squareHeight/2);
        cr->line_to(xc - squareWidth/2, yc + squareHeight/2);
        cr->line_to(xc - squareWidth/2, yc - squareHeight/2);

        // commit the changes to the screen!
        cr->stroke();

        // line for the insides of the board
        cr->set_line_width(BOARD_LINE_WIDTH);
        cr->set_source_rgb(
                BOARD_RED,
                BOARD_GREEN,
                BOARD_BLUE);

        // draw the inside lines of the board
        for (int32_t i = 1; i < m_the1v1Game.GetBoard().GetNRows(); i++)
        {
            cr->move_to(xc - squareWidth/2 + littleSquareWidth*i, yc - squareHeight/2);
            cr->line_to(xc - squareWidth/2 + littleSquareWidth*i, yc + squareHeight/2);
        }

        for (int32_t i = 1; i < m_the1v1Game.GetBoard().GetNColumns(); i++)
        {
            cr->move_to(xc - squareWidth/2, yc - squareHeight/2 + littleSquareHeight*i);
            cr->line_to(xc + squareWidth/2, yc - squareHeight/2 + littleSquareHeight*i);
        }

        // commit the changes to the screen!
        cr->stroke();


	    // print the current selected piece in the place where the mouse pointer is
	    // using a bit of transparency
	    if ( (m_editPiece.GetType() != e_noPiece)   &&
             (m_lastCoord.m_X != COORD_UNITIALISED) &&
             (m_lastCoord.m_Y != COORD_UNITIALISED) )
	    {
	    	if (m_the1v1Game.GetPlayerOpponent().NumberOfPiecesAvailable() == e_numberOfPieces)
	    	{
	    		if (Rules::IsPieceDeployableInStartingPoint(
		    			m_the1v1Game.GetBoard(),
		    			m_editPiece,
		    			m_lastCoord,
		    			Coordinate(STARTING_COORD_X_OPPONENT, STARTING_COORD_Y_OPPONENT)))
	    		{
					cr->set_source_rgba(
							GHOST_PIECE_RIGHT_RED,
							GHOST_PIECE_RIGHT_GREEN,
							GHOST_PIECE_RIGHT_BLUE,
							GHOST_PIECE_ALPHA_TRANSPARENCY);
	    		}
	    		else
	    		{
					cr->set_source_rgba(
							GHOST_PIECE_WRONG_RED,
							GHOST_PIECE_WRONG_GREEN,
							GHOST_PIECE_WRONG_BLUE,
							GHOST_PIECE_ALPHA_TRANSPARENCY);
	    		}
	    	}
	    	else if (Rules::IsPieceDeployable(
	    			m_the1v1Game.GetBoard(),
	    			m_editPiece,
	    			m_lastCoord,
	    			m_the1v1Game.GetPlayerOpponent()))
	    	{
				cr->set_source_rgba(
						GHOST_PIECE_RIGHT_RED,
						GHOST_PIECE_RIGHT_GREEN,
						GHOST_PIECE_RIGHT_BLUE,
						GHOST_PIECE_ALPHA_TRANSPARENCY);
	    	}
	    	else
	    	{
				cr->set_source_rgba(
						GHOST_PIECE_WRONG_RED,
						GHOST_PIECE_WRONG_GREEN,
						GHOST_PIECE_WRONG_BLUE,
						GHOST_PIECE_ALPHA_TRANSPARENCY);
	    	}

			// draw the piece in the edit pieces drawing area
			for (uint8_t i = 0; i < m_editPiece.GetNSquares(); i++)
			{
				SetSquareInBoard(
						Coordinate(
								m_lastCoord.m_X + m_editPiece.m_coords[i].m_X,
								m_lastCoord.m_Y + m_editPiece.m_coords[i].m_Y),
						cr);
				cr->fill();
			}
	    }
	}

	return true;
}

bool MainWindow::BoardDrawingArea_ButtonPressed(GdkEventButton *event)
{
    //std::cout << "clicked in (" << event->x << ", " << event->y << ")" << std::endl;

	Coordinate thisCoord;
	if ( (m_editPiece.GetType() == e_noPiece) ||
	     (WindowToBoardCoord(event->x, event->y, thisCoord) == false) )
	{
	    return true;
	}

	if (m_workerThread.IsThreadComputingMove())
	{
	    std::cout
            << "The worker thread is busy. Please be patient (a polite way to say: 'Fuck off')"
            << std::endl;

	    return true;
	}

	std::cout << "SQUARE: (" << thisCoord.m_X << ", " << thisCoord.m_Y << ")" << std::endl;

    if( ( (m_the1v1Game.GetPlayerOpponent().NumberOfPiecesAvailable() == e_numberOfPieces) &&
          (!Rules::IsPieceDeployableInStartingPoint(
                    m_the1v1Game.GetBoard(),
                    m_editPiece,
                    m_lastCoord,
                    Coordinate(STARTING_COORD_X_OPPONENT, STARTING_COORD_Y_OPPONENT))) ) ||
        ( (m_the1v1Game.GetPlayerOpponent().NumberOfPiecesAvailable() < e_numberOfPieces) &&
          (!Rules::IsPieceDeployable(
                    m_the1v1Game.GetBoard(),
                    m_editPiece,
                    thisCoord,
                    m_the1v1Game.GetPlayerOpponent())) ) )
    {
        std::cout << "Cheeky you! Don't try to put a piece where it's not allowed" << std::endl;
    }
    else
    {
        // set the cursor to busy in the board drawing area, so the user
        // won't get (very) nervous while the computer is processing the next move
        Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea->get_window();
        if (window)
        {
            window->set_cursor(Gdk::Cursor(Gdk::WATCH));
        }

        // we are positive the move is valid
        m_the1v1Game.PutDownPieceOpponent(m_editPiece, thisCoord);

        if (!m_workerThread.ComputeMove(m_the1v1Game, m_editPiece, thisCoord))
        {
            std::cout
                << "Error while telling the thread to start computing."
                << std::endl
                << "The worker thread seems to be busy"
                << std::endl;
        }

        // disable (make it not-clickable) the toggle button that represents the
        // piece what was just put down
        m_selectPieceToggleButtons[m_editPiece.GetType()]->set_sensitive(false);
        // remove the actual piece being edited from the edit piece drawing area
        UpdateSelectedPiece(e_noPiece);

        // force the board to be redraw
        InvalidateDrawingArea(m_boardDrawingArea);
    }

    return true;
}

bool MainWindow::BoardDrawingArea_MotionNotify(GdkEventMotion *event)
{
    //std::cout << "moved to (" << event->x << ", " << event->y << ")" << std::endl;

	Coordinate thisCoord;
	if (WindowToBoardCoord(event->x, event->y, thisCoord))
	{
		if ( (m_lastCoord.m_X != thisCoord.m_X) ||
             (m_lastCoord.m_Y != thisCoord.m_Y) )
		{
			m_lastCoord.m_X = thisCoord.m_X;
			m_lastCoord.m_Y = thisCoord.m_Y;

			// force the board to be redraw
			InvalidateDrawingArea(m_boardDrawingArea);
		}
	}
	else if ( (m_lastCoord.m_X != COORD_UNITIALISED) ||
              (m_lastCoord.m_Y != COORD_UNITIALISED) )
	{
		m_lastCoord.m_X = m_lastCoord.m_Y = COORD_UNITIALISED;

		// force the board to be redraw
		InvalidateDrawingArea(m_boardDrawingArea);
	}

	return true;
}

bool MainWindow::EditPiecesDrawingArea_ExposeEvent(GdkEventExpose* event)
{
	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = m_editPiecesDrawingArea->get_window();
	if(window)
	{
		Gtk::Allocation allocation = m_editPiecesDrawingArea->get_allocation();

		int32_t width  = allocation.get_width();
		int32_t height = allocation.get_height();
		int32_t squareSize = std::min(width, height);

		int32_t littleSquare = squareSize / NSQUARES_EDIT_PIECES_BOARD;

		int32_t squareHeight = littleSquare * NSQUARES_EDIT_PIECES_BOARD;
		int32_t squareWidth  = littleSquare * NSQUARES_EDIT_PIECES_BOARD;

		// coordinates for the centre of the window
		int32_t xc = width  / 2;
		int32_t yc = height / 2;

		// get the pen to draw
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

		// clip to the area indicated by the expose event so that we only redraw
		// the portion of the window that needs to be redrawn
		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);
		cr->clip();

		// print the piece in the edit piece drawing area
        if (m_editPiece.GetType() != e_noPiece)
        {
            cr->set_source_rgb(
                    PLAYER_OPPONENT_RED,
                    PLAYER_OPPONENT_GREEN,
                    PLAYER_OPPONENT_BLUE);

            // draw the piece in the edit pieces drawing area
            for (uint8_t i = 0; i < m_editPiece.GetNSquares(); i++)
            {
                cr->rectangle(
                        (xc - littleSquare/2) + (littleSquare * m_editPiece.m_coords[i].m_X) + 1,
                        (yc - littleSquare/2) + (littleSquare * m_editPiece.m_coords[i].m_Y) + 1,
                        littleSquare - 2,
                        littleSquare - 2);

                cr->fill();
            }
        }

        // print the small little board to edit the piece easier
		// we'll use same colour and width as for the inside of the board
		cr->set_line_width(BOARD_LINE_WIDTH);
		cr->set_source_rgb(
				BOARD_RED,
				BOARD_GREEN,
				BOARD_BLUE);

		// draw the border of the board
		cr->move_to(xc - squareWidth/2, yc - squareHeight/2);
		cr->line_to(xc + squareWidth/2, yc - squareHeight/2);
		cr->line_to(xc + squareWidth/2, yc + squareHeight/2);
		cr->line_to(xc - squareWidth/2, yc + squareHeight/2);
		cr->line_to(xc - squareWidth/2, yc - squareHeight/2);


		// draw the inside lines of the board
		for (int32_t i = 1; i < NSQUARES_EDIT_PIECES_BOARD; i++)
		{
			cr->move_to(xc - squareWidth/2 + littleSquare*i, yc - squareHeight/2);
			cr->line_to(xc - squareWidth/2 + littleSquare*i, yc + squareHeight/2);

			cr->move_to(xc - squareWidth/2, yc - squareHeight/2 + littleSquare*i);
			cr->line_to(xc + squareWidth/2, yc - squareHeight/2 + littleSquare*i);
		}

		// commit the changes to the screen!
		cr->stroke();
	}

	return true;
}

void MainWindow::RotateButton_ButtonPressed()
{
	m_editPiece.Rotate();
	InvalidateDrawingArea(m_editPiecesDrawingArea);
}

void MainWindow::MirrorButton_ButtonPressed()
{
	m_editPiece.Mirror();
	InvalidateDrawingArea(m_editPiecesDrawingArea);
}

void MainWindow::Togglebtn_1baby()
    {
        UpdateSelectedPiece(e_1Piece_BabyPiece);
    }

void MainWindow::Togglebtn_2long()
{
    UpdateSelectedPiece(e_2Piece_TwoPiece);
}

void MainWindow::Togglebtn_3long()
{
    UpdateSelectedPiece(e_3Piece_LongPiece);
}

void MainWindow::Togglebtn_3triangle()
{
    UpdateSelectedPiece(e_3Piece_Triangle);
}

void MainWindow::Togglebtn_4long()
{
    UpdateSelectedPiece(e_4Piece_LongPiece);
}

void MainWindow::Togglebtn_4littleS()
{
    UpdateSelectedPiece(e_4Piece_LittleS);
}

void MainWindow::Togglebtn_4littleL()
{
    UpdateSelectedPiece(e_4Piece_LittleL);
}

void MainWindow::Togglebtn_4littleT()
{
    UpdateSelectedPiece(e_4Piece_LittleT);
}

void MainWindow::Togglebtn_4fullSquare()
{
    UpdateSelectedPiece(e_4Piece_FullSquare);
}

void MainWindow::Togglebtn_5bigS()
{
    UpdateSelectedPiece(e_5Piece_BigS);
}

void MainWindow::Togglebtn_5saf()
{
    UpdateSelectedPiece(e_5Piece_SafPiece);
}

void MainWindow::Togglebtn_5w()
{
    UpdateSelectedPiece(e_5Piece_WPiece);
}

void MainWindow::Togglebtn_5cunt()
{
    UpdateSelectedPiece(e_5Piece_CuntPiece);
}

void MainWindow::Togglebtn_5penis()
{
    UpdateSelectedPiece(e_5Piece_BigPenis);
}

void MainWindow::Togglebtn_5cross()
{
    UpdateSelectedPiece(e_5Piece_Cross);
}

void MainWindow::Togglebtn_5halfSquare()
{
    UpdateSelectedPiece(e_5Piece_HalfSquare);
}

void MainWindow::Togglebtn_5bigL()
{
    UpdateSelectedPiece(e_5Piece_BigL);
}

void MainWindow::Togglebtn_5mrT()
{
    UpdateSelectedPiece(e_5Piece_MrT);
}

void MainWindow::Togglebtn_5squarePlusPlus()
{
    UpdateSelectedPiece(e_5Piece_SquarePlus);
}

void MainWindow::Togglebtn_5boring()
{
    UpdateSelectedPiece(e_5Piece_BoringPiece);
}

void MainWindow::Togglebtn_5ultimate()
{
    UpdateSelectedPiece(e_5Piece_TheUltimate);
}

bool MainWindow::WindowToBoardCoord(
		int32_t a_windowX,
		int32_t a_windowY,
		Coordinate &out_boardCoord)
{
	Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea->get_window();
	if (!window)
	{
		return false;
	}

	Gtk::Allocation allocation = m_boardDrawingArea->get_allocation();

	int32_t width  = allocation.get_width();
	int32_t height = allocation.get_height();
	int32_t squareSize = std::min(width, height);

	int32_t littleSquareHeight = squareSize / m_the1v1Game.GetBoard().GetNColumns();
	int32_t littleSquareWidth  = squareSize / m_the1v1Game.GetBoard().GetNRows();

	int32_t squareHeight = littleSquareHeight * m_the1v1Game.GetBoard().GetNColumns();
	int32_t squareWidth  = littleSquareWidth  * m_the1v1Game.GetBoard().GetNRows();

	int32_t xc = width  / 2;
	int32_t yc = height / 2;

	if ( ( (a_windowX > (xc - squareWidth/2))  && (a_windowX < (xc + squareWidth/2))  ) &&
         ( (a_windowY > (yc - squareHeight/2)) && (a_windowY < (yc + squareHeight/2)) ) )
	{
		int32_t x, y;
		for (x = 0; x < m_the1v1Game.GetBoard().GetNRows(); x++)
		{
			if (a_windowX < ((xc - squareWidth/2) + (littleSquareWidth * x)))
			{
				break;
			}
		}

		for (y = 1; y < m_the1v1Game.GetBoard().GetNColumns(); y++)
		{
			if (a_windowY < ((yc - squareHeight/2) + (littleSquareHeight * y)))
			{
				break;
			}
		}

		out_boardCoord.m_X = x - 1;
		out_boardCoord.m_Y = y - 1;

		return true;
	}

	return false;
}

void MainWindow::SetSquareInBoard(const Coordinate &a_coord, Cairo::RefPtr<Cairo::Context> cr)
{
	Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea->get_window();
	if (window)
	{
		Gtk::Allocation allocation = m_boardDrawingArea->get_allocation();

		int32_t width  = allocation.get_width();
		int32_t height = allocation.get_height();
		int32_t squareSize = std::min(width, height);

		int32_t littleSquareHeight = squareSize / m_the1v1Game.GetBoard().GetNColumns();
		int32_t littleSquareWidth  = squareSize / m_the1v1Game.GetBoard().GetNRows();

		int32_t squareHeight = littleSquareHeight * m_the1v1Game.GetBoard().GetNColumns();
		int32_t squareWidth  = littleSquareWidth  * m_the1v1Game.GetBoard().GetNRows();

		int32_t xc = width  / 2;
		int32_t yc = height / 2;

		cr->rectangle(
				(xc - squareWidth/2) +  (littleSquareWidth  * a_coord.m_X) + 1,
				(yc - squareHeight/2) + (littleSquareHeight * a_coord.m_Y) + 1,
				littleSquareWidth  - 2,
				littleSquareHeight - 2);

		cr->fill();
	}
}

void MainWindow::UpdateSelectedPiece(ePieceType_t a_newPiece)
{
    if ( (a_newPiece < e_numberOfPieces) &&
         (m_selectPieceToggleButtons[a_newPiece]->get_active() == false) )
    {
        return;
    }

    for (int8_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
    {
        if (static_cast<ePieceType_t>(i) != a_newPiece)
        {
            m_selectPieceToggleButtons[i]->set_active(false);
        }
    }

    m_editPiece = Piece(a_newPiece);
    if (m_editPiece.CanRotateOriginally())
    {
        m_rotateButton->set_sensitive(true);
    }
    else
    {
        m_rotateButton->set_sensitive(false);
    }
    if (m_editPiece.CanMirrorOriginally())
    {
        m_mirrorButton->set_sensitive(true);
    }
    else
    {
        m_mirrorButton->set_sensitive(false);
    }

    InvalidateDrawingArea(m_editPiecesDrawingArea);
}

void MainWindow::NotifyGameFinished()
{
    // reset the cursor (even if it's been already done)
    Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea->get_window();
    if (window)
    {
        window->set_cursor();
    }

    int32_t squaresLeftOpponent = 0;
    int32_t squaresLeftMe = 0;
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (m_the1v1Game.GetPlayerMe().IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftMe +=
                    m_the1v1Game.GetPlayerMe().m_pieces[i].GetNSquares();
        }

        if (m_the1v1Game.GetPlayerOpponent().IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftOpponent +=
                    m_the1v1Game.GetPlayerOpponent().m_pieces[i].GetNSquares();
        }
    }

    char theMessage[MESSAGE_LENGTH];
    if (squaresLeftOpponent > squaresLeftMe)
    {
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "<b>Computer</b> has beaten you!\n\tYou have <b>%d</b> squares left and the computer <b>%d</b>",
                squaresLeftOpponent,
                squaresLeftMe);
    }
    else if (squaresLeftMe > squaresLeftOpponent)
    {
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "<b>YOU</b> have beaten the computer!\n\tYou have <b>%d</b> squares left and the computer <b>%d</b>",
                squaresLeftOpponent,
                squaresLeftMe);
    }
    else // squaresLeftMe > squaresLeftOpponent
    {
        snprintf(theMessage,
                MESSAGE_LENGTH,
                "That was a <b>DRAW</b>!\n\tBoth the computer and you have <b>%d</b> squares left",
                squaresLeftOpponent);
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
        ; // the dialog only have 1 button
    }
}

void MainWindow::NotifyMoveComputed()
{
    // invalidate the board drawing area
    InvalidateDrawingArea(m_boardDrawingArea);

    // restore the mouse cursor if the user can put down a piece
    // or if the game is finished
    if (m_the1v1Game.CanPlayerOpponentGo())
    {
        Glib::RefPtr<Gdk::Window> window = m_boardDrawingArea->get_window();
        if (window)
        {
            window->set_cursor();
        }
    }
}

bool MainWindow::InvalidateDrawingArea(Gtk::DrawingArea* a_drawingArea)
{
    // force the drawing area to be redraw
    Glib::RefPtr<Gdk::Window> window = a_drawingArea->get_window();
    if(window)
    {
        Gdk::Rectangle rect(
                0,
                0,
                a_drawingArea->get_allocation().get_width(),
                a_drawingArea->get_allocation().get_height());

        window->invalidate_rect(rect, false);
        return true;
    }

    return false;
}
