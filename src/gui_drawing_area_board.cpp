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
/// @file gui_drawing_area_board.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 8-Apr-2010  Original development
/// @endhistory
///
// ============================================================================

#include <iostream>
#include "gui_drawing_area_board.h"
#include "rules.h"

/// @brief uninitialised coord value not to print pieces in random places
static const int32_t COORD_UNITIALISED = 0xffffffff;

//TODO this should be set using getters and setters
static const float STARTING_COORD_ALPHA_TRANSPARENCY = 0.4;

static const float BOARD_BORDER_RED        = 0.8;
static const float BOARD_BORDER_GREEN      = 0.0;
static const float BOARD_BORDER_BLUE       = 0.0;
static const float BOARD_BORDER_LINE_WIDTH = 5.0;

static const float BOARD_RED        = BOARD_BORDER_RED;
static const float BOARD_GREEN      = BOARD_BORDER_GREEN;
static const float BOARD_BLUE       = BOARD_BORDER_BLUE;
static const float BOARD_LINE_WIDTH = 1.0;

static const float GHOST_PIECE_WRONG_RED          = 0.9;
static const float GHOST_PIECE_WRONG_GREEN        = 0.0;
static const float GHOST_PIECE_WRONG_BLUE         = 0.0;
static const float GHOST_PIECE_ALPHA_TRANSPARENCY = 0.2;


DrawingAreaBoard::DrawingAreaBoard(const Board &a_board) :
    Gtk::DrawingArea(),
    m_theBoard(a_board),
    m_currentPlayer(NULL),
    m_currentPiece(e_noPiece),
    m_currentCoord(COORD_UNITIALISED, COORD_UNITIALISED)
{
    this->add_events(Gdk::BUTTON_PRESS_MASK);
    this->add_events(Gdk::POINTER_MOTION_MASK);
    this->add_events(Gdk::LEAVE_NOTIFY_MASK);
}

DrawingAreaBoard::~DrawingAreaBoard()
{
}

void DrawingAreaBoard::AddPlayerToList(const Player* a_player)
{
    m_playerList.push_back(a_player);
}

void DrawingAreaBoard::ResetPlayerList()
{
    m_playerList.clear();
}

