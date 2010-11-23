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
/// @file main_window_worker_thread.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 24 Nov 2009  Original development
///           Faustino Frechilla 15-Mar-2010  pthreads moved to gthreads
///           Faustino Frechilla 30-Apr-2010  CancelComputing logic added
///           Faustino Frechilla 02-May-2010  blockin call to ComputeMove
///           Faustino Frechilla 23-Jul-2010  i18n
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#include "gettext.h" // i18n
#include "gui/main_window_worker_thread.h"

// flags to set the sig_atomic_t variable to stop the Minimax computing
// every non-zero value will force the minimax algorithm to return
//
// This is a state machine that shows the values that the sig_atomic_t m_threadStatus
// can hold. Values held by m_threadStatus are shown inside the boxes, transitions
// indicates which method is called to change the state:
//
//                        Join +---------+ Join
//                 +----------*|   DIE   |*---------+
//                 |           +---------+          |
//                 |                * Join          |
//            +---------+           |           +--------+
//            | WAITING |*----------+-----------|  STOP  |
//            +---------+ Thread    |           +--------+
//      Thread *   |      Routine   |               * Cancel
//     Routine |   |           +---------+          |
//             |   +----------*| ACTIVE  |----------+
//             |   ComputeMove +---------+
//             |                    |
//             +--------------------+
//
// m_threadStatus is set to PROCESSING_DIE when the thread is told to kill itself
#define PROCESSING_DIE     3
// m_threadStatus is set to PROCESSING_STOP when the thread is told to cancel
// current computing process and get back to wait for more jobs
#define PROCESSING_STOP    2
// worker thread waiting for new jobs. m_threadStatus should be set to PROCESSING_ACTIVE
#define PROCESSING_WAITING 1
// normal operation. No cancelation nor death requested
#define PROCESSING_ACTIVE  0

MainWindowWorkerThread::MainWindowWorkerThread() throw (GUIException):
        m_localGame(),
        m_localLatestPiece(e_noPiece),
        m_localLatestCoord(),
        m_playerToMove(Game1v1::e_Game1v1Player1), // by default. It will be always set before calculating next move anyway
        m_heuristic(NULL),                         // by default. It will be always set before calculating next move anyway
        m_searchTreeDepth(3),                      // by default. It will be always set before calculating next move anyway
        m_threadStatus(PROCESSING_WAITING),
        m_thread(NULL),
        m_mutex(NULL),
        m_cond(NULL)
{
    m_mutex = g_mutex_new();
    if (m_mutex == NULL)
    {
        throw new GUIException(e_GUIException_ProcessingThreadErr, __FILE__, __LINE__);
#ifdef DEBUG
        assert(0);
#endif
    }

    m_cond = g_cond_new();
    if (m_cond == NULL)
    {
        throw new GUIException(e_GUIException_ProcessingThreadErr, __FILE__, __LINE__);
#ifdef DEBUG
        assert(0);
#endif
    }

    SpawnThread();
}

MainWindowWorkerThread::~MainWindowWorkerThread()
{
    g_cond_free(m_cond);
    g_mutex_free(m_mutex);
}

void MainWindowWorkerThread::SpawnThread() throw (GUIException)
{
    // configure the option JOINABLE to the thread creation
    bool joinable = true;
    GError* err   = NULL;

//    GThread * g_thread_create  ( GThreadFunc func,
//                                 gpointer data,
//                                 gboolean joinable,
//                                 GError **error);
    m_thread = g_thread_create(
            reinterpret_cast<GThreadFunc>(MainWindowWorkerThread::ThreadRoutine),
            reinterpret_cast<void*>(this),
            joinable,
            &err);

    if (m_thread == NULL)
    {
        // TODO err->message??
        g_error_free(err);
        throw new GUIException(e_GUIException_ProcessingThreadErr, __FILE__, __LINE__);
    }
}

void MainWindowWorkerThread::Join()
{
    g_mutex_lock(m_mutex);

    // request the thread to kill itself
    // current thread (the one that calls to this function) will be waiting for the worker thread
    // to die in the g_thread_join function
    m_threadStatus = PROCESSING_DIE;

    // wake up the worker thread to exit the ThreadRoutine in case it stuck there
    g_cond_broadcast(m_cond);

    g_mutex_unlock(m_mutex);

    void* dataReturned;
    dataReturned = g_thread_join(m_thread);
}

void MainWindowWorkerThread::CancelComputing()
{
    g_mutex_lock(m_mutex);

    if (m_threadStatus == PROCESSING_ACTIVE)
    {
        // worker thread is busy
        // request the thread to stop
        m_threadStatus = PROCESSING_STOP;

        // wait for the worker thread to cancel computing before keep going
        while (m_threadStatus == PROCESSING_STOP)
        {
            g_cond_wait(m_cond, m_mutex);
        }
    }

    g_mutex_unlock(m_mutex);
}

bool MainWindowWorkerThread::IsThreadComputingMove()
{
    bool rv;

    g_mutex_lock(m_mutex);
    rv = (m_threadStatus == PROCESSING_ACTIVE);
    g_mutex_unlock(m_mutex);

    return rv;
}

