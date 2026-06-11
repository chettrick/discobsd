/*
 * esp32h2.h -- ESP32-H2 peripheral register definitions.
 * All addresses from ESP32-H2 Technical Reference Manual v1.1.
 */
#ifndef _MACHINE_ESP32H2_H_
#define _MACHINE_ESP32H2_H_

#define MMIO32(addr)    (*(volatile unsigned int *)(addr))

/* ------------------------------------------------------------------ */
/* SRAM                                                                */
/* ------------------------------------------------------------------ */
#define HP_SRAM_BASE    0x40800000u     /* 320 KB HP SRAM start        */
#define HP_SRAM_SIZE    (320 * 1024)

/* ------------------------------------------------------------------ */
/* RISC-V CLINT -- machine timer (mtime / mtimecmp)                   */
/* TRM §1.7 -- register base 0x6000_0000                              */
/* ------------------------------------------------------------------ */
#define CLINT_BASE          0x60000000u
#define CLINT_MSIP          MMIO32(CLINT_BASE + 0x000)
#define CLINT_MTIMECMPL     MMIO32(CLINT_BASE + 0x004)
#define CLINT_MTIMECMPH     MMIO32(CLINT_BASE + 0x008)
#define CLINT_MTIMEL        MMIO32(CLINT_BASE + 0x00C)
#define CLINT_MTIMEH        MMIO32(CLINT_BASE + 0x010)

/* ------------------------------------------------------------------ */
/* UART0  (TRM §27)                                                    */
/* Base address 0x6000_0000 -- same as CLINT on some docs; on         */
/* ESP32-H2 UART0 peripheral base is 0x6000_0000 + 0x0000 per TRM    */
/* Table 4-2.  CLINT registers are CSR-mapped, not MMIO.              */
/* ------------------------------------------------------------------ */
#define UART0_BASE          0x60000000u
#define UART1_BASE          0x60010000u

/* Register offsets */
#define UART_FIFO_OFF       0x000   /* read: RX data, write: TX data   */
#define UART_INT_RAW_OFF    0x004
#define UART_INT_ST_OFF     0x008
#define UART_INT_ENA_OFF    0x00C
#define UART_INT_CLR_OFF    0x010
#define UART_CLKDIV_OFF     0x014
#define UART_STATUS_OFF     0x01C
#define UART_CONF0_OFF      0x020
#define UART_CONF1_OFF      0x024

/* Accessors for UART0 */
#define UART0_FIFO      MMIO32(UART0_BASE + UART_FIFO_OFF)
#define UART0_INT_CLR   MMIO32(UART0_BASE + UART_INT_CLR_OFF)
#define UART0_CLKDIV    MMIO32(UART0_BASE + UART_CLKDIV_OFF)
#define UART0_STATUS    MMIO32(UART0_BASE + UART_STATUS_OFF)
#define UART0_CONF0     MMIO32(UART0_BASE + UART_CONF0_OFF)
#define UART0_CONF1     MMIO32(UART0_BASE + UART_CONF1_OFF)

/* STATUS register fields */
#define UART_TXFIFO_CNT_MASK    0x03FF0000u
#define UART_TXFIFO_CNT_SHIFT   16
#define UART_RXFIFO_CNT_MASK    0x000003FFu
#define UART_RXFIFO_CNT_SHIFT   0
#define UART_TXFIFO_DEPTH       128

/* CLKDIV register fields */
#define UART_CLKDIV_INT_MASK    0x000FFFFFu
#define UART_CLKDIV_FRAG_SHIFT  20

/* ------------------------------------------------------------------ */
/* PCR -- peripheral clock and reset  (TRM §7, base 0x6000_E000)     */
/* ------------------------------------------------------------------ */
#define PCR_BASE        0x6000E000u
#define PCR_UART0_CONF  MMIO32(PCR_BASE + 0x024)
#define PCR_UART1_CONF  MMIO32(PCR_BASE + 0x028)
#define PCR_UART_RST_EN     (1u << 1)
#define PCR_UART_CLK_EN     (1u << 0)

/* ------------------------------------------------------------------ */
/* Watchdog timers -- must be disabled at startup                      */
/* TIMG0 base 0x6001_F000  (TRM §13)                                  */
/* ------------------------------------------------------------------ */
#define TIMG0_BASE          0x6001F000u
#define TIMG0_WDTCONFIG0    MMIO32(TIMG0_BASE + 0x048)
#define TIMG0_WDTFEED       MMIO32(TIMG0_BASE + 0x060)
#define TIMG0_WDTWPROTECT   MMIO32(TIMG0_BASE + 0x064)
#define TIMG_WDT_WKEY       0x50D83AA1u

/* RTC / LP WDT (Super WDT)  base 0x6000_8000 */
#define RTC_CNTL_BASE       0x60008000u
#define RTC_SWD_CONF        MMIO32(RTC_CNTL_BASE + 0x0AC)
#define RTC_SWD_WPROTECT    MMIO32(RTC_CNTL_BASE + 0x0B0)
#define RTC_SWD_WKEY        0x8F1D312Au
#define RTC_SWD_AUTO_FEED_EN (1u << 31)

#endif /* _MACHINE_ESP32H2_H_ */
