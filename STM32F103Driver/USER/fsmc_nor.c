/**
  ******************************************************************************
  * @file    stm3210e_eval_fsmc_nor.c
  * @author  MCD Application Team
  * @version V4.5.0
  * @date    07-March-2011
  * @brief   This file provides a set of functions needed to drive the M29W128FL,
  *          M29W128GL and S29GL128P NOR memories mounted on STM3210E-EVAL board.
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

/* Includes ------------------------------------------------------------------*/
#include "fsmc_nor.h"

#define CWORD(a) (*(uint16_t*)&a)

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup STM3210E_EVAL
  * @{
  */

/** @addtogroup STM3210E_EVAL_FSMC_NOR
  * @brief      This file provides a set of functions needed to drive the M29W128FL,
  *             M29W128GL and S29GL128P NOR memories mounted on STM3210E-EVAL board.
  * @{
  */

/** @defgroup STM3210E_EVAL_FSMC_NOR_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STM3210E_EVAL_FSMC_NOR_Private_Defines
  * @{
  */
/**
  * @brief  FSMC Bank 1 NOR/SRAM2
  */

/* Delay definition */
#define SectorErase_Timeout    ((uint32_t)0x00A00000)
#define BlockErase_Timeout    ((uint32_t)0x00A00000)
#define ChipErase_Timeout     ((uint32_t)0x30000000)
#define Program_Timeout       ((uint32_t)0x00001400)
/**
  * @}
  */


/** @defgroup STM3210E_EVAL_FSMC_NOR_Private_Macros
  * @{
  */
#define ADDR1_SHIFT(A) (NOR1_FLASH_ADDR + (2 * (A)))
#define ADDR2_SHIFT(A) (NOR2_FLASH_ADDR + (2 * (A)))
#define NOR_WRITE(Address, Data)  (*(__IO uint16_t *)(Address) = (Data))
/**
  * @}
  */


/** @defgroup STM3210E_EVAL_FSMC_NOR_Private_Variables
  * @{
  */
/**
  * @}
  */


/** @defgroupSTM3210E_EVAL_FSMC_NOR_Private_Function_Prototypes
  * @{
  */
/**
  * @}
  */


/** @defgroup STM3210E_EVAL_FSMC_NOR_Private_Functions
  * @{
  */

/**
  * @brief  Configures the FSMC and GPIOs to interface with the NOR memory.
  *         This function must be called before any write/read operation
  *         on the NOR.
  * @param  None
  * @retval None
  */
void FSMC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

    /* Enable GPIOs clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |
										 RCC_APB2Periph_GPIOF, ENABLE);

/*-- GPIOs Configuration -----------------------------------------------------*/
/*
 +-------------------+--------------------+------------------+------------------+
 | PD0  <-> FSMC_D2  | PE0  <-> FSMC_NBL0 | PF0 <-> FSMC_A0  | PG0 <-> FSMC_A10 |
 | PD1  <-> FSMC_D3  | PE1  <-> FSMC_NBL1 | PF1 <-> FSMC_A1  | PG1 <-> FSMC_A11 |
 | PD4  <-> FSMC_NOE | PE2  <-> --------- | PF2 <-> FSMC_A2  | PG2 <-> FSMC_A12 |
 | PD5  <-> FSMC_NWE | PE3  <-> FSMC_A19  | PF3 <-> FSMC_A3  | PG3 <-> FSMC_A13 |
 | PD8  <-> FSMC_D13 | PE4  <-> FSMC_A20  | PF4 <-> FSMC_A4  | PG4 <-> FSMC_A14 |
 | PD9  <-> FSMC_D14 | PE5  <-> FSMC_A21  | PF5 <-> FSMC_A5  | PG5 <-> FSMC_A15 |
 | PD10 <-> FSMC_D15 | PE6  <-> --------- | PF12 <-> FSMC_A6 | PG9 <-> FSMC_NE2 |
 | PD11 <-> FSMC_A16 | PE7  <-> FSMC_D4   | PF13 <-> FSMC_A7 | PG10 <-> FSMC_NE3 |
 | PD12 <-> FSMC_A17 | PE8  <-> FSMC_D5   | PF14 <-> FSMC_A8 |
 | PD13 <-> FSMC_A18 | PE9  <-> FSMC_D6   | PF15 <-> FSMC_A9 |
 | PD14 <-> FSMC_D0  | PE10 <-> FSMC_D7   |------------------+
 | PD15 <-> FSMC_D1  | PE11 <-> FSMC_D8   |
 +-------------------| PE12 <-> FSMC_D9   |
   PD7  <-> FSMC_NE1 | PE13 <-> FSMC_D10  |
                     | PE14 <-> FSMC_D11  |
                     | PE15 <-> FSMC_D12  |
                     +--------------------+
*/

	/* GPIOD configuration */
	// GPIOD相关引脚配置成FSMC功能、推挽、无上下拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_7 |
																GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11 |
																GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* GPIOE configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_3 |
																GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_7 |
																GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11|
																GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* GPIOF configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2 | GPIO_Pin_3 |
																GPIO_Pin_4  | GPIO_Pin_5  |
																GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* GPIOG configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2 | GPIO_Pin_3 |
																GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/*-- FSMC Configuration ------------------------------------------------------*/
	//------------FSMC_Bank1_NORSRAM10--------------------------------------------//
	p.FSMC_AddressSetupTime = 2;      //地址建立时间 0 5
	p.FSMC_AddressHoldTime = 0;       //地址保持时间 0 3
	p.FSMC_DataSetupTime = 10;         //数据建立时间 5 6
	p.FSMC_BusTurnAroundDuration = 0; //1
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_B;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;                    // SRAM使用FSMC_NE2片选
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;  // 不使用数据线/地址线复用模式
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;            // 存储器类型:PSRAM
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;    // 存储器宽度16bit
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;      // 扩展模式(读、写使用不同的时序)
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	//------------FSMC_Bank1_NORSRAM2--------------------------------------------//
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	//------------FSMC_Bank1_NORSRAM3 配置外设-----------------------------------//
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	//------------使能FSMC_Bank1_NORSRAM1、2、3、4 ------------------------------//
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
}

