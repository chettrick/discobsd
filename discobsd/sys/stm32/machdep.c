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

#include <machine/uart.h>
#include <machine/stm32f4xx_ll_bus.h>
#include <machine/stm32f4xx_ll_gpio.h>
#include <machine/stm32f4xx_ll_pwr.h>
#include <machine/stm32f4xx_ll_rcc.h>
#include <machine/stm32f4xx_ll_system.h>
#include <machine/stm32f4xx_hal.h>
#include <machine/stm32469i_discovery.h>

#ifdef POWER_ENABLED
extern void power_init();
extern void power_off();
#endif

#define LED_TTY_ON()        BSP_LED_On(LED_GREEN)
#define LED_TTY_OFF()       BSP_LED_Off(LED_GREEN)
#define LED_SWAP_ON()       BSP_LED_On(LED_ORANGE)
#define LED_SWAP_OFF()      BSP_LED_Off(LED_ORANGE)
#define LED_DISK_ON()       BSP_LED_On(LED_RED)
#define LED_DISK_OFF()      BSP_LED_Off(LED_RED)
#define LED_KERNEL_ON()     BSP_LED_On(LED_BLUE)
#define LED_KERNEL_OFF()    BSP_LED_Off(LED_BLUE)

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

#ifdef CONFIG
/*
 * Build using old configuration utility (configsys).
 */
dev_t   rootdev = NODEV;
dev_t   swapdev = NODEV;
dev_t   dumpdev = NODEV;
#endif
dev_t   pipedev;
daddr_t dumplo = (daddr_t) 1024;

static void
SystemClock_Config(void)
{
    /* Enable HSE oscillator */
    LL_RCC_HSE_Enable();
    while(LL_RCC_HSE_IsReady() != 1)
    {
    };

    /* Set FLASH latency */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

#ifdef STM32F469xx
    /* Enable PWR clock */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet. */
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

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
#endif /* STM32F469xx */

    /* Main PLL configuration and activation */
#ifdef STM32F407xx      /* 168 MHz */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 336, LL_RCC_PLLP_DIV_2);
#endif /* STM32F407xx */
#ifdef STM32F469xx      /* 180 MHz */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_8, 360, LL_RCC_PLLP_DIV_2);
#endif /* STM32F469xx */

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
#ifdef STM32F407xx      /* 168 MHz */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
#endif /* STM32F407xx */
#ifdef STM32F469xx      /* 180 MHz */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
#endif /* STM32F469xx */
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);

    /* Set systick to 1ms */
    SysTick_Config(CPU_KHZ);

    /* Update CMSIS variable (or through SystemCoreClockUpdate()) */
    SystemCoreClock = CPU_KHZ * 1000;
}

/*
 * Check whether button 1 is pressed.
 */
static inline int
button1_pressed()
{
// XXX BUTTON
    return 0;
}

/*
 * Machine dependent startup code
 */
