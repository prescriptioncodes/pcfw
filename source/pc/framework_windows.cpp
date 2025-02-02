// Author: oknauta
// License: MIT
// File: pcfw_windows.cpp
// Date: 2024-12-09

#ifdef _WIN32

#include <windows.h>

#include "pc/framework_internal.hpp"

#include "pc/framework.hpp"
#include <pc/log.hpp>

namespace PC::Framework
{
    // Keys

    const int MOUSE_LEFT_BUTTON = 0;
    const int MOUSE_RIGHT_BUTTON = 2;
    const int MOUSE_MIDDLE_BUTTON = 1;

    const int MOUSE_PRESS_BUTTON = 0;
    const int MOUSE_RELEASE_BUTTON = 1;

    constexpr int KEY_LEFT_SHIFT = 0xA0;
    constexpr int KEY_RIGHT_SHIFT = 0xA1;

    constexpr int KEY_LEFT_CONTROL = 0xA2;
    constexpr int KEY_RIGHT_CONTROL = 0xA3;

    constexpr int KEY_LEFT_ALT = 0xA4;
    constexpr int KEY_RIGHT_ALT = 0xA5;

    constexpr int KEY_F1 = 0x70;
    constexpr int KEY_F2 = 0x71;
    constexpr int KEY_F3 = 0x72;
    constexpr int KEY_F4 = 0x73;
    constexpr int KEY_F5 = 0x74;
    constexpr int KEY_F6 = 0x75;
    constexpr int KEY_F7 = 0x76;
    constexpr int KEY_F8 = 0x77;
    constexpr int KEY_F9 = 0x78;
    constexpr int KEY_F10 = 0x79;
    constexpr int KEY_F11 = 0x7A;
    constexpr int KEY_F12 = 0x7B;

    constexpr int KEY_0 = 0x30;
    constexpr int KEY_1 = 0x31;
    constexpr int KEY_2 = 0x32;
    constexpr int KEY_3 = 0x33;
    constexpr int KEY_4 = 0x34;
    constexpr int KEY_5 = 0x35;
    constexpr int KEY_6 = 0x36;
    constexpr int KEY_7 = 0x37;
    constexpr int KEY_8 = 0x38;
    constexpr int KEY_9 = 0x39;

    constexpr int KEY_A = 0x41;
    constexpr int KEY_B = 0x42;
    constexpr int KEY_C = 0x43;
    constexpr int KEY_D = 0x44;
    constexpr int KEY_E = 0x45;
    constexpr int KEY_F = 0x46;
    constexpr int KEY_G = 0x47;
    constexpr int KEY_H = 0x48;
    constexpr int KEY_I = 0x49;
    constexpr int KEY_J = 0x4A;
    constexpr int KEY_K = 0x4B;
    constexpr int KEY_L = 0x4C;
    constexpr int KEY_M = 0x4D;
    constexpr int KEY_N = 0x4E;
    constexpr int KEY_O = 0x4F;
    constexpr int KEY_P = 0x50;
    constexpr int KEY_Q = 0x51;
    constexpr int KEY_R = 0x52;
    constexpr int KEY_S = 0x53;
    constexpr int KEY_T = 0x54;
    constexpr int KEY_U = 0x55;
    constexpr int KEY_V = 0x56;
    constexpr int KEY_W = 0x57;
    constexpr int KEY_X = 0x58;
    constexpr int KEY_Y = 0x59;
    constexpr int KEY_Z = 0x5A;

    // Functions

    void set_window_limits(window *window, int minimum_width, int minimum_height, int maximum_width, int maximum_height)
    {
        if (!window)
        {
            Log::warning("No window to set limits");
            return;
        }

        window->_minimum_width = minimum_width;
        window->_minimum_height = minimum_height;
        window->_maximum_width = maximum_width;
        window->_maximum_height = maximum_height;
    }

    void *get_proc_address(const char *proc)
    {
        void *proc_address = (void *)wglGetProcAddress(proc);

        return proc_address;
    }

    static void handle_mouse(window *window, const int &BUTTON, const int &ACTION)
    {
        if (!window)
        {
            return;
        }

        window->_mouse_callback(BUTTON, ACTION, 0);
    }

