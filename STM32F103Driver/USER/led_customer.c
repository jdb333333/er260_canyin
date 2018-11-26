
#if defined(LCD10_DISP)//为10字符带状态标签显示屏
#include "lcd_ht1621.c"
#else

#include "king.h"
#include "exthead.h"

#include "stm32f10x.h"
#include "led_customer.h"
#include "string.h"


/*
//          a
//        ┏━┓
//       f┃g ┃ b
//        ┣━┫
//       e┃d ┃ c
//        ┗━┛● DP
*/

#define	LED_DATA_High()		GPIOF->BSRR = GPIO_Pin_10
#define	LED_DATA_Low()		GPIOF->BRR =  GPIO_Pin_10
#define	LED_CLK_High()	 	GPIOG->BSRR = GPIO_Pin_14
#define	LED_CLK_Low()	 	GPIOG->BRR =  GPIO_Pin_14
#define	LED_LATCH_High()	GPIOF->BSRR = GPIO_Pin_9
#define	LED_LATCH_Low()		GPIOF->BRR =  GPIO_Pin_9


/* LED字模, 共阴数码管 */
const uint8_t LED_TAB[] = {
        0x00,//0x00,//00000000B    ; 32 blank
        0x86,//0x61,//01100001B    ; 33 !
        0x22,//0x44,//01000100B    ; 34 @
        0x00,//0x00,//00000000B    ; 35 #
        0x00,//0x00,//00000000B    ; 36 $
        0x00,//0x00,//00000000B    ; 37 %
        0x00,//0x00,//00000000B    ; 38 &
        0x02,//0x40,//01000000B    ; 39 '
        0x39,//0x9C,//10011100B    ; 40 (
        0x0F,//0xF0,//11110000B    ; 41 )
        0x00,//0x00,//00000000B           ; 42 *
        0x00,//0x00,//00000000B           ; 43 +
        0x80,//0x01,//00000001B           ; 44 ,
        0x40,//0x02,//00000010B           ; 45 -
        0x80,//0x01,//00000001B           ; 46 .
        0x52,//0x4A,//01001010B           ; 47 /
        0x3F,//0xFC,//11111100B           ; 48 0
        0x06,//0x60,//01100000B           ; 49 1
        0x5B,//0xDA,//11011010B           ; 50 2
        0x4F,//0xF2,//11110010B           ; 51 3
        0x66,//0x66,//01100110B           ; 52 4
        0x6D,//0xB6,//10110110B           ; 53 5
        0x7D,//0xBE,//10111110B           ; 54 6
        0x07,//0xE0,//11100000B           ; 55 7
        0x7F,//0xFE,//11111110B           ; 56 8
        0x6F,//0xF6,//11110110B           ; 57 9
        0x09,//0x90,//10010000B           ; 58 :
        0x48,//0x12,//00010010B           ; 59 ;
        0x00,//0x00,//00000000B           ; 60 <
        0x48,//0x12,//00010010B           ; 61 =
        0x00,//0x00,//00000000B           ; 62 >
        0x08,//0x10,//00010000B           ; 63 _
        0x00,//0x00,//00000000B           ; 64 @
        0x77,//0xEE,//11101110B           ; 65 A
        0x7C,//0x3E,//00111110B           ; 66 B
        0x39,//0x9C,//10011100B           ; 67 C
        0x5E,//0x7A,//01111010B           ; 68 D
        0x79,//0x9E,//10011110B           ; 69 E
        0x71,//0x8E,//10001110B           ; 70 F
        0x3D,//0xBC,//10111100B           ; 71 G
        0x76,//0x6E,//01101110B           ; 72 H
        0x30,//0x0C,//00001100B           ; 73 I
        0x1E,//0x78,//01111000B           ; 74 J
        0x72,//0x4E,//01001110B           ; 75 K
        0x38,//0x1C,//00011100B           ; 76 L
        0x37,//0xEC,//11101100B           ; 77 M
        0x54,//0x2A,//00101010B           ; 78 N
        0x5C,//0x3A,//00111010B           ; 79 O
        0x73,//0xCE,//11001110B           ; 80 P
        0x67,//0xE6,//11100110B           ; 81 Q
        0x50,//0x0A,//00001010B           ; 82 R
        0x6D,//0xB6,//10110110B           ; 83 S
        0x78,//0x1E,//00011110B           ; 84 T
        0x1C,//0x38,//00111000B           ; 85 U
        0x3E,//0x7C,//01111100B           ; 86 V
        0x6A,//0x56,//01010110B           ; 87 W
        0x49,//0x92,//10010010B           ; 88 X
        0x6E,//0x76,//01110110B           ; 89 Y
        0x5B,//0xDA,//11011010B           ; 90 Z
        0x39,//0x9C,//10011100B           ; 91 [
        0x64,//0x26,//00100110B           ; 92 \;
        0x0F,//0xF0,//11110000B           ; 93 ]
        0x23,//0xC4,//11000100B           ; 94 ^
        0x08,//0x10,//00010000B           ; 95 _
        0x00,//0x00,//00000000B           ; 96 '
        0x77,//0xEE,//11101110B           ;  a
        0x7C,//0x3E,//00111110B           ;  b
        0x39,//0x9C,//10011100B           ;  c
        0x5E,//0x7A,//01111010B           ;  d
        0x79,//0x9E,//10011110B           ;  e
        0x71,//0x8E,//10001110B           ;  f
        0x3D,//0xBC,//10111100B           ;  g
        0x76,//0x6E,//01101110B           ;  h
        0x30,//0x0C,//00001100B           ;  i
        0x1E,//0x78,//01111000B           ;  j
        0x72,//0x4E,//01001110B           ;  k
        0x38,//0x1C,//00011100B           ;  l
        0x37,//0xEC,//11101100B           ;  m
        0x54,//0x2A,//00101010B           ;  n
        0x5C,//0x3A,//00111010B           ;  o
        0x73,//0xCE,//11001110B           ;  p
        0x67,//0xE6,//11100110B           ;  q
        0x50,//0x0A,//00001010B           ;  r
        0x6D,//0xB6,//10110110B           ;  s
        0x78,//0x1E,//00011110B           ;  t
        0x1C,//0x38,//00111000B           ;  u
        0x3E,//0x7C,//01111100B           ;  v
        0x6A,//0x56,//01010110B           ;  w
        0x49,//0x92,//10010010B           ;  x
        0x6E,//0x76,//01110110B           ;  y
        0x5B,//0xDA,//11011010B           ;  z
};

