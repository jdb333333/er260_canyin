
#include <stdint.h>
#include "king.h"
#include "exthead.h"
//#include "exth.h"

#include "stm32f10x.h"
#include "TypeDef.h"
#include "lcd_st7565r.h"
#include "Flashb.h"

#ifdef CHAR_6X8

#define Y_Height	8
#define X_Width		6
extern const uint8_t ASCIITAB_5x8[ ];

#else
#define Y_Height	16			// 字体高度
#define X_Width		8				// 字体最小宽度
#endif

uint8_t ActiveLines = SCREENLN;

uint8_t DotBuffer[8][128]; //page addr:8  column addr:128

const uint8_t pagemap[8] = {3, 2, 1, 0, 7, 6, 5, 4,};
extern const uint8_t ASCIITAB_8x16[];
extern const uint8_t RByte[256];

void ST7565R_IO_Init(void);
void ST7565R_WR_CMD(uint8_t cmd);
void ST7565R_WR_DATA(uint8_t dat);
void st7565SetXY(uint8_t pos_x,uint8_t pos_y);
void st7565DrawPixel( uint8_t x, uint8_t y );
void st7565ClearPixel( uint8_t x, uint8_t y );
void st7565Refresh( void );
void mDispLogo(void);


extern uint8_t NorFlash_WReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

/**************************************************************************/
/*  延时约1ms
***************************************************************************/
void ST7565R_Delay_ms(uint16_t cnt)
{
	uint16_t i;
	while (cnt--)
		for(i=0; i<20000; i++);
}

void ST7565R_NOP(void)
{
	uint8_t cnt=30/*20*/;
	while(cnt--)
		__NOP(); // __NOP()约5~6ns
}
/**************************************************************************/
/* SPI接口初始化	ST7565R :SCLK\SI\CS\A0
***************************************************************************/
void ST7565R_IO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

	//GPIOF: CLK--GPIO_Pin_6, A0--GPIO_Pin_7, SID--GPIO_Pin_8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	//GPIOG: CS--GPIO_Pin_15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	//GPIOB: BL--GPIO_Pin_2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/**************************************************************************/
/* 初始化汉显模块
***************************************************************************/
extern void SysDelay_ms(uint16_t delay_ms);
void mInitialLCD(void)
{

	ApplVar.ScreenStat = 0;

	ST7565R_IO_Init();

	ST7565R_WR_CMD(ST7565R_CMD_RESET);
	ST7565R_NOP();//ST7565R_Delay_ms(1);

	ST7565R_WR_CMD(ST7565R_CMD_ADC_NORMAL);
	ST7565R_WR_CMD(ST7565R_CMD_NORMAL_SCAN_DIRECTION);

	ST7565R_WR_CMD(ST7565R_CMD_LCD_BIAS_9);

	ST7565R_WR_CMD(ST7565R_CMD_POWER_CTRL_ALL_ON);
	//ST7565R_WR_CMD(ST7565R_CMD_POWER_CTRL_ALL_ON+1); //??

	ST7565R_WR_CMD(ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_4);

	ST7565R_WR_CMD(ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET);
	ST7565R_WR_CMD(ST7565R_CMD_ELECTRONIC_VOLUME(0x0C/*0x24*/));

	ST7565R_WR_CMD(ST7565R_CMD_BOOSTER_RATIO_SET);
	ST7565R_WR_CMD(ST7565R_CMD_BOOSTER_RATIO_5X);

	ST7565R_WR_CMD(ST7565R_CMD_DISPLAY_ON);

	mSetContrast(6);
	mClearScreen();
  mLightLCDOn();
	mDispLogo();
}

/**************************************************************************/
/* GPIO模拟SPI通讯发送位数据
***************************************************************************/

void ST7565R_SendData(uint8_t dat)
{
	uint8_t i;

	ST7565R_CLK_LOW();
	for (i=0; i<8; i++)
	{
		if ((dat<<i) & 0x80) //高低在先
			ST7565R_SID_HIGH();
		else
			ST7565R_SID_LOW();
		ST7565R_NOP();
		ST7565R_CLK_HIGH();
		ST7565R_NOP();
		ST7565R_CLK_LOW();
	}
	//ST7565R_NOP();
	//ST7565R_CLK_HIGH();
}


/**************************************************************************/
/* 写命令
***************************************************************************/
void ST7565R_WR_CMD(uint8_t cmd)
{

	ST7565R_CS_LOW();
	ST7565R_A0_CMD();
	ST7565R_SendData(cmd);
	ST7565R_CS_HIGH();


}

/**************************************************************************/
/* 写数据
***************************************************************************/
void ST7565R_WR_DATA(uint8_t dat)
{

	ST7565R_CS_LOW();
	ST7565R_A0_DATA();
	ST7565R_SendData(dat);
	ST7565R_CS_HIGH();
}

