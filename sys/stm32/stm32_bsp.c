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

#include <machine/stm32_bsp.h>

#include "stm32f4xx_ll_gpio.h"

struct bsp_gpio {
	GPIO_TypeDef	*port;
	uint32_t	 pin;
	uint32_t	 invert;
};

static const struct bsp_gpio led[LED_COUNT] = {
#ifdef LED1_GPIO_PORT
	{ LED1_GPIO_PORT, LED1_PIN, LED1_INVERT },
#endif
#ifdef LED2_GPIO_PORT
	{ LED2_GPIO_PORT, LED2_PIN, LED2_INVERT },
#endif
#ifdef LED3_GPIO_PORT
	{ LED3_GPIO_PORT, LED3_PIN, LED3_INVERT },
#endif
#ifdef LED4_GPIO_PORT
	{ LED4_GPIO_PORT, LED4_PIN, LED4_INVERT },
#endif
};

static const struct bsp_gpio button[BUTTON_COUNT] = {
#ifdef BUTTON_USER_GPIO_PORT
	{ BUTTON_USER_GPIO_PORT, BUTTON_USER_PIN, BUTTON_USER_INVERT },
#endif
};

void
BSP_LED_Init(Led_TypeDef Led)
{
	GPIO_TypeDef *port = led[Led].port;
	uint32_t pin = led[Led].pin;
	uint32_t invert = led[Led].invert;

	LL_GPIO_EnableClock(port);

	LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);

	if (invert) {
		LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_NO);
	} else {
		LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_UP);
	}

	BSP_LED_Off(Led);
}

void
BSP_LED_On(Led_TypeDef Led)
{
	GPIO_TypeDef *port = led[Led].port;
	uint32_t pin = led[Led].pin;
	uint32_t invert = led[Led].invert;

	if (invert) {
		LL_GPIO_ResetOutputPin(port, pin);
	} else {
		LL_GPIO_SetOutputPin(port, pin);
	}
}

void
BSP_LED_Off(Led_TypeDef Led)
{
	GPIO_TypeDef *port = led[Led].port;
	uint32_t pin = led[Led].pin;
	uint32_t invert = led[Led].invert;

	if (invert) {
		LL_GPIO_SetOutputPin(port, pin);
	} else {
		LL_GPIO_ResetOutputPin(port, pin);
	}
}

void
BSP_LED_Toggle(Led_TypeDef Led)
{
	GPIO_TypeDef *port = led[Led].port;
	uint32_t pin = led[Led].pin;

	LL_GPIO_TogglePin(port, pin);
}

void
BSP_PB_Init(Button_TypeDef Button)
{
	GPIO_TypeDef *port = button[Button].port;
	uint32_t pin = button[Button].pin;
	uint32_t invert = button[Button].invert;

	LL_GPIO_EnableClock(port);

	LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_HIGH);

	if (invert) {
		LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_NO);
	} else {
		LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_DOWN);
	}
}

uint32_t
BSP_PB_GetState(Button_TypeDef Button)
{
	GPIO_TypeDef *port = button[Button].port;
	uint32_t pin = button[Button].pin;
	uint32_t invert = button[Button].invert;
	uint32_t level = LL_GPIO_ReadInputPort(port) & pin;

	if (invert) {
		return ((level == 0) ? 1 : 0);
	} else {
		return ((level == 0) ? 0 : 1);
	}
}
