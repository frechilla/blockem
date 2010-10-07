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
/// @file gui_game_total_allocation_widget.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 06-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#include <cstdio>    // printf/snprintf (needed for better i18n)
#include <iomanip>   // setw
#endif

#include "gettext.h" // i18n
#include "gui_game_total_allocation_widget.h"
#include "gui_glade_defs.h"
#include "rules.h"

/// maximum size of the string to notify the end of the game
static const uint32_t GAME_FINISHED_BUFFER_LENGTH = 256;

static const uint8_t DEFAULT_PLAYER_COLOUR_R  = 31;
static const uint8_t DEFAULT_PLAYER_COLOUR_G  = 130;
static const uint8_t DEFAULT_PLAYER_COLOUR_B  = 31;

static const int32_t BOARD_NROWS = 14;
static const int32_t BOARD_NCOLS = 14;


GameTotalAllocationWidget::GameTotalAllocationWidget():
    Gtk::VBox(), //Calls the base class constructor
    m_currentGameFinished(false),
    m_theTotalAllocationGame(BOARD_NROWS, BOARD_NCOLS, Coordinate()),
    m_pickPiecesDrawingArea(
        m_theTotalAllocationGame.GetPlayer(),
        DrawingAreaShowPieces::eOrientation_leftToRight),
    m_boardDrawingArea(m_theTotalAllocationGame.GetBoard()),
    m_editPieceTable(),
    m_statusBar(1, false) // 1 player. Without progress bar
{
    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();

    // TODO setting default colours to the player
    // this should be done loading from some config class
    m_theTotalAllocationGame.SetPlayerColour(
        DEFAULT_PLAYER_COLOUR_R,
        DEFAULT_PLAYER_COLOUR_G,
        DEFAULT_PLAYER_COLOUR_B);

    // configure hbox edit pieces
    m_hBoxEditPieces.set_spacing(10);
    m_hBoxEditPieces.set_size_request(-1, 200);
    m_hBoxEditPieces.pack_start(m_pickPiecesDrawingArea, true, true);
    m_hBoxEditPieces.pack_start(m_editPieceTable, false, false);

    // add widgets to this table
    // pack_start (Widget& child, bool expand, bool fill, guint padding=0)
    this->pack_start(m_boardDrawingArea, true, true);
    this->pack_start(m_hBoxEditPieces, false, true);
    this->pack_start(m_statusBar, false, true);

    // update the score shown in the status bar before setting them up as visible
    // so the widgets take their proper size at GUI startup
    m_statusBar.SetScoreStatus(1, m_theTotalAllocationGame.GetPlayer());

    // stopwatch must be initialised also
    m_statusBar.SetStopwatchPrefix(1, m_theTotalAllocationGame.GetPlayer());

    // connect the signal coming from the pickPiecesDrawingArea to update TableEditPiece
    m_pickPiecesDrawingArea.signal_piecePicked().connect(
            sigc::mem_fun(m_editPieceTable, &TableEditPiece::SetPiece));

    // connect the signal coming from the editing piece table to process the change in the
    // currently editing piece
    m_editPieceTable.signal_pieceChanged().connect(
            sigc::mem_fun(m_boardDrawingArea, &DrawingAreaBoard::SetCurrentPiece));

    // connect the signal coming fromt he board drawing area to process when the user clicks
    // on the board
    m_boardDrawingArea.signal_boardPicked().connect(
            sigc::mem_fun(*this, &GameTotalAllocationWidget::BoardDrawingArea_BoardClicked));

    // initialise the list of players of the board drawing area
    m_boardDrawingArea.AddPlayerToList(m_theTotalAllocationGame.GetPlayer());

    // set piece colour to player's in the edit piece area
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
    m_theTotalAllocationGame.GetPlayer().GetColour(red, green, blue);
    m_editPieceTable.SetPieceRGB(
        static_cast<float>(red)   / 255,
        static_cast<float>(green) / 255,
        static_cast<float>(blue)  / 255);

    // human beings are allowed to edit pieces
    m_editPieceTable.set_sensitive(true);

    // player is a human and he/she will put down a piece
    m_boardDrawingArea.SetCurrentPlayer(m_theTotalAllocationGame.GetPlayer());

    // launch the game!!
    LaunchNewGame();
}

GameTotalAllocationWidget::~GameTotalAllocationWidget()
{
}

DrawingAreaBoard& GameTotalAllocationWidget::BoardDrawingArea()
{
    return m_boardDrawingArea;
}

void GameTotalAllocationWidget::ShowInfluenceAreaInBoard(bool a_show)
{
    if (a_show)
    {
        m_boardDrawingArea.ShowPlayerInfluenceArea(
            m_theTotalAllocationGame.GetPlayer());
    }
    else
    {
        m_boardDrawingArea.HidePlayerInfluenceArea();
    }
}

void GameTotalAllocationWidget::ShowForbiddenAreaInBoard(bool a_show)
{
    if (a_show)
    {
        m_boardDrawingArea.ShowPlayerForbiddenArea(
            m_theTotalAllocationGame.GetPlayer());
    }
    else
    {
        m_boardDrawingArea.HidePlayerForbiddenArea();
    }
}

