// ============================================================================
// Copyright 2009 Faustino Frechilla
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
/// @file GUIException.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 26-Nov-2009  Original development
///           Faustino Frechilla 21-Jul-2010  i18n
/// @endhistory
///
// ============================================================================

#ifndef GUIEXCEPTION_H_
#define GUIEXCEPTION_H_

#include <stdexcept> // std::runtime_error
#include <stdint.h>  // for types

#define GUIEXCEPTION_BUFFER_SIZE 128

/// types of GUI exception. Each one of them is described by a predefined
/// string
typedef enum
{
    e_GUIException_GTKBuilderErr = 0,   // error loading stuff from gtkbuilder
    e_GUIException_ProcessingThreadErr, // error instantiating processing thread

    e_GUIException_MaxExceptionCount,   // maximum index assigned to exceptions
} eGUIExceptionType_t;

/// @brief default exception handling the GUI
class GUIException : public std::runtime_error
{
public:
    /// @param type of exception (see eGUIExceptionType_t)
    /// @param file where the exception happened (use gcc's __FILE__)
    /// @param line in in the file were the exception was raised (use gcc's
    ///        __LINE__)
    GUIException(eGUIExceptionType_t a_exceptionType,
                 const char*         a_fileName,
                 int32_t             a_line);

    virtual ~GUIException() throw();

    /// create and return an internationalised string describing the exception
    /// Format of the string:
    /// file:line string-describing-what-happened
    /// @return an internationalised string describing the exception
    virtual const char* what() const throw();

private:
    /// internal buffer to hold exception description in a string
    char m_theMessage[GUIEXCEPTION_BUFFER_SIZE];

    /// array with the strings that describe each type of exception
    const static char* m_exceptionDescription[];
};

#endif /* GUIEXCEPTION_H_ */
