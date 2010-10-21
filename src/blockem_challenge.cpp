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
///           Faustino Frechilla 05-Oct-2010  Support for infochallenge tag
///           Faustino Frechilla 07-Oct-2010  i18n
/// @endhistory
///
// ============================================================================

#ifdef DEBUG_PRINT
#include <iostream>
#endif // DEBUG_PRINT
#include <sstream>   // std::istringstream, std::stringstream
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::set_union
#include <set>
#include <gtkmm.h>   // g_file_test, G_FILE_TEST_IS_REGULAR
// XML parsing
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "gettext.h" // i18n
#include "blockem_challenge.h"

static const uint32_t   ERROR_STRING_BUFFER_SIZE = 256;

static const char       DEFAULT_CHALLENGE_NAME[] = "default challenge";
static const int32_t    DEFAULT_NROWS = 0;
static const int32_t    DEFAULT_NCOLS = 0;
static const Coordinate DEFAULT_CHALLENGER_STARTING_COORD = Coordinate(COORD_UNINITIALISED, COORD_UNINITIALISED);

BlockemChallenge::BlockemChallenge() :
    m_xmlDoc(NULL),
    m_challengeName(DEFAULT_CHALLENGE_NAME),
    m_nRows(DEFAULT_NROWS),
    m_nColumns(DEFAULT_NCOLS),
    m_challengerStartingCoord(DEFAULT_CHALLENGER_STARTING_COORD)
{
    // load defaults
    Reset();
}

BlockemChallenge::BlockemChallenge(const std::string &a_path) throw (std::runtime_error)
{
    LoadXMLChallenge(a_path);
}

BlockemChallenge::~BlockemChallenge()
{
}

bool BlockemChallenge::Initialised() const
{
    // when a new challenge is loaded, the board has to have at least 1 square
    // an uninitialised challenge sets up its board to 0x0 (impossible to play)
    return ((m_nRows != DEFAULT_NROWS) && (m_nColumns != DEFAULT_NCOLS));
}

void BlockemChallenge::Reset()
{
    m_xmlDoc = NULL;
    m_challengeName = DEFAULT_CHALLENGE_NAME;
    m_nRows = DEFAULT_NROWS;
    m_nColumns = DEFAULT_NCOLS;
    m_challengerStartingCoord = DEFAULT_CHALLENGER_STARTING_COORD;

    m_opponentTakenSquares.clear();
    m_challengerTakenSquares.clear();

    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
    {
        // default is having NO pieces at all
        m_challengerPiecesPresent[i] = false;
    }
}

