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
/// @file main_window.cpp
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
///           Faustino Frechilla 06-Oct-2010  Adapted to Game1v1Widget
///           Faustino Frechilla 11-Oct-2010  New Game dialog
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif

#include <cstdio>    // printf/snprintf (needed for better i18n)
#include "gettext.h" // i18n
#include "gui/main_window.h"
#include "gui/glade_defs.h"

/// message to be shown to the user when he/she requested the
/// application to be closed, and the worker thread is busy computing
static const char MESSAGE_ASK_BEFORE_CLOSE[] =
        N_("The game is computing the next move. Are you sure do you want to close the application?");


MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException):
    Gtk::Window(cobject), //Calls the base class constructor
    m_gtkBuilder(a_gtkBuilder),
    m_aboutDialog(NULL),
    m_config1v1Dialog(NULL),
    m_newGameDialog(NULL),
    m_game1v1Widget(),
    m_gameTotalAllocationWidget(),
    m_gameChallengeWidget(),
    m_game4PlayersWidget()
{
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

    // retrieve the about dialog. It must be retrieved calling get_widget_derived
    // otherwise app will core
    m_gtkBuilder->get_widget_derived(GUI_DIALOG_ABOUT_NAME, m_aboutDialog);
    if (m_aboutDialog == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // retrieve the config dialog. It must be retrieved calling get_widget_derived
    // otherwise app will core
    m_gtkBuilder->get_widget_derived(GUI_DIALOG_CONFIG_1V1_NAME, m_config1v1Dialog);
    if (m_config1v1Dialog == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // retrieve the new game dialog. It must be retrieved calling get_widget_derived
    // otherwise app will core
    m_gtkBuilder->get_widget_derived(GUI_DIALOG_NEW_GAME_NAME, m_newGameDialog);
    if (m_newGameDialog == NULL)
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

    m_gameMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_GAME));
    if (!m_gameMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_viewMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_VIEW));
    if (!m_viewMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS));
    if (!m_settingsMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_helpMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_HELP));
    if (!m_helpMenuItem)
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

    m_viewOpponentPiecesMenuItem = Glib::RefPtr<Gtk::CheckMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_VIEW_PLAYERS_PIECES));
    if (!m_viewOpponentPiecesMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsNKPointsMenuItem = Glib::RefPtr<Gtk::CheckMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_NKPOINTS));
    if (!m_settingsNKPointsMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsForbiddenAreaMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_FORBIDDENAREA));
    if (!m_settingsForbiddenAreaMenuItem)
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

    m_settingsForbiddenAreaPlayer3MenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_FORBIDDENAREA_PLAYER3));
    if (!m_settingsForbiddenAreaPlayer3MenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsForbiddenAreaPlayer4MenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_FORBIDDENAREA_PLAYER4));
    if (!m_settingsForbiddenAreaPlayer4MenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsForbiddenAreaNoShowMenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_FORBIDDENAREA_NOSHOW));
    if (!m_settingsForbiddenAreaNoShowMenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsInfluenceAreaMenuItem = Glib::RefPtr<Gtk::MenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_INFLUENCEAREA));
    if (!m_settingsInfluenceAreaMenuItem)
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

    m_settingsInfluenceAreaPlayer3MenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_INFLUENCEAREA_PLAYER3));
    if (!m_settingsInfluenceAreaPlayer3MenuItem)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_settingsInfluenceAreaPlayer4MenuItem = Glib::RefPtr<Gtk::RadioMenuItem>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_MENU_ITEM_SETTINGS_INFLUENCEAREA_PLAYER4));
    if (!m_settingsInfluenceAreaPlayer4MenuItem)
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

    m_gtkBuilder->get_widget(GUI_VBOX_MAIN, m_vBoxMain);
    if (m_vBoxMain == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();

    // accelerators for main_window menu
    this->add_accel_group(m_accelGroup);

    // add game widgets into the window. (hide them all before hand)
    // Expand and fill!
    m_game1v1Widget.hide_all();
    m_gameTotalAllocationWidget.hide_all();
    m_gameChallengeWidget.hide_all();
    m_game4PlayersWidget.hide_all();
    m_vBoxMain->pack_start(m_game1v1Widget, true, true);
    m_vBoxMain->pack_start(m_gameTotalAllocationWidget, true, true);
    m_vBoxMain->pack_start(m_gameChallengeWidget, true, true);
    m_vBoxMain->pack_start(m_game4PlayersWidget, true, true);

    // connect also its handlers
    m_game1v1Widget.signal_fatalError().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_FatalError));
    m_game1v1Widget.signal_gameFinished().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_GameFinished));
    m_gameTotalAllocationWidget.signal_fatalError().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_FatalError));
    m_gameTotalAllocationWidget.signal_gameFinished().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_GameFinished));
    m_gameChallengeWidget.signal_fatalError().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_FatalError));
    m_gameChallengeWidget.signal_gameFinished().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_GameFinished));
    m_game4PlayersWidget.signal_fatalError().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_FatalError));
    m_game4PlayersWidget.signal_gameFinished().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_GameFinished));

    // connect signals to handlers
    // if the handler is not part of an object use sigc::ptr_fun
    this->signal_delete_event().connect(
            sigc::mem_fun(*this, &MainWindow::MainWindow_DeleteEvent));
    m_newMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemGameNew_Activate));
    m_quitMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemGameQuit_Activate));
    m_helpAboutMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemHelpAbout_Activate));
    m_viewOpponentPiecesMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemViewOpponentsPieces_Toggled));
    m_settingsPrefsMenuItem->signal_activate().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsPreferences_Activate));
    m_settingsNKPointsMenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsViewNKPoints_Toggled));
    m_settingsForbiddenAreaPlayer1MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowForbiddenArea_Toggled));
    m_settingsForbiddenAreaPlayer2MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowForbiddenArea_Toggled));
    m_settingsForbiddenAreaPlayer3MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowForbiddenArea_Toggled));
    m_settingsForbiddenAreaPlayer4MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowForbiddenArea_Toggled));
    m_settingsForbiddenAreaNoShowMenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowForbiddenArea_Toggled));
    m_settingsInfluenceAreaPlayer1MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowInfluenceArea_Toggled));
    m_settingsInfluenceAreaPlayer2MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowInfluenceArea_Toggled));
    m_settingsInfluenceAreaPlayer3MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowInfluenceArea_Toggled));
    m_settingsInfluenceAreaPlayer4MenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowInfluenceArea_Toggled));
    m_settingsInfluenceAreaNoShowMenuItem->signal_toggled().connect(
            sigc::mem_fun(*this, &MainWindow::MenuItemSettingsShowInfluenceArea_Toggled));

    // set up window to show a 1vs1 game by default
    SetupWindowForNewGame(e_gameType1vs1);

    // launch the 1v1 game by default!!
    m_game1v1Widget.LaunchNewGame();
}

