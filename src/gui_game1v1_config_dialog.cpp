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
/// @file  gui_game1v1_config_dialog.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 01-May-2010  Original development
///           Faustino Frechilla 21-Jul-2010  i18n
///           Faustino Frechilla 23-Sep-2010  Renamed to gui_game1v1_config_dialog
/// @endhistory
///
// ============================================================================

#include "gettext.h" // i18n
#include "gui_game1v1_config_dialog.h"
#include "gui_glade_defs.h"
#include "gui_game1v1_config.h"
#include "game1v1.h" // BOARD_1VS1_ROWS and DEFAULT_STARTING_COORD_PLAYER[1-2]
#include "piece.h"   // e_numberOfPieces


// strings for the user to choose the type of player
static const char COMBO_PLAYER_TYPE_HUMAN[]    = N_("Human");
static const char COMBO_PLAYER_TYPE_COMPUTER[] = N_("Computer");

// maximum and minimum coordinate values for the starting coordinate spinbuttons
// they start by 1.0 'cos is more user friendly than the c-style 0
static const double MINIMUM_STARTING_COORD_ROW = 1;
static const double MAXIMUM_STARTING_COORD_ROW = BOARD_1VS1_ROWS;
static const double MINIMUM_STARTING_COORD_COL = 1;
static const double MAXIMUM_STARTING_COORD_COL = BOARD_1VS1_COLUMNS;

Game1v1ConfigDialog::Game1v1ConfigDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder)  throw (GUIException) :
    Gtk::Dialog(cobject), //Calls the base class constructor
    m_gtkBuilder(a_gtkBuilder),
    m_spinbuttonStartingRowPlayer1Adj(
        Game1v1Config::Instance().GetPlayer1StartingCoord().m_row,
        MINIMUM_STARTING_COORD_ROW,
        MAXIMUM_STARTING_COORD_ROW),
    m_spinbuttonStartingColumnPlayer1Adj(
        Game1v1Config::Instance().GetPlayer1StartingCoord().m_col,
        MINIMUM_STARTING_COORD_COL,
        MAXIMUM_STARTING_COORD_COL),
    m_spinbuttonStartingRowPlayer2Adj(
        Game1v1Config::Instance().GetPlayer2StartingCoord().m_row,
        MINIMUM_STARTING_COORD_ROW,
        MAXIMUM_STARTING_COORD_ROW),
    m_spinbuttonStartingColumnPlayer2Adj(
        Game1v1Config::Instance().GetPlayer2StartingCoord().m_col,
        MINIMUM_STARTING_COORD_COL,
        MAXIMUM_STARTING_COORD_COL),
    m_spinbuttonDepthPlayer1Adj(0.0, 0.0, e_numberOfPieces * 2), // maximum depth is putting down all the pieces
    m_spinbuttonDepthPlayer2Adj(0.0, 0.0, e_numberOfPieces * 2)  // maximum depth is putting down all the pieces
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
    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_TABLE_PLAYER1, m_tablePlayer1);
    if (m_tablePlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_TABLE_PLAYER2, m_tablePlayer2);
    if (m_tablePlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_SPINBUTTON_STARTROW1, m_spinbuttonStartingRowPlayer1);
    if (m_spinbuttonStartingRowPlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_SPINBUTTON_STARTCOL1, m_spinbuttonStartingColumnPlayer1);
    if (m_spinbuttonStartingColumnPlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_SPINBUTTON_STARTROW2, m_spinbuttonStartingRowPlayer2);
    if (m_spinbuttonStartingRowPlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_SPINBUTTON_STARTCOL2, m_spinbuttonStartingColumnPlayer2);
    if (m_spinbuttonStartingColumnPlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_AI_FRAME_PLAYER1, m_AIFramePlayer1);
    if (m_AIFramePlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_AI_FRAME_PLAYER2, m_AIFramePlayer2);
    if (m_AIFramePlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_AI_TABLE_PLAYER1, m_AITablePlayer1);
    if (m_AITablePlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_AI_TABLE_PLAYER2, m_AITablePlayer2);
    if (m_AITablePlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_AI_SPINBUTTON_DEPTH1, m_spinbuttonDepthPlayer1);
    if (m_spinbuttonDepthPlayer1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_AI_SPINBUTTON_DEPTH2, m_spinbuttonDepthPlayer2);
    if (m_spinbuttonDepthPlayer2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_AI_TEXTVIEW_HEURISTIC1, m_textViewHeuristic1);
    if (m_textViewHeuristic1 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_AI_TEXTVIEW_HEURISTIC2, m_textViewHeuristic2);
    if (m_textViewHeuristic2 == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER1_LABEL, m_player1Label);
    if (m_player1Label == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER2_LABEL, m_player2Label);
    if (m_player2Label == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER1_TYPE_LABEL, m_player1TypeLabel);
    if (m_player1TypeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER2_TYPE_LABEL, m_player2TypeLabel);
    if (m_player2TypeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER1_STARTING_ROW_LABEL, m_player1StartingRowLabel);
    if (m_player1StartingRowLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER2_STARTING_ROW_LABEL, m_player2StartingRowLabel);
    if (m_player2StartingRowLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER1_STARTING_COL_LABEL, m_player1StartingColLabel);
    if (m_player1StartingColLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER2_STARTING_COL_LABEL, m_player2StartingColLabel);
    if (m_player2StartingColLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER1_AI_HEADER_LABEL, m_player1AIHeaderLabel);
    if (m_player1AIHeaderLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER2_AI_HEADER_LABEL, m_player2AIHeaderLabel);
    if (m_player2AIHeaderLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER1_AI_TYPE_LABEL, m_player1AITypeLabel);
    if (m_player1AITypeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER2_AI_TYPE_LABEL, m_player2AITypeLabel);
    if (m_player2AITypeLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER1_AI_DEPTH_LABEL, m_player1AIDepthLabel);
    if (m_player1AIDepthLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_CONFIG_1V1_PLAYER2_AI_DEPTH_LABEL, m_player2AIDepthLabel);
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
    
    // starting coords cannot be configured. They must be set up before the
    // new game is launched
    SetStartingCoordEditionSensitive(false);

    //signal handling
    m_comboTypePlayer1.signal_changed().connect(
            sigc::mem_fun(*this, &Game1v1ConfigDialog::ComboPlayer1Type_signalChanged));
    m_comboTypePlayer2.signal_changed().connect(
            sigc::mem_fun(*this, &Game1v1ConfigDialog::ComboPlayer2Type_signalChanged));
    m_comboHeuristicPlayer1.signal_changed().connect(
            sigc::mem_fun(*this, &Game1v1ConfigDialog::ComboHeuristicPlayer1_signalChanged));
    m_comboHeuristicPlayer2.signal_changed().connect(
            sigc::mem_fun(*this, &Game1v1ConfigDialog::ComboHeuristicPlayer2_signalChanged));

    m_spinbuttonDepthPlayer1->signal_value_changed().connect(
            sigc::mem_fun(*this, &Game1v1ConfigDialog::SpinButtonDepthPlayer1_SignalValueChanged));
    m_spinbuttonDepthPlayer2->signal_value_changed().connect(
            sigc::mem_fun(*this, &Game1v1ConfigDialog::SpinButtonDepthPlayer2_SignalValueChanged));
}

