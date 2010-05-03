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
/// @file  gui_config_dialog.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 01-May-2010  Original development
/// @endhistory
///
// ============================================================================

#include <iostream> // std::cerr
#include "config.h" // PACKAGE_VERSION
#include "gui_config_dialog.h"
#include "gui_glade_defs.h"
#include "gui_game1v1_config.h"

// strings for the user to choose the type of player
static const char COMBO_PLAYER_TYPE_HUMAN[]    = "Human";
static const char COMBO_PLAYER_TYPE_COMPUTER[] = "Computer";

ConfigDialog::ConfigDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder)  throw (GUIException) :
    Gtk::Dialog(cobject), //Calls the base class constructor
    m_gtkBuilder(a_gtkBuilder),
    m_spinbuttonStartingRowPlayer1Adj(4.0, 0.0, 13.0),
    m_spinbuttonStartingColumnPlayer1Adj(4.0, 0.0, 13.0),
    m_spinbuttonStartingRowPlayer2Adj(9.0, 0.0, 13.0),
    m_spinbuttonStartingColumnPlayer2Adj(9.0, 0.0, 13.0)
{
    // add ok and cancel buttons
#ifdef WIN32
    // win32 systems have ok/accept button at the left of the cancel button
    this->add_button(Gtk::StockID("gtk-cancel"), Gtk::RESPONSE_OK);
    this->add_button(Gtk::StockID("gtk-ok"), Gtk::RESPONSE_CANCEL);
#else
    // any other system will follow gnome GUI guidelines:
    // OK button at the right of the cancel button
    this->add_button(Gtk::StockID("gtk-cancel"), Gtk::RESPONSE_CANCEL);
    this->add_button(Gtk::StockID("gtk-ok"), Gtk::RESPONSE_OK);
#endif // WIN32

    // retrieve widgets from the .glade file
    m_gtkBuilder->get_widget(GUI_CONFIG_TABLE_PLAYER1, m_tablePlayer1);
    if (m_tablePlayer1 == NULL)
    {
        throw new GUIException(std::string("gtk::table for player 1 item retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_TABLE_PLAYER2, m_tablePlayer2);
    if (m_tablePlayer2 == NULL)
    {
        throw new GUIException(std::string("gtk::table for player 2 item retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_SPINBUTTON_STARTROW1, m_spinbuttonStartingRowPlayer1);
    if (m_spinbuttonStartingRowPlayer1 == NULL)
    {
        throw new GUIException(std::string("gtk::spinbutton for starting row (player1) retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_SPINBUTTON_STARTCOL1, m_spinbuttonStartingColumnPlayer1);
    if (m_spinbuttonStartingColumnPlayer1 == NULL)
    {
        throw new GUIException(std::string("gtk::spinbutton for starting col (player1) retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_SPINBUTTON_STARTROW2, m_spinbuttonStartingRowPlayer2);
    if (m_spinbuttonStartingRowPlayer2 == NULL)
    {
        throw new GUIException(std::string("gtk::spinbutton for starting row (player2) retrieval failed"));
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_SPINBUTTON_STARTCOL2, m_spinbuttonStartingColumnPlayer2);
    if (m_spinbuttonStartingColumnPlayer2 == NULL)
    {
        throw new GUIException(std::string("gtk::spinbutton for starting col (player2) retrieval failed"));
    }

    // adjustments for starting coordinates
    m_spinbuttonStartingRowPlayer1->set_adjustment(m_spinbuttonStartingRowPlayer1Adj);
    m_spinbuttonStartingColumnPlayer1->set_adjustment(m_spinbuttonStartingColumnPlayer1Adj);
    m_spinbuttonStartingRowPlayer2->set_adjustment(m_spinbuttonStartingRowPlayer2Adj);
    m_spinbuttonStartingColumnPlayer2->set_adjustment(m_spinbuttonStartingColumnPlayer2Adj);

    // configure custom widgets
    // type of player1
    m_comboTypePlayer1.insert_text(0, COMBO_PLAYER_TYPE_HUMAN);
    m_comboTypePlayer1.append_text(COMBO_PLAYER_TYPE_COMPUTER);
    m_comboTypePlayer1.set_active_text(COMBO_PLAYER_TYPE_HUMAN);
    //type of player2
    m_comboTypePlayer2.insert_text(0, COMBO_PLAYER_TYPE_HUMAN);
    m_comboTypePlayer2.append_text(COMBO_PLAYER_TYPE_COMPUTER);
    m_comboTypePlayer2.set_active_text(COMBO_PLAYER_TYPE_COMPUTER);
    // heuristics
    for (int32_t i = Heuristic::e_heuristicStartCount;
         i < Heuristic::e_heuristicCount;
         i++)
    {
        const Heuristic::sHeuristicData_t &heuristicData =
            Heuristic::m_heuristicData[static_cast<Heuristic::eHeuristicType_t>(i)];

        m_comboHeuristicPlayer1.append_text(heuristicData.m_name);
        m_comboHeuristicPlayer2.append_text(heuristicData.m_name);
    }
    const Heuristic::sHeuristicData_t &selectedHeuristicData1 =
        Heuristic::m_heuristicData[Game1v1Config::Instance().GetHeuristicTypePlayer1()];
    m_comboHeuristicPlayer1.set_active_text(selectedHeuristicData1.m_name);
    const Heuristic::sHeuristicData_t &selectedHeuristicData2 =
        Heuristic::m_heuristicData[Game1v1Config::Instance().GetHeuristicTypePlayer2()];
    m_comboHeuristicPlayer2.set_active_text(selectedHeuristicData2.m_name);

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

    //TODO attach m_comboHeuristicPlayer1 and m_comboHeuristicPlayer2
}

ConfigDialog::~ConfigDialog()
{
}

bool ConfigDialog::IsPlayer1TypeComputer() const
{
    return (m_comboTypePlayer1.get_active_text().compare(COMBO_PLAYER_TYPE_COMPUTER) == 0);
}

bool ConfigDialog::IsPlayer2TypeComputer() const
{
    return (m_comboTypePlayer2.get_active_text().compare(COMBO_PLAYER_TYPE_COMPUTER) == 0);
}

void ConfigDialog::GetPlayer1StartingCoord(Coordinate &a_coord) const
{
    //TODO retrieve value from widgets and remove hardcoded stuff
    a_coord = Coordinate(4, 4);
}

void ConfigDialog::GetPlayer2StartingCoord(Coordinate &a_coord) const
{
    //TODO retrieve value from widgets and remove hardcoded stuff
    a_coord = Coordinate(9, 9);
}

Heuristic::eHeuristicType_t ConfigDialog::GetPlayer1Heuristic() const
{
    for (int32_t i = Heuristic::e_heuristicStartCount;
         i < Heuristic::e_heuristicCount;
         i++)
    {
        const Heuristic::sHeuristicData_t &heuristicData =
            Heuristic::m_heuristicData[static_cast<Heuristic::eHeuristicType_t>(i)];

        if (m_comboHeuristicPlayer1.get_active_text().compare(heuristicData.m_name) == 0)
        {
            return static_cast<Heuristic::eHeuristicType_t>(i);
        }
    }

#ifdef DEBUG
    assert(0);
#endif
    return Heuristic::e_heuristicStartCount;
}

Heuristic::eHeuristicType_t ConfigDialog::GetPlayer2Heuristic() const
{
    for (int32_t i = Heuristic::e_heuristicStartCount;
         i < Heuristic::e_heuristicCount;
         i++)
    {
        const Heuristic::sHeuristicData_t &heuristicData =
            Heuristic::m_heuristicData[static_cast<Heuristic::eHeuristicType_t>(i)];

        if (m_comboHeuristicPlayer2.get_active_text().compare(heuristicData.m_name) == 0)
        {
            return static_cast<Heuristic::eHeuristicType_t>(i);
        }
    }

#ifdef DEBUG
    assert(0);
#endif
    return Heuristic::e_heuristicStartCount;
}

int ConfigDialog::run()
{
    // load current global configuration into the widgets
    // before showing the dialog

    // type of players
    if (Game1v1Config::Instance().IsPlayer1Computer())
    {
        m_comboTypePlayer1.set_active_text(COMBO_PLAYER_TYPE_COMPUTER);
    }
    else
    {
        m_comboTypePlayer1.set_active_text(COMBO_PLAYER_TYPE_HUMAN);
    }

    if (Game1v1Config::Instance().IsPlayer2Computer())
    {
        m_comboTypePlayer2.set_active_text(COMBO_PLAYER_TYPE_COMPUTER);
    }
    else
    {
        m_comboTypePlayer2.set_active_text(COMBO_PLAYER_TYPE_HUMAN);
    }

    //TODO starting coordinates into widgets!!

    // heuristic type
    const Heuristic::sHeuristicData_t &selectedHeuristicData1 =
        Heuristic::m_heuristicData[Game1v1Config::Instance().GetHeuristicTypePlayer1()];
    m_comboHeuristicPlayer1.set_active_text(selectedHeuristicData1.m_name);
    const Heuristic::sHeuristicData_t &selectedHeuristicData2 =
        Heuristic::m_heuristicData[Game1v1Config::Instance().GetHeuristicTypePlayer2()];
    m_comboHeuristicPlayer2.set_active_text(selectedHeuristicData2.m_name);

    return Gtk::Dialog::run();
}

