/*
 * Copyright (c) 2016, 2017 STMicroelectronics
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
 */

#ifndef	_MACHINE_STM32_BSP_SD_H_
#define	_MACHINE_STM32_BSP_SD_H_

#include <stm32/hal/stm32f4xx_hal.h>

#define	SD_DATATIMEOUT	((uint32_t)100000000)

enum {
	BSP_SD_OK = 0x00,
	BSP_SD_ERROR = 0x01,
	BSP_SD_TIMEOUT
};

enum {
	SD_TRANSFER_OK = 0x00,
	SD_TRANSFER_BUSY = 0x01
};

#ifdef	SPI_SD_ENABLED			/* Kernel Config 'options' definition. */

typedef struct {
	uint8_t		Reserved1:2;		/* Reserved */
	uint16_t	DeviceSize:12;		/* Device Size */
	uint8_t		MaxRdCurrentVDDMin:3;	/* Max. read current @ VDD min */
	uint8_t		MaxRdCurrentVDDMax:3;	/* Max. read current @ VDD max */
	uint8_t		MaxWrCurrentVDDMin:3;	/* Max. write current @ VDD min */
	uint8_t		MaxWrCurrentVDDMax:3;	/* Max. write current @ VDD max */
	uint8_t		DeviceSizeMul:3;	/* Device size multiplier */
} struct_v1;

typedef struct {
	uint8_t		Reserved1:6;		/* Reserved */
	uint32_t	DeviceSize:22;		/* Device Size */
	uint8_t		Reserved2:1;		/* Reserved */
} struct_v2;

typedef struct {
	/* Header part */
	uint8_t		CSDStruct:2;		/* CSD structure */
	uint8_t		Reserved1:6;		/* Reserved */
	uint8_t		TAAC:8;			/* Data read access-time 1 */
	uint8_t		NSAC:8;			/* Data read access-time 2 in CLK cycles */
	uint8_t		MaxBusClkFrec:8;	/* Max. bus clock frequency */
	uint16_t	CardComdClasses:12;	/* Card command classes */
	uint8_t		RdBlockLen:4;		/* Max. read data block length */
	uint8_t		PartBlockRead:1;	/* Partial blocks for read allowed */
	uint8_t		WrBlockMisalign:1;	/* Write block misalignment */
	uint8_t		RdBlockMisalign:1;	/* Read block misalignment */
	uint8_t		DSRImpl:1;		/* DSR implemented */

	/* v1 or v2 struct */
	union csd_version {
		struct_v1	v1;
		struct_v2	v2;
	} version;

	uint8_t		EraseSingleBlockEnable:1;	/* Erase single block enable */
	uint8_t		EraseSectorSize:7;	/* Erase group size multiplier */
	uint8_t		WrProtectGrSize:7;	/* Write protect group size */
	uint8_t		WrProtectGrEnable:1;	/* Write protect group enable */
	uint8_t		Reserved2:2;		/* Reserved */
	uint8_t		WrSpeedFact:3;		/* Write speed factor */
	uint8_t		MaxWrBlockLen:4;	/* Max. write data block length */
	uint8_t		WriteBlockPartial:1;	/* Partial blocks for write allowed */
	uint8_t		Reserved3:5;		/* Reserved */
	uint8_t		FileFormatGroup:1;	/* File format group */
	uint8_t		CopyFlag:1;		/* Copy flag (OTP) */
	uint8_t		PermWrProtect:1;	/* Permanent write protection */
	uint8_t		TempWrProtect:1;	/* Temporary write protection */
	uint8_t		FileFormat:2;		/* File Format */
	uint8_t		Reserved4:2;		/* Reserved */
	uint8_t		crc:7;			/* Reserved */
	uint8_t		Reserved5:1;		/* always 1 */
} SD_CSD;

typedef struct {
	__IO uint8_t	ManufacturerID;		/* ManufacturerID */
	__IO uint16_t	OEM_AppliID;		/* OEM/Application ID */
	__IO uint32_t	ProdName1;		/* Product Name part1 */
	__IO uint8_t	ProdName2;		/* Product Name part2 */
	__IO uint8_t	ProdRev;		/* Product Revision */
	__IO uint32_t	ProdSN;			/* Product Serial Number */
	__IO uint8_t	Reserved1;		/* Reserved1 */
	__IO uint16_t	ManufactDate;		/* Manufacturing Date */
	__IO uint8_t	CID_CRC;		/* CID CRC */
	__IO uint8_t	Reserved2;		/* always 1 */
} SD_CID;

typedef struct {
	SD_CSD		Csd;
	SD_CID		Cid;
	uint32_t	CardCapacity;		/*!< Card Capacity */
	uint32_t	CardBlockSize;		/*!< Card Block Size */
	uint32_t	LogBlockNbr;		/*!< Specifies the Card logical Capacity in blocks */
	uint32_t	LogBlockSize;		/*!< Specifies logical block size in bytes */
} SD_CardInfo;

#define	BSP_SD_CardInfo	SD_CardInfo

/* Link external functions for SD Card peripheral */
extern void	SD_IO_Init(void);
extern void	SD_IO_CSState(uint8_t state);
extern void	SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
extern uint8_t	SD_IO_WriteByte(uint8_t Data);

/* Link function for HAL delay */
void	HAL_Delay(__IO uint32_t Delay);

#endif	/* SPI_SD_ENABLED */

#ifdef	SDIO_ENABLED			/* Kernel Config 'controller' definition. */

#define	BSP_SD_CardInfo	HAL_SD_CardInfoTypeDef

void	BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params);
void	BSP_SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params);

#endif	/* SDIO_ENABLED */

uint8_t	BSP_SD_Init(void);
uint8_t	BSP_SD_DeInit(void);
uint8_t	BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t	BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t	BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t	BSP_SD_GetCardState(void);
uint8_t	BSP_SD_GetCardInfo(BSP_SD_CardInfo *pCardInfo);

#endif	/* !_MACHINE_STM32_BSP_SD_H_ */