Game1v1ConfigDialog::~Game1v1ConfigDialog()
{
}

bool Game1v1ConfigDialog::on_expose_event (GdkEventExpose* event)
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

    return Gtk::Dialog::on_expose_event(event);
}

void Game1v1ConfigDialog::SetStartingCoordEditionSensitive(bool action)
{
    m_spinbuttonStartingRowPlayer1->set_sensitive(action);
    m_spinbuttonStartingColumnPlayer1->set_sensitive(action);
    m_spinbuttonStartingRowPlayer2->set_sensitive(action);
    m_spinbuttonStartingColumnPlayer2->set_sensitive(action);
}

void Game1v1ConfigDialog::SpinButtonDepthPlayer1_SignalValueChanged()
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

void Game1v1ConfigDialog::SpinButtonDepthPlayer2_SignalValueChanged()
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

void Game1v1ConfigDialog::ComboPlayer1Type_signalChanged()
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

void Game1v1ConfigDialog::ComboPlayer2Type_signalChanged()
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

void Game1v1ConfigDialog::ComboHeuristicPlayer1_signalChanged()
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

void Game1v1ConfigDialog::ComboHeuristicPlayer2_signalChanged()
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

bool Game1v1ConfigDialog::IsPlayer1TypeComputer() const
{
    return (m_comboTypePlayer1.get_active_text().compare(_(COMBO_PLAYER_TYPE_COMPUTER)) == 0);
}

bool Game1v1ConfigDialog::IsPlayer2TypeComputer() const
{
    return (m_comboTypePlayer2.get_active_text().compare(_(COMBO_PLAYER_TYPE_COMPUTER)) == 0);
}

void Game1v1ConfigDialog::GetPlayer1StartingCoord(Coordinate &a_coord) const
{
    // -1 because coordinates are shown to the user starting by 1 (not 0)
    a_coord = Coordinate(
                static_cast<int32_t>(m_spinbuttonStartingRowPlayer1Adj.get_value())    - 1,
                static_cast<int32_t>(m_spinbuttonStartingColumnPlayer1Adj.get_value()) - 1);
}

void Game1v1ConfigDialog::GetPlayer2StartingCoord(Coordinate &a_coord) const
{
    // -1 because coordinates are shown to the user starting by 1 (not 0)
    a_coord = Coordinate(
                static_cast<int32_t>(m_spinbuttonStartingRowPlayer2Adj.get_value())    - 1,
                static_cast<int32_t>(m_spinbuttonStartingColumnPlayer2Adj.get_value()) - 1);
}

int32_t Game1v1ConfigDialog::GetPlayer1SearchTreeDepth() const
{
    return static_cast<uint32_t>(m_spinbuttonDepthPlayer1Adj.get_value());
}

int32_t Game1v1ConfigDialog::GetPlayer2SearchTreeDepth() const
{
    return static_cast<uint32_t>(m_spinbuttonDepthPlayer2Adj.get_value());
}

Heuristic::eHeuristicType_t Game1v1ConfigDialog::GetPlayer1Heuristic() const
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

Heuristic::eHeuristicType_t Game1v1ConfigDialog::GetPlayer2Heuristic() const
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

int Game1v1ConfigDialog::run()
{
    // load current global configuration into the widgets before showing the dialog

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

    // run the Gtk dialog once the custom widgets are configured properly
    return Gtk::Dialog::run();
}

void Game1v1ConfigDialog::SaveCurrentConfigIntoGlobalSettings() const
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

#ifdef WIN32
void Game1v1ConfigDialog::ForceTranslationOfWidgets()
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
void Game1v1ConfigDialog::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
