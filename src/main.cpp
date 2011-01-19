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
///           Faustino Frechilla 23-Sep-2010  Reorganisation. Blockem Options support
/// @endhistory
///
// ============================================================================

#include <stdio.h>
#include <cstdio>    // printf (needed for better i18n)
#include <stdlib.h>
#include <math.h>
#include <string.h>  // strcmp
#include <iostream>
#include <fstream>
#include <glib.h>    // glib-Commandline-option-parser
#include <gtkmm.h>
#include "config.h"  // autotools header file
#include "gettext.h" // i18n
#include "assert.h"

// header file created during the make process which saves the current date
#include "compiletime.h"
#include "blockem_config.h"
#include "game1v1.h"                // 1vs1 and total allocation games are
#include "game_total_allocation.h"  // available from terminal
#include "heuristic.h"
#include "gui/glade_defs.h"
#include "gui/main_window.h"
#include "gui/game_1v1_config.h"               // initialise singleton
#include "gui/game_4players_config.h"         // initialise singleton
#include "gui/game_total_allocation_config.h" // initialise singleton
#include "gui/game_challenge_config.h"        // initialise singleton


// the following code is needed to load the gui into the application.
// (See big comment in src/gui/glade_defs.h)
// objdump in windows and linux is different and they create
// the following variables different from one another.
// Windows hasn't got any underscore at the start, while the object in linux
// has it. This is enough to check whether we are in the windows environment
// or not
#ifdef WIN32
extern const char binary_gui_gui_glade_start[];
extern const char binary_gui_gui_glade_end[];
extern int        binary_gui_gui_glade_size[];

#define __BIN_GUI_GLADE_START__ binary_gui_gui_glade_start
#define __BIN_GUI_GLADE_END__   binary_gui_gui_glade_end
#define __BIN_GUI_GLADE_SIZE__  binary_gui_gui_glade_size

#else
extern const char _binary_gui_gui_glade_start[];
extern const char _binary_gui_gui_glade_end[];
extern int        _binary_gui_gui_glade_size[];

#define __BIN_GUI_GLADE_START__ _binary_gui_gui_glade_start
#define __BIN_GUI_GLADE_END__   _binary_gui_gui_glade_end
#define __BIN_GUI_GLADE_SIZE__  _binary_gui_gui_glade_size

#endif // ifdef WIN32


// list of error codes returned to the OS
static const int COMMAND_LINE_PARSING_ERR    = 1;
static const int TOTAL_ALLOC_BAD_OPTIONS_ERR = 3;
static const int GAME1V1_BAD_OPTIONS_ERR     = 7;
static const int BAD_MODE_OPTION_ERR         = 15;
static const int GUI_EXCEPTION_ERR           = 127;

