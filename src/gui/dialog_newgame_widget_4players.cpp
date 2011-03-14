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
/// @file  dialog_newgame_widget_4players.cpp
/// @brief NewGameTable4Players methods
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 22-Nov-2010  Original development. Moved from gui_dialog_newgame.cpp
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#include "gui/dialog_newgame_widget_4players.h"
#include "gettext.h" // i18n
#include "game_4players.h"
#include "gui/game_4players_config.h"
#include "gui/glade_defs.h"

// maximum and minimum values for size of the total allocation board
static const double MINIMUM_4PLAYERS_BOARD_NROWS = 4;
static const double MAXIMUM_4PLAYERS_BOARD_NROWS = 99;
static const double MINIMUM_4PLAYERS_BOARD_NCOLS = 4;
static const double MAXIMUM_4PLAYERS_BOARD_NCOLS = 99;

NewGameTable4Players::NewGameTable4Players(
    BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder)  throw (GUIException) :
        NewGameTable(cobject), //Calls the base class constructor
        m_gtkBuilder(a_gtkBuilder),
        m_spinbuttonNRowsAdj(
            Game4PlayersConfig::Instance().GetNRows(),
            MINIMUM_4PLAYERS_BOARD_NROWS,
            MAXIMUM_4PLAYERS_BOARD_NROWS),
        m_spinbuttonNColsAdj(
            Game4PlayersConfig::Instance().GetNColumns(),
            MINIMUM_4PLAYERS_BOARD_NCOLS,
            MAXIMUM_4PLAYERS_BOARD_NCOLS),
        m_spinbuttonStartingRowPlayer1Adj(
            Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player1).m_row + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            1,
            Game4PlayersConfig::Instance().GetNRows()),
        m_spinbuttonStartingColumnPlayer1Adj(
            Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player1).m_col + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            1,
            Game4PlayersConfig::Instance().GetNColumns()),
        m_spinbuttonStartingRowPlayer2Adj(
            Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player2).m_row + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            1,
            Game4PlayersConfig::Instance().GetNRows()),
        m_spinbuttonStartingColumnPlayer2Adj(
            Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player2).m_col + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            1,
            Game4PlayersConfig::Instance().GetNColumns()),
        m_spinbuttonStartingRowPlayer3Adj(
            Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player3).m_row + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            1,
            Game4PlayersConfig::Instance().GetNRows()),
        m_spinbuttonStartingColumnPlayer3Adj(
            Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player3).m_col + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            1,
            Game4PlayersConfig::Instance().GetNColumns()),
        m_spinbuttonStartingRowPlayer4Adj(
            Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player4).m_row + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            1,
            Game4PlayersConfig::Instance().GetNRows()),
        m_spinbuttonStartingColumnPlayer4Adj(
            Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player4).m_col + 1, // +1 'cause coords are shown to the user starting by 1 (not 0)
            1,
            Game4PlayersConfig::Instance().GetNColumns())
{
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_NROWS, m_spinbuttonNRows);
    if (m_spinbuttonNRows == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_NCOLS, m_spinbuttonNCols);
    if (m_spinbuttonNCols == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_STARTROW1, m_spinbuttonStartingRowPlayer1);
    if (m_spinbuttonStartingRowPlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_STARTCOL1, m_spinbuttonStartingColumnPlayer1);
    if (m_spinbuttonStartingColumnPlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_STARTROW2, m_spinbuttonStartingRowPlayer2);
    if (m_spinbuttonStartingRowPlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_STARTCOL2, m_spinbuttonStartingColumnPlayer2);
    if (m_spinbuttonStartingColumnPlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_STARTROW3, m_spinbuttonStartingRowPlayer3);
    if (m_spinbuttonStartingRowPlayer3 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_STARTCOL3, m_spinbuttonStartingColumnPlayer3);
    if (m_spinbuttonStartingColumnPlayer3 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_STARTROW4, m_spinbuttonStartingRowPlayer4);
    if (m_spinbuttonStartingRowPlayer4 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_SPINBUTTON_STARTCOL4, m_spinbuttonStartingColumnPlayer4);
    if (m_spinbuttonStartingColumnPlayer4 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // load labels so they can be translated at startup
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_LABEL_NROWS, m_nRowsLabel);
    if (m_nRowsLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_LABEL_NCOLS, m_nColsLabel);
    if (m_nColsLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER1_LABEL, m_player1Label);
    if (m_player1Label == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER2_LABEL, m_player2Label);
    if (m_player2Label == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER3_LABEL, m_player3Label);
    if (m_player3Label == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER4_LABEL, m_player4Label);
    if (m_player4Label == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER1_STARTING_ROW_LABEL, m_startingRowLabel1);
    if (m_startingRowLabel1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER1_STARTING_COL_LABEL, m_startingColLabel1);
    if (m_startingColLabel1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER2_STARTING_ROW_LABEL, m_startingRowLabel2);
    if (m_startingRowLabel2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER2_STARTING_COL_LABEL, m_startingColLabel2);
    if (m_startingColLabel2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER3_STARTING_ROW_LABEL, m_startingRowLabel3);
    if (m_startingRowLabel3 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER3_STARTING_COL_LABEL, m_startingColLabel3);
    if (m_startingColLabel3 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER4_STARTING_ROW_LABEL, m_startingRowLabel4);
    if (m_startingRowLabel4 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }
    m_gtkBuilder->get_widget(GUI_NEWGAME_4PLAYERS_PLAYER4_STARTING_COL_LABEL, m_startingColLabel4);
    if (m_startingColLabel4 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();

    // adjustments for spinbuttons
    m_spinbuttonNRows->set_adjustment(m_spinbuttonNRowsAdj);
    m_spinbuttonNCols->set_adjustment(m_spinbuttonNColsAdj);
    m_spinbuttonStartingRowPlayer1->set_adjustment(m_spinbuttonStartingRowPlayer1Adj);
    m_spinbuttonStartingColumnPlayer1->set_adjustment(m_spinbuttonStartingColumnPlayer1Adj);
    m_spinbuttonStartingRowPlayer2->set_adjustment(m_spinbuttonStartingRowPlayer2Adj);
    m_spinbuttonStartingColumnPlayer2->set_adjustment(m_spinbuttonStartingColumnPlayer2Adj);
    m_spinbuttonStartingRowPlayer3->set_adjustment(m_spinbuttonStartingRowPlayer3Adj);
    m_spinbuttonStartingColumnPlayer3->set_adjustment(m_spinbuttonStartingColumnPlayer3Adj);
    m_spinbuttonStartingRowPlayer4->set_adjustment(m_spinbuttonStartingRowPlayer4Adj);
    m_spinbuttonStartingColumnPlayer4->set_adjustment(m_spinbuttonStartingColumnPlayer4Adj);
    // store also curent values as the "old" values to be able to avoid 
    // the user from setting up a game with all players starting off the same position
    m_spinbuttonStartingRowPlayer1OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingRowPlayer1Adj.get_value());
    m_spinbuttonStartingColumnPlayer1OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingColumnPlayer1Adj.get_value());
    m_spinbuttonStartingRowPlayer2OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingRowPlayer2Adj.get_value());
    m_spinbuttonStartingColumnPlayer2OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingColumnPlayer2Adj.get_value());
    m_spinbuttonStartingRowPlayer3OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingRowPlayer3Adj.get_value());
    m_spinbuttonStartingColumnPlayer3OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingColumnPlayer3Adj.get_value());
    m_spinbuttonStartingRowPlayer4OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingRowPlayer4Adj.get_value());
    m_spinbuttonStartingColumnPlayer4OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingColumnPlayer4Adj.get_value());

    // connect the signals
    m_spinbuttonNRows->signal_value_changed().connect(
        sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonNRows_SignalValueChanged));
    m_spinbuttonNCols->signal_value_changed().connect(
        sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonNCols_SignalValueChanged));
        
    m_spinbuttonStartingRowPlayer1->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonStartingRowPlayer1_SignalValueChanged));
    m_spinbuttonStartingColumnPlayer1->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonStartingColPlayer1_SignalValueChanged));
    m_spinbuttonStartingRowPlayer2->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonStartingRowPlayer2_SignalValueChanged));
    m_spinbuttonStartingColumnPlayer2->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonStartingColPlayer2_SignalValueChanged));
    m_spinbuttonStartingRowPlayer3->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonStartingRowPlayer3_SignalValueChanged));
    m_spinbuttonStartingColumnPlayer3->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonStartingColPlayer3_SignalValueChanged));
    m_spinbuttonStartingRowPlayer4->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonStartingRowPlayer4_SignalValueChanged));
    m_spinbuttonStartingColumnPlayer4->signal_value_changed().connect(
            sigc::mem_fun(*this, &NewGameTable4Players::SpinButtonStartingColPlayer4_SignalValueChanged));
}

