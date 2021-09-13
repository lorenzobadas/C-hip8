#include "CHIP8.h"
#include "GUI.h"

int main(int argc, char** argv) {
    char *filename = NULL;
    uint32_t cycle_delay = 2000;
    uint8_t scale = 25;

    if (argc != 4) {
        printf("\nUsage: \"%s <ROM path> <Delay> <Scale>\"\n >> Initializing with default values...\n\n", argv[0]);
    }
    else {
        filename = argv[1];
        cycle_delay = strtol(argv[2], NULL, 0);
        scale = strtol(argv[3], NULL, 0);
    }

    Chip8 chip8;
    GUI sdl_gui;

    initialize_chip8(&chip8);
    if (filename == NULL) {
        load_default_rom(&chip8);
    }
    else {
        load_rom(&chip8, filename);
    }
    load_fontset(&chip8);
    srand(time(NULL));

    initialize_gui(&sdl_gui, scale);

    run(&chip8, &sdl_gui, cycle_delay);
    
    free_alloc_chip8(&chip8);
    free_alloc_SDL(&sdl_gui);
    return 0;
}