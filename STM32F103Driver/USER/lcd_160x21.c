/****************************************Copyright (c)**************************************************
**                               Hunan EUTRON information Equipment Co.,LTD.
**
**                                 http://www.eutron.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:       LCD_Drv.c
** Descriptions:    S6A0069驱动
**
**------------------------------------------------------------------------------------------------------
** Created by:      Ruby
** Created date:    2011-12-05
** Version:         1.0
** Descriptions:    The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
********************************************************************************************************/
#include "stm32f10x.h"
#include "lcd_160x21.h"
#include "fsmc_sram.h"
#include "SysTick.h"

#include <string.h>  //ouhs 20140815

#define LCD_D0	GPIO_Pin_8
#define LCD_D1	GPIO_Pin_9
#define LCD_D2	GPIO_Pin_10
#define LCD_D3	GPIO_Pin_11
#define LCD_DataPort	GPIOF

#define LCDC_E	GPIO_Pin_14//GPIO_Pin_14  //Customer
#define LCDO_E	GPIO_Pin_15//GPIO_Pin_15  //Operator
#define LCD_E_Port		GPIOG

#define LCD_RS	GPIO_Pin_7
#define LCD_RW	GPIO_Pin_6
#define LCD_BL	GPIO_Pin_2

#define	RS_Low()	GPIOF->BRR =	LCD_RS
#define	RS_High()	GPIOF->BSRR =	LCD_RS

#define	RW_Low()	GPIOF->BRR =	LCD_RW
#define	RW_High()	GPIOF->BSRR =	LCD_RW

#define	C_E_Low()		LCD_E_Port->BRR =	LCDC_E
#define	C_E_High()	LCD_E_Port->BSRR =	LCDC_E
#define	O_E_Low()		LCD_E_Port->BRR =	LCDO_E
#define	O_E_High()	LCD_E_Port->BSRR =	LCDO_E

#define	LINE_NUM	2

#if CASE_ER260
const char  Disp[LINE_NUM][DISLEN] = {"Fiscal ECR ER260", "   Good For You!"};

#elif CASE_ER380
const char  Disp[LINE_NUM][DISLEN] = {"Fiscal ECR ER380", "   Good For You!"};
#elif CASE_PCR01
const char  Disp[LINE_NUM][DISLEN] = {"Fiscal ECR PCR01", "   Good For You!"};
#elif CASE_GIADA3000
const char  Disp[LINE_NUM][DISLEN] = {" ECR  GIADA3000U", "   Good For You!"};
#elif CASE_ER520U
const char  Disp[LINE_NUM][DISLEN] = {" ECR  ER520U   ", " Good For You!"};
#elif CASE_ER100
const char  Disp[LINE_NUM][DISLEN] = {"Fiscal ECR ER100", " Good For You!"};
#elif CASE_MCR30
const char  Disp[LINE_NUM][DISLEN] = {"Fiscal ECR MCR30", " Good For You!"};
#endif

char SaveDispO[LINE_NUM][DISLEN],SaveDispC[LINE_NUM][DISLEN];//保护显示内容
volatile uint8_t LCD_BL_CTRL;

void LCD_CheckBusy(uint8_t LCDn);
void LCD_ModeInit4Bit(uint8_t LCDn);
void PutsO_Only(const char *str);
void PutsC_Only(const char *str);
void LCD160x_Test(void);

// 延时1ms
void LCD160x_Delay_ms(uint16_t cnt)
{
	uint16_t i;
	while (cnt--)
		for(i=0; i<12000; i++);
}

void LCD160x_Dly80ns(uint8_t cnt)
{
	while(cnt--)
		__NOP(); // __NOP()约80ns
}

// 发送4bit数据
void LCD_Sendbit(uint16_t ch)
{
	ch <<= 8;
	LCD_DataPort->BSRR = ch;
	LCD_DataPort->BRR = (~ch)&0xF00;

}

