/*
 * Copyright (c) 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/buf.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/inode.h>
#include <sys/proc.h>
#include <sys/clist.h>
#include <sys/tty.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <machine/spi.h>
#include <machine/uart.h>

#include <sys/swap.h>

extern int strcmp(char *s1, char *s2);

#ifdef SD_ENABLED
#   include <machine/sd.h>
#endif
#ifdef PTY_ENABLED
#   include <sys/pty.h>
#endif
#ifdef SDIO_ENABLED
#   include <machine/sdio.h>
#endif

/*
 * Null routine; placed in insignificant entries
 * in the bdevsw and cdevsw tables.
 */
int nulldev()
{
    return 0;
}

int noopen(dev, flag, mode)
    dev_t dev;
    int flag, mode;
{
    return ENXIO;
}

int norw(dev, uio, flag)
    dev_t dev;
    struct uio *uio;
    int flag;
{
    return 0;
}

int noioctl(dev, cmd, data, flag)
    dev_t dev;
    u_int cmd;
    caddr_t data;
    int flag;
{
    return EIO;
}

/*
 * root attach routine
 */
daddr_t nosize(dev)
    dev_t dev;
{
    return 0;
}

#define NOBDEV \
    noopen,         noopen,         nostrategy, \
    nosize,         noioctl,

/*
 * The RetroDisks require the same master number as the disk entry in the
 * rdisk.c file.  A bit of a bind, but it means that the RetroDisk
 * devices must be numbered from master 0 upwards.
 */
const struct bdevsw bdevsw[] = {
{   /* 0 - sd */
#ifdef SD_ENABLED
    sdopen,         sdclose,        sdstrategy,
    sdsize,         sdioctl,
#else
    NOBDEV
#endif
},
{   /* 1 - sramc */
    NOBDEV
},
{   /* 2 - sdramp */
    NOBDEV
},
{   /* 3 - mrams */
    NOBDEV
},
{   /* 4 - swap */
    swopen,         swclose,        swstrategy,
    swsize,         swcioctl,
},
{   /* 5 - spirams */
    NOBDEV
},

{ 0 },
};

const int nblkdev = sizeof(bdevsw) / sizeof(bdevsw[0]) - 1;

#define NOCDEV \
    noopen,         noopen,         norw,           norw, \
    noioctl,        nulldev,        0,              seltrue, \
    nostrategy,     0,              0,

const struct cdevsw cdevsw[] = {

/*
 * Static drivers - every system has these:
 */
{   /* 0 - console */
    cnopen,         cnclose,        cnread,         cnwrite,
    cnioctl,        nulldev,        cnttys,         cnselect,
    nostrategy,     0,              0,
},
{   /* 1 - mem, kmem, null, zero */
#if MEM_MAJOR != 1
#   error Wrong MEM_MAJOR value!
#endif
    nulldev,        nulldev,        mmrw,           mmrw,
    noioctl,        nulldev,        0,              seltrue,
    nostrategy,     0,              0,
},
{   /* 2 - tty */
    syopen,         nulldev,        syread,         sywrite,
    syioctl,        nulldev,        0,              syselect,
    nostrategy,     0,              0,
},
{   /* 3 - fd */
    fdopen,         nulldev,        norw,           norw,
    noioctl,        nulldev,        0,              seltrue,
    nostrategy,     0,              0,
},
{   /* 4 - temp (temporary allocation in swap space) */
    swcopen,        swcclose,       swcread,        swcwrite,
    swcioctl,       nulldev,        0,              seltrue,
    nostrategy,     0,              0,
},

/*
 * Optional drivers from here on:
 */
{   /* 5 - log */
#ifdef LOG_ENABLED
    logopen,        logclose,       logread,        norw,
    logioctl,       nulldev,        0,              logselect,
    nostrategy,     0,              0,
#else
    NOCDEV
#endif
},
{   /* 6 - tty uart */
#if UART_MAJOR != 6
#   error Wrong UART_MAJOR value!
#endif
#if defined(UART1_ENABLED) || defined(UART2_ENABLED) || \
    defined(UART3_ENABLED) || defined(UART4_ENABLED) || \
    defined(UART5_ENABLED) || defined(UART6_ENABLED)
    uartopen,       uartclose,      uartread,       uartwrite,
    uartioctl,      nulldev,        uartttys,       uartselect,
    nostrategy,     uartgetc,       uartputc,
#else
    NOCDEV
#endif
},
{   /* 7 - tty usb */
    NOCDEV
},
{   /* 8, 9 - pty */
#ifdef PTY_ENABLED
    ptsopen,        ptsclose,       ptsread,        ptswrite,
    ptyioctl,       nulldev,        pt_tty,         ptcselect,
    nostrategy,     0,              0,
}, {
    ptcopen,        ptcclose,       ptcread,        ptcwrite,
    ptyioctl,       nulldev,        pt_tty,         ptcselect,
    nostrategy,     0,              0,
#else
    NOCDEV }, { NOCDEV
#endif
},
{   /* 10 - gpio */
    NOCDEV
},
{   /* 11 - adc */
    NOCDEV
},
{   /* 12 - spi */
#if defined(SPI1_ENABLED) || defined(SPI2_ENABLED) || \
    defined(SPI3_ENABLED) || defined(SPI4_ENABLED) || \
    defined(SPI5_ENABLED) || defined(SPI6_ENABLED) || \
    defined(SPI7_ENABLED) || defined(SPI8_ENABLED)
    spi_open,       spi_close,      spi_read,       spi_write,
    spi_ioctl,      nulldev,        0,              seltrue,
    nostrategy,     0,              0,
#else
    NOCDEV
#endif
},
{   /* 13 - glcd */
    NOCDEV
},
{   /* 14 - pwm */
    NOCDEV
},
{   /* 15 - picga */            // Ignore this for now - it's WIP.
    NOCDEV
},
{   /* 16 - hxtft */
    NOCDEV
},
{   /* 17 - skel */
    NOCDEV
},
{   /* 18 - sdio */
#ifdef SDIO_ENABLED
    sdio_open,      sdio_close,     sdio_read,      sdio_write,
    sdio_ioctl,     nulldev,        0,              seltrue,
    nostrategy,     0,              0,
#else
    NOCDEV
#endif
},

/*
 * End the list with a blank entry
 */
{ 0 },
};

const int nchrdev = sizeof(cdevsw) / sizeof(cdevsw[0]) - 1;

/*
 * Routine that identifies /dev/mem and /dev/kmem.
 *
 * A minimal stub routine can always return 0.
 */
int
iskmemdev(dev)
    register dev_t dev;
{
    if (major(dev) == 1 && (minor(dev) == 0 || minor(dev) == 1))
        return 1;
    return 0;
}

/*
 * Routine to determine if a device is a disk.
 *
 * A minimal stub routine can always return 0.
 */
int
isdisk(dev, type)
    dev_t dev;
    register int type;
{
    if (type != IFBLK)
        return 0;

    switch (major(dev)) {
    case 0:                 /* rd0 */
    case 1:                 /* rd1 */
    case 2:                 /* rd2 */
    case 3:                 /* rd3 */
    case 4:                 /* sw */
        return 1;
    default:
        return 0;
    }
    /* NOTREACHED */
}

/*
 * Routine to convert from character to block device number.
 * A minimal stub routine can always return NODEV.
 */
int
chrtoblk(dev_t dev)
{
    return NODEV;
}
