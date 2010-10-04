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
/// @file gui_stop_watch_label.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 5-Apr-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef _GUI_STOP_WATCH_LABEL_H_
#define _GUI_STOP_WATCH_LABEL_H_

#include <gtkmm.h>
#include <glib.h>
#include <stdint.h> // for types

// warning: this stopwatchlabel is not thread safe. it must be used in the context of
// only the same thread that creates it
class StopWatchLabel :
    public Gtk::Label
{
public:
    /// @brief constructor of the class
    /// the label widget will be updated every a_periodMillis milliseconds
    /// a_prefix will be added in front of the elapsed time when the label is updated
    StopWatchLabel(
        uint32_t a_periodMillis, 
        const std::string &a_prefix = std::string());
    ~StopWatchLabel();

    /// @brief resets the timer to when the world was created (0)
    ///        it stops the timer in case it was on
    void Reset();

    /// @brief starts the timer. It resumes it if it was stopped
    void Continue();

    /// @brief pauses the timer
    void Stop();

    /// @brief return true if the stopwatch is running
    bool IsRunning() const;

    /// @brief sets the prefix to be shown before the timestamp in the label
    /// the widget will be updated automatically
    void SetPrefix(const std::string &a_prefix);

private:
    /// @brief contains the time elapsed so far
    GTimer* m_timer;

    /// @brief flag which describes if the timer is running or is stopped
    bool m_timerRunning;

    /// @brief period of time which will take to update the widget
    uint32_t m_periodMillis;

    /// @brief it'll be added in front of the time value when the widget is updated
    std::string m_prefix;

    /// @brief updates the label to show the new time
    /// @param flag to indicate if the widget has to be updated because the timer was stopped
    void UpdateWidget(bool a_stopped = false);

    /// @brief callback to be called by the g timer
    static gboolean timerCallback(void* param);

    // prevent default constructors from being used
    StopWatchLabel();
    StopWatchLabel(const StopWatchLabel &a_src);
    StopWatchLabel& operator=(const StopWatchLabel &a_src);
};
#endif // _GUI_STOP_WATCH_LABEL_H_
