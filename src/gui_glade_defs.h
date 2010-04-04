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
// compile line is:
//     g++ main.cpp tutgui.o -o main `pkg-config gtkmm-2.4 libglademm-2.4 --cflags --libs`


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
const char GUI_MENU_ITEM_GAME_NEW[]        = "imagemenuitem_game_new";
const char GUI_MENU_ITEM_GAME_QUIT[]       = "imagemenuitem_game_quit";
const char GUI_MENU_ITEM_HELP_ABOUT[]      = "imagemenuitem_help_about";
const char GUI_VBOX_DRAWING_NAME[]         = "vbox_drawing";
const char GUI_HBOX_GAME_STATUS_NAME[]     = "hbox_gamestatus";
const char GUI_DRAWINGAREA_BOARD_NAME[]    = "drawingarea_board";
const char GUI_HBOX_COMPUTER_PIECES_NAME[] = "hbox_computerpieces";
const char GUI_HBOX_PIECES_AREA_NAME[]     = "hbox_editpieces";
const char GUI_TABLE_EDITING_PIECE_NAME[]  = "table_editing_piece";
const char GUI_BUTTON_ROTATE_NAME[]        = "button_rotate";
const char GUI_BUTTON_MIRROR_NAME[]        = "button_mirror";
const char GUI_DRAWINGAREA_EDITING_PIECE[] = "drawingarea_editing_piece";
const char GUI_STATUSBAR_NAME[]            = "statusbar1";
const char GUI_PROGRESSBAR_NAME[]          = "progressbar1";
const char GUI_LABEL_COMPUTERSCORE_NAME[]  = "label_computerscore";
const char GUI_LABEL_USERSCORE_NAME[]      = "label_userscore";

#endif /* GUI_GLADE_H_ */
