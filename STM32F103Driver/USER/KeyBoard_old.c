/****************************************Copyright (c)**************************************************
**                               Hunan EUTRON information device Co.,LTD.
**
**                                 http://www.eutron.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:       KeyBoardDrv.c
** Descriptions:    键盘驱动
**
**------------------------------------------------------------------------------------------------------
** Created by:      Ruby
** Created date:    2011-12-06
** Version:         1.0
** Descriptions:    The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
********************************************************************************************************/
#include "king.h"
#include "appl.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"

#include "stm32f10x.h"
#include "KeyBoard.h"
#include "TypeDef.h"
#include "interface.h"
#include "fsmc_sram.h"
//#include "SysTick.h"


extern BYTE    KeyFrHost;       //ApplVar.Key code from host computer.
extern BYTE Bios_Key0_Key;			//    key number: 00 = No key


#define KB_ROW	0x303F

#define KEY_STATE_0 0
#define KEY_STATE_1 1
#define KEY_STATE_2 2
#define KEY_STATE_3 3

#define CLOSE_KEY	0x0b

uint8_t dis_buf[KEY_BUFFER_SIZE]= {0}; //显示缓存
uint8_t key_number[COMB_KEY_TOTAL],key_total;
uint8_t key_state = 0;
uint8_t comb_index = 0,flag_press = 0;
uint8_t bitflag_key = 0; /* bit0:有新键标志 bit1:按键按下标志 bit7:按键弹起标志 */
#ifdef DD_LOCK
uint8_t LockCurVal=0; //锁的钥匙最后位置的键值
#endif

#if CASE_ER220
uint8_t key_value[]=					   /*  扫描码与传给应用层的键值一一对应表: 数组下标代表扫描码 */
{
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x23,0x22,0x21,0x20,0x1F,0x1E,0x1D,
	 0x00,0x1C,0x1B,0x1A,0x19,0x18,0x17,0x16,
	 0x00,0x15,0x14,0x13,0x12,0x11,0x10,0x0F,
	 0x00,0x0E,0x0D,0x0C,0x0B,0x0A,0x09,0x08,
	 0x00,0x07,0x06,0x05,0x04,0x03,0x02,0x01
};

