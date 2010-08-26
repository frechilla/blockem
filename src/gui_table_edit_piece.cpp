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
///           Faustino Frechilla 25-Apr-2010  libglade dependency removed. Code migrated to GtkBuilder
///           Faustino Frechilla 23-Jul-2010  i18n
/// @endhistory
///
// ============================================================================

#include "gettext.h" // i18n
#include "gui_table_edit_piece.h"
#include "gui_glade_defs.h"

//TODO this should go away at some stage
// default colours to draw the pieces
static const float DEFAULT_PLAYER_RED   = 0.5;
static const float DEFAULT_PLAYER_GREEN = 0.5;
static const float DEFAULT_PLAYER_BLUE  = 0.5;

/// @brief line width of the circle which surrounds the edit piece
static const float EDIT_CIRCLE_LINE_WIDTH = 1.0;

/// @brief number of squares of the edit piece board
static const int32_t NSQUARES_EDIT_PIECES_BOARD = 5;

TableEditPiece::TableEditPiece(
    BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException):
        Gtk::Table(cobject), //Calls the base class constructor
        m_gtkBuilder(a_gtkBuilder),
        m_thePiece(e_noPiece),
        m_red(DEFAULT_PLAYER_RED),
        m_green(DEFAULT_PLAYER_GREEN),
        m_blue(DEFAULT_PLAYER_BLUE)
{
	// retrieve the objects from the GUI design
	m_gtkBuilder->get_widget(GUI_DRAWINGAREA_EDITING_PIECE, m_editPieceDrawingArea);
	if (m_editPieceDrawingArea == NULL)
	{
		throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
	}

	m_gtkBuilder->get_widget(GUI_BUTTON_ROTATE_LEFT_NAME, m_rotateLeftButton);
    if (m_rotateLeftButton == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_BUTTON_ROTATE_RIGHT_NAME, m_rotateRightButton);
	if (m_rotateRightButton == NULL)
	{
		throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
	}

	m_gtkBuilder->get_widget(GUI_LABEL_ROTATE_NAME, m_rotateLabel);
    if (m_rotateLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_LABEL_MIRROR_NAME, m_mirrorLabel);
    if (m_mirrorLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_BUTTON_MIRROR_YAXIS_NAME, m_mirrorButtonYAxis);
	if (m_mirrorButtonYAxis == NULL)
	{
		throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
	}

	m_gtkBuilder->get_widget(GUI_BUTTON_MIRROR_XAXIS_NAME, m_mirrorButtonXAxis);
    if (m_mirrorButtonXAxis == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();

	// connect the signals to the handlers
	m_editPieceDrawingArea->signal_expose_event().connect(
			sigc::mem_fun(*this, &TableEditPiece::EditPieceDrawingArea_ExposeEvent));

	m_rotateRightButton->signal_clicked().connect(
			sigc::mem_fun(*this, &TableEditPiece::RotateRightButton_ButtonPressed));
	m_rotateLeftButton->signal_clicked().connect(
	            sigc::mem_fun(*this, &TableEditPiece::RotateLeftButton_ButtonPressed));
	m_mirrorButtonYAxis->signal_clicked().connect(
			sigc::mem_fun(*this, &TableEditPiece::MirrorYAxisButton_ButtonPressed));
	m_mirrorButtonXAxis->signal_clicked().connect(
            sigc::mem_fun(*this, &TableEditPiece::MirrorXAxisButton_ButtonPressed));

	// set the rotate and mirror button to not sensitive, since at the beginning
	// there's no piece loaded in the edit piece drawing area
	m_rotateRightButton->set_sensitive(false);
	m_rotateLeftButton->set_sensitive(false);
	m_rotateLabel->set_sensitive(false); // nicer visual effect
	m_mirrorLabel->set_sensitive(false); // nicer visual effect
	m_mirrorButtonYAxis->set_sensitive(false);
	m_mirrorButtonXAxis->set_sensitive(false);
}

TableEditPiece::~TableEditPiece()
{
}

void TableEditPiece::set_sensitive(gboolean a_sensitive)
{
    if ( a_sensitive && (GetPiece().GetType() != e_noPiece) )
    {
        SetPiece(e_noPiece);
    }

    // call the parent's method to apply visual changes
    Gtk::Table::set_sensitive(a_sensitive);
}

void TableEditPiece::SetPieceRGB(float a_red, float a_green, float a_blue)
{
	m_red   = a_red;
	m_green = a_green;
	m_blue  = a_blue;

	// update the view
	InvalidateEditPieceDrawingArea();
}

void TableEditPiece::SetPiece(ePieceType_t a_newPiece)
{
    // only update the piece ONLY if it changes and
    // widget is set to sensitive
    if ( (a_newPiece == m_thePiece.GetType()) ||
         (Gtk::Table::is_sensitive() == false) )
    {
        return;
    }

	m_thePiece = Piece(a_newPiece);

	if (m_thePiece.GetType() == e_noPiece)
	{
        m_rotateLabel->set_sensitive(false); // nicer visual effect
        m_mirrorLabel->set_sensitive(false); // nicer visual effect
        m_rotateRightButton->set_sensitive(false);
        m_rotateLeftButton->set_sensitive(false);
        m_mirrorButtonYAxis->set_sensitive(false);
        m_mirrorButtonXAxis->set_sensitive(false);
	}
	else
	{
        m_rotateLabel->set_sensitive(true); // nicer visual effect
        m_mirrorLabel->set_sensitive(true); // nicer visual effect
        m_rotateRightButton->set_sensitive(true);
        m_rotateLeftButton->set_sensitive(true);
        m_mirrorButtonYAxis->set_sensitive(true);
        m_mirrorButtonXAxis->set_sensitive(true);
	}

    // notify to whoever is listening to the signal that the editing piece changed
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
		int32_t squareSize = std::min(width - 3, height - 3);

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
                (squareSize/2) + 1,
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
                        (xc - littleSquare/2) + (littleSquare * m_thePiece.GetCoord(i).m_col) + 1,
                        (yc - littleSquare/2) + (littleSquare * m_thePiece.GetCoord(i).m_row) + 1,
                        littleSquare - 1,
                        littleSquare - 1);

                cr->fill();
            }
        }
	}

	return true;
}

