/*
 * Hardware register defines for MIPS32 architecture.
 *
 * Copyright (C) 2008-2010 Serge Vakulenko, <serge@vak.ru>
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice appear in all
 * copies and that both that the copyright notice and this
 * permission notice and warranty disclaimer appear in supporting
 * documentation, and that the name of the author not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * The author disclaim all warranties with regard to this
 * software, including all implied warranties of merchantability
 * and fitness.  In no event shall the author be liable for any
 * special, indirect or consequential damages or any damages
 * whatsoever resulting from loss of use, data or profits, whether
 * in an action of contract, negligence or other tortious action,
 * arising out of or in connection with the use or performance of
 * this software.
 */

#ifndef __ASSEMBLER__

#ifndef KERNEL
/*
 * 2BSD system call extensions: use with care.
 */

/*
 * Read peripheral register.
 */
unsigned ufetch (unsigned addr);

/*
 * Write peripheral register.
 */
unsigned ustore (unsigned addr, unsigned value);

/*
 * Call a kernel function.
 */
unsigned ucall (int priority, void *address, int arg1, int arg2);

#endif /* KERNEL */

#endif /* __ASSEMBLER__ */