/**
  * @brief  Reads NOR memory's Manufacturer and Device Code.
  * @param  NOR_ID: pointer to a NOR_IDTypeDef structure which will hold the
  *         Manufacturer and Device Code.
  * @retval None
  */
void NOR1_ReadID(NOR_IDTypeDef* NOR_ID)
{
#if CMD_LONGADDR
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x0090);

#else
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x0090);
#endif

  NOR_ID->Manufacturer_Code = *(__IO uint16_t *) ADDR1_SHIFT(0x0000);
  NOR_ID->Device_Code1 = *(__IO uint16_t *) ADDR1_SHIFT(0x0001);
  //NOR_ID->Device_Code2 = *(__IO uint16_t *) ADDR_SHIFT(0x000E);
  //NOR_ID->Device_Code3 = *(__IO uint16_t *) ADDR_SHIFT(0x000F);

  NOR1_ReturnToReadMode();
}

/**
  * @brief  Erases the specified Nor memory Sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
//新增-扇区擦除
NOR_Status NOR1_EraseSector(uint32_t SectorAddr)
{
  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

#if CMD_LONGADDR
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x0080);
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x0080);
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
#endif

  NOR_WRITE((NOR1_FLASH_ADDR + SectorAddr), 0x30);

  return (NOR1_GetStatus(SectorErase_Timeout));
}


/**
  * @brief  Erases the specified Nor memory block.
  * @param  BlockAddr: address of the block to erase.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR1_EraseBlock(uint32_t BlockAddr)
{
  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

#if CMD_LONGADDR
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x0080);
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x0080);
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
#endif

  NOR_WRITE((NOR1_FLASH_ADDR + BlockAddr), 0x50);

  return (NOR1_GetStatus(BlockErase_Timeout));
}

/**
  * @brief  Erases the entire chip.
  * @param  None
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR1_EraseChip(void)
{
  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

#if CMD_LONGADDR
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x0080);
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x0010);

#else
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x0080);
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x0010);
#endif

  return (NOR1_GetStatus(ChipErase_Timeout));
}

/**
  * @brief  Writes a half-word to the NOR memory.
  * @param  WriteAddr: NOR memory internal address to write to.
  * @param  Data: Data to write.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR1_WriteHalfWord(uint32_t WriteAddr, uint16_t Data)
{
  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

#if CMD_LONGADDR
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00A0);

#else
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00A0);
#endif

  NOR_WRITE((NOR1_FLASH_ADDR + WriteAddr), Data);

  return (NOR1_GetStatus(Program_Timeout));
}

/**
  * @brief  Writes a half-word buffer to the FSMC NOR memory.
  * @param  pBuffer: pointer to buffer.
  * @param  WriteAddr: NOR memory internal address from which the data will be
  *         written.
  * @param  NumHalfwordToWrite: number of Half words to write.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR1_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
  NOR_Status status = NOR_ONGOING;

  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

  do
  {
    /*!< Transfer data to the memory */
    status = NOR1_WriteHalfWord(WriteAddr, *pBuffer++);
    WriteAddr = WriteAddr + 2;
    NumHalfwordToWrite--;
  }
  while((status == NOR_SUCCESS) && (NumHalfwordToWrite != 0));

  return (status);
}

