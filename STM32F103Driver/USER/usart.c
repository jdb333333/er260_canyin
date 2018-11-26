
/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>

#include "stm32f10x.h"

#include "usart.h"
#include "TypeDef.h"
#include "SysTick.h"


volatile uint32_t UART0Status, UART1Status;
volatile uint8_t UART0TxEmpty = 1, UART1TxEmpty = 1,UART2TxEmpty = 1;
volatile TUART RxUART[NUMPORT];

/*****************************************************************************
** Function name:		UARTInit
** Descriptions:		UART config
** PortNum:Point to LOGIC_COM1,LOGIC_COM2...,
** baudrate:1200,2400,4800,9600,19200,38400,57600,115200
** Parity: 0-NONE,1 = ODD,2- EVEN
** StopBit: 1= 1 stop bit;2-2 stop bit
** BitCar: 8 = 8 bit per carattere;9-9 bit per carattere
** Returned value:		true---config success; false---	failure
*****************************************************************************/
uint32_t UARTInit( uint32_t PortNum, uint32_t baudrate,uint8_t Databits,uint8_t Parity,uint8_t Stopbits )

{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* USARTx mode config : ,8,N,1*/
	USART_InitStructure.USART_BaudRate = baudrate;

    //无7位方式
    (Databits==8)?(USART_InitStructure.USART_WordLength = USART_WordLength_8b):\
                  (USART_InitStructure.USART_WordLength = USART_WordLength_9b);

    (Stopbits==1)?(USART_InitStructure.USART_StopBits = USART_StopBits_1):\
                  (USART_InitStructure.USART_StopBits = USART_StopBits_2);
    switch (Parity)
    {
	    case 0:USART_InitStructure.USART_Parity = USART_Parity_No;break;
	    case 1:USART_InitStructure.USART_Parity = USART_Parity_Odd;break;
	    case 2:USART_InitStructure.USART_Parity = USART_Parity_Even;break;
    }

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	if ( PortNum == LOGIC_COM1 )
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	// 使能GPIOA时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	// 使能GPIOB时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	// 使能USART1时钟

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

#if !defined(CASE_ER260) && !defined(CASE_ER220)
		/* USART1 GPIO config */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;								// USART1 Tx (PA.09)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;								// USART1 Rx (PA.10)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
		GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);	//ouhs20160701

		/* USART1 GPIO config */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;								// USART1 Tx (PB.06)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;								// USART1 Rx (PB.07)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif

		/* Enable the USART OverSampling by 8 */
		USART_OverSampling8Cmd(USART1, ENABLE);

		/* USART1 mode config*/
		USART_Init(USART1, &USART_InitStructure);

		/* NVIC configuration */
		/* Configure the Priority Group to 2 bits */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

		/* Enable the USARTx Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ouhs 20140814 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		RxUART[0].GetOut = RxUART[0].PutIn = 0;

		USART_Cmd(USART1, ENABLE);

		/**使能中断**/
		//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		//USART_ITConfig(USART1, USART_IT_TC, ENABLE);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

		return ( true );
	}
	else if ( PortNum == LOGIC_COM2 )
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	// 使能GPIOB时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	// 使能USARTx时钟

		/* USARTx GPIO config */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;							// USART3 Tx (PB.10)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;							// USART3 Rx (PB.11)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* Enable the USART OverSampling by 8 */
		USART_OverSampling8Cmd(USART3, ENABLE);

		/* USART3 mode config */
		USART_Init(USART3, &USART_InitStructure);

		/* NVIC configuration */
		/* Configure the Priority Group to 2 bits */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

		/* Enable the USARTx Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ouhs 20140814 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		RxUART[1].GetOut = RxUART[1].PutIn = 0;

		USART_Cmd(USART3, ENABLE);

		/**使能中断**/
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	// 开启接收中断

		return ( true );
	}
#if USART_EXTEND_EN
	else if ( PortNum == LOGIC_COM3 )
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	// 使能GPIOA时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	// 使能USART2时钟

		/* USART2 GPIO config */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;								// USART2 Tx (PA.02)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;								// USART2 Rx (PA.03)
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#if (0)  //不启用硬件流控
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_USART2);	// USART2 RTS (PA.01)
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_USART2);	// USART2 CTS (PA.0)
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
		/* Enable the USART OverSampling by 8 */
	  	USART_OverSampling8Cmd(USART2, ENABLE);

		/* USART2 mode config*/
		USART_Init(USART2, &USART_InitStructure);

		/* NVIC configuration */
		/* Configure the Priority Group to 2 bits */
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

		/* Enable the USARTx Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ouhs 20140814 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		RxUART[2].GetOut = RxUART[2].PutIn = 0;

		USART_Cmd(USART2, ENABLE);

		/**使能中断**/
		//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
		//USART_ITConfig(USART2, USART_IT_TC, ENABLE);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        UARTSend(LOGIC_COM3,XON);//ccr2014-08-27
		return ( true );
	}

