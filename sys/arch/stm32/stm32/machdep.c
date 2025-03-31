/*
 * Copyright (c) 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 */
#include <sys/param.h>
#include <sys/dir.h>
#include <sys/inode.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/fs.h>
#include <sys/map.h>
#include <sys/buf.h>
#include <sys/file.h>
#include <sys/clist.h>
#include <sys/callout.h>
#include <sys/reboot.h>
#include <sys/msgbuf.h>
#include <sys/namei.h>
#include <sys/mount.h>
#include <sys/systm.h>
#include <sys/kconfig.h>
#include <sys/tty.h>

#include <machine/fault.h>
#include <machine/uart.h>
#include <machine/stm32f4xx_ll_bus.h>
#include <machine/stm32f4xx_ll_gpio.h>
#include <machine/stm32f4xx_ll_pwr.h>
#include <machine/stm32f4xx_ll_rcc.h>
#include <machine/stm32f4xx_ll_system.h>
#include <machine/stm32f4xx_ll_utils.h>
#include <machine/stm32f4xx_hal.h>
#include <machine/stm32_bsp.h>

/*
 * Kernel-specific uses of LEDs and buttons provided by the
 * board support package and defined in the kernel Config.
 * LED activity indicators: TTY, SWAP, DISK, KERNEL
 * User Button: Enter single user mode when pressed during boot.
 */
#if defined(BSP) && defined(BSP_LED_TTY)
#define LED_TTY_INIT()          BSP_LED_Init(BSP_LED_TTY)
#define LED_TTY_ON()            BSP_LED_On(BSP_LED_TTY)
#define LED_TTY_OFF()           BSP_LED_Off(BSP_LED_TTY)
#else
#define LED_TTY_INIT()          /* Nothing. */
#define LED_TTY_ON()            /* Nothing. */
#define LED_TTY_OFF()           /* Nothing. */
#endif

#if defined(BSP) && defined(BSP_LED_SWAP)
#define LED_SWAP_INIT()         BSP_LED_Init(BSP_LED_SWAP)
#define LED_SWAP_ON()           BSP_LED_On(BSP_LED_SWAP)
#define LED_SWAP_OFF()          BSP_LED_Off(BSP_LED_SWAP)
#else
#define LED_SWAP_INIT()         /* Nothing. */
#define LED_SWAP_ON()           /* Nothing. */
#define LED_SWAP_OFF()          /* Nothing. */
#endif

#if defined(BSP) && defined(BSP_LED_DISK)
#define LED_DISK_INIT()         BSP_LED_Init(BSP_LED_DISK)
#define LED_DISK_ON()           BSP_LED_On(BSP_LED_DISK)
#define LED_DISK_OFF()          BSP_LED_Off(BSP_LED_DISK)
#else
#define LED_DISK_INIT()         /* Nothing. */
#define LED_DISK_ON()           /* Nothing. */
#define LED_DISK_OFF()          /* Nothing. */
#endif

#if defined(BSP) && defined(BSP_LED_KERNEL)
#define LED_KERNEL_INIT()       BSP_LED_Init(BSP_LED_KERNEL)
#define LED_KERNEL_ON()         BSP_LED_On(BSP_LED_KERNEL)
#define LED_KERNEL_OFF()        BSP_LED_Off(BSP_LED_KERNEL)
#else
#define LED_KERNEL_INIT()       /* Nothing. */
#define LED_KERNEL_ON()         /* Nothing. */
#define LED_KERNEL_OFF()        /* Nothing. */
#endif

#if defined(BSP) && defined(BSP_BUTTON_USER)
#define BUTTON_USER_INIT()      BSP_PB_Init(BSP_BUTTON_USER)
#define BUTTON_USER_PRESSED()   BSP_PB_GetState(BSP_BUTTON_USER)
#else
#define BUTTON_USER_INIT()      /* Nothing. */
#define BUTTON_USER_PRESSED()   (0)     /* Not pressed. */
#endif

char    machine[] = MACHINE;            /* from <machine/machparam.h> */
char    machine_arch[] = MACHINE_ARCH;  /* from <machine/machparam.h> */
char    cpu_model[64];

