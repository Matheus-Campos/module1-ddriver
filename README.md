# Module1 Device Driver

Just playing with C, make and device drivers for Linux. This repository is for the Software Infrastructure discipline of my superior course Informations Systems Bachelor's.

## Initial steps

1. As a super user, run the `make` command in root directory to compile and build the driver's files.

2. A file named "module1.ko" must have appeared in root directory, run `insmod ./module1.ko` from root directory in order to install the module on kernel.

3. `lsmod` command can be used to list all device drivers installed on the machine.

4. To uninstall the device driver, just execute `rmmod module1` as a super user.

### Logging

The command `dmesg` shows the kernel messages buffer (log) and can be used to discover what the device driver is doing.
