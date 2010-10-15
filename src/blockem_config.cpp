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
/// @file  blockem_config.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 23-Sep-2010  Original development
/// @endhistory
///
// ============================================================================

#include <iostream>
#include <fstream>
#include <stdexcept> // std::runtime_error
#include <gtkmm.h>   // g_file_test, G_FILE_TEST_IS_REGULAR
// XML parsing
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "blockem_config.h"
#include "config.h" // PACKAGE

/// expected name of the configuration file. Yes, it is hardcoded, but it only matters
/// to this class, so who cares?
static const char CONFIG_FILE_NAME[] = "blockem.conf";

// defaults for when config file can't be loaded
static const char  DEFAULT_LANGUAGE[] = "en_UK"; // english from united kingdom as default

/// null-terminated array which contains all valid languages supported by blockem
static const char* g_validLanguagesList[] =
{
    "en_UK",
    "es_ES",
    NULL
};


BlockemConfig::BlockemConfig() throw (std::runtime_error):
    Singleton<BlockemConfig>(),
    m_languageISO(DEFAULT_LANGUAGE)
{

// full PATH where the config file is stored. It depends on the platform
// in windows it will be stored in ./etc/, in any other
// platform it'll be saved in $HOME/.blockem/
#ifdef WIN32
    // directory "etc". Do not ask why. There is no real reason
    m_configFileFull = std::string("./etc/") + std::string(CONFIG_FILE_NAME);
#else
    // $HOME/.blockem/
    char *home;
    home = getenv("HOME");

    m_configFileFull =
        std::string (home)   +
        std::string("/.")    +
        std::string(PACKAGE) +
        std::string("/")     +
        std::string(CONFIG_FILE_NAME);
#endif // WIN32


// config file is only needed in win32 for now, since it is used to set the
// locale of the application (for i18n)
// no need to do anything in any other OS's for now since their locale are
// set differently (through the LANG environment variable)
#ifdef WIN32
    // try to load configuration from config file. If it doesn't exist create
    // a default one
    if ( (g_file_test(m_configFileFull.c_str(), G_FILE_TEST_IS_REGULAR)) ||
         (CreateDefaultConfigFile()) )
    {
        // load config from config file (it's a bit redundant if the config file
        // had just been created, but it should only happen the 1st time app
        // is run)
        LoadConfigFromXmlFile();
    }
#ifdef DEBUG_PRINT
    else
    {
        // default config file could not be created. Default settings have
        // already been loaded through this constructor's initialisation list
        std::cerr << "Could not create default configuration file" << std::endl;
        std::cerr << "Loading default settings..." << std::endl;
    }
#endif // DEBUG_PRINT

#endif // WIN32
}

BlockemConfig::~BlockemConfig()
{
}

const std::string& BlockemConfig::GetLanguageISO() const
{
    return m_languageISO;
}

