#include "input.h"

void initInput(void) {
    for (int i = 0; i < KEYBOARD_SIZE; i++) {
        keys[i] = 0;
    }
}

// TODO MARK KEYS WITH ENUM

void handleInput(SDL_Event* event) {
    switch (event->type) {
        case SDL_KEYDOWN: 
            switch (event->key.keysym.sym) {
                case SDLK_LEFT: 
                    keys[LEFT_INDEX] = 1;
                    break;
                case SDLK_RIGHT:
                    // TODO: FINISH
            }
        break;
        case SDL_KEYUP;
            switch (event->key.keysym.sym) {
                case SDLK_LEFT:
                    // TODO FINISH...

            }
        break;
        default:
            break;
    }
}