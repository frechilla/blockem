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
#include <sstream>   // std::istringstream
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
static const Coordinate DEFAULT_OPPONENT_STARTING_COORD = Coordinate(COORD_UNINITIALISED, COORD_UNINITIALISED);
static const Coordinate DEFAULT_CHALLENGER_STARTING_COORD = Coordinate(0, 0);
static const bool       DEFAULT_OPPONENT_PIECES_PRESENT[e_numberOfPieces] =
{
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
};
static const bool       DEFAULT_CHALLENGER_PIECES_PRESENT[e_numberOfPieces] =
{
    false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false
};

static const STLCoordinateSet_t DEFAULT_OPPONENT_TAKEN_SQUARES = STLCoordinateSet_t();
static const STLCoordinateSet_t DEFAULT_CHALLENGER_TAKEN_SQUARES = STLCoordinateSet_t();


BlockemChallenge::BlockemChallenge() :
    m_xmlDoc(NULL),
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

void BlockemChallenge::Reset()
{
    m_xmlDoc = NULL;
    m_challengeName = DEFAULT_CHALLENGE_NAME;
    m_nRows = DEFAULT_NROWS;
    m_nColumns = DEFAULT_NCOLS;
    m_opponentActive = DEFAULT_OPPONENT_ACTIVE;
    m_opponentStartingCoord = DEFAULT_OPPONENT_STARTING_COORD;
    m_challengerStartingCoord = DEFAULT_CHALLENGER_STARTING_COORD;

    m_opponentTakenSquares.clear();
    m_challengerTakenSquares.clear();
    
    for (int32_t i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
    {
        m_opponentPiecesPresent[i] = DEFAULT_OPPONENT_PIECES_PRESENT[i];
        m_challengerPiecesPresent[i] = DEFAULT_CHALLENGER_PIECES_PRESENT[i];
    }
}

void BlockemChallenge::LoadXMLChallenge(const std::string &a_path) throw (std::runtime_error)
{
    // XML parsing based on http://www.yolinux.com/TUTORIALS/GnomeLibXml2.html
    xmlNode* cur_node   = NULL;
    xmlNode* child_node = NULL;
    xmlNode* root       = NULL;
    xmlChar* strValue   = NULL;
    
    // Deletes current challenge stored in this
    Reset();

    // ensure config file exists
    if (!g_file_test(a_path.c_str(), G_FILE_TEST_IS_REGULAR))
    {
        throw new std::runtime_error(
            std::string("Challenge .xml file doesn't exist: ") + a_path);
    }

    // open XML doc and try to parse it
    m_xmlDoc = xmlParseFile(a_path.c_str());
    if (m_xmlDoc == NULL)
    {
        XMLParsingFatalError(a_path,
            std::string("XML syntax error"));
    }

    // retrieve root element
    root = xmlDocGetRootElement(m_xmlDoc);

    // XML file MUST have a root element called blockem_challenge
    if ( (root == NULL)       ||
         (root->name == NULL) ||
         xmlStrcmp(root->name, (const xmlChar*) "blockem_challenge") )
    {
        XMLParsingFatalError(a_path,
            std::string("Root element does not exist or is not called \"blockem_challenge\""));
    }

    strValue = xmlGetProp(root, (const xmlChar*)"name");
    if (strValue == NULL)
    {
        // property not present
        XMLParsingFatalError(a_path,
                std::string("Root element does not have mandatory property \"name\""));
    }
    if (xmlStrcmp(strValue, (const xmlChar*) "") == 0)
    {
        // property not present
        xmlFree(strValue);

        XMLParsingFatalError(a_path,
                std::string("Root element's property \"name\" can't be empty"));
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
            // xml element called "board"
            boardTagPresent = true;
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)cur_node->name << "\"" << std::endl;                      
#endif
            
            // board has no attributes and is made up of two elements: 
            //     <nrows> and <ncolumns>
            // ensure both are present
            bool nrowsPresent = false;
            bool ncolumnsPresent = false;
            for (child_node = cur_node->children; child_node != NULL; child_node = child_node->next)
            {
                if ( (child_node->type == XML_ELEMENT_NODE) &&
                     (xmlStrcmp(child_node->name, (const xmlChar*) "nrows") == 0) )
                {
                    // xml element nrows
                    nrowsPresent = true;
                    strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
                    std::cout << "XML Parsing: \""
                              << (const char*)cur_node->name   << "\" -> \"" 
                              << (const char*)child_node->name << "\" -> \""
                              << (const char*)strValue         << "\""
                              << std::endl;                      
#endif
                    std::istringstream iStrStream(std::string((const char*)strValue));
                    xmlFree(strValue);
                    
                    int32_t nRows;
                    if (!(iStrStream >> nRows))
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"nrows\""));
                    }
                    if (nRows <= 0)
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"nrows\". MUST be a positive number"));
                    }
                    
                    SetNRows(nRows);
                } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "ncolumns") == 0)
                
                if ( (child_node->type == XML_ELEMENT_NODE) &&
                     (xmlStrcmp(child_node->name, (const xmlChar*) "nrows") == 0) )
                {
                    // xml element ncolumns
                    ncolumnsPresent = true;
                    strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
                    std::cout << "XML Parsing: \""
                              << (const char*)cur_node->name   << "\" -> \"" 
                              << (const char*)child_node->name << "\" -> \""
                              << (const char*)strValue         << "\""
                              << std::endl;                      
#endif
                    std::istringstream iStrStream(std::string((const char*)strValue));
                    xmlFree(strValue);
                    
                    int32_t nCols;
                    if (!(iStrStream >> nCols))
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"ncolumns\""));
                    }
                    if (nCols <= 0)
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"ncolumns\". MUST be a positive number"));
                    }
                    
                    SetNColumns(nCols);
                } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "ncolumns") == 0)
            } // for (child_node = cur_node->children
            
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
                
                XMLParsingFatalError(a_path,
                    std::string("Mandatory element(s) in \"board\" missing:") + mandatoryElemMissing);
            } // if ( (!nrowsPresent) || (!ncolumnsPresent) )
            
        } // if ... (xmlStrcmp(cur_node->name, (const xmlChar*) "board") == 0)
        
        /////////////////
        // opponent tag
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "opponent") == 0) )
        {
            // xml element called "opponent"
            opponentTagPresent = true;
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)cur_node->name << "\"" << std::endl;                      
#endif

            // opponent has a mandatory attribute ("active") and is made up of four elements: 
            //     <taken>, <piece>, <starting_row> and <starting_col>
            // <taken> can be present 0 or several times 
            // <piece> can be present up to 21 times. 
            // <starting_row> and <starting_col> can be 0 or 1 time present each

            // check "active" property exists. It MUST be present
            strValue = xmlGetProp(cur_node, (const xmlChar*)"active");
            if (strValue == NULL)
            {
                // property not present
                XMLParsingFatalError(a_path,
                    std::string("\"opponent\" mandatory attribute \"active\" is missing"));
            }

            std::string tmpStr((const char*)strValue);
            xmlFree(strValue);            
            if (tmpStr == "yes")
            {
                SetOpponentActive(true);
            }
            else if (tmpStr == "no")
            {
                SetOpponentActive(false);
            }
            else
            {
                // "active" property is neither "yes" nor "no". This is an error
                XMLParsingFatalError(a_path,
                    std::string("\"opponent\" mandatatory attribute \"active\" must be set to either \"yes\" or \"no\""));
            }
    
            // starting row and starting column are optional values, but if one of them
            // is set the other one MUST be set too
            Coordinate startingCord(COORD_UNINITIALISED, COORD_UNINITIALISED);
            for (child_node = cur_node->children; child_node != NULL; child_node = child_node->next)
            {
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
                        XMLParsingFatalError(a_path,
                            std::string("\"opponent\" -> \"taken\" mandatory attribute \"row\" is missing"));
                    }
                    std::istringstream iStrStream(std::string((const char*)strValue));
                    xmlFree(strValue);
                    
                    if (!(iStrStream >> takenCoord.m_row))
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"row\" attribute (\"taken tag\")"));
                    }
                    if (takenCoord.m_row < 0)
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"row\" attribute (\"taken tag\"). MUST be >= 0"));
                    }
                    else if (takenCoord.m_row > GetBoardRows())
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"row\" attribute (\"taken tag\"). MUST be lower than the number of rows of the board"));
                    }
                    
                    //////////
                    // COLUMN
                    strValue = xmlGetProp(child_node, (const xmlChar*)"col");
                    if (strValue == NULL)
                    {
                        // property not present
                        XMLParsingFatalError(a_path,
                            std::string("\"opponent\" -> \"taken\" mandatory attribute \"col\" is missing"));
                    }
                    iStrStream.str(std::string((const char*)strValue));
                    xmlFree(strValue);
                    
                    if (!(iStrStream >> takenCoord.m_col))
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"col\" attribute (\"taken tag\")"));
                    }
                    if (takenCoord.m_col < 0)
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"col\" attribute (\"taken tag\"). MUST be >= 0"));
                    }
                    else if (takenCoord.m_col > GetBoardColumns())
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"col\" attribute (\"taken tag\"). MUST be lower than the number of columns of the board"));
                    }
                    
                    // save taken coordiante into the opponentTakenSquares set
