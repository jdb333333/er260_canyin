/**
  ******************************************************************************
  * @file    usb_endp.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Endpoint routines
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "usart.h"  //ccr2016-04-14
#include "Systick.h"  //ccr2016-04-14

/* Private typedef -----------------------------------------------------------*/
volatile TUART RxUSB;//ccr2016-04-14
/* Private define ------------------------------------------------------------*/

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define VCOMPORT_IN_FRAME_INTERVAL             5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t packet_sent;
extern __IO uint32_t packet_receive;
extern __IO uint8_t Receive_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
uint32_t Receive_length;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void CopyUSBPMAToQuery(uint16_t wNBytes);
void SaveToUSBQuery(uint8_t RxData);

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

void EP1_IN_Callback (void)
{
  packet_sent = 1;
}

/*******************************************************************************
* Function Name  : EP3_OUT_Callback
* Description    : 将USB收到的数据存入Receive_Buffer中,
* Input          : None.
* Output         : Receive_length:数据数目.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
    int sLp;//ccr2016-06-24
    packet_receive = 1;
    Receive_length = GetEPRxCount(ENDP3);
    PMAToUserBufferCopy((unsigned char*)Receive_Buffer, ENDP3_RXADDR, Receive_length);
    //ccr2016-06-24>>>>>
    for (sLp=0;sLp<Receive_length;sLp++)
    {
        SaveToUSBQuery(Receive_Buffer[sLp]);
    }
    //ccr2016-06-24<<<<<
}
/**
 * 将从USB收到的一个字节写入USB数据队列中
 *
 * @author EutronSoftware (2016-04-25)
 *
 * @param aByte
 */
void SaveToUSBQuery(uint8_t RxData)
{

    uint32_t sCount;

    /* Receive the USART data */

    sCount = RxUSB.PutIn+1;
    if (sCount>=rxBUFSIZE)
        sCount = 0;

    if (sCount != RxUSB.GetOut)
    {
        RxUSB.RxBuffer[RxUSB.PutIn] = RxData;
        RxUSB.PutIn = sCount;
#if (0)//ccr2016-04-18
        if (sCount>RxUSB.GetOut)
            sCount -= RxUSB.GetOut;
        else
            sCount += (rxBUFSIZE-RxUSB.GetOut);
        if (RxUSB.Status & XOFF_FLOWCTRL)
        {
            if (sCount>XOFF_AT && !BIT(RxUSB.Status ,XOFF_Flag))
            {
                SETBIT(RxUSB.Status ,XOFF_Flag);
                UARTSend(LOGIC_COM1,XOFF);  //by ouhs
            }
        }
#endif
    }

}
/*******************************************************************************
* Function Name  : CopyUSBPMAToQuery
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - wPMABufAddr = address into PMA.
*                  - wNBytes     = no. of bytes to be copied.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CopyUSBPMAToQuery(uint16_t wNBytes)
{
  uint32_t n = (wNBytes + 1) >> 1;/* /2*/
  uint32_t i;
  uint32_t *pdwVal;
  uint16_t sWord;

  pdwVal = (uint32_t *)(ENDP3_RXADDR * 2 + PMAAddr);

  for (i = n; i != 0; i--)
  {
    sWord = *pdwVal++;
    SaveToUSBQuery(sWord & 0xff);
    SaveToUSBQuery(sWord >> 8);
  }

}

/**
  * @brief  Read one character from USB (none blocking read).
  *
  * @param  ch -the char get from UARTx;==NULL,检测是否有数据
  *         sdelay-delay for timeout
  * @retval TRUE-if data has been read, FALSE-no data
  */
uint8_t USBGet(uint8_t *ch,uint32_t sdelay)
{

    uint32_t sCount,delay=GetSystemTimer();

    while (RxUSB.GetOut==RxUSB.PutIn)	//watting for data
    {
        if (!sdelay || (GetSystemTimer()-delay)>sdelay )
            return 0;
    }
    if (ch==NULL)
        return 1;//表示有数据
    else
    {
        *ch = RxUSB.RxBuffer[RxUSB.GetOut];

        RxUSB.GetOut++;					//point to the next
        if (RxUSB.GetOut >= rxBUFSIZE)
            RxUSB.GetOut = 0;			//overflow

#if (0) //ccr2016-04-18
        sCount = RxUSB.PutIn;
        if (sCount>RxUSB.GetOut)
            sCount -= RxUSB.GetOut;
        else
            sCount = (rxBUFSIZE-(RxUSB.GetOut-sCount));
#if XOFF_AT
        if (BIT(RxUSB.Status ,XOFF_Flag) && sCount<=XON_AT)
        {
           CLRBIT(RxUSB.Status ,XOFF_Flag);
           UARTSend(portNum,XON);
        }
#endif
#endif
        return 1;
    }

}

//从USB口读取NumBytes字节的数据并存入Dest中;
//NumBytes=0:等待串口有数据可读;=-1,清空收到的数据
//Dest=0:判断串口是否有数据可读,此项功能用来检测串口是否有数据,无数据时不等待.
//返回实际所读取的字节数
short USB_PortRead(void *Dest, short NumBytes,uint32_t TimeOut)
{
    uint32_t sDelay=GetSystemTimer();
    int i;
    uint8_t *sBuf=(uint8_t*)Dest;

        if (NumBytes==0)
        {//wating data for read
            while (RxUSB.GetOut == RxUSB.PutIn)//watting for data
            {
                if (!TimeOut || (GetSystemTimer()-sDelay)>TimeOut)
                    return 0;//无数据
            }
            return 1;//有数据
        }
        else if (NumBytes==-1)
        {//clean data of the UART port
            RxUSB.GetOut = RxUSB.PutIn = 0;//ccr20110901
            return 1;
        }
        else
        {
            if (Dest==0)
            {//check data for read
                return (RxUSB.GetOut != RxUSB.PutIn);
            }
            for (i=0;i<NumBytes;i++)
            {//read data from UART
                if (!USBGet(sBuf,TimeOut))
                    return i;
                sBuf++;
            }
            return NumBytes;
        }
}



//Read one byte from USBport
//out:(receive data)
// return false if no data
uint8_t ReadUSB(uint8_t *rCH)
{
    return USBGet(rCH,500);
}


uint8_t CheckUSB()
{
    return (RxUSB.GetOut!=RxUSB.PutIn);
}


void EmptyUSB()
{
    RxUSB.GetOut=RxUSB.PutIn=0;
}

void SendUSB(uint8_t ch)
{
    uint8_t buf[1];
    buf[0]=ch;
    CDC_Send_DATA(buf,1);
}

void SendUSBString(uint8_t *sStr,int len)
{
    CDC_Send_DATA(sStr,len);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