/**
  * @brief  Writes a half-word buffer to the FSMC NOR memory. This function
  *         must be used only with S29GL128P NOR memory.
  * @param  pBuffer: pointer to buffer.
  * @param  WriteAddr: NOR memory internal address from which the data will be
  *         written.
  * @param  NumHalfwordToWrite: number of Half words to write.
  *         The maximum allowed value is 32 Half words (64 bytes).
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR1_ProgramBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
  uint32_t lastloadedaddress = 0x00;
  uint32_t currentaddress = 0x00;
  uint32_t endaddress = 0x00;

  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

  /*!< Initialize variables */
  currentaddress = WriteAddr;
  endaddress = WriteAddr + NumHalfwordToWrite - 1;
  lastloadedaddress = WriteAddr;

  /*!< Issue unlock command sequence */
#if CMD_LONGADDR
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
#endif

  /*!< Write Write Buffer Load Command */
  NOR_WRITE(ADDR1_SHIFT(WriteAddr), 0x0025);
  NOR_WRITE(ADDR1_SHIFT(WriteAddr), (NumHalfwordToWrite - 1));

  /*!< Load Data into NOR Buffer */
  while(currentaddress <= endaddress)
  {
    /*!< Store last loaded address & data value (for polling) */
    lastloadedaddress = currentaddress;

    NOR_WRITE(ADDR1_SHIFT(currentaddress), *pBuffer++);
    currentaddress += 1;
  }

  NOR_WRITE(ADDR1_SHIFT(lastloadedaddress), 0x29);

  return(NOR1_GetStatus(Program_Timeout));
}

/**
  * @brief  Reads a half-word from the NOR memory.
  * @param  ReadAddr: NOR memory internal address to read from.
  * @retval Half-word read from the NOR memory
  */
uint16_t NOR1_ReadHalfWord(uint32_t ReadAddr)
{
#if CMD_LONGADDR
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
#endif
  NOR_WRITE((NOR1_FLASH_ADDR + ReadAddr), 0x00F0 );

  return (*(__IO uint16_t *)((NOR1_FLASH_ADDR + ReadAddr)));
}

/**
  * @brief  Reads a Byte from the NOR memory.
  * @param  ReadAddr: NOR memory internal address to read from.
  * @retval Byte read from the NOR memory
  */