NewGameTable4Players::~NewGameTable4Players()
{
}

void NewGameTable4Players::GetStartingCoordPlayer1(Coordinate &a_coord) const
{
    // -1 because coordinates are shown to the user starting by 1 (not 0)
    a_coord = Coordinate(
                static_cast<int32_t>(m_spinbuttonStartingRowPlayer1Adj.get_value())    - 1,
                static_cast<int32_t>(m_spinbuttonStartingColumnPlayer1Adj.get_value()) - 1);
}

void NewGameTable4Players::GetStartingCoordPlayer2(Coordinate &a_coord) const
{
    // -1 because coordinates are shown to the user starting by 1 (not 0)
    a_coord = Coordinate(
                static_cast<int32_t>(m_spinbuttonStartingRowPlayer2Adj.get_value())    - 1,
                static_cast<int32_t>(m_spinbuttonStartingColumnPlayer2Adj.get_value()) - 1);
}

void NewGameTable4Players::GetStartingCoordPlayer3(Coordinate &a_coord) const
{
    // -1 because coordinates are shown to the user starting by 1 (not 0)
    a_coord = Coordinate(
                static_cast<int32_t>(m_spinbuttonStartingRowPlayer3Adj.get_value())    - 1,
                static_cast<int32_t>(m_spinbuttonStartingColumnPlayer3Adj.get_value()) - 1);
}

