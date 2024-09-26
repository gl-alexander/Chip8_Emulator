#include "display.h"

int PIXEL_WIDTH = 10;
int PIXEL_HEIGHT = 10;

int initSDL(void) {
	// Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized!\n"
               "SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
    // Disable compositor bypass
    if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
    {
        printf("SDL can not disable compositor bypass!\n");
        return 0;
    }
#endif
	return 0;
}



int initDisplay(display* gfx)
{
	initSDL();
       // Create window
    gfx->window = SDL_CreateWindow("Chip8 Emulator",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
	PIXEL_HEIGHT = SCREEN_HEIGHT / HEIGHT;
	PIXEL_WIDTH = SCREEN_WIDTH / WIDTH;
    if(!gfx->window)
    {
        printf("Window could not be created!\n"
               "SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Create renderer
        gfx->renderer = SDL_CreateRenderer(gfx->window, -1, SDL_RENDERER_ACCELERATED);
        if(!gfx->renderer)
        {
            printf("Renderer could not be created!\n"
                   "SDL_Error: %s\n", SDL_GetError());
        }
    }
    return 0;
}


void render(display* gfx) {
	// Initialize renderer color white for the background
	SDL_SetRenderDrawColor(gfx->renderer, COL_BACKGROUND, 0xFF);

	// Clear screen
	SDL_RenderClear(gfx->renderer);

	renderGraphics(gfx, gfx->screen);
	// Update screen
	SDL_RenderPresent(gfx->renderer);
}	

void renderGraphics(display* gfx, u8 screen[HEIGHT][WIDTH]) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			renderPixel(gfx, x, y, screen[y][x]);
		}
	}
}

void renderPixel(display * gfx, u8 x, u8 y, u8 value) {
	// x is 0 to 64
	// y is 0 to 32
	if (value == 1)	{
		SDL_SetRenderDrawColor(gfx->renderer, COL_PIXEL, 0xFF * value);
	}
	else {
		SDL_SetRenderDrawColor(gfx->renderer, COL_BACKGROUND, 0xFF * value);
	} 

	SDL_Rect curr = {x * PIXEL_WIDTH, y * PIXEL_HEIGHT, PIXEL_WIDTH, PIXEL_HEIGHT};

	SDL_RenderFillRect(gfx->renderer, &curr);
}
