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
/// @file gui_game_statusbar.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 01-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef GAME_STATUSBAR_H_
#define GAME_STATUSBAR_H_

#include <gtkmm.h>

#include "player.h"
#include "gui_stop_watch_label.h"

/// @brief class to handle the status bar to be shown on the main window
class GameStatusBar :
    public Gtk::HBox
{
public:
    GameStatusBar();
    ~GameStatusBar();

    /// update score of only player1
    void SetScoreStatus(const Player &a_player1);

    /// update score of both player1 and player2
    void SetScoreStatus(const Player &a_player1, const Player &a_player2);

    /// set prefix to be shown before the stopwatches values of both
    /// player1 and player2
    void SetStopwatchPrefix(const Player &a_player1, const Player &a_player2);

    /// Reset all stopwatches contained in the status bar
    void ResetAllStopwatches();

    /// stop all stopwatches
    void StopAllStopwatches();

    /// resume player1's stopwatch count
    void StopwatchPlayer1Continue();

    /// swap stopwatches. Stop the stopwatch currently running and starts
    /// the next one
    /// If no stopwatches are running it does nothing
    void SwapStopwatches();

    /// @param player2 information will be shown if it's true. This data will
    ///        be hidden if it set to false
    /// Player2 is enabled by default
    void EnablePlayer2(bool a_action);

    /// sets the fraction of the progress bar shown in the status bar
    /// @param a floating point number between 0.0 to 1.0 (0% to 100%)
    void SetFraction(float a_fraction);

private:
    /// @brief vertical separators to be used in the status bar
    //TODO that number 3 is magic!!
    Gtk::VSeparator m_arrayStatusBarSeparator[3];

    /// @brief progress bar for when the computer is "thinking".
    ///        To be shown in the status bar
    Gtk::ProgressBar m_progressBar;

    /// @brief label to show the user score in the status bar
    Gtk::Label m_player1ScoreLabel;

    /// @brief label to show the computer score in the status bar
    Gtk::Label m_player2ScoreLabel;

    /// @brief show the time the player1 takes to think
    StopWatchLabel m_stopWatchLabelPlayer1;

    /// @brief show the time the player2 takes to think
    StopWatchLabel m_stopWatchLabelPlayer2;
};

#endif // GAME_STATUSBAR_H_
