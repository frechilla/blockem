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
/// @file main.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 30-Mar-2009  Original development
/// @endhistory
///
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // strcmp
#include <iostream>
#include <fstream>
#include <glib.h>   // glib-Commandline-option-parser
#include <libglademm/xml.h>
#include <gtkmm.h>
#include "config.h" // autotools header file
#include "assert.h"

// header file created during the make process which saves the current date
#include "compiletime.h"

#include "gui_main_window.h"
#include "game1v1.h"


// the following code is needed to load the gui into the application.
// (See big comment in gui.h)
// objdump in windows and linux is different and they create
// the following variables different from one another.
// Windows hasn't got any underscore at the start, while the object in linux
// has it. This is enough to check whether we are in the windows environment
// or not
#ifdef WIN32
extern const char binary_gui_glade_start[];
extern const char binary_gui_glade_end[];
extern int        binary_gui_glade_size[];

#define __BIN_GUI_GLADE_START__ binary_gui_glade_start
#define __BIN_GUI_GLADE_END__   binary_gui_glade_end
#define __BIN_GUI_GLADE_SIZE__  binary_gui_glade_size

#else
extern const char _binary_gui_glade_start[];
extern const char _binary_gui_glade_end[];
extern int        _binary_gui_glade_size[];

#define __BIN_GUI_GLADE_START__ _binary_gui_glade_start
#define __BIN_GUI_GLADE_END__   _binary_gui_glade_end
#define __BIN_GUI_GLADE_SIZE__  _binary_gui_glade_size

#endif // ifdef WIN32

// parsing command line arguments
// http://library.gnome.org/devel/glib/unstable/glib-Commandline-option-parser.html
// there was also the option of popt (http://directory.fsf.org/project/popt) but it would have added
// an extra dependency
static gboolean s_version = 0;
static gint s_depth = 0;
static gchar *s_blockemfilePath = NULL;

static GOptionEntry s_cmdEntries[] =
{
    { "version",   0, 0, G_OPTION_ARG_NONE,   &s_version, "Prints the current version of the software and exists", NULL },
    { "compute", 'c', 0, G_OPTION_ARG_STRING, &s_blockemfilePath, "Loads game saved in FILE, calculates next move and prints out the result in console", "FILE"},
    { "depth"  , 'd', 0, G_OPTION_ARG_INT,    &s_depth, "sets the maximum depth of search tree to N when using \"--compute\"", "N"},
//    {G_OPTION_REMAINING, 0, G_OPTION_ARG_FILENAME_ARRAY, &s_fileArray, "Blockem game file to load. The app will calculate next move and print out the result in console. GUI won't be started", "[FILE]"},
//    { "max-size", 'm', 0, G_OPTION_ARG_INT, &max_size, "Test up to 2^M items", "M" },
//    { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Be verbose", NULL },
//    { "beep", 'b', 0, G_OPTION_ARG_NONE, &beep, "Beep when done", NULL },
//    { "rand", 0, 0, G_OPTION_ARG_NONE, &rand, "Randomize the data", NULL },
    { NULL }
};


