/**
  ******************************************************************************
  * @file    stm32_sdio_sd.c
  * @author  MCD Application Team
  * @version V2.0.0
  * @date    27-January-2017
  * @brief   This file includes the SDIO driver for micro SD cards on boards.
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

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is used to drive the external micro SD card mounted on boards.
   - This driver does not need a specific component driver for the micro SD device
     to be included with.

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the micro SD card using the BSP_SD_Init() function. This
       function includes the MSP layer hardware resources initialization and the
       SDIO interface configuration to interface with the external micro SD. It
       also includes the micro SD initialization sequence.
     o Checking the SD card presence is not managed.
     o The function BSP_SD_GetCardInfo() is used to get the micro SD card information
       which is stored in the structure "BSP_SD_CardInfo".

     + Micro SD card operations
        o The micro SD card can be accessed with read/write block(s) operations once
          it is ready for access. The access can be performed whether using the polling
          mode by calling the functions BSP_SD_ReadBlocks()/BSP_SD_WriteBlocks(), or by DMA
          transfer using the functions BSP_SD_ReadBlocks_DMA()/BSP_SD_WriteBlocks_DMA()
        o The DMA transfer complete is used with interrupt mode. Once the SD transfer
          is complete, the SD interrupt is handled using the function BSP_SD_IRQHandler(),
          the DMA Tx/Rx transfer complete are handled using the functions
          BSP_SD_DMA_Tx_IRQHandler()/BSP_SD_DMA_Rx_IRQHandler(). The corresponding user callbacks
          are implemented by the user at application level.
        o The SD erase block(s) is performed using the function BSP_SD_Erase() with
          specifying the number of blocks to erase.
        o The SD runtime status is returned when calling the function BSP_SD_GetCardState().

------------------------------------------------------------------------------*/

#ifdef SDIO_ENABLED                     /* Kernel Config 'controller' definition. */

/* Includes ------------------------------------------------------------------*/
#include <machine/stm32_bsp_sd.h>

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"

SD_HandleTypeDef uSdHandle;

/**
  * @brief  Initializes the SD card device.
  * @retval SD status
  */
uint8_t
BSP_SD_Init(void)
{
  uint8_t sd_state = BSP_SD_OK;

  /* PLLSAI is dedicated to LCD periph. Do not use it to get 48MHz. */

  /* uSD device interface configuration */
  uSdHandle.Instance = SDIO;

  uSdHandle.Init.ClockEdge           = SDIO_CLOCK_EDGE_RISING;
  uSdHandle.Init.ClockBypass         = SDIO_CLOCK_BYPASS_DISABLE;
  uSdHandle.Init.ClockPowerSave      = SDIO_CLOCK_POWER_SAVE_DISABLE;
  uSdHandle.Init.BusWide             = SDIO_BUS_WIDE_1B;
  uSdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_ENABLE;

#if defined(SDIO_XFER_CLK_DIV)       /* Kernel Config 'options' definition. */
  uSdHandle.Init.ClockDiv            = SDIO_XFER_CLK_DIV;
#else                                /* Default in stm32f4xx_ll_sdmmc.h */
  uSdHandle.Init.ClockDiv            = SDIO_TRANSFER_CLK_DIV;
#endif

  /* Msp SD initialization */
  BSP_SD_MspInit(&uSdHandle, NULL);

  /* HAL SD initialization */
  if (HAL_SD_Init(&uSdHandle) != HAL_OK) {
    sd_state = BSP_SD_ERROR;
  }

  /* Configure SD Bus width */
  if (sd_state == BSP_SD_OK) {
    /* Enable wide operation */
    if (HAL_SD_ConfigWideBusOperation(&uSdHandle, SDIO_BUS_WIDE_4B) != HAL_OK) {
      sd_state = BSP_SD_ERROR;
    } else {
      sd_state = BSP_SD_OK;
    }
  }
  return  sd_state;
}

/**
  * @brief  DeInitializes the SD card device.
  * @retval SD status
  */
