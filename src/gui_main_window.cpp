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
/// @file gui_main_window.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Nov-2009  Original development
///           Faustino Frechilla 01-Apr-2010  Add drawing board with computer's pieces left
///           Faustino Frechilla 06-Apr-2010  StopWatch for human and computer
///           Faustino Frechilla 25-Apr-2010  libglade dependency removed. Code migrated to GtkBuilder
///           Faustino Frechilla 27-Apr-2010  2 human players allowed. Computer can start from both player1 and 2
///           Faustino Frechilla 21-Jul-2010  i18n
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#include <cstdio>    // printf/snprintf (needed for better i18n)
#endif
#include <queue>     // queue computer's moves
#include <iomanip>   // setw
#include "gettext.h" // i18n
#include "gui_main_window.h"
#include "gui_game1v1_config.h"
#include "gui_glade_defs.h"

/// message to be shown to the user when he/she requested the
/// application to be closed, and the worker thread is busy computing
static const char MESSAGE_ASK_BEFORE_CLOSE[] =
        N_("The game is computing the next move. Are you sure do you want to close the application?");

/// title of the new 1vs1 game dialog box
static const char MESSAGE_NEW_1V1GAME_DIALOG_TITLE[] = N_("New 1vs1 game");

/// title of the configure current game dialog box
static const char MESSAGE_CONFIGURE_GAME_DIALOG_TITLE[] = N_("Configure current game");

/// maximum size of the string to notify the end of the game
static const uint32_t GAME_FINISHED_BUFFER_LENGTH = 256;

/// maximum size of the string to apply to score labels
static const uint32_t SCORE_LABEL_BUFFER_LENGTH = 64;

/// how often stopwatches are updated
static const uint32_t STOPWATCH_UPDATE_PERIOD_MILLIS = 500; // 1000 = 1 second


/// static methods to communicate Game1v1 progress with MainWindow
/// this float is used to communicate the worker thread with the main thread
/// in a thread-safe manner. It's a bit dirty but it will do it for now
static float s_computingCurrentProgress = 0.0;
// http://tadeboro.blogspot.com/2009/06/multi-threaded-gtk-applications.html
/// static easy to use mutex for shared data across threads
G_LOCK_DEFINE_STATIC(s_computingCurrentProgress);

/// this is a pointer to the MainWindow itself so it can be used from the static method
/// MainWindow::ProgressUpdate. It is dirty, but it works (and it is enough for now)
/// WARNING: it won't work if MainWindow is instantiated twice
static MainWindow *g_pMainWindow = NULL;