uint8_t NOR1_ReadByte(uint32_t ReadAddr)
{
	uint16_t temp;
	temp = NOR1_ReadHalfWord(ReadAddr);
	if(ReadAddr & 0x0001)
		return (uint8_t)(temp>>8);
	else
		return (uint8_t)(temp&0x00FF);
}

/**
  * @brief  Reads a block of data from the FSMC NOR memory.
  * @param  pBuffer: pointer to the buffer that receives the data read from the
  *         NOR memory.
  * @param  ReadAddr: NOR memory internal address to read from.
  * @param  NumHalfwordToRead : number of Half word to read.
  * @retval None
  */
void NOR1_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
#if CMD_LONGADDR
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
#endif
  NOR_WRITE((NOR1_FLASH_ADDR + ReadAddr), 0x00F0);

  for(; NumHalfwordToRead != 0x00; NumHalfwordToRead--) /*!< while there is data to read */
  {
    /*!< Read a Halfword from the NOR */
    *pBuffer++ = *(__IO uint16_t *)((NOR1_FLASH_ADDR + ReadAddr));
    ReadAddr = ReadAddr + 2;
  }
}

/**
  * @brief  Returns the NOR memory to Read mode.
  * @param  None
  * @retval NOR_SUCCESS
  */
NOR_Status NOR1_ReturnToReadMode(void)
{
  NOR_WRITE(NOR1_FLASH_ADDR, 0x00F0);
  return (NOR_SUCCESS);
}

/**
  * @brief  Returns the NOR memory to Read mode and resets the errors in the NOR
  *         memory Status Register.
  * @param  None
  * @retval NOR_SUCCESS
  */
NOR_Status NOR1_Reset(void)
{
#if CMD_LONGADDR
  NOR_WRITE(ADDR1_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR1_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR1_SHIFT(0x02AA), 0x0055);
#endif
  NOR_WRITE(NOR1_FLASH_ADDR, 0x00F0);

  return (NOR_SUCCESS);
}

/**
  * @brief  Returns the NOR operation status.
  * @param  Timeout: NOR progamming Timeout
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR1_GetStatus(uint32_t Timeout)
{
  uint16_t val1 = 0x00, val2 = 0x00;
  NOR_Status status = NOR_ONGOING;
  uint32_t timeout = Timeout;

  /*!< Poll on NOR memory Ready/Busy signal ----------------------------------*/
  //while((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) != RESET) && (timeout > 0))
  //{
  //  timeout--;
  //}

  //timeout = Timeout;

  //while((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == RESET) && (timeout > 0))
  //{
  //  timeout--;
  //}

  /*!< Get the NOR memory operation status -----------------------------------*/
  //while((Timeout != 0x00) && (status != NOR_SUCCESS))
  while((status != NOR_SUCCESS) && (Timeout != 0x00))
  {
    Timeout--;

    /*!< Read DQ6 and DQ5 */
    val1 = *(__IO uint16_t *)(NOR1_FLASH_ADDR);
    val2 = *(__IO uint16_t *)(NOR1_FLASH_ADDR);

    /*!< If DQ6 did not toggle between the two reads then return NOR_Success */
    if((val1 & 0x0040) == (val2 & 0x0040)) //DQ6
    {
      return NOR_SUCCESS;
    }

#if CMD_LONGADDR
    //if((val1 & 0x0080) != 0x0080)	//DQ7
    //{
    //  status = NOR_ONGOING;
    //}
#else
    if((val1 & 0x0020) != 0x0020)	//DQ5
    {
      status = NOR_ONGOING;
    }
#endif

    val1 = *(__IO uint16_t *)(NOR1_FLASH_ADDR);
    val2 = *(__IO uint16_t *)(NOR1_FLASH_ADDR);

    if((val1 & 0x0040) == (val2 & 0x0040)) //DQ6
    {
      return NOR_SUCCESS;
    }

#if CMD_LONGADDR
    //else if((val1 & 0x0080) == 0x0080)	//DQ7
    //{
    //  return NOR_ERROR;
    //}
#else
    else if((val1 & 0x0020) == 0x0020)	//DQ5
    {
      return NOR_ERROR;
    }
#endif
  }

  if(Timeout == 0x00)
  {
    status = NOR_TIMEOUT;
  }

  /*!< Return the operation status */
  return (status);
}