MainWindow::~MainWindow()
{
    // http://library.gnome.org/devel/gtkmm/stable/classGtk_1_1Builder.html#ab8c6679c1296d6c4d8590ef907de4d5a
    // Note that you are responsible for deleting top-level widgets (windows and dialogs) instantiated
    // by the Builder object. Other widgets are instantiated as managed so they will be deleted
    // automatically if you add them to a container widget
    delete(m_aboutDialog);
    delete(m_config1v1Dialog);
    delete(m_newGameDialog);
    //delete(m_editPieceTable);
}

bool MainWindow::MainWindow_DeleteEvent(GdkEventAny*)
{
    if (m_game1v1Widget.IsComputingMove())
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
    m_game1v1Widget.CancelComputing();

    // continue with delete event
    return false;
}

void MainWindow::MenuItemGameQuit_Activate()
{
    if (m_game1v1Widget.IsComputingMove())
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

    // cancel the worker thread
    m_game1v1Widget.CancelComputing();

    // exit the app
    this->hide();
}

void MainWindow::MenuItemGameNew_Activate()
{
    Gtk::ResponseType result;
    bool dialogStaysOnScreen = true;

    while (dialogStaysOnScreen == true)
    {
        result = static_cast<Gtk::ResponseType>(m_newGameDialog->run());

        if ( (result == Gtk::RESPONSE_OK) &&
             (m_newGameDialog->GetSelectedTypeOfGame() == e_gameTypeChallenge) &&
             (m_newGameDialog->GetCurrentBlockemChallenge().Initialised() == false) )
        {
            // handling of challenge games is a bit special
            // if current selected challenge is not valid we'll be showing
            // a message box telling the user he/she can't start a challenge
            // game from an invalid blockem challenge file.
            // afterwards the function will return without hiding the dialog
            Gtk::MessageDialog::MessageDialog infoMessage(
                *m_newGameDialog,
                _("A challenge game can't start without a valid blockem challenge file"),
                true,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK,
                true);

            infoMessage.run(); // there will be only one type of value returned

            // newGameDialog must stay. settings weren't correct to be able to
            // launch a new game. stay in the loop
            continue;
        }

        dialogStaysOnScreen = false;
    } // while (dialogStaysOnScreen == true)

    if (result == Gtk::RESPONSE_OK)
    {
        // save configuration shown by the dialog into global config singleton
        // it only saves the one regarding the selected game type (it is passed
        // as a parameter)
        m_newGameDialog->SaveCurrentConfigIntoGlobalSettings(
            m_newGameDialog->GetSelectedTypeOfGame());

        // set up main window widgets for this new game
        SetupWindowForNewGame(m_newGameDialog->GetSelectedTypeOfGame());

        // go for a new game with the new settings. Gotta check type of game first!!
        switch(m_newGameDialog->GetSelectedTypeOfGame())
        {
        case e_gameType1vs1:
        {
            // launch this new game!!
            m_game1v1Widget.LaunchNewGame();
            break;
        }
        case e_gameTypeTotalAllocation:
        {
            // launch this new game!!
            m_gameTotalAllocationWidget.LaunchNewGame();
            break;
        }
        case e_gameTypeChallenge:
        {
            // launch the new challenge!!
            m_gameChallengeWidget.LaunchNewGame();
            break;
        }
        case e_gameType4Players:
        {
            // launch the new 4players game!!
            m_game4PlayersWidget.LaunchNewGame();
            break;
        }
#ifdef DEBUG
        default:
            assert(0);
#endif
        } // switch(m_newGameDialog->GetSelectedTypeOfGame())

    } // if (result == Gtk::RESPONSE_OK)
#ifdef DEBUG
    else if ( (result != Gtk::RESPONSE_CANCEL) && (result != Gtk::RESPONSE_DELETE_EVENT))
    {
        // unexpected
        assert(0);
    }
#endif // DEBUG

    // hide the new game dialog. New game has been launched
    m_newGameDialog->hide();
}

