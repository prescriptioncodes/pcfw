// Author: oknauta
// License:
// File: window.c
// Date: 2024-11-18

#include <GL/gl.h>
#include <iostream>
#include <pcfw.hpp>

void callback(int w, int h)
{
    
}

int main(void)
{
    PCFW::window *window = PCFW::createWindow(800, 600, "GL");

    PCFW::setFramebufferSizeCallback(window, callback);

    while (!PCFW::windowShouldClose(window))
    {
        PCFW::pollEvents(window);
    }

    return 0;
}