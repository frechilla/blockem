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
/// @file gui_drawing_area_board.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 8-Apr-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef _GUI_DRAWING_AREA_BOARD_H_
#define _GUI_DRAWING_AREA_BOARD_H_

#include <gtkmm.h>
#include "board.h"
#include "player.h"
#include "coordinate.h"

class DrawingAreaBoard :
    public Gtk::DrawingArea
{
public:
    DrawingAreaBoard(const Board &a_board);
    ~DrawingAreaBoard();

    /// @brief adds a player whose pieces on the board must be represented accordingly
    void AddPlayerToList(const Player* a_player);

    /// @brief removes all the players from the list
    void ResetPlayerList();

    /// @brief force the drawing area to be reprinted
    bool Invalidate();

    void inline SetCurrentPlayer(const Player* a_player)
    {
        m_currentPlayer = a_player;
    }

    void inline SetCurrentPiece(const Piece &a_piece)
    {
        m_currentPiece = a_piece;
    }

    /// Access to the m_signalBoardClicked signal private member, which is sent
    /// when a the user clicked on the board and there is a current player and piece selected
    inline sigc::signal<void, const Coordinate &, const Piece &, const Player &>& signal_boardPicked()
    {
        return m_signalBoardClicked;
    }

private:
    /// The blockem board represented by this drawing area
    const Board &m_theBoard;

    /// List of players whose pieces are (or can be) on the board
    /// If a player has a piece on the Board instance and is not added to this list
    /// its pieces won't be correctly represented on the drawing area
    std::list<const Player*> m_playerList;

    /// This is the player who's got the turn now. His/her selected piece
    /// must be drawn on the board while he/she moves around trying to allocate it
    const Player* m_currentPlayer;

    /// This is a copy of the piece currently selected by 'm_currentPlayer'
    Piece m_currentPiece;

    /// current place in the board where the user moved the mouse pointer (inside the board)
    Coordinate m_currentCoord;

    /// signal to be sent when a the user clicked on the board and there is a current player and piece
    /// selected
    sigc::signal<void, const Coordinate &, const Piece &, const Player &> m_signalBoardClicked;

    /// @brief it'll be  called when the pick piece drawing area is redrawn on screen
    bool on_expose_event(GdkEventExpose* event);

    /// @brief it'll be  called when a button is pressed inside the pick pieces drawing area
    bool on_button_press_event(GdkEventButton* event);

    /// @brief it'll be called when the mouse moves inside the board drawing area
    bool on_motion_notify_event(GdkEventMotion* event);

    /// @brief it'll be called when the mouse leaves the board drawing area
    bool on_leave_notify_event(GdkEventCrossing* event);

    /// @brief translates an absolute drawingarea coordinate into the board coord
    /// @return true if the translation was successful (the window coordinate was in the board)
    bool DrawingAreaToBoardCoord(int32_t a_windowX, int32_t a_windowY, Coordinate &out_boardCoord);

    // prevent the default constructors from being used
    DrawingAreaBoard();
    DrawingAreaBoard(const DrawingAreaBoard &a_src);
    DrawingAreaBoard& operator=(const DrawingAreaBoard &a_src);
};


#endif // _GUI_DRAWING_AREA_BOARD_H_