void BlockemChallenge::LoadXMLChallenge(const std::string &a_path) throw (std::runtime_error)
{
    // XML parsing based on http://www.yolinux.com/TUTORIALS/GnomeLibXml2.html
    xmlNode* cur_node   = NULL;
    xmlNode* root       = NULL;
    xmlChar* strValue   = NULL;

    // Deletes current challenge stored in this
    Reset();

    // ensure config file exists
    if (!g_file_test(a_path.c_str(), G_FILE_TEST_IS_REGULAR))
    {
        char errorStringBuffer[ERROR_STRING_BUFFER_SIZE];
        snprintf(errorStringBuffer,
                ERROR_STRING_BUFFER_SIZE,
                // i18n TRANSLATORS: Please, leave that %s as it is. It will be replaced
                // i18n by the path to the file that could not be loaded
                // i18n Thank you for contributing to this project
                _("Challenge .xml file doesn't exist: %s"),
                a_path.c_str());

        throw std::runtime_error(errorStringBuffer);
    }

    // open XML doc and try to parse it
    m_xmlDoc = xmlParseFile(a_path.c_str());
    if (m_xmlDoc == NULL)
    {
        XMLParsingFatalError(a_path, _("XML syntax error"));
    }

    // retrieve root element
    root = xmlDocGetRootElement(m_xmlDoc);

    // XML file MUST have a root element called blockem_challenge
    if ( (root == NULL)       ||
         (root->name == NULL) ||
         xmlStrcmp(root->name, (const xmlChar*) "blockem_challenge") )
    {
        XMLParsingFatalError(a_path,
            // i18n TRANSLATORS: Please, leave quoted strings as they are since they
            // i18n reference to xml tags
            _("Root element does not exist or is not called \"blockem_challenge\""));
    }

    strValue = xmlGetProp(root, (const xmlChar*)"name");
    if (strValue == NULL)
    {
        // property not present
        XMLParsingFatalError(a_path,
            // i18n TRANSLATORS: Please, leave quoted strings as they are since they
            // i18n reference to xml tags
            _("Root element does not have mandatory property \"name\""));
    }
    if (xmlStrcmp(strValue, (const xmlChar*) "") == 0)
    {
        // property not present
        xmlFree(strValue);

        XMLParsingFatalError(a_path,
            // i18n TRANSLATORS: Please, leave quoted strings as they are since they
            // i18n reference to xml tags
            _("Root element's property \"name\" can't be empty"));
    }

#ifdef DEBUG_PRINT
    std::cout << "XML Parsing blockem_challenge name: \""
              << (const char*) strValue << "\""
              << std::endl;
#endif
    SetChallengeName(std::string((const char*)strValue));
    xmlFree(strValue);


    // Ensure board, opponent and challenger tags are present
    bool boardTagPresent      = false;
    bool opponentTagPresent   = false;
    bool challengerTagPresent = false;
    for (cur_node = root->children; cur_node != NULL; cur_node = cur_node->next)
    {
        //////////////
        // board tag
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "board") == 0) )
        {
            boardTagPresent = true;
            XMLParseTagBoard(a_path, cur_node);
        }

        /////////////////
        // opponent tag
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "opponent") == 0) )
        {
            // xml element called "opponent"
            opponentTagPresent = true;
            XMLParseTagOpponent(a_path, cur_node);
        }

        /////////////////
        // challenger tag
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "challenger") == 0) )
        {
            // xml element called "challenger"
            challengerTagPresent = true;
            XMLParseTagChallenger(a_path, cur_node);
        }

        /////////////////////
        // infochallenge tag
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "infochallenge") == 0) )
        {
            // this tag is optional. No need to check if it has been set or not
            XMLParseTagInfochallenge(a_path, cur_node);
        }
    }

    // The three mandatory tags MUST be present. Ensure it is true
    if ( (!boardTagPresent)    ||
         (!opponentTagPresent) ||
         (!challengerTagPresent) )
    {
        std::string mandatoryElemMissing;
        if (!boardTagPresent)
        {
            mandatoryElemMissing += " \"board\"";
        }
        if (!opponentTagPresent)
        {
            mandatoryElemMissing += " \"opponent\"";
        }
        if (!challengerTagPresent)
        {
            mandatoryElemMissing += " \"challenger\"";
        }

        XMLParsingFatalError(a_path,
            // i18n TRANSLATORS: This string will be followed by a list
            // i18n of missing xml tags
            std::string(_("Mandatory elements missing:")) + mandatoryElemMissing);
    }

    // m_challengerTakenSquares INTERSECTION m_opponentTakenSquares MUST
    // be an empty set
    STLCoordinateSet_t takenSquaresIntersectionSet;
    // The function std::set_intersection() can be used to construct an
    // intersection of two sets.
    // The two sets are specified by iterator pairs, and the intersection is
    // copied into an output iterator that is supplied as a fifth argument
    // http://www.cplusplus.com/reference/algorithm/set_union/
    // http://www2.roguewave.com/support/docs/sourcepro/edition9/html/stdlibug/8-2.html
    std::set_intersection(
        m_opponentTakenSquares.begin(),
        m_opponentTakenSquares.end(),
        m_challengerTakenSquares.begin(),
        m_challengerTakenSquares.end(),
        std::inserter(
            takenSquaresIntersectionSet,
            takenSquaresIntersectionSet.begin()));

    if (takenSquaresIntersectionSet.empty() == false)
    {
        // This is an error. The same coordinate can't be taken by both
        // opponent and challenger
        std::stringstream  errorCoords;
        STLCoordinateSet_t::const_iterator it;
        for (it  = takenSquaresIntersectionSet.begin();
             it != takenSquaresIntersectionSet.end();
             it++)
        {
            errorCoords << " (" << it->m_row << ", " << it->m_col << ")";
        }

        XMLParsingFatalError(a_path,
            // i18n TRANSLATORS: This string will be followed by a set of coordinates
            // i18n for example: (x, y) (x2, y2)
            std::string(_("Following coords are taken by both challenger and opponent:")) + errorCoords.str());
    }

    // If challenger's coordinate has been set, ensure it is free, this is, it
    // doesn't belong to any of the taken squares sets (m_opponentTakenSquares
    // or m_challengerTakenSquares)
    if (IsChallengerStartingCoordSet())
    {
        if (m_opponentTakenSquares.isPresent(GetChallengerStartingCoord()))
        {
            XMLParsingFatalError(a_path,
                // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                // i18n reference to xml tags
                _("Challenger's starting coordinate is taken by the opponent"));
        }

        if (m_challengerTakenSquares.isPresent(GetChallengerStartingCoord()))
        {
            XMLParsingFatalError(a_path,
                // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                // i18n reference to xml tags
                _("Challenger's starting coordinate is taken by the challenger"));
        }
    }

    // success parsing the file!
    // free the XML document
    xmlFreeDoc(m_xmlDoc);

    // set it back to NULL
    m_xmlDoc = NULL;

    // Free the global variables that may
    // have been allocated by the parser
    xmlCleanupParser();
}

