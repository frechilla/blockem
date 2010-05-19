// ============================================================================
/// @file g_blocking_queue.h
/// @brief This file contains the Blocking queue class.
///
/// @copyright
/// @history
/// Ref        Who                When        What
///            Faustino Frechilla 04-May-2009 Original development
///            Faustino Frechilla 19-May-2010 Adapted to use glib mutexes and conds
/// @endhistory
///
// ============================================================================

#ifndef _GBLOCKINGQUEUE_H_
#define _GBLOCKINGQUEUE_H_

#include <glib.h>
#include <queue>

#include <limits> // std::numeric_limits<>::max

#define BLOCKING_QUEUE_NO_LIMIT         std::numeric_limits<std::size_t >::max()
#define BLOCKING_QUEUE_DEFAULT_MAX_SIZE BLOCKING_QUEUE_NO_LIMIT

/// @brief blocking thread-safe queue
/// It uses a mutex+condition variables to protect the internal queue implementation.
/// Inserting or reading elements use the same mutex
template <typename T>
class BlockingQueue
{
public:
    BlockingQueue(std::size_t a_maxSize = BLOCKING_QUEUE_DEFAULT_MAX_SIZE);
    ~BlockingQueue();

    /// @brief Check if the queue is empty
    /// This call can block if another thread owns the lock that protects the queue
    /// @return true if the queue is empty. False otherwise
    bool IsEmpty();

    /// @brief inserts an element into queue queue
    /// This call can block if another thread owns the lock that protects the queue. If the queue is full
    /// The thread will be blocked in this queue until someone else gets an element from the queue
    /// @param element to insert into the queue
    /// @return true if the elem was successfully inserted into the queue. False otherwise
    bool Push(const T &a_elem);

    /// @brief inserts an element into queue queue
    /// This call can block if another thread owns the lock that protects the queue. If the queue is full
    /// The call will return false and the element won't be inserted
    /// @param element to insert into the queue
    /// @return true if the elem was successfully inserted into the queue. False otherwise
    bool TryPush(const T &a_elem);

    /// @brief extracts an element from the queue (and deletes it from the queue)
    /// If the queue is empty this call will block the thread until there is something in the queue to be extracted
    /// @param a reference where the element from the queue will be saved to
    void Pop(T &out_data);

    /// @brief extracts an element from the queue (and deletes it from the queue)
    /// This call gets the block that protects the queue. It will extract the element from the queue
    /// only if there are elements in it
    /// @param reference to the variable where the result will be saved
    /// @return bool if the element was retrieved from the queue. False if the qeue was empty
    bool TryPop(T &out_data);

    /// @brief extracts an element from the queue (and deletes it from the queue)
    /// If the queue is empty this call will block the thread until there is something in the queue to be extracted
    /// or until the timer (2nd parameter) expires
    /// @param reference to the variable where the result will be saved
    /// @param microsecondsto wait before returning if the queue was empty
    /// @return bool if the element was retrieved from the queue. False if the timeout was reached
    bool TimedWaitPop(T &data, int32_t microsecs);

protected:
    std::queue<T> m_theQueue;
    /// maximum number of elements for the queue
    std::size_t m_maximumSize;
    /// Mutex to protect the queue
    GMutex* m_mutex;
    /// Conditional variable to wake up threads
    GCond*  m_cond;
};

// include the implementation
#include "g_blocking_queue_impl.h"

#endif /* _GBLOCKINGQUEUE_H_ */
