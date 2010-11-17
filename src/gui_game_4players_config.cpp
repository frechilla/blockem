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
/// @file  gui_game_4players_config.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 17-Nov-2010  Original development
/// @endhistory
///
// ============================================================================

#include "gui_game_4players_config.h"

// colours are here so they can't be accesed from elsewhere
static const uint8_t DEFAULT_PLAYER1_COLOUR_R  =  31;
static const uint8_t DEFAULT_PLAYER1_COLOUR_G  = 130;
static const uint8_t DEFAULT_PLAYER1_COLOUR_B  =  31;
static const uint8_t DEFAULT_PLAYER2_COLOUR_R  =   0;
static const uint8_t DEFAULT_PLAYER2_COLOUR_G  =   0;
static const uint8_t DEFAULT_PLAYER2_COLOUR_B  = 229;
static const uint8_t DEFAULT_PLAYER3_COLOUR_R  = 180;
static const uint8_t DEFAULT_PLAYER3_COLOUR_G  =   4;
static const uint8_t DEFAULT_PLAYER3_COLOUR_B  =  95;
static const uint8_t DEFAULT_PLAYER4_COLOUR_R  = 223;
static const uint8_t DEFAULT_PLAYER4_COLOUR_G  = 116;
static const uint8_t DEFAULT_PLAYER4_COLOUR_B  =   1;

static const int32_t DEFAULT_NROWS = 20;
static const int32_t DEFAULT_NCOLS = 20;

static const Coordinate PLAYER1_DEFAULT_STARTING_COORD = 
    Coordinate(0, 0);
static const Coordinate PLAYER2_DEFAULT_STARTING_COORD = 
    Coordinate(0, DEFAULT_NCOLS - 1);
static const Coordinate PLAYER3_DEFAULT_STARTING_COORD = 
    Coordinate(DEFAULT_NROWS - 1, DEFAULT_NCOLS - 1);
static const Coordinate PLAYER4_DEFAULT_STARTING_COORD = 
    Coordinate(DEFAULT_NROWS - 1, 0);


Game4PlayersConfig::Game4PlayersConfig() :
    Singleton<Game4PlayersConfig>(),
    m_nRows(DEFAULT_NROWS),
    m_nColumns(DEFAULT_NCOLS)
{
    // set default starting coords
    m_startingCoords[0] = PLAYER1_DEFAULT_STARTING_COORD;
    m_startingCoords[1] = PLAYER2_DEFAULT_STARTING_COORD;
    m_startingCoords[2] = PLAYER3_DEFAULT_STARTING_COORD;
    m_startingCoords[3] = PLAYER4_DEFAULT_STARTING_COORD;

    // set player1's colour
    m_redArray[0]   = DEFAULT_PLAYER1_COLOUR_R;
    m_greenArray[0] = DEFAULT_PLAYER1_COLOUR_G;
    m_blueArray[0]  = DEFAULT_PLAYER1_COLOUR_B;    
    // set player2's colour
    m_redArray[1]   = DEFAULT_PLAYER2_COLOUR_R;
    m_greenArray[1] = DEFAULT_PLAYER2_COLOUR_G;
    m_blueArray[1]  = DEFAULT_PLAYER2_COLOUR_B;
    // set player3's colour
    m_redArray[2]   = DEFAULT_PLAYER3_COLOUR_R;
    m_greenArray[2] = DEFAULT_PLAYER3_COLOUR_G;
    m_blueArray[2]  = DEFAULT_PLAYER3_COLOUR_B;
    // set player4's colour
    m_redArray[3]   = DEFAULT_PLAYER4_COLOUR_R;
    m_greenArray[3] = DEFAULT_PLAYER4_COLOUR_G;
    m_blueArray[3]  = DEFAULT_PLAYER4_COLOUR_B;
}

Game4PlayersConfig::~Game4PlayersConfig()
{
}

int32_t Game4PlayersConfig::GetNRows() const
{
    return m_nRows;
}

int32_t Game4PlayersConfig::GetNColumns() const
{
    return m_nColumns;
}

const Coordinate& Game4PlayersConfig::GetStartingCoord(
    Game4Players::eGame4_Player_t a_player) const
{
    int32_t index = GetInternalIndex(a_player);    
    return m_startingCoords[index];
}

bool Game4PlayersConfig::IsStartingCoordSet(
    Game4Players::eGame4_Player_t a_player) const
{
    return GetStartingCoord(a_player).Initialised();
}

void Game4PlayersConfig::GetPlayerColour(
        Game4Players::eGame4_Player_t a_player,
        uint8_t &red, 
        uint8_t &green, 
        uint8_t &blue) const
{
    int32_t colourIndex = GetInternalIndex(a_player);
    
    red   = m_redArray[colourIndex];
    green = m_greenArray[colourIndex];
    blue  = m_blueArray[colourIndex];
}

void Game4PlayersConfig::SetNRows(int32_t a_nRows)
{
    m_nRows = a_nRows;
}

void Game4PlayersConfig::SetNColumns(int32_t a_nColumns)
{
    m_nColumns = a_nColumns;
}

int32_t Game4PlayersConfig::GetInternalIndex(
    Game4Players::eGame4_Player_t a_player) const
{
    int32_t index = 0;
    switch(a_player)
    {
        case Game4Players::e_Game4_Player1:
        {
            index = 0;
            break;
        }            
        case Game4Players::e_Game4_Player2:
        {
            index = 1;
            break;
        }            
        case Game4Players::e_Game4_Player3:
        {
            index = 2;
            break;
        }            
        case Game4Players::e_Game4_Player4:
        {
            index = 3;
            break;
        }            
        case Game4Players::e_Game4_NoPlayer: // let it fall through        
        default:
        {
#ifdef DEBUG
            assert(0);
#endif
            break;
        }
    } // switch(a_player)
    
    return index;
}
