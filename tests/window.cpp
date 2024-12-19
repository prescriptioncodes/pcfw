// Author: oknauta
// License:
// File: window.c
// Date: 2024-11-18

#include <GL/glew.h>
#include <iostream>
#include <pcfw/pcfw.hpp>
#include <pclog/pclog.hpp>

void callback(int w, int h)
{
    glViewport(0, 0, w, h);
}

void mouse_callback(int mouse_button, int status, int mods)
{
    if(mouse_button == PCFW::MOUSE_LEFT_BUTTON && status == PCFW::MOUSE_PRESS_BUTTON)
    {
        PCLOG::info("Left mouse button pressed");
        return;
    }
    if(mouse_button == PCFW::MOUSE_RIGHT_BUTTON && status == PCFW::MOUSE_PRESS_BUTTON)
    {
        PCLOG::info("Right mouse button pressed");
        return;
    }
    
    if(mouse_button == PCFW::MOUSE_MIDDLE_BUTTON && status == PCFW::MOUSE_PRESS_BUTTON)
    {
        PCLOG::info("Middle mouse button pressed");
        return;
    }
}

int main(void)
{
    PCFW::window *window = PCFW::createWindow(800, 600, "GL");

    PCFW::makeContextCurrent(window);

    PCFW::setFramebufferSizeCallback(window, callback);

    PCFW::setMouseCallback(window, mouse_callback);

    PCFW::setSwapInterval(window, 1);

    glewInit();

    int cursor_x, cursor_y;

    while (!PCFW::windowShouldClose(window))
    {
        // PCFW::getCursorPosition(window, &cursor_x, &cursor_y);
        // std::cout << cursor_x << "x" << cursor_y << "\n";

        if (PCFW::getKey(window, PCFW::KEY_A, PCFW::KEY_PRESS))
            std::cout << "A pressed!\n";
        if (PCFW::getKey(window, PCFW::KEY_0, PCFW::KEY_PRESS))
            std::cout << "0 pressed!\n";
        if (PCFW::getKey(window, PCFW::KEY_F1, PCFW::KEY_PRESS))
            std::cout << "F1 pressed!\n";

        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);

        glVertex2f(0.0f, 1.0f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);

        glEnd();

        PCFW::swapBuffers(window);
        PCFW::pollEvents(window);
    }

    return 0;
}