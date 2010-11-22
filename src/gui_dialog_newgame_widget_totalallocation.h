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
/// @file  gui_dialog_newgame_widget_totalallocation.h
/// @brief 
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 22-Nov-2010  Original development. Moved from gui_dialog_newgame.h
/// @endhistory
///
// ============================================================================

#ifndef _GUI_DIALOG_NEWGAME_WIDGET_TOTALALLOCATION_H_
#define _GUI_DIALOG_NEWGAME_WIDGET_TOTALALLOCATION_H_

#include <gtkmm.h>
#include "gui_dialog_newgame_widget.h"
#include "gui_exception.h"
#include "coordinate.h"

/// @brief table to be shown on the new game dialog when the user selects
///        total allocation game
class NewGameTableTotalAllocation :
    public NewGameTable
{
public:
    // to be used with m_gtkBuilder->get_widget_derived(GUI_NEWGAME_1V1_HBOX, m_table);
    NewGameTableTotalAllocation(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException);
    virtual ~NewGameTableTotalAllocation();

    /// @brief load current global configuration into the widgets
    virtual void LoadCurrentConfigFromGlobalSettings();
    /// @brief save info saved in the widgets into current global configuration
    virtual void SaveCurrentConfigIntoGlobalSettings() const;

private:
    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gtk::Builder> m_gtkBuilder;

    /// starting row spin button
    Gtk::SpinButton* m_spinbuttonStartingRow;
    Gtk::Adjustment  m_spinbuttonStartingRowAdj;
    /// starting column spin button
    Gtk::SpinButton* m_spinbuttonStartingColumn;
    Gtk::Adjustment  m_spinbuttonStartingColumnAdj;

    /// spin button for the number of rows
    Gtk::SpinButton* m_spinbuttonNRows;
    Gtk::Adjustment  m_spinbuttonNRowsAdj;
    /// spin button for for the number of columns
    Gtk::SpinButton* m_spinbuttonNCols;
    Gtk::Adjustment  m_spinbuttonNColsAdj;

    /// start from anywhere check button
    Gtk::CheckButton* m_checkbuttonStartFromAnywhere;

    // labels loaded here because they need to be translated at startup on win32 platform
    // have a look at ForceTranslationOfWidgets
    Gtk::Label* m_boardSizeLabel;
    Gtk::Label* m_startingPosLabel;
    Gtk::Label* m_startingRowLabel;
    Gtk::Label* m_startingColLabel;
    Gtk::Label* m_nRowsLabel;
    Gtk::Label* m_nColsLabel;

    /// @brief callback for whenever the spinbutton for the number of rows is changed
    void SpinButtonNRows_SignalValueChanged();
    /// @brief callback for whenever the spinbutton for the number of columns is changed
    void SpinButtonNCols_SignalValueChanged();
    /// @brief callback to be called when the "start from anywhere" checkbutton is toggled
    void CheckbuttonStartFromAnywhere_Toggled();

    /// @return the starting coordinate configured by the displayed widgets
    void GetStartingCoord(Coordinate &a_coord) const;

    /// Calls gettext per every static widget in the dialog. These strings
    /// are those ones included in the .glade file that never change during the
    /// execution of the application, for example a menu called "Game", or a
    /// label that contains the word "rotate"
    ///
    /// So far this is only needed in win32 platform due to some unknown issue
    /// that prevents those strings to be automatically translated. It works
    /// fine in linux, so there's no need there to explicitly call to gettext
    void ForceTranslationOfWidgets();

    NewGameTableTotalAllocation();
    NewGameTableTotalAllocation(const NewGameTableTotalAllocation &a_src);
    NewGameTableTotalAllocation& operator=(const NewGameTableTotalAllocation &a_src);

}; // class NewGameTableTotalAllocation

#endif /* _GUI_DIALOG_NEWGAME_WIDGET_TOTALALLOCATION_H_ */
