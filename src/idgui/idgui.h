//    File: idgui.h
//    Date: 2024-09-09
//  Author: oknauta
// License: GNU General Public License v2.0

#ifndef IDGUI
#define IDGUI

#define NO_ERROR 0
#define GENERIC_ERROR 1

/// @brief Window object.
typedef struct _ID_window *ID_window;

/// @brief Creating a window with IDGUI.
/// @param WINDOW_TITLE What will be showed up on the window.
/// @param WINDOW_WIDTH That will be width size.
/// @param WINDOW_HEIGHT That will be height size.
/// @return A created window.
ID_window ID_createWindow(const char *WINDOW_TITLE, const int WINDOW_WIDTH, const int WINDOW_HEIGHT);

/// @brief Destroying a window with IDGUI.
/// @param window Needed window for show or hide it.
/// @param show_window Needed parameter for show/hide it. 0/1 for show/hide.
/// @return A showed/hided window.
int ID_showWindow(ID_window window, int show_window);

int ID_destroyWindow(ID_window window);

#endif // IDGUI