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

#include "SYS.h"

ENTRY(pipe)
	mov	r3, r0			/* Copy &filedes[0] to r3. */
	svc	#SYS_pipe
	bcs	1f			/* Error if carry set. */
	str	r0, [r3, #0]		/* Save read fd in &filedes[0]. */
	str	r1, [r3, #4]		/* Save write fd in &filedes[1]. */
	mov	r0, #0
	bx	lr			/* Success return. */
1:	ldr	r1, =errno
	str	r0, [r1]
	mov	r0, #0
	mvn	r0, r0
	bx	lr			/* Error return. */
END(pipe)
