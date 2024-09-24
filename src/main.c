#include <stdio.h>
#include "cpu.h"

int main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

    chip8_cpu chip8;

    // TODO remove render() call in init
	initDisplay(&chip8.display);

    //set up input
    initInput(chip8.keypad);

    initialize(&chip8);
    //load_game("some_game_path");

    bool quit = false;
    while (!quit) {
        //emulate_cycle(&chip8);

        if (chip8.drawFlag) {
            render(&chip8.display);
        }

        handleInput(chip8.keypad, &quit);
    }

    
    SDL_DestroyWindow(chip8.display.window);
	SDL_DestroyRenderer(chip8.display.renderer);
    SDL_Quit();

    return 0;
}