// 发送数据到LCD
void LCD_SendData(uint8_t LCDn, uint8_t data)
{
	if(LCDn)
		C_E_Low();
	else
		O_E_Low();

	LCD_Sendbit(data);
	//LCD160x_Dly80ns(1);
	if(LCDn)
		C_E_High();
	else
		O_E_High();
	LCD160x_Dly80ns(5);	 //410ns

	if(LCDn)
		C_E_Low();
	else
		O_E_Low();
	LCD160x_Dly80ns(1);
}

// 设置4位数据线模式
void LCD_ModeInit4Bit(uint8_t LCDn)
{
	RW_Low();
	RS_Low();
	LCD_SendData(LCDn, 0x02);		// 命令：0x28  4bit,16x1-line,5x8dots
	LCD_SendData(LCDn, 0x08);
}

// 读取状态标志
void LCD_CheckBusy(uint8_t LCDn)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t ReadData, ReadData_H, ReadData_L;
	uint16_t temp = 0xfff;

  LCD_DataPort->BSRR = LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3; //先置1再读取

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3;
	GPIO_Init(LCD_DataPort, &GPIO_InitStructure);
	RW_High();
	RS_Low();
	do{

		if(LCDn)
			C_E_High();
		else
			O_E_High();
		LCD160x_Dly80ns(5);
		ReadData_H = (LCD_DataPort->IDR)>>4;	// Pin11|Pin10|Pin9|Pin8

		if(LCDn)
			C_E_Low();
		else
			O_E_Low();
		LCD160x_Dly80ns(5);
	   	if(LCDn)
			C_E_High();
		else
			O_E_High();
		LCD160x_Dly80ns(5);
		ReadData_L = (LCD_DataPort->IDR)>>8;	// Pin11|Pin10|Pin9|Pin8

		if(LCDn)
			C_E_Low();
		else
			O_E_Low();
		LCD160x_Dly80ns(5);

		ReadData = ((ReadData_H) | (ReadData_L & 0x0F));
	}while((ReadData & 0x80) && (temp--));

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_DataPort, &GPIO_InitStructure);
}


// 写命令
uint8_t LCD_WRCmd(uint8_t LCDn, uint8_t commmand)
{
	LCD_CheckBusy(LCDn);
	RW_Low();
	RS_Low();
   	LCD_SendData(LCDn, commmand>>4);	   // 先发送高4位
	LCD_SendData(LCDn, commmand&0x0F);	   // 后发送低4位
	return 1;
}

// 写数据
uint8_t LCD_WRData(uint8_t LCDn, uint8_t data)
{
	LCD_CheckBusy(LCDn);
	RW_Low();
	RS_High();
   	LCD_SendData(LCDn, data>>4);	   // 先发送高4位
	LCD_SendData(LCDn, data&0x0F);	   // 后发送低4位
	return 1;
}

//操作显示屏
void LCDO_Pos(uint8_t LCDn, uint8_t LinNum, uint8_t pos)
{
	pos &= 0x1f;
	if(LinNum == 0)
		LCD_WRCmd(LCDn, pos | 0x80);
	else
		LCD_WRCmd(LCDn, (pos&0x0F)|0xC0);

}

//顾客显示屏
void LCDC_Pos(uint8_t LCDn, uint8_t pos)
{
	pos &= 0x0f;
	if(pos<8)
		LCD_WRCmd(LCDn, pos | 0x80);
	else
		LCD_WRCmd(LCDn, (pos&0x07)|0xC0);
}

void LCD160x_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3 | LCD_RS | LCD_RW;
	GPIO_Init(LCD_DataPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_BL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCDC_E | LCDO_E;
	GPIO_Init(LCD_E_Port, &GPIO_InitStructure);
	O_E_Low();
	C_E_Low();
}


