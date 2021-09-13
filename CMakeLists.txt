cmake_minimum_required(VERSION 3.18)
project(chip8_emulator C)

set(CMAKE_C_STANDARD 11)

add_executable(chip8_emulator src/main.c src/GUI.h src/GUI.c src/CHIP8.c src/STRUCT_DECL.h src/MACROS.h)

target_link_libraries(chip8_emulator SDL2)
