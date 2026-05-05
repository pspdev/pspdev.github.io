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

The stub library only contains the Module Name, the NID (Native ID?) of the function and a stub implementation of the function.   

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

### Write the Export Table
{: .fs-4 .fw-700 }

Create a file with an `.exp` extension. It is used to declare which functions are accessible from external modules. The format is as follows:

> Guidelines
> 1. `syslib` is an essential keyword that must be included in the export table. 
> 2. It is generally not recommended to export global variables. (TODO: Reference needed)

Here is an example export table (you can view it in the [SDK source code](https://github.com/pspdev/pspsdk/blob/master/src/samples/prx/testprx/exports.exp)):

```
# Define the exports for the prx
PSP_BEGIN_EXPORTS

# These four lines are mandatory (although you can add other functions like module_stop)
# syslib is a psynonym for the single mandatory export.
PSP_EXPORT_START(syslib, 0, 0x8000)
PSP_EXPORT_FUNC_HASH(module_start)
PSP_EXPORT_VAR_HASH(module_info)
PSP_EXPORT_END

# Export our function
PSP_EXPORT_START(MyLib, 0, 0x0001)
PSP_EXPORT_FUNC_HASH(getModuleInfo)
PSP_EXPORT_END

PSP_END_EXPORTS
```

### Compile the PRX Module
{: .fs-4 .fw-700 }

If you are using Makefiles, you can add the following lines to your Makefile to compile the PRX module:

```makefile
# Define to build this as a prx (instead of a static elf)
BUILD_PRX=1
# Define the name of our custom exports (minus the .exp extension)
PRX_EXPORTS=exports.exp
```

When `PRX_EXPORTS` is defined, the build system calls `psp-build-exports -b` and automatically generates the code to compile into the PRX module.

> The `.rodata.sceResident` segment contains the exported NIDs and function addresses.
> 
> The `.lib.ent` segment contains the module name and export table metadata, including a pointer to the export table in the `.rodata.sceResident` segment.
> 
> You can inspect the generated code by running `psp-build-exports -b exports.exp` in your terminal.

### Generate the Stub Library
{: .fs-4 .fw-700 }

To generate the stub library, first run the following command in the terminal:

```sh
psp-build-exports -k my_lib.exp
```

This generates a stub file in `.S` assembly format. This file can be compiled into a static library using `psp-gcc` followed by `psp-ar`, or distributed directly. When others want to use your PRX module, they can simply include this `.S` file as a source file.

## Dynamically Loading PRX Modules
{: .fs-6 .fw-700 }

To dynamically load a PRX module, you can use the `sceKernelLoadModule` and `sceKernelStartModule` functions.

When building your programs, include the generated `.S` stub file (or the `.a` stub library) in your `OBJS` or `LIBS` for linking.

The generated `.S` file uses several macros along with the post-processing tool `psp-fixup-imports` to construct the import table.

## Further Reading
{: .fs-6 .fw-700 }

- [PSP Modules, Exports, Imports and Patches](https://uofw.github.io/upspd/docs/software/ModuleTutorialv1.pdf): A short tutorial released in 2007 describing modules used in Sony’s PSP by Anissian.