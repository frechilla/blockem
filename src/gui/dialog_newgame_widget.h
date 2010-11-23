// ============================================================================
// Copyright 2010 Faustino Frechilla
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
/// @file  dialog_newgame_widget.h
/// @brief file which contains the common interface of those widgets to be shown on the new game dialog
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 22-Nov-2010  Original development. Moved from gui_dialog_newgame.h
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#ifndef _GUI_DIALOG_NEWGAME_WIDGET_H_
#define _GUI_DIALOG_NEWGAME_WIDGET_H_

#include <gtkmm.h>

/// @brief abstract class to be inherited from by those classes (Gtk::HBox)
///        that hold the widgets which configure new games
class NewGameTable :
    public Gtk::HBox
{
public:
    NewGameTable(BaseObjectType* cobject) : Gtk::HBox(cobject)
    {};
    virtual ~NewGameTable()
    {};

    /// @brief load current global configuration corresponding to the type of
    ///        gamer represented by the derived classts
    virtual void LoadCurrentConfigFromGlobalSettings() = 0;
    /// @brief save info contained in the derived class' widgets into current
    ///        global configuration
    virtual void SaveCurrentConfigIntoGlobalSettings() const = 0;

private:
    NewGameTable();
    NewGameTable(const NewGameTable &a_src);
    NewGameTable& operator=(const NewGameTable &a_src);
};

#endif /* _GUI_DIALOG_NEWGAME_WIDGET_H_ */