bool MainWindowWorkerThread::ComputeMove(
            const Game1v1            &a_game,
            Game1v1::eGame1v1Player_t a_whoMoves,
            Heuristic::EvalFunction_t a_heuristic,
            int32_t                   a_searchTreeDepth,
            bool                      a_blockCaller,
            const Coordinate         &a_latestCoordinate,
            const Piece              &a_latestPiece)
{
    bool rv = false;

    g_mutex_lock(m_mutex);

    // block current thread if a_blockCaller is set to true until worker thread
    // is ready to calculate next move or worker thread is requested to die
    while ( a_blockCaller &&
           (m_threadStatus != PROCESSING_WAITING) &&
           (m_threadStatus != PROCESSING_DIE) )
    {
        g_cond_wait(m_cond, m_mutex);
    }

    if (m_threadStatus == PROCESSING_WAITING)
    {
        // copy the 1v1Game, latest piece (and coord) put down by the opponent
        // before computing the calculation
        m_localGame        = a_game;
        m_localLatestPiece = a_latestPiece;
        m_localLatestCoord = a_latestCoordinate;
        m_playerToMove     = a_whoMoves;
        m_heuristic        = a_heuristic;
        m_searchTreeDepth  = a_searchTreeDepth;

        // set the thread to calculate a move (computing flag to active)
        m_threadStatus = PROCESSING_ACTIVE;

        // wake up the worker thread to start computing the next move
        g_cond_broadcast(m_cond);

        // the function will return true
        rv = true;
    }

    g_mutex_unlock(m_mutex);

    return rv;
}

void* MainWindowWorkerThread::ThreadRoutine(void *a_ThreadParam)
{
    MainWindowWorkerThread* thisThread =
            reinterpret_cast< MainWindowWorkerThread* >(a_ThreadParam);

    // Piece calculated by the Game1v1 AI
    Piece resultPiece;
    // Coordinate where the resultPiece should be put down
    Coordinate resultCoord;
    // the returned value by the computing process
    int32_t resultReturnedValue;

    while (thisThread->m_threadStatus != PROCESSING_DIE)
    {
        g_mutex_lock(thisThread->m_mutex);

        // the move has been calculated. Update the variable accordingly
        if (thisThread->m_threadStatus != PROCESSING_DIE)
        {
            // previous processing was finished (either it was computed or cancelled)
            // get back to waiting state unless the thread has been told to die
            thisThread->m_threadStatus = PROCESSING_WAITING;
        }

        while (thisThread->m_threadStatus == PROCESSING_WAITING)
        {
            // Previous processing is finished, either because it's been calculated
            // or it has been cancelled. Either way someone might be waiting
            // for us to notify thread is going to be ready. Send a signal to them to wake them up
            g_cond_broadcast(thisThread->m_cond);

            // wait for new jobs to arrive
            g_cond_wait(thisThread->m_cond, thisThread->m_mutex);
        }

        if (thisThread->m_threadStatus != PROCESSING_ACTIVE)
        {
            // there's nothing to compute
            // Go back to the beginning to check if we must die
            g_mutex_unlock(thisThread->m_mutex);
            continue;
        }

        // get out of the mutex. m_threadStatus is PROCESSING_ACTIVE
        // data which saves the next movement to be calculated is protected
        // (it is only written in ComputeMove, and it cannot be written while
        // m_threadStatus is set to PROCESSING_ACTIVE)
        g_mutex_unlock(thisThread->m_mutex);

        // calculate whose move is going to be calculated
        //const Player &currentPlayer = thisThread->m_localGame.GetPlayer(thisThread->m_playerToMove);
        const Player &opponent      = thisThread->m_localGame.GetOpponent(thisThread->m_playerToMove);

        // we've been told to calculate the move
        do
        {
            resultPiece = Piece(e_noPiece);
            resultCoord = Coordinate();

            resultReturnedValue = thisThread->m_localGame.MinMax(
                                        thisThread->m_heuristic,
                                        thisThread->m_searchTreeDepth,
                                        thisThread->m_playerToMove,
                                        resultPiece,
                                        resultCoord,
                                        thisThread->m_threadStatus,
                                        thisThread->m_localLatestCoord,
                                        thisThread->m_localLatestPiece);

            if (thisThread->m_threadStatus != PROCESSING_ACTIVE)
            {
                resultPiece = e_noPiece;
                resultCoord = Coordinate();

                // no need to notify the cancelled result to any listener
                break;
            }

            // notify the result
            thisThread->signal_computingFinished().emit(
                    resultPiece,
                    resultCoord,
                    thisThread->m_playerToMove,
                    resultReturnedValue);

            // update the local game as well in case the computer has
            // to put more than 1 move in a row
            if (resultPiece.GetType() != e_noPiece)
            {
                thisThread->m_localGame.PutDownPiece(
                        resultPiece,
                        resultCoord,
                        thisThread->m_playerToMove);
            }

        } while ( (resultPiece.GetType() != e_noPiece) &&
                  (rules::CanPlayerGo(thisThread->m_localGame.GetBoard(),
                                      opponent) == false) );

    } // while (thisThread->m_threadStatus != PROCESSING_DIE)

    return NULL;
}
