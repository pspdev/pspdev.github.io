---
title: Basic Programs
layout: home
nav_order: 4
---

# Basic Programs
{: .fs-8 .fw-700 .text-center }

## Hello world
{: .fs-6 .fw-700 }

![](images/hello.png)

This is a simple Hello World program for the PSP.

Click on view source below to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c
{% include samples/hello/main.c %}
```


**CMakeLists.txt**

```cmake
{% include samples/hello/CMakeLists.txt %}
```

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.

</details>

## Drawing shapes
{: .fs-6 .fw-700 }

![](images/shape.png)

This is a simple square drawn on the PSP. It uses the native libgu library. 

Click on view source below to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c
{% include samples/shape/main.c %}
```

**CMakeLists.txt**

```cmake
{% include samples/shape/CMakeLists.txt %}
```

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

<p>This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.</p>

More libgu examples can be found <a href="https://github.com/pspdev/pspsdk/tree/master/src/samples/gu">here</a>.

</details>

## Drawing Sprites
{: .fs-6 .fw-700 }

![](images/sprite.png)

This is a sprite drawn on the PSP. It uses the native libgu library to draw and the stb_image library to load the image. stb_image supports a lot of image formats. For the Playstation Portable it is important that the width of images used is a power of 2. In this case 16, but 32, 64, 128 and 256 will also work. This limitation does not exist for the example with SDL2 below.

Click on view source below to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c
{% include samples/sprite/main.c %}
```

**CMakeLists.txt**

```cmake
{% include samples/sprite/CMakeLists.txt %}
```

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and add the grass image file, download it from <a href="/resources/grass.png">here</a>, to be able to run it on the PSP. 

More libgu examples can be found <a href="https://github.com/pspdev/pspsdk/tree/master/src/samples/gu">here</a>.

</details>

## Input
{: .fs-6 .fw-700 }

![](images/controls.png)

This is a simple program to use the PSP's input functions.

Click on view source below to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c
{% include samples/controls/main.c %}
```

**CMakeLists.txt**

```cmake
{% include samples/controls/CMakeLists.txt %}
```

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

<p>This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.</p>

</details>

## Audio
{: .fs-6 .fw-700 }

![](images/audio.png)

This is a simple program to use the audio of the PSP with minimal effort. It uses native audio library. 

Click on view source below to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c
{% include samples/audio/main.c %}
```

**CMakeLists.txt**

```cmake
{% include samples/audio/CMakeLists.txt %}
``````

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

<p>This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.</p>

More audiolib examples can be found <a href="https://github.com/pspdev/pspsdk/tree/master/src/samples/audio">here</a>.

</details>

## Using SDL2
{: .fs-6 .fw-700 }

![](images/shape.png)

SDL2 is a library which handles system specific things like input, audio and window management for you. It can also be used to render shapes and images, just like the native libgu. This can be slower, but will result in code that is easier to read and can run on multiple platforms.

Despite this example adding an option to close by pressing the start button, the code is much shorter. It can even be build for Linux without any further modifications.

Click on view source below for the to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c
{% include samples/sdl2/main.c %}
```

**CMakeLists.txt**

```cmake
{% include samples/sdl2/CMakeLists.txt %}
``````

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

<p>This will result in an EBOOT.PBP` file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.</p>

If you have sdl2 dev package and a compiler installed this code will also build on Linux for Linux by running:

```shell
mkdir build && cd build
cmake ..
make
```

More documentation on SDL can be found <a href="http://wiki.libsdl.org/FrontPage">here</a>.

</details>

## Using SDL2 Image
{: .fs-6 .fw-700 }

![](images/sprite.png)

SDL2 image is a library which adds support for multiple image formats to SDL2. This example results in the same image as the sprite example using libgu above. Here the limitation of the image width being a power 2 does not apply.

Despite this example adding an option to close by pressing the start button, the code is much shorter. It can even be build for Linux without any further modifications.

Click on view source below to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c
{% include samples/sdl2_image/main.c %}
```

**CMakeLists.txt**

```cmake
{% include samples/sdl2_image/CMakeLists.txt %}
```

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and add the grass image file, download it from <a href="/resources/grass.png">here</a>, to be able to run it on the PSP. 

If you have sdl2 sdl2-image dev packages and a compiler installed this code will also build on Linux for Linux by running:

```shell
mkdir build && cd build
cmake ..
make
```

Documentation for SDL2_image can be found <a href="https://wiki.libsdl.org/SDL2_image/FrontPage">here</a>.

</details>


## Using SDL2 mixer
{: .fs-6 .fw-700 }

![](images/sdl2_mixer.png)

This is a simple program to use the SDL2_mixer library. It handle audio playback in multimedia applications and games. It supports various audio formats(MP3/OGG).

Click on view source below to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c
{% include samples/sdl2_mixer/main.c %}
```

**CMakeLists.txt**

```cmake
{% include samples/sdl2_mixer/CMakeLists.txt %}
``````

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and you need an audio file to test the program, download it from <a href="/resources/test.ogg">here</a>. Put it in a directory in ms0:/MUSIC/ and then rename the audio file same as name on your *MUSIC_PATH* macro in your C code and the PSP can run it.

Documentation for SDL2_mixer can be found <a href="https://wiki.libsdl.org/SDL2_mixer/FrontPage">here</a>.

</details>

## Using SDL2 ttf
{: .fs-6 .fw-700 }

![](images/sdl2_ttf.jpg)

This is a simple program to use the SDL2_ttf library. It provides functionality for rendering TrueType fonts for your PSP.

Click on view source below to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c
{% include samples/sdl2_ttf/main.c %}
```

**CMakeLists.txt**

```cmake
{% include samples/sdl2_ttf/CMakeLists.txt %}
``````

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and you need a font file to test the program, download it from <a href="/resources/Pacifico.ttf">here</a>. Put it in a directory same as EBOOT.PBP and the PSP can run it.

Documentation for SDL2_ttf can be found <a href="https://wiki.libsdl.org/SDL2_ttf/FrontPage">here</a>.

</details>

## More Examples
{: .fs-6 .fw-700 }

More examples on how to use specific functions offered in the PSPDEV toolchain can be found [here](https://github.com/pspdev/pspsdk/tree/master/src/samples). Additional documentation on these functions can be found [here](https://pspdev.github.io/pspsdk/).

## Debugging
{: .fs-6 .fw-700 }

When making changes to the example programs listed above, you might run into errors or even crashes. To learn how to figure out what went wrong, check out the [debugging](debugging.html) page.