void MainWindow::ProgressUpdate(float a_progress)
{
    // WARNING: this method is run by another thread.
    // once the m_signal_moveComputed signal is emited the
    // main thread will update the GUI widgets

    G_LOCK(s_computingCurrentProgress);
        s_computingCurrentProgress = a_progress;
    G_UNLOCK(s_computingCurrentProgress);

    if (g_pMainWindow)
    {
        g_pMainWindow->m_signal_computingProgressUpdated.emit();
    }
}

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException):
    Gtk::Window(cobject), //Calls the base class constructor
    m_currentGameFinished(false),
    m_currentMovingPlayer(Game1v1::e_Game1v1Player1),
    m_gtkBuilder(a_gtkBuilder),
    m_the1v1Game(
        Game1v1Config::Instance().GetPlayer1StartingCoord(),
        Game1v1Config::Instance().GetPlayer2StartingCoord()),
    m_lastCoord(COORD_UNINITIALISED, COORD_UNINITIALISED),
    m_workerThread(),
    m_aboutDialog(NULL),
    m_pickPiecesDrawingArea(
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1),
        DrawingAreaShowPieces::eOrientation_leftToRight),
    m_showOpponentPiecesDrawingArea(
        m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2),
        DrawingAreaShowPieces::eOrientation_bottomToTop),
    m_boardDrawingArea(m_the1v1Game.GetBoard()),
    m_editPieceTable(NULL),
    m_stopWatchLabelPlayer1(
        STOPWATCH_UPDATE_PERIOD_MILLIS),
    m_stopWatchLabelPlayer2(
        STOPWATCH_UPDATE_PERIOD_MILLIS)
{
    //TODO this is dirty (even though it works) the way MainWindow::ProgressUpdate
    // access the MainWindow Instance should be fixed in some way
#ifdef DEBUG
    // assert there's no more than 1 instance of MainWindow
    assert(g_pMainWindow == NULL);
#endif
    g_pMainWindow = this;

    // create and initialise the randomizer using now time as the seed
    GTimeVal timeNow;
    g_get_current_time(&timeNow);
    m_randomizer = g_rand_new_with_seed(static_cast<guint32>(timeNow.tv_sec ^ timeNow.tv_usec));

    // icon of the window
    Glib::RefPtr< Gdk::Pixbuf > icon;
    // gboolean g_file_test(const gchar *filename, GFileTest test);
    if (g_file_test(GUI_PATH_TO_16PICTURE_STR.c_str(), G_FILE_TEST_IS_REGULAR))
    {
        try
        {
            icon = Gdk::Pixbuf::create_from_file(GUI_PATH_TO_16PICTURE_STR.c_str());
        } catch(...)
        {
            icon.reset();

#ifdef DEBUG_PRINT
            std::cerr
               // i18n TRANSLATORS: Bear in mind this string will be printed followed by the path to the 16x16 icon file
               // i18n Thank you for contributing to this project
               << _("WARNING: Exception occurred when setting the 16x16 icon into the Main Window from ")
               << GUI_PATH_TO_16PICTURE_STR
               << std::endl;
#endif
        }

        if (icon)
        {
            this->set_icon(icon);
        }
    }

    // retrieve the config dialog. It must be retrieved calling get_widget_derived
    // otherwise app will core
    m_gtkBuilder->get_widget_derived(GUI_CONFIG_DIALOG_NAME, m_configDialog);
    if (m_configDialog == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // retrieve the about dialog. It must be retrieved calling get_widget_derived
    // otherwise app will core
    m_gtkBuilder->get_widget_derived(GUI_ABOUT_DIALOG_NAME, m_aboutDialog);
    if (m_aboutDialog == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // retrieve the editing piece table. It must be retrieved calling get_widget_derived
    // otherwise app will core
    m_gtkBuilder->get_widget_derived(GUI_TABLE_EDITING_PIECE_NAME, m_editPieceTable);
    if (m_editPieceTable == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // retrieve the rest of objects from the GUI design
    m_accelGroup = Glib::RefPtr<Gtk::AccelGroup>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_ACCELERATOR));
    if (!m_accelGroup)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_newMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_GAME_NEW));
    if (!m_newMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_quitMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_GAME_QUIT));
    if (!m_quitMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsNKPointsMenuItem = Glib::RefPtr<Gtk::CheckMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_NKPOINTS));
    if (!m_settingsNKPointsMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsForbiddenAreaPlayer1MenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_FORBIDDENAREA_PLAYER1));
    if (!m_settingsForbiddenAreaPlayer1MenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsForbiddenAreaPlayer2MenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_FORBIDDENAREA_PLAYER2));
    if (!m_settingsForbiddenAreaPlayer2MenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsForbiddenAreaNoShowMenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_FORBIDDENAREA_NOSHOW));
    if (!m_settingsForbiddenAreaNoShowMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsInfluenceAreaPlayer1MenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_INFLUENCEAREA_PLAYER1));
    if (!m_settingsInfluenceAreaPlayer1MenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsInfluenceAreaPlayer2MenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_INFLUENCEAREA_PLAYER2));
    if (!m_settingsInfluenceAreaPlayer2MenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsInfluenceAreaNoShowMenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_INFLUENCEAREA_NOSHOW));
    if (!m_settingsInfluenceAreaNoShowMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsPrefsMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_PREFS));
    if (!m_settingsPrefsMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_helpAboutMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_HELP_ABOUT));
    if (!m_helpAboutMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_VBOX_DRAWING_NAME, m_vBoxDrawing);
    if (m_vBoxDrawing == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_HBOX_GAME_STATUS_NAME, m_hBoxGameStatus);
    if (m_hBoxGameStatus == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_HBOX_OPPONENT_PIECES_NAME, m_hBoxOpponentPieces);
    if (m_hBoxOpponentPieces == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_HBOX_PIECES_AREA_NAME, m_hBoxEditPieces);
    if (m_hBoxEditPieces == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_HBOX_STATUSBAR_NAME, m_hBoxStatusBar);
    if (m_hBoxStatusBar == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // accelerators for main_window menu
    this->add_accel_group(m_accelGroup);

    // place the custom widgets where they are expected to be
    // pack_start (Widget& child, bool expand, bool fill, guint padding=0)
    // first the board drawing area to the game status hbox and display it
    m_hBoxGameStatus->pack_start(m_boardDrawingArea, true, true);
    m_hBoxGameStatus->reorder_child(m_boardDrawingArea, 0);
    m_boardDrawingArea.show();

    // pick pieces drawing area, edit pieces table and show opponent's pieces
    m_hBoxEditPieces->pack_start(m_pickPiecesDrawingArea, true, true);
    m_hBoxEditPieces->pack_start(*m_editPieceTable, false, false);

    m_hBoxOpponentPieces->pack_start(
            m_showOpponentPiecesDrawingArea,
            true,
            true);

    // if we don't show them, nobody will be able to see them
    // set_visible doesn't work in 2.16 (which is used in windows). use show!
    m_pickPiecesDrawingArea.show();
    m_showOpponentPiecesDrawingArea.show();

    // update the score shown in the status bar before setting them up as visible
    // so the widgets take their proper size at GUI startup
    UpdateScoreStatus();

    // retrieve the default colour from the config class to apply it to the players
    // and use HTML tags so the stop watch labels show each player's name written
    // with its corresponding colour. Both widgets will also get the proper size
    // at GUI startup since they have their expected size before being added to the
    // horizontal box "status bar"
    uint8_t red, green, blue;
    std::stringstream theMessage;
    Game1v1Config::Instance().GetPlayer1Colour(red, green, blue);
    m_the1v1Game.SetPlayerColour(Game1v1::e_Game1v1Player1, red, green, blue);
    theMessage << "<span color=\"#"
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(red)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(green)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(blue)
               << "\">"
               << m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1).GetName()
               << " </span>";
    m_stopWatchLabelPlayer1.SetPrefix(theMessage.str());

    Game1v1Config::Instance().GetPlayer2Colour(red, green, blue);
    m_the1v1Game.SetPlayerColour(Game1v1::e_Game1v1Player2, red, green, blue);
    theMessage.str(std::string());
    theMessage << "<span color=\"#"
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(red)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(green)
               << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(blue)
               << "\">"
               << m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2).GetName()
               << " </span>";
    m_stopWatchLabelPlayer2.SetPrefix(theMessage.str());

    // the custom status bar
    m_hBoxStatusBar->pack_start(m_player1ScoreLabel, true, true);
    m_hBoxStatusBar->pack_start(m_arrayStatusBarSeparator[0], false, true);
    m_hBoxStatusBar->pack_start(m_stopWatchLabelPlayer1, true, true);
    m_hBoxStatusBar->pack_start(m_arrayStatusBarSeparator[1], false, true);
    m_hBoxStatusBar->pack_start(m_player2ScoreLabel, true, true);
    m_hBoxStatusBar->pack_start(m_arrayStatusBarSeparator[2], false, true);
    m_hBoxStatusBar->pack_start(m_stopWatchLabelPlayer2, true, true);
    m_progressBar.set_orientation(Gtk::PROGRESS_LEFT_TO_RIGHT);
    m_progressBar.set_fraction(0.0);
    m_hBoxStatusBar->pack_start(m_progressBar, true, true);
    m_hBoxStatusBar->show_all();

    // progress handler for the computing process of the MinMax algorithm
    m_the1v1Game.SetProgressHandler(&MainWindow::ProgressUpdate);

    // connect the interthread communication (GLib::Dispatcher) to invalidate the
    // board drawing area
    m_signal_moveComputed.connect(
            sigc::mem_fun(*this, &MainWindow::NotifyMoveComputed));

    // connect the interthread communication to update the progress bar
    m_signal_computingProgressUpdated.connect(
            sigc::mem_fun(*this, &MainWindow::NotifyProgressUpdate));

    // connect the worker thread signal
    m_workerThread.signal_computingFinished().connect(
            sigc::mem_fun(*this, &MainWindow::WorkerThread_computingFinished));
            //sigc::ptr_fun(f) );

    // connect the signal coming from the pickPiecesDrawingArea to update TableEditPiece
    m_pickPiecesDrawingArea.signal_piecePicked().connect(
            sigc::mem_fun(*m_editPieceTable, &TableEditPiece::SetPiece));

    // connect the signal coming from the editing piece table to process the change in the
    // currently editing piece
    m_editPieceTable->signal_pieceChanged().connect(
            sigc::mem_fun(m_boardDrawingArea, &DrawingAreaBoard::SetCurrentPiece));

    // connect the signal coming fromt he board drawing area to process when the user clicks
    // on the board
    m_boardDrawingArea.signal_boardPicked().connect(
            sigc::mem_fun(*this, &MainWindow::BoardDrawingArea_BoardClicked));


    // connect the rest of the signals to the handlers
    // if the handler is not part of an object use sigc::ptr_fun
    this->signal_delete_event().connect(
            sigc::mem_fun(*this, &MainWindow::MainWindow_DeleteEvent));
    m_newMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemGameNew_Activate));
    m_quitMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemGameQuit_Activate));
    m_helpAboutMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemHelpAbout_Activate));
    m_settingsPrefsMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsPreferences_Activate));
    m_settingsNKPointsMenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsViewNKPoints_Toggled));
    m_settingsForbiddenAreaPlayer1MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowPlayer1ForbiddenArea_Toggled));
    m_settingsForbiddenAreaPlayer2MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowPlayer2ForbiddenArea_Toggled));
    m_settingsForbiddenAreaNoShowMenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowNoneForbiddenArea_Toggled));
    m_settingsInfluenceAreaPlayer1MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowPlayer1InfluenceArea_Toggled));
    m_settingsInfluenceAreaPlayer2MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowPlayer2InfluenceArea_Toggled));
    m_settingsInfluenceAreaNoShowMenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowNoneInfluenceArea_Toggled));

    // initialise the list of players of the board drawing area
    m_boardDrawingArea.AddPlayerToList(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    m_boardDrawingArea.AddPlayerToList(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2));

    // launch the game!!
    LaunchNewGame();
}

