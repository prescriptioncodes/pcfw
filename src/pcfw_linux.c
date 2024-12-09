// Author: oknauta
// License: Unknown
// File: linux.c
// Date: 2024-11-18

#include "pcfw.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

struct PC_window
{
    int _position_x, _position_y;                         // Position in vec2
    int _width, _height;                                  // Size
    const char *_title;                                   // Title
    int _screen;                                          // Screen
    int _loop;                                            // If the window is looping
    Display *_display;                                    // Display
    Window _window;                                       // Window
    Colormap _colormap;                                   // Colormap
    XEvent _event;                                        // Events
    Visual *_visual;                                      // Visual
    XVisualInfo *_visual_info;                            // Visual info
    GLXContext _context;                                  // Context
    XSetWindowAttributes _attributes;                     // Attributes
    Atom _wm_delete_window;                               // Deleting the window when pressing the X button
    framebuffer_size_callback _framebuffer_size_callback; // Callbacks the framebuffer size
};

void PC_setSwapInterval(PC_window *window, int interval)
{
    static PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = NULL;

    if (!glXSwapIntervalEXT)
    {
        glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalEXT");
    }

    if (glXSwapIntervalEXT)
    {
        glXSwapIntervalEXT(window->_display, window->_window, interval);
    }
}

const char *PC_getWindowTitle(PC_window *window)
{
    if (window->_title)
        return window->_title;
    else
        return NULL;
}

void PC_setFramebufferSizeCallback(PC_window *window, framebuffer_size_callback callback)
{
    if (window)
        window->_framebuffer_size_callback = callback;
}

int PC_windowShouldClose(PC_window *window)
{
    if (window->_loop)
        return window->_loop;
    else
        return NULL;
}

void PC_pollEvents(PC_window *window)
{
    if (XPending(window->_display) > 0)
    {
        XNextEvent(window->_display, &window->_event);

        switch (window->_event.type)
        {
        case ClientMessage:
            if (window->_event.xclient.data.l[0] == window->_wm_delete_window)
            {
                window->_loop = 1;
            }
            break;

        case ConfigureNotify:
            int new_width = window->_event.xconfigure.width;
            int new_height = window->_event.xconfigure.height;
            if (window->_framebuffer_size_callback)
            {
                window->_framebuffer_size_callback(new_width, new_height);
            }
            break;
        }
    }
}

PC_window *PC_createWindow(int width, int height, const char *title)
{
    PC_window *_window = malloc(sizeof(PC_window));
    if (!_window)
    {
        return NULL;
    }

    _window->_loop = 0;
    _window->_title = title;

    _window->_display = XOpenDisplay(NULL);
    if (!_window->_display)
    {
        free(_window);
        return NULL;
    }

    _window->_screen = DefaultScreen(_window->_display);

    _window->_visual_info = glXChooseVisual(_window->_display, _window->_screen, (int[]){GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None});
    if (!_window->_visual_info)
    {
        free(_window);
        return NULL;
    }

    _window->_visual = _window->_visual_info->visual;
    _window->_attributes.colormap = XCreateColormap(_window->_display, RootWindow(_window->_display, _window->_screen), _window->_visual, AllocNone);
    _window->_attributes.event_mask = ExposureMask | KeyPressMask;

    _window->_window = XCreateWindow(_window->_display, RootWindow(_window->_display, _window->_screen), 0, 0, width, height, 0, CopyFromParent, InputOutput, _window->_visual, CWColormap | CWEventMask, &_window->_attributes);

    XMapWindow(_window->_display, _window->_window);
    XStoreName(_window->_display, _window->_window, _window->_title);

    _window->_context = glXCreateContext(_window->_display, _window->_visual_info, NULL, GL_TRUE);
    if (!_window->_context)
    {
        free(_window);
        return NULL;
    }

    _window->_wm_delete_window = XInternAtom(_window->_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(_window->_display, _window->_window, &_window->_wm_delete_window, 1);

    XSelectInput(_window->_display, _window->_window, StructureNotifyMask | KeyPressMask | ButtonPressMask);

    return _window;
}

void PC_destroyWindow(PC_window *window)
{

    if (window)
    {
        if (window->_context)
        {
            glXMakeCurrent(window->_display, None, NULL);
            glXDestroyContext(window->_display, window->_context);
        }

        if (window->_colormap)
        {
            XFreeColormap(window->_display, window->_colormap);
        }

        if (window->_window)
        {
            XDestroyWindow(window->_display, window->_window);
        }

        if (window->_display)
        {
            XCloseDisplay(window->_display);
        }

        free(window);
    }
}

void PC_swapBuffers(PC_window *window)
{
    if (window->_display && window->_window)
        glXSwapBuffers(window->_display, window->_window);
}

void PC_makeCurrentContext(PC_window *window)
{
    if (!glXMakeCurrent(window->_display, window->_window, window->_context))
        fprintf(stderr, "Failed to make context current.\n");
}