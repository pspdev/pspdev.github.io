---
title: Quickstart
layout: home
nav_order: 2
---

# Quickstart
{: .fs-10 .fw-700 .text-center }

## Installing the toolchain
{: .fs-8 .fw-700 }

### Dependencies
{: .fs-6 .fw-700 }

The PSPDEV toolchain requires a couple of dependencies to be installed before use.

#### Windows(WSL)
{: .fs-4 .fw-700 }

On Windows it is advised to use Ubuntu on Microsoft's WSL system. To set it up run the following commands in a Powershell window started as administrator:

```powershell
wsl --install
```

Now run bash and run the following commands to install the dependencies:

```shell
sudo apt-get update
```

```shell
sudo apt-get install build-essential cmake pkgconf libreadline8 libusb-0.1 libgpgme11 libarchive-tools fakeroot
```

Going forwards, all commands will be run within bash. The filesystem of your Ubuntu installation can be accessed by opening the network location `\\wsl$`.

#### Ubuntu
{: .fs-4 .fw-700 }

On Ubuntu run the following commands to install the dependencies:

```shell
sudo apt-get update
```

```shell
sudo apt-get install build-essential cmake pkgconf libreadline8 libusb-0.1 libgpgme11 libarchive-tools fakeroot
```

#### MacOS
{: .fs-4 .fw-700 }

On MacOS make sure to install [Brew](https://brew.sh/) first, then run the following commands to install the dependencies:

```shell
brew install cmake pkgconf gnu-sed bash openssl libtool libarchive gettext texinfo bison flex isl gsl gmp mpfr
```


### Toolchain 
{: .fs-6 .fw-700 }

To install the PSPDEV toolchain, first [download the latest version](https://github.com/pspdev/pspdev/releases/tag/latest) for your system. Extract it into your user's home directory, which would be `\\wsl$\home\username` on Windows, otherwise `~`. If you're on Windows, pick the Ubuntu build.

Now set the required environment variables. On Mac edit the ``~/.zprofile`` on Linux/WSL the ``~/.bashrc`` file. Add the following at the bottom:

```shell
export PSPDEV=~/pspdev
```

```shell
export PATH=$PATH:$PSPDEV/bin
```

This can simply be done with `nano ~/.bashrc` (`~/.zprofile` on Mac). To save press `Ctrl`+`X`, then press `Y`, then enter/return twice. After that run `source ~/.bashrc` to apply the changes.

On MacOS, run `sudo xattr -rd com.apple.quarantine $PSPDEV` to prevent Gatekeeper from blocking the toolchain.

That's it, now the PSPDEV toolchain can be used to build PSP software. Below is the sample code and how to run it. For testing on real hardware, check out [psplink](https://pspdev.github.io/psplinkusb/).

## Coding
{: .fs-8 .fw-700 }

### Hello world
{: .fs-6 .fw-700 }

![](images/hello.png?raw=true)

> This is a simple Hello World program for the PSP. 

Click on the details below to see the code and how to build it.

<details markdown="1">

<summary>View source</summary>

**main.c**

```c

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

// PSP_MODULE_INFO is required
PSP_MODULE_INFO("Hello World", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}

int callback_thread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setup_callbacks(void) {
    int thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if(thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

int main(void)  {
    // Use above functions to make exiting possible
    setup_callbacks();
    
    // Print Hello World! on a debug screen on a loop
    pspDebugScreenInit();
    while(1) {
        pspDebugScreenSetXY(0, 0);
        pspDebugScreenPrintf("Hello World!");
        sceDisplayWaitVblankStart();
    }

    return 0;
}
```

**CMakeLists.txt**

```cmake
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
```

Building can be done with:

```shell
mkdir build && cd build
psp-cmake ..
make
```

This will result in an EBOOT.PBP file in the build directory. Put it in a directory in ms0:/PSP/GAME/ and the PSP can run it.

</details>

For more example take a look at the [Basic Programs](basic_programs.html) page.
