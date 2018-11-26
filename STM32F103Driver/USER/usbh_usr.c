/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file includes the user application layer
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
#include "stm32f2xx.h"
#include "monitor.h"

#include "usbh_usr.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"


/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_HID_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief This file is the Header file for usbh_usr.c
* @{
*/


/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/


/**
* @}
*/

extern  int16_t  x_loc, y_loc;
extern __IO int16_t  prev_x, prev_y;

/** @addtogroup USER
* @{
*/

/** @defgroup USBH_USR
* @brief    This file includes the user application layer
* @{
*/

/** @defgroup USBH_CORE_Exported_Types
* @{
*/



/** @defgroup USBH_USR_Private_Defines
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup USBH_USR_Private_Variables
* @{
*/
extern USB_OTG_CORE_HANDLE           USB_OTG_Core;
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
  NULL,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/
/*--------------- LCD Messages -----------1234567890123456----*/
const uint8_t MSG_HOST_INIT[]          = "Host Library OK\n";          //"Host Library Initialized\n";
const uint8_t MSG_DEV_ATTACHED[]       = "Device Attached\n";                   //"Device Attached\n";
const uint8_t MSG_DEV_DISCONNECTED[]   = "Device Removed\n";               //"Device Disconnected\n";
const uint8_t MSG_DEV_ENUMERATED[]     = "Enum completed\n";             //"Enumeration completed\n";
const uint8_t MSG_DEV_HIGHSPEED[]      = "HiSpeed device\n";        //"High speed device detected\n";
const uint8_t MSG_DEV_FULLSPEED[]      = "FulSpeed device\n";        //"Full speed device detected\n";
const uint8_t MSG_DEV_LOWSPEED[]       = "LoSpeed device\n";         //"Low speed device detected\n";
const uint8_t MSG_DEV_ERROR[]          = "Device fault \n";                     //"Device fault \n";
                                                                                //
const uint8_t MSG_MSC_CLASS[]          = "Mass Storage \n";     //"Mass storage device connected\n";
const uint8_t MSG_HID_CLASS[]          = "HID device \n";              //"HID device connected\n";
                                                                                //
const uint8_t USB_HID_MouseStatus[]    = "HID: Mouse\n";           //"HID Demo Device : Mouse\n";
const uint8_t USB_HID_KeybrdStatus[]   = "HID: KeyBoard\n";        //"HID Demo Device : Keyboard\n";
const uint8_t MSG_UNREC_ERROR[]        = "UNRECOVERED ERR\n";           //"UNRECOVERED ERROR STATE\n";
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
void USBH_USR_Init(void)
{
  static uint8_t startup = 0;

  if(startup == 0 )
  {
    startup = 1;

    //STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_GPIO);

	//xprintf("USB OTG FS HID Host\n");
    //xprintf("> USB Host library started.\r\n");
    //        1234567890123456
    xprintf ("USB Host v2.1.0\n" );
  }
}

/**
* @brief  USBH_USR_DeviceAttached
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
  xprintf("%S", MSG_DEV_ATTACHED);
}

/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{
  xprintf("%S", MSG_UNREC_ERROR);
}

/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_USR_DeviceDisconnected (void)
{
  xprintf("%S", MSG_DEV_DISCONNECTED);
}

/**
* @brief  USBH_USR_ResetUSBDevice
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  Devicespeed : Device Speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
    xprintf("%S", MSG_DEV_HIGHSPEED);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
    xprintf("%S", MSG_DEV_FULLSPEED);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
    xprintf("%S", MSG_DEV_LOWSPEED);
  }
  else
  {
    xprintf("%S", MSG_DEV_ERROR);
  }
}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on LCD for device descriptor
* @param  DeviceDesc : device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;

  xprintf("VID:%04Xh\n", (uint32_t)(*hs).idVendor);
  xprintf("PID:%04Xh\n", (uint32_t)(*hs).idProduct);
}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{

}


/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  ConfDesc : Configuration descriptor
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
	xprintf("%S", MSG_MSC_CLASS);
  }
  else if((*id).bInterfaceClass  == 0x03)
  {
	xprintf("%S", MSG_HID_CLASS);
  }
}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
//  xprintf(">%S\n", (char *)ManufacturerString);//Made

}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
  xprintf(">%S\n", (char *)ProductString);//Model:
}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
  xprintf("S/N:%S\n", (char *)SerialNumString);
}

/**
* @brief  EnumerationDone
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
  /* Enumeration complete */
  xprintf("%S", MSG_DEV_ENUMERATED);
//  xprintf("To start the HID class operations: ");
//  xprintf("Press Key...                       ");
}

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
  xprintf(">Not supported.\n");
}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{

  USBH_USR_Status usbh_usr_status;

  usbh_usr_status = USBH_USR_NO_RESP;

  /*Key B3 is in polling mode to detect user action */
  //if(STM_EVAL_PBGetState(Button_KEY) == RESET)
  //{
    usbh_usr_status = USBH_USR_RESP_OK;		//testonly
  //}


  return usbh_usr_status;

}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_USR_OverCurrentDetected (void)
{
   xprintf(">Overcurrent.\n");

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
* @brief  USR_KEYBRD_Init
*         Init Keyboard window
* @param  None
* @retval None
*/
void  USR_KEYBRD_Init (void)
{

  xprintf("%S", USB_HID_KeybrdStatus);
  //xprintf("> Use Keyboard to tape characters: \n\n");

}


/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
}

/** 处理USB键盘输入,当USB键盘有输入时,自动调用USR_KEYBRD_ProcessData
* @brief  USR_KEYBRD_ProcessData
*         Process Keyboard data
* @param  data : Keyboard data to be displayed
* @retval None
*/
short chFromUsb=0;

void  USR_KEYBRD_ProcessData (uint8_t data)
{
    chFromUsb=data;
    //xprintf("%c", data);//testonly

}
//测试USB键盘是否有输入
int TestUsbKey(void)
{
    return (chFromUsb>0);
}
//获取USB键盘输入
short GetUsbKEY(void)
{
    short p=chFromUsb;
    chFromUsb = 0;
    return p;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

