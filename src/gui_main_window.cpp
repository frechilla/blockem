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
///           Faustino Frechilla 06-Oct-2010  Adapted to Game1v1Widget
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#include <cstdio>    // printf/snprintf (needed for better i18n)
#endif

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


MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException):
    Gtk::Window(cobject), //Calls the base class constructor
    m_gtkBuilder(a_gtkBuilder),
    m_aboutDialog(NULL),
    m_configDialog(NULL),
    m_game1v1Widget(),
    m_gameTotalAllocation()
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

    // add game widgets into the window. Expand and fill!
    m_vBoxMain->pack_start(m_game1v1Widget, true, true);    
    m_vBoxMain->pack_start(m_gameTotalAllocation, true, true);
    
    // set_visible doesn't work in 2.16 (which is used in windows). use show!
    m_game1v1Widget.show_all();
    //m_gameTotalAllocation.show_all();

    // connect also its handlers
    m_game1v1Widget.signal_fatalError().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_FatalError));
    m_game1v1Widget.signal_gameFinished().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_GameFinished));
    m_gameTotalAllocation.signal_fatalError().connect(
            sigc::mem_fun(*this, &MainWindow::Notify_FatalError));
    m_gameTotalAllocation.signal_gameFinished().connect(
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
    delete(m_configDialog);
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
    m_configDialog->set_title(_(MESSAGE_NEW_1V1GAME_DIALOG_TITLE));
    // starting coords can always be edited when a new game is launched
    m_configDialog->SetStartingCoordEditionSensitive(true);

    Gtk::ResponseType result =
        static_cast<Gtk::ResponseType>(m_configDialog->run());
    if (result == Gtk::RESPONSE_OK)
    {
        // save configuration shown by the dialog into global config singleton
        m_configDialog->SaveCurrentConfigIntoGlobalSettings();

        // go for the brand new game with the new settings!!
        m_game1v1Widget.LaunchNewGame();
    }
#ifdef DEBUG
    else if ( (result != Gtk::RESPONSE_CANCEL) && (result != Gtk::RESPONSE_DELETE_EVENT))
    {
        // unexpected
        assert(0);
    }
#endif // DEBUG

    m_configDialog->hide();
}

void MainWindow::MenuItemSettingsViewNKPoints_Toggled()
{
    if (m_settingsNKPointsMenuItem->property_active())
    {
        m_game1v1Widget.BoardDrawingArea().ShowNucleationPoints();
        m_gameTotalAllocation.BoardDrawingArea().ShowNucleationPoints();
    }
    else
    {
        m_game1v1Widget.BoardDrawingArea().HideNucleationPoints();
        m_gameTotalAllocation.BoardDrawingArea().ShowNucleationPoints();
    }
}

void MainWindow::MenuItemSettingsShowPlayer1ForbiddenArea_Toggled()
{
    if (m_settingsForbiddenAreaPlayer1MenuItem->property_active())
    {
        m_game1v1Widget.ShowForbiddenAreaInBoard(Game1v1::e_Game1v1Player1);
    }
}

void MainWindow::MenuItemSettingsShowPlayer2ForbiddenArea_Toggled()
{
    if (m_settingsForbiddenAreaPlayer2MenuItem->property_active())
    {
        m_game1v1Widget.ShowForbiddenAreaInBoard(Game1v1::e_Game1v1Player2);
    }
}

void MainWindow::MenuItemSettingsShowNoneForbiddenArea_Toggled()
{
    if (m_settingsForbiddenAreaNoShowMenuItem->property_active())
    {
        m_game1v1Widget.ShowForbiddenAreaInBoard(Game1v1::e_Game1v1NoPlayer);
    }
}

void MainWindow::MenuItemSettingsShowPlayer1InfluenceArea_Toggled()
{
    if (m_settingsInfluenceAreaPlayer1MenuItem->property_active())
    {
        m_game1v1Widget.ShowInfluenceAreaInBoard(Game1v1::e_Game1v1Player1);
    }
}

void MainWindow::MenuItemSettingsShowPlayer2InfluenceArea_Toggled()
{
    if (m_settingsInfluenceAreaPlayer2MenuItem->property_active())
    {
        m_game1v1Widget.ShowInfluenceAreaInBoard(Game1v1::e_Game1v1Player2);
    }
}

void MainWindow::MenuItemSettingsShowNoneInfluenceArea_Toggled()
{
    if (m_settingsInfluenceAreaNoShowMenuItem->property_active())
    {
        m_game1v1Widget.ShowInfluenceAreaInBoard(Game1v1::e_Game1v1NoPlayer);
    }
}

void MainWindow::MenuItemSettingsPreferences_Activate()
{
    m_configDialog->set_title(_(MESSAGE_CONFIGURE_GAME_DIALOG_TITLE));

    //TODO starting coords cannot be edited through the configuration dialog yet
    m_configDialog->SetStartingCoordEditionSensitive(false);

    // show a message informing the user a move was cancelled?
    bool showInfoMessage = false;
    Gtk::ResponseType result =
        static_cast<Gtk::ResponseType>(m_configDialog->run());
    if (result == Gtk::RESPONSE_OK)
    {
        showInfoMessage =
            m_game1v1Widget.ProcessChangeInCurrentGame(*m_configDialog);
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

    m_settingsMenuItem->set_label(
        _(m_settingsMenuItem->get_label().c_str()));

    m_helpMenuItem->set_label(
        _(m_helpMenuItem->get_label().c_str()));

    m_settingsNKPointsMenuItem->set_label(
        _(m_settingsNKPointsMenuItem->get_label().c_str()));

    m_settingsForbiddenAreaMenuItem->set_label(
            _(m_settingsForbiddenAreaMenuItem->get_label().c_str()));

    m_settingsForbiddenAreaPlayer1MenuItem->set_label(
        _(m_settingsForbiddenAreaPlayer1MenuItem->get_label().c_str()));

    m_settingsForbiddenAreaPlayer2MenuItem->set_label(
        _(m_settingsForbiddenAreaPlayer2MenuItem->get_label().c_str()));

    m_settingsForbiddenAreaNoShowMenuItem->set_label(
        _(m_settingsForbiddenAreaNoShowMenuItem->get_label().c_str()));

    m_settingsInfluenceAreaMenuItem->set_label(
            _(m_settingsInfluenceAreaMenuItem->get_label().c_str()));

    m_settingsInfluenceAreaPlayer1MenuItem->set_label(
        _(m_settingsInfluenceAreaPlayer1MenuItem->get_label().c_str()));

    m_settingsInfluenceAreaPlayer2MenuItem->set_label(
        _(m_settingsInfluenceAreaPlayer2MenuItem->get_label().c_str()));

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
