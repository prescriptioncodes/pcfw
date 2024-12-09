// Author: oknauta
// License: MIT
// File: pcfw_linux.cpp
// Date: 2024-11-18

#ifdef __linux__

#include "pcfw/pcfw_internal.hpp"
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#include <bitset>

namespace PCFW
{
    int getKey(window *window, int key, int type)
    {
        if (type == KEY_PRESS)
        {
            return window->key_state[key];
        }
        else if (type == KEY_RELEASE)
        {
            return !window->key_state[key];
        }

        return 0;
    }

    void setSwapInterval(window *window, int interval)
    {
        static PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = nullptr;

        if (!glXSwapIntervalEXT)
        {
            glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalEXT");
        }

        if (glXSwapIntervalEXT)
        {
            glXSwapIntervalEXT(window->_display, window->_window, interval);
        }
    }

    const char *getWindowTitle(window *window)
    {
        return window ? window->_title : nullptr;
    }

    void setFramebufferSizeCallback(window *window, framebuffer_size_callback callback)
    {
        if (window)
            window->_framebuffer_size_callback = callback;
    }

    bool windowShouldClose(window *window)
    {
        return window ? window->_should_close : false;
    }

    int getWindowWidth(window *window)
    {
        return window ? window->_width : 0;
    }
    int getWindowHeight(window *window)
    {
        return window ? window->_height : 0;
    }

    void pollEvents(window *window)
    {
        if (XPending(window->_display) > 0)
        {
            XNextEvent(window->_display, &window->_event);

            switch (window->_event.type)
            {
            case ClientMessage:
                if (window->_event.xclient.data.l[0] == window->_wm_delete_window)
                {
                    window->_should_close = true;
                }
                break;

            case ConfigureNotify:
            {
                int new_width = window->_event.xconfigure.width;
                int new_height = window->_event.xconfigure.height;
                window->_width = new_width;
                window->_height = new_height;
                if (window->_framebuffer_size_callback)
                {
                    window->_framebuffer_size_callback(new_width, new_height);
                }
                break;
            }
            case KeyPress:
            {
                window->key_state[window->_event.xkey.keycode] = true;
                break;
            }
            case KeyRelease:
                window->key_state[window->_event.xkey.keycode] = false;
                break;
            }
        }
    }

    void INTERNAL_showWindow(window *window)
    {
        XMapWindow(window->_display, window->_window);
    }

    void INTERNAL_setWindowTitle(window *window)
    {
        XStoreName(window->_display, window->_window, window->_title);
    }

    void INTERNAL_createWindow(window *window)
    {
        window->_display = XOpenDisplay(nullptr);
        if (!window->_display && window)
        {
            delete window;
            return;
        }
        window->_screen = DefaultScreen(window->_display);
        window->_window = XCreateWindow(window->_display, RootWindow(window->_display, window->_screen), 0, 0, window->_width, window->_height, 0, CopyFromParent, InputOutput, window->_visual, CWColormap | CWEventMask, &window->_attributes);

        static int _attributes[]{GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

        window->_visual_info = glXChooseVisual(window->_display, window->_screen, _attributes);

        if (!window->_visual_info && window)
        {
            delete window;
            return;
        }

        window->_visual = window->_visual_info->visual;
        window->_attributes.colormap = XCreateColormap(window->_display, RootWindow(window->_display, window->_screen), window->_visual, AllocNone);
        window->_attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;
    }

    void INTERNAL_createContext(window *window)
    {
        window->_context = glXCreateContext(window->_display, window->_visual_info, nullptr, GL_TRUE);
        if (!window->_context && window)
        {
            delete window;
            return;
        }
    }

    void INTERNAL_setEvents(window *window)
    {
        window->_wm_delete_window = XInternAtom(window->_display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(window->_display, window->_window, &window->_wm_delete_window, 1);

        XSelectInput(window->_display, window->_window, StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask);
    }

    void destroyWindow(window *window)
    {
        if (!window)
            return;

        if (window->_context)
        {
            glXMakeCurrent(window->_display, None, nullptr);
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

        delete window;
    }

    void swapBuffers(window *window)
    {
        if (window->_display && window->_window)
            glXSwapBuffers(window->_display, window->_window);
    }

    void makeCurrentContext(window *window)
    {
        if (!glXMakeCurrent(window->_display, window->_window, window->_context))
            return;
    }
} // namespace PCFW

#endif