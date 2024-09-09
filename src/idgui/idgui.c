//    File: idgui.c
//    Date: 2024-09-09
//  Author: oknauta
// License: GNU General Public License v2.0

#include "idgui.h"
#include <stdio.h>

#ifdef __linux__
#include <X11/Xlib.h>
#endif // __linux__

struct _ID_window
{
    #ifdef __linux__
    Window   window;
    Display *display;
    XEvent   event;
    int      screen;
    #endif // __linux__
};


ID_window ID_createWindow(const char *WINDOW_TITLE, const int WINDOW_WIDTH, const int WINDOW_HEIGHT)
{
    static struct _ID_window window;
    
    printf("Window name is %s\n", WINDOW_TITLE);
    printf("Window size is %dx%d\n", WINDOW_WIDTH, WINDOW_HEIGHT);
    
    #ifdef __linux__
    
    window.display = XOpenDisplay(NULL);
    
    if(window.display == NULL)
    {
        printf("Error to reach to XServer.");
    }
    
    window.screen = DefaultScreen(window.display);
    
    window.window = XCreateSimpleWindow(window.display, RootWindow(window.display, window.screen), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1, BlackPixel(window.display, window.screen), WhitePixel(window.display, window.screen));
    
    XStoreName(window.display, window.window, WINDOW_TITLE);
    
    XMapWindow(window.display, window.window);
    
    while (1) {
        XNextEvent(window.display, &window.event);
        
        if (window.event.type == Expose)
        {
            break;
        }
    }
    
    XDestroyWindow(window.display, window.window);
    XCloseDisplay(window.display);
    
    #endif // __linux__
    
    return &window;
}

int ID_showWindow(ID_window window, int show_window)
{
    #ifdef __linux__
    // Your code here
    #endif // __linux__
    return NO_ERROR;
}

int ID_destroyWindow(ID_window window)
{
    #ifdef __linux__
    // Your code
    #endif // __linux__
    return NO_ERROR;
}