/// maximum size of error strings
static const int32_t ERROR_STRING_BUFFER_SIZE = 128;

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
      N_("Prints current version of the software and exists"),
      NULL },

    { "mode", 'm', 0, G_OPTION_ARG_INT, &g_mode,
      N_("Specifies the mode blockem runs. Valid options are: "
      "'0' GUI is shown (Default); '1' one player total-allocation; "
      "'2' 1vs1 Game"),
      "M" },

    { "rows", 'r', 0, G_OPTION_ARG_INT, &g_rows,
      N_("1 player total allocation game's board will have N rows. "
      "This is a MANDATORY parameter for --mode=1"),
      "N"},

    { "columns", 'c', 0, G_OPTION_ARG_INT, &g_columns,
      N_("1 player total allocation game's board will have N columns. "
      "This is a MANDATORY parameter for --mode=1"),
      "N"},

    { "starting-column", 'x', 0, G_OPTION_ARG_INT, &g_startingColumn,
      N_("Computer will start to allocate pieces in column X (when running with --mode=1). "
      "1st valid column is 0, so maximum allowed column will be (number_of_columns - 1). "
      "This is an optional parameter for --mode=1 but if it is set --starting-row must be set too"),
      "X"},

    { "starting-row", 'y', 0, G_OPTION_ARG_INT, &g_startingRow,
      N_("Computer will start to allocate pieces in row Y (when running with --mode=1). "
      "1st valid row is 0, so maximum allowed row will be (number_of_rows - 1). "
      "This is an optional parameter for --mode=1 but if it is set --starting-column must be set too"),
      "Y"},

    { "depth"  , 'd', 0, G_OPTION_ARG_INT, &g_depth,
      N_("Sets the maximum depth of search tree to D when 1vs1 Game is selected. "
      "This is a MANDATORY parameter for --mode=2"),
      "D"},

    { "heuristic", 'i', 0, G_OPTION_ARG_INT, &g_heuristic,
      N_("Heuristic to be used when mode is set to 1v1 game (--mode=2). Valid options: "
      "'0' \"Influence Area\" (Default); '1' \"Mr. Eastwood\"; '2' \"NK weighted\"; "
      "'3' \"Centre focused\"; '4' \"Simple\"; '5' \"Random\""),
      "H" },

    { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &g_blockemfilePath,
      N_("Paths to 1vs1game files (mode 2). Blockem will calculate next move per each one of them "
      "and print out the result in console. "
      "Specifying at least 1 file is MANDATORY for --mode=2"),
      // i18n TRANSLATORS: This string is supposed to mean that one or more
      // i18n files might be specified
      // i18n Thank you for contributing to this project
      N_("[FILE(S)]")},

    { NULL }
};

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
              << ": "
              // i18n TRANSLATORS:
              // i18n Bear in mind this string will be used to print out a help message when an error is found
              // i18n A typical example would be: "./blockem: Try './blockem --help'". That is the "try" word
              // i18n that is to be translated here
              // i18n Thank you for contributing to this project
              << _("Try")
              << " '"
              << a_binName
              <<  " --help'"
              << std::endl;

    // end of the app
    exit(a_errorCode);
}

/// @brief enable internationalisation support
/// It has to do some magic for i18n to work on win32 platforms
void I18nInit()
{
    //////////////////
    // gettext internationalisation initialisation

    // Prevents gtk_init(), gtk_init_check(), gtk_init_with_args() and
    // gtk_parse_args() from automatically calling setlocale (LC_ALL, "").
    // You would want to use this function if you wanted to set the locale
    // for your program to something other than the user's locale, or if you
    // wanted to set different values for different locale categories
    gtk_disable_setlocale();

#ifdef WIN32
    // TODO language should be picked through some menu on the main window
    // at the moment is just read from configuration file (BlockemConfig)

    // http://www.mail-archive.com/gtk-app-devel-list@gnome.org/msg11457.html
    // Language selection under windows (environment variables are a bit tricky
    // in that platform). putenv is used to set the environment variables
    // that are easily set in *nix systems through command line
    // if this environment variable is not set language will be retrieved
    // from whatever is configured as current language in the windows platform

    // An example of a call to putenv to set the LANG environment variable to
    //   + to english UK (en_UK):    putenv ("LANG=en_UK");
    //   + to spanish Spain (es_ES): putenv ("LANG=es_ES");
    std::string langEnvironmentStr(
            std::string("LANG=") + BlockemConfig::Instance().GetLanguageISO());

#ifdef DEBUG_PRINT
    std::cout << "Setting environment for i18n: \"" << langEnvironmentStr << "\"" << std::endl;
#endif

    putenv (langEnvironmentStr.c_str());

#endif // WIN32


    // this call must be done whatever the platform it is
    setlocale (LC_ALL, "");


#ifdef WIN32
    // win32 is a bit special again in the call to bindtextdomain
    // we cannot use LOCALEDIR because the language files will be
    // located in path-to-binary/share/locale. LOCALEDIR will be
    // calculated at running time based on the current directory

    // _pgmptr: full path to the executable file on win32
    // http://msdn.microsoft.com/en-us/library/tza1y5f7%28VS.80%29.aspx
    extern char* _pgmptr;
    std::string fullPathToBin(_pgmptr);

    // locale dir MUST be currentdir/share/locale for full translation support
    // in windows. I learnt this by the old test-error method, and it took a good
    // while
    std::string fullPathToLocaleDir =
        fullPathToBin.substr(0, fullPathToBin.find_last_of("/\\")).append("\\share\\locale");

    // make sure first that the message catalog can be found
    bindtextdomain (GETTEXT_PACKAGE, fullPathToLocaleDir.c_str());
    bindtextdomain ("gtk+", fullPathToLocaleDir.c_str());

#else
    // in any other platform we will be using LOCALEDIR which is defined
    // at compile time (through configure script)

    // make sure first that the message catalog can be found
    bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);

