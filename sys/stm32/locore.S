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
#ifdef __thumb2__
	stmea	r0!, {r4-r10,fp,ip,lr}	/* Save regs in env arg. */
#else /* __thumb__ */
	stmea	r0!, {r4-r7}		/* Save v1-v4 in env arg. Incr r0. */
	mov	r4, r8			/* Bring high register v5 to low. */
	mov	r5, r9			/* Bring high register v6 to low. */
	mov	r6, r10			/* Bring high register v7 to low. */
	mov	r7, r11			/* Bring high register v8 to low. */
	stmea	r0!, {r4-r7}		/* Save v5-v8 in env arg. Incr r0. */
	mov	r4, ip			/* Bring high register ip to low. */
	mov	r5, lr			/* Bring high register lr to low. */
	stmea	r0!, {r4-r5}		/* Save ip and lr in env arg. */
#endif
	movs	r0, #0			/* Returns zero. */
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

	mov	r10, a2			/* Save env (second arg). */
	ldr	r1, =u0			/* Pointer to u0. area start addr. */
	ldr	r2, =u			/* Pointer to u. area start addr. */
	cmp	a1, r2			/* If u (first arg) is not u. area, */
	bne	1f			/*   then remap u. area. */
	ldr	r3, =(2f + 1)		/*   else no need to remap u. area. */
	bx	r3			/*   The +1 sets Thumb mode bit. */
	.pool				/* Insert literal pool for labels. */
1:
	ldr	r3, [r2, #0]		/* Pointer to u.u_procp structure. */
	str	r1, [r3, #60]		/* Save &u0 in u.u_procp->p_addr. */

	/* Exchange contents of u. and u0. */
	mov	r0, r2			/* Pointer to u. area start addr. */

	.rept	USIZE / 3 / 4		/* 4 * 3 * 256 = 3072 = USIZE */
#ifdef __thumb2__
	ldmia	r0, {r2-r4}
	ldmia	r1, {r5-r7}
#else /* __thumb__ */
	ldmia	r0!, {r2-r4}
	ldmia	r1!, {r5-r7}
	subs	r0, #(3 * 4)
	subs	r1, #(3 * 4)
#endif
	stmia	r1!, {r2-r4}
	stmia	r0!, {r5-r7}
	.endr

	ldr	r2, =u			/* Pointer to u. area start addr. */

	ldr	r3, [r2, #0]		/* Pointer to u.u_procp structure. */
	str	r2, [r3, #60]		/* Save &u in u.u_procp->p_addr. */
2:
	mov	a2, r10			/* Restore env (second arg). */
#ifdef __thumb2__
	ldmia	a2!, {r4-r10,fp,ip,lr}	/* Restore regs from env arg. */
#else /* __thumb__ */
	adds	a2, #(8 * 4)		/* Index to &env[8]. */
	ldmia	a2!, {r4-r5}		/* Restore ip and lr to low regs. */
	mov	ip, r4			/* Move temp low register to ip. */
	mov	lr, r5			/* Move temp low register to lr. */
	subs	a2, #(6 * 4)		/* Index to &env[4]. */
	ldmia	a2!, {r4-r7}		/* Restore v5-v8 to low regs. */
	mov	r8, r4			/* Move temp low register to v5. */
	mov	r9, r5			/* Move temp low register to v6. */
	mov	r10, r6			/* Move temp low register to v7. */
	mov	r11, r7			/* Move temp low register to v8. */
	subs	a2, #(8 * 4)		/* Index to &env[0]. */
	ldmia	a2!, {r4-r7}		/* Restore v1-v4 to own registers. */
#endif
	mov	sp, ip

	cpsie	i			/* Enable interrupts. */

	movs	a1, #1			/* Transfer back to setjmp() return */
	bx	lr			/*   with a return value of one. */
	.pool				/* Literal pool for local labels. */
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
	movs.n	r0, #(sbininit - icode)	/* Offset for /sbin/init path. */
	movs.n	r1, #(argv - icode)	/* Offset for *argv[] array. */
	ldr	r2, =UBASE		/* User space base address. */
	add	r0, r2			/* execv() *name argument. */
	add	r1, r2			/* execv() *argv[] argument. */
	svc	#11			/* Make SYS_execv syscall. */
	svc	#1			/* Make SYS_exit syscall. */

forever:				/* Catch runaway code. */
	b	forever
	.pool				/* Put literal pool inside icode(). */

	.align	2
	.type	sbininit, %object
sbininit:
	.ascii	"/sbin/init\0"

	.globl	initflags
	.type	initflags, %object
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
