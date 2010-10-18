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
///           Faustino Frechilla 18-Oct-2010  default buttons
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif

#include "gui_dialog_newgame.h"
#include "gettext.h" // i18n
#include "game1v1.h"
#include "gui_game1v1_config.h"
#include "gui_game_total_allocation_config.h"
#include "gui_glade_defs.h"

// strings to describe each type of game
static const char GAME_1V1_NAME[]         = N_("1 vs 1 Game (2 players)");
static const char GAME_TOTAL_ALLOC_NAME[] = N_("Total deployment (solo play)");

// strings for the user to choose the type of player in game 1v1
static const char COMBO_PLAYER_TYPE_HUMAN[]    = N_("Human");
static const char COMBO_PLAYER_TYPE_COMPUTER[] = N_("Computer");


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


    // set up icon view to store the type of games. Based on:
    // http://git.gnome.org/browse/gtkmm-documentation/tree/examples/book/iconview
    m_typeGameIconViewModel = Gtk::ListStore::create(m_modelColumns);
    m_typeGameIconViewModel->set_sort_column(
            m_modelColumns.m_col_description, Gtk::SORT_ASCENDING);

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



//////////////////////////////////////////////////////////////////////////////
// NewGameTableTotalAllocation methods

// maximum and minimum values for size of the total allocation board
static const double MINIMUM_TOTAL_ALLOC_BOARD_NROWS = 1;
static const double MAXIMUM_TOTAL_ALLOC_BOARD_NROWS = 99;
static const double MINIMUM_TOTAL_ALLOC_BOARD_NCOLS = 1;
static const double MAXIMUM_TOTAL_ALLOC_BOARD_NCOLS = 99;

NewGameTableTotalAllocation::NewGameTableTotalAllocation(
    BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder)  throw (GUIException) :
        NewGameTable(cobject), //Calls the base class constructor
        m_gtkBuilder(a_gtkBuilder),
        m_spinbuttonStartingRowAdj(
            (GameTotalAllocationConfig::Instance().IsStartingCoordSet()) ?
                    // +1 'cause coords are shown to the user starting by 1 (not 0)
                    GameTotalAllocationConfig::Instance().GetStartingCoord().m_row + 1 : 1,
            1,
            GameTotalAllocationConfig::Instance().GetNRows()),
        m_spinbuttonStartingColumnAdj(
            (GameTotalAllocationConfig::Instance().IsStartingCoordSet()) ?
                    // +1 'cause coords are shown to the user starting by 1 (not 0)
                    GameTotalAllocationConfig::Instance().GetStartingCoord().m_col + 1 : 1,
            1,
            GameTotalAllocationConfig::Instance().GetNColumns()),
        m_spinbuttonNRowsAdj(
            GameTotalAllocationConfig::Instance().GetNRows(),
            MINIMUM_TOTAL_ALLOC_BOARD_NROWS,
            MAXIMUM_TOTAL_ALLOC_BOARD_NROWS),
        m_spinbuttonNColsAdj(
            GameTotalAllocationConfig::Instance().GetNColumns(),
            MINIMUM_TOTAL_ALLOC_BOARD_NCOLS,
            MAXIMUM_TOTAL_ALLOC_BOARD_NCOLS)
{
    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_SPINBUTTON_STARTROW, m_spinbuttonStartingRow);
    if (m_spinbuttonStartingRow == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_SPINBUTTON_STARTCOL, m_spinbuttonStartingColumn);
    if (m_spinbuttonStartingColumn == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_SPINBUTTON_NROWS, m_spinbuttonNRows);
    if (m_spinbuttonNRows == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_SPINBUTTON_NCOLS, m_spinbuttonNCols);
    if (m_spinbuttonNCols == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_LABEL_BOARD, m_boardSizeLabel);
    if (m_boardSizeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_LABEL_STARTINGCOORD, m_startingPosLabel);
    if (m_startingPosLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_LABEL_STARTROW, m_startingRowLabel);
    if (m_spinbuttonNRows == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_LABEL_STARTCOL, m_startingColLabel);
    if (m_spinbuttonNCols == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_LABEL_NROWS, m_nRowsLabel);
    if (m_boardSizeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_LABEL_NCOLS, m_nColsLabel);
    if (m_startingPosLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_TOTALALLOC_CHECKBUTTON_START, m_checkbuttonStartFromAnywhere);
    if (m_checkbuttonStartFromAnywhere == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();

    // adjustments for spinbuttons
    m_spinbuttonStartingRow->set_adjustment(m_spinbuttonStartingRowAdj);
    m_spinbuttonStartingColumn->set_adjustment(m_spinbuttonStartingColumnAdj);
    m_spinbuttonNRows->set_adjustment(m_spinbuttonNRowsAdj);
    m_spinbuttonNCols->set_adjustment(m_spinbuttonNColsAdj);

    // connect the signals
    m_spinbuttonNRows->signal_value_changed().connect(
        sigc::mem_fun(*this, &NewGameTableTotalAllocation::SpinButtonNRows_SignalValueChanged));
    m_spinbuttonNCols->signal_value_changed().connect(
        sigc::mem_fun(*this, &NewGameTableTotalAllocation::SpinButtonNCols_SignalValueChanged));
    m_checkbuttonStartFromAnywhere->signal_toggled().connect(
        sigc::mem_fun(*this, &NewGameTableTotalAllocation::CheckbuttonStartFromAnywhere_Toggled));

}