void NewGameTable4Players::GetStartingCoordPlayer4(Coordinate &a_coord) const
{
    // -1 because coordinates are shown to the user starting by 1 (not 0)
    a_coord = Coordinate(
                static_cast<int32_t>(m_spinbuttonStartingRowPlayer4Adj.get_value())    - 1,
                static_cast<int32_t>(m_spinbuttonStartingColumnPlayer4Adj.get_value()) - 1);
}

void NewGameTable4Players::SpinButtonNRows_SignalValueChanged()
{
    int32_t currentStartingRow;
    int32_t currentNRows = static_cast<int32_t>(m_spinbuttonNRowsAdj.get_value());

    currentStartingRow = static_cast<int32_t>(m_spinbuttonStartingRowPlayer1Adj.get_value());
    m_spinbuttonStartingRowPlayer1Adj.set_upper(currentNRows);
    if (currentStartingRow > currentNRows)
    {
        m_spinbuttonStartingRowPlayer1Adj.set_value(currentNRows);
    }

    currentStartingRow = static_cast<int32_t>(m_spinbuttonStartingRowPlayer2Adj.get_value());
    m_spinbuttonStartingRowPlayer2Adj.set_upper(currentNRows);
    if (currentStartingRow > currentNRows)
    {
        m_spinbuttonStartingRowPlayer2Adj.set_value(currentNRows);
    }

    currentStartingRow = static_cast<int32_t>(m_spinbuttonStartingRowPlayer3Adj.get_value());
    m_spinbuttonStartingRowPlayer3Adj.set_upper(currentNRows);
    if (currentStartingRow > currentNRows)
    {
        m_spinbuttonStartingRowPlayer3Adj.set_value(currentNRows);
    }

    currentStartingRow = static_cast<int32_t>(m_spinbuttonStartingRowPlayer4Adj.get_value());
    m_spinbuttonStartingRowPlayer4Adj.set_upper(currentNRows);
    if (currentStartingRow > currentNRows)
    {
        m_spinbuttonStartingRowPlayer4Adj.set_value(currentNRows);
    }
}

