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
/// @file gui_game_statusbar.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 01-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#include <stdint.h>  // uint32_t
#include <iomanip>   // setw
#include "gettext.h" // i18n
#include "gui_game_statusbar.h"

/// maximum size of the string to apply to score labels
static const uint32_t SCORE_LABEL_BUFFER_LENGTH = 64;

/// how often stopwatches are updated
static const uint32_t STOPWATCH_UPDATE_PERIOD_MILLIS = 500; // 1000 = 1 second


GameStatusBar::GameStatusBar():
    m_stopWatchLabelPlayer1(
        STOPWATCH_UPDATE_PERIOD_MILLIS),
    m_stopWatchLabelPlayer2(
        STOPWATCH_UPDATE_PERIOD_MILLIS)
{
    // custom settings for the hbox
    this->set_spacing(10);
    this->set_homogeneous(false);

    // build up the status bar
    this->pack_start(m_player1ScoreLabel, true, true);
    this->pack_start(m_arrayStatusBarSeparator[0], false, true);
    this->pack_start(m_stopWatchLabelPlayer1, true, true);
    this->pack_start(m_arrayStatusBarSeparator[1], false, true);
    this->pack_start(m_player2ScoreLabel, true, true);
    this->pack_start(m_arrayStatusBarSeparator[2], false, true);
    this->pack_start(m_stopWatchLabelPlayer2, true, true);
    m_progressBar.set_orientation(Gtk::PROGRESS_LEFT_TO_RIGHT);
    m_progressBar.set_fraction(0.0);
    this->pack_start(m_progressBar, true, true);
}

GameStatusBar::~GameStatusBar()
{
}

void GameStatusBar::SetScoreStatus(
    const Player &a_player1)
{
    // this buffer will contain the string to be applied to the labels
    char theMessage[SCORE_LABEL_BUFFER_LENGTH];

    // calculate the number of squares left
    int32_t squaresLeftPlayer1 = 0;
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (a_player1.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer1 += a_player1.m_pieces[i].GetNSquares();
        }
    }

    uint8_t red, green, blue;
    a_player1.GetColour(red, green, blue);

    snprintf (theMessage,
              SCORE_LABEL_BUFFER_LENGTH,
              // i18n TRANSLATORS: the first 3 %02X  will be replaced by the
              // i18n colour of the current player. The '%s' will be replaced
              // i18n by this player's name and the '%2d' his/her score
              // i18n For a better GUI experience all these strings should take
              // i18n the same amount of characters
              ngettext("<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       "<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       static_cast<int32_t>(squaresLeftPlayer1)),
              red,
              green,
              blue,
              a_player1.GetName().c_str(),
              squaresLeftPlayer1);
    m_player1ScoreLabel.set_markup(theMessage);
}

void GameStatusBar::SetScoreStatus(
    const Player &a_player1,
    const Player &a_player2)
{
    // this buffer will contain the string to be applied to the labels
    char theMessage[SCORE_LABEL_BUFFER_LENGTH];

    // set score status of player1
    SetScoreStatus(a_player1);

    // calculate the number of squares left of player2
    int32_t squaresLeftPlayer2 = 0;
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (a_player2.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer2 += a_player2.m_pieces[i].GetNSquares();
        }
    }

    uint8_t red, green, blue;
    a_player2.GetColour(red, green, blue);

    // update player2's GUI widget
    snprintf (theMessage,
              SCORE_LABEL_BUFFER_LENGTH,
              // i18n TRANSLATORS: the first 3 %02X  will be replaced by the
              // i18n colour of the current player. The '%s' will be replaced
              // i18n by this player's name and the '%2d' his/her score
              // i18n For a better GUI experience both strings should take
              // i18n the same amount of characters
              ngettext("<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       "<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       static_cast<int32_t>(squaresLeftPlayer2)),
              red,
              green,
              blue,
              a_player2.GetName().c_str(),
              squaresLeftPlayer2);
    m_player2ScoreLabel.set_markup(theMessage);
}

void GameStatusBar::SetStopwatchPrefix(
    const Player &a_player1,
    const Player &a_player2)
{
    // retrieve the default colour from the  players and use HTML tags so
    // the stop watch labels show each player's name written with its
    // corresponding colour
    uint8_t red, green, blue;
    a_player1.GetColour(red, green, blue);

    std::stringstream theMessage;
    theMessage << "<span color=\"#"
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(red)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(green)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(blue)
               << "\">"
               << a_player1.GetName()
               << " </span>";
    m_stopWatchLabelPlayer1.SetPrefix(theMessage.str());

    a_player2.GetColour(red, green, blue);

    theMessage.clear();
    theMessage.str("");
    theMessage << "<span color=\"#"
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(red)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(green)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(blue)
               << "\">"
               << a_player2.GetName()
               << " </span>";
    m_stopWatchLabelPlayer2.SetPrefix(theMessage.str());
}

void GameStatusBar::ResetAllStopwatches()
{
    m_stopWatchLabelPlayer1.Reset();
    m_stopWatchLabelPlayer2.Reset();
}

void GameStatusBar::StopAllStopwatches()
{
    m_stopWatchLabelPlayer1.Stop();
    m_stopWatchLabelPlayer2.Stop();
}

void GameStatusBar::SwapStopwatches()
{
    if (m_stopWatchLabelPlayer1.IsRunning())
    {
        m_stopWatchLabelPlayer1.Stop();
        m_stopWatchLabelPlayer2.Continue();
    }
    else if (m_stopWatchLabelPlayer2.IsRunning())
    {
        m_stopWatchLabelPlayer2.Stop();
        m_stopWatchLabelPlayer1.Continue();
    }
}

void GameStatusBar::StopwatchPlayer1Continue()
{
    m_stopWatchLabelPlayer1.Continue();
}

void GameStatusBar::SetFraction(float a_fraction)
{
    m_progressBar.set_fraction(a_fraction);
}

void GameStatusBar::EnablePlayer2(bool a_action)
{
    if (a_action == true)
    {
        m_arrayStatusBarSeparator[1].show();
        m_player2ScoreLabel.show();
        m_arrayStatusBarSeparator[2].show();
        m_stopWatchLabelPlayer2.show();

        m_stopWatchLabelPlayer2.Continue();
    }
    else
    {
        m_arrayStatusBarSeparator[1].hide();
        m_player2ScoreLabel.hide();
        m_arrayStatusBarSeparator[2].hide();
        m_stopWatchLabelPlayer2.hide();

        m_stopWatchLabelPlayer2.Stop();
    }
}
