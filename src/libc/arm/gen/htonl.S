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
 * uint32_t
 * htonl(uint32_t host32);
 *
 * Host order    -> Network order
 * 0x78.56.34.12 -> 0x12.34.56.78
 */
	.text
	.align	2
	.thumb_func
	.globl	htonl
	.type	htonl, %function
htonl:
	rev	r0, r0		/* Swap byte order in 32-bit word. */
	bx	lr
	.size	htonl, . - htonl


/*
 * uint32_t
 * ntohl(uint32_t net32);
 *
 * Network order -> Host order
 * 0x12.34.56.78 -> 0x78.56.34.12
 */
	.text
	.align	2
	.thumb_func
	.globl	ntohl
	.type	ntohl, %function
ntohl:
	rev	r0, r0		/* Swap byte order in 32-bit word. */
	bx	lr
	.size	ntohl, . - ntohl
