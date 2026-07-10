#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>

// Define screen dimensions
#define SCREEN_WIDTH    480
#define SCREEN_HEIGHT   272

int main(int argc, char **argv) {
    // This prevents compiler warnings
    // We don't actually need these variables, but they do need to be there so SDL_main works
    (void)argc;
    (void)argv;

    // Initialize sdl
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Initialize SDL mixer
    if(!MIX_Init()) {
        SDL_Log("Couldn't initialize SDL mixer: %s", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    // Initialise audio device
    MIX_Mixer * mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer) {
        SDL_Log("Couldn't open default audio device: %s", SDL_GetError());
        MIX_Quit();
        SDL_Quit();
        return 3;
    }

    // Initialize channel to play music on, called a track in SDL mixer
    MIX_Track * music_track = MIX_CreateTrack(mixer);
    if (!music_track) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        MIX_DestroyMixer(mixer);
        MIX_Quit();
        SDL_Quit();
        return 3;
    }


    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    if (!SDL_CreateWindowAndRenderer("window", 480, 272, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        MIX_DestroyMixer(mixer);
        MIX_Quit();
        SDL_Quit();
        return 4;
    }

    // Load ogg file
    MIX_Audio *ogg_file = MIX_LoadAudio(mixer, "test.ogg", false);
    if (!ogg_file) {
        SDL_Log("Couldn't load audio file: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        MIX_DestroyMixer(mixer);
        MIX_Quit();
        SDL_Quit();
        return 5;
    }

    SDL_FRect rect;

    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    rect.w = SDL_min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.0f;
    rect.h = SDL_min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.0f;

    // Square position: In the middle of the screen
    rect.x = SCREEN_WIDTH / 2 - rect.w / 2;
    rect.y = SCREEN_HEIGHT / 2 - rect.h / 2;


    // Declare rects of pause symbol
    SDL_FRect pause_rect1, pause_rect2;

    pause_rect1.h = rect.h / 2.0f;
    pause_rect1.w = 40.0f;
    pause_rect1.x = rect.x + (rect.w - pause_rect1.w * 3.0f) / 2.0f;
    pause_rect1.y = rect.y + rect.h / 4.0f;
    pause_rect2 = pause_rect1;
    pause_rect2.x += pause_rect1.w * 2.0f;
    
    // play the music 8 times
    MIX_SetTrackAudio(music_track, ogg_file);
    MIX_SetTrackGain(music_track, 1.0f);
    if (!MIX_PlayTrack(music_track, 8)) {
        SDL_Log("Couldn't play audio: %s", SDL_GetError());
        MIX_DestroyAudio(ogg_file);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        MIX_DestroyMixer(mixer);
        MIX_Quit();
        SDL_Quit();
        return 6;
    }

    int running = 1;
    SDL_Event e;
    while (running) {
        if(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_EVENT_QUIT:
                    running = 0;
                    break;
                case SDL_EVENT_GAMEPAD_ADDED:
                    SDL_OpenGamepad(e.cdevice.which);
                    break;
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                    // pause using cross button
                    if (e.gbutton.button == SDL_GAMEPAD_BUTTON_SOUTH) {
                        MIX_PauseTrack(music_track);
                    // resume using circle button
                    } else if (e.gbutton.button == SDL_GAMEPAD_BUTTON_EAST) {
                        MIX_ResumeTrack(music_track);
                    // press start button to exit
                    } else if (e.gbutton.button == SDL_GAMEPAD_BUTTON_START) {
                        running = 0;
                    }
                    break;		
            }
        }

        // Initialize renderer color black for the background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        // Clear screen
        SDL_RenderClear(renderer);

        // Set renderer color green to draw the square
        SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF);

        // Draw filled square
        SDL_RenderFillRect(renderer, &rect);

        // Check pause status
        if(!MIX_TrackPlaying(music_track)) {
            // Set renderer color black to draw the pause symbol
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

            // Draw pause symbol
            SDL_RenderFillRect(renderer, &pause_rect1);
            SDL_RenderFillRect(renderer, &pause_rect2);
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    MIX_DestroyAudio(ogg_file);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    MIX_DestroyMixer(mixer);
    MIX_Quit();
    SDL_Quit();

    return 0;
}