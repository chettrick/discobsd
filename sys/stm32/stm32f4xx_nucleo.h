/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo.h
  * @author  MCD Application Team
  * @version V1.2.6
  * @date    27-January-2017
  * @brief   This file contains definitions for:
  *          - LEDs and push-button available on STM32F4XX-Nucleo Kit
  *            from STMicroelectronics
  *          - MicroSD available on Adafruit MicroSD Breakout Board+ (ID 254)
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
#ifndef __STM32F4XX_NUCLEO_H
#define __STM32F4XX_NUCLEO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"

/* To be defined only if provided with the Adafruit MicroSD Breakout Board+ */
/* https://www.adafruit.com/product/254 */
#define ADAFRUIT_SD_ID254

typedef enum
{
  LED2 = 0,
  LED_GREEN = LED2
} Led_TypeDef;

typedef enum
{
  BUTTON_USER = 0,
  /* Alias */
  BUTTON_KEY  = BUTTON_USER
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0
} ButtonMode_TypeDef;

/**
  * @brief Define for STM32F4XX_NUCLEO board
  */
#if !defined (USE_STM32F4XX_NUCLEO)
 #define USE_STM32F4XX_NUCLEO
#endif

#define LEDn                                    ((uint8_t)1)

#define LED2_PIN                                ((uint32_t)LL_GPIO_PIN_5)
#define LED2_GPIO_PORT                          ((GPIO_TypeDef *)GPIOA)
#define LED2_GPIO_CLK_ENABLE()                  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#define LED2_GPIO_CLK_DISABLE()                 LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA)

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)         LED2_GPIO_CLK_ENABLE()
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)        LED2_GPIO_CLK_DISABLE()

#define BUTTONn                                 ((uint8_t)1)

/**
  * @brief User push-button
  */
#define BUTTON_USER_PRESSED                     ((uint32_t)0)

#define USER_BUTTON_PIN                         ((uint32_t)LL_GPIO_PIN_13)
#define USER_BUTTON_GPIO_PORT                   ((GPIO_TypeDef *)GPIOC)
#define USER_BUTTON_GPIO_CLK_ENABLE()           LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC)
#define USER_BUTTON_GPIO_CLK_DISABLE()          LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOC)

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)       USER_BUTTON_GPIO_CLK_ENABLE()
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)      USER_BUTTON_GPIO_CLK_DISABLE()

/* Aliases */
#define KEY_BUTTON_PIN                       USER_BUTTON_PIN
#define KEY_BUTTON_GPIO_PORT                 USER_BUTTON_GPIO_PORT
#define KEY_BUTTON_GPIO_CLK_ENABLE()         USER_BUTTON_GPIO_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()        USER_BUTTON_GPIO_CLK_DISABLE()

/*############################### SPI1 #######################################*/
#ifdef HAL_SPI_MODULE_ENABLED

#define NUCLEO_SPIx                                     SPI1
#define NUCLEO_SPIx_CLK_ENABLE()                        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1)

#define NUCLEO_SPIx_SCK_AF                              ((uint32_t)LL_GPIO_AF_5)
#define NUCLEO_SPIx_SCK_PIN                             ((uint32_t)LL_GPIO_PIN_5)
#define NUCLEO_SPIx_SCK_GPIO_PORT                       ((GPIO_TypeDef *)GPIOA)
#define NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE()               LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#define NUCLEO_SPIx_SCK_GPIO_CLK_DISABLE()              LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA)

#define NUCLEO_SPIx_MISO_MOSI_AF                        ((uint32_t)LL_GPIO_AF_5)
#define NUCLEO_SPIx_MISO_PIN                            ((uint32_t)LL_GPIO_PIN_6)
#define NUCLEO_SPIx_MOSI_PIN                            ((uint32_t)LL_GPIO_PIN_7)
#define NUCLEO_SPIx_MISO_MOSI_GPIO_PORT                 ((GPIO_TypeDef *)GPIOA)
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()         LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()        LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define NUCLEO_SPIx_TIMEOUT_MAX                   1000

/**
  * @brief  SD Control Lines management
  */
#define SD_CS_LOW()                               LL_GPIO_ResetOutputPin(SD_CS_GPIO_PORT, SD_CS_PIN)
#define SD_CS_HIGH()                              LL_GPIO_SetOutputPin(SD_CS_GPIO_PORT, SD_CS_PIN)

/**
  * @brief  SD Control Interface pins (MicroSD Breakout Board+ D10 PB6)
  */
#define SD_CS_PIN                                 ((uint32_t)LL_GPIO_PIN_6)
#define SD_CS_GPIO_PORT                           ((GPIO_TypeDef *)GPIOB)
#define SD_CS_GPIO_CLK_ENABLE()                   LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)
#define SD_CS_GPIO_CLK_DISABLE()                  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_GPIOB)

#endif /* HAL_SPI_MODULE_ENABLED */

uint32_t         BSP_GetVersion(void);
void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);
void             BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
void             BSP_PB_DeInit(Button_TypeDef Button);
uint32_t         BSP_PB_GetState(Button_TypeDef Button);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4XX_NUCLEO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
