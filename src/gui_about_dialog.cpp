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
/// @file  gui_about_dialog.cpp
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 26-Nov-2009  Original development
/// @endhistory
///
// ============================================================================

#include <iostream> // std::cerr
#include "config.h" // PACKAGE_VERSION
#include "gui_about_dialog.h"
#include "gui_glade_defs.h"

AboutDialog::AboutDialog(Glib::RefPtr<Gnome::Glade::Xml> a_refXml) throw (GUIException):
    m_refXml(a_refXml)
{
    Glib::RefPtr< Gdk::Pixbuf > picture;

    // retrieve the about dialog from and set it up
    m_refXml->get_widget(GUI_ABOUT_DIALOG_NAME, m_theAboutDialog);
    if (m_theAboutDialog == NULL)
    {
       throw new GUIException(std::string("AboutDialog retrieval failed"));
    }

    // set the current version on the about dialog
    m_theAboutDialog->set_version(PACKAGE_VERSION);

    // try to set the big logo in the AboutDialog
    if (g_file_test(GUI_PATH_TO_LOGO, G_FILE_TEST_IS_REGULAR))
    {
        try
        {
            picture = Gdk::Pixbuf::create_from_file(GUI_PATH_TO_LOGO);
        }
        catch(...)
        {
            picture.reset();
            std::cerr
               << "WARNING: Exception occurred when setting the logo into the AboutDialog from "
               << GUI_PATH_TO_LOGO
               << std::endl;
        }

        if (picture)
        {
           m_theAboutDialog->set_logo(picture);
        }
    }

    // try to set up the icon in the AboutDialog
    if (g_file_test(GUI_PATH_TO_16PICTURE, G_FILE_TEST_IS_REGULAR))
    {
        try
        {
            picture = Gdk::Pixbuf::create_from_file(GUI_PATH_TO_16PICTURE);
        }
        catch(...)
        {
            picture.reset();
            std::cerr
               << "WARNING: Exception occurred when setting the 16x16 icon into the AboutDialog from "
               << GUI_PATH_TO_16PICTURE
               << std::endl;
        }

        if (picture)
        {
           m_theAboutDialog->set_icon(picture);
        }
    }
}

AboutDialog::~AboutDialog()
{
    delete(m_theAboutDialog);
}
