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

    /**
     * @brief Gets a key from keyboard or mouse
     * @param window What that will receive the input
     * @param key It can be `KEY_A` until `KEY_Z`
     * @param type If it will be `KEY_RELEASE` or `KEY_PRESS`
     * @return A key pressed or released
     */
    PCFW_API int getKey(window *window, int key, int type);

    /**
     * @brief Creates a window with predefined size and title
     * @param width The width in `int`
     * @param height The height in `int`
     * @param title The title in `const char *`
     * @return A created window
     */
    PCFW_API window *createWindow(int width, int height, const char *title);

    /**
     * @brief Gets the width of a window
     * @param window What that will get the width
     * @return A width of a window in `int`
     */
    PCFW_API int getWindowWidth(window *window);

    /**
     * @brief Gets the height of a window
     * @param window What that will get the height
     * @return A height of a window in `int`
     */
    PCFW_API int getWindowHeight(window *window);

    /**
     * @brief Destroys a window
     * @param window What that will destroyed
     * @return A destroyed window
     */
    PCFW_API void destroyWindow(window *window);

    /**
     * @brief Swap frame on screen
     * @param window What that will swap the frame
     * @return A swapped frame
     */
    PCFW_API void swapBuffers(window *window);

    /**
     * @brief Makes the context of a window
     * @param window What that will make the context
     * @return A made context
     */
    PCFW_API void makeCurrentContext(window *window);

    /**
     * @brief Poll the events
     * @param window What that will poll the events
     * @return A polled events at window
     */
    PCFW_API void pollEvents(window *window);

    /**
     * @brief Sets the framebuffer size callback
     * @param window What that will receive the callback
     * @param callback The callback
     * @return A callbacked window
     */
    PCFW_API void setFramebufferSizeCallback(window *window, framebuffer_size_callback callback);

    /**
     * @brief Sets the swap interval of a window
     * @param window What that will be intervaled
     * @param interval The interval. It can be `1` for V-Sync or `0` for unlimited FPS
     * @return A setted swap interval
     */
    PCFW_API void setSwapInterval(window *window, int interval);

    /**
     * @brief Detects if a window should close
     * @param window What will be detected
     * @return If a window should should close
     */
    PCFW_API bool windowShouldClose(window *window);

    /**
     * @brief Gets the title of a window
     * @param window What will be got the title
     */
    PCFW_API const char *getWindowTitle(window *window);

#ifdef _WIN32

    const int KEY_PRESS = 0;
    const int KEY_RELEASE = 1;

    const int KEY_A = 0x41;
    const int KEY_B = 0x42;
    const int KEY_C = 0x43;
    const int KEY_D = 0x44;
    const int KEY_E = 0x45;
    const int KEY_F = 0x46;
    const int KEY_G = 0x47;
    const int KEY_H = 0x48;
    const int KEY_I = 0x49;
    const int KEY_J = 0x4A;
    const int KEY_K = 0x4B;
    const int KEY_L = 0x4C;
    const int KEY_M = 0x4D;
    const int KEY_N = 0x4E;
    const int KEY_O = 0x4F;
    const int KEY_P = 0x50;
    const int KEY_Q = 0x51;
    const int KEY_R = 0x52;
    const int KEY_S = 0x53;
    const int KEY_T = 0x54;
    const int KEY_U = 0x55;
    const int KEY_V = 0x56;
    const int KEY_W = 0x57;
    const int KEY_X = 0x58;
    const int KEY_Y = 0x59;
    const int KEY_Z = 0x5A;

#elif __linux__

    const int KEY_PRESS = 0;
    const int KEY_RELEASE = 1;

    const int KEY_A = 38;
    const int KEY_B = 56;
    const int KEY_C = 54;
    const int KEY_D = 40;
    const int KEY_E = 26;
    const int KEY_F = 41;
    const int KEY_G = 42;
    const int KEY_H = 43;
    const int KEY_I = 31;
    const int KEY_J = 44;
    const int KEY_K = 45;
    const int KEY_L = 46;
    const int KEY_M = 58;
    const int KEY_N = 57;
    const int KEY_O = 32;
    const int KEY_P = 33;
    const int KEY_Q = 24;
    const int KEY_R = 27;
    const int KEY_S = 39;
    const int KEY_T = 28;
    const int KEY_U = 30;
    const int KEY_V = 55;
    const int KEY_W = 25;
    const int KEY_X = 53;
    const int KEY_Y = 29;
    const int KEY_Z = 52;

#endif

} // namespace PCFW

#endif // _PCFW_HPP