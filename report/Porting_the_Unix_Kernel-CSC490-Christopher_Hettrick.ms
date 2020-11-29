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
as the basis for this development.
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
.NH
.XN "Relevant History of BSD"
.PP
RetroBSD is a semi-modernized version of 2.11BSD targeted to the
PIC32MX7 MIPS-based microcontroller.
.[
RetroBSD.org
.]
The early history of RetroBSD has been lost.
It can only be concluded that RetroBSD was started some time in 2011,
or perhaps some time even before that.
The earliest post on the RetroBSD forum was from August 15, 2011.
The project could have started much earlier than the creation of the forum.
.PP
2.11BSD has a long lineage going back to the first release of
2BSD on May 10, 1979.
.[
PUPS
.]
The 2BSD line of software distributions for the DEC\(tm PDP-11
continued on until the most recent release of 2.11BSD in 1991.
.[
2.11BSD
.]
This release was a celebration of the 20\*{th\*} anniversary of the PDP-11.
It is the culmination of the efforts to port 4.3BSD features
\*- which runs on the DEC VAX \*- to the PDP-11.
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
they are RAM-constrained, 32-bit, and do not have a memory management unit.
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
.NH
.XN "System Startup"
.PP
.NH 2
.XN "Linker Script and Bootstrapping"
.PP
.NH 2
.XN "Kernel Initialization"
.PP
.NH 3
.XN "Assembly-language Startup"
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
.NH
.XN "Userland"
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
