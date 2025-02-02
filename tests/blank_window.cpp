// License: MIT

#include "pc/framework.hpp"

int main(void)
{
	// Creating a window:
	// We make a variable
	// And we call a function that is "create_window"
	// This creates and shows a window
	PC::Framework::window *window = PC::Framework::create_window(800, 600, "GL Blank Window");
	// This makes the context of a window
	// Consult: https://www.khronos.org/opengl/wiki/OpenGL_Context for more information
	PC::Framework::make_context_current(window);
	// And here we swap the frames to show something
	PC::Framework::swap_buffers(window);
	

	// While the window shoudn't close...
		while (!PC::Framework::window_should_close(window))
	{
		// Polls the events
		PC::Framework::poll_events(window);
	}

	return 0;
}
