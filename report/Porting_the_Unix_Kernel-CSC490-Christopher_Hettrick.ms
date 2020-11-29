.RP
.TL
Porting the Unix Kernel
.AU
Christopher K. Hettrick
.AI
University of Victoria
Department of Computer Science
CSC490
Supervised by Bill Bird
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
.AE
.NH
.XN "Introduction"
.PP
Porting the MIPS32\(rg M4K\(rg architecture to the
Arm\(rg Cortex\(rg-M4 architecture.
.NH
.XN "History of RetroBSD"
.PP
.NH
.XN "Hardware"
.PP
.NH 2
.XN "PIC32 Development Board"
.PP
.NH 2
.XN "STM32 Development Board"
.PP
.NH
.XN "Host Development Environment"
.PP
.NH 2
.XN "Development Tools on OpenBSD"
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
.TC
