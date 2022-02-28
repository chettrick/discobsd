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

#include "machine/pic32mx.h"

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

/*
 * Read C0 coprocessor register.
 */
#define mips_read_c0_register(reg,sel) \
(0)
/* XXX
    ({  int __value; \
        asm volatile ( \
        "mfc0   %0, $%1, %2" \
        : "=r" (__value) : "K" (reg), "K" (sel)); \
        __value; \
    })
XXX */

/*
 * Write C0 coprocessor register.
 */
#define mips_write_c0_register(reg, sel, value) \
(0)
/* XXX
    do { \
        asm volatile ( \
        "mtc0   %z0, $%1, %2 \n ehb" \
        : : "r" ((unsigned int) (value)), "K" (reg), "K" (sel)); \
    } while (0)
XXX */

/*
 * Disable the hardware interrupts,
 * saving the interrupt state into the supplied variable.
 */
static int inline __attribute__ ((always_inline))
mips_intr_disable ()
{
    int status;
// XXX    asm volatile ("di   %0" : "=r" (status));
    return status;
}

/*
 * Restore the hardware interrupt mode using the saved interrupt state.
 */
static void inline __attribute__ ((always_inline))
mips_intr_restore (int x)
{
    /* C0_STATUS */
    mips_write_c0_register (12, 0, x);
}

/*
 * Enable hardware interrupts.
 */
static int inline __attribute__ ((always_inline))
mips_intr_enable ()
{
    int status;
// XXX    asm volatile ("ei   %0" : "=r" (status));
    return status;
}

#endif /* __ASSEMBLER__ */