void TableEditPiece::RotateRightButton_ButtonPressed()
{
    // if the piece has only 1 possible configuration do not modify it
    if ( (m_thePiece.GetNRotations() <= 1) && (m_thePiece.CanMirror() == false) )
    {
        return;
    }

	m_thePiece.RotateRight();

	// notify to whoever is listening to the signal that the piece changed
    m_signalPieceChanged.emit(m_thePiece);
    // update the view
	InvalidateEditPieceDrawingArea();
}

void TableEditPiece::RotateLeftButton_ButtonPressed()
{
    // if the piece has only 1 possible configuration do not modify it
    if ( (m_thePiece.GetNRotations() <= 1) && (m_thePiece.CanMirror() == false) )
    {
        return;
    }

    m_thePiece.RotateLeft();

    // notify to whoever is listening to the signal that the piece changed
    m_signalPieceChanged.emit(m_thePiece);
    // update the view
    InvalidateEditPieceDrawingArea();
}

void TableEditPiece::MirrorYAxisButton_ButtonPressed()
{
    // if the piece has only 1 possible configuration do not modify it
    if ( (m_thePiece.GetNRotations() <= 1) && (m_thePiece.CanMirror() == false) )
    {
        return;
    }

    m_thePiece.MirrorYAxis();

    // notify to whoever is listening to the signal that the piece changed
    m_signalPieceChanged.emit(m_thePiece);
    // update the view
    InvalidateEditPieceDrawingArea();
}

void TableEditPiece::MirrorXAxisButton_ButtonPressed()
{
    // if the piece has only 1 possible configuration do not modify it
    if ( (m_thePiece.GetNRotations() <= 1) && (m_thePiece.CanMirror() == false) )
    {
        return;
    }

    m_thePiece.MirrorXAxis();

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

#ifdef WIN32
void TableEditPiece::ForceTranslationOfWidgets()
{
    // in win32 systems gettext fails when the string is static and marked as
    // translatable with N_() but _() is never called explicitely. Basically
    // there are 2 kinds of strings that are not translated:
    //  + Those included in the GOptionEntry list, which show the available
    //    options that can be passed to the program through command line
    //  + Strings included in the .glade file that never change during the
    //    execution of the application, for example a menu called "Game", or a
    //    label that contains the word "rotate"
    //
    // We'll be calling here to _() for every string found in the .glade file
    // so it gets properly translated into the current domain (the 2nd case
    // described above)

    m_rotateLabel->set_label( _(m_rotateLabel->get_label().c_str()) );
    m_mirrorLabel->set_label( _(m_mirrorLabel->get_label().c_str()) );
}
#else
void TableEditPiece::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