#endif // WIN32

    // UTF-8 chosen as codeset
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    // initialise the message catalog
    textdomain (GETTEXT_PACKAGE);

    // gettext internationalisation initialisation
    //////////////////
}

/// @brief load command line options and and parse command input
/// It uses i18n to translate command line options into the selected language
/// WARNING: I18nInit() MUST be called before to enable i18n in all platforms
/// (this function does some magic to ensure i18n is enabled under win32
/// platforms)
/// It might cause the application to exit if the command line options couldn't
/// be parsed (it calls to FatalError with the proper error code and message)
/// @param main's argc
/// @param main's argv
void ProcessCommandLine(int argc, char **argv)
{
    GError* error = NULL;
    GOptionContext* cmdContext = NULL;

    // create new command line context. This resource must be deleted before existing the app
    // i18n TRANSLATORS: Please leave the starting dash as it is part of the glib command line
    // i18n parsing formatting
    // i18n Thank you for contributing to this project
    cmdContext = g_option_context_new (_("- The GNU polyominoes board game"));

#ifdef WIN32
    // in win32 systems gettext fails when the string is static and marked as
    // translatable with N_() but _() is never called explicitely. Basically
    // there are 2 kinds of strings that are not translated:
    //  + Those included in the GOptionEntry list, which show the available
    //    options that can be passed to the program through command line
    //  + Strings included in the .glade file that never change during the
    //    execution of the application, for example a menu called "Game", or a
    //    label that contains the word "rotate"
    //
    // We'll be calling explicitely here to _() per each translatable element
    // of g_cmdEntries, so they get properly translated into the current domain

    // g_cmdEntries is a null terminated array of structs
    // iterate through to call _() explicitely
    GOptionEntry* optEntryIterator = g_cmdEntries;
    while (*(reinterpret_cast<char*>(optEntryIterator)) != '\0')
    {
        optEntryIterator->description = _(optEntryIterator->description);
        optEntryIterator->arg_description = _(optEntryIterator->arg_description);

        optEntryIterator++;
    }

#endif // WIN32

    // to disable i18n write NULL instead of GETTEXT_PACKAGE in the 3rd parameter
    //     g_option_context_add_main_entries (cmdContext, g_cmdEntries, NULL);
    g_option_context_add_main_entries (
        cmdContext,
        g_cmdEntries,
        GETTEXT_PACKAGE);

    // no need for this. It is already set in g_option_context_add_main_entries
    // http://library.gnome.org/devel/glib/stable/glib-Commandline-option-parser.html#g-option-context-set-translation-domain
    //g_option_context_set_translation_domain (cmdContext, GETTEXT_PACKAGE);

    // http://library.gnome.org/devel/glib/unstable/glib-Commandline-option-parser.html#g-option-context-add-group
    // note that the group will be freed together with the context when
    // g_option_context_free() is called, so you must not free the group
    // yourself after adding it to a context
    g_option_context_add_group (cmdContext, gtk_get_option_group (TRUE));

    // http://library.gnome.org/devel/glib/unstable/glib-Commandline-option-parser.html#g-option-group-set-translation-domain
    //g_option_group_set_translation_domain (grp, GETTEXT_PACKAGE);
    if (!g_option_context_parse (cmdContext, &argc, &argv, &error))
    {
        std::stringstream errMessage;
        errMessage << _("Error parsing command line")
                   << ": "
                   << error->message;
        g_error_free(error);

        FatalError(argv[0], errMessage.str().c_str(), COMMAND_LINE_PARSING_ERR);
    }

    // free command line parsing resources
    g_option_context_free(cmdContext);
}

