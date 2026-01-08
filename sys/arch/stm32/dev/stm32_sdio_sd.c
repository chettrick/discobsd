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
#include <stm32/dev/stm32_bsp_sd.h>

#include <stm32/hal/stm32f4xx_ll_bus.h>
#include <stm32/hal/stm32f4xx_ll_gpio.h>

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

struct gpio_pin {
	GPIO_TypeDef	*port;
	char		 port_name;
	uint32_t	 pin;
	uint32_t	 mode;
	uint32_t	 speed;
	uint32_t	 pull;
	uint32_t	 af;
	uint32_t	 out_type;
};

struct sdio_inst {
#define	NUM_PINS	6
	struct gpio_pin	pins[NUM_PINS];
	int		prio;
	int		irq;
};

static const struct sdio_inst sdio = {
#define	PIN2		LL_GPIO_PIN_2
#define	PIN8		LL_GPIO_PIN_8
#define	PIN9		LL_GPIO_PIN_9
#define	PIN10		LL_GPIO_PIN_10
#define	PIN11		LL_GPIO_PIN_11
#define	PIN12		LL_GPIO_PIN_12
#define	AF12		LL_GPIO_AF_12
#define	ALT		LL_GPIO_MODE_ALTERNATE
#define	VH		LL_GPIO_SPEED_FREQ_VERY_HIGH
#define	PUP		LL_GPIO_PULL_UP
#define	OPP		LL_GPIO_OUTPUT_PUSHPULL
	{
		{ GPIOC, 'C', PIN8,  ALT, VH, PUP, AF12, OPP },	/* D0 */
		{ GPIOC, 'C', PIN9,  ALT, VH, PUP, AF12, OPP },	/* D1 */
		{ GPIOC, 'C', PIN10, ALT, VH, PUP, AF12, OPP },	/* D2 */
		{ GPIOC, 'C', PIN11, ALT, VH, PUP, AF12, OPP },	/* D3 */
		{ GPIOC, 'C', PIN12, ALT, VH, PUP, AF12, OPP },	/* CLK */
		{ GPIOD, 'D', PIN2,  ALT, VH, PUP, AF12, OPP },	/* CMD */
	}, IPL_BIO, SDIO_IRQn
};

/**
  * @brief  Initializes the SD MSP.
  * @param  hsd: SD handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void
BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params)
{
	int i;
	int num_pins = sizeof(sdio.pins) / sizeof(sdio.pins[0]);

	/* Enable SDIO clock. */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SDIO);

	/* Enable GPIO clocks and configure GPIO pins. */
	for (i = 0; i < num_pins; ++i) {
		GPIO_TypeDef	*port = sdio.pins[i].port;
		uint32_t	 pin = sdio.pins[i].pin;

		LL_GPIO_EnableClock(port);
		LL_GPIO_SetPinMode(port, pin, sdio.pins[i].mode);
		LL_GPIO_SetPinSpeed(port, pin, sdio.pins[i].speed);
		LL_GPIO_SetPinPull(port, pin, sdio.pins[i].pull);
		LL_GPIO_SetAFPin(port, pin, sdio.pins[i].af);
		LL_GPIO_SetPinOutputType(port, pin, sdio.pins[i].out_type);
	}

	/* NVIC configuration for SDIO interrupts. */
	arm_intr_set_priority(sdio.irq, sdio.prio);
	arm_intr_enable_irq(sdio.irq);
}

/**
  * @brief  DeInitializes the SD MSP.
  * @param  hsd: SD handle
  * @param  Params : pointer on additional configuration parameters, can be NULL.
  */
__weak void
BSP_SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params)
{
	/* Disable NVIC for SDIO interrupts. */
	arm_intr_disable_irq(sdio.irq);

	/* Disable SDIO clock. */
	LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_SDIO);
}

#endif /* SDIO_ENABLED */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
