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
/// @file table_edit_piece.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2010  Original development
///           Faustino Frechilla 25-Apr-2010  libglade dependency removed. Code migrated to GtkBuilder
///           Faustino Frechilla 07-Oct-2010  Widgets are not loaded from glade file. GtkBuilder not needed
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#ifndef _GUI_TABLE_EDIT_PIECE_H_
#define _GUI_TABLE_EDIT_PIECE_H_

#include <gtkmm.h>
#include "piece.h"

class TableEditPiece :
    public Gtk::Table
{
public:
	/// Instantiates the class
	TableEditPiece();
	~TableEditPiece();

    /// @brief gray out the whole table and children widgets
    /// It resets the current piece so there won't be any piece shown
    void set_sensitive(gboolean a_sensitive);

    /// @brief callback to be called when the edit piece drawing area is redraw on screen
    bool EditPieceDrawingArea_ExposeEvent(GdkEventExpose* event);

    /// @brief callback to be called when the rotate right button is pressed
    void RotateRightButton_ButtonPressed();

    /// @brief callback to be called when the rotate left button is pressed
    void RotateLeftButton_ButtonPressed();

    /// @brief callback to be called when the mirror over the Y axis button is pressed
    void MirrorYAxisButton_ButtonPressed();

    /// @brief callback to be called when the mirror over the X axis button is pressed
    void MirrorXAxisButton_ButtonPressed();

    /// @brief return the piece being edited at the moment in the widget
    inline const Piece& GetPiece()
    {
    	return m_thePiece;
    }

    /// Access to the piece_changed signal private member
    inline sigc::signal<void, const Piece&>& signal_pieceChanged()
    {
        return m_signalPieceChanged;
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
    /// @brief instance of a piece which is being edited by the user
    Piece m_thePiece;

    /// @brief rotate label
    Gtk::Label m_rotateLabel;

    /// @brief mirror label
    Gtk::Label m_mirrorLabel;

    /// @brief the drawing area where the piece is edited
    Gtk::DrawingArea m_editPieceDrawingArea;

    Gtk::Arrow m_arrowYLeft;
    Gtk::Arrow m_arrowYRight;
    Gtk::Arrow m_arrowXUp;
    Gtk::Arrow m_arrowXDown;
    Gtk::Arrow m_arrowRotateRight;
    Gtk::Arrow m_arrowRotateLeft;

    /// @brief the rotate right button to edit the piece
    Gtk::Button m_rotateRightButton;

    /// @brief the rotate left button to edit the piece
    Gtk::Button m_rotateLeftButton;

    /// @brief the mirror over Y axis button to edit the piece
    Gtk::Button m_mirrorButtonYAxis;

    /// @brief hbox to be inserted into m_mirrorButtonYAxis
    Gtk::HBox m_mirrorButtonYAxisHBox;

    /// @brief the mirror over X axis button to edit the piece
    Gtk::Button m_mirrorButtonXAxis;

    /// @brief vbox to be inserted into m_mirrorButtonXAxis
    Gtk::VBox m_mirrorButtonXAxisVBox;

    /// @brief Piece's colour: red channel
    float m_red;
    /// @brief Piece's colour: green channel
    float m_green;
    /// @brief Piece's colour: blue channel
    float m_blue;

    /// signal to be sent when the current piece being edited changes
    /// either because it's a different piece or because it was edited
    sigc::signal<void, const Piece&> m_signalPieceChanged;

    /// force the editPiece drawing area to be repainted
    bool InvalidateEditPieceDrawingArea();

    // prevent the default constructors from being used
	TableEditPiece(const TableEditPiece &a_src);
	TableEditPiece& operator=(const TableEditPiece &a_src);
};

#endif // _GUI_TABLE_EDIT_PIECE_H_
