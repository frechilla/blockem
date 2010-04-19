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
/// @file gui_main_window.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 12-Nov-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <string>
#include <exception>
#include <libglademm/xml.h>
#include <gtkmm.h>

#include "gui_about_dialog.h"
#include "gui_drawing_area_show_pieces.h"
#include "gui_drawing_area_board.h"
#include "gui_exception.h"
#include "gui_main_window_worker_thread.h"
#include "gui_stop_watch_label.h"
#include "gui_table_edit_piece.h"
#include "game1v1.h"
#include "coordinate.h"
#include "piece.h"
#include "singleton.h"


/// @brief class to handle the main window in the GUI
class MainWindow:
    public Singleton<MainWindow>
{
public:
    MainWindow();
    virtual ~MainWindow();

    /// Initialise the window. This methid MUST be called before the first use
    /// of the window
    void Initialize(Glib::RefPtr<Gnome::Glade::Xml> a_refXml)throw (GUIException) ;

    // property to access the Gtk::window object retrieved from the .glade file
    inline Gtk::Window& window()
    {
        return *m_theWindow;
    }

    /// @brief callback to be called whenever the worker thread finishes computing a move
    void WorkerThread_computingFinished(
            const Piece      &a_piece,
            const Coordinate &a_coord,
            int32_t           a_returnValue);

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

    /// @brief callback to be called when a button is pressed inside the board
    void BoardDrawingArea_BoardClicked(const Coordinate &, const Piece &, const Player &);

    /// @brief to be used as a functor so Game1v1 notifies this class the progress of the computing
    ///        process for computer's next move
    /// this function can be called from a different thread because it uses signal dispatcher
    /// see: http://library.gnome.org/devel/glibmm/stable/thread_2dispatcher_8cc-example.html
    static void ProgressUpdate(float a_progress);

private:
    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gnome::Glade::Xml> m_refXml;

    /// @brief the Gtk window object
    Gtk::Window* m_theWindow;

    /// @brief the 1vs1 game which will be represented in the window
    Game1v1 m_the1v1Game;

    /// @brief latest place of the board where the user had the mouse pointer in
    Coordinate m_lastCoord;

    /// @brief the worker thread used to leave the GUI active while the next moves is calculated
    MainWindowWorkerThread* m_workerThread;

    /// @brief the about dialog class
    AboutDialog* m_aboutDialog;

    /// @brief the drawing area where the pieces can be picked up by the user
    DrawingAreaShowPieces m_pickPiecesDrawingArea;

    /// @brief the drawing area where the computer's pieces left will be shown
    DrawingAreaShowPieces m_showComputerPiecesDrawingArea;

    /// @the drawing area responsible for showing the state of the board
    DrawingAreaBoard m_boardDrawingArea;

    /// @brief the table where the selected piece is edited
    TableEditPiece* m_editPieceTable;

    Gtk::MenuItem* m_newMenuItem;
    Gtk::MenuItem* m_quitMenuItem;
    Gtk::MenuItem* m_helpAboutMenuItem;

    /// @brief the vertical box that keeps the board + pieces
    Gtk::VBox* m_vBoxDrawing;

    /// @brief the table that contains the board + pieces left
    Gtk::HBox* m_hBoxGameStatus;

    /// @brief the table that contains the computer pieces
    Gtk::HBox* m_hBoxComputerPieces;

    /// @brief the horizontal box where the pieces are picked + edited
    Gtk::HBox* m_hBoxEditPieces;

    /// @brief hbox which serves as status bar
    Gtk::HBox* m_hBoxStatusBar;

    /// @brief vertical separators to be used in the status bar
    //TODO that number 3 is magic!!
    Gtk::VSeparator m_arrayStatusBarSeparator[3];

    /// @brief progress bar for when the computer is "thinking".
    ///        To be shown in the status bar
    Gtk::ProgressBar m_progressBar;

    /// @brief label to show the user score in the status bar
    Gtk::Label m_userScoreLabel;

    /// @brief label to show the computer score in the status bar
    Gtk::Label m_computerScoreLabel;

    /// @brief show the time the user takes to think
    StopWatchLabel m_stopWatchLabelUser;

    /// @brief show the time the computer takes to think
    StopWatchLabel m_stopWatchLabelComputer;

    /// Signal class for inter-thread communication to
    /// notify the next move has been computed
    Glib::Dispatcher m_signal_moveComputed;

    /// Signal class for inter-thread communication to
    /// notify the user the game is finished
    Glib::Dispatcher m_signal_gameFinished;

    /// Signal class for inter-thread communication to
    /// notify a change in computing progress
    Glib::Dispatcher m_signal_computingProgressUpdated;
    
    /// @brief current computing progress
    static float m_computingCurrentProgress;

    /// @brief notifies to the user that the game is finished using a fancy message box
    void NotifyGameFinished();

    /// Do all the stuff that needs to be done when a move has been computed
    ///     1) Invalidates the board drawing area.
    ///     2) restores the mouse cursor
    /// It can be used with  Glib::Dispatcher as it is a no-argument void function
    void NotifyMoveComputed();

    /// updates the progress bar with the value saved in m_computingCurrentProgress;
    void NotifyProgressUpdate();

    /// updates the score shown in the status bar
    /// takes the status of the game from
    /// m_computerSquaresLeft and m_userSquaresLeft
    /// so someone else is responsible for updating these
    /// variables accordingly
    void UpdateScoreStatus();

    // prevent the default constructors to be used
    MainWindow(const MainWindow &a_src);
    MainWindow& operator=(const MainWindow &a_src);
};

#endif /* MAINWINDOW_H_ */
