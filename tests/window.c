// Author: oknauta
// License:
// File: window.c
// Date: 2024-11-18

#include <GL/gl.h>
#include <pcfw.h>

#include <stdio.h>

void framebufferSizeCallback(int width, int height)
{
    printf("%dx%d\n", width, height);
    
    glViewport(0, 0, width, height);
}

int main(void)
{
    PC_window *window = PC_createWindow(800, 600, "IDFW OpenGL");

    PC_makeCurrentContext(window);

    PC_setFramebufferSizeCallback(window, framebufferSizeCallback);

    PC_setSwapInterval(window, 1);

    while (!PC_windowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 1.0f);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f(1.0f, -1.0f);
        glEnd();

        PC_swapBuffers(window);

        PC_pollEvents(window);
    }

    PC_destroyWindow(window);

    return 0;
}