MainWindow::~MainWindow()
{
    //TODO this is dirty too. See comment at the beginning of the constructor
    // it might cause a lot of trouble if there are more than 1 instance of MainWindow
    g_pMainWindow = NULL;

    // delete the worker thread
    m_workerThread.Join();

    // delete the randomizer too
    g_rand_free(m_randomizer);

    // http://library.gnome.org/devel/gtkmm/stable/classGtk_1_1Builder.html#ab8c6679c1296d6c4d8590ef907de4d5a
    // Note that you are responsible for deleting top-level widgets (windows and dialogs) instantiated
    // by the Builder object. Other widgets are instantiated as managed so they will be deleted
    // automatically if you add them to a container widget
    delete(m_aboutDialog);
    delete(m_configDialog);
    //delete(m_editPieceTable);
}

bool MainWindow::MainWindow_DeleteEvent(GdkEventAny*)
{
    if (m_workerThread.IsThreadComputingMove())
    {
        Gtk::MessageDialog::MessageDialog exitingMessage(
                *this,
                _(MESSAGE_ASK_BEFORE_CLOSE),
                true,
                Gtk::MESSAGE_QUESTION,
                Gtk::BUTTONS_YES_NO,
                true);

        if (exitingMessage.run() == Gtk::RESPONSE_NO)
        {
            // cancel delete event
            return true;
        }
    }

    // cancel the worker thread
    m_workerThread.CancelComputing();

    // continue with delete event
    return false;
}

void MainWindow::MenuItemGameQuit_Activate()
{
    if (m_workerThread.IsThreadComputingMove())
    {
        Gtk::MessageDialog::MessageDialog exitingMessage(
                *this,
                _(MESSAGE_ASK_BEFORE_CLOSE),
                true,
                Gtk::MESSAGE_QUESTION,
                Gtk::BUTTONS_YES_NO,
                true);

        if (exitingMessage.run() == Gtk::RESPONSE_NO)
        {
            // cancel delete event
            return;
        }
    }

    m_workerThread.CancelComputing();

    // exit the app
    this->hide();
}

void MainWindow::MenuItemGameNew_Activate()
{
    m_configDialog->set_title(_(MESSAGE_NEW_1V1GAME_DIALOG_TITLE));
    // starting coords can always be edited when a new game is launched
    m_configDialog->SetStartingCoordEditionSensitive(true);

    Gtk::ResponseType result = static_cast<Gtk::ResponseType>(m_configDialog->run());
    if (result == Gtk::RESPONSE_OK)
    {
        // save configuration shown by the dialog into global config singleton
        m_configDialog->SaveCurrentConfigIntoGlobalSettings();

        // go for the brand new game!!
        LaunchNewGame();
    }
#if defined(DEBUG_PRINT) || defined (DEBUG)
    else if ( (result == Gtk::RESPONSE_CANCEL) || (result == Gtk::RESPONSE_DELETE_EVENT))
    {
        // config dialog cancelled
        ;
#ifdef DEBUG_PRINT
        std::cout << _("Configuration Dialog cancelled") << std::endl;
#endif // DEBUG_PRINT
    }
#ifdef DEBUG
    else
    {
        // unexpected
        assert(0);
    }
#endif // DEBUG
#endif // defined(DEBUG_PRINT) || defined (DEBUG)

    m_configDialog->hide();
}

void MainWindow::MenuItemSettingsViewNKPoints_Toggled()
{
    if (m_settingsNKPointsMenuItem->property_active())
    {
        m_boardDrawingArea.ShowNucleationPoints();
    }
    else
    {
        m_boardDrawingArea.HideNucleationPoints();
    }
}

void MainWindow::MenuItemSettingsShowPlayer1ForbiddenArea_Toggled()
{
    if (m_settingsForbiddenAreaPlayer1MenuItem->property_active())
    {
        m_boardDrawingArea.ShowPlayerForbiddenArea(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    }
}

void MainWindow::MenuItemSettingsShowPlayer2ForbiddenArea_Toggled()
{
    if (m_settingsForbiddenAreaPlayer2MenuItem->property_active())
    {
        m_boardDrawingArea.ShowPlayerForbiddenArea(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2));
    }
}

void MainWindow::MenuItemSettingsShowNoneForbiddenArea_Toggled()
{
    if (m_settingsForbiddenAreaNoShowMenuItem->property_active())
    {
        m_boardDrawingArea.HidePlayerForbiddenArea();
    }
}

void MainWindow::MenuItemSettingsShowPlayer1InfluenceArea_Toggled()
{
    if (m_settingsInfluenceAreaPlayer1MenuItem->property_active())
    {
        m_boardDrawingArea.ShowPlayerInfluenceArea(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    }
}

void MainWindow::MenuItemSettingsShowPlayer2InfluenceArea_Toggled()
{
    if (m_settingsInfluenceAreaPlayer2MenuItem->property_active())
    {
        m_boardDrawingArea.ShowPlayerInfluenceArea(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2));
    }
}

