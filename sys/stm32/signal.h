/*
 * Copyright (c) 2021, 2022 Christopher Hettrick <chris@structfoo.com>
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
	u_int	sc_r0;
	u_int	sc_r1;
	u_int	sc_r2;
	u_int	sc_r3;
	u_int	sc_ip;
	u_int	sc_lr;
	u_int	sc_pc;
	u_int	sc_psr;
	u_int	sc_r4;
	u_int	sc_r5;
	u_int	sc_r6;
	u_int	sc_r7;
	u_int	sc_r8;
	u_int	sc_r9;
	u_int	sc_r10;
	u_int	sc_r11;
};

#define	sc_r12	sc_ip		/* Intra-Procedure-Call Scratch Register */
#define	sc_sp	sc_ip		/* Stack Pointer (as passed by IP) */
#define	sc_r14	sc_lr		/* Link Register */
#define	sc_r15	sc_pc		/* Program Counter */

#endif /* !_MACHINE_SIGNAL_H_ */
