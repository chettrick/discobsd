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
and do not have a memory management unit.
The lack of an MMU rules out any possibility of virtual memory,
which is a critical component in most major operating systems.
A secondary feature of the target hardware is that their processors have the
ability to protect kernel code from user code with a memory protection unit.
This feature was not explored in this project,
but is a viable focus of additional study.
.NH 2
.XN "PIC32 Development Board"
.PP
.\" XXX Insert image of MAX32 development board.
.NH 2
.XN "STM32 Development Board"
.PP
.\" XXX Insert image of STM32F4-Discovery development board.
.NH
.XN "Simulators and Emulators"
.PP
.NH 2
.XN "PIC32 VirtualMIPS Simulator"
.PP
.NH 2
.XN "QEMU-based Arm Cortex-M Emulator"
.PP
.NH
.XN "Host Development Environment"
.PP
.NH 2
.XN "Development Tools on OpenBSD"
.PP
.NH 2
.XN "Development Tools on Linux"
.PP
.NH
.XN "Kernel Operation Overview"
.PP
Coverage of the kernel operation will be limited to the relevant issues
for this project. For example, signals, communication facilities,
and the filesystem will not be covered, but are, nonetheless,
important facilities of any kernel.
System startup, process creation, and process management
will be covered in outline in this section.
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
After a system reset, the kernel gets loaded into RAM from Flash by
initial reset code and execution begins at the kernel's entry point,
which eventually arrives at the kernel's
.CW main()
function.
The kernel's various data structures are initialized.
Machine dependent (MD) peripherals are set up and initialized.
Machine independent (MI) peripherals are set up and initialized.
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
.LP
A trimmed down version of the kernel's linker script is as follows:
.PP
.DS L
.B1
.CW ""
.CW "\tMEMORY {"
.CW "\t    FLASH (r x) : ORIGIN = 0x08000000, LENGTH = 1024K"
.CW "\t    RAM   (rwx) : ORIGIN = 0x20000188, LENGTH = 32k - 0x188"
.CW "\t}"
.CW ""
.CW "\tENTRY(Reset_Handler)"
.CW ""
.CW "\t_estack = 0x20020000;"
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
.NH 2
.XN "Assembly Language Startup"
.PP
.NH 2
.XN "Kernel Initialization"
.PP
.NH 2
.XN "Kernel Configuration"
.PP
.NH 2
.XN "Getting to main()"
.PP
.NH 2
.XN "Getting to /sbin/init"
.PP
.\" XXX Add reference to page 405 in D&I 4.3BSD OS
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
.NH 2
.XN "Getting to the User's Shell"
.PP
.NH
.XN "Userland"
.PP
.NH
.XN "Build System"
.PP
.NH 2
.XN "Multi-Architecture Features"
.PP
.NH
.XN "Project Difficulties"
.PP
.NH
.XN "Future Work"
.PP
.NH
.XN "Conclusion"
.PP
.bp
.R1
.R2
.TC
