/****************************************Copyright (c)**************************************************
*        EUTRON Human Information Equipment Co.,Ltd.
*                    www.eutron.com.cn
* File name:		Print_ltp1245.c
* Descriptions:     ltp1245打印机驱动程序
*------------------------------------------------------------------------------------------------------
*  Created by:			chen churong
* Created date:        2011-10-01
* Version: 1.0
********************************************************************************************************/

#include "print.h"


// move motor by step 1
void Move_Motor()
{
	Drive_Motor(TAB_PHASEJ[Prn_Jphase]);//out
	Prn_Jphase++;
	Prn_Jphase &= 0x03;

}
//Caculate the strobe
void PR_CalcStrb()
{
	WORD temp_time = 0;
	WORD DST_time,temp_value,AD_value;
	BYTE resis_value = 40;
	BYTE volt_value;
/*	ULONG k = __raw_readl( REG_MFSEL );
	k = (k & 0xFFFF3FFF) | 0x00004000; //GPIOG[1:0]->I2C Pin Function;
	Set_Motor_Speed( k, REG_MFSEL );*/
	//读热敏电阻值到resis_value
//	WR5 = WR5|0x0002;
//	Set_Hot_Time(WR5, EBI_eutronbase+(5<<20));
	udelay(100);
//	resis_value = __raw_readb( EBI_eutronbase+(2<<20) );
//	volt_value = __raw_readb( EBI_eutronbase+(2<<20)+2 );
//	resis_value = 40;
//	WR5 = WR5&(~0x0002);
//	Set_Hot_Time(WR5, EBI_eutronbase+(5<<20));

/*	printk( "Ruby debug 0:AD check. save_client=%d\n",save_client);
	AD_value = i2c_smbus_read_word_data(save_client, 0x00);
	resis_value = AD_value>>4;
	printk( "Ruby debug:resis_value is %d!\n",resis_value);*/

	Prn_VNTCR = resis_value;
	resis_value >>= 2;
	resis_value &= 0xFE;
	DST_time = TABSTROBE24V[resis_value]*COEFFICIENT;
	//调_TestDencity
/*	if(HighDencity_flag)
	{
		temp_value = DST_time;
		DST_time += temp_value >> 2;
	}*/
	Prn_HotTime = DST_time;
	temp_time += DST_time;
/*	if(bit_pri_grafv && (Prn_GraFlg & 0x80) && bit_pri_gralfa)
		temp_time += DST_time;
*/
	if(temp_time <= Prn_MotorTime)
		return;
	temp_time += 10*15;
	Prn_MotorTime = temp_time;
	//重新计数t4,Prn_MotorTime
	Set_Motor_Speed( Prn_MotorTime );
	temp_value = (VMAXSPEED/2) * 2;
	do{
		temp_time = TAB_PRTIME[temp_value];
		if(temp_time > Prn_MotorTime)
			break;
		temp_value -= 2;
	}while(!temp_value);
	temp_value >>= 1;
	Prn_AccStep = temp_value;
}
/************************************************************************************
  Tasks for print
************************************************************************************/
//restart motor
void Prn_task00()
{
	int i;
	Move_Motor();
	Set_Motor_Speed( Prn_MotorTime );

	Prn_TaskNow = 1;
}
//
void Prn_task01()
{
	int i;
    BYTE *linep;

	Move_Motor();
	Set_Motor_Speed( Prn_MotorTime );

	GetTaskFromPool();
	if(Prn_Status.g_bPrintGraph)//print a graphic without text
	{
		Prn_TaskNow = 6;
		return;
	}
	Create384Dots();
    linep = DotsOfCurrent;

	for(i=0;i<DOTSBUFF;i++)
	{
		Set_Hot_Data( *linep++ , i );
		udelay(1);
	}
	Prn_TaskNow = 6;
}
//print text
void Prn_task02()
{
	int i;
    BYTE *linep;

	Move_Motor();
	//timer2 start
	Set_Motor_Speed( Prn_MotorTime );
	//timer4 start
	Set_Hot_Time( Prn_HotTime );
	//double high

	if(Prn_LineIndex<TEXT_HIGH)
	{
		Prn_TaskNow = 6;
//		if(PoolForPrint[PoolCurrent].DoublHigh)
        if (FontsOffset[0] & DBHFLAG)
		{
			if(Prn_Status.g_bDoublHight)
			{
				Prn_Status.g_bDoublHight = false;
				return;//print current dots twice
			}
			Prn_Status.g_bDoublHight = true;
		}
		Create384Dots();
        linep = DotsOfCurrent;
		for(i=0;i<DOTSBUFF;i++)
		{
			Set_Hot_Data( *linep++ , i );
			udelay(1);
		}
		Prn_LineIndex++;
	}
	else
	{
		Prn_LineIndex = 0;
		Prn_TaskNow = 4;
		Prn_BlankAppend = LINE_BLANK;
		PoolPopOut = PoolCurrent;
	}
}
// Print graphic
void Prn_task03()
{
	Move_Motor();
	//timer2 start
	Set_Motor_Speed( Prn_MotorTime );
	//timer4 start
	Set_Hot_Time( Prn_HotTime );
	if(!Prn_Status.g_bGraphicOver)
	{
		if(Prn_Status.g_bGraphChar)
			GraphicDotsWithText();
		else
			CreateGraphicDots();
		Prn_TaskNow = 6;
	}
	else
	{
		Prn_Status.g_bPrintGraph = false;
		Prn_TaskNow = 4;
		Prn_BlankAppend = LINE_BLANK;
		PoolPopOut = PoolCurrent;
	}
}
//feed paper
void Prn_task04()
{
	int i;
    BYTE *linep;

	Move_Motor();
	Set_Motor_Speed( Prn_MotorTime );
	if(Prn_BlankAppend)
		Prn_BlankAppend--;
	else
	{
		GetTaskFromPool();
		if(Prn_Status.g_bPrintOver)//所有数据打印完毕
		{
			Stop_Motor();//关闭motor
			Prn_AccStep = 0;
			Prn_TaskNow = 0;
		}
		else
		{
			if(Prn_Status.g_bPrintGraph)//打印图片
			{
				Prn_TaskNow = 6;
				return;
			}
			Create384Dots();
            linep = DotsOfCurrent;
			for(i=0;i<DOTSBUFF;i++)
			{
				Set_Hot_Data( *linep++ , i );
				udelay(1);
			}
			Prn_LineIndex++;
			Prn_TaskNow = 6;
		}
	}

}
//Drive motor half step
void Prn_task06()
{

	Latch_Data();//latch active,wr5,Q6
	Move_Motor();
	//timer2 start
	Set_Motor_Speed( Prn_MotorTime );

	disLatch_Data();//latch disable WR5|0x20
//	bit_ps_burn = 1;
	PR_CalcStrb();
	//timer3 start RDST
	Set_Hot_Time( Prn_HotTime );
	if(Prn_Status.g_bPrintGraph)
	{
		Prn_TaskNow = 3;
	}
	else
	{
		Prn_TaskNow = 2;
	}
}
//////////////////////////////////////////////////////////
void Wait_Paper()
{
	Set_Motor_Speed( Prn_MotorTime );
	Stop_Motor();//close motor
	Prn_AccStep = 0;
	Prn_TaskNow = 0;
	Prn_LineIndex = 0;
	Prn_BlankAppend = LINE_BLANK;
	Prn_Status.g_bPrintGraph = false;

}
// main for print
void timer2_interrupt()
{
	int i = 0;

	Check_Print();
	if((!(Prn_Status.g_bNOPAPER  &0x01))&&(!Prn_Status.g_bPaperIn))
	{
		Move_Motor();
		if(!(paper_move--))
		{
			Prn_Status.g_bPaperIn = true;
			Stop_Motor();//close motor
		}
		return ;
	}
	if(Prn_Status.g_bPrintOver)//No data for print
	{
		return;
	}

	if((!Prn_Status.g_bPaperOK) || (Prn_Status.g_bNOPAPER))
	{
		Wait_Paper();
		return;
	}
	if(Prn_AccStep<VMAXSPEED)
    {
        Prn_MotorTime = TAB_PRTIME[Prn_AccStep];
		Prn_AccStep++;
    }

	switch(Prn_TaskNow)
	{
		case 0:
			Prn_task00();
			break;
		case 1:
			Prn_task01();
			break;
		case 2:
			Prn_task02();
			break;
		case 3:
			Prn_task03();
			break;
		case 4:
			Prn_task04();
			break;
		case 5:
			break;
		case 6:
			Prn_task06();
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		default:
			break;
	}
	return;
}


