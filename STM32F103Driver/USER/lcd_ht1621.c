#include "king.h"
#include "exthead.h"

#include "stm32f10x.h"
#include "lcd_ht1621.h"
#include "string.h"


/*
//          a
//        ┏━┓
//       f┃g ┃ b
//        ┣━┫
//       e┃d ┃ c
//        ┗━┛● DP
*/

#define	HT1621_DATA_High()		GPIOF->BSRR = GPIO_Pin_10
#define	HT1621_DATA_Low()		GPIOF->BRR =  GPIO_Pin_10
#define	HT1621_WR_High()	 	GPIOG->BSRR = GPIO_Pin_14
#define	HT1621_WR_Low()	 	GPIOG->BRR =  GPIO_Pin_14
#define	HT1621_CS_High()	GPIOF->BSRR = GPIO_Pin_9
#define	HT1621_CS_Low()		GPIOF->BRR =  GPIO_Pin_9


#define HT1621_RCOSC 	0x18	//on-chip RC oscillator
#define HT1621_BIAS 	0x29	//1/3bias,4commons
#define HT1621_SYSEN 	0x01	//Turn on system oscillator
#define HT1621_LCDOFF 0x02	//Turn off LCD bias generator
#define HT1621_LCDON 	0x03	//Turn on LCD bias generator





