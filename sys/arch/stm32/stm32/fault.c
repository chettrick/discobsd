/*
 * Copyright (c) 2022, 2023 Christopher Hettrick <chris@structfoo.com>
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

/*
 * ARMv7-M Fault Handling
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/vm.h>

#include <machine/fault.h>
#include <machine/frame.h>

/*
 * Fault descriptions of Configurable Fault Status Register bits.
 */
const char * const fault_type[] = {
	"instruction access",			/*  0 T_MM_IACCVIOL */
	"data access",				/*  1 T_MM_DACCVIOL */
	"reserved",
	"exception return unstacking",		/*  3 T_MM_MUNSTKERR */
	"exception entry stacking",		/*  4 T_MM_MSTKERR */
	"fp lazy state preservation",		/*  5 T_MM_MLSPERR */
	"reserved",
	"reserved",
	"precise instruction prefetch",		/*  8 T_BF_IBUSERR */
	"precise data bus access",		/*  9 T_BF_PRECISERR */
	"imprecise data bus access",		/* 10 T_BF_IMPRECISERR */
	"exception return unstacking",		/* 11 T_BF_UNSTKERR */
	"exception entry stacking",		/* 12 T_BF_STKERR */
	"fp lazy state preservation",		/* 13 T_BF_LSPERR */
	"reserved",
	"reserved",
	"undefined instruction",		/* 16 T_UF_UNDEFINSTR */
	"invalid execution state",		/* 17 T_UF_INVSTATE */
	"exception return integrity",		/* 18 T_UF_INVPC */
	"no coprocessor access",		/* 19 T_UF_NOCP */
	"reserved",
	"reserved",
	"reserved",
	"reserved",
	"unaligned load or store",		/* 24 T_UF_UNALIGNED */
	"divide by zero",			/* 25 T_UF_DIVBYZERO */
};

const int fault_types = sizeof(fault_type) / sizeof(fault_type[0]);

/*
 * Bus error received during vector fetch.
 * Escalation of configurable fault exceptions.
 * Execution of breakpoint instruction (BKPT) with a debugger attached
 * (halt debugging not enabled) and debug monitor exception not enabled.
 */
void
HardFault_Handler(void)
{
__asm volatile (
"	.syntax	unified		\n\t"
"	.thumb			\n\t"

"	mov	r2, #(0x01)	\n\t"	/* Fault type is HardFault T_HF. */
"	mov	r1, lr		\n\t"	/* Value of lr when fault occurred. */
"	tst	lr, #0x4	\n\t"	/* Test bit 2 (SPSEL) of EXC_RETURN. */
"	ite	eq		\n\t"	/* Came from user or kernel mode? */
"	mrseq	r0, MSP		\n\t"	/* Kernel mode; fault frame on MSP. */
"	mrsne	r0, PSP		\n\t"	/* User mode; fault frame on PSP. */
"	b	arm_fault	\n\t"	/* Call arm_fault(frame, lr, type); */
);
}

/*
 * Violation of access rules defined by MPU configuration.
 * Attempt to execute program code in execute never (XN) region.
 */
void
MemManage_Handler(void)
{
__asm volatile (
"	.syntax	unified		\n\t"
"	.thumb			\n\t"

"	mov	r2, #(0x02)	\n\t"	/* Fault type is MemManage T_MM. */
"	mov	r1, lr		\n\t"	/* Value of lr when fault occurred. */
"	tst	lr, #0x4	\n\t"	/* Test bit 2 (SPSEL) of EXC_RETURN. */
"	ite	eq		\n\t"	/* Came from user or kernel mode? */
"	mrseq	r0, MSP		\n\t"	/* Kernel mode; fault frame on MSP. */
"	mrsne	r0, PSP		\n\t"	/* User mode; fault frame on PSP. */
"	b	arm_fault	\n\t"	/* Call arm_fault(frame, lr, type); */
);
}

/*
 * Error responses received from the processor bus interface
 * during a memory access, for example:
 *   instruction fetch or read, also called prefetch abort;
 *   data read or write, also called data abort.
 * Bus fault during stacking/unstacking of exception handling sequence.
 *
 * Attempt to access an invalid memory location.
 * Unprivileged access to the Private Peripheral Bus (PPB) that
 * violates the default memory access permission.
 */
void
BusFault_Handler(void)
{
__asm volatile (
"	.syntax	unified		\n\t"
"	.thumb			\n\t"

"	mov	r2, #(0x04)	\n\t"	/* Fault type is BusFault T_BF. */
"	mov	r1, lr		\n\t"	/* Value of lr when fault occurred. */
"	tst	lr, #0x4	\n\t"	/* Test bit 2 (SPSEL) of EXC_RETURN. */
"	ite	eq		\n\t"	/* Came from user or kernel mode? */
"	mrseq	r0, MSP		\n\t"	/* Kernel mode; fault frame on MSP. */
"	mrsne	r0, PSP		\n\t"	/* User mode; fault frame on PSP. */
"	b	arm_fault	\n\t"	/* Call arm_fault(frame, lr, type); */
);
}

