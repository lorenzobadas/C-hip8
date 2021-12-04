#include "CHIP8.h"

void initialize_chip8(Chip8* chip8) {
    chip8->registers = create_registers();
    chip8->memory = create_memory();
    chip8->index = 0;
    chip8->program_counter = START_ADDRESS_ROM;
    chip8->stack = create_stack();
    chip8->stack_pointer = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    chip8->keypad = create_keypad();
    chip8->video = create_video();
    chip8->opcode = 0;
}

uint8_t* create_registers() {
    uint8_t* registers = (uint8_t*) calloc(16, sizeof(char));
    return registers;
}

uint8_t * create_memory() {
    uint8_t* memory = (uint8_t*) calloc(4096, sizeof(char));
    return memory;
}

uint16_t* create_stack() {
    uint16_t* stack = (uint16_t*) calloc(16, sizeof(uint16_t));
    return stack;
}

uint8_t* create_keypad() {
    uint8_t* keypad = (uint8_t*) calloc(16, sizeof(char));
    return keypad;
}

uint32_t* create_video() {
    uint32_t* video = (uint32_t*) calloc(64*32, sizeof(int));
    return video;
}

void load_rom(Chip8* chip8, char* filename) {
    FILE* fptr = fopen(filename, "rb");

    if (fptr == NULL) {
        perror("ERROR: File not found while loading ROM\n");
        exit(-1);
    }

    int i = 0;
    while ( !feof(fptr) && i < (0x1000 - START_ADDRESS_ROM) ) {
        chip8->memory[START_ADDRESS_ROM + i] = getc(fptr);
        i++;
    }
    i--;
    fclose(fptr);

    if (i == (0x1000 - START_ADDRESS_ROM)) {
        perror("ERROR: ROM file is too big!\n");
        exit(-1);
    }

    chip8->memory[START_ADDRESS_ROM + i] = 0;
}

void load_default_rom(Chip8 *chip8) {
    char default_rom[] = "00E0A2486000611E6200D202D21272083240120A6000613E6202A24AD02ED12E720ED02ED12EA258600B6108D01F700AA267D01F700AA276D01F7003A285D01F700AA294D01F1246FFFFC0C0C0C0C0C0C0C0C0C0C0C0C0C0FF80808080808080808080808080FF81818181818181FF81818181818181808080808080808080808080808080FF818181818181FF80808080808080FF818181818181FF818181818181FFFF";
    char buf[3];
    buf[2] = '\0';

    for (uint32_t i = 0; i < strlen(default_rom); ++i) {
        buf[0] = default_rom[i * 2];
        buf[1] = default_rom[i * 2 + 1];
        chip8->memory[START_ADDRESS_ROM + i] = strtol(buf, NULL, 16);
    }
}

void load_fontset(Chip8* chip8) {
    uint8_t fontset[FONTSET_SIZE] =
            {
                    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };

    for (int i = 0; i < FONTSET_SIZE; ++i) {
        chip8->memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }
}

void free_alloc_chip8(Chip8* chip8) {
    free(chip8->registers);
    free(chip8->memory);
    free(chip8->stack);
    free(chip8->keypad);
    free(chip8->video);
}

void OP_00E0(Chip8* chip8) {
    // CLS - Clear the Display

    memset(chip8->video, 0, sizeof(uint32_t) * 64 * 32);
}

void OP_00EE(Chip8* chip8) {
    // RET - Return from a subroutine

    chip8->stack_pointer -= 1;
    chip8->program_counter = chip8->stack[chip8->stack_pointer];
}

void OP_1nnn(Chip8* chip8) {
    // JP addr - Jump to location nnn

    uint16_t address = chip8->opcode & 0x0FFF;
    chip8->program_counter = address;
}

void OP_2nnn(Chip8* chip8) {
    // CALL addr - Call subroutine at nnn

    uint16_t address = chip8->opcode & 0x0FFF;
    chip8->stack[chip8->stack_pointer] = chip8->program_counter;
    chip8->stack_pointer += 1;
    chip8->program_counter = address;
}

void OP_3xkk(Chip8* chip8) {
    // SE Vx, byte - Skip next instruction if Vx == kk

    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
    uint8_t kk = chip8->opcode & 0x00FF;
    if (chip8->registers[Vx] == kk) {
        chip8->program_counter += 2;
    }
}

void OP_4xkk(Chip8* chip8) {
    // SNE Vx, byte - Skip next instruction if Vx != kk

    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
    uint8_t kk = chip8->opcode & 0x00FF;
    if (chip8->registers[Vx] != kk) {
        chip8->program_counter += 2;
    }
}

