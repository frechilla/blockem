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
/// @file  gui_game_total_allocation_config.h
/// @brief Singleton which stores GUI related configuration data for 1vs1 games
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 13-Oct-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef __GUI_GAME_TOTAL_ALLOCATION_CONFIG__
#define __GUI_GAME_TOTAL_ALLOCATION_CONFIG__

#include "singleton.h"
#include "coordinate.h"


/// singleton which stores the configuration to apply to Game1v1
class GameTotalAllocationConfig :
    public Singleton<GameTotalAllocationConfig>
{
public:

    /// If uninitialised game could start from anywhere in the board
    const Coordinate& GetStartingCoord() const;
    /// @return true if starting coord set. False otherwise
    bool IsStartingCoordSet() const;
    /// Get the number of rows of the board
    int32_t GetNRows() const;
    /// Get the number of columns of the board
    int32_t GetNColumns() const;

    /// @brief set starting coordinate
    /// set it to an uninitialised coordinate if game can start from
    /// anywhere on the board
    void SetStartingCoord(const Coordinate &a_startingCoord);
    /// @brief set amount of rows of this game's board
    void SetNRows(int32_t a_nRows);
    /// @brief set amount of columns of this game's board
    void SetNColumns(int32_t a_nColumns);

private:
    /// number of rows of the board
    int32_t m_nRows;
    /// number of columns of the board
    int32_t m_nColumns;
    /// starting coordinate. If uninitialised game could start from
    /// anywhere in the board
    Coordinate m_startingCoord;

    friend class Singleton<GameTotalAllocationConfig>;
    GameTotalAllocationConfig();
    ~GameTotalAllocationConfig();
};

#endif // __GUI_GAME_TOTAL_ALLOCATION_CONFIG__
