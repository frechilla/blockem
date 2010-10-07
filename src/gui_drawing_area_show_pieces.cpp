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
/// @file gui_drawing_area_show_pieces.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 25-Mar-2010  Original development
///           Faustino Frechilla 31-Mar-2010  No dependencies from any glade design
///           Faustino Frechilla 01-Apr-2010  Orientation option added to facilitate reusability
/// @endhistory
///
// ============================================================================

#include "gui_drawing_area_show_pieces.h"


// minimum size requested. The user won't be able to set the size of the window
// if it sets the size of this widget to something smaller than this value
// ican be applied to height or width depending on the way the widget will be used
// (vertical or horizontal)
static const int32_t MINIMUM_WIDGET_SIZE = 360;

// size of the array used to display the pieces for the user to select them
static const int32_t PICK_PLAYER_PIECES_ARRAY_NROWS = 7;
static const int32_t PICK_PLAYER_PIECES_ARRAY_NCOLS = 32;

// sorry for the length of this array (more than 600 characters per line)
// it helps out the editing when done in this way if you've got an horizontal
// editing bar, though it _might_ be a _bit_ complicated to edit it in a
// console
static const ePieceType_t pickPlayerPiecesArray
        [PICK_PLAYER_PIECES_ARRAY_NROWS][PICK_PLAYER_PIECES_ARRAY_NCOLS] =
{
    {e_1Piece_BabyPiece , e_noPiece          , e_2Piece_TwoPiece  , e_2Piece_TwoPiece , e_noPiece         , e_noPiece         , e_3Piece_Triangle, e_3Piece_Triangle, e_noPiece        , e_4Piece_LongPiece, e_4Piece_LongPiece , e_4Piece_LongPiece , e_4Piece_LongPiece, e_noPiece       , e_noPiece       , e_noPiece       , e_4Piece_LittleL, e_4Piece_LittleL, e_4Piece_LittleL, e_noPiece           , e_noPiece           , e_4Piece_LittleS    , e_4Piece_LittleS    , e_noPiece      , e_4Piece_FullSquare, e_4Piece_FullSquare, e_noPiece           , e_5Piece_CuntPiece  , e_5Piece_CuntPiece, e_5Piece_CuntPiece, e_noPiece    , e_5Piece_BigPenis},
    {e_noPiece          , e_noPiece          , e_noPiece          , e_noPiece         , e_noPiece         , e_noPiece         , e_noPiece        , e_3Piece_Triangle, e_noPiece        , e_noPiece         , e_noPiece          , e_noPiece          , e_noPiece         , e_noPiece       , e_4Piece_LittleT, e_noPiece       , e_noPiece       , e_noPiece       , e_4Piece_LittleL, e_noPiece           , e_4Piece_LittleS    , e_4Piece_LittleS    , e_noPiece           , e_noPiece      , e_4Piece_FullSquare, e_4Piece_FullSquare, e_noPiece           , e_5Piece_CuntPiece  , e_noPiece         , e_5Piece_CuntPiece, e_noPiece    , e_5Piece_BigPenis},
    {e_5Piece_HalfSquare, e_noPiece          , e_noPiece          , e_3Piece_LongPiece, e_3Piece_LongPiece, e_3Piece_LongPiece, e_noPiece        , e_noPiece        , e_noPiece        , e_noPiece         , e_5Piece_SquarePlus, e_5Piece_SquarePlus, e_noPiece         , e_4Piece_LittleT, e_4Piece_LittleT, e_4Piece_LittleT, e_noPiece       , e_noPiece       , e_noPiece       , e_noPiece           , e_noPiece           , e_noPiece           , e_noPiece           , e_noPiece      , e_noPiece          , e_noPiece          , e_noPiece           , e_noPiece           , e_noPiece         , e_noPiece         , e_noPiece    , e_5Piece_BigPenis},
    {e_5Piece_HalfSquare, e_noPiece          , e_noPiece          , e_noPiece         , e_noPiece         , e_noPiece         , e_noPiece        , e_noPiece        , e_noPiece        , e_noPiece         , e_5Piece_SquarePlus, e_5Piece_SquarePlus, e_noPiece         , e_noPiece       , e_noPiece       , e_noPiece       , e_noPiece       , e_5Piece_Cross  , e_noPiece       , e_noPiece           , e_noPiece           , e_noPiece           , e_5Piece_WPiece     , e_5Piece_WPiece, e_noPiece          , e_noPiece          , e_noPiece           , e_5Piece_TheUltimate, e_noPiece         , e_5Piece_BigL     , e_noPiece    , e_5Piece_BigPenis},
    {e_5Piece_HalfSquare, e_5Piece_HalfSquare, e_5Piece_HalfSquare, e_noPiece         , e_5Piece_BigS     , e_5Piece_BigS     , e_noPiece        , e_5Piece_SafPiece, e_noPiece        , e_noPiece         , e_5Piece_SquarePlus, e_noPiece          , e_noPiece         , e_noPiece       , e_5Piece_MrT    , e_noPiece       , e_5Piece_Cross  , e_5Piece_Cross  , e_5Piece_Cross  , e_noPiece           , e_noPiece           , e_noPiece           , e_noPiece           , e_5Piece_WPiece, e_5Piece_WPiece    , e_noPiece          , e_5Piece_TheUltimate, e_5Piece_TheUltimate, e_noPiece         , e_5Piece_BigL     , e_noPiece    , e_5Piece_BigPenis},
    {e_noPiece          , e_noPiece          , e_noPiece          , e_noPiece         , e_5Piece_BigS     , e_noPiece         , e_noPiece        , e_5Piece_SafPiece, e_5Piece_SafPiece, e_noPiece         , e_noPiece          , e_noPiece          , e_5Piece_MrT      , e_5Piece_MrT    , e_5Piece_MrT    , e_noPiece       , e_noPiece       , e_5Piece_Cross  , e_noPiece       , e_noPiece           , e_noPiece           , e_5Piece_BoringPiece, e_noPiece           , e_noPiece      , e_5Piece_WPiece    , e_noPiece          , e_5Piece_TheUltimate, e_noPiece           , e_noPiece         , e_5Piece_BigL     , e_noPiece    , e_noPiece        },
    {e_noPiece          , e_noPiece          , e_noPiece          , e_5Piece_BigS     , e_5Piece_BigS     , e_noPiece         , e_5Piece_SafPiece, e_5Piece_SafPiece, e_noPiece        , e_noPiece         , e_noPiece          , e_noPiece          , e_noPiece         , e_noPiece       , e_5Piece_MrT    , e_noPiece       , e_noPiece       , e_noPiece       , e_noPiece       , e_5Piece_BoringPiece, e_5Piece_BoringPiece, e_5Piece_BoringPiece, e_5Piece_BoringPiece, e_noPiece      , e_noPiece          , e_noPiece          , e_5Piece_TheUltimate, e_noPiece           , e_noPiece         , e_5Piece_BigL     , e_5Piece_BigL, e_noPiece        }
};