#endif	//USART_EXTEND_EN
	return( false );

}

/*****************************************************************************
** Function name:		UARTSend
** Descriptions:		Transmits single data through the portNum
** parameters:			PortNum:Point to LOGIC_COM1,LOGIC_COM2...,
**						ch:the data to transmit;
** Returned value:		None
*****************************************************************************/
void UARTSend( uint8_t portNum, uint8_t ch )
{
	uint32_t timeOut;

    if ( portNum == LOGIC_COM1 )
    {
        USART_SendData(UART0, ch);   			// 写入数据
		timeOut = UART_BLOCKING_TIMEOUT;
		while( (USART_GetFlagStatus(UART0, USART_FLAG_TC) == RESET) && timeOut )	// 发送完成或超时
		{
            timeOut--;
        }
	}
	else if ( portNum == LOGIC_COM2 )
    {
        USART_SendData(UART1, ch);   			// 写入数据
        timeOut = UART_BLOCKING_TIMEOUT;
		while( (USART_GetFlagStatus(UART1, USART_FLAG_TC) == RESET) && timeOut )	// 发送完成或超时
		{
            timeOut--;
        }
	}
#if USART_EXTEND_EN
    else if ( portNum == LOGIC_COM3 )
    {
        USART_SendData(UART2, ch);   			// 写入数据
        timeOut = UART_BLOCKING_TIMEOUT;
        while( (USART_GetFlagStatus(UART2, USART_FLAG_TC) == RESET) && timeOut )	// 发送完成或超时
        {
            timeOut--;
        }
    }
#endif
	return;
}

/*****************************************************************************
** Function name:		UARTSendStr
**
** Descriptions:		Send a block of data to the UART 0 port based
**						on the data length
** PortNum:Point to LOGIC_COM1,LOGIC_COM2...,
** parameters:			portNum, buffer pointer, and data length
** Returned value:		None
**
*****************************************************************************/
void UARTSendStr( uint8_t portNum, uint8_t *BufferPtr, uint32_t Length )
{
    if ( portNum == LOGIC_COM1 )
    {
        while ( Length != 0 )
        {
            while ( !(UART0TxEmpty & 0x01) );	 // 发送数据寄存器为空
	        USART_SendData(UART0, *BufferPtr);   // 写入数据
            UART0TxEmpty = 0;
            BufferPtr++;
            Length--;
        }
    }
    else if ( portNum == LOGIC_COM2 )
    {
        while ( Length != 0 )
        {
            while ( !(UART1TxEmpty & 0x01) );
	        USART_SendData(UART1, *BufferPtr);
            UART1TxEmpty = 0;
            BufferPtr++;
            Length--;
        }
    }
#if USART_EXTEND_EN
    else if ( portNum == LOGIC_COM3 )
    {
        while ( Length != 0 )
        {
            while ( !(UART2TxEmpty & 0x01) );
            USART_SendData(UART2, *BufferPtr);
            UART2TxEmpty = 0;
            BufferPtr++;
            Length--;
        }
    }
#endif
    return;
}

/**
  * @brief  Read one character from UARTx (none blocking read).
  *
  * @param  portNum-Point to LOGIC_COM1,LOGIC_COM2...,
  *         ch -the char get from UARTx;==NULL,检测是否有数据
  *         sdelay-delay for timeout
  * @retval TRUE-if data has been read, FALSE-no data
  */
uint8_t UARTGet(uint8_t portNum, uint8_t *ch,uint32_t sdelay)
{
    volatile TUART *sRxUART;		//
    uint32_t sCount,delay=GetSystemTimer();

    if (portNum==LOGIC_COM1)
        sRxUART=&RxUART[0];
    else if (portNum==LOGIC_COM2)
        sRxUART=&RxUART[1];
#if USART_EXTEND_EN
    else if ( portNum == LOGIC_COM3 )
        sRxUART=&RxUART[2];
#endif
//        return 0;

	sCount = 0xffffff;//最大等待延迟不超过大约5秒。
    while (sRxUART->GetOut==sRxUART->PutIn)	//watting for data
    {
        if (!sdelay || (GetSystemTimer()-delay)>sdelay || !sCount)
            return false;
		sCount--;
    }
    if (ch==NULL)
        return true;//表示有数据
    else
    {
        *ch = sRxUART->RxBuffer[sRxUART->GetOut];

        sRxUART->GetOut++;					//point to the next
        if (sRxUART->GetOut >= rxBUFSIZE)
            sRxUART->GetOut = 0;			//overflow

        sCount = sRxUART->PutIn;
        if (sCount>sRxUART->GetOut)
            sCount -= sRxUART->GetOut;
        else
            sCount = (rxBUFSIZE-(sRxUART->GetOut-sCount));
#if XOFF_AT
        if (BIT(sRxUART->Status ,XOFF_Flag) && sCount<=XON_AT)
        {
           CLRBIT(sRxUART->Status ,XOFF_Flag);
           UARTSend(portNum,XON);
        }
#endif
        return true;
    }

}

