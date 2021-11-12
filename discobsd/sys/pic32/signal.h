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

#ifndef _MACHINE_SIGNAL_H_
#define _MACHINE_SIGNAL_H_

/*
 * Information pushed on stack when a signal is delivered.
 * This is used by the kernel to restore state following
 * execution of the signal handler.  It is also made available
 * to the handler to allow it to properly restore state if
 * a non-standard exit is performed.
 */
struct	sigcontext {
	int	sc_onstack;	/* sigstack state to restore */
	long	sc_mask;	/* signal mask to restore */
	int	sc_r1;		/* r1 to restore */
	int	sc_r2;		/* and other registers */
	int	sc_r3;
	int	sc_r4;
	int	sc_r5;
	int	sc_r6;
	int	sc_r7;
	int	sc_r8;
	int	sc_r9;
	int	sc_r10;
	int	sc_r11;
	int	sc_r12;
	int	sc_r13;
	int	sc_r14;
	int	sc_r15;
	int	sc_r16;
	int	sc_r17;
	int	sc_r18;
	int	sc_r19;
	int	sc_r20;
	int	sc_r21;
	int	sc_r22;
	int	sc_r23;
	int	sc_r24;
	int	sc_r25;
	int	sc_gp;
	int	sc_sp;		/* sp to restore */
	int	sc_fp;
	int	sc_ra;
	int	sc_lo;
	int	sc_hi;
	int	sc_pc;		/* pc to restore */
};

#endif /* !_MACHINE_SIGNAL_H_ */
