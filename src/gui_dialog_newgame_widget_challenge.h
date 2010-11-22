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
/// @file  gui_dialog_newgame_widget_challenge.h
/// @brief 
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 22-Nov-2010  Original development. Moved from gui_dialog_newgame.h
/// @endhistory
///
// ============================================================================

#ifndef _GUI_DIALOG_NEWGAME_WIDGET_CHALLENGE_H_
#define _GUI_DIALOG_NEWGAME_WIDGET_CHALLENGE_H_

#include <gtkmm.h>
#include "gui_dialog_newgame_widget.h"
#include "gui_exception.h"
#include "blockem_challenge.h"
#include "coordinate.h"

/// @brief table to be shown on the new game dialog when the user selects
///        a challenge game
class NewGameTableChallenge :
    public NewGameTable
{
public:
    // to be used with m_gtkBuilder->get_widget_derived(GUI_NEWGAME_CHALLENGE_HBOX, m_table);
    NewGameTableChallenge(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException);
    virtual ~NewGameTableChallenge();

    /// @brief load current global configuration into the widgets
    virtual void LoadCurrentConfigFromGlobalSettings();
    /// @brief save info saved in the widgets into current global configuration
    virtual void SaveCurrentConfigIntoGlobalSettings() const;

    /// @return currenttle selected blockem challenge
    const BlockemChallenge& GetCurrentBlockemChallenge() const;

private:
    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gtk::Builder> m_gtkBuilder;

    // Tree model columns:
    //
    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:

        ModelColumns()
        {
            add(m_blockem_challenge);
            add(m_col_challengename);
        }

        Gtk::TreeModelColumn<BlockemChallenge> m_blockem_challenge;
        Gtk::TreeModelColumn<Glib::ustring>  m_col_challengename;
    };

    // columns model
    ModelColumns m_modelColumns;
    // tree views for the list of challenges
    Glib::RefPtr<Gtk::IconView>  m_treeViewListOfChallenges;
    Glib::RefPtr<Gtk::ListStore> m_treeViewListOfChallengesModel;
    std::set<Glib::ustring>      m_treeViewListOfChallengesNamesSet;
    Gtk::TreeModel::Path         m_currentSelectedPath;

    // radio buttons and file chooser
    Gtk::RadioButton* m_radioButtonList;
    Gtk::RadioButton* m_radioButtonFileChooser;
    Gtk::FileChooserButton* m_buttonChallengeFileChooser;

    // challenge's misc info
    Gtk::Entry*    m_nRowsEntry;
    Gtk::Entry*    m_nColsEntry;
    Gtk::Entry*    m_nTakenSquaresEntry;
    Gtk::Entry*    m_nTakenSquaresChallengerEntry;
    Gtk::Entry*    m_authorEntry;
    Gtk::Entry*    m_emailEntry;
    Gtk::TextView* m_descriptionTextView;

    // labels loaded here because they need to be translated at startup on win32 platform
    // have a look at ForceTranslationOfWidgets
    Gtk::Label* m_nRowsLabel;
    Gtk::Label* m_nColsLabel;
    Gtk::Label* m_nTakenSquaresLabel;
    Gtk::Label* m_nTakenSquaresChallengerLabel;
    Gtk::Label* m_infoAuthorLabel;
    Gtk::Label* m_infoEmailLabel;
    Gtk::Label* m_infoDescriptionLabel;

    // current selected challenge by either the tree view or the file chooser
    BlockemChallenge m_currentSelectedChallenge;

    // signal handlers
    void RadioButtonBuiltInList_Toggled();
    void RadioButtonFileChooser_Toggled();
    void ChallengeList_on_selection_changed();
    void ChallengeFileChooser_on_file_set();

    /// @brief updates list of built-in challenges
    void UpdateBuiltInChallengesList();

    /// @brief loads selected challenge's info into the info widgets
    void CurrentChallengeToWidgets();

    /// Calls gettext per every static widget in the dialog. These strings
    /// are those ones included in the .glade file that never change during the
    /// execution of the application, for example a menu called "Game", or a
    /// label that contains the word "rotate"
    ///
    /// So far this is only needed in win32 platform due to some unknown issue
    /// that prevents those strings to be automatically translated. It works
    /// fine in linux, so there's no need there to explicitly call to gettext
    void ForceTranslationOfWidgets();

    NewGameTableChallenge();
    NewGameTableChallenge(const NewGameTableChallenge &a_src);
    NewGameTableChallenge& operator=(const NewGameTableChallenge &a_src);

}; // class NewGameTableChallenge

#endif /* _GUI_DIALOG_NEWGAME_WIDGET_CHALLENGE_H_ */
