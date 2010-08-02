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
/// @file GUIException.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 02-Aug-2010  Original development
/// @endhistory
///
// ============================================================================

#include <cstdio>    // snprintf
#include "gettext.h" // i18n
#include "gui_exception.h"

// this set of strings describes each one of the possible GUI expcetion types
// included in the enum eGUIExceptionType_t (see gui_Exception.h)
// each string here applies to the corresponding enum value
const char* GUIException::m_exceptionDescription[] =
{
    // i18n Please, leave GtkBuilder as it is here since it is the name of the
    // i18n programming toold that failed
    // i18n Thank you for contributing to this project
    N_("Error loading a GUI object from GtkBuilder definitions"),
    N_("Error creating processing thread")
};

GUIException::GUIException(
    eGUIExceptionType_t a_exceptionType,
    const char*         a_fileName,
    int32_t             a_line) :
        std::runtime_error(_("GUI Exception happened"))
{
    // string describing this exception must be created here
    if (a_exceptionType < e_GUIException_MaxExceptionCount)
    {
        snprintf(m_theMessage,
                 GUIEXCEPTION_BUFFER_SIZE,
                 "%s:%d %s: %s",
                 a_fileName,
                 a_line,
                 std::runtime_error::what(),
                 _(GUIException::m_exceptionDescription[a_exceptionType]));
    }
    else
    {
        snprintf(m_theMessage,
                 GUIEXCEPTION_BUFFER_SIZE,
                 "%s:%d %s",
                 a_fileName,
                 a_line,
                 std::runtime_error::what());
    }
}

GUIException::~GUIException() throw()
{
}

const char* GUIException::what() const throw()
{
    // a c-like string must be returned
    return m_theMessage;
}
