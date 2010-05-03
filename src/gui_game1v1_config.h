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
/// @file  gui_game1v1_config.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 29-Apr-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef __GUI_GAME1V1_CONFIG__
#define __GUI_GAME1V1_CONFIG__

#include "singleton.h"
#include "heuristic.h"
#include "coordinate.h"

/// depth should be set to this value whenever search tree depth is
/// left up to the Application
const int32_t GAME1V1CONFIG_DEPTH_AUTOADJUST = 0;

/// singleton which stores the configuration to apply to Game1v1
class Game1v1Config :
    public Singleton<Game1v1Config>
{
public:
    // types of players. I can only think of 2 types. Human being and computer
    typedef enum
    {
        e_playerComputer,
        e_playerHuman,
    } ePlayerType_t;

    /// @brief returns player1's heuristic type
    Heuristic::eHeuristicType_t GetHeuristicTypePlayer1() const;
    /// @brief returns player2's heuristic type
    Heuristic::eHeuristicType_t GetHeuristicTypePlayer2() const;
    /// @brief returns true if player1 is set to computer type
    bool IsPlayer1Computer() const;
    /// @brief returns true if player2 is set to computer type
    bool IsPlayer2Computer() const;
    /// @brief returns the minimax depth for player1
    /// GAME1V1CONFIG_DEPTH_AUTOADJUST means autoadjust value depending on complexity of search tree
    int32_t GetMinimaxDepthPlayer1() const;
    /// @brief returns the minimax depth for player1
    /// GAME1V1CONFIG_DEPTH_AUTOADJUST means autoadjust value depending on complexity of search tree
    int32_t GetMinimaxDepthPlayer2() const;
    /// @brief gets the colour of player1
    /// saves the values in the 3 paremeter references
    void GetPlayer1Colour(uint8_t &red, uint8_t &green, uint8_t &blue) const;
    /// @brief gets the colour of player2
    /// saves the values in the 3 paremeter references
    void GetPlayer2Colour(uint8_t &red, uint8_t &green, uint8_t &blue) const;
    /// @return player1's starting coordinate
    const Coordinate& GetPlayer1StartingCoord() const;
    /// @return player2's starting coordinate
    const Coordinate& GetPlayer2StartingCoord() const;

    /// @brief sets player1's current heuristic
    void SetHeuristicTypePlayer1(Heuristic::eHeuristicType_t a_heuristic);
    /// @brief sets player2's current heuristic
    void SetHeuristicTypePlayer2(Heuristic::eHeuristicType_t a_heuristic);
    /// @brief sets the player1 type
    void SetPlayer1Type(ePlayerType_t a_playerType);
    /// @brief sets the player2 type
    void SetPlayer2Type(ePlayerType_t a_playerType);
    /// @brief sets the minimax depth for player1
    /// if a_depth is lower or equal to 0, minimax depth will be set to GAME1V1CONFIG_DEPTH_AUTOADJUST,
    /// which means autoadjust value depending on complexity of search tree
    void SetMinimaxDepthPlayer1(int32_t a_depth);
    /// @brief sets the minimax depth for player2
    /// if a_depth is lower or equal to 0, minimax depth will be set to GAME1V1CONFIG_DEPTH_AUTOADJUST,
    /// which means autoadjust value depending on complexity of search tree
    void SetMinimaxDepthPlayer2(int32_t a_depth);
    /// @brief set player1's starting coordinate
    void SetPlayer1StartingCoord(const Coordinate &a_startingCoord);
    /// @brief set player2's starting coordinate
    void SetPlayer2StartingCoord(const Coordinate &a_startingCoord);

private:
    /// currently selected heuristic for player1. It can be used as the index for Heuristic::m_heuristicData
    Heuristic::eHeuristicType_t m_currentHeuristicPlayer1;
    /// currently selected heuristic for player2. It can be used as the index for Heuristic::m_heuristicData
    Heuristic::eHeuristicType_t m_currentHeuristicPlayer2;
    /// type of player of player1
    ePlayerType_t m_player1Type;
    /// type of player of player2
    ePlayerType_t m_player2Type;
    /// depth for the minimax algorithm for player1. GAME1V1CONFIG_DEPTH_AUTOADJUST means autoadjust
    int32_t m_minimaxDepthPlayer1;
    /// depth for the minimax algorithm for player2. GAME1V1CONFIG_DEPTH_AUTOADJUST means autoadjust
    int32_t m_minimaxDepthPlayer2;
    /// red channel of player1's colour
    uint8_t m_player1Red;
    /// green channel of player1's colour
    uint8_t m_player1Green;
    /// blue channel of player1's colour
    uint8_t m_player1Blue;
    /// red channel of player2's colour
    uint8_t m_player2Red;
    /// green channel of player2's colour
    uint8_t m_player2Green;
    /// blue channel of player2's colour
    uint8_t m_player2Blue;
    /// player1's starting coordinate
    Coordinate m_player1StartingCoord;
    /// player2's starting coordinate
    Coordinate m_player2StartingCoord;

    friend class Singleton<Game1v1Config>;
    Game1v1Config();
    ~Game1v1Config();
};

#endif // __GUI_GAME1V1_CONFIG__
