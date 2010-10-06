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
/// @file gui_main_window.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 12-Nov-2009  Original development
///           Faustino Frechilla 25-Apr-2010  libglade dependency removed. Code migrated to GtkBuilder
///           Faustino Frechilla 06-Oct-2010  Adapted to Game1v1Widget
/// @endhistory
///
// ============================================================================

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <string>
#include <exception>
#include <gtkmm.h>

#include "gui_about_dialog.h"
#include "gui_game1v1_config_dialog.h"
#include "gui_exception.h"
#include "gui_game1v1_widget.h"
#include "gui_game_total_allocation_widget.h"


/// @brief class to handle the main window in the GUI
class MainWindow :
    public Gtk::Window
{
public:
    /// Instantiates the class. It needs a Gtk::Builder object to retrieve the
    /// glade info
    // to be used with m_gtkBuilder->get_widget_derived(GUI_VBOX_MAIN, pMainWindow)
    MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException);
    virtual ~MainWindow();


    /// @brief handle fatal errors coming from the Game1v1Widget
    void Game1v1Widget_FatalError(const std::string& a_msg);
    
    /// @brief handle game finished signals coming from the Game1v1Widget
    void Game1v1Widget_GameFinished(const std::string& a_msg);

    /// @brief callback to be called when the window is about to be closed
    ///        using the X on the corner
    bool MainWindow_DeleteEvent(GdkEventAny*);

    /// @brief callback to be called when the window is about to be closed
    ///        using the menutiem Game->quit
    void MenuItemGameQuit_Activate();

    /// @brief callback to be called when the menuitem Game->new is pressed
    void MenuItemGameNew_Activate();

    /// @brief callback to be called when the menuitem Help->about is pressed
    void MenuItemHelpAbout_Activate();

    /// @brief callback to be called when the menuitem Settings->Preferences is pressed
    void MenuItemSettingsPreferences_Activate();

    /// @brief  callback to be called when the menuitem Settings->view nk points is toggled
    void MenuItemSettingsViewNKPoints_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->show player1's forbidden area is toggled
    void MenuItemSettingsShowPlayer1ForbiddenArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->show player2 forbidden area is toggled
    void MenuItemSettingsShowPlayer2ForbiddenArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->do not show forbidden area is toggled
    void MenuItemSettingsShowNoneForbiddenArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->show player1's influence area is toggled
    void MenuItemSettingsShowPlayer1InfluenceArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->show player2 influence area is toggled
    void MenuItemSettingsShowPlayer2InfluenceArea_Toggled();

    /// @brief  callback to be called when the menuitem Settings->areas->do not show influence area is toggled
    void MenuItemSettingsShowNoneInfluenceArea_Toggled();

private:

    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gtk::Builder> m_gtkBuilder;

    /// @brief the about dialog class
    AboutDialog* m_aboutDialog;

    /// @brief the config dialog class
    Game1v1ConfigDialog* m_configDialog;
    
    /// @brief the game1v1 widget
    Game1v1Widget* m_game1v1Widget;
    
    /// @brief the total allocation game widget
    GameTotalAllocationWidget m_gameTotalAllocation;
    
    // accel group + menu items
    Glib::RefPtr<Gtk::AccelGroup> m_accelGroup;
    Glib::RefPtr<Gtk::MenuItem> m_gameMenuItem;
    Glib::RefPtr<Gtk::MenuItem> m_settingsMenuItem;
    Glib::RefPtr<Gtk::MenuItem> m_helpMenuItem;
    Glib::RefPtr<Gtk::MenuItem> m_newMenuItem;
    Glib::RefPtr<Gtk::MenuItem> m_quitMenuItem;
    Glib::RefPtr<Gtk::MenuItem> m_helpAboutMenuItem;
    Glib::RefPtr<Gtk::MenuItem> m_settingsPrefsMenuItem;
    Glib::RefPtr<Gtk::CheckMenuItem> m_settingsNKPointsMenuItem;
    Glib::RefPtr<Gtk::MenuItem> m_settingsForbiddenAreaMenuItem;
    Glib::RefPtr<Gtk::RadioMenuItem> m_settingsForbiddenAreaPlayer1MenuItem;
    Glib::RefPtr<Gtk::RadioMenuItem> m_settingsForbiddenAreaPlayer2MenuItem;
    Glib::RefPtr<Gtk::RadioMenuItem> m_settingsForbiddenAreaNoShowMenuItem;
    Glib::RefPtr<Gtk::MenuItem> m_settingsInfluenceAreaMenuItem;
    Glib::RefPtr<Gtk::RadioMenuItem> m_settingsInfluenceAreaPlayer1MenuItem;
    Glib::RefPtr<Gtk::RadioMenuItem> m_settingsInfluenceAreaPlayer2MenuItem;
    Glib::RefPtr<Gtk::RadioMenuItem> m_settingsInfluenceAreaNoShowMenuItem;

    /// @brief Main vertical box (It's got the menu, vbox drawing plus status bar)
    Gtk::VBox* m_vBoxMain;

    /// Calls gettext per every static widget in the main window. These strings
    /// are those ones included in the .glade file that never change during the
    /// execution of the application, for example a menu called "Game", or a
    /// label that contains the word "rotate"
    ///
    /// So far this is only needed in win32 platform due to some unknown issue
    /// that prevents those strings to be automatically translated. It works
    /// fine in linux, so there's no need there to explicitly call to gettext
    void ForceTranslationOfWidgets();

    // prevent the default constructors to be used
    MainWindow();
    MainWindow(const MainWindow &a_src);
    MainWindow& operator=(const MainWindow &a_src);
};

#endif /* MAINWINDOW_H_ */
