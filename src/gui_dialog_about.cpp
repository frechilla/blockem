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
/// @file  gui_dialog_about.cpp
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
#include "config.h"  // PACKAGE_VERSION
#include "gettext.h" // i18n
#include "gui_dialog_about.h"
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
               << "WARNING: Exception occurred when setting the logo into the AboutDialog from "
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
               << "WARNING: Exception occurred when setting the 16x16 icon into the AboutDialog from "
               << GUI_PATH_TO_16PICTURE_STR
               << std::endl;
#endif
        }

        if (picture)
        {
            this->set_icon(picture);
        }
    }

    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();
}

AboutDialog::~AboutDialog()
{
}

#ifdef WIN32
void AboutDialog::ForceTranslationOfWidgets()
{
    // in win32 systems gettext fails when the string is static and marked as
    // translatable with N_() but _() is never called explicitely. Basically
    // there are 2 kinds of strings that are not translated:
    //  + Those included in the GOptionEntry list, which show the available
    //    options that can be passed to the program through command line
    //  + Strings included in the .glade file that never change during the
    //    execution of the application, for example a menu called "Game", or a
    //    label that contains the word "rotate"
    //
    // We'll be calling here to _() for every string found in the .glade file
    // so it gets properly translated into the current domain (the 2nd case
    // described above)

    // for some reason titles do get translated
    //set_title( _(get_title().c_str()) );

    if (get_program_name().size() > 0)
    {
        set_program_name( _(get_program_name().c_str()) );
    }
    if (get_copyright().size() > 0)
    {
        set_copyright( _(get_copyright().c_str()) );
    }
    if (get_comments().size() > 0)
    {
        set_comments( _(get_comments().c_str()) );
    }
    if (get_license().size() > 0)
    {
        set_license( _(get_license().c_str()) );
    }
    if (get_website_label().size() > 0)
    {
        set_website_label( _(get_website_label().c_str()) );
    }
    //set_authors(get_authors().c_str());
    //set_documenters(get_documenters().c_str());
    //set_artists(get_artists().c_str());
    if (get_translator_credits().size() > 0)
    {
        set_translator_credits( _(get_translator_credits().c_str()) );
    }

    // do not set. It can break the icon
    //set_logo_icon_name( _(get_logo_icon_name().c_str()) );
}
#else
void AboutDialog::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