/*************************************************************************
 设置显示对比度
***************************************************************************/
void mSetContrast(uint8_t contrast)
{//设置显示对比度

	    // 0x00-0x3f对比度由浅到深 //
		contrast *= 4;// (ApplVar.AP.Flag[CONTRAST] & 0x0f) * 5;
		ST7565R_WR_CMD(ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET);
		ST7565R_WR_CMD(ST7565R_CMD_ELECTRONIC_VOLUME(contrast));
}

/**************************************************************************/
/* 打开显示背光
***************************************************************************/
void mLightLCDOn(void)
{
	ST7565R_BL_ON();
	ApplVar.ScreenStat |= LIGHTON;
}

/**************************************************************************/
/* 关闭显示背光
***************************************************************************/
void mLightLCDOff(void)
{
	ST7565R_BL_OFF();
	ApplVar.ScreenStat &= ~LIGHTON;
}

/**************************************************************************/
/* 允许显示光标
***************************************************************************/
void mEnCursor(uint8_t NewState )
{

	if(NewState)
	{//打开光标
        SETBIT(ApplVar.ScreenStat,CURSOREN);
	}
	else
	{
        if (TESTBIT(ApplVar.ScreenStat,CURSOREN |CURSORON)==(CURSOREN |CURSORON))
            mDispCursor();//关闭光标
        RESETBIT(ApplVar.ScreenStat,(CURSOREN | CURSORON));
	}
}

/**************************************************************************/
/* 显示光标
***************************************************************************/
void mDispCursor(void)
{
	uint8_t valh,vall;
	int x,y;

	if(TESTBIT(ApplVar.ScreenStat,CURSOREN))
	{
		SETBIT(ApplVar.ScreenStat,CRITICAL);
		x = ApplVar.sCursor.x;
		y = ApplVar.sCursor.y;
#ifndef CHAR_6X8
    y *= 2;
		x *= X_Width;
#else
		x *= X_Width;
		if(SCREENWD>x>0)
			x -= 1;
#endif

		if (ApplVar.sCursor.x>=SCREENWD)
			x--;//在屏幕的右边界外显示光标

		INVERTBIT(ApplVar.ScreenStat,CURSORON);
		if(TESTBIT(ApplVar.ScreenStat,CURSORON))
		{
			valh = vall = 0xff;
		}
		else /*ccr2013 if (ApplVar.sCursor.x<SCREENWD)*/ //在屏幕界内处显示光标
		{
			valh = DotBuffer[y][x];
#ifndef CHAR_6X8
			vall = DotBuffer[y+1][x];
#endif
		}
/*ccr2013		else //在屏幕的右边界外显示光标
		{
			valh = 0;
			vall = 0;
		}
*/

		st7565SetXY(x, pagemap[y]);
        if(x==0)
            ST7565R_WR_DATA(valh);	//???? 去掉后会导致发送的第1列8bit点阵没有显示,	发送的内容可为任意字节
		ST7565R_WR_DATA(valh);
#ifndef CHAR_6X8
		st7565SetXY(x, pagemap[y+1]);
        if(x==0)
            ST7565R_WR_DATA(vall);	//???? 去掉后会导致发送的第1列8bit点阵没有显示,	发送的内容可为任意字节
		ST7565R_WR_DATA(vall);
#endif
		RESETBIT(ApplVar.ScreenStat,CRITICAL);
	}
}

/**************************************************************************/
/* 移动光标	在第y(0~3)行,第x(0~15)列位置显示光标，光标靠右对齐
***************************************************************************/
void mMoveCursor(uint8_t x, uint8_t y)
{
	uint8_t i;
    uint8_t cur=TESTBIT(ApplVar.ScreenStat,CURSOREN);

    if (cur)
        mEnCursor(DISABLE);//关闭光标

	for(i = 0; i < x; )
	{
		if(ApplVar.ScreenMap[y][i]<0xA1)  //汉字或ASCII
			i++;
		else
			i += 2;
	}
	ApplVar.sCursor.x = i;
	ApplVar.sCursor.y = y;
    if (cur)
        mEnCursor(ENABLE);
}
/**************************************************************************/
/* 清除反显属性
***************************************************************************/
//void mClearInvAttr(void)
//{
//	RESETBIT(ApplVar.ScreenStat,INVATTR);
//}

/**************************************************************************/
/* 设置反显属性,此后的所有显示将以反显显示
***************************************************************************/
//void mSetInvAttr(void)
//{
//	SETBIT(ApplVar.ScreenStat,INVATTR);
//}

