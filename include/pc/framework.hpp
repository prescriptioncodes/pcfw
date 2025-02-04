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

namespace PC::Framework
{
	typedef struct window window;
	typedef void (*framebuffer_size_callback)(window *window, int width, int height);
	typedef void (*mouse_callback)(int mouse_button, int status, int mods);
	typedef void *(*proc)(const char *name);
	typedef void (*key_callback)(int key, int scancode, int action, int mods);
	int set_key_callback(window* window, key_callback callback);

	// Gets the OpenGL proc address
	PCFW_API void *get_proc_address(const char *proc);

    // Sets the window limits. e.g., the minimum resolution
    PCFW_API int set_window_limits(window *window, int minimum_width, int minimum_height, int maximum_width, int maximum_height);

    /**
     * @brief Gets a key from keyboard or mouse
     * @param window What that will receive the input
     * @param key It can be `KEY_A` until `KEY_Z`
     * @param type If it will be `KEY_RELEASE` or `KEY_PRESS`
     * @return A key pressed or released
     */
    PCFW_API int get_key(window *window, int key, int type);

    /**
     * @brief Creates a window with predefined size and title
     * @param width The width in `int`
     * @param height The height in `int`
     * @param title The title in `const char *`
     * @return A created window
     */
    PCFW_API window *create_window(int width, int height, const char *title);

    /**
     * @brief Gets the width of a window
     * @param window What that will get the width
     * @return A width of a window in `int`
     */
    PCFW_API int get_window_width(window *window);

    /**
     * @brief Gets the height of a window
     * @param window What that will get the height
     * @return A height of a window in `int`
     */
    PCFW_API int get_window_height(window *window);

    /**
     * @brief Destroys a window
     * @param window What that will destroyed
     * @return A destroyed window
     */
    PCFW_API int destroy_window(window *window);

    /**
     * @brief Swap frame on screen
     * @param window What that will swap the frame
     * @return A swapped frame
     */
    PCFW_API void swap_buffers(window *window);

    /**
     * @brief Makes the context of a window
     * @param window What that will make the context
     * @return A made context
     */
    PCFW_API int make_context_current(window *window);

    /**
     * @brief Poll the events
     * @param window What that will poll the events
     * @return A polled events at window
     */
    PCFW_API void poll_events(window *window);

    /**
     * @brief Sets the framebuffer size callback
     * @param window What that will receive the callback
     * @param callback The callback
     * @return A callbacked window
     */
    PCFW_API int set_framebuffer_size_callback(window *window, framebuffer_size_callback callback);

    /**
     * @brief Sets the mouse callback
     * @param window What will receive the callback
     * @param callback The callback
     */
    PCFW_API int set_mouse_callback(window *window, mouse_callback callback);

    /**
     * @brief Sets the swap interval of a window
     * @param window What that will be intervaled
     * @param interval The interval. It can be `1` for V-Sync or `0` for unlimited FPS
     * @return A setted swap interval
     */
    PCFW_API void set_swap_interval(window *window, int interval);

    /**
     * @brief Detects if a window should close
     * @param window What will be detected
     * @return If a window should should close
     */
    PCFW_API int window_should_close(window *window);

    /**
     * @brief Gets the title of a window
     * @param window What will be got the title
     */
    PCFW_API const char *get_window_title(window *window);

    /**
     * @brief Gets the cursor position
     * @param window What will be get from
     * @param x The variable that the value will be storaged the horizontal position
     * @param y The variable that the value will be storaged the vertical position
     */
    PCFW_API int get_cursor_position(window *window, int *x, int *y);

    // Constants
    
    constexpr int DONT_CARE = -1;

    // Keys

    constexpr int KEY_PRESS = 0;
    constexpr int KEY_RELEASE = 1;

    extern PCFW_API const int MOUSE_LEFT_BUTTON;
    extern PCFW_API const int MOUSE_RIGHT_BUTTON;
    extern PCFW_API const int MOUSE_MIDDLE_BUTTON;

    extern PCFW_API const int MOUSE_PRESS_BUTTON;
    extern PCFW_API const int MOUSE_RELEASE_BUTTON;

    extern PCFW_API const int KEY_LEFT_SHIFT;
    extern PCFW_API const int KEY_RIGHT_SHIFT;

    extern PCFW_API const int KEY_LEFT_CONTROL;
    extern PCFW_API const int KEY_RIGHT_CONTROL;

    extern PCFW_API const int KEY_LEFT_ALT;
    extern PCFW_API const int KEY_RIGHT_ALT;

    extern PCFW_API const int KEY_F1;
    extern PCFW_API const int KEY_F2;
    extern PCFW_API const int KEY_F3;
    extern PCFW_API const int KEY_F4;
    extern PCFW_API const int KEY_F5;
    extern PCFW_API const int KEY_F6;
    extern PCFW_API const int KEY_F7;
    extern PCFW_API const int KEY_F8;
    extern PCFW_API const int KEY_F9;
    extern PCFW_API const int KEY_F10;
    extern PCFW_API const int KEY_F11;
    extern PCFW_API const int KEY_F12;

    extern PCFW_API const int KEY_0;
    extern PCFW_API const int KEY_1;
    extern PCFW_API const int KEY_2;
    extern PCFW_API const int KEY_3;
    extern PCFW_API const int KEY_4;
    extern PCFW_API const int KEY_5;
    extern PCFW_API const int KEY_6;
    extern PCFW_API const int KEY_7;
    extern PCFW_API const int KEY_8;
    extern PCFW_API const int KEY_9;

    extern PCFW_API const int KEY_A;
    extern PCFW_API const int KEY_B;
    extern PCFW_API const int KEY_C;
    extern PCFW_API const int KEY_D;
    extern PCFW_API const int KEY_E;
    extern PCFW_API const int KEY_F;
    extern PCFW_API const int KEY_G;
    extern PCFW_API const int KEY_H;
    extern PCFW_API const int KEY_I;
    extern PCFW_API const int KEY_J;
    extern PCFW_API const int KEY_K;
    extern PCFW_API const int KEY_L;
    extern PCFW_API const int KEY_M;
    extern PCFW_API const int KEY_N;
    extern PCFW_API const int KEY_O;
    extern PCFW_API const int KEY_P;
    extern PCFW_API const int KEY_Q;
    extern PCFW_API const int KEY_R;
    extern PCFW_API const int KEY_S;
    extern PCFW_API const int KEY_T;
    extern PCFW_API const int KEY_U;
    extern PCFW_API const int KEY_V;
    extern PCFW_API const int KEY_W;
    extern PCFW_API const int KEY_X;
    extern PCFW_API const int KEY_Y;
    extern PCFW_API const int KEY_Z;

} // namespace PC

#endif // _PCFW_HPP