void MainWindow::MenuItemSettingsShowNoneInfluenceArea_Toggled()
{
    if (m_settingsInfluenceAreaNoShowMenuItem->property_active())
    {
        m_boardDrawingArea.HidePlayerInfluenceArea();
    }
}

void MainWindow::MenuItemSettingsPreferences_Activate()
{
    m_configDialog->set_title(_(MESSAGE_CONFIGURE_GAME_DIALOG_TITLE));

    //TODO starting coords cannot be edited through the configuration dialog yet
    m_configDialog->SetStartingCoordEditionSensitive(false);

    // show a message informing the user a move was cancelled?
    bool showInfoMessage = false;
    Gtk::ResponseType result;
    result = static_cast<Gtk::ResponseType>(m_configDialog->run());
    if (result == Gtk::RESPONSE_OK)
    {
        // if current player is a human being and it's been set to computer
        // next move will have to be requested to the worker thread.
        // if current player is the computer and its settings has been changed
        // current move will be cancelled and new settings will be applied
        // (both if player has been set up to be human or computer)

        if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1)     &&
               (Game1v1Config::Instance().IsPlayer1Computer() == false) &&
               (m_configDialog->IsPlayer1TypeComputer() == true)        )
             ||
             ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2)     &&
               (Game1v1Config::Instance().IsPlayer2Computer() == false) &&
               (m_configDialog->IsPlayer2TypeComputer() == true)        )
           )
        {
            // current player is not the computer and it has been
            // requested in the config dialog to change it to be the computer
            // save configuration shown by the dialog into global config singleton
            // and request the processing thread to compute next player1 move
            m_configDialog->SaveCurrentConfigIntoGlobalSettings();

            // next move was being calculated by a human being, but now it has
            // been swapped to a computer player. Humans are not allowed to edit
            // pieces while computer is thinking
            m_editPieceTable->set_sensitive(false);

            // requesting the thread!!
            RequestThreadToComputeNextMove(m_currentMovingPlayer, true);
        }
        else if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1)    &&
                    (Game1v1Config::Instance().IsPlayer1Computer() == true) )
                ||
                  ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2)    &&
                    (Game1v1Config::Instance().IsPlayer2Computer() == true) )
                )
        {
            if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1) &&
                   (m_configDialog->IsPlayer1TypeComputer() == false) )
                 ||
                 ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2) &&
                   (m_configDialog->IsPlayer2TypeComputer() == false) ) )
            {
                // current moving player is the computer and it has been set to be a human user
                // cancel current calculation and launch the new move

                // current move is going to be cancelled
                showInfoMessage = true;

                // cancel worker thread current computing process
                m_workerThread.CancelComputing();

                // empty out move queue
                while (!m_moveQueue.IsEmpty())
                {
                    CalculatedMove_t dummy;
                    m_moveQueue.Pop(dummy);
                }

                // save new settings into global config
                m_configDialog->SaveCurrentConfigIntoGlobalSettings();

                // restart the progress bar (computer is not thinking this move anymore)
                G_LOCK(s_computingCurrentProgress);
                    s_computingCurrentProgress = 0.0;
                G_UNLOCK(s_computingCurrentProgress);
                m_progressBar.set_fraction(0.0);

                // allow the new human user to put down pieces on the board
                m_editPieceTable->set_sensitive(true);

                // restore the mouse cursor so the human being who has to put down next piece
                // can do it
                ResetCursor();
            } // if (m_configDialog->IsPlayer1TypeComputer() == false)
            else if (
                  ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1)      &&
                    ( (Game1v1Config::Instance().GetMinimaxDepthPlayer1() !=
                          m_configDialog->GetPlayer1SearchTreeDepth()        )
                      ||
                      (Game1v1Config::Instance().GetHeuristicTypePlayer1() !=
                          m_configDialog->GetPlayer1Heuristic()              ) ) )
                  ||
                  ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2)      &&
                    ( (Game1v1Config::Instance().GetMinimaxDepthPlayer2() !=
                           m_configDialog->GetPlayer2SearchTreeDepth()        )
                       ||
                       (Game1v1Config::Instance().GetHeuristicTypePlayer2() !=
                           m_configDialog->GetPlayer2Heuristic()              ) ) ) )
            {
                // current player was the computer and it still is.
                // cancel current move process if there has been a change
                // in any of the rest of the settings (all but the
                // type of player: type of heuristic, depth of
                // search tree)

                // current move is going to be cancelled
                showInfoMessage = true;

                // cancel worker thread current computing process
                m_workerThread.CancelComputing();

                // empty out move queue
                while (!m_moveQueue.IsEmpty())
                {
                    CalculatedMove_t dummy;
                    m_moveQueue.Pop(dummy);
                }

                // restart the progress bar (computer is not thinking this move anymore)
                G_LOCK(s_computingCurrentProgress);
                    s_computingCurrentProgress = 0.0;
                G_UNLOCK(s_computingCurrentProgress);
                m_progressBar.set_fraction(0.0);

                // save current config for it to be applied in the future
                m_configDialog->SaveCurrentConfigIntoGlobalSettings();

                // requesting the thread!!
                RequestThreadToComputeNextMove(m_currentMovingPlayer, true);
            }
            else
            {
                // current player settings weren't modified. Save them
                // but do not cancel current calculation
                m_configDialog->SaveCurrentConfigIntoGlobalSettings();
            }
        }
        else
        {
            // changes to the config dialog don't affect current move
            // save configuration shown by the dialog into global config singleton
            // new configuratio will be applied once current move is calculated
            m_configDialog->SaveCurrentConfigIntoGlobalSettings();
        }
    } // if (result == Gtk::RESPONSE_OK)

    m_configDialog->hide();

    if (showInfoMessage)
    {
        // message for the user to inform his/her calculation was cancelled
        // after the config dialog was hidden
        Gtk::MessageDialog::MessageDialog infoMessage(
                *this,
                _("Previous move had to be cancelled before applying new settings"),
                true,
                Gtk::MESSAGE_INFO,
                Gtk::BUTTONS_OK,
                true);

        infoMessage.run(); // there will be only one type of value returned
    }
}

void MainWindow::MenuItemHelpAbout_Activate()
{
    m_aboutDialog->run();
    m_aboutDialog->hide();
}

