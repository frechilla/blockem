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
#include "game1v1.h"
#include "gui_exception.h"

/// @brief Thread which will be used by the MainWindow to do heavy computing
/// heavy computing must be done in a separate thread because the GUI is not updated
/// while the next move is being calculated, which ends up in an unresponsive window
class MainWindowWorkerThread :
    public sigc::trackable // trackable must be inherited when objects shall automatically
                           // invalidate slots referring to them on destruction
{
public:
    MainWindowWorkerThread() throw (GUIException);
    virtual ~MainWindowWorkerThread();

    /// @returns true if the thread is busy computing a move. False otherwise
    bool IsThreadComputingMove();

    /// sends a signal to the thread to start calculating a move
    /// If the thread is already calculating a move it won't do anything
    /// @param the game to be used for the calculation.
    ///        it will be copied in the thread to compute the move
    /// @param player whose move is supposed to be calculated by MinMax
    /// @param heuristic/evaluation function to be used by the MinMax algorithm
    /// @param search tree depth. It might be set to GAME1V1CONFIG_DEPTH_AUTOADJUST to indicate
    ///        it will be left up to this thread depending on the expected lenght of the search tree
    /// @param Caller will block until worker thread finishes current job and starts to compute
    ///        the job notified by this call. If it set to true it can only fail if worker thread
    ///        is requested to die
    /// @param coordinate where the latest piece was put down
    /// @param latest piece put down by the opponent
    /// @return true if the thread was idle. false if it was already computing
    bool ComputeMove(
            const Game1v1            &a_game,
            Game1v1::eGame1v1Player_t a_whoMoves,
            Heuristic::EvalFunction_t a_heuristic,
            int32_t                   a_searchTreeDepth,
            bool                      a_blockCaller,
            const Coordinate         &a_latestCoordinate = Coordinate(),
            const Piece              &a_latestPiece      = Piece(e_noPiece));

    /// Cancels the move being computed by the worker thread and force it to get ready
    /// for new jobs. The caller is blocked until the worker thread is ready and waiting
    /// for another move to be computed
    void CancelComputing();
            
    /// suspends execution of the calling thread until the target thread terminates
    void Join();

    /// Access to the signal private member
    sigc::signal<void, const Piece&, const Coordinate&, Game1v1::eGame1v1Player_t, int32_t>& signal_computingFinished()
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
    /// Whose move will be calculated next time MinMax is called
    Game1v1::eGame1v1Player_t m_playerToMove;
    /// Heuristic/evaluation function to be used by the MinMAx algorithm
    Heuristic::EvalFunction_t m_heuristic;
    /// depth of search tree for the minimax algorithm
    /// It might be set to GAME1V1CONFIG_DEPTH_AUTOADJUST to indicate
    /// t will be left up to this thread depending on the expected lenght of the search tree
    int32_t m_searchTreeDepth;
    /// set to true whenever we want the thread to terminate by itself
    volatile sig_atomic_t m_threadStatus;

    /// the actual thread
    GThread* m_thread;
    /// Mutex to protect the queue
    GMutex* m_mutex;
    /// Conditional variable to wake up threads
    GCond* m_cond;

    /// signal to be sent when the computing is finished
    sigc::signal<void, const Piece&, const Coordinate&, Game1v1::eGame1v1Player_t, int32_t> m_signal_computingFinished;

    /// creates the gthread
    void SpawnThread() throw (GUIException);

    /// The routine that will be run by the gthread created in SpawnThread
    static void* ThreadRoutine(void *a_ThreadParam);
    
    // no copy constructor or operator= allowed
    MainWindowWorkerThread(const MainWindowWorkerThread& a_src);
    MainWindowWorkerThread& operator= (const MainWindowWorkerThread& a_src);
};

#endif /* MAINWINDOWWORKERTHREAD_H_ */