// This is a more human readable appearance of the pickPLayerPiecesArray if:
// e_noPiece            = 0
// e_1Piece_BabyPiece   = 1,
// e_2Piece_TwoPiece    = 2,
// e_3Piece_LongPiece   = 3,
// e_3Piece_Triangle    = 4,
// e_4Piece_LongPiece   = 5,
// e_4Piece_LittleS     = 6,
// e_4Piece_LittleT     = 7,
// e_4Piece_LittleL     = 8,
// e_4Piece_FullSquare  = 9,
// e_5Piece_BigS        = 10,
// e_5Piece_SafPiece    = 11,
// e_5Piece_WPiece      = 12,
// e_5Piece_CuntPiece   = 13,
// e_5Piece_BigPenis    = 14,
// e_5Piece_Cross       = 15,
// e_5Piece_HalfSquare  = 16,
// e_5Piece_BigL        = 17,
// e_5Piece_MrT         = 18,
// e_5Piece_SquarePlus  = 19,
// e_5Piece_BoringPiece = 20,
// e_5Piece_TheUltimate = 21,
//
// {
//   {1 , 0 , 2 , 2 , 0 , 0 , 4 , 4 , 0 , 5 , 5 , 5 , 5 , 0 , 0 , 0 , 8 , 8 , 8 , 0 , 0 , 6 , 6 , 0 , 9 , 9 , 0 , 13, 13, 13, 0 , 14},
//   {0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 , 0 , 0 , 0 , 0 , 0 , 0 , 7 , 0 , 0 , 0 , 8 , 0 , 6 , 6 , 0 , 0 , 9 , 9 , 0 , 13, 0 , 13, 0 , 14},
//   {16, 0 , 0 , 3 , 3 , 3 , 0 , 0 , 0 , 0 , 19, 19, 0 , 7 , 7 , 7 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 14},
//   {16, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 19, 19, 0 , 0 , 0 , 0 , 0 , 15, 0 , 0 , 0 , 0 , 12, 12, 0 , 0 , 0 , 21, 0 , 17, 0 , 14},
//   {16, 16, 16, 0 , 10, 10, 0 , 11, 0 , 0 , 19, 0 , 0 , 0 , 18, 0 , 15, 15, 15, 0 , 0 , 0 , 0 , 12, 12, 0 , 21, 21, 0 , 17, 0 , 14},
//   {0 , 0 , 0 , 0 , 10, 0 , 0 , 11, 11, 0 , 0 , 0 , 18, 18, 18, 0 , 0 , 15, 0 , 0 , 0 , 20, 0 , 0 , 12, 0 , 21, 0 , 0 , 17, 0 , 0 },
//   {0 , 0 , 0 , 10, 10, 0 , 11, 11, 0 , 0 , 0 , 0 , 0 , 0 , 18, 0 , 0 , 0 , 0 , 20, 20, 20, 20, 0 , 0 , 0 , 21 ,0 , 0 , 17, 17, 0 }
// };


