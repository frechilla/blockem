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
#include <string.h> // strcmp

#include "blockem_config.h"
#include "config.h" // PACKAGE

/// expected name of the configuration file. Yes, it is hardcoded, but it only matters
/// to this class, so who cares?
static const char CONFIG_FILE_NAME[] = "blockem.conf";

/// default language
static const char  DEFAULT_LANGUAGE[] = "en_UK"; // english from united kingdom as default


BlockemConfig::BlockemConfig() throw (std::runtime_error):
    Singleton<BlockemConfig>(),
    m_languageISO(DEFAULT_LANGUAGE),
    m_setCompletedChalleges() // empty set
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
    //TODO have a look at Glib::get_home_dir
    home = getenv("HOME");

    m_configFileFull =
        std::string (home)   +
        std::string("/.")    +
        std::string(PACKAGE) +
        std::string("/")     +
        std::string(CONFIG_FILE_NAME);
#endif // WIN32


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
}

BlockemConfig::~BlockemConfig()
{
    if (SaveConfigIntoXmlFile() == false)
    {
#ifdef DEBUG_PRINT
        std::cerr << "Could not save current configuration into "
                  << m_configFileFull
                  << std::endl;
#endif
    }
}

const std::string& BlockemConfig::GetLanguageISO() const
{
    return m_languageISO;
}

bool BlockemConfig::IsChallengeCompleted(const std::string &a_challengeName) const
{
    completedChallengesContainer_t::iterator it =
        m_setCompletedChalleges.find(a_challengeName);

    return (it != m_setCompletedChalleges.end());
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
    oStr << "<!ELEMENT blockem_config (language, challenges_completed)>"                << std::endl;
    oStr << "  <!ELEMENT language (#PCDATA)>"                                           << std::endl;
    oStr << "  <!ELEMENT challenges_completed (challenge_name*)>"                       << std::endl;
    oStr << "    <!ELEMENT challenge_name (#PCDATA)>"                                   << std::endl;
    oStr << "]>"                                                                        << std::endl;
    oStr << ""                                                                          << std::endl;
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
    oStr << "    Default is en_UK"                                                      << std::endl;
    oStr << "  -->"                                                                     << std::endl;
    oStr << "  <language>en_UK</language>"                                              << std::endl;
    oStr << ""                                                                          << std::endl;
    oStr << "  <!--"                                                                    << std::endl;
    oStr << "    Between the \"challenges_completed\" tags there is a list of"          << std::endl;
    oStr << "    challenges that have been already completed by the user."              << std::endl;
    oStr << ""                                                                          << std::endl;
    oStr << "    As described in file blockem_challenge.dtd every challenge xml"        << std::endl;
    oStr << "    has to have a \"name\" property which identifies each one of the"      << std::endl;
    oStr << "    challenges. This unique identifier is used to identify those"          << std::endl;
    oStr << "    challenges that have been completed."                                  << std::endl;
    oStr << "  -->"                                                                     << std::endl;
    oStr << "  <challenges_completed>"                                                  << std::endl;
    oStr << "  </challenges_completed>"                                                 << std::endl;
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

        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "challenges_completed") == 0) )
        {
#ifdef DEBUG_PRINT
            std::cout << "Parsing \""
                      << cur_node->name
                      << "\":"
                      << std::endl;
#endif

            // xml element called "challenges_completed"
            // it contains a list of XML items (completed challenges)
            xmlNode* challengeCompleteNode;
            for (challengeCompleteNode = cur_node->children;
                 challengeCompleteNode != NULL;
                 challengeCompleteNode = challengeCompleteNode->next)
            {
                if ( (challengeCompleteNode->type == XML_ELEMENT_NODE) &&
                     (xmlStrcmp(challengeCompleteNode->name, (const xmlChar*) "name") == 0) )
                {
                    strValue = xmlNodeGetContent(challengeCompleteNode);
#ifdef DEBUG_PRINT
                    std::cout << "\t\""
                              << (const char*)challengeCompleteNode->name
                              << "\" --> \""
                              << (const char*)strValue
                              << "\""
                              << std::endl;
#endif
                    xmlFree(strValue);
                }
            }
        }
    }

    // free the document
    xmlFreeDoc(doc);

    // Free the global variables that may
    // have been allocated by the parser
    xmlCleanupParser();
}