uint8_t LED_Buffer[DISP_WIDTH] = {0xbf,0x3f,0x3f,};	//0.00

char LED_Saved[DISP_WIDTH];//保护显示内容

uint8_t pos_shift=0;

uint16_t LED_G;
uint16_t LED_Ctrl=0;

uint8_t LedReady=0;//标示LED客显是否初始化允许显示,主要是用来控制JLINK.

static void LED_SendBit(uint8_t databit);
static void LED_SendData(uint16_t pos, uint8_t dat, uint8_t SigBit);
static void LED_Delay(void);

void LED_Test(void);

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

	// GPIO_Pin_9--LED_LATCH(CS), GPIO_Pin_10--LED_DATA, GPIO_Pin_14--LED_CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	LED_SendData(LED_G_CON, 0xFF, 3); //防开机初次显示乱码

	LED_Ctrl = 500;	//500ms后刷新LED显示
    LedReady = 1;
}

static void LED_Delay(void)
{
	uint8_t i;
	//while (cnt--)
		for(i=0; i<10; i++);
}

// GPIO模拟SPI通讯发送位数据
static void LED_SendBit(uint8_t databit)
{
	if (databit & 0x01)
		LED_DATA_High();
	else
		LED_DATA_Low();
	LED_Delay();
	LED_CLK_High();
	LED_Delay();
	LED_CLK_Low();
}

// 发送LED位置、数据
static void LED_SendData(uint16_t pos, uint8_t dat, uint8_t SigBit)
{
	uint8_t i;
	LED_LATCH_Low();
	for (i=0; i<DISP_WIDTH; i++)	  // 位置
	{
		LED_SendBit(pos>>i);
	}
	for (i=0; i<8; i++)				
	{	
		if (SigBit==0)				  
			LED_SendBit((dat&0x0F)>>i); //发送低4位数据
		else if(SigBit==1)
			LED_SendBit((dat&0xF0)>>i);	//发送高4位数据
		else
			LED_SendBit(dat>>i);
	}
	LED_LATCH_High();
}

