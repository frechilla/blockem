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
// with Foobar. If not, see http://www.gnu.org/licenses/.
//
/// @file MainWindowWorkerThread.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 24 Nov 2009  Original development
/// @endhistory
///
// ============================================================================

#include "MainWindowWorkerThread.h"

MainWindowWorkerThread::MainWindowWorkerThread():
        m_localGame(),
        m_localLatestPiece(e_noPiece),
        m_computeNextMove(false),
        m_terminate(false)
{
    if (pthread_mutex_init(&m_mutex, NULL) != 0)
    {
        assert(0);
    }

    if (pthread_cond_init(&m_cond, NULL) != 0)
    {
        assert(0);
    }

    SpawnThread();
}

MainWindowWorkerThread::~MainWindowWorkerThread()
{
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

void MainWindowWorkerThread::SpawnThread()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // add the option JOINABLE to the attribute struct
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if (pthread_create(
            &m_thread,
            &attr,
            MainWindowWorkerThread::ThreadRoutine,
            reinterpret_cast<void*>(this)) != 0)
    {
        assert(0);
    }
}

bool MainWindowWorkerThread::Join()
{
    void* dataReturned;

    pthread_mutex_lock(&m_mutex);

    m_terminate = true;

    // wake up the worker thread to exit the ThreadRoutine
    pthread_cond_broadcast(&m_cond);

    pthread_mutex_unlock(&m_mutex);

    if (pthread_join(m_thread, &dataReturned) != 0)
    {
        return false;
    }

    return true;
}

void MainWindowWorkerThread::Terminate()
{
    pthread_cancel(m_thread);
}

bool MainWindowWorkerThread::IsThreadComputingMove()
{
    bool rv;

    pthread_mutex_lock(&m_mutex);
    rv = m_computeNextMove;
    pthread_mutex_unlock(&m_mutex);

    return rv;
}

bool MainWindowWorkerThread::ComputeMove(
        const Game1v1    &a_game,
        const Piece      &a_latestPiece,
        const Coordinate &a_latestCoordinate)
{
    bool rv = false;

    pthread_mutex_lock(&m_mutex);
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
        pthread_cond_broadcast(&m_cond);
    }

    pthread_mutex_unlock(&m_mutex);

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

    bool terminate = false;
    while (terminate == false)
    {
        pthread_mutex_lock(&thisThread->m_mutex);

        int32_t retcode;
        while ( (thisThread->m_terminate == false) &&
                (thisThread->m_computeNextMove == false) )
        {
            retcode = pthread_cond_wait(&thisThread->m_cond, &thisThread->m_mutex);
        }

        // save the protected variables into a temporary one
        bool doMove = thisThread->m_computeNextMove;
        terminate   = thisThread->m_terminate;

        // get out of the mutex
        pthread_mutex_unlock(&thisThread->m_mutex);

        if ( doMove && (terminate == false) )
        {
            // we've been told to calculate the move
            do
            {
                resultPiece = Piece(e_noPiece);
                resultCoord.m_X = resultCoord.m_Y = 0;

                int32_t depth = 3;
                Heuristic::calculateMethod_t heuristicMethod = Heuristic::CalculateNKWeighted;
                if (thisThread->m_localGame.GetPlayerMe().NumberOfPiecesAvailable() < 14)
                {
                    depth = 5;
                }

                resultReturnedValue = thisThread->m_localGame.MinMax(
                                            heuristicMethod,
                                            depth,
                                            resultPiece,
                                            resultCoord,
                                            thisThread->m_localLatestCoord,
                                            thisThread->m_localLatestPiece);

                // notify the result
                thisThread->signal_computingFinished().emit(
                        resultPiece,
                        resultCoord,
                        resultReturnedValue);

                // update the local game asa well in case the computer has
                // put more than 1 move in a row
                if (resultPiece.GetType() != e_noPiece)
                {
                    thisThread->m_localGame.PutDownPieceMe(resultPiece, resultCoord);
                }

            } while ( (thisThread->m_localGame.CanPlayerOpponentGo() == false) &&
                      (resultPiece.GetType() != e_noPiece) );

            // the move has been calculated. Update the variable accordingly
            pthread_mutex_lock(&thisThread->m_mutex);
            thisThread->m_computeNextMove = false;
            pthread_mutex_unlock(&thisThread->m_mutex);
        }
    }

    return NULL;
}