void NewGameTable4Players::SpinButtonNCols_SignalValueChanged()
{
    int32_t currentStartingCol;
    int32_t currentNCols = static_cast<int32_t>(m_spinbuttonNColsAdj.get_value());

    currentStartingCol = static_cast<int32_t>(m_spinbuttonStartingColumnPlayer1Adj.get_value());
    m_spinbuttonStartingColumnPlayer1Adj.set_upper(currentNCols);
    if (currentStartingCol > currentNCols)
    {
        m_spinbuttonStartingColumnPlayer1Adj.set_value(currentNCols);
    }

    currentStartingCol = static_cast<int32_t>(m_spinbuttonStartingColumnPlayer2Adj.get_value());
    m_spinbuttonStartingColumnPlayer2Adj.set_upper(currentNCols);
    if (currentStartingCol > currentNCols)
    {
        m_spinbuttonStartingColumnPlayer2Adj.set_value(currentNCols);
    }

    currentStartingCol = static_cast<int32_t>(m_spinbuttonStartingColumnPlayer3Adj.get_value());
    m_spinbuttonStartingColumnPlayer3Adj.set_upper(currentNCols);
    if (currentStartingCol > currentNCols)
    {
        m_spinbuttonStartingColumnPlayer3Adj.set_value(currentNCols);
    }

    currentStartingCol = static_cast<int32_t>(m_spinbuttonStartingColumnPlayer4Adj.get_value());
    m_spinbuttonStartingColumnPlayer4Adj.set_upper(currentNCols);
    if (currentStartingCol > currentNCols)
    {
        m_spinbuttonStartingColumnPlayer4Adj.set_value(currentNCols);
    }
}

void NewGameTable4Players::SpinButtonStartingRowPlayer1_SignalValueChanged()
{
    UpdateStartingCoordValue(
        m_spinbuttonStartingRowPlayer1Adj,
        m_spinbuttonStartingRowPlayer1OldValue,
        e_startingCoord_player1);
        
    m_spinbuttonStartingRowPlayer1OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingRowPlayer1Adj.get_value());
}

void NewGameTable4Players::SpinButtonStartingColPlayer1_SignalValueChanged()
{
    UpdateStartingCoordValue(
        m_spinbuttonStartingColumnPlayer1Adj,
        m_spinbuttonStartingColumnPlayer1OldValue,
        e_startingCoord_player1);

    m_spinbuttonStartingColumnPlayer1OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingColumnPlayer1Adj.get_value());
}

void NewGameTable4Players::SpinButtonStartingRowPlayer2_SignalValueChanged()
{
    UpdateStartingCoordValue(
        m_spinbuttonStartingRowPlayer2Adj,
        m_spinbuttonStartingRowPlayer2OldValue,
        e_startingCoord_player2);
        
    m_spinbuttonStartingRowPlayer2OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingRowPlayer2Adj.get_value());
}

void NewGameTable4Players::SpinButtonStartingColPlayer2_SignalValueChanged()
{
    UpdateStartingCoordValue(
        m_spinbuttonStartingColumnPlayer2Adj,
        m_spinbuttonStartingColumnPlayer2OldValue,
        e_startingCoord_player2);        

    m_spinbuttonStartingColumnPlayer2OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingColumnPlayer2Adj.get_value());
}

void NewGameTable4Players::SpinButtonStartingRowPlayer3_SignalValueChanged()
{
    UpdateStartingCoordValue(
        m_spinbuttonStartingRowPlayer3Adj,
        m_spinbuttonStartingRowPlayer3OldValue,
        e_startingCoord_player3);        

    m_spinbuttonStartingRowPlayer3OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingRowPlayer3Adj.get_value());
}