NewGameTableTotalAllocation::~NewGameTableTotalAllocation()
{
}

void NewGameTableTotalAllocation::SpinButtonNRows_SignalValueChanged()
{
    int32_t currentNRows = static_cast<int32_t>(m_spinbuttonNRowsAdj.get_value());
    int32_t curentNCols = static_cast<int32_t>(m_spinbuttonNColsAdj.get_value());
    int32_t currentStartingRow = static_cast<int32_t>(m_spinbuttonStartingRowAdj.get_value());

    m_spinbuttonStartingRowAdj.set_upper(currentNRows);
    if (currentStartingRow > currentNRows)
    {
        m_spinbuttonStartingRowAdj.set_value(currentNRows);
    }
}

void NewGameTableTotalAllocation::SpinButtonNCols_SignalValueChanged()
{
    int32_t currentNRows = static_cast<int32_t>(m_spinbuttonNRowsAdj.get_value());
    int32_t curentNCols = static_cast<int32_t>(m_spinbuttonNColsAdj.get_value());
    int32_t currentStartingCol = static_cast<int32_t>(m_spinbuttonStartingColumnAdj.get_value());

    m_spinbuttonStartingColumnAdj.set_upper(curentNCols);
    if (currentStartingCol > curentNCols)
    {
        m_spinbuttonStartingColumnAdj.set_value(curentNCols);
    }
}

void NewGameTableTotalAllocation::CheckbuttonStartFromAnywhere_Toggled()
{
    if (m_checkbuttonStartFromAnywhere->property_active())
    {
        m_spinbuttonStartingRow->set_sensitive(false);
        m_spinbuttonStartingColumn->set_sensitive(false);
    }
    else
    {
        m_spinbuttonStartingRow->set_sensitive(true);
        m_spinbuttonStartingColumn->set_sensitive(true);
    }
}

void NewGameTableTotalAllocation::GetStartingCoord(Coordinate &a_coord) const
{
    if (m_checkbuttonStartFromAnywhere->property_active())
    {
        // uninitialised coord
        a_coord = Coordinate();
    }
    else
    {
        // -1 because coordinates are shown to the user starting by 1 (not 0)
        a_coord = Coordinate(
                    static_cast<int32_t>(m_spinbuttonStartingRowAdj.get_value())    - 1,
                    static_cast<int32_t>(m_spinbuttonStartingColumnAdj.get_value()) - 1);
    }
}

void NewGameTableTotalAllocation::SaveCurrentConfigIntoGlobalSettings() const
{
    // retrieve user settings from dialog and use them to set up global configuration

    Coordinate startingCoord;
    GetStartingCoord(startingCoord);
    GameTotalAllocationConfig::Instance().SetStartingCoord(startingCoord);

    GameTotalAllocationConfig::Instance().SetNRows(
            static_cast<int32_t>(m_spinbuttonNRowsAdj.get_value()));
    GameTotalAllocationConfig::Instance().SetNColumns(
        static_cast<int32_t>(m_spinbuttonNColsAdj.get_value()));
}

