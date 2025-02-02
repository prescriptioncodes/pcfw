// Author: oknauta
// License:
// File: window.cpp
// Date: 2024-11-18

#include <glad/glad.h>
#include <pc/framework.hpp>
#include <pc/log.hpp>

GLuint vertex_buffer;

void init_triangle()
{
	GLfloat vertices[] =
	{
		0.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f
	};

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void render_triangle()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void cleanup()
{
	glDeleteBuffers(1, &vertex_buffer);
}

void framebuffer_size_callback(PC::Framework::window *window, int w, int h)
{
	glViewport(0, 0, w, h);
	PC::Log::info("%dx%d", w, h);
}

void mouse_callback(int mouse_button, int status, int mods)
{
	if (status == PC::Framework::MOUSE_PRESS_BUTTON)
	{
		PC::Log::debug("Pressed a button!");
	}
}

int main()
{
    PC::Log::info("Hello, world!");

    PC::Framework::window *window = PC::Framework::create_window(800, 600, "GL");

    if (!window)
    {
        PC::Log::error("Failed to create window");
        return 1;
    }

    PC::Framework::make_context_current(window);
    PC::Framework::set_framebuffer_size_callback(window, framebuffer_size_callback);
    PC::Framework::set_mouse_callback(window, mouse_callback);
    PC::Framework::set_swap_interval(window, 1);
    PC::Framework::set_window_limits(window, 300, 300, PC::Framework::DONT_CARE, PC::Framework::DONT_CARE);

    if (!gladLoadGLLoader(PC::Framework::get_proc_address))
    {
        PC::Log::error("Failed to load glad");
        PC::Framework::destroy_window(window);
        return 1;
    }

    init_triangle();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    while (!PC::Framework::window_should_close(window))
    {
        PC::Framework::poll_events(window);

        glClear(GL_COLOR_BUFFER_BIT);
        render_triangle();
        
        PC::Framework::swap_buffers(window);
    }

    cleanup();
    PC::Framework::destroy_window(window);

    return 0;
}
