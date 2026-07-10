#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

// Define screen dimensions
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272

int main(int argc, char **argv) {
    // This prevents compiler warnings
    // We don't actually need these variables, but they do need to be there so SDL_main works
    (void)argc;
    (void)argv;

    // Initialize sdl
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Initialize SDL ttf
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL ttf: %s", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    if (!SDL_CreateWindowAndRenderer("window", 480, 272, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        SDL_Quit();
        return 3;
    }

    // Load the font file, make sure it is in the directory the eboot file is in
    TTF_Font *font = TTF_OpenFont("Pacifico.ttf", 40);
    if (!font) {
        SDL_Log("Couldn't load font: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 4;
    }

    // Set the text and background color
    SDL_Color text_color = {0x00, 0x00, 0x00, 0xff};
    SDL_Color bg_color = {0xff, 0xff, 0xff, 0xff};

    // Create a texture using a string and SDL ttf
    const char * text = "Hello World!";
    SDL_FRect text_rect = {0, 0, 0, 0};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, SDL_strlen(text), text_color);
    if (!surface) {
        SDL_Log("Couldn't render text: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 5;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Get text dimensions
    text_rect.w = (float) surface->w;
    text_rect.h = (float) surface->h;

    SDL_DestroySurface(surface);

    text_rect.x = (SCREEN_WIDTH - text_rect.w) / 2.0f;
    text_rect.y = text_rect.h + 30.0f;

    int running = 1;
    SDL_Event e;
    while (running) {
        if (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:  // Event that is passed when the user closes the program
                    running = 0;
                    break;
                case SDL_EVENT_GAMEPAD_ADDED:  // Activate any gamepad that is connected
                    SDL_OpenGamepad(e.cdevice.which);
                    break;
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                    if (e.gbutton.button == SDL_GAMEPAD_BUTTON_START) {
                        running = 0;
                    }
                    break;
            }
        }

        // Refresh the screen and redraw the text every frame
        SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, &text_rect);
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}