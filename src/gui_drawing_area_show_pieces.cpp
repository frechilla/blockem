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
/// @endhistory
///
// ============================================================================

#include "gui_drawing_area_show_pieces.h"
#include "gui_glade.h"


// default colours to draw the pieces
static const float DEFAULT_PLAYER_RED   = 0.3;
static const float DEFAULT_PLAYER_GREEN = 0.8;
static const float DEFAULT_PLAYER_BLUE  = 0.3;

// size of the array used to display the pieces for the user to select them
const int32_t PICK_PLAYER_PIECES_ARRAY_NROWS = 7;
const int32_t PICK_PLAYER_PIECES_ARRAY_NCOLS = 32;

// sorry for the length of this array (more than 600 characters per line)
// it helps out the editing when done in this way if you've got an horizontal
// editing bar, though it _might_ be a _bit_ complicated to edit it in a
// console
const ePieceType_t pickPlayerPiecesArray
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


DrawingAreaPickPieces::DrawingAreaPickPieces(Glib::RefPtr<Gnome::Glade::Xml> a_refXml, const Player &a_player)
throw (GUIException) :
    m_refXml(a_refXml),
    m_thePlayer(a_player),
    m_red(DEFAULT_PLAYER_RED),
    m_green(DEFAULT_PLAYER_GREEN),
    m_blue(DEFAULT_PLAYER_BLUE)
{
    if (!m_refXml)
    {
        throw new GUIException(std::string("Invalid reference to Gnome::Glade::Xml in DrawingAreaPickPieces"));
    }

    m_refXml->get_widget(GUI_DRAWINGAREA_PICK_PIECES, m_theDrawingArea);
    if (m_theDrawingArea == NULL)
    {
        throw new GUIException(std::string("Pick pieces drawing area retrieval failed"));
    }

    // connect the signals to the handlers
    m_theDrawingArea->signal_expose_event().connect(
            sigc::mem_fun(*this, &DrawingAreaPickPieces::ExposeEvent));
    m_theDrawingArea->add_events(Gdk::BUTTON_PRESS_MASK);
    m_theDrawingArea->signal_button_press_event().connect(
                sigc::mem_fun(*this, &DrawingAreaPickPieces::ButtonPressed));
}

DrawingAreaPickPieces::~DrawingAreaPickPieces()
{
}

void DrawingAreaPickPieces::SetPlayerRGB(float a_red, float a_green, float a_blue)
{
    m_red   = a_red;
    m_green = a_green;
    m_blue  = a_blue;
}

bool DrawingAreaPickPieces::ExposeEvent(GdkEventExpose* event)
{
    Glib::RefPtr<Gdk::Window> window = m_theDrawingArea->get_window();
    if(window)
    {
        Gtk::Allocation allocation = m_theDrawingArea->get_allocation();

        int32_t width  = allocation.get_width();
        int32_t height = allocation.get_height();

        int32_t littleSquare = std::min(
                                height / PICK_PLAYER_PIECES_ARRAY_NROWS,
                                width / PICK_PLAYER_PIECES_ARRAY_NCOLS);
        int32_t squareHeight = littleSquare * PICK_PLAYER_PIECES_ARRAY_NROWS;
        int32_t squareWidth  = littleSquare * PICK_PLAYER_PIECES_ARRAY_NCOLS;

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

        // colour of the pieces
        cr->set_source_rgb(
                m_red,
                m_green,
                m_blue);

        // draw the opponent's pieces left
        int32_t hzIndex = (xc - squareWidth/2) + (littleSquare/2);
        int32_t vtIndex = (yc - squareHeight/2) + (littleSquare/2);
        for (int32_t row = 0; row < PICK_PLAYER_PIECES_ARRAY_NROWS; row++)
        {
            for (int32_t col = 0; col < PICK_PLAYER_PIECES_ARRAY_NCOLS; col++)
            {
                ePieceType_t pieceType = pickPlayerPiecesArray[row][col];
                if ( (pieceType != e_noPiece) &&
                     (m_thePlayer.IsPieceAvailable(pieceType)) )
                {
                    cr->rectangle(
                            (hzIndex - littleSquare/2) + 1,
                            (vtIndex - littleSquare/2) + 1,
                            littleSquare - 2,
                            littleSquare - 2);

                    cr->fill();
                }
                hzIndex += littleSquare;
            }

            hzIndex = (xc - squareWidth/2) + (littleSquare/2);
            vtIndex += littleSquare;
        }

        // commit the changes to the screen!
        cr->stroke();
    }

    return true;
}

bool DrawingAreaPickPieces::ButtonPressed(GdkEventButton *event)
{
    //std::cout << "clicked in (" << event->x << ", " << event->y << ")" << std::endl;

    Glib::RefPtr<Gdk::Window> window = m_theDrawingArea->get_window();
    if (!window)
    {
        return false;
    }

    Gtk::Allocation allocation = m_theDrawingArea->get_allocation();

    int32_t width  = allocation.get_width();
    int32_t height = allocation.get_height();

    int32_t littleSquare = std::min(
                            height / PICK_PLAYER_PIECES_ARRAY_NROWS,
                            width / PICK_PLAYER_PIECES_ARRAY_NCOLS);
    int32_t squareHeight = littleSquare * PICK_PLAYER_PIECES_ARRAY_NROWS;
    int32_t squareWidth  = littleSquare * PICK_PLAYER_PIECES_ARRAY_NCOLS;

    int32_t xc = width  / 2;
    int32_t yc = height / 2;

    if ( ( ((event->x) > (xc - squareWidth/2))  && ((event->x) < (xc + squareWidth/2))  ) &&
         ( ((event->y) > (yc - squareHeight/2)) && ((event->y) < (yc + squareHeight/2)) ) )
    {
        int32_t row, col;
        for (col = 0; col < PICK_PLAYER_PIECES_ARRAY_NCOLS; col++)
        {
            if (event->x < ((xc - squareWidth/2) + (littleSquare * col)))
            {
                break;
            }
        }

        for (row = 1; row < PICK_PLAYER_PIECES_ARRAY_NROWS; row++)
        {
            if (event->y < ((yc - squareHeight/2) + (littleSquare * row)))
            {
                break;
            }
        }

        ePieceType_t pieceType = pickPlayerPiecesArray[row-1][col-1];
        if ( (pieceType != e_noPiece) &&
             (m_thePlayer.IsPieceAvailable(pieceType)) )
        {
            // the user clicked where some piece was being shown
            // notify it to whoever listens
            m_signalPiecePicked.emit(pieceType);

            /*
            if (pieceType != m_editPiece.GetType())
            {
                UpdateSelectedPiece(pieceType);
            }
             */

            return true;
        }
    }

    // got to here. The User clicked in somewhere where there was no piece.
    // selected piece is restarted (no piece will be shown)
    m_signalPiecePicked.emit(e_noPiece);

    return true;
}

bool DrawingAreaPickPieces::Invalidate()
{
    // force the drawing area to be redraw
    Glib::RefPtr<Gdk::Window> window = m_theDrawingArea->get_window();
    if(window)
    {
        Gdk::Rectangle rect(
                0,
                0,
                m_theDrawingArea->get_allocation().get_width(),
                m_theDrawingArea->get_allocation().get_height());

        window->invalidate_rect(rect, false);
        return true;
    }

    return false;
}