bool DrawingAreaBoard::on_expose_event(GdkEventExpose* event)
{
    // This is where we draw on the window
    Glib::RefPtr<Gdk::Window> window = this->get_window();
    if(!window)
    {
        return false;
    }

    Gtk::Allocation allocation = this->get_allocation();

    int32_t width  = allocation.get_width();
    int32_t height = allocation.get_height();
    int32_t squareSize = std::min(width, height);

    int32_t littleSquare = std::min(
            squareSize / m_theBoard.GetNRows(),
            squareSize / m_theBoard.GetNColumns());

    int32_t squareHeight = littleSquare * m_theBoard.GetNRows();
    int32_t squareWidth  = littleSquare * m_theBoard.GetNColumns();

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

    std::list<const Player*>::const_iterator it = m_playerList.begin();
    while (it != m_playerList.end())
    {
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        const Player* thisPlayer = *it;
        thisPlayer->GetColour(red, green, blue);

        if ( thisPlayer->NumberOfPiecesAvailable() == e_numberOfPieces)
        {
            // if this player put down no pieces yet, draw a small circle in the starting point
            cr->set_source_rgba(
                    static_cast<float>(red)/255,
                    static_cast<float>(green)/255,
                    static_cast<float>(blue)/255,
                    STARTING_COORD_ALPHA_TRANSPARENCY);

            cr->arc(xc - squareWidth/2  +
                        (littleSquare * thisPlayer->GetStartingCoordinate().m_row) + littleSquare/2,
                    yc - squareHeight/2 +
                        (littleSquare * thisPlayer->GetStartingCoordinate().m_col) + littleSquare/2,
                    (littleSquare / 2) - (littleSquare / 4),
                    0.0,
                    2 * M_PI);

            cr->fill();
        }
        else
        {
            // this player already put some pieces on the board. Draw them all!
            cr->set_source_rgb(
                    static_cast<float>(red)/255,
                    static_cast<float>(green)/255,
                    static_cast<float>(blue)/255);

            for (int32_t rowCount = 0;
                         rowCount < m_theBoard.GetNRows() ;
                         rowCount++)
            {
                for (int32_t columnCount = 0;
                             columnCount <  m_theBoard.GetNColumns() ;
                             columnCount++)
                {
                    if (m_theBoard.IsPlayerInCoord(
                            rowCount,
                            columnCount,
                            *thisPlayer))
                    {
                        cr->rectangle(
                                (xc - squareWidth/2) +  (littleSquare * columnCount) + 1,
                                (yc - squareHeight/2) + (littleSquare * rowCount) + 1,
                                littleSquare - 1,
                                littleSquare - 1);

                        cr->fill();
                    }
                }
            }
        }

        // next player...
        it++;
    }


    // line width and colour for the border of the board
    cr->set_line_width(BOARD_BORDER_LINE_WIDTH);
    cr->set_source_rgb(
            BOARD_BORDER_RED,
            BOARD_BORDER_GREEN,
            BOARD_BORDER_BLUE);

    // draw the border of the board
    cr->move_to(xc - squareWidth/2, yc - squareHeight/2);
    cr->line_to(xc + squareWidth/2, yc - squareHeight/2);
    cr->line_to(xc + squareWidth/2, yc + squareHeight/2);
    cr->line_to(xc - squareWidth/2, yc + squareHeight/2);
    cr->line_to(xc - squareWidth/2, yc - squareHeight/2);

    // line for the insides of the board
    cr->set_line_width(BOARD_LINE_WIDTH);
    cr->set_source_rgb(
            BOARD_RED,
            BOARD_GREEN,
            BOARD_BLUE);

    // draw the inside lines of the board
    for (int32_t i = 1; i < m_theBoard.GetNRows(); i++)
    {
        cr->move_to(xc - squareWidth/2 + littleSquare*i, yc - squareHeight/2);
        cr->line_to(xc - squareWidth/2 + littleSquare*i, yc + squareHeight/2);
    }

    for (int32_t i = 1; i < m_theBoard.GetNColumns(); i++)
    {
        cr->move_to(xc - squareWidth/2, yc - squareHeight/2 + littleSquare*i);
        cr->line_to(xc + squareWidth/2, yc - squareHeight/2 + littleSquare*i);
    }

    // commit the changes to the screen!
    cr->stroke();

    // print the current selected piece in the place where the mouse pointer is
    // if there's a currently selected piece and a current player
    // we are so good we'll be using a bit of transparency!
    if ( (m_currentPlayer != NULL)                &&
         (m_currentPiece.GetType() != e_noPiece)  &&
         (m_currentCoord.m_row != COORD_UNITIALISED) &&
         (m_currentCoord.m_col != COORD_UNITIALISED) )
    {
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        m_currentPlayer->GetColour(red, green, blue);

        if (m_currentPlayer->NumberOfPiecesAvailable() == e_numberOfPieces)
        {
            if (Rules::IsPieceDeployableInStartingPoint(
                    m_theBoard,
                    m_currentPiece,
                    m_currentCoord,
                    m_currentPlayer->GetStartingCoordinate()))
            {
                cr->set_source_rgba(
                        static_cast<float>(red)/255,
                        static_cast<float>(green)/255,
                        static_cast<float>(blue)/255,
                        GHOST_PIECE_ALPHA_TRANSPARENCY);
            }
            else
            {
                cr->set_source_rgba(
                        GHOST_PIECE_WRONG_RED,
                        GHOST_PIECE_WRONG_GREEN,
                        GHOST_PIECE_WRONG_BLUE,
                        GHOST_PIECE_ALPHA_TRANSPARENCY);
            }
        }
        else if (Rules::IsPieceDeployable(
                m_theBoard,
                m_currentPiece,
                m_currentCoord,
                *m_currentPlayer))
        {
            cr->set_source_rgba(
                    static_cast<float>(red)/255,
                    static_cast<float>(green)/255,
                    static_cast<float>(blue)/255,
                    GHOST_PIECE_ALPHA_TRANSPARENCY);
        }
        else
        {
            cr->set_source_rgba(
                    GHOST_PIECE_WRONG_RED,
                    GHOST_PIECE_WRONG_GREEN,
                    GHOST_PIECE_WRONG_BLUE,
                    GHOST_PIECE_ALPHA_TRANSPARENCY);
        }

        // draw the piece in the edit pieces drawing area
        for (uint8_t i = 0; i < m_currentPiece.GetNSquares(); i++)
        {
            Coordinate coord(
                    m_currentCoord.m_row + m_currentPiece.m_coords[i].m_row,
                    m_currentCoord.m_col + m_currentPiece.m_coords[i].m_col);

            cr->rectangle(
                    (xc - squareWidth /2) + (littleSquare * coord.m_col) + 1,
                    (yc - squareHeight/2) + (littleSquare * coord.m_row) + 1,
                    littleSquare - 1,
                    littleSquare - 1);

            cr->fill();
        }
    }

    return true;
}

bool DrawingAreaBoard::on_button_press_event(GdkEventButton* event)
{
#ifdef DEBUG_PRINT
    std::cout << "clicked in (" << event->x << ", " << event->y << ")" << std::endl;
#endif

    Coordinate thisCoord;
    if ( (m_currentPlayer == NULL)               ||
         (m_currentPiece.GetType() == e_noPiece) ||
         (DrawingAreaToBoardCoord(event->x, event->y, thisCoord) == false) )
    {
        return true;
    }

#ifdef DEBUG_PRINT
    std::cout << "SQUARE: (" << thisCoord.m_row << ", " << thisCoord.m_col << ")" << std::endl;
#endif

    // someone clicked on the board. Notify the coordinate, the piece and the player who did it
    m_signalBoardClicked.emit(thisCoord, m_currentPiece, *m_currentPlayer);

    return true;
}

