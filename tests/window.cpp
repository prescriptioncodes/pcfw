// Author: oknauta
// License:
// File: window.c
// Date: 2024-11-18

#include <GL/glew.h>
#include <iostream>
#include <pcfw/pcfw.hpp>

void callback(int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(void)
{
    PCFW::window *window = PCFW::createWindow(800, 600, "GL");

    PCFW::makeContextCurrent(window);

    PCFW::setFramebufferSizeCallback(window, callback);

    PCFW::setSwapInterval(window, 1);

    glewInit();

    while (!PCFW::windowShouldClose(window))
    {
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