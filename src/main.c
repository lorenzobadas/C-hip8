#include "CHIP8.h"
#include "GUI.h"

void parse_arguments(int, char**, uint32_t*, uint8_t*);

int main(int argc, char** argv) {
    char *filename = NULL;
    uint32_t cycle_delay = 2000;
    uint8_t scale = 25;

    if (argc == 1) {
        printf("\nUsage: \"%s <ROM path> [-d <Delay>] [-s <Scale>]\"\n >> Initializing with default values...\n\n", argv[0]);
    }
    else {
        filename = argv[1];
        parse_arguments(argc, argv, &cycle_delay, &scale);
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

void parse_arguments(int argc, char** argv, uint32_t* cycle_delay, uint8_t* scale) {
    for (int i = 2; i < argc; ++i) {

        uint8_t bad_conditions = argc > 6 || *scale < 1 || *scale > 100 || *cycle_delay < 0 || *cycle_delay > 0xFFFFFFFF;
        if (bad_conditions) {
            printf("Error while parsing arguments!\n >> Exiting...\n");
            exit(-1);
        }

        if ( (strcmp(argv[i], "-d")) == 0 && (i < argc - 1) ) {
            *cycle_delay = strtol(argv[i + 1], NULL, 0);
        }
        else if ( (strcmp(argv[i], "-s") == 0) && (i < argc - 1) ) {
            *scale = strtol(argv[i + 1], NULL, 0);
        }
    }
}