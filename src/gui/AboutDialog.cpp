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
// with Foobar. If not, see http://www.gnu.org/licenses/.
//
/// @file  AboutDialog.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 26-Nov-2009  Original development
/// @endhistory
///
// ============================================================================

#include <iostream>
#include "AboutDialog.h"

AboutDialog::AboutDialog(Glib::RefPtr<Gnome::Glade::Xml> a_refXml) throw (GUIException):
    m_refXml(a_refXml)
{
    // retrieve the about dialog from and set it up
    m_refXml->get_widget(GUI_ABOUT_DIALOG_NAME, m_theAboutDialog);
    if (m_theAboutDialog == NULL)
    {
       throw new GUIException(std::string("AboutDialog retrieval failed"));
    }

    // try to set the big logo in the AboutDialog
    Glib::RefPtr< Gdk::Pixbuf > picture;
    try
    {
        picture = Gdk::Pixbuf::create_from_file(GUI_PATH_TO_LOGO);
    }
    catch(...)
    {
        picture.reset();
       std::cerr
           << "WARNING: Exception occurred when setting the logo into the AboutDialog. Using default..."
           << std::endl;
    }
    if (picture)
    {
       m_theAboutDialog->set_logo(picture);
    }

    // try to set up the icon in the AboutDialog
    try
    {
        picture = Gdk::Pixbuf::create_from_file(GUI_PATH_TO_16PICTURE);
    }
    catch(...)
    {
        picture.reset();
       std::cerr
           << "WARNING: Exception occurred when setting the 16x16 icon into the AboutDialog"
           << std::endl;
    }
    if (picture)
    {
       m_theAboutDialog->set_icon(picture);
    }
}

AboutDialog::~AboutDialog()
{
    delete(m_theAboutDialog);
}