void BlockemChallenge::XMLParseTagBoard(
        const std::string &a_xmlFile,
        xmlNode* board_node) throw (std::runtime_error)
{
    xmlNode* child_node = NULL;
    xmlChar* strValue   = NULL;

#ifdef DEBUG
    assert(board_node->type == XML_ELEMENT_NODE);
    assert(xmlStrcmp(board_node->name, (const xmlChar*) "board") == 0);
#endif

    // board has no attributes and is made up of two elements:
    //     <nrows> and <ncolumns>
    // ensure both are present
    bool nrowsPresent = false;
    bool ncolumnsPresent = false;
    for (child_node = board_node->children; child_node != NULL; child_node = child_node->next)
    {
        //////////////
        // nrows tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "nrows") == 0) )
        {
            // xml element nrows
            nrowsPresent = true;
            strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)board_node->name   << "\" -> \""
                      << (const char*)child_node->name << "\" -> \""
                      << (const char*)strValue         << "\""
                      << std::endl;
#endif
            std::istringstream iStrStream(std::string((const char*)strValue));
            xmlFree(strValue);

            int32_t nRows;
            if (!(iStrStream >> nRows))
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in \"nrows\""));
            }
            if (nRows <= 0)
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in \"nrows\". MUST be a positive number"));
            }

            SetBoardRows(nRows);
        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "nrows") == 0)

        ////////////////
        // ncolumns tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "ncolumns") == 0) )
        {
            // xml element ncolumns
            ncolumnsPresent = true;
            strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)board_node->name   << "\" -> \""
                      << (const char*)child_node->name << "\" -> \""
                      << (const char*)strValue         << "\""
                      << std::endl;