void NewGameTableTotalAllocation::LoadCurrentConfigFromGlobalSettings()
{
    // load current global configuration into the widgets

    m_spinbuttonNRowsAdj.set_value(
            GameTotalAllocationConfig::Instance().GetNRows());
    m_spinbuttonNColsAdj.set_value(
            GameTotalAllocationConfig::Instance().GetNColumns());

    if (GameTotalAllocationConfig::Instance().IsStartingCoordSet())
    {
        m_checkbuttonStartFromAnywhere->set_active(false);

        m_spinbuttonStartingRowAdj.set_value(
                GameTotalAllocationConfig::Instance().GetStartingCoord().m_row);
        m_spinbuttonStartingColumnAdj.set_value(
                GameTotalAllocationConfig::Instance().GetStartingCoord().m_col);
    }
    else
    {
        m_checkbuttonStartFromAnywhere->set_active(true);

        m_spinbuttonStartingRowAdj.set_value(MINIMUM_TOTAL_ALLOC_BOARD_NROWS);
        m_spinbuttonStartingColumnAdj.set_value(MINIMUM_TOTAL_ALLOC_BOARD_NCOLS);
    }
}

#ifdef WIN32
void NewGameTableTotalAllocation::ForceTranslationOfWidgets()
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

    m_boardSizeLabel->set_text(
            _(m_boardSizeLabel->get_text().c_str()) );

    m_startingPosLabel->set_text(
            _(m_startingPosLabel->get_text().c_str()) );

    m_startingRowLabel->set_text(
            _(m_startingRowLabel->get_text().c_str()) );

    m_startingColLabel->set_text(
            _(m_startingColLabel->get_text().c_str()) );

    m_nRowsLabel->set_text(
            _(m_nRowsLabel->get_text().c_str()) );

    m_nColsLabel->set_text(
            _(m_nColsLabel->get_text().c_str()) );

    m_checkbuttonStartFromAnywhere->set_label(
            _(m_checkbuttonStartFromAnywhere->get_label().c_str()));
}
#else
void NewGameTableTotalAllocation::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32



//////////////////////////////////////////////////////////////////////////////
// NewGameTable1v1 methods

// maximum and minimum coordinate values for the starting coordinate spinbuttons
// they start by 1.0 'cos is more user friendly than the c-style 0
static const double MINIMUM_GAME1V1_STARTING_COORD_ROW = 1;
static const double MAXIMUM_GAME1V1_STARTING_COORD_ROW = BOARD_1VS1_ROWS;
static const double MINIMUM_GAME1V1_STARTING_COORD_COL = 1;
static const double MAXIMUM_GAME1V1_STARTING_COORD_COL = BOARD_1VS1_COLUMNS;