bool BlockemConfig::CreateDefaultConfigFile()
{
    std::string configFileDirectory(
            m_configFileFull.substr(0, m_configFileFull.find_last_of("/")));

    if (!g_file_test(configFileDirectory.c_str(), G_FILE_TEST_IS_DIR))
    {
#ifdef DEBUG_PRINT
        std::cerr << configFileDirectory << " doesn't exist. Creating..." << std::endl;
#endif

        // create the directory first of all
        if (g_mkdir_with_parents(configFileDirectory.c_str(), 0754) != 0)
        {
#ifdef DEBUG_PRINT
        std::cerr << configFileDirectory << " couldn't be created" << std::endl;
#endif
            // directory could not be created
            return false;
        }
    }

    // directory has been properly created. Go ahead with file now
    std::ofstream oStr(m_configFileFull.c_str(),
                       std::ios_base::out | std::ios_base::trunc);

    if(!oStr)
    {
        // file could not be created
#ifdef DEBUG_PRINT
        std::cerr << m_configFileFull << " couldn't be opened" << std::endl;
#endif
        return false;
    }

    // default config file. Includes DTD and comments!
    oStr << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"                                << std::endl;
    oStr << "<!DOCTYPE blockem_config ["                                                << std::endl;
    oStr << "<!ELEMENT blockem_config (language)>"                                      << std::endl;
    oStr << "<!ELEMENT language (#PCDATA)>"                                             << std::endl;
    oStr << "]>"                                                                        << std::endl;
    oStr << ""                                                                          << std::endl;
    oStr << "<blockem_config>"                                                          << std::endl;
    oStr << "  <!--"                                                                    << std::endl;
    oStr << "    This is the language blockem will use at startup. If it is changed"    << std::endl;
    oStr << "    using the menu, this option will be overwritten. Bear in mind this"    << std::endl;
    oStr << "    is only used in Windows OS. To pick a language in GNU linux use"       << std::endl;
    oStr << "    LANG environment variable instead, i.e. run in the command line"       << std::endl;
    oStr << "    \"$ LANG=es_ES blockem\""                                              << std::endl;
    oStr << ""                                                                          << std::endl;
    oStr << "    This is a list of available languages. This option should be set to"   << std::endl;
    oStr << "    the string that follows the full language name. For example, to start" << std::endl;
    oStr << "    up blockem in English you should write:"                               << std::endl;
    oStr << "      <language>en_UK</language>"                                          << std::endl;
    oStr << ""                                                                          << std::endl;
    oStr << "    The offending list:"                                                   << std::endl;
    oStr << "      + English (UK):    en_UK"                                            << std::endl;
    oStr << "      + Spanish (Spain): es_ES"                                            << std::endl;
    oStr << ""                                                                          << std::endl;
    oStr << "    Default is en_UK. If a wrong option (or no option) is specified, "     << std::endl;
    oStr << "    default will be used."                                                 << std::endl;
    oStr << "  -->"                                                                     << std::endl;
    oStr << "  <language>en_UK</language>"                                              << std::endl;
    oStr << "</blockem_config>"                                                         << std::endl;

    oStr.close();

    return true;
}

void BlockemConfig::LoadConfigFromXmlFile() throw (std::runtime_error)
{
    // based (almost copied and pasted) on http://www.yolinux.com/TUTORIALS/GnomeLibXml2.html
    xmlNode* cur_node = NULL;
    xmlNode* root = NULL;
    xmlDocPtr doc;
    xmlChar* strValue = NULL;

    // ensure config file exists
    if (!g_file_test(m_configFileFull.c_str(), G_FILE_TEST_IS_REGULAR))
    {
        throw new std::runtime_error(
            std::string("Config file does not exist: ") + m_configFileFull);
    }

    // open XML doc and try to parse it
    doc = xmlParseFile(m_configFileFull.c_str());
    if (doc == NULL)
    {
        throw new std::runtime_error(
            m_configFileFull + std::string(" could not be parsed"));
    }

    // retrieve root element
    root = xmlDocGetRootElement(doc);

    // XML file MUST have a root element called blockem_config
    if ( (root == NULL) ||
         (root->name == NULL) ||
         xmlStrcmp(root->name, (const xmlChar*) "blockem_config") )
    {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        throw new std::runtime_error(
            m_configFileFull + std::string(": Root element does not exist or is not called \"blockem_config\""));
    }

    // root's children. They are inside a loop to ensure they can be written to the file in any order
    // if any of the expected options is not present default settings for it will be loaded instead
    for (cur_node = root->children; cur_node != NULL; cur_node = cur_node->next)
    {
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "language") == 0) )
        {
            // xml element called "language"
            strValue = xmlNodeGetContent(cur_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \"" << (const char*)cur_node->name
                      << "\" --> \"" << (const char*)strValue << "\""
                      << std::endl;
#endif
            SetLanguageISO(std::string((const char*)strValue));
            xmlFree(strValue);
        }
    }

    // free the document
    xmlFreeDoc(doc);

    // Free the global variables that may
    // have been allocated by the parser
    xmlCleanupParser();
}

bool BlockemConfig::SetLanguageISO(const std::string &a_lang)
{
    int32_t i = 0;
    while (g_validLanguagesList[i] != NULL)
    {
        if (strcmp(a_lang.c_str(), g_validLanguagesList[i]) == 0)
        {
            // it is a valid language string. Set it and exit
            m_languageISO = a_lang;
            return true;
        }

        i++;
    }

    // a_lang is not a valid language string. Do nothing (keep the old value)
#ifdef DEBUG_PRINT
    std::cerr << a_lang << " is not a supported language string" << std::endl;
    std::cerr << PACKAGE << " will still be using " << m_languageISO << std::endl;
#endif

    return false;
}