int main(int argc, char **argv)
{
    GError *error = NULL;
    GOptionContext *cmdContext;

    cmdContext = g_option_context_new ("- A GNU clone of 1vs1 blokus");
    // to add i18n:
    //   g_option_context_add_main_entries (cmdContext, s_cmdEntries, GETTEXT_PACKAGE);
    g_option_context_add_main_entries (cmdContext, s_cmdEntries, NULL);
    // http://library.gnome.org/devel/glib/unstable/glib-Commandline-option-parser.html#g-option-context-add-group
    g_option_context_add_group (cmdContext, gtk_get_option_group (TRUE));
    if (!g_option_context_parse (cmdContext, &argc, &argv, &error))
    {
        std::cout << argv[0] << ": Error parsing command line: " << error->message << std::endl;
        std::cout << argv[0] << ": Try '" << argv[0] <<  " --help'" << std::endl;
        return 1;
    }

    if (s_version)
    {
        std::cout << "Blockem, version " << VERSION << " (r" << SVN_REVISION << ")" << std::endl;
        std::cout << "  Compiled " << COMPILETIME << std::endl << std::endl;
        std::cout << "Copyright (C) 2009-2010 Faustino Frechilla" << std::endl;
        std::cout << "Blockem is open source software, see http://blockem.sourceforge.net/"
                  << std::endl << std::endl;

        return 0;
    }

    if (s_blockemfilePath != NULL)
    {
        if (!g_file_test(s_blockemfilePath, G_FILE_TEST_IS_REGULAR))
        {
            std::cerr << "Error: " << s_blockemfilePath << " doesn't exist. Exiting..." << std::endl;
            return 2;
        }

        if (s_depth <= 0)
        {
            std::cerr << "Error: depth must be set to a positive value. Exiting..." << std::endl;
            return 3;
        }

        std::cout << "Loading game from " << s_blockemfilePath << " and calculating next move..." << std::endl
                  << "    (search tree depth set to: " << s_depth << ")" << std::endl << std::endl;


        if ( (s_depth & 0x01) == 0)
        {
            std::cerr << "Warning: For better results you might want to set the depth to an odd number" << std::endl;
        }

        Heuristic::EvalFunction_t heuristicNKWeighted = Heuristic::CalculateNKWeighted;
        //Heuristic::EvalFunction_t heuristicSimple = Heuristic::CalculateSimple;
        //Heuristic::EvalFunction_t heuristicPiecePerNKPoint = Heuristic::CalculatePiecesPerNKPoint;

        Heuristic::EvalFunction_t heuristic = heuristicNKWeighted;
        //Heuristic::EvalFunction_t heuristic = heuristicSimple;
        //Heuristic::EvalFunction_t heuristic = heuristicPiecePerNKPoint;

        std::ifstream cin;

        cin.open(s_blockemfilePath);
        if(!cin)
        {
            std::cerr << "Error: file could not be opened" << std::endl;
            return 4;
        }

        Game1v1 theGame;
        if (theGame.LoadGame(cin) == false)
        {
            std::cerr << "Error while loading the game from " << s_blockemfilePath << std::endl;
            cin.close();

            return 4;
        }

        cin.close();

        theGame.SaveGame(std::cout);

        //test(&theGame, theGame.GetMe(), theGame.GetOpponent());
        //exit(0);

        //std::cout << std::endl << std::endl;
        //theGame.GetMe().PrintNucleationPoints(std::cout);

        //std::cout << std::endl << std::endl;
        //theGame.GetOpponent().PrintNucleationPoints(std::cout);


        Piece resultPiece(e_noPiece);
        Coordinate resultCoord;

        volatile sig_atomic_t dummyAtomic = 0;
        int32_t minimaxWinner =
            theGame.MinMax(
                    heuristic,
                    s_depth,
                    Game1v1::e_Game1v1Player2,
                    resultPiece,
                    resultCoord,
                    dummyAtomic);

        std::cout << "Winning evaluation function value: " << minimaxWinner << std::endl;

        if (resultPiece.GetType() == e_noPiece)
        {
            std::cout << std::endl;
            std::cout << "\t===================================================" << std::endl;
            std::cout << "\t=== END OF THE GAME. NO PIECE COULD BE PUT DOWN ===" << std::endl;
            std::cout << "\t===================================================" << std::endl;
        }
        else
        {
            theGame.PutDownPiece(
                    resultPiece,
                    resultCoord,
                    Game1v1::e_Game1v1Player2);
        }

        theGame.SaveGame(std::cout);

        //theGame.GetMe().PrintNucleationPoints(std::cout);
        //theGame.GetOpponent().PrintNucleationPoints(std::cout);

        return 0;
    } // if (s_blockemfilePath != NULL)


    //////////////////
    // GUI

    Gtk::Main::init_gtkmm_internals();

    // Initialise gthreads even before gtk_init
    if(!g_thread_supported())
    {
        g_thread_init(NULL);
        // This call is needed only if extra threads (not the main thread)
        // updates directly the GUI widgets using gdk_threads_enter
        // and gdk_threads_leave
        // It's not really needed since there's only 1 extra thread
        // (apart from the main one) and it doesn't update the GUI
        // straight away. It uses signals for that matter (see comment
        // in MainWindow::WorkerThread_computingFinished)
        //gdk_threads_init();
    }
    else
    {
        std::cerr << "error: gthread_supported call failed when initialising gthreads" << std::endl;
        std::cerr << "error: App can't start without gthreads support" << std::endl;
        return -1;
    }

    Gtk::Main kit(argc, argv);

    Glib::RefPtr<Gnome::Glade::Xml> refXml;
#ifdef GLIBMM_EXCEPTIONS_ENABLED
    try
    {
        refXml = Gnome::Glade::Xml::create_from_buffer(
                reinterpret_cast<const char *>(__BIN_GUI_GLADE_START__),
                reinterpret_cast<long int>(__BIN_GUI_GLADE_SIZE__),
                "",
                "");
    }
    catch(const Gnome::Glade::XmlError& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
#else
    std::auto_ptr<Gnome::Glade::XmlError> error;
    refXml = Gnome::Glade::Xml::create_from_buffer(
            reinterpret_cast<const char *>(__BIN_GUI_GLADE_START__),
            reinterpret_cast<long int>(__BIN_GUI_GLADE_SIZE__),
            "",
            "",
            error);

    if(error.get())
    {
        std::cerr << error->what() << std::endl;
        return 1;
    }
#endif // GLIBMM_EXCEPTIONS_ENABLED

    try
    {
        MainWindow::Instance();
        MainWindow::Instance().Initialize(refXml);

        // if gdk_threads_enter and gdk_threads_leave were to be used
        // the Gtk::Main::run loop should be surrounded by
        // gdk_threads_enter and gdk_threads_leave
        // http://tadeboro.blogspot.com/2009/06/multi-threaded-gtk-applications.html

        kit.run(MainWindow::Instance().window());
    }
    catch (GUIException ex)
    {
        std::cerr << ex.what() << std::endl;
        exit(-1);
    }

    // GUI
    //////////////////

    return 0;
}



