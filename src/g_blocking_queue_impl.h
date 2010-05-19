// ============================================================================
/// @file
/// @brief This file contains the implementation of the Blocking queue class.
///
/// @copyright
/// @history
/// Ref        Who                When        What
///            Faustino Frechilla 04-May-2009 Original development
/// @endhistory
///
// ============================================================================

#ifndef BLOCKINGQUEUEIMPL_H_
#define BLOCKINGQUEUEIMPL_H_

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

    gboolean retcode;
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

#endif /* BLOCKINGQUEUEIMPL_H_ */
