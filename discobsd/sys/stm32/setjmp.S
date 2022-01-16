/*
 * Copyright (c) 2022 Christopher Hettrick <chris@structfoo.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define UBASE 0x20000000	/* User space base address. */

	.syntax	unified
	.thumb

/*
 * setjmp()
 */
	.text
	.align	2
	.thumb_func
	.globl	setjmp
	.type	setjmp, %function
setjmp:
	mov	ip, sp
	stmea	r0!, {r4-r10, fp, ip, lr}	/* v1-v7, FP, IP/SP, LR */
	mov	r0, #0
	bx	lr
	.size	setjmp, . - setjmp

/*
 * longjmp()
 */
	.text
	.align	2
	.thumb_func
	.globl	longjmp
	.type	longjmp, %function
longjmp:
	ldmia	r0!, {r4-r10, fp, ip, lr}	/* v1-v7, FP, IP/SP, LR */
	mov	sp, ip
	mov	r0, #1
	bx	lr
	.size	longjmp, . - longjmp

/*
 * icode() is copied out to process 1 to exec /sbin/init.
 * If the exec fails, process 1 exits.
 */
	.text
	.align	2
	.globl	icode
	.type	icode, %object
icode:
	.thumb_func
	.type	icode1, %function
icode1:
	ldr	r0, =UBASE
	ldr	r1, =UBASE
	adds	r0, sbininit - icode
	adds	r1, argv - icode
	ldr	r7, =11			/* SYS_execv */
	swi	0x0			/* syscall */
	ldr	r7, =1			/* SYS_exit */
	swi	0x0			/* syscall */

forever:				/* Catch runaway code. */
	b	forever

	.align	2
	.type	sbininit, %object
sbininit:
	.ascii	"/sbin/init\0"
	.align	2

	.globl	initflags
initflags:
	.ascii	"-\0\0\0\0\0\0\0\0\0\0"	/* space for options */

	.align	2
	.type	argv, %object
argv:
	.word	sbininit + 6 - icode + UBASE	/* address of "init\0" */
	.word	initflags - icode + UBASE	/* init options */
	.word	0

	.globl	icodeend
	.type	icodeend, %object
icodeend:
	.size	icode, . - icode
