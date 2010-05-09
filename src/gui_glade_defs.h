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
/// @file  gui_glade.h
/// @brief
///
/// This file is meant to contain all the definitions needed for embedding the
/// glade file into the application
///
/// @author Faustino Frechilla
/// @history
/// Ref       Who                When         What
///           Faustino Frechilla 25-Nov-2009  Original development
///           Faustino Frechilla 25-Apr-2010  libglade dependency removed. Code migrated to GtkBuilder
/// @endhistory
///
// ============================================================================

// HOWTO make the .glade file a .o file to be included into the binary
// from http://www.procrastinatorstangent.com/?q=node/23
//    //convert file to binary obj file
//    ld -r -b binary -o gui.o gui.glade
//
//    //output obj file to know what the data section is called
//    objdump -x gui.o
//
//    //the ouput should be something like this:
//    Sections:
//    Idx Name          Size      VMA       LMA       File off  Algn
//      0 .rodata       0000227a  00000000  00000000  00000034  2**0
//                      CONTENTS, ALLOC, LOAD, READONLY, DATA
//    SYMBOL TABLE:
//    00000000 l    d  .rodata    00000000 .rodata
//    0000227a g       .rodata    00000000 _binary_gui_glade_end
//    0000227a g       *ABS*      00000000 _binary_gui_glade_size
//    00000000 g       .rodata    00000000 _binary_gui_glade_start
//    //what's important is "_binary_gui_glade_size" and "_binary_gui_glade_start" these are the start
//    //and size symbols that we will use in the cpp folder
//
//    //copy obj to make into a readonly format
//    objcopy --rename-section .data=.rodata,alloc,load,readonly,data,contents gui.o gui.o
//
//    //compile app
//    gcc test.c foo.o -o test
//
// you'll have to use some of the variables of that object file in the main application
// as global variables, like this:
//     extern const char _binary_gui_glade_start[];
//     extern int _binary_gui_glade_size[];
//
// then, to access the xml data from the main function do:
//     Glib::RefPtr<Gnome::Glade::Xml> refXml;
//     refXml = Gnome::Glade::Xml::create_from_buffer(
//                  reinterpret_cast<const char *>(_binary_tutgui_glade_start),
//                  reinterpret_cast<int>(_binary_tutgui_glade_size),
//                  "",
//                  "");
//
// NOTE: If you have decided to use GtkBuilder instead of libglade use this instead:
//      Glib::RefPtr<Gtk::Builder> gtkBuilder = Gtk::Builder::create();
//      gtkBuilder->add_from_string(
//                reinterpret_cast<const char *>(__BIN_GUI_GLADE_START__),
//                reinterpret_cast<long int>(__BIN_GUI_GLADE_SIZE__))
//
//
// compile line is:
//     g++ main.cpp tutgui.o -o main `pkg-config gtkmm-2.4 libglademm-2.4 --cflags --libs`
//
// If not using libglade:
//     g++ main.cpp tutgui.o -o main `pkg-config gtkmm-2.4 --cflags --libs`

#ifndef GUI_GLADE_H_
#define GUI_GLADE_H_

// path to the pictures and logos
#ifdef DATADIR
const char GUI_PATH_TO_LOGO[]       = DATADIR"/pixmaps/blockemlogo.png";
const char GUI_PATH_TO_16PICTURE[]  = DATADIR"/pixmaps/blockemlogo16.png";
const char GUI_PATH_TO_48PICTURE[]  = DATADIR"/pixmaps/blockemlogo48.png";
const char GUI_PATH_TO_128PICTURE[] = DATADIR"/pixmaps/blockemlogo128.png";
#else
const char GUI_PATH_TO_LOGO[]       = "pixmaps/blockemlogo.png";
const char GUI_PATH_TO_16PICTURE[]  = "pixmaps/blockemlogo16.png";
const char GUI_PATH_TO_48PICTURE[]  = "pixmaps/blockemlogo48.png";
const char GUI_PATH_TO_128PICTURE[] = "pixmaps/blockemlogo128.png";
#endif // DATADIR

