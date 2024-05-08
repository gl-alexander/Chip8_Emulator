#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>


// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// graphics constants
#define WIDTH 64
#define HEIGHT 32
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

// display state:
struct display {
	uint8_t screen[WIDTH * HEIGHT];
	SDL_Window* window;
	SDL_Renderer* renderer;
};

typedef struct display display;

int initSDL(void);
int initDisplay(display *gfx);
void render(display *gfx);

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
        else 
		{
			render(gfx);
		}
        

        // Destroy window
        SDL_DestroyWindow(gfx->window);
    }

    // Quit SDL
    SDL_Quit();

    return 0;
}

void render(display* gfx) {
	// Declare rect of square
	SDL_Rect squareRect;

	// Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
	squareRect.w = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
	squareRect.h = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

	// Square position: In the middle of the screen
	squareRect.x = SCREEN_WIDTH / 2 - squareRect.w / 2;
	squareRect.y = SCREEN_HEIGHT / 2 - squareRect.h / 2;


	// Event loop exit flag
	bool quit = false;

	// Event loop
	while(!quit)
	{
		SDL_Event e;

		// Wait indefinitely for the next available event
		SDL_WaitEvent(&e);

		// User requests quit
		if(e.type == SDL_QUIT)
		{
			quit = true;
		}

		// Initialize renderer color white for the background
		SDL_SetRenderDrawColor(gfx->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		// Clear screen
		SDL_RenderClear(gfx->renderer);

		// Set renderer color red to draw the square
		SDL_SetRenderDrawColor(gfx->renderer, 0xFF, 0x00, 0x00, 0xFF);

		// Draw filled square
		SDL_RenderFillRect(gfx->renderer, &squareRect);

			// Update screen
			SDL_RenderPresent(gfx->renderer);
		}

		// Destroy renderer
		SDL_DestroyRenderer(gfx->renderer);
}	