DrawingAreaShowPieces::DrawingAreaShowPieces(const Player &a_player, eOrientation_t a_orientation):
	Gtk::DrawingArea(),
    m_thePlayer(&a_player),
    m_orientation(a_orientation)
{
	// the drawing area will handle the button_press event
    this->add_events(Gdk::BUTTON_PRESS_MASK);

    //default configuration of this widget
    switch (m_orientation)
    {
    case eOrientation_leftToRight:
    case eOrientation_rightToLeft:
    	set_size_request(MINIMUM_WIDGET_SIZE, -1);
    	break;

    case eOrientation_bottomToTop:
    case eOrientation_topToBottom:
    	set_size_request(-1, MINIMUM_WIDGET_SIZE);
    	break;
    }

    // configure tooltip signal handling
    this->set_has_tooltip();
    this->signal_query_tooltip().connect(sigc::mem_fun(
        *this, &DrawingAreaShowPieces::on_query_tooltip));
}

DrawingAreaShowPieces::~DrawingAreaShowPieces()
{
}

bool DrawingAreaShowPieces::on_expose_event(GdkEventExpose* event)
{
    Glib::RefPtr<Gdk::Window> window = this->get_window();
    if (!window)
    {
        return false;
    }

	Gtk::Allocation allocation = this->get_allocation();

	int32_t width  = allocation.get_width();
	int32_t height = allocation.get_height();

	// side of the small square which makes up the pieces
	int32_t littleSquare = 0;

	// coordinates for the centre of the drawing area
	// they are different depending on the orientation of the drawing area
	int32_t xc = 0;
	int32_t yc = 0;

	switch(m_orientation)
	{
	case eOrientation_leftToRight:
	case eOrientation_rightToLeft:
	{
		// horizontal orientation
		littleSquare = std::min(
							height / PICK_PLAYER_PIECES_ARRAY_NROWS,
							width  / PICK_PLAYER_PIECES_ARRAY_NCOLS);

		xc = width  / 2;
		yc = height / 2;
		break;
	}

	case eOrientation_bottomToTop:
	case eOrientation_topToBottom:
	{
		// height and width are turn around here because the drawing area
		// will be shown vertically
		littleSquare = std::min(
							width  / PICK_PLAYER_PIECES_ARRAY_NROWS,
							height / PICK_PLAYER_PIECES_ARRAY_NCOLS);

		xc = height / 2;
		yc = width  / 2;

		break;
	}
	} // switch(m_orientation)

	// the sides of the rectangle which contains the pieces
	int32_t rectHeight = littleSquare * PICK_PLAYER_PIECES_ARRAY_NROWS;
	int32_t rectWidth  = littleSquare * PICK_PLAYER_PIECES_ARRAY_NCOLS;

	// get the pen to draw
	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

	// clip to the area indicated by the expose event so that we only redraw
	// the portion of the window that needs to be redrawn
	cr->rectangle(event->area.x, event->area.y,
		event->area.width, event->area.height);
	cr->clip();

	// colour of the pieces
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
    m_thePlayer->GetColour(red, green, blue);
	cr->set_source_rgb(
	        static_cast<float>(red)  /255,
            static_cast<float>(green)/255,
            static_cast<float>(blue) /255);

	int32_t hzIndex = (xc - rectWidth/2) + (littleSquare/2);
	int32_t vtIndex = (yc - rectHeight/2) + (littleSquare/2);

	// draw player's pieces left
	switch(m_orientation)
	{
	case eOrientation_leftToRight:
	{
		for (int32_t row = 0; row < PICK_PLAYER_PIECES_ARRAY_NROWS; row++)
		{
			for (int32_t col = 0; col < PICK_PLAYER_PIECES_ARRAY_NCOLS; col++)
			{
				ePieceType_t pieceType = pickPlayerPiecesArray[row][col];
				if ( (pieceType != e_noPiece) &&
					 (m_thePlayer->IsPieceAvailable(pieceType)) )
				{
					cr->rectangle(
							(hzIndex - littleSquare/2) + 1,
							(vtIndex - littleSquare/2) + 1,
							littleSquare - 1,
							littleSquare - 1);

					cr->fill();
				}

				hzIndex += littleSquare;
			}

			hzIndex = (xc - rectWidth/2) + (littleSquare/2);
			vtIndex += littleSquare;
		}

		break;
	}
	case eOrientation_rightToLeft:
	{
		for (int32_t row = PICK_PLAYER_PIECES_ARRAY_NROWS - 1; row >= 0; row--)
		{
			for (int32_t col = 0; col < PICK_PLAYER_PIECES_ARRAY_NCOLS; col++)
			{
				ePieceType_t pieceType = pickPlayerPiecesArray[row][col];
				if ( (pieceType != e_noPiece) &&
					 (m_thePlayer->IsPieceAvailable(pieceType)) )
				{
					cr->rectangle(
							(hzIndex - littleSquare/2) + 1,
							(vtIndex - littleSquare/2) + 1,
							littleSquare - 1,
							littleSquare - 1);

					cr->fill();
				}

				hzIndex += littleSquare;
			}

			hzIndex = (xc - rectWidth/2) + (littleSquare/2);
			vtIndex += littleSquare;
		}

		break;
	}
	case eOrientation_bottomToTop:
	{
		for (int32_t row = 0; row < PICK_PLAYER_PIECES_ARRAY_NROWS; row++)
		{
			for (int32_t col = PICK_PLAYER_PIECES_ARRAY_NCOLS - 1; col >= 0; col--)
			{
				ePieceType_t pieceType = pickPlayerPiecesArray[row][col];
				if ( (pieceType != e_noPiece) &&
					 (m_thePlayer->IsPieceAvailable(pieceType)) )
				{
					cr->rectangle(
							(vtIndex - littleSquare/2) + 1,
							(hzIndex - littleSquare/2) + 1,
							littleSquare - 1,
							littleSquare - 1);

					cr->fill();
				}

				hzIndex += littleSquare;
			}

			hzIndex = (xc - rectWidth/2) + (littleSquare/2);
			vtIndex += littleSquare;
		}

		break;
	}
	case eOrientation_topToBottom:
	{
		for (int32_t row = PICK_PLAYER_PIECES_ARRAY_NROWS - 1; row >= 0; row--)
		{
			for (int32_t col = 0; col < PICK_PLAYER_PIECES_ARRAY_NCOLS; col++)
			{
				ePieceType_t pieceType = pickPlayerPiecesArray[row][col];
				if ( (pieceType != e_noPiece) &&
					 (m_thePlayer->IsPieceAvailable(pieceType)) )
				{
					cr->rectangle(
							(vtIndex - littleSquare/2) + 1,
							(hzIndex - littleSquare/2) + 1,
							littleSquare - 1,
							littleSquare - 1);

					cr->fill();
				}

				hzIndex += littleSquare;
			}

			hzIndex = (xc - rectWidth/2) + (littleSquare/2);
			vtIndex += littleSquare;
		}

		break;
	}
	} // switch(m_orientation)

	// commit the changes to the screen!
	cr->stroke();

    return true;
}