bool DrawingAreaBoard::on_motion_notify_event(GdkEventMotion* event)
{
    Coordinate thisCoord;
    if (DrawingAreaToBoardCoord(event->x, event->y, thisCoord))
    {
        // the mouse is moving inside the board now
        if ( (m_currentCoord.m_row != thisCoord.m_row) ||
             (m_currentCoord.m_col != thisCoord.m_col) )
        {
            m_currentCoord.m_row = thisCoord.m_row;
            m_currentCoord.m_col = thisCoord.m_col;

            // force the board to be redrawn. There has been a change
            // in the last coord, which might be used to draw the
            // ghost piece
            Invalidate();
        }
    }
    else if ( (m_currentCoord.m_row != COORD_UNITIALISED) ||
              (m_currentCoord.m_col != COORD_UNITIALISED) )
    {
        // the mouse is moving outside the board. update the value
        // of the last coord to unitialised so no "ghost" piece will be
        // painted on the board
        m_currentCoord.m_row = m_currentCoord.m_col = COORD_UNITIALISED;

        // force the board to be redraw since there might be a ghost piece
        // drawn on the board
        Invalidate();
    }

    return true;
}

bool DrawingAreaBoard::on_leave_notify_event(GdkEventCrossing* event)
{
    if ( (m_currentCoord.m_row != COORD_UNITIALISED) ||
         (m_currentCoord.m_col != COORD_UNITIALISED) )
    {
        m_currentCoord.m_row = m_currentCoord.m_col = COORD_UNITIALISED;

        // force the board to be redraw. The mouse pointer went out
        // of the drawing area
        Invalidate();
    }

    return true;
}

bool DrawingAreaBoard::DrawingAreaToBoardCoord(
        int32_t a_windowX,
        int32_t a_windowY,
        Coordinate &out_boardCoord)
{
    Glib::RefPtr<Gdk::Window> window = this->get_window();
    if (!window)
    {
        return false;
    }

    Gtk::Allocation allocation = this->get_allocation();

    int32_t width  = allocation.get_width();
    int32_t height = allocation.get_height();
    int32_t squareSize = std::min(width, height);

    int32_t littleSquare = std::min(
                squareSize / m_theBoard.GetNRows(),
                squareSize / m_theBoard.GetNColumns());

    int32_t squareHeight = littleSquare * m_theBoard.GetNRows();
    int32_t squareWidth  = littleSquare * m_theBoard.GetNColumns();

    int32_t xc = width  / 2;
    int32_t yc = height / 2;

    if ( ( (a_windowX > (xc - squareWidth/2))  && (a_windowX < (xc + squareWidth/2))  ) &&
         ( (a_windowY > (yc - squareHeight/2)) && (a_windowY < (yc + squareHeight/2)) ) )
    {
        int32_t row, col;
        for (row = 0; row < m_theBoard.GetNRows(); row++)
        {
            if (a_windowY < ((yc - squareHeight/2) + (littleSquare * row)))
            {
                break;
            }
        }

        for (col = 1; col < m_theBoard.GetNColumns(); col++)
        {
            if (a_windowX < ((xc - squareWidth/2) + (littleSquare * col)))
            {
                break;
            }
        }

        out_boardCoord.m_row = row - 1;
        out_boardCoord.m_col = col - 1;

        return true;
    }

    return false;
}

/*
void DrawingAreaBoard::SetSquareInBoard(const Coordinate &a_coord, Cairo::RefPtr<Cairo::Context> cr)
{
    Glib::RefPtr<Gdk::Window> window = this->get_window();
    if (window)
    {
        Gtk::Allocation allocation = this->get_allocation();

        int32_t width  = allocation.get_width();
        int32_t height = allocation.get_height();
        int32_t squareSize = std::min(width, height);

        int32_t littleSquare = std::min(
                    squareSize / m_theBoard.GetNRows(),
                    squareSize / m_theBoard.GetNColumns());

        int32_t squareHeight = littleSquare * m_theBoard.GetNRows();
        int32_t squareWidth  = littleSquare * m_theBoard.GetNColumns();

        int32_t xc = width  / 2;
        int32_t yc = height / 2;

        cr->rectangle(
                (xc - squareWidth/2) +  (littleSquare * a_coord.m_col) + 1,
                (yc - squareHeight/2) + (littleSquare * a_coord.m_row) + 1,
                littleSquare - 1,
                littleSquare - 1);

        cr->fill();
    }
}
*/

bool DrawingAreaBoard::Invalidate()
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
