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
/// @file  dialog_newgame_widget_challenge.cpp
/// @brief NewGameTableChallenge methods
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 22-Nov-2010  Original development. Moved from gui_dialog_newgame.cpp
///           Faustino Frechilla 23-Nov-2010  Moved into gui/ directory
/// @endhistory
///
// ============================================================================

#if (defined DEBUG_PRINT) || (defined DEBUG)
#include <iostream>
#endif

#include "gui/dialog_newgame_widget_challenge.h"
#include "gettext.h" // i18n
#include "game_challenge.h"
#include "gui/game_challenge_config.h"
#include "gui/glade_defs.h"


NewGameTableChallenge::NewGameTableChallenge(
    BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& a_gtkBuilder)  throw (GUIException) :
        NewGameTable(cobject), //Calls the base class constructor
        m_gtkBuilder(a_gtkBuilder),
        m_currentSelectedChallenge()
{
    // retrieve widgets from the .glade file
    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_RBUTTON_LIST, m_radioButtonList);
    if (m_radioButtonList == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_RBUTTON_FILE, m_radioButtonFileChooser);
    if (m_radioButtonFileChooser == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_FILECHOOSER, m_buttonChallengeFileChooser);
    if (m_buttonChallengeFileChooser == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_ENTRY_NROWS, m_nRowsEntry);
    if (m_nRowsEntry == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_ENTRY_NCOLS, m_nColsEntry);
    if (m_nColsEntry == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_ENTRY_TAKEN, m_nTakenSquaresEntry);
    if (m_nTakenSquaresEntry == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_ENTRY_TAKENCHALLENGER, m_nTakenSquaresChallengerEntry);
    if (m_nTakenSquaresChallengerEntry == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_ENTRY_AUTHOR, m_authorEntry);
    if (m_authorEntry == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_ENTRY_EMAIL, m_emailEntry);
    if (m_emailEntry == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_TEXTVIEW_DESCRIPTION, m_descriptionTextView);
    if (m_descriptionTextView == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_LABEL_NROWS, m_nRowsLabel);
    if (m_nRowsLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_LABEL_NCOLS, m_nColsLabel);
    if (m_nColsLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_LABEL_TAKEN, m_nTakenSquaresLabel);
    if (m_nTakenSquaresLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_LABEL_TAKENCHALLENGER, m_nTakenSquaresChallengerLabel);
    if (m_nTakenSquaresChallengerLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_LABEL_AUTHOR, m_infoAuthorLabel);
    if (m_infoAuthorLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_LABEL_EMAIL, m_infoEmailLabel);
    if (m_infoEmailLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_gtkBuilder->get_widget(GUI_NEWGAME_CHALLENGE_LABEL_DESCRIPTION, m_infoDescriptionLabel);
    if (m_infoDescriptionLabel == NULL)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    m_treeViewListOfChallenges = Glib::RefPtr<Gtk::IconView>::cast_dynamic(
            m_gtkBuilder->get_object(GUI_NEWGAME_CHALLENGE_TREEVIEW_FILELIST));
    if (!m_treeViewListOfChallenges)
    {
        throw new GUIException(e_GUIException_GTKBuilderErr, __FILE__, __LINE__);
    }

    // set up icon view to store the type of games. Based on:
    // http://git.gnome.org/browse/gtkmm-documentation/tree/examples/book/iconview
    m_treeViewListOfChallengesModel = Gtk::ListStore::create(m_modelColumns);
    m_treeViewListOfChallengesModel->set_sort_column(
            m_modelColumns.m_col_challengename, Gtk::SORT_ASCENDING);

    m_treeViewListOfChallenges->set_model(m_treeViewListOfChallengesModel);
    m_treeViewListOfChallenges->set_text_column(m_modelColumns.m_col_challengename);
    m_treeViewListOfChallenges->signal_selection_changed().connect(sigc::mem_fun(*this,
            &NewGameTableChallenge::ChallengeList_on_selection_changed)); // item selected by a single click

    // fill the list with the built-in challenges
    UpdateBuiltInChallengesList();


    // configure file chooser dialog
    Gtk::FileFilter xmlFilter;// = Gtk::FileFilter::create();
    xmlFilter.set_name(_("XML Files"));
    //xmlFilter.add_pattern("*.[Xx][Mm][Ll]");
    xmlFilter.add_mime_type("text/xml");

    Gtk::FileFilter allFilesFilter;// = Gtk::FileFilter::create();
    allFilesFilter.set_name(_("All Files"));
    allFilesFilter.add_pattern("*.*");

    m_buttonChallengeFileChooser->add_filter(allFilesFilter);
    m_buttonChallengeFileChooser->add_filter(xmlFilter);


    // this call will work in different ways depending on the current platform
    ForceTranslationOfWidgets();

    // connect the signals
    m_radioButtonList->signal_toggled().connect(
            sigc::mem_fun(*this, &NewGameTableChallenge::RadioButtonBuiltInList_Toggled));
    m_radioButtonFileChooser->signal_toggled().connect(
            sigc::mem_fun(*this, &NewGameTableChallenge::RadioButtonFileChooser_Toggled));
    m_buttonChallengeFileChooser->signal_file_set().connect(
            sigc::mem_fun(*this, &NewGameTableChallenge::ChallengeFileChooser_on_file_set));
}