NewGameTable1v1::NewGameTable1v1(
    BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder)  throw (GUIException) :
        NewGameTable(cobject), //Calls the base class constructor
        m_gtkBuilder(a_gtkBuilder),
        m_spinbuttonStartingRowPlayer1Adj(
            Game1v1Config::Instance().GetPlayer1StartingCoord().m_row + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            MINIMUM_GAME1V1_STARTING_COORD_ROW,
            MAXIMUM_GAME1V1_STARTING_COORD_ROW),
        m_spinbuttonStartingColumnPlayer1Adj(
            Game1v1Config::Instance().GetPlayer1StartingCoord().m_col + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            MINIMUM_GAME1V1_STARTING_COORD_COL,
            MAXIMUM_GAME1V1_STARTING_COORD_COL),
        m_spinbuttonStartingRowPlayer2Adj(
            Game1v1Config::Instance().GetPlayer2StartingCoord().m_row + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            MINIMUM_GAME1V1_STARTING_COORD_ROW,
            MAXIMUM_GAME1V1_STARTING_COORD_ROW),
        m_spinbuttonStartingColumnPlayer2Adj(
            Game1v1Config::Instance().GetPlayer2StartingCoord().m_col + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            MINIMUM_GAME1V1_STARTING_COORD_COL,
            MAXIMUM_GAME1V1_STARTING_COORD_COL),
        m_spinbuttonDepthPlayer1Adj(0.0, 0.0, e_numberOfPieces * 2), // maximum depth is putting down all the pieces
        m_spinbuttonDepthPlayer2Adj(0.0, 0.0, e_numberOfPieces * 2)  // maximum depth is putting down all the pieces
{
    // retrieve widgets from the .glade file
    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_TABLE_PLAYER1, m_tablePlayer1);
    if (m_tablePlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_TABLE_PLAYER2, m_tablePlayer2);
    if (m_tablePlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_SPINBUTTON_STARTROW1, m_spinbuttonStartingRowPlayer1);
    if (m_spinbuttonStartingRowPlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_SPINBUTTON_STARTCOL1, m_spinbuttonStartingColumnPlayer1);
    if (m_spinbuttonStartingColumnPlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_SPINBUTTON_STARTROW2, m_spinbuttonStartingRowPlayer2);
    if (m_spinbuttonStartingRowPlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_SPINBUTTON_STARTCOL2, m_spinbuttonStartingColumnPlayer2);
    if (m_spinbuttonStartingColumnPlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_AI_FRAME_PLAYER1, m_AIFramePlayer1);
    if (m_AIFramePlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_AI_FRAME_PLAYER2, m_AIFramePlayer2);
    if (m_AIFramePlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_AI_TABLE_PLAYER1, m_AITablePlayer1);
    if (m_AITablePlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_AI_TABLE_PLAYER2, m_AITablePlayer2);
    if (m_AITablePlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_AI_SPINBUTTON_DEPTH1, m_spinbuttonDepthPlayer1);
    if (m_spinbuttonDepthPlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_AI_SPINBUTTON_DEPTH2, m_spinbuttonDepthPlayer2);
    if (m_spinbuttonDepthPlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_AI_TEXTVIEW_HEURISTIC1, m_textViewHeuristic1);
    if (m_textViewHeuristic1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_AI_TEXTVIEW_HEURISTIC2, m_textViewHeuristic2);
    if (m_textViewHeuristic2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER1_LABEL, m_player1Label);
    if (m_player1Label == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER2_LABEL, m_player2Label);
    if (m_player2Label == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER1_TYPE_LABEL, m_player1TypeLabel);
    if (m_player1TypeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER2_TYPE_LABEL, m_player2TypeLabel);
    if (m_player2TypeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER1_STARTING_ROW_LABEL, m_player1StartingRowLabel);
    if (m_player1StartingRowLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER2_STARTING_ROW_LABEL, m_player2StartingRowLabel);
    if (m_player2StartingRowLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER1_STARTING_COL_LABEL, m_player1StartingColLabel);
    if (m_player1StartingColLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER2_STARTING_COL_LABEL, m_player2StartingColLabel);
    if (m_player2StartingColLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER1_AI_HEADER_LABEL, m_player1AIHeaderLabel);
    if (m_player1AIHeaderLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER2_AI_HEADER_LABEL, m_player2AIHeaderLabel);
    if (m_player2AIHeaderLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER1_AI_TYPE_LABEL, m_player1AITypeLabel);
    if (m_player1AITypeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER2_AI_TYPE_LABEL, m_player2AITypeLabel);
    if (m_player2AITypeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER1_AI_DEPTH_LABEL, m_player1AIDepthLabel);
    if (m_player1AIDepthLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_1V1_PLAYER2_AI_DEPTH_LABEL, m_player2AIDepthLabel);
    if (m_player2AIDepthLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }


    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();

    // adjustments for starting coordinate spinbuttons
    m_spinbuttonStartingRowPlayer1->set_adjustment(m_spinbuttonStartingRowPlayer1Adj);
    m_spinbuttonStartingColumnPlayer1->set_adjustment(m_spinbuttonStartingColumnPlayer1Adj);
    m_spinbuttonStartingRowPlayer2->set_adjustment(m_spinbuttonStartingRowPlayer2Adj);
    m_spinbuttonStartingColumnPlayer2->set_adjustment(m_spinbuttonStartingColumnPlayer2Adj);

    // adjustments for search tree depth
    m_spinbuttonDepthPlayer1->set_adjustment(m_spinbuttonDepthPlayer1Adj);
    m_spinbuttonDepthPlayer2->set_adjustment(m_spinbuttonDepthPlayer2Adj);

    // configure custom widgets
    // type of player1
    m_comboTypePlayer1.insert_text( 0, _(COMBO_PLAYER_TYPE_HUMAN) );
    m_comboTypePlayer1.append_text( _(COMBO_PLAYER_TYPE_COMPUTER) );
    m_comboTypePlayer1.set_active_text( _(COMBO_PLAYER_TYPE_HUMAN) );
    //type of player2
    m_comboTypePlayer2.insert_text( 0, _(COMBO_PLAYER_TYPE_HUMAN) );
    m_comboTypePlayer2.append_text( _(COMBO_PLAYER_TYPE_COMPUTER) );
    m_comboTypePlayer2.set_active_text( _(COMBO_PLAYER_TYPE_COMPUTER) );
    // heuristics
    for (int32_t i = Heuristic::e_heuristicStartCount;
         i < Heuristic::e_heuristicCount;
         i++)
    {
        const Heuristic::sHeuristicData_t &heuristicData =
            Heuristic::m_heuristicData[static_cast<Heuristic::eHeuristicType_t>(i)];

        m_comboHeuristicPlayer1.append_text(_(heuristicData.m_name));
        m_comboHeuristicPlayer2.append_text(_(heuristicData.m_name));

        // fill the TextBuffers with the descriptions
        m_refHeuristicDescriptionBuffer[i] = Gtk::TextBuffer::create();
        m_refHeuristicDescriptionBuffer[i]->set_text(_(heuristicData.m_description));
    }

    const Heuristic::sHeuristicData_t &selectedHeuristicData1 =
        Heuristic::m_heuristicData[Game1v1Config::Instance().GetHeuristicTypePlayer1()];
    m_comboHeuristicPlayer1.set_active_text(_(selectedHeuristicData1.m_name));
    m_textViewHeuristic1->set_buffer(m_refHeuristicDescriptionBuffer[selectedHeuristicData1.m_type]);

    const Heuristic::sHeuristicData_t &selectedHeuristicData2 =
        Heuristic::m_heuristicData[Game1v1Config::Instance().GetHeuristicTypePlayer2()];
    m_comboHeuristicPlayer2.set_active_text(_(selectedHeuristicData2.m_name));
    m_textViewHeuristic2->set_buffer(m_refHeuristicDescriptionBuffer[selectedHeuristicData2.m_type]);

    // attach custom widgets (the ones not present in the .glade file) into the dialog
    //    void Gtk::Table::attach (
    //            Widget&      child,
    //            guint   left_attach,
    //            guint   right_attach,
    //            guint   top_attach,
    //            guint   bottom_attach,
    //            AttachOptions   xoptions = FILL|EXPAND,
    //            AttachOptions   yoptions = FILL|EXPAND,
    //            guint   xpadding = 0,
    //            guint   ypadding = 0 )
    m_tablePlayer1->attach(
            m_comboTypePlayer1,
            1,
            4,
            0,
            1,
            Gtk::FILL | Gtk::EXPAND,
            Gtk::EXPAND,
            0,
            0);
    m_comboTypePlayer1.show();

    m_tablePlayer2->attach(
            m_comboTypePlayer2,
            1,
            4,
            0,
            1,
            Gtk::FILL | Gtk::EXPAND,
            Gtk::EXPAND,
            0,
            0);
    m_comboTypePlayer2.show();

    //m_comboHeuristicPlayer1 and m_comboHeuristicPlayer2
    m_AITablePlayer1->attach(
            m_comboHeuristicPlayer1,
            0,
            2,
            1,
            2,
            Gtk::FILL | Gtk::EXPAND,
            Gtk::FILL,
            0,
            0);
    m_comboHeuristicPlayer1.show();

    m_AITablePlayer2->attach(
            m_comboHeuristicPlayer2,
            0,
            2,
            1,
            2,
            Gtk::FILL | Gtk::EXPAND,
            Gtk::FILL,
            0,
            0);
    m_comboHeuristicPlayer2.show();

    //signal handling
    m_comboTypePlayer1.signal_changed().connect(
            sigc::mem_fun(*this, &NewGameTable1v1::ComboPlayer1Type_signalChanged));
    m_comboTypePlayer2.signal_changed().connect(
            sigc::mem_fun(*this, &NewGameTable1v1::ComboPlayer2Type_signalChanged));
    m_comboHeuristicPlayer1.signal_changed().connect(
            sigc::mem_fun(*this, &NewGameTable1v1::ComboHeuristicPlayer1_signalChanged));
    m_comboHeuristicPlayer2.signal_changed().connect(
            sigc::mem_fun(*this, &NewGameTable1v1::ComboHeuristicPlayer2_signalChanged));

    m_spinbuttonDepthPlayer1->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable1v1::SpinButtonDepthPlayer1_SignalValueChanged));
    m_spinbuttonDepthPlayer2->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable1v1::SpinButtonDepthPlayer2_SignalValueChanged));
}