int     hz = HZ;
int     usechz = (1000000L + HZ - 1) / HZ;
#ifdef TIMEZONE
struct  timezone tz = { TIMEZONE, DST };
#else
struct  timezone tz = { 8*60, 1 };
#endif
int     nproc = NPROC;

struct  namecache namecache [NNAMECACHE];
char    bufdata [NBUF * MAXBSIZE];
struct  inode inode [NINODE];
struct  callout callout [NCALL];
struct  mount mount [NMOUNT];
struct  buf buf [NBUF], bfreelist [BQUEUES];
struct  bufhd bufhash [BUFHSZ];
struct  cblock cfree [NCLIST];
struct  proc proc [NPROC];
struct  file file [NFILE];

/*
 * Remove the ifdef/endif to run the kernel in unsecure mode even when in
 * a multiuser state.  Normally 'init' raises the security level to 1
 * upon transitioning to multiuser.  Setting the securelevel to -1 prevents
 * the secure level from being raised by init.
 */
#ifdef  PERMANENTLY_INSECURE
int securelevel = -1;
#else
int securelevel = 0;
#endif

struct mapent   swapent[SMAPSIZ];
struct map  swapmap[1] = {
    { swapent,
      &swapent[SMAPSIZ],
      "swapmap" },
};

int waittime = -1;

static int
nodump(dev)
    dev_t dev;
{
    printf("\ndumping to dev %o off %D: not implemented\n", dumpdev, dumplo);
    return 0;
}

int (*dump)(dev_t) = nodump;

dev_t   pipedev;
daddr_t dumplo = (daddr_t) 1024;

static void
SystemClock_Config(void)
{
    /* Enable HSE oscillator */
#if defined(STM32F411xE)
    LL_RCC_HSE_EnableBypass();
#endif

    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1)
    {
    };

    /* Set FLASH latency */
#if defined(STM32F405xx) || defined(STM32F407xx) || defined(STM32F469xx)
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);
#endif
#if defined(STM32F411xE) || defined(STM32F412Rx) || defined(STM32F412Zx)
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);
#endif

    /* Enable PWR clock */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

#if defined(STM32F469xx)
    /* Activation OverDrive Mode */
    LL_PWR_EnableOverDriveMode();
    while(LL_PWR_IsActiveFlag_OD() != 1)
    {
    };

    /* Activation OverDrive Switching */
    LL_PWR_EnableOverDriveSwitching();
    while(LL_PWR_IsActiveFlag_ODSW() != 1)
    {
    };
#endif

    /* Main PLL configuration and activation */
#ifdef STM32F405xx      /* 168 MHz */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 336, LL_RCC_PLLP_DIV_2);
#endif
#ifdef STM32F407xx      /* 168 MHz */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 336, LL_RCC_PLLP_DIV_2);
#endif
#ifdef STM32F411xE      /* 100 MHz */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 400, LL_RCC_PLLP_DIV_4);
#endif
#ifdef STM32F412Rx      /* 100 MHz */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 200, LL_RCC_PLLP_DIV_2);
#endif
#ifdef STM32F412Zx      /* 100 MHz */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 200, LL_RCC_PLLP_DIV_2);
#endif
#ifdef STM32F469xx      /* 180 MHz */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 360, LL_RCC_PLLP_DIV_2);
#endif

    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1)
    {
    };

    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    };

    /* Set APB1 & APB2 prescaler */
#ifdef STM32F405xx      /* 168 MHz */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
#endif
#ifdef STM32F407xx      /* 168 MHz */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
#endif
#ifdef STM32F411xE      /* 100 MHz */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
#endif
#ifdef STM32F412Rx      /* 100 MHz */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
#endif
#ifdef STM32F412Zx      /* 100 MHz */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
#endif
#ifdef STM32F469xx      /* 180 MHz */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
#endif

    /* Set SysTick to 1ms */
    SysTick_Config(CPU_KHZ);

    /* Update CMSIS variable (or through SystemCoreClockUpdate()) */
    SystemCoreClock = CPU_KHZ * 1000;
}

/*
 * Machine dependent startup code
 */
