/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *  @(#)tip.h   5.3.1 (2.11BSD GTE) 1/1/94
 */

/*
 * tip - terminal interface program
 */

#include <sys/types.h>
#include <sys/file.h>

#include <sgtty.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <ctype.h>
#include <setjmp.h>
#include <errno.h>

#ifdef CTRL
#undef CTRL
#endif
#define CTRL(c) ('c' & 037)

/*
 * Remote host attributes
 */
char    *DV;            /* UNIX device(s) to open */
char    *EL;            /* chars marking an EOL */
char    *CM;            /* initial connection message */
char    *IE;            /* EOT to expect on input */
char    *OE;            /* EOT to send to complete FT */
char    *CU;            /* call unit if making a phone call */
char    *AT;            /* acu type */
char    *PN;            /* phone number(s) */
char    *DI;            /* disconnect string */
char    *PA;            /* parity to be generated */

char    *PH;            /* phone number file */
char    *RM;            /* remote file name */
char    *HO;            /* host name */

int BR;                 /* line speed for conversation */
int FS;                 /* frame size for transfers */

char    DU;             /* this host is dialed up */
char    HW;             /* this device is hardwired, see hunt.c */
char    *ES;            /* escape character */
char    *EX;            /* exceptions */
char    *FO;            /* force (literal next) char*/
char    *RC;            /* raise character */
char    *RE;            /* script record file */
char    *PR;            /* remote prompt */
int DL;                 /* line delay for file transfers to remote */
int CL;                 /* char delay for file transfers to remote */
int ET;                 /* echocheck timeout */
char    HD;             /* this host is half duplex - do local echo */

/*
 * String value table
 */
typedef struct {
    char    *v_name;    /* whose name is it */
    char    v_type;     /* for interpreting set's */
    char    v_access;   /* protection of touchy ones */
    char    *v_abrev;   /* possible abreviation */
    char    *v_value;   /* casted to a union later */
} value_t;

#define STRING  01      /* string valued */
#define BOOL    02      /* true-false value */
#define NUMBER  04      /* numeric value */
#define CHAR    010     /* character value */

#define WRITE   01      /* write access to variable */
#define READ    02      /* read access */

#define CHANGED 01      /* low bit is used to show modification */
#define PUBLIC  1       /* public access rights */
#define PRIVATE 03      /* private to definer */
#define ROOT    05      /* root defined */

#define TRUE    1
#define FALSE   0

#define ENVIRON 020     /* initialize out of the environment */
#define IREMOTE 040     /* initialize out of remote structure */
#define INIT    0100    /* static data space used for initialization */
#define TMASK   017

/*
 * Definition of ACU line description
 */
typedef struct {
    char    *acu_name;
    int (*acu_dialer)();
    int (*acu_disconnect)();
    int (*acu_abort)();
} acu_t;

#define equal(a, b) (strcmp(a,b)==0)/* A nice function to string compare */

#define value(v)        vtable[v].v_value

#define boolean(v)      ((short)(long)(v))
#define number(v)       ((long)(v))
#define character(v)    ((char)(long)(v))
#define address(v)      ((long *)(v))

#define setboolean(v, n)    do { (v) = (char *)(long)(n); } while (0)
#define setnumber(v, n)     do { (v) = (char *)(long)(n); } while (0)
#define setcharacter(v, n)  do { (v) = (char *)(long)(n); } while (0)
#define setaddress(v, n)    do { (v) = (char *)(n); } while (0)

/*
 * Escape command table definitions --
 *   lookup in this table is performed when ``escapec'' is recognized
 *   at the begining of a line (as defined by the eolmarks variable).
*/

typedef struct {
    char    e_char;         /* char to match on */
    char    e_flags;        /* experimental, priviledged */
    char    *e_help;        /* help string */
    int     (*e_func)();    /* command */
} esctable_t;

#define NORM    00          /* normal protection, execute anyone */
#define EXP     01          /* experimental, mark it with a `*' on help */
#define PRIV    02          /* priviledged, root execute only */

extern int      vflag;      /* verbose during reading of .tiprc file */
extern value_t  vtable[];   /* variable table */

/*
 * Definition of indices into variable table so
 *  value(DEFINE) turns into a static address.
 */

#define BEAUTIFY    0
#define BAUDRATE    1
#define DIALTIMEOUT 2
#define EOFREAD     3
#define EOFWRITE    4
#define EOL         5
#define ESCAPE      6
#define EXCEPTIONS  7
#define FORCE       8
#define FRAMESIZE   9
#define HOST        10
#define LOG         11
#define PHONES      12
#define PROMPT      13
#define RAISE       14
#define RAISECHAR   15
#define RECORD      16
#define REMOTE      17
#define SCRIPT      18
#define TABEXPAND   19
#define VERBOSE     20
#define SHELL       21
#define HOME        22
#define ECHOCHECK   23
#define DISCONNECT  24
#define TAND        25
#define LDELAY      26
#define CDELAY      27
#define ETIMEOUT    28
#define RAWFTP      29
#define HALFDUPLEX  30
#define LECHO       31
#define PARITY      32

#define NOVAL   ((value_t *)NULL)
#define NOACU   ((acu_t *)NULL)
#define NOSTR   ((char *)NULL)
#define NOFILE  ((FILE *)NULL)
#define NOPWD   ((struct passwd *)0)

struct sgttyb   arg;        /* current mode of local terminal */
struct sgttyb   defarg;     /* initial mode of local terminal */
struct tchars   tchars;     /* current state of terminal */
struct tchars   defchars;   /* initial state of terminal */
struct ltchars  ltchars;    /* current local characters of terminal */
struct ltchars  deflchars;  /* initial local characters of terminal */

FILE    *fscript;           /* FILE for scripting */

int fildes[2];              /* file transfer synchronization channel */
int repdes[2];              /* read process sychronization channel */
int FD;                     /* open file descriptor to remote host */
int AC;                     /* open file descriptor to dialer (v831 only) */
int vflag;                  /* print .tiprc initialization sequence */
int sfd;                    /* for ~< operation */
int pid;                    /* pid of tipout */
uid_t   uid, euid;          /* real and effective user id's */
gid_t   gid, egid;          /* real and effective group id's */
int stop;                   /* stop transfer session flag */
int quit;                   /* same; but on other end */
int intflag;                /* recognized interrupt */
int stoprompt;              /* for interrupting a prompt session */
int timedout;               /* ~> transfer timedout */
int cumode;                 /* simulating the "cu" program */

char    fname[80];          /* file name buffer for ~< */
char    copyname[80];       /* file name buffer for ~> */
char    ccc;                /* synchronization character */
char    ch;                 /* for tipout */
char    *uucplock;          /* name of lock file for uucp's */

int odisc;                  /* initial tty line discipline */
extern  int disc;           /* current tty discpline */

char *ctrl (int c);
char *connect (void);
void delock (char *s);
void pwrite (int fd, char *buf, int n);
int size (char *s);
void disconnect (char *reason);
void loginit (void);
void logent (char *group, char *num, char *acu, char *message);
int any (int c, char *p);
void raw (void);
void unraw (void);
int prompt (char *s, char *p);
void user_uid (void);
void shell_uid (void);
void daemon_uid (void);
void vinit (void);
void vlex (char *s);
int vstring (char *s, char *v);
void setparity (char *defparity);
int speed (int n);
int hunt (char *name);
void ttysetup (int speed);
int mlock (char *sys);
int rgetent (char *bp, char *name);
char *rgetstr (char *id, char **area);
int rgetnum (char *id);
int rgetflag (char *id);
void cumain (int argc, char *argv[]);
void tipout (void);
void setscript (void);
int escape (void);
void tipabort (char *msg);
void finish (void);

#ifndef ACULOG
#define logent(a, b, c, d)
#define loginit()
#endif
