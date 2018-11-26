  /****************************************Copyright (c)**************************************************
**                               Hunan EUTRON information device Co.,LTD.
**                                      
**                                 http://www.eutron.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:            KeyBoardDrv.h
** Last modified Date:   2011-12-06
** Last Version:         1.0
** Descriptions:         LCD的操作函数
**
**------------------------------------------------------------------------------------------------------
** Created By:           Ruby
** Created date:         2011-12-06
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

#ifndef __KEYDRV_H 
#define __KEYDRV_H

#define COMB_KEY_TOTAL 4
#define KEY_BUFFER_SIZE 10

extern uint8_t key_putin;
extern uint8_t key_getout;

extern uint8_t dis_buf[KEY_BUFFER_SIZE]; //显示缓存

extern void KeyBoardInit(void);

#endif

/********************************************************************************************************
** End Of File
*********************************************************************************************************/

