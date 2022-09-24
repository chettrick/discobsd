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

	.syntax	unified
	.thumb

/*
 * int
 * setjmp(jmp_buf env);
 */
	.text
	.align	2
	.thumb_func
	.globl	setjmp
	.type	setjmp, %function
setjmp:
	push	{lr}			/* Save return address. */
	movs	r1, #1			/* When saving sigmask, env[0]=1.. */
	str	r1, [r0]		/*   so longjmp restores sigmask; */
	adds	r0, r0, #(2 * 4)	/*   skip over sigmask in env[1]. */
	mov	ip, sp			/* Save stack pointer (via ip). */
	stmia	r0, {r4-r11,ip,lr}	/* Save regs in env[2-11]. */

	subs	r2, r0, #(1 * 4)	/* &env[1] signal mask for oset. */
	movs	r1, #0			/* Null for set. Mask is unchanged. */
	movs	r0, #1			/* SIG_BLOCK, but how is irrelevant. */
	bl	sigprocmask		/* Get current sigmask into env[1]. */

	movs	r0, #0			/* Always return a 0. */
	pop	{pc}			/* Return to whence we came. */
	.size	setjmp, . - setjmp


/*
 * void
 * longjmp(jmp_buf env, int val);
 */
	.text
	.align	2
	.thumb_func
	.globl	longjmp
	.type	longjmp, %function
longjmp:
	mov	r3, r0			/* Save env before sigprocmask. */

	movs	r2, #0			/* Null for oset. */
	adds	r1, r0, #(1 * 4)	/* &env[1] signal mask for set. */
	movs	r0, #3			/* SIG_SETMASK for how. */
	bl	sigprocmask		/* Restore sigmask from env[1]. */

	mov	r0, r3			/* Restore env after sigprocmask. */

	adds	r0, r0, #(2 * 4)	/* Skip savesigs and signal mask. */
	ldmia	r0, {r4-r11,ip,lr}	/* Restore regs from env[2-11]. */
	mov	sp, ip			/* Restore stack pointer (via ip). */
	movs	r0, r1			/* Return val.. */
	cmp	r0, #0			/*   but first test if val is 0.. */
	bne	1f
	movs	r0, #1			/*   and return a 1 if val is 0. */
1:
	bx	lr
	.size	longjmp, . - longjmp
