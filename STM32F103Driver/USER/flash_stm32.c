
/* Includes ------------------------------------------------------------------*/
#include "flash_stm32.h"

/* Private define ------------------------------------------------------------*/
/* Define the STM32F10x FLASH Page Size depending on the used STM32 device */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

//设置片上Flash高地址空间10Kbytes
#define DATA_START_ADDR  ((uint32_t)0x0807D800)	//page251
#define DATA_END_ADDR    ((uint32_t)0x0807F800)	//page255

#ifdef STM32F10X_XL
 #define BANK2_WRITE_START_ADDR   ((uint32_t)0x08088000)
 #define BANK2_WRITE_END_ADDR     ((uint32_t)0x0808C000)
#endif /* STM32F10X_XL */

//
const uint32_t STM32FlashOffset[5] =
{
	0x0807D800,
	0x0807E000,
	0x0807E800,
	0x0807F000,
	0x0807F800
};

//擦除多个Page
uint8_t STMFlash_EraseMultiPages(uint32_t StartPageAddr, uint32_t EndPageAddr)
{
	uint8_t NbrOfPage, EraseCounter;
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	
	if((StartPageAddr<DATA_START_ADDR) || (EndPageAddr>DATA_END_ADDR) || (StartPageAddr > EndPageAddr))
		return (ERROR);

	/* Unlock the Flash Bank1 Program Erase controller */
	FLASH_Unlock();

	/* Define the number of page to be erased */
	NbrOfPage = (EndPageAddr - StartPageAddr) / FLASH_PAGE_SIZE;

	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	/* Erase the FLASH pages */
	for(EraseCounter = 0; EraseCounter < NbrOfPage; EraseCounter++)
	{
		FLASHStatus = FLASH_ErasePage(DATA_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
		if(FLASHStatus != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return (ERROR);
		}
	}
	
	FLASH_Lock();

	return (SUCCESS);
}


//
uint8_t STMFlash_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumBytesToWrite)
{
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	
	if((WriteAddr<DATA_START_ADDR) || (WriteAddr>DATA_END_ADDR) || ((WriteAddr+NumBytesToWrite)> DATA_END_ADDR))
		return (ERROR);
	
	if(WriteAddr & 0x01)
		return (ERROR);
	
	FLASH_Unlock();
	
  for (; NumBytesToWrite >=2; NumBytesToWrite-=2) 
  {
    FLASHStatus = FLASH_ProgramHalfWord(WriteAddr, *(uint16_t *)pBuffer);
		if(FLASHStatus != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return (ERROR);
		}
    WriteAddr += 2;
		pBuffer ++;
  }
	
  FLASH_Lock();	
	return (SUCCESS);
}


//
uint8_t STMFlash_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumBytesToRead)
{
	volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	
	if((ReadAddr<DATA_START_ADDR) || (ReadAddr>DATA_END_ADDR) || ((ReadAddr+NumBytesToRead)> DATA_END_ADDR))
		return (ERROR);
	
	if(ReadAddr & 0x01)
		return (ERROR);
	
  for (; NumBytesToRead >=2; NumBytesToRead-=2) 
  {
		*pBuffer = (*(__IO uint16_t*) ReadAddr);
    ReadAddr += 2;
		pBuffer ++;
  }
	
	return (SUCCESS);
}