void MainWindow::LaunchNewGame()
{
    if (m_workerThread.IsThreadComputingMove())
    {
        // stop worker thread. Current calculation won't be needed in the new game
        // this call blocks the current thread until the worker thread is ready to
        // calculate next move
        m_workerThread.CancelComputing();
    }

    // clear old moves from the move queue. They belong to the old game
    while (!m_moveQueue.IsEmpty())
    {
        CalculatedMove_t dummy;
        m_moveQueue.Pop(dummy);
    }

    // reset the current game
    m_the1v1Game.Reset(
        Game1v1Config::Instance().GetPlayer1StartingCoord(),
        Game1v1Config::Instance().GetPlayer2StartingCoord());

    // update the board view
    m_boardDrawingArea.CancelLatestPieceDeployedEffect();
    m_boardDrawingArea.Invalidate();
    // player1 will be picking next piece
    m_pickPiecesDrawingArea.Invalidate(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    // player2's pieces will be shown on the right side
    m_showOpponentPiecesDrawingArea.Invalidate(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2));

    // it will be player1's go next. set piece colour to player1's
    uint8_t red   = 0;
    uint8_t green = 0;
    uint8_t blue  = 0;
    m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1).GetColour(red, green, blue);
    m_editPieceTable->SetPieceRGB(
        static_cast<float>(red)   / 255,
        static_cast<float>(green) / 255,
        static_cast<float>(blue)  / 255);

    UpdateScoreStatus();

    // reset the cursor (even if it's been already done)
    ResetCursor();

    // stopwatches must be restarted.
    m_stopWatchLabelPlayer1.Reset();
    m_stopWatchLabelPlayer2.Reset();

    // restart the progress bar
    G_LOCK(s_computingCurrentProgress);
        s_computingCurrentProgress = 0.0;
    G_UNLOCK(s_computingCurrentProgress);
    m_progressBar.set_fraction(0.0);

    // Start player1's timer
    m_stopWatchLabelPlayer1.Continue();

    // reset and force redraw editPieceTable. It'l be set to sensitive
    // or unsensitive depending on the type of player1
    m_editPieceTable->SetPiece(e_noPiece);

    // new game just started. It can't be finished!
    m_currentGameFinished = false;
    // player1 is the one who's got to put a piece next
    m_currentMovingPlayer = Game1v1::e_Game1v1Player1;

    if (Game1v1Config::Instance().IsPlayer1Computer())
    {
        // next player is the computer. Disallow editing the edit piece
        // area while computer is processing next move
        m_editPieceTable->set_sensitive(false);

        RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player1, false);
    }
    else
    {
        // a human b eing is allowed to edit next piece
        m_editPieceTable->set_sensitive(true);

        // player1 is a human and he/she will put down the next piece (player1 goes always first)
        m_boardDrawingArea.SetCurrentPlayer(m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1));
    }
}

void MainWindow::RequestThreadToComputeNextMove(
        Game1v1::eGame1v1Player_t a_whoMoves,
        bool                      a_blockCall,
        const Coordinate         &a_coordinate,
        const Piece              &a_piece)
{
    // set the cursor to busy in the board drawing area, so the user
    // won't get (very) nervous while the computer is processing the next move
    SetWaitCursor();

    // retrieve eval function pointer and depth from global configuration
    Heuristic::eHeuristicType_t heuristicType = Heuristic::e_heuristicStartCount;
    int32_t searchTreeDepth = GAME1V1CONFIG_DEPTH_AUTOADJUST;
    switch(a_whoMoves)
    {
    case Game1v1::e_Game1v1Player1:
        heuristicType   = Game1v1Config::Instance().GetHeuristicTypePlayer1();
        searchTreeDepth = Game1v1Config::Instance().GetMinimaxDepthPlayer1();
        break;
    case Game1v1::e_Game1v1Player2:
        heuristicType   = Game1v1Config::Instance().GetHeuristicTypePlayer2();
        searchTreeDepth = Game1v1Config::Instance().GetMinimaxDepthPlayer2();
        break;
#ifdef DEBUG
    default:
        assert(0);
#endif // DEBUG
    } // switch(a_whoMoves)

    const Heuristic::sHeuristicData_t &heuristicData = Heuristic::m_heuristicData[heuristicType];
    const Player &currentPlayer = m_the1v1Game.GetPlayer(a_whoMoves);
    const Player &opponent = m_the1v1Game.GetOpponent(a_whoMoves);

    // search tree depth. If it is set to GAME1V1CONFIG_DEPTH_AUTOADJUST
    // its value will be set here to a sensible value
    if (searchTreeDepth == GAME1V1CONFIG_DEPTH_AUTOADJUST)
    {
        searchTreeDepth = 3;

        //TODO 14 is a magic number
        if ( (currentPlayer.NumberOfPiecesAvailable() < 14) &&
             (rules::CanPlayerGo(m_the1v1Game.GetBoard(), opponent) ) )
        {
            // if the opponent cannot go there is no point on setting the depth to 5.
            // 3 should be good enough in that case
            searchTreeDepth = 5;
        }
    }

    bool computeMoveSucceded;
    if ( (currentPlayer.NumberOfPiecesAvailable() == e_numberOfPieces) &&
         (g_rand_int_range(m_randomizer, 0, 2) == 0) )
    {
        // pass the move made by the opponent to the minmax algorithm half of the times
        // it will show a bit of randomness at the start to a potencial human user
        computeMoveSucceded = m_workerThread.ComputeMove(
                                    m_the1v1Game,
                                    a_whoMoves,
                                    heuristicData.m_evalFunction,
                                    searchTreeDepth,
                                    a_blockCall);
    }
    else
    {
        // use the latest coord and piece deployed by the opponent for half the starting moves
        // and for all the rest of the moves
        computeMoveSucceded = m_workerThread.ComputeMove(
                                    m_the1v1Game,
                                    a_whoMoves,
                                    heuristicData.m_evalFunction,
                                    searchTreeDepth,
                                    a_blockCall,
                                    a_coordinate,
                                    a_piece);
    }

    if (!computeMoveSucceded)
    {
#ifdef DEBUG_PRINT
        std::cout
            << _("Error while telling the thread to start computing. Worker thread is busy")
            << std::endl;
#endif
        std::stringstream theMessage;
        // i18n TRANSLATORS: Please, leave <b> and </b> tags sorrounding the translation of "Fatal error"
        // i18n Thank you for contributing to this project
        theMessage << _("<b>Fatal Error:</b> Could not communicate with worker thread. Application will exit now!");

        Gtk::MessageDialog::MessageDialog fatalErrorMessage(
            *this,
            theMessage.str().c_str(),
            true,
            Gtk::MESSAGE_ERROR,
            Gtk::BUTTONS_OK,
            true);

        if (fatalErrorMessage.run())
        {
            ; // the dialog only has 1 button
        }

        // kill the worker thread
        m_workerThread.CancelComputing();

        // exit the app
        this->hide();
    }
}

