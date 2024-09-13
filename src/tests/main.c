// File: main.c
// Date: 2024-09-09

#include <idgui/idgui.h>

int main()
{
	window *window = createWindow("IDGUI window", 800, 600, 0, 0);
	while(!windowShouldClose(window));
	return NO_ERROR;
}