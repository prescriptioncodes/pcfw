build:
	gcc ./src/tests/main.c -I ./src/ ./src/idgui/*.c -lX11 -lGL

build_windows:
	gcc ./src/tests/main.c -I ./src/ ./src/idgui/idgui.c -mwindows