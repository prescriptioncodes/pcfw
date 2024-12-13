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

    void makeContextCurrent(window *window)
    {
        if (window)
            wglMakeCurrent(window->_handle_device_context, window->_context);
    }
} // namespace PCFW

#endif
