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
    m_gtkBuilder(a_gtkBuilder)
{
    // add ok and cancel buttons
    this->add_button(Gtk::StockID("gtk-cancel"), Gtk::RESPONSE_CANCEL);
    this->add_button(Gtk::StockID("gtk-ok"), Gtk::RESPONSE_OK);


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



    //    void Gtk::Table::attach     (   Widget&      child,
    //            guint   left_attach,
    //            guint   right_attach,
    //            guint   top_attach,
    //            guint   bottom_attach,
    //            AttachOptions   xoptions = FILL|EXPAND,
    //            AttachOptions   yoptions = FILL|EXPAND,
    //            guint   xpadding = 0,
    //            guint   ypadding = 0
    //        )
    m_tablePlayer1->attach(
            m_comboTypePlayer1,
            1,
            2,
            0,
            1,
            Gtk::FILL | Gtk::EXPAND,
            Gtk::EXPAND,
            0,
            0);
    m_comboTypePlayer1.insert_text(0, COMBO_PLAYER_TYPE_HUMAN);
    m_comboTypePlayer1.append_text(COMBO_PLAYER_TYPE_COMPUTER);
    m_comboTypePlayer1.set_active_text(COMBO_PLAYER_TYPE_HUMAN);
    m_comboTypePlayer1.show();

    m_tablePlayer2->attach(
            m_comboTypePlayer2,
            1,
            2,
            0,
            1,
            Gtk::FILL | Gtk::EXPAND,
            Gtk::EXPAND,
            0,
            0);
    m_comboTypePlayer2.insert_text(0, COMBO_PLAYER_TYPE_HUMAN);
    m_comboTypePlayer2.append_text(COMBO_PLAYER_TYPE_COMPUTER);
    m_comboTypePlayer2.set_active_text(COMBO_PLAYER_TYPE_COMPUTER);
    m_comboTypePlayer2.show();
}

ConfigDialog::~ConfigDialog()
{
}

int ConfigDialog::run()
{
    // show the current configuration before showing the dialog
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

    return Gtk::Dialog::run();
}

bool ConfigDialog::IsPlayer1TypeComputer()
{
    return (m_comboTypePlayer1.get_active_text().compare(COMBO_PLAYER_TYPE_COMPUTER) == 0);
}

bool ConfigDialog::IsPlayer2TypeComputer()
{
    return (m_comboTypePlayer2.get_active_text().compare(COMBO_PLAYER_TYPE_COMPUTER) == 0);
}
