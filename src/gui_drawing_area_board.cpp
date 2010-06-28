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

//TODO this should be set using getters and setters
static const float BOARD_BORDER_RED        = 0.8;
static const float BOARD_BORDER_GREEN      = 0.0;
static const float BOARD_BORDER_BLUE       = 0.0;
static const float BOARD_BORDER_LINE_WIDTH = 5.0;

static const float BOARD_RED        = BOARD_BORDER_RED;
static const float BOARD_GREEN      = BOARD_BORDER_GREEN;
static const float BOARD_BLUE       = BOARD_BORDER_BLUE;
static const float BOARD_LINE_WIDTH = 1.0;

static const float COLOUR_BLACK_CHANNEL_RED   = 0.0;
static const float COLOUR_BLACK_CHANNEL_GREEN = 0.0;
static const float COLOUR_BLACK_CHANNEL_BLUE  = 0.0;

static const float STARTING_COORD_ALPHA    = 0.6;
static const float GHOST_PIECE_ALPHA_RIGHT = 0.8;
static const float GHOST_PIECE_ALPHA_WRONG = 0.2;
static const float INFLUENCE_AREA_ALPHA    = 0.3;
static const float FORBIDDEN_AREA_ALPHA    = 0.5;

// 1000 = 1 second
static const uint32_t LAST_PIECE_EFFECT_MILLIS = 300;
// minimum transparency for the latest piece deployed effect
static const float    LAST_PIECE_EFFECT_MIN_ALPHA = 0.4;
// initial level of transparency for the last piece deployed effect
static const float    LAST_PIECE_EFFECT_INITIAL_ALPHA = 1.0;


DrawingAreaBoard::DrawingAreaBoard(const Board &a_board) :
    Gtk::DrawingArea(),
    m_theBoard(a_board),
    m_currentPlayer(NULL),
    m_currentPiece(e_noPiece),
    m_currentCoord(COORD_UNINITIALISED, COORD_UNINITIALISED),
    m_latestPieceDeployedEffectOn(false), // no glowing piece effect at the beginning
    m_latestPieceDeployed(e_noPiece),
    m_latestPieceDeployedCoord(COORD_UNINITIALISED, COORD_UNINITIALISED),
    m_latestPieceDeployedPlayer(NULL),
    m_latestPieceDeployedTransparency(LAST_PIECE_EFFECT_INITIAL_ALPHA),
    m_showNKPoints(false),       // nk points won't be shown by default
    m_forbiddenAreaPlayer(NULL), // no forbidden area will be shown by default
    m_influenceAreaPlayer(NULL)  // no influence area will be shown by default
{
    // these events are going to be handled by the drawing area (apart from the usual expose event)
    this->add_events(Gdk::BUTTON_PRESS_MASK);
    this->add_events(Gdk::POINTER_MOTION_MASK);
    this->add_events(Gdk::LEAVE_NOTIFY_MASK);
}

DrawingAreaBoard::~DrawingAreaBoard()
{
}

void DrawingAreaBoard::AddPlayerToList(const Player &a_player)
{
    m_playerList.push_back(&a_player);
}

void DrawingAreaBoard::ResetPlayerList()
{
    m_playerList.clear();
}

void DrawingAreaBoard::SetCurrentPlayer(const Player &a_player)
{
    m_currentPlayer = &a_player;
}

void DrawingAreaBoard::UnsetCurrentPlayer()
{
    m_currentPlayer = NULL;
}

void DrawingAreaBoard::SetCurrentPiece(const Piece &a_piece)
{
    m_currentPiece = a_piece;
}

void DrawingAreaBoard::ShowNucleationPoints()
{
    if (m_showNKPoints == false)
    {
        // Invalidating is expensive. Do it only if it is needed
        m_showNKPoints = true;
        Invalidate();
    }
}

void DrawingAreaBoard::HideNucleationPoints()
{
    if (m_showNKPoints == true)
    {
        // Invalidating is expensive. Do it only if it is needed
        m_showNKPoints = false;
        Invalidate();
    }
}

