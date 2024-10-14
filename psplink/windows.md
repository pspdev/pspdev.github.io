---
title: Setting up PSPLINK on Windows
layout: home
nav_exclude: true
---

# Setting up PSPLINK on Windows
{: .fs-8 .fw-700 .text-center }

On Windows a driver needs to be installed before PSPLINK can be used. To do this take the following steps:

1. Make sure the programs `usbhostfs_pc` and `pspsh` are available in cmd. Otherwise download them [here](https://github.com/pspdev/psplinkusb/releases/download/latest/pspsh-windows.zip).
2. Start PSPLINK on the Playstation Portable and connect it to the computer through USB.
3. Download [Zadig](https://zadig.akeo.ie/) and start it. It will ask if you want to run it as administrator, click yes.
4. In Zadig, click on `options` -> `List All Devices`.
5. Select the entry `"PSP" type B` from the dropdown list.
6. Left of driver, select the `libusb-win32` driver. Then click install.
7. Wait for the installation to finish, then disconnect the USB cable from the PSP.

Now PSPLINK can be used with Windows. Continue with the instructions on the [Debugging](../debugging.html) page.
