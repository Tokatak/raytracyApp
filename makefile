gcc:
	gcc -Wall -Wextra -Wpedantic -DRENDERER_EXTERNAL -g -o main ./win/main.c ./integration.c -lgdi32 -lwinmm

gccO:
	gcc -Wall -Wextra -Wpedantic -DRENDERER_EXTERNAL -g -O3 -o main ./win/main.c ./integration.c -lgdi32 -lwinmm

msvc:
	cl /Zi /DEBUG /DRENDERER_EXTERNAL /EHsc ./win/main.c ./integration.c user32.lib gdi32.lib && main.exe
msvcO:
	cl /Zi /O2 /DRENDERER_EXTERNAL /EHsc ./win/main.c ./integration.c user32.lib gdi32.lib && main.exe

