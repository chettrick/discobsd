/*
 * machdep.c -- ESP32-H2 machine-dependent code for DiscoBSD Phase 1.
 *
 * Provides:
 *   timer_init()  -- arm the RISC-V mtime periodic tick at HZ
 *   trap()        -- called from locore.S on every trap / interrupt
 */

#include <machine/cpu.h>
#include <machine/esp32h2.h>

/* Provided by main.c */
extern void hardclock(struct reg *frame);
extern void panic(const char *msg) __attribute__((noreturn));

/* Provided by uart.c */
extern void uart_putc(int c);
extern void uart_puts(const char *s);

/* ------------------------------------------------------------------ */
/*  T i m e r                                                          */
/* ------------------------------------------------------------------ */

/*
 * The ESP32-H2 mtime counter increments at 16 MHz (XTAL / 2).
 * Adjust MTIME_FREQ if your clock tree differs.
 */
#define MTIME_FREQ  16000000u
#define TICK_CYCLES (MTIME_FREQ / HZ)

static unsigned long long
read_mtime(void)
{
    unsigned int hi, lo;
    do {
        hi = CLINT_MTIMEH;
        lo = CLINT_MTIMEL;
    } while (hi != CLINT_MTIMEH);
    return ((unsigned long long)hi << 32) | (unsigned long long)lo;
}

static void
arm_next_tick(void)
{
    unsigned long long next = read_mtime() + TICK_CYCLES;
    /* Write high word to MAX first to prevent a spurious early match */
    CLINT_MTIMECMPH = 0xFFFFFFFFu;
    CLINT_MTIMECMPL = (unsigned int)(next & 0xFFFFFFFFu);
    CLINT_MTIMECMPH = (unsigned int)(next >> 32);
}

void
timer_init(void)
{
    /* Disarm completely before re-arming */
    CLINT_MTIMECMPH = 0xFFFFFFFFu;
    CLINT_MTIMECMPL = 0xFFFFFFFFu;

    arm_next_tick();

    /* Enable machine-timer interrupt in mie (MTIE = bit 7) */
    csr_set(mie, MTIE_BIT);
}

/* ------------------------------------------------------------------ */
/*  T r a p   d i s p a t c h e r                                    */
/* ------------------------------------------------------------------ */

/*
 * Print an unsigned int in hex without using printf.
 */
static void
print_hex(unsigned int v)
{
    int i;
    uart_puts("0x");
    for (i = 28; i >= 0; i -= 4)
        uart_putc("0123456789abcdef"[(v >> i) & 0xF]);
}

/*
 * trap() -- called from _trap_entry in locore.S.
 *
 * mcause bit 31 = 1  -> interrupt,  0 -> synchronous exception.
 * Interrupt cause 7  -> machine timer (the only one we use in Phase 1).
 */
void
trap(struct reg *frame)
{
    unsigned int cause   = frame->r_mcause;
    unsigned int is_intr = cause >> 31;
    unsigned int code    = cause & 0x7FFFFFFFu;

    if (is_intr) {
        if (code == 7) {            /* machine timer interrupt */
            arm_next_tick();
            hardclock(frame);
        } else {
            uart_puts("\r\n[unexpected interrupt, code=");
            print_hex(code);
            uart_puts("]\r\n");
        }
    } else {
        /* Synchronous exception -- print and panic */
        uart_puts("\r\n*** EXCEPTION ***\r\n");
        uart_puts("  mepc   = "); print_hex(frame->r_mepc);    uart_puts("\r\n");
        uart_puts("  mcause = "); print_hex(frame->r_mcause);  uart_puts("\r\n");
        uart_puts("  mtval  = "); print_hex(frame->r_mtval);   uart_puts("\r\n");
        panic("unhandled exception");
    }
}
