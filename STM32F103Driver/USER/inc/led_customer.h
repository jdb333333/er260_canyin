
#ifndef __LED_CUSTOMER_H
#define __LED_CUSTOMER_H

#if defined(LCD10_DISP)//Îª10×Ö·û´ø×´Ì¬±êÇ©ÏÔÊ¾ÆÁ
#include "lcd_ht1621.h"
#else
#if defined(CASE_ER260) || defined(CASE_ER100)
#define DISP_WIDTH  	8
#else
#define DISP_WIDTH  	12
#endif


#if (DISP_WIDTH==8)
#define LED_G_CON  	0xff
#else
//#define LED_G_CON  	0x000	//#if CASE_ER520U
#define LED_G_CON  	0xfff
#endif


void LED_Init(void);
void LED_DispData(uint8_t* pBuffer, uint8_t BufferSize);
void LED_Refresh(uint8_t SigBit);
void LED_On(void);
void LED_Off(void);
void SysDelay_ms(uint16_t delay_ms);
#define LCD10_SetFlag(flag) {}
#endif
#endif // __LED_CUSTOMER_H
