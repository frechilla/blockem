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
/// @file  gui_dialog_newgame_widget_totalallocation.cpp
/// @brief NewGameTableTotalAllocation methods
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 22-Nov-2010  Original development. Moved from gui_dialog_newgame.cpp
/// @endhistory
///
// ============================================================================

#include "gui_dialog_newgame_widget_totalallocation.h"
#include "gettext.h" // i18n
#include "game_total_allocation.h"
#include "gui_game_total_allocation_config.h"
#include "gui_glade_defs.h"


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
    int32_t currentStartingRow = static_cast<int32_t>(m_spinbuttonStartingRowAdj.get_value());

    m_spinbuttonStartingRowAdj.set_upper(currentNRows);
    if (currentStartingRow > currentNRows)
    {
        m_spinbuttonStartingRowAdj.set_value(currentNRows);
    }
}

void NewGameTableTotalAllocation::SpinButtonNCols_SignalValueChanged()
{
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
    this->GetStartingCoord(startingCoord);
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

        //  adds 1 to every starting coord because coords are shown to the
        // user starting from 1, but they are stored internally starting from 0
        m_spinbuttonStartingRowAdj.set_value(
                GameTotalAllocationConfig::Instance().GetStartingCoord().m_row + 1);
        m_spinbuttonStartingColumnAdj.set_value(
                GameTotalAllocationConfig::Instance().GetStartingCoord().m_col + 1);
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
