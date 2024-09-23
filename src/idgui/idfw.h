//    File: idfw.h
//    Date: 2024-09-09
//  Author: oknauta
// License: GNU General Public License v2.0

#ifndef IDFM_H
#define IDFM_H

#define NO_ERROR 0
#define GENERIC_ERROR 1
#define SHOW_WINDOW 0
#define HIDE_WINDOW 1
#define UNDEFINED_POSITION -1

/// @brief Window object.
typedef struct _window window;

/// @brief Creates a window with IDGUI.
/// @param WINDOW_TITLE What will be showed up on the window.
/// @param WINDOW_WIDTH That will be width size.
/// @param WINDOW_HEIGHT That will be height size.
/// @param WINDOW_X The window position X.
/// @param WINDOW_Y The window position Y.
/// @return A created window.
window *createWindow(char *WINDOW_TITLE, const unsigned int WINDOW_WIDTH, const unsigned int WINDOW_HEIGHT, const int WINDOW_X, int WINDOW_Y);

/// @brief Close a window.
/// @param window Put the window that will close. 
/// @return A closed window.
int windowShouldClose(window *window);

// void pollEvents(window *window);

/// @brief Destroys a window with IDGUI.
/// @param window Needed window for show or hide it.
/// @param show_window Needed parameter for show/hide it. 0/1 for show/hide.
/// @return A showed/hided window.
int showWindow(window *window, int show_window);

/// @brief Destroys a window.
/// @param window Needed window to be destroyed.
/// @return A destroyed window.
int destroyWindow(window *window);

#endif // IDFM_H
