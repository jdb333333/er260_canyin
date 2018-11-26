
/* Includes ------------------------------------------------------------------*/
#include "spi_m25p80.h"

typedef enum ERTYPE{Sec1,Sec8,Sec16,Chip} ErType;
//typedef enum IDTYPE{Manu_ID,Dev_ID,Jedec_ID} idtype;

// Fiscla Memory初始化
void  FM_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// 配置 FM 检测线
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// SPI Flash 接口
	FiscalMem_Init();
}

// Fiscla Memory检测
uint8_t FMDetect(void)
{
	uint8_t status;
	if (GPIOA->IDR & GPIO_Pin_15)
		status =  ERROR;
	else
		status =  SUCCESS;
	return (status);
}
/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void FiscalMem_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	// 使能SPI2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	// 使能GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// 连接SPI2---AF5, 配置SPI2(SCK/MISO/MOSI)推挽、下拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // SPI2_SCK
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; // SPI2_MOSI
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; // SPI2_MISO
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 配置SPI FLASH CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			  // SPI2_CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	FiscalMem_CS_HIGH();  // 关闭片选

	// SPI2 配置:全双工、主机、8位帧、SCK上升沿采样(SCK极性为1、相位为1)、CS由软件控制、
	//			 SCK最大速度APB1/2=18MHz、高位在前
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
#else
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
#endif
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	// 使能SPI2
	SPI_Cmd(SPI2, ENABLE);
}
/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
uint8_t FiscalMem_EraseSector(uint32_t SectorNo)
{
	uint32_t SectorAddr;
  uint32_t DelayFor = 0;			// 	超时设置
	uint8_t flashstatus = 0;

	if(SectorNo > FiscalMem_SPI_MAXSECNUM)
	{
		return (ERROR);
	}

	SectorAddr = FiscalMem_SPI_SECTORSIZE * SectorNo;
  /*!< Send write enable instruction */
  FiscalMem_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();
  /*!< Send Sector Erase instruction */
  FiscalMem_SendByte(FiscalMem_CMD_SE);
  /*!< Send SectorAddr high nibble address byte */
  FiscalMem_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  FiscalMem_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  FiscalMem_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  FiscalMem_CS_HIGH();

  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();
  /*!< Send "Read Status Register" instruction */
  FiscalMem_SendByte(FiscalMem_CMD_RDSR);
  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = FiscalMem_SendByte(FiscalMem_DUMMY_BYTE);

  }
  //while ((flashstatus & FiscalMem_WIP_FLAG) == SET); /* Write in progress */
  while (((flashstatus & FiscalMem_WIP_FLAG) == SET) && (++DelayFor<TIMEOUT_SE)); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  FiscalMem_CS_HIGH();

  if ((flashstatus & FiscalMem_WIP_FLAG) == SET)
  	return(ERROR);
  return (SUCCESS);
}

// 擦除多个扇区
// 根据指定的起止扇区号选择相应的快速擦除算法
uint8_t FiscalMem_Erase(uint32_t StartSector, uint32_t EndSector)
{
	uint32_t temp;
	uint32_t SecsNum;
	uint32_t CurrentSec;
	uint8_t reValue;

	// 检查入口参数
	if ((StartSector > FiscalMem_SPI_MAXSECNUM)||(EndSector > FiscalMem_SPI_MAXSECNUM))
  {
		return (ERROR);
  }

  // 自动调整输入参数使StartSector<EndSector
  if (StartSector > EndSector)
  {
    temp = StartSector;
    StartSector  = EndSector;
    EndSector  = temp;
  }

  // 若起止扇区号相同则为擦除单个扇区
  if (StartSector == EndSector)
  {
  	reValue = FiscalMem_EraseSector(StartSector);
  	return (reValue);
  }

  // 若起止扇区号分区为0和FiscalMem_SPI_MAXSECNUM,则执行芯片擦除

  if ((StartSector == 0) && (EndSector == FiscalMem_SPI_MAXSECNUM))
  {
  	reValue = FiscalMem_EraseBulk();
  	return (reValue);
  }

  // 擦除数个扇区
  SecsNum = EndSector - StartSector + 1;
  CurrentSec = StartSector;
  if (SecsNum>1)
  {
	  while(SecsNum-- >= 1)
	  {
	  	reValue = FiscalMem_EraseSector(CurrentSec++);
	  	if (reValue == ERROR)
	  		return (ERROR);
	  }
	}
  return (SUCCESS);
}

