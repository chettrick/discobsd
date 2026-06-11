/*
 * locore.S -- ESP32-H2 RISC-V RV32IMAC low-level startup for DiscoBSD.
 *
 * The ESP32-H2 ROM bootloader loads our image into SRAM at 0x40800000
 * and jumps to _start.  We do NOT need to copy .data from flash because
 * the linker script places LMA == VMA (everything already in SRAM).
 *
 * Trap frame layout on stack (must match struct reg in cpu.h):
 *   sp+  0  mepc
 *   sp+  4  mstatus
 *   sp+  8  mcause
 *   sp+ 12  mtval
 *   sp+ 16  x1  (ra)
 *   sp+ 20  x2  (original sp = sp+140)
 *   sp+ 24  x3  (gp)
 *   sp+ 28  x4  (tp)
 *   sp+ 32  x5  (t0)
 *   sp+ 36  x6  (t1)
 *   sp+ 40  x7  (t2)
 *   sp+ 44  x8  (s0)
 *   sp+ 48  x9  (s1)
 *   sp+ 52  x10 (a0)
 *   sp+ 56  x11 (a1)
 *   sp+ 60  x12 (a2)
 *   sp+ 64  x13 (a3)
 *   sp+ 68  x14 (a4)
 *   sp+ 72  x15 (a5)
 *   sp+ 76  x16 (a6)
 *   sp+ 80  x17 (a7)
 *   sp+ 84  x18 (s2)
 *   sp+ 88  x19 (s3)
 *   sp+ 92  x20 (s4)
 *   sp+ 96  x21 (s5)
 *   sp+100  x22 (s6)
 *   sp+104  x23 (s7)
 *   sp+108  x24 (s8)
 *   sp+112  x25 (s9)
 *   sp+116  x26 (s10)
 *   sp+120  x27 (s11)
 *   sp+124  x28 (t3)
 *   sp+128  x29 (t4)
 *   sp+132  x30 (t5)
 *   sp+136  x31 (t6)
 *   Total: 140 bytes
 */

    .section .text.init,"ax",@progbits
    .globl  _start
    .type   _start, @function
_start:
    /*
     * 1. Set global pointer (enables GP-relative relaxation).
     */
    .option push
    .option norelax
    la      gp, __global_pointer$
    .option pop

    /*
     * 2. Set stack pointer to top of kernel stack (_estack from ld script).
     */
    la      sp, _estack

    /*
     * 3. Install trap vector (direct mode: all traps -> _trap_entry).
     */
    la      t0, _trap_entry
    csrw    mtvec, t0

    /*
     * 4. Disable global interrupts while initialising.
     */
    csrci   mstatus, 8          /* clear MIE (bit 3) */

    /*
     * 5. Disable TIMG0 watchdog.
     *    Write-protect key -> WDTWPROTECT (unlock), then zero WDTCONFIG0.
     */
    li      t0, 0x6001F000
    li      t1, 0x50D83AA1
    sw      t1, 0x064(t0)       /* TIMG0_WDTWPROTECT = key  (unlock) */
    sw      zero, 0x048(t0)     /* TIMG0_WDTCONFIG0  = 0    (disable) */
    sw      zero, 0x064(t0)     /* TIMG0_WDTWPROTECT = 0    (re-lock) */

    /*
     * 6. Disable RTC Super WDT.
     */
    li      t0, 0x60008000
    li      t1, 0x8F1D312A
    sw      t1, 0x0B0(t0)       /* RTC_SWD_WPROTECT = key   (unlock) */
    li      t1, (1 << 31)       /* AUTO_FEED_EN bit disables SWD */
    sw      t1, 0x0AC(t0)       /* RTC_SWD_CONF */
    sw      zero, 0x0B0(t0)     /* re-lock */

    /*
     * 7. Zero BSS.
     */
    la      t0, __bss_start
    la      t1, __bss_end
    bge     t0, t1, .Lbss_done
.Lbss_loop:
    sw      zero, 0(t0)
    addi    t0, t0, 4
    blt     t0, t1, .Lbss_loop
