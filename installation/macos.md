---
title: Installation on MacOS
layout: home
nav_exclude: true
---

# Installation on MacOS
{: .fs-8 .fw-700 .text-center }

## Dependencies
{: .fs-6 .fw-700 }

The PSP SDK requires a couple of dependencies to be installed before use. Installing them can be done using [brew](https://brew.sh/).

Once brew is installed, run the following command from a terminal to install the dependencies:

```shell
brew install cmake pkgconf gnu-sed bash openssl libtool libmpc libarchive gettext texinfo bison flex isl gsl gmp mpfr
```

## Toolchain 
{: .fs-6 .fw-700 }

Installing the PSP SDK itself can be done with the following steps:

1. Download the latest version of the toolchain for your system here:
    - [arm64](https://github.com/pspdev/pspdev/releases/latest/download/pspdev-macos-latest-arm64.tar.gz) for M1 or newer CPUs.
    - [x86_64](https://github.com/pspdev/pspdev/releases/latest/download/pspdev-macos-13-x86_64.tar.gz) for Intel CPUs.
2. Extract the downloaded archive into your home directory, resulting in `/home/YOURUSERNAME/pspdev` being created.
3. To make the toolchain usable, some environment variables need to be set. The first step in doing so it to open the `~/.zprofile` file with the `pico` text editor using the following command from a terminal:
    ```shell
    pico ~/.zprofile
    ```
4. Add the following lines at the bottom of the file in the text editor:
    ```shell
    export PSPDEV="$HOME/pspdev"
    export PATH="$PATH:$PSPDEV/bin"
    ```
5. Now save and exit by pressing `Ctrl`+`X`, then `Y` and then enter/return.
6. Run the following command to remove the gatekeeper quarantine, allowing executables to be run:
    ```shell
    xattr -rd com.apple.quarantine $HOME/pspdev
    ```
7. Close the current terminal and open a new one.
8. From the new terminal, run the following command to confirm everything is set up correctly:
    ```shell
    psp-config --pspdev-path
    ```

That's it, now the PSP SDK can be used to build PSP software. Check out the [How to Use](../how_to_use.html) page for a guide on how to do so.
