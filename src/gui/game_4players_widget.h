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
/// @file game_4players_widget.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 17-Nov-2010  Original development
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
///           Faustino Frechilla 12-Jan-2010  non-moving players' pieces shown around the board
/// @endhistory
///
// ============================================================================

#ifndef _GAME_4PLAYERS_WIDGET_H_
#define _GAME_4PLAYERS_WIDGET_H_

#include <string>
#include <exception>
#include <gtkmm.h>

#include "gui/drawing_area_show_pieces.h"
#include "gui/drawing_area_board.h"
#include "gui/stop_watch_label.h"
#include "gui/table_edit_piece.h"
#include "gui/statusbar_game.h"
#include "game_4players.h"
#include "coordinate.h"
#include "piece.h"


/// @brief class to handle the game 4players widget
/// It can be added to a window and it would handle its own messages
class Game4PlayersWidget :
    public Gtk::VBox
{
public:
    /// Instantiates the class
    Game4PlayersWidget();
    virtual ~Game4PlayersWidget();

    /// @brief Recursively hides a widget and any child widgets
    /// stops all glib timers owned by the object
    void hide_all();

    /// Launches a new game.
    /// WARNING: resets the game completely
    void LaunchNewGame();

    /// @return a reference to the drawing area shown by this widget
    DrawingAreaBoard& BoardDrawingArea();

    /// @brief shows the influence area of a player on the board
    /// @param a_playerType. Player whose influence area will be shown.
    ///        set to e_Game4_NoPlayer if influence area shouldn't be shown at all
    void ShowInfluenceAreaInBoard(Game4Players::eGame4_Player_t a_playerType);

    /// @brief shows forbidden area of a player on the board
    /// @param a_playerType. Player whose forbidden area will be shown.
    ///        set to e_Game4_NoPlayer if influence area shouldn't be shown at all
    void ShowForbiddenAreaInBoard(Game4Players::eGame4_Player_t a_playerType);
    
    /// @brief shows the pieces of the opponents to the current moving player
    /// @param true to show the pieces. False to hide them
    void ShowOpponentsPieces(bool a_showPieces);

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

    /// this variables contains whose go it is
    Game4Players::eGame4_Player_t m_currentMovingPlayer;

    /// @brief the total allocation game which will be represented in the window
    Game4Players m_the4PlayersGame;

    /// @brief the drawing area responsible for showing the state of the board
    DrawingAreaBoard m_boardDrawingArea;

    /// @brief the drawing area where the pieces can be picked up by the user
    DrawingAreaShowPieces m_pickPiecesDrawingArea;

    /// @brief drawing area where the pieces of an opponent to current player
    ///        will be shown at the left of the board
    DrawingAreaShowPieces m_showPiecesDrawingAreaLeft;

    /// @brief the table that contains the opponent's pieces shown on the left
    ///        needed for proper behaviour when window resizing
    Gtk::HBox m_hBoxOpponentLeft;

    /// @brief drawing area where the pieces of an opponent to current player
    ///        will be shown on top of the board
    DrawingAreaShowPieces m_showPiecesDrawingAreaTop;

    /// @brief the table that contains the opponent's pieces shown on top
    ///        needed for proper behaviour when window resizing
    Gtk::VBox m_vBoxOpponentTop;

    /// @brief drawing area where the pieces of an opponent to current player
    ///        will be shown at the right of the board
    DrawingAreaShowPieces m_showPiecesDrawingAreaRight;

    /// @brief the table that contains the opponent's pieces shown on the right
    ///        needed for proper behaviour when window resizing
    Gtk::HBox m_hBoxOpponentRight;

    /// @brieftable where the selected piece is edited
    TableEditPiece m_editPieceTable;

    /// @brief table where the board and pieces of those players who are not moving
    ///        are shown
    Gtk::Table m_gameTable;

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

    /// @brief callback to be called when a button is pressed inside the board
    void BoardDrawingArea_BoardClicked(const Coordinate &, const Piece &, const Player &);

    /// updates the score shown in the status bar
    void UpdateScoreStatus();

    // prevent the default constructors to be used
    Game4PlayersWidget(const Game4PlayersWidget &a_src);
    Game4PlayersWidget& operator=(const Game4PlayersWidget &a_src);
};

#endif /* _GAME_4PLAYERS_WIDGET_H_ */
