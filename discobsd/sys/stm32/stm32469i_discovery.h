/**
  ******************************************************************************
  * @file    stm32469i_discovery.h
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    27-January-2017
  * @brief   This file contains definitions for STM32469I-Discovery LEDs,
  *          push-buttons hardware resources.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32469I_DISCOVERY_H
#define __STM32469I_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif


 /* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"

/** @brief Led_TypeDef
  *  STM32469I_Discovery board leds definitions.
  */
typedef enum
{
 LED1 = 0,
 LED_GREEN = LED1,
 LED2 = 1,
 LED_ORANGE = LED2,
 LED3 = 2,
 LED_RED = LED3,
 LED4 = 3,
 LED_BLUE = LED4

} Led_TypeDef;

/* Always four LEDs for all revisions of Discovery boards */
#define LEDn                             ((uint8_t)4)

/* 4 Leds are connected to MCU directly on PG6, PD4, PD5, PK3 */
#define LED1_GPIO_PORT                  ((GPIO_TypeDef *)GPIOG)
#define LED2_GPIO_PORT                  ((GPIO_TypeDef *)GPIOD)
#define LED3_GPIO_PORT                  ((GPIO_TypeDef *)GPIOD)
#define LED4_GPIO_PORT                  ((GPIO_TypeDef *)GPIOK)

#define LED1_GPIO_CLK_ENABLE()          LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG)
#define LED1_GPIO_CLK_DISABLE()         LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOG)
#define LED2_GPIO_CLK_ENABLE()          LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD)
#define LED2_GPIO_CLK_DISABLE()         LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOD)
#define LED3_GPIO_CLK_ENABLE()          LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD)
#define LED3_GPIO_CLK_DISABLE()         LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOD)
#define LED4_GPIO_CLK_ENABLE()          LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOK)
#define LED4_GPIO_CLK_DISABLE()         LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOK)

#define LED1_PIN                        ((uint32_t)LL_GPIO_PIN_6)
#define LED2_PIN                        ((uint32_t)LL_GPIO_PIN_4)
#define LED3_PIN                        ((uint32_t)LL_GPIO_PIN_5)
#define LED4_PIN                        ((uint32_t)LL_GPIO_PIN_3)

/**
  * @brief SD-detect signal
  */
#define SD_DETECT_PIN                   ((uint32_t)LL_GPIO_PIN_2)
#define SD_DETECT_GPIO_PORT             ((GPIO_TypeDef *)GPIOG)
#define SD_DETECT_GPIO_CLK_ENABLE()     LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG)
#define SD_DETECT_GPIO_CLK_DISABLE()    LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOG)
#define SD_DETECT_EXTI_IRQn             EXTI2_IRQn

uint32_t         BSP_GetVersion(void);
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);

#ifdef __cplusplus
}
#endif

#endif /* __STM32469I_DISCOVERY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
