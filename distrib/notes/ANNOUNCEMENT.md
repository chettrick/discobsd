--- DiscoBSD 2.5 RELEASED ---

# DiscoBSD 2.5 Released

August 11, 2025

DiscoBSD 2.5 is released.

This is the sixth official release of DiscoBSD, the multi-platform
2.11BSD-based Unix-like operating system for microcontrollers.

DiscoBSD 2.5 offers ports to two different microcontroller platforms:
* DiscoBSD/stm32 - STM32F4 family of 32-bit Arm Cortex-M4
  microcontrollers from STMicroelectronics
* DiscoBSD/pic32 - PIC32MX7 family of 32-bit MIPS32 M4K
  microcontrollers from Microchip

DiscoBSD/stm32, unique only to DiscoBSD, offers a familiar BSD
environment on the many available STM32F4 development boards. 

DiscoBSD/pic32, inherited from RetroBSD, offers a familiar BSD
environment on the many available PIC32MX7 development boards,
as well as full use with the included VirtualMIPS PIC32 simulator.

A nearly-complete development environment is included in DiscoBSD.

There are: various text editors and compilers, a MIPS assembler and
MIPS linker, and many more programming languages in addition to C
and asm, such as Scheme, BASIC, Forth, RetroForth, lex, yacc, and TCL.
Examples are provided in the file system at /usr/share/examples.

As a descendant of 2.11BSD, DiscoBSD inherits its strong BSD heritage.
The userland is powerful, full-featured, and comfortable to competent
UNIX users, as it is derived from the rich 4.3BSD-Tahoe userland, modern
implementations of classic utilities, and improvements along the way.

Install, build, and debug instructions can be found in the README files.


## Significant Changes and Improvements

### New Features in this Release

* Kernel source tree follows 4.4BSD hierarchy, to facilitate future ports.
* Separate out machine-dependent and machine-independent code.
* Refresh of kernel compile Configs and Makefiles for pic32 and stm32.
* Updated host cross debugger to GDB 12.1 with DWARF 5 for stm32.
* Added Section 9 to the manual pages system, with update to man(1).
* Added intro manual pages to Sections 5, 6, and 9; added style(9).
* More work on OpenBSD host tools and custom ports.
* Default timezone is updated from `US/Pacific` to `Canada/Mountain`.
* Clarity and improvements in documentation.

### Filesystem

* 4.4BSD-style locations of C include headers.

### Build System

Continuing the overhaul of the build system.
* Kernels are now built in `sys/arch/${MACHINE}/compile/${BOARD}`.
* Linker scripts and common configs in `sys/arch/${MACHINE}/conf`.
* Updated host cross debugger to GDB 12.1 with DWARF 5 for stm32.
* More length-bound string functions for kconfig(8).
* Both BSD make and GNU make are fully supported.
* FreeBSD's version of BSD make requires `MAKESYSPATH` set.
* Linux uses `libbsd-dev` via `pkg-config` throughout source tree.
* Fixed all OpenBSD-specific linker warnings for unsafe functions.

### DiscoBSD/stm32 Specific Improvements

* Separate out arm-specific and stm32-specific code, to facilitate
   future ports of other arm-based microcontrollers.

### DiscoBSD/pic32 Specific Improvements

* Enabled kernel builds for the Pontech QUICK240 system.
* Unified on common linker scripts across many boards.

### Bugfixes and Corrections

* Update to GDB 12.1 corrected stm32 DWARF 5 debugging.
* Steady improvements and corrections in documentation.
* Manual page fixes and improvements.


## Host Development Environment

While DiscoBSD is primarily developed and tested on OpenBSD,
Linux and FreeBSD are also supported as host environments.

These host development environments have been tested:

### OpenBSD 7.6
* Host compiler Clang 16.0.6
* Host compiler GCC 11.2.0
* Host compiler Clang 17.0.6
* BSD make and GNU make
* DiscoBSD/stm32
  * Custom port of arm-none-eabi-gcc 12.2.0 (rmprofile)
  * OpenBSD package of arm-none-eabi-binutils 2.40
  * Custom port of arm-none-eabi-gdb 12.1
  * OpenBSD package of OpenOCD 0.11.0
  * Custom port of ST-Link 1.8.0
* DiscoBSD/pic32
  * Custom port of mips-elf-gcc 12.2.0
  * Custom port of mips-elf-binutils 2.40

### Ubuntu 23.04
* Host compiler GCC 12.3.0
* Host compiler Clang 15.0.7
* BSD make and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 12.2.1
* DiscoBSD/pic32
  * Untested

### FreeBSD 13.2
* Host compiler Clang 14.0.5
* BSD make (with MAKESYSPATH set) and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 10.3.1 (gcc-arm-embedded)
* DiscoBSD/pic32
  * Untested


## Release Build Environment

DiscoBSD distribution releases are cross-built on OpenBSD.

The release build environment is configured as below:

### OpenBSD 7.6
* Host compiler Clang 16.0.6
* BSD make
* DiscoBSD/stm32
  * Custom port of arm-none-eabi-gcc 12.2.0 (rmprofile)
  * OpenBSD package of arm-none-eabi-binutils 2.40
* DiscoBSD/pic32
  * Custom port of mips-elf-gcc 12.2.0
  * Custom port of mips-elf-binutils 2.40


## Developers and Contributors this Release
* @chettrick

## Full Changelog
https://github.com/chettrick/discobsd/compare/DISCOBSD_2_4...DISCOBSD_2_5
