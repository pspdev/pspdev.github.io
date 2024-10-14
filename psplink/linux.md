---
title: Setting up PSPLINK on Linux
layout: home
nav_exclude: true
---

# Setting up PSPLINK on Linux
{: .fs-8 .fw-700 .text-center }

With Linux PSPLINK will work without making any changes, but it will require using sudo for the `usbhostfs_pc` command. To make it work without sudo, a udev rule can be added.

To make using PSPLINK without sudo create file called `/etc/udev/rules.d/50-psplink.rules` (for example with `sudo nano /etc/udev/rules.d/50-psplink.rules`) and add the following content:

```
SUBSYSTEM=="usb", ATTR{idVendor}=="054c", ATTR{idProduct}=="01c9", SYMLINK+="psp", MODE="0666"
```

Save this, in Nano this can be done with Ctrl+O and pressing enter. The run the following command:

```
sudo udevadm control --reload
```

Now PSPLINK can be used without sudo. Continue with the instructions on the [Debugging](../debugging.html) page.