void NewGameTable4Players::SpinButtonStartingColPlayer3_SignalValueChanged()
{
    UpdateStartingCoordValue(
        m_spinbuttonStartingColumnPlayer3Adj,
        m_spinbuttonStartingColumnPlayer3OldValue,
        e_startingCoord_player3);

    m_spinbuttonStartingColumnPlayer3OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingColumnPlayer3Adj.get_value());
}

void NewGameTable4Players::SpinButtonStartingRowPlayer4_SignalValueChanged()
{
    UpdateStartingCoordValue(
        m_spinbuttonStartingRowPlayer4Adj,
        m_spinbuttonStartingRowPlayer4OldValue,
        e_startingCoord_player4);        

    m_spinbuttonStartingRowPlayer4OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingRowPlayer4Adj.get_value());
}

void NewGameTable4Players::SpinButtonStartingColPlayer4_SignalValueChanged()
{
    UpdateStartingCoordValue(
        m_spinbuttonStartingColumnPlayer4Adj,
        m_spinbuttonStartingColumnPlayer4OldValue,
        e_startingCoord_player4);

    m_spinbuttonStartingColumnPlayer4OldValue = 
        static_cast<int32_t>(m_spinbuttonStartingColumnPlayer4Adj.get_value());
}

void NewGameTable4Players::UpdateStartingCoordValue(
    Gtk::Adjustment &a_adj, 
    int32_t a_oldAdjValue,
    eStartingCoordPlayerID_t a_startingCoordPlayerID)
{
    Coordinate startingCoords[e_startingCoord_nPlayers];
    
    GetStartingCoordPlayer1(startingCoords[e_startingCoord_player1]);
    GetStartingCoordPlayer2(startingCoords[e_startingCoord_player2]);
    GetStartingCoordPlayer3(startingCoords[e_startingCoord_player3]);
    GetStartingCoordPlayer4(startingCoords[e_startingCoord_player4]);
    
    for (int32_t i = 0; i < e_startingCoord_nPlayers; i++)
    {
        if ( (i != a_startingCoordPlayerID) && 
             (startingCoords[i] == startingCoords[a_startingCoordPlayerID]) )
        {
            int32_t currentValue = static_cast<int32_t>(a_adj.get_value());

            if (a_oldAdjValue > currentValue)
            {
                if (static_cast<int32_t>(a_adj.get_lower()) == currentValue)
                {
                    if (static_cast<int32_t>(a_adj.get_upper()) == a_oldAdjValue)
                    {                    
                        a_adj.set_value(static_cast<int32_t>(a_adj.get_lower() + 1));
                    }
                    else
                    {
                        a_adj.set_value(static_cast<int32_t>(a_adj.get_upper()));
                    }
                }
                else
                {
                    a_adj.set_value(currentValue - 1);
                }
            }
            else
            {
                if (static_cast<int32_t>(a_adj.get_upper()) == currentValue)
                {
                    if (static_cast<int32_t>(a_adj.get_lower()) == a_oldAdjValue)
                    {                    
                        a_adj.set_value(static_cast<int32_t>(a_adj.get_upper() - 1));
                    }
                    else
                    {
                        a_adj.set_value(static_cast<int32_t>(a_adj.get_lower()));
                    }                
                }
                else
                {
                    a_adj.set_value(currentValue + 1);
                }            
            }
        }
    }
}

