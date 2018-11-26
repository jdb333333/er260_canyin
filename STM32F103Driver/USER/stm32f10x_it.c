/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "stm32f10x_it.h"
#include "sdio_sd.h"
#include "usart.h"
#include "print.h"

#if defined(STM32F10X_HD_USBCOMM)
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_istr.h"
#endif

#if defined(CASE_ASCIIDISPLAY)
#include "lcd_160x.h"
#endif

#include "TypeDef.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
volatile unsigned long RTCTimer=0;//系统计时器,以毫秒为单位递增计时
volatile unsigned long msTimer_Delay0=0,//延时计时器0
         msTimer_Delay1=0,//延时计时器1
         msTimer_Delay2=0;//延时计时器2

volatile uint16_t msTimer_Delay3=0;

void SysTick_Handler(void)
{
  if (msTimer_Delay0)    msTimer_Delay0--;
  if (msTimer_Delay1)    msTimer_Delay1--;
  if (msTimer_Delay2)    msTimer_Delay2--;

	if (msTimer_Delay3)	   msTimer_Delay3--;

  RTCTimer++;

#if !defined(CASE_INNERDOTPRN)
  if (Prn_Delay)
  {//打印机因为过热而暂停
	  Prn_Delay++;//打印机暂停时长
	  if (Prn_Delay>PRNPAUSEMAX)
		  Prn_Delay = 0;//打印机结束暂停
  }
#endif
}


/**
* @brief  This function handles USRAT interrupt request.
* @param  None
* @retval None
*/
extern void SetRTS(BYTE port);  //ouhs 20140814
void USART1_IRQHandler(void)
{
	uint8_t RxData;
	uint32_t sCount;

	/* USART in mode Tramitter -------------------------------------------------*/
	if (USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	{
		/* Disable the USARTx transmit data register empty interrupt */
		//USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
		UART0TxEmpty = 1;
	}

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		/* Receive the USART data */
		RxData = USART_ReceiveData(USART1);
		sCount = RxUART[0].PutIn+1;
		if (sCount>=rxBUFSIZE)
			sCount = 0;

		if (sCount != RxUART[0].GetOut)
		{
			RxUART[0].RxBuffer[RxUART[0].PutIn] = RxData;
			RxUART[0].PutIn = sCount;
			if (sCount>RxUART[0].GetOut)
				sCount -= RxUART[0].GetOut;
			else
				sCount += (rxBUFSIZE-RxUART[0].GetOut);
            if (RxUART[0].Status & XOFF_FLOWCTRL)
            {
	            if (sCount>XOFF_AT && !BIT(RxUART[0].Status ,XOFF_Flag))
	            {
	                SETBIT(RxUART[0].Status ,XOFF_Flag);
					UARTSend(LOGIC_COM1,XOFF);  //by ouhs
				}
            }
            else
            {
                if (RxUART[0].Status & RTS_FLOWCTRL)
                {
                    if (sCount>XOFF_AT && !BIT(RxUART[0].Status ,RTS_Flag))
                    {
                        SetRTS(LOGIC_COM1);
                        SETBIT(RxUART[0].Status ,RTS_Flag);
                    }
                }
            }
		}
	}
}

void USART3_IRQHandler(void)
{
	uint8_t RxData;
	uint32_t sCount;

	/* USART in mode Tramitter -------------------------------------------------*/
	if (USART_GetITStatus(USART3, USART_IT_TXE) == SET)
	{
        UART1TxEmpty = 1;
	}

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		/* Receive the USART data */
		RxData = USART_ReceiveData(USART3);
        sCount = RxUART[1].PutIn+1;//address for new data
        if (sCount>=rxBUFSIZE)
            sCount = 0;

        if (sCount != RxUART[1].GetOut)
        {//buffer dose not full
            RxUART[1].RxBuffer[RxUART[1].PutIn] = RxData;
            RxUART[1].PutIn = sCount;//point to the next address
            //count of the data in the buffer
            if (sCount>RxUART[1].GetOut)
                sCount -= RxUART[1].GetOut;
            else
                sCount += (rxBUFSIZE-RxUART[1].GetOut);
            if (RxUART[1].Status & XOFF_FLOWCTRL)
            {

	            if (sCount>XOFF_AT && !BIT(RxUART[1].Status ,XOFF_Flag))
	            {
	                SETBIT(RxUART[1].Status ,XOFF_Flag);
					UARTSend(LOGIC_COM2,XOFF);  //by ouhs
                }
            }
            else
            {
                if (RxUART[1].Status & RTS_FLOWCTRL)
                {
                    if (sCount>XOFF_AT && !BIT(RxUART[1].Status ,RTS_Flag))
                    {
                        SetRTS(LOGIC_COM2);
                        SETBIT(RxUART[1].Status ,RTS_Flag);
                    }
                }
            }
		}
	}
}