// 刷新LED显示
void LED_Refresh(uint8_t SigBit)
{
    if (LedReady==1)
    {
        if(pos_shift>=DISP_WIDTH)
            pos_shift = 0;
		LED_G = LED_G_CON;				

				
#if 0 
		LED_G &= ~(1<<(DISP_WIDTH-pos_shift-1));	
		LED_SendData(LED_G, LED_Buffer[pos_shift], SigBit); //一个8段数字分两次发送
        
		if (SigBit==1)
			pos_shift++;
#else
		LED_G &= ~(1<<(DISP_WIDTH-pos_shift-1));	
		LED_SendData(LED_G, LED_Buffer[pos_shift], 3);//一个8段数字一次发送
		
		pos_shift++;

#endif		
    }
}

// 更新需要显示的数据
void PutsO( const char *str)
{
	int		sCp,slen;
	char	sDot,sExit;
	uint8_t	ch;
	uint8_t	sDispBuf[DISP_WIDTH];


    if (str)
    {
#ifdef CASE_ER260
        str += 4;//只有8位,剔除高四位
#endif

        if (!str[0])
            return ;
        memset(sDispBuf,' ',DISP_WIDTH);
        slen=strlen(str);
        if (slen>DISP_WIDTH)
            slen = DISP_WIDTH;
        memcpy(sDispBuf,str,slen);
        slen=DISP_WIDTH-1;

        sCp = slen;
        sDot = sExit = 0;

        ch = sDispBuf[DISP_WIDTH-1];
        while (sCp>=0)
        {
            if (ch!=' ')
                sExit = 1;

            if (ch=='.')
            {//遇到小数点
                sDot=0x80;
                sCp--;
            }
            else //if (sCp<slen)
            {
                sDispBuf[slen]=LED_TAB[ch-0x20] | sDot;
                sDot = 0;
                slen--;
                sCp--;
            }
            ch = sDispBuf[sCp];
            if (ch==' ' && sExit)
                break;
        }
        for (;slen>=0 && sDispBuf[slen]!=' ';slen--)
            sDispBuf[slen]=0;//fill the first with ' '
        for (;slen>=0;slen--)
            sDispBuf[slen]=LED_TAB[sDispBuf[slen]-0x20];

        memcpy(LED_Buffer,sDispBuf,DISP_WIDTH);
        memcpy(LED_Saved,sDispBuf,DISP_WIDTH);
    }

}

//显示被保护的内容
void PutsO_Saved(void)
{
    memcpy(LED_Buffer,LED_Saved,DISP_WIDTH);
}

// 关闭定时刷新，向所有LED写0关闭显示
void LED_Off(void)
{
	LED_Ctrl = 0;
	LED_SendData(LED_G_CON, 0, 0);
	LED_SendData(LED_G_CON, 0, 1);
}

// 点亮LED,显示前次显示的数据
void LED_On(void)
{
	LED_Ctrl = 1;
}

// TIM6 500us  中断处理
volatile uint16_t LED_TimeDelay;
extern void mDispCursor(void);
void TIM6_IRQHandler(void)
{
  static int CursorTime=0;
	static int SigBit=0;
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		/*  LED客显  */
		if(LED_Ctrl > 1)
			LED_Ctrl--;
		else if(LED_Ctrl==1)
			LED_Refresh(SigBit);
		SigBit ^= 1;

		 /*  LCD光标闪烁 */
        CursorTime++;
        if (CursorTime>=500
           && TESTBIT(ApplVar.ScreenStat,(CURSOREN | CRITICAL))==CURSOREN)
        {
           mDispCursor();
           CursorTime = 0;
        }
        if (TESTBIT(ApplVar.ScreenStat,CRITICAL))
            CursorTime = 0;

		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
    if(LED_TimeDelay)
        LED_TimeDelay--;

}

// 延时ms
void SysDelay_ms(uint16_t delay_ms)
{
	LED_TimeDelay = delay_ms*2;
	while(LED_TimeDelay);
}
#endif