#endif
            std::istringstream iStrStream(std::string((const char*)strValue));
            xmlFree(strValue);

            int32_t nCols;
            if (!(iStrStream >> nCols))
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in \"ncolumns\""));
            }
            if (nCols <= 0)
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in \"ncolumns\". MUST be a positive number"));
            }

            SetBoardColumns(nCols);
        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "ncolumns") == 0)

    } // for (child_node = cur_node->children

    // board configuration (nrows and ncolumns) MUST be present
    if ( (!nrowsPresent) || (!ncolumnsPresent) )
    {
        std::string mandatoryElemMissing;
        if (!nrowsPresent)
        {
            mandatoryElemMissing += " \"nrows\"";
        }
        if (!ncolumnsPresent)
        {
            mandatoryElemMissing += " \"ncolumns\"";
        }

        XMLParsingFatalError(a_xmlFile,
            // i18n TRANSLATORS: Please, leave quoted strings as they are since they
            // i18n reference to xml tags. This string will be followed by a set of
            // i18n missing mandatory xml tags
            std::string(_("Mandatory elements in \"board\" missing:")) + mandatoryElemMissing);
    } // if ( (!nrowsPresent) || (!ncolumnsPresent) )
}

void BlockemChallenge::XMLParseTagOpponent(
        const std::string &a_xmlFile,
        xmlNode* opponent_node) throw (std::runtime_error)
{
    xmlNode* child_node = NULL;
    xmlChar* strValue   = NULL;

#ifdef DEBUG
    assert(opponent_node->type == XML_ELEMENT_NODE);
    assert(xmlStrcmp(opponent_node->name, (const xmlChar*) "opponent") == 0);
#endif

    // opponent tag contains only a set of taken coordinates. They are marked
    // as taken by this "virtual" player, so they can't be used by the
    // challenger

    for (child_node = opponent_node->children; child_node != NULL; child_node = child_node->next)
    {
        //////////////
        // taken tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "taken") == 0) )
        {
            // This is an empty element. It must have both row and column attributes
            Coordinate takenCoord;

            ///////
            // ROW
            strValue = xmlGetProp(child_node, (const xmlChar*)"row");
            if (strValue == NULL)
            {
                // property not present
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("\"opponent\" -> \"taken\" mandatory attribute \"row\" is missing"));
            }
            std::istringstream iStrStream(std::string((const char*)strValue));
            xmlFree(strValue);

            if (!(iStrStream >> takenCoord.m_row))
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in opponent's \"row\" attribute (\"taken\" tag)"));
            }
            if (takenCoord.m_row < 0)
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in opponent's \"row\" attribute (\"taken\" tag). MUST be >= 0"));
            }
            else if (takenCoord.m_row >= GetBoardRows())
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in opponent's \"row\" attribute (\"taken\" tag). MUST be lower than the number of rows of the board"));
            }

            //////////
            // COLUMN
            strValue = xmlGetProp(child_node, (const xmlChar*)"col");
            if (strValue == NULL)
            {
                // property not present
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("\"opponent\" -> \"taken\" mandatory attribute \"col\" is missing"));
            }

            iStrStream.clear(); // clear error flags. MUST be done first
            iStrStream.str(std::string((const char*)strValue));
            xmlFree(strValue);

            if (!(iStrStream >> takenCoord.m_col))
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in opponent's \"col\" attribute (\"taken\" tag)"));
            }
            if (takenCoord.m_col < 0)
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in opponent's \"col\" attribute (\"taken\" tag). MUST be >= 0"));
            }
            else if (takenCoord.m_col >= GetBoardColumns())
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in opponent's \"col\" attribute (\"taken\" tag). MUST be lower than the number of columns of the board"));
            }

#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)opponent_node->name   << "\" -> \""
                      << (const char*)child_node->name << "\" -> ("
                      << takenCoord.m_row << ", "
                      << takenCoord.m_col << ")"
                      << std::endl;

            if (m_opponentTakenSquares.isPresent(takenCoord))
            {
                std::cerr << "Coordinate ("
                          << takenCoord.m_row << ", "
                          << takenCoord.m_col
                          << ") has already been set as taken in "
                          << a_xmlFile
                          << std::endl;
            }
#endif // DEBUG_PRINT

            // save taken coordinate into the opponentTakenSquares set
            m_opponentTakenSquares.insert(takenCoord);

        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "taken") == 0)

    } // for (child_node = opponent_node->children
}

