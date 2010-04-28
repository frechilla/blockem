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
/// @file  gui_game1v1_config.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 29-Apr-2010  Original development
/// @endhistory
///
// ============================================================================

#include "gui_game1v1_config.h"

static Heuristic::eHeuristicType_t  DEFAULT_CURRENT_HEURISTIC = Heuristic::e_heuristicNKWeighted;
static Game1v1Config::ePlayerType_t DEFAULT_PLAYER1_TYPE      = Game1v1Config::e_playerHuman;
static Game1v1Config::ePlayerType_t DEFAULT_PLAYER2_TYPE      = Game1v1Config::e_playerComputer;
static int32_t                      DEFAULT_MINIMAX_DEPTH     = GAME1V1CONFIG_DEPTH_AUTOADJUST;
static uint8_t                      DEFAULT_PLAYER1_COLOUR_R  = 76;
static uint8_t                      DEFAULT_PLAYER1_COLOUR_G  = 204;
static uint8_t                      DEFAULT_PLAYER1_COLOUR_B  = 76;
static uint8_t                      DEFAULT_PLAYER2_COLOUR_R  = 0;
static uint8_t                      DEFAULT_PLAYER2_COLOUR_G  = 0;
static uint8_t                      DEFAULT_PLAYER2_COLOUR_B  = 229;

Game1v1Config::Game1v1Config() :
    Singleton<Game1v1Config>(),
    m_currentHeuristic(DEFAULT_CURRENT_HEURISTIC),
    m_player1Type(DEFAULT_PLAYER1_TYPE),
    m_player2Type(DEFAULT_PLAYER2_TYPE),
    m_minimaxDepth(DEFAULT_MINIMAX_DEPTH),
    m_player1Red(DEFAULT_PLAYER1_COLOUR_R),
    m_player1Green(DEFAULT_PLAYER1_COLOUR_G),
    m_player1Blue(DEFAULT_PLAYER1_COLOUR_B),
    m_player2Red(DEFAULT_PLAYER2_COLOUR_R),
    m_player2Green(DEFAULT_PLAYER2_COLOUR_G),
    m_player2Blue(DEFAULT_PLAYER2_COLOUR_B)
{
    // add available evaluation functions to the corresponding array
    m_heuristicData[Heuristic::e_heuristicNKWeighted].m_evalFunction =
        &Heuristic::CalculateNKWeighted;
    m_heuristicData[Heuristic::e_heuristicNKWeighted].m_name =
        std::string("Nucleation Point weighted");
    m_heuristicData[Heuristic::e_heuristicNKWeighted].m_description =
        std::string("Nucleation points are weighted. The more in the middle in the board a NK point is, the more important it is");

    m_heuristicData[Heuristic::e_heuristicSimple].m_evalFunction =
        &Heuristic::CalculateSimple;
    m_heuristicData[Heuristic::e_heuristicSimple].m_name =
        std::string("Simple");
    m_heuristicData[Heuristic::e_heuristicSimple].m_description =
        std::string("takes into account only the amount of squares of the deployed pieces");
}

Game1v1Config::~Game1v1Config()
{
}

Heuristic::eHeuristicType_t Game1v1Config::GetHeuristicType() const
{
    return m_currentHeuristic;
}
void Game1v1Config::SetHeuristicType(Heuristic::eHeuristicType_t a_heuristic)
{
#ifdef DEBUG
    assert(a_heuristic >= e_heuristicStartCount);
    assert(a_heuristic <  e_heuristicCount);
#endif

    m_currentHeuristic = a_heuristic;
}

const Game1v1Config::sHeuristicConfig_t& Game1v1Config::GetHeuristicData(Heuristic::eHeuristicType_t a_heuristic) const
{
#ifdef DEBUG
    assert(a_heuristic >= e_heuristicStartCount);
    assert(a_heuristic <  e_heuristicCount);
#endif

    return m_heuristicData[a_heuristic];
}

void Game1v1Config::SetPlayer1Type(ePlayerType_t a_playerType)
{
    m_player1Type = a_playerType;
}

bool Game1v1Config::IsPlayer1Computer()
{
    return (m_player1Type == e_playerComputer);
}

void Game1v1Config::SetPlayer2Type(ePlayerType_t a_playerType)
{
    m_player2Type = a_playerType;
}

bool Game1v1Config::IsPlayer2Computer()
{
    return (m_player2Type == e_playerComputer);
}

void Game1v1Config::SetMinimaxDepth(int32_t a_depth)
{
    if (a_depth <= 0)
    {
        m_minimaxDepth = GAME1V1CONFIG_DEPTH_AUTOADJUST;
    }
    else
    {
        m_minimaxDepth = a_depth;
    }
}

int32_t Game1v1Config::GetMinimaxDepth() const
{
    return m_minimaxDepth;
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
