
#include "stm32f10x_it.h"
#include "pwr_ctrl.h"



__IO uint16_t Vin_Vrtc_Val[3]; // Vin_Vrtc_Val[0]---电池电压 ,Vin_Vrtc_Val[1]---RTC电压, Vin_Vrtc_Val[2]---打印机温度检测

uint8_t PowerFlags;		//检测到的电源状态
/*
    Bit0:=0,为适配器供电;=1,为电池供电
    Bit1:=1,为电池供电时,出现报警PWR_WARNING1(电压次低)
    Bit2:=1,为电池供电时,出现报警PWR_WARNING2(电压过低)

    Bit4:=1,纽扣电池出现报警PWR_WARNING1(电压次低)
    Bit5:=1,纽扣电池出现报警PWR_WARNING2(电压过低)
*/

void ADC1_DMA_Config(void);
void pwrIO_Init(void);
void pwrVolDetecCtrl(uint8_t NewState);
void pwrADCCtrl(FunctionalState NewState);


extern void	USART_printf (const uint8_t *format, ...);

/**************************************************************************/
/* 电源管理初始化函数
***************************************************************************/
void pwr_Init(void)
{
	pwrIO_Init();
	ADC1_DMA_Config();
	pwrADCCtrl(ENABLE);
}

void pwrIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DC_DET_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DC_DET_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BAT_CHG_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(BAT_STAT_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = BAT_DONE_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(BAT_STAT_PORT, &GPIO_InitStructure);
	
	/* ADC Channel 14 -> PC4, 15 -> PC5 */
	GPIO_InitStructure.GPIO_Pin = VIN_Pin | VRTC_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/**************************************************************************/
/* ADC1功能配置:ADC1_14用于检测主电源电压，ADC1_15用于检测RTC电池电压
***************************************************************************/
void ADC1_DMA_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef	DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
	/* ADCCLK = PCLK2/2 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);
#else
	/* ADCCLK = PCLK2/4 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div8/*RCC_PCLK2_Div4*/);	//72MHz/8=9MHz
#endif

#if (1)
	/* DMA2_Stream2 channel1 configuration **************************************/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Vin_Vrtc_Val;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3;//2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* DMA2_Stream2 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);
#endif

	/* ADC2 regular channels 14 15 configuration*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //转换不受外界决定
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 3;//2;	   //扫描通道数
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC2 regular channels 14, 15 configuration */   //	外部输入阻抗太大，需要较多的采样周期才能保证精度
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_28Cycles5);	//打印机温度检测

	/* Enable DMA request after last transfer (Single-ADC mode) */
	//ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	//ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	//ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */
	//ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	//while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	//ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	//while(ADC_GetCalibrationStatus(ADC1));

	/* Start ADC1 Software Conversion */
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**************************************************************************/
/* 电压检测ADC功能开启或关闭。参数：NewState=ENABLE---开启，DISABLE---关闭
***************************************************************************/
void pwrADCCtrl(FunctionalState NewState)
{
	// 开启ADC检测会产生一次PFO掉电信号？？？
	ADC_DMACmd(ADC1, NewState);
	ADC_Cmd(ADC1, NewState);
	ADC_SoftwareStartConvCmd(ADC1, NewState);
	if(NewState == DISABLE)
	{//关闭电压检测时清0
		Vin_Vrtc_Val[0] = 0;
		Vin_Vrtc_Val[1] = 0;
	}
}

/**************************************************************************/
/*@param	GET_VIN_STATUS
			GET_VRTC_STATUS
  @retval	PWR_STAT_OK
			PWR_WARNING1
			PWR_WARNING2
***************************************************************************/
/*
    Bit0:=0,为适配器供电;=1,为电池供电
    Bit1:=1,为电池供电时,出现报警PWR_WARNING1(电压次低)
    Bit2:=1,为电池供电时,出现报警PWR_WARNING2(电压过低)

    Bit4:=1,纽扣电池出现报警PWR_WARNING1(电压次低)
    Bit5:=1,纽扣电池出现报警PWR_WARNING2(电压过低)
*/
uint8_t pwrGetStatus(uint8_t GetStatus)
{
	uint8_t status;
	uint16_t Vin_Val, Vrtc_Val;

	PowerFlags = 0;

#if POWERCTRL
	//return 0;//testonly skip test
	if (!DC_DET_GET())
	{//为电池供电
		PowerFlags = PWR_BY_BAT;//=1,为电池供电

		if (GetStatus == GET_VIN_STATUS)
		{
			Vin_Val = Vin_Vrtc_Val[0];
			//USART_printf("VIN=0x%x\n", Vin_Val);	//testonly by ouhs
		    if(Vin_Val > VIN_OK)
			{
				status = PWR_STAT_OK;
			}
			else if(Vin_Val > VIN_LOW)
			{
				status = PWR_WARNING1;
				PowerFlags = PWR_BY_BAT | PWR_WARNING1;//为电池供电时,出现报警PWR_WARNING1(电压次低)，提示充电
			}
			else
			{
				status = PWR_WARNING2;
				PowerFlags = PWR_BY_BAT | PWR_WARNING2;//为电池供电时,出现报警PWR_WARNING2(电压过低),停止使用
			}

		}
	}
#endif
	if(GetStatus == GET_VRTC_STATUS)
	{
		Vrtc_Val = Vin_Vrtc_Val[1];
		//USART_printf("\r\nVrtc=0x%x  \n", Vrtc_Val);	//test by ouhs
		if(Vrtc_Val > VRTC_2V5)
		{
			status = PWR_STAT_OK;
		}
		else if(Vrtc_Val > VRTC_2V2)
		{
			status = PWR_WARNING1;
			PowerFlags |= PWR_BUT_WARNING1;//纽扣电池出现报警PWR_WARNING1(电压次低)
		}
		else
		{
			status = PWR_WARNING2;
			PowerFlags |= PWR_BUT_WARNING2;//纽扣电池出现报警PWR_WARNING2(电压过低)
		}
	}
	return status;
}


