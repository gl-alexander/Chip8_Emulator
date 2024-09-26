#include <stdio.h>
#include "cpu.h"

#define DELAY 3
#define REFRESH_RATE 500

int main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

    char romPath[] = "../games/test_opcode.ch8";

    chip8_cpu chip8;
    float cycleDelay = (1.0F / REFRESH_RATE);

    // TODO remove render() call in init
	initDisplay(&chip8.display);

    //set up input
    initInput(chip8.keypad);

    initialize(&chip8);
    load_game(&chip8, romPath);

    clock_t lastCycleTime = clock();
    bool quit = false;

    while (!quit) {
        handleInput(chip8.keypad, &quit);

        clock_t currentTime = clock();
        float dt = (float)(currentTime - lastCycleTime) * 1000.0f / CLOCKS_PER_SEC; 

        if (dt > cycleDelay) {
            lastCycleTime = currentTime;
            emulate_cycle(&chip8);
            if (chip8.drawFlag) {
                render(&chip8.display);
            }
        }

    }

    
    SDL_DestroyWindow(chip8.display.window);
	SDL_DestroyRenderer(chip8.display.renderer);
    SDL_Quit();

    return 0;
}
