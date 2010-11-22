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
/// @file  gui_dialog_newgame.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 02-Oct-2010  Original development
///           Faustino Frechilla 22-Nov-2010  Split up contents in several files
/// @endhistory
///
// ============================================================================

#if (defined DEBUG_PRINT) || (defined DEBUG)
#include <iostream>
#endif

#include "gui_dialog_newgame.h"
#include "gettext.h" // i18n
#include "gui_game_4players_config.h"
#include "gui_game_total_allocation_config.h"
#include "gui_game_challenge_config.h"
#include "gui_glade_defs.h"

// strings to describe each type of game
static const char GAME_1V1_NAME[]         = N_("1 vs 1 Game (2 players)");
static const char GAME_TOTAL_ALLOC_NAME[] = N_("Total deployment (solo play)");
static const char GAME_CHALLENGE_NAME[]   = N_("Challenge (solo play)");
static const char GAME_4PLAYERS_NAME[]    = N_("4 players game");

DialogNewGame::DialogNewGame(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder)  throw (GUIException) :
    Gtk::Dialog(cobject), //Calls the base class constructor
    m_gtkBuilder(a_gtkBuilder)
{
    // add ok and cancel buttons
#ifdef WIN32
    // win32 systems have ok/accept button at the left of the cancel button
    this->add_button(Gtk::StockID("gtk-ok"), Gtk::RESPONSE_OK);
    this->add_button(Gtk::StockID("gtk-cancel"), Gtk::RESPONSE_CANCEL);
#else
    // any other system will follow gnome GUI guidelines:
    // OK button at the right of the cancel button
    this->add_button(Gtk::StockID("gtk-cancel"), Gtk::RESPONSE_CANCEL);
    this->add_button(Gtk::StockID("gtk-ok"), Gtk::RESPONSE_OK);
#endif // OS_WIN32

    // retrieve widgets from the .glade file
    m_typeGameIconView = Glib::RefPtr<Gtk::IconView>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_NEWGAME_TYPE_ICONVIEW));
    if (!m_typeGameIconView)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // boxes with the actual widgets to configure new games
    m_gtkBuilder->get_widget_derived(GUI_NEWGAME_1V1_HBOX, m_newGameTable1v1);
    if (m_newGameTable1v1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget_derived(GUI_NEWGAME_TOTALALLOC_HBOX, m_newGameTableTotalAllocation);
    if (m_newGameTableTotalAllocation == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget_derived(GUI_NEWGAME_CHALLENGE_HBOX, m_newGameTableChallenge);
    if (m_newGameTableChallenge == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget_derived(GUI_NEWGAME_4PLAYERS_HBOX, m_newGameTable4Players);
    if (m_newGameTable4Players == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // set up icon view to store the type of games. Based on:
    // http://git.gnome.org/browse/gtkmm-documentation/tree/examples/book/iconview
    m_typeGameIconViewModel = Gtk::ListStore::create(m_modelColumns);
    // uncomment these lines if you want the gametype icon view to be ordered
    // by the description. Otherwise it will be ordered from firstly inserted to last
    //m_typeGameIconViewModel->set_sort_column(
    //        m_modelColumns.m_col_description, Gtk::SORT_ASCENDING);

    m_typeGameIconView->set_model(m_typeGameIconViewModel);
    m_typeGameIconView->set_markup_column(m_modelColumns.m_col_description);
    m_typeGameIconView->set_pixbuf_column(m_modelColumns.m_col_pixbuf);
    m_typeGameIconView->signal_selection_changed().connect(sigc::mem_fun(*this,
            &DialogNewGame::IconView_on_selection_changed)); // item selected by a single click
#ifdef DEBUG
    m_typeGameIconView->signal_item_activated().connect(sigc::mem_fun(*this,
            &DialogNewGame::IconView_on_item_activated)); // double click (or click + enter)
#endif

    // add now the elements to the icon view
    // if you'd like to add a new element to the gametype icon view, just add a new call
    // to AddEntry. The rest of the callbacks will take care of what widget will be shown
    AddEntry(
        e_gameType1vs1,
        m_newGameTable1v1,
        GUI_PATH_TO_NEWGAME_1VS1GAME,
        _(GAME_1V1_NAME));
    AddEntry(
        e_gameTypeTotalAllocation,
        m_newGameTableTotalAllocation,
        GUI_PATH_TO_NEWGAME_TOTALALLOC,
        _(GAME_TOTAL_ALLOC_NAME));
    AddEntry(
        e_gameTypeChallenge,
        m_newGameTableChallenge,
        GUI_PATH_TO_NEWGAME_CHALLENGE,
        _(GAME_CHALLENGE_NAME));
    AddEntry(
        e_gameType4Players,
        m_newGameTable4Players,
        GUI_PATH_TO_NEWGAME_4PLAYERS,
        _(GAME_4PLAYERS_NAME));

    // activate game1v1 as default
    Gtk::TreeNodeChildren allChildren = m_typeGameIconViewModel->children();
    if(!allChildren.empty())
    {
        Gtk::TreeModel::iterator iter = *allChildren.begin();
        // mark as selected ONLY the 1st one!
        Gtk::TreeModel::Path path = m_typeGameIconViewModel->get_path(iter);
        m_typeGameIconView->select_path(path);
        // save the current path so whenever the user clicks somewhere
        // where no rows get selected it can be used to restore the latest
        // selection
        // a row MUST be always selected
        m_currentSelectedPath = path;

#ifdef DEBUG
        int32_t nElems = 0;
        for (iter =  *allChildren.begin();
             iter != *allChildren.end();
             iter++)
        {
            nElems++;
        }
        std::cout << "Elements in Dialog New Game Tree View: " << nElems << std::endl;
#endif
    }


    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();
}

DialogNewGame::~DialogNewGame()
{
}

void DialogNewGame::AddEntry(
        e_blockemGameType_t  a_gametype,
        NewGameTable*        a_newGameTableWidget,
        const std::string&   a_imgFilename,
        const Glib::ustring& a_description)
{
    Gtk::TreeModel::Row row = *(m_typeGameIconViewModel->append());

    row[m_modelColumns.m_col_gametype] = a_gametype;
    row[m_modelColumns.m_col_newgame_table_widget] = a_newGameTableWidget;
    row[m_modelColumns.m_col_description] = a_description;

    // try to set the set the pixmap contained in a_imgFilename
    if (g_file_test(a_imgFilename.c_str(), G_FILE_TEST_IS_REGULAR))
    {
        Glib::RefPtr< Gdk::Pixbuf > picture;

        try
        {
            picture = Gdk::Pixbuf::create_from_file(a_imgFilename.c_str());
        }
        catch(...)
        {
            picture.reset();
#ifdef DEBUG_PRINT
            std::cerr
               << "WARNING: Exception occurred when setting an icon in the new game dialog. File: \""
               << a_imgFilename << "\""
               << std::endl;
#endif
        }

        if (picture)
        {
            row[m_modelColumns.m_col_pixbuf] = picture;
        }
    }
}

e_blockemGameType_t DialogNewGame::GetSelectedTypeOfGame()
{
    Gtk::TreeModel::iterator iter = m_typeGameIconViewModel->get_iter(m_currentSelectedPath);
    Gtk::TreeModel::Row row = *iter;
    e_blockemGameType_t gametype = row[m_modelColumns.m_col_gametype];

    return gametype;
}

int DialogNewGame::run()
{
    Gtk::TreeModel::iterator iter;
    Gtk::TreeModel::Row row;

    // load current global configuration into the widgets before showing
    // anything
    // also, hide those widgets that are not to be shown now
    Gtk::TreeNodeChildren allChildren = m_typeGameIconViewModel->children();
    if(!allChildren.empty())
    {
        for (iter =  *allChildren.begin();
             iter != *allChildren.end();
             iter++)
        {
            row = *iter;
            NewGameTable* newGameTableWidget =
                row[m_modelColumns.m_col_newgame_table_widget];

            // load global config into this widget
            newGameTableWidget->LoadCurrentConfigFromGlobalSettings();

            e_blockemGameType_t thisGameType = row[m_modelColumns.m_col_gametype];
            if (m_currentSelectedPath != thisGameType)
            {
                newGameTableWidget->hide_all();
            }
        }
    }

    // show currently selected new game table
    iter = m_typeGameIconViewModel->get_iter(m_currentSelectedPath);
    row = *iter;
    NewGameTable* currentGameTableWidget = row[m_modelColumns.m_col_newgame_table_widget];
    currentGameTableWidget->show_all();

    // run the Gtk dialog once the custom widgets are configured properly
    return Gtk::Dialog::run();
}

void DialogNewGame::IconView_on_selection_changed()
{
    // http://git.gnome.org/browse/gtkmm-documentation/tree/examples/book/iconview
    typedef std::list<Gtk::TreeModel::Path> type_list_paths;
    type_list_paths selected = m_typeGameIconView->get_selected_items();
    if(!selected.empty())
    {
        // save the last path picked so whenever the user clicked somewhere
        // where no rows get selected it can be used to restore the latest
        // selection
        m_currentSelectedPath = *selected.begin();

#ifdef DEBUG
        const Gtk::TreeModel::Path& path = *selected.begin();
        Gtk::TreeModel::iterator iter = m_typeGameIconViewModel->get_iter(path);
        Gtk::TreeModel::Row row = *iter;

        e_blockemGameType_t gametype = row[m_modelColumns.m_col_gametype];
        const Glib::ustring description = row[m_modelColumns.m_col_description];

        std::cout  << "Selection Changed to: type="
                   << gametype
                   << ", description="
                   << description
                   << std::endl;
#endif
    }
    else
    {
        // there must be a row ALWAYS selected. The user picked somewhere were no
        // rows got selected. Select back the last one picked by him/her
        m_typeGameIconView->select_path(m_currentSelectedPath);
    }

    Gtk::TreeModel::iterator iter = m_typeGameIconViewModel->get_iter(m_currentSelectedPath);
    Gtk::TreeModel::Row row = *iter;
    e_blockemGameType_t currentGametype  = row[m_modelColumns.m_col_gametype];
    NewGameTable* currentGameTableWidget = row[m_modelColumns.m_col_newgame_table_widget];

    Gtk::TreeNodeChildren allChildren = m_typeGameIconViewModel->children();
    if(!allChildren.empty())
    {
        // hide the ones that are not to be shown now
        for (iter =  *allChildren.begin();
             iter != *allChildren.end();
             iter++)
        {
            Gtk::TreeModel::Row row = *iter;
            e_blockemGameType_t thisGameType = row[m_modelColumns.m_col_gametype];
            if (currentGametype != thisGameType)
            {
                NewGameTable* newGameTableWidget = row[m_modelColumns.m_col_newgame_table_widget];
                newGameTableWidget->hide_all();
            }
        }
    }

    // use current game table widget to show the corresponding widgets
    currentGameTableWidget->show_all();
}

void DialogNewGame::SaveCurrentConfigIntoGlobalSettings(e_blockemGameType_t a_gameType) const
{
    // retrieve user settings from dialog and use them to set up global configuration
    // only save the one represented by a_gameType

    Gtk::TreeNodeChildren allChildren = m_typeGameIconViewModel->children();
    if(!allChildren.empty())
    {
        Gtk::TreeModel::iterator iter;
        for (iter =  *allChildren.begin();
             iter != *allChildren.end();
             iter++)
        {
            Gtk::TreeModel::Row row = *iter;
            e_blockemGameType_t gametype = row[m_modelColumns.m_col_gametype];
            if (gametype == a_gameType)
            {
                NewGameTable* newGameTableWidget = row[m_modelColumns.m_col_newgame_table_widget];
                newGameTableWidget->SaveCurrentConfigIntoGlobalSettings();
            }
        }
    }
}

const BlockemChallenge& DialogNewGame::GetCurrentBlockemChallenge() const
{
    return m_newGameTableChallenge->GetCurrentBlockemChallenge();
}

#ifdef DEBUG
void DialogNewGame::IconView_on_item_activated(const Gtk::TreeModel::Path& path)
{
    // http://git.gnome.org/browse/gtkmm-documentation/tree/examples/book/iconview
    Gtk::TreeModel::iterator iter = m_typeGameIconViewModel->get_iter(path);
    Gtk::TreeModel::Row row = *iter;

    e_blockemGameType_t gametype = row[m_modelColumns.m_col_gametype];
    const Glib::ustring description = row[m_modelColumns.m_col_description];

    std::cout  << "Item activated: type="
               << gametype
               << ", description="
               << description
               << std::endl;
}
#endif

#ifdef WIN32
void DialogNewGame::ForceTranslationOfWidgets()
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

    set_title( _(get_title().c_str()) );
}
#else
void DialogNewGame::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