void LCDInit(void)
{
	LCD160x_IO_Init();

	// 初始化显示模式设置4次
	LCD_ModeInit4Bit(0);	// 4bit, 16x1-line, 5x8dots
	LCD_ModeInit4Bit(1);
	LCD160x_Delay_ms(3);		// 3ms

	LCD_ModeInit4Bit(0);
	LCD_ModeInit4Bit(1);
	LCD160x_Delay_ms(3);
	/*
	LCD_ModeInit4Bit(0);
	LCD_ModeInit4Bit(1);
	LCD160x_Delay_ms(3);
	LCD_ModeInit4Bit(0);
	LCD_ModeInit4Bit(1);
	LCD160x_Delay_ms(3);
	*/
	LCD_WRCmd(0, 0x08);		// 关显示
	LCD_WRCmd(1, 0x08);
	LCD_WRCmd(0, 0x01);		// 清屏
	LCD_WRCmd(1, 0x01);
	LCD160x_Delay_ms(3);	// 3ms

	LCD_WRCmd(0, 0x06);		// 写入新数据后,光标向右移动,画面不移动
	LCD_WRCmd(1, 0x06);

	LCD_WRCmd(0, 0x0c);		// 显示功能开、有光标、 无闪烁
	LCD_WRCmd(1, 0x0c);

	LCDOpen();			//开背光
    LCD_BL_CTRL &= ~0x02;//LCD_BL_CTRL |= 2;//=1,客显与主显显示相同的内容

	PutsO_Only(Disp[0]);
	Puts1_Only(Disp[1]);

	PutsC0_Only(Disp[0]);

	//LCD160x_Test();
}

#if 0
const char  Disp1[] = "0123456789abcdef";
void LCD160x_Test(void)
{
	uint8_t i;
	while(1)
	{
		for(i=0; i<50; i++)
		{
			PutsO_Only(Disp1);
			PutsO_Only(Disp);
		}
		LCD160x_Delay_ms(1000);

	}}
#endif

void LCDClose(void)
{
	LCD_BL_OFF();

	LCD_WRCmd(0, 0x01);
	LCD_WRCmd(1, 0x01);
}

void LCDOpen(void)
{
	LCD_BL_ON();
}

// 清除屏幕LinNum行显示
void LCDClearLine(uint8_t LCDn, uint8_t LinNum)
{
	uint8_t i;
	LCDO_Pos(LCDn, LinNum, 0);
	for(i=0; i<DISLEN; i++)
	{
		LCD_WRData(LCDn, ' ');
	}
	LCDO_Pos(LCDn, LinNum, 0); //重定位
}

void PutsC(const char *str)
{//在顾客显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;

	memset(SaveDispC[0],' ',DISLEN);

	LCD_WRCmd(1, 0x01);
	//LCD160x_Delay_ms(3);	// 3ms
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		LCDC_Pos(1, addr);
		LCD_WRData(1, *str);
		SaveDispC[0][addr]=*str;
        if( ++addr >= DISLEN)
			break;
	}
}

void PutsC1(const char *str)
{
}

void PutsC0_Only(const char *str)
{//在顾客显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;

	LCD_WRCmd(1, 0x01);
	//LCD160x_Delay_ms(3);
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		LCDC_Pos(1, addr);
		LCD_WRData(1, *str);
        if( ++addr >= DISLEN)
			break;
	}
}

void PutsC1_Only(const char *str)
{
}

//显示被保护的内容
void PutsC_Saved(void)
{
	PutsC0_Only(SaveDispC[0]);
}

//只显示，同时保存内容
void PutsO(const char *str)
{//在操作员显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;

    if (LCD_BL_CTRL & 0x02)
        PutsC(str);

	memset(SaveDispO[0],' ',DISLEN);
	//LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
	LCDClearLine(0, 0);
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		//skip LCD_Pos(0, 0, addr);
		SaveDispO[0][addr]=*str;
		LCD_WRData(0, *str);
		if( ++addr >= DISLEN)
			break;
	}
}

