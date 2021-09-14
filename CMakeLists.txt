cmake_minimum_required(VERSION 3.18)
project(c-hip8 C)

set(CMAKE_C_STANDARD 11)

add_executable(c-hip8 src/main.c src/GUI.h src/GUI.c src/CHIP8.c src/STRUCT_DECL.h src/MACROS.h)

target_link_libraries(c-hip8 SDL2)