void
startup()
{
    HAL_Init();

    SystemClock_Config();

    /* Enable all configurable fault handlers. */
    arm_enable_fault(MM_FAULT_ENABLE);
    arm_enable_fault(BF_FAULT_ENABLE);
    arm_enable_fault(UF_FAULT_ENABLE);

    /* Syscalls (via PendSV) have the lowest interrupt priority. */
    arm_intr_set_priority(PendSV_IRQn, IPL_PENDSV);

    /* SVCall exceptions have the highest interrupt priority. */
    arm_intr_set_priority(SVCall_IRQn, IPL_SVCALL);

    /* SysTick exceptions have the interrupt priority of IPL_CLOCK. */
    arm_intr_set_priority(SysTick_IRQn, IPL_SYSTICK);

    /*
     * Configure LED pins.
     */
    LED_TTY_INIT();                     /* Green.   Terminal i/o */
    LED_SWAP_INIT();                    /* Orange.  Auxiliary swap */
    LED_DISK_INIT();                    /* Red.     Disk i/o */
    LED_KERNEL_INIT();                  /* Blue.    Kernel activity */

    LED_TTY_ON();
    LED_SWAP_ON();
    LED_DISK_ON();
    LED_KERNEL_ON();

    LED_TTY_OFF();
    LED_SWAP_OFF();
    LED_DISK_OFF();
    LED_KERNEL_OFF();

    led_control(LED_ALL, 1);
    led_control(LED_ALL, 0);

    /*
     * Configure User Button.
     */
    BUTTON_USER_INIT();

    /*
     * Early setup for console devices.
     */
#if CONS_MAJOR == UART_MAJOR
    uartinit(CONS_MINOR);
#endif

    /*
     * When User button is pressed - boot to single user mode.
     */
    boothowto = 0;
    if (BUTTON_USER_PRESSED()) {
        boothowto |= RB_SINGLE;
    }
}

static void
cpuidentify()
{
    unsigned devid  = LL_DBGMCU_GetDeviceID();
    unsigned revid  = LL_DBGMCU_GetRevisionID();

    printf("cpu: ");
    switch (devid) {
    /*
     * Device errata 2.2.2: MCU device ID is incorrect
     *
     * Revision A STM32F40x and STM32F41x devices have the same MCU device
     * ID as the STM32F20x and STM32F21x devices.
     * Reading the revision identifier returns 0x2000 instead of 0x1000.
     */
    case 0x0411:
        physmem = 192 * 1024;   /* Total 192kb RAM size. */
        copystr("STM32F405/407xx", cpu_model, sizeof(cpu_model), NULL);
        printf("STM32F405/407xx");
        printf(" rev ");
        switch (revid) {
        case 0x2000:
            printf("A");
            break;
        default:
            printf("unknown 0x%04x", revid);
            break;
        }
        break;
    case 0x0413:
        physmem = 192 * 1024;   /* Total 192kb RAM size. */
        copystr("STM32F405/407xx", cpu_model, sizeof(cpu_model), NULL);
        printf("STM32F405/407xx");
        printf(" rev ");
        switch (revid) {
        case 0x1000:
            printf("A");
            break;
        case 0x1001:
            printf("Z");
            break;
        case 0x1003:
            printf("1");
            break;
        case 0x1007:
            printf("2");
            break;
        case 0x100f:
            printf("4/Y");      /* Device marking revisions 4 and Y. */
            break;
        case 0x101f:
            printf("5/6");      /* Device marking revisions 5 and 6. */
            break;
        default:
            printf("unknown 0x%04x", revid);
            break;
        }
        break;
    case 0x0431:
        physmem = 128 * 1024;   /* Total 128kb RAM size. */
        copystr("STM32F411xC/E", cpu_model, sizeof(cpu_model), NULL);
        printf("STM32F411xC/E");
        printf(" rev ");
        switch (revid) {
        case 0x1000:
            printf("1/2/A");    /* Device marking revisions 1, 2, and A. */
            break;
        default:
            printf("unknown 0x%04x", revid);
            break;
        }
        break;
    case 0x0434:
        physmem = 384 * 1024;   /* Total 384kb RAM size. */
        copystr("STM32F469/479xx", cpu_model, sizeof(cpu_model), NULL);
        printf("STM32F469/479xx");
        printf(" rev ");
        switch (revid) {
        case 0x1000:
            printf("1/A");      /* Device marking revisions 1 and A. */
            break;
        default:
            printf("unknown 0x%04x", revid);
            break;
        }
        break;
    case 0x0441:
        physmem = 256 * 1024;   /* Total 256kb RAM size. */
        copystr("STM32F412xE/G", cpu_model, sizeof(cpu_model), NULL);
        printf("STM32F412xE/G");
        printf(" rev ");
        switch (revid) {
        case 0x1001:
            printf("Z");
            break;
        case 0x2000:
            printf("B");
            break;
        case 0x3000:
            printf("1/C");      /* Device marking revisions 1 and C. */
            break;
        default:
            printf("unknown 0x%04x", revid);
            break;
        }
        break;
    default:
        physmem = 128 * 1024;   /* Minimum of 128kb total RAM size. */
        copystr("STM32 device unknown", cpu_model, sizeof(cpu_model), NULL);
        printf("device unknown 0x%04x", devid);
        printf(" rev unknown 0x%04x", revid);
        break;
    }
    printf(", %u MHz, bus %u MHz\n", CPU_KHZ/1000, BUS_KHZ/1000);

    printf("oscillator: ");
    switch (LL_RCC_GetSysClkSource()) {
    case LL_RCC_SYS_CLKSOURCE_STATUS_HSI:
        printf("high speed internal\n");
        break;
    case LL_RCC_SYS_CLKSOURCE_STATUS_HSE:
        printf("high speed external\n");
        break;
    case LL_RCC_SYS_CLKSOURCE_STATUS_PLL:
        printf("phase-locked loop, clock source: ");
        switch (LL_RCC_PLL_GetMainSource()) {
        case LL_RCC_PLLSOURCE_HSI:
            printf("high speed internal\n");
            break;
        case LL_RCC_PLLSOURCE_HSE:
            printf("high speed external\n");
            break;
        default:
            printf("unknown\n");
            break;
        }
        break;
    default:
        printf("unknown\n");
        break;
    }
}

