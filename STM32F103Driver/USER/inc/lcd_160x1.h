/****************************************Copyright (c)**************************************************
**                               Hunan EUTRON information Equipment Co.,LTD.
**
**                                 http://www.eutron.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:            LCD_Drv.h
** Last modified Date:   2011-12-05
** Last Version:         1.0
** Descriptions:         LCD的操作函数
**
**------------------------------------------------------------------------------------------------------
** Created By:           Ruby
** Created date:         2011-12-05
** Version:              1.0
** Descriptions:         First version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/

#ifndef __LCD_160X1_H
#define __LCD_160X1_H

#include "stm32f10x.h"

#define     DISLEN 		16

/*
LCD_BL_CTRL:
   bit0:=1,为打开背光方式
   bit1:=1,客显与主显显示相同的内容;=0,客显与主显显示不同的内容
*/
#define 	LCD_BL_ON()		GPIOB->BSRR = LCD_BL  //LCD_BL_CTRL |= 1
#define 	LCD_BL_OFF()	GPIOB->BRR = LCD_BL  //LCD_BL_CTRL &= ~1
//#define		LCD_ADJ_VAL		1   //调显示亮度,值越小越亮

extern volatile uint8_t LCD_BL_CTRL;

void LCDInit(void);
void LCDOpen(void);
void LCDClose(void);

void PutsO(const char *str);
void PutsO_Only(const char *str);
void PutsO_Saved(void);
void PutsC(const char *str);
void PutsC_Only(const char *str);
void PutsC_Saved(void);

uint8_t GetsO_At(uint8_t addr);
void PutsO_At(uint8_t ch,uint8_t addr);
void Save_LCD(char *saveO,char *saveC);

#endif	// __LCD_160X_H

/********************************************************************************************************
** End Of File
*********************************************************************************************************/