/******************************************************************************************************
*******************************************************************************************************
*******************************************************************************************************
*******************************************************************************************************
******************************************************************************************************/
/**
  * @brief  Reads NOR memory's Manufacturer and Device Code.
  * @param  NOR_ID: pointer to a NOR_IDTypeDef structure which will hold the
  *         Manufacturer and Device Code.
  * @retval None
  */
void NOR2_ReadID(NOR_IDTypeDef* NOR_ID)
{
#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x0090);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x0090);
#endif

  NOR_ID->Manufacturer_Code = *(__IO uint16_t *) ADDR2_SHIFT(0x0000);
  NOR_ID->Device_Code1 = *(__IO uint16_t *) ADDR2_SHIFT(0x0001);
  //NOR_ID->Device_Code2 = *(__IO uint16_t *) ADDR_SHIFT(0x000E);
  //NOR_ID->Device_Code3 = *(__IO uint16_t *) ADDR_SHIFT(0x000F);

  NOR2_ReturnToReadMode();
}

/**
  * @brief  Erases the specified Nor memory Sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
//新增-扇区擦除
NOR_Status NOR2_EraseSector(uint32_t SectorAddr)
{
  NOR_Status sStatus;
  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x0080);
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x0080);
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
#endif

  NOR_WRITE((NOR2_FLASH_ADDR + SectorAddr), 0x30);

  sStatus = NOR2_GetStatus(SectorErase_Timeout);
  NOR_WRITE(NOR2_FLASH_ADDR, 0x00F0);//ccr2014-10-15
  return (sStatus);

}


/**
  * @brief  Erases the specified Nor memory block.
  * @param  BlockAddr: address of the block to erase.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR2_EraseBlock(uint32_t BlockAddr)
{
    NOR_Status sStatus;

  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x0080);
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x0080);
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
#endif

  NOR_WRITE((NOR2_FLASH_ADDR + BlockAddr), 0x50);


  sStatus = NOR2_GetStatus(BlockErase_Timeout);
  NOR_WRITE(NOR2_FLASH_ADDR, 0x00F0);//ccr2014-10-15
  return (sStatus);

}

/**
  * @brief  Erases the entire chip.
  * @param  None
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR2_EraseChip(void)
{
    NOR_Status sStatus;

  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x0080);
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x0010);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x0080);
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x0010);
#endif

  sStatus = NOR2_GetStatus(ChipErase_Timeout);
  NOR_WRITE(NOR2_FLASH_ADDR, 0x00F0);//ccr2014-10-15
  return (sStatus);

}

/**
  * @brief  Writes a half-word to the NOR memory.
  * @param  WriteAddr: NOR memory internal address to write to.
  * @param  Data: Data to write.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR2_WriteHalfWord(uint32_t WriteAddr, uint16_t Data)
{
    NOR_Status sStatus;

  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00A0);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00A0);
#endif

  NOR_WRITE((NOR2_FLASH_ADDR + WriteAddr), Data);

  sStatus = NOR2_GetStatus(Program_Timeout);
  NOR_WRITE(NOR2_FLASH_ADDR, 0x00F0);//ccr2014-10-15
  return (sStatus);

}

/**
  * @brief  Writes a half-word buffer to the FSMC NOR memory.
  * @param  pBuffer: pointer to buffer.
  * @param  WriteAddr: NOR memory internal address from which the data will be
  *         written.
  * @param  NumHalfwordToWrite: number of Half words to write.
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR2_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
  NOR_Status status = NOR_ONGOING;

  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

  do
  {
    /*!< Transfer data to the memory */
    status = NOR2_WriteHalfWord(WriteAddr, *pBuffer++);
    WriteAddr = WriteAddr + 2;
    NumHalfwordToWrite--;
  }
  while((status == NOR_SUCCESS) && (NumHalfwordToWrite != 0));

  NOR_WRITE(NOR2_FLASH_ADDR, 0x00F0);//ccr2014-10-15
  return (status);
}