/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
uint8_t FiscalMem_EraseBulk(void)
{
	uint8_t flashstatus = 0;
  /*!< Send write enable instruction */
  FiscalMem_WriteEnable();

  /*!< Bulk Erase */
  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();
  /*!< Send Bulk Erase instruction  */
  FiscalMem_SendByte(FiscalMem_CMD_BE);
  /*!< Deselect the FLASH: Chip Select high */
  FiscalMem_CS_HIGH();

  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();
  /*!< Send "Read Status Register" instruction */
  FiscalMem_SendByte(FiscalMem_CMD_RDSR);
  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = FiscalMem_SendByte(FiscalMem_DUMMY_BYTE);
  }
  while ((flashstatus & FiscalMem_WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  FiscalMem_CS_HIGH();

  return (SUCCESS);
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "FiscalMem_PAGESIZE" value.
  * @retval None
  */
uint8_t FiscalMem_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /*!< Enable the write access to the FLASH */
  FiscalMem_WriteEnable();

  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();
  /*!< Send "Write to Memory " instruction */
  FiscalMem_SendByte(FiscalMem_CMD_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
  FiscalMem_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  FiscalMem_SendByte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  FiscalMem_SendByte(WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    FiscalMem_SendByte(*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  FiscalMem_CS_HIGH();

  /*!< Wait the end of Flash writing */
  return (FiscalMem_WaitForWriteEnd());
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
uint8_t CheckBuffer[512];
uint8_t FiscalMem_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
  uint8_t reValue;
	uint32_t ReadAddr;
	uint8_t* Buffer;

	Buffer = pBuffer;


	ReadAddr = WriteAddr;

  Addr = WriteAddr % FiscalMem_SPI_PAGESIZE;
  count = FiscalMem_SPI_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / FiscalMem_SPI_PAGESIZE;
  NumOfSingle = NumByteToWrite % FiscalMem_SPI_PAGESIZE;

	if ((WriteAddr + NumByteToWrite > (FiscalMem_SPI_MAXADDR+1))//Out of the max size of flash
      ||(NumByteToWrite == 0))
    return (ERROR);

  if (Addr == 0) /*!< WriteAddr is FiscalMem_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < FiscalMem_PAGESIZE */
    {
      reValue = FiscalMem_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > FiscalMem_PAGESIZE */
    {
      while (NumOfPage--)
      {
        reValue = FiscalMem_WritePage(pBuffer, WriteAddr, FiscalMem_SPI_PAGESIZE);
        if (reValue == ERROR)	return (ERROR);

        WriteAddr +=  FiscalMem_SPI_PAGESIZE;
        pBuffer += FiscalMem_SPI_PAGESIZE;
      }

      reValue = FiscalMem_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
    if (reValue == ERROR)	return (ERROR);
  }
  else /*!< WriteAddr is not FiscalMem_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < FiscalMem_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > FiscalMem_PAGESIZE */
      {
        temp = NumOfSingle - count;

        reValue = FiscalMem_WritePage(pBuffer, WriteAddr, count);
        if (reValue == ERROR)	return (ERROR);

        WriteAddr +=  count;
        pBuffer += count;

        reValue = FiscalMem_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
        reValue = FiscalMem_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
      if (reValue == ERROR)	return (ERROR);
    }
    else /*!< NumByteToWrite > FiscalMem_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / FiscalMem_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % FiscalMem_SPI_PAGESIZE;

      reValue = FiscalMem_WritePage(pBuffer, WriteAddr, count);
      if (reValue == ERROR)	return (ERROR);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        reValue = FiscalMem_WritePage(pBuffer, WriteAddr, FiscalMem_SPI_PAGESIZE);
        if (reValue == ERROR)	return (ERROR);
        WriteAddr +=  FiscalMem_SPI_PAGESIZE;
        pBuffer += FiscalMem_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        reValue = FiscalMem_WritePage(pBuffer, WriteAddr, NumOfSingle);
        if (reValue == ERROR)	return (ERROR);
      }
    }
  }

//>>>>>>>>>>>>>>>>
    if (NumByteToWrite>sizeof(CheckBuffer))
        NumByteToWrite=sizeof(CheckBuffer);
    FiscalMem_ReadBuffer(CheckBuffer, ReadAddr, NumByteToWrite);
	if(memcmp(CheckBuffer, Buffer, NumByteToWrite))
		return (ERROR);
//<<<<<<<<<<<<<<<<<

  return (SUCCESS);
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
uint8_t FiscalMem_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{

	if ((ReadAddr + NumByteToRead > (FiscalMem_SPI_MAXADDR+1)) //Out of the max size of flash
      || (NumByteToRead == 0))
    return (ERROR);
  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  FiscalMem_SendByte(FiscalMem_CMD_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  FiscalMem_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  FiscalMem_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  FiscalMem_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = FiscalMem_SendByte(FiscalMem_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  FiscalMem_CS_HIGH();

  return (SUCCESS);
}

/**
  * @brief  Reads FLASH identification.
  * @param  None
  * @retval FLASH identification
  */
uint32_t FiscalMem_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();

  /*!< Send "RDID " instruction */
  FiscalMem_SendByte(0x9F);

  /*!< Read a byte from the FLASH */
  Temp0 = FiscalMem_SendByte(FiscalMem_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp1 = FiscalMem_SendByte(FiscalMem_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  Temp2 = FiscalMem_SendByte(FiscalMem_DUMMY_BYTE);

  /*!< Deselect the FLASH: Chip Select high */
  FiscalMem_CS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/**
  * @brief  Initiates a read data byte (READ) sequence from the Flash.
  *   This is done by driving the /CS line low to select the device, then the READ
  *   instruction is transmitted followed by 3 bytes address. This function exit
  *   and keep the /CS line low, so the Flash still being selected. With this
  *   technique the whole content of the Flash is read with a single READ instruction.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @retval None
  */
void FiscalMem_StartReadSequence(uint32_t ReadAddr)
{
  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  FiscalMem_SendByte(FiscalMem_CMD_READ);

  /*!< Send the 24-bit address of the address to read from -------------------*/
  /*!< Send ReadAddr high nibble address byte */
  FiscalMem_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte */
  FiscalMem_SendByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte */
  FiscalMem_SendByte(ReadAddr & 0xFF);
}

/**
  * @brief  Reads a byte from the SPI Flash.
  * @note   This function must be used only if the Start_Read_Sequence function
  *         has been previously called.
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */
uint8_t FiscalMem_ReadByte(void)
{
  return (FiscalMem_SendByte(FiscalMem_DUMMY_BYTE));
}

/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t FiscalMem_SendByte(uint8_t byte)
{
#if (0)
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(FiscalMem_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(FiscalMem_SPI, byte);

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(FiscalMem_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(FiscalMem_SPI);
#endif

#if (1)
	while ((SPI2->SR & SPI_I2S_FLAG_TXE) == RESET);
	SPI2->DR = byte;
	while ((SPI2->SR & SPI_I2S_FLAG_RXNE) == RESET);
	return SPI2->DR;
#endif
}

/**
  * @brief  Sends a Half Word through the SPI interface and return the Half Word
  *         received from the SPI bus.
  * @param  HalfWord: Half Word to send.
  * @retval The value of the received Half Word.
  */
uint16_t FiscalMem_SendHalfWord(uint16_t HalfWord)
{
  /*!< Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(FiscalMem_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send Half Word through the sFLASH peripheral */
  SPI_I2S_SendData(FiscalMem_SPI, HalfWord);

  /*!< Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(FiscalMem_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  /*!< Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(FiscalMem_SPI);
}

/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void FiscalMem_WriteEnable(void)
{
  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();

  /*!< Send "Write Enable" instruction */
  FiscalMem_SendByte(FiscalMem_CMD_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  FiscalMem_CS_HIGH();
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
uint8_t FiscalMem_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;
  uint32_t DelayFor = 0;			// 	超时设置

  /*!< Select the FLASH: Chip Select low */
  FiscalMem_CS_LOW();

  /*!< Send "Read Status Register" instruction */
  FiscalMem_SendByte(FiscalMem_CMD_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = FiscalMem_SendByte(FiscalMem_DUMMY_BYTE);

  }
  //while ((flashstatus & FiscalMem_WIP_FLAG) == SET); /* Write in progress */
  while (((flashstatus & FiscalMem_WIP_FLAG) == SET) && (++DelayFor<TIMEOUT_WR)); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  FiscalMem_CS_HIGH();

  if ((flashstatus & FiscalMem_WIP_FLAG) == SET)
  	return(ERROR);
  return (SUCCESS);
}

#if (0)

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void FiscalMem_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Enable the SPI clock */
  FiscalMem_SPI_CLK_INIT(FiscalMem_SPI_CLK, ENABLE);

  /*!< Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(FiscalMem_SPI_SCK_GPIO_CLK | FiscalMem_SPI_MISO_GPIO_CLK |
                         FiscalMem_SPI_MOSI_GPIO_CLK | FiscalMem_CS_GPIO_CLK, ENABLE);

  /*!< SPI pins configuration *************************************************/

  /*!< Connect SPI pins to AF5 */
  GPIO_PinAFConfig(FiscalMem_SPI_SCK_GPIO_PORT, FiscalMem_SPI_SCK_SOURCE, FiscalMem_SPI_SCK_AF);
  GPIO_PinAFConfig(FiscalMem_SPI_MISO_GPIO_PORT, FiscalMem_SPI_MISO_SOURCE, FiscalMem_SPI_MISO_AF);
  GPIO_PinAFConfig(FiscalMem_SPI_MOSI_GPIO_PORT, FiscalMem_SPI_MOSI_SOURCE, FiscalMem_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  /*!< SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = FiscalMem_SPI_SCK_PIN;
  GPIO_Init(FiscalMem_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< SPI MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  FiscalMem_SPI_MOSI_PIN;
  GPIO_Init(FiscalMem_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /*!< SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin =  FiscalMem_SPI_MISO_PIN;
  GPIO_Init(FiscalMem_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sFLASH Card CS pin in output pushpull mode ********************/
  GPIO_InitStructure.GPIO_Pin = FiscalMem_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(FiscalMem_CS_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  DeInitializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void FiscalMem_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Disable the FiscalMem_SPI  ************************************************/
  SPI_Cmd(FiscalMem_SPI, DISABLE);

  /*!< DeInitializes the FiscalMem_SPI *******************************************/
  SPI_I2S_DeInit(FiscalMem_SPI);

  /*!< FiscalMem_SPI Periph clock disable ****************************************/
  FiscalMem_SPI_CLK_INIT(FiscalMem_SPI_CLK, DISABLE);

  /*!< Configure all pins used by the SPI as input floating *******************/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = FiscalMem_SPI_SCK_PIN;
  GPIO_Init(FiscalMem_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FiscalMem_SPI_MISO_PIN;
  GPIO_Init(FiscalMem_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FiscalMem_SPI_MOSI_PIN;
  GPIO_Init(FiscalMem_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = FiscalMem_CS_PIN;
  GPIO_Init(FiscalMem_CS_GPIO_PORT, &GPIO_InitStructure);
}

#endif

/**
  * @}
  */
//ccr20110908>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/******************************************************************************
   向FLASH中写入一个数据块，此数据块可能集中在FLASH的一个扇区内，
   也可能分散在连续的几个扇区内,在写数据前,先把对应块的数据读入到Cache中
   然后擦除块,把要写入的数据填入Cache后,再把Cache中的数据全部写入Flash中
Name: WriteBlock(addr,buff,size); addr数据写入地址
parameters：
   addr－数据写入地址
   buff－需要写入的数据
   size－写入的数据长度
Return：SUCCESS or ERROR
******************************************************************************/
#if 0
char CacheForFLASH[FiscalMem_SPI_SECTORSIZE];//cache for a sector

uint8_t WriteBlock(uint32_t addr,char *buff,uint32_t size)
{
    uint32_t secFr=addr/FiscalMem_SPI_SECTORSIZE,        //数据写入开始扇区号
        secTo=(addr+size-1)/FiscalMem_SPI_SECTORSIZE,    //数据写入开始扇区号
        addrFr=addr & (FiscalMem_SPI_SECTORSIZE-1),      //数据写入开始地址
        pi=0,                           //buff中待写入数据的指针
        wrLp,secLp;

    for (secLp=secFr;(secLp<=secTo) && size;secLp++)
    {

        if (FiscalMem_ReadBuffer(CacheForFLASH, secLp * FiscalMem_SPI_SECTORSIZE, FiscalMem_SPI_SECTORSIZE)==ERROR)//缓存待写入区的数据
            return ERROR;//读数据出错

        if (FiscalMem_Erase(secLp,secLp)==ERROR)
            return ERROR;//擦除出错
        wrLp = FiscalMem_SPI_SECTORSIZE - addrFr;//限制在一块内写入数据
        if (wrLp>size)//不能超过要写入的数据量
            wrLp = size;
        memcpy(CacheForFLASH+addrFr,buff+pi,wrLp);

        if  (FiscalMem_WriteBuffer(CacheForFLASH, secLp * FiscalMem_SPI_SECTORSIZE, FiscalMem_SPI_SECTORSIZE)==ERROR)
            return ERROR;//写入数据出错
        pi += wrLp;// point to the next position
        addrFr = 0;//下一批数据总是从块的首地址开始填入
        size -= wrLp;
    }
    return SUCCESS;
}
#endif
