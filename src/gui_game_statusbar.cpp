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
///           Faustino Frechilla 04-Oct-2010  Extended to support N players
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


GameStatusBar::GameStatusBar(uint32_t a_nPlayers, bool a_progressBarPresent):
    Gtk::VBox(),
    m_nPlayers(a_nPlayers),
    m_arrayStatusBarSeparator(0),
    m_arrayScoreLabel(0),
    m_arrayStopwatchLabel(0),
    m_progressBar(NULL)
{    
    this->set_spacing(1);
    this->set_homogeneous(false);
    
    // add the status bar separator
    this->pack_start(m_barSeparator, true, true);
    
    // add now the horizontal box ehich contains the different widgets
    this->pack_start(m_elementsBox, true, true);
    
    // custom settings for the hbox
    m_elementsBox.set_spacing(10);
    m_elementsBox.set_homogeneous(false);
    
    if (a_nPlayers > 0)
    {
        m_arrayStatusBarSeparator.resize((m_nPlayers * 2) - 1, NULL);
        m_arrayScoreLabel.resize(m_nPlayers, NULL);
        m_arrayStopwatchLabel.resize(m_nPlayers, NULL);

        for (uint32_t i = 0; i < (m_nPlayers * 2) - 1; i++)
        {
            m_arrayStatusBarSeparator[i] = new Gtk::VSeparator;
        }

        uint32_t separatorIndex = 0;
        for (uint32_t playerIndex = 0; playerIndex < m_nPlayers; playerIndex++)
        {
            m_arrayScoreLabel[playerIndex] = new Gtk::Label;
            m_arrayStopwatchLabel[playerIndex] =
                new StopWatchLabel(STOPWATCH_UPDATE_PERIOD_MILLIS);

            if (playerIndex != 0)
            {
                m_elementsBox.pack_start(
                    *m_arrayStatusBarSeparator[separatorIndex++],
                    false,
                    true);
            }

            m_elementsBox.pack_start(*m_arrayScoreLabel[playerIndex], true, true);

            m_elementsBox.pack_start(
                *m_arrayStatusBarSeparator[separatorIndex++],
                false,
                true);

            m_elementsBox.pack_start(*m_arrayStopwatchLabel[playerIndex], true, true);
        }
    }

    // finally, add the progress bar if requested to do so
    if (a_progressBarPresent)
    {
        m_progressBar = new Gtk::ProgressBar;
        m_progressBar->set_orientation(Gtk::PROGRESS_LEFT_TO_RIGHT);
        m_progressBar->set_fraction(0.0);

        m_elementsBox.pack_start(*m_progressBar, true, true);
    }
}

GameStatusBar::~GameStatusBar()
{
    for (uint32_t i = 0; i < (m_nPlayers * 2) - 1; i++)
    {
        m_arrayStatusBarSeparator[i]->hide();

        delete m_arrayStatusBarSeparator[i];
        m_arrayStatusBarSeparator[i] = NULL;
    }

    for (uint32_t i = 0; i < m_nPlayers; i++)
    {
        m_arrayScoreLabel[i]->hide();
        m_arrayStopwatchLabel[i]->hide();

        delete m_arrayScoreLabel[i];
        delete m_arrayStopwatchLabel[i];

        m_arrayScoreLabel[i]     = NULL;
        m_arrayStopwatchLabel[i] = NULL;
    }

    if (m_progressBar != NULL)
    {
        delete m_progressBar;
    }
}

bool GameStatusBar::SetScoreStatus(
    uint32_t a_playerIndex,
    const Player &a_player)
{
#ifdef DEBUG
    assert (a_playerIndex >= 1);
    assert (a_playerIndex <= m_nPlayers);
#endif

    // this buffer will contain the string to be applied to the labels
    char theMessage[SCORE_LABEL_BUFFER_LENGTH];

    // calculate the number of squares left
    int32_t squaresLeftPlayer = 0;
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (a_player.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer += a_player.m_pieces[i].GetNSquares();
        }
    }

    uint8_t red, green, blue;
    a_player.GetColour(red, green, blue);

    snprintf (theMessage,
              SCORE_LABEL_BUFFER_LENGTH,
              // i18n TRANSLATORS: the first 3 %02X  will be replaced by the
              // i18n colour of the current player. The '%s' will be replaced
              // i18n by this player's name and the '%2d' his/her score
              // i18n For a better GUI experience all these strings should take
              // i18n the same amount of characters
              ngettext("<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       "<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       static_cast<int32_t>(squaresLeftPlayer)),
              red,
              green,
              blue,
              a_player.GetName().c_str(),
              squaresLeftPlayer);

    if ((a_playerIndex >= 1) && (a_playerIndex <= m_nPlayers))
    {
        m_arrayScoreLabel[a_playerIndex - 1]->set_markup(theMessage);
        return true;
    }

    return false;
}

bool GameStatusBar::SetStopwatchPrefix(
    uint32_t a_playerIndex,
    const Player &a_player)
{
#ifdef DEBUG
    assert (a_playerIndex >= 1);
    assert (a_playerIndex <= m_nPlayers);
#endif

    // retrieve the colour from player and use HTML tags so
    // the stop watch labels show player's name written with its
    // corresponding colour
    uint8_t red, green, blue;
    a_player.GetColour(red, green, blue);

    std::stringstream theMessage;
    theMessage << "<span color=\"#"
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(red)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(green)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(blue)
               << "\">"
               << a_player.GetName()
               << " </span>";

    if ((a_playerIndex >= 1) && (a_playerIndex <= m_nPlayers))
    {
        m_arrayStopwatchLabel[a_playerIndex - 1]->SetPrefix(theMessage.str());
        return true;
    }

    return false;
}

void GameStatusBar::ResetAllStopwatches()
{
    for (uint32_t i = 0; i < m_nPlayers; i++)
    {
        m_arrayStopwatchLabel[i]->Reset();
    }
}

void GameStatusBar::StopAllStopwatches()
{
    for (uint32_t i = 0; i < m_nPlayers; i++)
    {
        m_arrayStopwatchLabel[i]->Stop();
    }
}

void GameStatusBar::SwapStopwatches()
{
    if (m_nPlayers <= 1)
    {
        // do nothing.
        // There is only 1 stopwatch (or none) so it cannot be swapped
        return;
    }

    for (uint32_t i = 0; i < m_nPlayers; i++)
    {
        if (m_arrayStopwatchLabel[i]->IsRunning())
        {
            // stop it
            m_arrayStopwatchLabel[i]->Stop();

            // resume next stopwatch's count
            if ((i + 1) < m_nPlayers)
            {
                m_arrayStopwatchLabel[i + 1]->Continue();
            }
            else
            {
                m_arrayStopwatchLabel[0]->Continue();
            }

            return;
        }
    }
}

bool GameStatusBar::ContinueStopwatch(uint32_t a_playerIndex)
{
#ifdef DEBUG
    assert (a_playerIndex >= 1);
    assert (a_playerIndex <= m_nPlayers);
#endif

    if ((a_playerIndex >= 1) && (a_playerIndex <= m_nPlayers))
    {
        m_arrayStopwatchLabel[a_playerIndex - 1]->Continue();
        return true;
    }

    return false;
}

void GameStatusBar::SetFraction(float a_fraction)
{
    if (m_progressBar)
    {
        m_progressBar->set_fraction(a_fraction);
    }
}