    void get_cursor_position(window *window, int *x, int *y)
    {
        POINT _point; // Point that will storage the x and y

        // Getting the cursor position
        if (!GetCursorPos(&_point))
        {
            Log::error("Failed to get cursor position");
            return;
        }

        // Converting the screen coordinates from the point on the screen to client-area coordinates
        if (!ScreenToClient(window->_handle_window, &_point))
        {
            Log::error("Failed to pass screen to client");
            return;
        }

        // Giving the _point.x/.y for x/y
        *x = _point.x;
        *y = _point.y;
    }

    int get_key(window *window, int key, int type)
    {
        if (!window)
        {
            Log::warning("function `get_key` doesn't know what window should get the keys");
            return 0;
        }

        if (type == KEY_PRESS)
        {
            return window->_key_state.test(key);
        }
        else if (type == KEY_RELEASE)
        {
            return !window->_key_state.test(key);
        }

        return 0;
    }

    void set_swap_interval(window *window, int interval)
    {
        typedef BOOL(WINAPI * SWAP_INTERVAL)(int);
        static SWAP_INTERVAL wglSwapIntervalEXT = nullptr;

        if (!wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT = (SWAP_INTERVAL)wglGetProcAddress("wglSwapIntervalEXT");
        }

        if (wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT(interval);
        }
    }