void BlockemChallenge::XMLParseTagChallenger(
        const std::string &a_xmlFile,
        xmlNode* challenger_node) throw (std::runtime_error)
{
    xmlNode* child_node = NULL;
    xmlChar* strValue   = NULL;

#ifdef DEBUG
    assert(challenger_node->type == XML_ELEMENT_NODE);
    assert(xmlStrcmp(challenger_node->name, (const xmlChar*) "challenger") == 0);
#endif

    // starting row and starting column are optional values, but if one of them
    // is set the other one MUST be set too
    Coordinate startingCord(COORD_UNINITIALISED, COORD_UNINITIALISED);

    for (child_node = challenger_node->children; child_node != NULL; child_node = child_node->next)
    {
        //////////////
        // taken tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "taken") == 0) )
        {
            // This is an empty element. It must have both row and column attributes
            Coordinate takenCoord;

            ///////
            // ROW
            strValue = xmlGetProp(child_node, (const xmlChar*)"row");
            if (strValue == NULL)
            {
                // property not present
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("\"challenger\" -> \"taken\" mandatory attribute \"row\" is missing"));
            }
            std::istringstream iStrStream(std::string((const char*)strValue));
            xmlFree(strValue);

            if (!(iStrStream >> takenCoord.m_row))
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"row\" attribute (\"taken\" tag)"));
            }
            if (takenCoord.m_row < 0)
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"row\" attribute (\"taken\" tag). MUST be >= 0"));
            }
            else if (takenCoord.m_row >= GetBoardRows())
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"row\" attribute (\"taken\" tag). MUST be lower than the number of rows of the board"));
            }

            //////////
            // COLUMN
            strValue = xmlGetProp(child_node, (const xmlChar*)"col");
            if (strValue == NULL)
            {
                // property not present
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("\"challenger\" -> \"taken\" mandatory attribute \"col\" is missing"));
            }

            iStrStream.clear(); // clear error flags. MUST be done first
            iStrStream.str(std::string((const char*)strValue));
            xmlFree(strValue);

            if (!(iStrStream >> takenCoord.m_col))
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"col\" attribute (\"taken\" tag)"));
            }
            if (takenCoord.m_col < 0)
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"col\" attribute (\"taken\" tag). MUST be >= 0"));
            }
            else if (takenCoord.m_col >= GetBoardColumns())
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"col\" attribute (\"taken\" tag). MUST be lower than the number of columns of the board"));
            }

#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)challenger_node->name   << "\" -> \""
                      << (const char*)child_node->name << "\" -> ("
                      << takenCoord.m_row << ", "
                      << takenCoord.m_col << ")"
                      << std::endl;

            if (m_challengerTakenSquares.isPresent(takenCoord))
            {
                std::cerr << "Coordinate ("
                          << takenCoord.m_row << ", "
                          << takenCoord.m_col
                          << ") has already been set as taken in "
                          << a_xmlFile
                          << std::endl;
            }
#endif // DEBUG_PRINT

            // save taken coordinate into the opponentTakenSquares set
            m_challengerTakenSquares.insert(takenCoord);

        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "taken") == 0)

        /////////////
        // piece tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "piece") == 0) )
        {
            // check "available" property exists. It MUST be present
            strValue = xmlGetProp(child_node, (const xmlChar*)"available");
            if (strValue == NULL)
            {
                // property not present
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("\"challenger\" -> \"piece\" mandatory attribute \"available\" is missing"));
            }

            bool currentPieceAvailable = false;
            std::string tmpStr((const char*)strValue);
            xmlFree(strValue);
            if (tmpStr == "yes")
            {
                currentPieceAvailable = true;
            }
            else if (tmpStr == "no")
            {
                currentPieceAvailable = false;
            }
            else
            {
                // "active" property is neither "yes" nor "no". This is an error
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("\"challenger\" -> \"piece\" mandatory attribute \"available\" must be set to either \"yes\" or \"no\""));
            }

            // look for the piece which description matches one of the
            // descriptions in piece.cpp
            strValue = xmlNodeGetContent(child_node);
            std::string pieceNameXML((const char*)strValue);
            xmlFree(strValue);
            int32_t pieceIndex = 0;
            for (pieceIndex = e_minimumPieceIndex;
                 pieceIndex < e_numberOfPieces;
                 pieceIndex++)
            {
                const char* pieceName =
                    Piece::GetPieceDescription(static_cast<ePieceType_t>(pieceIndex));

                if (strcmp(pieceName, pieceNameXML.c_str()) == 0)
                {
                    if (currentPieceAvailable)
                    {
                        m_challengerPiecesPresent[pieceIndex] = true;
                    }
                    else
                    {
                        m_challengerPiecesPresent[pieceIndex] = false;
                    }

                    break;
                }
            }

