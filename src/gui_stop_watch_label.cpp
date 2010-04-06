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
/// @file gui_stop_watch_label.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 5-Apr-2009  Original development
/// @endhistory
///
// ============================================================================

#include <iostream>
#include <iomanip> // setw
#include "gui_stop_watch_label.h"

StopWatchLabel::StopWatchLabel(uint32_t a_periodMillis, std::string a_prefix) :
    Gtk::Label(),
    m_timerRunning(false),
    m_periodMillis(a_periodMillis),
    m_prefix(a_prefix)
{
    m_timer = g_timer_new();
    g_timer_stop(m_timer);

    // set the widget size. that magic 9 is the number of characters used for the time:
    // mmm:ss.ll
    // which gives room for 999 minutes (16 hours). If a game takes more than 16 hours
    // it shouldn't matter as the the latest characters are the milliseconds
    set_width_chars(a_prefix.length() + 9);

    UpdateWidget(false);
}

StopWatchLabel::~StopWatchLabel()
{
    g_timer_destroy(m_timer);
}

void StopWatchLabel::Reset()
{
    m_timerRunning = false;
    g_timer_reset(m_timer);
    g_timer_stop(m_timer);

    UpdateWidget();
}

void StopWatchLabel::Continue()
{
    m_timerRunning = true;
    g_timer_continue(m_timer);

    g_timeout_add(m_periodMillis, timerCallback, static_cast<void*>(this));
}

void StopWatchLabel::Stop()
{
    m_timerRunning = false;
    g_timer_stop(m_timer);

    UpdateWidget(true);
}

void StopWatchLabel::UpdateWidget(bool a_stopped)
{
    gulong microsecs;
    uint32_t secs;
    uint32_t mins;
    secs = static_cast<uint32_t>(g_timer_elapsed(m_timer, &microsecs));

    mins = secs / 60;
    secs = secs % 60;

    std::stringstream theMessage;
    theMessage << m_prefix;

    if (m_timerRunning)
    {
        theMessage << "<b>";
    }

    theMessage << mins << ":";
    theMessage << std::setfill('0') << std::setw(2) << secs;

    if (a_stopped)
    {
        theMessage << ".";
        theMessage << std::setfill('0') << std::setw(2) << static_cast<uint32_t>(microsecs / 10000);
    }

    if (m_timerRunning)
    {
        theMessage << "</b>";
        set_markup(theMessage.str().c_str());
    }
    else
    {
        set_text(theMessage.str().c_str());
    }
}

gboolean StopWatchLabel::timerCallback(void* param)
{
    StopWatchLabel* pThis = static_cast<StopWatchLabel*> (param);

    if (pThis->m_timerRunning == false)
    {
        // stop the timeout returning FALSE
        return FALSE;
    }

    pThis->UpdateWidget();

    return TRUE;
}
