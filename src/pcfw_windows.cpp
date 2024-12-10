// Author: oknauta
// License: MIT
// File: pcfw_winapi.cpp
// Date: 2024-12-09

#ifdef _WIN32

#include "pcfw/pcfw.hpp"
#include <GL/gl.h>
#include <windows.h>

namespace PCFW
{
    struct window
    {
        int _position_x, _position_y;                         // Position
        int _width, _height;                                  // Size
        const char *_title;                                   // Title
        HWND _hwnd;                                           // Window Handle
        HDC _hdc;                                             // Device Context
        HGLRC _context;                                       // OpenGL Context
        int _should_close;                                    // If the window is looping
        framebuffer_size_callback _framebuffer_size_callback; // Framebuffer size callback
    };

    void setSwapInterval(window *window, int interval)
    {
        typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int);
        static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

        if (!wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        }

        if (wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT(interval);
        }
    }

    const char *getWindowTitle(window *window)
    {
        return window ? window->_title : nullptr;
    }

    void setFramebufferSizeCallback(window *window, framebuffer_size_callback callback)
    {
        if (window)
        {
            window->_framebuffer_size_callback = callback;
        }
    }

    int windowShouldClose(window *window)
    {
        return window ? window->_should_close : 0;
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
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    LRESULT CALLBACK windowProc(HWND h_window, UINT u_message, WPARAM w_param, LPARAM l_param)
    {
        window *_window = reinterpret_cast<window *>(GetWindowLongPtr(h_window, GWLP_USERDATA));

        switch (u_message)
        {
        case WM_CLOSE:
            if (_window)
                _window->_should_close = 1;
            break;

        case WM_SIZE:
            if (_window)
            {
                _window->_width = LOWORD(l_param);
                _window->_height = HIWORD(l_param);
                if (_window->_framebuffer_size_callback)
                {
                    _window->_framebuffer_size_callback(_window->_width, _window->_height);
                }
            }
            break;

        default:
            return DefWindowProc(h_window, u_message, w_param, l_param);
        }
        return 0;
    }

    window *createWindow(int width, int height, const char *title)
    {
        window *_window = new window;
        if (!_window)
        {
            return nullptr;
        }

        _window->_should_close = 0;
        _window->_title = title;

        WNDCLASSA wc = {};
        wc.lpfnWndProc = windowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = "PCFW_WindowClass";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        RegisterClassA(&wc);

        _window->_hwnd = CreateWindowExA(
            0, "PCFW_WindowClass", _window->_title, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, wc.hInstance, nullptr);

        if (!_window->_hwnd)
        {
            delete _window;
            return nullptr;
        }

        SetWindowLongPtr(_window->_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(_window));

        _window->_hdc = GetDC(_window->_hwnd);

        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cRedBits = 8;
        pfd.cGreenBits = 8;
        pfd.cBlueBits = 8;
        pfd.cAlphaBits = 8;
        pfd.cDepthBits = 24;

        int pixelFormat = ChoosePixelFormat(_window->_hdc, &pfd);
        SetPixelFormat(_window->_hdc, pixelFormat, &pfd);

        _window->_context = wglCreateContext(_window->_hdc);
        if (!_window->_context)
        {
            destroyWindow(_window);
            return nullptr;
        }

        wglMakeCurrent(_window->_hdc, _window->_context);

        ShowWindow(_window->_hwnd, SW_SHOW);
        UpdateWindow(_window->_hwnd);

        _window->_width = width;
        _window->_height = height;

        return _window;
    }

    void destroyWindow(window *window)
    {
        if (window)
        {
            if (window->_context)
            {
                wglMakeCurrent(window->_hdc, nullptr);
                wglDeleteContext(window->_context);
            }

            if (window->_hdc)
            {
                ReleaseDC(window->_hwnd, window->_hdc);
            }

            if (window->_hwnd)
            {
                DestroyWindow(window->_hwnd);
            }

            delete window;
        }
    }

    void swapBuffers(window *window)
    {
        if (window && window->_hdc)
            SwapBuffers(window->_hdc);
    }

    void makeCurrentContext(window *window)
    {
        if (window)
            wglMakeCurrent(window->_hdc, window->_context);
    }
} // namespace PCFW

#endif
