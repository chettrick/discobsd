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
 * _setjmp(jmp_buf env);
 */
	.text
	.align	2
	.thumb_func
	.globl	_setjmp
	.type	_setjmp, %function
_setjmp:
	movs	r1, #0			/* Not saving sigmask, env[0]=0.. */
	stmia	r0!, {r1}		/*   so _longjmp ignores sigmask. */
	stmia	r0!, {r1}		/* Ignore sigmask, so env[1]=0. */
	mov	ip, sp			/* Save stack pointer (via ip). */
	stmia	r0, {r4-r11,ip,lr}	/* Save regs in env[2-11]. */
	movs	r0, #0			/* Always return a 0. */
	bx	lr
	.size	_setjmp, . - _setjmp


/*
 * void
 * _longjmp(jmp_buf env, int val);
 */
	.text
	.align	2
	.thumb_func
	.globl	_longjmp
	.type	_longjmp, %function
_longjmp:
	adds	r0, r0, #(2 * 4)	/* Skip savesigs and signal mask. */
	ldmia	r0, {r4-r11,ip,lr}	/* Restore regs from env[2-11]. */
	mov	sp, ip			/* Restore stack pointer (via ip). */
	movs	r0, r1			/* Return val.. */
	cmp	r0, #0			/*   but first test if val is 0.. */
	bne	1f
	movs	r0, #1			/*   and return a 1 if val is 0. */
1:
	bx	lr
	.size	_longjmp, . - _longjmp
