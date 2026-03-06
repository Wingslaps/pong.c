@echo off
@REM gcc ray_test.c -o ray_test.exe -lraylib -lopengl32 -lgdi32 -lwinmm
gcc *.c -o pong.exe -Wall -Wextra -static -lraylib -lopengl32 -lgdi32 -lwinmm