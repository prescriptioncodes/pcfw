// Author: oknauta
// License:
// File: window.cpp
// Date: 2024-12-10

#include "pcfw/pcfw.hpp"

#include "pcfw/pcfw_internal.hpp"

namespace PCFW
{
    
    window *createWindow(int width, int height, const char *title)
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

        INTERNAL_createWindow(_window);
        INTERNAL_showWindow(_window);
        INTERNAL_setWindowTitle(_window);
        INTERNAL_createContext(_window);
        INTERNAL_setEvents(_window);

        return _window;
    }

} // namespace PCFW