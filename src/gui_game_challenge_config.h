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
/// @file  gui_game_challenge_config.h
/// @brief Singleton which stores GUI related configuration data for challenge games
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 16-Nov-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef __GUI_GAME_CHALLENGE_CONFIG__
#define __GUI_GAME_CHALLENGE_CONFIG__

#include "singleton.h"
#include "coordinate.h"
#include "blockem_challenge.h"


/// singleton which stores the configuration to apply to challenge games
class GameChallengeConfig :
    public Singleton<GameChallengeConfig>
{
public:
    /// @return currently configured blockem challenge
    const BlockemChallenge& GetBlockemChallenge() const;

    /// @brief set blockem challenge
    void SetBlockemChallenge(const BlockemChallenge &a_blockemChallenge);

private:
    /// challenge saved in this config singleton
    BlockemChallenge m_challenge;

    friend class Singleton<GameChallengeConfig>;
    GameChallengeConfig();
    ~GameChallengeConfig();
};

#endif // __GUI_GAME_CHALLENGE_CONFIG__
