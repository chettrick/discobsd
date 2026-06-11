/*
 * cpu.h -- ESP32-H2 RISC-V CPU definitions for DiscoBSD Phase 1.
 */
#ifndef _MACHINE_CPU_H_
#define _MACHINE_CPU_H_

/* ------------------------------------------------------------------ */
/* Trap frame -- saved by locore.S on every trap/interrupt.           */
/* Offsets must match locore.S exactly.                               */
/* 4 CSR words + 31 GP register words = 35 words = 140 bytes.        */
/* ------------------------------------------------------------------ */
struct reg {
    unsigned int r_mepc;        /*  0 */
    unsigned int r_mstatus;     /*  4 */
    unsigned int r_mcause;      /*  8 */
    unsigned int r_mtval;       /* 12 */
    unsigned int r_ra;          /* 16  x1  */
    unsigned int r_sp;          /* 20  x2  */
    unsigned int r_gp;          /* 24  x3  */
    unsigned int r_tp;          /* 28  x4  */
    unsigned int r_t0;          /* 32  x5  */
    unsigned int r_t1;          /* 36  x6  */
    unsigned int r_t2;          /* 40  x7  */
    unsigned int r_s0;          /* 44  x8  */
    unsigned int r_s1;          /* 48  x9  */
    unsigned int r_a0;          /* 52  x10 */
    unsigned int r_a1;          /* 56  x11 */
    unsigned int r_a2;          /* 60  x12 */
    unsigned int r_a3;          /* 64  x13 */
    unsigned int r_a4;          /* 68  x14 */
    unsigned int r_a5;          /* 72  x15 */
    unsigned int r_a6;          /* 76  x16 */
    unsigned int r_a7;          /* 80  x17 */
    unsigned int r_s2;          /* 84  x18 */
    unsigned int r_s3;          /* 88  x19 */
    unsigned int r_s4;          /* 92  x20 */
    unsigned int r_s5;          /* 96  x21 */
    unsigned int r_s6;          /* 100 x22 */
    unsigned int r_s7;          /* 104 x23 */
    unsigned int r_s8;          /* 108 x24 */
    unsigned int r_s9;          /* 112 x25 */
    unsigned int r_s10;         /* 116 x26 */
    unsigned int r_s11;         /* 120 x27 */
    unsigned int r_t3;          /* 124 x28 */
    unsigned int r_t4;          /* 128 x29 */
    unsigned int r_t5;          /* 132 x30 */
    unsigned int r_t6;          /* 136 x31 */
};

/* ------------------------------------------------------------------ */
/* CSR access macros                                                   */
/* ------------------------------------------------------------------ */
#define csr_read(csr) ({                            \
    unsigned int __v;                               \
    __asm__ volatile("csrr %0, " #csr : "=r"(__v));\
    __v; })

#define csr_write(csr, val) \
    __asm__ volatile("csrw " #csr ", %0" :: "rK"(val))

#define csr_set(csr, bits) \
    __asm__ volatile("csrs " #csr ", %0" :: "rK"(bits))

#define csr_clear(csr, bits) \
    __asm__ volatile("csrc " #csr ", %0" :: "rK"(bits))

/* ------------------------------------------------------------------ */
/* Interrupt control                                                   */
/* ------------------------------------------------------------------ */
#define MIE_BIT     (1u << 3)   /* global interrupt enable in mstatus */
#define MTIE_BIT    (1u << 7)   /* machine timer interrupt enable in mie */

#define splhigh()   ({ unsigned int _s = csr_read(mstatus); \
                       csr_clear(mstatus, MIE_BIT); _s; })
#define splx(s)     do { if ((s) & MIE_BIT) csr_set(mstatus, MIE_BIT); \
                         else csr_clear(mstatus, MIE_BIT); } while(0)
#define spl0()      csr_set(mstatus, MIE_BIT)

/* ------------------------------------------------------------------ */
/* Misc                                                                */
/* ------------------------------------------------------------------ */
#ifndef HZ
#define HZ  100
#endif

#ifndef CONFIG_CPU_FREQ_MHZ
#define CONFIG_CPU_FREQ_MHZ 96
#endif

#define DELAY_LOOPS_PER_US  10

extern void cpu_reset(void) __attribute__((noreturn));
extern void cpu_halt(void)  __attribute__((noreturn));

#endif /* _MACHINE_CPU_H_ */
