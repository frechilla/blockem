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
/// @file  dialog_newgame_widget_4players.h
/// @brief 
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 22-Nov-2010  Original development. Moved from gui_dialog_newgame.h
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#ifndef _GUI_DIALOG_NEWGAME_WIDGET_4PLAYERS_H_
#define _GUI_DIALOG_NEWGAME_WIDGET_4PLAYERS_H_

#include <gtkmm.h>
#include "gui/dialog_newgame_widget.h"
#include "gui/gui_exception.h"
#include "coordinate.h"

/// @brief table to be shown on the new game dialog when the user selects
///        a 4 players game
class NewGameTable4Players:
    public NewGameTable
{
public:
    // to be used with m_gtkBuilder->get_widget_derived(GUI_NEWGAME_4PLAYERS_HBOX, m_table);
    NewGameTable4Players(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException);
    virtual ~NewGameTable4Players();

    /// @brief load current global configuration corresponding to the type of
    ///        gamer epresented by the derived classes
    virtual void LoadCurrentConfigFromGlobalSettings();
    /// @brief save info contained in the derived class' widgets into current
    ///        global configuration
    virtual void SaveCurrentConfigIntoGlobalSettings() const;

private:
    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gtk::Builder> m_gtkBuilder;

    /// spin button for the number of rows
    Gtk::SpinButton* m_spinbuttonNRows;
    Gtk::Adjustment  m_spinbuttonNRowsAdj;
    /// spin button for for the number of columns
    Gtk::SpinButton* m_spinbuttonNCols;
    Gtk::Adjustment  m_spinbuttonNColsAdj;

    /// starting row spin button for player1
    Gtk::SpinButton* m_spinbuttonStartingRowPlayer1;
    Gtk::Adjustment  m_spinbuttonStartingRowPlayer1Adj;
    /// starting column spin button for player1
    Gtk::SpinButton* m_spinbuttonStartingColumnPlayer1;
    Gtk::Adjustment  m_spinbuttonStartingColumnPlayer1Adj;

    /// starting row spin button for player2
    Gtk::SpinButton* m_spinbuttonStartingRowPlayer2;
    Gtk::Adjustment  m_spinbuttonStartingRowPlayer2Adj;
    /// starting column spin button for player2
    Gtk::SpinButton* m_spinbuttonStartingColumnPlayer2;
    Gtk::Adjustment  m_spinbuttonStartingColumnPlayer2Adj;

    /// starting row spin button for player3
    Gtk::SpinButton* m_spinbuttonStartingRowPlayer3;
    Gtk::Adjustment  m_spinbuttonStartingRowPlayer3Adj;
    /// starting column spin button for player3
    Gtk::SpinButton* m_spinbuttonStartingColumnPlayer3;
    Gtk::Adjustment  m_spinbuttonStartingColumnPlayer3Adj;

    /// starting row spin button for player4
    Gtk::SpinButton* m_spinbuttonStartingRowPlayer4;
    Gtk::Adjustment  m_spinbuttonStartingRowPlayer4Adj;
    /// starting column spin button for player4
    Gtk::SpinButton* m_spinbuttonStartingColumnPlayer4;
    Gtk::Adjustment  m_spinbuttonStartingColumnPlayer4Adj;

    // labels loaded here because they need to be translated at startup on win32 platform
    // have a look at ForceTranslationOfWidgets
    Gtk::Label* m_nRowsLabel;
    Gtk::Label* m_nColsLabel;
    Gtk::Label* m_player1Label;
    Gtk::Label* m_player2Label;
    Gtk::Label* m_player3Label;
    Gtk::Label* m_player4Label;
    Gtk::Label* m_startingRowLabel1;
    Gtk::Label* m_startingColLabel1;
    Gtk::Label* m_startingRowLabel2;
    Gtk::Label* m_startingColLabel2;
    Gtk::Label* m_startingRowLabel3;
    Gtk::Label* m_startingColLabel3;
    Gtk::Label* m_startingRowLabel4;
    Gtk::Label* m_startingColLabel4;

    /// @brief callback for whenever the spinbutton for the number of rows is changed
    void SpinButtonNRows_SignalValueChanged();
    /// @brief callback for whenever the spinbutton for the number of columns is changed
    void SpinButtonNCols_SignalValueChanged();

    /// @return player1's starting coordinate configured by the displayed widgets
    void GetStartingCoordPlayer1(Coordinate &a_coord) const;
    /// @return player1's starting coordinate configured by the displayed widgets
    void GetStartingCoordPlayer2(Coordinate &a_coord) const;
    /// @return player1's starting coordinate configured by the displayed widgets
    void GetStartingCoordPlayer3(Coordinate &a_coord) const;
    /// @return player1's starting coordinate configured by the displayed widgets
    void GetStartingCoordPlayer4(Coordinate &a_coord) const;

    /// Calls gettext per every static widget in the dialog. These strings
    /// are those ones included in the .glade file that never change during the
    /// execution of the application, for example a menu called "Game", or a
    /// label that contains the word "rotate"
    ///
    /// So far this is only needed in win32 platform due to some unknown issue
    /// that prevents those strings to be automatically translated. It works
    /// fine in linux, so there's no need there to explicitly call to gettext
    void ForceTranslationOfWidgets();

    NewGameTable4Players();
    NewGameTable4Players(const NewGameTable4Players &a_src);
    NewGameTable4Players& operator=(const NewGameTable4Players &a_src);
};

#endif /* _GUI_DIALOG_NEWGAME_WIDGET_4PLAYERS_H_ */