void
startup()
{
    HAL_Init();

    SystemClock_Config();

    /*
     * Configure LED pins.
     */
    BSP_LED_Init(LED1);                 /* Green.   Terminal i/o */
    BSP_LED_Init(LED2);                 /* Orange.  Auxiliary swap */
    BSP_LED_Init(LED3);                 /* Red.     Disk i/o */
    BSP_LED_Init(LED4);                 /* Blue.    Kernel activity */

    LED_TTY_ON();
    LED_SWAP_ON();
    LED_DISK_ON();
    LED_KERNEL_ON();

    LED_TTY_OFF();
    LED_SWAP_OFF();
    LED_DISK_OFF();
    LED_KERNEL_OFF();

#if 0 // XXX
    extern void _etext(), _exception_base_();
    extern unsigned __data_start;

    /* Initialize STATUS register: master interrupt disable.
     * Setup interrupt vector base. */
    mips_write_c0_register(C0_STATUS, 0, ST_CU0 | ST_BEV);
    mips_write_c0_register(C0_EBASE, 1, _exception_base_);
    mips_write_c0_register(C0_STATUS, 0, ST_CU0);

    /* Set vector spacing: not used really, but must be nonzero. */
    mips_write_c0_register(C0_INTCTL, 1, 32);

    /* Clear CAUSE register: use special interrupt vector 0x200. */
    mips_write_c0_register(C0_CAUSE, 0, CA_IV);

    /* Setup memory. */
    BMXPUPBA = 512 << 10;               /* Kernel Flash memory size */
#ifdef KERNEL_EXECUTABLE_RAM
    /*
     * Set boundry for kernel executable ram on smallest
     * 2k boundry required to allow the keram segment to fit.
     * This means that there is possibly some u0area ramspace that
     * is executable, but as it is isolated from userspace this
     * should be ok, given the apparent goals of this project.
     */
    extern void _keram_start(), _keram_end();
    unsigned keram_size = (((char*)&_keram_end-(char*)&_keram_start+(2<<10))/(2<<10)*(2<<10));
    BMXDKPBA = ((32<<10)-keram_size);   /* Kernel RAM size */
    BMXDUDBA = BMXDKPBA+(keram_size);   /* Executable RAM in kernel */
#else
    BMXDKPBA = 32 << 10;                /* Kernel RAM size */
    BMXDUDBA = BMXDKPBA;                /* Zero executable RAM in kernel */
#endif
    BMXDUPBA = BMXDUDBA;                /* All user RAM is executable */

    /*
     * Setup interrupt controller.
     */
    INTCON = 0;                         /* Interrupt Control */
    IPTMR = 0;                          /* Temporal Proximity Timer */

    /* Interrupt Flag Status */
    IFS(0) = PIC32_IPC_IP0(2) | PIC32_IPC_IP1(1) |
             PIC32_IPC_IP2(1) | PIC32_IPC_IP3(1) |
             PIC32_IPC_IS0(0) | PIC32_IPC_IS1(0) |
             PIC32_IPC_IS2(0) | PIC32_IPC_IS3(0) ;
    IFS(1) = 0;
    IFS(2) = 0;

    /* Interrupt Enable Control */
    IEC(0) = 0;
    IEC(1) = 0;
    IEC(2) = 0;

    /* Interrupt Priority Control */
    unsigned ipc = PIC32_IPC_IP0(1) | PIC32_IPC_IP1(1) |
                   PIC32_IPC_IP2(1) | PIC32_IPC_IP3(1) |
                   PIC32_IPC_IS0(0) | PIC32_IPC_IS1(0) |
                   PIC32_IPC_IS2(0) | PIC32_IPC_IS3(0) ;
    IPC(0) = ipc;
    IPC(1) = ipc;
    IPC(2) = ipc;
    IPC(3) = ipc;
    IPC(4) = ipc;
    IPC(5) = ipc;
    IPC(6) = ipc;
    IPC(7) = ipc;
    IPC(8) = ipc;
    IPC(9) = ipc;
    IPC(10) = ipc;
    IPC(11) = ipc;
    IPC(12) = ipc;

    /*
     * Setup wait states.
     */
    CHECON = 2;
    BMXCONCLR = 0x40;
    CHECONSET = 0x30;

    /* Disable JTAG port, to use it for i/o. */
    DDPCON = 0;

    /* Use all B ports as digital. */
    AD1PCFG = ~0;

    /* Config register: enable kseg0 caching. */
    mips_write_c0_register(C0_CONFIG, 0,
    mips_read_c0_register(C0_CONFIG, 0) | 3);

    /* Kernel mode, interrupts disabled.  */
    mips_write_c0_register(C0_STATUS, 0, ST_CU0);

#ifdef POWER_ENABLED
    power_init();
#endif

    /* Initialize .data + .bss segments by zeroes. */
    bzero(&__data_start, KERNEL_DATA_SIZE - 96);

#if __MPLABX__
    /* Microchip C32 compiler generates a .dinit table with
     * initialization values for .data segment. */
    extern const unsigned _dinit_addr[];
    unsigned const *dinit = &_dinit_addr[0];
    for (;;) {
        char *dst = (char*) (*dinit++);
        if (dst == 0)
            break;

        unsigned nbytes = *dinit++;
        unsigned fmt = *dinit++;
        if (fmt == 0) {                 /* Clear */
            do {
                *dst++ = 0;
            } while (--nbytes > 0);
        } else {                        /* Copy */
            char *src = (char*) dinit;
            do {
                *dst++ = *src++;
            } while (--nbytes > 0);
            dinit = (unsigned*) ((unsigned) (src + 3) & ~3);
        }
    }
#else
    /* Copy the .data image from flash to ram.
     * Linker places it at the end of .text segment. */
    extern unsigned _edata;
    unsigned *src = (unsigned*) &_etext;
    unsigned *dest = &__data_start;
    unsigned *limit = &_edata;
    while (dest < limit) {
        /*printf("copy %08x from (%08x) to (%08x)\n", *src, src, dest);*/
        *dest++ = *src++;
    }

#ifdef KERNEL_EXECUTABLE_RAM
    /* Copy code that must run out of ram (due to timing restrictions)
     * from flash to the executable section of kernel ram.
     * This was added to support swap on sdram */

    extern void _ramfunc_image_begin();
    extern void _ramfunc_begin();
    extern void _ramfunc_end();

    unsigned *src1 = (unsigned*) &_ramfunc_image_begin;
    unsigned *dest1 = (unsigned*)&_ramfunc_begin;
    unsigned *limit1 = (unsigned*)&_ramfunc_end;
    /*printf("copy from (%08x) to (%08x)\n", src1, dest1);*/
    while (dest1 < limit1) {
        *dest1++ = *src1++;
    }
#endif
#endif /* __MPLABX__ */

    /*
     * Setup peripheral bus clock divisor.
     */
    unsigned osccon = OSCCON & ~PIC32_OSCCON_PBDIV_MASK;
#if BUS_DIV == 1
    osccon |= PIC32_OSCCON_PBDIV_1;
#elif BUS_DIV == 2
    osccon |= PIC32_OSCCON_PBDIV_2;
#elif BUS_DIV == 4
    osccon |= PIC32_OSCCON_PBDIV_4;
#elif BUS_DIV == 8
    osccon |= PIC32_OSCCON_PBDIV_8;
#else
#error Incorrect BUS_DIV value!
#endif
    /* Unlock access to OSCCON register */
    SYSKEY = 0;
    SYSKEY = 0xaa996655;
    SYSKEY = 0x556699aa;

    OSCCON = osccon;
#endif // XXX

    /*
     * Early setup for console devices.
     */
#if CONS_MAJOR == UART_MAJOR
    uartinit(CONS_MINOR);
#endif

    /*
     * When button 1 is pressed - boot to single user mode.
     */
    boothowto = 0;
    if (button1_pressed()) {
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
    case 0x0411:
        physmem = 192 * 1024;   /* Total 192kb RAM size. */
        printf("STM32F407xx");
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
    case 0x0434:
        physmem = 384 * 1024;   /* Total 384kb RAM size. */
        printf("STM32F469xx");
        printf(" rev ");
        switch (revid) {
        case 0x1000:
            printf("A");
            break;
        default:
            printf("unknown 0x%04x", revid);
            break;
        }
        break;
    default:
        physmem = 128 * 1024;   /* Minimum of 128kb total RAM size. */
        printf("device unknown 0x%03x", devid);
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

    /* Wait for something to happen. */
// XXX    asm volatile ("wait");

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
            udelay(40000L * iter);
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
        /* XXX Reset microcontroller */
    }
    printf("halted\n");

#ifdef HALTREBOOT
    printf("press any key to reboot...");
    cngetc();

    /* XXX Reset microcontroller */
#endif

    for (;;) {
#ifdef POWER_ENABLED
        if (howto & RB_POWEROFF)
            power_off();
#endif
// XXX        asm volatile ("wait");
    }
    /*NOTREACHED*/
}

