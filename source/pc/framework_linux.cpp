// Author: oknauta
// License: MIT
// File: pcfw_linux.cpp
// Date: 2024-11-18

#ifdef __linux__

#include "pc/framework.hpp"
#include "pc/framework_internal.hpp"
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>

#include <pc/log.hpp>

namespace PC::Framework
{
    // Keys
    
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

    // Functions
    
    int INTERNAL_set_framebuffer_size_callback(window *window, framebuffer_size_callback callback)
    {
        window->event._framebuffer_size_callback = callback;

        if (!window->event._framebuffer_size_callback)
        {
            PC::Log::warning("Failed to set framebuffer size callback");
            return 1;
        }

        return 0;
    }

    int INTERNAL_set_mouse_callback(window *window, mouse_callback callback)
    {
        window->event._mouse_callback = callback;

        if (!window->event._mouse_callback)
        {
            PC::Log::error("Failed to set mouse callback");
            return 1;
        }

        return 0;
    }

    void INTERNAL_swap_buffers(window *window)
    {
        glXSwapBuffers(window->internal._display, window->internal._handle);
    }

    int INTERNAL_create_window(window *window)
    {
        if (!window)
        {
            PC::Log::error("PCFW Internal: No window to create");
            return 1;
        }

        window->internal._display = XOpenDisplay(nullptr);
        if (!window->internal._display)
        {
            PC::Log::error("PCFW Internal: Failed to open display");
            return 1;
        }

        window->internal._screen = DefaultScreen(window->internal._display);
        if (window->internal._screen < 0)
        {
            PC::Log::error("PCFW Internal: Failed to set internal display");
            return 1;
        }

        static int _attributes[]
        {
            GLX_RGBA,
            GLX_DEPTH_SIZE,
            24,
            GLX_DOUBLEBUFFER,
            None
        };

        window->internal._visual_info = glXChooseVisual(window->internal._display, window->internal._screen, _attributes);
        if (!window->internal._visual_info)
        {
            PC::Log::error("PCFW Internal: Failed to set visual info");
            return 1;
        }
        
        Window root = RootWindow(window->internal._display, window->internal._screen);
        window->internal._attributes.colormap = XCreateColormap(window->internal._display, root, window->internal._visual_info->visual, AllocNone);
        if (!window->internal._attributes.colormap)
        {
            PC::Log::error("PCFW Internal: Failed to set colormap");
            return 1;
        }

        window->internal._attributes.event_mask = ExposureMask | KeyPressMask | ButtonPress | StructureNotifyMask | ButtonReleaseMask | KeyReleaseMask | EnterWindowMask | LeaveWindowMask | PointerMotionMask | Button1MotionMask | VisibilityChangeMask | ColormapChangeMask;
        
        window->internal._gl_context = glXCreateContext(window->internal._display, window->internal._visual_info, nullptr, GL_TRUE);
        if (!window->internal._gl_context)
        {
            PC::Log::error("PCFW Internal: Failed to create GLX context");
            return 1;
        }

        window->internal._handle = XCreateWindow(window->internal._display, root, 0, 0, window->config._width, window->config._height, 0, CopyFromParent, InputOutput, window->internal._visual_info->visual, CWColormap | CWEventMask, &window->internal._attributes);

        if (!window->internal._handle)
        {
            PC::Log::error("PCFW Internal: Failed to create window");
            return 1;
        }

        XMapWindow(window->internal._display, window->internal._handle);
        XFlush(window->internal._display);

        return 0;
    }

    int INTERNAL_destroy_window(window *window)
    {
        if (window->internal._gl_context)
        {
            glXMakeCurrent(window->internal._display, None, nullptr);
            glXDestroyContext(window->internal._display, window->internal._gl_context);
        }

        if (window->internal._colormap)
        {
            XFreeColormap(window->internal._display, window->internal._colormap);
        }

        if (window->internal._handle)
        {
            XDestroyWindow(window->internal._display, window->internal._handle);
        }

        if (window->internal._display)
        {
            XCloseDisplay(window->internal._display);
        }

        return 0;
    }

    bool INTERNAL_window_should_close(window *window)
    {
        return window ? window->config._should_close : false;
    }

    static void handle_client_message(window *window, XEvent *event)
    {
        if (event->xclient.data.l[0] == window->internal._wm_delete_window)
        {
            window->config._should_close = true;
        }
    }

