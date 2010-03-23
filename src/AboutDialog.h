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
/// @file  AboutDialog.h
/// @brief
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 26-Nov-2009  Original development
/// @endhistory
///
// ============================================================================

#ifndef ABOUTDIALOG_H_
#define ABOUTDIALOG_H_

#include <libglademm/xml.h>
#include <gtkmm.h>

#include "gui_glade.h"
#include "GUIException.h"

class AboutDialog
{
public:
    AboutDialog(Glib::RefPtr<Gnome::Glade::Xml> a_refXml)
        throw (GUIException);
    virtual ~AboutDialog();

    inline Gtk::AboutDialog& dialog()
    {
        return *m_theAboutDialog;
    }

private:
    /// @brief the about dialog
    Gtk::AboutDialog* m_theAboutDialog;

    /// @brief used to retrieve the objects from the Glade design
    Glib::RefPtr<Gnome::Glade::Xml> m_refXml;

    // prevent the default constructors to be used
    AboutDialog();
    AboutDialog(const AboutDialog &a_src);
    AboutDialog& operator=(const AboutDialog &a_src);
};

#endif /* ABOUTDIALOG_H_ */