#if USART_EXTEND_EN
void USART2_IRQHandler(void)
{
	uint8_t RxData;
	uint32_t sCount;

	/* USART in mode Tramitter -------------------------------------------------*/
	if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
	{
		/* Disable the USARTx transmit data register empty interrupt */
		//USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
		UART2TxEmpty = 1;
	}

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		/* Receive the USART data */
		RxData = USART_ReceiveData(USART2);
		sCount = RxUART[2].PutIn+1;
		if (sCount>=rxBUFSIZE)
			sCount = 0;

		if (sCount != RxUART[2].GetOut)
		{
			RxUART[2].RxBuffer[RxUART[2].PutIn] = RxData;
			RxUART[2].PutIn = sCount;
			if (sCount>RxUART[2].GetOut)
				sCount -= RxUART[2].GetOut;
			else
				sCount += (rxBUFSIZE-RxUART[2].GetOut);

            if (RxUART[2].Status & XOFF_FLOWCTRL)
            {
	            if (sCount>XOFF_AT && !BIT(RxUART[2].Status ,XOFF_Flag))
	            {
	                SETBIT(RxUART[2].Status ,XOFF_Flag);
					UARTSend(LOGIC_COM3,XOFF);  //by ouhs
                }
            }
            else
            {
                if (RxUART[2].Status & RTS_FLOWCTRL)
                {
                    if (sCount>XOFF_AT && !BIT(RxUART[2].Status ,RTS_Flag))
                    {
                        SetRTS(LOGIC_COM3);
                        SETBIT(RxUART[2].Status ,RTS_Flag);
                    }
                }
            }
		}
	}
}
#endif	//USART_EXTEND_EN


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
extern void (*PWF_CallBack)(void);//ccr20110901
extern volatile uint8_t KeyScanEnable; //ouhs 20140814
void EXTI2_IRQHandler(void)
{//掉电中断服务程序

	//testonly		__disable_irq();    //开机有打印时,有可能引起掉电,此时禁止所有中断将引起系统运行异常
	//testonly		__disable_fault_irq();

////ouhs test 20140827
	BYTE i=0;
	BYTE stat=0;

//	for(i=0; i<15; i++)	//??15us
	{
//		usDelay(1);
		if (GPIOE->IDR & GPIO_Pin_2)
		{
			stat = 0;
//			break;
		}
		else
			stat = 1;
	}

	if (stat==1)
	{
	#if defined(CASE_ASCIIDISPLAY)
		Copy_LCD(false);
	#else
		mLightLCDOff();
		LED_Off();
	#endif
        Save_ApplRam();
        Save_Config(false);
	}
//    UARTSend(LOGIC_COM1,'x');//testonly

	EXTI_ClearITPendingBit(EXTI_Line2);

	//testonly        while(1);//系统停机
}


/**
  * @brief  This function handles External line 9..5 interrupt request.
  * @param  None
  * @retval None
  */
extern volatile uint8_t KeyScanEnable;
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line6) != RESET)
  {	//按键中断
	  KeyScanEnable = 1;
	  EXTI_ClearITPendingBit(EXTI_Line6);
	  //USART_printf("EXTI_IRQHandler\r\n");  //testonly by ouhs
  }
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {	//以太网中断
	  /**/
	  EXTI_ClearITPendingBit(EXTI_Line8);
  }
}


/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/**
  * @brief  This function handles DMA2 Stream3 or DMA2 Stream6 global interrupts
  *         requests.
  * @param  None
  * @retval None
  */
//void SD_SDIO_DMA_IRQHANDLER(void)
//{
//  /* Process DMA2 Stream3 or DMA2 Stream6 Interrupt Sources */
//  SD_ProcessDMAIRQ();
//}




/*******************************************************************************
* Function Name  : USB_IRQHandler
* Description    : This function handles USB Low Priority interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)|| defined (STM32F37X)
void USB_LP_IRQHandler(void)
#else
void USB_LP_CAN1_RX0_IRQHandler(void)
#endif
{
#if defined(STM32F10X_HD_USBCOMM)
  USB_Istr();
#endif
}

/*******************************************************************************
* Function Name  : USB_FS_WKUP_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
void USB_FS_WKUP_IRQHandler(void)
#else
void USBWakeUp_IRQHandler(void)
#endif
{
  EXTI_ClearITPendingBit(EXTI_Line18);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