void MainWindow::WorkerThread_computingFinished(
        const Piece              &a_piece,
        const Coordinate         &a_coord,
        Game1v1::eGame1v1Player_t a_playerToMove,
        int32_t                   a_returnValue)
{
    // WARNING: this method is run by another thread.
    // once the m_signal_moveComputed signal is emited the
    // main thread will update the GUI widgets
    CalculatedMove_t thisMove;
    thisMove.piece = a_piece;
    thisMove.coord = a_coord;
    thisMove.playerToMove = a_playerToMove;

    // if the queue is full this call might block, but there aren't that many moves really...
    m_moveQueue.Push(thisMove);

#ifdef DEBUG_PRINT
    if (a_piece.GetType() == e_noPiece)
    {
        std::cout << _("Computer can't move") << std::endl;
    }
#endif

    // this signal is being issued from the MainWindowWorkerThread
    // GTK is not thread safe, so every GUI function should be called from the same thread
    // we need inter-thread communication to force the main thread to invalidate the
    // board drawing area
    // have a look at:
    //     http://library.gnome.org/devel/glibmm/stable/thread_2dispatcher_8cc-example.html
    m_signal_moveComputed.emit();
}

void MainWindow::BoardDrawingArea_BoardClicked(const Coordinate &a_coord, const Piece &a_piece, const Player &a_player)
{
	if (m_workerThread.IsThreadComputingMove())
	{
#ifdef DEBUG_PRINT
	    std::cout
            << _("Worker thread is busy. Please be patient while it is calculating next move")
            << std::endl;
#endif
	    return;
	}

    if( ( (a_player.NumberOfPiecesAvailable() == e_numberOfPieces) &&
          (!rules::IsPieceDeployableInStartingPoint(
                    m_the1v1Game.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_player.GetStartingCoordinate())) ) ||
        ( (a_player.NumberOfPiecesAvailable() < e_numberOfPieces) &&
          (!rules::IsPieceDeployableCompute(
                    m_the1v1Game.GetBoard(),
                    a_piece.GetCurrentConfiguration(),
                    a_coord,
                    a_player)) ) )
    {
#ifdef DEBUG_PRINT
        std::cout << _("Cheeky you! Don't try to deploy a piece where it's not allowed to")
                  << std::endl;
#endif
        return;
    }

    // don't save the move directly in the board. Use the same interface as the worker thread
    // Game 1v1 board will be only be modified in NotifyMoveComputed
    CalculatedMove_t thisMove;
    thisMove.piece = a_piece;
    thisMove.coord = a_coord;
    thisMove.playerToMove =  m_the1v1Game.GetPlayerType(a_player);

    // main thread cannot get blocked, if it fails inserting the element in
    // the queue just do nothing
    bool result = m_moveQueue.TryPush(thisMove);

    if (result == false)
    {
#ifdef DEBUG_PRINT
        std::cout << _("Human move could not be added to the queue") << std::endl;
#endif

        Gtk::MessageDialog::MessageDialog errorMsg(
                *this,
                _("Internal Error: Move could not be processed. Please click on the board normally to try again"),
                true,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK,
                true);

        if (errorMsg.run())
        {
            ; // the dialog has only 1 button
        }

        // main thread cannot get blocked. It failed inserting the element so
        // get back and do nothing (do not notify this move)
        return;
    }

    // this method is not expected to be run by another thread, but if both worker thread and human
    // user use the same interface, things are much easier to understand
    // so, there's no need to emit any signal (which adds a bit of overhead), but
    // interfaces must be followed, so NotifyMoveComputed is caled directly instead of emiting interthread
    // signals
    //m_signal_moveComputed.emit();
    NotifyMoveComputed();

    return;
}

