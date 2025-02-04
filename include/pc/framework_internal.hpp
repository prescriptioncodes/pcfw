// Author: oknauta
// License: MIT
// File: pcfw_internal.hpp
// Date: 2024-12-10

// DON'T USE THIS FILE!! THIS IS FILE FOR PCFW ESSENTIAL FUNCTIONALITIES 

#ifndef PCFW_INTERNAL_HPP
#define PCFW_INTERNAL_HPP

#include "framework.hpp"
#include <X11/X.h>

#ifdef __linux__
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif

#ifdef _WIN32
#include <GL/gl.h>
#include <windows.h>
#endif

namespace PC::Framework
{
	// Internal functions
	PCFW_API int INTERNAL_create_window(window *window);
	PCFW_API int INTERNAL_destroy_window(window *window);
	PCFW_API int INTERNAL_make_context_current(window *window);
	PCFW_API int INTERNAL_set_mouse_callback(window *window, mouse_callback callback);
	PCFW_API int INTERNAL_set_key_callback(window *window, key_callback callback);
	PCFW_API int INTERNAL_set_framebuffer_size_callback(window *window, framebuffer_size_callback callback);
	PCFW_API bool INTERNAL_window_should_close(window *window);
	PCFW_API void INTERNAL_poll_events(window *window);
	PCFW_API void INTERNAL_swap_buffers(window *window);
	PCFW_API void INTERNAL_set_swap_interval(window *window, int interval);
	PCFW_API void INTERNAL_set_window_limits(window *window, int minimum_width, int minimum_height, int maximum_width, int maximum_height);
	PCFW_API void *INTERNAL_get_proc_address(const char *proc);


	// Implementation of the opaque struct "window"
    struct window
    {
    public:
        struct config
        {
            int _width, _height;
            const char *_title;
            bool _should_close;
            bool _key_state[256];
            void (*_proc)(const char *proc_name);
        } config;

        struct event
        {
            framebuffer_size_callback _framebuffer_size_callback;
            mouse_callback _mouse_callback;
	    key_callback _key_callback;
        } event;
        
        struct internal
        {
#ifdef __linux__ 
            // Xlib stuff
            Display *_display;
            Window _handle;
            Colormap _colormap;
            int _screen;
            XEvent _event;
            // Visual *_visual;
            XVisualInfo *_visual_info;
            GLXContext _gl_context;
            XSetWindowAttributes _attributes;
            Atom _wm_delete_window;
#elif _WIN64
            // Windows stuff
            // To be added
#endif
        } internal;
    };
} // namespace PCFW

#endif // PCFW_INTERNAL_HPP
