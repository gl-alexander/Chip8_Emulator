#include <stdio.h>
#include "cpu.h"

#define REFRESH_RATE 100

int str_contains(const char* str, int len, char c) {
    for (int i = 0; i < len; i++) {
        if (str[i] == c) return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Expected a single argument: the path to the rom.\n");
        exit(1);
    }

    char rom_path[1024] = {0};

    int rom_name_len = strlen(argv[1]);

    if (str_contains(argv[1], rom_name_len, '/')) {
        // run the game
        strcpy(rom_path, argv[1]);
        
    }
    else {
        // append game name to the games folder:
        strcat(rom_path, "../games/");
        strcat(rom_path, argv[1]);
        if (strcmp(argv[1] + rom_name_len - 4, ".ch8") != 0) {
            strcat(rom_path, ".ch8");
        }
    }


    chip8_cpu chip8;
    // Engine refresh rate
    float cycleDelay = (1.0F / REFRESH_RATE);

    initDisplay(&chip8.display);
    initInput(chip8.keypad);
    initialize(&chip8);
    // Loads the passed ROM to the Chip8 Memory
    load_game(&chip8, rom_path);

    clock_t lastCycleTime = clock();
    bool quit = false;
    while (!quit) {
        handleInput(chip8.keypad, &quit);

        clock_t currentTime = clock();
        float dt = (float)(currentTime - lastCycleTime) / CLOCKS_PER_SEC; 

        if (dt > cycleDelay) {
            lastCycleTime = currentTime;
            emulate_cycle(&chip8);
            if (chip8.drawFlag) {
                render(&chip8.display);
                chip8.drawFlag = 0;
            }
        }

    }

    
    SDL_DestroyWindow(chip8.display.window);
	SDL_DestroyRenderer(chip8.display.renderer);
    SDL_Quit();
    return 0;
}
