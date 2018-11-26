//-----------------------------------------------
// File : BIOS_DIR.H
//       -----------
//---------------------------------------------
//CASE_ER220 = 0
//---------------------------------------------
//
#ifndef BIOS_DIR_H
#define BIOS_DIR_H

//#if defined(STM32F10X_HD)
//#define  STM32F10X_HD_USBCOMM
//#endif

//#define DD_LOCK		0		//For 520 是否启用方式锁 //

//#define CASE_GPRS		0		//FOR GPRS MODEL..090815

#if defined(CASE_ETHERNET)
#define TCPIP		    1		//change to open TCP/IP FUNCTION .FROM 0->1
#endif

//------------------------------------------------------------------------
#if (defined(DEBUGBYPC))

//#define CASE_ER220		1		            // 	;采用串行打印字库 //
#define CASE_ER260		    1		// 	;采用串行打印字库 //
//#define CASE_ER520U		0		// 	;采用并行字库,显示字库与打印字库放在同一芯片,有portW2管理 //
//#define CASE_GIADA3000    0

//#define CASE_INDONESIA    1       //印度尼西亚版本

#if defined(CASE_INDONESIA)
#define CASE_GPRS		1		//FOR GPRS MODEL..090815
#else
//#define CASE_FORHANZI	1		// 是否支持汉字打印 //
#endif

#endif

//#define CASE_SPANISH    0       // 是否为西班牙语版本

//--------------------------------------------------------------------------

// Definizioni riguardanti l'hardware

#define DD_S1245		0
#define DD_FTP628		1
#define DD_Z245		0

//-------------------------------------------------

#define	DD_CUTTER	    1	//1 = con taglierina automatica
//------------------------------------------------------------------------------
#if defined(CASE_FORHANZI)
#define EN_EXASCII      0   //汉字方式下，禁止扩展ASCII输入方式
#else
#define EN_EXASCII      1   //英文版本，允许扩展ASCII输入方式
#endif

//--------------------------------------------------------
#ifdef CASE_GPRS
#define DD_CHIPC		0		//IC卡模块和GPRS模块公用一个端口
#else
#define DD_CHIPC		1		//ccr chipcard	//1 = SI gestione chip-card
#endif

#define DD_ECRVIP       1       //ccr2016-05-13 启用ECRVIP会员功能(必备功能)

#if defined(DEBUGBYPC)
#define CASE_MFRIC		0		//mifare RFIC,PC平台下禁止MFRIC
#else
#define CASE_MFRIC		1		//mifare RFIC,当启用DD_CHIPC后,CASE_MFRIC决定是射频卡还是接触卡
#endif

#if (CASE_MFRIC==1)
#define DD_4442IC		0
#else
#define DD_4442IC		1
#endif

#define B_SIOF_TX	     0	//bit 0 : 1 = In atto tasmissione di un blocco
#define B_SIOF_PWFL	     9	//bit 9  : riservato per segnalazione pwfl in RX      (NON MODIFICARE !)
#define B_SIOF_TMORX	 10	//bit 10 : riservato per segnalazione time-out in RX  (NON MODIFICARE !)
#define B_SIOF_OVFL	     11	//bit 11 : 1 = Rx buffer in overflow		     (NON MODIFICARE !)
#define B_SIOF_OVERRUN	 12	//bit 12 : Rilevati errori di overrun                 (NON MODIFICARE !)
#define B_SIOF_FRAME	 13	//bit 13 : Rilevati errori di frame                   (NON MODIFICARE !)
#define B_SIOF_PARITY	 14	//bit 14 : Rilevati errori di parita'                 (NON MODIFICARE !)
#define B_SIOF_ERRORS	 15	//bit 15 : OR dei bit di errore                       (NON MODIFICARE !)



#define CASE_EPOS			0	//=1,启用外置储值卡进行付款

#if defined(STM32F10X_HD)
#define CASE_USBDISK		0	//=1,启用U盘功能
#else
#define CASE_USBDISK		1	//=1,启用U盘功能
#endif


/* Central Lock Codes as used by the MA  register */
/* now also used for KT because KT translated to MA  */

#endif
