DiscoBSD
========

2.11BSD-based UNIX-like Operating System for STM32 and PIC32 Microcontrollers
-----------------------------------------------------------------------------

DiscoBSD is a 2.11BSD-based UNIX-like operating system for microcontrollers,
with a focus on high portability to memory constrained devices without a
memory management unit.

The current and fourth official release of DiscoBSD is [DiscoBSD 2.3][1],
released on August 11, 2024.

This microcontroller-focused operating system is an independent continuation
of RetroBSD, a 2.11BSD-based OS targeting the MIPS-based PIC32MX7.
DiscoBSD is multi-platform, as it also supports Arm Cortex-M4 STM32F4 devices.

Source code to the system is freely available under a BSD-like license.

History
-------

[DiscoBSD][2] began as an undergraduate [Directed Study][3] in the winter of
2020 at the University of Victoria, Canada, as a case study of [RetroBSD][4]
to port the operating system to the Arm Cortex-M4 architecture, and to enable
portability in the hosting environment and target architectures and platforms.
The paper [*Porting the Unix Kernel*][5] details this initial porting effort.

Work on DiscoBSD has progressed in earnest since the completion of the
Directed Study, with the DiscoBSD/stm32 port booting multi-user in
August 2022. The system is quite usable on supported development boards.

And work continues...

[1]: https://github.com/chettrick/discobsd/releases/tag/DISCOBSD_2_3
[2]: http://DiscoBSD.org
[3]: https://github.com/chettrick/CSC490
[4]: https://RetroBSD.org
[5]: https://github.com/chettrick/CSC490/raw/master/project_outputs/Porting_the_Unix_Kernel-CSC490-Christopher_Hettrick.pdf

DiscoBSD Resource Requirements
------------------------------

A basic, minimal system uses 128 Kbytes of flash and 128 Kbytes of RAM.

The kernel is loaded into the flash and only uses 32 Kbytes of RAM.
User programs each use the remaining 96 Kbytes of RAM, via swapping.
Devices with more RAM can be used to run larger user programs.

An SD card, at least 512 Mbytes in size, is required for the root file system.

Installing and Running
----------------------

Installation consists of loading the kernel into the microcontroller's flash
memory, and imaging the SD card with the file `sdcard.img`.

The `make` target `installfs` uses the `dd` utility to image the SD card
attached to the host operating system at `SDCARD`, such as `/dev/rsdXc` or
`/dev/sdX` or `/dev/rdiskX`, replacing `X` with the actual drive number or
letter, as the case may be.

For example, imaging an SD card attached at `sd2` on an OpenBSD host
operating system through the raw i/o device:

    $ SDCARD=/dev/rsd2c make installfs

Communication with the DiscoBSD console requires a serial port. A USB to TTL
device or the built-in VCP USB serial port on development boards can be used.

    $ cu -l /dev/cuaU0 -s 115200

Depending on the host system, other serial port utilities such as `screen`,
`minicom`, `putty`, or `teraterm` may be used.

Log in to DiscoBSD with user `root` and a blank password.
Shutdown DiscoBSD with the `halt`, `shutdown`, or `reboot` commands.

Manual pages for commands are available through the `man` command.

Building
--------

DiscoBSD is cross-built on UNIX-like host operating systems.

Currently supported host operating systems: OpenBSD, Linux, FreeBSD.

Instructions to configure an OpenBSD host development environment for
Arm and MIPS targets is available [here][6].

The build system fully supports both BSD make and GNU make.

From the source tree root, run:

    $ make build

or just:

    $ make

which builds ELF-formatted kernels in the files `sys/stm32/${BOARD}/unix`
and builds and populates a whole file system userland in `DESTDIR`.

Running:

    $ make distribution

will build everything from a `make build`, plus generate a file system
image in the file `distrib/stm32/sdcard.img` for imaging to an SD card.

DiscoBSD/stm32 is the default port, but DiscoBSD/pic32 may be built via:

    $ make clean
    $ make MACHINE=pic32 MACHINE_ARCH=mips distribution

to generate a file system image in the file `distrib/pic32/sdcard.img`
for imaging to an SD card, `sys/pic32/${BOARD}/unix` ELF-formatted
kernels, and `sys/pic32/${BOARD}/unix.hex` Intel HEX-formatted kernels.

Note that using BSD make on a FreeBSD host requires the system makefile
include directory to be specified on the command line or via the
`MAKESYSPATH` environment variable. For example:

    $ make -m /usr/share/mk

or

    $ MAKESYSPATH=/usr/share/mk
    $ export MAKESYSPATH

    $ make

[6]: tools/openbsd/README.md

Building a DiscoBSD Release
---------------------------

A DiscoBSD release consists of, for each supported architecture:

* a file system image that contains the full base system
* kernels, in various file formats, for each supported development board
* the README.md file for the architecture
* this README.md file

