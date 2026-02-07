--- DiscoBSD 2.3 RELEASED ---

# DiscoBSD 2.3 Released

August 11, 2024

DiscoBSD 2.3 is released.

This is the fourth official release of DiscoBSD, the multi-platform
2.11BSD-based Unix-like operating system for microcontrollers.

DiscoBSD 2.3 offers ports to two different microcontroller platforms:
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

* Make targets for distribution and release.
* Support for the WeAct Studio STM32F412RET6 Core board.
* Support for the WeAct Studio STM32F405RGT6 Core board.
* Imported the LL Cortex driver from STM32CubeF4.
* Imported strlcpy(3), strlcat(3), basename(3), and dirname(3).

### Filesystem

* Cleanup of C include headers.
* Added libgen.h C include header for strlcpy(3), strlcat(3).

### Build System

Continuing the overhaul of the source tree and build system.
* Both BSD make and GNU make are fully supported.
* FreeBSD's version of make requires MAKESYSPATH set.
* OSRev variable in share/mk/sys.mk for release naming.
* RELEASEDIR variable is default distrib/obj/releasedir.
* Release tar and zip archives generated in RELEASEDIR.
* Linux uses libbsd-dev and pkg-config variables in tools.

### Kernel

* Import sys/syslimits.h header from OpenBSD.
* Define PATH_MAX, to eventually replace MAXPATHLEN.

### DiscoBSD/stm32 Specific Improvements

* Add optional SDIO_XFER_CLK_DIV Config for SDIO clock.
* Explicit IDs and chip revisions in cpuidentify().
* Support for WeAct Studio STM32F412RET6 Core board.
* Support for WeAct Studio STM32F405RGT6 Core board.

### DiscoBSD/pic32 Specific Improvements

* VirtualMIPS SD card size increased to 512MB.

### Bugfixes and Corrections

* Normalize formatting of .TH dates in man(7) manuals.
* Clean up all DOS line endings with Unix line endings.
* Many strcpy(3) -> strlcpy(3), strcat(3) -> strlcat(3), and
   sprintf(3) -> snprintf(3) conversions.
* Steady improvements and corrections in documentation.
* Steady improvements and corrections in libraries.
* Manual page fixes and improvements.


## Host Development Environment

While DiscoBSD is primarily developed and tested on OpenBSD,
Linux and FreeBSD are also supported as host environments.

These host development environments have been tested:

### OpenBSD 7.3
* Host compiler Clang 13.0.0
* BSD make and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 7.4.1
* DiscoBSD/pic32
  * Custom port of mips-elf-gcc 4.8.1

### OpenBSD 6.8
* Host compiler GCC 4.2.1
* Host compiler GCC 8.4.0
* Host compiler Clang 10.0.1
* BSD make and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 7.4.1
* DiscoBSD/pic32
  * Custom port of mips-elf-gcc 4.8.1

### Ubuntu 23.04
* Host compiler GCC 12.3.0
* Host compiler Clang 15.0.7
* BSD make and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 12.2.1
  * arm-none-eabi-gcc 10.3
* DiscoBSD/pic32
  * Untested

### Ubuntu 18.04
* Host compiler GCC 9.2.1
* BSD make and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 7.3.1
* DiscoBSD/pic32
  * Custom compilation of mips-elf-gcc 4.8.1

### FreeBSD 13.2
* Host compiler Clang 14.0.5
* BSD make (with MAKESYSPATH set) and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 10.3.1
* DiscoBSD/pic32
  * Untested


## Developers and Contributors this Release
* @chettrick 
* @extrowerk

## Full Changelog
https://github.com/chettrick/discobsd/compare/DISCOBSD_2_2...DISCOBSD_2_3
