/*
 * Copyright (c) 2023 Christopher Hettrick <chris@structfoo.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef	_MACHINE_FAULT_H_
#define	_MACHINE_FAULT_H_

/*
 * Fault types ARMv7-M
 *
 * Reference: "ARMv7-M Architecture Reference Manual"
 *	DDI 0403E.d
 *	June 29, 2018
 *	Sections B3.2.14 - B3.2.18
 */

/*
 * HardFault Status Register bit assignments
 * HFSR
 *
 * Bit position  |31                                          0|
 * HFSR register |          HardFault Status Register          |
 * Fault bits    | 11xx xxxx  xxxx xxxx   xxxx xxxx  xxxx xx1x |
 */

#define	FAULTED(reg, bit, mask)	((((reg) & (mask)) >> (bit)) & 0x1)

#define	T_ALL_HARDF_MASK	0xc0000002	/* Fault bits of HFSR */
#define	HARD_FAULT(reg, bit)	FAULTED(reg, bit, T_ALL_HARDF_MASK)

/* HardFault */
#define	T_HF_VECTTBL		 1	/* vector read error */
#define	T_HF_FORCED		30	/* fault escalation */
#define	T_HF_DEBUGEVT		31	/* breakpoint escalation */

/*
 * Configurable Fault Status Register bit assignments
 * CFSR = UFSR  UsageFault Status Register
 *	| BFSR  BusFault Status Register
 *	| MMFSR MemManage Fault Status Register
 *
 * Bit position  |31                 16|15        8|7         0|
 * CFSR register |      UsageFault     | BusFault  | MemManage |
 * Fault bits    | xxxx xx11 xxxx 1111 | xx11 1111 | xx11 1x11 |
 */

#define	T_CFSR_FAULTS_MASK	0x030f3f3b	/* Fault bits of CFSR */
#define	CFSR_FAULT(reg, bit)	FAULTED(reg, bit, T_CFSR_FAULTS_MASK)

/* MemManage Fault */
#define	T_MM_IACCVIOL		 0	/* instruction access */
#define	T_MM_DACCVIOL		 1	/* data access */
#define	T_MM_MUNSTKERR		 3	/* exception return unstacking */
#define	T_MM_MSTKERR		 4	/* exception entry stacking */
#define	T_MM_MLSPERR		 5	/* fp lazy state preservation */

/* BusFault */
#define	T_BF_IBUSERR		 8	/* precise instruction prefetch */
#define	T_BF_PRECISERR		 9	/* precise data bus access */
#define	T_BF_IMPRECISERR	10	/* imprecise data bus access */
#define	T_BF_UNSTKERR		11	/* exception return unstacking */
#define	T_BF_STKERR		12	/* exception entry stacking */
#define	T_BF_LSPERR		13	/* fp lazy state preservation */

/* UsageFault */
#define	T_UF_UNDEFINSTR		16	/* undefined instruction */
#define	T_UF_INVSTATE		17	/* invalid execution state */
#define	T_UF_INVPC		18	/* exception return integrity */
#define	T_UF_NOCP		19	/* no coprocessor access */
#define	T_UF_UNALIGNED		24	/* unaligned load or store */
#define	T_UF_DIVBYZERO		25	/* divide by zero */

#define	MMFARVALID_BIT		0x0080	/* CFSR bit[7] MMFAR contents valid */
#define	BFARVALID_BIT		0x8000	/* CFSR bit[15] BFAR contents valid */

#define	T_HF			0x1	/* Fault type HardFault */
#define	T_MM			0x2	/* Fault type MemManage Fault */
#define	T_BF			0x4	/* Fault type BusFault */
#define	T_UF			0x8	/* Fault type UsageFault */

#define	MM_FAULT_ENABLE		SCB_SHCSR_MEMFAULTENA_Msk
#define	BF_FAULT_ENABLE		SCB_SHCSR_BUSFAULTENA_Msk
#define	UF_FAULT_ENABLE		SCB_SHCSR_USGFAULTENA_Msk

static inline void
arm_disable_fault(uint32_t fault)
{
	SCB->SHCSR &= ~fault;
}

static inline void
arm_enable_fault(uint32_t fault)
{
	SCB->SHCSR |= fault;
}

static inline void
arm_clear_fault(int bit, int type)
{
	/* Writing a 1 to the fault indicator bit will clear it. */
	switch (type) {
	/* Fault type is HardFault. */
	case T_HF:
		SCB->HFSR |= 1UL << bit;
		break;
	/* Fault type is MemManage Fault, BusFault, or UsageFault. */
	case T_MM:
	case T_BF:
	case T_UF:
		SCB->CFSR |= 1UL << bit;
		break;
	/* Unknown fault type. */
	default:
		printf("not clearing unknown fault type %d\n", type);
		break;
	}
}

#endif	/* !_MACHINE_FAULT_H_ */
