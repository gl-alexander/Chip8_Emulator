#include <SDL.h>
#include "utils.h"

#define KEYBOARD_SIZE 16

u8 keys[KEYBOARD_SIZE]; // hex keyboard

void initInput(void);
void handleInput(SDL_Event* event);
