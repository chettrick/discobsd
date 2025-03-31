/*
 * Copyright (c) 2022 Christopher Hettrick <chris@structfoo.com>
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

#ifndef	_MACHINE_INTR_H_
#define	_MACHINE_INTR_H_

#ifdef	KERNEL

#include <machine/stm32f4xx.h>

/*
 * Logical interrupt priority level: low prio (0) -> high prio (6)
 * Interrupts at or lower than numerical level are blocked.
 *
 * Notes:
 * - IPLs are the inverse of the Cortex-M hardware interrupt scheme.
 *     low prio (0b111x.xxxx) -> high prio (0b001x.xxxx), 3 NVIC prio bits
 *     low prio (0b0111.xxxx) -> high prio (0b0001.xxxx), 4 NVIC prio bits
 * - A zero value disables BASEPRI register; IPL levels are 0 -> 6.
 * - PendSV exception is IPL level 0; SVCall exception is IPL level 7.
 */
#define	IPL_NONE	0	/* Blocks nothing. */
#define	IPL_SOFTCLOCK	1	/* Blocks low-priority clock processing. */
#define	IPL_NET		2	/* Blocks network protocol processing. */
#define	IPL_BIO		3	/* Blocks disk controllers. */
#define	IPL_TTY		4	/* Blocks terminal multiplexers. */
#define	IPL_CLOCK	5	/* Blocks high-priority clock processing. */
#define	IPL_HIGH	6	/* Blocks all interrupt activity. */

#define	IPL_TOP		(IPL_HIGH + 1)	/* +1 since zero disables BASEPRI. */
#define	IPL_BITS	(8U - __NVIC_PRIO_BITS)	/* MSB prio shift bits. */

/* Cortex-M core exception/interrupt priority levels. */
#define	IPL_PENDSV	IPL_NONE	/* PendSV exception at lowest prio. */
#define	IPL_SVCALL	IPL_TOP		/* SVC exception at highest prio. */
#define	IPL_SYSTICK	IPL_CLOCK	/* SysTick exception at clock prio. */

#define	IPLTOREG(ipl) \
	(uint8_t)((ipl) ? (((IPL_TOP - (ipl)) << IPL_BITS) & 0xFFUL) : 0)
#define	REGTOIPL(reg) \
	(uint32_t)((reg) ? (IPL_TOP - ((reg) >> IPL_BITS)) : 0)

static inline int
arm_intr_disable(void)
{
	int s = __get_PRIMASK();
	__disable_irq();			/* Disable interrupts. */
	__ISB();
	return s;
}

static inline int
arm_intr_enable(void)
{
	int s = __get_PRIMASK();
	__enable_irq();				/* Enable interrupts. */
	__ISB();
	return s;
}

static inline void
arm_intr_restore(int s)
{
	__set_PRIMASK(s);
	__ISB();
}

static inline void
arm_intr_disable_irq(int irq)
{
	NVIC_DisableIRQ(irq);
}

static inline void
arm_intr_enable_irq(int irq)
{
	NVIC_EnableIRQ(irq);
}

static inline void
arm_intr_set_priority(int irq, int prio)
{
	/*
	 * This CMSIS function bitshifts prio into the most significant bits
	 * and expects an inverted prio for the Cortex-M interrupt priority
	 * scheme (zero has more priority than one), so IPL_TOP - prio.
	 */
	NVIC_SetPriority(irq, IPL_TOP - prio);
}

#ifdef __thumb2__

static inline int
splraise(int new)
{
	int old;

	old = REGTOIPL(__get_BASEPRI());
	__set_BASEPRI_MAX(IPLTOREG(new));
	__ISB();

	return old;
}

#define	splhigh()	splraise(IPL_HIGH)
#define	splclock()	splraise(IPL_CLOCK)
#define	spltty()	splraise(IPL_TTY)
#define	splnet()	splraise(IPL_NET)
#define	splbio()	splraise(IPL_BIO)

#define	splsoftclock()	splraise(IPL_SOFTCLOCK)

static inline void
splx(int s)
{
	__set_BASEPRI(IPLTOREG(s));
	__ISB();
}

static inline int
spl0(void)
{
	int old;

	old = REGTOIPL(__get_BASEPRI());
	splx(IPL_NONE);

	return old;
}

#else /* __thumb__ */

#define	splhigh()	arm_intr_disable()
#define	splclock()	arm_intr_disable()
#define	spltty()	arm_intr_disable()
#define	splnet()	arm_intr_disable()
#define	splbio()	arm_intr_disable()

#define	splsoftclock()	arm_intr_enable()

#define	splx(s)		arm_intr_restore(s)

#define	spl0()		arm_intr_enable()

#endif

#endif	/* KERNEL */

#endif	/* !_MACHINE_INTR_H_ */
