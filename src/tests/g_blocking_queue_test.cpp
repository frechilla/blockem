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
/// @file g_blocking_queue_test.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref Who                When        What
///     Faustino Frechilla 15-Jul-2010 Original development
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif
#include "g_blocking_queue_test.h"

#define TEST_BLOCKING_Q_SIZE 10
#define TEST_BLOCKING_Q_ITERATIONS 10

BlockingQueueTest::BlockingQueueTest() :
    m_theQueue(TEST_BLOCKING_Q_SIZE),
    m_thread(NULL)
{
}

BlockingQueueTest::~BlockingQueueTest()
{
}

void BlockingQueueTest::DoTest()
{
    Coordinate tmpCoord;

    // assert queue is empty at the very beginning
    assert(m_theQueue.IsEmpty());

    // no elements can be popped
    assert(m_theQueue.TryPop(tmpCoord) == false);

    // insert an element
    assert (m_theQueue.TryPush(Coordinate(0, 0)) == true);

    // inly one element can be extracted off the queue
    assert(m_theQueue.TryPop(tmpCoord) ==true);
    assert (tmpCoord.m_row == 0 && tmpCoord.m_col == 0);
    assert(m_theQueue.TryPop(tmpCoord) == false);

    // assert queue is empty again
    assert(m_theQueue.IsEmpty());

    // spawn the extra thread and
    SpawnThread();

    // the extra thread will block in a Pop call. This thread will block for a sec in the
    // TimedWaitPop too, we "ensure" the extra thread is blocked in the pop call when the main thread
    // is back from the timewaitpop (system must be unresponsive for a looong time if it can't
    // get the 2 threads waiting for a full second)
    assert(m_theQueue.TimedWaitPop(tmpCoord, 1000000) == false);

    for (int i = 0; i < TEST_BLOCKING_Q_SIZE * TEST_BLOCKING_Q_ITERATIONS; i++)
    {
        // block in this call until the main thread pushes something into the queue
        m_theQueue.Push(Coordinate(i, i / 2));
    }

    // extra thread must finish
    g_thread_join(m_thread);

    // fill up the q
    for (int i = 0; i < TEST_BLOCKING_Q_SIZE; i++)
    {
        // block in this call until the main thread pushes something into the queue
        assert(m_theQueue.TryPush(Coordinate(i, i / 2)) == true);
    }
    // q is full
    assert(m_theQueue.TryPush(Coordinate(0, 0)) == false);

    // fill up the q
    for (int i = 0; i < TEST_BLOCKING_Q_SIZE; i++)
    {
        // block in this call until the main thread pushes something into the queue
        assert(m_theQueue.TryPop(tmpCoord) == true);

        assert( (tmpCoord.m_row == i) && (tmpCoord.m_col == (i / 2)) );
    }
    // q is empty
    assert(m_theQueue.IsEmpty());
    assert(m_theQueue.TryPop(tmpCoord) == false);

    // test is DONE!!!
}

void BlockingQueueTest::SpawnThread()
{
    // configure the option JOINABLE to the thread creation
    bool joinable = true;
    GError* err = NULL;

    // GThread * g_thread_create ( GThreadFunc func,
    // gpointer data,
    // gboolean joinable,
    // GError **error);
    m_thread = g_thread_create(
        reinterpret_cast<GThreadFunc>(BlockingQueueTest::ThreadRoutine),
        reinterpret_cast<void*>(this),
        joinable,
        &err);

    if (m_thread == NULL)
    {
#ifdef DEBUG_PRINT
        std::cerr << std::string("Testing Thread creation failed. ")
                  << std::string(err->message)
                  << std::endl;
#endif
        g_error_free(err);
        assert(0);
    }
}

void BlockingQueueTest::ThreadRoutine(void *a_ThreadParam)
{
    BlockingQueueTest* pThis = static_cast<BlockingQueueTest*>(a_ThreadParam);
    Coordinate tmpCoord;

    int i;
    for (i = 0; i < TEST_BLOCKING_Q_SIZE * (TEST_BLOCKING_Q_ITERATIONS - 1); i++)
    {
        // block in this call until the main thread pushes something into the queue
        pThis->m_theQueue.Pop(tmpCoord);

        // main thread inserted something
        assert ( (tmpCoord.m_row == i) && (tmpCoord.m_col == (i / 2)) );
    }

    // retrieve the last items using
    for (; i < TEST_BLOCKING_Q_SIZE * TEST_BLOCKING_Q_ITERATIONS; i++)
    {
        // block in this call until the main thread pushes something into the queue
        // or until 10 millis are gone with no elements in the q
        while (pThis->m_theQueue.TimedWaitPop(tmpCoord, 10) == false)
        {
            ;
        }

        // main thread inserted something
        assert ( (tmpCoord.m_row == i) && (tmpCoord.m_col == (i / 2)) );
    }
}
