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
/// @file  dialog_newgame.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 02-Oct-2010  Original development
///           Faustino Frechilla 17-Nov-2010  4players game
///           Faustino Frechilla 22-Nov-2010  Split up contents in several files
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#ifndef _GUI_DIALOG_NEWGAME_H_
#define _GUI_DIALOG_NEWGAME_H_

#include <gtkmm.h>
#include "blockem_config.h"
#include "blockem_challenge.h"
#include "heuristic.h"
#include "gui/gui_exception.h"
#include "gui/dialog_newgame_widget.h"
#include "gui/dialog_newgame_widget_1v1.h"
#include "gui/dialog_newgame_widget_4players.h"
#include "gui/dialog_newgame_widget_challenge.h"
#include "gui/dialog_newgame_widget_totalallocation.h"


/// @brief the new game dialog!!
class DialogNewGame :
    public Gtk::Dialog
{
public:
    // to be used with m_gtkBuilder->get_widget_derived(GUI_DIALOG_NEW_GAME_NAME, m_newDialog);
    DialogNewGame(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) throw (GUIException);
    virtual ~DialogNewGame();

    /// saves all the configuration showed in the widgets related to a_gameType
    /// into the corresponding config singleton
    void SaveCurrentConfigIntoGlobalSettings(e_blockemGameType_t a_gameType) const;

    /// override Dialog::run. It will call Dialog::run internally to show the dialog on the screen
    int run();

    /// @return type of game currently selected
    e_blockemGameType_t GetSelectedTypeOfGame();

    /// @return currenttle selected blockem challenge
    const BlockemChallenge& GetCurrentBlockemChallenge() const;

private:

    // Tree model columns:
    //
    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:

        ModelColumns()
        {
            add(m_col_gametype);
            add(m_col_newgame_table_widget);
            add(m_col_description);
            add(m_col_pixbuf);
        }

        Gtk::TreeModelColumn<e_blockemGameType_t> m_col_gametype;
        Gtk::TreeModelColumn<NewGameTable*> m_col_newgame_table_widget;
        Gtk::TreeModelColumn<Glib::ustring>  m_col_description;
        Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_col_pixbuf;
    };

    // columns model
    ModelColumns m_modelColumns;

    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gtk::Builder> m_gtkBuilder;

    // widgets
    Glib::RefPtr<Gtk::IconView>  m_typeGameIconView;
    Glib::RefPtr<Gtk::ListStore> m_typeGameIconViewModel;
    Gtk::TreeModel::Path         m_currentSelectedPath;

    // the boxes with widgets to set up new games
    NewGameTable1v1*             m_newGameTable1v1;
    NewGameTableTotalAllocation* m_newGameTableTotalAllocation;
    NewGameTableChallenge*       m_newGameTableChallenge;
    NewGameTable4Players*        m_newGameTable4Players;

    /// double click (or click + enter) on an item of the icon view
    void IconView_on_item_activated(const Gtk::TreeModel::Path& path);

    /// click (single) on an item of the icon view. Item gets selected
    void IconView_on_selection_changed();

    /// Calls gettext per every static widget in the dialog. These strings
    /// are those ones included in the .glade file that never change during the
    /// execution of the application, for example a menu called "Game", or a
    /// label that contains the word "rotate"
    ///
    /// So far this is only needed in win32 platform due to some unknown issue
    /// that prevents those strings to be automatically translated. It works
    /// fine in linux, so there's no need there to explicitly call to gettext
    void ForceTranslationOfWidgets();

    /// Adds an entry to the icon view
    void AddEntry(
        e_blockemGameType_t  a_gametype,
        NewGameTable*        a_newGameTableWidget,
        const std::string&   a_imgFilename,
        const Glib::ustring& a_description);

    // prevent the default constructors to be used
    DialogNewGame();
    DialogNewGame(const DialogNewGame &a_src);
    DialogNewGame& operator=(const DialogNewGame &a_src);
};

#endif /* _GUI_DIALOG_NEWGAME_H_ */
