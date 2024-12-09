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
    typedef void (*framebuffer_size_callback)(int width, int height);
    typedef struct window window;

    PCFW_API window *createWindow(int width, int height, const char *title);
    PCFW_API int getWindowWidth(window *window);
    PCFW_API int getWindowHeight(window *window);
    PCFW_API void destroyWindow(window *window);
    PCFW_API void swapBuffers(window *window);
    PCFW_API void makeCurrentContext(window *window);
    PCFW_API void pollEvents(window *window);
    PCFW_API void setFramebufferSizeCallback(window *window, framebuffer_size_callback callback);
    PCFW_API void setSwapInterval(window *window, int interval);
    PCFW_API int windowShouldClose(window *window);
    PCFW_API const char *getWindowTitle(window *window);

} // namespace PCFW

#endif // _PCFW_HPP