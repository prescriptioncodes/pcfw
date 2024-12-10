// Author: oknauta
// License: MIT
// File: pcfw.hpp
// Date: 2024-11-18

#ifndef _PCFW_HPP
#define _PCFW_HPP

#ifdef _WIN32
#ifdef PCFW_EXPORTS
#define PCFW_API __declspec(dllexport)
#else
#define PCFW_API __declspec(dllimport)
#endif
#else
#define PCFW_API
#endif

namespace PCFW
{
    extern const int KEY_PRESS;
    extern const int KEY_RELEASE;
    
    extern const int KEY_A;
    extern const int KEY_B;
    extern const int KEY_C;
    extern const int KEY_D;
    extern const int KEY_E;
    extern const int KEY_F;
    extern const int KEY_G;
    extern const int KEY_H;
    extern const int KEY_I;
    extern const int KEY_J;
    extern const int KEY_K;
    extern const int KEY_M;
    extern const int KEY_N;
    extern const int KEY_O;
    extern const int KEY_P;
    extern const int KEY_Q;
    extern const int KEY_R;
    extern const int KEY_S;
    extern const int KEY_T;
    extern const int KEY_U;
    extern const int KEY_V;
    extern const int KEY_W;
    extern const int KEY_X;
    extern const int KEY_Y;
    extern const int KEY_Z;
    
    typedef void (*framebuffer_size_callback)(int width, int height);
    typedef struct window window;

    PCFW_API int getKey(window *window, int key, int type);
    PCFW_API window *createWindow(int width, int height, const char *title);
    PCFW_API int getWindowWidth(window *window);
    PCFW_API int getWindowHeight(window *window);
    PCFW_API void destroyWindow(window *window);
    PCFW_API void swapBuffers(window *window);
    PCFW_API void makeCurrentContext(window *window);
    PCFW_API void pollEvents(window *window);
    PCFW_API void setFramebufferSizeCallback(window *window, framebuffer_size_callback callback);
    PCFW_API void setSwapInterval(window *window, int interval);
    PCFW_API bool windowShouldClose(window *window);
    PCFW_API const char *getWindowTitle(window *window);

} // namespace PCFW

#endif // _PCFW_HPP