void NewGameTable4Players::SaveCurrentConfigIntoGlobalSettings() const
{
    // retrieve user settings from dialog and use them to set up global configuration
    Coordinate startingCoord;
    this->GetStartingCoordPlayer1(startingCoord);
    Game4PlayersConfig::Instance().SetStartingCoord(Game4Players::e_Game4_Player1, startingCoord);
    this->GetStartingCoordPlayer2(startingCoord);
    Game4PlayersConfig::Instance().SetStartingCoord(Game4Players::e_Game4_Player2, startingCoord);
    this->GetStartingCoordPlayer3(startingCoord);
    Game4PlayersConfig::Instance().SetStartingCoord(Game4Players::e_Game4_Player3, startingCoord);
    this->GetStartingCoordPlayer4(startingCoord);
    Game4PlayersConfig::Instance().SetStartingCoord(Game4Players::e_Game4_Player4, startingCoord);

    Game4PlayersConfig::Instance().SetNRows(
            static_cast<int32_t>(m_spinbuttonNRowsAdj.get_value()));
    Game4PlayersConfig::Instance().SetNColumns(
        static_cast<int32_t>(m_spinbuttonNColsAdj.get_value()));
}

void NewGameTable4Players::LoadCurrentConfigFromGlobalSettings()
{
    // load current global configuration into the widgets
    m_spinbuttonNRowsAdj.set_value(
            Game4PlayersConfig::Instance().GetNRows());
    m_spinbuttonNColsAdj.set_value(
            Game4PlayersConfig::Instance().GetNColumns());

    // following snippet of code adds 1 to every starting coord because coords are shown to the
    // user starting from 1, but they are stored internally starting from 0
    if (Game4PlayersConfig::Instance().IsStartingCoordSet(Game4Players::e_Game4_Player1))
    {
        m_spinbuttonStartingRowPlayer1Adj.set_value(
                Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player1).m_row + 1);
        m_spinbuttonStartingColumnPlayer1Adj.set_value(
                Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player1).m_col + 1);
    }
    if (Game4PlayersConfig::Instance().IsStartingCoordSet(Game4Players::e_Game4_Player2))
    {
        m_spinbuttonStartingRowPlayer2Adj.set_value(
                Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player2).m_row + 1);
        m_spinbuttonStartingColumnPlayer2Adj.set_value(
                Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player2).m_col + 1);
    }
    if (Game4PlayersConfig::Instance().IsStartingCoordSet(Game4Players::e_Game4_Player3))
    {
        m_spinbuttonStartingRowPlayer3Adj.set_value(
                Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player3).m_row + 1);
        m_spinbuttonStartingColumnPlayer3Adj.set_value(
                Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player3).m_col + 1);
    }
    if (Game4PlayersConfig::Instance().IsStartingCoordSet(Game4Players::e_Game4_Player4))
    {
        m_spinbuttonStartingRowPlayer4Adj.set_value(
                Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player4).m_row + 1);
        m_spinbuttonStartingColumnPlayer4Adj.set_value(
                Game4PlayersConfig::Instance().GetStartingCoord(Game4Players::e_Game4_Player4).m_col + 1);
    }
}

#ifdef WIN32
void NewGameTable4Players::ForceTranslationOfWidgets()
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

    m_nRowsLabel->set_text(_(m_nRowsLabel->get_text().c_str()));
    m_nColsLabel->set_text(_(m_nColsLabel->get_text().c_str()));
    m_player1Label->set_text(_(m_player1Label->get_text().c_str()));
    m_player2Label->set_text(_(m_player2Label->get_text().c_str()));
    m_player3Label->set_text(_(m_player3Label->get_text().c_str()));
    m_player4Label->set_text(_(m_player4Label->get_text().c_str()));
    m_startingRowLabel1->set_text(_(m_startingRowLabel1->get_text().c_str()));
    m_startingColLabel1->set_text(_(m_startingColLabel1->get_text().c_str()));
    m_startingRowLabel2->set_text(_(m_startingRowLabel2->get_text().c_str()));
    m_startingColLabel2->set_text(_(m_startingColLabel2->get_text().c_str()));
    m_startingRowLabel3->set_text(_(m_startingRowLabel3->get_text().c_str()));
    m_startingColLabel3->set_text(_(m_startingColLabel3->get_text().c_str()));
    m_startingRowLabel4->set_text(_(m_startingRowLabel4->get_text().c_str()));
    m_startingColLabel4->set_text(_(m_startingColLabel4->get_text().c_str()));
}
#else
void NewGameTable4Players::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