/**************************************************************************/
/* 打开或关闭状态行
***************************************************************************/
void mSetState(uint8_t NewState)
{
	if(NewState)
	{
		SETBIT(ApplVar.ScreenStat,STATEON);
		ActiveLines = (SCREENLN-STATELN);
	}
	else
	{
		RESETBIT(ApplVar.ScreenStat,STATEON);
		ActiveLines = SCREENLN;
	}
}
/**************************************************************************/
/* 清除屏幕内容
***************************************************************************/
void mClearScreen(void)
{
	uint8_t p, pn;
	uint16_t c;

    SETBIT(ApplVar.ScreenStat,CRITICAL);

	if (TESTBIT(ApplVar.ScreenStat,STATEON))
	{//状态行打开时，只清除内容显示行，不清除状态行
#ifndef CHAR_6X8
		pn = 6;
#else
		pn = 7;
#endif
		memset(ApplVar.ScreenMap,' ',(SCREENLN-1)*SCREENWD);
	}
	else
	{
		pn = 8;
		memset(ApplVar.ScreenMap,' ',sizeof(ApplVar.ScreenMap));
	}

	for(p=0; p<pn; p++)
	{
		st7565SetXY(0, pagemap[p]);
        ST7565R_WR_DATA(0x0);
		for(c = 0; c < 129; c++)
		{
			ST7565R_WR_DATA(0x0);
		}
	}
	memset(DotBuffer,0,pn*128);//清除buffer点阵
	GotoXY(0,0);
    RESETBIT(ApplVar.ScreenStat,CRITICAL);

}

/**************************************************************************/
/* 清空屏幕
***************************************************************************/
void mBalnkScreen(void)
{
	uint8_t p, pn;
	uint16_t c;

    SETBIT(ApplVar.ScreenStat,CRITICAL);

	for(p=0; p<8; p++)
	{
		st7565SetXY(0, pagemap[p]);
        ST7565R_WR_DATA(0x0);
		for(c = 0; c < 129; c++)
		{
			ST7565R_WR_DATA(0x0);
		}
	}
    RESETBIT(ApplVar.ScreenStat,CRITICAL);

}

/**************************************************************************/
/* 设置显示位置 x=(0..127), y=(0..63)
***************************************************************************/
void st7565SetXY(uint8_t pos_x,uint8_t pos_y)
{
    if (pos_x) pos_x++;//ccr2016-05-11解决第2个字符向左便宜问题????????
	ST7565R_WR_CMD(ST7565R_CMD_PAGE_ADDRESS_SET(pos_y));				// 页地址
	ST7565R_WR_CMD(ST7565R_CMD_COLUMN_ADDRESS_SET_MSB(pos_x >> 4));  	// 列地址高4位
	ST7565R_WR_CMD(ST7565R_CMD_COLUMN_ADDRESS_SET_LSB(pos_x & 0x0f));	// 列地址低4位
}

/**************************************************************************/
/* 把DotBuffer中指定行LinNum(0~3),列ColNum位置的字符点阵重新显示在屏幕上,
适用：仅更新指定位置一个字符时使用
hzFlag:=0,显示ascii字符;=1,显示汉字字符
***************************************************************************/
void mDrawChar(uint8_t ColNum, uint8_t LinNum,uint8_t hzFlag)
{
	uint8_t j;
	uint8_t *pDot;
	uint8_t num;

	num = ColNum;//ouhs20160227
	ColNum = ColNum*X_Width;

#ifndef CHAR_6X8
	LinNum *= 2;
#endif

#if defined(CASE_FORHANZI)
	if(hzFlag)	 // 汉字模式显示为16位宽度
		hzFlag = 16;
	else
#endif
		hzFlag = X_Width;

	//显示上半部分
	pDot = &DotBuffer[LinNum][ColNum];
    SETBIT(ApplVar.ScreenStat,CRITICAL);

#ifdef CHAR_6X8
	if(TESTBIT(ApplVar.ScreenStat,DOUBLEWIDTH))//字符倍宽，字符间距不变 ouhs20160227
	{
		if(ColNum==0)	//状态行左端空5个点，向右对齐 ouhs20160229
		{
			st7565SetXY(0, pagemap[LinNum]);
			ST7565R_WR_DATA(0x80);ST7565R_WR_DATA(0x9c);ST7565R_WR_DATA(0x9c);
			ST7565R_WR_DATA(0x9c);ST7565R_WR_DATA(0x9c);ST7565R_WR_DATA(0x80);
		}
		st7565SetXY(ColNum*2-num+5, pagemap[LinNum]);
//		else
//			st7565SetXY(ColNum*2-num, pagemap[LinNum]);
	}
	else
#endif
		st7565SetXY(ColNum, pagemap[LinNum]);

	if(ColNum==0)
		ST7565R_WR_DATA(0x80);	//???? 去掉后会导致发送的第1列8bit点阵没有显示,	发送的内容可为任意字节

	for(j = 0; j < hzFlag; j++)
	{
#ifdef CHAR_6X8
		if(TESTBIT(ApplVar.ScreenStat,DOUBLEWIDTH) && (j<(hzFlag-1)))//ouhs20160227
			ST7565R_WR_DATA(*pDot);
#endif
		ST7565R_WR_DATA(*pDot++);
	}



#ifndef CHAR_6X8
	//显示下半部分
	LinNum++;
	pDot = &DotBuffer[LinNum][ColNum];
	st7565SetXY(ColNum, pagemap[LinNum]);
	if(ColNum==0)
		ST7565R_WR_DATA(0x80);	//???? 去掉后会导致发送的第1列8bit点阵没有显示,	发送的内容可为任意字节
	for(j = 0; j < hzFlag; j++)
		ST7565R_WR_DATA(*pDot++);

#endif
    RESETBIT(ApplVar.ScreenStat,CRITICAL);

}

