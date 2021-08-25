/*
 * Copyright (c) 1987 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#include "SYS.h"

ENTRY(_exit)
#if 0 // XXX Arm ASM
		syscall	SYS_exit
#endif // XXX Arm ASM
