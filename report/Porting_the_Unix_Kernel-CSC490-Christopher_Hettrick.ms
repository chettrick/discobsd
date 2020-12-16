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
University of Victoria
Department of Computer Science
CSC490
Supervised by Dr. Bill Bird
.AB
This report describes the process of porting a variant of the Unix kernel
from the MIPS architecture to the Arm architecture.
A heavily modified 2.11BSD version of the Unix kernel called RetroBSD is used
as a case study, and is the basis of this development.
The goal of this project is to run this ported kernel on both a simulator
and on a physical embedded development board.
An additional portion of this work is devoted to adapting
the large-scale codebase of RetroBSD to more modern and
sustainable development standards that will facilitate future ports to
other platforms and architectures.
.\" XXX Talk about the host environment, and how it works on Linux and OpenBSD.
.AE
.NH
.XN "Introduction"
.PP
Porting the MIPS32\(rg M4K\(rg architecture to the
Arm\(rg Cortex\(rg-M4 architecture.
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.\" XXX Fill out this section.
.\" XXX Add specifics about DiscoBSD, so future references are understandable.
.NH
.XN "Relevant History of BSD"
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
The project was started and lead by Serge Vakulenko,
a systems programmer who started working at MIPS Technologies in 2011.
.[
HomepageSerge
.]
.PP
2BSD is a family of operating systems for the DEC\(tm PDP-11 derived from
Research UNIX and developed at the University of California at Berkeley.
2.11BSD has a long lineage going back to the first release of
2BSD on May 10, 1979.
.[
PUPS
.]
2BSD is a direct descendant of the Sixth Edition of Research UNIX,
commonly known as V6 UNIX.
2.8BSD incorporated features from the Seventh Editition of Research UNIX,
32V UNIX, and 4.1BSD.
.\" XXX Add reference to UNIX system family tree, pages 5-6 of D&I 4.4BSD OS.
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
.PP
DiscoBSD derives from the most recent commit to the RetroBSD codebase,
which is revision 506 from February 17, 2019.
.[
RetroBSD_rev506
.]
.NH
.XN "Hardware"
.PP
The defining features of the target hardware for RetroBSD and DiscoBSD are that
they are RAM-constrained, have 32-bit processors,
and do not have a memory management unit (MMU).
The lack of an MMU rules out any possibility of virtual memory,
which is a critical component in most major operating systems.
A secondary feature of the target hardware is that their processors have the
ability to protect kernel code from user code with a memory protection unit.
This feature was not explored in this project,
but is a viable focus of additional study.
.NH 2
.XN "PIC32 Development Board"
.PP
The default development board that was used for the design and development
of RetroBSD is the Max32 board,
produced by Diligent.
It employs a PIC32MX795F512 32-bit MIPS-based microcontroller.
The processor runs at 80 MHz,
has 512KB of flash program memory, and
128KB of SRAM data memory.
It is powered either through the external power connector
or the onboard USB connector.
A UART terminal is achieved through the USB connection.
This board does not have an onboard SD card,
but one can be made available through an external Arduino-compatible shield.
The SD card connects to the SPI2 port of the microcontroller.
83 general purpose I/O (GPIO) pins and some onboard LEDs are available.
The board offers many more peripherals than outlined here,
but they have no relevance to this project.
.PP
Programming and debugging of the board is achieved through the use of the
PICkit3 in-system programmer/debugger and the connected USB cable.
The MPLAB IDE software is used on a Windows system
to load the firmware into the flash memory in the microcontroller.
.KS
.PP
An image of the Max32 development board is shown below:
.PSPIC max32.eps 5
.KE
.PP
.NH 2
.XN "STM32 Development Board"
.PP
The default development board that is used for the design and development
of DiscoBSD is the STM32F4Discovery board,
produced by STMicroelectronics.
A fully compatible revised edition of the board has been
released under the model name STM32F407G-DISC1.
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.KS
.PP
An image of the STM32F4Discovery development board is shown below:
.PSPIC stm32.eps 5
.KE
.PP
.NH
.XN "Simulators and Emulators"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH 2
.XN "PIC32 VirtualMIPS Simulator"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.KS
.PP
An image of RetroBSD booting in the VirtualMIPS simulator is shown below:
.PSPIC RetroBSD_boot_on_VirtualMIPS.eps 5
.KE
.PP
.NH 2
.XN "QEMU-based Arm Cortex-M Emulator"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH
.XN "Host Development Environment"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH 2
.XN "Development Tools on OpenBSD"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH 2
.XN "Development Tools on Linux"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH
.XN "Kernel Operation Overview"
.PP
.\" XXX Fix for flow. What we are covering and then what *not* covering.
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
normally at the end of RAM for the full-decending stack on the Arm Cortex-M4.
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
.\" XXX Add reference to Arm documentation.
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
.\" XXX Add reference to file availability from ST website.
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
The absense of any required standard device will cause the kernel to panic.
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
no matter what state the process is currently in.
.\" XXX Add reference D&I 4.3BSD page 72
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
.\" XXX Add reference D&I 4.3BSD pages 77-78
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
.\" XXX Add reference to page 405 in D&I 4.3BSD OS
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
in his famous Commentary on UNIX 6th Edition.
.\" XXX Add reference to page 6-2 in Lions' Commentary
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
.\" XXX Add reference to page 405-406 in D&I 4.3BSD OS
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
.\" XXX Add reference to pages 406-407 in D&I 4.3BSD OS
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
.\" XXX Add reference to kconfig RetrobSD webpage.
.\" XXX http://retrobsd.org/wiki/doku.php/doc/kconfig
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
The specific source files used to build the kernel are retrieved from the file
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
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH
.XN "Build System"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH 2
.XN "Multi-Architecture Features"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH
.XN "Project Difficulties"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH
.XN "Future Work"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.NH
.XN "Conclusion"
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.PP
XXX Words
.bp
.R1
.R2
.TC
