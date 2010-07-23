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
///           Faustino Frechilla 23-Jun-2010  Nice handling of command line parameters
///           Faustino Frechilla 21-Jul-2010  i18n
/// @endhistory
///
// ============================================================================

#include <stdio.h>
#include <cstdio>        // printf (needed for better i18n)
#include <stdlib.h>
#include <math.h>
#include <string.h>      // strcmp
#include <iostream>
#include <fstream>
#include <glib.h>       // glib-Commandline-option-parser
#include <gtkmm.h>
#include <glib/gi18n.h> // i18n
#include "config.h"     // autotools header file
#include "assert.h"

// header file created during the make process which saves the current date
#include "compiletime.h"
#include "gui_glade_defs.h"
#include "gui_main_window.h"
#include "game1v1.h"
#include "game_total_allocation.h"
#include "gui_game1v1_config.h" // initialise singleton
#include "heuristic.h"

// list of error codes returned to the OS
static const int COMMAND_LINE_PARSING_ERR    = 1;
static const int TOTAL_ALLOC_BAD_OPTIONS_ERR = 3;
static const int GAME1V1_BAD_OPTIONS_ERR     = 7;
static const int BAD_MODE_OPTION_ERR         = 15;
static const int GUI_EXCEPTION_ERR           = 127;

// the following code is needed to load the gui into the application.
// (See big comment in src/gui_glade.h)
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
static gint GOPTION_INT_NOT_SET = -2147483647;

static gboolean g_version    = FALSE; // default is not to show --version message
static gint g_mode           = 0;     // default is --mode=0
static gint g_heuristic      = 0;     // default is --heuristic=0
static gint g_rows           = GOPTION_INT_NOT_SET;
static gint g_columns        = GOPTION_INT_NOT_SET;
static gint g_startingRow    = GOPTION_INT_NOT_SET;
static gint g_startingColumn = GOPTION_INT_NOT_SET;
static gint g_depth          = GOPTION_INT_NOT_SET;
static gchar** g_blockemfilePath = NULL;

// typedef struct {
//   const gchar *long_name;
//   gchar        short_name;
//   gint         flags;
//
//   GOptionArg   arg;
//   gpointer     arg_data;
//
//   const gchar *description;
//   const gchar *arg_description;
// } GOptionEntry;
static GOptionEntry g_cmdEntries[] =
{
    { "version", 0, 0, G_OPTION_ARG_NONE, &g_version,
        "Prints current version of the software and exists", NULL },
    { "mode", 'm', 0, G_OPTION_ARG_INT, &g_mode,
        "Mandatory parameter which specifies the mode blockem runs. Valid options:\n"\
        "                                  + 0: GUI is shown (Default operation)\n"\
        "                                  + 1: 1 player total-allocation\n"\
        "                                  + 2: 1vs1 Game", "M" },
    { "rows", 'r', 0, G_OPTION_ARG_INT, &g_rows,
        "1 player total allocation game's board will have N rows.\n"\
        "                              This is a MANDATORY parameter for --mode=1", "N"},
    { "columns", 'c', 0, G_OPTION_ARG_INT, &g_columns,
        "1 player total allocation game's board will have N columns.\n"\
        "                              This is a MANDATORY parameter for --mode=1", "N"},
    { "starting-row", 'x', 0, G_OPTION_ARG_INT, &g_startingRow,
        "Computer will start to allocate pieces in row X (when running with --mode=1).\n"\
        "                              First valid row is 0 (so maximum allowed row will be (number_of_rows - 1).\n"\
        "                              This is a MANDATORY parameter for --mode=1", "X"},
    { "starting-column", 'y', 0, G_OPTION_ARG_INT, &g_startingColumn,
        "Computer will start to allocate pieces in column Y (when running with --mode=1).\n"\
        "                              First valid column is 0 (so maximum allowed column will be (number_of_columns - 1).\n"\
        "                              This is a MANDATORY parameter for --mode=1", "Y"},
    { "depth"  , 'd', 0, G_OPTION_ARG_INT, &g_depth,
        "sets the maximum depth of search tree to N when 1vs1 Game is selected.\n"\
        "                              This is a MANDATORY parameter for --mode=2", "N"},
    { "heuristic", 'h', 0, G_OPTION_ARG_INT, &g_heuristic,
        "Heuristic to be used when mode is set to 1v1 game (--mode=2). Valid options:\n"\
        "                                  + 0 \"Influence Area\" (Default)\n"\
        "                                  + 1 \"Mr. Eastwood\"\n"\
        "                                  + 2 \"NK weighted\"\n"\
        "                                  + 3 \"Centre focused\"\n"\
        "                                  + 4 \"Simple\"\n"\
        "                                  + 5 \"Random\"", "N" },

    { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &g_blockemfilePath,
        "Paths to 1vs1game files (mode 2). Blockem will calculate next move per each one of them\n"\
        "                              and print out the result in console.\n"\
        "                              Specifying at least 1 file is MANDATORY for --mode=2", "FILE [ FILE ... ]"},

    { NULL }
};