/**************************************************************************/
/* 把DotBuffer中指定行LinNum(0~3)的字符点阵重新显示在屏幕上,
   即把屏幕上的第LinNum行重新显示一遍,适用：需要更新指定行多个字符时使用
***************************************************************************/
void mDraw1Line(uint8_t LinNum)
{
	uint8_t i, j;

    SETBIT(ApplVar.ScreenStat,CRITICAL);
#ifndef CHAR_6X8
	for(i = 0; i < 2; i++)
	{
		st7565SetXY(0, pagemap[LinNum*2+i]);
		ST7565R_WR_DATA(0xff);	//???? 去掉后会导致发送的第1列8bit点阵没有显示,	发送的内容可为任意字节
		for(j = 0; j < 128; j++)
		{
			ST7565R_WR_DATA(DotBuffer[LinNum*2+i][j]);
		}
	}
#else
		st7565SetXY(0, pagemap[LinNum]);
		ST7565R_WR_DATA(0xff);	//???? 去掉后会导致发送的第1列8bit点阵没有显示,	发送的内容可为任意字节
		for(j = 0; j < 128; j++)
		{
			ST7565R_WR_DATA(DotBuffer[LinNum][j]);
		}
#endif
    RESETBIT(ApplVar.ScreenStat,CRITICAL);

}

/**************************************************************************/
/* 发送点阵数据
***************************************************************************/
void st7565Refresh(void)
{
	uint8_t c, p;

    SETBIT(ApplVar.ScreenStat,CRITICAL);
	for(p = 0; p < 8; p++)
	{
		st7565SetXY(0, pagemap[p]);
		ST7565R_WR_DATA(0xff);	//???? 去掉后会导致发送的第1列8bit点阵没有显示,	发送的内容可为任意字节
		for(c = 0; c < 128; c++)
		{
			ST7565R_WR_DATA(DotBuffer[p][c]);
		}
	}
    RESETBIT(ApplVar.ScreenStat,CRITICAL);
}

/**************************************************************************/
/* 画点 x=(0..127), y=(0..63)
***************************************************************************/
void st7565DrawPixel(uint8_t x, uint8_t y)
{
	if ((x >= 128) || (y >= 64))
		return;

	DotBuffer[y/8][x] |= (1 << (7-(y%8)));
}

/**************************************************************************/
/* 清点 x=(0..127), y=(0..63)
***************************************************************************/
void st7565ClearPixel(uint8_t x, uint8_t y)
{
	if ((x >= 128) || (y >= 64))
		return;

	DotBuffer[y/8][x] &= ~(1 << (7-(y%8)));
}

