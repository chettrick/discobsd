--- DiscoBSD 2.1 RELEASED ---

# DiscoBSD 2.1 Released

August 31, 2023

DiscoBSD 2.1 is released.

This is the second official release of DiscoBSD, the multi-platform
2.11BSD-based operating system for microcontrollers.

DiscoBSD 2.1 offers ports to two different microcontroller platforms:
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
MIPS linker, as well as many more programming languages in addition
to C and asm, such as Scheme, BASIC, Forth, RetroForth, yacc, and TCL.
Examples are provided in the file system at /usr/share/examples.

As a descendant of 2.11BSD, DiscoBSD inherits its strong BSD heritage.
The userland is powerful, full-featured, and comfortable to competent
UNIX users, as it is derived from the rich 4.3BSD-Tahoe userland and
steady improvements along the way.

Install, build, and debug instructions can be found in the README files.


## Significant Changes and Improvements

### New Features

* libtcl 6.7 available through build system, mainly for tclsh(1).

### Filesystem

Major overhaul of DiscoBSD's root filesystem.
The root filesystem is now more in line with a 4.4BSD system
than the 2.9BSD-style system inherited from RetroBSD.

Notable changes are:
* The return of the /usr directory.
* Separation of executables into /bin, /sbin, /usr/bin, /usr/sbin,
  /usr/libexec, and /usr/games, based on single user mode needs.
* The creation of a user /home directory, on its own filesystem.
* Both the root filesystem and user filesystem are 200 MB each.

### Build System

Major overhaul of DiscoBSD's source tree and build system.
* Both BSD make and GNU make are fully supported.
* Non-portable features of GNU make and BSD make are
  replaced with portable alternatives.
* The source tree follows a more 4.4BSD layout, with each
  executable having its own directory in the right hierarchy.
* Build objects are now universally installed into ${DESTDIR},
  default location is /distrib/obj/destdir.${MACHINE}.
* /etc/Makefile target distrib-dirs builds the skeleton
  filesystem hierarchy in ${DESTDIR}.
* Build tools install into ${TOOLBINDIR}, default location
  is the persistent /tools/bin directory.
* Build outputs in ${DESTDIR} from different architectures
  can co-exist simultaneously.
* Added ${HOST_CC} make variable for host compiler.
* tools/aoututils for building a.out system libraries.
* Kernels are now called unix, not unix.elf.
* Consistent CFLAGS, AFLAGS, ASFLAGS, LIBS, LDLIBS.
* Build system with -fcommon in ${CFLAGS}.
* FreeBSD is now a host development environment.

### DiscoBSD/stm32 Specific Improvements

* Implemented ARMv7-M fault trap handling.
* Dumps fault stack frame upon fault.
* Add MCU and revision IDs for STM32F407xx.
* Portable Makefiles for kernel builds.

### DiscoBSD/pic32 Specific Improvements

* Portable Makefiles for kernel builds.

### Bugfixes and Corrections

* Numerous K&R -> ANSI fixes.
* Newer compilers exposed many unsafe structures,
  and they were promptly remedied.
* Many updates concerning the hierarchy changes.
* Perform ranlib -t to touch libraries after installation.
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
* DiscoBSD/stm32
  * arm-none-eabi-gcc 10.3.1
* DiscoBSD/pic32
  * Untested


## Developers and Contributors
* @chettrick 
* @lhondareyte 
* @saper 
* @omarandlorraine
* @amarkee 
