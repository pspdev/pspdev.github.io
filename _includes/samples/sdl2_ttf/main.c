#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

// Define screen dimensions
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL2 could not be initialized!\n"
               "SDL2 Error: %s\n",
               SDL_GetError());
        return 0;
    }

    // Initialize SDL2_ttf
    if (TTF_Init() < 0)
    {
        printf("SDL2_ttf could not be initialized!\n"
               "SDL2_ttf Error: %s\n",
               SDL_GetError());
        return 0;
    }

    SDL_Window *win = SDL_CreateWindow(
        "window",
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    if (!win)
    {
        printf("Window could not be created!\n"
               "SDL_Error: %s\n",
               SDL_GetError());
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, NULL);
    TTF_Font *font = TTF_OpenFont("Pacifico.ttf", 40);

    // Set the text and background color
    SDL_Color text_color = {0xff, 0xff, 0xff, 0xff};
    SDL_Color bg_color = {0x00, 0x00, 0x00, 0xff};

    SDL_FRect text_rect;
    SDL_Surface *surface = TTF_RenderText_Shaded(font, "Hello World!", 0, text_color, bg_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Get text dimensions
    text_rect.w = surface->w;
    text_rect.h = surface->h;

    SDL_DestroySurface(surface);

    text_rect.x = (SCREEN_WIDTH - text_rect.w) / 2;
    text_rect.y = text_rect.h + 30;

    int running = 1;
    SDL_Event e;
    while (running)
    {
        if (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                running = 0;
                break;
            case SDL_EVENT_GAMEPAD_ADDED:
                SDL_OpenGamepad(e.cdevice.which);
                break;
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                if (e.button.button == SDL_GAMEPAD_BUTTON_START)
                {
                    running = 0;
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
        SDL_RenderTexture(renderer, texture, NULL, &text_rect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