Before making a release, first create a distribution in `DESTDIR`
with kernels in `sys/${MACHINE}/${BOARD}` by following the steps
in [Building](#building):

    $ make distribution

Once all the distribution source objects exist then a release can
be created; one release for each architecture.

A DiscoBSD/stm32 release, as the default architecture, is created by:

    $ make release

A DiscoBSD/pic32 release is created by:

    $ make MACHINE=pic32 MACHINE_ARCH=mips release

A DiscoBSD release is created from an already-populated `DESTDIR`,
and it is placed in `RELEASEDIR`, `distrib/obj/releasedir` by default.

[Releases][7] are available, for each architecture, as a `.tar.gz`
gzip-compressed tar archive and as a `.zip` zip-compressed archive.

[7]: https://github.com/chettrick/discobsd/releases

Debugging
---------

DiscoBSD/stm32 is debugged through OpenOCD and GDB. The `make` targets for
debugging are `ocd` and `gdb-ocd`.

Debug a particular development board via:

    $ BOARD=f412gdisco make ocd

in one terminal, and:

    $ BOARD=f412gdisco make gdb-ocd

in another terminal.

Additional Information
----------------------

Port-specific information can be found in `distrib/${MACHINE}/README.md`
for [stm32][8] and [pic32][9].

[8]: distrib/stm32/README.md
[9]: distrib/pic32/README.md

Source Tree Roadmap
-------------------

    bin         User utilities in both single and multi-user environments.
    distrib     System distributions and releases.
    etc         Templates for system configuration files and scripts in /etc.
    games       Useful and semi-frivolous programs. The important stuff.
    include     Standard C include files.
    lib         System libraries.
    libexec     System daemons and utilities (executed by other programs).
    sbin        System administration utilities in both single and
                multi-user environments.
    share       Architecture-independent shared resource data files.
    sys         Kernel sources.
    tools       Build tools and simulators.
    usr.bin     User utilities in multi-user environments.
    usr.sbin    System administration utilities in multi-user environments.

DiscoBSD/stm32 dmesg
--------------------

```
DiscoBSD 2.3 (F412GDISCO) #1 543: Sun Aug 11 11:11:11 MDT 2024
     chris@stm32.discobsd.org:/sys/stm32/f412gdisco
cpu: STM32F412xE/G rev 1/C, 100 MHz, bus 50 MHz
oscillator: phase-locked loop, clock source: high speed external
uart2: pins tx=PA2/rx=PA3, af=7, console
sd0: port sdio0
sd0: type SDHC, size 31178752 kbytes
sd0a: partition type b7, sector 2, size 204800 kbytes
sd0b: partition type b8, sector 409602, size 2048 kbytes
sd0c: partition type b7, sector 413698, size 204800 kbytes
phys mem  = 256 kbytes
user mem  = 96 kbytes
root dev  = (0,1)
swap dev  = (0,2)
root size = 204800 kbytes
swap size = 2048 kbytes
Automatic boot in progress: starting file system checks.
/dev/sd0a: 1468 files, 12056 used, 191943 free
/dev/sd0c: 3 files, 3 used, 203996 free
Updating motd... done
Starting daemons: update cron 
Sun Aug 11 11:11:11 MDT 2024


2.11 BSD UNIX (name.my.domain) (console)

login: root
Password:
DiscoBSD 2.3 (F412GDISCO) #1 543: Sun Aug 11 11:11:11 MDT 2024

Welcome to DiscoBSD.

erase ^?, kill ^U, intr ^C
# 
```

DiscoBSD/pic32 dmesg
--------------------

```
DiscoBSD 2.3 (MAX32) #1 543: Sun Aug 11 11:11:11 MDT 2024
     chris@pic32.discobsd.org:/sys/pic32/max32
cpu: 795F512L 80 MHz, bus 80 MHz
oscillator: HS crystal, PLL div 1:2 mult x20
spi2: pins sdi=RG7/sdo=RG8/sck=RG6
uart1: pins rx=RF2/tx=RF8, interrupts 26/27/28, console
uart2: pins rx=RF4/tx=RF5, interrupts 40/41/42
uart4: pins rx=RD14/tx=RD15, interrupts 67/68/69
sd0: port spi2, pin cs=RC14
gpio0: portA, pins ii---ii-iiiioiii
gpio1: portB, pins iiiiiiiiiiiiiiii
gpio2: portC, pins i-ii-------iiii-
gpio3: portD, pins --iiiiiiiiiiiiii
gpio4: portE, pins ------iiiiiiiiii
gpio5: portF, pins --ii--------i-ii
gpio6: portG, pins iiii--i-----iiii
adc: 15 channels
pwm: 5 channels
sd0: type I, size 524288 kbytes, speed 10 Mbit/sec
sd0a: partition type b7, sector 2, size 204800 kbytes
sd0b: partition type b8, sector 409602, size 2048 kbytes
sd0c: partition type b7, sector 413698, size 204800 kbytes
phys mem  = 128 kbytes
user mem  = 96 kbytes
root dev  = (0,1)
swap dev  = (0,2)
root size = 204800 kbytes
swap size = 2048 kbytes
Automatic boot in progress: starting file system checks.
/dev/sd0a: 1474 files, 12346 used, 191653 free
/dev/sd0c: 3 files, 3 used, 203996 free
Updating motd... done
Starting daemons: update cron 
Sun Aug 11 11:11:11 MDT 2024


2.11 BSD UNIX (name.my.domain) (console)

login: root
Password:
DiscoBSD 2.3 (MAX32) #1 543: Sun Aug 11 11:11:11 MDT 2024

Welcome to DiscoBSD.

erase ^?, kill ^U, intr ^C
# 
```
