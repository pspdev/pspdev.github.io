---
title: Installation on Docker
layout: home
nav_exclude: true
---

# Installation on Docker
{: .fs-8 .fw-700 .text-center }

## Docker
{: .fs-6 .fw-700 }

Docker can be used on almost every platform and offers and easy way to use the PSP SDK with very little setup.

To use it, install Docker itself using the instructions [here](https://docs.docker.com/engine/install/).

## PSP SDK 
{: .fs-6 .fw-700 }

Once docker is installed, the PSPDEV image can be downloaded using the following command from a terminal:

```shell
docker pull pspdev/pspdev:latest
```

To work with it, open a terminal in the directory with the code you'd like to build in it and run the following command:

```shell
docker run -ti -v $PWD:/source pspdev/pspdev:latest
```

This mounts the current directory to a directory called source in the container, so run `cd /source` to navigate to it.

That's it, now the PSP SDK can be used to build PSP software. Check out the [Basic Programs](../basic_programs.html) page to for examples on what you can do with it.

### Windows

Using the container on Windows can be a bit tricky.

* Replace $PWD with the absolute path in a UNIX format. Example: `/c/Users/John/my_psp_code`.
* Your code *must* be located somewhere under "home folder" (like `C:\Users\John`), or the mounted directory will be empty when viewed from inside the container due to permissions.
* If you get an `Operation not permitted` error from gmake while using psp-cmake, you'll need to add the `--privileged=true` option to your `docker run` command.
