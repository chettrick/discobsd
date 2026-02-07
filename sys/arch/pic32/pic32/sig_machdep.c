/*
 * Copyright (c) 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)machdep.c	2.4 (2.11BSD) 1999/9/13
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signalvar.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/map.h>
#include <sys/syslog.h>
#include <machine/frame.h>

#if 0
#define DIAGNOSTIC
#endif

/*
 * Send an interrupt to process.
 *
 * Stack is set up to allow trampoline code stored at u.u_sigtramp (as
 * specified by the user process) to call the user's real signal catch
 * routine, followed by sys sigreturn to the sigreturn routine below
 * (see lib/libc/mips/sys/sigaction.S).  After sigreturn resets the signal
 * mask, the stack, and the frame pointer, it returns to the user specified
 * pc and regs.
 */
void
sendsig (p, sig, mask)
    sig_t p;
    int sig;
    long mask;
{
    struct sigframe {
        int     sf_space [4];
        struct  sigcontext sf_sc;
    };
    struct trapframe *regs = u.u_frame;
    register struct sigframe *sfp;
    int oonstack;

#ifdef DIAGNOSTIC
    printf("(%u)sendsig %d, mask=%#x, handler=%#x, tramp=%#x\n",
        u.u_procp->p_pid, sig, mask, p, u.u_sigtramp);
#endif
    oonstack = u.u_sigstk.ss_flags & SA_ONSTACK;

    /*
     * Allocate and validate space for the signal frame.
     */
    if ((u.u_psflags & SAS_ALTSTACK) &&
         ! (u.u_sigstk.ss_flags & SA_ONSTACK) &&
        (u.u_sigonstack & sigmask(sig))) {
        sfp = (struct sigframe*) (u.u_sigstk.ss_base +
            u.u_sigstk.ss_size);
        u.u_sigstk.ss_flags |= SA_ONSTACK;
    } else
        sfp = (struct sigframe*) regs->tf_sp;

    sfp--;
    if (! (u.u_sigstk.ss_flags & SA_ONSTACK)) {
        if ((caddr_t) sfp < (caddr_t) u.u_procp->p_daddr + u.u_dsize) {
            /*
             * Process has trashed its stack; give it an illegal
             * instruction violation to halt it in its tracks.
             */
            fatalsig(SIGILL);
            return;
        }
        if (u.u_procp->p_ssize < USER_DATA_END - (unsigned) sfp) {
            u.u_procp->p_ssize = USER_DATA_END - (unsigned) sfp;
            u.u_procp->p_saddr = (unsigned) sfp;
            u.u_ssize = u.u_procp->p_ssize;
        }
    }

    /*
     * Build the signal context to be used by sigreturn.
     */
    sfp->sf_sc.sc_onstack = oonstack;
    sfp->sf_sc.sc_mask = mask;
    sfp->sf_sc.sc_r1  = regs->tf_r1;
    sfp->sf_sc.sc_r2  = regs->tf_r2;
    sfp->sf_sc.sc_r3  = regs->tf_r3;
    sfp->sf_sc.sc_r4  = regs->tf_r4;
    sfp->sf_sc.sc_r5  = regs->tf_r5;
    sfp->sf_sc.sc_r6  = regs->tf_r6;
    sfp->sf_sc.sc_r7  = regs->tf_r7;
    sfp->sf_sc.sc_r8  = regs->tf_r8;
    sfp->sf_sc.sc_r9  = regs->tf_r9;
    sfp->sf_sc.sc_r10 = regs->tf_r10;
    sfp->sf_sc.sc_r11 = regs->tf_r11;
    sfp->sf_sc.sc_r12 = regs->tf_r12;
    sfp->sf_sc.sc_r13 = regs->tf_r13;
    sfp->sf_sc.sc_r14 = regs->tf_r14;
    sfp->sf_sc.sc_r15 = regs->tf_r15;
    sfp->sf_sc.sc_r16 = regs->tf_r16;
    sfp->sf_sc.sc_r17 = regs->tf_r17;
    sfp->sf_sc.sc_r18 = regs->tf_r18;
    sfp->sf_sc.sc_r19 = regs->tf_r19;
    sfp->sf_sc.sc_r20 = regs->tf_r20;
    sfp->sf_sc.sc_r21 = regs->tf_r21;
    sfp->sf_sc.sc_r22 = regs->tf_r22;
    sfp->sf_sc.sc_r23 = regs->tf_r23;
    sfp->sf_sc.sc_r24 = regs->tf_r24;
    sfp->sf_sc.sc_r25 = regs->tf_r25;
    sfp->sf_sc.sc_gp  = regs->tf_gp;
    sfp->sf_sc.sc_sp  = regs->tf_sp;
    sfp->sf_sc.sc_fp  = regs->tf_fp;
    sfp->sf_sc.sc_ra  = regs->tf_ra;
    sfp->sf_sc.sc_lo  = regs->tf_lo;
    sfp->sf_sc.sc_hi  = regs->tf_hi;
    sfp->sf_sc.sc_pc  = regs->tf_pc;

    /* Call signal handler */
    regs->tf_r4 = sig;                      /* $a0 - signal number */
    regs->tf_r5 = u.u_code;                 /* $a1 - code */
    regs->tf_r6 = (int) &sfp->sf_sc;        /* $a2 - address of sigcontext */
    regs->tf_ra = (int) u.u_sigtramp;       /* $ra - sigtramp */
    regs->tf_sp = (int) sfp;
    regs->tf_pc = (int) p;
#ifdef DIAGNOSTIC
    printf("    ...call handler %p (sig=%d, code=%#x, context=%p)\n",
        p, sig, u.u_code, &sfp->sf_sc);
    printf("    ...stack=%p, return to %p\n", sfp, u.u_sigtramp);
#endif
}

