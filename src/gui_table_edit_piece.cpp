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
/// @file gui_table_edit_piece.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2010  Original development
/// @endhistory
///
// ============================================================================

#include "gui_table_edit_piece.h"
#include "gui_glade_defs.h"

//TODO this should go away at some stage
// default colours to draw the pieces
static const float DEFAULT_PLAYER_RED   = 0.3;
static const float DEFAULT_PLAYER_GREEN = 0.8;
static const float DEFAULT_PLAYER_BLUE  = 0.3;

/// @brief line width of the circle which surrounds the edit piece
static const float EDIT_CIRCLE_LINE_WIDTH = 1.0;

/// @brief number of squares of the edit piece board
static const int32_t NSQUARES_EDIT_PIECES_BOARD = 5;

TableEditPiece::TableEditPiece(
		Glib::RefPtr<Gnome::Glade::Xml> a_refXml) throw (GUIException):
			m_refXml(a_refXml),
			m_thePiece(e_noPiece),
		    m_red(DEFAULT_PLAYER_RED),
		    m_green(DEFAULT_PLAYER_GREEN),
		    m_blue(DEFAULT_PLAYER_BLUE)
{
	// retrieve the objects from the GUI design
	m_refXml->get_widget(GUI_TABLE_EDITING_PIECE_NAME, m_theTable);
	if (m_theTable == NULL)
	{
		throw new GUIException(std::string("Edit pieces table retrieval failed"));
	}

	m_refXml->get_widget(GUI_DRAWINGAREA_EDITING_PIECE, m_editPieceDrawingArea);
	if (m_editPieceDrawingArea == NULL)
	{
		throw new GUIException(std::string("Edit pieces drawing area retrieval failed"));
	}

	m_refXml->get_widget(GUI_BUTTON_ROTATE_NAME, m_rotateButton);
	if (m_rotateButton == NULL)
	{
		throw new GUIException(std::string("rotate button retrieval failed"));
	}

	m_refXml->get_widget(GUI_BUTTON_MIRROR_NAME, m_mirrorButton);
	if (m_mirrorButton == NULL)
	{
		throw new GUIException(std::string("mirror button retrieval failed"));
	}

	// connect the signals to the handlers
	m_editPieceDrawingArea->signal_expose_event().connect(
			sigc::mem_fun(*this, &TableEditPiece::EditPieceDrawingArea_ExposeEvent));

	m_rotateButton->signal_clicked().connect(
			sigc::mem_fun(*this, &TableEditPiece::RotateButton_ButtonPressed));
	m_mirrorButton->signal_clicked().connect(
			sigc::mem_fun(*this, &TableEditPiece::MirrorButton_ButtonPressed));

	// set the rotate and mirror button to not sensitive, since at the beginning
	// there's no piece loaded in the edit piece drawing area
	m_rotateButton->set_sensitive(false);
	m_mirrorButton->set_sensitive(false);
}

TableEditPiece::~TableEditPiece()
{
}

void TableEditPiece::SetPieceRGB(float a_red, float a_green, float a_blue)
{
	m_red   = a_red;
	m_green = a_green;
	m_blue  = a_blue;
}

void TableEditPiece::SetPiece(ePieceType_t a_newPiece)
{
    // only update the piece ONLY if it changes
    if (a_newPiece == m_thePiece.GetType())
    {
        return;
    }

	m_thePiece = Piece(a_newPiece);

    if (m_thePiece.CanRotateOriginally())
    {
        m_rotateButton->set_sensitive(true);
    }
    else
    {
        m_rotateButton->set_sensitive(false);
    }
    if (m_thePiece.CanMirrorOriginally())
    {
        m_mirrorButton->set_sensitive(true);
    }
    else
    {
        m_mirrorButton->set_sensitive(false);
    }

    // notify to whoever is listening to the signal that the piece changed
    m_signalPieceChanged.emit(m_thePiece);

    // update the view
    InvalidateEditPieceDrawingArea();
}

bool TableEditPiece::EditPieceDrawingArea_ExposeEvent(GdkEventExpose* event)
{
	// This is where we draw on the window
	Glib::RefPtr<Gdk::Window> window = m_editPieceDrawingArea->get_window();
	if(window)
	{
		Gtk::Allocation allocation = m_editPieceDrawingArea->get_allocation();

		int32_t width  = allocation.get_width();
		int32_t height = allocation.get_height();
		int32_t squareSize = std::min(width - 2, height - 2);

		int32_t littleSquare = squareSize / NSQUARES_EDIT_PIECES_BOARD;
		squareSize = littleSquare * NSQUARES_EDIT_PIECES_BOARD;

		// coordinates for the centre of the window
		int32_t xc = width  / 2;
		int32_t yc = height / 2;

		// get the pen to draw
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

		// clip to the area indicated by the expose event so that we only redraw
		// the portion of the window that needs to be redrawn
		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);
		cr->clip();

		// print a circle which will surround the piece being edited
		cr->set_line_width(EDIT_CIRCLE_LINE_WIDTH);
		cr->set_source_rgb(
				m_red,
				m_green,
				m_blue);

        cr->arc(xc + 1,
                yc + 1,
                (squareSize/2) + 2,
                0.0,
                2 * M_PI);

        cr->stroke();

		// print the piece in the edit piece drawing area
        if (m_thePiece.GetType() != e_noPiece)
        {
            cr->set_source_rgb(
            		m_red,
            		m_green,
            		m_blue);

            // draw the piece in the edit piece drawing area
            for (uint8_t i = 0; i < m_thePiece.GetNSquares(); i++)
            {
                cr->rectangle(
                        (xc - littleSquare/2) + (littleSquare * m_thePiece.m_coords[i].m_col) + 1,
                        (yc - littleSquare/2) + (littleSquare * m_thePiece.m_coords[i].m_row) + 1,
                        littleSquare - 1,
                        littleSquare - 1);

                cr->fill();
            }
        }
	}

	return true;
}

void TableEditPiece::RotateButton_ButtonPressed()
{
	m_thePiece.Rotate();

	// notify to whoever is listening to the signal that the piece changed
    m_signalPieceChanged.emit(m_thePiece);
    // update the view
	InvalidateEditPieceDrawingArea();
}

void TableEditPiece::MirrorButton_ButtonPressed()
{
	m_thePiece.Mirror();

    // notify to whoever is listening to the signal that the piece changed
    m_signalPieceChanged.emit(m_thePiece);
    // update the view
	InvalidateEditPieceDrawingArea();
}

bool TableEditPiece::InvalidateEditPieceDrawingArea()
{
    // force the drawing area to be redraw
    Glib::RefPtr<Gdk::Window> window = m_editPieceDrawingArea->get_window();
    if(window)
    {
        Gdk::Rectangle rect(
                0,
                0,
                m_editPieceDrawingArea->get_allocation().get_width(),
                m_editPieceDrawingArea->get_allocation().get_height());

        window->invalidate_rect(rect, false);
        return true;
    }

    return false;
}