void MainWindow::NotifyMoveComputed()
{
    // after the lock protected loop these variables will contain
    // the latest piece and latest coord deployed
    Piece latestPiece(e_noPiece);
    Coordinate latestCoord;
    Game1v1::eGame1v1Player_t latestPlayerToMove = Game1v1::e_Game1v1Player1;

    CalculatedMove_t currentMove;
    bool moveQueueEmpty = true;
    while(m_moveQueue.TryPop(currentMove))
    {
        if (moveQueueEmpty == true)
        {
            moveQueueEmpty = false;
        }

        latestPiece        = currentMove.piece;
        latestCoord        = currentMove.coord;
        latestPlayerToMove = currentMove.playerToMove;

        if (latestPiece.GetType() != e_noPiece)
        {
            m_the1v1Game.PutDownPiece(
                    latestPiece,
                    latestCoord,
                    latestPlayerToMove);
        }
    } // while(m_moveQueue.TryPop(currentMove))

    if (moveQueueEmpty)
    {
        // there were no moves stored in the queue. Nothing can be done
        return;
    }

    // actions will depend on who put down the latest piece
    const Player &latestPlayer   = m_the1v1Game.GetPlayer(latestPlayerToMove);
    const Player &latestOpponent = m_the1v1Game.GetOpponent(latestPlayerToMove);

    // player who didn't put a piece on the board this time
    Game1v1::eGame1v1Player_t followingPlayer;
    // pointer to the stopwatch of the latest player to move
    StopWatchLabel* stopWatchPlayer = NULL;
    // pointer to the stopwatch of the opponent of latest player to move
    StopWatchLabel* stopWatchOpponent = NULL;
    switch (latestPlayerToMove)
    {
    case Game1v1::e_Game1v1Player1:
    {
        stopWatchPlayer   = &m_stopWatchLabelPlayer1;
        stopWatchOpponent = &m_stopWatchLabelPlayer2;
        followingPlayer   = Game1v1::e_Game1v1Player2;
        break;
    }
    case Game1v1::e_Game1v1Player2:
    {
        stopWatchPlayer   = &m_stopWatchLabelPlayer2;
        stopWatchOpponent = &m_stopWatchLabelPlayer1;
        followingPlayer   = Game1v1::e_Game1v1Player1;
        break;
    }
    default:
    {
        // this is impossible. latestPlayerToMove should always be player1 or player2
        // return here avoids compiling warnings
#ifdef DEBUG
        assert(0);
#endif
        return;
    }
    } // switch (latestPlayerToMove)


    // invalidate the board drawing area to show the new moves
    // activating the latest piece deployed glowing effect
    m_boardDrawingArea.Invalidate(
            latestPiece,
            latestCoord,
            latestPlayer);

	UpdateScoreStatus();

    // there's a few things that have to be done if the opponent of the latest
    // player to put down a piece can move
    if ( rules::CanPlayerGo(m_the1v1Game.GetBoard(), latestOpponent) )
    {
        // next player to move is the opponent
        m_boardDrawingArea.SetCurrentPlayer(latestOpponent);

        // stop current player's stopwatch
        stopWatchPlayer->Stop();
        // resume the opponent's stopwatch
        stopWatchOpponent->Continue();
        // restart the progress bar
        G_LOCK(s_computingCurrentProgress);
            s_computingCurrentProgress = 0.0;
        G_UNLOCK(s_computingCurrentProgress);
        m_progressBar.set_fraction(0.0);

        // it will be platestOpponent's go next. set piece colour to latestOpponent's
        uint8_t red   = 0;
        uint8_t green = 0;
        uint8_t blue  = 0;
        latestOpponent.GetColour(red, green, blue);
        m_editPieceTable->SetPieceRGB(
            static_cast<float>(red)   / 255,
            static_cast<float>(green) / 255,
            static_cast<float>(blue)  / 255);

        // remove the actual piece being edited from the edit piece drawing area
        // and force the edit piece drawing area to be redraw
        m_editPieceTable->SetPiece(e_noPiece);

        // swap pieces being shown in pickPiecesDrawingArea and showOpponentPiecesDrawingArea
        // opponent will select pieces while currentPlayer's will be only shown
        m_pickPiecesDrawingArea.Invalidate(latestOpponent);
        m_showOpponentPiecesDrawingArea.Invalidate(latestPlayer);

        // current player to move will have to be updated
        m_currentMovingPlayer = followingPlayer;

        // --it checks if the next player to put down a piece is a computer--
        if ( ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player1) &&
                    Game1v1Config::Instance().IsPlayer1Computer() ) ||
             ( (m_currentMovingPlayer == Game1v1::e_Game1v1Player2) &&
                    Game1v1Config::Instance().IsPlayer2Computer() ) )
        {
            // next player is the computer. Disallow editing the edit piece
            // area while computer is processing next move
            m_editPieceTable->set_sensitive(false);

            // tell the worker thread to compute next move
            if (latestPlayerToMove == Game1v1::e_Game1v1Player1)
            {
                // block till the job is transfered to the worker thread
                RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player2, true, latestCoord, latestPiece);
            }
            else
            {
                // block till the job is transfered to the worker thread
                RequestThreadToComputeNextMove(Game1v1::e_Game1v1Player1, true, latestCoord, latestPiece);
            }
        }
        else
        {
            // allow next player to edit the piece to put down on the board
            // 'cos it's a human being
            m_editPieceTable->set_sensitive(true);

            // restore the mouse cursor so the human being who has to put down next piece
            // can do it
            ResetCursor();
        }
    }
    else
    {
        // remove the actual piece being edited from the edit piece drawing area
        // and force the edit piece drawing area to be redraw
        m_editPieceTable->SetPiece(e_noPiece);

        // latestPlayer can put down more pieces. Game is still on
        m_pickPiecesDrawingArea.Invalidate();

        if ( !rules::CanPlayerGo(m_the1v1Game.GetBoard(), latestPlayer) )
        {
            // neither player1 nor player2 can move. Game is over
            // piece calculated by computer is noPiece which means, no move, or
            // human player can't put down any piece after the latest one
            GameFinished();
        }
        // else
        // game is still on. Current player can put down more pieces.
        // Do nothing while current player thinks of the next move
    }
}

