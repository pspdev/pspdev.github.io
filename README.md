PSPDEV is an open source toolchain for Playstation Portable development. It allows you to make apps and games for both custom and official firmwares. This is a community project made by enthusiasts, it is in no way affiliated with Sony.

# Getting started

## Installing

### dependencies

The PSPDEV toolchain requires a couple of dependencies to be installed before use.

#### Windows

On Windows it is advised to use Ubuntu on Microsoft's WSL system. To set it up run the following commands in a Powershell window started as administrator:

```
wsl --install
```

Now run bash and run the following commands to install the dependencies:

```
sudo apt-get update
sudo apt-get install build-essential cmake pkgconf libreadline8 libusb-0.1 libgpgme11 libarchive-tools fakeroot
```

Going forwards, all commands will be run within bash. The filesystem of your Ubuntu installation can be accessed by opening the network location `\\wsl$`.

#### Ubuntu

On Ubuntu run the following commands to install the dependencies:

```
sudo apt-get update
sudo apt-get install build-essential cmake pkgconf libreadline8 libusb-0.1 libgpgme11 libarchive-tools fakeroot
```

#### MacOS


On MacOS make sure to install [Brew](https://brew.sh/) first, then run the following commands to install the dependencies:

```
brew install cmake pkgconf gnu-sed bash openssl libtool libarchive gettext texinfo bison flex isl gsl gmp mpfr
```


### Toolchain 

To install the PSPDEV toolchain, first [download the latest version](https://github.com/pspdev/pspdev/releases/tag/latest) for your system. Extract it into your user's home directory, which would be `\\wsl$\home\username` on Windows, otherwise `~`. If you're on Windows, pick the Ubuntu build.

Now set the required environment variables. On Mac edit the ``~/.zprofile`` on Linux/WSL the ``~/.bashrc`` file. Add the following at the bottom:

```
export PSPDEV=~/pspdev
export PATH=$PATH:$PSPDEV/bin
```

This can simply be done with `nano ~/.bashrc` (``~/.zprofile`` on Mac). To save press Ctrl+X, then press Y, then enter/return twice. After that run `source ~/.bashrc` to apply the changes.

On MacOS, run ``sudo xattr -rd com.apple.quarantine $PSPDEV`` to prevent Gatekeeper from blocking the toolchain.

That's it, now the PSPDEV toolchain can be used to build PSP software. Below are some examples of programs and how to run them. For testing on real hardware, check out [psplink](https://pspdev.github.io/psplinkusb/).

## Basic programs

Below are some basic examples of programs for the PSP. More can be found [here](https://github.com/pspdev/pspsdk/tree/master/src/samples).

### Hello world

![](images/hello.png?raw=true)


This is a simple Hello World program for the PSP. Click on the details below to see the code and how to build it.

<p><details>

<b>main.c</b>:

<pre>
#include &lt;pspkernel.h&gt;
#include &lt;pspdebug.h&gt;
#include &lt;pspdisplay.h&gt;

// PSP_MODULE_INFO is required
PSP_MODULE_INFO("Hello World", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void *common)
{
    sceKernelExitGame();
    return 0;
}

int callback_thread(SceSize args, void *argp)
{
    int cbid = sceKernelCreateCallback("Exit Callback",
        exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setup_callbacks(void)
{
    int thid = sceKernelCreateThread("update_thread",
        callback_thread, 0x11, 0xFA0, 0, 0);

    if(thid &gt;= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

int main(void) 
{
    // Use above functions to make exiting possible
    setup_callbacks();
    
    // Print Hello World! on a debug screen on a loop
    pspDebugScreenInit();
    while(1)
    {
        pspDebugScreenSetXY(0, 0);
        pspDebugScreenPrintf("Hello World!");
        sceDisplayWaitVblankStart();
    }

    return 0;
}
</pre>

<b>CMakeLists.txt</b>:

<pre>
cmake_minimum_required(VERSION 3.0)

project(hello)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pspdebug
    pspdisplay
    pspge
)

# Create an EBOOT.PBP file
create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE ${PROJECT_NAME}
)
</pre>

Building can be done with:

<pre>
mkdir build && cd build
psp-cmake ..
make
</pre>

This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.
</details></p>

### Drawing shapes

![](images/shape.png?raw=true)

This is a simple square drawn on the PSP. It uses the native libgu library. Click on the details below to see the code and how to build it.

<p><details>

<b>main.c</b>:

<pre>
#include &lt;pspkernel.h&gt;
#include &lt;pspgu.h&gt;

PSP_MODULE_INFO("gutest", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);

#define BUFFER_WIDTH 512
#define BUFFER_HEIGHT 272
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT BUFFER_HEIGHT

char list[0x20000] __attribute__((aligned(64)));

void initGu(){
    sceGuInit();

    //Set up buffers
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888,(void*)0,BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH,SCREEN_HEIGHT,(void*)0x88000,BUFFER_WIDTH);
    sceGuDepthBuffer((void*)0x110000,BUFFER_WIDTH);

    //Set up viewport
    sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    //Set some stuff
    sceGuDepthRange(65535, 0); //Use the full buffer for depth testing - buffer is reversed order

    sceGuDepthFunc(GU_GEQUAL); //Depth buffer is reversed, so GEQUAL instead of LEQUAL
    sceGuEnable(GU_DEPTH_TEST); //Enable depth testing

    sceGuFinish();
    sceGuDisplay(GU_TRUE);
}

void endGu(){
    sceGuDisplay(GU_FALSE);
    sceGuTerm();
}

void startFrame(){
    sceGuStart(GU_DIRECT, list);
    sceGuClearColor(0xFFFFFFFF); // White background
    sceGuClear(GU_COLOR_BUFFER_BIT);
}

void endFrame(){
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}

typedef struct {
    unsigned short u, v;
    short x, y, z;
} Vertex;

void drawRect(float x, float y, float w, float h) {

    Vertex* vertices = (struct Vertex*)sceGuGetMemory(2 * sizeof(Vertex));

    vertices[0].x = x;
    vertices[0].y = y;

    vertices[1].x = y + w;
    vertices[1].y = x + h;

    sceGuColor(0xFF0000FF); // Red, colors are ABGR
    sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
}


int main() {
    initGu();
    int running = 1;
    while(running){
        startFrame();

        drawRect(32, 32, 64, 64);

        endFrame();
    }

    return 0;
}
</pre>

<b>CMakeLists.txt</b>:

<pre>
cmake_minimum_required(VERSION 3.0)

project(shape)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pspgu
    pspge
    pspdisplay
)

# Create an EBOOT.PBP file
create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE ${PROJECT_NAME}
)
</pre>

Building can be done with:

<pre>
mkdir build && cd build
psp-cmake ..
make
</pre>

<p>This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.</p>

More libgu examples can be found <a href="https://github.com/pspdev/pspsdk/tree/master/src/samples/gu">here</a>.

</details></p>

### Controller

![](images/controls.png?raw=true)

This is a simple program to use the PSP controller. Click on details below to see the code and how to build it.

<p><details>

<b>main.c</b>:

<pre>
#include &lt;pspkernel.h&gt;
#include &lt;pspdebug.h&gt;
#include &lt;pspctrl.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;

PSP_MODULE_INFO("Controller", 0, 1, 1);

PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#define printf pspDebugScreenPrintf

int done = 0;

int exit_callback(int arg1, int arg2, void *common)
{
    done = 1;
    return 0;
}

int callback_thread(SceSize args, void *argp)
{
    int cbid = sceKernelCreateCallback("Exit Callback",
        exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setup_callbacks(void)
{
    int thid = sceKernelCreateThread("update_thread",
        callback_thread, 0x11, 0xFA0, 0, 0);

    if(thid &gt;= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

int main(void)
{
    SceCtrlData pad;

    pspDebugScreenInit();
    setup_callbacks();

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    while (!done)
    {
        pspDebugScreenSetXY(0, 2);
        sceCtrlReadBufferPositive(&pad, 1);

        printf("Analog X = %d, ", pad.Lx);
        printf("Analog Y = %d \n", pad.Ly);

        if (pad.Buttons != 0)
        {
            if (pad.Buttons & PSP_CTRL_SQUARE)
            {
                printf("Square pressed! \n");
            }
            if (pad.Buttons & PSP_CTRL_TRIANGLE)
            {
                printf("Triangle pressed! \n");
            }
            if (pad.Buttons & PSP_CTRL_CIRCLE)
            {
                printf("Circle pressed! \n");
            }
            if (pad.Buttons & PSP_CTRL_CROSS)
            {
                printf("Cross pressed! \n");
            }

            if (pad.Buttons & PSP_CTRL_UP)
            {
                printf("Up direction pad pressed! \n");
            }
            if (pad.Buttons & PSP_CTRL_DOWN)
            {
                printf("Down direction pad pressed! \n");
            }
            if (pad.Buttons & PSP_CTRL_LEFT)
            {
                printf("Left direction pad pressed! \n");
            }
            if (pad.Buttons & PSP_CTRL_RIGHT)
            {
                printf("Right direction pad pressed! \n");
            }
        }
    }

    sceKernelExitGame();
    return 0;
}
</pre>

<b>CMakeLists.txt</b>:

<pre>
cmake_minimum_required(VERSION 3.0)

project(controls)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pspdebug
    pspdisplay
    pspge
    pspctrl
)

# Create an EBOOT.PBP file
create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE ${PROJECT_NAME}
)
</pre>

Building can be done with:

<pre>
mkdir build && cd build
psp-cmake ..
make
</pre>

<p>This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.</p>

</details></p>

### Audio

![](images/audio.png?raw=true)

This is a simple program to use the audio of the PSP with minimal effort. It uses native audio library. Click on the details below to see the code and how to build it.

<p><details>

<b>main.c</b>:

<pre>
#include &lt;pspkernel.h&gt;
#include &lt;pspdebug.h&gt;
#include &lt;pspaudiolib.h&gt;
#include &lt;pspaudio.h&gt;
#include &lt;pspdisplay.h&gt;
#include &lt;pspctrl.h&gt;

#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;math.h&gt;
#include &lt;limits.h&gt;

PSP_MODULE_INFO("audio", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#define printf	pspDebugScreenPrintf

/* Exit callback */
int exitCallback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}

/* Callback thread */
int callbackThread(SceSize args, void *argp) {
    int cbid;

    cbid = sceKernelCreateCallback("Exit Callback", (void*) exitCallback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();

    return 0;
}

/* Sets up the callback thread and returns its thread id */
int setupCallbacks(void) {
    int thid = 0;

    thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, 0);
    if (thid &gt;= 0) {
        sceKernelStartThread(thid, 0, 0);
    }
    return thid;
}

/* Main code */

const float PI = 3.1415926535897932f;
const int sampleRate = 44100;
float frequency = 440.0f;
float currentTime = 0;
int function = 0;

typedef struct {
        short l, r;
} sample_t;

float currentFunction(const float time) {
    double x;
    float t = modf((time / (2 * PI)), &x);

    switch(function) {
        case 0: // SINE
            return sinf(time);
        case 1: // SQUARE
            if (t &lt; 0.5f) {
                return -0.2f;
            } else {
                return 0.2f;
            }
        case 2: // TRIANGLE
            if (t &lt; 0.5f) {
                return (t * 2.0f) - 0.5f;
            } else {
                return 0.5f - (t - 0.5f) * 2.0f;
            }
        default:
             return 0.0f;
    }
}

/* This function gets called by pspaudiolib every time the
   audio buffer needs to be filled. The sample format is
   16-bit, stereo. */
void audioCallback(void* buf, unsigned int length, void *userdata) {
    const float sampleLength = 1.0f / sampleRate;
    const float scaleFactor = SHRT_MAX - 1.0f;
    static float freq0 = 440.0f;
    sample_t* ubuf = (sample_t*) buf;
    int i;
    
    if (frequency != freq0) {
            currentTime *= (freq0 / frequency);
    }
    for (i = 0; i &lt; length; i++) {
        short s = (short) (scaleFactor * currentFunction(2.0f * PI * frequency * currentTime));
        ubuf[i].l = s;
        ubuf[i].r = s;
        currentTime += sampleLength;
    }
    if (currentTime * frequency &gt; 1.0f) {
        double d;
        currentTime = modf(currentTime * frequency, &d) / frequency;
    }

    freq0 = frequency;
}

/* Read the analog stick and adjust the frequency */
void controlFrequency(void) {
    static int oldButtons = 0;
    const int zones[6] = {30, 70, 100, 112, 125, 130};
    const float response[6] = {0.0f, 0.1f, 0.5f, 1.0f, 4.0f, 8.0f};
    const float minFreq = 32.0f;
    const float maxFreq = 7040.0f;
    SceCtrlData pad;
    float direction;
    int changedButtons;
    int i, v;

    sceCtrlReadBufferPositive(&pad, 1);

    v = pad.Ly - 128;
    if (v &lt; 0) {
           direction = 1.0f;
        v = -v;
    } else {
        direction = -1.0f;
    }

    for (i = 0; i &lt; 6; i++) {
        if (v &lt; zones[i]) {
            frequency += (response[i] * direction);
            break;
        }
    }

    if (frequency &lt; minFreq) {
        frequency = minFreq;
    } else if (frequency &gt; maxFreq) {
        frequency = maxFreq;
    }

    changedButtons = pad.Buttons & (~oldButtons);
    if (changedButtons & PSP_CTRL_CROSS) {
        function++;
        if (function &gt; 2) {
            function = 0;
        }
    }

    oldButtons = pad.Buttons;
}

int main(void) {
    pspDebugScreenInit();
    setupCallbacks();

    pspAudioInit();
    pspAudioSetChannelCallback(0, audioCallback, NULL);

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    printf("Press up and down to select frequency\nPress X to change function\n");
    
    while(1) {
        sceDisplayWaitVblankStart();
        pspDebugScreenSetXY(0,2);
        printf("freq = %.2f \n", frequency);

        switch(function) {
            case 0:
                printf("SINE WAVE. \n");
                break;
            case 1:
                  printf("SQUARE WAVE. \n");
                break;
            case 2:
                  printf("TRIANGLE WAVE. \n");
                break;
        }

        controlFrequency();
    }

    return 0;
}
</pre>

<b>CMakeLists.txt</b>:

<pre>
cmake_minimum_required(VERSION 3.0)

project(audio)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pspdebug
    pspdisplay
    pspge
    pspctrl
    pspaudio
    pspaudiolib
    psputility
)

# Create an EBOOT.PBP file
create_pbp_file(
    TARGET ${PROJECT_NAME}
    ICON_PATH NULL
    BACKGROUND_PATH NULL
    PREVIEW_PATH NULL
    TITLE ${PROJECT_NAME}
)
</pre>

Building can be done with:

<pre>
mkdir build && cd build
psp-cmake ..
make
</pre>

<p>This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.</p>

More audiolib examples can be found <a href="https://github.com/pspdev/pspsdk/tree/master/src/samples/audio">here</a>.

</details></p>

### Using SDL2

![](images/sdl2.png?raw=true)

SDL2 is a library which handles system specific things like input, audio and window management for you. It can also be used to render shapes and images, just like the native libgu. This will be slower, but will result in code that can be run more easily on multiple platforms. Click on details below for the to see the code and how to build it.

<p><details>

<b>main.c</b>:

<pre>
#include &lt;SDL.h&gt;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

    SDL_Window * window = SDL_CreateWindow(
        "window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        480,
        272,
        0
    );

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect square = {216, 96, 34, 64}; 

    int running = 1;
    SDL_Event event;
    while (running) { 
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    SDL_GameControllerOpen(event.cdevice.which);
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    if(event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
                        running = 0;
                    break;
            }
        }

        // Clear the screen
        SDL_RenderClear(renderer);

        // Draw a red square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &square);

        // Draw everything on a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderPresent(renderer);
    }

    return 0;
}
</pre>

<b>CMakeLists.txt</b>:

<pre>
cmake_minimum_required(VERSION 3.0)

project(square)

add_executable(${PROJECT_NAME} main.c)

find_package(SDL2 REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
)

if(PSP)
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
    )
