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
/// @file  blockem_challenge.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 26-Sep-2010  Original development
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif // DEBUG_PRINT

#include <stdexcept> // std::runtime_error
#include <gtkmm.h>   // g_file_test, G_FILE_TEST_IS_REGULAR
// XML parsing
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "blockem_challenge.h"

static const char       DEFAULT_CHALLENGE_NAME[] = "default challenge";
static const int32_t    DEFAULT_NROWS = 14;
static const int32_t    DEFAULT_NCOLS = 14;
static const bool       DEFAULT_OPPONENT_ACTIVE = false;
static const Coordinate DEFAULT_OPPONENT_STARTING_COORD = Coordinate(0, 0);
static const Coordinate DEFAULT_CHALLENGER_STARTING_COORD = Coordinate(0, 0);
static const bool       DEFAULT_OPPONENT_PIECES_PRESENT[e_numberOfPieces] =
{
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
};
static const bool       DEFAULT_CHALLENGER_PIECES_PRESENT[e_numberOfPieces] =
{
    true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
};

static const std::set<Coordinate> DEFAULT_OPPONENT_TAKEN_SQUARES = std::set<Coordinate>();
static const std::set<Coordinate> DEFAULT_CHALLENGER_TAKEN_SQUARES = std::set<Coordinate>();


BlockemChallenge::BlockemChallenge() :
    m_challengeName(DEFAULT_CHALLENGE_NAME),
    m_nRows(DEFAULT_NROWS),
    m_nColumns(DEFAULT_NCOLS),
    m_opponentActive(DEFAULT_OPPONENT_ACTIVE),
    m_opponentTakenSquares(DEFAULT_OPPONENT_TAKEN_SQUARES),
    m_opponentStartingCoord(DEFAULT_OPPONENT_STARTING_COORD),
    m_challengerTakenSquares(DEFAULT_CHALLENGER_TAKEN_SQUARES),
    m_challengerStartingCoord(DEFAULT_CHALLENGER_STARTING_COORD)
{
    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
    {
        m_opponentPiecesPresent[i] = DEFAULT_OPPONENT_PIECES_PRESENT[i];
        m_challengerPiecesPresent[i] = DEFAULT_CHALLENGER_PIECES_PRESENT[i];
    }
}

BlockemChallenge::BlockemChallenge(const std::string &a_path) throw (std::runtime_error)
{
    LoadXMLChallenge(a_path);
}

BlockemChallenge::~BlockemChallenge()
{
}

void BlockemChallenge::LoadXMLChallenge(const std::string &a_path) throw (std::runtime_error)
{
    // based (almost copied and pasted) on http://www.yolinux.com/TUTORIALS/GnomeLibXml2.html
    xmlNode* cur_node = NULL;
    xmlNode* root = NULL;
    xmlDocPtr doc;
    xmlChar* strValue = NULL;

    // ensure config file exists
    if (!g_file_test(a_path.c_str(), G_FILE_TEST_IS_REGULAR))
    {
        throw new std::runtime_error(
            std::string("Challenge .xml file doesn't exist: ") + a_path);
    }

    // open XML doc and try to parse it
    doc = xmlParseFile(a_path.c_str());
    if (doc == NULL)
    {
        throw new std::runtime_error(
            a_path + std::string(" could not be parsed. XML Error"));
    }

    // retrieve root element
    root = xmlDocGetRootElement(doc);

    // XML file MUST have a root element called blockem_challenge
    if ( (root == NULL) ||
         (root->name == NULL) ||
         xmlStrcmp(root->name, (const xmlChar*) "blockem_challenge") )
    {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        throw new std::runtime_error(
                a_path + std::string(": Root element does not exist or is not called \"blockem_challenge\""));
    }

    strValue = xmlGetProp(cur_node, (const xmlChar*)"name");
    if (strValue == NULL)
    {
        // property not present
        xmlFreeDoc(doc);
        xmlCleanupParser();
        throw new std::runtime_error(
                a_path + std::string(": Root element does not have mandatory property \"name\""));
    }
    if (xmlStrcmp(strValue, (const xmlChar*) "") == 0)
    {
        // property not present
        xmlFree(strValue);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        throw new std::runtime_error(
                a_path + std::string(": Root element's property \"name\" can't be empty"));
    }

#ifdef DEBUG_PRINT
            std::cout << "XML Parsing blockem_challenge name: \""
                      << (const char*) strValue << "\""
                      << std::endl;
#endif
    SetChallengeName(std::string(strValue));
    xmlFree(strValue);


    // Ensure board, opponent and challenger tags are present
    bool boardTagPresent      = false;
    bool opponentTagPresent   = false;
    bool challengerTagPresent = false;
    for (cur_node = root->children; cur_node != NULL; cur_node = cur_node->next)
    {
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "board") == 0) )
        {
            // xml element called "language"
            strValue = xmlNodeGetContent(cur_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)cur_node->name << "\""
                      << std::endl;
#endif
            xmlFree(strValue);
        }
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "opponent") == 0) )
        {
            // xml element called "language"
            strValue = xmlNodeGetContent(cur_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)cur_node->name << "\""
                      << std::endl;
#endif
            xmlFree(strValue);
        }
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "challenger") == 0) )
        {
            // xml element called "language"
            strValue = xmlNodeGetContent(cur_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)cur_node->name << "\""
                      << std::endl;
#endif
            xmlFree(strValue);
        }
    }

    // free the document
    xmlFreeDoc(doc);

    // Free the global variables that may
    // have been allocated by the parser
    xmlCleanupParser();
}
