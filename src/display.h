#include <stdint.h>
#include <stdio.h>
#include <SDL.h>
#include "utils.h"

// graphics constants
#define WIDTH 64
#define HEIGHT 32
#define SCREEN_WIDTH 1280	
#define SCREEN_HEIGHT 720

//TODO default color
#define COL_BLACK 0, 0, 0
#define COL_WHITE 255, 255, 255
#define COL_ORANGE 255, 170, 0
#define COL_BACKGROUND COL_WHITE
#define COL_PIXEL COL_ORANGE

// display state:
typedef struct display {
	u8 screen[HEIGHT][WIDTH];
	SDL_Window* window;
	SDL_Renderer* renderer;
} display;

int initSDL(void);
int initDisplay(display *gfx);
void render(display *gfx);
void renderGraphics(display* gfx, u8 screen[HEIGHT][WIDTH]);
void renderPixel(display * gfx, u8 x, u8 y, u8 value);