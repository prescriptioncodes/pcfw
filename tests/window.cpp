// Author: oknauta
// License:
// File: window.c
// Date: 2024-11-18

#include <glad/glad.h>
#include <iostream>
#include <pc/framework.hpp>
#include <pc/log.hpp>

void callback(PC::window *window, int w, int h)
{
    glViewport(0, 0, w, h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    PC::swap_buffers(window);
}

void mouse_callback(int mouse_button, int status, int mods)
{
    if (mouse_button == PC::MOUSE_LEFT_BUTTON && status == PC::MOUSE_PRESS_BUTTON)
    {
        PC::Log::info("Left mouse button pressed");
        return;
    }
    if (mouse_button == PC::MOUSE_RIGHT_BUTTON && status == PC::MOUSE_PRESS_BUTTON)
    {
        PC::Log::info("Right mouse button pressed");
        return;
    }

    if (mouse_button == PC::MOUSE_MIDDLE_BUTTON && status == PC::MOUSE_PRESS_BUTTON)
    {
        PC::Log::info("Middle mouse button pressed");
        return;
    }
}

int main(void)
{
    PC::Log::info("Hello, world!");

    PC::window *window = PC::create_window(800, 600, "GL");

    PC::make_context_current(window);

    PC::set_framebuffer_size_callback(window, callback);

    PC::set_mouse_callback(window, mouse_callback);

    PC::set_swap_interval(window, 1);

    PC::set_window_limits(window, 300, 300, PC::DONT_CARE, PC::DONT_CARE);

    if (!gladLoadGLLoader(PC::get_proc_address))
    {
        PC::Log::error("Failed to load glad");
        return 1;
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    PC::swap_buffers(window);

    int cursor_x, cursor_y;

    while (!PC::window_should_close(window))
    {
        PC::poll_events(window);

        // PCFW::getCursorPosition(window, &cursor_x, &cursor_y);
        // std::cout << cursor_x << "x" << cursor_y << "\n";

        if (PC::get_key(window, PC::KEY_A, PC::KEY_PRESS))
            std::cout << "A pressed!\n";
        if (PC::get_key(window, PC::KEY_0, PC::KEY_PRESS))
            std::cout << "0 pressed!\n";
        if (PC::get_key(window, PC::KEY_F1, PC::KEY_PRESS))
            std::cout << "F1 pressed!\n";

        // glBegin(GL_TRIANGLES);

        // glVertex2f(0.0f, 1.0f);
        // glVertex2f(-1.0f, -1.0f);
        // glVertex2f(1.0f, -1.0f);

        // glEnd();
    }

    return 0;
}