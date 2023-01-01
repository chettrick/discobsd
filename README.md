DiscoBSD
========

2.11BSD-based UNIX-like Operating System for STM32 and PIC32 Microcontrollers
-----------------------------------------------------------------------------

DiscoBSD is a 2.11BSD-based UNIX-like operating system for microcontrollers,
with a focus on high portability to memory constrained devices without a
memory management unit.

This microcontroller-focused operating system is the continuation of RetroBSD,
a 2.11BSD-based OS targeting only the MIPS-based PIC32MX7.
DiscoBSD is multi-platform, as it also supports Arm Cortex-M4 STM32F4 devices.

Source code to the system is freely available under a BSD-like license.

DiscoBSD Resource Requirements
------------------------------

A basic, minimal system uses 128 Kbytes of flash and 128 Kbytes of RAM.

The kernel is loaded into the flash and only uses 32 Kbytes of RAM.
User programs each use the remaining 96 Kbytes of RAM, via swapping.
Devices with more RAM can be used to run larger user programs.

An SD card is required for the root file system.

Installing and Running
----------------------

Installing consists of loading the kernel into the flash memory, and imaging
the SD card with the file `sdcard.img`.

Communication with DiscoBSD requires a serial port. A USB to TTL device or
the built-in VCP USB serial port on development boards can be used.

    $ cu -l /dev/cuaU0 -s 115200

Depending on the host system, other serial port utilities such as `screen`,
`minicom`, `putty`, or `teraterm` may be used.

Log in to DiscoBSD with user `root` and a blank password.
Shutdown DiscoBSD with the `halt`, `shutdown`, or `reboot` commands.

Manual pages for commands are available through the `man` command.

Building
--------

DiscoBSD is cross-built on UNIX-like host operating systems.

From the source tree root, run:

    $ gmake

which will build a file system image in the file `distrib/stm32/sdcard.img`
and kernels in the files `sys/stm32/${BOARD}/unix.elf`.

DiscoBSD/stm32 is the default port, but DiscoBSD/pic32 may be built via:

    $ MACHINE=pic32 MACHINE_ARCH=mips gmake

which will build a file system image in the file `distrib/pic32/sdcard.img`
and kernels in the files `sys/pic32/${BOARD}/unix.elf`.

Debugging
---------

DiscoBSD/stm32 is debugged through OpenOCD and GDB. The `make` targets for
debugging are `ocd` and `gdb`.

Debug a particular development board via:

    $ BOARD=f412gdisco gmake ocd

in one terminal, and:

    $ BOARD=f412gdisco gmake gdb

in another terminal.

Additional Information
----------------------

Port-specific information can be found in `distrib/${MACHINE}/README.md`.

DiscoBSD/stm32 dmesg
--------------------

```
2.11 BSD UNIX for STM32, rev G274 #1: Fri Dec 30 18:42:28 PST 2022
     chris@stm32.discobsd.org:/sys/stm32/f412gdisco
cpu: STM32F412xx rev C, 100 MHz, bus 50 MHz
oscillator: phase-locked loop, clock source: high speed external
uart2: pins tx=PA2/rx=PA3, af=7, console
sd0: port sdio0, 4-bit bus
sd0: type SDHC, size 31178752 kbytes
sd0a: partition type b7, sector 2, size 102400 kbytes
sd0b: partition type b8, sector 204802, size 2048 kbytes
sd0c: partition type b7, sector 208898, size 102400 kbytes
phys mem  = 256 kbytes
user mem  = 96 kbytes
root dev  = (0,1)
swap dev  = (0,2)
root size = 102400 kbytes
swap size = 2048 kbytes
Automatic boot in progress: starting file system checks.
/dev/sd0a: 1449 files, 11899 used, 90100 free
/dev/sd0c: 3 files, 3 used, 101996 free
Updating motd... done
Starting daemons: update cron 
Sat Dec 31 17:05:27 PST 2022

2.11 BSD UNIX (name.my.domain) (console)

login: root
Password:
2.11 BSD UNIX for STM32, rev G274 #1: Fri Dec 30 18:42:28 PST 2022

Welcome to DiscoBSD.

erase ^?, kill ^U, intr ^C
# 
```
