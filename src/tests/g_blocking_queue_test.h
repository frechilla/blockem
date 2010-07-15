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
/// @file g_blocking_queue_test.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref Who                When        What
///     Faustino Frechilla 15-Jul-2010 Original development
/// @endhistory
///
// ============================================================================

#ifndef G_BLOCKING_QUEUE_TEST_H
#define G_BLOCKING_QUEUE_TEST_H

#include <glib.h> // threads
#include "g_blocking_queue.h"
#include "coordinate.h"

/// @brief regression testing for the BlockingQueue class
class BlockingQueueTest
{
public:
    BlockingQueueTest();
    virtual ~BlockingQueueTest();

    void DoTest();

private:
    BlockingQueue<Coordinate> m_theQueue;

    // an extra thread is needed to test blocking mechanisms
    GThread* m_thread;

    /// spawns testing thread
    void SpawnThread();

    /// routine for the testing thread
    static void ThreadRoutine(void *a_ThreadParam);
};

#endif // G_BLOCKING_QUEUE_TEST_H
