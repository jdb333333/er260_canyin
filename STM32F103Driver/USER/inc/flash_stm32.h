#ifndef __FLASH_STM32_H
#define __FLASH_STM32_H

#include "stm32f10x.h"

extern const uint32_t STM32FlashOffset[5];

uint8_t STMFlash_EraseMultiPages(uint32_t StartPageAddr, uint32_t EndPageAddr);
uint8_t STMFlash_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumBytesToWrite);
uint8_t STMFlash_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumBytesToRead);


#endif //__FLASH_STM32_H