NewGameTable1v1::~NewGameTable1v1()
{
}

bool NewGameTable1v1::on_expose_event (GdkEventExpose* event)
{
    // workaround to show the "Auto" string in the depth spin buttons
    // m_spinbuttonDepthPlayer1->set_text("Auto") doesn't work until
    // the dialog is shown
    static bool firstTime = false;
    if (firstTime == false)
    {
        firstTime = true;
        m_spinbuttonDepthPlayer1Adj.value_changed();
        m_spinbuttonDepthPlayer2Adj.value_changed();
    }

    return Gtk::HBox::on_expose_event(event);
}

void NewGameTable1v1::SpinButtonDepthPlayer1_SignalValueChanged()
{
    if (static_cast<int32_t>(m_spinbuttonDepthPlayer1Adj.get_value()) == 0)
    {
        if (m_spinbuttonDepthPlayer1->get_numeric() == true)
        {
            m_spinbuttonDepthPlayer1->set_numeric(false);
        }

        // i18n This string shouldn't be bigger than 4 characters long
        // i18n strings bigger than those 4 characters will be stripped out
        // i18n Thank you for contributing to this project
        m_spinbuttonDepthPlayer1->set_text(_("Auto"));
    }
    else if (m_spinbuttonDepthPlayer1->get_numeric() == false)
    {
        m_spinbuttonDepthPlayer1->set_numeric(true);
    }
}