#elif CASE_ER520U
#define KB_LOCK	GPIO_Pin_3
uint8_t key_value[]=					   /*  扫描码与传给应用层的键值一一对应表: 数组下标代表扫描码 */
{
/*   关机 销售 读账 清账 经理 设置           */
	 OFF+1,RG+1,X+1, Z+1,MG+1,SET+1,0x00,0x00,
	 0x23,0x19,0x0F,0x05,0x1A,0x2C,0x10,0x06,
	 0x22,0x18,0x0E,0x04,0x1B,0x2B,0x11,0x07,
	 0x21,0x17,0x0D,0x03,0x1C,0x2A,0x12,0x08,
	 0x20,0x16,0x0C,0x02,0x1D,0x29,0x13,0x09,
	 0x1F,0x15,0x0B,0x01,0x1E,0x28,0x14,0x0A,
	 0x00,0x00,0x00,0x00,0x2D,0x00,0x24,0x25,
	 0x00,0x00,0x00,0x00,0x2E,0x00,0x2F,0x26,
	 0x00,0x00,0x00,0x00,0x27,0x00,0x30,0x00
};
#elif CASE_ER260
uint8_t key_value[]=					   /*  扫描码与传给应用层的键值一一对应表: 数组下标代表扫描码 */
{
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
	 0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,
	 0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
	 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	 0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
#elif CASE_PCR01
uint8_t key_value[]=
{
    0x00,0x25,0x1F,0x19,0x13,0x0D,0x07,0x01,
    0x00,0x26,0x20,0x1A,0x14,0x0E,0x00,0x02,
    0x00,0x27,0x21,0x1B,0x15,0x0F,0x09,0x03,
    0x00,0x22,0x24,0x18,0x16,0x10,0x0A,0x04,
    0x00,0x28,0x00,0x1E,0x1C,0x11,0x0B,0x05,
    0x00,0x00,0x12,0x0C,0x1D,0x00,0x00,0x00,
    0x00,0x00,0x00,0x06,0x17,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
#elif CASE_ER100
uint8_t key_value[]=
{
	 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x00,
	 0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x00,
	 0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x00,
	 0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x00,
	 0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x00,
	 0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x00,
	 0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x00,
	 0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x00,
};
#elif CASE_GIADA3000
uint8_t key_value[]=
{
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x17,0x06,0x00,0x00,0x00,
    0x00,0x00,0x00,0x1D,0x0C,0x12,0x00,0x00,
    0x05,0x0B,0x11,0x1C,0x1E,0x00,0x28,0x00,
    0x04,0x0A,0x10,0x16,0x18,0x24,0x22,0x00,
    0x03,0x09,0x0F,0x15,0x1B,0x21,0x27,0x00,
    0x02,0x00,0x0E,0x14,0x1A,0x20,0x26,0x00,
    0x01,0x07,0x0D,0x13,0x19,0x1F,0x25,0x00,
};
#elif CASE_MCR30
uint8_t key_value[]=
{	
	 0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x00,
	 0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x00,
	 0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x00,
	 0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x00,
	 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*	
	 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	 0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
	 0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
	 0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,
	 0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,
	 0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,
	 0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
	 0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,
	*/	
	
};

#endif

uint32_t keydown_flag = 0;	                 // 每一位代表相应键的按下状态，如第3位为1，代表键值为3的键被按下
uint8_t keydown_flag2 = 0;

uint8_t key_putin = 0;
uint8_t key_getout = 0;

volatile uint32_t Times_Of_10ms = 0;

uint8_t fBeep = true;
volatile uint16_t beep_len=0;
volatile uint8_t KeyScanEnable=0;

extern volatile BYTE DrawerOpen_time;

extern void TIM7_Init(void);

void KeyBoardDeInit(void);
void EXTILine_Init(void);
void KeyBoardInit(void)
{

#if CASE_ER520U
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KB_LOCK; //锁扫描输出信号
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif

	KeyBoardDeInit();
	TIM7_Init();
	EXTILine_Init();

}


#ifdef DD_LOCK

void lockscan(void)
{
	uint8_t i,j;
	uint16_t k_col;
	
	uint8_t cnt=0;

	GPIOC->BRR = KB_LOCK;	//锁扫描输出信号
	*(uint16_t *)(KB_ADDR) = 0xFF;
	j = 0xF0;//0x30;
	while(j--);
	k_col = *(__IO uint16_t*) (KB_ADDR);
	if(k_col != 0xFF)
	{
		for(j=0; j<8; j++)
		{
			if(!((k_col >> j)&0x01))
				cnt++;
		}
		if (cnt == 1)	//ouhs20161205 同时按下超过两行两列的键,会影响锁位置的跳变
		{
			for(j=0; j<8; j++)
			{
				if(!((k_col >> j)&0x01))
				{
					i = 0xF0;	//0x50;
					while(i--);
					if(!((k_col >> j)&0x01))
					{
						if(LockCurVal != key_value[j])
						{
							LockCurVal = key_value[j];
							dis_buf[key_putin++] =key_value[j];
							if(key_putin >= KEY_BUFFER_SIZE)
							 key_putin = 0;
						}
					}
				}
			}
		}
	}
	GPIOC->BSRR = KB_LOCK;
}

void keyscan(void)
{
	uint8_t i, j;
	uint16_t k_col;
	key_total = 0;

	lockscan();//锁扫描

	j = 0xF0;//0x80;
	while(j--);

	for(i=1; i<9; i++)
	{

		*(uint16_t *)(KB_ADDR) = ~(1<<(i-1));
		j = 0xF0;//0x80;
		while(j--);
		k_col = *(__IO uint16_t*) (KB_ADDR);
		if(k_col != 0xFF)
		{
			for(j=0; j<8; j++)
			{
				if(!((k_col >> j)&0x01))
				{
					key_total++;
					if(key_total >= COMB_KEY_TOTAL)
						key_total = COMB_KEY_TOTAL;
					key_number[key_total - 1] = key_value[(i<<3 | j)];	//获取按下的键号
				}
			}
		}
	}
}
#else
void keyscan(void)
{
	uint8_t i, j;
	uint16_t k_col;
	key_total = 0;
	for(i=0; i<8; i++)
	{
		*(uint16_t *)(KB_ADDR) = ~(1<<i);
		j = 0xC0;//0x80;
		while(j--);
		k_col = *(__IO uint16_t*) (KB_ADDR);
		if(k_col != 0xFF)
		{
			for(j=0; j<8; j++)
			{
				if(!((k_col >> j)&0x01))
				{
					key_total++;
					if(key_total >= COMB_KEY_TOTAL)
						key_total = COMB_KEY_TOTAL;
                    key_number[key_total - 1] = key_value[(i | j<<3)];	//获取按下的键号
				}
			}
		}
	}
}
#endif



void keydown(void)
{
    uint8_t i;
    static uint8_t key_total_old;

	//扫描键盘
    keyscan();

	//键盘状态处理
    switch (key_state)
    {
        //按键初始态 无闭合键
        case KEY_STATE_0:
			flag_press = 0;
			comb_index = 0;
            key_total_old = 0;
            //键被按下，转换到键确认态
            if (key_total)
            {
                key_total_old = key_total;

                key_state = KEY_STATE_1;
            }
            break;

        //按键确认态
        case KEY_STATE_1:
			//按键抖动，返回原状态
            if (key_total_old != key_total) //此时 key_total >= 1
            {
	            //初始态抖动，返回按键初始态
            	if (flag_press == 0)
	            {
	                key_state = KEY_STATE_0;
	            }
	            //等待态抖动，返回按键等待态
            	else
	            {
	                key_state = KEY_STATE_2;
	            }
            }
            //按键稳定，更新组合键队列，转入等待态
        	else
        	{

				//有新键按下
	            if (key_total > comb_index) //两者不可能相等，否则按键应处于稳定等待态
				{
					//将第一次同时按下的键放组合键队列头
		            if (0 == comb_index)
		            {
						comb_index = key_total;
						keydown_flag = 0;
						keydown_flag2 = 0;
						for (i = 0; i < key_total; i++)
		                {
							if(key_number[i]<32)
								keydown_flag |= (1UL<<key_number[i]);
							else
								keydown_flag2 |= (1UL<<(key_number[i]-32));

							if (key_number[i]>0)//ccr
								dis_buf[key_putin++] =key_number[i];
		                }
						flag_press = 1;
		            }
					//将新键加到组合键队列尾
		            else
		            {
		                for (i = 0; i < key_total; i++)
		                {
							if(key_number[i]<32)
							{
								if(!(keydown_flag&(1UL<<key_number[i])))
								{
                                    if (key_number[i]>0)//ccr
                                    {
                                        dis_buf[key_putin++] = key_number[i];
                                        comb_index++;
                                    }

								}
							}
							else
							{
								if(!(keydown_flag2&(1UL<<(key_number[i]-32))))
								{
                                    if (key_number[i]>0)//ccr
									{
										dis_buf[key_putin++] =key_number[i];
										comb_index++;
									}
								}
							}
			            }
		            }
					if(key_putin >= KEY_BUFFER_SIZE)
						 key_putin = 0;

					bitflag_key |= 0x81;	//串口发送标记

	                key_state = KEY_STATE_2;

				}
				//有键释放
				else
				{
					//构建返回值
					keydown_flag = 0;
					keydown_flag2 = 0;
					comb_index = key_total;
					for (i = 0; i < key_total; i++)
	                {
						if(key_number[i]<32)
							keydown_flag |= (1UL<<key_number[i]);
						else
							keydown_flag2 |= (1UL<<key_number[i]);
	                }

		            //所有按键释放，转换到按键初始态
	            	if (key_total == 0)
		            {
		            	flag_press = 0;
						comb_index = 0;
						keydown_flag = 0;
						keydown_flag2 = 0;
		                key_state = KEY_STATE_0;
						bitflag_key &= (~0x80);
		            }
		            //还有按键未释放，转换到按键等待态
	            	else
		            {
		                key_state = KEY_STATE_2;
		            }
				}
			}
            break;

        //等待态 有闭合键，等待新动作
        case KEY_STATE_2:
            if (key_total != key_total_old)
        	{
                key_total_old = key_total;

                key_state = KEY_STATE_1;
        	}
			break;
    	}

}
/** 读取键盘输入的按键
  * @param  None
  * @retval :key pressed
  */


uint8_t Getch(void)
{  //读取按键,在读取按键之前,先使用BiosCmd_CheckKeyborad命令检测是否有按键.

    if (TESTBIT(ApplVar.MyFlags, KEYFROMHOST))
    {
        RESETBIT(ApplVar.MyFlags, KEYFROMHOST);
        return(Bios_Key0_Key-1);
    }
	else
	{
		Bios_Key0_Key=dis_buf[key_getout++]-1;
		if(key_getout >= KEY_BUFFER_SIZE)
			 key_getout = 0;

		return (Bios_Key0_Key);
	}
}
/** 检测键盘是否有输入
  * @param  None
  * @retval true:a key pressed
  */
uint8_t CheckKeyboard(void)
{
	if (key_putin==key_getout)
		return 0;
	else
	{
#if !defined(CASE_ASCIIDISPLAY)
//		if (!TESTBIT(ApplVar.ScreenStat,LIGHTON))
			mLightLCDOn();
        LED_On();
#endif
		return (dis_buf[key_getout]);
	}
}

/** 允许或禁止键盘蜂鸣器
  * @param  on_off:
  * @retval None
  */
void EnableBeep(uint8_t on_off)
{
	fBeep = on_off;
}

/** 用来处理键盘按键输入
  * @brief  This function handles TIM7 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM7_IRQHandler(void)
{
//	OSIntEnter();
	//if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

		//Times_Of_10ms++;
#ifndef CASE_ER520U	//因使用方式锁，需要取消KB_INT中断触发扫描,否则会频繁产生中断
		if (KeyScanEnable < 10)		//100ms
#endif
		{
			keydown();
#ifndef CASE_ER520U	//
			KeyScanEnable++;
			KeyBoardDeInit();
#endif
		}

		if((bitflag_key & 0x01))
		{
			if(fBeep)
				Beep(1);
			//Times_Of_10ms = 0;
		}
		else
		{
			//if(!(bitflag_key & 0x80))
				//Times_Of_10ms = 0;
		}
#if (0) //testonly
		if(key_putin != key_getout)
		{
			//xputc(dis_buf[key_getout++]);     //testonly
			USART_printf("KeyValue:0x%x \r\n",dis_buf[key_getout++]);
			if(key_getout >= KEY_BUFFER_SIZE)
				key_getout = 0;
		}
#endif
		bitflag_key &= (~0x01);

		/* start beep: beep_len*10ms  */
		if(beep_len)
		{
			if (--beep_len==0)
				BEEP_OFF();
		}
		/* start DRAWER pulse output:DrawerOpen_time*10ms  */

		if(DrawerOpen_time)
		{
			if(--DrawerOpen_time == 0)
				DRAWER_OFF();
		}

		/* end DRAWER pulse output   */
	}
//	OSIntExit();
}

