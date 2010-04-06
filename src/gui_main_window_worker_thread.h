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
/// @file gui_main_window_worker_thread.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 24 Nov 2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef MAINWINDOWWORKERTHREAD_H_
#define MAINWINDOWWORKERTHREAD_H_

#include <glib.h>             // glib-2.0/glib.h
#include <sigc++/trackable.h> // sigc++-2.0/sigc++
#include <sigc++/signal.h>    // sigc++-2.0/sigc++
#include "Game1v1.h"

/// @brief Thread which will be used by the MainWindow to do heavy computing
/// heavy computing must be done in a separate thread because the GUI is not updated
/// while the next move is being calculated, which ends up in an unresponsive window
class MainWindowWorkerThread :
    public sigc::trackable // trackable must be inherited when objects shall automatically
                           // invalidate slots referring to them on destruction
{
public:
    MainWindowWorkerThread();
    virtual ~MainWindowWorkerThread();

    /// @returns true if the thread is busy computing a move. False otherwise
    bool IsThreadComputingMove();

    /// sends a signal to the thread to start calculating a move
    /// If the thread is already calculating a move it won't do anything
    /// @param the game to be sued for the calculation.
    ///        it will be copied in the thread to compute the move
    /// @param latest piece put down by the opponent
    /// @param coordinate where the latest piece was put down
    /// @returns true if the thread was idle. false if it was already computing
    bool ComputeMove(
            const Game1v1 &a_game,
            const Piece      &a_latestPiece,
            const Coordinate &a_latestCoordinate);

    /// suspends execution of the calling thread until the target thread terminates
    /// @return true if success. False otherwise
    bool Join();

    /// The routine that will be called by the posix thread
    static void* ThreadRoutine(void *a_ThreadParam);

    /// Access to the signal private member
    sigc::signal<void, const Piece&, const Coordinate&, int32_t>& signal_computingFinished()
    {
        return m_signal_computingFinished;
    }

private:
    /// The game that the thread will use for calculations
    Game1v1 m_localGame;
    /// The latest piece put down by the opponent
    Piece m_localLatestPiece;
    /// The coordinate where the latest piece was put down
    Coordinate m_localLatestCoord;
    /// set to true when the thread has to calculate some move
    /// it'll be set to false once the move's been already calculated
    bool m_computeNextMove;
    /// set to true whenever we want the thread to terminate by itself
    volatile sig_atomic_t m_terminate;

    /// the actual thread
    GThread* m_thread;
    /// Mutex to protect the queue
    GMutex* m_mutex;
    /// Conditional variable to wake up threads
    GCond* m_cond;

    /// we might use a bit of randomness when computing the next move
    GRand* m_randomizer;

    /// signal to be sent when the computing is finished
    sigc::signal<void, const Piece&, const Coordinate&, int32_t> m_signal_computingFinished;

    void SpawnThread();

    // no copy constructor or operator= allowed
    MainWindowWorkerThread(const MainWindowWorkerThread& a_src);
    MainWindowWorkerThread& operator= (const MainWindowWorkerThread& a_src);
};

#endif /* MAINWINDOWWORKERTHREAD_H_ */