void NewGameTable1v1::SpinButtonDepthPlayer2_SignalValueChanged()
{
    if (static_cast<int32_t>(m_spinbuttonDepthPlayer2Adj.get_value()) == 0)
    {
        if (m_spinbuttonDepthPlayer2->get_numeric() == true)
        {
            m_spinbuttonDepthPlayer2->set_numeric(false);
        }

        // i18n This string shouldn't be bigger than 4 characters long
        // i18n strings bigger than those 4 characters will be stripped out
        // i18n Thank you for contributing to this project
        m_spinbuttonDepthPlayer2->set_text(_("Auto"));
    }
    else if (m_spinbuttonDepthPlayer2->get_numeric() == false)
    {
        m_spinbuttonDepthPlayer2->set_numeric(true);
    }
}

void NewGameTable1v1::ComboPlayer1Type_signalChanged()
{
    if (IsPlayer1TypeComputer())
    {
        m_AIFramePlayer1->set_sensitive(true);
    }
    else
    {
        m_AIFramePlayer1->set_sensitive(false);
    }
}

void NewGameTable1v1::ComboPlayer2Type_signalChanged()
{
    if (IsPlayer2TypeComputer())
    {
        m_AIFramePlayer2->set_sensitive(true);
    }
    else
    {
        m_AIFramePlayer2->set_sensitive(false);
    }
}

void NewGameTable1v1::ComboHeuristicPlayer1_signalChanged()
{
    const Heuristic::sHeuristicData_t &heuristicData =
                Heuristic::m_heuristicData[GetPlayer1Heuristic()];

    m_textViewHeuristic1->set_buffer(
            m_refHeuristicDescriptionBuffer[heuristicData.m_type]);

    // this old depth will save what the depth was before
    // the random heuristic was selected to use that value when the random
    // heuristic is unselected again. -1 is a magic number that can't be
    // entered by the user according to MINIMUM_STARTING_COORD_ROW
    // and MINIMUM_STARTING_COORD_COL
    static int32_t oldDepth = -1;
    if (heuristicData.m_type == Heuristic::e_heuristicRandom)
    {
        oldDepth = m_spinbuttonDepthPlayer1Adj.get_value();

        m_spinbuttonDepthPlayer1Adj.set_value(1);
        m_spinbuttonDepthPlayer1->set_sensitive(false);
    }
    else
    {
        if (oldDepth != -1)
        {
            m_spinbuttonDepthPlayer1Adj.set_value(oldDepth);
            oldDepth = -1;
        }
        m_spinbuttonDepthPlayer1->set_sensitive(true);
    }

}

void NewGameTable1v1::ComboHeuristicPlayer2_signalChanged()
{
    const Heuristic::sHeuristicData_t &heuristicData =
                Heuristic::m_heuristicData[GetPlayer2Heuristic()];

    m_textViewHeuristic2->set_buffer(
            m_refHeuristicDescriptionBuffer[heuristicData.m_type]);

    // this old depth will save what the depth was before
    // the random heuristic was selected to use that value when the random
    // heuristic is unselected again. -1 is a magic number that can't be
    // entered by the user according to MINIMUM_STARTING_COORD_ROW
    // and MINIMUM_STARTING_COORD_COL
    static int32_t oldDepth = -1;

    if (heuristicData.m_type == Heuristic::e_heuristicRandom)
    {
        oldDepth = m_spinbuttonDepthPlayer2Adj.get_value();

        m_spinbuttonDepthPlayer2Adj.set_value(1);
        m_spinbuttonDepthPlayer2->set_sensitive(false);
    }
    else
    {
        if (oldDepth != -1)
        {
            m_spinbuttonDepthPlayer2Adj.set_value(oldDepth);
            oldDepth = -1;
        }
        m_spinbuttonDepthPlayer2->set_sensitive(true);
    }
}

