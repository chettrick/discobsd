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

#include <sys/param.h>
#include <sys/signalvar.h>
#include <sys/systm.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/conf.h>
#include <sys/tty.h>

#include <machine/frame.h>
#include <machine/uart.h>

#include "stm32f4xx_hal.h"

void
SysTick_Handler(void)
{
__asm volatile (
"	tst	lr, #0x4	\n\t"	/* Test bit 2 (SPSEL) of EXC_RETURN. */
"	ite	eq		\n\t"	/* Came from user or kernel mode? */
"	mrseq	r0, MSP		\n\t"	/* Kernel mode; stack frame on MSP. */
"	mrsne	r0, PSP		\n\t"	/* User mode; stack frame on PSP. */
"	b	systick		\n\t"	/* Call systick(frame); */
);
}

/*
 * Default system time base for Cortex-M.
 * Internal hardware SysTick interrupt every 1 millisecond.
 */
void
systick(struct clockframe *frame)
{
	HAL_IncTick();		/* Required for HAL driver subsystems. */

	hardclock((caddr_t)frame->cf_pc, frame->cf_psr);

	uartintr(makedev(UART_MAJOR, 2));	/* USART3, console */
}