void MainWindow::MenuItemViewOpponentsPieces_Toggled()
{
    // only 1vs1 and 4 players games show the opponents pieces
    if (m_viewOpponentPiecesMenuItem->property_active())
    {
        m_game1v1Widget.ShowOpponentsPieces(true);
        m_game4PlayersWidget.ShowOpponentsPieces(true);
    }
    else
    {
        m_game1v1Widget.ShowOpponentsPieces(false);
        m_game4PlayersWidget.ShowOpponentsPieces(false);
    }
}

void MainWindow::MenuItemSettingsViewNKPoints_Toggled()
{
    if (m_settingsNKPointsMenuItem->property_active())
    {
        m_game1v1Widget.BoardDrawingArea().ShowNucleationPoints();
        m_gameTotalAllocationWidget.BoardDrawingArea().ShowNucleationPoints();
        m_gameChallengeWidget.BoardDrawingArea().ShowNucleationPoints();
        m_game4PlayersWidget.BoardDrawingArea().ShowNucleationPoints();
    }
    else
    {
        m_game1v1Widget.BoardDrawingArea().HideNucleationPoints();
        m_gameTotalAllocationWidget.BoardDrawingArea().HideNucleationPoints();
        m_gameChallengeWidget.BoardDrawingArea().HideNucleationPoints();
        m_game4PlayersWidget.BoardDrawingArea().HideNucleationPoints();
    }
}

void MainWindow::MenuItemSettingsShowForbiddenArea_Toggled()
{
    if (m_settingsForbiddenAreaNoShowMenuItem->property_active())
    {
        m_game1v1Widget.ShowForbiddenAreaInBoard(Game1v1::e_Game1v1NoPlayer);
        m_gameTotalAllocationWidget.ShowForbiddenAreaInBoard(false);
        m_gameChallengeWidget.ShowForbiddenAreaInBoard(false);
        m_game4PlayersWidget.ShowForbiddenAreaInBoard(Game4Players::e_Game4_NoPlayer);
    }
    else if (m_settingsForbiddenAreaPlayer1MenuItem->property_active())
    {
        m_game1v1Widget.ShowForbiddenAreaInBoard(Game1v1::e_Game1v1Player1);
        m_gameTotalAllocationWidget.ShowForbiddenAreaInBoard(true);
        m_gameChallengeWidget.ShowForbiddenAreaInBoard(true);
        m_game4PlayersWidget.ShowForbiddenAreaInBoard(Game4Players::e_Game4_Player1);
    }
    else if (m_settingsForbiddenAreaPlayer2MenuItem->property_active())
    {
        m_game1v1Widget.ShowForbiddenAreaInBoard(Game1v1::e_Game1v1Player2);
        m_game4PlayersWidget.ShowForbiddenAreaInBoard(Game4Players::e_Game4_Player2);
    }
    else if (m_settingsForbiddenAreaPlayer3MenuItem->property_active())
    {
        m_game4PlayersWidget.ShowForbiddenAreaInBoard(Game4Players::e_Game4_Player3);
    }
    else if (m_settingsForbiddenAreaPlayer4MenuItem->property_active())
    {
        m_game4PlayersWidget.ShowForbiddenAreaInBoard(Game4Players::e_Game4_Player4);
    }
}

