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
/// @file gui_main_window_worker_thread.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 24 Nov 2009  Original development
/// @endhistory
///
// ============================================================================

//TODO iostream should go away once gthreads are handled correctly
#include <iostream>
#include "gui_main_window_worker_thread.h"

#define PROCESSING_STOP   1
#define PROCESSING_ACTIVE 0

MainWindowWorkerThread::MainWindowWorkerThread():
        m_localGame(),
        m_localLatestPiece(e_noPiece),
        m_computeNextMove(false),
        m_terminate(PROCESSING_ACTIVE),
        m_thread(NULL),
        m_mutex(NULL),
        m_cond(NULL)
{
    m_mutex = g_mutex_new();
    if (m_mutex == NULL)
    {
        std::cerr << "Mutex in worker thread couldn't be initialised" << std::endl;
        assert(0);
    }

    m_cond = g_cond_new();
    if (m_cond == NULL)
    {
        std::cerr << "Conditional variable in worker thread couldn't be initialised" << std::endl;
        assert(0);
    }

    // create and initialise the randomizer using now time as the seed
    GTimeVal timeNow;
    g_get_current_time(&timeNow);
    m_randomizer = g_rand_new_with_seed(static_cast<guint32>(timeNow.tv_sec ^ timeNow.tv_usec));

    SpawnThread();
}

MainWindowWorkerThread::~MainWindowWorkerThread()
{
    g_cond_free(m_cond);
    g_mutex_free(m_mutex);

    g_rand_free(m_randomizer);
}

void MainWindowWorkerThread::SpawnThread()
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
        std::cerr << "Worker Thread creation failed. System won't work as expected" << std::endl;
        std::cerr << "    " << err->message << std::endl;
        g_error_free(err) ;
        assert(0);
    }
}

bool MainWindowWorkerThread::Join()
{
    g_mutex_lock(m_mutex);

    m_terminate = PROCESSING_STOP;

    // wake up the worker thread to exit the ThreadRoutine
    g_cond_broadcast(m_cond);

    g_mutex_unlock(m_mutex);

    void* dataReturned;
    dataReturned = g_thread_join(m_thread);

    return true;
}

bool MainWindowWorkerThread::IsThreadComputingMove()
{
    bool rv;

    g_mutex_lock(m_mutex);
    rv = m_computeNextMove;
    g_mutex_unlock(m_mutex);

    return rv;
}

bool MainWindowWorkerThread::ComputeMove(
        const Game1v1    &a_game,
        const Piece      &a_latestPiece,
        const Coordinate &a_latestCoordinate)
{
    bool rv = false;

    g_mutex_lock(m_mutex);
    if (m_computeNextMove == false)
    {
        // copy the 1v1Game, latest piece (and coord) put down by the opponent
        // before computing the calculation
        m_localGame        = a_game;
        m_localLatestPiece = a_latestPiece;
        m_localLatestCoord = a_latestCoordinate;

        // set the thread to calculate a move
        m_computeNextMove = true;
        // the function will return true
        rv = true;
    }

    if (rv == true)
    {
        // wake up the worker thread to start computing the next move
        g_cond_broadcast(m_cond);
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

    while (thisThread->m_terminate == PROCESSING_ACTIVE)
    {
        g_mutex_lock(thisThread->m_mutex);

        while ( (thisThread->m_terminate == PROCESSING_ACTIVE) &&
                (thisThread->m_computeNextMove == false) )
        {
            g_cond_wait(thisThread->m_cond, thisThread->m_mutex);
        }

        if ( (thisThread->m_computeNextMove == false) ||
             (thisThread->m_terminate == PROCESSING_STOP) )
        {
            g_mutex_unlock(thisThread->m_mutex);
            continue;
        }

        // get out of the mutex
        g_mutex_unlock(thisThread->m_mutex);

        // we've been told to calculate the move
        do
        {
            resultPiece = Piece(e_noPiece);
            resultCoord.m_row = resultCoord.m_col = 0;

            int32_t depth = 3;
            Heuristic::calculateMethod_t heuristicMethod = Heuristic::CalculateNKWeighted;
            if ( (thisThread->m_localGame.GetPlayerMe().NumberOfPiecesAvailable() < 14) &&
                 (thisThread->m_localGame.CanPlayerOpponentGo()) )
            {
                depth = 5;
                // heuristicMethod = Heuristic::CalculatePiecesPerNKPoint;
            }

            if (thisThread->m_localGame.GetPlayerMe().NumberOfPiecesAvailable() == e_numberOfPieces)
            {
                // pass the move made by the opponent to the minmax algorithm at the start half of the times
                // it will show a bit of randomness at the start to a human user
                if (g_rand_int_range(thisThread->m_randomizer, 0, 2) == 0)
                {
                    resultReturnedValue = thisThread->m_localGame.MinMax(
                                                heuristicMethod,
                                                depth,
                                                resultPiece,
                                                resultCoord,
                                                thisThread->m_terminate,
                                                thisThread->m_localLatestCoord,
                                                thisThread->m_localLatestPiece);
                }
                else
                {
                    resultReturnedValue = thisThread->m_localGame.MinMax(
                                                heuristicMethod,
                                                depth,
                                                resultPiece,
                                                resultCoord,
                                                thisThread->m_terminate);
                }
            }
            else
            {
                // it's not the starting move
                resultReturnedValue = thisThread->m_localGame.MinMax(
                                            heuristicMethod,
                                            depth,
                                            resultPiece,
                                            resultCoord,
                                            thisThread->m_terminate,
                                            thisThread->m_localLatestCoord,
                                            thisThread->m_localLatestPiece);
            }

            // notify the result
            thisThread->signal_computingFinished().emit(
                    resultPiece,
                    resultCoord,
                    resultReturnedValue);

            // update the local game as well in case the computer has
            // put more than 1 move in a row
            if (resultPiece.GetType() != e_noPiece)
            {
                thisThread->m_localGame.PutDownPieceMe(resultPiece, resultCoord);
            }

        } while ( (thisThread->m_localGame.CanPlayerOpponentGo() == false) &&
                  (resultPiece.GetType() != e_noPiece) );

        // the move has been calculated. Update the variable accordingly
        g_mutex_lock(thisThread->m_mutex);
        thisThread->m_computeNextMove = false;
        g_mutex_unlock(thisThread->m_mutex);
    }

    return NULL;
}
