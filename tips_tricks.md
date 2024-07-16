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

## Managing Libraries
{: .fs-6 .fw-700 }

There are many C and C++ libraries available within the PSPDEV toolchain and most of them will be installed by default. Libraries are where most of the updates within the toolchain happen and they can be updated manually without redownload the toolchain using `psp-pacman`.

Updating the libraries installed can be done with the following command:

```shell
psp-pacman -Syu
```

After updating, you can list all available libraries with the following command:

```shell
psp-pacman -Sl
```

For a full list with information on each library, it's easiest to take a look on the [repository's web pages](https://pspdev.github.io/psp-packages).

Installing a library can be done with the following command:

```shell
psp-pacman -S library
```
