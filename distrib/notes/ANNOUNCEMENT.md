--- DiscoBSD 2.4 RELEASED ---

# DiscoBSD 2.4 Released

February 11, 2025

DiscoBSD 2.4 is released.

This is the fifth official release of DiscoBSD, the multi-platform
2.11BSD-based Unix-like operating system for microcontrollers.

The release of DiscoBSD 2.4 marks the 33rd anniversary of 2.11BSD.

DiscoBSD 2.4 offers ports to two different microcontroller platforms:
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

* Update host cross compilers to GCC 12.2.0 for stm32 and pic32.
* Updated single-precision floating-point library to 7.0.1 from LLVM.
* Imported double-precision floating-point library 7.0.1 from LLVM.
* Handle mips-elf-gcc dropping -fshort-double throughout tree.
* Imported, updated, and enabled 2.11BSD install(1).
* K&R to ANSI updates.
* Major overhaul to OpenBSD host tools and custom ports.
* Clarity and improvements in documentation.
* Windows instructions for installing releases and firmware.

### Filesystem

* Cleanup of C include headers.
* Addition of int64_t, uint64_t, and min-width macros to stdint.h.
* Added stdbool.h C include header from OpenBSD.
* Machine dependent _float.h headers.

### Build System

Continuing the overhaul of the  build system.
* Updated to OpenBSD 7.6 as main host development environment.
* K&R to ANSI updates for all host-built utilities.
* Length-bound string functions for all host-built utilities.
* Both BSD make and GNU make are fully supported.
* FreeBSD's version of BSD make requires MAKESYSPATH set.
* Linux uses libbsd-dev via pkg-config throughout source tree.
* Handle linker warning --no-warn-rwx-segments for all hosts.
* Fixed all OpenBSD-specific linker warnings for unsafe functions.

### DiscoBSD/stm32 Specific Improvements

* Kernels available in Intel HEX .hex and binary .bin formats.
* MD _float.h header properly handles doubles and long doubles.

### DiscoBSD/pic32 Specific Improvements

* Migrated from 32-bit short-doubles to 64-bit doubles.
* Imported and updated LLVM floating-point library,
  which enabled the removal of -fshort-double in GCC.
* Double floats enabled migration to GCC 12.2.0 from 4.8.1.
* MD _float.h header properly handles doubles and long doubles.

### Bugfixes and Corrections

* Use host system's strmode(3) in tools ar(1); fixes type conflict.
* Steady improvements and corrections in documentation.
* Steady improvements and corrections in libraries.
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
* DiscoBSD/pic32
  * Custom port of mips-elf-gcc 12.2.0

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
* DiscoBSD/pic32
  * Custom port of mips-elf-gcc 12.2.0


## Developers and Contributors this Release
* @chettrick

## Full Changelog
https://github.com/chettrick/discobsd/compare/DISCOBSD_2_3...DISCOBSD_2_4

## Release Correction - February 24, 2025
The DiscoBSD/stm32 release in `DiscoBSD_2_4_stm32.tar.gz`
and `DiscoBSD_2_4_stm32.zip` was __incorrectly__ built with
arm-none-eabi-gcc 12.2.0 __without__ multilib support.

The updated DiscoBSD/stm32 release in `DiscoBSD_2_4_stm32p0.tar.gz`
and `DiscoBSD_2_4_stm32p0.zip` was __correctly__ built with
arm-none-eabi-gcc 12.2.0 __with__ multilib support.

It is recommended to use the __updated__ DiscoBSD/stm32 release.

The DiscoBSD/pic32 release is unaffected.
