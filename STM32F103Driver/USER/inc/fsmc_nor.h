/**
  ******************************************************************************
  * @file    stm3210e_eval_fsmc_nor.h
  * @author  MCD Application Team
  * @version V4.5.0
  * @date    07-March-2011
  * @brief   This file contains all the functions prototypes for the 
  *          stm3210e_eval_fsmc_nor firmware driver.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSMC_NOR_H
#define __FSMC_NOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

	 
#define Bank1_SRAM1_ADDR  ((uint32_t)0x60000000)
#define Bank1_SRAM2_ADDR  ((uint32_t)0x64000000)
#define Bank1_SRAM3_ADDR  ((uint32_t)0x68000000)
#define Bank1_SRAM4_ADDR  ((uint32_t)0x6C000000)

// FSMC外设地址
#define NOR1_FLASH_ADDR		Bank1_SRAM1_ADDR	//存放字库
#define NOR2_FLASH_ADDR  	Bank1_SRAM2_ADDR	//EJ
#define PRT_PH_ADDR				(Bank1_SRAM3_ADDR)
#define PRT_DST_ADDR			(Bank1_SRAM3_ADDR + 0x40000)
#define KB_ADDR						(Bank1_SRAM3_ADDR + 0x80000)

//#define NOR_FLASH_ADDR		NOR1_FLASH_ADDR	 
	 
/** @defgroup STM3210E_EVAL_FSMC_NOR_Exported_Macros
  * @{
  */ 
#define  CMD_LONGADDR	0		//=1:SST39VF1601, =0:SST39VF1601C

#define  MACRONIX			0xC2
#define  MX29LV160DB 	0x2249	
#define  MX29LV160DT	0x22C4

#define	 MICROCHIP		0xBF
#define	 SST39VF1601	0x234B
#define	 SST39VF1602	0x234A
#define	 SST39VF1601C	0x234F
#define	 SST39VF1602C	0x234E


#define 	FLASH_AppRAM	0


//字库地址和数据空间
//#define FLASH_HZK16_ADDR				0
//#define FLASH_HZK16_SIZE				0x40000		//128Kx16bit	实际占用:0---0x3FC60
//#define FLASH_HZK16_BLOCKFr		0
//#define FLASH_HZK16_BLOCKS		7			//128Kx16bit	实际占用:0---0x3FC60


//#define FLASH_HZK24_ADDR 				0x40000
//#define FLASH_HZK24_SIZE				0x80000		//256Kx16bit	实际占用:0x40000---0xBF140
//#define FLASH_HZK24_BLOCKFr		7
//#define FLASH_HZK24_BLOCKS		8			//256Kx16bit	实际占用:0x40000---0xBF140

//SST39VF1601 SST39VF1601C SectorSize：
#define NOR_FLASH_SECTOR_SIZE		0x1000		//4KB 2KWord
#define NOR_FLASH_SIZE					0x200000	//2MB


/** @defgroup STM3210E_EVAL_FSMC_NOR_Exported_Types
  * @{
  */
typedef struct
{
  uint16_t Manufacturer_Code;
  uint16_t Device_Code1;
  uint16_t Device_Code2;
  uint16_t Device_Code3;
}NOR_IDTypeDef;

/* NOR Status */
typedef enum
{
  NOR_SUCCESS = 0,
  NOR_ONGOING,
  NOR_ERROR,
  NOR_TIMEOUT
}NOR_Status;  
/**
  * @}
  */
  

/**
  * @}
  */ 

/** @defgroup STM3210E_EVAL_FSMC_NOR_Exported_Functions
  * @{
  */ 
void FSMC_Init(void);
void NOR1_ReadID(NOR_IDTypeDef* NOR_ID);
NOR_Status NOR1_EraseSector(uint32_t SectorAddr);
NOR_Status NOR1_EraseBlock(uint32_t BlockAddr);
NOR_Status NOR1_EraseChip(void);
NOR_Status NOR1_WriteHalfWord(uint32_t WriteAddr, uint16_t Data);
NOR_Status NOR1_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
NOR_Status NOR1_ProgramBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
uint16_t NOR1_ReadHalfWord(uint32_t ReadAddr);
uint8_t NOR1_ReadByte(uint32_t ReadAddr);
void NOR1_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);
NOR_Status NOR1_ReturnToReadMode(void);
NOR_Status NOR1_Reset(void);
NOR_Status NOR1_GetStatus(uint32_t Timeout);


void NOR2_ReadID(NOR_IDTypeDef* NOR_ID);
NOR_Status NOR2_EraseSector(uint32_t SectorAddr);
NOR_Status NOR2_EraseBlock(uint32_t BlockAddr);
NOR_Status NOR2_EraseChip(void);
NOR_Status NOR2_WriteHalfWord(uint32_t WriteAddr, uint16_t Data);
NOR_Status NOR2_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
NOR_Status NOR2_ProgramBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
uint16_t NOR2_ReadHalfWord(uint32_t ReadAddr);
uint8_t NOR2_ReadByte(uint32_t ReadAddr);
void NOR2_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);
NOR_Status NOR2_ReturnToReadMode(void);
NOR_Status NOR2_Reset(void);
NOR_Status NOR2_GetStatus(uint32_t Timeout);

NOR_Status NOR2_ProgramBufferB( uint32_t ulOff, uint32_t NumBytes,uint8_t *ucArrayPointer );
NOR_Status NOR2_ReadBufferB(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumBytesToRead);

#ifdef __cplusplus
}
#endif

#define FlashReadWord(Addr)     *(__IO uint16_t*)(Addr)
#define FlashReadByte(Addr)     *(__IO uint8_t*)(Addr)


#endif /* __FSMC_NOR_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
