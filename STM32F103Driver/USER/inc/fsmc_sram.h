
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSMC_SRAM_H
#define __FSMC_SRAM_H


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
#define NOR_FLASH_ADDR	Bank1_SRAM1_ADDR
#define SRAM_ADDR  		Bank1_SRAM2_ADDR
#define PRT_PH_ADDR		(Bank1_SRAM3_ADDR)
#define PRT_DST_ADDR	(Bank1_SRAM3_ADDR + 0x40000)
#define KB_ADDR			(Bank1_SRAM3_ADDR + 0x80000)

// External SRAM memory size
//#define SRAMSIZE	(0x80000)	//使用1片512KB SRAM EM643FV16F
//#define SRAMSIZE	(0x40000)	//使用1片256KB SRAM EM621FV16B

#define	SRAMSIZE_SHIFT	0 //1
#if SRAMSIZE_SHIFT //只在硬件设计1片512KB却焊接1片256KB芯片时使用。
#define SRAM_8KB	0x2000 //外部存储器地址A12需要移位.(内部AHB地址HADDR13对应外部存储器地址FSMC A12.
#define SRAM_A12_SHIFT(WriteAddr)	{ if(WriteAddr & 0x2000){ \
										WriteAddr &= ~0x2000; \
										WriteAddr += 0x4000;} \
									} //HADDR13若为1，则将HADDR13清0，在HADDR14上加1
#endif


uint32_t SRAM_Detect(void);

void FSMC_Init(void);
void SRAM_Move(uint32_t toAddr, uint32_t fromAddr, int32_t BytesToMove);
void SRAM_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void SRAM_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

#ifdef __cplusplus
}
#endif

#endif /* __FSMC_SRAM_H */