ePieceType_t DrawingAreaShowPieces::CoordinateToPieceType(
    int32_t x, int32_t y)
{
    Gtk::Allocation allocation = this->get_allocation();

    int32_t width  = allocation.get_width();
    int32_t height = allocation.get_height();

    // side of the small square which makes up the pieces
    int32_t littleSquare = 0;

    // the sides of the rectangle which contains the pieces
    int32_t rectHeight = 0;
    int32_t rectWidth = 0;

    switch(m_orientation)
    {
    case eOrientation_leftToRight:
    case eOrientation_rightToLeft:
    {
    	// horizontal orientation
        littleSquare = std::min(
							height / PICK_PLAYER_PIECES_ARRAY_NROWS,
							width  / PICK_PLAYER_PIECES_ARRAY_NCOLS);

        rectHeight = littleSquare * PICK_PLAYER_PIECES_ARRAY_NROWS;
        rectWidth  = littleSquare * PICK_PLAYER_PIECES_ARRAY_NCOLS;

    	break;
    }

    case eOrientation_bottomToTop:
    case eOrientation_topToBottom:
    {
    	// height and width are turn around here because the drawing area
    	// will be shown vertically
        littleSquare = std::min(
							width  / PICK_PLAYER_PIECES_ARRAY_NROWS,
							height / PICK_PLAYER_PIECES_ARRAY_NCOLS);

        rectHeight = littleSquare * PICK_PLAYER_PIECES_ARRAY_NCOLS;
        rectWidth  = littleSquare * PICK_PLAYER_PIECES_ARRAY_NROWS;

    	break;
    }
    } // switch(m_orientation)

    // coordinates for the centre of the drawing area
    int32_t xc = width  / 2;
    int32_t yc = height / 2;

    if ( ( ((x) > (xc - rectWidth/2))  && ((x) < (xc + rectWidth/2))  ) &&
         ( ((y) > (yc - rectHeight/2)) && ((y) < (yc + rectHeight/2)) ) )
    {
        int32_t row, col;

        if ( (m_orientation == eOrientation_leftToRight) ||
             (m_orientation == eOrientation_rightToLeft) )
        {
			for (col = 1; col < PICK_PLAYER_PIECES_ARRAY_NCOLS; col++)
			{
				if (x < ((xc - rectWidth/2) + (littleSquare * col) + 1))
				{
					break;
				}
			}

			for (row = 1; row < PICK_PLAYER_PIECES_ARRAY_NROWS; row++)
			{
				if (y < ((yc - rectHeight/2) + (littleSquare * row) + 1))
				{
					break;
				}
			}

			if (m_orientation == eOrientation_rightToLeft)
			{
				// row must be adapted to retrieve the real piece out of
				// pickPlayerPiecesArray
				row = PICK_PLAYER_PIECES_ARRAY_NROWS - row + 1;
			}
        }
        else // eOrientation_bottomToTop OR eOrientation_topToBottom
        {
			for (row = 1; row < PICK_PLAYER_PIECES_ARRAY_NROWS; row++)
			{
				if (x < ((xc - rectWidth/2) + (littleSquare * row) + 1))
				{
					break;
				}
			}

			for (col = 1; col < PICK_PLAYER_PIECES_ARRAY_NCOLS; col++)
			{
				if (y < ((yc - rectHeight/2) + (littleSquare * col) + 1))
				{
					break;
				}
			}

			if (m_orientation == eOrientation_bottomToTop)
			{
				// col must be adapted to retrieve the real piece out of
				// pickPlayerPiecesArray
				col = PICK_PLAYER_PIECES_ARRAY_NCOLS - col + 1;
			}
			else // (m_orientation == eOrientation_topToBottom)
			{
				// row must be adapted to retrieve the real piece out of
				// pickPlayerPiecesArray
				row = PICK_PLAYER_PIECES_ARRAY_NROWS - row + 1;
			}
        } // if ( (m_orientation == eOrientation_leftToRight) ...


        return pickPlayerPiecesArray[row-1][col-1];
    }

    return e_noPiece;
}

