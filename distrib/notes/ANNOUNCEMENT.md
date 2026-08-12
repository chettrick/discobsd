--- DiscoBSD 2.7 RELEASED ---

# DiscoBSD 2.7 Released

August 11, 2026

DiscoBSD 2.7 is released.

This is the eighth official release of DiscoBSD, the multi-platform
2.11BSD-based Unix-like operating system for microcontrollers.

DiscoBSD 2.7 offers ports to two different microcontroller platforms:
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
* Import TinyUSB V0.20.0, Device CDC, DWC2; currently unused.
* Refresh of kernel compile Configs and Makefiles for pic32 and stm32.
* KNF style(9) and ANSI cleanup in kernel, ports, and userland.
* Clarity, bugfixes, and improvements in documentation.

### Filesystem

* Add pointer types and max constants to sys/stdint.h.
* Symlink sys/sys/std{int,bool}.h headers to /usr/include.

### Build System

Continuing the overhaul of the build system.
* Both BSD make and GNU make are fully supported.
* FreeBSD's version of BSD make requires `MAKESYSPATH` set.
* Remove '-D' option from /tools install(1) and manual page.
* Releases now include ANNOUNCEMENT.md, maintained in tree.
* Add back many SCCS version tags from 2.11BSD.

### Kernel Specific Improvements

* Fixed double free of script inode bug when exec an interpreter.

### DiscoBSD/stm32 Specific Improvements

* Support for DevEBox STM32F4VE development board.
* Support for WeAct Studio STM32F446RET6 Core development board.
* Support for STMicroelectronics NUCLEO-F446RE development board.
* Enable uart6 on STM32F412G-DISCO board; true multi-user.
* Added sys/stdint.h for kernel; now compiles with -nostdinc.
* Remove libgcc.a from ${LDADD}; it is now not needed nor used.
* Added -z noexecstack to ${LDFLAGS} for no executable stack.
* Rename linker files based on microcontroller and its flash size.
* Added makeoptions to set DEBUG in Config.

### DiscoBSD/pic32 Specific Improvements

* Added makeoptions to set DEBUG in Config.
* KNF and ANSI in dev/usb_uart.[ch] to prepare for TinyUSB CDC.

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
* @ramangopalan
* @Sch-LikA

## Full Changelog
https://github.com/chettrick/discobsd/compare/DISCOBSD_2_6...DISCOBSD_2_7
