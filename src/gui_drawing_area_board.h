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
    void AddPlayerToList(const Player &a_player);

    /// @brief removes all the players from the list
    void ResetPlayerList();

    /// @brief force the drawing area to be reprinted
    bool Invalidate();

    /// @brief force the drawing area to be reprinted because a_piece was put down in a_coord
    /// this function sets up a visual effect over this latest piece deployed on the board
    /// to show the human user what was the latest piece deployed
    bool Invalidate(const Piece &a_piece, const Coordinate &a_coord, const Player &a_player);

    /// @brief cancel visual effect over latest piece deployed started by
    /// DrawingAreaBoard::Invalidate(const Piece &a_piece, const Coordinate &a_coord, const Player &a_player)
    void CancelLatestPieceDeployedEffect();

    /// sets the player who will put down next piece
    /// If there is a player currently set, a call to this function
    /// will override the value. You can use UnsetCurrentPlayer to
    /// to set the current player to nobody
    void SetCurrentPlayer(const Player &a_player);

    /// sets the current player who is supposed to put down the next piece
    /// to nobody. No ghost piece will be draw when the mouse pointer is moved
    /// over the board
    void UnsetCurrentPlayer();

    /// sets the current piece used by current player
    void SetCurrentPiece(const Piece &a_piece);

    /// nk points of every player will be shown on the board as a small circle
    /// it invalidates the drawing area to show the new configuration
    void ShowNucleationPoints();

    /// nk points won't be shown
    /// it invalidates the drawing area to show the new configuration
    void HideNucleationPoints();

    /// forbidden area of a_player will be shown on the board filling
    /// squares with a small hint of current player's colour
    /// it invalidates the drawing area to show the new configuration
    /// if a_player has not be added to the list using AddPlayerToList
    /// nothing will be shown
    void ShowPlayerForbiddenArea(const Player &a_player);

    /// forbidden area of whatever player was set by ShowPlayerForbiddenArea won't be shown
    /// it invalidates the drawing area to show the new configuration
    void HidePlayerForbiddenArea();

    /// influence area of a_player will be shown on the board filling
    /// squares with a small hint of current player's colour
    /// it invalidates the drawing area to show the new configuration
    /// if a_player has not be added to the list using AddPlayerToList
    /// nothing will be shown
    void ShowPlayerInfluenceArea(const Player &a_player);

    /// influence area of whatever player was set by ShowPlayerInfluenceArea won't be shown
    /// it invalidates the drawing area to show the new configuration
    void HidePlayerInfluenceArea();

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

    /// Is the effect over the latest piece deployed on?
    bool m_latestPieceDeployedEffectOn;

    /// This is a copy of the latest piece deployed on the board so a visual effect
    /// to notify the user what was the latest piece deployed can be added
    Piece m_latestPieceDeployed;

    /// Indicates where the latest piece was deployed
    Coordinate m_latestPieceDeployedCoord;

    /// player who deployed the latest piece on the board. it will be used for the latest
    /// piece visual effect
    const Player* m_latestPieceDeployedPlayer;

    /// current alpha transparency of the latest deployed piece
    /// this is modified in timerCallback to add a bit of an effect
    /// to the latest piece deployed
    float m_latestPieceDeployedTransparency;

    /// are nucleation points shown on the board?
    bool m_showNKPoints;

    /// pointer to the player whose forbidden area (coords in the board
    /// where the he/she can't go) are to be shown on the board. NULL
    /// if nothing is to be shown
    const Player* m_forbiddenAreaPlayer;

    /// pointer to the player whose influence area is to be shown on the board
    /// set to NULL if nothing is to be shown
    const Player* m_influenceAreaPlayer;

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

    /// @brief callback to be called by the g timer
    static gboolean timerCallback(void* param);

    // prevent the default constructors from being used
    DrawingAreaBoard();
    DrawingAreaBoard(const DrawingAreaBoard &a_src);
    DrawingAreaBoard& operator=(const DrawingAreaBoard &a_src);
};


#endif // _GUI_DRAWING_AREA_BOARD_H_
