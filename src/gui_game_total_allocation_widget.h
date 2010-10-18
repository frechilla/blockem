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
/// @file gui_game_total_allocation_widget.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 06-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef _GAME_TOTAL_ALLOCATION_WIDGET_H_
#define _GAME_TOTAL_ALLOCATION_WIDGET_H_

#include <string>
#include <exception>
#include <gtkmm.h>

#include "gui_drawing_area_show_pieces.h"
#include "gui_drawing_area_board.h"
#include "gui_stop_watch_label.h"
#include "gui_table_edit_piece.h"
#include "gui_statusbar_game.h"
#include "game_total_allocation.h"
#include "coordinate.h"
#include "piece.h"


/// @brief class to handle the game total allocation widget
/// It can be added to a window and it would handle its own messages
class GameTotalAllocationWidget :
    public Gtk::VBox
{
public:
    /// Instantiates the class
    GameTotalAllocationWidget();
    virtual ~GameTotalAllocationWidget();
    
    /// @brief Recursively hides a widget and any child widgets
    /// stops all glib timers owned by the object
    void hide_all();

    /// Launches a new game.
    /// WARNING: resets completely the game
    void LaunchNewGame();

    /// @return a reference to the drawing area shown by this widget
    DrawingAreaBoard& BoardDrawingArea();

    /// @brief shows the influence area of the player on the board
    /// @param a_show. influence area will be shown if true
    void ShowInfluenceAreaInBoard(bool a_show);

    /// @brief shows forbidden area of the player on the board
    /// @param a_show. forbidden area will be shown if true
    void ShowForbiddenAreaInBoard(bool a_show);

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

private:

    /// @brief current game is finished. It is set to false at the beginning of a game
    ///        in LaunchNewGame and set to true whenever GameFinished is called
    /// it prevents GameFinished code to run twice for the same game
    bool m_currentGameFinished;

    /// @brief the total allocation game which will be represented in the window
    GameTotalAllocation m_theTotalAllocationGame;

    /// @brief the drawing area where the pieces can be picked up by the user
    DrawingAreaShowPieces m_pickPiecesDrawingArea;

    /// @the drawing area responsible for showing the state of the board
    DrawingAreaBoard m_boardDrawingArea;

    /// @brieftable where the selected piece is edited
    TableEditPiece m_editPieceTable;

    /// @brief the horizontal box where the pieces are picked + edited
    Gtk::HBox m_hBoxEditPieces;

    /// @brief hbox which serves as status bar
    StatusBarGame m_statusBar;

    /// Signal object to notify fatal errors
    sigc::signal<void, const std::string&> m_signal_fatalError;

    /// Signal object to notify when game is finished
    sigc::signal<void, const std::string&> m_signal_gameFinished;

    /// @brief notifies to the user that the game is finished using m_signal_gameFinished
    void GameFinished();

    /// @brief set up the widgets that make up this object and add them into it
    void BuildGUI();

    // prevent the default constructors to be used
    GameTotalAllocationWidget(const GameTotalAllocationWidget &a_src);
    GameTotalAllocationWidget& operator=(const GameTotalAllocationWidget &a_src);
};

#endif /* _GAME_TOTAL_ALLOCATION_WIDGET_H_ */
