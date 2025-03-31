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

#include <sys/param.h>
#include <sys/signalvar.h>
#include <sys/systm.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/vm.h>

#include <machine/frame.h>

/*
 * SVC_Handler(frame)
 *	struct trapframe *frame;
 *
 * Exception handler entry point for system calls (via 'svc' instruction).
 * The real work is done in PendSV_Handler at the lowest exception priority.
 */
void
SVC_Handler(void)
{
	/* Set a PendSV exception to immediately tail-chain into. */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

	__DSB();
	__ISB();

	/* PendSV has lowest priority, so need to allow it to fire. */
	(void)spl0();
}

/*
 * PendSV_Handler(frame)
 *	struct trapframe *frame;
 *
 * System call handler (via SVC_Handler pending a PendSV exception).
 * Save the processor state in a trap frame and pass it to syscall().
 * Restore processor state from returned trap frame on return from syscall().
 */
void
PendSV_Handler(void)
{
__asm volatile (
"	.syntax	unified		\n\t"
"	.thumb			\n\t"

"	cpsid	i		\n\t"	/* Disable interrupts. */

#ifdef __thumb2__
	/*
	 * ARMv7-M hardware already pushed r0-r3, ip, lr, pc, psr on PSP,
	 * and then switched to MSP and is currently in Handler Mode.
	 */
"	push	{r4-r11}	\n\t"	/* Push v1-v8 registers onto MSP. */
"	mrs	r1, PSP		\n\t"	/* Get pointer to trap frame. */
"	ldmfd	r1, {r2-r9}	\n\t"	/* Copy trap frame from PSP. */
"	mov	r6, r1		\n\t"	/* Set trap frame sp as PSP. */
"	push	{r2-r9}		\n\t"	/* Push that trap frame onto MSP. */

"	mrs	r0, MSP		\n\t"	/* MSP trap frame is syscall() arg. */
"	bl	syscall		\n\t"	/* Call syscall() with MSP as arg. */

"	pop	{r2-r9}		\n\t"	/* Pop off trap frame from MSP. */
"	mov	r1, r6		\n\t"	/* PSP will be trap frame sp. */
"	stmia	r1, {r2-r9}	\n\t"	/* Hardware pops off PSP on return. */
"	msr	PSP, r1		\n\t"	/* Set PSP as trap frame sp. */
"	pop	{r4-r11}	\n\t"	/* Pop from MSP into v1-v8 regs. */

	/*
	 * On return, ARMv7-M hardware sets PSP as stack pointer,
	 * pops from PSP to registers r0-r3, ip, lr, pc, psr,
	 * and then switches back to Thread Mode (exception completed).
	 */
"	mov	lr, #0xFFFFFFFD	\n\t"	/* EXC_RETURN Thread Mode, PSP */
					/* Return to Thread Mode. */
#else /* __thumb__ */
	/*
	 * ARMv6-M hardware already pushed r0-r3, ip, lr, pc, psr on PSP,
	 * and then switched to MSP and is currently in Handler Mode.
	 */
"	mov	r0, r8		\n\t"	/* Bring high register v5 to low. */
"	mov	r1, r9		\n\t"	/* Bring high register v6 to low. */
"	mov	r2, r10		\n\t"	/* Bring high register v7 to low. */
"	mov	r3, r11		\n\t"	/* Bring high register v8 to low. */
"	push	{r0-r3}		\n\t"	/* Push v5-v8 registers onto MSP. */
"	push	{r4-r7}		\n\t"	/* Push v1-v4 registers onto MSP. */

"	mrs	r1, PSP		\n\t"	/* Get pointer to trap frame. */
"	mov	r2, r1		\n\t"	/* Pointer to use for top half. */
"	adds	r2, #(4 * 4)	\n\t"	/* Index to top half of trap frame. */
"	ldmfd	r2!, {r4-r7}	\n\t"	/* Copy frame top half from PSP. */
"	mov	r4, r1		\n\t"	/* Set trap frame sp as PSP. */
"	push	{r4-r7}		\n\t"	/* Push frame top half onto MSP. */
"	ldmfd	r1!, {r4-r7}	\n\t"	/* Copy frame low half from PSP. */
"	push	{r4-r7}		\n\t"	/* Push frame low half onto MSP. */

"	mrs	r0, MSP		\n\t"	/* MSP trap frame is syscall() arg. */
"	bl	syscall		\n\t"	/* Call syscall() with MSP as arg. */

"	pop	{r0-r7}		\n\t"	/* Pop off trap frame from MSP. */
"	msr	PSP, r4		\n\t"	/* Set PSP as trap frame sp. */
"	stmia	r4!, {r0-r3}	\n\t"	/* Copy trap frame low half to PSP. */
"	mrs	r1, PSP		\n\t"	/* Get PSP again as trap frame sp. */
"	stmia	r4!, {r1,r5-r7}	\n\t"	/* Copy trap frame top half to PSP. */

"	pop	{r4-r7}		\n\t"	/* Pop from MSP into v1-v4 regs. */
"	pop	{r0-r3}		\n\t"	/* Pop from MSP for v5-v8 regs. */
"	mov	r11, r3		\n\t"	/* Move low register to high v8. */
"	mov	r10, r2		\n\t"	/* Move low register to high v7. */
"	mov	r9, r1		\n\t"	/* Move low register to high v6. */
"	mov	r8, r0		\n\t"	/* Move low register to high v5. */

	/*
	 * On return, ARMv6-M hardware sets PSP as stack pointer,
	 * pops from PSP to registers r0-r3, ip, lr, pc, psr,
	 * and then switches back to Thread Mode (exception completed).
	 */
"	ldr	r1, =0xFFFFFFFD	\n\t"	/* EXC_RETURN Thread Mode, PSP */
"	mov	lr, r1		\n\t"	/* Return to Thread Mode. */
#endif
);
}