/**************************************************************************/
/* 指定位置显示一个字符ch
   在LCD屏幕第y(0~3)行,第x(0~15)列位置显示字符ch的点阵
   ch可以是ASCII或者汉字
***************************************************************************/
void mDispCharXY(WORD ch, BYTE x, BYTE y)
{
	uint8_t col, ch_l, x_c, y_p,hzFlag;
	uint32_t addr;
	uint8_t *fontH,*fontL;

	if(y==SCREENLN)
	{
		if (TESTBIT(ApplVar.ScreenStat,STATEON))
			y--;
		else
			return;
	}

	x_c = x*X_Width;	// column address
#ifndef CHAR_6X8
	y_p = y*2;		 	  // page
#else
	y_p = y;
#endif


#if defined(CASE_FORHANZI)
	if ((ch <= 128))
#else
    ch &= 0xff;    //ccr2016-08-31
#endif
	{/* ASCII点阵 */
		hzFlag = 0;
        if (ch < 0x20) ch =0x20;
#ifndef CHAR_6X8
		addr = (ch - 0x20) * 16;
		fontH = &DotBuffer[y_p][x_c];
		fontL = &DotBuffer[y_p+1][x_c];
		memcpy(fontH,ASCIITAB_8x16+addr,8); 	 // 上半部分
		memcpy(fontL,ASCIITAB_8x16+addr+8,8); 	 // 下半部分
#else
		addr = (ch - 0x20) * 5;
		fontH = &DotBuffer[y_p][x_c];
		memcpy(fontH,ASCIITAB_5x8+addr,5);
#endif

		if(TESTBIT(ApplVar.ScreenStat,INVATTR))
		{
			for (col = 0; col < X_Width; col++)
			{
				*fontH++ ^= 0xff; 	 // 上半部分
#ifndef CHAR_6X8
				*fontL++ ^= 0xff;	 //下半部分
#endif
			}

			fontH -= X_Width;
		}
#ifdef CHAR_6X8
		if(TESTBIT(ApplVar.ScreenStat,INVATTR))
			*(fontH + 5) = 0xff;
		else
			*(fontH + 5) = 0x00;
#endif

		if((y>=(SCREENLN-1)) && (TESTBIT(ApplVar.ScreenStat,STATEON)))		   // 状态行显示分隔线
		{
			for (col = 0; col < X_Width; col++)
			{
				*fontH++ |= 0x80; 	// 上半部分
			}
		}

	}
#if defined(CASE_FORHANZI)
	else
	{/* 汉字处理 CCLIB16  */
		hzFlag = 1;
		ch -= 0xA1A1;
		ch_l = ch & 0xff;
		if (ch_l >= 15)
			ch_l -= 1;

		//字库芯片是按字进行编址,因此必须32/2,64为ASCII个数(两个ASCII=一个汉字):128/2
		addr = (uint32_t)(ch_l*94 + (ch>>8)+64)*32 + FLASH_HZK16_ADDR;		// 字偏移地址(基地址+偏移地址,16bit)

		fontH = &DotBuffer[y_p][x_c];
		fontL = &DotBuffer[y_p+1][x_c];


		//汉字左半部分
		wFlashReadBuffer((WORD*)fontH, addr, 4); 	//左上部分, 读取8个字节
		wFlashReadBuffer((WORD*)fontL, addr+8, 4); 	//左下部分, 读取8个字节
		fontH+=8;fontL+=8;addr+=16;
		//汉字右半部分
		wFlashReadBuffer((WORD*)fontH, addr, 4); 	//右上部分, 读取8个字节
		wFlashReadBuffer((WORD*)fontL, addr+8, 4); 	//右下部分, 读取8个字节


		fontH -= 8;fontL -= 8;
		if(TESTBIT(ApplVar.ScreenStat,INVATTR))
		{
			for (col = 0; col < 16; col++)
			{
				*fontH++ ^= 0xff; 	 // 上半部分
				*fontL++ ^= 0xff;	 //下半部分
			}
			fontH -= 16;
		}
		if((y>=(SCREENLN-1)) && (TESTBIT(ApplVar.ScreenStat,STATEON)))		   // 状态行显示分隔线
		{
			for (col = 0; col < 16; col++)
			{
				*fontH++ |= 0x80; 	// 上半部分
			}
		}
	}
#endif

	mDrawChar(x, y,hzFlag);
}

/**************************************************************************/
/* 指定位置显示字符串
***************************************************************************/
void mDispStringXY(BYTE* str, BYTE x, BYTE y)
{
	uint8_t i;
	uint8_t *pstr;

	pstr = str;

	for (i = 0; i < SCREENWD && *pstr; )
	{
#if defined(CASE_FORHANZI)
		if(*pstr>=0xA1)
		{//HZK
			mDispCharXY(CWORD(pstr[0]), (x + i) , y);
			pstr+=2;
			i += 2;
		}
		else
#endif
		{//ASCII
			mDispCharXY(*pstr, (x + i) , y);
			pstr++;
			i++;
		}
	}
}

/**************************************************************************
将ApplVar.ScreenMap中的指定行重新显示在屏幕上
***************************************************************************/
void  mRefreshLine(BYTE y)
{
	if(y < (SCREENLN - (ApplVar.ScreenStat & STATEON)))
		mDispStringXY((uint8_t *)ApplVar.ScreenMap[y], 0, y);
	else if	((ApplVar.ScreenStat & STATEON) && (y==SCREENLN))
	{
		mDispStringXY((uint8_t *)ApplVar.ScreenMap[SCREENLN], 0, SCREENLN);
	}
}



/**************************************************************************
 在屏幕指定的ASCII字符位置右上角闪烁显示GPRS标示
***************************************************************************/
void mDrawGPRS(uint8_t ColNum, uint8_t LinNum,uint8_t force)
{
	uint8_t j;
	uint8_t *pDot;
    static uint8_t gFlash=0x40;

    if ((force || (GetSystemTimer() & 0x100) == 0) && !TESTBIT(ApplVar.ScreenStat,CRITICAL))
    {
		if (!(gFlash & 1))
		{
			ColNum = ColNum*X_Width;
			LinNum *= 2;

			//显示上半部分
			pDot = &DotBuffer[LinNum][ColNum]+5;
			SETBIT(ApplVar.ScreenStat,CRITICAL);

			st7565SetXY(ColNum+5, pagemap[LinNum]);
			for(j = 0; j < 8-5; j++)
			{
				gFlash ^= 0xc0;
				ST7565R_WR_DATA((*pDot | gFlash));
				pDot++;
			}
            RESETBIT(ApplVar.ScreenStat,CRITICAL);
			if (!force)
                gFlash |= 1;
		}
    }
	else
		gFlash &=0xc0;

}