/**
  * @brief  Writes a half-word buffer to the FSMC NOR memory. This function
  *         must be used only with S29GL128P NOR memory.
  * @param  pBuffer: pointer to buffer.
  * @param  WriteAddr: NOR memory internal address from which the data will be
  *         written.
  * @param  NumHalfwordToWrite: number of Half words to write.
  *         The maximum allowed value is 32 Half words (64 bytes).
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR2_ProgramBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
    NOR_Status sStatus;

  uint32_t lastloadedaddress;
  uint32_t currentaddress;
  uint32_t endaddress;

  while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

  /*!< Initialize variables */
  currentaddress = WriteAddr;
  endaddress = WriteAddr + NumHalfwordToWrite - 1;
  lastloadedaddress = WriteAddr;

  /*!< Issue unlock command sequence */
#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
#endif

  /*!< Write Write Buffer Load Command */
  NOR_WRITE(ADDR2_SHIFT(WriteAddr), 0x0025);
  NOR_WRITE(ADDR2_SHIFT(WriteAddr), (NumHalfwordToWrite - 1));

  /*!< Load Data into NOR Buffer */
  while(currentaddress <= endaddress)
  {
    /*!< Store last loaded address & data value (for polling) */
    lastloadedaddress = currentaddress;

    NOR_WRITE(ADDR2_SHIFT(currentaddress), *pBuffer++);
    currentaddress += 1;
  }

  NOR_WRITE(ADDR2_SHIFT(lastloadedaddress), 0x29);

  sStatus = NOR2_GetStatus(Program_Timeout);
  NOR_WRITE(NOR2_FLASH_ADDR, 0x00F0);//ccr2014-10-15
  return (sStatus);

}

/**
  * @brief  Reads a half-word from the NOR memory.
  * @param  ReadAddr: NOR memory internal address to read from.
  * @retval Half-word read from the NOR memory
  */
uint16_t NOR2_ReadHalfWord(uint32_t ReadAddr)
{
#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
#endif
  NOR_WRITE((NOR2_FLASH_ADDR + ReadAddr), 0x00F0 );

  return (*(__IO uint16_t *)((NOR2_FLASH_ADDR + ReadAddr)));
}

/**
  * @brief  Reads a Byte from the NOR memory.
  * @param  ReadAddr: NOR memory internal address to read from.
  * @retval Byte read from the NOR memory
  */
uint8_t NOR2_ReadByte(uint32_t ReadAddr)
{
	uint16_t temp;
	temp = NOR2_ReadHalfWord(ReadAddr);
	if(ReadAddr & 0x0001)
		return (uint8_t)(temp>>8);
	else
		return (uint8_t)(temp&0x00FF);
}

/**
  * @brief  Reads a block of data from the FSMC NOR memory.
  * @param  pBuffer: pointer to the buffer that receives the data read from the
  *         NOR memory.
  * @param  ReadAddr: NOR memory internal address to read from.
  * @param  NumHalfwordToRead : number of Half word to read.
  * @retval None
  */
void NOR2_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
#endif
  NOR_WRITE((NOR2_FLASH_ADDR + ReadAddr), 0x00F0);

  for(; NumHalfwordToRead != 0x00; NumHalfwordToRead--) /*!< while there is data to read */
  {
    /*!< Read a Halfword from the NOR */
    *pBuffer++ = *(__IO uint16_t *)((NOR2_FLASH_ADDR + ReadAddr));
    ReadAddr = ReadAddr + 2;
  }
}