bool NewGameTable1v1::IsPlayer1TypeComputer() const
{
    return (m_comboTypePlayer1.get_active_text().compare(_(COMBO_PLAYER_TYPE_COMPUTER)) == 0);
}

bool NewGameTable1v1::IsPlayer2TypeComputer() const
{
    return (m_comboTypePlayer2.get_active_text().compare(_(COMBO_PLAYER_TYPE_COMPUTER)) == 0);
}

void NewGameTable1v1::GetPlayer1StartingCoord(Coordinate &a_coord) const
{
    // -1 because coordinates are shown to the user starting by 1 (not 0)
    a_coord = Coordinate(
                static_cast<int32_t>(m_spinbuttonStartingRowPlayer1Adj.get_value())    - 1,
                static_cast<int32_t>(m_spinbuttonStartingColumnPlayer1Adj.get_value()) - 1);
}

void NewGameTable1v1::GetPlayer2StartingCoord(Coordinate &a_coord) const
{
    // -1 because coordinates are shown to the user starting by 1 (not 0)
    a_coord = Coordinate(
                static_cast<int32_t>(m_spinbuttonStartingRowPlayer2Adj.get_value())    - 1,
                static_cast<int32_t>(m_spinbuttonStartingColumnPlayer2Adj.get_value()) - 1);
}

int32_t NewGameTable1v1::GetPlayer1SearchTreeDepth() const
{
    return static_cast<uint32_t>(m_spinbuttonDepthPlayer1Adj.get_value());
}

int32_t NewGameTable1v1::GetPlayer2SearchTreeDepth() const
{
    return static_cast<uint32_t>(m_spinbuttonDepthPlayer2Adj.get_value());
}

Heuristic::eHeuristicType_t NewGameTable1v1::GetPlayer1Heuristic() const
{
    for (int32_t i = Heuristic::e_heuristicStartCount;
         i < Heuristic::e_heuristicCount;
         i++)
    {
        const Heuristic::sHeuristicData_t &heuristicData =
            Heuristic::m_heuristicData[static_cast<Heuristic::eHeuristicType_t>(i)];

        if (m_comboHeuristicPlayer1.get_active_text().compare(_(heuristicData.m_name)) == 0)
        {
            return static_cast<Heuristic::eHeuristicType_t>(i);
        }
    }

#ifdef DEBUG
    assert(0);
#endif
    return Heuristic::e_heuristicStartCount;
}

Heuristic::eHeuristicType_t NewGameTable1v1::GetPlayer2Heuristic() const
{
    for (int32_t i = Heuristic::e_heuristicStartCount;
         i < Heuristic::e_heuristicCount;
         i++)
    {
        const Heuristic::sHeuristicData_t &heuristicData =
            Heuristic::m_heuristicData[static_cast<Heuristic::eHeuristicType_t>(i)];

        if (m_comboHeuristicPlayer2.get_active_text().compare(_(heuristicData.m_name)) == 0)
        {
            return static_cast<Heuristic::eHeuristicType_t>(i);
        }
    }

#ifdef DEBUG
    assert(0);
#endif
    return Heuristic::e_heuristicStartCount;
}

void NewGameTable1v1::SaveCurrentConfigIntoGlobalSettings() const
{
    // retrieve user settings from dialog and use them to set up global configuration

    // type of players
    if (this->IsPlayer1TypeComputer())
    {
        Game1v1Config::Instance().SetPlayer1Type(Game1v1Config::e_playerComputer);
    }
    else
    {
        Game1v1Config::Instance().SetPlayer1Type(Game1v1Config::e_playerHuman);
    }

    if (this->IsPlayer2TypeComputer())
    {
        Game1v1Config::Instance().SetPlayer2Type(Game1v1Config::e_playerComputer);
    }
    else
    {
        Game1v1Config::Instance().SetPlayer2Type(Game1v1Config::e_playerHuman);
    }

    // starting coords
    Coordinate player1StartingCoord;
    this->GetPlayer1StartingCoord(player1StartingCoord);
    Coordinate player2StartingCoord;
    this->GetPlayer2StartingCoord(player2StartingCoord);
    Game1v1Config::Instance().SetPlayer1StartingCoord(player1StartingCoord);
    Game1v1Config::Instance().SetPlayer2StartingCoord(player2StartingCoord);

    // heuristic
    Game1v1Config::Instance().SetHeuristicTypePlayer1(this->GetPlayer1Heuristic());
    Game1v1Config::Instance().SetHeuristicTypePlayer2(this->GetPlayer2Heuristic());

    // search tree depth
    Game1v1Config::Instance().SetMinimaxDepthPlayer1(this->GetPlayer1SearchTreeDepth());
    Game1v1Config::Instance().SetMinimaxDepthPlayer2(this->GetPlayer2SearchTreeDepth());
}