void MainWindow::MenuItemSettingsShowInfluenceArea_Toggled()
{
    if (m_settingsInfluenceAreaNoShowMenuItem->property_active())
    {
        m_game1v1Widget.ShowInfluenceAreaInBoard(Game1v1::e_Game1v1NoPlayer);
        m_gameTotalAllocationWidget.ShowInfluenceAreaInBoard(false);
        m_gameChallengeWidget.ShowInfluenceAreaInBoard(false);
        m_game4PlayersWidget.ShowInfluenceAreaInBoard(Game4Players::e_Game4_NoPlayer);
    }
    else if (m_settingsInfluenceAreaPlayer1MenuItem->property_active())
    {
        m_game1v1Widget.ShowInfluenceAreaInBoard(Game1v1::e_Game1v1Player1);
        m_gameTotalAllocationWidget.ShowInfluenceAreaInBoard(true);
        m_gameChallengeWidget.ShowInfluenceAreaInBoard(true);
        m_game4PlayersWidget.ShowInfluenceAreaInBoard(Game4Players::e_Game4_Player1);
    }
    else if (m_settingsInfluenceAreaPlayer2MenuItem->property_active())
    {
        m_game1v1Widget.ShowInfluenceAreaInBoard(Game1v1::e_Game1v1Player2);
        m_game4PlayersWidget.ShowInfluenceAreaInBoard(Game4Players::e_Game4_Player2);
    }
    else if (m_settingsInfluenceAreaPlayer3MenuItem->property_active())
    {
        m_game4PlayersWidget.ShowInfluenceAreaInBoard(Game4Players::e_Game4_Player3);
    }
    else if (m_settingsInfluenceAreaPlayer4MenuItem->property_active())
    {
        m_game4PlayersWidget.ShowInfluenceAreaInBoard(Game4Players::e_Game4_Player4);
    }
}

