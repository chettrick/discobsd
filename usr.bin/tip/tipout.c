/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#include "tip.h"

/*
 * tip
 *
 * lower fork of tip -- handles passive side
 *  reading from the remote host
 */

static  jmp_buf sigbuf;

/*
 * TIPOUT wait state routine --
 *   sent by TIPIN when it wants to posses the remote host
 */
void intIOT(int sig)
{
    write(repdes[1],&ccc,1);
    read(fildes[0], &ccc,1);
    longjmp(sigbuf, 1);
}

/*
 * Scripting command interpreter --
 *  accepts script file name over the pipe and acts accordingly
 */
void intEMT(int sig)
{
    char c, line[256];
    register char *pline = line;
    char reply;

    read(fildes[0], &c, 1);
    while (c != '\n') {
        *pline++ = c;
        read(fildes[0], &c, 1);
    }
    *pline = '\0';
    if (boolean(value(SCRIPT)) && fscript != NULL)
        fclose(fscript);
    if (pline == line) {
        setboolean(value(SCRIPT), FALSE);
        reply = 'y';
    } else {
        if ((fscript = fopen(line, "a")) == NULL)
            reply = 'n';
        else {
            reply = 'y';
            setboolean(value(SCRIPT), TRUE);
        }
    }
    write(repdes[1], &reply, 1);
    longjmp(sigbuf, 1);
}

void intTERM(int sig)
{
    if (boolean(value(SCRIPT)) && fscript != NULL)
        fclose(fscript);
    exit(0);
}

void intSYS(int sig)
{
    setboolean(value(BEAUTIFY), !boolean(value(BEAUTIFY)));
    longjmp(sigbuf, 1);
}

/*
 * ****TIPOUT   TIPOUT****
 */
void tipout()
{
    char buf[BUFSIZ];
    register char *cp;
    register int cnt;
    extern int errno;
    long omask;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGEMT, intEMT);     /* attention from TIPIN */
    signal(SIGTERM, intTERM);   /* time to go signal */
    signal(SIGIOT, intIOT);     /* scripting going on signal */
    signal(SIGHUP, intTERM);    /* for dial-ups */
    signal(SIGSYS, intSYS);     /* beautify toggle */
    (void) setjmp(sigbuf);
    for (omask = 0;; sigsetmask(omask)) {
        cnt = read(FD, buf, BUFSIZ);
        if (cnt <= 0) {
            /* lost carrier */
            if (cnt < 0 && errno == EIO) {
                sigblock(sigmask(SIGTERM));
                intTERM(0);
                /*NOTREACHED*/
            }
            continue;
        }
#define ALLSIGS sigmask(SIGEMT)|sigmask(SIGTERM)|sigmask(SIGIOT)|sigmask(SIGSYS)
        omask = sigblock(ALLSIGS);
        for (cp = buf; cp < buf + cnt; cp++)
            *cp &= 0177;
        write(1, buf, cnt);
        if (boolean(value(SCRIPT)) && fscript != NULL) {
            if (!boolean(value(BEAUTIFY))) {
                fwrite(buf, 1, cnt, fscript);
                continue;
            }
            for (cp = buf; cp < buf + cnt; cp++)
                if ((*cp >= ' ' && *cp <= '~') ||
                    any(*cp, value(EXCEPTIONS)))
                    putc(*cp, fscript);
        }
    }
}
