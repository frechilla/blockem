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
/// @file  game_4players_config.h
/// @brief Singleton which stores GUI related configuration data for challenge games
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 17-Nov-2010  Original development
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#ifndef __GUI_GAME_4PLAYERS_CONFIG__
#define __GUI_GAME_4PLAYERS_CONFIG__

#include "singleton.h"
#include "coordinate.h"
#include "game_4players.h" // eGame4_Player_t


/// singleton which stores the configuration to apply to 4players games
class Game4PlayersConfig :
    public Singleton<Game4PlayersConfig>
{
public:    

    /// If uninitialised game could start from anywhere in the board
    /// If a_player is not e_Game4_Player[1-4] info returned is undefined
    /// @param player whose starting coordinate will be returned
    /// @return starting coordinate of a_player 
    const Coordinate& GetStartingCoord(
        Game4Players::eGame4_Player_t a_player) const;
    /// @return true if starting coord set. False otherwise
    /// @param layer whose starting coordinate will be checked
    /// If a_player is not e_Game4_Player[1-4] info returned is undefined
    bool IsStartingCoordSet(Game4Players::eGame4_Player_t a_player) const;
    /// Get the number of rows of the board
    int32_t GetNRows() const;
    /// Get the number of columns of the board
    int32_t GetNColumns() const;
    /// @brief gets the colour of player specified by parameter a_player
    /// If a_player is not e_Game4_Player[1-4] info returned is undefined
    /// saves the values in the 3 paremeter references
    void GetPlayerColour(
        Game4Players::eGame4_Player_t a_player,
        uint8_t &red, 
        uint8_t &green, 
        uint8_t &blue) const;
        
    /// @brief set starting coordinate
    /// set it to an uninitialised coordinate if game can start from
    /// anywhere on the board
    /// If a_player is not e_Game4_Player[1-4] info returned is undefined
    /// @param player whose starting coordinate will be set
    /// @param the starting coordinate to be set to a_player
    void SetStartingCoord(
        Game4Players::eGame4_Player_t a_player,
        const Coordinate &a_startingCoord);
    /// @brief set amount of rows of this game's board
    void SetNRows(int32_t a_nRows);
    /// @brief set amount of columns of this game's board
    void SetNColumns(int32_t a_nColumns);

private:
    /// number of rows of the board
    int32_t m_nRows;
    /// number of columns of the board
    int32_t m_nColumns;
    
    /// array with the starting coordinates of all players
    Coordinate m_startingCoords[4];
    
    /// array of 4 uint8_t which saves the red colour for each player
    uint8_t m_redArray[4];
    /// array of 4 uint8_t which saves the green colour for each player
    uint8_t m_greenArray[4];
    /// array of 4 uint8_t which saves the blue colour for each player
    uint8_t m_blueArray[4];
    
    /// @return internal index that corresponds to a_player to be used with:
    ///         m_startingCoords, m_redArray, m_greenArray and m_blueArray
    int32_t GetInternalIndex(Game4Players::eGame4_Player_t a_player) const;

    friend class Singleton<Game4PlayersConfig>;
    Game4PlayersConfig();
    ~Game4PlayersConfig();
};

#endif // __GUI_GAME_4PLAYERS_CONFIG__
