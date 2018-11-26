/**
 * CASE_ASCIIDISPLAY:表示客显和主显均为纯ASCII字符显示,否则主显为大屏幕点阵汉字显示
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INCLUDE_H
#define __INCLUDE_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

//#include "debug.h"
#include "stm32f10x.h"
#include "SysTick.h"
#include "fsmc_sram.h"
#include "usart.h"
#include "sdio_sd.h"
#include "spi_m25p80.h"
#include "flash_stm32.h"

#if !defined(CASE_ASCIIDISPLAY)
#include "lcd_st7565r.h"
#include "led_customer.h"
#include "fsmc_nor.h"
#endif

#include "pwr_ctrl.h"

#include "TypeDef.h"

#include "ff.h"     //Fat Fs

#define EN_PRINT    1
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define StartTimer(TIMx)    TIMx->CR1 |= TIM_CR1_CEN        //TIM_Cmd(TIM2, ENABLE)
#define StopTimer(TIMx)     TIMx->CR1 &= (uint16_t)~TIM_CR1_CEN //TIM_Cmd(TIM2, DISABLE)

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
