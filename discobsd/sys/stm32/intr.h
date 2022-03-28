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

static inline int
arm_intr_disable(void)
{
	int s = __get_PRIMASK();
	__disable_irq();
	__ISB();
	return s;
}

static inline int
arm_intr_enable(void)
{
	int s = __get_PRIMASK();
	__enable_irq();
	__ISB();
	return s;
}

static inline void
arm_intr_restore(int s)
{
	__set_PRIMASK(s);
	__ISB();
}

#define	splhigh()	arm_intr_disable()
#define	splclock()	arm_intr_disable()
#define	spltty()	arm_intr_disable()
#define	splnet()	arm_intr_disable()
#define	splbio()	arm_intr_disable()

#define	splsoftclock()	arm_intr_enable()

#define	spl0()		arm_intr_enable()

#define	splx(s)		arm_intr_restore(s)

#endif	/* KERNEL */

#endif	/* !_MACHINE_INTR_H_ */
