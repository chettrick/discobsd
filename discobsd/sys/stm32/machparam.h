/*
 * Machine dependent constants for STM32.
 *
 * Copyright (c) 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#ifndef ENDIAN

/*
 * Definitions for byte order,
 * according to byte significance from low address to high.
 */
#define LITTLE          1234            /* least-significant byte first (vax) */
#define BIG             4321            /* most-significant byte first */
#define PDP             3412            /* LSB first in word, MSW first in long (pdp) */
#define ENDIAN          LITTLE          /* byte order on stm32 */

/*
 * The time for a process to be blocked before being very swappable.
 * This is a number of seconds which the system takes as being a non-trivial
 * amount of real time.  You probably shouldn't change this;
 * it is used in subtle ways (fractions and multiples of it are, that is, like
 * half of a ``long time'', almost a long time, etc.)
 * It is related to human patience and other factors which don't really
 * change over time.
 */
#define MAXSLP          20

/*
 * Clock ticks per second. The HZ value must be an integer factor of 1000.
 */
#ifndef HZ
#define HZ              200
#endif

/*
 * System parameter formulae.
 */
#ifndef NBUF
#define NBUF            10                      /* number of i/o buffers */
#endif
#ifndef MAXUSERS
#define MAXUSERS        1                       /* number of user logins */
#endif
#ifndef NPROC
#define NPROC           25                      /* number of processes */
#endif
#ifndef NINODE
#define NINODE          24
#endif
#ifndef NFILE
#define NFILE           24
#endif
#define NNAMECACHE      (NINODE * 11/10)
#define NCALL           (16 + 2 * MAXUSERS)
#define NCLIST          32                      /* number or CBSIZE blocks */
#ifndef SMAPSIZ
#define SMAPSIZ         NPROC                   /* size of swap allocation map */
#endif

/*
 * Disk blocks.
 */
#define DEV_BSIZE       1024            /* the same as MAXBSIZE */
#define DEV_BSHIFT      10              /* log2(DEV_BSIZE) */
#define DEV_BMASK       (DEV_BSIZE-1)

/* Bytes to disk blocks */
#define btod(x)         (((x) + DEV_BSIZE-1) >> DEV_BSHIFT)

/* XXX FLASH SRAM
 * On PIC32, there are total 512 kbytes of flash and 128 kbytes of RAM.
 * We reserve for kernel 192 kbytes of flash and 32 kbytes of RAM.
 */
#define FLASH_SIZE              (1024*1024)     /* Minimum from STM32F407 chip. */
#define DATA_SIZE               (128*1024)

#define KERNEL_FLASH_START      (0x08000000)
#define KERNEL_FLASH_SIZE       (192*1024)
#define KERNEL_FLASH_END        (KERNEL_FLASH_START + KERNEL_FLASH_SIZE)

#define USER_FLASH_START        (KERNEL_FLASH_START + KERNEL_FLASH_SIZE)
#define USER_FLASH_END          (KERNEL_FLASH_START + FLASH_SIZE)

#define KERNEL_DATA_START       (0x10000000)
#define KERNEL_DATA_SIZE        (64*1024)       /* CCM RAM for kernel is 64kb. */
#define KERNEL_DATA_END         (KERNEL_DATA_START + KERNEL_DATA_SIZE)

#define USER_DATA_START         (0x20000000)
#define USER_DATA_SIZE          (112*1024)      /* Minimum from STM32F407 chip. */
#define USER_DATA_END           (USER_DATA_START + USER_DATA_SIZE)

#define stacktop(siz)           (USER_DATA_END)
#define stackbas(siz)           (USER_DATA_END-(siz))

/*
 * User area: a user structure, followed by the kernel
 * stack.  The number for USIZE is determined empirically.
 *
 * Note that the SBASE and STOP constants are only used by the assembly code,
 * but are defined here to localize information about the user area's
 * layout (see pdp/genassym.c).  Note also that a networking stack is always
 * allocated even for non-networking systems.  This prevents problems with
 * applications having to be recompiled for networking versus non-networking
 * systems.
 */
#define USIZE           3072
#define SSIZE           2048            /* initial stack size (bytes) */

/*
 * Collect kernel statistics by default.
 */
#if !defined(UCB_METER) && !defined(NO_UCB_METER)
#define UCB_METER
#endif

#ifdef KERNEL
#include "machine/io.h"

/*
 * Macros to decode processor status word.
 */
#define USERMODE(ps)    (((ps) & ST_UM) != 0)
#define BASEPRI(ps)     (CA_RIPL(ps) == 0)

#define splbio()        mips_intr_disable()
#define spltty()        mips_intr_disable()
#define splclock()      mips_intr_disable()
#define splhigh()       mips_intr_disable()
#define splnet()        mips_intr_enable()
#define splsoftclock()  mips_intr_enable()
#define spl0()          mips_intr_enable()
#define splx(s)         mips_intr_restore(s)

#define noop()          asm volatile("nop")

/*
 * Wait for something to happen.
 */
void idle(void);

/*
 * Microsecond delay routine.
 */
void udelay(unsigned usec);

/*
 * Setup system timer for `hz' timer interrupts per second.
 */
void clkstart(void);

/*
 * Control LEDs, installed on the board.
 */
#define LED_TTY         0x08
#define LED_SWAP        0x04
#define LED_DISK        0x02
#define LED_KERNEL      0x01

void led_control(int mask, int on);

/*
 * SD timeouts, for sysctl.
 */
extern int sd_timo_cmd;
extern int sd_timo_send_op;
extern int sd_timo_send_csd;
extern int sd_timo_read;
extern int sd_timo_wait_cmd;
extern int sd_timo_wait_wdata;
extern int sd_timo_wait_wdone;
extern int sd_timo_wait_wstop;
extern int sd_timo_wait_widle;

#endif /* KERNEL */

#endif /* ENDIAN */