endif()
</pre>

Building can be done with:

<pre>
mkdir build && cd build
psp-cmake ..
make
</pre>

<p>This will result in an EBOOT.PBP` file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.</p>

If you have sdl2 dev package and a compiler installed this code will also build on Linux for Linux by running:

<pre>
mkdir build && cd build
cmake ..
make
</pre>

More documentation on SDL can be found <a href="http://wiki.libsdl.org/FrontPage">here</a>.

</details></p>

### Using SDL2 mixer

![](images/sdl2_mixer.png?raw=true)

This is a simple program to use the SDL2_mixer library. Click on details below to see the code and how to build it.

<p><details>

<b>main.c</b>:

<pre>
#include &lt;SDL2/SDL.h&gt;
#include &lt;SDL2/SDL_mixer.h&gt;

// Define MIN macro
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
#define SCREEN_WIDTH    480
#define SCREEN_HEIGHT   272

// audio file path
#define MUSIC_PATH "ms0:/MUSIC/test.ogg" // ogg/mp3 file format

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // Initialize sdl
    SDL_Init(SDL_INIT_VIDEO |
        SDL_INIT_AUDIO |
        SDL_INIT_GAMECONTROLLER
    );

    // Initialize sdl2_mixer
    Mix_OpenAudio(44100, 
        MIX_DEFAULT_FORMAT, 
        MIX_DEFAULT_CHANNELS, 
        2048
    );

    // create window
    SDL_Window *win = SDL_CreateWindow(
        "psp_win",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    // Create Renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
        win, -1, 0
    );

    // Load ogg file
    Mix_Music *ogg_file = NULL;
    ogg_file = Mix_LoadMUS(MUSIC_PATH);
    if (!ogg_file) {
        return 0;
    }

    SDL_Rect rect;

    // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
    rect.w = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;
    rect.h = MIN(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

    // Square position: In the middle of the screen
    rect.x = SCREEN_WIDTH / 2 - rect.w / 2;
    rect.y = SCREEN_HEIGHT / 2 - rect.h / 2;


    // Declare rects of pause symbol
    SDL_Rect pause_rect1, pause_rect2;

    pause_rect1.h = rect.h / 2;
    pause_rect1.w = 40;
    pause_rect1.x = rect.x + (rect.w - pause_rect1.w * 3) / 2;
    pause_rect1.y = rect.y + rect.h / 4;
    pause_rect2 = pause_rect1;
    pause_rect2.x += pause_rect1.w * 2;
    
    // play the music 8 times
    if (Mix_PlayMusic(ogg_file, 8) == -1) {
        return 0;
    }

    int running = 1;
    SDL_Event e;
    while (running) {
        if(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    running = 0;
                break;
                case SDL_CONTROLLERDEVICEADDED:
                    SDL_GameControllerOpen(e.cdevice.which);
                break;
                case SDL_CONTROLLERBUTTONDOWN:
                    // pause using cross button
                    if (e.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
                        Mix_PauseMusic();
                    // resume using circle button
                    } else if (e.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
                        Mix_ResumeMusic();
                    }	
                    // press start button to exit
                    if (e.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
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
        if(Mix_PausedMusic()) {
            // Set renderer color black to draw the pause symbol
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

            // Draw pause symbol
            SDL_RenderFillRect(renderer, &pause_rect1);
             SDL_RenderFillRect(renderer, &pause_rect2);
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    Mix_FreeMusic(ogg_file);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
</pre>

<b>CMakeLists.txt</b>:

<pre>
cmake_minimum_required(VERSION 3.0)

project(sdl2_mixer)

add_executable(${PROJECT_NAME} main.c)

find_package(SDL2 REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
)

if(PSP)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2_mixer
        SDL2
        vorbisfile
        vorbis
        ogg
        xmp
    )
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
    )
endif()
</pre>

Building can be done with:

<pre>
mkdir build && cd build
psp-cmake ..
make
</pre>

<p>This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and you need an audio file to test the program, download it from <a href="/resources/test.ogg">here</a>. Put it in a directory in ms0:/MUSIC/ and then rename the audio file same as name on your <i>MUSIC_PATH</i> macro in your C code and the PSP can run it.</p>

</details></p>

### Using SDL2 ttf

![](images/sdl2_ttf.jpg?raw=true)

This is a simple program to use the SDL2_ttf library. Click on details below to see the code and how to build it.

<p><details>

<b>main.c</b>:

<pre>
#include &lt;stdio.h&gt;

#include &lt;SDL2/SDL.h&gt;
#include &lt;SDL2/SDL_ttf.h&gt;

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
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    if (!win)
    {
        printf("Window could not be created!\n"
               "SDL_Error: %s\n",
               SDL_GetError());
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, 0);
    TTF_Font *font = TTF_OpenFont("Pacifico.ttf", 40);

    // Set the text and background color
    SDL_Color text_color = {0xff, 0xff, 0xff, 0xff};
    SDL_Color bg_color = {0x00, 0x00, 0x00, 0xff};

    SDL_Rect text_rect;
    SDL_Surface *surface = TTF_RenderText(font, "Hello World!", text_color, bg_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Get text dimensions
    text_rect.w = surface->w;
    text_rect.h = surface->h;

    SDL_FreeSurface(surface);

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
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_CONTROLLERDEVICEADDED:
                SDL_GameControllerOpen(e.cdevice.which);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                if (e.cbutton.button == SDL_CONTROLLER_BUTTON_START)
                {
                    running = 0;
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
        SDL_RenderCopy(renderer, texture, NULL, &text_rect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
</pre>

<b>CMakeLists.txt</b>:

<pre>
cmake_minimum_required(VERSION 3.0)

project(text-ttf)

add_executable(${PROJECT_NAME} main.c)

find_package(SDL2 REQUIRED)

target_include_directories(${PROJECT_NAME} 
    PRIVATE ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
)

if(PSP)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2_ttf
        freetype
        m
        bz2
        png16
        z
    )
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
    )
endif()
</pre>

Building can be done with:

<pre>
mkdir build && cd build
psp-cmake ..
make
</pre>

<p>This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and you need a font file to test the program, download it from <a href="/resources/Pacifico.ttf">here</a>. Put it in a directory same as EBOOT.PBP and the PSP can run it.</p>

</details></p>

## Tips and Tricks

Here some useful tips for developing for the PSP.

### Making Programs Work on Unmodded PSPs

The PSPDEV toolchain contains tools for making your program work on unmodded PSPs. This can be done by running psp-cmake with some additional commands when building like so:

```
mkdir build && cd build
psp-cmake -DBUILD_PRX=1 -DENC_PRX=1 ..
make
```

This does require `create_pbp_file` to be used in your CMakeLists.txt file. After the first build, running `make` is enough to get an `EBOOT.PBP` file which works on official firmware with any new changes made to the code.

### Add PSP Specific Code to a Multi-Platform Programs

When porting a game to the PSP, some PSP specific code might be needed. To make this code only apply to PSP, it is possible to use a simple ifdef statements which checks for `__PSP__` like so:
 
```
#ifdef __PSP__
    // Do PSP specific thing
#else
    // Do the thing all other systems should do
#endif
```

This makes sure that the other systems supported by the program keeps working the same, while still making it possible to add support for the PSP.

## More Libraries

There are many C and C++ libraries available within the PSPDEV toolchain which can add functionality to your program. Some examples:

- Audio formats: mp3, ogg
- Image formats: png, jpeg
- Data formats: json, yaml, sqlite
- Support for compression, physics, fonts and much more


For the full list take a look at the [psp-packages repository](https://github.com/pspdev/psp-packages) or run `psp-pacman -Syl`. Updating libraries can be done with `psp-pacman -Syu`.

## Contact

If you need help or would like to contribute, don't hesitate to join us on [Discord](https://discord.gg/bePrj9W) or open an issue on [GitHub](https://github.com/pspdev/pspdev/issues). See you there!

