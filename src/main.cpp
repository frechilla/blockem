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
#include <glib.h>
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

/// @brief parse the command line arguments
/// @return 0 if the command line arguments were correct but the execution is finished
///         >0 if the command line arguments were correct and the execution has to continue
///         <0 if the command line arguments were not correct, so the execution is finished
int parseCommandLine(int argc, char** argv)
{
    //TODO this should be done using some library when it gets more complicated.
    // have a look at boost::program_options::options_description

    if (argc > 2)
    {
        std::cout << argv[0] << ": unrecognized options. Try: " << argv[0] << " --help" << std::endl;
        return -1;
    }
    else if (argc == 2)
    {
        if ((strcmp(argv[1], "--version") == 0) || (strcmp(argv[1], "-v") == 0))
        {
            std::cout << "Blockem, version " << VERSION << " (r" << SVN_REVISION << ")" << std::endl;
            std::cout << "  Compiled " << COMPILETIME << std::endl << std::endl;
            std::cout << "Copyright (C) Faustino Frechilla" << std::endl;
            std::cout << "Blockem is open source software, see http://sourceforge.net/projects/blockem" << std::endl;

            return 0;
        }
        else if((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0))
        {
            std::cout << "usage: " << argv[0] << " [OPTION]" << std::endl << std::endl;
            std::cout << "  -v, --version     prints the current version of the software and exists" << std::endl;
            std::cout << "  -h, --help        prints this help text and exists" << std::endl;

            return 0;
        }

        return -1;
    }

    return 1;
}

int main(int argc, char **argv)
{
    int rv = parseCommandLine(argc, argv);
    if (rv <= 0)
    {
        return rv;
    }

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

    return 0;

    // GUI
    //////////////////


#if 0
    if (argc < 3)
    {
        std::cerr << "usage: " << argv[0] << " [depth] [file]" << std::endl;
        exit(-1);
    }

    int32_t depth = atoi(argv[1]);
    if ( (depth & 0x01) == 0)
    {
        std::cerr << "Warning: Are you sure you don't want the depth to be an odd number?" << std::endl;
    }

    Heuristic::calculateMethod_t heuristicNKWeighted = Heuristic::CalculateNKWeighted;
    Heuristic::calculateMethod_t heuristicSimple     = Heuristic::CalculateSimple;

    Heuristic::calculateMethod_t heuristic = heuristicNKWeighted;
    //Heuristic::calculateMethod_t heuristic = heuristicSimple;

    std::string filename(argv[2]);
    std::ifstream cin;

    cin.open(filename.c_str());
    if(!cin)
    {
        std::cerr << "Error: file could not be opened" << std::endl;

        exit (-2);
    }

    Game1v1 theGame;
    if (theGame.LoadGame(cin) == false)
    {
        std::cerr << "Error while loading the game from " << filename.c_str() << std::endl;

        cin.close();

        exit(-3);
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

    std::cout << theGame.MinMax(heuristic, depth, resultPiece, resultCoord) << std::endl;

    if (resultPiece.GetType() == e_noPiece)
    {
        std::cout << std::endl;
        std::cout << "\t===================================================" << std::endl;
        std::cout << "\t=== END OF THE GAME. NO PIECE COULD BE PUT DOWN ===" << std::endl;
        std::cout << "\t===================================================" << std::endl;
    }
    else
    {
        theGame.PutDownPieceMe(resultPiece, resultCoord);
    }

    theGame.SaveGame(std::cout);

    //theGame.GetMe().PrintNucleationPoints(std::cout);
    //theGame.GetOpponent().PrintNucleationPoints(std::cout);


    return 0;
#endif
}



