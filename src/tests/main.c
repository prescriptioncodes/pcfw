// File: main.c
// Date: 2024-09-09

#include <idgui/idgui.h>

int main()
{
	ID_window window = ID_createWindow("ID window", 800, 600);
	ID_destroyWindow(window);
	
	return NO_ERROR;
}
