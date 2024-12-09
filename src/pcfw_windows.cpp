// Author: oknauta
// License: MIT
// File: pcfw_windows.cpp
// Date: 2024-12-09

#ifdef _WIN32

#include "pcfw/pcfw_internal.hpp"
#include <GL/gl.h>
#include <windows.h>

namespace PCFW
{
    int getKey(window *window, int key, int type)
    {
        if (!window)
            return 0;

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

    void INTERNAL_createContext(window *window)
    {

        SetWindowLongPtr(window->_handle_window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));

        window->_handle_device_context = GetDC(window->_handle_window);

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

        int pixelFormat = ChoosePixelFormat(window->_handle_device_context, &pfd);
        SetPixelFormat(window->_handle_device_context, pixelFormat, &pfd);

        window->_context = wglCreateContext(window->_handle_device_context);
        if (!window->_context)
        {
            destroyWindow(window);
            return;
        }
    }

    void INTERNAL_setEvents(window *window)
    {
    }

    void INTERNAL_setWindowTitle(window *window)
    {
        SetWindowTextA(window->_handle_window, window->_title);
    }

    bool windowShouldClose(window *window)
    {
        return window->_should_close ? true : false;
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

    LRESULT CALLBACK windowProc(HWND handle_window, UINT u_message, WPARAM w_param, LPARAM l_param)
    {
        window *_window = reinterpret_cast<window *>(GetWindowLongPtr(handle_window, GWLP_USERDATA));

        switch (u_message)
        {
        case WM_CLOSE:
            if (_window)
                _window->_should_close = true;
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

    void INTERNAL_createWindow(window *window)
    {

        WNDCLASSEXA wc = {};
        wc.lpfnWndProc = windowProc;
        wc.cbSize = sizeof(WNDCLASSEXA);
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = "PCFW_WindowClass";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        RegisterClassExA(&wc);

        window->_handle_window = CreateWindowExA(
            0, "PCFW_WindowClass", window->_title, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, window->_width, window->_height, nullptr, nullptr, wc.hInstance, nullptr);

        if (!window->_handle_window)
        {
            delete window;
            return;
        }
    }
    void INTERNAL_showWindow(window *window)
    {
        ShowWindow(window->_handle_window, SW_SHOW);
        UpdateWindow(window->_handle_window);
    }

    void destroyWindow(window *window)
    {
        if (!window)
            return;

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

    void swapBuffers(window *window)
    {
        if (window && window->_handle_device_context)
            SwapBuffers(window->_handle_device_context);
    }

    void makeCurrentContext(window *window)
    {
        if (window)
            wglMakeCurrent(window->_handle_device_context, window->_context);
    }
} // namespace PCFW

#endif
