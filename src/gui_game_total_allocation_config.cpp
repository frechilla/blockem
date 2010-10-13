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
/// @file  gui_game_total_allocation_config.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#include "gui_game_total_allocation_config.h"

static const int32_t    DEFAULT_NROWS = 14;
static const int32_t    DEFAULT_NCOLS = 14;
static const Coordinate DEFAULT_STARTING_COORD = Coordinate(); // uninitialised coord

GameTotalAllocationConfig::GameTotalAllocationConfig() :
    Singleton<GameTotalAllocationConfig>(),
    m_nRows(DEFAULT_NROWS),
    m_nColumns(DEFAULT_NCOLS),
    m_startingCoord(DEFAULT_STARTING_COORD)
{
}

GameTotalAllocationConfig::~GameTotalAllocationConfig()
{
}

const Coordinate& GameTotalAllocationConfig::GetStartingCoord() const
{
    return m_startingCoord;
}

bool GameTotalAllocationConfig::IsStartingCoordSet() const
{
    return m_startingCoord.Initialised();
}

int32_t GameTotalAllocationConfig::GetNRows() const
{
    return m_nRows;
}

int32_t GameTotalAllocationConfig::GetNColumns() const
{
    return m_nColumns;
}

void GameTotalAllocationConfig::SetStartingCoord(const Coordinate &a_startingCoord)
{
    m_startingCoord = a_startingCoord;
}

void GameTotalAllocationConfig::SetNRows(int32_t a_nRows)
{
    m_nRows = a_nRows;
}

void GameTotalAllocationConfig::SetNColumns(int32_t a_nColumns)
{
    m_nColumns = a_nColumns;
}
