#include <stdio.h>
#include "cpu.h"

#define REFRESH_RATE 100

int main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

    char romPath[] = "../games/test_opcode.ch8";

    chip8_cpu chip8;
    // Engine refresh rate
    float cycleDelay = (1.0F / REFRESH_RATE);

    initDisplay(&chip8.display);
    initInput(chip8.keypad);
    initialize(&chip8);
    // Loads the passed ROM to the Chip8 Memory
    load_game(&chip8, romPath);

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
