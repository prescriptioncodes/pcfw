//    File: idgui.c
//    Date: 2024-09-09
//  Author: oknauta
// License: GNU General Public License v2.0

#include "idgui.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#include <X11/Xlib.h>
#endif // __linux__

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

struct _ID_window
{
    #ifdef __linux__
    Window   window;
    Display *display;
    XEvent   event;
    int      screen;
    #endif // __linux__
    
    #ifdef _WIN32
    HWND hWnd;
    HINSTANCE hInstance;
    WNDCLASS wc;
    #endif // _WIN32
};

#ifdef _WIN32
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return NO_ERROR;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return NO_ERROR;
        }
        default:
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}
#endif // _WIN32

ID_window ID_createWindow(const char *WINDOW_TITLE, const unsigned int WINDOW_WIDTH, const unsigned int WINDOW_HEIGHT, const int WINDOW_X, int WINDOW_Y)
{
    ID_window window = malloc(sizeof(ID_window));
    
    printf("Window name is %s\n", WINDOW_TITLE);
    printf("Window size is %dx%d\n", WINDOW_WIDTH, WINDOW_HEIGHT);
    
    #ifdef __linux__
    
    window->display = XOpenDisplay(NULL);
    
    if (window->display == NULL)
    {
        printf("Error: Unable to reach XServer.\n");
        return NULL;
    }
    
    window->display = XOpenDisplay(NULL);
    
    
    // Monochrome
    unsigned long black_pixel = BlackPixel(window->display, window->screen);
    unsigned long white_pixel = WhitePixel(window->display, window->screen);
    Window root_window = RootWindow(window->display, window->screen);
    
    // Creating window
    window->window = XCreateSimpleWindow(window->display, root_window, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 1, black_pixel, white_pixel);
    
    // Putting window name
    XStoreName(window->display, window->window, WINDOW_TITLE);
    
    // Show window
    XMapWindow(window->display, window->window);
    
    #endif // __linux__
    
    #ifdef _WIN32
    
    window.hInstance = GetModuleHandle(NULL);
    
    window.wc.lpfnWndProc = WindowProc;
    window.wc.lpszClassName = "ID_WindowClass";
    window.wc.hInstance = window.hInstance;
    window.wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    window.wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 0);
    
    if (!RegisterClass(&window.wc))
    {
        printf("Error while registering window.\n");
        return NULL;
    }
    
    window.hWnd = CreateWindowEx(0, window.wc.lpszClassName, WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, window.hInstance,
        NULL
    );
    
    if (window.hWnd == NULL)
    {
        printf("Error creating window.\n");
        return NULL;
    }
    
    ShowWindow(window.hWnd, SW_SHOW);
    UpdateWindow(window.hWnd);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    #endif // _WIN32
    
    return window;
}

int ID_shouldClose(ID_window window)
{
    if(XPending(window->display) > 0)
    {
        XNextEvent(window->display, &window->event);
    }
    
    return NO_ERROR;
}

int ID_showWindow(ID_window window, int show_window)
{
    #ifdef __linux__
    if(show_window == SHOW_WINDOW)
    {
        XMapWindow(window->display, window->window);
        XFlush(window->display);
    }
    #endif // __linux__

    #ifdef _WIN32
    if(show_window == SHOW_WINDOW)
    {
        ShowWindow(window->hWnd, SW_SHOW);
    }
    else if(show_window == HIDE_WINDOW)
    {
        ShowWindow(window->hWnd, SW_HIDE);
    }
    #endif // _WIN32

    return NO_ERROR;
}

int ID_destroyWindow(ID_window window)
{   
    #ifdef __linux__
    XDestroyWindow(window->display, window->window);
    XCloseDisplay(window->display);
    #endif // __linux__

    #ifdef _WIN32
    if (window->hWnd != NULL)
    {
        DestroyWindow(window->hWnd);
    }
    #endif // _WIN32

    return NO_ERROR;
}

void ID_pollEvents(ID_window window)
{
    if(window->event.type == Expose)
    {
        
    }
}