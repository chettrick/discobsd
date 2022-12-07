/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo.c
  * @author  MCD Application Team
  * @version V1.2.6
  * @date    27-January-2017
  * @brief   This file provides set of firmware functions to manage:
  *          - LEDs and push-button available on STM32F4XX-Nucleo Kit
  *            from STMicroelectronics
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#ifdef F411RENUCLEO                     /* Kernel Config 'board' definition. */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo.h"

/**
  * @brief STM32F4xx NUCLEO BSP Driver version number V1.2.6
  */
#define __STM32F4xx_NUCLEO_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __STM32F4xx_NUCLEO_BSP_VERSION_SUB1   (0x02) /*!< [23:16] sub1 version */
#define __STM32F4xx_NUCLEO_BSP_VERSION_SUB2   (0x06) /*!< [15:8]  sub2 version */
#define __STM32F4xx_NUCLEO_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32F4xx_NUCLEO_BSP_VERSION        ((__STM32F4xx_NUCLEO_BSP_VERSION_MAIN << 24)\
                                             |(__STM32F4xx_NUCLEO_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32F4xx_NUCLEO_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32F4xx_NUCLEO_BSP_VERSION_RC))

GPIO_TypeDef * const GPIO_PORT[] = {
    LED2_GPIO_PORT,
};

uint32_t const GPIO_PIN[] = {
    LED2_PIN,
};

GPIO_TypeDef * const BUTTON_PORT[] = {
    USER_BUTTON_GPIO_PORT,
};

uint32_t const BUTTON_PIN[] = {
    USER_BUTTON_PIN,
};

/**
  * @brief  This method returns the STM32F4xx NUCLEO BSP Driver revision
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t
BSP_GetVersion(void)
{
  return __STM32F4xx_NUCLEO_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
void
BSP_LED_Init(Led_TypeDef Led)
{
  if (Led == LED2) {
    GPIO_TypeDef *port = GPIO_PORT[Led];
    uint32_t pin = GPIO_PIN[Led];

    /* Enable the GPIO_LED clock */
    LL_GPIO_EnableClock(port);

    /* Configure the GPIO_LED pin */
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);

    /* By default, turn off LED by setting a low level on corresponding GPIO */
    LL_GPIO_ResetOutputPin(port, pin);
  }
}

/**
  * @brief  DeInit LEDs.
  * @param  Led: LED to be de-init.
  *   This parameter can be one of the following values:
  *     @arg  LED2
  * @note Led DeInit does not disable the GPIO clock nor disable the Mfx
  */
void
BSP_LED_DeInit(Led_TypeDef Led)
{
  if (Led == LED2) {
    GPIO_TypeDef *port = GPIO_PORT[Led];
    uint32_t pin = GPIO_PIN[Led];

    /* Turn off LED */
    LL_GPIO_ResetOutputPin(port, pin);

    /* Configure parameters to default values */
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_ANALOG);
    LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_LOW);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
  }
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
void
BSP_LED_On(Led_TypeDef Led)
{
  if (Led == LED2) {
    GPIO_TypeDef *port = GPIO_PORT[Led];
    uint32_t pin = GPIO_PIN[Led];

    LL_GPIO_SetOutputPin(port, pin);
  }
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
void
BSP_LED_Off(Led_TypeDef Led)
{
  if (Led == LED2) {
    GPIO_TypeDef *port = GPIO_PORT[Led];
    uint32_t pin = GPIO_PIN[Led];

    LL_GPIO_ResetOutputPin(port, pin);
  }
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
void
BSP_LED_Toggle(Led_TypeDef Led)
{
  if (Led == LED2) {
    GPIO_TypeDef *port = GPIO_PORT[Led];
    uint32_t pin = GPIO_PIN[Led];

    LL_GPIO_TogglePin(port, pin);
  }
}

/**
  * @brief  Configures Button GPIO.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_KEY
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  */
void
BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_TypeDef *port = BUTTON_PORT[Button];
  uint32_t pin = BUTTON_PIN[Button];

  /* Enable the BUTTON clock */
  LL_GPIO_EnableClock(port);

  if (Button_Mode == BUTTON_MODE_GPIO) {
    /* Configure Button pin as input */
    LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_DOWN);
    LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_HIGH);
  }
}

/**
  * @brief  Push Button DeInit.
  * @param  Button: Button to be configured
  *   This parameter should be: BUTTON_KEY
  * @note PB DeInit does not disable the GPIO clock
  */
void
BSP_PB_DeInit(Button_TypeDef Button)
{
  GPIO_TypeDef *port = BUTTON_PORT[Button];
  uint32_t pin = BUTTON_PIN[Button];

  /* Configure parameters to default values */
  LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinSpeed(port, pin, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(port, pin, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinOutputType(port, pin, LL_GPIO_OUTPUT_PUSHPULL);
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_KEY
  * @retval The Button GPIO pin value.
  */
uint32_t
BSP_PB_GetState(Button_TypeDef Button)
{
  GPIO_TypeDef *port = BUTTON_PORT[Button];
  uint32_t pin = BUTTON_PIN[Button];

  return (LL_GPIO_ReadInputPort(port) & pin);
}

#endif /* F411RENUCLEO */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
