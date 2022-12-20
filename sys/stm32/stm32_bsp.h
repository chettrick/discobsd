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

#ifndef	_MACHINE_STM32_BSP_H_
#define	_MACHINE_STM32_BSP_H_

#ifdef	KERNEL

#include "stm32f4xx_hal.h"

typedef enum {
#ifdef LED1_GPIO_PORT
	LED1,
#endif
#ifdef LED2_GPIO_PORT
	LED2,
#endif
#ifdef LED3_GPIO_PORT
	LED3,
#endif
#ifdef LED4_GPIO_PORT
	LED4,
#endif
	LED_COUNT
} Led_TypeDef;

typedef enum {
#ifdef BUTTON_USER_GPIO_PORT
	BUTTON_USER,
#endif
	BUTTON_COUNT
} Button_TypeDef;

void		BSP_LED_Init(Led_TypeDef Led);
void		BSP_LED_On(Led_TypeDef Led);
void		BSP_LED_Off(Led_TypeDef Led);
void		BSP_LED_Toggle(Led_TypeDef Led);
void		BSP_PB_Init(Button_TypeDef Button);
uint32_t	BSP_PB_GetState(Button_TypeDef Button);

#endif	/* KERNEL */

#endif	/* !_MACHINE_STM32_BSP_H_ */
