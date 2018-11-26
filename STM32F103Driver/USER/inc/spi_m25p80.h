/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_M25P80_H
#define __SPI_M25P80_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


/* M25P SPI Flash supported commands */
#define FiscalMem_CMD_WRITE          0x02  /* Write to Memory instruction */
#define FiscalMem_CMD_WRSR           0x01  /* Write Status Register instruction */
#define FiscalMem_CMD_WREN           0x06  /* Write enable instruction */
#define FiscalMem_CMD_READ           0x03  /* Read from Memory instruction */
#define FiscalMem_CMD_RDSR           0x05  /* Read Status Register instruction  */
#define FiscalMem_CMD_RDID           0x9F  /* Read identification */
#define FiscalMem_CMD_SE             0xD8  /* Sector Erase instruction */
#define FiscalMem_CMD_BE             0xC7  /* Bulk Erase instruction */

#define FiscalMem_WIP_FLAG           0x01  /* Write In Progress (WIP) flag */

#define FiscalMem_DUMMY_BYTE         0xA5

#define FiscalMem_SPI_MAXADDR       	0xFFFFF			/* 定义芯片内部最大地址         */
#define FiscalMem_SPI_MAXSECNUM      15         	/* 定义最大扇区号               */

#define FiscalMem_SPI_PAGESIZE       0x100				/* 页大小:256Byte                */
#define FiscalMem_SPI_SECTORSIZE     0x10000			/* 扇区大小:64KB                */
#define FiscalMem_SEC_MAX         (FiscalMem_SPI_MAXADDR+1)/FiscalMem_SPI_SECTORSIZE

#define TIMEOUT_WR  							1200000     /* 写数据超时限制 10ms*/
#define TIMEOUT_SE  							120000000 	/* 扇区擦除超时限制 1s*/
#define TIMEOUT_BE  							1200000 		/* 芯片擦除超时限制   */

/* M25P FLASH SPI Interface pins  */
#define FiscalMem_SPI                           SPI2

/* Exported macro ------------------------------------------------------------*/
/* Select sFLASH: Chip Select pin low */
#define FiscalMem_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
/* Deselect sFLASH: Chip Select pin high */
#define FiscalMem_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)

/* Exported functions ------------------------------------------------------- */

/* High layer functions  */
uint8_t FMDetect(void);
void  FM_Init (void);
void FiscalMem_DeInit(void);
void FiscalMem_Init(void);
uint8_t FiscalMem_Erase(uint32_t StartSector, uint32_t EndSector);
uint8_t FiscalMem_EraseSector(uint32_t SectorNo);
uint8_t FiscalMem_EraseBulk(void);
uint8_t FiscalMem_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
uint8_t FiscalMem_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
uint8_t FiscalMem_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t FiscalMem_ReadID(void);
void FiscalMem_StartReadSequence(uint32_t ReadAddr);

/* Low layer functions */
uint8_t FiscalMem_ReadByte(void);
uint8_t FiscalMem_SendByte(uint8_t byte);
uint16_t FiscalMem_SendHalfWord(uint16_t HalfWord);
void FiscalMem_WriteEnable(void);
uint8_t FiscalMem_WaitForWriteEnd(void);

uint8_t WriteBlock(uint32_t addr,char *buff,uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_M25P80_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