bool BlockemConfig::SaveConfigIntoXmlFile()
{
    // based  on LoadConfigFromXmlFile
    xmlNode* cur_node = NULL;
    xmlNode* root = NULL;
    xmlDocPtr doc;
    xmlChar* strValue = NULL;

    // ensure config file exists
    // What this method does is OVERWRITING the old configuration file with
    // the new settings. Parsing XML in C++ is hell, and this is a way of
    // easing it a bit.
    if (!g_file_test(m_configFileFull.c_str(), G_FILE_TEST_IS_REGULAR))
    {
        // A default configuration file should have been created at
        // startup. If it wasn't we won't be able to overwrite the settings
        return false;
    }

    // open XML doc and try to parse it
    doc = xmlParseFile(m_configFileFull.c_str());
    if (doc == NULL)
    {
        return false;
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
        return false;
    }

    // root's children. They are inside a loop to ensure they can be written to the file in any order
    // if any of the expected options is not present default settings for it will be loaded instead
    for (cur_node = root->children; cur_node != NULL; cur_node = cur_node->next)
    {
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "language") == 0) )
        {
            // xml element called "language"
            strValue = xmlCharStrdup(m_languageISO.c_str());
            xmlNodeSetContent(cur_node, strValue);
#ifdef DEBUG_PRINT
            std::cout << "Saving \""
                      << (const char*)cur_node->name
                      << "\"--> \""
                      << (const char*)strValue
                      << "\""
                      << std::endl;
#endif
            xmlFree(strValue);
        }

        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "challenges_completed") == 0) )
        {
            // xml element called "challenges_completed"
            // it contains a list of XML items (completed challenges)
            xmlNode* challengeCompleteNode;

            // remove first all complete challenges
            for (challengeCompleteNode = cur_node->children;
                 challengeCompleteNode != NULL;
                )
            {
                xmlNode* next = challengeCompleteNode->next;
                xmlUnlinkNode(challengeCompleteNode);
                xmlFreeNodeList(challengeCompleteNode);
                challengeCompleteNode = next;
            }

            completedChallengesContainer_t::const_iterator it;
            for (it =  m_setCompletedChalleges.begin();
                 it != m_setCompletedChalleges.end();
                 it++)
            {
                // save now the current list of challenges completed
                challengeCompleteNode = xmlNewNode(NULL, (const xmlChar*) "name");
                xmlNodeSetContent(challengeCompleteNode, (const xmlChar*) it->c_str());
                xmlAddChild(cur_node, challengeCompleteNode);
            }
        }
    }

    // save the document
    xmlSaveFile(m_configFileFull.c_str(), doc);

    // free the document
    xmlFreeDoc(doc);

    // Free the global variables that may
    // have been allocated by the parser
    xmlCleanupParser();

    return true;
}

void BlockemConfig::SetLanguageISO(const std::string &a_lang)
{
    // Set the language string and exit
    m_languageISO = a_lang;
    
#ifdef DEBUG_PRINT
    std::cout << "Language has been set to " << m_languageISO << std::endl;
#endif
}

void BlockemConfig::SetChallengeCompleted(const std::string &a_challengeName)
{
    completedChallengesContainer_t::iterator it =
        m_setCompletedChalleges.find(a_challengeName);

    if (it != m_setCompletedChalleges.end())
    {
#ifdef DEBUG_PRINT
        std::cerr << a_challengeName << " has been already set as completed" << std::endl;
#endif
        return;
    }

    m_setCompletedChalleges.insert(a_challengeName);
}
