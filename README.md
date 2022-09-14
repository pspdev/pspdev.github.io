PSPDEV is an open source toolchain for Playstation Portable development. It allows you to make apps and games for both custom and official firmwares. This is a community project made by enthusiasts, it is in no way affiliated with Sony.

# Getting started

## Installing

### dependencies

The PSPDEV toolchain requires a couple of dependencies to be installed before use.

#### Ubuntu

On Ubuntu run the following command to install the dependencies:

```
sudo apt-get update
sudo apt-get install build-essential cmake pkgconf libreadline8 libusb-0.1 libpython3.8 libgpgme11
```

### Toolchain 

To install the PSPDEV toolchain, first [download the latest version](https://github.com/pspdev/pspdev/releases/tag/latest) for your system. Extract it into your user's home directory.

Now set the required environment variables. On Mac edit the ``~/.bash_profile`` on Linux the ``~/.bashrc`` file. Add the following at the bottom:

```
export PSPDEV=~/pspdev
export PATH=$PATH:$PSPDEV/bin
```

That's it, now the PSPDEV toolchain can be used to build PSP software.

## Basic programs

Below are some basic examples of programs for the PSP. More can be found [here](https://github.com/pspdev/pspsdk/tree/master/src/samples).

### Hello world

![](images/hello.png?raw=true)


This is a simple Hello World program for the PSP. Click on details below for the to see the code and how to build it.

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

This is a simple a simple square drawn on the PSP. It uses the native libgu library. Click on details below for the to see the code and how to build it.

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

find_package(PkgConfig REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
)

if(PSP)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL2main
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

<p>This will result in an EBOOT.PBP` file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.</p>

If you have sdl2 dev package and a compiler installed this code will also build on Linux for Linux by running:

<pre>
mkdir build && cd build
cmake ..
make
</pre>

More documentation on SDL can be found <a href="http://wiki.libsdl.org/FrontPage">here</a>.

</details></p>

## Libraries

There are many C and C++ libraries available within the PSPDEV toolchain which can add functionality to your program. Some examples:

- Audio formats: mp3, ogg
- Image formats: png, jpeg
- Data formats: json, yaml, sqlite
- Support for compression, physics, fonts and much more


For the full list take a look at the [psp-packages repository](https://github.com/pspdev/psp-packages).

## Contact

If you need help or would like to contribute, don't hesitate to join us on [Discord](https://discord.gg/bePrj9W) or open an issue on [GitHub](https://github.com/pspdev/pspdev/issues). See you there!