/*
 * Check whether the controller has been successfully initialized.
 */
static int
is_controller_alive(driver, unit)
    struct driver *driver;
    int unit;
{
    struct conf_ctlr *ctlr;

    /* No controller - that's OK. */
    if (driver == 0)
        return 1;

    for (ctlr = conf_ctlr_init; ctlr->ctlr_driver; ctlr++) {
        if (ctlr->ctlr_driver == driver &&
            ctlr->ctlr_unit == unit &&
            ctlr->ctlr_alive)
        {
            return 1;
        }
    }
    return 0;
}

/*
 * Configure all controllers and devices as specified
 * in the kernel configuration file.
 */
void
kconfig()
{
    struct conf_ctlr *ctlr;
    struct conf_device *dev;

    cpuidentify();

    /* Probe and initialize controllers first. */
    for (ctlr = conf_ctlr_init; ctlr->ctlr_driver; ctlr++) {
        if ((*ctlr->ctlr_driver->d_init)(ctlr)) {
            ctlr->ctlr_alive = 1;
        }
    }

    /* Probe and initialize devices. */
    for (dev = conf_device_init; dev->dev_driver; dev++) {
        if (is_controller_alive(dev->dev_cdriver, dev->dev_ctlr)) {
            if ((*dev->dev_driver->d_init)(dev)) {
                dev->dev_alive = 1;
            }
        }
    }
}

/*
 * Sit and wait for something to happen...
 */
void
idle()
{
    /* Indicate that no process is running. */
    noproc = 1;

    /* Set SPL low so we can be interrupted. */
    int x = spl0();

    led_control(LED_KERNEL, 0);

    /* Wait for something to happen. */
    __DSB();
    __ISB();
    __WFI();

    /* Restore previous SPL. */
    splx(x);
}