void MainWindow::GameFinished()
{
    // once this function is called, game is supposed to be finished
    if (m_currentGameFinished == true)
    {
        // game was already notified to be finished
        return;
    }
    m_currentGameFinished = true;

    // reset the cursor (even if it's been already done)
    ResetCursor();

    // stop stopwatches (even if it was already done)
    m_stopWatchLabelPlayer1.Stop();
    m_stopWatchLabelPlayer2.Stop();

    // no player is expected to put down any piece on the board now
    m_boardDrawingArea.UnsetCurrentPlayer();

    // allow a possible human user to play with the edit pieces area
    m_editPieceTable->set_sensitive(true);

    // restart the progress bar
    G_LOCK(s_computingCurrentProgress);
        s_computingCurrentProgress = 0.0;
    G_UNLOCK(s_computingCurrentProgress);
    m_progressBar.set_fraction(0.0);

    int32_t squaresLeftPlayer1 = 0;
    int32_t squaresLeftPlayer2 = 0;
    const Player &player1 = m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1);
    const Player &player2 = m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2);
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (player1.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer1 += player1.m_pieces[i].GetNSquares();
        }

        if (player2.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer2 += player2.m_pieces[i].GetNSquares();
        }
    }

    uint8_t red1, green1, blue1;
    uint8_t red2, green2, blue2;
    player1.GetColour(red1, green1, blue1);
    player2.GetColour(red2, green2, blue2);

    char theMessage[GAME_FINISHED_BUFFER_LENGTH];
    if (squaresLeftPlayer1 == squaresLeftPlayer2)
    {
        snprintf(theMessage,
                GAME_FINISHED_BUFFER_LENGTH,
                // i18n TRANSLATORS: Please, leave the translated word DRAW between the <b> and </b> tags
                // i18n Bear in mind the first ' color=\"#%02X%02X%02X\" ' will be replaced by player1's colour,
                // i18n and the first %s will be replaced by player1's name
                // i18n The 2nd ' color=\"#%02X%02X%02X\" ' and 2nd %s will be replaced by player2's colour and
                // i18n player2's name respectively.
                // i18n The final %d will be replaced by the final score
                // i18n The order of these tags must be maintained in the translation
                // i18n A typical complete message:
                // i18n That was a <b>DRAW</b>!\nBoth <span color="#012345">Eddie</span> and <span color="#678901">John</span> have <b>5</b> squares left
                // i18n Thank you for contributing to this project
                _("That was a <b>DRAW</b>!\nBoth <span color=\"#%02X%02X%02X\">%s</span> and <span color=\"#%02X%02X%02X\">%s</span> have <b>%d</b> squares left"),
                static_cast<int32_t>(red1),
                static_cast<int32_t>(green1),
                static_cast<int32_t>(blue1),
                player1.GetName().c_str(),
                static_cast<int32_t>(red2),
                static_cast<int32_t>(green2),
                static_cast<int32_t>(blue2),
                player2.GetName().c_str(),
                squaresLeftPlayer1);

        // for the record. This was the old way before i18n
        /*
        std::stringstream theMessage;
        theMessage
            << "That was a <b>DRAW</b>!"
            << std::endl
            << "    Both "
            << "<span color=\"#"
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(red1)
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(green1)
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(blue1)
            << "\">"
            << player1.GetName()
            << "</span>"
            << " and "
            << "<span color=\"#"
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(red2)
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(green2)
            << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << static_cast<int32_t>(blue2)
            << "\">"
            << player2.GetName()
            << "</span>"
            << " have <b>"
            << std::setbase(10) << squaresLeftPlayer1
            << "</b> squares left";
        */
    }
    else //(squaresLeftPlayer1 != squaresLeftPlayer2)
    {
        snprintf(theMessage,
                GAME_FINISHED_BUFFER_LENGTH,
                // i18n TRANSLATORS: Please, respect the HTML formatting as much as possible
                // i18n Bear in mind that the 1st ' color=\"#%02X%02X%02X\" ' will be replaced by the winner's colour,
                // i18n and the first %s will be replaced by the winner's name
                // i18n The 2nd ' color=\"#%02X%02X%02X\" ' and 2nd %s will be replaced by player1's colour and
                // i18n player1's name respectively. the following %d would be player1's score
                // i18n The 3rd ' color=\"#%02X%02X%02X\" ' and 3rd %s will be replaced by player2's colour and
                // i18n player2's name respectively. the following %d would be player2's score
                // i18n The order of these tags must be taken into account for the translation
                // i18n A typical complete message:
                // i18n <b><span color="#012345">Eddie</span></b>!\n<span color="#012345">Eddie</span> has <b>5</b> squares left and <span color="#678901">John</span> has <b>7</b>
                // i18n Thank you for contributing to this project
                _("<b><span color=\"#%02X%02X%02X\">%s</span></b> won!\n<span color=\"#%02X%02X%02X\">%s</span> has <b>%d</b> squares left and <span color=\"#%02X%02X%02X\">%s</span> has <b>%d</b>"),
                (squaresLeftPlayer1 < squaresLeftPlayer2) ? red1   : red2,
                (squaresLeftPlayer1 < squaresLeftPlayer2) ? green1 : green2,
                (squaresLeftPlayer1 < squaresLeftPlayer2) ? blue1  : blue2,
                (squaresLeftPlayer1 < squaresLeftPlayer2) ? player1.GetName().c_str() : player2.GetName().c_str(),
                red1,
                green1,
                blue1,
                player1.GetName().c_str(),
                squaresLeftPlayer1,
                red2,
                green2,
                blue2,
                player2.GetName().c_str(),
                squaresLeftPlayer2);
    }

    //MessageDialog (
    //        Gtk::Window& parent,
    //        const Glib::ustring& message,
    //        bool use_markup=false,
    //        MessageType type=MESSAGE_INFO,
    //        ButtonsType buttons=BUTTONS_OK,
    //        bool modal=false)
    Gtk::MessageDialog::MessageDialog gameOverMessage(
            *this,
            theMessage,
            true,
            Gtk::MESSAGE_INFO,
            Gtk::BUTTONS_OK,
            true);

    if (gameOverMessage.run())
    {
        ; // the dialog only has 1 button
    }
}

void MainWindow::NotifyProgressUpdate()
{
    if (m_currentGameFinished)
    {
        // do not update the progress bar if the current game is finished
        return;
    }

    float progress;

    G_LOCK(s_computingCurrentProgress);
        progress = s_computingCurrentProgress;
    G_UNLOCK(s_computingCurrentProgress);

    m_progressBar.set_fraction(progress);
}

void MainWindow::UpdateScoreStatus()
{
    // this buffer will contain the string to be applied to the labels
    char theMessage[SCORE_LABEL_BUFFER_LENGTH];

    // calculate the number of squares left
    int32_t squaresLeftPlayer1 = 0;
    int32_t squaresLeftPlayer2 = 0;
    const Player &player1 = m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player1);
    const Player &player2 = m_the1v1Game.GetPlayer(Game1v1::e_Game1v1Player2);
    for (int8_t i = e_minimumPieceIndex ; i < e_numberOfPieces; i++)
    {
        if (player1.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer1 += player1.m_pieces[i].GetNSquares();
        }

        if (player2.IsPieceAvailable(static_cast<ePieceType_t>(i)))
        {
            squaresLeftPlayer2 += player2.m_pieces[i].GetNSquares();
        }
    }

    uint8_t red, green, blue;
    player2.GetColour(red, green, blue);

    // update player2's GUI widget
    snprintf (theMessage,
              SCORE_LABEL_BUFFER_LENGTH,
              // i18n TRANSLATORS: the first 3 %02X  will be replaced by the
              // i18n colour of the current player. The '%s' will be replaced
              // i18n by this player's name and the '%2d' his/her score
              // i18n For a better GUI experience both strings should take
              // i18n the same amount of characters
              ngettext("<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       "<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       static_cast<int32_t>(squaresLeftPlayer2)),
              red,
              green,
              blue,
              player2.GetName().c_str(),
              squaresLeftPlayer2);
    m_player2ScoreLabel.set_markup(theMessage);

    // and now player1's
    player1.GetColour(red, green, blue);
    snprintf (theMessage,
              SCORE_LABEL_BUFFER_LENGTH,
              // i18n TRANSLATORS: the first 3 %02X  will be replaced by the
              // i18n colour of the current player. The '%s' will be replaced
              // i18n by this player's name and the '%2d' his/her score
              // i18n For a better GUI experience all these strings should take
              // i18n the same amount of characters
              ngettext("<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       "<span color=\"#%02X%02X%02X\">%s</span>: %2d left",
                       static_cast<int32_t>(squaresLeftPlayer1)),
              red,
              green,
              blue,
              player1.GetName().c_str(),
              squaresLeftPlayer1);
    m_player1ScoreLabel.set_markup(theMessage);
}

void MainWindow::SetWaitCursor()
{
    Glib::RefPtr<Gdk::Window> window;
    window = m_editPieceTable->get_window();
    if (window)
    {
        get_window()->set_cursor(Gdk::Cursor(Gdk::WATCH));
    }
}
void MainWindow::ResetCursor()
{
    Glib::RefPtr<Gdk::Window> window;
    window= m_editPieceTable->get_window();
    if (window)
    {
        window->set_cursor();
    }
}