void GameTotalAllocationWidget::LaunchNewGame()
{
    //TODO size of the board should be stored in some configuration class
    // reset the current game
    // uninitialised starting coord. Start from everywhere
    m_theTotalAllocationGame.Reset(BOARD_NROWS, BOARD_NCOLS, Coordinate());

    // update the board view
    m_boardDrawingArea.CancelLatestPieceDeployedEffect();
    m_boardDrawingArea.Invalidate();
    // player1 will be picking next piece
    m_pickPiecesDrawingArea.Invalidate(m_theTotalAllocationGame.GetPlayer());

    // update score
    m_statusBar.SetScoreStatus(1, m_theTotalAllocationGame.GetPlayer());

    // stopwatches must be restarted.
    m_statusBar.ResetAllStopwatches();

    // Start player's timer
    m_statusBar.ContinueStopwatch(1);

    // reset and force redraw editPieceTable. It'll be set to sensitive
    // or unsensitive depending on the type of player1
    m_editPieceTable.SetPiece(e_noPiece);

    // new game just started. It can't be finished!
    m_currentGameFinished = false;
}

void GameTotalAllocationWidget::BoardDrawingArea_BoardClicked(
    const Coordinate &a_coord,
    const Piece &a_piece,
    const Player &a_player)
{
    if (a_player.NumberOfPiecesAvailable() == e_numberOfPieces)
    {
        if ( ( (!a_player.GetStartingCoordinate().Initialised()) &&
               (!rules::IsPieceDeployableInStartingPoint(
                    m_theTotalAllocationGame.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_coord)) ) ||
             ( (a_player.GetStartingCoordinate().Initialised()) &&
               (!rules::IsPieceDeployableInStartingPoint(
                    m_theTotalAllocationGame.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_player.GetStartingCoordinate())) ) )
        {
#ifdef DEBUG_PRINT
            std::cout << "Starting coordinate deployment failure: Cheeky you! Don't try to deploy a piece where it's not allowed to"
                      << std::endl;
#endif
            return;
        }
    }
    else
    {
        if (!rules::IsPieceDeployableCompute(
                    m_theTotalAllocationGame.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_player))
        {
#ifdef DEBUG_PRINT
            std::cout << "Deployment failure: Cheeky you! Don't try to deploy a piece where it's not allowed to"
                      << std::endl;
#endif
            return;
        }
    }

    // put down current piece before anything else
    m_theTotalAllocationGame.PutDownPiece(a_piece, a_coord);

    // invalidate the board drawing area to show the new moves
    // activating the latest piece deployed glowing effect
    m_boardDrawingArea.Invalidate(a_piece, a_coord, a_player);

	// update score
    m_statusBar.SetScoreStatus(1, a_player);

    // remove the actual piece being edited from the edit piece drawing area
    // and force the edit piece drawing area to be redraw
    m_editPieceTable.SetPiece(e_noPiece);

    // update the list of available pieces too
    m_pickPiecesDrawingArea.Invalidate();

    // there's a few things that have to be done if this player can't put
    // down any more pieces
    if (rules::CanPlayerGo(m_theTotalAllocationGame.GetBoard(), a_player) == false)
    {
        GameFinished();
    }
}

void GameTotalAllocationWidget::GameFinished()
{
    // once this function is called, game is supposed to be finished
    if (m_currentGameFinished == true)
    {
        // game was already notified to be finished
        return;
    }
    m_currentGameFinished = true;

    // stop stopwatches (even if it was already done)
    m_statusBar.StopAllStopwatches();

    // no player is expected to put down any piece on the board now
    m_boardDrawingArea.UnsetCurrentPlayer();

    // allow a possible human user to play with the edit pieces area
    m_editPieceTable.set_sensitive(true);

    int32_t squaresLeft = 0;
    const Player &player = m_theTotalAllocationGame.GetPlayer();
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (player.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeft += player.m_pieces[i].GetNSquares();
        }
    }

    char theMessage[GAME_FINISHED_BUFFER_LENGTH];
    if (squaresLeft == 0)
    {
        snprintf(theMessage,
            GAME_FINISHED_BUFFER_LENGTH,
            _("Done! You managed to allocate all your pieces"));

    }
    else //(squaresLeft != 0)
    {
        snprintf(theMessage,
                GAME_FINISHED_BUFFER_LENGTH,
                // i18n TRANSLATORS: Please, respect the HTML formatting as much as possible
                // i18n Note translations depend also on whether there is only 1 or more than
                // i18n one squares left
                // i18n Thank you for contributing to this project
                ngettext("You can't put down more pieces, but you still have the baby piece left",
                         "You can't put down more pieces, but you still have <b>%d</b> squares left",
                         squaresLeft),
                squaresLeft);
    }

    // notify this game is finished sending the final score message
    signal_gameFinished().emit(std::string(theMessage));
}

#ifdef WIN32
void GameTotalAllocationWidget::ForceTranslationOfWidgets()
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

}
#else
void GameTotalAllocationWidget::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
