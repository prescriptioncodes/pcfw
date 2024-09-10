// File: main.c
// Date: 2024-09-09

#include <GL/glew.h>
#include <idgui/idgui.h>

int main()
{
	ID_window window = ID_createWindow("ID window", 800, 600, 10, 10);
	
	while(!ID_shouldClose(window))
	{
		// ID_clear(GL_COLOR_BUFFER_BIT);
		ID_pollEvents(window);
	}
	
	ID_destroyWindow(window);
	
	return NO_ERROR;
}