/// @brief what can I say? This is the main function!
int main(int argc, char **argv)
{
    // fancy error messages creation with snprintf
    char errorStringBuffer[ERROR_STRING_BUFFER_SIZE];

    // Init type system as soon as possible
    g_type_init ();

    // SINGLETON creation is not thread safe. Ensure they are instantiated
    // before the app creates extra threads
    // BlockemConfig singleton contains general purpose configuration data that
    // will be loaded from the config xml file
    // It MUST be instantiated before i18n in windows because it loads the
    // preferred language
    BlockemConfig::Instance();

    // enable internationalisation support. It MUST be called before
    // ProcessCommandLine
    I18nInit();

    // parse command line options. options will be saved in a bunch of static
    // global variables (scope to this file only) defined at the beginning of
    // this file (they all start with 'g_')
    ProcessCommandLine(argc, argv);

    if (g_version)
    {
        // "version" option takes priority. If it set nothing else will be done
        std::cout << "Blockem, version " << VERSION << " (r" << SVN_REVISION << ")" << std::endl;
        std::cout << "  Compiled " << COMPILETIME << std::endl << std::endl;
        std::cout << "Copyright (C) 2009-2011 Faustino Frechilla" << std::endl;
        std::cout << "Blockem is open source software, see http://blockem.sourceforge.net/"
                  << std::endl << std::endl;
    }
    else if (g_mode == 0)
    {
        //////////////////
        // GUI MODE

        // gtkmm can do strange stuff if its internals are not initialised early
        // enough
        Gtk::Main::init_gtkmm_internals();

        // initialise singletons before extra threads are created.
        // Singleton creation is not thread safe
        // Game1v1Config data. Used to store GUI related configuration for 1vs1 games
        Game1v1Config::Instance();
        Game4PlayersConfig::Instance();
        GameTotalAllocationConfig::Instance();
        GameChallengeConfig::Instance();

        // g_thread_supported returns TRUE if the thread system is initialised,
        // and FALSE if it is not. Initiliase gthreads only if they haven't been
        // initialised already. Since glib 2.24.0 (from its changelog):
        // "the requirements for g_thread_init() have been relaxed slightly, it
        // can be called multiple times, and does not have to be the first call.
        // GObject now links to GThread and threads are enabled automatically
        // when g_type_init() is called"
        // for backwards compatibility call to g_thread_init only if it hasn't
        // been called already
        if(!g_thread_supported())
        {
            // Initialise gthreads even before gtk_init
            g_thread_init(NULL);
        }

        // This call is needed only if extra threads (not the main thread)
        // updates directly the GUI widgets using gdk_threads_enter
        // and gdk_threads_leave
        // It's not really needed since there's only 1 extra thread
        // (apart from the main one) and it doesn't update the GUI
        // straight away. It uses signals for that matter (see comment
        // in MainWindow::WorkerThread_computingFinished)
        //gdk_threads_init();

        // this should be called before starting manipulating GUI stuff
        Gtk::Main kit(argc, argv);

        // create a new gtk builder. add_from_string will load the definitions
        Glib::RefPtr<Gtk::Builder> gtkBuilder = Gtk::Builder::create();
        gtkBuilder->set_translation_domain(GETTEXT_PACKAGE);

        try
        {
            // Load the GUI from the xml code embedded in the executable file
            if (!gtkBuilder->add_from_string(
            // see src/gui_glade.h for more information
                    reinterpret_cast<const char *>(__BIN_GUI_GLADE_START__),
                    reinterpret_cast<gsize>(__BIN_GUI_GLADE_SIZE__)))
            {
                FatalError(
                    argv[0],
                    // i18n TRANSLATORS: Please leave the word Gtkbuilder as it is since it's the name
                    // i18n of the component that failed
                    // i18n Thank you for contributing to this project
                    _("Could not load Gtkbuilder definitions"),
                    GUI_EXCEPTION_ERR);
            }
        }
#ifdef GLIBMM_EXCEPTIONS_ENABLED
        catch(const Glib::MarkupError& ex)
        {
            FatalError(argv[0], ex.what().c_str(), GUI_EXCEPTION_ERR);
        }
        catch(const Gtk::BuilderError& ex)
        {
            FatalError(argv[0], ex.what().c_str(), GUI_EXCEPTION_ERR);
        }
#endif // GLIBMM_EXCEPTIONS_ENABLED
        catch (const GUIException& ex)
        {
            FatalError(argv[0], ex.what(), GUI_EXCEPTION_ERR);
        }

        MainWindow *pMainWindow = NULL;
        try
        {
            // first of all retrieve the Gtk::window object
            gtkBuilder->get_widget_derived(GUI_MAIN_WINDOW_NAME, pMainWindow);
            if (pMainWindow == NULL)
            {
                throw new GUIException(
                            e_GUIException_GTKBuilderErr,
                            __FILE__,
                            __LINE__);
            }

            // if gdk_threads_enter and gdk_threads_leave were to be used
            // the Gtk::Main::run loop should be surrounded by
            // gdk_threads_enter and gdk_threads_leave
            // http://tadeboro.blogspot.com/2009/06/multi-threaded-gtk-applications.html
            kit.run(*pMainWindow);
        }
        catch (const GUIException& ex)
        {
            FatalError(argv[0], ex.what(), GUI_EXCEPTION_ERR);
        }
        catch (const std::exception& ex)
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

            if ( (g_rows == GOPTION_INT_NOT_SET) ||
                 (g_columns == GOPTION_INT_NOT_SET) )
            {
                FatalError(
                    argv[0],
                    _("Number of rows and number of columns must be specified in mode '1'"),
                    TOTAL_ALLOC_BAD_OPTIONS_ERR);
            }

            if ( (g_rows <= 0) || (g_columns <= 0) )
            {
                FatalError(
                    argv[0],
                    _("Number of rows and number of columns must be a greater than 1"),
                    TOTAL_ALLOC_BAD_OPTIONS_ERR);
            }

            if ( (g_startingRow != g_startingColumn) &&
                 ( (g_startingRow == GOPTION_INT_NOT_SET) ||
                   (g_startingColumn == GOPTION_INT_NOT_SET) ) )
            {
                // only one of the 2 options starting row and starting columns was set
                FatalError(
                    argv[0],
                    _("Both starting row and column must be set to configure a starting coordinate"),
                    TOTAL_ALLOC_BAD_OPTIONS_ERR);
            }

            Coordinate startingCoord;
            if ( (g_startingRow != GOPTION_INT_NOT_SET) &&
                 (g_startingColumn != GOPTION_INT_NOT_SET) )
            {
                if ( (g_startingRow >= g_rows) || (g_startingRow < 0) ||
                     (g_startingColumn >= g_columns) || (g_startingColumn < 0) )
                {
                    FatalError(
                        argv[0],
                        _("Application cannot start outside the board boundaries"),
                        TOTAL_ALLOC_BAD_OPTIONS_ERR);
                }

                // overwrite uninitialised starting coordinate with the
                // user-selected one
                startingCoord.m_row = g_startingRow;
                startingCoord.m_col = g_startingColumn;
            }

            GameTotalAllocation theGame(g_rows, g_columns, startingCoord);
            if (theGame.Solve())
            {
                std::cout << std::endl;
                // i18n TRANSLATORS: This string is to be printed when the applications finds
                // i18n a solution to a one-player total-allocation game
                // i18n Thank you for contributing to this project
                std::cout << _("SOLVED!") << std::endl;

                // print solved board on the screen
                theGame.GetBoard().PrintBoard(std::cout);
            }
            else
            {
                std::cout << std::endl;
                if (startingCoord.Initialised())
                {
                    // i18n TRANSLATORS: This string is to be printed when the applications cannot find
                    // i18n a solution to a one-player total-allocation game. The integers printed on the screen
                    // i18n are (respectively) number of rows, number of columns, starting row and starting column
                    // i18n A typical string to be printed:
                    // i18n "Could not allocate all the pieces in this 8x8 board, starting from row 0, column 2"
                    // i18n Thank you for contributing to this project
                    printf(_("Could not allocate all the pieces in this %dx%d board, starting from row %d, columnd %d\n"),
                        g_rows,
                        g_columns,
                        g_startingRow,
                        g_startingColumn);
                }
                else
                {
                    // i18n TRANSLATORS: This string is to be printed when the applications cannot find
                    // i18n a solution to a one-player total-allocation game starting from anywhere in the board
                    // i18n Thank you for contributing to this project
                    printf(_("Could not allocate all the pieces in this %dx%d board starting from anywhere in the board\n"),
                        g_rows,
                        g_columns);
                }
            }
        }
        else if (g_mode == 2)
        {
            // 1vs1 Game (--mode=2)

            if ( (g_blockemfilePath == NULL) || (g_blockemfilePath[0] == NULL) )
            {
                FatalError(
                    argv[0],
                    _("At least one file with a 1vs1Game saved must be specified"),
                    GAME1V1_BAD_OPTIONS_ERR);
            }

            if ( (g_heuristic <  Heuristic::e_heuristicStartCount) ||
                 (g_heuristic >= Heuristic::e_heuristicCount) )
            {
                snprintf(errorStringBuffer,
                         ERROR_STRING_BUFFER_SIZE,
                         // i18n TRANSLATORS: Please, leave that %d as it is. It will be replaced
                         // i18n by the type of heuristic used by the user
                         // i18n Thank you for contributing to this project
                         _("Invalid heuristic type (%d)"),
                         g_heuristic);

                FatalError(
                    argv[0],
                    errorStringBuffer,
                    GAME1V1_BAD_OPTIONS_ERR);
            }

            // this little piece of magic uses the g_heuristic index to pick the expected evaluation
            // function. Have a look at heuristic.h
            Heuristic::EvalFunction_t heuristic = Heuristic::m_heuristicData[g_heuristic].m_evalFunction;
            // i18n TRANSLATORS: This string shows the chosen heuristic's name to the user
            // i18n the 1st %s will be replaced by the heuristic's name, and %d by its index
            // i18n Thank you for contributing to this project
            printf(_("Evaluation function set to %s (%d)\n"),
                   _(Heuristic::m_heuristicData[g_heuristic].m_name),
                   g_heuristic);

            if (g_depth <= 0)
            {
                FatalError(
                    argv[0],
                    _("Depth in 1vs1Game mode must be set to a positive value"),
                    GAME1V1_BAD_OPTIONS_ERR);
            }
            else if ( (g_depth & 0x01) == 0)
            {
                std::cerr << argv[0]
                          << ": "
                          << _("Warning: For better results you might want to set the depth to an odd number")
                          << std::endl;
            }

            // go through all the filenames array. Each file will be loaded into a gam1v1 and the next
            // move will be calculated and printed in the screen
            Game1v1 theGame;
            for (int32_t fileIndex = 0; g_blockemfilePath[fileIndex] != NULL; fileIndex++)
            {
                if (!g_file_test(g_blockemfilePath[fileIndex], G_FILE_TEST_IS_REGULAR))
                {
                    fprintf(stderr,
                            // i18n TRANSLATORS: This string is shown when a file specified by the user
                            // i18n cannot be loaded into the application. The 1st %s will be replaced
                            // i18n by the name of the binary (normally "./blockem"), the 2nd one by
                            // i18n the file that couldn't be accessed. Bear in mind the '\n' character should be there
                            // i18n in the translated version of the string too
                            // i18n Thank you for contributing to this project
                            _("%s: Error: '%s' doesn't exist. Trying next file...\n"),
                            argv[0],
                            g_blockemfilePath[fileIndex]);

                    continue;
                }

                std::ifstream cin;
                cin.open(g_blockemfilePath[fileIndex]);
                if(!cin)
                {
                    fprintf(stderr,
                            // i18n TRANSLATORS: This string is shown when a file specified by the user
                            // i18n could not be opened to be read. The 1st %s will be replaced
                            // i18n by the name of the binary (normally "./blockem"), the 2nd one by
                            // i18n the file that couldn't be opened. Bear in mind the '\n' character should be there
                            // i18n in the translated version of the string too
                            // i18n Thank you for contributing to this project
                            _("%s: Error: '%s' could not be opened. Trying next file...\n"),
                            argv[0],
                            g_blockemfilePath[fileIndex]);

                    continue;
                }

                theGame.Reset();
                if (theGame.LoadGame(cin) == false)
                {
                    fprintf(stderr,
                            // i18n TRANSLATORS: This string is shown when a file specified by the user
                            // i18n does not contain a valid 1vs1 game. The 1st %s will be replaced
                            // i18n by the name of the binary (normally "./blockem"), the 2nd one by
                            // i18n the file that couldn't be opened. Bear in mind the '\n' character should be there
                            // i18n in the translated version of the string too
                            // i18n Thank you for contributing to this project
                            _("%s: Error: '%s' does not contain a valid 1vs1Game. Trying next file...\n"),
                            argv[0],
                            g_blockemfilePath[fileIndex]);

                    cin.close();
                    continue;
                }
                cin.close();

                // i18n TRANSLATORS: '%s' will be replaced here by the path to the file
                // i18n successfully loaded. Bear in mind the '\n' character should be there
                // i18n in the translated version of the string too
                // i18n Thank you for contributing to this project
                printf (_("Game succesfully loaded from '%s'\n"),
                        g_blockemfilePath[fileIndex]);

                // print current game on the screen
                theGame.SaveGame(std::cout);

                //std::cout << std::endl << std::endl;
                //theGame.GetMe().PrintNucleationPoints(std::cout);

                //std::cout << std::endl << std::endl;
                //theGame.GetOpponent().PrintNucleationPoints(std::cout);

                // these 2 variables will save the result calculated by minimax
                Piece resultPiece(e_noPiece);
                Coordinate resultCoord;

                // dummy volatile because no one will change it
                volatile sig_atomic_t dummyAtomic = 0;

                int32_t minimaxWinner =
                    theGame.MinMax(
                            heuristic,
                            g_depth,
                            Game1v1::e_Game1v1Player1,
                            resultPiece,
                            resultCoord,
                            dummyAtomic);

                // i18n TRANSLATORS: '%d' will be replaced here by the value of the winning
                // i18n move calculated by the minimax engine. Bear in mind the '\n' character
                // i18n should be there in the translated version of the string too
                // i18n Thank you for contributing to this project
                printf(_("Winning evaluation function value: %d\n"), minimaxWinner);

                if (resultPiece.GetType() == e_noPiece)
                {
                    std::cout << std::endl;
                    // i18n TRANSLATORS: This string will be printed on the screen when the minimax
                    // i18n engine cannot put down a piece. It is left up to the translator to decide
                    // i18n to decide the formatting of the message, though this english version
                    // i18n could be used as the template
                    // i18n Thank you for contributing to this project
                    std::cout << _(" ===================================================\n"\
                                   " === END OF THE GAME. NO PIECE COULD BE PUT DOWN ===\n"\
                                   " ===================================================\n");
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
            snprintf(errorStringBuffer,
                     ERROR_STRING_BUFFER_SIZE,
                     // i18n TRANSLATORS: Please, leave that %d as it is. It will be replaced
                     // i18n by the mode picked by the user
                     // i18n Thank you for contributing to this project
                     _("Invalid --mode option (%d)"),
                     g_mode);

            FatalError(
                argv[0],
                errorStringBuffer,
                BAD_MODE_OPTION_ERR);
        }

        // CONSOLE mode
        //////////////////
    }

    return 0;
}
