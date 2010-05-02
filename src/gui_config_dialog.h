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
/// @file  gui_config_dialog.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 01-May-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef _CONFIGDIALOG_H_
#define _CONFIGDIALOG_H_

#include <gtkmm.h>
#include "gui_exception.h"

class ConfigDialog :
    public Gtk::Dialog
{
public:
    // to be used with m_gtkBuilder->get_widget_derived(GUI_ABOUT_DIALOG_NAME, m_aboutDialog);
    ConfigDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException);
    virtual ~ConfigDialog();

    // override Dialog::run. It will call Dialog::run internally to show the dialog on the screen
    int run();

    // returns if player1 has been set to computer within the dialog
    bool IsPlayer1TypeComputer();
    // returns if player2 has been set to computer within the dialog
    bool IsPlayer2TypeComputer();

private:
    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gtk::Builder> m_gtkBuilder;

    // Gtk::table used for configure player1
    Gtk::Table* m_tablePlayer1;

    // Gtk::table used for configure player2
    Gtk::Table* m_tablePlayer2;

    // combo box for type of player1
    Gtk::ComboBoxText m_comboTypePlayer1;
    // combo box for type of player2
    Gtk::ComboBoxText m_comboTypePlayer2;

    // prevent the default constructors to be used
    ConfigDialog();
    ConfigDialog(const ConfigDialog &a_src);
    ConfigDialog& operator=(const ConfigDialog &a_src);
};

#endif /* _CONFIGDIALOG_H_ */
