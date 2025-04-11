---
title: Installation on Arch Linux
layout: home
nav_exclude: true
---

# Installation on Arch Linux

{: .fs-8 .fw-700 .text-center }


## Prerequisites

{: .fs-6 .fw-700 }

Arch Linux users can install the PSP SDK using packages available in the Arch
User Repository (AUR). There are two primary options: a pre-compiled binary
package (recommended for faster installation) and a source package (which builds
the SDK locally).

### AUR Helper (Optional but Recommended)

An AUR helper (like `yay`, `paru`, etc.) simplifies installing and updating AUR
packages. If you don't have one, you can find instructions on the
[Arch Wiki's AUR helpers page](https://wiki.archlinux.org/title/AUR_helpers).
Using an AUR helper is assumed for the simplified commands below, but manual
installation steps are also provided.

## Installation via AUR (Recommended - Binary)

{: .fs-6 .fw-700 }

The [`psp-sdk-bin`](https://aur.archlinux.org/packages/psp-sdk-bin) package,
provides pre-compiled binaries for the PSP SDK, making installation much faster.

**Using an AUR helper (e.g., `yay`):**

```shell
yay -S psp-sdk-bin
```

**Manual Installation:**

```shell
# Clone the build files
git clone https://aur.archlinux.org/psp-sdk-bin.git
cd psp-sdk-bin

# Build and install the package
makepkg -si

# Clean up (optional)
cd ..
rm -rf psp-sdk-bin
```

## Installation via AUR (Alternative - Source)

{: .fs-6 .fw-700 }

The [`psp-sdk`](https://aur.archlinux.org/packages/psp-sdk) package builds the
entire SDK from source. This takes significantly longer but ensures you are
building against your system's exact libraries.

**Warning:** Building the full SDK can take a considerable amount of time and
CPU resources.

**Using an AUR helper (e.g., `yay`):**

```shell
yay -S psp-sdk
```

**Manual Installation:**

```shell
# Clone the build files
git clone https://aur.archlinux.org/psp-sdk.git
cd psp-sdk

# Build and install the package (this will take a while!)
makepkg -si

# Clean up (optional)
cd ..
rm -rf psp-sdk
```

## Verification

{: .fs-6 .fw-700 }

From a new terminal, run the following command to confirm the SDK is found and
the environment variable is set:

```shell
psp-config --pspdev-path
```

This command should output the path you set for `PSPDEV` (e.g., `/opt/pspdev`).
If the command itself is not found, ensure `/opt/pspdev/bin` (or the equivalent
`bin` directory for your installation) is in your `PATH`. The profile script
installed by the AUR package should handle this automatically after logging out
and back in or rebooting.

That's it, now the PSP SDK can be used to build PSP software. Check out the
[How to Use](../how_to_use.html) page for a guide on how to do so.