uint8_t
BSP_SD_DeInit(void)
{
  uint8_t sd_state = BSP_SD_OK;

  uSdHandle.Instance = SDIO;

  /* HAL SD deinitialization */
  if (HAL_SD_DeInit(&uSdHandle) != HAL_OK) {
    sd_state = BSP_SD_ERROR;
  }

  /* Msp SD deinitialization */
  uSdHandle.Instance = SDIO;
  BSP_SD_MspDeInit(&uSdHandle, NULL);

  return  sd_state;
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read
  * @param  NumOfBlocks: Number of SD blocks to read
  * @param  Timeout: Timeout for read operation
  * @retval SD status
  */
uint8_t
BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  if (HAL_SD_ReadBlocks(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) != HAL_OK) {
    return BSP_SD_ERROR;
  } else {
    return BSP_SD_OK;
  }
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  NumOfBlocks: Number of SD blocks to write
  * @param  Timeout: Timeout for write operation
  * @retval SD status
  */
uint8_t
BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  if (HAL_SD_WriteBlocks(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) != HAL_OK) {
    return BSP_SD_ERROR;
  } else {
    return BSP_SD_OK;
  }
}

/**
  * @brief  Erases the specified memory area of the given SD card.
  * @param  StartAddr: Start byte address
  * @param  EndAddr: End byte address
  * @retval SD status
  */
uint8_t
BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
  if (HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr) != HAL_OK) {
    return BSP_SD_ERROR;
  } else {
    return BSP_SD_OK;
  }
}

/**
  * @brief  Gets the current SD card data status.
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  */
uint8_t
BSP_SD_GetCardState(void)
{
  return((HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER ) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

/**
  * @brief  Get SD information about specific SD card.
  * @param  pCardInfo: Pointer to BSP_SD_CardInfo structure
  * @retval SD status
  */
uint8_t
BSP_SD_GetCardInfo(BSP_SD_CardInfo *pCardInfo)
{
  uint8_t status;

  /* Get SD card Information */
  status = HAL_SD_GetCardInfo(&uSdHandle, pCardInfo);

  return status;
}

/**
  * @brief  Initializes the SD MSP.
  * @param  hsd: SD handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void
BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params)
{
#define NUM_C_PINS 5

  uint32_t port_c_pins[NUM_C_PINS] = { LL_GPIO_PIN_8, LL_GPIO_PIN_9, LL_GPIO_PIN_10,
                                       LL_GPIO_PIN_11, LL_GPIO_PIN_12 };
  uint32_t port_d_pin = LL_GPIO_PIN_2;
  uint32_t i;

  /* Enable SDIO clock */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SDIO);

  /* Enable GPIO clocks */
  LL_GPIO_EnableClock(GPIOC);
  LL_GPIO_EnableClock(GPIOD);

  /* GPIOC configuration */
  for (i = 0; i < NUM_C_PINS; ++i) {
    LL_GPIO_SetPinMode(GPIOC, port_c_pins[i], LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinSpeed(GPIOC, port_c_pins[i], LL_GPIO_SPEED_FREQ_VERY_HIGH);
    LL_GPIO_SetPinPull(GPIOC, port_c_pins[i], LL_GPIO_PULL_UP);
    LL_GPIO_SetAFPin(GPIOC, port_c_pins[i], LL_GPIO_AF_12);
    LL_GPIO_SetPinOutputType(GPIOC, port_c_pins[i], LL_GPIO_OUTPUT_PUSHPULL);
  }

  /* GPIOD configuration */
  LL_GPIO_SetPinMode(GPIOD, port_d_pin, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinSpeed(GPIOD, port_d_pin, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinPull(GPIOD, port_d_pin, LL_GPIO_PULL_UP);
  LL_GPIO_SetAFPin(GPIOD, port_d_pin, LL_GPIO_AF_12);
  LL_GPIO_SetPinOutputType(GPIOD, port_d_pin, LL_GPIO_OUTPUT_PUSHPULL);

  /* NVIC configuration for SDIO interrupts */
  arm_intr_set_priority(SDIO_IRQn, IPL_BIO);
  arm_intr_enable_irq(SDIO_IRQn);
}

/**
  * @brief  DeInitializes the SD MSP.
  * @param  hsd: SD handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void
BSP_SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params)
{
  /* Disable NVIC for SDIO interrupts */
  arm_intr_disable_irq(SDIO_IRQn);

  /* Disable SDIO clock */
  LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SDIO);
}

#endif /* SDIO_ENABLED */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
