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
///           Faustino Frechilla 04-Oct-2010  Extended to support N players
///           Faustino Frechilla 06-Oct-2010  Added a horizontal separator
/// @endhistory
///
// ============================================================================

#ifndef GAME_STATUSBAR_H_
#define GAME_STATUSBAR_H_

#include <vector>
#include <gtkmm.h>

#include "player.h"
#include "gui_stop_watch_label.h"

/// @brief class to handle the status bar to be shown on the main window
class GameStatusBar :
    public Gtk::VBox
{
public:
    /// @param amount of players to be represented by this status bar (positive)
    /// @param true if the status bar should contain a progress bar
    GameStatusBar(uint32_t a_nPlayers, bool a_progressBarPresent);
    ~GameStatusBar();

    /// update score of a player
    /// @param index of the player. Valid range goes from 1 to the nPlayers
    ///        parameter used in the constructor
    /// @return true if it succesfully updated player's score
    ///         false otherwise
    bool SetScoreStatus(uint32_t playerIndex, const Player &a_player);

    /// set prefix to be shown before player's stopwatch
    /// @param index of the player. Valid range goes from 1 to the nPlayers
    ///        parameter used in the constructor
    /// @return true if it succesfully updated player's stopwatch
    ///         false otherwise
    bool SetStopwatchPrefix(uint32_t playerIndex, const Player &a_player);

    /// Reset all stopwatches contained in the status bar
    void ResetAllStopwatches();

    /// stop all stopwatches
    void StopAllStopwatches();

    /// swap stopwatches. Stop the stopwatch currently running and starts
    /// the next one. For example, in a status bar for 4 players, the following
    /// list represents the sequence of events:
    /// Running before: player1. Afterwards: player2
    /// Running before: player2. Afterwards: player3
    /// Running before: player3. Afterwards: player4
    /// Running before: player4. Afterwards: player1
    ///
    /// WARNINGS:
    ///   + If no stopwatches are running it does nothing
    ///   + If this status bar has only one player it does nothing either
    void SwapStopwatches();

    /// resume player's stopwatch count
    /// @param index of the player. Valid range goes from 1 to the nPlayers
    ///        parameter used in the constructor
    /// @return true if it succesfully issued the command to the stopwatch.
    ///         false otherwise
    bool ContinueStopwatch(uint32_t a_playerIndex);

    /// sets the fraction of the progress bar shown in the status bar
    /// It does nothing if the object has been instantiated without progress bar
    /// @param a floating point number between 0.0 to 1.0 (0% to 100%)
    void SetFraction(float a_fraction);

private:
    /// @brief number of players to be represented by this status bar
    uint32_t m_nPlayers;

    /// @brief horizontal separator to be shown on top of the status bar
    Gtk::HSeparator m_barSeparator;

    /// @brief horizontal box where all the elements to be displayed are added
    Gtk::HBox m_elementsBox;

    /// @brief vertical separators to be used in the status bar
    //TODO that number 3 is magic!!
    std::vector<Gtk::VSeparator*> m_arrayStatusBarSeparator;

    /// @brief labels to show the users score in the status bar
    std::vector<Gtk::Label*> m_arrayScoreLabel;

    /// @brief show time player's takes to think
    std::vector<StopWatchLabel*> m_arrayStopwatchLabel;

    /// @brief progress bar for when the computer is "thinking".
    ///        To be shown in the status bar
    Gtk::ProgressBar* m_progressBar;

    // prevent default constructors/operators from being accidentaly called
    GameStatusBar();
    GameStatusBar(const GameStatusBar &a_src);
    GameStatusBar& operator=(const GameStatusBar &a_src);

};

#endif // GAME_STATUSBAR_H_