/**
 * 在屏幕指定的ASCII字符位置显示电池容量
 *
 * @author EutronSoftware (2014-12-31)
 *
 * @param ctrl :0-不显示;=1测试并显示;=2显示
 * @param ColNum
 * @param LinNum
 */
extern void DisplayDecXY(WORD pDec,int x,int y,int pWdith);
#define VIN_FULL  VIN_8V7 //8.7V---3075mV---0xEE8   +修正值0x6B
#define VIN_NONE  VIN_7V0 //7.0V---1375mV---0x6AA
void mDrawPower(BYTE ctrl, uint8_t ColNum,uint8_t LinNum)
{
#if POWERCTRL
    static WORD gPower=0;

    uint8_t sByte;
	WORD pDot,j;

    //if (BatteryExist())//&& !DC_DET_GET())//只有在电池供电时,才显示电池容量
    {
        if (ctrl==0)
        {
            gPower = 0;
            return;
        }
        else if (ctrl==1)
        {
            if (gPower==0)
                return;
        }
        gPower = Vin_Vrtc_Val[0];
        //DisplayDecXY(gPower,ColNum+1,4,6);//testonly

        if (gPower>=VIN_FULL)
            j=Y_Height;
        else if (gPower<=VIN_NONE)
            j = 0;
        else
            j = ((gPower-VIN_NONE)*Y_Height)/(VIN_FULL-VIN_NONE);

        pDot = (1<<j)-1;

        ColNum = ColNum*X_Width;
        LinNum *= 2;

        //显示上半部分

        SETBIT(ApplVar.ScreenStat,CRITICAL);

        sByte = (pDot>>8) | 0x80;
        st7565SetXY(ColNum, pagemap[LinNum]);
        ST7565R_WR_DATA(0xff);
        ST7565R_WR_DATA(sByte | 0x40);
        for(j = 0; j < 4; j++)
            ST7565R_WR_DATA(sByte);
        ST7565R_WR_DATA(sByte | 0x40);
        ST7565R_WR_DATA(0xff);

        sByte = (pDot & 0xff);
        st7565SetXY(ColNum, pagemap[LinNum+1]);
        ST7565R_WR_DATA(0xff);
        for(j = 0; j < 6; j++)
            ST7565R_WR_DATA(sByte);
        ST7565R_WR_DATA(0xff);
    }
#endif
}

/*****************************************************************
 * 在屏幕指定的ASCII字符位置显示GPRS信号强度
 *
 * @author EutronSoftware (2014-12-31)
 *
 * @param Signal:2-30,信号强度
 * @param ColNum
 * @param LinNum
 ****************************************************************/
void mDrawGPRSSignal(BYTE Signal,uint8_t ColNum,uint8_t LinNum)
{
#if defined(CASE_GPRS)

    static BYTE gSignal=0xff;

    uint8_t sByte;
	WORD pDot,j,pH;

    //if (BatteryExist())//&& !DC_DET_GET())//只有在电池供电时,才显示电池容量
    if (gSignal!=Signal)
    {
        gSignal=Signal;

        Signal &= 0x1f;
        j= Signal >> (3-Y_Height/8);//高度

        pDot = (1<<j)-1;//高度对应的点阵值

        SETBIT(ApplVar.ScreenStat,CRITICAL);

        ColNum = ColNum*X_Width;
#ifndef CHAR_6X8
        LinNum *= 2;

        //显示上半部分

        st7565SetXY(ColNum, pagemap[LinNum]);
        pH=1;
        for(j = 0; j < 8; j++)
        {
            ST7565R_WR_DATA((pH>>8)|0x80);
            if (pH)
            {
               pH=(pH<<2) | 3;
               if (pH>pDot)
                   pH=0;
            }
        }


        st7565SetXY(ColNum, pagemap[LinNum+1]);
        pH=1;
        for(j = 0; j < 8; j++)
        {
            ST7565R_WR_DATA(pH);
            if (pH)
            {
               pH=(pH<<2) | 3;
               if (pH>pDot)
                   pH=0;
            }
        }
#else
        st7565SetXY(ColNum, pagemap[LinNum]);
        pH=1;
        for(j = 0; j < 8; j++)
        {
            ST7565R_WR_DATA(pH | 0x80);
            if (pH)
            {
               pH=(pH<<1) | 1;
               if (pH>pDot)
                   pH=0;
            }
        }
#endif
        RESETBIT(ApplVar.ScreenStat,CRITICAL);
    }
#endif
}