// must be deleted before the app exits
static GOptionContext* g_cmdContext = NULL;

/// @brief print the error message on the screen using a expected format and exits the app
/// @param name of the binary (argv[0] in main)
/// @param custom error message
/// @param value which will be returned to the OS
void FatalError(
    const char* a_binName,
    const char* a_customError,
    int         a_errorCode)
{

    std::cerr << a_binName
              // i18n TRANSLATORS: Please leave the starting colon since it is part of the expected format
              // i18n Bear in mind this string will be used to print out a help message when an error is found
              // i18n A typical example would be: "./blockem: Fatal error: error 123 while trying to connect"
              // i18n That is the "Fatal error" sentence that needs to be translated
              // i18n Thank you for contributing to this project
              << _(": Fatal error")
              << ": "
              << a_customError
              << std::endl;

    std::cerr << a_binName
              // i18n TRANSLATORS: Please leave the starting colon since it is part of the expected format
              // i18n Bear in mind this string will be used to print out a help message when an error is found
              // i18n A typical example would be: "./blockem: Try './blockem --help'". That is the "try" word
              // i18n that is to be translated here
              // i18n Thank you for contributing to this project
              << _(": Try")
              << " '"
              << a_binName
              <<  " --help'"
              << std::endl;

    // free command line parsing resources
    if (g_cmdContext != NULL)
    {
        g_option_context_free(g_cmdContext);
    }

    // end of the app
    exit(a_errorCode);
}