    static void handle_configure_notify(window *window, XConfigureEvent *configure_event)
    {
        window->config._width = configure_event->width;
        window->config._height = configure_event->height;
        if (window->event._framebuffer_size_callback)
        {
            window->event._framebuffer_size_callback(window, configure_event->width, configure_event->height);
        }
    }

    static void handle_mouse_event(window *window, XButtonEvent *button_event)
    {
        if (window->event._mouse_callback)
        {
            window->event._mouse_callback(button_event->button, button_event->type, button_event->state);
        }
    }

    static void handle_key_event(window *window, XKeyEvent *key_event)
    {
        if (key_event->keycode < 256) {
            window->config._key_state[key_event->keycode] = key_event->type == KeyPress;
        }
    }

    void INTERNAL_poll_events(window *window)
    {
        Display *display = window->internal._display;
        XEvent event = window->internal._event;

        while (XPending(display) > 0) {
            XNextEvent(display, &event);

            switch (event.type)
            {
                case ClientMessage:
                    handle_client_message(window, &event);
                    break;
                case ConfigureNotify:
                    handle_configure_notify(window, &event.xconfigure);
                    break;
                case ButtonPress:
                case ButtonRelease:
                    handle_mouse_event(window, &event.xbutton);
                    break;
                case KeyPress:
                case KeyRelease:
                    handle_key_event(window, &event.xkey);
                    break;
            }
        }
    }

    int INTERNAL_make_context_current(window *window)
    {
        if (!glXMakeCurrent(window->internal._display, window->internal._handle, window->internal._gl_context))
        {
            Log::error("PCFW Internal: Failed to make context current");
            return 1;
        }

        return 0;
    }
    
    void INTERNAL_set_swap_interval(window *window, int interval)
    {
        static PFNGLXSWAPINTERVALEXTPROC _swap_interval = nullptr;

        if (!_swap_interval)
        {
            _swap_interval = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddressARB((const GLubyte *)"glXSwapIntervalEXT");
        }
        else
        {
            _swap_interval(window->internal._display, window->internal._handle, interval);
        }
    }


    // void get_cursor_position(window *window, int *x, int *y)
    // {
    //     Window _root;
    //     Window _child;

    //     int _root_x, _root_y;

    //     unsigned int _mask;

    //     XQueryPointer(window->_display, window->_window, &_root, &_child, &_root_x, &_root_y, x, y, &_mask);
    // }

    // int get_key(window *window, int key, int type)
    // {
    //     if (type == KEY_PRESS)
    //     {
    //         return window->key_state[key];
    //     }
    //     else if (type == KEY_RELEASE)
    //     {
    //         return !window->key_state[key];
    //     }

    //     return 0;
    // }

    
    // const char *get_window_title(window *window)
    // {
    //     return window ? window->_title : nullptr;
    // }

    // void set_framebuffer_size_callback(window *window, framebuffer_size_callback callback)
    // {
    //     if (!window)
    //     {
    //         Log::error("Failed to set framebuffer size callback");
    //         return;
    //     }
    //     window->_framebuffer_size_callback = callback;
    // }

    

    int INTERNAL_get_window_width(window *window)
    {
        return window ? window->config._width : 0;
    }
    int INTERNAL_get_window_height(window *window)
    {
        return window ? window->config._height : 0;
    }
    
    void INTERNAL_set_window_limits(window *window, int minimum_width, int minimum_height, int maximum_width, int maximum_height)
    {
        XSizeHints *size_hints = XAllocSizeHints();   
        size_hints->flags = PMinSize;
        
        if (!size_hints)
        {
            Log::error("Failed to allocate memory for size_hints");
            return;
        }
        
        size_hints->min_width = minimum_width;
        size_hints->min_height = minimum_height;
        size_hints->max_width = maximum_width;
        size_hints->max_height = maximum_height;
        
        XSetNormalHints(window->internal._display, window->internal._handle, size_hints);
        
        XFree(size_hints);
    }

    void *INTERNAL_get_proc_address(const char *proc)
    {
        if (!proc)
            return nullptr;

        void *_address = (void*)glXGetProcAddress((const GLubyte*)proc);
        
        if (!_address)
        {
            Log::error("Failed to load proc");
            return nullptr;
        }

        return _address;
    }
} // namespace PCFW

#endif
