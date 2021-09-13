#pragma once

#include "MACROS.h"

typedef struct{
    /*
     * 16 8-bit registers
     * 4 KiB memory
     * 16-bit Program Counter
     * 16 levels Stack
     * 8-bit Stack Pointer
     * 8-bit Delay Timer
     * 8-bit Sound Timer
     * 16 Input Keys
     * 64*32 8-bit Monochrome Display Memory
     */

    uint8_t* registers;
    uint8_t* memory;
    uint16_t index;
    uint16_t program_counter;
    uint16_t* stack;
    uint8_t stack_pointer;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t* keypad;
    uint32_t* video;
    uint16_t opcode;
} Chip8;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} GUI;