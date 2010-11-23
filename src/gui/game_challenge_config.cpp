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
/// @file  game_challenge_config.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 16-Nov-2010  Original development
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#include "gui/game_challenge_config.h"

static const BlockemChallenge DEFAULT_CHALLENGE = BlockemChallenge(); // uninitialised challenge

GameChallengeConfig::GameChallengeConfig() :
    Singleton<GameChallengeConfig>(),
    m_challenge(DEFAULT_CHALLENGE)
{
}

GameChallengeConfig::~GameChallengeConfig()
{
}

const BlockemChallenge& GameChallengeConfig::GetBlockemChallenge() const
{
    return m_challenge;
}

void GameChallengeConfig::SetBlockemChallenge(const BlockemChallenge &a_blockemChallenge)
{
    m_challenge = a_blockemChallenge;
}
