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

#include <stdexcept>
#include "gettext.h" // i18n

/// @brief default exception handling the GUI
class GUIException : public std::runtime_error
{
public:
    GUIException(const std::string &a_message) :
        std::runtime_error(_("GUI Exception happened")),
        a_msg(a_message)
    {}

    virtual ~GUIException() throw()
    {}

    virtual const char* what() const throw()
    {
        return a_msg.c_str();
    }

private:
    std::string a_msg;
};

#endif /* GUIEXCEPTION_H_ */