/*********************************************************************************
  * @brief  Write one character to UART0.
  *
  * @param  ch: Character to be written
  * @retval None
  */
void UART_PutChar (uint8_t ch)
{
    UARTSend(PORT_DEBUG,ch);
}


/**
  * @brief  Read one character from UART0 (blocking read).
  *
  * @param  None
  * @retval Received character
  */
uint8_t UART_GetChar (void)
{
    uint8_t ch;

	while (UARTGet(PORT_DEBUG,&ch,0)==0)
    {
    }

    return(ch);
}

/**
  * @brief  Read one character from UART0 (non blocking read).
  *
  * @param  ch is the char read from UART
  * @retval Received character
  */
uint8_t UART_GetChar_nb (uint8_t *ch)
{
    if (UARTGet(PORT_DEBUG,ch,0)==0)
       return 0;
    else
        return 1;
}

/**
  * @brief  Write a string to UART0.
  *
  * @param  str: NULL-terminated char string to be written
  * @retval None
  */
void UART_PutString (uint8_t *str)
{
/* usage: LPC1700_UART_Printf("xxx\n\r");*/
#if 1
    while (*str != 0)
    {
        UART_PutChar(*str++);
    }

#else
/* usage: LPC1700_UART_Printf("xxx\n");*/
    while ((*str) != 0)
    {
        if (*str == '\n')
        {
            UART_PutChar(*str++);
            UART_PutChar('\r');
        }
        else
        {
            UART_PutChar(*str++);
        }
    }
#endif
}

/**
  * @brief  Print formatted string. This function takes variable length arguments.
  *
  * @param  format
  * @param  ...
  * @retval None
  *
  * Note: using library functions "vsprintf" will increase the RO size by about 6KB
  */
void  USART_printf (const  uint8_t *format, ...)
{
    static  uint8_t  buffer[200];
    va_list     vArgs;

    va_start(vArgs, format);
    vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);
    UART_PutString((uint8_t *) buffer);
}


/******************************************************************************/
//向串口发送NumBytes字节的数据(TxData),
//PortNum:Point to LOGIC_COM1,LOGIC_COM2...,
//1-串口1,2-串口2.....
unsigned short Bios_PortWrite(BYTE NumCom, void  *TxData, unsigned short NumBytes, unsigned char Flags)
{
    int i;
    BYTE *sBuf=(BYTE*)TxData;

    if (NumCom<=NUMPORT) //ouhs if(NumCom<NUMPORT)
    {
        for (i=0;i<NumBytes;i++)
        {
            UARTSend(NumCom,*sBuf);
            sBuf++;
        }
        return 1;
    }
    return 0;
}
//从串口读取NumBytes字节的数据并存入Dest中;
//NumCom:Point to LOGIC_COM1,LOGIC_COM2...,.
//NumBytes=0:等待串口有数据可读;=-1,清空收到的数据
//Dest=0:判断串口是否有数据可读,此项功能用来检测串口是否有数据,无数据时不等待.
//1-串口1,2-串口2.....
//返回实际所读取的字节数
short Bios_PortRead(BYTE NumCom, void *Dest, short NumBytes,ULONG TimeOut,unsigned  short *Status)
{
    ULONG sDelay=GetSystemTimer();
    int i,sCom;
    BYTE *sBuf=(BYTE*)Dest;

   sCom=NumCom-1; //sCom=NumCom;

    if (NumCom<=NUMPORT)//(NumCom<NUMPORT)
    {//
        if (Status)
            *Status = 0;//Set the Error status to zero

        if (NumBytes==0)
        {//wating data for read
            while (RxUART[sCom].GetOut == RxUART[sCom].PutIn)//watting for data
            {
                if (!TimeOut || (GetSystemTimer()-sDelay)>TimeOut)
                    return 0;//无数据
            }
            return 1;//有数据
        }
        else if (NumBytes==-1)
        {//clean data of the UART port
            RxUART[sCom].GetOut = RxUART[sCom].PutIn = 0;//ccr20110901
            return 1;
        }
        else
        {
            if (Dest==0)
            {//check data for read
                return (RxUART[sCom].GetOut != RxUART[sCom].PutIn);
            }
            for (i=0;i<NumBytes;i++)
            {//read data from UART
                if (!UARTGet(NumCom,sBuf,TimeOut))
                    return i;
                sBuf++;
            }
            return NumBytes;
        }
    }
    else
        return 0;
}


/******************************************************************************
**                            End Of File
******************************************************************************/


