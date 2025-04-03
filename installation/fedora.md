---
title: Installation on Fedora
layout: home
nav_exclude: true
---

# Installation on Fedora
{: .fs-8 .fw-700 .text-center }

## Dependencies
{: .fs-6 .fw-700 }

The PSP SDK requires a couple of dependencies to be installed before use. To install them, run the following command from a terminal:

```shell
sudo dnf -y install @development-tools cmake bsdtar libusb-compat-0.1 gpgme fakeroot xz nano
```

## Toolchain 
{: .fs-6 .fw-700 }

Installing the PSP SDK itself can be done with the following steps:

1. Download [the latest version of the toolchain here](https://github.com/pspdev/pspdev/releases/latest/download/pspdev-fedora-latest.tar.gz).
2. Extract the downloaded archive into your home directory, resulting in `/home/YOURUSERNAME/pspdev` being created.
3. To make the toolchain usable, some environment variables need to be set. The first step in doing so it to open the `~/.bashrc` file with the `nano` text editor using the following command from a terminal:
    ```shell
    nano ~/.bashrc
    ```
4. Add the following lines at the bottom of the file in the text editor:
    ```shell
    export PSPDEV="$HOME/pspdev"
    export PATH="$PATH:$PSPDEV/bin"
    ```
5. Now save and exit by pressing `Ctrl`+`X`, then `Y` and then enter/return.
6. Close the current terminal and open a new one.
7. From the new terminal, run the following command to confirm everything is set up correctly:
    ```shell
    psp-config --pspdev-path
    ```

That's it, now the PSP SDK can be used to build PSP software. Check out the [How to Use](../how_to_use.html) page for a guide on how to do so.