.Lbss_done:

    /*
     * 8. No .data copy needed -- LMA == VMA (everything already in SRAM).
     */

    /*
     * 9. Call main().  Never returns.
     */
    call    main

    j       cpu_halt


/* ================================================================== */
/*  T R A P   V E C T O R                                             */
/* ================================================================== */

    .section .text,"ax",@progbits
    .balign 4
    .globl  _trap_entry
    .type   _trap_entry, @function
_trap_entry:
    addi    sp, sp, -140

    /* Save CSRs */
    csrr    t0, mepc
    sw      t0,   0(sp)
    csrr    t0, mstatus
    sw      t0,   4(sp)
    csrr    t0, mcause
    sw      t0,   8(sp)
    csrr    t0, mtval
    sw      t0,  12(sp)

    /* Save all GP registers x1..x31 */
    sw      ra,  16(sp)
    addi    t0, sp, 140
    sw      t0,  20(sp)         /* original sp */
    sw      gp,  24(sp)
    sw      tp,  28(sp)
    sw      t0,  32(sp)
    sw      t1,  36(sp)
    sw      t2,  40(sp)
    sw      s0,  44(sp)
    sw      s1,  48(sp)
    sw      a0,  52(sp)
    sw      a1,  56(sp)
    sw      a2,  60(sp)
    sw      a3,  64(sp)
    sw      a4,  68(sp)
    sw      a5,  72(sp)
    sw      a6,  76(sp)
    sw      a7,  80(sp)
    sw      s2,  84(sp)
    sw      s3,  88(sp)
    sw      s4,  92(sp)
    sw      s5,  96(sp)
    sw      s6, 100(sp)
    sw      s7, 104(sp)
    sw      s8, 108(sp)
    sw      s9, 112(sp)
    sw      s10,116(sp)
    sw      s11,120(sp)
    sw      t3, 124(sp)
    sw      t4, 128(sp)
    sw      t5, 132(sp)
    sw      t6, 136(sp)

    mv      a0, sp
    call    trap

    /* Restore CSRs */
    lw      t0,   0(sp)
    csrw    mepc, t0
    lw      t0,   4(sp)
    csrw    mstatus, t0

    /* Restore GP registers */
    lw      ra,  16(sp)
    lw      gp,  24(sp)
    lw      tp,  28(sp)
    lw      t0,  32(sp)
    lw      t1,  36(sp)
    lw      t2,  40(sp)
    lw      s0,  44(sp)
    lw      s1,  48(sp)
    lw      a0,  52(sp)
    lw      a1,  56(sp)
    lw      a2,  60(sp)
    lw      a3,  64(sp)
    lw      a4,  68(sp)
    lw      a5,  72(sp)
    lw      a6,  76(sp)
    lw      a7,  80(sp)
    lw      s2,  84(sp)
    lw      s3,  88(sp)
    lw      s4,  92(sp)
    lw      s5,  96(sp)
    lw      s6, 100(sp)
    lw      s7, 104(sp)
    lw      s8, 108(sp)
    lw      s9, 112(sp)
    lw      s10,116(sp)
    lw      s11,120(sp)
    lw      t3, 124(sp)
    lw      t4, 128(sp)
    lw      t5, 132(sp)
    lw      t6, 136(sp)

    addi    sp, sp, 140
    mret


/* ================================================================== */
/*  U T I L I T Y                                                     */
/* ================================================================== */

    .globl  cpu_halt
    .type   cpu_halt, @function
cpu_halt:
    csrci   mstatus, 8
.Lhalt:
    wfi
    j       .Lhalt


    .globl  cpu_reset
    .type   cpu_reset, @function
cpu_reset:
    li      t0, 0x6001F000
    li      t1, 0x50D83AA1
    sw      t1, 0x064(t0)
    li      t1, (1 << 31)
    sw      t1, 0x048(t0)
    sw      zero, 0x060(t0)
.Lreset_wait:
    wfi
    j       .Lreset_wait
