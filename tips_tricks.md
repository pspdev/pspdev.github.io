---
title: Tips and Tricks
layout: home
nav_order: 5
---

# Tips and Tricks
{: .fs-8 .fw-700 .text-center }

Here some useful tips for developing for the PSP.

## Making Programs Work on Unmodded PSPs
{: .fs-6 .fw-700 }

The PSPDEV toolchain contains tools for making your program work on unmodded PSPs. This can be done by running psp-cmake with some additional commands when building like so:

```shell
mkdir build && cd build
```

```shell
psp-cmake -DBUILD_PRX=1 -DENC_PRX=1 .. && make
```


This does require `create_pbp_file` to be used in your CMakeLists.txt file. After the first build, running `make` is enough to get an `EBOOT.PBP` file which works on official firmware with any new changes made to the code.

## Add PSP Specific Code to a Multi-Platform Programs
{: .fs-6 .fw-700 }

When porting a game to the PSP, some PSP specific code might be needed. To make this code only apply to PSP, it is possible to use a simple ifdef statements which checks for `__PSP__` like so:
 
```c
#ifdef __PSP__
    // Do PSP specific thing
#else
    // Do the thing all other systems should do
#endif
```

This makes sure that the other systems supported by the program keeps working the same, while still making it possible to add support for the PSP.

## More Libraries
{: .fs-6 .fw-700 }

There are many C and C++ libraries available within the PSPDEV toolchain which can add functionality to your program. Some examples:

- Audio formats: mp3, ogg
- Image formats: png, jpeg
- Data formats: json, yaml, sqlite
- Support for compression, physics, fonts and much more


For the full list take a look at the [psp-packages repository](https://github.com/pspdev/psp-packages) or run `psp-pacman -Syl`. Updating libraries can be done with `psp-pacman -Syu`.
