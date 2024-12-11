// Author: oknauta
// License: MIT
// File: pcfw.hpp
// Date: 2024-11-18

#ifndef _PCFW_HPP
#define _PCFW_HPP

#ifdef _WIN32
#ifdef PCFW_EXPORTS
#define PCFW_API
#else
#define PCFW_API
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

} // namespace PCFW

#endif // _PCFW_HPP