void OP_5xy0(Chip8* chip8) {
    // SE Vx, Vy - Skip next instruction if Vx == Vy

    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;
    if (chip8->registers[Vx] == chip8->registers[Vy]) {
        chip8->program_counter += 2;
    }
}

void OP_6xkk(Chip8* chip8) {
    // LD Vx, byte - Set Vx = kk

    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
    uint8_t kk = chip8->opcode & 0x00FF;
    chip8->registers[Vx] = kk;
}

void OP_7xkk(Chip8* chip8) {
    // ADD Vx, byte - Set Vx = Vx + kk

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t kk = chip8->opcode & 0xFF;
    chip8->registers[Vx] += kk;
}

void OP_8xy0(Chip8* chip8) {
    // LD Vx, Vy - Set Vx = Vy

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t Vy = (chip8->opcode & 0xF0) >> 4;
    chip8->registers[Vx] = chip8->registers[Vy];
}

void OP_8xy1(Chip8* chip8) {
    // OR Vx, Vy - Set Vx = Vx | Vy

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t Vy = (chip8->opcode & 0xF0) >> 4;
    chip8->registers[Vx] |= chip8->registers[Vy];
}

void OP_8xy2(Chip8* chip8) {
    // AND Vx, Vy - Set Vx = Vx & Vy

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t Vy = (chip8->opcode & 0xF0) >> 4;
    chip8->registers[Vx] &= chip8->registers[Vy];
}

void OP_8xy3(Chip8* chip8) {
    // XOR Vx, Vy - Set Vx = Vx ^ Vy

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t Vy = (chip8->opcode & 0xF0) >> 4;
    chip8->registers[Vx] ^= chip8->registers[Vy];
}

void OP_8xy4(Chip8* chip8) {
    // ADD Vx, Vy - Set Vx = Vx + Vy, Set VF = carry

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t Vy = (chip8->opcode & 0xF0) >> 4;

    uint16_t sum = chip8->registers[Vx] + chip8->registers[Vy];

    chip8->registers[0xF] = sum > 0xFF;

    chip8->registers[Vx] = sum & 0xFF;
}

void OP_8xy5(Chip8* chip8) {
    // SUB Vx, Vy - Set Vx = Vx - Vy, Set VF = NOT borrow

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t Vy = (chip8->opcode & 0xF0) >> 4;

    chip8->registers[0xF] = chip8->registers[Vx] > chip8->registers[Vy];

    chip8->registers[Vx] -= chip8->registers[Vy];
}

void OP_8xy6(Chip8* chip8) {
    // SHR Vx - Set Vx = Vx SHR 1

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    chip8->registers[0xF] = chip8->registers[Vx] & 0x1;
    chip8->registers[Vx] >>= 1;
}

void OP_8xy7(Chip8* chip8) {
    // SUBN Vx, Vy - Set Vx = Vy - Vx,Set VF = NOT borrow

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t Vy = (chip8->opcode & 0xF0) >> 4;

    chip8->registers[0xF] = chip8->registers[Vy] > chip8->registers[Vx];

    chip8->registers[Vx] = chip8->registers[Vy] - chip8->registers[Vx];
}

void OP_8xyE(Chip8* chip8) {
    // Set Vx = Vx SHL 1

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;

    chip8->registers[Vx] <<= 1;
}

void OP_9xy0(Chip8* chip8) {
    // SNE Vx, Vy - Skip next instruction if Vx != Vy

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t Vy = (chip8->opcode & 0xF0) >> 4;

    if (chip8->registers[Vx] != chip8->registers[Vy]) {
        chip8->program_counter += 2;
    }
}

void OP_Annn(Chip8* chip8) {
    // LD I, addr - Set I = nnn

    uint16_t address = chip8->opcode & 0xFFF;

    chip8->index = address;
}

void OP_Bnnn(Chip8* chip8) {
    // JP V0, addr - Jump to location nnn + V0

    uint16_t address = chip8->opcode & 0xFFF;

    chip8->program_counter = chip8->registers[0x0] + address;
}

void OP_Cxkk(Chip8* chip8) {
    // RND Vx, byte - Set Vx = random byte AND kk

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t byte = chip8->opcode & 0xFF;

    chip8->registers[Vx] = rand() & byte;
}