// have a look at the name of the glade interfaces
const char GUI_MAIN_WINDOW_NAME[]          = "window_main";
const char GUI_ABOUT_DIALOG_NAME[]         = "aboutdialog";
const char GUI_CONFIG_DIALOG_NAME[]        = "configdialog";
const char GUI_MENU_ITEM_GAME_NEW[]        = "imagemenuitem_game_new";
const char GUI_MENU_ITEM_GAME_QUIT[]       = "imagemenuitem_game_quit";
const char GUI_MENU_ITEM_SETTINGS_NKPOINTS[] = "checkmenuitem_settings_nkpoints";
const char GUI_MENU_ITEM_SETTINGS_FORBIDDENAREA[] = "checkmenuitem_settings_forbidden";
const char GUI_MENU_ITEM_SETTINGS_PREFS[]  = "imagemenuitem_settings_preferences";
const char GUI_MENU_ITEM_HELP_ABOUT[]      = "imagemenuitem_help_about";
const char GUI_VBOX_DRAWING_NAME[]         = "vbox_drawing";
const char GUI_HBOX_GAME_STATUS_NAME[]     = "hbox_gamestatus";
const char GUI_HBOX_COMPUTER_PIECES_NAME[] = "hbox_computerpieces";
const char GUI_HBOX_PIECES_AREA_NAME[]     = "hbox_editpieces";
const char GUI_TABLE_EDITING_PIECE_NAME[]  = "table_editing_piece";
const char GUI_LABEL_ROTATE_NAME[]         = "label_rotate";
const char GUI_LABEL_MIRROR_NAME[]         = "label_mirror";
const char GUI_BUTTON_ROTATE_LEFT_NAME[]   = "button_rotate_left";
const char GUI_BUTTON_ROTATE_RIGHT_NAME[]  = "button_rotate_right";
const char GUI_BUTTON_MIRROR_XAXIS_NAME[]  = "button_mirror_xaxis";
const char GUI_BUTTON_MIRROR_YAXIS_NAME[]  = "button_mirror_yaxis";
const char GUI_DRAWINGAREA_EDITING_PIECE[] = "drawingarea_editing_piece";
const char GUI_HBOX_STATUSBAR_NAME[]       = "hbox_statusbar";
const char GUI_CONFIG_TABLE_PLAYER1[]      = "config_table_player1";
const char GUI_CONFIG_TABLE_PLAYER2[]      = "config_table_player2";
const char GUI_CONFIG_SPINBUTTON_STARTROW1[] = "spinbutton_startingrow1";
const char GUI_CONFIG_SPINBUTTON_STARTCOL1[] = "spinbutton_startingcol1";
const char GUI_CONFIG_SPINBUTTON_STARTROW2[] = "spinbutton_startingrow2";
const char GUI_CONFIG_SPINBUTTON_STARTCOL2[] = "spinbutton_startingcol2";
const char GUI_CONFIG_AI_FRAME_PLAYER1[]   = "config_frame_ai_player1";
const char GUI_CONFIG_AI_FRAME_PLAYER2[]   = "config_frame_ai_player2";
const char GUI_CONFIG_AI_TABLE_PLAYER1[]   = "config_table_ai_player1";
const char GUI_CONFIG_AI_TABLE_PLAYER2[]   = "config_table_ai_player2";
const char GUI_CONFIG_AI_SPINBUTTON_DEPTH1[] = "spinbutton_depth1";
const char GUI_CONFIG_AI_SPINBUTTON_DEPTH2[] = "spinbutton_depth2";
const char GUI_CONFIG_AI_TEXTVIEW_HEURISTIC1[] = "textview_heuristic_player1";
const char GUI_CONFIG_AI_TEXTVIEW_HEURISTIC2[] = "textview_heuristic_player2";

#endif /* GUI_GLADE_H_ */
