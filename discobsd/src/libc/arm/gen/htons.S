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
 * uint16_t
 * htons(uint16_t host16);
 *
 * Host order    -> Half-word rev -> Network order
 * 0x78.56.34.12 -> 0x56.78.12.34 -> 0x00.00.12.34
 *
 * Note: host16 parameter is a half-word, but registers are 32-bit words.
 */
	.text
	.align	2
	.thumb_func
	.globl	htons
	.type	htons, %function
htons:
	rev16	r0, r0		/* Swap bytes in each half-word. */
	uxth	r0, r0		/* Unsigned extend half-word to word. */
	bx	lr
	.size	htons, . - htons


/*
 * uint16_t
 * ntohs(uint16_t net16);
 *
 * Network order -> Half-word rev -> Host order
 * 0x12.34.56.78 -> 0x34.12.78.56 -> 0x00.00.78.56
 *
 * Note: net16 parameter is a half-word, but registers are 32-bit words.
 */
	.text
	.align	2
	.thumb_func
	.globl	ntohs
	.type	ntohl, %function
ntohs:
	rev16	r0, r0		/* Swap bytes in each half-word. */
	uxth	r0, r0		/* Unsigned extend half-word to word. */
	bx	lr
	.size	ntohs, . - ntohs
