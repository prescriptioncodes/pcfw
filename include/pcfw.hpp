// Author: oknauta
// License: MIT
// File: pcfw.hpp
// Date: 2024-11-18

#ifndef _PCFW_HPP
#define _PCFW_HPP

namespace PCFW
{
    typedef void (*framebuffer_size_callback)(int width, int height);
    typedef struct window window;

    window *createWindow(int width, int height, const char *title);
    int getWindowWidth(window *window);
    int getWindowHeight(window *window);
    void destroyWindow(window *window);
    void swapBuffers(window *window);
    void makeCurrentContext(window *window);
    void pollEvents(window *window);
    void setFramebufferSizeCallback(window *window, framebuffer_size_callback callback);
    void setSwapInterval(window *window, int interval);
    int windowShouldClose(window *window);
    const char *getWindowTitle(window *window);

} // namespace PCFW

#endif // _PCFW_HPP