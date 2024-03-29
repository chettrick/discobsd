/*
 * Copyright (c) 2012-2014 ARM Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the company may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

	.syntax	unified
	.thumb

	.text
	.align	2
	.thumb_func
	.globl	strcmp
	.type	strcmp, %function
strcmp:
1:
#ifdef __thumb2__
	/* Tiny version of strcmp in ARM state.  Used only when optimizing
	   for size.  Also supports Thumb-2.  */
	ldrb	r2, [r0], #1
	ldrb	r3, [r1], #1
	cmp	r2, #1
	it	cs
	cmpcs	r2, r3
	beq	1b
#else /* __thumb__ */
	/* This version is only used when we want a very basic Thumb1
	   implementation or for size, otherwise we use the base ARMv4
	   version.  This is also suitable for ARMv6-M.  */
	ldrb	r2, [r0]
	ldrb	r3, [r1]
	cmp	r2, #0
	beq	2f
	adds	r0, r0, #1
	adds	r1, r1, #1
	cmp	r2, r3
	beq	1b
#endif
2:
	subs	r0, r2, r3
	bx	lr
	.size	strcmp, . - strcmp
