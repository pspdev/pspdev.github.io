---
title: Creating PRX Modules
layout: home
nav_exclude: true
---
# Creating PRX Modules
{: .fs-8 .fw-700 .text-center }

## Basic Concepts
{: .fs-6 .fw-700 }

Multiple modules can be loaded at the same time on the PSP. The main module is the one which is launched when starting a homebrew, but additional modules can be loaded and unloaded at runtime. These additional modules are called PRX modules and have the `.prx` file extension. They are similar to DLLs on Windows or shared objects on Linux.

### Stub Library
{: .fs-4 .fw-700 }

To load a PRX module, the main module needs to know which functions it can call within that module. This is done by creating a stub library, a static library containing the signatures of the functions in the PRX module. The main module links against this stub library and calls the functions in the PRX module just like regular functions.

The stub library only contains the Module Name, the NID (hash of the function name) of the function and a stub implementation of the function.

## Basic Workflow
{: .fs-6 .fw-700 }

> PRX modules can act as a Resident Library, which exposes their own functions inside and export them for other modules to use.

The core steps for creating a PRX module that others can load are as follows:
1. PRX module developers declare which functions are accessible from external modules by writing an **Export Table**. 
2. Users can establish a reference to these external functions in their modules by using an **Import Table**, which is generated from the PRX module's export table and can be compiled into a **Stub Library**.

## Writing Code and Export Table
{: .fs-6 .fw-700 }

### Writing Code
{: .fs-4 .fw-700 }

Writing code for a PRX module is largely identical to writing code for a regular module. The primary difference is the entry point.

Instead of writing a standard `main` function, you write a `module_start` function (called when the module is loaded) and a `module_stop` function (called when unloaded). These functions should return 0 on success and a negative value on failure.

> Make sure to use at least one function from other modules (for example, `sceIoOpen`), otherwise `psp-fixup-imports` will complain `Error, no .lib.stub section found`.

Also it is not recommended to use any Standard C Library functions in your code. The `newlib` implementation in the PSPSDK often causes issues when used in PRX modules, and it is generally better to avoid it altogether. Instead, you can use the PSPSDK's own functions with `sce` prefix for memory management, string manipulation, etc.

Due to similar reasons, C++ is also not recommended for PRX modules.

```c
#include <pspuser.h>

// Module Name, Attributes, Major Version, Minor Version
// This module is a user module
PSP_MODULE_INFO("SamplePlugin", PSP_MODULE_USER, 1, 0);
PSP_NO_CREATE_MAIN_THREAD();

int module_start() {
    SceUID fd = sceIoOpen("log.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_APPEND, 0777);
    if (fd >= 0) {
        sceIoWrite(fd, "Hello World!", 12);
        sceIoClose(fd);
    }
    return 0;
}

int module_stop() {
    SceUID fd = sceIoOpen("log.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_APPEND, 0777);
    if (fd >= 0) {
        sceIoWrite(fd, "Goodbye World!", 14);
        sceIoClose(fd);
    }
    return 0;
}

// This function will be exported and can be called from other modules
int sum(int a, int b) {
    return a + b;
}
```

### Write the Export Table
{: .fs-4 .fw-700 }

Create a file with an `.exp` extension. It is used to declare which functions are accessible from external modules. The format is as follows:

> Guidelines
> 1. `syslib` is an essential keyword that must be included in the export table. 
> 2. It is generally not recommended to export global variables.

Here is an example export table:

```
# Define the exports for the prx
PSP_BEGIN_EXPORTS

# These lines are mandatory
PSP_EXPORT_START(syslib, 0, 0x8000)
PSP_EXPORT_FUNC(module_start)
PSP_EXPORT_FUNC(module_stop)
PSP_EXPORT_VAR(module_info)
PSP_EXPORT_END

# Export our function
# 'MyLib' is the name of the library, it doesn't have to match the module name
PSP_EXPORT_START(MyLib, 0, 0x0001)
PSP_EXPORT_FUNC_HASH(sum)
PSP_EXPORT_END

PSP_END_EXPORTS
```

### Compile the PRX Module
{: .fs-4 .fw-700 }

If you are using Makefiles, you can add the following lines to your Makefile to compile the PRX module:

```makefile
# Define to build this as a prx
BUILD_PRX=1
# Define the name of our custom exports
PRX_EXPORTS=exports.exp
```

When `PRX_EXPORTS` is defined, the build system calls `psp-build-exports -b` and automatically generates the code to compile into the PRX module.

> The `.rodata.sceResident` segment contains the exported NIDs and function addresses.
> 
> The `.lib.ent` segment contains the module name and export table metadata, including a pointer to the export table in the `.rodata.sceResident` segment.
> 
> You can inspect the generated code by running `psp-build-exports -b exports.exp` in your terminal.

Also, when building a PRX module, you should add the `-nostartfiles` flag to your `LDFLAGS` to prevent the linker from including the startup files, which are not needed and can cause issues in PRX modules.

```makefile
LDFLAGS = -nostartfiles
```

The whole Makefile looks like this:

```makefile
TARGET = SamplePlugin

OBJS = main.o

CFLAGS = -O2 -Wall
LDFLAGS = -nostartfiles

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
```

If you prefer CMake, you use the following `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Export compile commands for better IDE support

# Set project name  
project(SamplePlugin)

# Define source files  
set(SOURCES main.c)


# Create PRX module with exports  
add_prx_module(SamplePlugin ${SOURCES} exports.exp)

# Set compiler flags for C files only  
target_compile_options(SamplePlugin PRIVATE -O2 -Wall)
set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE C)

# Link all required libraries  
target_link_libraries(SamplePlugin PRIVATE
    pspgu
    pspdisplay
    pspctrl
    pspge
)
```

### Generate the Stub Library
{: .fs-4 .fw-700 }

To generate the stub library, first run the following command in the terminal:

```sh
psp-build-exports -k exports.exp
```

This generates a stub file in `.S` assembly format. It's name is `MyLib.S` as the library name in the export table is `MyLib`.

```c
	.set noreorder

#include "pspimport.s"

// Build files
// MyLib_0000.o MyLib_0001.o 

#ifdef F_MyLib_0000
	IMPORT_START "MyLib",0x00090000
#endif
#ifdef F_MyLib_0001
	IMPORT_FUNC  "MyLib",0x55F3DFE9,sum
#endif
```

This file can be compiled into a static library using `psp-gcc` followed by `psp-ar`, or distributed directly. When others want to use your PRX module, they can simply include this `.S` file as a source file.

## Dynamically Loading PRX Modules
{: .fs-6 .fw-700 }

To dynamically load a PRX module, you can use the `sceKernelLoadModule` and `sceKernelStartModule` functions.

When building your programs, include the generated `.S` stub file (or the `.a` stub library) in your `OBJS` or `LIBS` for linking.

The generated `.S` file uses several macros along with the post-processing tool `psp-fixup-imports` to construct the import table.

## Further Reading
{: .fs-6 .fw-700 }

- [PSP Modules, Exports, Imports and Patches](https://uofw.github.io/upspd/docs/software/ModuleTutorialv1.pdf): A short tutorial released in 2007 describing modules used in Sony’s PSP by Anissian.