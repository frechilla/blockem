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

#include <libglademm/xml.h>
#include <gtkmm.h>

#include "GUIException.h"
#include "Player.h"
#include "Piece.h"

class DrawingAreaPickPieces
{
public:
    /// instanciates the class. It needs a Gnome::Glade::Xml object to retrieve the
    /// glade info, and the player which will be used to know which pieces to show
    DrawingAreaPickPieces(
            Glib::RefPtr<Gnome::Glade::Xml> a_refXml,
            const Player &a_player) throw (GUIException);
    ~DrawingAreaPickPieces();

    /// @brief callback to be called when the pick piece drawing area is redraw on screen
    bool ExposeEvent(GdkEventExpose* event);

    /// @brief callback to be called when a button is pressed inside the pick pieces drawing area
    bool ButtonPressed(GdkEventButton *event);

    /// @brief sets the colour which will be used to draw the pieces
    void SetPlayerRGB(float a_red, float a_green, float a_blue);

    /// @brief accesor to the drawing area represented by this class
    inline Gtk::DrawingArea& DrawingArea()
    {
        return *m_theDrawingArea;
    }

    /// Access to the piece_picked signal private member
    inline sigc::signal<void, ePieceType_t>& signal_piecePicked()
    {
        return m_signalPiecePicked;
    }

    // force the drawing area to be reprinted
    bool Invalidate();

private:
    /// @brief the drawing area represented by this class
    Gtk::DrawingArea* m_theDrawingArea;

    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gnome::Glade::Xml> m_refXml;

    /// @brief player whose pieces will be shown in this drawing area
    const Player &m_thePlayer;

    /// signal to be sent when a piece has been picked
    /// it will report e_noPiece if the user clicked where there was no piece shown
    sigc::signal<void, ePieceType_t> m_signalPiecePicked;

    /// Pieces' red channel
    float m_red;
    /// Pieces' green channel
    float m_green;
    /// Pieces' blue channel
    float m_blue;

    // prevent the default constructors from being used
    DrawingAreaPickPieces();
    DrawingAreaPickPieces(const DrawingAreaPickPieces &a_src);
    DrawingAreaPickPieces& operator=(const DrawingAreaPickPieces &a_src);
};
