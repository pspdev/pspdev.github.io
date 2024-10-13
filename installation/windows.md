---
title: Installation on Windows
layout: home
nav_exclude: true
---

# Installation on Windows
{: .fs-8 .fw-700 .text-center }

## Windows Subsystem for Linux (WSL)
{: .fs-6 .fw-700 }

On Windows the PSPDEV toolchain is run on Ubuntu running on Microsoft's WSL. This is very easy to set up and will offer us the full power of Linux from a Windows machine.

To set up WSL with Ubuntu in it run the following commands in a Powershell window started as administrator (right click run as administrator on Powershell in the start menu):

```powershell
wsl --install
```

After this is done, Ubuntu can be selected from the start menu to open a terminal. This will be used for all commands going forward.

Files in Ubuntu can be accessed through a network share. In `This Computer` right click on the background and select `Map network drive`. Set the folder to `\\wsl$` to make it easily accessible.

## Dependencies
{: .fs-6 .fw-700 }

The PSPDEV toolchain requires a couple of dependencies to be installed before use. To install them, run the following command from an Ubuntu terminal:

```shell
sudo apt-get update
```

```shell
sudo apt-get install build-essential cmake pkgconf libreadline8 libusb-0.1 libgpgme11 libarchive-tools fakeroot curl
```

## Toolchain 
{: .fs-6 .fw-700 }

Installing the PSPDEV toolchain itself can be done with the following steps:

1. In a fresh WSL Session download the Toolchain Archive using curl `curl -O https://github.com/pspdev/pspdev/releases/latest/download/pspdev-ubuntu-latest-x86_64.tar.gz`
2. Extract the archive using `tar -xvf pspdev-ubuntu-latest-x86_64.tar.gz`
3. To make the toolchain usable, some environment variables need to be set. The first step in doing so it to open the `~/.bashrc` file with the `nano` text editor using the following command from an Ubuntu terminal:
    ```shell
    nano ~/.bashrc
    ```
4. Add the following lines at the bottom of the file in the text editor:
    ```shell
    export PSPDEV="$HOME/pspdev"
    export PATH="$PATH:$PSPDEV/bin"
    ```
5. Now save and exit by pressing `Ctrl`+`X`, then `Y` and then enter/return.
6. Close the current Ubuntu terminal and open a new one.
7. From the new Ubuntu terminal, run the following command to confirm everything is set up correctly:
    ```shell
    psp-config --pspdev-path
    ```

That's it, now the PSPDEV toolchain can be used to build PSP software. Check out the [Basic Programs](../basic_programs.html) page to for examples on what you can do with it.
