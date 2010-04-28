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

#include "heuristic.h"
#include "singleton.h"

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
    
    /// @brief contains the data relative to a heuristic
    typedef struct
    {
        Heuristic::EvalFunction_t m_evalFunction;
        std::string m_name;
        std::string m_description;
    } sHeuristicConfig_t;

    /// @brief returns the current heuristic type
    Heuristic::eHeuristicType_t GetHeuristicType() const; 
    /// @brief sets the current heuristic
    void SetHeuristicType(Heuristic::eHeuristicType_t a_heuristic);
    /// @brief returns the data of 'a_heuristic'
    const sHeuristicConfig_t& GetHeuristicData(Heuristic::eHeuristicType_t a_heuristic) const;
    /// @brief sets the player1 type
    void SetPlayer1Type(ePlayerType_t a_playerType);
    /// @brief returns true if player1 is set to computer type
    bool IsPlayer1Computer();
    /// @brief sets the player2 type
    void SetPlayer2Type(ePlayerType_t a_playerType);
    /// @brief returns true if player2 is set to computer type
    bool IsPlayer2Computer();
    /// @brief sets the minimax depth
    /// if a_depth is lower or equal to 0, minimax depth will be set to GAME1V1CONFIG_DEPTH_AUTOADJUST,
    /// which means autoadjust value depending on complexity of search tree
    void SetMinimaxDepth(int32_t a_depth);
    /// @brief returns the minimax depth
    /// GAME1V1CONFIG_DEPTH_AUTOADJUST means autoadjust value depending on complexity of search tree
    int32_t GetMinimaxDepth() const;
    /// @brief gets the colour of player1
    /// saves the values in the 3 paremeter references
    void GetPlayer1Colour(uint8_t &red, uint8_t &green, uint8_t &blue) const;
    /// @brief gets the colour of player2
    /// saves the values in the 3 paremeter references
    void GetPlayer2Colour(uint8_t &red, uint8_t &green, uint8_t &blue) const;
    
private:
    /// currently selected heuristic. It will be used as the index for m_heuristicData
    Heuristic::eHeuristicType_t m_currentHeuristic;
    /// array with the data of all configured heuristics
    /// indexes are Heuristic::eHeuristicType_t (in heuristic.h)
    sHeuristicConfig_t m_heuristicData[Heuristic::e_heuristicCount];
    /// type of player of player1
    ePlayerType_t m_player1Type;
    /// type of player of player2
    ePlayerType_t m_player2Type;
    /// depth for the minimax algorithm. 0 means unset
    int32_t m_minimaxDepth;
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

    friend class Singleton<Game1v1Config>;
    Game1v1Config();    
    ~Game1v1Config();  
};

#endif // __GUI_GAME1V1_CONFIG__