/**
  * @brief  Returns the NOR memory to Read mode.
  * @param  None
  * @retval NOR_SUCCESS
  */
NOR_Status NOR2_ReturnToReadMode(void)
{
  NOR_WRITE(NOR2_FLASH_ADDR, 0x00F0);
  return (NOR_SUCCESS);
}

/**
  * @brief  Returns the NOR memory to Read mode and resets the errors in the NOR
  *         memory Status Register.
  * @param  None
  * @retval NOR_SUCCESS
  */
NOR_Status NOR2_Reset(void)
{
#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
#endif
  NOR_WRITE(NOR2_FLASH_ADDR, 0x00F0);

  return (NOR_SUCCESS);
}

/**
  * @brief  Returns the NOR operation status.
  * @param  Timeout: NOR progamming Timeout
  * @retval NOR_Status: The returned value can be: NOR_SUCCESS, NOR_ERROR
  *         or NOR_TIMEOUT
  */
NOR_Status NOR2_GetStatus(uint32_t Timeout)
{
  uint16_t val1 = 0x00, val2 = 0x00;
  NOR_Status status = NOR_ONGOING;
  uint32_t timeout = Timeout;

  /*!< Poll on NOR memory Ready/Busy signal ----------------------------------*/
  //while((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) != RESET) && (timeout > 0))
  //{
  //  timeout--;
  //}

  //timeout = Timeout;

  //while((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == RESET) && (timeout > 0))
  //{
  //  timeout--;
  //}

  /*!< Get the NOR memory operation status -----------------------------------*/
  //while((Timeout != 0x00) && (status != NOR_SUCCESS))
  while((status != NOR_SUCCESS) && (Timeout != 0x00))
  {
    Timeout--;

    /*!< Read DQ6 and DQ5 */
    val1 = *(__IO uint16_t *)(NOR2_FLASH_ADDR);
    val2 = *(__IO uint16_t *)(NOR2_FLASH_ADDR);

    /*!< If DQ6 did not toggle between the two reads then return NOR_Success */
    if((val1 & 0x0040) == (val2 & 0x0040)) //DQ6
    {
      return NOR_SUCCESS;
    }

#if CMD_LONGADDR
    //if((val1 & 0x0080) != 0x0080)	//DQ7
    //{
    //  status = NOR_ONGOING;
    //}
#else
    if((val1 & 0x0020) != 0x0020)	//DQ5
    {
      status = NOR_ONGOING;
    }
#endif

    val1 = *(__IO uint16_t *)(NOR2_FLASH_ADDR);
    val2 = *(__IO uint16_t *)(NOR2_FLASH_ADDR);

    if((val1 & 0x0040) == (val2 & 0x0040)) //DQ6
    {
      return NOR_SUCCESS;
    }

#if CMD_LONGADDR
    //else if((val1 & 0x0080) == 0x0080)	//DQ7
    //{
    //  return NOR_ERROR;
    //}
#else
    else if((val1 & 0x0020) == 0x0020)	//DQ5
    {
      return NOR_ERROR;
    }
#endif
  }

  if(Timeout == 0x00)
  {
    status = NOR_TIMEOUT;
  }

  /*!< Return the operation status */
  return (status);
}



/**ccr2014-10-14>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 * 以字节方式将数据写入nor flash中
 *
 * @author EutronSoftware (2014-10-15)
 *
 * @param ulOff:字节地址
 * @param NumBytes:字节数目
 * @param ucArrayPointer:字节数据
 *
 * @return NOR_Status
 */