void
syscall(struct trapframe *frame)
{
	register int psig;
	time_t syst;
	int code;
	u_int sp;

	syst = u.u_ru.ru_stime;

	if ((unsigned) frame < (unsigned) &u + sizeof(u)) {
		panic("stack overflow");
		/* NOTREACHED */
	}

#ifdef UCB_METER
	cnt.v_trap++;
	cnt.v_syscall++;
#endif

	/* Enable interrupts. */
	(void)arm_intr_enable();

	u.u_error = 0;
	u.u_frame = frame;
	u.u_code = u.u_frame->tf_pc - INSN_SZ;	/* Syscall for sig handler. */

	led_control(LED_KERNEL, 1);

	/* Check stack. */
	sp = u.u_frame->tf_sp;
	if (sp < u.u_procp->p_daddr + u.u_dsize) {
		/* Process has trashed its stack; give it an illegal
		 * instruction violation to halt it in its tracks. */
		psig = SIGSEGV;
		goto bad;
	}
	if (u.u_procp->p_ssize < (size_t)__user_data_end - sp) {
		/* Expand stack. */
		u.u_procp->p_ssize = (size_t)__user_data_end - sp;
		u.u_procp->p_saddr = sp;
		u.u_ssize = u.u_procp->p_ssize;
	}

	code = *(int *)u.u_code & 0377;		/* Bottom 8 bits are index. */

	const struct sysent *callp = &sysent[0];

	if (code < nsysent)
		callp += code;

	if (callp->sy_narg) {
		/* In AAPCS, first four args are from trapframe regs r0-r3. */
		u.u_arg[0] = u.u_frame->tf_r0;	/* $a1 */
		u.u_arg[1] = u.u_frame->tf_r1;	/* $a2 */
		u.u_arg[2] = u.u_frame->tf_r2;	/* $a3 */
		u.u_arg[3] = u.u_frame->tf_r3;	/* $a4 */

		/* In AAPCS, stack must be double-word aligned. */
		int stkalign = 0;
		if (u.u_frame->tf_psr & SCB_CCR_STKALIGN_Msk) {
			stkalign = 4;		/* Skip over padding byte. */
		}

		/* Remaining args are from the stack, after the trapframe. */
		if (callp->sy_narg > 4) {
			u_int addr = (u.u_frame->tf_sp + 32 + stkalign) & ~3;
			if (! baduaddr((caddr_t)addr))
				u.u_arg[4] = *(u_int *)addr;
		}
		if (callp->sy_narg > 5) {
			u_int addr = (u.u_frame->tf_sp + 36 + stkalign) & ~3;
			if (! baduaddr((caddr_t)addr))
				u.u_arg[5] = *(u_int *)addr;
		}
	}

	u.u_rval = 0;

	if (setjmp(&u.u_qsave) == 0) {
		(*callp->sy_call)();		/* Make syscall. */
	}

	switch (u.u_error) {
	case 0:
		u.u_frame->tf_psr &= ~PSR_C;	/* Clear carry bit. */
		u.u_frame->tf_r0 = u.u_rval;	/* $a1 - result. */
		break;
	case ERESTART:
		u.u_frame->tf_pc -= INSN_SZ;	/* Return to svc syscall. */
		break;
	case EJUSTRETURN:			/* Return from sig handler. */
		break;
	default:
		u.u_frame->tf_psr |= PSR_C;	/* Set carry bit. */
		u.u_frame->tf_r0 = u.u_error;	/* $a1 - result. */
		break;
	}
	goto out;

bad:
	/* From this point and further the interrupts must be enabled. */
	psignal(u.u_procp, psig);

out:
	userret(u.u_frame->tf_pc, syst);

	led_control(LED_KERNEL, 0);
}
