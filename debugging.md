---
title: Debugging
layout: home
nav_order: 4
---

# Debugging
{: .fs-8 .fw-700 .text-center }

When developing for the Playstation Portable (PSP), you may run into crashes or code that does not quite work like expected. Figuring out what is going in is called debugging. This page will cover how to do that.

## PSPLINK
{: .fs-6 .fw-700 }

PSPLINK is THE tool to use for testing code on the PSP. It is an application which allows you to run and debug programs for the PSP from your PC through USB without requiring data to be transferred to the PSP. Setting it up for use requires different action for the PSP and the PC.

### Setting PSPLINK up on the PSP
{: .fs-4 .fw-700 }

Download the latest version of PSPLINK for the PSP [here](https://github.com/pspdev/psplinkusb/releases/download/latest/psplink.zip) and extract it in ``ms0:/PSP/GAME`` on the PSP memory card.

### Setting PSPLINK up on the PC
{: .fs-4 .fw-700 }

Depending on the operating system used the setup on PC is different. Pick the setup guide for your system here:

- [Windows](psplink/windows.html)
- [Linux](psplink/linux.html)

MacOS should not require any additional setup.

### Using PSPLINK
{: .fs-4 .fw-700 }

To be able to use PSPLINK with Playstation Portable homebrew, the homebrew will need to be build into an unencrypted ``.prx`` file. This can be done by running CMake like `psp-cmake -DBUILD_PRX=1 .` or if you're using a Makefile by adding `BUILD_PRX=1` to it. Then build the homebrew.

After having done all of the above, PSPLINK can be used using the following steps:

1. Connect the PSP with a USB cable to the computer.
2. Launch the PSPLINK homebrew on the PSP. The PSP is now waiting for input from the PC.
3. In the build directory of the homebrew you wish to test, open a terminal and run the following:
    ```sh
    usbhostfs_pc
    ```
    Keep this running!
4. Then open another terminal window and run the following there:
    ```sh
    pspsh
    ```
5. Now you can simply start our homebrew on the PSP by running the following command in the pspsh window:
    ```sh
    ./myhomebrew.prx
    ```
    Replace myhomebrew with the name of the `.prx` file which was generated. Now the PSP should be running your project!

When you're done with the current build, just run `reset`, rebuild the homebrew and try again.

Options available can be found when using the `help` command, but here are some notable ones:

- `scrshot screenshotname.bmp` for taking a screenshot.
- `exit` for closing PSPLINK on the PSP.
- `poweroff` for shutting down the PSP.

### Getting Basic Crash Information
{: .fs-4 .fw-700 }

When a crash happens a crash log will be shown with a hint of what might have happened at the top and some additional info. If you wish to figure out where the crash happened, only the address is needed.

To figure out where the crash happened, open another terminal in the build directory and use the address shown by PSPLINK in the following command:

```sh
psp-addr2line -e myhomebrew address
```

Replace `address` with the actual adress and replace myhomebrew with the name of the elf file. This is **NOT** the `.prx` file and either has no extension or `.elf` depending on the build system used.

If no result is returned, make sure to build with the `-g` or `-g3` option and without the `-O` option to make sure `psp-addr2line` knowns the function names and locations.

The information received from `psp-addr2line` will be limited and not always useful, for more information you'll have to use a debugger as described below.

## Using a Debugger
{: .fs-6 .fw-700 }

When using `psp-addr2line` is not enough to figure out what is going on, the best way to debug will be by using an actual debugger called GDB, which comes bundled with the PSPDEV toolchain as the `psp-gdb` command. PSPLINK allows this debugger to connect directly to the PSP.

### Preparation
{: .fs-4 .fw-700 }

Prepare a separate terminal for `usbhostfs_pc`, `pspsh` and `psp-gdb`. Open all of them in the directory in which your compiled `.prx` and the `elf` (PSP binary) files are located.

#### 1. usbhostfs_pc
{: .fs-2 .fw-700 }

Run `usbhostfs_pc` on your terminal dedicated for `usbhostfs_pc` and you will see the `waiting for device...` status. 

Now start the PSPLINK app on your PSP and connect the USB cable. You should see the `connected to device` status in the terminal, which means success. 

**Do not close this terminal after that.**

#### 2. pspsh
{: .fs-2 .fw-700 }

Run `pspsh` on your terminal dedicated for `pspsh` and you will see the `host0:/>`. Now run `debug file.prx`, and it will display something like this:

```sh
PSPLink USB GDBServer (c) 2k7 TyRaNiD
host0:/> Loaded host0:/<file.prx> - UID 0x0408A763, Entry 0x088040AC
```

> You need to replace `file.prx`  with the file you need to debug. This is a `.prx` file.

It means the debugger is succesfully loaded. You can type `reset` if there's something wrong with your GDBServer.

#### 3. psp-gdb
{: .fs-2 .fw-700 }

In a new terminal run `psp-gdb file -q` and you will see something like this:

> You need to replace `file` with the elf file of the program you're trying to debug. It has the same name as the file loaded in pspsh, but without the `.prx` ending.

> Check if your binary has enabled the debug symbols required for debugging by using `objdump --syms` command and should produce an output but if it says `no symbols` then it is disabled(You can enable it by adding `-g` option to gcc).

```sh
Reading symbols from <file>...
(gdb)
```

> `<file>` is the name of the binary you are debugging.

then type the `target remote :10001` to connect to your GDBServer and you will see the gdb output something like this:

```sh
Remote debugging using :10001
_start (args=0, argp=0x0) at crt0_prx.c:103
103         if (&sce_newlib_nocreate_thread_in_start != NULL) {
(gdb)
```
This will display the `_start` routine, it means you succesfully connected and ready to the debug your app!

Here are a few useful commands for getting around in psp-gdb:
- `b` or `break` - for setting breakpoints
- `c` or `continue` - for resuming program execution until the next breakpoint or program completion
- `s` or `step` - for executing the current line and, if it contains a function call, step into that function
- `n` or `next` - for executing the current line, but if it contains a function call, step over it without diving into the function
- `f` or `finish` - for executing the remaining lines of the current function and return to the caller
- `bt` or `backtrace` - for getting stacktrace
- `p $var` or `print $var` - for displaying the value of specific variable
- `i r` or `info registers` - for displaying the contents of CPU registers
- `d` or `delete` - for deleting all breakpoints
- `q` or `quit` - for exiting from psp-gdb

You can type `help` for more information about the psp-gdb commands.

## Done
{: .fs-6 .fw-700 }

Now you know how to debug your code. Make sure to check out the [Tips and Tricks](tips_tricks.html) section to get most out of your PSP development experience.