/// @brief what can I say? This is the main function!
int main(int argc, char **argv)
{
    GError *error = NULL;

    // i18n initialisation
    // make sure first that the message catalog can be found
    if (bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR) != NULL)
    {
        // UTF-8 chosen as codeset
        bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
        // initialise the message catalog
        textdomain (GETTEXT_PACKAGE);
    }
    else
    {
        std::cerr << "Error while setting the locale" << std::endl;
    }

    // create new command line context. This resource must be deleted before existing the app
    // i18n TRANSLATORS: Please leave the starting dash as it is part of the glib command line
    // i18n parsing formatting
    // i18n Thank you for contributing to this project
    g_cmdContext = g_option_context_new (_("- The GNU polyominoes board game"));

    // to diable i18n write NULL instead of GETTEXT_PACKAGE in the 3rd parameter
    //     g_option_context_add_main_entries (cmdContext, g_cmdEntries, NULL);
    g_option_context_add_main_entries (g_cmdContext, g_cmdEntries, GETTEXT_PACKAGE);

    // option group
    GOptionGroup* grp = gtk_get_option_group (TRUE);

    // http://library.gnome.org/devel/glib/unstable/glib-Commandline-option-parser.html#g-option-context-add-group
    g_option_context_add_group (g_cmdContext, grp);

    // http://library.gnome.org/devel/glib/unstable/glib-Commandline-option-parser.html#g-option-group-set-translation-domain
    g_option_group_set_translation_domain (grp, GETTEXT_PACKAGE);
    if (!g_option_context_parse (g_cmdContext, &argc, &argv, &error))
    {
        std::stringstream errMessage;
        errMessage << _("Error parsing command line")
                   << ": "
                   << _(error->message);

        FatalError(argv[0], errMessage.str().c_str(), COMMAND_LINE_PARSING_ERR);
    }

    if (g_version) // "version" option takes priority
    {
        std::cout << "Blockem, version " << VERSION << " (r" << SVN_REVISION << ")" << std::endl;
        std::cout << "  Compiled " << COMPILETIME << std::endl << std::endl;
        std::cout << "Copyright (C) 2009-2010 Faustino Frechilla" << std::endl;
        std::cout << "Blockem is open source software, see http://blockem.sourceforge.net/"
                  << std::endl << std::endl;
    }
    else if (g_mode == 0)
    {
        //////////////////
        // GUI MODE

        // gtkmm can do strange stuff if its internals are not initialised early enough
        Gtk::Main::init_gtkmm_internals();

        // g_thread_supported returns TRUE if the thread system is initialised,
        // and FALSE if it is not. Initiliase gthreads only if they haven't been
        // initialised already. Since glib 2.24.0 (from its changelog):
        // "the requirements for g_thread_init() have been relaxed slightly, it
        // can be called multiple times, and does not have to be the first call.
        // GObject now links to GThread and threads are enabled automatically
        // when g_type_init() is called"
        // for backwards compatibility call to g_thread_init only if it hasn't been
        // called already
        if(!g_thread_supported())
        {
            // Initialise gthreads even before gtk_init
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

        // this should be called before starting manipulating GUI stuff
        Gtk::Main kit(argc, argv);

        // create a new gtk builder. add_from_string will load the definitions
        Glib::RefPtr<Gtk::Builder> gtkBuilder = Gtk::Builder::create();

#ifdef GLIBMM_EXCEPTIONS_ENABLED
        try
        {
#endif // GLIBMM_EXCEPTIONS_ENABLED
            // Load the GUI from the xml code embedded in the executable file
            // see src/gui_glade.h for more information
            if (!gtkBuilder->add_from_string(
                    reinterpret_cast<const char *>(__BIN_GUI_GLADE_START__),
                    reinterpret_cast<long int>(__BIN_GUI_GLADE_SIZE__)))
            {
                FatalError(
                    argv[0],
                    // i18n TRANSLATORS: Please leave the word Gtkbuilder as it is since it's the name
                    // i18n of the component that failed
                    // i18n Thank you for contributing to this project
                    _("Could not load Gtkbuilder definitions"),
                    GUI_EXCEPTION_ERR);
            }

#ifdef GLIBMM_EXCEPTIONS_ENABLED
        }
        catch(const Glib::MarkupError& ex)
        {
            FatalError(argv[0], ex.what().c_str(), GUI_EXCEPTION_ERR);
        }
        catch(const Gtk::BuilderError& ex)
        {
            FatalError(argv[0], ex.what().c_str(), GUI_EXCEPTION_ERR);
        }
#endif // GLIBMM_EXCEPTIONS_ENABLED

        MainWindow *pMainWindow = NULL;
        try
        {
            // initialise singletons before extra threads are created.
            // Singleton creation is not thread safe
            Game1v1Config::Instance();

            // first of all retrieve the Gtk::window object
            gtkBuilder->get_widget_derived(GUI_MAIN_WINDOW_NAME, pMainWindow);
            if (pMainWindow == NULL)
            {
                throw new GUIException(std::string("couldn't retrieve the MainWindow from the .glade file"));
            }

            // if gdk_threads_enter and gdk_threads_leave were to be used
            // the Gtk::Main::run loop should be surrounded by
            // gdk_threads_enter and gdk_threads_leave
            // http://tadeboro.blogspot.com/2009/06/multi-threaded-gtk-applications.html

            kit.run(*pMainWindow);
        }
        catch (GUIException ex)
        {
            FatalError(argv[0], ex.what(), GUI_EXCEPTION_ERR);
        }

        if (pMainWindow)
        {
            // valgrind won't be happy if all this memory is not freed
            delete (pMainWindow);
        }

        // GUI MODE
        //////////////////
    }
    else // (g_mode != 0)
    {
        //////////////////
        // CONSOLE mode

        // do whatever we are requested to do per file passed as parameter to the
        // application. App will finish without running any GUI code

        if (g_mode == 1)
        {
            // 1 player total allocation (--mode=1)

            if ( (g_rows == GOPTION_INT_NOT_SET)        ||
                 (g_columns == GOPTION_INT_NOT_SET)     ||
                 (g_startingRow == GOPTION_INT_NOT_SET) ||
                 (g_startingColumn == GOPTION_INT_NOT_SET) )
            {
                FatalError(
                    argv[0],
                    "\"--rows\", \"--columns\", \"--starting-row\" and \"--starting-column\" must be specified in mode '1'",
                    TOTAL_ALLOC_BAD_OPTIONS_ERR);
            }

            if ( (g_rows <= 0) || (g_columns <= 0) )
            {
                FatalError(
                    argv[0],
                    "\"--rows\" and \"--columns\" must be a greater than 1",
                    TOTAL_ALLOC_BAD_OPTIONS_ERR);
            }

            if ( (g_startingRow >= g_rows) ||
                 (g_startingColumn >= g_columns) )
            {
                FatalError(
                    argv[0],
                    "App was told to start outside of the board",
                    TOTAL_ALLOC_BAD_OPTIONS_ERR);
            }

            GameTotalAllocation theGame(g_rows, g_columns);

            Coordinate startingCoord(g_startingRow, g_startingColumn);
            if (theGame.Solve(startingCoord))
            {
                std::cout << std::endl;
                std::cout << "SOLVED!" << std::endl;

                // print solved board on the screen
                theGame.GetBoard().PrintBoard(std::cout);
            }
            else
            {
                std::cout << std::endl;
                std::cout << "Could not allocate all the pieces in this "
                          << g_rows << "x" << g_columns << " board starting from row "
                          << g_startingRow << ", column " << g_startingColumn << std::endl;
            }
        }
        else if (g_mode == 2)
        {
            // 1vs1 Game (--mode=2)

            if ( (g_blockemfilePath == NULL) || (g_blockemfilePath[0] == NULL) )
            {
                FatalError(
                    argv[0],
                    "At least one file with a 1vs1Game saved must be specified",
                    GAME1V1_BAD_OPTIONS_ERR);
            }

            if ( (g_heuristic <  Heuristic::e_heuristicStartCount) ||
                 (g_heuristic >= Heuristic::e_heuristicCount) )
            {
                std::stringstream errMessage;
                errMessage << "Chosen heuristic type (" << g_heuristic << ") is invalid";

                FatalError(
                    argv[0],
                    errMessage.str().c_str(),
                    GAME1V1_BAD_OPTIONS_ERR);
            }

            // this little piece of magic uses the g_heuristic index to pick the expected evaluation
            // function. Have a look at heuristic.h
            Heuristic::EvalFunction_t heuristic = Heuristic::m_heuristicData[g_heuristic].m_evalFunction;
            std::cout << "Evaluation function set to \""
                      << Heuristic::m_heuristicData[g_heuristic].m_name
                      << "\" (" << g_heuristic << ")" << std::endl;

            if (g_depth <= 0)
            {
                FatalError(
                    argv[0],
                    "Depth in 1vs1Game mode must be set to a positive value",
                    GAME1V1_BAD_OPTIONS_ERR);
            }
            else if ( (g_depth & 0x01) == 0)
            {
                std::cerr << argv[0] << " Warning: For better results you might want to set the depth to an odd number" << std::endl;
            }

            // go through all the filenames array. Each file will be loaded into a gam1v1 and the next
            // move will be calculated and printed in the screen
            Game1v1 theGame;
            for (int32_t fileIndex = 0; g_blockemfilePath[fileIndex] != NULL; fileIndex++)
            {
                if (!g_file_test(g_blockemfilePath[fileIndex], G_FILE_TEST_IS_REGULAR))
                {
                    std::cerr << argv[0] << ": Error: " << g_blockemfilePath[fileIndex] << " doesn't exist. Trying next one..." << std::endl;
                    continue;
                }

                std::cout << "Loading game from " << g_blockemfilePath[fileIndex]
                          << " and calculating next move (search tree depth is "
                          << g_depth << ")" << std::endl;

                std::ifstream cin;
                cin.open(g_blockemfilePath[fileIndex]);
                if(!cin)
                {
                    std::cerr << argv[0] << ": Error: "
                              << g_blockemfilePath[fileIndex]
                              << " could not be opened. Trying next one..." << std::endl;
                    continue;
                }

                theGame.Reset();
                if (theGame.LoadGame(cin) == false)
                {
                    std::cerr << argv[0] << ": Error while loading the game from "
                              << g_blockemfilePath[fileIndex]
                              << ". Trying next one..." << std::endl;
                    cin.close();
                    continue;
                }
                cin.close();

                // print current game on the screen
                theGame.SaveGame(std::cout);

                //test(&theGame, theGame.GetMe(), theGame.GetOpponent());
                //exit(0);

                //std::cout << std::endl << std::endl;
                //theGame.GetMe().PrintNucleationPoints(std::cout);

                //std::cout << std::endl << std::endl;
                //theGame.GetOpponent().PrintNucleationPoints(std::cout);

                Piece resultPiece(e_noPiece);
                Coordinate resultCoord;

                // it is dummy because no one is expected to change it
                volatile sig_atomic_t dummyAtomic = 0;
                int32_t minimaxWinner =
                    theGame.MinMax(
                            heuristic,
                            g_depth,
                            Game1v1::e_Game1v1Player1,
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
                    // out down the result piece on the board
                    theGame.PutDownPiece(
                            resultPiece,
                            resultCoord,
                            Game1v1::e_Game1v1Player1);

                    // print the final game on the screen
                    theGame.SaveGame(std::cout);
                }

                //theGame.GetMe().PrintNucleationPoints(std::cout);
                //theGame.GetOpponent().PrintNucleationPoints(std::cout);

            } // for (int32_t fileIndex = 0; g_blockemfilePath[fileIndex] != NULL; fileIndex++)
        }
        else // (g_mode != 1 && g_mode != 2)
        {
            std::stringstream errMessage;
            errMessage << "Bad --mode option (" << g_mode << ")";

            FatalError(
                argv[0],
                errMessage.str().c_str(),
                BAD_MODE_OPTION_ERR);
        }

        // CONSOLE mode
        //////////////////
    }

    // free command line parsing resources
    g_option_context_free(g_cmdContext);

    return 0;
}
