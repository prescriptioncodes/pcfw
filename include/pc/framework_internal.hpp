// Author: oknauta
// License: MIT
// File: pcfw_internal.hpp
// Date: 2024-12-10

#ifndef PCFW_INTERNAL_HPP
#define PCFW_INTERNAL_HPP

#include "framework.hpp"

#ifdef __linux__
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <bitset>
#endif

#ifdef _WIN32
#include <GL/gl.h>
#include <bitset>
#include <windows.h>
#endif

namespace PC
{
    PCFW_API void INTERNAL_show_window(window *window);
    PCFW_API void INTERNAL_set_window_title(window *window);
    PCFW_API void INTERNAL_create_window(window *window);
    PCFW_API void INTERNAL_create_context(window *window);
    PCFW_API void INTERNAL_set_events(window *window);

#ifdef _WIN32
    struct window
    {
        int _position_x, _position_y;                         // Position
        int _width, _height;                                  // Size
        int _minimum_width, _minimum_height;
        int _maximum_width, _maximum_height;
        const char *_title;                                   // Title
        HWND _handle_window;                                  // Window Handle
        HDC _handle_device_context;                           // Device Context
        HGLRC _context;                                       // OpenGL Context
        int _should_close;                                    // If the window is looping
        framebuffer_size_callback _framebuffer_size_callback; // Framebuffer size callback
        std::bitset<256> _key_state;
        MSG _message;
        mouse_callback _mouse_callback;
    };
#endif

#ifdef __linux__
    struct window
    {
        void (*proc)(const char *proc_name);
        int _width, _height;
        const char *_title;
        int _screen;
        int _should_close;
        Display *_display;
        Window _window;
        Colormap _colormap;
        XEvent _event;
        Visual *_visual;
        XVisualInfo *_visual_info;
        GLXContext _context;
        XSetWindowAttributes _attributes;
        Atom _wm_delete_window;
        framebuffer_size_callback _framebuffer_size_callback;
        mouse_callback _mouse_callback;
        std::bitset<256> key_state;
    };
#endif

} // namespace PCFW

#endif // PCFW_INTERNAL_HPP