#include <SDL.h>
#include "SDL_keyboard.h"
#include "utils.h"

#define KEYBOARD_SIZE 16

void initInput(u8 keys[KEYBOARD_SIZE]);
void handleInput(u8 keys[KEYBOARD_SIZE], bool* quit);