#ifdef DEBUG_PRINT
            if (pieceIndex == e_numberOfPieces)
            {
                std::cerr << a_xmlFile << ": Invalid piece name in challenger: \""
                          << pieceNameXML << "\""
                          << std::endl;
            }
#endif
        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "piece") == 0)

        ////////////////////
        // starting_row tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "starting_row") == 0) )
        {
            // xml element starting_row
            strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)challenger_node->name   << "\" -> \""
                      << (const char*)child_node->name << "\" -> \""
                      << (const char*)strValue         << "\""
                      << std::endl;
#endif
            std::istringstream iStrStream(std::string((const char*)strValue));
            xmlFree(strValue);

            int32_t startingRow;
            if (!(iStrStream >> startingRow))
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"starting_row\""));
            }
            if (startingRow < 0)
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"starting_row\". MUST be >= 0"));
            }
            else if (startingRow >= GetBoardRows())
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"starting_row\". MUST be lower than the number of rows of the board"));
            }

            startingCord.m_row = startingRow;
        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "starting_row") == 0)

        ////////////////////
        // starting_col tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "starting_col") == 0) )
        {
            // xml element starting_col
            strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)challenger_node->name   << "\" -> \""
                      << (const char*)child_node->name << "\" -> \""
                      << (const char*)strValue         << "\""
                      << std::endl;
#endif
            std::istringstream iStrStream(std::string((const char*)strValue));
            xmlFree(strValue);

            int32_t startingCol;
            if (!(iStrStream >> startingCol))
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"starting_col\""));
            }
            if (startingCol < 0)
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"starting_col\". MUST be >= 0"));
            }
            else if (startingCol >= GetBoardColumns())
            {
                XMLParsingFatalError(a_xmlFile,
                    // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                    // i18n reference to xml tags
                    _("Bad integer value in challenger's \"starting_col\". MUST be lower than the number of columns of the board"));
            }

            startingCord.m_col = startingCol;
        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "starting_col") == 0)

    } // for (child_node = challenger_node->children

    // is starting coordinate set in the XML?
    if (startingCord.Initialised() == true)
    {
        SetChallengerStartingCoord(startingCord);
    }
    else if (startingCord.m_row != COORD_UNINITIALISED)
    {
        // if row is set, column should have been set too
        XMLParsingFatalError(a_xmlFile,
            // i18n TRANSLATORS: Please, leave quoted strings as they are since they
            // i18n reference to xml tags
            _("\"starting_row\" is present in \"challenger\", but \"starting_col\" is not"));
    }
    else if (startingCord.m_col != COORD_UNINITIALISED)
    {
        // if column is set, row should have been set too
        XMLParsingFatalError(a_xmlFile,
            // i18n TRANSLATORS: Please, leave quoted strings as they are since they
            // i18n reference to xml tags
            _("\"starting_col\" is present in \"challenger\", but \"starting_row\" is not"));
    }
    else
    {
        // Neither starting_row nor starting_col are present in challenger
        if (m_challengerTakenSquares.empty())
        {
            // starting coordiante hasn't been set and there's no taken squares
            // defined for the challenger either. Player wouldn't be able to
            // start in a challenge like this. This is an error!
            XMLParsingFatalError(a_xmlFile,
                // i18n TRANSLATORS: Please, leave quoted strings as they are since they
                // i18n reference to xml tags
                _("Starting coordinate hasn't been set and there aren't any taken squares by the challenger either"));
        }

        // we are happy with this. Challenger will start the challenge from one
        // of the nuclation points defined by its taken squares. Up to the
        // challenge designer to ensure at least one of them is valid
        SetChallengerStartingCoord(Coordinate());
    }
}

