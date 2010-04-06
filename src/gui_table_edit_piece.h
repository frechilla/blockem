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
/// @file gui_table_edit_piece.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef _GUI_TABLE_EDIT_PIECE_H_
#define _GUI_TABLE_EDIT_PIECE_H_

#include <libglademm/xml.h>
#include <gtkmm.h>

#include "gui_exception.h"
#include "Piece.h"

class TableEditPiece
{
public:
	/// Instantiates the class. It needs a Gnome::Glade::Xml object to retrieve the
	/// glade info
	TableEditPiece(Glib::RefPtr<Gnome::Glade::Xml> a_refXml) throw (GUIException);
	~TableEditPiece();

    /// @brief callback to be called when the edit piece drawing area is redraw on screen
    bool EditPieceDrawingArea_ExposeEvent(GdkEventExpose* event);

    /// @brief callback to be called when the rotate button is pressed
    void RotateButton_ButtonPressed();

    /// @brief callback to be called when the mirror button is pressed
    void MirrorButton_ButtonPressed();

    /// @brief accesor to the table represented by this class
    inline Gtk::Table& Table()
    {
        return *m_theTable;
    }

    /// @brief return the piece being edited at the moment in the widget
    inline const Piece& GetPiece()
    {
    	return m_thePiece;
    }

    /// @brief sets the piece to be shown to the user for it to be edited
    /// it always resets the piece to the new piece (if it is the same piece as
    /// it is currently being edited the edition will be restarted anyway)
    /// It does the following actions
    ///     1) update the current edit piece (m_thePiece) accordingly
    ///     2) invalidate the pieces drawing area
    /// It can also be used to clean the editPiecesDrawingArea using e_noPiece as a parameter
    /// since it will set m_editPiece to e_noPiece and
    /// invalidate the m_editPiecesDrawingArea (which will draw nothing)
    void SetPiece(ePieceType_t a_newPiece);

    /// @brief sets the colour which will be used to draw the piece
    void SetPieceRGB(float a_red, float a_green, float a_blue);

private:
    /// @brief the table represented by this class
    Gtk::Table* m_theTable;

    /// @brief the drawing area where the piece is edited
    Gtk::DrawingArea* m_editPieceDrawingArea;

    /// @brief the rotate button to edit the piece
    Gtk::Button* m_rotateButton;

    /// @brief the mirros button to edit the piece
    Gtk::Button* m_mirrorButton;

    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gnome::Glade::Xml> m_refXml;

    /// @brief instance of a piece which is being edited by the user
    Piece m_thePiece;

    /// @brief Piece's colour: red channel
    float m_red;
    /// @brief Piece's colour: green channel
    float m_green;
    /// @brief Piece's colour: blue channel
    float m_blue;

    // force the editPiece drawing area to be repainted
    bool InvalidateEditPieceDrawingArea();

    // prevent the default constructors from being used
	TableEditPiece();
	TableEditPiece(const TableEditPiece &a_src);
	TableEditPiece& operator=(const TableEditPiece &a_src);
};

#endif // _GUI_TABLE_EDIT_PIECE_H_
