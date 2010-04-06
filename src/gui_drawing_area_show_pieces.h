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
/// @file gui_drawing_area_show_pieces.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 25-Mar-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef _GUI_DRAWING_AREA_SHOW_PIECES_H_
#define _GUI_DRAWING_AREA_SHOW_PIECES_H_

#include <gtkmm.h>

#include "Player.h"
#include "Piece.h"

class DrawingAreaShowPieces :
	public Gtk::DrawingArea
{
public:

	/// contains the 4 available options to show the drawing area
	/// topToBottom and bottomToTop are vertical
	/// leftToRight and rightToLeft are horizontal
	/// have a look at the big comment at the beginning of
	/// gui_drawing_area_show_pieces.cpp to see the original setup (leftToRight)
	/// the user must take into account that only the leftToRight configuration shows the piece
	/// on the DrawingAreaShowPieces as it is created using the Piece constructor. Appearance
	/// of new pieces don't match what it is shown on the DrawingAreaShowPieces when using
	/// any other configuration. This is ok when this drawing area needs to be used only to
	/// show pieces (not to pick pieces and edit them later)
	typedef enum
	{
		eOrientation_leftToRight = 0,
		eOrientation_bottomToTop,
		eOrientation_rightToLeft,
		eOrientation_topToBottom,
	} eOrientation_t;

    /// Instantiates the class. It takes as a parameter
	/// the player which will be used to know which pieces to show
	/// and the orientation of the Drawing area
	DrawingAreaShowPieces(const Player &a_player, eOrientation_t a_orientation);
    ~DrawingAreaShowPieces();

    /// @brief sets the colour which will be used to draw the pieces
    void SetPlayerRGB(float a_red, float a_green, float a_blue);

    /// Access to the piece_picked signal private member
    inline sigc::signal<void, ePieceType_t>& signal_piecePicked()
    {
        return m_signalPiecePicked;
    }

    // force the drawing area to be reprinted
    bool Invalidate();

private:
    /// @brief player whose pieces will be shown in this drawing area
    const Player &m_thePlayer;

    /// @brief orientation to be used to draw the drawing area
    eOrientation_t m_orientation;

    /// @brief Pieces' colour: red channel
    float m_red;
    /// @brief Pieces' colour: green channel
    float m_green;
    /// @brief Pieces' colour: blue channel
    float m_blue;

    /// signal to be sent when a piece has been picked
    /// it will report e_noPiece if the user clicked where there was no piece shown
    sigc::signal<void, ePieceType_t> m_signalPiecePicked;

    /// @brief callback to be called when the pick piece drawing area is redrawn on screen
    bool on_expose_event(GdkEventExpose* event);

    /// @brief callback to be called when a button is pressed inside the pick pieces drawing area
    bool on_button_press_event(GdkEventButton* event);

    // prevent the default constructors from being used
    DrawingAreaShowPieces();
    DrawingAreaShowPieces(const DrawingAreaShowPieces &a_src);
    DrawingAreaShowPieces& operator=(const DrawingAreaShowPieces &a_src);
};

#endif // _GUI_DRAWING_AREA_SHOW_PIECES_H_