void MainWindow::MenuItemSettingsPreferences_Activate()
{
    // accept button must be deactivated if current game1v1 is finished
    bool dialogCanBeAccepted = !m_game1v1Widget.IsGameFinished();

    // show a message informing the user a move was cancelled?
    bool showInfoMessage = false;
    Gtk::ResponseType result =
        static_cast<Gtk::ResponseType>(m_config1v1Dialog->run(dialogCanBeAccepted));
    if (result == Gtk::RESPONSE_OK)
    {
        showInfoMessage =
            m_game1v1Widget.ProcessChangeInCurrentGame(*m_config1v1Dialog);
    } // if (result == Gtk::RESPONSE_OK)

    m_config1v1Dialog->hide();

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

void MainWindow::Notify_FatalError(const std::string& a_msg)
{
    // show the error message and exit the aplication
    Gtk::MessageDialog::MessageDialog fatalErrorMessage(
        *this,
        a_msg.c_str(),
        true,
        Gtk::MESSAGE_ERROR,
        Gtk::BUTTONS_OK,
        true);

    if (fatalErrorMessage.run())
    {
        ; // the dialog only has 1 button
    }

    // fatal errors can't be handled
    this->hide();
}

void MainWindow::Notify_GameFinished(const std::string& a_msg)
{
    // show the game finished message and wait for the user to accept

    //MessageDialog (
    //        Gtk::Window& parent,
    //        const Glib::ustring& message,
    //        bool use_markup=false,
    //        MessageType type=MESSAGE_INFO,
    //        ButtonsType buttons=BUTTONS_OK,
    //        bool modal=false)
    Gtk::MessageDialog::MessageDialog gameOverMessage(
        *this,
        a_msg.c_str(),
        true,
        Gtk::MESSAGE_INFO,
        Gtk::BUTTONS_OK,
        true);

    if (gameOverMessage.run())
    {
        ; // the dialog only has 1 button
    }
}

void MainWindow::SetupWindowForNewGame(e_blockemGameType_t a_gametype)
{
    // stop game1v1 computing process (it does nothing if it wasn't
    // processing anything)
    m_game1v1Widget.CancelComputing();

    switch(a_gametype)
    {
    case e_gameType1vs1:
    {
        // show player2's menus
        m_settingsForbiddenAreaPlayer2MenuItem->show();
        m_settingsInfluenceAreaPlayer2MenuItem->show();

        // hide player3 and player'4 menus
        m_settingsForbiddenAreaPlayer3MenuItem->hide();
        m_settingsForbiddenAreaPlayer4MenuItem->hide();
        m_settingsInfluenceAreaPlayer3MenuItem->hide();
        m_settingsInfluenceAreaPlayer4MenuItem->hide();

        // settings menu is clickable in 1vs1 games
        m_settingsPrefsMenuItem->set_sensitive(true);
        
        // user can decide whether to show opponent's pieces
        m_viewOpponentPiecesMenuItem->set_sensitive(true);        

        // configure now which game widget will be shown
        // set_visible doesn't work in 2.16 (which is used in windows)
        // show_all must be used instead!
        m_gameTotalAllocationWidget.hide_all();
        m_gameChallengeWidget.hide_all();
        m_game4PlayersWidget.hide_all();

        m_game1v1Widget.show_all();
        
        // hide opponent's pieces if option menu is unselected
        if (m_viewOpponentPiecesMenuItem->get_active() == false)
        {            
            m_game1v1Widget.ShowOpponentsPieces(false);
        }

        break;
    }
    case e_gameTypeTotalAllocation:
    {
        // hide player2's menus since there is no player2 in
        // total allocation games
        m_settingsForbiddenAreaPlayer2MenuItem->hide();
        m_settingsInfluenceAreaPlayer2MenuItem->hide();
        // hide also player3 and player'4 menus
        m_settingsForbiddenAreaPlayer3MenuItem->hide();
        m_settingsForbiddenAreaPlayer4MenuItem->hide();
        m_settingsInfluenceAreaPlayer3MenuItem->hide();
        m_settingsInfluenceAreaPlayer4MenuItem->hide();

        // total allocation games cannot be configured
        m_settingsPrefsMenuItem->set_sensitive(false);
        
        // there is no opponent so its pieces can't be shown
        m_viewOpponentPiecesMenuItem->set_sensitive(false);

        // configure now which game widget will be shown
        // set_visible doesn't work in 2.16 (which is used in windows)
        // show_all must be used instead!
        m_game1v1Widget.hide_all();
        m_gameChallengeWidget.hide_all();
        m_game4PlayersWidget.hide_all();

        m_gameTotalAllocationWidget.show_all();

        break;
    }
    case e_gameTypeChallenge:
    {
        // hide player2's menus since there is no player2 in
        // challenge games
        m_settingsForbiddenAreaPlayer2MenuItem->hide();
        m_settingsInfluenceAreaPlayer2MenuItem->hide();
        // hide also player3 and player'4 menus
        m_settingsForbiddenAreaPlayer3MenuItem->hide();
        m_settingsForbiddenAreaPlayer4MenuItem->hide();
        m_settingsInfluenceAreaPlayer3MenuItem->hide();
        m_settingsInfluenceAreaPlayer4MenuItem->hide();

        // challenge games cannot be configured
        m_settingsPrefsMenuItem->set_sensitive(false);
        
        // there is no opponent so its pieces can't be shown
        m_viewOpponentPiecesMenuItem->set_sensitive(false);

        // configure now which game widget will be shown
        // set_visible doesn't work in 2.16 (which is used in windows)
        // show_all must be used instead!
        m_game1v1Widget.hide_all();
        m_gameTotalAllocationWidget.hide_all();
        m_game4PlayersWidget.hide_all();

        m_gameChallengeWidget.show_all();

        break;
    }
    case e_gameType4Players:
    {
        // show player2, player3 and player4's menus
        m_settingsForbiddenAreaPlayer2MenuItem->show();
        m_settingsForbiddenAreaPlayer3MenuItem->show();
        m_settingsForbiddenAreaPlayer4MenuItem->show();
        m_settingsInfluenceAreaPlayer2MenuItem->show();
        m_settingsInfluenceAreaPlayer3MenuItem->show();
        m_settingsInfluenceAreaPlayer4MenuItem->show();

        // 4players games cannot be configured
        m_settingsPrefsMenuItem->set_sensitive(false);
        
        // user can decide whether to show opponent's pieces
        m_viewOpponentPiecesMenuItem->set_sensitive(true);

        // configure now which game widget will be shown
        // set_visible doesn't work in 2.16 (which is used in windows)
        // show_all must be used instead!
        m_game1v1Widget.hide_all();
        m_gameTotalAllocationWidget.hide_all();
        m_gameChallengeWidget.hide_all();

        m_game4PlayersWidget.show_all();
        
        // hide opponents' pieces if option menu is unselected
        if (m_viewOpponentPiecesMenuItem->get_active() == false)
        {            
            m_game4PlayersWidget.ShowOpponentsPieces(false);
        }

        break;
    }
#ifdef DEBUG
    default:
        assert(0);
#endif
    } // switch(a_gametype)
}

#ifdef WIN32
void MainWindow::ForceTranslationOfWidgets()
{
    // in win32 systems gettext fails when the string is static and marked as
    // translatable with N_() but _() is never called explicitely. Basically
    // there are 2 kinds of strings that are not translated:
    //  + Those included in the GOptionEntry list, which show the available
    //    options that can be passed to the program through command line
    //  + Strings included in the .glade file that never change during the
    //    execution of the application, for example a menu called "Game", or a
    //    label that contains the word "rotate"
    //
    // We'll be calling here to _() for every string found in the .glade file
    // so it gets properly translated into the current domain (the 2nd case
    // described above)

    // for some reason titles do get translated
    //set_title( _(get_title().c_str()) );

    m_gameMenuItem->set_label(
        _(m_gameMenuItem->get_label().c_str()));

    m_viewMenuItem->set_label(
            _(m_viewMenuItem->get_label().c_str()));

    m_settingsMenuItem->set_label(
        _(m_settingsMenuItem->get_label().c_str()));

    m_helpMenuItem->set_label(
        _(m_helpMenuItem->get_label().c_str()));

    m_viewOpponentPiecesMenuItem->set_label(
            _(m_viewOpponentPiecesMenuItem->get_label().c_str()));

    m_settingsNKPointsMenuItem->set_label(
        _(m_settingsNKPointsMenuItem->get_label().c_str()));

    m_settingsForbiddenAreaMenuItem->set_label(
            _(m_settingsForbiddenAreaMenuItem->get_label().c_str()));

    m_settingsForbiddenAreaPlayer1MenuItem->set_label(
        _(m_settingsForbiddenAreaPlayer1MenuItem->get_label().c_str()));

    m_settingsForbiddenAreaPlayer2MenuItem->set_label(
        _(m_settingsForbiddenAreaPlayer2MenuItem->get_label().c_str()));

    m_settingsForbiddenAreaPlayer3MenuItem->set_label(
        _(m_settingsForbiddenAreaPlayer3MenuItem->get_label().c_str()));

    m_settingsForbiddenAreaPlayer4MenuItem->set_label(
        _(m_settingsForbiddenAreaPlayer4MenuItem->get_label().c_str()));

    m_settingsForbiddenAreaNoShowMenuItem->set_label(
        _(m_settingsForbiddenAreaNoShowMenuItem->get_label().c_str()));

    m_settingsInfluenceAreaMenuItem->set_label(
            _(m_settingsInfluenceAreaMenuItem->get_label().c_str()));

    m_settingsInfluenceAreaPlayer1MenuItem->set_label(
        _(m_settingsInfluenceAreaPlayer1MenuItem->get_label().c_str()));

    m_settingsInfluenceAreaPlayer2MenuItem->set_label(
        _(m_settingsInfluenceAreaPlayer2MenuItem->get_label().c_str()));

    m_settingsInfluenceAreaPlayer3MenuItem->set_label(
        _(m_settingsInfluenceAreaPlayer3MenuItem->get_label().c_str()));

    m_settingsInfluenceAreaPlayer4MenuItem->set_label(
        _(m_settingsInfluenceAreaPlayer4MenuItem->get_label().c_str()));

    m_settingsInfluenceAreaNoShowMenuItem->set_label(
        _(m_settingsInfluenceAreaNoShowMenuItem->get_label().c_str()));
}
#else
void MainWindow::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
