---
title: Advanced Techniques
layout: home
nav_order: 7
---

# Advanced Techniques
{: .fs-8 .fw-700 .text-center }

When developing for the Playstation Portable (PSP), you may want to dynamically load code at runtime. This page will cover how to do that.

## Basic Concepts
{: .fs-6 .fw-700 }

Multiple modules can be loaded at the same time on the PSP. The main module is the one which is launched when starting a homebrew, but additional modules can be loaded and unloaded at runtime. These additional modules are called PRX modules and have the `.prx` file extension. They are similar to DLLs on Windows or shared objects on Linux.

### Stub Library
{: .fs-4 .fw-700 }

To be able to load a PRX module, the main module needs to know which functions it can call in the PRX module. This is done by creating a stub library, which is a static library that contains the function signatures of the functions in the PRX module. The main module can then link against this stub library and call the functions in the PRX module as if they were regular functions.

The stub library only contains the Module Name, the NID (Native ID?) of the function and a stub implementation of the function.   

## Basic Workflow
{: .fs-6 .fw-700 }

> PRX modules can act as a Resident Library, which exposes their own functions inside and export them for other modules to use.

The core steps for creating a PRX module that others can load are as follows:
1. PRX Module Developers declare which functions are accessible from external modules by **Writing** an **Export Table**. 
2. Target Users can establish a reference to the external functions in their own modules by using **Import Table**, which is generated from the PRX module's export table and can be further compiled into a **Stub Library**.

## Writing Code and Export Table
{: .fs-6 .fw-700 }

### Writing Code
{: .fs-4 .fw-700 }

When writing code for a PRX module, the code is mostly the same as writing code for a regular module. The main difference is that the entry point of the module is different.

When writing PRX module code, do not write regular `main` function, instead write an `module_start` function which will be called when the module is loaded and a `module_stop` function which will be called when the module is unloaded. These functions should return 0 on success and a negative value on failure.

### Write the Export Table
{: .fs-4 .fw-700 }

Create a file that ends with `.exp` extension. It is used to declare which functions are accessible from external modules. The format of the file is as follows:

> Guidelines
> 1. `syslib` is an essential keyword that must be included in the export table. 
> 2. It is said that Global Variables are not suggested to be exported. (TODO: Reference needed)

An example of an export table is as follows, you can check it out on the [SDK source code](https://github.com/pspdev/pspsdk/blob/master/src/samples/prx/testprx/exports.exp):

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

When PRX_EXPORTS is defined, the build system will call `psp-build-exports -b` for you and automatically generate code that will be compiled into the PRX module.

> `.rodata.sceResident` segment contains exported NID and function addresses.
> 
> `.lib.ent` segment contains Module Name and Export Table Metadata, including the pointer to the export table in `.rodata.sceResident` segment.
> 
> You can checkout the code by running `psp-build-exports -b exports.exp` in the terminal yourself.

### Generate the Stub Library
{: .fs-4 .fw-700 }

To generate the stub library, you can run the following command in the terminal:

```sh
psp-build-exports -k my_lib.exp
```

This will generate a Stub File in `.S` Assembly format. Such file can be compiled into a static library first using `psp-gcc` and then `psp-ar`. Also you can directly distribute it. When others want to use your PRX module, they can simply treat this `.S` as one of the source files.

## Dynamically Loading PRX Modules
{: .fs-6 .fw-700 }

To dynamically load a PRX module, you can use the `sceKernelLoadModule` function and the `sceKernelStartModule` function.

When building your programs, you need to include the generated `.S` stub file (or the stub `.a` library) into your OBJS or LIBS for linking.

The generated `.S` file uses a series of macros, working with the post-processing tool `psp-fixup-imports` to complete the construction of the import table.