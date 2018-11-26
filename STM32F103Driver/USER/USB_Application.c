/* Includes ------------------------------------------------------------------*/
/**
 *  * @author EutronSoftware (2013-08-16)
 */

#include <string.h>
#include "monitor.h"

#include "usbh_core.h"
#include "usbh_msc_usr.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbh_msc_core.h"
#include "usbd_msc_core.h"
#include "usbh_hid_core.h"
#include "usb_conf.h"

#include "usart.h"
#if !defined(CASE_ASCIIDISPLAY)
#include "CHNDisp.h"
#endif

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_Core __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USBH_HOST               USB_Host __ALIGN_END ;

/**
 * @author EutronSoftware (2013-08-16)
 *
*/
uint8_t Enum_Done;

//extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
//extern USBH_HOST                    USB_Host;

void USB_HandleDisconnect (void);

/**
* start USB @}
*/
void USB_Start()
{
      /* Init HS Core  : Demo start in host mode*/
#ifdef USE_USB_OTG_HS
      xputs(">USB Host High speed...\n");
#else
      xputs(">USB Host Full speed...\n");
#endif
      USBH_Init(&USB_OTG_Core,
#ifdef USE_USB_OTG_FS
                USB_OTG_FS_CORE_ID,
#elif defined USE_USB_OTG_HS
                USB_OTG_HS_CORE_ID,
#endif
                &USB_Host,
                &USBH_MSC_Callbacks,//ccr &USBH_HID_Callbacks,
                &USR_USBH_MSC_Callbacks);//ccr &USR_USBH_HID_Callbacks);
}
/**
 * @author EutronSoftware (2013-08-16)
 *
* Detect USB device,return Device type
*/
#if defined(CASE_ASCIIDISPLAY) //ouhs 20140814
extern void PutsO(const char *str);
#endif
extern void Beep(uint8_t blen);

uint8_t USB_Detect()
{
  if (!HCD_IsDeviceConnected(&USB_OTG_Core))
  {//物理拔除USB设备时或物理设备没有插时
      if (USB_Host.gState!=HOST_IDLE)
      {
          xputs("USB Device Disconnected.\n");
#if defined(CASE_ASCIIDISPLAY)
          PutsO("USB [-]");
#else
          DispStrXY("USB:[x]",4,SCREENLN);
#endif
          Beep(3);//设备异常
      }
      USB_HandleDisconnect();//执行完此语句后,USB_Host.gState=HOST_IDLE
      USB_Device = USB_NONE;
  }
  else
  {
      USBH_Process(&USB_OTG_Core , &USB_Host);
/*
      if (USB_Device == USB_KEYBOARD)
      {
        USB_Keyboard_Test();
      }
      else  if (USB_Device == USB_DISK)
      {
        UDisk_Test();
        //在UDisk_Test具有弹出U盘的功能(逻辑拔除设备)
        //目前问题:如果是逻辑弹出U盘,即使重新拔插U盘,都无法激活U盘
      }
 */
  }
  return USB_Device;
}

/**
 * @author EutronSoftware (2013-08-16)
 *
* @brief  USB_HandleDisconnect
*         deinit demo and astart again the enumeration
* @param  None
* @retval None
*/
void USB_HandleDisconnect (void)
{
  USBH_DeInit(&USB_OTG_Core, &USB_Host);
  Enum_Done = 0;
}

/**
 * @author EutronSoftware (2013-08-16)
 *
* Dismount  USB device:
*/

void USB_Dsimount()
{
  USB_HandleDisconnect();

  USB_OTG_StopHost(&USB_OTG_Core);

  /* Manage User disconnect operations*/
  USB_Host.usr_cb->DeviceDisconnected();

  /* Re-Initilaize Host for new Enumeration */
  USBH_DeInit(&USB_OTG_Core, &USB_Host);
  USB_Host.usr_cb->DeInit();
  USB_Host.class_cb->DeInit(&USB_OTG_Core, &USB_Host.device_prop);
  USB_Device = 0;
  return;
}

/**
 * @author EutronSoftware (2013-08-16)
 *
* USB Device Enumeration Done
*/

uint8_t USB_EnumDone()
{
  return Enum_Done;
}

/**
 * @author EutronSoftware (2013-08-16)
 *
* USB Device Enumeration Done
*/

uint8_t USB_DeviceType()
{
  return USB_Device;
}
/**
 * Get the Capacity of the USBDisk
 *
 * @author EutronSoftware (2013-08-16)
 *
 * @return DWORD :Capacity in byte
 */
DWORD USB_DiskCapacity()
{
  return USBH_MSC_Param.MSCapacity * USBH_MSC_Param.MSPageLength;
}

/**
 * test the protected flag
 *
 * @author EutronSoftware (2013-08-16)
 *
 * @return uint8_t
 */
uint8_t USB_DiskProtected()
{
    if (USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)
    {
        USBH_USR_ApplicationState = USH_USR_FS_DRAW;
        return true;
    }
    else
        return false;
}

/**
 *
 *
 * @author EutronSoftware (2013-08-16)
 *
 * @return WORD
 */
WORD USB_GetHostState()
{
    return USB_Host.gState;
}
/**
 * Test The USB device is connected or not
 *
 * @author EutronSoftware (2013-08-16)
 *
 * @return BYTE: true-Connected,false-Disconnected
 */
WORD USB_DeviceConnected()
{
    return HCD_IsDeviceConnected(&USB_OTG_Core);
}

/**
 * Test The USB Disk is connected or not
 *
 * @author EutronSoftware (2013-08-16)
 *
 * @return BYTE Value:true-USBDisk is ready, false-USBDisk does
 *         not ready
 */
BYTE USB_DiskReady()
{
    if (!HCD_IsDeviceConnected(&USB_OTG_Core) || USB_Detect()!=USB_DISK)
        return false;
    else
        return true;
}
