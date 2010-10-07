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

#include "singleton.h"

/// singleton which stores the general configuration to apply to blockem
class BlockemConfig :
    public Singleton<BlockemConfig>
{
public:
    /// @return ISO string of the language blockem has been configured to run (for example: "en_UK")
    const std::string& GetLanguageISO() const;

private:
    /// language string. iso 839 and 3166
    /// http://en.wikipedia.org/wiki/ISO_639
    /// http://en.wikipedia.org/wiki/ISO_3166
    std::string m_languageISO;

    /// full path to config file (includes config file name)
    /// It is stored in a variable because it is calculated at runtime
    /// depending on the platform
    std::string m_configFileFull;

    /// @brief sets ISO string of the language blockem has been configured to run
    /// it checks if a_lang represents a supported language. If it doesn't nothing
    /// will be done and the old value will be kept
    /// @return true if new language was successfully set. False otherwise
    bool SetLanguageISO(const std::string &a_lang);

    /// Creates a default config file in the expected path
    /// WARNING: it overwrites the current data if the file exists
    /// @return false if the file could not be created. True otherwise
    bool CreateDefaultConfigFile();

    /// Loads blockem configuration from the config xml file
    /// the config file name is calculated in the constructor
    /// (m_pathToConfigFile + CONFIG_FILE_NAME)
    void LoadConfigFromXmlFile() throw (std::runtime_error);

    friend class Singleton<BlockemConfig>;
    BlockemConfig() throw (std::runtime_error);
    ~BlockemConfig();
};

#endif // __BLOCKEM_CONFIG__
