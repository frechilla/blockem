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
/// @file gui_game1v1_widget.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 05-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef _GAME1V1_WIDGET_H_
#define _GAME1V1_WIDGET_H_

#include <string>
#include <exception>
#include <gtkmm.h>

#include "g_blocking_queue.h"
#include "gui_about_dialog.h"
#include "gui_game1v1_config_dialog.h"
#include "gui_drawing_area_show_pieces.h"
#include "gui_drawing_area_board.h"
#include "gui_exception.h"
#include "gui_main_window_worker_thread.h"
#include "gui_stop_watch_label.h"
#include "gui_table_edit_piece.h"
#include "gui_game_statusbar.h"
#include "game1v1.h"
#include "coordinate.h"
#include "piece.h"


/// @brief class to handle the game1v1 widget
/// It can be added to a window and it would handle its own messages
class Game1v1Widget :
    public Gtk::VBox
{
public:
    /// Instantiates the class. It needs a Gtk::Builder object to retrieve the
    /// glade info
    // to be used with m_gtkBuilder->get_widget_derived(GUI_VBOX_GAME1V1, m_game1v1Widget)
    Game1v1Widget(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException);
    virtual ~Game1v1Widget();

    /// @brief callback to be called whenever the worker thread finishes computing a move
    void WorkerThread_computingFinished(
            const Piece              &a_piece,
            const Coordinate         &a_coord,
            Game1v1::eGame1v1Player_t a_playerToMove,
            int32_t                   a_returnValue);

    /// @brief callback to be called when the window is about to be closed
    ///        using the X on the corner
    bool MainWindow_DeleteEvent(GdkEventAny*);

    /// @brief callback to be called when the window is about to be closed
    ///        using the menutiem Game->quit
    void MenuItemGameQuit_Activate();

    /// @brief callback to be called when the menuitem Game->new is pressed
    void MenuItemGameNew_Activate();

    /// @brief callback to be called when the menuitem Help->about is pressed
    void MenuItemHelpAbout_Activate();

    /// @brief callback to be called when the menuitem Settings->Preferences is pressed
    void MenuItemSettingsPreferences_Activate();

    /// @brief  callback to be called when the menuitem Settings->view nk points is toggled
    void MenuItemSettingsViewNKPoints_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->show player1's forbidden area is toggled
    void MenuItemSettingsShowPlayer1ForbiddenArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->show player2 forbidden area is toggled
    void MenuItemSettingsShowPlayer2ForbiddenArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->do not show forbidden area is toggled
    void MenuItemSettingsShowNoneForbiddenArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->show player1's influence area is toggled
    void MenuItemSettingsShowPlayer1InfluenceArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->show player2 influence area is toggled
    void MenuItemSettingsShowPlayer2InfluenceArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->do not show influence area is toggled
    void MenuItemSettingsShowNoneInfluenceArea_Toggled();

    /// @brief callback to be called when a button is pressed inside the board
    void BoardDrawingArea_BoardClicked(const Coordinate &, const Piece &, const Player &);

    /// @brief to be used as a functor so Game1v1 notifies this class the progress of the computing
    ///        process for computer's next move
    /// this function can be called from a different thread because it uses signal dispatcher
    /// see: http://library.gnome.org/devel/glibmm/stable/thread_2dispatcher_8cc-example.html
    static void ProgressUpdate(float a_progress);

private:

    /// struct which contains the info that stores a move (piece + where + who)
    /// it is for internal use within the MainWindow class
    typedef struct
    {
        Piece                     piece;
        Coordinate                coord;
        Game1v1::eGame1v1Player_t playerToMove;
    } CalculatedMove_t;

    /// @brief current game is finished. It is set to false at the beginning of a game
    ///        in LaunchNewGame and set to true whenever GameFinished is called
    /// it prevents GameFinished code to run twice for the same game
    bool m_currentGameFinished;

    /// a thread safe queue to save moves calculated by worker thread
    /// for the main thread to retrieve them
    BlockingQueue<CalculatedMove_t> m_moveQueue;

    /// this variables contains whose go it is
    Game1v1::eGame1v1Player_t m_currentMovingPlayer;

    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gtk::Builder> m_gtkBuilder;

    /// @brief the 1vs1 game which will be represented in the window
    Game1v1 m_the1v1Game;

    /// @brief latest place of the board where the user had the mouse pointer in
    Coordinate m_lastCoord;

    /// @brief the worker thread used to leave the GUI active while the next moves is calculated
    MainWindowWorkerThread m_workerThread;

    /// @brief randomizer 'cos we might use a bit of randomness when computing the next move
    GRand* m_randomizer;

    /// @brief the config dialog class
    ConfigDialog* m_configDialog;

    /// @brief the drawing area where the pieces can be picked up by the user
    DrawingAreaShowPieces m_pickPiecesDrawingArea;

    /// @brief the drawing area where the computer's pieces left will be shown
    DrawingAreaShowPieces m_showOpponentPiecesDrawingArea;

    /// @the drawing area responsible for showing the state of the board
    DrawingAreaBoard m_boardDrawingArea;

    /// @brief the table where the selected piece is edited
    TableEditPiece* m_editPieceTable;

    /// @brief the vertical box that keeps the board + pieces
    Gtk::VBox* m_vBoxDrawing;

    /// @brief the table that contains the board + pieces left
    Gtk::HBox* m_hBoxGameStatus;

    /// @brief the table that contains the opponent's pieces
    Gtk::HBox* m_hBoxOpponentPieces;

    /// @brief the horizontal box where the pieces are picked + edited
    Gtk::HBox* m_hBoxEditPieces;

    /// @brief hbox which serves as status bar
    GameStatusBar m_statusBar;

    /// Signal class for inter-thread communication to
    /// notify the next move has been computed
    Glib::Dispatcher m_signal_moveComputed;

    /// Signal class for inter-thread communication to
    /// notify a change in computing progress
    Glib::Dispatcher m_signal_computingProgressUpdated;

    /// Launches a new game. It cancels whatever the worker thread is computing now
    /// and resets the game to the starting configuration
    void LaunchNewGame();

    /// @brief requests the worker thread to compute a move
    /// It finishes the app if there's an error communicating with this thread
    /// @param player whose move will be calculated by the worker thread
    /// @param Caller will block until worker thread finishes current job and starts to compute
    ///        the job notified by this call. If it set to true it can only fail if worker thread
    ///        is requested to die
    /// @param previous move coordinate
    /// @param previous move piece
    void RequestThreadToComputeNextMove(
            Game1v1::eGame1v1Player_t a_whoMoves,
            bool                      a_blockCall = false,
            const Coordinate         &a_coordinate = Coordinate(),
            const Piece              &a_piece      = Piece(e_noPiece));

    /// @brief notifies to the user that the game is finished using a fancy message box
    void GameFinished();

    /// Do all the stuff that needs to be done when a move has been computed
    /// It is also called when a human user puts down a piece, so it is responsible for
    /// changing turns between players and allowing next player to move if it is a human
    /// being or launching the worker thread if it is the computer's move
    /// It can be used with  Glib::Dispatcher as it is a no-argument void function
    void NotifyMoveComputed();

    /// updates the progress bar with the value saved in m_computingCurrentProgress;
    void NotifyProgressUpdate();

    /// updates the score shown in the status bar
    void UpdateScoreStatus();

    /// sets mouse cursor to a watch in certain areas of the window
    /// to indicate computer is "thinking"
    void SetWaitCursor();

    /// sets mouse cursor to the default pointer in the areas where
    /// SetWaitCursor might have set it to a watch
    void ResetCursor();

    /// Calls gettext per every static widget in the main window. These strings
    /// are those ones included in the .glade file that never change during the
    /// execution of the application, for example a menu called "Game", or a
    /// label that contains the word "rotate"
    ///
    /// So far this is only needed in win32 platform due to some unknown issue
    /// that prevents those strings to be automatically translated. It works
    /// fine in linux, so there's no need there to explicitly call to gettext
    void ForceTranslationOfWidgets();

    // prevent the default constructors to be used
    Game1v1Widget();
    Game1v1Widget(const Game1v1Widget &a_src);
    Game1v1Widget& operator=(const Game1v1Widget &a_src);
};

#endif /* _GAME1V1_WIDGET_H_ */