NOR_Status NOR2_ProgramBufferB( uint32_t ulOff, uint32_t NumBytes,uint8_t *ucArrayPointer )
{
    NOR_Status sStatus;

	uint16_t sWord;
    uint32_t sNumWords;
    uint32_t lastloadedaddress;
    uint32_t currentaddress;
    uint32_t endaddress;

//	while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash

	endaddress = ulOff+NumBytes-1;
	if( endaddress >= NOR_FLASH_SIZE)
		return NOR_ERROR;
	if( !NumBytes )
		return NOR_SUCCESS;

    currentaddress = ulOff/2;

	if( ulOff & 0x01 )
	{//Program a BYTE in odd-address
		sWord = (*(uint16_t*)ucArrayPointer);
        sWord<<=8;
		sWord |= 0x00ff;
        NOR2_ProgramBuffer(&sWord,currentaddress,1);
		ulOff++;
		ucArrayPointer++;
		NumBytes--;
        currentaddress++;
	}

	if( !NumBytes )
		return NOR_SUCCESS;


  sNumWords = NumBytes/2;
  if ((NumBytes & 1))
        sNumWords++;

  endaddress = currentaddress + sNumWords - 1;
  lastloadedaddress = currentaddress;

  /*!< Issue unlock command sequence */
#if CMD_LONGADDR
  NOR_WRITE(ADDR2_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x2AAA), 0x0055);

#else
  NOR_WRITE(ADDR2_SHIFT(0x0555), 0x00AA);
  NOR_WRITE(ADDR2_SHIFT(0x02AA), 0x0055);
#endif

  /*!< Write Write Buffer Load Command */
  ulOff += NOR2_FLASH_ADDR;

  NOR_WRITE(ulOff, 0x0025);
  NOR_WRITE(ulOff, (sNumWords - 1));

  /*!< Load Data into NOR Buffer */
  while(NumBytes>1)//currentaddress <= endaddress)
  {
    /*!< Store last loaded address & data value (for polling) */
    lastloadedaddress = currentaddress;

    NOR_WRITE(ulOff, (*(uint16_t*)ucArrayPointer));
    ucArrayPointer+=2;
    currentaddress += 1;
    ulOff+=2;
    NumBytes-=2;
  }

  if( NumBytes==1 )
  {//Program the last byte in even-address
      sWord = *ucArrayPointer;
      sWord |= 0xff00;
      NOR_WRITE(ulOff, sWord);
  }

  NOR_WRITE(ADDR2_SHIFT(lastloadedaddress), 0x29);

  sStatus = NOR2_GetStatus(Program_Timeout);
  NOR_WRITE(NOR2_FLASH_ADDR, 0x00F0);//ccr2014-10-15
  return (sStatus);

}

/**按字读取数据
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */

NOR_Status NOR2_ReadBufferW(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
	ReadAddr+=NOR2_FLASH_ADDR;
	for (; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
	{
		/* Read a half-word from the memory */
		*pBuffer++ = FlashReadWord(ReadAddr);

		ReadAddr+=2;
	}
	return NOR_SUCCESS;
}

/**按字节读取数据,符合字地址时,则按字读取,提高读取效率
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
NOR_Status NOR2_ReadBufferB(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumBytesToRead)
{
	uint32_t sBytesToRead;

	ReadAddr+=NOR2_FLASH_ADDR;

	if( NumBytesToRead )
	{
		if( ReadAddr & 1 )
		{//read a byte in odd-address
			*pBuffer++ = FlashReadByte(ReadAddr);
			ReadAddr++;
			NumBytesToRead--;
		}
		sBytesToRead = NumBytesToRead & 1;
		NumBytesToRead >>=1;
		for (; NumBytesToRead != 0; NumBytesToRead--) /* while there is data to read */
		{//read word
			/* Read a half-word from the memory */
			CWORD(pBuffer[0]) = FlashReadWord(ReadAddr);
			pBuffer+=2;
			ReadAddr+=2;
		}
		if( sBytesToRead )
		{//read the last byte
			*pBuffer = FlashReadByte(ReadAddr);
		}
	}
	return NOR_SUCCESS;
}
//ccr2014-10-14<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