NewGameTableChallenge::~NewGameTableChallenge()
{
}

void NewGameTableChallenge::UpdateBuiltInChallengesList()
{
    // go through the PATH_TO_BUILTIN_CHALLENGES directory
    // looking for challenges to be shown on the list
    GDir* builtInChallegePath;
    GError* error = NULL;
    const gchar* fileName;
    BlockemChallenge tmpChallenge;

    builtInChallegePath = g_dir_open (
                                PATH_TO_BUILTIN_CHALLENGES.c_str(),
                                0,
                                &error);

    if (!builtInChallegePath)
    {
#ifdef DEBUG
        std::cout << "Error accessing built-in challenge directory: "
                  << error->message << std::endl;
#endif
        g_error_free(error);
        return;
    }

    fileName = g_dir_read_name(builtInChallegePath);
    while (fileName != NULL)
    {
        std::string fullFileName(
            PATH_TO_BUILTIN_CHALLENGES + static_cast<const char*>(fileName));

        // retrieve last 4 characters of filename. They must be equal to ".xml"
        std::string extension(std::string(fileName).substr(strlen(fileName) - 4));
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        if (extension != ".xml")
        {
#ifdef DEBUG
            std::cout << "File "
                      << fileName
                      << " (extension "
                      << extension
                      << ") was found in challenges directory and extension is not .xml"  << std::endl;
#endif
            // try out next file
            fileName = g_dir_read_name(builtInChallegePath);
            continue;
        }

        if (!g_file_test(fullFileName.c_str(), G_FILE_TEST_IS_REGULAR))
        {
#ifdef DEBUG
            std::cout << "Loading challenges from built-in challenge dir"
                      << std::endl
                      << "    "
                      << fullFileName << " is not a regular file"
                      << std::endl;
#endif
            // try out next file
            fileName = g_dir_read_name(builtInChallegePath);
            continue;
        }

        tmpChallenge.Reset();
        try
        {
            tmpChallenge.LoadXMLChallenge(fullFileName);
        }
        catch (const std::runtime_error &ex)
        {
            // challenge could not be loaded
#ifdef DEBUG
            std::cout << "Challenge could not be loaded from built-in challenge dir"
                      << std::endl << "    " << ex.what() << std::endl;
#endif
            fileName = g_dir_read_name(builtInChallegePath);
            continue;
        }

        // tmpChallenge must contain a valid challenge
        // is a challenge with this name already in the list?
        std::set<Glib::ustring>::const_iterator it;
        it = m_treeViewListOfChallengesNamesSet.find(tmpChallenge.GetChallengeName());
        if (it == m_treeViewListOfChallengesNamesSet.end())
        {
            // Brand new challenge name. Insert it into the list
            Gtk::TreeModel::Row row = *(m_treeViewListOfChallengesModel->append());
            row[m_modelColumns.m_blockem_challenge] = tmpChallenge;
            row[m_modelColumns.m_col_challengename] = tmpChallenge.GetChallengeName();

            // update also the challenge's names set
            m_treeViewListOfChallengesNamesSet.insert(tmpChallenge.GetChallengeName());
        }
#ifdef DEBUG
        else
        {
            std::cout << "Duplicated challenge name \""
                      << tmpChallenge.GetChallengeName()
                      << "\" at "
                      << static_cast<const char*>(fileName)
                      << std::endl;
        }
#endif

        // keep going. we must check all files in that directory
        fileName = g_dir_read_name(builtInChallegePath);

    } // while (fileName != NULL)

    g_dir_close(builtInChallegePath);
}

