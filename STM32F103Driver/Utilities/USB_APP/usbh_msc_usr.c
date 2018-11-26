/**
  ******************************************************************************
  * @file    usbh_msc_usr.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file includes the usb host library user callbacks
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
#include <string.h>
#include "usbd_usr.h"	//ccr

#include "usbh_msc_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "usbh_hid_core.h"	//ccr
#include "usbh_msc_core.h"	//ccr

#include "monitor.h"

void USB_HandleDisconnect(void);


/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_MSC_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief    This file includes the usb host stack user callbacks
* @{
*/

/** @defgroup USBH_USR_Private_TypesDefinitions
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_Private_Defines
* @{
*/
#define IMAGE_BUFFER_SIZE    512
/**
* @}
*/


/** @defgroup USBH_USR_Private_Macros
* @{
*/
extern USB_OTG_CORE_HANDLE          USB_OTG_FS_Core;
extern uint8_t Enum_Done;
/**
* @}
*/


/** @defgroup USBH_USR_Private_Variables
* @{
*/

uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_USBH_MSC_Callbacks =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError

};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/
/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]          = ">Host Library OK\n";
const uint8_t MSG_DEV_ATTACHED[]       = ">Device Attached\n";
const uint8_t MSG_DEV_DISCONNECTED[]   = ">Device plug out\n";
const uint8_t MSG_DEV_ENUMERATED[]     = ">Device Ready.\n";//">Enumeration OK\n";
const uint8_t MSG_DEV_HIGHSPEED[]      = ">HS Device Found\n";
const uint8_t MSG_DEV_FULLSPEED[]      = ">FS Device Found\n";
const uint8_t MSG_DEV_LOWSPEED[]       = ">LS Device Found\n";
const uint8_t MSG_DEV_ERROR[]          = ">Device fault \n";

const uint8_t MSG_MSC_CLASS[]          =  ">UDisk/SD Found\n";
const uint8_t MSG_DISK_SIZE[]       	= ">Size MBytes: \n";
const uint8_t MSG_LUN[]             	= ">LUN Available:\n";
const uint8_t MSG_ROOT_CONT[]       	= ">Exploring disk.\n";
const uint8_t MSG_WR_PROTECT[]      	= ">Disk is Locked.\n";

const uint8_t MSG_HID_CLASS[]          = ">HID Device Found\n";

const uint8_t USB_HID_MouseStatus[]    = ">HID Device:Mouse\n";
const uint8_t USB_HID_KeybrdStatus[]   = ">HID Device:Keyboard\n";
const uint8_t MSG_MSC_UNREC_ERROR[] = ">Unknow Error.\n";

/**
* @}
*/



/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/

/**
* @}
*/


/** @defgroup USBH_USR_Private_Functions
* @{
*/


/**
* @brief  USBH_USR_Init
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
uint8_t startup = 0;//ccr

void USBH_USR_Init(void)
{
  if(startup == 0 )
  {
    startup = 1;

    //STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);

	xputs(">USB OTG FS HID Host\r\n");
    xputs(">USB Host library started.\r\n");
    xputs(">USB Host Library v2.1.0 \r\n" );
  }
}

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
  startup = 0;//ccr
  USBH_USR_ApplicationState = USH_USR_FS_INIT;
}
/**
* @brief  USBH_USR_DeviceAttached
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
  xputs(MSG_DEV_ATTACHED);
}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{

  /* Set default screen color*/
  xputs(MSG_MSC_UNREC_ERROR);
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Staus
*/
void USBH_USR_DeviceDisconnected (void)
{
  USB_HandleDisconnect();
  xputs(MSG_DEV_DISCONNECTED);
  USB_Device = USB_NONE;
}
/**
* @brief  USBH_USR_ResetUSBDevice
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* Users can do their application actions here for the USB-Reset */
    xputs(">Reset device\n");
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  Device speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
    xputs(MSG_DEV_HIGHSPEED);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
    xputs(MSG_DEV_FULLSPEED);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
    xputs(MSG_DEV_LOWSPEED);
  }
  else
  {
    xputs(MSG_DEV_ERROR);
  }
}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on LCD for device descriptor
* @param  device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;

  xprintf(" VID: %04Xh\n", (uint32_t)(*hs).idVendor);
  xprintf(" PID: %04Xh\n", (uint32_t)(*hs).idProduct);
}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
    xputs(">Device Address Assigned\n");
}


/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;

  id = itfDesc;

  if((*id).bInterfaceClass  == 0x08)
  {
	xputs(MSG_MSC_CLASS);
    USB_Device = USB_DISK;//ccr
    Set_USBH_MSC_CallBacks();//ccr

  }
  else if((*id).bInterfaceClass  == 0x03)
  {
	xputs(MSG_HID_CLASS);
    USB_Device = USB_KEYBOARD;//ccr
    Set_USBH_HID_CallBacks();//ccr
  }
}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  Manufacturer String
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
  xprintf(">Manufacturer: %s\n", (char *)ManufacturerString);
}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
  xprintf(">Product: %s\n", (char *)ProductString);
}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
  xprintf( ">SN:%s\n", (char *)SerialNumString);
}