/**
 * 在字符行LinrFrom和LineTo之间画一个方框
 *
 * @author EutronSoftware (2016-02-24)
 *
 * @param LinFrom
 * @param LinTo
 */


#ifdef CHAR_6X8
void mDrawABox(BYTE LinFrom,BYTE LinTo)
{
    static BYTE LinNum;
    static BYTE maxLine;
    BYTE j;
    BYTE lin1LR,lin1C,lin1XOR;


    SETBIT(ApplVar.ScreenStat,CRITICAL);
    if (LinFrom==LinTo)
    {//回复被覆盖的行
        for (j=LinNum;j<=maxLine;j++)
            mRefreshLine(j);
        LinNum=1;maxLine=0;
    }
    else
    {
        maxLine=LinTo;
        if (maxLine>(SCREENLN-TESTBIT(ApplVar.ScreenStat,STATEON)-1))
            maxLine=(SCREENLN-TESTBIT(ApplVar.ScreenStat,STATEON)-1);

        for (LinNum=LinFrom;LinNum<=maxLine;LinNum++)
        {
            if (LinNum==LinFrom)
            {//画顶行线
                lin1LR=0x0f,lin1C=0x08+0x02;lin1XOR=0x07;
            }
            else if (LinNum==LinTo)
            {//画底行线
                lin1LR=0xf0,lin1C=0x10+0xa0;lin1XOR=0xe0;

            }
            else
            {//画中间线
                lin1LR=0xff,lin1C=0x00+0xaa;lin1XOR=0xff;
            }
            st7565SetXY(0, pagemap[LinNum]);
            ST7565R_WR_DATA(0xff);  //???? 去掉后会导致发送的第1列8bit点阵没有显示, 发送的内容可为任意字节
            ST7565R_WR_DATA(lin1LR);
            for(j = 1; j < SCREENWD*6-1; j++)
            {
                 if (lin1XOR==0xff && j>5 && j<(SCREENWD-1)*6)
                     ST7565R_WR_DATA(0x00);
                 else
                 {
                     ST7565R_WR_DATA(lin1C);
                     lin1C ^= lin1XOR;
                 }
            }
            ST7565R_WR_DATA(lin1LR);
            ST7565R_WR_DATA(0);ST7565R_WR_DATA(0);
        }
        LinNum=LinFrom;
    }
    RESETBIT(ApplVar.ScreenStat,CRITICAL);
}
#else//font size=8*8
void mDrawABox(BYTE LinFrom,BYTE LinTo)
{
    static BYTE LinNum;
    static BYTE maxLine;
    BYTE j;
    BYTE lin1LR,lin1C,lin1XOR;
    BYTE lin2LR,lin2C,lin2XOR;


    SETBIT(ApplVar.ScreenStat,CRITICAL);
    if (LinFrom==LinTo)
    {//回复被覆盖的行
        for (j=LinNum;j<=maxLine;j++)
            mRefreshLine(j);
        LinNum=1;maxLine=0;
    }
    else
    {
        maxLine=LinTo;
        if (maxLine>(SCREENLN-TESTBIT(ApplVar.ScreenStat,STATEON)-1))
            maxLine=(SCREENLN-TESTBIT(ApplVar.ScreenStat,STATEON)-1);

        for (LinNum=LinFrom;LinNum<=maxLine;LinNum++)
        {
            if (LinNum==LinFrom)
            {//画顶行线
                lin1LR=0x0f,lin1C=0x08+0x02;lin1XOR=0x07;
                lin2LR=0xff,lin2C=0x00+0xaa;lin2XOR=0xff;

            }
            else if (LinNum==LinTo)
            {//画底行线
                lin1LR=0xff,lin1C=0x00+0xaa;lin1XOR=0xff;
                lin2LR=0xf0,lin2C=0x10+0xa0;lin2XOR=0xe0;

            }
            else
            {//画中间线
                lin1LR=0xff,lin1C=0x00+0xaa;lin1XOR=0xff;
                lin2LR=0xff,lin2C=0x00+0xaa;lin2XOR=0xff;

            }
            st7565SetXY(0, pagemap[LinNum*2]);
            ST7565R_WR_DATA(0xff);  //???? 去掉后会导致发送的第1列8bit点阵没有显示, 发送的内容可为任意字节

            ST7565R_WR_DATA(lin1LR);
            for(j = 1; j < SCREENWD*8-1; j++)
            {
                if (lin1XOR==lin2LR && j>6 && j<(SCREENWD*8-7))
                    ST7565R_WR_DATA(0x00);
                else
                {
                    ST7565R_WR_DATA(lin1C);
                    lin1C ^= lin1XOR;
                }
            }
            ST7565R_WR_DATA(lin1LR);

            ST7565R_WR_DATA(0);ST7565R_WR_DATA(0);

            st7565SetXY(0, pagemap[LinNum*2+1]);
            ST7565R_WR_DATA(0xff);  //???? 去掉后会导致发送的第1列8bit点阵没有显示, 发送的内容可为任意字节

            ST7565R_WR_DATA(lin2LR);
            for(j = 1; j < SCREENWD*8-1; j++)
            {
                if (lin1XOR==lin2LR && j>6 && j<(SCREENWD*8-7))
                    ST7565R_WR_DATA(0x00);
                else
                {
                    ST7565R_WR_DATA(lin2C);
                    lin2C ^= lin2XOR;
                }
            }
            ST7565R_WR_DATA(lin2LR);

            ST7565R_WR_DATA(0);ST7565R_WR_DATA(0);
        }
        LinNum=LinFrom;
    }
    RESETBIT(ApplVar.ScreenStat,CRITICAL);
}
#endif