void
boot(dev, howto)
    register dev_t dev;
    register int howto;
{
    if ((howto & RB_NOSYNC) == 0 && waittime < 0 && bfreelist[0].b_forw) {
        register struct fs *fp;
        register struct buf *bp;
        int iter, nbusy;

        /*
         * Force the root filesystem's superblock to be updated,
         * so the date will be as current as possible after
         * rebooting.
         */
        fp = getfs(rootdev);
        if (fp)
            fp->fs_fmod = 1;
        waittime = 0;
        printf("syncing disks... ");
        (void) splnet();
        sync();
        for (iter = 0; iter < 20; iter++) {
            nbusy = 0;
            for (bp = &buf[NBUF]; --bp >= buf; )
                if (bp->b_flags & B_BUSY)
                    nbusy++;
            if (nbusy == 0)
                break;
            printf("%d ", nbusy);
            mdelay(40L * iter);
        }
        printf("done\n");
    }
    (void) splhigh();
    if (! (howto & RB_HALT)) {
        if ((howto & RB_DUMP) && dumpdev != NODEV) {
            /*
             * Take a dump of memory by calling (*dump)(),
             * which must correspond to dumpdev.
             * It should dump from dumplo blocks to the end
             * of memory or to the end of the logical device.
             */
            (*dump)(dumpdev);
        }
        /* Restart from dev, howto */

        /* Reset microcontroller */
        NVIC_SystemReset();
        /* NOTREACHED */
    }
    printf("halted\n");

#ifdef HALTREBOOT
    printf("press any key to reboot...\n");
    cngetc();

    /* Reset microcontroller */
    NVIC_SystemReset();
    /* NOTREACHED */
#endif

    printf("reboot failed; spinning\n");
    for (;;) {
        __DSB();
        __ISB();
        __WFI();
    }
    /* NOTREACHED */
}

/*
 * Millisecond delay routine.
 *
 * Uses SysTick, which must be configured to a 1ms timebase.
 * This is a busy-wait blocking delay, so be wise with use.
 */
void
mdelay(msec)
    u_int msec;
{
    LL_mDelay(msec);
}

/*
 * Control LEDs, installed on the board.
 */
void led_control(int mask, int on)
{
    if (mask & LED_TTY) {       /* Terminal i/o */
        if (on) LED_TTY_ON();
        else    LED_TTY_OFF();
    }
    if (mask & LED_SWAP) {      /* Auxiliary swap */
        if (on) LED_SWAP_ON();
        else    LED_SWAP_OFF();
    }
    if (mask & LED_DISK) {      /* Disk i/o */
        if (on) LED_DISK_ON();
        else    LED_DISK_OFF();
    }
    if (mask & LED_KERNEL) {    /* Kernel activity */
        if (on) LED_KERNEL_ON();
        else    LED_KERNEL_OFF();
    }
}

/*
 * Increment user profiling counters.
 */
void addupc(caddr_t pc, struct uprof *pbuf, int ticks)
{
    unsigned indx;

    if (pc < (caddr_t) pbuf->pr_off)
        return;

    indx = pc - (caddr_t) pbuf->pr_off;
    indx = (indx * pbuf->pr_scale) >> 16;
    if (indx >= pbuf->pr_size)
        return;

    pbuf->pr_base[indx] += ticks;
}

/*
 * ffs -- vax ffs instruction
 */
int
ffs(mask)
    register u_long mask;
{
    register int cnt;

    if (mask == 0)
        return (0);
    for (cnt = 1; !(mask&1); cnt++)
        mask >>= 1;
    return (cnt);
}

/*
 * Copy a null terminated string from one point to another.
 * Returns zero on success, ENOENT if maxlength exceeded.
 * If lencopied is non-zero, *lencopied gets the length of the copy
 * (including the null terminating byte).
 */
int
copystr(src, dest, maxlength, lencopied)
    register caddr_t src, dest;
    register u_int maxlength, *lencopied;
{
    caddr_t dest0 = dest;
    int error = ENOENT;

    if (maxlength != 0) {
        while ((*dest++ = *src++) != '\0') {
            if (--maxlength == 0) {
                /* Failed. */
                goto done;
            }
        }
        /* Succeeded. */
        error = 0;
    }
done:
    if (lencopied != 0)
        *lencopied = dest - dest0;
    return error;
}

/*
 * Calculate the length of a string.
 */
