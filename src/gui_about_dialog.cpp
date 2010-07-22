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
///           Faustino Frechilla 25-Apr-2010  libglade dependency removed. Code migrated to GtkBuilder
///           Faustino Frechilla 21-Jul-2010  i18n
/// @endhistory
///
// ============================================================================
#ifdef DEBUG_PRINT
#include <iostream> // std::cerr
#endif

#include <glib/gi18n.h> // i18n
#include "config.h" // PACKAGE_VERSION
#include "gui_about_dialog.h"
#include "gui_glade_defs.h"

AboutDialog::AboutDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder) :
    Gtk::AboutDialog(cobject), //Calls the base class constructor
    m_gtkBuilder(a_gtkBuilder)
{
    Glib::RefPtr< Gdk::Pixbuf > picture;

    // set the current version on the about dialog
    this->set_version(PACKAGE_VERSION);

    // try to set the big logo in the AboutDialog
    if (g_file_test(GUI_PATH_TO_LOGO_STR.c_str(), G_FILE_TEST_IS_REGULAR))
    {
        try
        {
            picture = Gdk::Pixbuf::create_from_file(GUI_PATH_TO_LOGO_STR.c_str());
        }
        catch(...)
        {
            picture.reset();
#ifdef DEBUG_PRINT
            std::cerr
               // i18n Bear in mind this string will be printed followed by the path to logo file
               // i18n The extra space at the end is necessary
               // i18n Thank you for contributing to this project
               << _("WARNING: Exception occurred when setting the logo into the AboutDialog from ")
               << GUI_PATH_TO_LOGO_STR
               << std::endl;
#endif
        }

        if (picture)
        {
            this->set_logo(picture);
        }
    }

    // try to set up the icon in the AboutDialog
    if (g_file_test(GUI_PATH_TO_16PICTURE_STR.c_str(), G_FILE_TEST_IS_REGULAR))
    {
        try
        {
            picture = Gdk::Pixbuf::create_from_file(GUI_PATH_TO_16PICTURE_STR.c_str());
        }
        catch(...)
        {
            picture.reset();
#ifdef DEBUG_PRINT
            std::cerr
               // i18n Bear in mind this string will be printed followed by the path to 16x16 icon file
               // i18n The extra space at the end is necessary
               // i18n Thank you for contributing to this project
               << _("WARNING: Exception occurred when setting the 16x16 icon into the AboutDialog from ")
               << GUI_PATH_TO_16PICTURE_STR
               << std::endl;
#endif
        }

        if (picture)
        {
            this->set_icon(picture);
        }
    }
}

AboutDialog::~AboutDialog()
{
}
