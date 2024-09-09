build:
	gcc ./src/tests/main.c -I ./src/ ./src/idgui/idgui.c -lX11

build_windows:
	gcc ./src/tests/main.c -I ./src/ ./src/idgui/idgui.c -mwindows