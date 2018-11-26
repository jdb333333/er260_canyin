/**
  ******************************************************************************
  * @file    app.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file provides all the Application firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern __IO uint8_t Receive_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
extern __IO  uint32_t Receive_length ;
extern __IO  uint32_t length ;

uint8_t USB_COMM_Ready();

void SaveToUSBQuery(uint8_t RxData);

uint8_t Send_Buffer[VIRTUAL_COM_PORT_DATA_SIZE];
uint32_t packet_sent=1;//ccr2016-06-24 初始值必须为1
uint32_t packet_receive=1;//ccr2016-06-24 初始值必须为1
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void USB_Start(void)
{
    Set_System();
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();
}

void USB_COMM_TEST()
{
    uint8_t Ch;
    if (USB_COMM_Ready())
    {
        while (CheckUSB() && ReadUSB(&Ch))
        {
            SendUSB(Ch);
        }
	}
}
/**
 * 测试USB状态
 *
 * @author EutronSoftware (2016-04-21)
 */
uint8_t USB_COMM_Ready()
{
//ccr2016-06-24    int sLp;

    if (bDeviceState == CONFIGURED)
    {
        CDC_Receive_DATA();
        //CDC_Send_DATA ((uint8_t *)Receive_Buffer,Receive_length);//testonly
//ccr2016-06-24        for (sLp=0;sLp<Receive_length;sLp++)
//ccr2016-06-24        {
//ccr2016-06-24            SaveToUSBQuery(Receive_Buffer[sLp]);
//ccr2016-06-24        }
//ccr2016-06-24        Receive_length=0;
//ccr2016-06-24        return (sLp>0);
        return (Receive_length>0);//ccr2016-06-24
    }
    else
        return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