size_t
strlen(s)
    register const char *s;
{
    const char *s0 = s;

    while (*s++ != '\0')
        ;
    return s - s0 - 1;
}

/*
 * Return 0 if a user address is valid.
 * There is only one memory region allowed for user: RAM.
 */
int
baduaddr(addr)
    register caddr_t addr;
{
    if (addr >= (caddr_t)__user_data_start &&
        addr < (caddr_t)__user_data_end)
        return 0;
    return 1;
}

/*
 * Return 0 if a kernel address is valid.
 * There are two memory regions allowed for kernel: RAM and flash.
 */
int
badkaddr(addr)
    register caddr_t addr;
{
    if (addr >= (caddr_t)__kernel_data_start &&
        addr < (caddr_t)__kernel_data_end)
        return 0;
    if (addr >= (caddr_t)__kernel_flash_start &&
        addr < (caddr_t)__kernel_flash_end)
        return 0;
    return 1;
}

/*
 * Insert the specified element into a queue immediately after
 * the specified predecessor element.
 */
void insque(void *element, void *predecessor)
{
    struct que {
        struct que *q_next;
        struct que *q_prev;
    };
    register struct que *e = (struct que *) element;
    register struct que *prev = (struct que *) predecessor;

    e->q_prev = prev;
    e->q_next = prev->q_next;
    prev->q_next->q_prev = e;
    prev->q_next = e;
}

/*
 * Remove the specified element from the queue.
 */
void remque(void *element)
{
    struct que {
        struct que *q_next;
        struct que *q_prev;
    };
    register struct que *e = (struct que *) element;

    e->q_prev->q_next = e->q_next;
    e->q_next->q_prev = e->q_prev;
}

/*
 * Compare strings.
 */
int strncmp(const char *s1, const char *s2, size_t n)
{
    register int ret, tmp;

    if (n == 0)
        return 0;
    do {
        ret = *s1++ - (tmp = *s2++);
    } while ((ret == 0) && (tmp != 0) && --n);
    return ret;
}

/* Nonzero if pointer is not aligned on a "sz" boundary.  */
#define UNALIGNED(p, sz)    ((unsigned) (p) & ((sz) - 1))

/*
 * Copy data from the memory region pointed to by src0 to the memory
 * region pointed to by dst0.
 * If the regions overlap, the behavior is undefined.
 */
void
bcopy(const void *src0, void *dst0, size_t nbytes)
{
    unsigned char *dst = dst0;
    const unsigned char *src = src0;
    unsigned *aligned_dst;
    const unsigned *aligned_src;

//printf("bcopy (%08x, %08x, %d)\n", src0, dst0, nbytes);
    /* If the size is small, or either SRC or DST is unaligned,
     * then punt into the byte copy loop.  This should be rare.  */
    if (nbytes >= 4*sizeof(unsigned) &&
        ! UNALIGNED(src, sizeof(unsigned)) &&
        ! UNALIGNED(dst, sizeof(unsigned))) {
        aligned_dst = (unsigned*) dst;
        aligned_src = (const unsigned*) src;

        /* Copy 4X unsigned words at a time if possible.  */
        while (nbytes >= 4*sizeof(unsigned)) {
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            nbytes -= 4*sizeof(unsigned);
        }

        /* Copy one unsigned word at a time if possible.  */
        while (nbytes >= sizeof(unsigned)) {
            *aligned_dst++ = *aligned_src++;
            nbytes -= sizeof(unsigned);
        }

        /* Pick up any residual with a byte copier.  */
        dst = (unsigned char*) aligned_dst;
        src = (const unsigned char*) aligned_src;
    }

    while (nbytes--)
        *dst++ = *src++;
}

void *
memcpy(void *dst, const void *src, size_t nbytes)
{
    bcopy(src, dst, nbytes);
    return dst;
}

/*
 * Fill the array with zeroes.
 */