#if defined(DEBUG_PRINT) || defined(DEBUG)
                    if (m_opponentTakenSquares.isPresent(takenCoord))
                    {
                        std::cerr << "Coordinate ("
                                  << takenCoord.m_row << ", "
                                  << takenCoord.m_col 
                                  << ") has already been set as taken in "
                                  << a_path
                                  << std::endl;
                    }
#endif // defined(DEBUG_PRINT) || defined(DEBUG)
                    m_opponentTakenSquares.insert(takenCoord);
                    
                } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "taken") == 0)
                
                if ( (child_node->type == XML_ELEMENT_NODE) &&
                     (xmlStrcmp(child_node->name, (const xmlChar*) "piece") == 0) )
                {
                    // check "available" property exists. It MUST be present
                    strValue = xmlGetProp(child_node, (const xmlChar*)"available");
                    if (strValue == NULL)
                    {
                        // property not present
                        XMLParsingFatalError(a_path,
                            std::string("\"opponent\" -> \"piece\" mandatory attribute \"available\" is missing"));
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
                        XMLParsingFatalError(a_path,
                            std::string("\"opponent\" -> \"piece\" mandatory attribute \"available\" must be set to either \"yes\" or \"no\""));
                    }
                    
                    // look for the piece which description matches one of the 
                    // descriptions in piece.cpp
                    strValue = xmlNodeGetContent(child_node);
                    std::string pieceNameXML((const char*)strValue);
                    xmlFree(strValue);
                    int32_t i = 0;
                    for (i = e_minimumPieceIndex; i < e_numberOfPieces; i++)
                    {
                        const char* pieceName = Piece::GetPieceDescription(static_cast<ePieceType_t>(i));
                        
                        if (strcmp((const char*) pieceName, pieceNameXML.c_str()) == 0)
                        {
                            if (currentPieceAvailable)
                            {
                                m_opponentPiecesPresent[i] = true;
                            }
                            else
                            {
                                m_opponentPiecesPresent[i] = false;
                            }
                            
                            break;
                        }
                    }
                   
#if defined(DEBUG_PRINT) || defined(DEBUG)
                    if (i == e_numberOfPieces)
                    {
                        std::cerr << a_path << ": Invalid piece name: \""
                                  << pieceNameXML << "\"" 
                                  << std::endl;
                    }
#endif
                    
                } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "piece") == 0)
                
                if ( (child_node->type == XML_ELEMENT_NODE) &&
                     (xmlStrcmp(child_node->name, (const xmlChar*) "starting_row") == 0) )
                {
                    // xml element starting_row
                    strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
                    std::cout << "XML Parsing: \""
                              << (const char*)cur_node->name   << "\" -> \"" 
                              << (const char*)child_node->name << "\" -> \""
                              << (const char*)strValue         << "\""
                              << std::endl;                      
#endif
                    std::istringstream iStrStream(std::string((const char*)strValue));
                    xmlFree(strValue);
                    
                    int32_t startingRow;
                    if (!(iStrStream >> startingRow))
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"starting_row\""));
                    }
                    if (startingRow < 0)
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"starting_row\". MUST be >= 0"));
                    }
                    else if (startingRow > GetBoardRows())
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"starting_row\". MUST be lower than the number of rows of the board"));
                    }
                    
                    startingCord.m_row = startingRow;
                } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "starting_row") == 0)
                
                if ( (child_node->type == XML_ELEMENT_NODE) &&
                     (xmlStrcmp(child_node->name, (const xmlChar*) "starting_col") == 0) )
                {
                    // xml element starting_col
                    strValue = xmlNodeGetContent(child_node);
#ifdef DEBUG_PRINT
                    std::cout << "XML Parsing: \""
                              << (const char*)cur_node->name   << "\" -> \"" 
                              << (const char*)child_node->name << "\" -> \""
                              << (const char*)strValue         << "\""
                              << std::endl;                      
#endif
                    std::istringstream iStrStream(std::string((const char*)strValue));
                    xmlFree(strValue);
                    
                    int32_t startingCol;
                    if (!(iStrStream >> startingCol))
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"starting_col\""));
                    }
                    if (startingCol < 0)
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"starting_col\". MUST be >= 0"));
                    }
                    else if (startingCol > GetBoardColumns())
                    {
                        XMLParsingFatalError(a_path,
                            std::string("Bad integer value in \"starting_col\". MUST be lower than the number of columns of the board"));
                    }
                    
                    startingCord.m_col = startingCol;
                } // if ... (xmlStrcmp(child_node->name, (const xmlChar*) "starting_col") == 0)
                
            } // for (child_node = cur_node->children
            
            // can the starting coordinate be set?
            if (startingCord.Initialised() == true)
            {
                SetOpponentStartingCoord(startingCord);
            }
            else if ( (startingCord.m_row != COORD_UNINITIALISED) ||
                      (startingCord.m_row != COORD_UNINITIALISED) )
            {
                // only one of the coords has been set
                XMLParsingFatalError(a_path,
                    std::string("Both \"starting_row\" and \"starting_col\" MUST be set to configure the starting coordinate"));
            }    
    
        } // if ... (xmlStrcmp(cur_node->name, (const xmlChar*) "opponent") == 0)

        /////////////////
        // challenger tag
        if ( (cur_node->type == XML_ELEMENT_NODE) &&
             (xmlStrcmp(cur_node->name, (const xmlChar*) "challenger") == 0) )
        {
            // xml element called "challenger"
            challengerTagPresent = true;
#ifdef DEBUG_PRINT
            std::cout << "XML Parsing: \""
                      << (const char*)cur_node->name << "\"" << std::endl;                      
#endif
            
            strValue = xmlNodeGetContent(cur_node);
            xmlFree(strValue);
        } // if ... (xmlStrcmp(cur_node->name, (const xmlChar*) "challenger") == 0)
    }
    
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
            std::string("Mandatory element(s) missing:") + mandatoryElemMissing);
    }

    // success parsing the file!
    
    // free the document
    xmlFreeDoc(m_xmlDoc);
    
    // set it back to NULL
    m_xmlDoc = NULL;

    // Free the global variables that may
    // have been allocated by the parser
    xmlCleanupParser();
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
    throw new std::runtime_error(std::string("Fatal error parsing ") + 
        a_xmlFile + std::string(": ") + a_errorMsg);
}
