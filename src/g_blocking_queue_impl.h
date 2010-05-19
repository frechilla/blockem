// ============================================================================
// Copyright 2009 Faustino Frechilla
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
/// @file
/// @brief This file contains the implementation of the Blocking queue class.
///
/// @history
/// Ref        Who                When        What
///            Faustino Frechilla 04-May-2009 Original development
///            Faustino Frechilla 19-May-2010 Adapted to blockem using glib mutexes
/// @endhistory
///
// ============================================================================

#ifndef _GBLOCKINGQUEUEIMPL_H_
#define _GBLOCKINGQUEUEIMPL_H_

#include <assert.h>
#include <errno.h>
#include <time.h>
#include <stdint.h> // for uint64_t

#define NANOSECONDS_PER_SECOND 1000000000

template <typename T>
BlockingQueue<T>::BlockingQueue(std::size_t a_maxSize) :
    m_maximumSize(a_maxSize)
{
    m_mutex = g_mutex_new();
#ifdef DEBUG
    if (m_mutex == NULL)
    {
        assert(0);
    }
#endif

    m_cond = g_cond_new();
#ifdef DEBUG
    if (m_cond == NULL)
    {
        assert(0);
    }
#endif
}

template <typename T>
BlockingQueue<T>::~BlockingQueue()
{
    g_cond_free(m_cond);
    g_mutex_free(m_mutex);
}

template <typename T>
bool BlockingQueue<T>::IsEmpty()
{
    bool rv;

    g_mutex_lock(m_mutex);
    rv = m_theQueue.empty();
    g_mutex_unlock(m_mutex);

    return rv;
}

template <typename T>
bool BlockingQueue<T>::Push(const T &a_elem)
{
    g_mutex_lock(m_mutex);

    while (m_theQueue.size() >= m_maximumSize)
    {
        g_cond_wait(m_cond, m_mutex);
    }

    bool queueEmpty = m_theQueue.empty();

    m_theQueue.push(a_elem);

    if (queueEmpty)
    {
        // wake up threads waiting for stuff
        g_cond_broadcast(m_cond);
    }

    g_mutex_unlock(m_mutex);

    return true;
}

template <typename T>
bool BlockingQueue<T>::TryPush(const T &a_elem)
{
    g_mutex_lock(m_mutex);

    bool rv = false;
    bool queueEmpty = m_theQueue.empty();

    if (m_theQueue.size() < m_maximumSize)
    {
        m_theQueue.push(a_elem);
        rv = true;
    }

    if (queueEmpty)
    {
        // wake up threads waiting for stuff
        g_cond_broadcast(m_cond);
    }

    g_mutex_unlock(m_mutex);

    return rv;
}

template <typename T>
void BlockingQueue<T>::Pop(T &out_data)
{
    g_mutex_lock(m_mutex);

    while (m_theQueue.empty())
    {
        g_cond_wait(m_cond, m_mutex);
    }

    bool queueFull = (m_theQueue.size() >= m_maximumSize) ? true : false;

    out_data = m_theQueue.front();
    m_theQueue.pop();

    if (queueFull)
    {
        // wake up threads waiting for stuff
        g_cond_broadcast(m_cond);
    }

    g_mutex_unlock(m_mutex);
}

template <typename T>
bool BlockingQueue<T>::TryPop(T &out_data)
{
    g_mutex_lock(m_mutex);

    bool rv = false;
    if (!m_theQueue.empty())
    {
        bool queueFull = (m_theQueue.size() >= m_maximumSize) ? true : false;

        out_data = m_theQueue.front();
        m_theQueue.pop();

        if (queueFull)
        {
            // wake up threads waiting for stuff
            g_cond_broadcast(m_cond);
        }

        rv = true;
    }

    g_mutex_unlock(m_mutex);

    return rv;
}

template <typename T>
bool BlockingQueue<T>::TimedWaitPop(T &data, int32_t microsecs)
{
    g_mutex_lock(m_mutex);

    // adding microsecs to now
    GTimeVal abs_time;
    g_get_current_time(&abs_time);
    g_time_val_add(&abs_time, microsecs);

    gboolean retcode = TRUE;
    while (m_theQueue.empty() && (retcode != FALSE))
    {
        // Returns TRUE if cond was signalled, or FALSE on timeout
        retcode = g_cond_timed_wait(m_cond, m_mutex, &abs_time);
    }

    bool rv = false;
    bool queueFull = (m_theQueue.size() >= m_maximumSize) ? true : false;
    if (retcode != FALSE)
    {
        data = m_theQueue.front();
        m_theQueue.pop();

        rv = true;
    }

    if (rv && queueFull)
    {
        // wake up threads waiting for stuff
        g_cond_broadcast(m_cond);
    }

    g_mutex_unlock(m_mutex);

    return rv;
}

#endif /* _GBLOCKINGQUEUEIMPL_H_ */
