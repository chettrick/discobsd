.\" XXX .de ref*biblio-start-hook
.\" XXX .  SH
.\" XXX .  XN "\\*[REFERENCES]"
.\" XXX .  nop \&\\*[REFERENCES]
.\" XXX .  par@reset
.\" XXX ..
.R1
label-in-reference
accumulate
no-move-punctuation
.R2
.RP
.TL
Porting the Unix Kernel
.AU
Christopher K. Hettrick
.AI
CSC 490
Department of Computer Science
University of Victoria
Victoria, British Columbia

Supervised by Dr. Bill Bird
.AB
This report describes the process of porting a variant of the Unix kernel
from the MIPS processor architecture to the Arm processor architecture.
A heavily modified 2.11BSD version of the Unix kernel called RetroBSD is used
as a case study, and is the basis of this development.
The goal of this project is to run this ported kernel on both an emulator
and on a physical embedded development board.
An additional portion of this work is devoted to adapting
the large-scale codebase of RetroBSD to more modern and
sustainable development standards that will facilitate future ports to
other platforms and architectures.
The host development environment is supported on both OpenBSD and Linux
as host operating systems for cross compilation to MIPS and Arm targets.
.AE
.NH
.XN "Introduction"
.PP
This report is a case study of the relevant operation of the Unix kernel
as exemplified in the kernel of the RetroBSD operating system.
RetroBSD was chosen for this project as
it is able to run on embedded system microcontrollers with
extremely low resources and no memory management unit,
hence no virtual memory.
Additionally, this report covers the technical considerations,
the development environment,
the hardware platform and software emulator,
and the many and varied issues and compromises
that occurred during the process of
porting the MIPS32 M4K-based RetroBSD kernel to
include support for the Arm Cortex-M4 processor architecture,
hereafter referred to as DiscoBSD.
The kernel of DiscoBSD is targeted to run on the
STM32F4-Discovery development hardware
available from STMicroelectronics.
.NH
.XN "Relevant History of Unix and BSD"
.PP
RetroBSD is a semi-modernized version of 2.11BSD targeted to the
PIC32MX7 MIPS-based microcontroller.
.[
Homepage of RetroBSD
.]
The early history of RetroBSD has been lost.
It can only be concluded that RetroBSD was started some time in 2011,
or perhaps some time even before that.
The earliest post on the RetroBSD forum was from August 15, 2011.
The project could have started much earlier than the creation of the forum.
The project was started and lead by Sergey (Serge) Vakulenko,
a systems programmer who started working at MIPS Technologies in 2011.
.[
HomepageSerge
.]
.PP
2BSD is a family of operating systems for the DEC PDP-11 derived from
Research Unix and developed at the University of California at Berkeley.
2.11BSD has a long lineage going back to the first release of
2BSD on May 10, 1979.
.[
PUPS
.]
2BSD is a direct descendant of the Sixth Edition of Research Unix,
commonly known as V6 Unix.
2.8BSD incorporated features from the Seventh Edition of Research Unix,
32V Unix, and 4.1BSD.
.[
DI44BSDOSpg5to6
.]
.\" Reference to Unix system family tree, pages 5-6 of D&I 4.4BSD OS.
The 2BSD line of software distributions
continued on until the most recent release of 2.11BSD in 1991.
.[
Announcement of Second Distribution of Berkeley PDP-11 Software
.]
This release was a celebration of the 20\*{th\*} anniversary of the PDP-11.
It is the culmination of the many efforts to port features from
4.3BSD and 4.3BSD-Tahoe
\*- which run on the DEC VAX \*- to the PDP-11.
Patches to 2.11BSD have been sporadically available since the initial
release in 1991 from the long-time maintainer Steven Schultz.
The most recent patch level is 469 and was released on April 14, 2020.
.[
469.patch
.]
RetroBSD was started from patch level 431,
which was released on April 21, 2000.
It is from this version that all RetroBSD development began.
DiscoBSD derives from the most recent commit to the RetroBSD codebase,
which is revision 506 from February 17, 2019.
.[
RetroBSD_rev506
.]
.NH
.XN "Hardware"
.\" XXX For some reason, this section shows up as pg 2 in the TOC, not pg 1.
.PP
The defining features of the target hardware for RetroBSD and DiscoBSD are
that they are RAM-constrained, have 32-bit processors,
and do not have a memory management unit (MMU).
The lack of an MMU rules out any possibility of virtual memory,
which is a critical component in most major operating systems.
A secondary feature of the target hardware is that their processors have the
ability to protect kernel code from user code with a memory protection unit.
This feature was not explored in this project,
but is a viable focus of additional study.
.bp 2
.NH 2
.XN "PIC32 Development Board"
.PP
The default development board that was used for the design and development
of RetroBSD is the Max32 board,
produced by Digilent.
It employs a PIC32MX795F512 32-bit MIPS-based microcontroller.
The processor runs at 80 MHz,
has 512KB of flash program memory, and
128KB of SRAM data memory.
It is powered either through the external power connector
or the onboard USB connector.
A serial port (UART) terminal is achieved through the USB connection.
This board does not have an onboard SD card,
but one can be made available through an external Arduino-compatible shield.
The SD card connects to the SPI2 port of the microcontroller.
83 general purpose I/O (GPIO) pins and onboard LEDs are available.
The board offers many more peripherals than outlined here,
but they have no relevance to this project.
.PP
Programming and debugging of the board is achieved through the use of the
PICkit3 in-system programmer/debugger and the connected USB cable.
The MPLAB IDE software is used on a Windows system
to load firmware into the flash memory of the microcontroller.
.KS
.PP
An image of the Max32 development board is shown below:
.[
max32eps
.]
.PSPIC max32.eps 5
.KE
.PP
.NH 2
.XN "STM32 Development Board"
.PP
The default development board that is used for the design and development
of DiscoBSD is the STM32F4-Discovery board,
.[
DB1421
.]
.\" Reference to stm32f4discovery.pdf spec sheet.
produced by STMicroelectronics.
A fully compatible revised edition of the board has been
released under the model name STM32F407G-DISC1.
All revisions of the boards employ an STM32F407VGT6 32-bit Arm Cortex-M4
microcontroller with a single-precision floating-point unit.
The processor runs at a max speed of 168 MHz,
has 1MB of flash program memory, and
192KB of SRAM data memory.
It is powered through the onboard USB connector.
A UART terminal is achieved through the USB connection.
This board does not have an onboard SD card,
but one can be made available through a connection to
the I/O pins on the extension headers.
The SD card connects to one of the microcontroller's SPI ports.
80 general purpose I/O (GPIO) pins, one User button,
and four onboard LEDs are available for use.
The board offers many more peripherals than outlined here,
but they have no relevance to this project.
.PP
A unique aspect of this microcontroller is that 64KB of its 192KB of SRAM is
Core Coupled Memory (CCM), which is only available to the processor's data bus.
This area of RAM is suitable for kernel data structures,
as it does not go through the Multi-AHB bus matrix,
thus has zero wait states and no contention for bus access.
SRAM1 has 112KB of the total 192KB of SRAM,
which is suitable for either user or kernel processes.
SRAM2 has the remaining 16KB of the total 192KB of SRAM,
which is tied to the System-bus and only available for
peripheral data transfers, i.e., USB, Ethernet, or DMA data transfers.
The SRAM2 is not used in the DiscoBSD kernel.
.\" XXX This .bp pushes the next paragraph to the next page.
.bp 3
.PP
Programming and debugging of the board is achieved through the use of the
onboard ST-LINK/V2-A in-system programmer/debugger and the connected USB cable.
The STLink software is used to load firmware,
such as the DiscoBSD kernel,
into the flash memory of the microcontroller.
Remote debugging of firmware running on the physical board is possible
through the GNU debugger, used as a remote protocol server,
paired with the OpenOCD on-chip debugger to access on-chip debug facilities.
The project's top-level Makefile has targets for
running OpenOCD in connection with the GNU debugger.
.KS
.PP
An image of the STM32F4-Discovery development board is shown below:
.[
stm32eps
.]
.PSPIC stm32.eps 5
.KE
.PP
.NH
.XN "Simulators and Emulators"
.PP
Employing either a simulator or an emulator for the development of
an embedded system is an efficient use of limited resources and
reduces unproductive time during the
code-compile-load-debug development cycle.
They are also valuable in enabling system development
during a lack of availability, or access, to the physical hardware.
.PP
Simulators and emulators are cycle-accurate representations of
the physical hardware.
They are computer programs that offer the same processor and
common peripherals as available on target development boards.
The development process amounts to loading a compiled binary firmware file,
an
.I "Intel Hex"
formatted file,
or an
.I ELF
formatted file into the simulator or emulator.
A debugger, such as the GNU debugger,
is attached and used to run and interrogate the system-under-test.
.NH 2
.XN "PIC32 VirtualMIPS Simulator"
.PP
The VirtualMIPS simulator is used to boot and run
the RetroBSD MIPS-based kernel and userland.
It is bundled with the RetroBSD codebase,
and is available in the
.CW /tools/virtualmips
directory.
The simulator executable is named
.CW pic32 .
RetroBSD compiles separate kernels for each of the
various PIC32-based development boards.
By default, VirtualMIPS is configured to simulate a
Digilent Max32 board and runs the
.CW /sys/pic32/max32
kernel along with the common MIPS-based userland.
The kernel,
named
.CW unix.bin ,
is provided as a binary firmware file.
The simulator provides virtual peripheral devices
such as an SPI port for the SD card interface,
a UART for the console terminal,
and GPIO pins for toggling LEDs.
Pulse width modulation and analog to digital converter peripherals
are also simulated.
.PP
VirtualMIPS compiles and runs on Mac OSX, OpenBSD, and Linux,
although only OpenBSD and Linux have been tested.
Debugging a RetroBSD kernel with the GNU debugger
through VirtualMIPS was not attempted,
but by all indications it is possible,
as the developers of RetroBSD debugged and developed in this manner
on Mac OSX and Linux.
.KS
.PP
An image of RetroBSD booting in the VirtualMIPS simulator is shown below:
.PSPIC RetroBSD_boot_on_VirtualMIPS.eps 5
.KE
.PP
.NH 2
.XN "QEMU-based Arm Cortex-M Emulator"
.PP
The QEMU-based Arm Cortex-M emulator (hereafter called qemu-arm) is used
to boot and run the DiscoBSD Arm-based kernel and userland.
It is available through the various package managers on Linux and
as a custom user-compiled port on OpenBSD.
.[
qemuarm
.]
.\" Reference to port in project repo.
.CW qemu-system-gnuarmeclipse
is the name of the emulator executable.
DiscoBSD currently compiles a single kernel,
targeting the STM32F4-Discovery development board.
qemu-arm is configured on the command line to emulate a
STMicroelectronics STM32F4-Discovery board and run the
.CW /sys/stm32/f4discovery
kernel along with the Arm-based userland.
Note that the Arm-based userland is not yet complete.
The kernel,
named
.CW unix.elf ,
is provided as an
.I ELF
formatted firmware file.
The emulator provides virtual peripheral devices
such as a USART for the console terminal,
and GPIO pins for toggling the four onboard LEDs
and reading from the user button.
The many other services afforded by the standard QEMU are present in
qemu-arm but have not been explored in this project.
.KS
.PP
An image of DiscoBSD booting in the qemu-arm emulator is shown below:
.PSPIC DiscoBSD_boot_on_QEMU.eps 5
.KE
.PP
The qemu-arm emulator runs on OpenBSD and Linux,
although only OpenBSD has been used for development.
Debugging a DiscoBSD kernel running on qemu-arm with the GNU debugger
is possible and is integrated into the codebase.
The project's top-level Makefile has targets for both
running qemu-arm and running the GNU debugger with qemu-arm as the target.
.NH
.XN "Host Development Environment"
.PP
This project was developed on Unix-based host operating systems.
Development was mainly on the OpenBSD operating system,
while compatibility and portability testing was performed on Linux as a host.
The original RetroBSD project was developed on Mac OSX and Linux,
with support for FreeBSD as a host near the end of RetroBSD's timeline.
.PP
The DiscoBSD host development environment consists of
a number of main development tools:
.RS
.RS
.IP \(bu 4
a binary flash downloader
.IP \(bu 4
a circuit board simulator or emulator
.IP \(bu 4
a compiler, assembler, and C library
.IP \(bu 4
a source-level debugger
.IP \(bu 4
an on-chip debugger
.RE
.RE
.PP
As supporting tools,
these commonly-present Unix programs are also required:
.RS
.RS
awk, bison, byacc, gmake, sed, shell (either Bourne or Bash)
.RE
.RE
.PP
The host development environment created for this project is
targeted to build and develop for both
the MIPS-based RetroBSD kernel
and the Arm-based DiscoBSD kernel.
A specific aim of this project is for the codebase to
concurrently support many architectures,
starting with the original MIPS code and then
expanded with the new Arm code developed for this project as DiscoBSD.
.NH 2
.XN "Development Tools on OpenBSD"
.PP
Significant resources were allocated to the construction of a
suitable development environment for both MIPS and Arm targets
on a Unix-based operating system, as an alternative and addition
to the well-established Linux operating system.
OpenBSD was chosen for this task,
as it is dissimilar in many ways to Linux,
while still maintaining POSIX compliance.
This satisfies an aim of this project for the development and testing
of portability between various host development systems.
.PP
Using the OpenBSD Ports Collection,
.[
OpenBSDPorts
.]
.\" Reference to OpenBSD ports collection in the OpenBSD FAQ.
custom user-compiled ports of third-party software was developed.
These include the mips-elf targeted GCC toolchain,
the STLink binary flash downloader for STM32 devices,
and the qemu-arm circuit board emulator.
The remaining software packages needed for development are available
as an OpenBSD package through the
.I pkg_add
system.
These packages include the OpenOCD on-chip debugger,
and the Linaro version of GCC targeted to arm-none-eabi,
with the associated Binutils, Newlib, and GNU debugger.
.[
OpenBSDDevEnv
.]
.\" Reference to tools/openbsd/README.md
.NH 2
.XN "Development Tools on Linux"
.PP
The Linux development environment has proven to be less of a challenge
in regards to the custom compiling and patching of development tools.
This is mainly due to many of the tools used in this project
to have originally been developed on Linux.
Note that Linux was not used for the bulk of the development of DiscoBSD;
portability in compiling and running of the project was the main focus.
.PP
The development tools required for the MIPS-based RetroBSD are
available in the documentation that comes with RetroBSD.
A user-compiled version of GCC targeted to mips-elf,
and the supporting Binutils,
can be obtained by running the supplied build script or
by using the pre-built binaries available with the documentation.
.[
MIPSGCCRetroBSD
.]
.\" Reference to MIPS toolchain documentation on RetroBSD.org
Supporting tools are either default programs on the operating system
or are added via the particular Linux package manager.
The BSD version of yacc, called byacc,
is a required program that is available on OpenBSD by default but not on Linux.
.PP
All of the main and supporting tools, outlined in Section 5,
that are required to compile and develop DiscoBSD
are available as packages through the particular Linux's package manager.
This includes GCC targeted to arm-none-eabi,
and the associated Binutils, Newlib, and GNU debugger.
.\" XXX This .bp pushes the next section to the next page.
.bp 7
.NH
.XN "Kernel Operation Overview"
.PP
Coverage of the kernel operation will be limited to the relevant issues
for this project.
System startup, process creation, and process management
will be covered in outline in this section.
For example, signals, communication facilities,
and the filesystem will not be covered, but are, nonetheless,
important facilities of any kernel.
.PP
The kernel gets loaded into RAM by reset and bootstrap code
in the system startup sequence, and then execution is passed to it.
It sets up the
.I swapper
process (PID 0), which the kernel will eventually become.
The kernel then hand-crafts the first new process (PID 1) which will be the
.I init
process.
The
.I init
process is the ancestor, and parent process,
of all future processes in the system.
Once
.I init
is created by a kernel-specific form of
.CW fork() ,
then the kernel becomes the
.I swapper
and manages scheduling processes.
.PP
In a roundabout and convoluted way, the
.I init
process loads the program
.CW /sbin/init
from the filesystem and it is set executing.
The
.I swapper
process eventually schedules the
.I init
process and runs it, which runs the
.CW /sbin/init
executable.
.CW /sbin/init
spawns a shell to interpret the commands in
.CW /etc/rc ,
then forks a copy of itself to invoke
.CW /libexec/getty ,
which further invokes
.CW /bin/login
to log a user on.
Upon a successful login,
.CW /bin/login
uses a call to
.CW exec()
to overlay itself with the user's shell.
The system is now in the position that general
.I "user mode"
programs can now be run by users through their shell,
and they will be scheduled and executed by the kernel
.I swapper
process.
.PP
The kernel uses a full swap policy wherein there can only be
one process running in RAM at a time,
in addition to the always-present kernel
.I swapper
process.
The processes not currently running will be swapped out to the
.I "swap area"
on the disk,
which in this case is a filesystem partition on the mounted SD card.
The reasoning for this policy is that the available RAM to the system
is not large enough to support multiple in-core processes.
This is a defining, and unavoidable, constraint of DiscoBSD.
.NH
.XN "System Startup"
.PP
.\" XXX Put this into a numbered list.
After a system hardware reset, the kernel gets loaded into RAM from Flash by
initial reset code and execution begins at the kernel's entry point,
which eventually arrives at the kernel's
.CW main()
function.
Machine dependent (MD) peripherals are set up and initialized.
The kernel's various data structures and services are initialized.
.\" XXX Machine independent (MI) peripherals are also set up and initialized.
The filesystem is mounted and set up.
The
.I init
process is created and forked.
The kernel process becomes the
.I swapper
to schedule all system processes.
The code for
.CW /sbin/init
is loaded from the filesystem into user memory and the
.I init
process "returns" to location zero of the code in user memory to execute it.
The specifics of how all this happens is covered in the following subsections.
.NH 2
.XN "Bootstrapping and Linker Script"
.PP
The default bootloader in STM32F4xx microcontrollers is set by the
.CW BOOT0
(held low by default) and
.CW BOOT1
(held high by default) pins.
This selects the main Flash memory as the boot space,
starting at address
.CW 0x00000000 .
.PP
There are two linker scripts that concern this operating system:
one for the kernel and one for user executables.
The former will be discussed in this section.
.PP
A linker script is a specifically formatted file that instructs the linker
\*- as the last step of the compilation process \*-
on how to lay out the various sections of the executable.
This amounts to placing kernel code in the read-only
.I .text
section,
initialized data in the read and write
.I .data
section, and
specifying where the
.I .bss
section is located for uninitialized data and variables.
The stack pointer is also placed accordingly,
normally at the end of RAM for the full-descending stack on the Arm Cortex-M4.
The stack pointer is defined by the label
.I _estack
and it is located at the end of RAM at address
.CW 0x20020000 .
.KS
.PP
A trimmed down version of the kernel's linker script is as follows:
.PP
.DS L
.B1
.CW ""
.CW "\tMEMORY {"
.CW "\t    FLASH (r x)  : ORIGIN = 0x08000000, LENGTH = 1024K"
.CW "\t    RAM   (rwx)  : ORIGIN = 0x20000188, LENGTH = 32K - 0x188"
.CW "\t    U0AREA(rw!x) : ORIGIN = 0x20008000, LENGTH = 3K"
.CW "\t    UAREA (rw!x) : ORIGIN = 0x20008C00, LENGTH = 3K"
.CW "\t}"
.CW ""
.CW "\t/* Higher addresses of the user mode stacks. */"
.CW "\tu0    = ORIGIN(U0AREA);"
.CW "\tu     = ORIGIN(UAREA);"
.CW "\tu_end = ORIGIN(UAREA) + LENGTH(UAREA);"
.CW ""
.CW "\t_estack = 0x20020000;"
.CW ""
.CW "\tENTRY(Reset_Handler)"
.CW ""
.CW "\tSECTIONS {"
.CW "\t    .text : {"
.CW "\t        KEEP(*(.isr_vector))"
.CW "\t        *(.text*)"
.CW "\t        *(.rodata*)"
.CW "\t    } > FLASH"
.CW "\t    _etext = .;"
.CW ""
.CW "\t    .data : AT (_etext) {"
.CW "\t        _sdata = .;"
.CW "\t        *(.data*)"
.CW "\t        . = ALIGN(8);"
.CW "\t        _edata = .;"
.CW "\t    } > RAM"
.CW ""
.CW "\t    .bss : {"
.CW "\t        . = ALIGN(8);"
.CW "\t        _sbss = .;"
.CW "\t        *(.bss*)"
.CW "\t        *(COMMON)"
.CW "\t        . = ALIGN(8);"
.CW "\t        _ebss = .;"
.CW "\t    } > RAM"
.CW "\t}"
.CW ""
.B2
.DE
.KE
.PP
All execution starts at
.CW ENTRY(\fIlabel\fP) where
.I label
is
.I Reset_Handler
on DiscoBSD (historically
.I start ).
In Arm Cortex-M4, the first 32 bits (first word) of the executable is
actually the address of the stack pointer,
and the second word is the address of
.I label .
This is handled by the linker.
.I label
refers to a label in the architecture-specific assembly language startup code.
This code will be covered in the next section.
.\" XXX Mention about User Structure here? See page 77 in D&I 4.3BSD.
.NH 2
.XN "Assembly Language Startup"
.PP
The assembly language startup code differs greatly between MIPS and Arm.
The MIPS startup code is entirely contained in the file
.CW /sys/pic32/startup.S ,
whereas Arm and STM has standardized on an elaborate set of files that are
common amongst each family of microcontrollers.
These standardized files are available from STMicroelectronics,
.[
STM32F4Cube
.]
.\" Reference to file availability from ST website.
the microcontroller vendor for STM32F407xx devices.
.PP
The following files are required by Arm for CMSIS functions:
.RS
.RS
.IP \(bu 4
.CW cmsis_gcc.h
.IP \(bu 4
.CW core_cm4.h
.IP \(bu 4
.CW core_cmFunc.h
.IP \(bu 4
.CW core_cmInstr.h
.IP \(bu 4
.CW core_cmSimd.h
.RE
.RE
.PP
The following files are required by STM for
processor and SysTick initialization:
.RS
.RS
.IP \(bu 4
.CW startup_stm32f407xx.s
.IP \(bu 4
.CW stm32_assert.h
.IP \(bu 4
.CW stm32f407xx.h
.IP \(bu 4
.CW stm32f4xx.h
.IP \(bu 4
.CW stm32f4xx_it.c
.IP \(bu 4
.CW stm32f4xx_it.h
.IP \(bu 4
.CW system_stm32f4xx.c
.IP \(bu 4
.CW system_stm32f4xx.h
.RE
.RE
.PP
The Arm file that contains the label
.I Reset_Handler
is
.CW /sys/stm32/startup_stm32f407.s
and is the file that starts all execution.
This file is specific to STM32F407xx microcontrollers.
Other microcontrollers in the STM32F4xx family have similar startup files,
named in a comparable way.
.KS
.PP
The structure of the code in
.CW startup_stm32f407xx.s
is as follows (shortened for brevity):
.PP
.DS L
.B1
.CW ""
.CW "\t.global Reset_Handler"
.CW ""
.CW "\tReset_Handler:"
.CW "\t\tldr\tsp, =_estack\t\t/* Set stack pointer. */"
.\" XXX This is what the code from Arm does,
.\" XXX but it doesn't make sense for setting up u0 and u.
.CW ""
.CW "\t\t/* Code to copy .data segment from flash to SRAM. */"
.CW ""
.CW "\t\t/* Code to fill .bss segment with zeros. */"
.CW ""
.CW "\t\tbl SystemInit\t\t/* Init system clock. */"
.CW ""
.CW "\t\tbl main\t\t\t\t/* Call main() in kernel. */"
.CW ""
.CW "\t\t/* Once main() returns here as PID 1: */"
.\" XXX This part of the code in DiscoBSD has not been written.
.CW "\t\t/*   enter user mode, */"
.CW "\t\t/*   run icode at address zero (to exec /sbin/init). */"
.CW "\t\t/* This is described in Section 7.4. */"
.CW ""
.B2
.DE
.KE
.PP
Exception handlers and interrupt service routines are defined and handled in
.CW stm32f4xx_it.c .
The Arm-required
.CW SystemInit()
function,
which is called from the startup assembly code shown above,
is defined in
.CW system_stm32f4xx.c .
The various header files have defines for the standard Arm environment.
Once the startup assembly code calls the
.CW main()
routine,
the kernel proper is running C code and
will start the kernel initialization process.
.\" XXX This .bp pushes the next section to the next page.
.bp 10
.NH 2
.XN "Kernel Initialization"
.PP
Kernel initialization is completely contained in the file
.CW init_main.c ,
which is where the
.CW main()
routine is located.
The kernel starts in
.I "kernel mode" .
.PP
The
.CW startup()
routine initializes machine dependent (MD) peripherals.
.CW startup()
is defined in
.CW /sys/stm32/machdep.c
and is highly specific to the processor architecture and
the available peripherals on the target board.
For example,
this is where LEDs and GPIO pins are initialized.
.PP
Kernel autoconfiguration is performed with a call to
.CW kconfig() ,
which probes for all the devices available to the system at boot time.
This is a dynamic process, and as such,
allows flexibility in the presence of optional devices.
The absence of any required standard device will cause the kernel to panic.
Kernel configuration is explained in more detail in Section 8.
.PP
The system process structure
.I "struct proc" ) (
for PID 0 is set up.
Each process in the system has an entry in the process table in the kernel.
The process table is implemented as an array of
.I "struct proc"
entries.
The process structure must always remain in main memory,
no matter the current state of the process.
.[
DI43BSDOSpg72
.]
.\" Reference to D&I 4.3BSD page 72
.PP
The init user structure
.I "struct user" ) (
is set up.
The user structure is quite unique.
There are two instances of the user structure:
.I u0
and
.I u ,
which are declared in the linker script.
.I u0
is dedicated to PID 0, the
.I swapper
process.
.I u
is the user structure of the in-core active process.
.[
DI43BSDOSpg77to78
.]
.\" Reference to D&I 4.3BSD pages 77-78
The user structure of any process not currently in a
.I runnable
state is swapped out.
.PP
Next, signals are initialized.
The kernel's various data structures, tables, and
protocols are initialized.
Well-known inodes are set up.
The kernel clock is set up.
Services are attached to the kernel.
.\" XXX Machine independent (MI) peripherals are also set up and initialized.
Detailed coverage of these topics is beyond the scope of this report.
.PP
The root filesystem is mounted.
If no root filesystem is found, the kernel will panic.
The swap file on the root filesystem is opened and cleared.
If no swap file is found, the kernel will also panic.
Timeout driven kernel events are started.
Finally, the root filesystem is set up.
.PP
The next section will continue the kernel initialization
with the final task of setting up a working kernel:
getting
.CW /sbin/init
to run.
.NH 2
.XN "Getting to /sbin/init"
.PP
Continuing on in the
.CW main()
routine,
and following the set up of the root filesystem,
the
.I init
process is created by the kernel-specific version of
.CW fork()
called
.CW newproc() .
The kernel process (as the parent process) officially becomes the
.I swapper
to schedule all system processes by calling the
.CW sched()
routine,
which never returns.
The child process of the fork
is the
.I init
process.
In the
.I init
process,
the code for a small assembly language routine called
.CW icode
is copied from the kernel image to the start of user memory.
.KS
.PP
The routine is effectively the same as the following program:
.[
DI43BSDOSpage405
.]
.\" Reference to page 405 in D&I 4.3BSD OS
.PP
.DS L
.B1
.CW ""
.CW "\tmain()"
.CW "\t{"
.CW "\t\tchar *argv[2];"
.CW ""
.CW "\t\targv[0] = \(dqinit\(dq;"
.CW "\t\targv[1] = 0;"
.CW "\t\texit(execv(\(dq/sbin/init\(dq, argv));"
.CW "\t}"
.CW ""
.B2
.DE
.KE
.PP
The last task in the
.CW main()
routine is for the
.I init
process to "return" to location zero of the code in user memory and execute it.
In effect, the return is from the branch to
.CW main()
in the startup assembly code,
and is a
.I thunk
to run the
.CW icode
just copied out.
This process has been been, rightly so,
described as "somewhat enigmatic" by John Lions
in his famous
.I "Commentary on UNIX 6th Edition" .
.[
LionsUnixpg6dash2
.]
.\" Reference to page 6-2 in Lions' Commentary.
The call to
.CW execv()
replaces the image of the
.I init
process with the userland image of
.CW /sbin/init ,
which is loaded from the mounted root filesystem.
It is especially important to understand that
.CW /sbin/init
is running in
.I "user mode" ,
not in
.I "kernel mode" ,
as a regular user process.
.NH 2
.XN "Getting to the User's Shell"
.PP
As shown in the previous section,
the
.I init
process starts up the
.CW /sbin/init
userland program,
and exits if the call to
.CW execv()
fails.
This makes the presence of
.CW /sbin/init
vital to the system bootstrapping procedure.
.[
DI43BSDOSpg405to406
.]
.\" Reference to page 405-406 in D&I 4.3BSD OS.
.PP
.CW /sbin/init
forks itself and spawns a shell to interpret the commands in
.CW /etc/rc ,
which performs various tasks such as filesystem consistency checks,
and starting up daemon processes like
.CW /sbin/cron
and
.CW /etc/update .
.CW /sbin/init
then forks a copy of itself for each terminal device that is marked
for use in the file
.CW /etc/ttys .
Each copy of
.CW /sbin/init
invokes
.CW /libexec/getty
to manage signing on to the system.
.CW /libexec/getty
eventually reads in a user's login name from its terminal and invokes
.CW /bin/login
to complete the login sequence.
Once the user password check is complete,
.CW /bin/login
uses an
.CW exec()
call to overlay itself with the user's shell
(normally
.CW /bin/sh ,
the standard Bourne shell).
.PP
The system is now, finally,
in a state to be commanded by users in the usual way.
.[
DI43BSDOSpg406to407
.]
.\" Reference to pages 406-407 in D&I 4.3BSD OS.
.NH
.XN "Kernel Configuration"
.PP
The kernel configuration program
.CW /tools/kconfig/kconfig
is used to configure a kernel,
based on the
.CW Config
file in the build directory, namely
.CW /sys/stm32/f4discovery/Config .
The support files
.CW Makefile.kconf ,
.CW devices.kconf ,
and
.CW files.kconf
in the
.CW /sys/stm32
directory are used in the configuration process.
Cursory coverage of
.CW kconfig
will be outlined below,
while detailed information is available from the
.CW kconfig
documentation.
.[
RetroBSDkconfig
.]
.\" Reference to kconfig RetroBSD webpage.
.PP
The purpose of
.CW kconfig
is to generate a
.CW Makefile ,
which is used to compile a specific kernel.
.CW Makefile.kconf
is a template
.CW Makefile
that has default build rules and directives,
as well as anchors to attach generated build rules.
The names of specific source files used to build the kernel
are retrieved from the file
.CW files.kconf
by matching both standard kernel files and optional device drivers.
.CW devices.kconf
contains a list of block devices and their major numbers for the filesystem.
.KS
.PP
A basic kernel configuration is possible with the following
.CW Config
configuration file:
.PP
.DS L
.B1
.CW ""
.\" XXX This should really be called machine, for 'stm32'.
.\" XXX And maybe architecture should be 'arm'.
.CW "\tarchitecture\t\t\(dqstm32\(dq\t\t\t\t\t# Processor architecture"
.CW "\tcpu\t\t\t\t\(dqSTM32F407xx\(dq\t\t\t# Processor variant"
.CW "\tboard\t\t\t\(dqF4DISCOVERY\(dq\t\t\t# Board type"
.CW "\tldscript\t\t\t\(dqf4discovery/STM32F407XG.ld\(dq\t# Linker script"
.CW ""
.CW "\toptions\t\t\t\(dqCPU_KHZ=80000\(dq\t\t\t# CPU core osc freq"
.CW "\toptions\t\t\t\(dqBUS_KHZ=80000\(dq\t\t\t# Peripheral bus freq"
.CW "\toptions\t\t\t\(dqBUS_DIV=1\(dq\t\t\t\t# Bus clock divisor"
.CW ""
.CW "\tconfig\t\t\tunix\t\troot on sd0a\t\t# Root filesystem"
.CW "\t\t\t\t\t\t\tswap on sd0b\t\t# Swap partition"
.CW ""
.CW "\tdevice\t\t\tuart1\t\t\t\t\t# Serial UART port 1"
.CW "\toptions\t\t\t\(dqCONS_MAJOR=UART_MAJOR\(dq\t# UART1 as console"
.CW "\toptions\t\t\t\(dqCONS_MINOR=0\(dq\t\t\t# /dev/tty0"
.CW ""
.CW "\tcontroller\t\tspi2\t\t\t\t\t\t# SD card"
.CW ""
.CW "\tdevice\t\t\tsd0\t\tat spi2 pic RC14\t# SD card select pin"
.CW "\toptions\t\t\t\(dqSD_MHZ=10\(dq\t\t\t\t# SD card speed 10 MHz"
.CW ""
.B2
.DE
.KE
.PP
Note that the full functionality of STM32-specific configuration
has not yet been added to
.CW kconfig .
A fully working
.CW Makefile
that is able to compile the DiscoBSD kernel,
using the above configuration defines,
has been created by hand.
.NH
.XN "Userland"
.PP
The userland consists of all parts of an operating system that
are not part of the system kernel proper.
Specifically, the shell, editors, the various system libraries,
and other user programs constitute the userland.
These programs interact with the kernel through
.I "system calls" ,
which are well-defined entry points into the kernel
.[
APUEpg20
.]
that request specific kernel services,
such as reading or writing to a file.
This affords a separation of interests between user applications
and the system and hardware management tasks of the kernel.
.KS
.PP
An image of the architecture of a Unix system is shown below:
.[
DUNIXOSpg5
.]
.\" Reference to Design of Unix System page 5.
.PSPIC Unix_Shell.eps 4
.KE
.PP
A defining difference between Linux-based and BSD-based operating systems
is that BSD-based systems are unified and complete,
composed of a kernel and a userland.
Linux is the kernel proper of a Linux-based system
and distribution creators pair the Linux kernel
with a userland of their choice, most commonly the GNU system.
.PP
The userland of DiscoBSD is not the focus of this project
but it deserves cursory attention in regards to the kernel porting effort.
The major areas of consideration are the C runtime startup code,
low-level assembly language routines in the C library for
various tasks such as string manipulations,
and the linker script for the memory layout of user executables.
.PP
Userland code is completely contained in the
.CW /src
directory.
An Arm-specific directory for the C runtime startup code has been created at
.CW /src/startup-arm
and the Arm-specific directory tree for the various C library
low-level assembly language routines has been created at
.CW /src/libc/arm .
The linker script for Arm executables is
.CW /src/elf32-arm.ld .
The selection of Arm-specific or MIPS-specific code is dependent
on specific build variables,
covered in the following two sections.
.NH
.XN "Build System"
.PP
Through the use of the previously covered host development environment,
a complete RetroBSD operating system
\*- and mostly complete DiscoBSD operating system \*-
can be constructed with the standard build features included in the codebase.
The build system is structured as a collection of build variables
and a hierarchy of Makefiles.
The
.CW make
build software (specifically
.CW gmake )
manages build relations between all source files and their dependencies.
The top-level Makefile orchestrates the compilation of build system tools,
system libraries, userland programs and their associated manual pages,
and the system kernel.
The final step in the build process is the creation of a filesystem image
for installation onto an SD card.
The executables, libraries, and supporting documentation are installed
into the root filesystem according to the configuration in the
.CW rootfs.manifest
filesystem manifest file.
The kernel is not installed into the filesystem,
rather it is installed into the flash memory of the microcontroller.
This procedure is performed via the specific tools associated with
the microcontroller.
.PP
The top-level Makefile has targets
for all the previously outlined build steps.
The standard process for building the system is to invoke
.CW gmake
from the root directory of the codebase,
which follows the creation of all dependencies
until the whole system is built.
Specific targets may be invoked by appending the target name after
.CW gmake
on the command line.
.NH 2
.XN "Multi-Architecture Features"
.PP
DiscoBSD's build system and its hierarchy of Makefiles have been amended to
support the ability to host multiple architectures under one unified codebase.
This development towards the concurrent support of many architectures
is a major aim of this project.
Two architectures are currently supported.
.PP
.I "Compulsory environment variables"
is the method used to achieve support for multiple architectures.
This simple method has historically been used to great success,
and is exemplified in the highly portable
NetBSD
.[
NetBSDbuildsh
.]
and OpenBSD operating systems.
The compulsory environment variables
.CW MACHINE
and
.CW MACHINE_ARCH
choose which hardware platform and processor architecture, respectively,
to use in compiling the system.
.CW MACHINE
derives from the command
.CW "uname -m" \*[U] \*[Q]
while
.CW MACHINE_ARCH
derives from the command
.CW "uname -p" \*[U] \*[Q]
on all Unix systems.
This structure enables the possibility of future ports
to other platforms and architectures.
.KS
.PP
The default platform and architecture for DiscoBSD are
.I stm32
and
.I arm ,
respectively.
To target the MIPS-based RetroBSD,
define
.CW MACHINE
as
.I pic32
and
.CW MACHINE_ARCH
as
.I mips .
This can be performed either by setting the environment variables
through the shell's functionality or by setting the environment variables
on the command line when invoking
.CW gmake ,
as shown below:
.PP
.DS L
.CW "\t $ MACHINE=pic32 MACHINE_ARCH=mips gmake"
.DE
.KE
.PP
Another multi-architecture feature enabled in DiscoBSD,
and alluded to in Section 5,
is the ability of the build system to detect the host operating system and
choose build and support tools that are specific to each operating system.
This process is automatic and developer input is not required.
The supporting Makefiles
.CW /target.mk ,
.CW /sys/stm32/gcc-config.mk ,
.CW /sys/pic32/gcc-config.mk ,
and
.CW /Makefile.inc
are responsible to selectively choose build and support tools
based on present operating system features.
The currently supported and tested host operating systems are
Linux and OpenBSD.
Although support for Mac OSX and FreeBSD was previously added to RetroBSD,
these systems have not been tested,
so their status is indeterminate.
.NH
.XN "Project Difficulties"
.PP
This project has been riddled with challenges and difficulties;
some small, while others were quite substantial.
The na\[:i]vet\['e] of thinking that a 50 year old codebase,
crafted over many tens of thousands of hours by
some of the world's best computer scientists,
could be fully ported with a complete kernel
in a four month semester cannot be underestimated.
The project is comprised of two overarching themes,
wherein each theme had their own particular difficulties:
a case study of the kernel of the RetroBSD operating system and
the development of the Arm-based DiscoBSD kernel.
The difficulties encountered and overcome in each of these themes will be
explored in turn in the following sections.
.NH 2
.XN "Case Study of Kernel Operation"
.PP
As RetroBSD derives from 2.11BSD,
which derives directly from Sixth Edition Unix and
indirectly from Seventh Edition and 32V Unix,
the vast historical literature of Unix development was
interrogated for knowledge of the system's operation.
Although there are books written specifically targeted to
4.4BSD,
.[
TheDI44BSDOS
.]
4.3BSD,
.[
DI43BSDUNIXOS
.]
Unix System V Release 2,
.[
TheDUNIXOS
.]
and Unix Sixth Edition,
.[
LionsCommentaryUnix
.]
there are no definitive works that cover 2.11BSD.
The combination of the
.I "Commentary on UNIX 6th Edition"
combined with the coverage of 4.3BSD in
.I "The Design and Implementation of the 4.3BSD UNIX Operating System"
is the closest there is to an authoritative reference on
2.11BSD system internals.
This made understanding the RetroBSD kernel quite challenging.
.\" XXX fix this sentence.
Synthesizing this disparate information,
in addition to effective code tracing and debug single-stepping,
allowed for a sufficient level of understanding of kernel operation and,
in turn, enabled and initiated the code-level porting process.
.NH 2
.XN "Development of Arm-based Kernel"
.PP
Before work on the kernel could begin,
a host development environment that targets Arm processors
needed to be designed and validated for efficacy.
As detailed in Section 5,
many different tools were required for this development effort.
Of note, the QEMU-based Arm Cortex-M emulator was a
challenge to port to the development environment.
In addition,
the older MIPS-based GCC compiler proved to be
of considerable difficulty to port,
and required custom patches to GCC to enable passing floating-point options
between the compiler and the assembler.
The modern version of the Arm-targeted GCC compiler exposed many bugs and
non-critical compiler warnings.
These issues needed to be fixed before development on the kernel could begin.
All this work has been a part of the additional goals of the project to
adapt the large-scale codebase of RetroBSD to
modern and sustainable development standards.
.PP
Arm uses a standardized set of initialization and configuration files
across all microcontrollers with a Cortex-M processor core.
Integrating these files into the codebase of DiscoBSD proved challenging,
in particular,
in finding a compromise between the imposed structure of the Arm files and
the historically validated structure of DiscoBSD.
More work in this area may reveal an optimal and symbiotic solution.
.NH
.XN "Future Work"
.PP
Even with the work completed throughout this project,
there remains a few significant barriers to a full Arm-based DiscoBSD kernel.
First, a user/kernel syscall API needs to be devised and validated.
The Procedure Call Standard for the Arm Architecture
.[
AAPCS
.]
could be a starting point for this work.
Userland and the C library code for user executables needs to be
completed and validated for proper function.
This would be a long-term task,
as there are potentially many difficulties
that may emerge throughout this work.
.PP
Kernel drivers for useful peripherals need to be written.
A UART driver and an SPI-based SD card driver would be enough
for the system to stand on its own and be commanded by a user.
These could be based off the embedded systems drivers supplied
by STMicroelectronics.
A GPIO driver would enable more functionality in the system
with minimal implementation effort.
.PP
The implementation of kernel memory protection from user processes
is a potential long-term goal.
The STM32F4xx family of microcontrollers is endowed with
a memory protection unit that is dedicated to this function.
A system that offers reliable service must guarantee
some sort of memory protection.
.NH
.XN "Conclusion"
.PP
This report surveyed the relevant operation of the Unix kernel
in a case study of an MMU-less operating system
targeted to low resource microcontrollers,
and documented the process of porting the kernel
to the Arm Cortex-M4 architecture.
RetroBSD was chosen for this project,
as it is itself a port of 2.11BSD from the PDP-11 to
the MIPS-based PIC32 microcontroller.
The specific areas of focus in this study were
the technical considerations,
the host development environment,
the hardware platform,
and the software emulator.
Many and varied issues and compromises were experienced and settled
during the process of porting the RetroBSD kernel to DiscoBSD.
Additionally, the build system was updated to facilitate and
enable future ports to processor architectures beyond MIPS and Arm.
.PP
There remains much work to be done to achieve the lofty,
but attainable,
goal of a fully self-hosted DiscoBSD operating system running on
an Arm Cortex-M4 processor.
In continuing and building on the work completed in this project,
that possibility will one day become reality.
.bp
.R1
.R2
.TC
.bp
.\" XXX Force the page to be numbered as ii.
.ds CH -ii-
.SH
.XN "Glossary"
.\" XXX Force the next page to be numbered as iii.
.LP
.ds CH -iii-
.IP AAPCS
.br
XXX Stuff about Arm Architecture Procedure Call Standard
.IP API
.br
XXX About the Application programming interface for the user/kernel syscalls.
.IP Arm
.br
XXX Stuff about Arm
.IP BSD
.br
XXX About the berkley software distribution
XXX Add in all the different versions. Namely 2.11BSD.
.IP CCM
.br
XXX About the core coupled memory
.IP CMSIS
.br
XXX Stuff about Arm CMSIS standard
.IP Cortex-M4
.br
XXX About the Cortex for microcontrollers
.IP DEC
.br
XXX About digital equipment corp
.IP DiscoBSD
.br
XXX About the DiscoBSD OS
.IP DMA
.br
XXX About the direct memory access.
.IP "ELF format"
.br
XXX About the ELF formatted file
.IP GPIO
.br
XXX About the general purpose ios.
.IP "Intel Hex format"
.br
XXX About the Intel Hex formatted file
.IP "Kernel mode"
.br
XXX Stuff about Kernel mode.
.IP LED
.br
XXX About the light emiting diodes
.IP MIPS
.br
XXX About the MIPS architecture.
.IP MMU
.br
XXX About the memory management unit
.IP "PDP-11"
.br
XXX About the pdp
.IP PIC32
.br
XXX About the PIC32 chip from Microchip.
.IP PID
.br
XXX About the process identifier
.IP POSIX
.br
XXX About the POSIX standard.
.IP QEMU
.br
XXX About the QEMU emulator
.IP RAM
.br
XXX About the random access memory
.IP RetroBSD
.br
XXX About the RetroBSD OS
.IP "SD Card"
.br
XXX About the SD card for file system.
.IP SPI
.br
XXX About the serial communication port
.IP SRAM
.br
XXX About the sram
.IP STM
.br
XXX Stuff about STMicro, or STM32F4 for family.
.IP SysTick
.br
XXX Stuff about the SysTick that is for all Arm chips.
.IP UART
.br
XXX Universal Asynchronous Reciever Transmitter
.IP USART
.br
XXX About the Universal Synchronous Asynchronous Reciever Transmitter
.IP "User mode"
.br
XXX Stuff about User mode.
.IP VAX
.br
XXX About the dec vax
.\" Return to regular page numbering.
.ds CH -\En[PN]-
