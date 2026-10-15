/*
 * main.c -- DiscoBSD/esp32h2 Phase 1 entry point.
 *
 * This is a self-contained replacement for the full DiscoBSD init_main.c.
 * It needs no SD card, no filesystem, no userland.
 *
 * After flashing you should see this on the serial port at 115200 baud:
 *
 *   ================================================
 *    DiscoBSD/esp32h2  Phase 1 -- UART console test
 *   ================================================
 *    CPU : ESP32-H2 RISC-V RV32IMAC @ 96 MHz
 *    UART: UART0 TX=GPIO24 RX=GPIO23, 115200 8N1
 *    HZ  : 100 ticks/sec (mtime @ 16 MHz)
 *    SRAM: 320 KB HP SRAM @ 0x40800000
 *   ------------------------------------------------
 *    Timer started.  Press Enter for uptime, ? for help.
 *   ================================================
 *   >
 *
 * Interactive commands (no Enter needed except for uptime):
 *   Enter  -- print tick count and uptime in seconds
 *   ?      -- print help
 *   r      -- reset the chip
 *   (any other character is echoed back)
 */

#include <machine/cpu.h>
#include <machine/esp32h2.h>

/* ------------------------------------------------------------------ */
/*  F u n c t i o n   p r o t o t y p e s                            */
/* ------------------------------------------------------------------ */

/* uart.c */
void uart_init(unsigned int baud);
void uart_putc(int c);
int  uart_getc(void);
void uart_puts(const char *s);

/* machdep.c */
void timer_init(void);

/* ------------------------------------------------------------------ */
/*  T i c k   c o u n t e r                                           */
/* ------------------------------------------------------------------ */

volatile unsigned long ticks;   /* incremented by hardclock() at HZ   */

/*
 * hardclock() -- called from machdep.c trap() on every timer IRQ.
 */
void
hardclock(struct reg *frame)
{
    (void)frame;
    ticks++;
}

/* ------------------------------------------------------------------ */
/*  p a n i c ()                                                       */
/* ------------------------------------------------------------------ */

void
panic(const char *msg)
{
    csr_clear(mstatus, MIE_BIT);   /* stop interrupts */
    uart_puts("\r\n*** PANIC: ");
    uart_puts(msg);
    uart_puts(" ***\r\n");
    for (;;)
        __asm__ volatile("wfi");
}

/* ------------------------------------------------------------------ */
/*  T i n y   p r i n t   h e l p e r s                              */
/* ------------------------------------------------------------------ */

static void
print_dec(unsigned long v)
{
    char buf[12];
    int  i = 0;
    if (v == 0) { uart_putc('0'); return; }
    while (v) { buf[i++] = '0' + (int)(v % 10); v /= 10; }
    while (i--) uart_putc(buf[i]);
}

/* ------------------------------------------------------------------ */
/*  C o n s o l e   l o o p                                           */
/* ------------------------------------------------------------------ */

static void
print_status(void)
{
    unsigned long t = ticks;
    uart_puts("\r\n  ticks=");
    print_dec(t);
    uart_puts("  uptime=");
    print_dec(t / HZ);
    uart_puts("s\r\n> ");
}

static void
console_loop(void)
{
    int c;
    uart_puts("> ");
    for (;;) {
        c = uart_getc();
        if (c < 0) {
            __asm__ volatile("wfi");    /* sleep until next interrupt */
            continue;
        }
        switch (c) {
        case '\r':
        case '\n':
            print_status();
            break;
        case '?':
            uart_puts("\r\n  Enter -- uptime\r\n");
            uart_puts("  r     -- reset\r\n");
            uart_puts("  ?     -- help\r\n");
            uart_puts("  other -- echo\r\n> ");
            break;
        case 'r':
            uart_puts("\r\nResetting...\r\n");
            cpu_reset();
            break;
        default:
            uart_putc(c);
            break;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  m a i n ()                                                         */
/* ------------------------------------------------------------------ */

void
main(void)
{
    /* 1. Bring up UART before anything else */
    uart_init(115200);

    /* 2. Banner */
    uart_puts("\r\n================================================\r\n");
    uart_puts(" DiscoBSD/esp32h2  Phase 1 -- UART console test \r\n");
    uart_puts("================================================\r\n");
    uart_puts(" CPU : ESP32-H2 RISC-V RV32IMAC @ ");
    print_dec(CONFIG_CPU_FREQ_MHZ);
    uart_puts(" MHz\r\n");
    uart_puts(" UART: UART0 TX=GPIO24 RX=GPIO23, 115200 8N1\r\n");
    uart_puts(" HZ  : ");
    print_dec(HZ);
    uart_puts(" ticks/sec (mtime @ 16 MHz)\r\n");
    uart_puts(" SRAM: 320 KB HP SRAM @ 0x40800000\r\n");
    uart_puts("------------------------------------------------\r\n");

    /* 3. Start the machine timer */
    timer_init();

    /* 4. Enable global interrupts */
    csr_set(mstatus, MIE_BIT);

    uart_puts(" Timer started.  Press Enter for uptime, ? for help.\r\n");
    uart_puts("================================================\r\n");

    /* 5. Interactive loop -- never returns */
    console_loop();
}
