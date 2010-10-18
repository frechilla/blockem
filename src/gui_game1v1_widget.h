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
#include "gui_drawing_area_show_pieces.h"
#include "gui_drawing_area_board.h"
#include "gui_main_window_worker_thread.h"
#include "gui_stop_watch_label.h"
#include "gui_table_edit_piece.h"
#include "gui_statusbar_game.h"
#include "gui_game1v1_config_dialog.h"
#include "game1v1.h"
#include "coordinate.h"
#include "piece.h"


/// @brief class to handle the game1v1 widget
/// It can be added to a window and it would handle its own messages
class Game1v1Widget :
    public Gtk::VBox
{
public:
    /// Instantiates the class
    Game1v1Widget();
    virtual ~Game1v1Widget();
    
    /// @brief Recursively hides a widget and any child widgets
    /// stops all glib timers owned by the object
    void hide_all();

    /// @return true if current game is finished (that is, no player can put 
    ///         down any piece). False otherwise
    bool IsGameFinished() const;
    
    /// Cancels computing thread and sets it to "waiting" state
    /// It is a blocking call (it gets blocked until the thread cancels its
    /// current calculation process)
    void CancelComputing();

    /// @return true if the worker thread is processing a new move
    bool IsComputingMove();

    /// Launches a new game.
    /// WARNING: It must be used carefully since it cancels whatever the worker
    /// thread is computing now and resets the game to the settigns stored in
    /// the Game1v1Config singleton
    void LaunchNewGame();

    /// Uses the game1v1 config dialog to decide what changes in the
    /// current game need to be done
    /// Then updates the general config singleton with the new configuration
    /// And launches requests to the worker thread if neccesary
    /// WARNING: It should only be called if the user accepted changes when
    /// he/she was shown the config dialog
    /// @param the game1v1 config dialog
    /// @return true if the current calculation of the worker thread had to be
    ///         cancelled. False otherwise
    bool ProcessChangeInCurrentGame(Game1v1ConfigDialog& a_configDialog);

    /// @return a reference to the drawing area shown by this widget
    DrawingAreaBoard& BoardDrawingArea();

    /// @brief shows the influence area of a player on the board
    /// @param a_game1v1Player if it is e_Game1v1NoPlayer no influence area will be shown
    void ShowInfluenceAreaInBoard(Game1v1::eGame1v1Player_t a_game1v1Player);

    /// @brief shows forbidden area of a player on the board
    /// @param a_game1v1Player if it is e_Game1v1NoPlayer no forbidden area will be shown
    void ShowForbiddenAreaInBoard(Game1v1::eGame1v1Player_t a_game1v1Player);

    /// @brief callback to be called whenever the worker thread finishes computing a move
    void WorkerThread_computingFinished(
            const Piece              &a_piece,
            const Coordinate         &a_coord,
            Game1v1::eGame1v1Player_t a_playerToMove,
            int32_t                   a_returnValue);

    /// @brief callback to be called when a button is pressed inside the board
    void BoardDrawingArea_BoardClicked(const Coordinate &, const Piece &, const Player &);

    /// Access to the fatal error signal private member. Note this signal is
    /// not thread safe so it must be processed by the same thread who
    /// handles the GUI
    /// It contains a HTML formatted error message describing the fatal error
    sigc::signal<void, const std::string&>& signal_fatalError()
    {
        return m_signal_fatalError;
    }

    /// Access to the game finished signal private member. Note this signal is
    /// not thread safe so it must be processed by the same thread who
    /// handles the GUI
    /// It contains a HTML formated message describing the final score
    sigc::signal<void, const std::string&>& signal_gameFinished()
    {
        return m_signal_gameFinished;
    }

    /// @brief receives signals coming from Game1v1 when it  notifies the
    ///        progress of the computing process for computer's next move
    /// this function can be called from a different thread because it uses signal dispatcher
    /// see: http://library.gnome.org/devel/glibmm/stable/thread_2dispatcher_8cc-example.html
    void ProgressUpdate(float a_progress);

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

    /// @brief the 1vs1 game which will be represented in the window
    Game1v1 m_the1v1Game;

    /// @brief latest place of the board where the user had the mouse pointer in
    Coordinate m_lastCoord;

    /// @brief the worker thread used to leave the GUI active while next moves are calculated
    MainWindowWorkerThread m_workerThread;

    /// @brief randomizer 'cos we might use a bit of randomness when computing the next move
    GRand* m_randomizer;

    /// @brief the drawing area where the pieces can be picked up by the user
    DrawingAreaShowPieces m_pickPiecesDrawingArea;

    /// @brief the drawing area where the computer's pieces left will be shown
    DrawingAreaShowPieces m_showOpponentPiecesDrawingArea;

    /// @the drawing area responsible for showing the state of the board
    DrawingAreaBoard m_boardDrawingArea;

    /// @brief the table where the selected piece is edited
    TableEditPiece m_editPieceTable;

    /// @brief the table that contains the board + pieces left
    Gtk::HBox m_hBoxGameStatus;

    /// @brief the table that contains the opponent's pieces
    Gtk::HBox m_hBoxOpponentPieces;

    /// @brief vertical separator to separate board from opponent's pieces
    Gtk::VSeparator m_opponentSeparator;

    /// @brief the horizontal box where the pieces are picked + edited
    Gtk::HBox m_hBoxEditPieces;

    /// @brief hbox which serves as status bar
    StatusBarGame m_statusBar;

    /// thread-safe signal object for inter-thread communication to
    /// notify the next move has been computed
    Glib::Dispatcher m_signal_moveComputed;

    /// thread-safe signal object for inter-thread communication to
    /// notify a change in computing progress
    Glib::Dispatcher m_signal_computingProgressUpdated;

    /// Signal object to notify fatal errors
    sigc::signal<void, const std::string&> m_signal_fatalError;

    /// Signal object to notify when game is finished
    sigc::signal<void, const std::string&> m_signal_gameFinished;

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

    /// @brief notifies to the user that the game is finished using m_signal_gameFinished
    void GameFinished();

    /// Do all the stuff that needs to be done when a move has been computed
    /// It is also called when a human user puts down a piece, so it is responsible for
    /// changing turns between players and allowing next player to move if it is a human
    /// being or launching the worker thread if it is the computer's move
    /// It can be used with  Glib::Dispatcher as it is a no-argument void function
    void NotifyMoveComputed();

    /// updates the progress bar using global values static to gui_game1v1_widget.cpp
    void ThreadSafeProgressUpdate();

    /// updates the score shown in the status bar
    void UpdateScoreStatus();

    /// sets mouse cursor to a watch in certain areas of the window
    /// to indicate computer is "thinking"
    void SetWaitCursor();

    /// sets mouse cursor to the default pointer in the areas where
    /// SetWaitCursor might have set it to a watch
    void ResetCursor();

    // prevent the default constructors to be used
    Game1v1Widget(const Game1v1Widget &a_src);
    Game1v1Widget& operator=(const Game1v1Widget &a_src);
};

#endif /* _GAME1V1_WIDGET_H_ */
