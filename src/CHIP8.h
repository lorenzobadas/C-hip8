#pragma once

#include "STRUCT_DECL.h"
#include "GUI.h"

void initialize_chip8(Chip8*);
uint8_t* create_registers();
uint8_t* create_memory();
uint16_t* create_stack();
uint8_t* create_keypad();
uint32_t* create_video();
void load_rom(Chip8*, char*);
void load_default_rom(Chip8*);
void load_fontset(Chip8*);
void free_alloc_chip8(Chip8 *chip8);

void OP_00E0(Chip8*);
void OP_00EE(Chip8*);
void OP_1nnn(Chip8*);
void OP_2nnn(Chip8*);
void OP_3xkk(Chip8*);
void OP_4xkk(Chip8*);
void OP_5xy0(Chip8*);
void OP_6xkk(Chip8*);
void OP_7xkk(Chip8*);
void OP_8xy0(Chip8*);
void OP_8xy1(Chip8*);
void OP_8xy2(Chip8*);
void OP_8xy3(Chip8*);
void OP_8xy4(Chip8*);
void OP_8xy5(Chip8*);
void OP_8xy6(Chip8*);
void OP_8xy7(Chip8*);
void OP_8xyE(Chip8*);
void OP_9xy0(Chip8*);
void OP_Annn(Chip8*);
void OP_Bnnn(Chip8*);
void OP_Cxkk(Chip8*);
void OP_Dxyn(Chip8*);
void OP_Ex9E(Chip8*);
void OP_ExA1(Chip8*);
void OP_Fx07(Chip8*);
void OP_Fx0A(Chip8*);
void OP_Fx15(Chip8*);
void OP_Fx18(Chip8*);
void OP_Fx1E(Chip8*);
void OP_Fx29(Chip8*);
void OP_Fx33(Chip8*);
void OP_Fx55(Chip8*);
void OP_Fx65(Chip8*);

void Table0(Chip8*);
void Table8(Chip8*);
void TableE(Chip8*);
void TableF(Chip8*);
void call_OP(Chip8 *chip8);
void cycle(Chip8*);

void run(Chip8*, GUI*, uint32_t);