/*
 * System call to cleanup state after a signal
 * has been taken.  Reset signal mask and
 * stack state from context left by sendsig (above).
 * Return to previous pc and ps as specified by
 * context left by sendsig. Check carefully to
 * make sure that the user has not modified the
 * ps to gain improper priviledges or to cause
 * a machine fault.
 */
void
sigreturn()
{
    struct trapframe *regs = u.u_frame;
    register struct sigcontext *scp =
        (struct sigcontext*) (regs->tf_sp + 16);

#ifdef DIAGNOSTIC
    printf("(%u)sigreturn stack=%#x, context=%p\n",
        u.u_procp->p_pid, regs->tf_sp, scp);
#endif
    if (baduaddr ((caddr_t) scp) ||
        baduaddr ((caddr_t) scp + sizeof(*scp))) {
        u.u_error = EFAULT;
        return;
    }
    u.u_error = EJUSTRETURN;
    if (scp->sc_onstack & SA_ONSTACK)
        u.u_sigstk.ss_flags |= SA_ONSTACK;
    else
        u.u_sigstk.ss_flags &= ~SA_ONSTACK;
    u.u_procp->p_sigmask = scp->sc_mask & ~sigcantmask;

    /* Return from signal handler */
    regs->tf_r1 = scp->sc_r1;
    regs->tf_r2 = scp->sc_r2;
    regs->tf_r3 = scp->sc_r3;
    regs->tf_r4 = scp->sc_r4;
    regs->tf_r5 = scp->sc_r5;
    regs->tf_r6 = scp->sc_r6;
    regs->tf_r7 = scp->sc_r7;
    regs->tf_r8 = scp->sc_r8;
    regs->tf_r9 = scp->sc_r9;
    regs->tf_r10 = scp->sc_r10;
    regs->tf_r11 = scp->sc_r11;
    regs->tf_r12 = scp->sc_r12;
    regs->tf_r13 = scp->sc_r13;
    regs->tf_r14 = scp->sc_r14;
    regs->tf_r15 = scp->sc_r15;
    regs->tf_r16 = scp->sc_r16;
    regs->tf_r17 = scp->sc_r17;
    regs->tf_r18 = scp->sc_r18;
    regs->tf_r19 = scp->sc_r19;
    regs->tf_r20 = scp->sc_r20;
    regs->tf_r21 = scp->sc_r21;
    regs->tf_r22 = scp->sc_r22;
    regs->tf_r23 = scp->sc_r23;
    regs->tf_r24 = scp->sc_r24;
    regs->tf_r25 = scp->sc_r25;
    regs->tf_gp = scp->sc_gp;
    regs->tf_sp = scp->sc_sp;
    regs->tf_fp = scp->sc_fp;
    regs->tf_ra = scp->sc_ra;
    regs->tf_lo = scp->sc_lo;
    regs->tf_hi = scp->sc_hi;
    regs->tf_pc = scp->sc_pc;
#ifdef DIAGNOSTIC
    printf("    ...to %#x, stack %#x\n", regs->tf_pc, regs->tf_sp);
#endif
}
