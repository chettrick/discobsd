--- DiscoBSD 2.2 RELEASED ---

# DiscoBSD 2.2 Released

February 29, 2024

DiscoBSD 2.2 is released.

This is the third official release of DiscoBSD, the multi-platform
2.11BSD-based operating system for microcontrollers.

DiscoBSD 2.2 offers ports to two different microcontroller platforms:
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

As a descendant of 2.11BSD, DiscoBSD inherits the strong BSD heritage.
The userland is powerful, full-featured, and comfortable to competent
UNIX users, as it is derived from the rich 4.3BSD-Tahoe userland, modern
implementations of classic utilities, and improvements along the way.

Install, build, and debug instructions can be found in the README files.


## Significant Changes and Improvements

### New Features

* Unprivileged builds, with our own version of install(1) in /tools.
* Imported and ported the Plan 9 versions of dc(1), bc(1), and lex(1).
* as(1) can assemble files that include the MIPS .module directive.

### Filesystem

* More general cleanup from the file system reorganization.

### Build System

Continuing the overhaul of the source tree and build system.
* Both BSD make and GNU make are fully supported.
* FreeBSD's version of make requires MAKESYSPATH set.
* Added machine-specific Makefile.inc files for debugging.
* Manual pages specify DiscoBSD as operating system name.
* Use ${INSTALL} throughout the source tree.
* tools/binstall is OpenBSD install(1), with an implementation of
  the -U unprivileged option inspired from NetBSD install(1) .
* Use the new tools/binstall for unprivileged builds and installs.
* System build Makefile from target.mk to share/mk/sys.mk.
* OpenBSD-style OSMAJOR, OSMINOR in share/mk/sys.mk.

### Kernel
* Proper BSD-style kernel version string, for each architecture.
* KERN_OSTYPE, KERN_OSRELEASE, and KERN_OSVERSION
  sysctls updated. E.g., DiscoBSD, 2.2, and F412GDISCO#5.
* Kernel versioning unified; driven by /sys/conf/newvers.sh.
* Clean up some ifdefs between architectures for ELF.
* Clean up uname(3) and struct field lengths to 256.

### DiscoBSD/stm32 Specific Improvements

* Enable new Plan 9 /usr/bin/lex for DiscoBSD/stm32.
* Add lex examples in /usr/share/examples/lex.
* Use the common /sys/conf/newvers.sh kernel shell script.
* Update to latest NetBSD Arm version of elf_machdep.h.

### DiscoBSD/pic32 Specific Improvements

* Use the common /sys/conf/newvers.sh kernel shell script.
* Update to latest NetBSD MIPS version of elf_machdep.h.
* Fix many implicit int warnings.
* Enable -fcommon for MIPS kernel builds.

### Bugfixes and Corrections

* Numerous K&R -> ANSI fixes.
* Newer compilers exposed many unsafe structures,
  and they were promptly remedied.
* Many updates concerning hierarchy changes.
* Steady improvements and corrections in games.
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


## Developers and Contributors
* @chettrick 
* @amarkee 

## Full Changelog
https://github.com/chettrick/discobsd/compare/DISCOBSD_2_1...DISCOBSD_2_2
