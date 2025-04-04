---
title: Tips and Tricks
layout: home
nav_order: 6
---

# Tips and Tricks
{: .fs-8 .fw-700 .text-center }

Here some useful tips for developing for the PSP.

## Making Programs Work on Unmodded PSPs
{: .fs-6 .fw-700 }

The PSP SDK contains tools for making your program work on unmodded PSPs. This can be done by running psp-cmake with some additional commands when building like so:

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

There are many C and C++ libraries available within the PSP SDK and most of them will be installed by default. Libraries are where most of the updates within the SDK happen and they can be updated manually without redownload the SDK using `psp-pacman`.

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

## Managing Licenses
{: .fs-6 .fw-700 }

Every project made with the PSP SDK will import at least some libraries, each with their own license. Everything bundled is free to use, but some libraries will ask you to share their license with your project or in rare cases give users access to your code to respect their licenses. The `psp-create-license-directory` tool can be used to easily collect the licenses that apply to your project, so you can comply with them and ship them with your project.

To create a directory with a copy of the licenses that are always used in project made with the PSP SDK use the following command:

```
psp-create-license-directory
```

For any library you added, for example `sdl2` or `jsoncpp`, you can add the licenses by adding the library names to the command:

```
psp-create-license-directory sdl2 jsoncpp
```

If you're not sure what the library you used is called exactly, you can use the `-l` option to list the installed libraries:

```
psp-create-license-directory -l
```

## Updating the PSP SDK
{: .fs-6 .fw-700 }

A new version of the PSP SDK is released at least once a month, so updating should be done regularly if you want to benefit from new features and bug fixes.

To update the SDK, simply follow the installation instruction for your system on the [Installation](installation.html) page. Installing the dependencies can be skipped.
