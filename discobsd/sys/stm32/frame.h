/*
 * Copyright (c) 2021 Christopher Hettrick <chris@structfoo.com>
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

#ifndef _MACHINE_FRAME_H_
#define _MACHINE_FRAME_H_

struct	trapframe {
	u_int	tf_psr;
	u_int	tf_r0;
	u_int	tf_r1;
	u_int	tf_r2;
	u_int	tf_r3;
	u_int	tf_r4;
	u_int	tf_r5;
	u_int	tf_r6;
	u_int	tf_r7;
	u_int	tf_r8;
	u_int	tf_r9;
	u_int	tf_r10;
	u_int	tf_r11;
	u_int	tf_r12;
	u_int	tf_r13;
	u_int	tf_r14;
	u_int	tf_r15;
};

#define	tf_sp	tf_r13		/* Stack Pointer */
#define	tf_lr	tf_r14		/* Link Register */
#define	tf_pc	tf_r15		/* Program Counter */

#endif /* !_MACHINE_FRAME_H_ */