/**
* @brief  EnumerationDone
*         User response request is displayed to ask application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{

  /* Enumeration complete */
    xputs(MSG_DEV_ENUMERATED);
}


/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
  xputs (">Device not supported:LUN>0\n");
}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
  return USBH_USR_RESP_OK;
}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Staus
*/
void USBH_USR_OverCurrentDetected (void)
{
  xputs (">Overcurrent detected.");
}


/**
* @brief  USBH_USR_MSC_Application
*         Demo application for mass storage
* @param  None
* @retval Staus
*/
int USBH_USR_MSC_Application(void)
{
  if(Enum_Done == 0)
  {
    Enum_Done = 1;
  }
  return(0);
}


/**
* @brief  USR_KEYBRD_Init
*         Init Keyboard window
* @param  None
* @retval None
*/
void  USR_KEYBRD_Init (void)
{

  xputs(USB_HID_KeybrdStatus);
//  xputs("> Use Keyboard to tape characters: \n\n");

}
//ccr2013>>>>>>>>>>>>>>>>>>>>>>>>
#define USBKEYMAX   16
uint8_t USBKeyBuf[USBKEYMAX];//USBKeyBuf最多存储USBKEYMAX-1个键值
int USBKeyIn=0;//USB键存入位置
int USBKeyOut=0;//USB键读取位置

//检查是否有从USB键盘输入
//返回True:有数据,false:无数据
uint8_t CheckUSBKey()
{
    return (USBKeyIn!=USBKeyOut);
}
//
//
/**
 *读取从USB键盘输入的数据
 *
 * @author EutronSoftware (2014-08-21)
 *
 * @param clean 清空数据区
 *
 * @return uint8_t 返回0xff,按键不可用,其他值为有效键
 */
uint8_t GetUSBKey(BYTE clean)
{
    uint8_t dat= USBKeyBuf[USBKeyOut];

    if (clean)
    {
        USBKeyOut=USBKeyIn=0;
        return 0xff;//为无效键值
    }
    if(USBKeyOut==USBKeyIn)
        return 0xff;//为无效键值
    else
    {
        if(++USBKeyOut==USBKEYMAX)
            USBKeyOut = 0;
        return dat;//返回有效键值
    }
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/**USB键盘数据处理函数:当有按键时,调用USR_KEYBRD_ProcessData
* @brief  USR_KEYBRD_ProcessData
*         Process Keyboard data
* @param  data : Keyboard data to be received
* @retval None
*/
void  USR_KEYBRD_ProcessData (uint8_t data)
{
  int posIn;

  posIn = USBKeyIn+1;
  if(posIn>=USBKEYMAX)
      posIn = 0;
  if(posIn!=USBKeyOut && data)
  {
      USBKeyBuf[USBKeyIn]=data;//将数据存入队列
      USBKeyIn = posIn;
  }
}

/**
* @brief  USR_MOUSE_Init
*         Init Mouse window
* @param  None
* @retval None
*/
void USR_MOUSE_Init	(void)
{
#if 0
  LCD_UsrLog((void*)USB_HID_MouseStatus);
  LCD_UsrLog("\n\n\n\n\n\n\n\n");
  LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                   ");
  LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "                                   ");

  /* Display Mouse Window */
  LCD_DrawRect(MOUSE_WINDOW_X,
               MOUSE_WINDOW_Y,
               MOUSE_WINDOW_HEIGHT,
               MOUSE_WINDOW_WIDTH);

  HID_MOUSE_ButtonReleased(0);
  HID_MOUSE_ButtonReleased(1);
  HID_MOUSE_ButtonReleased(2);


  LCD_SetTextColor(LCD_COLOR_GREEN);
  LCD_SetBackColor(LCD_COLOR_BLACK);

  LCD_DisplayChar(MOUSE_WINDOW_X + 1,
                            MOUSE_WINDOW_Y - 1,
                            'x');
  x_loc  = 0;
  y_loc  = 0;
  prev_x = 0;
  prev_y = 0;
#endif
}

/**
* @brief  USR_MOUSE_ProcessData
*         Process Mouse data
* @param  data : Mouse data to be displayed
* @retval None
*/
void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{

#if 0
  uint8_t idx = 1;
  static uint8_t b_state[3] = { 0, 0 , 0};

  if ((data->x != 0) && (data->y != 0))
  {
    HID_MOUSE_UpdatePosition(data->x , data->y);
  }

  for ( idx = 0 ; idx < 3 ; idx ++)
  {

    if(data->button & 1 << idx)
    {
      if(b_state[idx] == 0)
      {
        HID_MOUSE_ButtonPressed (idx);
        b_state[idx] = 1;
      }
    }
    else
    {
      if(b_state[idx] == 1)
      {
        HID_MOUSE_ButtonReleased (idx);
        b_state[idx] = 0;
      }
    }
  }
#endif

}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