/**************************************************************************/
/* 适配器供电状态下检测电池是否安装,以及电池充电状态
***************************************************************************/
uint8_t pwrBATStatus(void)
{
	uint8_t status = 0;

	if((BAT_STAT_PORT->IDR & BAT_CHG_Pin) && (BAT_STAT_PORT->IDR & BAT_DONE_Pin))	//电池没有安装
		status = STAT_NOBATERRY;
	else if (((BAT_STAT_PORT->IDR & BAT_CHG_Pin)==0) && ((BAT_STAT_PORT->IDR & BAT_DONE_Pin)!=0))	//正在充电
		status = STAT_CHARGING;
	else if (((BAT_STAT_PORT->IDR & BAT_CHG_Pin)!=0) && ((BAT_STAT_PORT->IDR & BAT_DONE_Pin)==0))	//充电完成
		status = STAT_DONE;

	return status;	
}



/*---------------------------------------------------------------------------------
----------------------------------------------------------------------------------*/

#if defined STM32F10X_MD_VL || defined STM32F10X_MD
  #define BKP_DR_NUMBER              10
#else
  #define BKP_DR_NUMBER              42
#endif /* USE_STM3210B_EVAL or USE_STM32100B_EVAL */


#if defined STM32F10X_MD_VL || defined STM32F10X_MD
uint16_t BKPDataReg[BKP_DR_NUMBER] =
  {
    BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4, BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8,
    BKP_DR9, BKP_DR10
  };
#else
uint16_t BKPDataReg[BKP_DR_NUMBER] =
  {
    BKP_DR1, BKP_DR2, BKP_DR3, BKP_DR4, BKP_DR5, BKP_DR6, BKP_DR7, BKP_DR8,
    BKP_DR9, BKP_DR10, BKP_DR11, BKP_DR12, BKP_DR13, BKP_DR14, BKP_DR15, BKP_DR16,
    BKP_DR17, BKP_DR18, BKP_DR19, BKP_DR20, BKP_DR21, BKP_DR22, BKP_DR23, BKP_DR24,
    BKP_DR25, BKP_DR26, BKP_DR27, BKP_DR28, BKP_DR29, BKP_DR30, BKP_DR31, BKP_DR32,
    BKP_DR33, BKP_DR34, BKP_DR35, BKP_DR36, BKP_DR37, BKP_DR38, BKP_DR39, BKP_DR40,
    BKP_DR41, BKP_DR42
  };
#endif /* USE_STM3210B_EVAL or USE_STM32100B_EVAL */

/**
 *对MCU内部备份寄存器初始化电源和时钟
 *
**/
void BackupReg_Init(void)
{
	/* Enable PWR and BKP clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Enable write access to Backup domain */
	PWR_BackupAccessCmd(ENABLE);

	/* Clear Tamper pin Event(TE) pending flag */
	BKP_ClearFlag();

	/* Check if the Power On Reset flag is set */
	//if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		/* Clear reset flags */
		//RCC_ClearFlag();
	}
}

/**
  * @brief  Writes data Backup DRx registers.
  * @param
  * @retval None
  */
void WriteToBackupReg(uint8_t bkp_addr, uint16_t bkp_data)
{
	if(bkp_addr < BKP_DR_NUMBER)
		BKP_WriteBackupRegister(BKPDataReg[bkp_addr], bkp_data);
}


/**
  * @brief  Reads data Backup DRx registers.
  * @param
  * @retval None
  */
uint16_t ReadFromBackupReg(uint8_t bkp_addr)
{
	if(bkp_addr < BKP_DR_NUMBER)
		return BKP_ReadBackupRegister(BKPDataReg[bkp_addr]);
}



/**
 * 对内部BackupSRAM访问需要增加以下代码
 * MCU内部RAM用来存储下载的用户自定义要所图片
 */

//#define BKPSRAM_SIZE 0x1000

/**Backup_SRAM访问初始化**/
/**
 *
 *
 * @author EutronSoftware (2014-03-25)
 */

 /*
void BKPSRAM_Init(void)
{
	//使能电源模块时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	//使能对备份域的访问
	PWR_BackupAccessCmd(ENABLE);
	//使能备份域SRAM的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);

	//使能内部BackupSRAM电源
	PWR_BackupRegulatorCmd(ENABLE);

	//等待电源稳定
	while(PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
	{
	}
}
*/

/**
 * 写BackupSRAM
 *
 * @author EutronSoftware (2014-03-25)
 *
 * @param pBuffer
 * @param WriteAddr:0~4095
 * @param NumHalfwordToWrite
 */
/*
void BKPSRAM_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
    if (NumHalfwordToWrite)
        memcpy((uint8_t *)(WriteAddr+BKP_BASE),pBuffer,NumHalfwordToWrite);
}
*/

/**
 * 读BackupSRAM
 *
 * @author EutronSoftware (2014-03-25)
 *
 * @param pBuffer
 * @param ReadAddr:0~4095
 * @param NumHalfwordToRead
 */

 /*
void BKPSRAM_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
    if (NumHalfwordToRead)
        memcpy(pBuffer,(uint8_t *)(ReadAddr+BKP_BASE), NumHalfwordToRead);
}
*/