void NewGameTableChallenge::ChallengeFileChooser_on_file_set()
{
    if (m_buttonChallengeFileChooser->get_filename().empty())
    {
        // ensure info widgets get blanked out before returning
        m_currentSelectedChallenge.Reset();
        CurrentChallengeToWidgets();

        return;
    }

    std::string exceptionMessage;
    try
    {
        m_currentSelectedChallenge.LoadXMLChallenge(
                m_buttonChallengeFileChooser->get_filename());
    }
    catch (const std::runtime_error &ex)
    {
        // could not load challenge from file
        m_currentSelectedChallenge.Reset();

        // unselect file being showed on the file chooser
        m_buttonChallengeFileChooser->unselect_filename(
                m_buttonChallengeFileChooser->get_filename());

        exceptionMessage = ex.what();

#ifdef DEBUG_PRINT
        std::cout << "Exception happened trying to load challenge file: " << std::endl
                  << "    " << ex.what() << std::endl;
#endif
    } // catch

    if (!m_currentSelectedChallenge.Initialised())
    {
        // tell the user something went wrong (outside the catch statement)
        Gtk::Window* topLevelWindow = NULL;
        topLevelWindow = static_cast<Gtk::Window*>(this->get_toplevel());

        if (topLevelWindow)
        {
            Gtk::MessageDialog errorMsg(
                    *topLevelWindow,
                    exceptionMessage,
                    true,
                    Gtk::MESSAGE_ERROR,
                    Gtk::BUTTONS_OK,
                    true);

            if (errorMsg.run())
            {
                ; // the dialog has only 1 button
            }
        }
#ifdef DEBUG
        else
        {
            // could not show error message. assert here
            assert(0);
        }
#endif
    } // if (!m_currentSelectedChallenge.Initialised())
#ifdef DEBUG_PRINT
    else
    {
        std::cout << "Challenge successfully loaded" << std::endl;
    }
#endif

    CurrentChallengeToWidgets();
}

void NewGameTableChallenge::ChallengeList_on_selection_changed()
{
    // http://git.gnome.org/browse/gtkmm-documentation/tree/examples/book/iconview
    typedef std::list<Gtk::TreeModel::Path> type_list_paths;
    type_list_paths selected = m_treeViewListOfChallenges->get_selected_items();
    if(!selected.empty())
    {
        // save the last path picked so whenever the user clicked somewhere
        // where no rows get selected it can be used to restore the latest
        // selection
        m_currentSelectedPath = *selected.begin();
    }
    else if (m_currentSelectedPath)
    {
        // there must be a row ALWAYS selected. The user picked somewhere were no
        // rows got selected. Select back the last one picked by him/her
        m_treeViewListOfChallenges->select_path(m_currentSelectedPath);
    }

    if (m_currentSelectedPath)
    {
        Gtk::TreeModel::iterator iter = m_treeViewListOfChallengesModel->get_iter(m_currentSelectedPath);
        Gtk::TreeModel::Row row = *iter;
        m_currentSelectedChallenge = row[m_modelColumns.m_blockem_challenge];
    }
    else
    {
        // no selected challenge since there i no row selected on the tree view
        m_currentSelectedChallenge.Reset();
    }

    CurrentChallengeToWidgets();
}

void NewGameTableChallenge::RadioButtonBuiltInList_Toggled()
{
    m_treeViewListOfChallenges->set_sensitive(
            m_radioButtonList->property_active());

    if (m_radioButtonList->property_active())
    {
        if (m_currentSelectedPath)
        {
            Gtk::TreeModel::iterator iter = m_treeViewListOfChallengesModel->get_iter(m_currentSelectedPath);
            Gtk::TreeModel::Row row = *iter;
            m_currentSelectedChallenge = row[m_modelColumns.m_blockem_challenge];
        }
        else
        {
            // no selected challenge since there i no row selected on the tree view
            m_currentSelectedChallenge.Reset();
        }

        CurrentChallengeToWidgets();
    }
}

