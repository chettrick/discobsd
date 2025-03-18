/*
 * Copyright (c) 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#ifndef NSIG
#include <sys/types.h>

#include <machine/signal.h>     /* sigcontext */

#define NSIG        32

#define SIGHUP      1       /* hangup */
#define SIGINT      2       /* interrupt */
#define SIGQUIT     3       /* quit */
#define SIGILL      4       /* illegal instruction (not reset when caught) */
#define SIGTRAP     5       /* trace trap (not reset when caught) */
#define SIGIOT      6       /* IOT instruction */
#define SIGABRT     SIGIOT  /* compatibility */
#define SIGEMT      7       /* EMT instruction */
#define SIGFPE      8       /* floating point exception */
#define SIGKILL     9       /* kill (cannot be caught or ignored) */
#define SIGBUS      10      /* bus error */
#define SIGSEGV     11      /* segmentation violation */
#define SIGSYS      12      /* bad argument to system call */
#define SIGPIPE     13      /* write on a pipe with no one to read it */
#define SIGALRM     14      /* alarm clock */
#define SIGTERM     15      /* software termination signal from kill */
#define SIGURG      16      /* urgent condition on IO channel */
#define SIGSTOP     17      /* sendable stop signal not from tty */
#define SIGTSTP     18      /* stop signal from tty */
#define SIGCONT     19      /* continue a stopped process */
#define SIGCHLD     20      /* to parent on child stop or exit */
#define SIGCLD      SIGCHLD /* compatibility */
#define SIGTTIN     21      /* to readers pgrp upon background tty read */
#define SIGTTOU     22      /* like TTIN for output if (tp->t_local&LTOSTOP) */
#define SIGIO       23      /* input/output possible signal */
#define SIGXCPU     24      /* exceeded CPU time limit */
#define SIGXFSZ     25      /* exceeded file size limit */
#define SIGVTALRM   26      /* virtual time alarm */
#define SIGPROF     27      /* profiling time alarm */
#define SIGWINCH    28      /* window size changes */
#define SIGUSR1     30      /* user defined signal 1 */
#define SIGUSR2     31      /* user defined signal 2 */

typedef void (*sig_t) (int); /* type of signal function */

#define SIG_ERR     (sig_t) -1
#define SIG_DFL     (sig_t) 0
#define SIG_IGN     (sig_t) 1

typedef unsigned long sigset_t;

/*
 * Signal vector "template" used in sigaction call.
 */
struct  sigaction {
    sig_t   sa_handler;         /* signal handler */
    sigset_t sa_mask;           /* signal mask to apply */
    int     sa_flags;           /* see signal options below */
};

#define SA_ONSTACK      0x0001  /* take signal on signal stack */
#define SA_RESTART      0x0002  /* restart system on signal return */
#define SA_DISABLE      0x0004  /* disable taking signals on alternate stack */
#define SA_NOCLDSTOP    0x0008  /* do not generate SIGCHLD on child stop */

/*
 * Flags for sigprocmask:
 */
#define SIG_BLOCK       1       /* block specified signal set */
#define SIG_UNBLOCK     2       /* unblock specified signal set */
#define SIG_SETMASK     3       /* set specified signal set */

/*
 * Structure used in sigaltstack call.
 */
struct  sigaltstack {
    char    *ss_base;           /* signal stack base */
    int     ss_size;            /* signal stack length */
    int     ss_flags;           /* SA_DISABLE and/or SA_ONSTACK */
};
#define MINSIGSTKSZ     128                 /* minimum allowable stack */
#define SIGSTKSZ        (MINSIGSTKSZ + 384) /* recommended stack size */

/*
 * 4.3 compatibility:
 * Signal vector "template" used in sigvec call.
 */
struct  sigvec {
    sig_t   sv_handler;         /* signal handler */
    long    sv_mask;            /* signal mask to apply */
    int     sv_flags;           /* see signal options below */
};
#define SV_ONSTACK      SA_ONSTACK  /* take signal on signal stack */
#define SV_INTERRUPT    SA_RESTART  /* same bit, opposite sense */
#define sv_onstack      sv_flags    /* isn't compatibility wonderful! */

/*
 * 4.3 compatibility:
 * Structure used in sigstack call.
 */
struct  sigstack {
    char    *ss_sp;             /* signal stack pointer */
    int     ss_onstack;         /* current status */
};

/*
 * Macro for converting signal number to a mask suitable for
 * sigblock().
 */
#define sigmask(m)              (1L << ((m)-1))
#define sigaddset(set, signo)   (*(set) |= 1L << ((signo) - 1), 0)
#define sigdelset(set, signo)   (*(set) &= ~(1L << ((signo) - 1)), 0)
#define sigemptyset(set)        (*(set) = (sigset_t)0, (int)0)
#define sigfillset(set)         (*(set) = ~(sigset_t)0, (int)0)
#define sigismember(set, signo) ((*(set) & (1L << ((signo) - 1))) != 0)

#ifdef KERNEL

/* Table of signal properties. */
extern const char sigprop [NSIG + 1];

/*
 * Send an interrupt to process.
 */
void sendsig (sig_t p, int sig, long mask);

#else /* KERNEL */

sig_t   signal (int, sig_t);
int     sigaction (int signum, const struct sigaction *act,
                   struct sigaction *oldact);
int     sigvec (int sig, struct sigvec *vec, struct sigvec *ovec);
int     kill (pid_t pid, int sig);
int     sigpause (int mask);
int     sigblock (int mask);
int     sigsetmask (int mask);
int     sigprocmask (int how, const sigset_t *set, sigset_t *oldset);
int     siginterrupt (int sig, int flag);
int     sigsuspend (const sigset_t *mask);

#endif /* KERNEL */

#endif /* NSIG */
