// Author: oknauta
// License: MIT
// File: window.cpp
// Date: 2024-12-10

#include "pc/framework.hpp"
#include "pc/framework_internal.hpp"

namespace PC
{
    window *create_window(int width, int height, const char *title)
    {
        window *_window = new window;
        if (!_window)
        {
            return nullptr;
        }

        _window->_should_close = false;
        _window->_title = title;
        _window->_width = width;
        _window->_height = height;

        INTERNAL_create_window(_window);
        INTERNAL_show_window(_window);
        INTERNAL_set_window_title(_window);
        INTERNAL_create_context(_window);

        return _window;
    }

} // namespace PCFW