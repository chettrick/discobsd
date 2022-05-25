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
		/*NOTREACHED*/
	}

#ifdef UCB_METER
	cnt.v_trap++;
	cnt.v_syscall++;
#endif

	/* XXX Enable interrupts. */

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
	if (u.u_procp->p_ssize < USER_DATA_END - sp) {
		/* Expand stack. */
		u.u_procp->p_ssize = USER_DATA_END - sp;
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

		/* Remaining args are from the stack, after the trapframe. */
		if (callp->sy_narg > 4) {
			u_int addr = (u.u_frame->tf_sp + 32) & ~3;
			if (! baduaddr((caddr_t)addr))
				u.u_arg[4] = *(u_int *)addr;
		}
		if (callp->sy_narg > 5) {
			u_int addr = (u.u_frame->tf_sp + 36) & ~3;
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
	/* Process all received signals. */
	for (;;) {
		psig = CURSIG(u.u_procp);
		if (psig <= 0)
			break;
		postsig(psig);
	}
	curpri = setpri(u.u_procp);

	/* Switch to another process. */
	if (runrun) {
		setrq(u.u_procp);
		u.u_ru.ru_nivcsw++;
		swtch();
	}

	/* Update profiling information. */
	if (u.u_prof.pr_scale)
		addupc((caddr_t) u.u_frame->tf_pc,
		    &u.u_prof, (int) (u.u_ru.ru_stime - syst));

	led_control(LED_KERNEL, 0);
}
