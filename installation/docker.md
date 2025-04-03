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

## Toolchain 
{: .fs-6 .fw-700 }

Once docker is installed, the PSPDEV image can be downloaded using the following command from a terminal:

```shell
docker pull pspdev/pspdev:latest
```

To work with it, open a terminal in the directory with the code you'd like to build in it and run the following command:

```shell
docker run -ti -v .:/source pspdev/pspdev:latest
```

After that you can run `cd /source` to navigate to the code.

That's it, now the PSP SDK can be used to build PSP software. Check out the [Basic Programs](../basic_programs.html) page to for examples on what you can do with it.
