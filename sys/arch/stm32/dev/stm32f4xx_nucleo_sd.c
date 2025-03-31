/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_sd.c
  * @author  MCD Application Team
  * @version V1.2.6
  * @date    27-January-2017
  * @brief   This file provides set of firmware functions to manage:
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_sd.h"

/**
 * @brief BUS variables
 */

#ifdef HAL_SPI_MODULE_ENABLED

uint32_t SpixTimeout = NUCLEO_SPIx_TIMEOUT_MAX; /*<! Value of Timeout when SPI communication fails */
static SPI_HandleTypeDef hnucleo_Spi;

static void       SPIx_Init(void);
static void       SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
static void       SPIx_Error(void);
static void       SPIx_MspInit(SPI_HandleTypeDef *hspi);

/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

/******************************* SPI ********************************/

/**
  * @brief  Initializes SPI MSP.
  */
static void
SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  LL_GPIO_EnableClock(NUCLEO_SPIx_SCK_GPIO_PORT);
  LL_GPIO_EnableClock(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT);

  /* Configure SPI SCK */
  LL_GPIO_SetPinMode(NUCLEO_SPIx_SCK_GPIO_PORT, NUCLEO_SPIx_SCK_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin(NUCLEO_SPIx_SCK_GPIO_PORT, NUCLEO_SPIx_SCK_PIN, NUCLEO_SPIx_SCK_AF);
  LL_GPIO_SetPinSpeed(NUCLEO_SPIx_SCK_GPIO_PORT, NUCLEO_SPIx_SCK_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(NUCLEO_SPIx_SCK_GPIO_PORT, NUCLEO_SPIx_SCK_PIN, LL_GPIO_PULL_UP);

  /* Configure SPI MOSI */
  LL_GPIO_SetPinMode(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, NUCLEO_SPIx_MOSI_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, NUCLEO_SPIx_MOSI_PIN, NUCLEO_SPIx_MISO_MOSI_AF);
  LL_GPIO_SetPinSpeed(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, NUCLEO_SPIx_MOSI_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, NUCLEO_SPIx_MOSI_PIN, LL_GPIO_PULL_DOWN);

  /* Configure SPI MISO */
  LL_GPIO_SetPinMode(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, NUCLEO_SPIx_MISO_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, NUCLEO_SPIx_MISO_PIN, NUCLEO_SPIx_MISO_MOSI_AF);
  LL_GPIO_SetPinSpeed(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, NUCLEO_SPIx_MISO_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(NUCLEO_SPIx_MISO_MOSI_GPIO_PORT, NUCLEO_SPIx_MISO_PIN, LL_GPIO_PULL_DOWN);

  /*** Configure the SPI peripheral ***/
  /* Enable SPI clock */
  NUCLEO_SPIx_CLK_ENABLE();
}

/**
  * @brief  Initializes SPI HAL.
  */
static void
SPIx_Init(void)
{
  if (HAL_SPI_GetState(&hnucleo_Spi) == HAL_SPI_STATE_RESET) {
    /* SPI Config */
    hnucleo_Spi.Instance = NUCLEO_SPIx;
      /* SPI baudrate is set to 12,5 MHz maximum (APB1/SPI_BaudRatePrescaler = 100/8 = 12,5 MHz)
         to verify these constraints:
          - SD card SPI interface max baudrate is 25MHz for write/read
          - PCLK2 max frequency is 100 MHz
       */
    hnucleo_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hnucleo_Spi.Init.Direction = SPI_DIRECTION_2LINES;
    hnucleo_Spi.Init.CLKPhase = SPI_PHASE_2EDGE;
    hnucleo_Spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hnucleo_Spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hnucleo_Spi.Init.CRCPolynomial = 7;
    hnucleo_Spi.Init.DataSize = SPI_DATASIZE_8BIT;
    hnucleo_Spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hnucleo_Spi.Init.NSS = SPI_NSS_SOFT;
    hnucleo_Spi.Init.TIMode = SPI_TIMODE_DISABLE;
    hnucleo_Spi.Init.Mode = SPI_MODE_MASTER;

    SPIx_MspInit(&hnucleo_Spi);
    HAL_SPI_Init(&hnucleo_Spi);
  }
}

/**
  * @brief  SPI Write a byte to device
  * @param  DataIn: value to be written
  * @param  DataOut: value to be read
  * @param  DataLength: length of data
  */
static void
SPIx_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_TransmitReceive(&hnucleo_Spi, (uint8_t*) DataIn, DataOut, DataLength, SpixTimeout);

  /* Check the communication status */
  if (status != HAL_OK) {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI error treatment function.
  */
static void
SPIx_Error(void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hnucleo_Spi);

  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}

/* To be defined only if provided with the Adafruit MicroSD Breakout Board+ */
/* https://www.adafruit.com/product/254 */
#ifdef SPI_SD_ENABLED                   /* Kernel Config 'options' definition. */

/******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/********************************* LINK SD ************************************/
/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for
  *         data transfer).
  */
void
SD_IO_Init(void)
{
  uint8_t counter;

  /* SD_CS_GPIO Periph clock enable */
  LL_GPIO_EnableClock(SD_CS_GPIO_PORT);

  /* Configure SD_CS_PIN pin: SD Card CS pin */
  LL_GPIO_SetPinMode(SD_CS_GPIO_PORT, SD_CS_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetAFPin(SD_CS_GPIO_PORT, SD_CS_PIN, NUCLEO_SPIx_MISO_MOSI_AF);
  LL_GPIO_SetPinSpeed(SD_CS_GPIO_PORT, SD_CS_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinPull(SD_CS_GPIO_PORT, SD_CS_PIN, LL_GPIO_PULL_UP);
  LL_GPIO_SetPinOutputType(SD_CS_GPIO_PORT, SD_CS_PIN, LL_GPIO_OUTPUT_PUSHPULL);

  /*------------Put SD in SPI mode--------------*/
  /* SD SPI Config */
  SPIx_Init();

  /* SD chip select high */
  SD_CS_HIGH();

  /* Send dummy byte 0xFF, 10 times with CS high */
  /* Rise CS and MOSI for 80 clocks cycles */
  for (counter = 0; counter <= 9; counter++) {
    /* Send dummy byte 0xFF */
    SD_IO_WriteByte(0xFF);
  }
}

/**
  * @brief Set the SD_CS pin.
  * @param val: pin value.
  */
void
SD_IO_CSState(uint8_t val)
{
  if (val == 1) {
    SD_CS_HIGH();
  } else {
    SD_CS_LOW();
  }
}

/**
  * @brief Write a byte on the SD.
  * @param  DataIn: value to be written
  * @param  DataOut: value to be read
  * @param  DataLength: length of data
  */
void
SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  /* Send the byte */
  SPIx_WriteReadData(DataIn, DataOut, DataLength);
}

/**
  * @brief  Writes a byte on the SD.
  * @param  Data: byte to send.
  */
uint8_t
SD_IO_WriteByte(uint8_t Data)
{
  uint8_t tmp;
  /* Send the byte */
  SPIx_WriteReadData(&Data, &tmp, 1);
  return tmp;
}

#endif /* SPI_SD_ENABLED */

#endif /* HAL_SPI_MODULE_ENABLED */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