void DrawingAreaBoard::ShowPlayerForbiddenArea(const Player &a_player)
{
    if (m_forbiddenAreaPlayer != &a_player)
    {
        m_forbiddenAreaPlayer = &a_player;
        Invalidate();
    }
}

void DrawingAreaBoard::HidePlayerForbiddenArea()
{
    if (m_forbiddenAreaPlayer != NULL)
    {
        // Invalidating is expensive. Do it only if it is needed
        m_forbiddenAreaPlayer = NULL;
        Invalidate();
    }
}

void DrawingAreaBoard::ShowPlayerInfluenceArea(const Player &a_player)
{
    if (m_influenceAreaPlayer != &a_player)
    {
        m_influenceAreaPlayer = &a_player;
        Invalidate();
    }
}

void DrawingAreaBoard::HidePlayerInfluenceArea()
{
    if (m_influenceAreaPlayer != NULL)
    {
        // Invalidating is expensive. Do it only if it is needed
        m_influenceAreaPlayer = NULL;
        Invalidate();
    }
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
    //cr->save();

    // clip to the area indicated by the expose event so that we only redraw
    // the portion of the window that needs to be redrawn
    cr->rectangle(event->area.x, event->area.y,
        event->area.width, event->area.height);
    cr->clip();

    // set with all the nk points of the players. if a nk point is shared
    // between players it'll draw with another colour
    STLCoordinateSet_t globalNKPointSet;

    std::list<const Player*>::const_iterator it = m_playerList.begin();
    while (it != m_playerList.end())
    {
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        const Player* thisPlayer = *it;
        thisPlayer->GetColour(red, green, blue);

        if (thisPlayer->NumberOfPiecesAvailable() == e_numberOfPieces)
        {
            // if this player put down no pieces yet, draw a small circle in the starting point
            cr->set_source_rgba(
                    static_cast<float>(red)  / 255,
                    static_cast<float>(green)/ 255,
                    static_cast<float>(blue) / 255,
                    STARTING_COORD_ALPHA);

            cr->arc(xc - squareWidth/2  +
                        (littleSquare * thisPlayer->GetStartingCoordinate().m_col) + littleSquare/2,
                    yc - squareHeight/2 +
                        (littleSquare * thisPlayer->GetStartingCoordinate().m_row) + littleSquare/2,
                    (littleSquare / 2) - (littleSquare / 4),
                    0.0,
                    2 * M_PI);

            cr->fill();
        }
        else
        {
            // this player already put some pieces on the board.
            // go through all the squares of the board drawing them all +
            // some information that the board drawing area has been requested
            // to draw too
            cr->set_source_rgb(
                    static_cast<float>(red)  / 255,
                    static_cast<float>(green)/ 255,
                    static_cast<float>(blue) / 255);

            Coordinate thisCoord(0, 0);
            for (thisCoord.m_row = 0;
                 thisCoord.m_row < m_theBoard.GetNRows() ;
                 thisCoord.m_row++)
            {
                for (thisCoord.m_col = 0;
                     thisCoord.m_col <  m_theBoard.GetNColumns() ;
                     thisCoord.m_col++)
                {
                    if (m_theBoard.IsPlayerInCoord(
                            thisCoord,
                            *thisPlayer))
                    {
                        cr->rectangle(
                                (xc - squareWidth/2) +  (littleSquare * thisCoord.m_col) + 1,
                                (yc - squareHeight/2) + (littleSquare * thisCoord.m_row) + 1,
                                littleSquare - 1,
                                littleSquare - 1);

                        cr->fill();
                    }
                    else if ( m_theBoard.IsCoordEmpty(thisCoord) &&
                              ( (m_forbiddenAreaPlayer != NULL) ||
                                (m_influenceAreaPlayer != NULL) ) )

                    {
                        // save current cairo context in an internal stack. This coord
                        // might belong to current player's forbidden or influence area
                        cr->save();

                        // draw the influence area
                        if ( (thisPlayer == m_influenceAreaPlayer) &&
                             thisPlayer->IsCoordInfluencedByPlayer(thisCoord) )
                        {
                            cr->set_source_rgba(
                                    static_cast<float>(red)  / 255,
                                    static_cast<float>(green)/ 255,
                                    static_cast<float>(blue) / 255,
                                    INFLUENCE_AREA_ALPHA);

                            cr->rectangle(
                                    (xc - squareWidth/2) +  (littleSquare * thisCoord.m_col) + 1,
                                    (yc - squareHeight/2) + (littleSquare * thisCoord.m_row) + 1,
                                    littleSquare - 1,
                                    littleSquare - 1);

                            cr->fill();
                        }

                        // mark the coords in the board where the current player can't go
                        // if a coordinate belongs to the influenced area it can't belong
                        // to the forbidden area (see definition of influenced area in rules.h)
                        // it can save a few cycles cos IsCoordTouchingPlayer is not a trivial function
                        if ( (thisPlayer == m_forbiddenAreaPlayer) &&
                             (thisPlayer->IsCoordInfluencedByPlayer(thisCoord) == false) &&
                             rules::IsCoordTouchingPlayerCompute(m_theBoard, thisCoord, *thisPlayer) )
                        {
                            // forbidden areas are drawn with exactly the inverse colour
                            cr->set_source_rgba(
                                    1 - (static_cast<float>(red)  / 255),
                                    1 - (static_cast<float>(green)/ 255),
                                    1 - (static_cast<float>(blue) / 255),
                                    FORBIDDEN_AREA_ALPHA);

                            cr->rectangle(
                                    (xc - squareWidth/2) +  (littleSquare * thisCoord.m_col) + 1,
                                    (yc - squareHeight/2) + (littleSquare * thisCoord.m_row) + 1,
                                    littleSquare - 1,
                                    littleSquare - 1);

                            cr->fill();
                        }

                        // restore the cairo context from the internal stack
                        cr->restore();
                    }
                } // for (int32_t columnCount
            } // for (int32_t rowCount

            // draw a small little circle where nk points are (with a bit of transparency)
            if (m_showNKPoints)
            {
                cr->set_source_rgba(
                        static_cast<float>(red)  / 255,
                        static_cast<float>(green)/ 255,
                        static_cast<float>(blue) / 255,
                        STARTING_COORD_ALPHA);

                STLCoordinateSet_t nkPointSet;
                thisPlayer->GetAllNucleationPoints(nkPointSet);
                STLCoordinateSet_t::const_iterator nkIterator = nkPointSet.begin();
                while(nkIterator != nkPointSet.end())
                {
                    const Coordinate &thisCoord = *nkIterator;
                    STLCoordinateSet_t::iterator globalIterator = globalNKPointSet.find(thisCoord);
                    if (globalIterator == globalNKPointSet.end())
                    {
                        // this nk point is not shared. Draw it with the default colour
                        globalNKPointSet.insert(thisCoord);

                        cr->arc(xc - squareWidth/2  +
                                    (littleSquare * thisCoord.m_col) + littleSquare/2,
                                yc - squareHeight/2 +
                                    (littleSquare * thisCoord.m_row) + littleSquare/2,
                                (littleSquare / 2) - (littleSquare / 3),
                                0.0,
                                2 * M_PI);
                        cr->fill();
                    }
                    else
                    {
                        // shared nk point. Draw it black
                        cr->save();

                        cr->set_operator(Cairo::OPERATOR_CLEAR);
                        cr->set_source_rgba(
                                COLOUR_BLACK_CHANNEL_RED,
                                COLOUR_BLACK_CHANNEL_GREEN,
                                COLOUR_BLACK_CHANNEL_BLUE,
                                STARTING_COORD_ALPHA);

                        cr->arc(xc - squareWidth/2  +
                                    (littleSquare * thisCoord.m_col) + littleSquare/2,
                                yc - squareHeight/2 +
                                    (littleSquare * thisCoord.m_row) + littleSquare/2,
                                (littleSquare / 2) - (littleSquare / 3),
                                0.0,
                                2 * M_PI);
                        cr->fill();

                        cr->restore();
                    }

                    nkIterator++;
                } // while(nkIterator != nkPointSet.end())
            } // if (m_showNKPoints)
        } // else if ( thisPlayer->NumberOfPiecesAvailable() == e_numberOfPieces)

        // next player...
        it++;
    }

    // print the current selected piece in the place where the mouse pointer is
    // if there's a currently selected piece and a current player
    // we are so good we'll be using a bit of transparency!
    if ( (m_currentPlayer != NULL)                     &&
         (m_currentPiece.GetType() != e_noPiece)       &&
         (m_currentCoord.m_row != COORD_UNINITIALISED) &&
         (m_currentCoord.m_col != COORD_UNINITIALISED) )
    {
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        m_currentPlayer->GetColour(red, green, blue);

        if (m_currentPlayer->NumberOfPiecesAvailable() == e_numberOfPieces)
        {
            if (rules::IsPieceDeployableInStartingPoint(
                    m_theBoard,
                    m_currentPiece.GetCurrentConfiguration(),
                    m_currentCoord,
                    m_currentPlayer->GetStartingCoordinate()))
            {
                cr->set_source_rgba(
                        static_cast<float>(red)  / 255,
                        static_cast<float>(green)/ 255,
                        static_cast<float>(blue) / 255,
                        GHOST_PIECE_ALPHA_RIGHT);
            }
            else
            {
                cr->set_source_rgba(
                        static_cast<float>(red)  / 255,
                        static_cast<float>(green)/ 255,
                        static_cast<float>(blue) / 255,
                        GHOST_PIECE_ALPHA_WRONG);
            }
        }
        else if (rules::IsPieceDeployableCompute(
                m_theBoard,
                m_currentPiece.GetCurrentConfiguration(),
                m_currentCoord,
                *m_currentPlayer))
        {
            cr->set_source_rgba(
                    static_cast<float>(red)  / 255,
                    static_cast<float>(green)/ 255,
                    static_cast<float>(blue) / 255,
                    GHOST_PIECE_ALPHA_RIGHT);
        }
        else
        {
            cr->set_source_rgba(
                    static_cast<float>(red)  / 255,
                    static_cast<float>(green)/ 255,
                    static_cast<float>(blue) / 255,
                    GHOST_PIECE_ALPHA_WRONG);
        }

        // draw the piece
        for (uint8_t i = 0; i < m_currentPiece.GetNSquares(); i++)
        {
            Coordinate coord(
                    m_currentCoord.m_row + m_currentPiece.GetCoord(i).m_row,
                    m_currentCoord.m_col + m_currentPiece.GetCoord(i).m_col);

            cr->rectangle(
                    (xc - squareWidth /2) + (littleSquare * coord.m_col) + 1,
                    (yc - squareHeight/2) + (littleSquare * coord.m_row) + 1,
                    littleSquare - 1,
                    littleSquare - 1);

            cr->fill();
        }
    }

    //cr->restore();

    // add a beautiful effect to the latest piece deployed on the board
    if ( (m_latestPieceDeployedEffectOn == true) &&
         (m_latestPieceDeployedPlayer != NULL)   &&
         (m_latestPieceDeployed.GetType() != e_noPiece) )
    {
        //TODO have a look at cairo_save and cairo_restore to draw transparency
        // http://www.cairographics.org/manual/cairo-context.html
        //
        // or maybe have a look at the operator CAIRO_OPERATOR_CLEAR instead of SOURCE
        // http://cairographics.org/operators/

        // The second object is drawn as if nothing else were below
        cr->set_operator(Cairo::OPERATOR_SOURCE);

        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        m_latestPieceDeployedPlayer->GetColour(red, green, blue);

        cr->set_source_rgba(
                    static_cast<float>(red)  / 255,
                    static_cast<float>(green)/ 255,
                    static_cast<float>(blue) / 255,
                    m_latestPieceDeployedTransparency);

        // draw the piece
        for (uint8_t i = 0; i < m_latestPieceDeployed.GetNSquares(); i++)
        {
            Coordinate coord(
                    m_latestPieceDeployedCoord.m_row + m_latestPieceDeployed.GetCoord(i).m_row,
                    m_latestPieceDeployedCoord.m_col + m_latestPieceDeployed.GetCoord(i).m_col);

            cr->rectangle(
                    (xc - squareWidth /2) + (littleSquare * coord.m_col) + 1,
                    (yc - squareHeight/2) + (littleSquare * coord.m_row) + 1,
                    littleSquare - 1,
                    littleSquare - 1);

            cr->fill();
        }
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
    else if ( (m_currentCoord.m_row != COORD_UNINITIALISED) ||
              (m_currentCoord.m_col != COORD_UNINITIALISED) )
    {
        // the mouse is moving outside the board. update the value
        // of the last coord to unitialised so no "ghost" piece will be
        // painted on the board
        m_currentCoord.m_row = m_currentCoord.m_col = COORD_UNINITIALISED;

        // force the board to be redraw since there might be a ghost piece
        // drawn on the board
        Invalidate();
    }

    return true;
}

bool DrawingAreaBoard::on_leave_notify_event(GdkEventCrossing* event)
{
    if ( (m_currentCoord.m_row != COORD_UNINITIALISED) ||
         (m_currentCoord.m_col != COORD_UNINITIALISED) )
    {
        m_currentCoord.m_row = m_currentCoord.m_col = COORD_UNINITIALISED;

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

bool DrawingAreaBoard::Invalidate(const Piece &a_piece, const Coordinate &a_coord, const Player& a_player)
{
    m_latestPieceDeployed = a_piece;
    m_latestPieceDeployedCoord = a_coord;
    m_latestPieceDeployedPlayer = &a_player;

    if (a_piece.GetType() == e_noPiece)
    {
        // if latest piece is noPiece nothing will glow on the board. Stop unnecesary timers
        m_latestPieceDeployedEffectOn = false;
    }
    else if (m_latestPieceDeployedEffectOn == false)
    {
        m_latestPieceDeployedEffectOn = true;
        g_timeout_add(LAST_PIECE_EFFECT_MILLIS, timerCallback, static_cast<void*>(this));
    }

    return Invalidate();
}

void DrawingAreaBoard::CancelLatestPieceDeployedEffect()
{
    m_latestPieceDeployedTransparency = LAST_PIECE_EFFECT_INITIAL_ALPHA;
    m_latestPieceDeployedEffectOn = false;
}

gboolean DrawingAreaBoard::timerCallback(void* param)
{
    static Piece sLatestPieceDeployedProcessed(e_noPiece);
    static Coordinate sLatestPieceDeployedCoord(COORD_UNINITIALISED, COORD_UNINITIALISED);
    static bool alphaGrowing = true; // transparency grows or drops

    DrawingAreaBoard* pThis = static_cast<DrawingAreaBoard*> (param);

    if (pThis->m_latestPieceDeployedEffectOn == false)
    {
        // stop the timeout returning FALSE
        return FALSE;
    }

    // no need to check if the player is the same.
    // 2 pieces can't be deployed in the same coordinate of the board
    if ( (sLatestPieceDeployedProcessed.GetType()  !=
              pThis->m_latestPieceDeployed.GetType())  ||
         (sLatestPieceDeployedCoord.m_row !=
              pThis->m_latestPieceDeployedCoord.m_row) ||
         (sLatestPieceDeployedCoord.m_col !=
              pThis->m_latestPieceDeployedCoord.m_col) )
    {
        // piece changed
        // at the very beginning of the animation transparency is the minimum
        pThis->m_latestPieceDeployedTransparency = LAST_PIECE_EFFECT_INITIAL_ALPHA;

        sLatestPieceDeployedProcessed = pThis->m_latestPieceDeployed;
        sLatestPieceDeployedCoord     = pThis->m_latestPieceDeployedCoord;
        alphaGrowing = false;
    }


    // latest piece deployed didn't change. Just change the alpha transparency
    // before invalidating the board
    if ( (pThis->m_latestPieceDeployedTransparency < 1.0) &&
         (alphaGrowing == true) )
    {
        pThis->m_latestPieceDeployedTransparency += 0.1;

        if (pThis->m_latestPieceDeployedTransparency >= 1.0)
        {
            alphaGrowing = false;
        }
    }
    else
    {
        pThis->m_latestPieceDeployedTransparency -= 0.1;

        if (pThis->m_latestPieceDeployedTransparency <= LAST_PIECE_EFFECT_MIN_ALPHA)
        {
            alphaGrowing = true;
        }
    }

    pThis->Invalidate();

    return TRUE;
}