void OP_Dxyn(Chip8* chip8) {
    // DRW Vx, Vy, height - Display n-byte sprite starting at memory location I at (Vx, Vy), Set VF = collision

    // we also know that the sprite is always 8 pixels wide, so total area is 8 * height

    uint8_t Vx = (chip8->opcode & 0x0F00) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0) >> 4;
    uint8_t height = chip8->opcode & 0x000F;

    uint8_t xPos = chip8->registers[Vx] % VIDEO_WIDTH;
    uint8_t yPos = chip8->registers[Vy] % VIDEO_HEIGHT;

    chip8->registers[0xF] = 0;

    for (int row = 0; row < height; ++row) {
        uint8_t sprite_byte = chip8->memory[chip8->index + row];

        for (int col = 0; col < 8; ++col) {
            uint8_t sprite_pixel = (sprite_byte & (0x80 >> col)) >> (7 - col); // extract pixel in position col from sprite_byte
            uint32_t *screen_pixel = &(chip8->video[(yPos + row) * VIDEO_WIDTH + (xPos + col)]);

            if (sprite_pixel == 1){
                if (*screen_pixel == 0xFFFFFFFF) { // handles collision screen-sprite, set VF if collision
                    chip8->registers[0xF] = 1;
                }

                *screen_pixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void OP_Ex9E(Chip8* chip8) {
    // SKP Vx - Skip next instruction if key with the value of Vx is pressed

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t key = chip8->registers[Vx];

    if (chip8->keypad[key]) {
        chip8->program_counter += 2;
    }
}

void OP_ExA1(Chip8* chip8) {
    // SKNP Vx - Skip next instruction if key with the value of Vx is not pressed

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t key = chip8->registers[Vx];

    if ( !(chip8->keypad[key]) ) {
        chip8->program_counter += 2;
    }
}

void OP_Fx07(Chip8* chip8) {
    // LD Vx, DT - Set Vx = delay timer value

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;

    chip8->registers[Vx] = chip8->delay_timer;
}

void OP_Fx0A(Chip8* chip8) {
    // LD Vx, K - Wait for a key press, store the value of the key in Vx

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;

    uint8_t key = 0;
    while(!chip8->keypad[key] && key < 16) {
        key++;
    }

    if (key < 16) {
        chip8->registers[Vx] = key;
    }
    else {
        chip8->program_counter -= 2;
    }
}

void OP_Fx15(Chip8* chip8) {
    // LD DT, Vx - Set delay timer = Vx

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    chip8->delay_timer = chip8->registers[Vx];
}

void OP_Fx18(Chip8* chip8) {
    // LD ST, Vx - Set sound timer = Vx

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    chip8->sound_timer = chip8->registers[Vx];
}

void OP_Fx1E(Chip8* chip8) {
    // ADD I, Vx - Set I = I + Vx

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;

    chip8->index += chip8->registers[Vx];
}

void OP_Fx29(Chip8* chip8) {
    // LD F, Vx - Set I = location of sprite for digit Vx

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;

    chip8->index = FONTSET_START_ADDRESS + (5 * chip8->registers[Vx]);
}

void OP_Fx33(Chip8* chip8) {
    // LD B, Vx - Store BCD representation of Vx in memory locations I, I+1 and I+2

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    uint8_t value = chip8->registers[Vx];

    chip8->memory[chip8->index + 2] = value % 10;
    value /= 10;
    chip8->memory[chip8->index + 1] = value % 10;
    value /= 10;
    chip8->memory[chip8->index] = value % 10;
}

void OP_Fx55(Chip8* chip8) {
    // LD [I], Vx - Store registers V0 through Vx in memory starting at location I

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    for (uint8_t i = 0; i <= Vx; ++i) {
        chip8->memory[chip8->index + i] = chip8->registers[i];
    }
}

void OP_Fx65(Chip8* chip8) {
    // LD Vx, [I] - Read registers V0 through Vx from memory starting at location I

    uint8_t Vx = (chip8->opcode & 0xF00) >> 8;
    for (uint8_t i = 0; i <= Vx; ++i) {
        chip8->registers[i] = chip8->memory[chip8->index + i];
    }
}

void Table0(Chip8* chip8) {
    uint8_t filtered_OP = chip8->opcode & 0x000F;

    switch (filtered_OP) {
        case 0x0:
            OP_00E0(chip8);
            break;
        case 0xE:
            OP_00EE(chip8);
            break;
        default:
            perror("Error decoding OP in Table0()!\nGot opcode: %X", chip8->opcode);
            exit(-1);
    }
}

void Table8(Chip8* chip8) {
    uint8_t filtered_OP = chip8->opcode & 0xF;

    switch (filtered_OP) {
        case 0x0:
            OP_8xy0(chip8);
            break;
        case 0x1:
            OP_8xy1(chip8);
            break;
        case 0x2:
            OP_8xy2(chip8);
            break;
        case 0x3:
            OP_8xy3(chip8);
            break;
        case 0x4:
            OP_8xy4(chip8);
            break;
        case 0x5:
            OP_8xy5(chip8);
            break;
        case 0x6:
            OP_8xy6(chip8);
            break;
        case 0x7:
            OP_8xy7(chip8);
            break;
        case 0xE:
            OP_8xyE(chip8);
            break;
        default:
            perror("Error decoding OP in Table8()!\nGot opcode: %X", chip8->opcode);
            exit(-1);
    }
}

void TableE(Chip8* chip8) {
    uint8_t filtered_OP = chip8->opcode & 0xF;

    switch (filtered_OP) {
        case 0x1:
            OP_ExA1(chip8);
            break;
        case 0xE:
            OP_Ex9E(chip8);
            break;
        default:
            perror("Error decoding OP in TableE()!\nGot opcode: %X", chip8->opcode);
            exit(-1);
    }
}

void TableF(Chip8* chip8) {
    uint8_t filtered_OP = chip8->opcode & 0xFF;

    switch (filtered_OP) {
        case 0x07:
            OP_Fx07(chip8);
            break;
        case 0x0A:
            OP_Fx0A(chip8);
            break;
        case 0x15:
            OP_Fx15(chip8);
            break;
        case 0x18:
            OP_Fx18(chip8);
            break;
        case 0x1E:
            OP_Fx1E(chip8);
            break;
        case 0x29:
            OP_Fx29(chip8);
            break;
        case 0x33:
            OP_Fx33(chip8);
            break;
        case 0x55:
            OP_Fx55(chip8);
            break;
        case 0x65:
            OP_Fx65(chip8);
            break;
        default:
            perror("Error decoding OP in TableF()!\nGot opcode: %X", chip8->opcode);
            exit(-1);
    }
}

void call_OP(Chip8* chip8) {
    uint8_t filtered_OP = (chip8->opcode & 0xF000) >> 12;

    switch (filtered_OP) {
        case 0x0:
            Table0(chip8);
            break;
        case 0x1:
            OP_1nnn(chip8);
            break;
        case 0x2:
            OP_2nnn(chip8);
            break;
        case 0x3:
            OP_3xkk(chip8);
            break;
        case 0x4:
            OP_4xkk(chip8);
            break;
        case 0x5:
            OP_5xy0(chip8);
            break;
        case 0x6:
            OP_6xkk(chip8);
            break;
        case 0x7:
            OP_7xkk(chip8);
            break;
        case 0x8:
            Table8(chip8);
            break;
        case 0x9:
            OP_9xy0(chip8);
            break;
        case 0xA:
            OP_Annn(chip8);
            break;
        case 0xB:
            OP_Bnnn(chip8);
            break;
        case 0xC:
            OP_Cxkk(chip8);
            break;
        case 0xD:
            OP_Dxyn(chip8);
            break;
        case 0xE:
            TableE(chip8);
            break;
        case 0xF:
            TableF(chip8);
            break;
        default:
            perror("Error decoding OP in call_OP()!\nGot opcode: %X", chip8->opcode);
            exit(-1);
    }
}

void cycle(Chip8* chip8) {
    // Fetch next instruction
    chip8->opcode = (chip8->memory[chip8->program_counter] << 8) | (chip8->memory[chip8->program_counter + 1]);

    // Increment Program Counter before executing the instruction
    chip8->program_counter += 2;

    // Execute instruction
    call_OP(chip8);

    //Decrement delay and sound timer if they've been set
    if (chip8->delay_timer > 0) {
        chip8->delay_timer--;
    }
    if (chip8->sound_timer > 0) {
        chip8->sound_timer--;
    }
}

void run(Chip8* chip8, GUI* sdl_gui, uint32_t cycle_delay) {
    uint8_t quit = 0;
    uint64_t current_time;
    uint64_t last_cycle = current_time;

    while (!quit) {
        quit = process_input(chip8);
        current_time = clock();
        uint64_t delta_time = current_time - last_cycle;

        if (delta_time > cycle_delay) {
            last_cycle = current_time;

            cycle(chip8);
            update_gui(sdl_gui, chip8);
        }
    }
}