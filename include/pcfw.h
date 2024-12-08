// Author: oknauta
// License: Unknown
// File: pcfw.h
// Date: 2024-11-18

#ifndef _IDFW_H
#define _IDFW_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void (*framebuffer_size_callback)(int width, int height);

    typedef struct _PC_window PC_window;

    PC_window *PC_createWindow(int width, int height, const char *title);

    void PC_destroyWindow(PC_window *window);
    void PC_swapBuffers(PC_window *window);
    void PC_makeCurrentContext(PC_window *window);
    void PC_pollEvents(PC_window *window);
    void PC_setFramebufferSizeCallback(PC_window *window, framebuffer_size_callback callback);
    void PC_setSwapInterval(PC_window *window, int interval);
    int PC_windowShouldClose(PC_window *window);
    const char *PC_getWindowTitle(PC_window *window);

#ifdef __cplusplus
}
#endif

#endif // _IDFW_H