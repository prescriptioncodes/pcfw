// Author: oknauta
// License: MIT
// File: window.cpp
// Date: 2024-12-10

#include "pc/framework.hpp"
#include "pc/framework_internal.hpp"
#include <pc/log.hpp>

namespace PC::Framework
{
	int set_key_callback(window *window, key_callback callback)
	{
		if (!window)
		{
			Log::warning("No window to set key callback");
			return 1;
		}

		if (!callback)
		{
			Log::warning("No key callback to set into the window");
			return 1;
		}

		if (INTERNAL_set_key_callback(window, callback) != 0)
		{
			return 1;
		}

		return 0;
	}

	int set_framebuffer_size_callback(window *window, framebuffer_size_callback callback)
	{
		if (!window)
		{
			PC::Log::warning("No window to set framebuffer size callback");
			return 1;
		}
		
		if (!callback)
		{
			PC::Log::warning("No framebuffer size callback to set into the window");
			return 1;
		}

		if (INTERNAL_set_framebuffer_size_callback(window, callback) != 0)
		{
			return 1;
		}

		return 0;
	};

	int set_mouse_callback(window *window, mouse_callback callback)
	{
		if (!window)
		{
	    		PC::Log::warning("No window to set mouse callback");
	    		return 1;
		}

		if (!callback)
		{
			PC::Log::warning("No mouse callback setted");
	    		return 1;
		}

		if (INTERNAL_set_mouse_callback(window, callback) != 0)
		{
	    		return 1;
		}

		return 0;
	};

	void *get_proc_address(const char *proc)
	{
		return INTERNAL_get_proc_address(proc);
	}

	int set_window_limits(window *window, int minimum_width, int minimum_height, int maximum_width, int maximum_height)
	{
		if (!window)
		{
			PC::Log::warning("No window to set limits");
			return 1;
		}

		INTERNAL_set_window_limits(window, minimum_width, minimum_height, maximum_width, maximum_height);

		return 0;
	}

	int make_context_current(window *window)
	{
		if (!window)
		{
			PC::Log::warning("No window to make context current");
			return 1;
		}

		if (INTERNAL_make_context_current(window))
		{
			return 1;
		}

		return 0;
	}

	void swap_buffers(window *window)
	{
		if (!window)
		{
			PC::Log::warning("No window to swap buffer");
			return;
		}

		INTERNAL_swap_buffers(window);
	}

	void set_swap_interval(window *window, int interval)
	{
		if (!window)
		{
			PC::Log::warning("No window to set swap interval");
			return;
		}
		INTERNAL_set_swap_interval(window, interval);
	}

	int window_should_close(window *window)
	{
		if (!window)
		{
			PC::Log::warning("No window shall close");
			return true;
		}
		return INTERNAL_window_should_close(window);
	}

	void poll_events(window *window)
	{
		if (!window)
		{
			PC::Log::warning("No window to poll events");
			return;
		}
		INTERNAL_poll_events(window);
	}

	window *create_window(int width, int height, const char *title)
	{
		window *_window = new window;
		if (!_window)
		{
			PC::Log::error("Failed to allocate memory to the window");
			return nullptr;
		}

		_window->config._should_close = false;
		_window->config._title = title;
		_window->config._width = width;
		_window->config._height = height;

		if (INTERNAL_create_window(_window))
		{
		    delete _window;
		    return nullptr;
		}

		return _window;
	}

	int destroy_window(window *window)
	{
		if (window == nullptr)
		{
			PC::Log::warning("No window to destroy");
			return 1;
		}
		INTERNAL_destroy_window(window);
		delete window;
		return 0;
	}
} // namespace PCFW