//ccr20131120 在屏幕第一行指定位置显示字符
void PutsO_At(uint8_t ch,uint8_t addr)
{//在操作员显示屏幕上显示内容,16位1行显示

	//LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
    if (addr<DISLEN)
    {
        SaveDispO[0][addr] = ch;
        addr=0;
        LCDClearLine(0, 0);
        for(addr=0;addr<DISLEN;addr++) //需要修改结束字符标识
        {
            //skip LCD_Pos(0, 1, addr);
            LCD_WRData(0, SaveDispO[0][addr]);
        }
    }
}

//只显示，同时保存内容
void Puts1(const char *str)
{//在操作员显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;

    if (LCD_BL_CTRL & 0x02)
        PutsC1(str);

	memset(SaveDispO[1],' ',DISLEN);
	//LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
	LCDClearLine(0, 1);
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		//skip LCD_Pos(0, 1, addr);
		SaveDispO[1][addr]=*str;
		LCD_WRData(0, *str);
		if( ++addr >= DISLEN)
			break;
	}

}
//-----------------------------------------------
//ccr20131120 在第二行靠右显示，同时保存内容
void Puts1_Right(const char *str)
{//在操作员显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;
    int i,l;

    l = strlen(str);
    if (l<DISLEN)
    {
        memset(SaveDispO[1],' ',DISLEN);
        memcpy(SaveDispO[1]+DISLEN-l,str,l);
    }
    else
        memcpy(SaveDispO[1],str,DISLEN);

    if (LCD_BL_CTRL & 0x02)
        PutsC1(SaveDispO[1]);

	//LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
	LCDClearLine(0, 1);
	for(;addr<DISLEN;addr++) //需要修改结束字符标识
	{
		//skip LCD_Pos(0, 1, addr);
		LCD_WRData(0, SaveDispO[1][addr]);
	}

}

//ccr20131120 在屏幕第二行指定位置显示字符
void Puts1_At(uint8_t ch,uint8_t addr)
{//在操作员显示屏幕上显示内容,16位1行显示

	//LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
    if (addr<DISLEN)
    {
        SaveDispO[1][addr] = ch;
        addr=0;
        LCDClearLine(0, 1);
        for(addr=0;addr<DISLEN;addr++) //需要修改结束字符标识
        {
            //skip LCD_Pos(0, 1, addr);
            LCD_WRData(0, SaveDispO[1][addr]);
        }
    }
}


//只显示，不保存内容
void PutsO_Only(const char *str)
{//在操作员显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;

    if (LCD_BL_CTRL & 0x02)
        PutsC0_Only(str);

	//LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
	LCDClearLine(0, 0);
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		//skip LCD_Pos(0, 0, addr);
		LCD_WRData(0, *str);
		if( ++addr >= DISLEN)
			break;
	}
}

//只显示，不保存内容
void Puts1_Only(const char *str)
{//在操作员显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;

    if (LCD_BL_CTRL & 0x02)
        PutsC1_Only(str);

	//LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
	LCDClearLine(0, 1);
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		//skip LCD_Pos(0, 1, addr);
		LCD_WRData(0, *str);
		if( ++addr >= DISLEN)
			break;
	}
}

//显示被保护的内容
void PutsO_Saved(void)
{
	PutsO_Only(SaveDispO[0]);
	PutsC0_Only(SaveDispC[0]);
    Puts1_Only(SaveDispO[1]);
//    PutsC1_Only(SaveDispC[1]);
}

//保护的LCD上显示的nrong内容
void Save_LCD(char *saveO,char *saveC)
{
	memcpy(saveO,SaveDispO,sizeof(SaveDispO));
	memcpy(saveC,SaveDispC,sizeof(SaveDispC));
}

//设置客显是否与主显同步显示
void LCDSet_Cust(char type)
{
    if (type)
        LCD_BL_CTRL |= 0x02;
    else
        LCD_BL_CTRL &= ~0x02;
}

/*********************************************************************************************************
** End Of File
*********************************************************************************************************/
