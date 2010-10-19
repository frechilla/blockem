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
/// @file  gui_game1v1_config_dialog.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 01-May-2010  Original development
///           Faustino Frechilla 23-Sep-2010  Renamed to gui_game1v1_config_dialog
/// @endhistory
///
// ============================================================================

#ifndef _GUI_GAME1V1_CONFIG_DIALOG_H_
#define _GUI_GAME1V1_CONFIG_DIALOG_H_

#include <gtkmm.h>
#include "gui_exception.h"
#include "coordinate.h"
#include "heuristic.h"

class Game1v1ConfigDialog :
    public Gtk::Dialog
{
public:
    // to be used with m_gtkBuilder->get_widget_derived(GUI_DIALOG_CONFIG_1V1_NAME, m_aboutDialog);
    Game1v1ConfigDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException);
    virtual ~Game1v1ConfigDialog();

    /// @brief override Dialog::run. 
    /// It will call Dialog::run internally to show the dialog on the screen
    /// @param if set to false the ok button will be deactivated
    int run(bool a_dialogCanBeAccepted = true);

    /// @return true if player1 has been set to computer within the dialog
    bool IsPlayer1TypeComputer() const;
    /// @return true if player2 has been set to computer within the dialog
    bool IsPlayer2TypeComputer() const;

    /// @return player1's starting coordinate in the dialog
    void GetPlayer1StartingCoord(Coordinate &a_coord) const;
    /// @return player2's starting coordinate in the dialog
    void GetPlayer2StartingCoord(Coordinate &a_coord) const;

    /// @return player1's search tree depth shown in the dialog
    int32_t GetPlayer1SearchTreeDepth() const;
    /// @return player2's search tree depth shown in the dialog
    int32_t GetPlayer2SearchTreeDepth() const;

    /// @return selected player1's heuristic
    Heuristic::eHeuristicType_t GetPlayer1Heuristic() const;
    /// @return selected player2's heuristic
    Heuristic::eHeuristicType_t GetPlayer2Heuristic() const;

    /// saves all the configuration shows in the widgets of this dialog
    /// into the game1v1 config singleton
    void SaveCurrentConfigIntoGlobalSettings() const;

private:
    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gtk::Builder> m_gtkBuilder;

    /// Gtk::table used for configure player1
    Gtk::Table* m_tablePlayer1;
    /// Gtk::table used for configure player2
    Gtk::Table* m_tablePlayer2;
    /// combo box for type of player1
    Gtk::ComboBoxText m_comboTypePlayer1;
    /// combo box for type of player2
    Gtk::ComboBoxText m_comboTypePlayer2;

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

    /// AI frame player1
    Gtk::Frame* m_AIFramePlayer1;

    /// AI table player1
    Gtk::Table* m_AITablePlayer1;

    /// AI frame player1
    Gtk::Frame* m_AIFramePlayer2;

    /// AI table player2
    Gtk::Table* m_AITablePlayer2;

    /// heuristic type combobox for player1
    Gtk::ComboBoxText m_comboHeuristicPlayer1;
    /// heuristic type combobox for player2
    Gtk::ComboBoxText m_comboHeuristicPlayer2;

    /// search tree depth player1 spin button
    Gtk::SpinButton* m_spinbuttonDepthPlayer1;
    Gtk::Adjustment m_spinbuttonDepthPlayer1Adj;

    /// search tree depth player2 spin button
    Gtk::SpinButton* m_spinbuttonDepthPlayer2;
    Gtk::Adjustment m_spinbuttonDepthPlayer2Adj;

    /// heuristic explanation player1
    Gtk::TextView* m_textViewHeuristic1;

    /// heuristic explanation player2
    Gtk::TextView* m_textViewHeuristic2;

    /// Text Buffers which contain every heuristic description to be shown in
    /// m_textViewHeuristic1 and m_textViewHeuristic2
    Glib::RefPtr<Gtk::TextBuffer> m_refHeuristicDescriptionBuffer[Heuristic::e_heuristicCount];

    // labels loaded here because they need to be translated at startup on win32 platform
    // have a look at ForceTranslationOfWidgets
    Gtk::Label* m_player1Label;
    Gtk::Label* m_player2Label;
    Gtk::Label* m_player1TypeLabel;
    Gtk::Label* m_player2TypeLabel;
    Gtk::Label* m_player1StartingRowLabel;
    Gtk::Label* m_player2StartingRowLabel;
    Gtk::Label* m_player1StartingColLabel;
    Gtk::Label* m_player2StartingColLabel;
    Gtk::Label* m_player1AIHeaderLabel;
    Gtk::Label* m_player2AIHeaderLabel;
    Gtk::Label* m_player1AITypeLabel;
    Gtk::Label* m_player2AITypeLabel;
    Gtk::Label* m_player1AIDepthLabel;
    Gtk::Label* m_player2AIDepthLabel;

    // signals
    void ComboPlayer1Type_signalChanged();
    void ComboPlayer2Type_signalChanged();
    void ComboHeuristicPlayer1_signalChanged();
    void ComboHeuristicPlayer2_signalChanged();
    void SpinButtonDepthPlayer1_SignalValueChanged();
    void SpinButtonDepthPlayer2_SignalValueChanged();
    void DefaultButton_SignalReleased();

    /// will get called when the dialog is shown / reprinted on the screen
    bool on_expose_event (GdkEventExpose* event);
    
    /// set starting coords spinbuttons to sensitive or unsensitive so they can/cannot be edited
    /// @param true will set it to sensitive. false will blur them so they cannot be edited
    void SetStartingCoordEditionSensitive(bool action);

    /// Calls gettext per every static widget in the dialog. These strings
    /// are those ones included in the .glade file that never change during the
    /// execution of the application, for example a menu called "Game", or a
    /// label that contains the word "rotate"
    ///
    /// So far this is only needed in win32 platform due to some unknown issue
    /// that prevents those strings to be automatically translated. It works
    /// fine in linux, so there's no need there to explicitly call to gettext
    void ForceTranslationOfWidgets();

    // prevent the default constructors to be used
    Game1v1ConfigDialog();
    Game1v1ConfigDialog(const Game1v1ConfigDialog &a_src);
    Game1v1ConfigDialog& operator=(const Game1v1ConfigDialog &a_src);
};

#endif /* _GUI_GAME1V1_CONFIG_DIALOG_H_ */
