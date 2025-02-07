Para gerar o executável:

windres -i resource.rc -o resource.o

gcc main.c resource.o -o mouse_mover.exe -lgdi32 -mwindows