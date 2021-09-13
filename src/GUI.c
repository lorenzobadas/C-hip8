#include "GUI.h"

void initialize_gui(GUI *sdl_gui, uint8_t scale) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error! SDL_Init() failed\n");
        exit(-1);
    }

    sdl_gui->window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, VIDEO_WIDTH * scale, VIDEO_HEIGHT * scale, SDL_WINDOW_SHOWN);
    if (sdl_gui->window == NULL) {
        printf("Error! SDL_CreateWindow() failed\n");
        exit(-1);
    }

    sdl_gui->renderer = SDL_CreateRenderer(sdl_gui->window, -1, SDL_RENDERER_ACCELERATED);

    sdl_gui->texture = SDL_CreateTexture(sdl_gui->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, VIDEO_WIDTH, VIDEO_HEIGHT);
}

void free_alloc_SDL(GUI *sdl_gui) {
    SDL_DestroyTexture(sdl_gui->texture);
    SDL_DestroyRenderer(sdl_gui->renderer);
    SDL_DestroyWindow(sdl_gui->window);
    SDL_Quit();
}

uint8_t handle_key_input(SDL_Event event, Chip8 *chip8) {
    uint8_t quit = 0;

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = 1;
                break;
            case SDLK_x:
                chip8->keypad[0x0] = 1;
                break;
            case SDLK_1:
                chip8->keypad[0x1] = 1;
                break;
            case SDLK_2:
                chip8->keypad[0x2] = 1;
                break;
            case SDLK_3:
                chip8->keypad[0x3] = 1;
                break;
            case SDLK_q:
                chip8->keypad[0x4] = 1;
                break;
            case SDLK_w:
                chip8->keypad[0x5] = 1;
                break;
            case SDLK_e:
                chip8->keypad[0x6] = 1;
                break;
            case SDLK_a:
                chip8->keypad[0x7] = 1;
                break;
            case SDLK_s:
                chip8->keypad[0x8] = 1;
                break;
            case SDLK_d:
                chip8->keypad[0x9] = 1;
                break;
            case SDLK_z:
                chip8->keypad[0xA] = 1;
                break;
            case SDLK_c:
                chip8->keypad[0xB] = 1;
                break;
            case SDLK_4:
                chip8->keypad[0xC] = 1;
                break;
            case SDLK_r:
                chip8->keypad[0xD] = 1;
                break;
            case SDLK_f:
                chip8->keypad[0xE] = 1;
                break;
            case SDLK_v:
                chip8->keypad[0xF] = 1;
                break;
        }
    }
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_x:
                chip8->keypad[0x0] = 0;
                break;
            case SDLK_1:
                chip8->keypad[0x1] = 0;
                break;
            case SDLK_2:
                chip8->keypad[0x2] = 0;
                break;
            case SDLK_3:
                chip8->keypad[0x3] = 0;
                break;
            case SDLK_q:
                chip8->keypad[0x4] = 0;
                break;
            case SDLK_w:
                chip8->keypad[0x5] = 0;
                break;
            case SDLK_e:
                chip8->keypad[0x6] = 0;
                break;
            case SDLK_a:
                chip8->keypad[0x7] = 0;
                break;
            case SDLK_s:
                chip8->keypad[0x8] = 0;
                break;
            case SDLK_d:
                chip8->keypad[0x9] = 0;
                break;
            case SDLK_z:
                chip8->keypad[0xA] = 0;
                break;
            case SDLK_c:
                chip8->keypad[0xB] = 0;
                break;
            case SDLK_4:
                chip8->keypad[0xC] = 0;
                break;
            case SDLK_r:
                chip8->keypad[0xD] = 0;
                break;
            case SDLK_f:
                chip8->keypad[0xE] = 0;
                break;
            case SDLK_v:
                chip8->keypad[0xF] = 0;
                break;
        }
    }

    return quit;
}

uint8_t process_input(Chip8 *chip8) {
    uint8_t quit = 0;
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = 1;
        }
        else {
            quit = handle_key_input(event, chip8);
        }
    }

    return quit;
}

void update_gui(GUI* sdl_gui, Chip8* chip8) {
    SDL_UpdateTexture(sdl_gui->texture, NULL, chip8->video, sizeof(uint32_t) * VIDEO_WIDTH);
    SDL_RenderClear(sdl_gui->renderer);
    SDL_RenderCopy(sdl_gui->renderer, sdl_gui->texture, NULL, NULL);
    SDL_RenderPresent(sdl_gui->renderer);
}