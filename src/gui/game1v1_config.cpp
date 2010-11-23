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
/// @file  game1v1_config.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 29-Apr-2010  Original development
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#include "gui/game1v1_config.h"

// colours are here so they can't be accesed from elsewhere
static const uint8_t DEFAULT_PLAYER1_COLOUR_R  = 31;
static const uint8_t DEFAULT_PLAYER1_COLOUR_G  = 130;
static const uint8_t DEFAULT_PLAYER1_COLOUR_B  = 31;
static const uint8_t DEFAULT_PLAYER2_COLOUR_R  = 0;
static const uint8_t DEFAULT_PLAYER2_COLOUR_G  = 0;
static const uint8_t DEFAULT_PLAYER2_COLOUR_B  = 229;

Game1v1Config::Game1v1Config() :
    Singleton<Game1v1Config>(),
    m_currentHeuristicPlayer1(DEFAULT_CURRENT_HEURISTIC),
    m_currentHeuristicPlayer2(DEFAULT_CURRENT_HEURISTIC),
    m_player1Type(DEFAULT_PLAYER1_TYPE),
    m_player2Type(DEFAULT_PLAYER2_TYPE),
    m_minimaxDepthPlayer1(DEFAULT_MINIMAX_DEPTH),
    m_minimaxDepthPlayer2(DEFAULT_MINIMAX_DEPTH),
    m_player1Red(DEFAULT_PLAYER1_COLOUR_R),
    m_player1Green(DEFAULT_PLAYER1_COLOUR_G),
    m_player1Blue(DEFAULT_PLAYER1_COLOUR_B),
    m_player2Red(DEFAULT_PLAYER2_COLOUR_R),
    m_player2Green(DEFAULT_PLAYER2_COLOUR_G),
    m_player2Blue(DEFAULT_PLAYER2_COLOUR_B),
    m_player1StartingCoord(DEFAULT_STARTING_ROW_1, DEFAULT_STARTING_COL_1),
    m_player2StartingCoord(DEFAULT_STARTING_ROW_2, DEFAULT_STARTING_COL_2)
{
}

Game1v1Config::~Game1v1Config()
{
}

Heuristic::eHeuristicType_t Game1v1Config::GetHeuristicTypePlayer1() const
{
    return m_currentHeuristicPlayer1;
}

Heuristic::eHeuristicType_t Game1v1Config::GetHeuristicTypePlayer2() const
{
    return m_currentHeuristicPlayer2;
}

bool Game1v1Config::IsPlayer1Computer() const
{
    return (m_player1Type == e_playerComputer);
}

bool Game1v1Config::IsPlayer2Computer() const
{
    return (m_player2Type == e_playerComputer);
}

int32_t Game1v1Config::GetMinimaxDepthPlayer1() const
{
    return m_minimaxDepthPlayer1;
}

int32_t Game1v1Config::GetMinimaxDepthPlayer2() const
{
    return m_minimaxDepthPlayer2;
}

void Game1v1Config::GetPlayer1Colour(uint8_t &red, uint8_t &green, uint8_t &blue) const
{
    red   = m_player1Red;
    green = m_player1Green;
    blue  = m_player1Blue;
}

void Game1v1Config::GetPlayer2Colour(uint8_t &red, uint8_t &green, uint8_t &blue) const
{
    red   = m_player2Red;
    green = m_player2Green;
    blue  = m_player2Blue;
}

const Coordinate& Game1v1Config::GetPlayer1StartingCoord() const
{
    return m_player1StartingCoord;
}

const Coordinate& Game1v1Config::GetPlayer2StartingCoord() const
{
    return m_player2StartingCoord;
}

void Game1v1Config::SetHeuristicTypePlayer1(Heuristic::eHeuristicType_t a_heuristic)
{
#ifdef DEBUG
    assert(a_heuristic >= Heuristic::e_heuristicStartCount);
    assert(a_heuristic <  Heuristic::e_heuristicCount);
#endif

    m_currentHeuristicPlayer1 = a_heuristic;
}

void Game1v1Config::SetHeuristicTypePlayer2(Heuristic::eHeuristicType_t a_heuristic)
{
#ifdef DEBUG
    assert(a_heuristic >= Heuristic::e_heuristicStartCount);
    assert(a_heuristic <  Heuristic::e_heuristicCount);
#endif

    m_currentHeuristicPlayer2 = a_heuristic;
}

void Game1v1Config::SetPlayer1Type(ePlayerType_t a_playerType)
{
    m_player1Type = a_playerType;
}

void Game1v1Config::SetPlayer2Type(ePlayerType_t a_playerType)
{
    m_player2Type = a_playerType;
}

void Game1v1Config::SetMinimaxDepthPlayer1(int32_t a_depth)
{
    if (a_depth <= 0)
    {
        m_minimaxDepthPlayer1 = GAME1V1CONFIG_DEPTH_AUTOADJUST;
    }
    else
    {
        m_minimaxDepthPlayer1 = a_depth;
    }
}

void Game1v1Config::SetMinimaxDepthPlayer2(int32_t a_depth)
{
    if (a_depth <= 0)
    {
        m_minimaxDepthPlayer2 = GAME1V1CONFIG_DEPTH_AUTOADJUST;
    }
    else
    {
        m_minimaxDepthPlayer2 = a_depth;
    }
}

void Game1v1Config::SetPlayer1StartingCoord(const Coordinate &a_startingCoord)
{
    m_player1StartingCoord = a_startingCoord;
}

void Game1v1Config::SetPlayer2StartingCoord(const Coordinate &a_startingCoord)
{
    m_player2StartingCoord = a_startingCoord;
}
