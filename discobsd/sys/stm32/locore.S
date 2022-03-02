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
#define USIZE 3072		/* From user.h */

	.syntax	unified
	.thumb

/*
 * setjmp(env)
 *	label_t *env;
 *
 * setjmp(env) will save the process' current register context
 * and return a zero.
 */
	.text
	.align	2
	.thumb_func
	.globl	setjmp
	.type	setjmp, %function
setjmp:
	mov	ip, sp
	stmea	r0!, {r4-r10,fp,ip,lr}	/* Save regs in env arg. */
	mov	r0, #0
	bx	lr
	.size	setjmp, . - setjmp

/*
 * longjmp(u, env)
 * resume(u, env)
 *	memaddr	u;
 *	label_t	*env;
 *
 * longjmp(u, env) (and resume) will generate a "return(1)" from the last
 * call to setjmp(env) by mapping in the user structure pointed to by u,
 * restoring the context saved by setjmp in env and returning a one.
 * Note that registers are recovered statically from the env buffer rather
 * than dynamically from the stack ...
 *
 * This longjmp differs from the longjmp found in the standard library and
 * the VAX 4.3 kernel - it's actually closer to the resume routine of the
 * 4.3 kernel and, indeed, even used to be called resume in the 2.9 kernel.
 * We've given it both names to promote some degree of compatibility between
 * the 4.3 and 2.10 C kernel source ...
 */
	.extern u0
	.extern u

	.text
	.align	2
	.thumb_func
	.globl	longjmp
	.type	longjmp, %function
longjmp:
	.globl	resume
	.type	resume, %function
resume:
	cpsid	i			/* Disable interrupts. */

	ldr	r11, =u			/* Pointer to u. area start addr. */
	cmp	a1, r11			/* If u (first arg) is u. area, */
	beq	1f			/*   then no need to remap u. area. */

	mov	r10, a2			/* Save env (second arg). */
	ldr	r1, =u0			/* Pointer to u0. area start addr. */

	ldr	r9, [r11, #0]		/* Pointer to u.u_procp structure. */
	str	r1, [r9, #60]		/* Save &u0 in u.u_procp->p_addr. */

	/* Exchange contents of u. and u0. */
	mov	r0, r11			/* Pointer to u. area start addr. */
	.rept	USIZE / 3 / 4		/* 4 * 3 * 256 = 3072 = USIZE */
	ldmia	r0, {r2-r4}
	ldmia	r1, {r5-r7}
	stmia	r1!, {r2-r4}
	stmia	r0!, {r5-r7}
	.endr

	ldr	r9, [r11, #0]		/* Pointer to u.u_procp structure. */
	str	r11, [r9, #60]		/* Save &u in u.u_procp->p_addr. */

	mov	a2, r10			/* Restore env (second arg). */
1:
	ldmia	a2!, {r4-r10,fp,ip,lr}	/* Restore regs from env arg. */
	mov	sp, ip

	cpsie	i			/* Enable interrupts. */

	mov	a1, #1			/* Transfer back to setjmp() return */
	bx	lr			/*   with a return value of one. */
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
	swi	0x0			/* Make syscall. */
	ldr	r7, =1			/* SYS_exit */
	swi	0x0			/* Make syscall. */

forever:				/* Catch runaway code. */
	b	forever

	.align	2
	.type	sbininit, %object
sbininit:
	.ascii	"/sbin/init\0"
	.align	2

	.globl	initflags
initflags:
	.ascii	"-\0\0\0\0\0\0\0\0\0\0"	/* Space for options. */

	.align	2
	.type	argv, %object
argv:
	.word	sbininit+6-icode+UBASE	/* Address of "init\0". */
	.word	initflags-icode+UBASE	/* Init options. */
	.word	0

	.globl	icodeend
	.type	icodeend, %object
icodeend:
	.size	icode, . - icode