void BlockemChallenge::XMLParseTagInfochallenge(
    const std::string &a_xmlFile,
    xmlNode* infochallenge_node) throw (std::runtime_error)
{
    xmlNode* child_node = NULL;
    xmlChar* strValue   = NULL;

#ifdef DEBUG
    assert(infochallenge_node->type == XML_ELEMENT_NODE);
    assert(xmlStrcmp(infochallenge_node->name, (const xmlChar*) "infochallenge") == 0);
#endif

    // infochallenge has no attributes. All elements are OPTIONAL:
    //     <author>, <email> and <description>
    for (child_node = infochallenge_node->children;
         child_node != NULL;
         child_node = child_node->next)
    {
        //////////////
        // author tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "author") == 0) )
        {
            strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)infochallenge_node->name << "\" -> \""
                      << (const char*)child_node->name         << "\" -> \""
                      << (const char*)strValue                 << "\""
                      << std::endl;
#endif
            m_challengeInfo.authorName.assign((const char*)strValue);
            xmlFree(strValue);
        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "author") == 0)

        //////////////
        // email tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "email") == 0) )
        {
            strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)infochallenge_node->name << "\" -> \""
                      << (const char*)child_node->name         << "\" -> \""
                      << (const char*)strValue                 << "\""
                      << std::endl;
#endif
            m_challengeInfo.authorEmail.assign((const char*)strValue);
            xmlFree(strValue);
        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "email") == 0)

        //////////////
        // email tag
        if ( (child_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(child_node->name, (const xmlChar*) "description") == 0) )
        {
            strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)infochallenge_node->name << "\" -> \""
                      << (const char*)child_node->name         << "\" -> \""
                      << (const char*)strValue                 << "\""
                      << std::endl;
#endif
            m_challengeInfo.description.assign((const char*)strValue);
            xmlFree(strValue);
        } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "description") == 0)

    } // for (child_node = cur_node->children
}

void BlockemChallenge::XMLParsingFatalError(
    const std::string &a_xmlFile,
    const std::string &a_errorMsg) throw (std::runtime_error)
{
    if (m_xmlDoc)
    {
        // free the document
        xmlFreeDoc(m_xmlDoc);

        // set it back to NULL
        m_xmlDoc = NULL;
    }

    // Free the global variables that may
    // have been allocated by the parser
    xmlCleanupParser();

    // throw the exception!
    char errorStringBuffer[ERROR_STRING_BUFFER_SIZE];
    snprintf(errorStringBuffer,
            ERROR_STRING_BUFFER_SIZE,
            // i18n TRANSLATORS: Please, leave those %s
            // i18n 1st %s will be replaced by filename (without path) that could not be parsed
            // i18n 2nd %s will be replaced by the error message
            // i18n Thank you for contributing to this project
            _("Fatal error parsing %s: %s"),
            Glib::path_get_basename(a_xmlFile).c_str(), // http://library.gnome.org/devel/glibmm/2.23/group__MiscUtils.html
            a_errorMsg.c_str());

    throw std::runtime_error(errorStringBuffer);
}

void BlockemChallenge::SetChallengeName(const std::string &a_name)
{
    m_challengeName = a_name;
}

void BlockemChallenge::SetBoardRows(int32_t a_nRows)
{
    m_nRows = a_nRows;
}

void BlockemChallenge::SetBoardColumns(int32_t a_nColumns)
{
    m_nColumns = a_nColumns;
}

void BlockemChallenge::SetChallengerStartingCoord(const Coordinate &a_coord)
{
    m_challengerStartingCoord = a_coord;
}