/* LED字模, 共阴数码管 */
const uint8_t HT1621_TAB[] = {
	   //pcbadegf
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 32 blank
0xE0,//11100000B,//0x86,//0x61,//01100001B    ; 33 !
0x21,//00100001B,//0x22,//0x44,//01000100B    ; 34 @
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 35 #
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 36 $
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 37 %
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 38 &
0x20,//00100000B,//0x02,//0x40,//01000000B    ; 39 '
0x1D,//00011101B,//0x39,//0x9C,//10011100B    ; 40 (
0x78,//01111000B,//0x0F,//0xF0,//11110000B    ; 41 )
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 42 *
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 43 +
0x80,//10000000B,//0x80,//0x01,//00000001B    ; 44 ,
0x02,//00000010B,//0x40,//0x02,//00000010B    ; 45 -
0x80,//10000000B,//0x80,//0x01,//00000001B    ; 46 .	
0x26,//00100110B,//0x52,//0x4A,//01001010B    ; 47 /	
0x7D,//01111101B,//0x3F,//0xFC,//11111100B    ; 48 0	
0x60,//01100000B,//0x06,//0x60,//01100000B    ; 49 1	
0x3E,//00111110B,//0x5B,//0xDA,//11011010B    ; 50 2	
0x7A,//01111010B,//0x4F,//0xF2,//11110010B    ; 51 3	
0x63,//01100011B,//0x66,//0x66,//01100110B    ; 52 4	
0x5B,//01011011B,//0x6D,//0xB6,//10110110B    ; 53 5  	
0x5F,//01011111B,//0x7D,//0xBE,//10111110B    ; 54 6
0x70,//01110000B,//0x07,//0xE0,//11100000B    ; 55 7
0x7F,//01111111B,//0x7F,//0xFE,//11111110B    ; 56 8
0x7B,//01111011B,//0x6F,//0xF6,//11110110B    ; 57 9
0x18,//00011000B,//0x09,//0x90,//10010000B    ; 58 :
0x0A,//00001010B,//0x48,//0x12,//00010010B    ; 59 ;
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 60 <
0x0A,//00001010B,//0x48,//0x12,//00010010B    ; 61 =
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 62 >
0x08,//00001000B,//0x08,//0x10,//00010000B    ; 63 _
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 64 @
0x77,//01110111B,//0x77,//0xEE,//11101110B    ; 65 A
0x4F,//01001111B,//0x7C,//0x3E,//00111110B    ; 66 B
0x1D,//00011101B,//0x39,//0x9C,//10011100B    ; 67 C
0x6E,//01101110B,//0x5E,//0x7A,//01111010B    ; 68 D
0x1F,//00011111B,//0x79,//0x9E,//10011110B    ; 69 E
0x17,//00010111B,//0x71,//0x8E,//10001110B    ; 70 F
0x5D,//01011101B,//0x3D,//0xBC,//10111100B    ; 71 G
0x67,//01100111B,//0x76,//0x6E,//01101110B    ; 72 H
0x05,//00000101B,//0x30,//0x0C,//00001100B    ; 73 I
0x6C,//01101100B,//0x1E,//0x78,//01111000B    ; 74 J
0x27,//00100111B,//0x72,//0x4E,//01001110B    ; 75 K
0x0D,//00001101B,//0x38,//0x1C,//00011100B    ; 76 L
0x75,//01110101B,//0x37,//0xEC,//11101100B    ; 77 M
0x46,//01000110B,//0x54,//0x2A,//00101010B    ; 78 N
0x4E,//01001110B,//0x5C,//0x3A,//00111010B    ; 79 O
0x37,//00110111B,//0x73,//0xCE,//11001110B    ; 80 P
0x73,//01110011B,//0x67,//0xE6,//11100110B    ; 81 Q
0x06,//00000110B,//0x50,//0x0A,//00001010B    ; 82 R
0x5B,//01011011B,//0x6D,//0xB6,//10110110B    ; 83 S
0x0F,//00001111B,//0x78,//0x1E,//00011110B    ; 84 T
0x4C,//01001100B,//0x1C,//0x38,//00111000B    ; 85 U
0x6D,//01101101B,//0x3E,//0x7C,//01111100B    ; 86 V
0x2B,//00101011B,//0x6A,//0x56,//01010110B    ; 87 W
0x1A,//00011010B,//0x49,//0x92,//10010010B    ; 88 X
0x6B,//01101011B,//0x6E,//0x76,//01110110B    ; 89 Y
0x3E,//00111110B,//0x5B,//0xDA,//11011010B    ; 90 Z
0x1D,//00011101B,//0x39,//0x9C,//10011100B    ; 91 [
0x43,//01000011B,//0x64,//0x26,//00100110B    ; 92 \;
0x78,//01111000B,//0x0F,//0xF0,//11110000B    ; 93 ]
0x31,//00110001B,//0x23,//0xC4,//11000100B    ; 94 ^
0x08,//00001000B,//0x08,//0x10,//00010000B    ; 95 _
0x00,//00000000B,//0x00,//0x00,//00000000B    ; 96 '
0x77,//01110111B,//0x77,//0xEE,//11101110B    ;  a
0x4F,//01001111B,//0x7C,//0x3E,//00111110B    ;  b
0x1D,//00011101B,//0x39,//0x9C,//10011100B    ;  c
0x6E,//01101110B,//0x5E,//0x7A,//01111010B    ;  d
0x1F,//00011111B,//0x79,//0x9E,//10011110B    ;  e
0x17,//00010111B,//0x71,//0x8E,//10001110B    ;  f
0x5D,//01011101B,//0x3D,//0xBC,//10111100B    ;  g
0x67,//01100111B,//0x76,//0x6E,//01101110B    ;  h
0x05,//00000101B,//0x30,//0x0C,//00001100B    ;  i
0x6C,//01101100B,//0x1E,//0x78,//01111000B    ;  j
0x27,//00100111B,//0x72,//0x4E,//01001110B    ;  k
0x0D,//00001101B,//0x38,//0x1C,//00011100B    ;  l
0x75,//01110101B,//0x37,//0xEC,//11101100B    ;  m
0x46,//01000110B,//0x54,//0x2A,//00101010B    ;  n
0x4E,//01001110B,//0x5C,//0x3A,//00111010B    ;  o
0x37,//00110111B,//0x73,//0xCE,//11001110B    ;  p
0x73,//01110011B,//0x67,//0xE6,//11100110B    ;  q
0x06,//00000110B,//0x50,//0x0A,//00001010B    ;  r
0x5B,//01011011B,//0x6D,//0xB6,//10110110B    ;  s
0x0F,//00001111B,//0x78,//0x1E,//00011110B    ;  t
0x4C,//01001100B,//0x1C,//0x38,//00111000B    ;  u
0x6D,//01101101B,//0x3E,//0x7C,//01111100B    ;  v
0x2B,//00101011B,//0x6A,//0x56,//01010110B    ;  w
0x1A,//00011010B,//0x49,//0x92,//10010010B    ;  x
0x6B,//01101011B,//0x6E,//0x76,//01110110B    ;  y
0x3E,//00111110B,//0x5B,//0xDA,//11011010B    ;  z
};

uint8_t HT1621_Buffer[DISP_WIDTH] = {0xfd,0x7d,0x7d,};	//0.00

char HT1621_Saved[DISP_WIDTH];//保护显示内容


static void HT1621_Delay(void);
static void HT1621_SendBitH(uint8_t dat, uint8_t cnt); 
static void HT1621_SendBitL(uint8_t dat, uint8_t cnt);
static void HT1621_WriteCmd(uint8_t cmd);
static void HT1621_WriteData(uint8_t addr, uint8_t dat, uint8_t cnt);

void HT1621_ClearScreen(void);

