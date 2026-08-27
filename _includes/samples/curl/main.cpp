#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <curl/curl.h>

#include <string>

#include "network.hpp"

static constexpr const bool verbose = true;

static unsigned int windowWidth = 480;
static unsigned int windowHeight = 272;

static SDL_Event event;
static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
static TTF_Font *baseFont = nullptr;
static TTF_Font *jokeFont = nullptr;
static SDL_Texture *titleTexture = nullptr;
static SDL_Rect titleRect = {0, 0, 0, 0};
static SDL_Rect jokeRect = {0, 0, 0, 0};
static SDL_Rect jokeButtonRect = {0, 0, 0, 0};
static SDL_Texture *jokeButtonTexture = nullptr;
static SDL_Texture *jokeTexture = nullptr;
static bool loading = false;
static SDL_Rect loadingRect = {0, 0, 0, 0};
static SDL_Texture *loadingTexture = nullptr;

static bool running = true;

void init()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS);
    TTF_Init();

    window = SDL_CreateWindow("Jokr", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    jokeFont = TTF_OpenFont("res/font.ttf", 16);
    baseFont = TTF_OpenFont("res/font_joke.ttf", 32);
    if (!baseFont)
    {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        exit(1);
    };

    SDL_Surface *titleSurface = TTF_RenderUTF8_Blended(baseFont, "Jokr", {0, 0, 0, 255});
    titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);
    SDL_QueryTexture(titleTexture, nullptr, nullptr, &titleRect.w, &titleRect.h);
    titleRect.x = windowWidth / 2 - titleRect.w / 2;

    SDL_Surface *jokeButtonSurface = TTF_RenderUTF8_Blended(jokeFont, "X Get a joke", {0, 0, 0, 255});
    jokeButtonTexture = SDL_CreateTextureFromSurface(renderer, jokeButtonSurface);
    SDL_FreeSurface(jokeButtonSurface);
    SDL_QueryTexture(jokeButtonTexture, nullptr, nullptr, &jokeButtonRect.w, &jokeButtonRect.h);
    jokeButtonRect.x = windowWidth / 2 - jokeButtonRect.w / 2;
    jokeButtonRect.y = windowHeight - jokeButtonRect.h;

    SDL_Surface *jokeSurface = TTF_RenderUTF8_Blended(jokeFont, "Press the button to get a joke", {0, 0, 0, 255});
    jokeTexture = SDL_CreateTextureFromSurface(renderer, jokeSurface);
    SDL_FreeSurface(jokeSurface);
    SDL_QueryTexture(jokeTexture, nullptr, nullptr, &jokeRect.w, &jokeRect.h);
    jokeRect.x = windowWidth / 2 - jokeRect.w / 2;
    jokeRect.y = windowHeight / 2 - jokeRect.h / 2;

    SDL_Surface *loadingSurface = TTF_RenderUTF8_Blended(baseFont, "Loading...", {0, 0, 0, 255});
    loadingTexture = SDL_CreateTextureFromSurface(renderer, loadingSurface);
    SDL_FreeSurface(loadingSurface);
    SDL_QueryTexture(loadingTexture, nullptr, nullptr, &loadingRect.w, &loadingRect.h);
    loadingRect.w /= 1.5;
    loadingRect.h /= 1.5;
    loadingRect.x = windowWidth / 2 - loadingRect.w / 2;
    loadingRect.y = windowHeight - loadingRect.h - 20;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void render()
{

    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    SDL_RenderCopy(renderer, jokeButtonTexture, nullptr, &jokeButtonRect);
    SDL_RenderCopy(renderer, jokeTexture, nullptr, &jokeRect);

    if (loading)
        SDL_RenderCopy(renderer, loadingTexture, nullptr, &loadingRect);
}

size_t writeCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

CURLcode getRequest(const std::string &url, std::string &buffer, bool verbose = false)
{
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        SDL_Log("Failed to initialize curl");
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.64.1");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(nullptr, "Accept: text/plain"));
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 20L);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        SDL_Log("Failed to perform curl request: %s", curl_easy_strerror(res));

    curl_easy_cleanup(curl);
    return res;
}

void getNewJoke()
{
    loading = true;
    SDL_RenderClear(renderer);
    render();
    SDL_RenderPresent(renderer);

    std::string response;

    if (getRequest("https://icanhazdadjoke.com/", response, verbose) != CURLE_OK)
        response = "Failed to get joke :(";

    SDL_Log("Response: %s", response.c_str());

    SDL_Surface *jokeSurface = TTF_RenderUTF8_Blended_Wrapped(jokeFont, response.c_str(), {0, 0, 0, 255}, windowWidth - 20);
    jokeTexture = SDL_CreateTextureFromSurface(renderer, jokeSurface);
    SDL_FreeSurface(jokeSurface);
    SDL_QueryTexture(jokeTexture, nullptr, nullptr, &jokeRect.w, &jokeRect.h);
    jokeRect.x = windowWidth / 2 - jokeRect.w / 2;
    jokeRect.y = windowHeight / 2 - jokeRect.h / 2;

    loading = false;
}

void cleanup()
{
    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(jokeButtonTexture);
    SDL_DestroyTexture(jokeTexture);
    SDL_DestroyTexture(loadingTexture);
    printf("Textures destroyed\n");
    TTF_CloseFont(baseFont);
    TTF_CloseFont(jokeFont);
    printf("Fonts closed\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    printf("Window destroyed\n");
    TTF_Quit();
    printf("TTF quit\n");
    SDL_Quit();
    printf("SDL quit\n");
}

void handleInput()
{
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_CONTROLLERDEVICEADDED:
            SDL_GameControllerOpen(event.cdevice.which);
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
                running = false;
            else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                getNewJoke();
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_x)
                getNewJoke();
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                windowWidth = event.window.data1;
                windowHeight = event.window.data2;
                jokeButtonRect.y = windowHeight - jokeButtonRect.h;
                jokeRect.y = windowHeight / 2 - jokeRect.h / 2;
                titleRect.x = windowWidth / 2 - titleRect.w / 2;
                jokeButtonRect.x = windowWidth / 2 - jokeButtonRect.w / 2;
                jokeRect.x = windowWidth / 2 - jokeRect.w / 2;
                loadingRect.x = windowWidth / 2 - loadingRect.w / 2;
                loadingRect.y = windowHeight - loadingRect.h - 20;
            }
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    init();

    startNetworking(renderer);

    while (running)
    {
        handleInput();

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        render();

        SDL_RenderPresent(renderer);
    }

    stopNetworking();

    cleanup();

    return 0;
}
