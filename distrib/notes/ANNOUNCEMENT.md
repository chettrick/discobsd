--- DiscoBSD 2.6 RELEASED ---

# DiscoBSD 2.6 Released

February 11, 2026

DiscoBSD 2.6 is released.

This is the seventh official release of DiscoBSD, the multi-platform
2.11BSD-based Unix-like operating system for microcontrollers.

DiscoBSD 2.6 offers ports to two different microcontroller platforms:
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
* Separate out console and {,k}mem drivers to be machine-independent.
* Refresh of kernel compile Configs and Makefiles for pic32 and stm32.
* Many improvements and cleanup of sysctl(8) and kernel-side sysctl code.
* KNF style(9) and ANSI cleanup in kernel, ports, and userland.
* Clarity, bugfixes, and improvements in documentation.

### Filesystem

* Hard link uptime(1) to w(1); uptime(1) now enabled.
* Properly symlink /etc/localtime with DiscoBSD zone files.
* Update libc bzero(3) to take a size_t, not unsigned int or long.
* FD_ZERO(2) in libc now uses memset(3) instead of bzero(3).
* Many instances in tree of replacing bzero(3) with memset(3).

### Build System

Continuing the overhaul of the build system.
* Rename kconfig(8) back to config(8), in line with all BSDs.
* 4.4BSD names for kernel files locore.S, conf.c, and sig_machdep.c.
* Both BSD make and GNU make are fully supported.
* FreeBSD's version of BSD make requires `MAKESYSPATH` set.
* Speed up in imaging SD card with DiscoBSD file system.
* Speed up in `make release`, up to 5x faster.
* Releases now include ANNOUNCEMENT.md, maintained in tree.
* Add back many SCCS version tags from 2.11BSD.

### DiscoBSD/stm32 Specific Improvements

* Support for STM32F413H-DISCO development board.
* Extensions to HAL library for the Memory Protection Unit (MPU).
* MPU sysctls for Enabled, num of Regions, and CTRL register.
* Add sysctl(8) support to read and display MPU information.
* Clean structure for SDIO GPIO pins; extensible to new boards.
* Separate nulldev() into nullopen(), nullclose(), and nullstop().

### DiscoBSD/pic32 Specific Improvements

* Refactor and normalize kernel compile options with stm32.
* Consolidate compiler tools and paths under pic32/conf/compiler.mk.
* Separate nulldev() into nullopen(), nullclose(), and nullstop().

### Documentation, Bugfixes, and Corrections

* Documentation to set up a Linux host development environment.
* Releases are documented in ANNOUNCEMENT.md, maintained in tree.
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

### Ubuntu 24.04 (Zorin OS 18 Core)
* Host compiler GCC 13.2.0
* Host compiler Clang 18.1.3
* BSD make and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 13.2.1
  * arm-none-eabi-binutils 2.42
* DiscoBSD/pic32
  * Untested

### Ubuntu 23.04
* Host compiler GCC 12.3.0
* Host compiler Clang 15.0.7
* BSD make and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 12.2.1
  * arm-none-eabi-binutils 2.39
* DiscoBSD/pic32
  * Untested

### FreeBSD 13.2
* Host compiler GCC 12.2.0
* Host compiler Clang 14.0.5
* BSD make (with MAKESYSPATH set) and GNU make
* DiscoBSD/stm32
  * arm-none-eabi-gcc 10.3.1 (gcc-arm-embedded)
  * arm-none-eabi-binutils 2.40
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
https://github.com/chettrick/discobsd/compare/DISCOBSD_2_5...DISCOBSD_2_6
