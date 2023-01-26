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

#ifndef _MACHINE_FRAME_H_
#define _MACHINE_FRAME_H_

#define	INSN_SZ		2	/* Thumb are 2 bytes; some are 4 bytes. */

#define	PSR_C		(1UL << 29U)	/* Carry bit. */

struct	trapframe {
/* The following 8 registers are pushed on stack by hardware in an SVCall. */
	u_int	tf_r0;		/* Argument / Scratch Register 1 */
	u_int	tf_r1;		/* Argument / Scratch Register 2 */
	u_int	tf_r2;		/* Argument / Scratch Register 3 */
	u_int	tf_r3;		/* Argument / Scratch Register 4 */
	u_int	tf_ip;		/* Stack Pointer (as passed by IP) */
	u_int	tf_lr;		/* Link Register */
	u_int	tf_pc;		/* Program Counter */
	u_int	tf_psr;		/* Program Status Register */
/* The following 8 registers are manually pushed on stack in SVC_Handler. */
	u_int	tf_r4;		/* Variable Register 1 */
	u_int	tf_r5;		/* Variable Register 2 */
	u_int	tf_r6;		/* Variable Register 3 */
	u_int	tf_r7;		/* Variable Register 4 */
	u_int	tf_r8;		/* Variable Register 5 */
	u_int	tf_r9;		/* Variable Register 6 */
	u_int	tf_r10;		/* Variable Register 7 */
	u_int	tf_r11;		/* Variable Register 8 or Frame Pointer */
};

#define	tf_r12	tf_ip		/* Intra-Procedure-Call Scratch Register */
#define	tf_sp	tf_ip		/* Stack Pointer (as passed by IP) */
#define	tf_r14	tf_lr		/* Link Register */
#define	tf_r15	tf_pc		/* Program Counter */

/*
 * Exception stack frame of a SysTick exception.
 * A clockframe encapsulates the machine state when the SysTick
 * exception occured.
 * The clockframe is used by hardclock(), softclock(), and gatherstats().
 */
struct	clockframe {
/* These 8 registers are pushed on stack by hardware in an exception. */
	u_int	cf_r0;		/* Argument / Scratch Register 1 */
	u_int	cf_r1;		/* Argument / Scratch Register 2 */
	u_int	cf_r2;		/* Argument / Scratch Register 3 */
	u_int	cf_r3;		/* Argument / Scratch Register 4 */
	u_int	cf_ip;		/* Stack Pointer (as passed by IP) */
	u_int	cf_lr;		/* Link Register */
	u_int	cf_pc;		/* Program Counter */
	u_int	cf_psr;		/* Program Status Register */
};

#define	cf_sp	cf_ip		/* Stack Pointer (as passed by IP) */

/*
 * Exception stack frame of a fault exception.
 * A faultframe encapsulates the machine state when the fault occured.
 *
 * ARMv6-M fault types: HardFault
 * ARMv7-M fault types: HardFault, MemManage, BusFault, UsageFault
 */
struct	faultframe {
/* These 8 registers are pushed on stack by hardware in a fault exception. */
	u_int	ff_r0;		/* Argument / Scratch Register 1 */
	u_int	ff_r1;		/* Argument / Scratch Register 2 */
	u_int	ff_r2;		/* Argument / Scratch Register 3 */
	u_int	ff_r3;		/* Argument / Scratch Register 4 */
	u_int	ff_ip;		/* Stack Pointer (as passed by IP) */
	u_int	ff_lr;		/* Link Register */
	u_int	ff_pc;		/* Program Counter */
	u_int	ff_psr;		/* Program Status Register */
};

#define	ff_sp	ff_ip		/* Stack Pointer (as passed by IP) */

#endif /* !_MACHINE_FRAME_H_ */