bool DrawingAreaShowPieces::on_query_tooltip(
    int x,
    int y,
    bool keyboard_tooltip,
    const Glib::RefPtr<Gtk::Tooltip>& tooltip)
{
    ePieceType_t pieceType = CoordinateToPieceType(x, y);

    if ( (pieceType != e_noPiece) &&
         (m_thePlayer->IsPieceAvailable(pieceType)) )
    {
        // mouse pointer is over an available piece. Show tooltip with
        // the name of the piece (it must be internationalised first)
        tooltip->set_text(_(Piece::GetPieceDescription(pieceType)));

        // true cos we want the tooltip to be shown right now
        return true;
    }

    // do not show the tooltip. Piece is not available or mouse is not
    // stopped over any piece
    return false;
}

bool DrawingAreaShowPieces::on_button_press_event(GdkEventButton *event)
{
    ePieceType_t pieceType = CoordinateToPieceType(event->x, event->y);

    if ( (pieceType != e_noPiece) &&
         (m_thePlayer->IsPieceAvailable(pieceType)) )
    {
        // the user clicked where some piece was being shown
        // notify it to whoever listens
        m_signalPiecePicked.emit(pieceType);
    }
    else
    {
        // User clicked in somewhere where there was no piece.
        // selected piece is restarted (no piece will be shown)
        m_signalPiecePicked.emit(e_noPiece);
    }

    return true;
}

bool DrawingAreaShowPieces::Invalidate(const Player &a_newPlayer)
{
    m_thePlayer = &a_newPlayer;
    return Invalidate();
}

bool DrawingAreaShowPieces::Invalidate()
{
    // force the drawing area to be redraw
    Glib::RefPtr<Gdk::Window> window = this->get_window();
    if(window)
    {
        Gdk::Rectangle rect(
                0,
                0,
                this->get_allocation().get_width(),
                this->get_allocation().get_height());

        window->invalidate_rect(rect, false);
        return true;
    }

    return false;
}
