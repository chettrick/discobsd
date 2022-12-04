/*
 * Copyright (c) 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
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

//#define DIAGNOSTIC

/*
 * Send an interrupt to process.
 *
 * Stack is set up to allow trampoline code stored at u.u_sigtramp (as
 * specified by the user process) to call the user's real signal catch
 * routine, followed by sys sigreturn to the sigreturn routine below
 * (see lib/libc/arm/sys/sigaction.S).  After sigreturn resets the signal
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
        if (u.u_procp->p_ssize < (size_t)__user_data_end - (unsigned) sfp) {
            u.u_procp->p_ssize = (size_t)__user_data_end - (unsigned) sfp;
            u.u_procp->p_saddr = (unsigned) sfp;
            u.u_ssize = u.u_procp->p_ssize;
        }
    }

    /*
     * Build the signal context to be used by sigreturn.
     */
    sfp->sf_sc.sc_onstack = oonstack;
    sfp->sf_sc.sc_mask = mask;
    sfp->sf_sc.sc_r0  = regs->tf_r0;
    sfp->sf_sc.sc_r1  = regs->tf_r1;
    sfp->sf_sc.sc_r2  = regs->tf_r2;
    sfp->sf_sc.sc_r3  = regs->tf_r3;
    sfp->sf_sc.sc_sp  = regs->tf_sp;
    sfp->sf_sc.sc_lr  = regs->tf_lr;
    sfp->sf_sc.sc_pc  = regs->tf_pc;
    sfp->sf_sc.sc_psr = regs->tf_psr;
    sfp->sf_sc.sc_r4  = regs->tf_r4;
    sfp->sf_sc.sc_r5  = regs->tf_r5;
    sfp->sf_sc.sc_r6  = regs->tf_r6;
    sfp->sf_sc.sc_r7  = regs->tf_r7;
    sfp->sf_sc.sc_r8  = regs->tf_r8;
    sfp->sf_sc.sc_r9  = regs->tf_r9;
    sfp->sf_sc.sc_r10 = regs->tf_r10;
    sfp->sf_sc.sc_r11 = regs->tf_r11;

    /* Call signal handler */
    regs->tf_r0 = sig;                  /* $a1 - signal number */
    regs->tf_r1 = u.u_code;             /* $a2 - code */
    regs->tf_r2 = (int) &sfp->sf_sc;    /* $a3 - address of sigcontext */
    regs->tf_lr = (int) u.u_sigtramp;   /* $lr - sigtramp */
    regs->tf_sp = (int) sfp;            /* $sp - stack */
    regs->tf_pc = (int) p;              /* $pc - handler */
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
    regs->tf_r0  = scp->sc_r0;
    regs->tf_r1  = scp->sc_r1;
    regs->tf_r2  = scp->sc_r2;
    regs->tf_r3  = scp->sc_r3;
    regs->tf_sp  = scp->sc_sp;
    regs->tf_lr  = scp->sc_lr;
    regs->tf_pc  = scp->sc_pc;
    regs->tf_psr = scp->sc_psr;
    regs->tf_r4  = scp->sc_r4;
    regs->tf_r5  = scp->sc_r5;
    regs->tf_r6  = scp->sc_r6;
    regs->tf_r7  = scp->sc_r7;
    regs->tf_r8  = scp->sc_r8;
    regs->tf_r9  = scp->sc_r9;
    regs->tf_r10 = scp->sc_r10;
    regs->tf_r11 = scp->sc_r11;
#ifdef DIAGNOSTIC
    printf("    ...to %#x, stack %#x\n", regs->tf_pc, regs->tf_sp);
#endif
}
