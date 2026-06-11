/*
 * uart.c -- ESP32-H2 UART0 polled driver for DiscoBSD Phase 1.
 *
 * No kernel headers required.  Compiles with machine/ includes only.
 *
 * UART0 default pins: TX = GPIO24, RX = GPIO23 (IO_MUX direct path,
 * same as the ROM bootloader -- no GPIO matrix configuration needed).
 */

#include <machine/esp32h2.h>
#include <machine/cpu.h>

/*
 * APB clock frequency.
 * Default configuration: CPU = 96 MHz, APB = CPU / 2 = 48 MHz.
 */
#define APB_CLK_HZ  ((unsigned long)(CONFIG_CPU_FREQ_MHZ) * 1000000UL / 2UL)

void
uart_init(unsigned int baud)
{
    unsigned long clkdiv_full;
    unsigned int  frag, clkdiv;

    /* Enable UART0 clock then release reset */
    PCR_UART0_CONF |=  PCR_UART_CLK_EN;
    PCR_UART0_CONF |=  PCR_UART_RST_EN;
    PCR_UART0_CONF &= ~PCR_UART_RST_EN;

    /*
     * Baud-rate divisor (TRM §27.3.1):
     *   clkdiv_full       = (APB_CLK_HZ * 16) / baud
     *   integer  part     = clkdiv_full >> 4       -> bits [19:0]
     *   fractional part   = clkdiv_full & 0xF      -> bits [23:20]
     */
    clkdiv_full = (APB_CLK_HZ / baud) * 16UL
                + (APB_CLK_HZ % baud) * 16UL / baud;
    clkdiv = (unsigned int)(clkdiv_full >> 4)  & UART_CLKDIV_INT_MASK;
    frag   = (unsigned int)(clkdiv_full & 0xFu);
    UART0_CLKDIV = clkdiv | (frag << UART_CLKDIV_FRAG_SHIFT);

    /*
     * CONF0: 8 data bits (BIT_NUM = 3, field [3:2]),
     *        1 stop bit  (STOP_BIT_NUM = 1, field [5:4]),
     *        no parity, no flow control.
     */
    UART0_CONF0 = (3u << 2) | (1u << 4);

    /* Flush FIFOs */
    UART0_CONF0 |=  (1u << 23) | (1u << 22);   /* RXFIFO_RST | TXFIFO_RST */
    UART0_CONF0 &= ~((1u << 23) | (1u << 22));
}

/* Send one byte; spin-wait while TX FIFO is full. */
void
uart_putc(int c)
{
    while (((UART0_STATUS & UART_TXFIFO_CNT_MASK) >> UART_TXFIFO_CNT_SHIFT)
           >= (UART_TXFIFO_DEPTH - 1))
        /* spin */ ;

    UART0_FIFO = (unsigned int)(unsigned char)c;

    if (c == '\n')
        uart_putc('\r');    /* always send CR after LF */
}

/* Return next byte from RX FIFO, or -1 if empty (non-blocking). */
int
uart_getc(void)
{
    if ((UART0_STATUS & UART_RXFIFO_CNT_MASK) == 0)
        return -1;
    return (int)(UART0_FIFO & 0xFFu);
}

/* Write a NUL-terminated string. */
void
uart_puts(const char *s)
{
    while (*s)
        uart_putc((unsigned char)*s++);
}
