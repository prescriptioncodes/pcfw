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

#include <pclog/pclog.hpp>

namespace PCFW
{
    constexpr int MOUSE_LEFT_BUTTON = 1;
    constexpr int MOUSE_MIDDLE_BUTTON = 2;
    constexpr int MOUSE_RIGHT_BUTTON = 3;
    
    constexpr int MOUSE_PRESS_BUTTON = 4;
    constexpr int MOUSE_RELEASE_BUTTON = 5;
    
    constexpr int KEY_LEFT_SHIFT = 160;
    constexpr int KEY_RIGHT_SHIFT = 161;

    constexpr int KEY_LEFT_CONTROL = 162;
    constexpr int KEY_RIGHT_CONTROL = 163;

    constexpr int KEY_LEFT_ALT = 164;
    constexpr int KEY_RIGHT_ALT = 165;

    constexpr int KEY_F1 = 67;
    constexpr int KEY_F2 = 68;
    constexpr int KEY_F3 = 69;
    constexpr int KEY_F4 = 70;
    constexpr int KEY_F5 = 71;
    constexpr int KEY_F6 = 72;
    constexpr int KEY_F7 = 73;
    constexpr int KEY_F8 = 74;
    constexpr int KEY_F9 = 75;
    constexpr int KEY_F10 = 76;
    constexpr int KEY_F11 = 95;
    constexpr int KEY_F12 = 96;

    constexpr int KEY_1 = 10;
    constexpr int KEY_2 = 11;
    constexpr int KEY_3 = 12;
    constexpr int KEY_4 = 13;
    constexpr int KEY_5 = 14;
    constexpr int KEY_6 = 15;
    constexpr int KEY_7 = 16;
    constexpr int KEY_8 = 17;
    constexpr int KEY_9 = 18;
    constexpr int KEY_0 = 19;

    constexpr int KEY_A = 38;
    constexpr int KEY_B = 56;
    constexpr int KEY_C = 54;
    constexpr int KEY_D = 40;
    constexpr int KEY_E = 26;
    constexpr int KEY_F = 41;
    constexpr int KEY_G = 42;
    constexpr int KEY_H = 43;
    constexpr int KEY_I = 31;
    constexpr int KEY_J = 44;
    constexpr int KEY_K = 45;
    constexpr int KEY_L = 46;
    constexpr int KEY_M = 58;
    constexpr int KEY_N = 57;
    constexpr int KEY_O = 32;
    constexpr int KEY_P = 33;
    constexpr int KEY_Q = 24;
    constexpr int KEY_R = 27;
    constexpr int KEY_S = 39;
    constexpr int KEY_T = 28;
    constexpr int KEY_U = 30;
    constexpr int KEY_V = 55;
    constexpr int KEY_W = 25;
    constexpr int KEY_X = 53;
    constexpr int KEY_Y = 29;
    constexpr int KEY_Z = 52;

    void getCursorPosition(window *window, int *x, int *y)
    {
        Window _root;
        Window _child;

        int _root_x, _root_y;

        unsigned int _mask;

        XQueryPointer(window->_display, window->_window, &_root, &_child, &_root_x, &_root_y, x, y, &_mask);
    }

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
        if (!window)
        {
            PCLOG::error("Failed to set framebuffer size callback");
            return;
        }
        window->_framebuffer_size_callback = callback;
    }

    void setMouseCallback(window *window, mouse_callback callback)
    {
        if (!window)
        {
            PCLOG::warning("No window to set mouse callback");
            return;
        }

        window->_mouse_callback = callback;
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
            case ButtonPress:
            {
                if (window->_mouse_callback)
                {
                    window->_mouse_callback(window->_event.xbutton.button, window->_event.xbutton.type, window->_event.xbutton.state);
                }
                break;
            }
            case ButtonRelease:
                if (window->_mouse_callback)
                {
                    window->_mouse_callback(window->_event.xbutton.button, window->_event.xbutton.type, window->_event.xbutton.state);
                }
                break;
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
        if (!window->_display && !window->_window)
        {
            PCLOG::error("Failed to show window");
            return;
        }
        XMapWindow(window->_display, window->_window);
    }

    void INTERNAL_setWindowTitle(window *window)
    {
        if (!window->_display && !window->_window)
        {
            PCLOG::error("Failed to set internal window title");
            return;
        }
        XStoreName(window->_display, window->_window, window->_title);
    }

    void INTERNAL_createWindow(window *window)
    {
        window->_display = XOpenDisplay(nullptr);
        if (!window->_display && window)
        {
            PCLOG::error("Failed to open X display");
            delete window;
            return;
        }
        window->_screen = DefaultScreen(window->_display);
        window->_window = XCreateWindow(window->_display, RootWindow(window->_display, window->_screen), 0, 0, window->_width, window->_height, 0, CopyFromParent, InputOutput, window->_visual, CWColormap | CWEventMask, &window->_attributes);

        static int _attributes[]{GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

        window->_visual_info = glXChooseVisual(window->_display, window->_screen, _attributes);

        window->_visual = window->_visual_info->visual;
        window->_attributes.colormap = XCreateColormap(window->_display, RootWindow(window->_display, window->_screen), window->_visual, AllocNone);
        window->_attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;

        window->_wm_delete_window = XInternAtom(window->_display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(window->_display, window->_window, &window->_wm_delete_window, 1);
        XSelectInput(window->_display, window->_window, StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);

        if (!window)
        {
            PCLOG::error("Failed to create window");
            delete window;
        }
    }

    void INTERNAL_createContext(window *window)
    {
        if (!window)
        {
            PCLOG::warning("No window to create context");
        }
        window->_context = glXCreateContext(window->_display, window->_visual_info, nullptr, GL_TRUE);
    }

    void destroyWindow(window *window)
    {
        if (!window)
            PCLOG::warning("No window to destroy");
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
        if (!window->_display && !window->_window)
        {
            PCLOG::warning("No window to swap buffers");
            return;
        }
        glXSwapBuffers(window->_display, window->_window);
    }

    void makeContextCurrent(window *window)
    {
        if (!glXMakeCurrent(window->_display, window->_window, window->_context))
            PCLOG::warning("No window to make context");
    }
} // namespace PCFW

#endif