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
 * sigsetjmp(sigjmp_buf env, int savesigs);
 */
	.text
	.align	2
	.thumb_func
	.globl	sigsetjmp
	.type	sigsetjmp, %function
sigsetjmp:
	cmp	r1, #0			/* If savesigs is zero.. */
	bne	1f
	ldr	r3, =_setjmp		/*   skip signal mask handling; */
	bx	r3
1:
	ldr	r3, =setjmp		/*   else handle signal mask. */
	bx	r3
	.size	sigsetjmp, . - sigsetjmp


/*
 * void
 * siglongjmp(sigjmp_buf env, int val);
 */
	.text
	.align	2
	.thumb_func
	.globl	siglongjmp
	.type	siglongjmp, %function
siglongjmp:
	ldr	r2, [r0]		/* Get savesigs. */
	cmp	r2, #0			/* If savesigs was zero.. */
	bne	1f
	ldr	r3, =_longjmp		/*   skip signal mask handling; */
	bx	r3
1:
	ldr	r3, =longjmp		/*   else handle signal mask. */
	bx	r3
	.size	siglongjmp, . - siglongjmp