/**************************************************************************/
/* 显示一幅图片
***************************************************************************/
const uint8_t EUTRON_LOGO_128064[8][128]= {
{0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x1F,0x1F,0x3F,0x3E,0x7D,0x7B,0x77,0x6F,0x5F,0x3F,0x7E,0x7D,0x7B,0x77,0x6F,0x1F,0x3F,0x3F,0x1E,0x0D,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x1F,0x7E,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7E,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7E,0xFD,0xFB,0xF7,0xEF,0xDF,0x3F,0x1F,0x00,0x00,0x00,0x30,0x30,0x3F,0x3F,0x3F,0x3F,0x30,0x30,0x37,0x38,0x3C,0x3E,0x00,0x00,0x00,0x30,0x3F,0x3F,0x3F,0x3F,0x30,0x30,0x00,0x00,0x30,0x30,0x3F,0x3F,0x30,
0x00,0x00,0x3E,0x38,0x30,0x30,0x3F,0x3F,0x3F,0x3F,0x30,0x30,0x38,0x38,0x3E,0x00,0x30,0x30,0x3F,0x3F,0x3F,0x3F,0x30,0x30,0x38,0x3F,0x1F,0x0F,0x07,0x00,0x00,0x00,0x01,0x07,0x0F,0x1F,0x1F,0x38,0x30,0x30,0x30,0x30,0x1C,0x1F,0x0F,0x07,0x03,0x00,0x00,0x30,0x30,0x3F,0x3E,0x3F,0x1F,0x0F,0x03,0x01,0x00,0x30,0x3F,0x3F,0x30,0x00},
{0x00,0x7C,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7E,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7E,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x61,0xF1,0xFD,0x01,0x03,0x07,0x0F,0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0x07,0x01,0x01,0x01,0x03,0x07,0xFE,0xF8,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x60,0x78,0x7C,0xFF,0xFF,0xCF,0x87,0x01,0x00,0x00,0xF0,0xFC,0xFE,0xFF,0xFF,0x03,0x01,0x00,0x00,0x01,0x07,0xFF,0xFF,0xFE,0xF8,0x00,0x00,0x00,0x01,0xFF,0x01,0x00,0xC0,0xE0,0xF0,0xF8,0xFC,0x7E,0xFF,0xFF,0x00,0x00},
{0x00,0x00,0x80,0xC0,0xE0,0xE0,0xD8,0xBC,0x7E,0xFC,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC7,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xE7,0xFF,0xFF,0xFE,0x7C,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x00,0x00,0x00,0x18,0x38,0x38,0x78,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x0F,0x7F,0xFF,0xFF,0xF0,0xC0,0xC0,0xC0,0xE0,0xFF,0xFF,0x7F,0x0F,0x00,0x00,0x00,0x0F,0x7F,0xFF,0xFF,0xF0,0xC0,0xC0,0xC0,0xE0,0xFF,0xFF,0x7F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x87,0x07,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xFE,0xFF,0xFF,0x7F,0x01,0x00,0x00,0x00,0x00,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xF0,0xFE,0xFF,0xFF,0x0F,0x03,0x03,0x03,0x0F,0xFF,0xFF,0xFE,0xF0,0x00,0x00,0x00,0xF0,0xFE,0xFF,0xFF,0x0F,0x03,0x03,0x03,0x0F,0xFF,0xFF,0xFE,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

void mDispLogo(void)
{
#ifdef  CASE_ER100
	uint8_t p, pn;
	uint16_t c;

	pn = 8;

	for(p=0; p<pn; p++)
	{
		st7565SetXY(0, pagemap[p]);
		for(c = 0; c < 129; c++)
		{
			ST7565R_WR_DATA(EUTRON_LOGO_128064[p][c]);
		}
	}
#endif
}