/*
 * Microsecond delay routine for MIPS processor.
 *
 * We rely on a hardware register Count, which is increased
 * every next clock cycle, i.e. at rate CPU_KHZ/2 per millisecond.
 */
void
udelay(usec)
    u_int usec;
{
    unsigned now = mips_read_c0_register(C0_COUNT, 0);
    unsigned final = now + usec * (CPU_KHZ / 1000) / 2;

    for (;;) {
        now = mips_read_c0_register(C0_COUNT, 0);

        /* This comparison is valid only when using a signed type. */
        if ((int) (now - final) >= 0)
            break;
    }
}

/*
 * Control LEDs, installed on the board.
 */
void led_control(int mask, int on)
{
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
    if (mask & LED_TTY) {       /* Terminal i/o */
        if (on) LED_TTY_ON();
        else    LED_TTY_OFF();
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
 * There are two memory regions allowed for user: flash and RAM.
 */
int
baduaddr(addr)
    register caddr_t addr;
{
    if (addr >= (caddr_t) USER_FLASH_START &&
        addr < (caddr_t) USER_FLASH_END)
        return 0;
    if (addr >= (caddr_t) USER_DATA_START &&
        addr < (caddr_t) USER_DATA_END)
        return 0;
    return 1;
}

/*
 * Return 0 if a kernel address is valid.
 * There is only one memory region allowed for kernel: RAM.
 */
int
badkaddr(addr)
    register caddr_t addr;
{
    if (addr >= (caddr_t) KERNEL_DATA_START &&
        addr < (caddr_t) KERNEL_DATA_END)
        return 0;
    if (addr >= (caddr_t) KERNEL_FLASH_START &&
        addr < (caddr_t) KERNEL_FLASH_START + FLASH_SIZE)
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
