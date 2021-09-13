#pragma once

#include "STRUCT_DECL.h"
#include "CHIP8.h"

void initialize_gui(GUI*, uint8_t);
void free_alloc_SDL(GUI*);

uint8_t handle_key_input(SDL_Event, Chip8*);
uint8_t process_input(Chip8*);
void update_gui(GUI*, Chip8*);