void
bzero(void *dst0, size_t nbytes)
{
    unsigned char *dst;
    unsigned *aligned_dst;

    dst = (unsigned char*) dst0;
    while (UNALIGNED(dst, sizeof(unsigned))) {
        *dst++ = 0;
        if (--nbytes == 0)
            return;
    }
    if (nbytes >= sizeof(unsigned)) {
        /* If we get this far, we know that nbytes is large and dst is word-aligned. */
        aligned_dst = (unsigned*) dst;

        while (nbytes >= 4*sizeof(unsigned)) {
            *aligned_dst++ = 0;
            *aligned_dst++ = 0;
            *aligned_dst++ = 0;
            *aligned_dst++ = 0;
            nbytes -= 4*sizeof(unsigned);
        }
        while (nbytes >= sizeof(unsigned)) {
            *aligned_dst++ = 0;
            nbytes -= sizeof(unsigned);
        }
        dst = (unsigned char*) aligned_dst;
    }

    /* Pick up the remainder with a bytewise loop.  */
    while (nbytes--)
        *dst++ = 0;
}

/*
 * Compare not more than nbytes of data pointed to by m1 with
 * the data pointed to by m2. Return an integer greater than, equal to or
 * less than zero according to whether the object pointed to by
 * m1 is greater than, equal to or less than the object
 * pointed to by m2.
 */
int
bcmp(const void *m1, const void *m2, size_t nbytes)
{
    const unsigned char *s1 = (const unsigned char*) m1;
    const unsigned char *s2 = (const unsigned char*) m2;
    const unsigned *aligned1, *aligned2;

    /* If the size is too small, or either pointer is unaligned,
     * then we punt to the byte compare loop.  Hopefully this will
     * not turn up in inner loops.  */
    if (nbytes >= 4*sizeof(unsigned) &&
        ! UNALIGNED(s1, sizeof(unsigned)) &&
        ! UNALIGNED(s2, sizeof(unsigned))) {
        /* Otherwise, load and compare the blocks of memory one
           word at a time.  */
        aligned1 = (const unsigned*) s1;
        aligned2 = (const unsigned*) s2;
        while (nbytes >= sizeof(unsigned)) {
            if (*aligned1 != *aligned2)
                break;
            aligned1++;
            aligned2++;
            nbytes -= sizeof(unsigned);
        }

        /* check remaining characters */
        s1 = (const unsigned char*) aligned1;
        s2 = (const unsigned char*) aligned2;
    }
    while (nbytes--) {
        if (*s1 != *s2)
            return *s1 - *s2;
        s1++;
        s2++;
    }
    return 0;
}

int
copyout(caddr_t from, caddr_t to, u_int nbytes)
{
    //printf("copyout (from=%p, to=%p, nbytes=%u)\n", from, to, nbytes);
    if (baduaddr(to) || baduaddr(to + nbytes - 1))
        return EFAULT;
    bcopy(from, to, nbytes);
    return 0;
}

int copyin (caddr_t from, caddr_t to, u_int nbytes)
{
    if (baduaddr(from) || baduaddr(from + nbytes - 1))
        return EFAULT;
    bcopy(from, to, nbytes);
    return 0;
}

/**
  * @brief  Enable AHB1 peripherals clock on dedicated port.
  * @param  GPIOx GPIO Port
  * @note   This function is an extension to the LL_GPIO library.
  */
void
LL_GPIO_EnableClock(GPIO_TypeDef *GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));

    if (GPIOx == GPIOA) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    } else if (GPIOx == GPIOB) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    } else if (GPIOx == GPIOC) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
#if defined(GPIOD)
    } else if (GPIOx == GPIOD) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
#endif /* GPIOD */
#if defined(GPIOE)
    } else if (GPIOx == GPIOE) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
#endif /* GPIOE */
#if defined(GPIOF)
    } else if (GPIOx == GPIOF) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
#endif /* GPIOF */
#if defined(GPIOG)
    } else if (GPIOx == GPIOG) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
#endif /* GPIOG */
#if defined(GPIOH)
    } else if (GPIOx == GPIOH) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
#endif /* GPIOH */
#if defined(GPIOI)
    } else if (GPIOx == GPIOI) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI);
#endif /* GPIOI */
#if defined(GPIOJ)
    } else if (GPIOx == GPIOJ) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOJ);
#endif /* GPIOJ */
#if defined(GPIOK)
    } else if (GPIOx == GPIOK) {
        LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOK);
#endif /* GPIOK */
    }
}