void NewGameTable1v1::LoadCurrentConfigFromGlobalSettings()
{
    // load current global configuration into the widgets

    // type of players
    if (Game1v1Config::Instance().IsPlayer1Computer())
    {
        m_comboTypePlayer1.set_active_text( _(COMBO_PLAYER_TYPE_COMPUTER) );
    }
    else
    {
        m_comboTypePlayer1.set_active_text( _(COMBO_PLAYER_TYPE_HUMAN) );
    }

    if (Game1v1Config::Instance().IsPlayer2Computer())
    {
        m_comboTypePlayer2.set_active_text( _(COMBO_PLAYER_TYPE_COMPUTER) );
    }
    else
    {
        m_comboTypePlayer2.set_active_text( _(COMBO_PLAYER_TYPE_HUMAN) );
    }

    // starting coordinates. +1 because coordinates are shown to the user starting by 1 (not 0)
    m_spinbuttonStartingRowPlayer1Adj.set_value(
        Game1v1Config::Instance().GetPlayer1StartingCoord().m_row + 1);
    m_spinbuttonStartingColumnPlayer1Adj.set_value(
        Game1v1Config::Instance().GetPlayer1StartingCoord().m_col + 1);
    m_spinbuttonStartingRowPlayer2Adj.set_value(
        Game1v1Config::Instance().GetPlayer2StartingCoord().m_row + 1);
    m_spinbuttonStartingColumnPlayer2Adj.set_value(
        Game1v1Config::Instance().GetPlayer2StartingCoord().m_col + 1);

    // search tree depth
    m_spinbuttonDepthPlayer1Adj.set_value(Game1v1Config::Instance().GetMinimaxDepthPlayer1());
    m_spinbuttonDepthPlayer2Adj.set_value(Game1v1Config::Instance().GetMinimaxDepthPlayer2());

    // heuristic type
    const Heuristic::sHeuristicData_t &selectedHeuristicData1 =
        Heuristic::m_heuristicData[Game1v1Config::Instance().GetHeuristicTypePlayer1()];
    m_comboHeuristicPlayer1.set_active_text(_(selectedHeuristicData1.m_name));
    const Heuristic::sHeuristicData_t &selectedHeuristicData2 =
        Heuristic::m_heuristicData[Game1v1Config::Instance().GetHeuristicTypePlayer2()];
    m_comboHeuristicPlayer2.set_active_text(_(selectedHeuristicData2.m_name));
}

#ifdef WIN32
void NewGameTable1v1::ForceTranslationOfWidgets()
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

    m_player1Label->set_text(
            _(m_player1Label->get_text().c_str()) );

    m_player2Label->set_text(
            _(m_player2Label->get_text().c_str()) );

    m_player1TypeLabel->set_text(
            _(m_player1TypeLabel->get_text().c_str()) );

    m_player2TypeLabel->set_text(
            _(m_player2TypeLabel->get_text().c_str()) );

    m_player1StartingRowLabel->set_text(
            _(m_player1StartingRowLabel->get_text().c_str()) );

    m_player2StartingRowLabel->set_text(
            _(m_player2StartingRowLabel->get_text().c_str()) );

    m_player1StartingColLabel->set_text(
            _(m_player1StartingColLabel->get_text().c_str()) );

    m_player2StartingColLabel->set_text(
            _(m_player2StartingColLabel->get_text().c_str()) );

    m_player1AIHeaderLabel->set_text(
            _(m_player1AIHeaderLabel->get_text().c_str()) );

    m_player2AIHeaderLabel->set_text(
            _(m_player2AIHeaderLabel->get_text().c_str()) );

    m_player1AITypeLabel->set_text(
            _(m_player1AITypeLabel->get_text().c_str()) );

    m_player2AITypeLabel->set_text(
            _(m_player2AITypeLabel->get_text().c_str()) );

    m_player1AIDepthLabel->set_text(
            _(m_player1AIDepthLabel->get_text().c_str()) );

    m_player2AIDepthLabel->set_text(
            _(m_player2AIDepthLabel->get_text().c_str()) );
}
#else
void NewGameTable1v1::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