void NewGameTableChallenge::RadioButtonFileChooser_Toggled()
{
    m_buttonChallengeFileChooser->set_sensitive(
            m_radioButtonFileChooser->property_active());

    if (m_radioButtonFileChooser->property_active())
    {
        // check if current selected file in the file chooser button is a valid
        // challenge. If so save it into the m_currentSelectedChallenge attribute
        ChallengeFileChooser_on_file_set(); // CurrentChallengeToWidgets is called from that function
    }
}

void NewGameTableChallenge::CurrentChallengeToWidgets()
{
    if (m_currentSelectedChallenge.Initialised())
    {
        std::ostringstream outputStr;

        outputStr << static_cast<int32_t>(m_currentSelectedChallenge.GetBoardRows());
        m_nRowsEntry->set_text(outputStr.str());

        outputStr.clear(); // clear error flags. MUST be done first
        outputStr.str("");
        outputStr << static_cast<int32_t>(m_currentSelectedChallenge.GetBoardColumns());
        m_nColsEntry->set_text(outputStr.str());

        outputStr.clear(); // clear error flags. MUST be done first
        outputStr.str("");
        outputStr << static_cast<int32_t>(
                         m_currentSelectedChallenge.GetChallengerTakenSquares().size() +
                         m_currentSelectedChallenge.GetOpponentTakenSquares().size());
        m_nTakenSquaresEntry->set_text(outputStr.str());

        outputStr.clear(); // clear error flags. MUST be done first
        outputStr.str("");
        outputStr << static_cast<int32_t>(m_currentSelectedChallenge.GetChallengerTakenSquares().size());
        m_nTakenSquaresChallengerEntry->set_text(outputStr.str());

        m_authorEntry->set_text(
            m_currentSelectedChallenge.GetChallengeInfo().authorName);
        m_emailEntry->set_text(
            m_currentSelectedChallenge.GetChallengeInfo().authorEmail);
        m_descriptionTextView->get_buffer()->set_text(
            m_currentSelectedChallenge.GetChallengeInfo().description);
    }
    else
    {
        m_nRowsEntry->set_text("");
        m_nColsEntry->set_text("");
        m_nTakenSquaresEntry->set_text("");
        m_nTakenSquaresChallengerEntry->set_text("");
        m_authorEntry->set_text("");
        m_emailEntry->set_text("");
        m_descriptionTextView->get_buffer()->set_text("");
    }
}

void NewGameTableChallenge::SaveCurrentConfigIntoGlobalSettings() const
{
    // retrieve user settings from dialog and use them to set up global configuration
    GameChallengeConfig::Instance().SetBlockemChallenge(m_currentSelectedChallenge);
}

void NewGameTableChallenge::LoadCurrentConfigFromGlobalSettings()
{
    // load current global configuration into the widgets
}

const BlockemChallenge& NewGameTableChallenge::GetCurrentBlockemChallenge() const
{
    return m_currentSelectedChallenge;
}

#ifdef WIN32
void NewGameTableChallenge::ForceTranslationOfWidgets()
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

    m_radioButtonList->set_label(
            _(m_radioButtonList->get_label().c_str()));

    m_radioButtonFileChooser->set_label(
            _(m_radioButtonFileChooser->get_label().c_str()));

    m_nRowsLabel->set_text(_(m_nRowsLabel->get_text().c_str()));
    m_nColsLabel->set_text(_(m_nColsLabel->get_text().c_str()));
    m_nTakenSquaresLabel->set_text(_(m_nTakenSquaresLabel->get_text().c_str()));
    m_nTakenSquaresChallengerLabel->set_text(_(m_nTakenSquaresChallengerLabel->get_text().c_str()));
    m_infoAuthorLabel->set_text(_(m_infoAuthorLabel->get_text().c_str()));
    m_infoEmailLabel->set_text(_(m_infoEmailLabel->get_text().c_str()));
    m_infoDescriptionLabel->set_text(_(m_infoDescriptionLabel->get_text().c_str()));
}
#else
void NewGameTableChallenge::ForceTranslationOfWidgets()
{
    // So far this is only needed in win32 platform due to some unknown issue
    // that prevents those strings to be automatically translated. It works
    // fine in linux, so there's no need there to explicitly call to gettext
}
#endif // WIN32