void KeyBoardDeInit(void)
{
	*(uint16_t *)(KB_ADDR) = 0;	// 将KB_C[7..0]设置为低电平
#ifdef CASE_ER520U
	GPIOC->BRR = KB_LOCK;	//锁扫描输出信号
#endif
}

/*********************************************************************************************************
** Function name: EXTILine_Init
** Descriptions:  外部中断信号初始化函数，包括键盘输入中断、掉电检测中断。
*********************************************************************************************************/
void EXTILine_Init(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);

	/* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// PE2掉电检测信号,PE6键盘检测信号
	/* Configure pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/*  PA8 以太网中断信号 */

	/* Connect EXTI Line to Selected pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);   //PFO

	/* Configure EXTI Line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	  // 下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

    /* Configure the NVIC Preemption Priority Bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

#ifndef CASE_ER520U	//因使用方式锁，需要取消KB_INT中断触发扫描,否则会频繁产生中断

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource6); 	//KB_INT

	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  	  // 上升沿
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //ouhs 20140814 =0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //ouhs 20140814 =3
	NVIC_Init(&NVIC_InitStructure);
#endif

}
//ccr clean keyboar data
void CleanKeyboard()
{
    key_putin = 0;
    key_getout = key_putin;

}
/*********************************************** End Of File *******************************************/
