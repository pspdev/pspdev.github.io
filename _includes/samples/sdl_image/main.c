#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);

    SDL_Window * window = SDL_CreateWindow(
        "window",
        480,
        272,
        0
    );

    SDL_Renderer * renderer = SDL_CreateRenderer(window, NULL);

    // Load the texture
    SDL_Surface * pixels = IMG_Load("grass.png");
    SDL_Texture * sprite = SDL_CreateTextureFromSurface(renderer, pixels);
    SDL_DestroySurface(pixels);

    // Store the dimensions of the texture
    SDL_FRect sprite_rect;
    SDL_GetTextureSize(sprite, &sprite_rect.w, &sprite_rect.h);

    // Set the position to draw to in the middle of the screen
    sprite_rect.x = 480.0f / 2.0f - sprite_rect.w / 2.0f;
    sprite_rect.y = 272.0f / 2.0f - sprite_rect.h / 2.0f;

    int running = 1;
    SDL_Event event;
    while (running) { 
        // Process input
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    // End the loop if the programs is being closed
                    running = 0;
                    break;
                case SDL_EVENT_GAMEPAD_ADDED:
                    // Connect a controller when it is connected
                    SDL_OpenGamepad(event.cdevice.which);
                    break;
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                    if(event.gbutton.button == SDL_GAMEPAD_BUTTON_START) {
                        // Close the program if start is pressed
                        running = 0;
                    }
                    break;
            }
        }

        // Clear the screen
        SDL_RenderClear(renderer);

        // Draw the 'grass' sprite
        SDL_RenderTexture(renderer, sprite, NULL, &sprite_rect);

        // Draw everything on a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}