/*
 * Execution of undefined instruction or co-processor instruction.
 * Trying to switch to ARM state (only Thumb ISA is supported).
 * Invalid EXC_RETURN code during exception return sequence.
 * Unaligned memory access with multiple load or store instructions.
 * Execution of SVC when priority level of SVC is same or lower
 * than current level.
 * Exception return with Interrupt-Continuable Instruction (ICI)
 * bits in unstacked xPSR, but instruction being executed after
 * exception return is not a multiple-load/store instruction.
 *
 * If Configuration Control Register (CCR) is set up:
 *   Divide by zero;
 *   All unaligned memory accesses.
 */
void
UsageFault_Handler(void)
{
__asm volatile (
"	.syntax	unified		\n\t"
"	.thumb			\n\t"

"	mov	r2, #(0x08)	\n\t"	/* Fault type is UsageFault T_UF. */
"	mov	r1, lr		\n\t"	/* Value of lr when fault occurred. */
"	tst	lr, #0x4	\n\t"	/* Test bit 2 (SPSEL) of EXC_RETURN. */
"	ite	eq		\n\t"	/* Came from user or kernel mode? */
"	mrseq	r0, MSP		\n\t"	/* Kernel mode; fault frame on MSP. */
"	mrsne	r0, PSP		\n\t"	/* User mode; fault frame on PSP. */
"	b	arm_fault	\n\t"	/* Call arm_fault(frame, lr, type); */
);
}

void
arm_fault(struct faultframe *frame, uint32_t fault_lr, int type)
{
	int psig = SIGILL;		/* Default signal. */
	time_t syst;
	int bit;

	/* MUST read MMFAR and BFAR registers before reading CFSR. */
	uint32_t mmfar = SCB->MMFAR;	/* MemManage Fault Address */
	uint32_t bfar = SCB->BFAR;	/* BusFault Address */

	uint32_t hfsr = SCB->HFSR;	/* HardFault Status */
	uint32_t cfsr = SCB->CFSR;	/* Configurable Fault Status */

	/* If true, MMFAR holds the MemManage fault-triggering address. */
	uint32_t mmfarvalid = cfsr & MMFARVALID_BIT;	/* CFSR bit[7] */

	/* If true, BFAR holds the BusFault fault-triggering address. */
	uint32_t bfarvalid = cfsr & BFARVALID_BIT;	/* CFSR bit[15] */

	led_control(LED_KERNEL, 1);
	syst = u.u_ru.ru_stime;
#ifdef UCB_METER
	cnt.v_trap++;
#endif

	printf("fault type: 0x%x", type);
	printf("%s\n", HARD_FAULT(hfsr, T_HF_FORCED) ?
	    ", fault escalated" : "");

	for (bit = 0; bit < fault_types; ++bit) {
		if (CFSR_FAULT(cfsr, bit)) {
			printf("fault %d: %s, ", bit, fault_type[bit]);
			switch (bit) {
			case T_MM_IACCVIOL:
			case T_MM_DACCVIOL:
			case T_MM_MUNSTKERR:
			case T_MM_MSTKERR:
			case T_MM_MLSPERR:
				psig = SIGSEGV;
				printf("MemManage Fault\n");
				arm_clear_fault(bit, T_MM);
				break;
			case T_BF_IBUSERR:
			case T_BF_PRECISERR:
			case T_BF_IMPRECISERR:
			case T_BF_UNSTKERR:
			case T_BF_STKERR:
			case T_BF_LSPERR:
				psig = SIGBUS;
				printf("BusFault\n");
				arm_clear_fault(bit, T_BF);
				break;
			case T_UF_UNDEFINSTR:
			case T_UF_INVSTATE:
			case T_UF_INVPC:
			case T_UF_NOCP:
			case T_UF_UNALIGNED:
			case T_UF_DIVBYZERO:
				psig = SIGILL;
				printf("UsageFault\n");
				arm_clear_fault(bit, T_UF);
				break;
			default:
				psig = SIGILL;
				printf("unknown fault type\n");
				arm_clear_fault(bit, type);
				break;
			}
		}
	}

	printf("fault trap frame:\n");
	printf(" r0:\t0x%08x\tip:\t0x%08x\n", frame->ff_r0, frame->ff_ip);
	printf(" r1:\t0x%08x\tlr:\t0x%08x\n", frame->ff_r1, frame->ff_lr);
	printf(" r2:\t0x%08x\tpc:\t0x%08x\n", frame->ff_r2, frame->ff_pc);
	printf(" r3:\t0x%08x\tpsr:\t0x%08x\n", frame->ff_r3, frame->ff_psr);

	printf("fault status registers:\n");
	printf(" hfsr:\t0x%08x\tcfsr:\t0x%08x\n", hfsr, cfsr);
	printf("fault address registers:\n");
	printf(" mmfar:\t0x%08x\t%svalid\n", mmfar, mmfarvalid ? "" : "not ");
	printf(" bfar:\t0x%08x\t%svalid\n", bfar, bfarvalid ? "" : "not ");
	printf("fault entry EXC_RETURN value:\n");
	printf(" lr:\t0x%08x\n", fault_lr);

	arm_intr_enable();

	psignal(u.u_procp, psig);
	userret(frame->ff_pc, syst);

	led_control(LED_KERNEL, 0);
}
