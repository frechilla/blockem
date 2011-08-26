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
/// @file  blockem_config.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 23-Sep-2010  Original development
/// @endhistory
///
// ============================================================================

#ifndef __BLOCKEM_CONFIG__
#define __BLOCKEM_CONFIG__

#include <set>
#include "singleton.h"

/// types of game available in blockem
typedef enum
{
    e_gameType1vs1 = 0,        // see game1v1.h
    e_gameTypeTotalAllocation, // see game_total_allocation.h
    e_gameTypeChallenge,       // see game_challenge.h
    e_gameType4Players,        // see game_4players.h
} e_blockemGameType_t;

/// singleton which stores the general configuration to apply to blockem
class BlockemConfig :
    public Singleton<BlockemConfig>
{
public:
    /// @return ISO string of the language blockem has been configured to run (for example: "en_UK")
    const std::string& GetLanguageISO() const;
    
    /// @brief sets the ISO string of the language blockem has been configured to run in
    void SetLanguageISO(const std::string &a_lang);
    
    /// @return true if the challenge whose name is 'a_challengeName' is marked as completed
    /// @param a_challengeName the name of the challenge requested for checking
    bool IsChallengeCompleted(const std::string &a_challengeName) const;

    /// @brief sets a specific challenge's name as "completed"
    /// @param 'a_challengeName' name of the challenge
    void SetChallengeCompleted(const std::string &a_challengeName);

private:
    /// full path to config file (includes config file name)
    /// It is stored in a variable because it is calculated at runtime
    /// depending on the platform
    std::string m_configFileFull;

    /// language string. iso 839 and 3166
    /// http://en.wikipedia.org/wiki/ISO_639
    /// http://en.wikipedia.org/wiki/ISO_3166
    std::string m_languageISO;

    /// set of completed challenges
    typedef std::set<std::string> completedChallengesContainer_t;
    completedChallengesContainer_t m_setCompletedChalleges;

    /// Creates a default config file in the expected path
    /// WARNING: it overwrites the current data if the file exists
    /// @return false if the file could not be created. True otherwise
    bool CreateDefaultConfigFile();

    /// Loads blockem configuration from the config xml file
    /// the config file name is calculated in the constructor
    /// (m_pathToConfigFile + CONFIG_FILE_NAME)
    void LoadConfigFromXmlFile() throw (std::runtime_error);

    /// this method OVERWRITES the old configuration file with
    /// the new settings. It won't create a new XML file, it just overwrites
    /// the settings with the new configuration
    /// @return true if thw writing success. False otherwise
    bool SaveConfigIntoXmlFile();

    friend class Singleton<BlockemConfig>;
    BlockemConfig() throw (std::runtime_error);
    ~BlockemConfig();
};

#endif // __BLOCKEM_CONFIG__