    void INTERNAL_create_context(window *window)
    {
        if (!window)
        {
            Log::error("No window to create context");
            return;
        }

        // Changing the attributes of the window
        SetWindowLongPtr(window->_handle_window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

        // Getting the device context
        window->_handle_device_context = GetDC(window->_handle_window);

        // Configuring pixel format
        PIXELFORMATDESCRIPTOR pixel_format_descriptor = {};
        pixel_format_descriptor.nSize = sizeof(pixel_format_descriptor);
        pixel_format_descriptor.nVersion = 1;
        pixel_format_descriptor.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
        pixel_format_descriptor.iPixelType = PFD_TYPE_RGBA;
        pixel_format_descriptor.cColorBits = 24;
        pixel_format_descriptor.cRedBits = 8;
        pixel_format_descriptor.cGreenBits = 8;
        pixel_format_descriptor.cBlueBits = 8;
        pixel_format_descriptor.cAlphaBits = 8;
        pixel_format_descriptor.cDepthBits = 24;

        int pixel_format = ChoosePixelFormat(window->_handle_device_context, &pixel_format_descriptor);
        SetPixelFormat(window->_handle_device_context, pixel_format, &pixel_format_descriptor);

        // Creating context
        window->_context = wglCreateContext(window->_handle_device_context);
        if (!window->_context)
        {
            Log::error("Failed to create context");
            destroy_window(window);
            return;
        }
    }

    void INTERNAL_set_window_title(window *window)
    {
        if (!SetWindowTextA(window->_handle_window, window->_title))
            Log::warning("No window to set title");
    }

    bool window_should_close(window *window)
    {
        return window->_should_close ? true : false;
    }

    const char *get_window_title(window *window)
    {
        return window ? window->_title : nullptr;
    }

    void set_framebuffer_size_callback(window *window, framebuffer_size_callback callback)
    {
        if (!window)
        {
            Log::warning("No window to set framebuffer size callback");
            return;
        }
        window->_framebuffer_size_callback = callback;
    }

    void set_mouse_callback(window *window, mouse_callback callback)
    {
        if (!window)
        {
            Log::warning("No window to set mouse callback");
            return;
        }

        window->_mouse_callback = callback;
    }

    int get_window_width(window *window)
    {
        return window ? window->_width : 0;
    }

    int get_window_height(window *window)
    {
        return window ? window->_height : 0;
    }

    void poll_events(window *window)
    {
        while (PeekMessage(&window->_message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&window->_message);
            DispatchMessage(&window->_message);
        }
    }

    static LRESULT CALLBACK window_proc(HWND handle_window, UINT u_message, WPARAM w_param, LPARAM l_param)
    {
        window *_window = reinterpret_cast<window *>(GetWindowLongPtr(handle_window, GWLP_USERDATA));

        switch (u_message)
        {
        case WM_CLOSE:
            if (_window)
                _window->_should_close = true;
            break;

        case WM_GETMINMAXINFO:
        {
            if (!_window)
            {
                break;
            }

            LPMINMAXINFO minimum_maximum_info = (LPMINMAXINFO)l_param;
            if (_window->_minimum_width > 0)
            {
                minimum_maximum_info->ptMinTrackSize.x = _window->_minimum_width;
            }

            if (_window->_minimum_height > 0)
            {
                minimum_maximum_info->ptMinTrackSize.y = _window->_minimum_height;
            }

            if (_window->_maximum_width > 0)
            {
                minimum_maximum_info->ptMaxTrackSize.x = _window->_maximum_width;
            }

            if (_window->_maximum_height > 0)
            {
                minimum_maximum_info->ptMaxTrackSize.y = _window->_maximum_height;
            }
            break;
        }

        case WM_SIZE:
            if (_window)
            {
                _window->_width = LOWORD(l_param);
                _window->_height = HIWORD(l_param);
                if (_window->_framebuffer_size_callback)
                {
                    _window->_framebuffer_size_callback(_window, _window->_width, _window->_height);
                }
            }
            break;

        case WM_LBUTTONDOWN:
            handle_mouse(_window, MOUSE_LEFT_BUTTON, MOUSE_PRESS_BUTTON);
            break;

        case WM_LBUTTONUP:
            handle_mouse(_window, MOUSE_LEFT_BUTTON, MOUSE_RELEASE_BUTTON);
            break;

        case WM_RBUTTONDOWN:
            handle_mouse(_window, MOUSE_RIGHT_BUTTON, MOUSE_PRESS_BUTTON);
            break;

        case WM_RBUTTONUP:
            handle_mouse(_window, MOUSE_RIGHT_BUTTON, MOUSE_RELEASE_BUTTON);
            break;

        case WM_MBUTTONDOWN:
            handle_mouse(_window, MOUSE_MIDDLE_BUTTON, MOUSE_PRESS_BUTTON);
            break;

        case WM_MBUTTONUP:
            handle_mouse(_window, MOUSE_MIDDLE_BUTTON, MOUSE_RELEASE_BUTTON);
            break;

        case WM_KEYDOWN:

            _window->_key_state.set(w_param, true); // Set the key state to "pressed"

            break;

        case WM_KEYUP:

            _window->_key_state.set(w_param, false); // Set the key state to "released"

            break;

        default:
            return DefWindowProc(handle_window, u_message, w_param, l_param);
        }
        return 0;
    }

    void INTERNAL_create_window(window *window)
    {

        WNDCLASSEXA wc = {};
        wc.lpfnWndProc = window_proc;
        wc.cbSize = sizeof(WNDCLASSEXA);
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = "PCFW_WindowClass";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        RegisterClassExA(&wc);

        window->_handle_window = CreateWindowExA(0, "PCFW_WindowClass", window->_title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, window->_width, window->_height, nullptr, nullptr, wc.hInstance, nullptr);

        if (!window->_handle_window)
        {
            Log::error("Failed to create window");
            delete window;
            return;
        }
    }

    void INTERNAL_show_window(window *window)
    {
        if (ShowWindow(window->_handle_window, SW_SHOW))
        {
            Log::warning("No window to show");
            return;
        }

        if (!UpdateWindow(window->_handle_window))
        {
            Log::error("Failed to update window");
            return;
        }
    }

    void destroy_window(window *window)
    {
        if (!window)
        {
            Log::warning("No window to destroy");
            return;
        }

        if (window->_context)
        {
            wglMakeCurrent(window->_handle_device_context, nullptr);
            wglDeleteContext(window->_context);
        }

        if (window->_handle_device_context)
            ReleaseDC(window->_handle_window, window->_handle_device_context);

        if (window->_handle_window)
            DestroyWindow(window->_handle_window);

        delete window;
    }

    void swap_buffers(window *window)
    {
        if (!window)
        {
            Log::warning("No window to swap buffer");
            return;
        }

        if (!window->_handle_device_context)
        {
            Log::error("Failed to get handle device context to swap buffer");
            return;
        }
        SwapBuffers(window->_handle_device_context);
    }

    void make_context_current(window *window)
    {
        if (!window)
        {
            Log::warning("No window to make context");
            return;
        }
        if (!window->_handle_device_context)
        {
            Log::error("Failed to get handle device context to make the context of a window");
            return;
        }
        wglMakeCurrent(window->_handle_device_context, window->_context);
    }
} // namespace PC

#endif