void HT1621_Init(void)
{
	uint16_t i=0,j=500;
	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

	// GPIO_Pin_9--HT1621_CS, GPIO_Pin_10--HT1621_DATA, GPIO_Pin_14--HT1621_WR
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	HT1621_Delay();HT1621_Delay();HT1621_Delay();HT1621_Delay();HT1621_Delay();

	HT1621_WriteCmd(HT1621_BIAS);
	HT1621_WriteCmd(HT1621_RCOSC);
	HT1621_WriteCmd(HT1621_SYSEN);
	HT1621_WriteCmd(HT1621_LCDON);
	
	HT1621_ClearScreen();

#if(1)
	PutsO("  0123456789");
	LCD10_SetFlag(FLAG_PRICE+FLAG_TOTAL+FLAG_COLLECT+FLAG_CHANGE);
	while (j--)  for(i=0; i<12000; i++); j=500;
#endif
	PutsO("        0.00");		
	LCD10_SetFlag(FLAG_TOTAL);
}

static void HT1621_Delay(void)
{
	uint8_t i;
	//while (cnt--)
		for(i=0; i<20; i++);
}

//将dat高cnt位写入ht1621，高位在前
static void HT1621_SendBitH(uint8_t dat, uint8_t cnt) 
{
	uint8_t i;
	for(i =0; i <cnt; i++)
	{
		HT1621_WR_Low();
		HT1621_Delay();		
		if(dat&0x80) 
			HT1621_DATA_High();
		else
			HT1621_DATA_Low();
		HT1621_Delay();
		HT1621_WR_High();
		HT1621_Delay();
		dat<<=1;
	}
}

//将dat低cnt位写入ht1621，低位在前
static void HT1621_SendBitL(uint8_t dat, uint8_t cnt)
{
	uint8_t i;
	for(i =0; i <cnt; i++)
	{
		HT1621_WR_Low();
		HT1621_Delay();
		if(dat&0x01) 
			HT1621_DATA_High();
		else
			HT1621_DATA_Low();
		HT1621_Delay();
		HT1621_WR_High();
		HT1621_Delay();
		dat>>=1;
	}
}

static void HT1621_WriteCmd(uint8_t cmd)
{
	HT1621_CS_Low();
	HT1621_Delay();
	HT1621_SendBitH(0x80, 3);
	HT1621_SendBitH(cmd, 9);
	HT1621_CS_High();
	HT1621_Delay();
}

static void HT1621_WriteData(uint8_t addr, uint8_t dat, uint8_t cnt)
{
	addr<<=2;
	
	HT1621_CS_Low();
	HT1621_Delay();
	HT1621_SendBitH(0xa0, 3);
	HT1621_SendBitH(addr, 6);
	HT1621_SendBitL(dat, cnt);
	HT1621_CS_High();
	HT1621_Delay();
}

// 刷新LED显示
void HT1621_Refresh(void)
{
	uint8_t pos_shift;
	if(pos_shift>=DISP_WIDTH)
			pos_shift = 0;

	for(pos_shift=0; pos_shift<DISP_WIDTH; pos_shift++)	
	{		
			HT1621_WriteData(pos_shift*2, HT1621_Buffer[pos_shift], 8);
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
        //str += 2;//显示10位,剔除高两位

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
                sDispBuf[slen]=HT1621_TAB[ch-0x20] | sDot;
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
            sDispBuf[slen]=HT1621_TAB[sDispBuf[slen]-0x20];

        memcpy(HT1621_Buffer,sDispBuf,DISP_WIDTH);
        memcpy(HT1621_Saved,sDispBuf,DISP_WIDTH);
    }	
			HT1621_Refresh();
}


//显示被保护的内容
void PutsO_Saved(void)
{
    memcpy(HT1621_Buffer,HT1621_Saved,DISP_WIDTH);
}


/*更新LCD10显示标示
 * 
 * @param flag = 	FLAG_TOTAL 
 * 								FLAG_COLLECT
 * 								FLAG_CHANGE
 *  							FLAG_PRICE
 */ 
void LCD10_SetFlag(uint8_t flag)
{
	HT1621_WriteData(20, flag, 4);
}


// 
void LED_Off(void)
{
//	HT1621_ClearScreen();
//	HT1621_WriteCmd(HT1621_LCDOFF);
}

// 
void LED_On(void)
{
//	HT1621_WriteCmd(HT1621_LCDON);
}

void HT1621_ClearScreen(void)
{
	uint8_t i;
	for(i=0; i<=DISP_WIDTH; i++)
	{
		HT1621_WriteData(i*2, 0, 8);
	}
}


// TIM6 500us  中断处理
volatile uint16_t HT1621_TimeDelay;
extern void mDispCursor(void);
void TIM6_IRQHandler(void)
{
  static int CursorTime=0;
	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
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

}


