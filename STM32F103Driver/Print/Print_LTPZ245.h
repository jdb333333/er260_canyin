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

void   Prn_task11_ResetCutter(void);
void   Prn_task16_CutPaper(void);

extern WORD PrnTemperature(void);

/****************************************************************
  Name: TESTKEYS  检测按键状态;
 return: true-按键按下; false-按键没有按下
*****************************************************************/
BYTE TESTKEYS(void)
{
    ULONG t1,t2;
#if 0
    do {
        t1 = (LPC_GPIO0->FIOPIN & KEY1);?????
        t2 = t1;
        t2 = (LPC_GPIO0->FIOPIN & KEY1);?????
    } while (t1!=t2);
#endif
   	if (t1)
        return false;
   	else
        return true;

}

#if ENCUTTER
/****************************************************************
  Name: TESTCUTS  检测切刀HOME状态;
 return: true-切刀在home位置; false-切刀不在HOME位置
*****************************************************************/
BYTE TESTCUTS(void)
{
    ULONG t1,t2;

    do {
        t1 = (LPC_GPIO0->FIOPIN & CUTS);//?????
        t2 = t1;
        t2 = (LPC_GPIO0->FIOPIN & CUTS);//?????
    } while (t1!=t2);

   	if (t1)
        return true;
   	else
        return false;

}
#endif


// move motor by step 1
void Move_Motor()
{
	Drive_Motor(TAB_PHASEJ[Prn_Jphase]);//out
	Prn_Jphase++;
	Prn_Jphase &= (PHASEMAX-1);

}
#if ENCUTTER
// move cutter motor forword by step 1
void Move_CutMotorF()
{
	Drive_CutMotor(TAB_PHASECUT[Prn_Jphase]);//out
	Prn_Jphase++;
	Prn_Jphase &= (CPHASEMAX-1);

}
// move cutter motor reverse by step 1
void Move_CutMotorR()
{
	Drive_CutMotor(TAB_PHASECUT[Prn_Jphase]);//out
	Prn_Jphase--;
	Prn_Jphase &= (CPHASEMAX-1);

}
#endif
/****************************************************************
  Name: Prn_CalcHotTime  计算加热时间
 return: Prn_HotTime-加热时间
*****************************************************************/
#define PRNMAX  4096
//WORD prnbuff[PRNMAX];//testonly
//int prncounter=0;//testonly

void Prn_CalcHotTime()
{
	WORD DST_time,VTempValue;

    VTempValue = PrnTemperature();
//    prnbuff[prncounter++]=VTempValue; prncounter&=(PRNMAX-1);//testonly
    VTempValue /= 128;//change it into 0~32
    VTempValue+=2; //HJ 增加浓度
    if (VTempValue>31) VTempValue = 31;

#if 1
    if (VTempValue<6)//Too hot of the printer
#else//testonly
    if (VTempValue<16)//Too hot of the printer
#endif
    {
//testonly        Prn_Status.g_bTOOHOT = 1;
    }
#if POWERCTRL //HJ 获取加热时间
    if (TESTBIT(PowerFlags,BIT0))
    {//电池供电,分六组进行加热
        //UART_PutChar(VTempValue);//testonly
        Prn_HotTime = TABSTROBE24VB[VTempValue];
    }
    else
#endif
	{
	    if      (Prn_MotorTime<315*uSECOND) DST_time=14;    //3200 PPS
	    else if (Prn_MotorTime<335*uSECOND) DST_time=13;    //3000 PPS
	    else if (Prn_MotorTime<359*uSECOND) DST_time=12;    //2800 PPS
	    else if (Prn_MotorTime<389*uSECOND) DST_time=11;    //2600 PPS
	    else if (Prn_MotorTime<417*uSECOND) DST_time=10;    //2400 PPS
	    else if (Prn_MotorTime<459*uSECOND) DST_time=9;     //2200 PPS
	    else if (Prn_MotorTime<508*uSECOND) DST_time=8;     //2000 PPS
	    else if (Prn_MotorTime<563*uSECOND) DST_time=7;     //1800 PPS
	    else if (Prn_MotorTime<641*uSECOND) DST_time=6;     //16000 PPS
	    else if (Prn_MotorTime<733*uSECOND) DST_time=5;     //1400 PPS
	    else if (Prn_MotorTime<836*uSECOND) DST_time=4;     //1200 PPS
	    else if (Prn_MotorTime<1074*uSECOND) DST_time=3;     //1000 PPS
	    else if (Prn_MotorTime<1302*uSECOND) DST_time=2;     //800 PPS
	    else if (Prn_MotorTime<1769*uSECOND) DST_time=1;     //600 PPS
	    else  DST_time = 0;        							//400 PPS
	    Prn_HotTime = TABSTROBE24V[VTempValue][DST_time];
	}



#if GROUPMODE==2
    VTempValue = Prn_HotTime * Prn_HotMAX * 2;//每组加热两次
#else
    VTempValue = Prn_HotTime;
#endif

#if (defined(CASE_FORHANZI))
	if(VTempValue+Prn_HZTime >= Prn_MotorTime * STEPS1LINE)//不分组,两步一线
    {//如果加热时间大于步进电机时间,修正步进时间
        Prn_MotorTime = (VTempValue+Prn_HZTime)/2 + 5 * uSECOND ;
#else
    if((VTempValue+PDATATIME+ASCTIMES*PRNWIDTH) >= Prn_MotorTime * STEPS1LINE)//不分组,两步一线
    {//如果加热时间大于步进电机时间,修正步进时间
            Prn_MotorTime = (VTempValue+PDATATIME+ASCTIMES*PRNWIDTH)/2 ;//+ 5 * uSECOND ;
#endif

        Prn_Status.g_bHotOver = 1;
        if (Prn_AccStep>=VMAXSPEED)
            Prn_AccStep = VMAXSPEED-1;
            
        while (Prn_AccStep)
        {
            if (Prn_MotorTime< TAB_PRTIME[Prn_AccStep])
                break;
            Prn_AccStep --;
        }
    }
    else
        Prn_Status.g_bHotOver = 0;

	//HJ 每个点由两次加热，变为1次加热
//	if (TESTBIT(PowerFlags,BIT0))
	{
		Prn_HotTime = Prn_HotTime + Prn_HotTime;
	}
}
/************************************************************************************
  Tasks for print
************************************************************************************/
//Stop motor===========================================
void Prn_task00_Stop()
{
    Drive_Off();//    stop print motor
	Set_Motor_Speed( TIMESTART );
    PrnPowerOFF();
	Prn_TaskNow = 0xff;

    Prn_Jphase = 0;
    Prn_AccStep = 0;
}
//===========================================
void Prn_task01_Start()
{
	int i;
    BYTE *linep;

    GetTaskFromPool();
    if(Prn_Status.g_bPrintOver)//所有数据打印完毕
    {
        Stop_Motor();//关闭motor
    }
#if ENCUTTER
    else if (BIT(ApplVar.Prn_Command, PRN_CUTF_Mode | PRN_CUTH_Mode))
    {//cut paper
        Prn_task16_CutPaper();
    }
    else if (BIT(ApplVar.Prn_Command,PRN_CUTRESET_Mode))
    {//reset cutter
        Prn_task11_ResetCutter();
    }
    else
#endif
    {//print function
#if ENCUTTER
        Drive_CutMotor(CPHSTOP);//stop cut motor
#endif
        Move_Motor();
        Set_Motor_Speed( Prn_MotorTime );

    	if(Prn_Status.g_bPrintGraph)//print a graphic without text
    	{
    		Prn_TaskNow = TASK05_FeedHalf;
            Prn_TaskNext = TASK03_PrintGraphic;
    	}
        else   if (Prn_LineChars)
        {
            Create384Dots();
            linep = DotsOfCurrent;

            //开始送点阵之前,进行必要的初始化
            Prn_LineDots = 0;
#if GROUPMODE==2
            memset(Prn_GroupDots,0,sizeof(Prn_GroupDots));
#endif

#if WORDorBYTE
            for(i=0;i<DOTSBUFF;i+=2)
            {
                Set_Hot_DataW( ((WORD)linep[0]<<8) + linep[1], i );
                linep+=2;
            }
#else
            for(i=0;i<DOTSBUFF;i++)
            {
                Set_Hot_Data( *linep++ , i );
            }
#endif
#if GROUPMODE==2
            //送完最后一个字节时,设定加热组属性数据
            Set_Hot_Group();
#endif
            Prn_LineIndex=1;

            Start_Hot();

            Prn_TaskNow = TASK05_FeedHalf;
            Prn_TaskNext = TASK02_PrintChar;

        }
        else
        {
            Prn_TaskNow = TASK04_FeedPaper;
            Prn_BlankAppend = (LINE_BLANK + TEXT_HIGH) * STEPS1LINE;
        }
    }
}
//print text============================================
void Prn_task02_PrintChar()
{
	int i;
    BYTE *linep;

	Move_Motor();
    Set_Motor_Speed( Prn_MotorTime );

	if(Prn_LineIndex<TEXT_HIGH)
	{
		Prn_TaskNow = TASK05_FeedHalf;
        Prn_TaskNext = TASK02_PrintChar;

        if (BIT(ApplVar.Prn_Command,PRN_DBLH_Mode))
		{ //double high
			if(Prn_Status.g_bDoublHight)
			{//对同一行进行再次加热
				Prn_Status.g_bDoublHight = false;
                //20130709>>>>>>>>>
#if GROUPMODE==2
            //送完最后一个字节时,设定加热组属性数据
                Set_Hot_Group();
#endif
                Start_Hot();//<<<<<<<<<<
				return;//print current dots twice
			}
            else
                Prn_Status.g_bDoublHight = true;
		}
        if (Prn_LineChars)
        {
            Create384Dots();
            linep = DotsOfCurrent;

            //开始送点阵之前,进行必要的初始化
            Prn_LineDots = 0;
#if GROUPMODE==2
            memset(Prn_GroupDots,0,sizeof(Prn_GroupDots));
#endif
#if WORDorBYTE
            for(i=0;i<DOTSBUFF;i+=2)
            {
                Set_Hot_DataW( ((WORD)linep[0]<<8) + linep[1], i );
                linep+=2;
            }
#else
            for(i=0;i<DOTSBUFF;i++)
            {
                Set_Hot_Data( *linep++ , i );
            }
#endif

#if GROUPMODE==2
            //送完最后一个字节时,设定加热组属性数据
            Set_Hot_Group();
#endif

            Start_Hot();
        }
		Prn_LineIndex++;
	}
	else
	{
#if defined(CASE_FORHANZI)
        Prn_HZTime = 0;
        if (Prn_AccCopy)
        {
            Prn_AccStep = Prn_AccCopy;
            Prn_AccCopy = 0;
        }
#endif
		Prn_LineIndex = 0;// 可以省略
		Prn_TaskNow = TASK04_FeedPaper;
		Prn_BlankAppend = LINE_BLANK*STEPS1LINE;
		ApplVar.PoolPopOut = Prn_PoolCurrent;
	}
}
// Print graphic====================================
void Prn_task03_PrintGraphic()
{
	Move_Motor();
    Set_Motor_Speed( Prn_MotorTime );

	if(!Prn_Status.g_bGraphicOver)
	{
		if(Prn_GraphTextMax)
			CreatGraphicWithText();
		else
			CreateGraphicDots();
		Prn_TaskNow = TASK05_FeedHalf;
        Prn_TaskNext = TASK03_PrintGraphic;
	}
	else
	{
#if defined(CASE_FORHANZI)
        Prn_HZTime = 0;
        if (Prn_AccCopy)
        {
            Prn_AccStep = Prn_AccCopy;
            Prn_AccCopy = 0;
        }
#endif
		Prn_Status.g_bPrintGraph = false;
		Prn_TaskNow = TASK04_FeedPaper;
		Prn_BlankAppend = LINE_BLANK*STEPS1LINE;
		ApplVar.PoolPopOut = Prn_PoolCurrent;
	}
}
//feed paper=======================================
void Prn_task04_FeedPaper()
{
	int i;
    BYTE *linep;

    PrnHotOFF();//!!!!@@@@@@@@@!!!!!!
    Move_Motor();

	if(Prn_BlankAppend)
    {
        Set_Motor_Speed( Prn_MotorTime );
		Prn_BlankAppend--;
    }
	else
	{
        ApplVar.PoolPopOut = Prn_PoolCurrent;

        GetTaskFromPool();

		if(Prn_Status.g_bPrintOver)//所有数据打印完毕
		{
			Stop_Motor();//关闭motor
		}
#if ENCUTTER
        else  if (BIT(ApplVar.Prn_Command, PRN_CUTF_Mode | PRN_CUTH_Mode | PRN_CUTRESET_Mode))
        {//cut paper
            Prn_TaskNow = 0xff;
            Start_Motor(ApplVar.Prn_Command);
        }
#endif
        else
		{
            Set_Motor_Speed( Prn_MotorTime );
			if(Prn_Status.g_bPrintGraph)//打印图片
			{
				Prn_TaskNow = TASK05_FeedHalf;
                Prn_TaskNext = TASK03_PrintGraphic;
			}
            else if (Prn_LineChars)
            {
                Create384Dots();
                linep = DotsOfCurrent;
                //开始送点阵之前,进行必要的初始化
                Prn_LineDots = 0;
#if GROUPMODE==2
                memset(Prn_GroupDots,0,sizeof(Prn_GroupDots));
#endif
#if WORDorBYTE
                for(i=0;i<DOTSBUFF;i+=2)
                {
                    Set_Hot_DataW( ((WORD)linep[0]<<8) + linep[1], i );
                    linep+=2;
                }
#else
                for(i=0;i<DOTSBUFF;i++)
                {
                    Set_Hot_Data( *linep++ , i );
                }
#endif

#if GROUPMODE==2
            //送完最后一个字节时,设定加热组属性数据
                Set_Hot_Group();
#endif
                Start_Hot();

    			Prn_LineIndex=1;
    			Prn_TaskNow = TASK05_FeedHalf;
                Prn_TaskNext = TASK02_PrintChar;
            }
            else
            {
                Prn_BlankAppend = (LINE_BLANK + TEXT_HIGH) * STEPS1LINE;
                ApplVar.PoolPopOut = Prn_PoolCurrent;
            }
		}
	}

}
//Drive motor half step=============================
#if (STEPS1LINE>2)
BYTE StepS=1;
#endif

void Prn_task05_FeedHalf()
{

	Move_Motor();
	Set_Motor_Speed( Prn_MotorTime );
    Prn_Status.g_bHotOver = 0;


#if (STEPS1LINE>2)
    StepS++;

#if GROUPMODE==2
/* 加热计时器启用中断服务时，此处代码取消
    if (StepS==STEPS1LINE/2 && Prn_LineDots)
    {
        Prn_HotIndex=0;
        Set_HotPIN();//多组合并加热
        START_HOT_TIMER();
    }
    */
#endif

    if(StepS<STEPS1LINE)
        return;
    StepS = 1;

#elif GROUPMODE==2
/* 加热计时器启用中断服务时，此处代码取消
    if (Prn_LineDots)
    {
        Prn_HotIndex=0;
        Set_HotPIN();//多组合并加热
        START_HOT_TIMER();
    }
*/
#endif
    Prn_TaskNow = Prn_TaskNext;
}

//tasks for cutter>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#if ENCUTTER
//reset cutter===========================================
void   Prn_task10_StopCutter()
{

    Drive_CutMotor(CPHSTOP);//out

    if (Cut_Status.g_bStop)
    {//stop cut motor
        ApplVar.PoolPopOut = Prn_PoolCurrent;// 当前切纸任务处理完毕

        LPC_RIT->RICTRL &= ~RIT_CTRL_TEN;			 //disable counter
        if (ApplVar.PoolPushIn != ApplVar.PoolPopOut)
        {//A new task wait for service
            Prn_PoolCurrent = (ApplVar.PoolPopOut + 1);
            if (Prn_PoolCurrent>=MAX_PRNPOOL) Prn_PoolCurrent = 0;
            Prn_TaskNow = 0xff;
            Start_Motor(ApplVar.PoolForPrint[Prn_PoolCurrent].Prn_Command);
        }
        else
        {// not any task in the pool
            Prn_TaskNow = 0xff;
        }
    }
    else if (BIT(ApplVar.Prn_Command,PRN_CUTRESET_Mode))
    {// 停止切纸电机后,改变切纸电机的走向(反向.)
        Set_Motor_Speed( TIMESTART );
        Cut_Status.g_bReverse = 1;
        Prn_TaskNow = TASK15_FeedStepAlpha;
        Prn_TaskNext = TASK15_FeedStepAlpha;
        CutMotorMax = CUTSTEPALPHA;
    }
    else //if (BIT(ApplVar.Prn_Command,PRN_CUTF_Mode | PRN_CUTH_Mode))
    {// 停止切纸电机后,改变切纸电机的走向(反向.),继续切纸任务
        Set_Motor_Speed( TIMESTART );
        Cut_Status.g_bReverse = 1;
        Prn_TaskNow = TASK18_FeedStepC;
        Prn_TaskNext = TASK18_FeedStepC;
        if (BIT(ApplVar.Prn_Command,PRN_CUTF_Mode))
            CutMotorMax = CUTSTEPC_F;
        else
            CutMotorMax = CUTSTEPC_H;
        if (Cut_Status.g_bFirst)
            CutMotorMax *= 2;
        Cut_Status.g_bFirst = 0;
    }


    Prn_FeedCount = 0;
    Prn_Jphase = 0;
    Prn_AccStep = 0;

}
//reset cutter===========================================
void   Prn_task11_ResetCutter()
{
    Drive_Off();// stop print motor

    Cut_Status.g_bCutOK = 0;
    Prn_FeedCount = 0;
    if (TESTCUTS())
    {
        Move_CutMotorF();
        Set_Motor_Speed( Prn_MotorTime );
        Prn_TaskNow = TASK12_FeedCutter;   //Drive cutter motor half step
        Prn_TaskNext = TASK13_FeedStepCA;
        Cut_Status.g_bReverse = 0;
        if (Cut_Status.g_bFirst)
            CutMotorMax = CUTSTEPA*2;
        else
            CutMotorMax = CUTSTEPA;
        Cut_Status.g_bFirst = 0;
    }
    else
    {
        Move_CutMotorR();
        Set_Motor_Speed( Prn_MotorTime );
        Prn_TaskNow = TASK12_FeedCutter;   //Drive cutter motor half step
        Prn_TaskNext = TASK13_FeedStepCA;
        Cut_Status.g_bReverse = 1;
        if (Cut_Status.g_bFirst)
            CutMotorMax = CUTSTEPC_F*2;
        else
            CutMotorMax = CUTSTEPC_F;
        Cut_Status.g_bFirst = 0;

    }
}
//feed cutter motor half phase===================================
void   Prn_task12_FeedCutter()
{
    if (Cut_Status.g_bReverse)
        Move_CutMotorR();
    else
        Move_CutMotorF();
    Set_Motor_Speed( Prn_MotorTime );

#if (STEPS1LINE>2)
    StepS++;
    if(StepS<STEPS1LINE)
        return;
    StepS = 1;
#endif

    Prn_TaskNow = Prn_TaskNext;

}
//Move cutter by STEPC===========================================
void   Prn_task13_FeedStepC()
{
    if (Cut_Status.g_bReverse)
    {
        Move_CutMotorR();
        if (TESTCUTS())
        {// cutter is ok
            Set_Motor_Speed( Prn_MotorTime );
            Prn_TaskNow = TASK14_FeedStepE;//drive cut motor STEPSE
            Prn_FeedCount = 0;
        }
        else
        {//Feed reverse by STEPC
            if (Prn_FeedCount>CutMotorMax)
            {//error found,stop motor
                Set_Motor_Speed( TIMESTOP );
                Cut_Status.g_bStop = 1;
                Prn_TaskNow = TASK10_StopCutter;//stop cut motor
            }
            else
            {
                Prn_TaskNow = TASK12_FeedCutter;
                Set_Motor_Speed( Prn_MotorTime );
            }
        }
    }
    else
    {
        Move_CutMotorF();
        if (!TESTCUTS())
        {// move motor forword
            if (BIT(ApplVar.Prn_Command,PRN_CUTRESET_Mode))
            {//task for reset cutter
                Set_Motor_Speed( TIMESTOP );//stop motor first
                Cut_Status.g_bStop = 0;
                Prn_TaskNow = TASK10_StopCutter;//stop cut motor first
            }
            else
            {//task for cut paper

                if (BIT(ApplVar.Prn_Command,PRN_CUTF_Mode))
                    CutMotorMax = CUTSTEPB_F;
                else
                    CutMotorMax = CUTSTEPB_H;
                Prn_FeedCount = 0;
                Prn_TaskNow = TASK17_FeedStepB;//feed cutter by STEPB
            }
        }
        else
        {// feed by STEPA
            if (Prn_FeedCount>CutMotorMax)
            {//error found,stop motor
                Set_Motor_Speed( TIMESTOP );
                Cut_Status.g_bStop = 1;
                Prn_TaskNow = TASK10_StopCutter;//stop cut motor
            }
            else
            {
                Prn_TaskNow = TASK12_FeedCutter;
                Prn_TaskNext = TASK13_FeedStepCA;
                Set_Motor_Speed( Prn_MotorTime );
            }
        }
    }
}
//reverse Move cutter by STEPE===========================================
void   Prn_task14_FeedStepE()
{
    Move_CutMotorR();
    Set_Motor_Speed( Prn_MotorTime );

#if (STEPS1LINE>2)
    StepS++;
    if(StepS<STEPS1LINE)
        return;
    StepS = 0;
#endif

    if (Prn_FeedCount>CUTSTEPE)
    {
        Set_Motor_Speed( TIMESTOP );
        Prn_TaskNow = TASK10_StopCutter;//stop cut motor
        Cut_Status.g_bCutOK = 1;
        Cut_Status.g_bStop = 1;
    }

}
//reverse Move cutter by STEPE===========================================
void   Prn_task15_FeedStepAlpha()
{
        Move_CutMotorR();
        if (TESTCUTS())
        {// cutter is ok
            Set_Motor_Speed( Prn_MotorTime );
            Prn_TaskNow = TASK14_FeedStepE;//drive cut motor STEPSE
            Prn_FeedCount = 0;
        }
        else
        {
            if (Prn_FeedCount>CutMotorMax)
            {
                Set_Motor_Speed( TIMESTOP );
                Cut_Status.g_bStop = 1;
                Prn_TaskNow = TASK10_StopCutter;//stop cut motor
            }
            else
            {
                Set_Motor_Speed( Prn_MotorTime );
            }
        }
}
// tasks for cut paper>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void   Prn_task16_CutPaper()
{
    Drive_Off();// stop print motor
    Prn_FeedCount = 0;
    Prn_TaskNow = TASK13_FeedStepCA;   //Drive cutter motor half step
    Cut_Status.g_bReverse = 0;
    if (Cut_Status.g_bFirst)
        CutMotorMax = CUTSTEPA*2;
    else
        CutMotorMax = CUTSTEPA;
    Cut_Status.g_bFirst = 0;
    Prn_task13_FeedStepC();//feed by stepA
}
// task for feed by stepB============================================
void Prn_task17_FeedStepB()
{
    Move_CutMotorF();
    Set_Motor_Speed( Prn_MotorTime );

#if (STEPS1LINE>2)
    StepS++;
    if(StepS<STEPS1LINE)
        return;
    StepS = 0;
#endif

    if (Prn_FeedCount>CutMotorMax)
    {
        Set_Motor_Speed( TIMESTOP );
        Prn_TaskNow = TASK10_StopCutter;//stop cut motor
        Cut_Status.g_bStop = 0;
        Cut_Status.g_bStepC1st = 1;
        Prn_TaskNext = TASK18_FeedStepC;
    }
}
// task for stepC======================================================
void Prn_task18_FeedStepC()
{
    Move_CutMotorR();
    Set_Motor_Speed( Prn_MotorTime );

#if (STEPS1LINE>2)
    StepS++;
    if(StepS<STEPS1LINE)
        return;
    StepS = 0;
#endif

    if (TESTCUTS())
    {// cutter is ok

        if (Prn_FeedCount<CUTSTEPD_F)//?????????????????????????
        {//drive cut motor STEPE
            Set_Motor_Speed( Prn_MotorTime );
            Prn_TaskNow = TASK14_FeedStepE;
            Prn_FeedCount = 0;
        }
        else
        {//error found
            Set_Motor_Speed( TIMESTOP );
            Cut_Status.g_bStop = 1;
            Cut_Status.g_bCutOK = 0;
            Prn_TaskNow = TASK10_StopCutter;//stop cut motor
        }
    }
    else
    {//Feed reverse by STEPC
        if (Prn_FeedCount>CutMotorMax)
        {//
            if (Cut_Status.g_bStepC1st)
            {//backward the motor by 8 dots line.
                Cut_Status.g_bStepC1st = 0;
                PrnPowerON();
                Drive_CutMotor(CPHSTOP);//stop cutmotor
                Drive_Motor(PHSTART);//out
                Set_Motor_Speed( TIMESTART );
                Prn_Jphase = 0;
                Prn_AccStep = 0;
                Prn_TaskNow = TASK19_RevFeed8Line;
            }
            else
            {//error found
                Set_Motor_Speed( TIMESTOP );
                Cut_Status.g_bStop = 1;
                Cut_Status.g_bCutOK = 0;
                Prn_TaskNow = TASK10_StopCutter;//stop cut motor
            }
        }
    }
}
//backward the motor by 8 dots line.===================
void Prn_task19_RevFeed8Line()
{
    Prn_Jphase--;
    Prn_Jphase &= (PHASEMAX-1);
    Drive_Motor(TAB_PHASEJ[Prn_Jphase]);//out
    Set_Motor_Speed( Prn_MotorTime );
#if (STEPS1LINE>2)
    StepS++;
    if(StepS<STEPS1LINE)
        return;
    StepS = 0;
#endif
    if (Prn_AccStep>8*STEPS1LINE)
    {
        Drive_Off();//     stop print motor
        Drive_CutMotor(CPHSTART);//out
        Set_Motor_Speed( TIMESTART );
        PrnPowerOFF();
        Prn_Jphase = 0;
        Prn_AccStep = 0;
        Prn_TaskNow = TASK18_FeedStepC;
        Prn_FeedCount=0;
    }

}

#endif
/************************************************************
当检测到打印机无纸时,设置等待装纸标志Prn_Status.g_bWaitPaper
关闭打印机,但是定时启动打印机任务处理程序,以便检测是否装纸
*************************************************************/
void Wait_Paper()
{
    PrnHotOFF();
    Drive_Off();//     stop print motor
    Set_Motor_Speed( TIMESTART );
    PrnPowerOFF();
    Prn_TaskNow = 0xff;

    Prn_Jphase = 0;
    Prn_AccStep = 0;

    Prn_LineIndex = 0;
	Prn_BlankAppend = LINE_BLANK*STEPS1LINE;
	Prn_Status.g_bPrintGraph = false;
    Prn_Status.g_bWaitPaper = true;//设置等待装纸标志

}
/*================================ main for print===============================*/
/***************************打印任务调度服务处理********************************
 TIM2_IRQHandler在打印过程中如果发现无纸
1. 调用Wait_Paper,自动关闭打印机的电源, 以PHSTART定期调度TIM2_IRQHandler
2. 当重新检测到有纸后,启动任务4,先走一段空白后,再继续打印
********************************************************************************/

void TIM2_IRQHandler(void)
{
  /* Clear TIM2 update interrupt */
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  StopTimer(TIM2);

#if ENCUTTER
    // 切纸工作模式下,不检测无纸等状态
    if (!BIT(ApplVar.Prn_Command,PRN_CUTRESET_Mode | PRN_CUTF_Mode | PRN_CUTH_Mode))
#endif
    {
        Check_Print();
#if POWERCTRL //HJ ISR中的状态监测
        if(Prn_Status.g_bTOOHOT || BATTERYTooLow(0x05))//打印机过热,ccr20131015电池电压过低,暂停打印
#else
        if(Prn_Status.g_bTOOHOT)//打印机过热,ccr20131015电池电压过低,暂停打印
#endif
        {//
#if POWERCTRL //HJ ISR中的状态监测
            if(!Prn_Status.g_bSuspend || BATTERYTooLow(0x05))
#else
            if(!Prn_Status.g_bSuspend)
#endif
            {//打印机过热或电池电压过低,暂停打印
                Suspend_Motor();
            }
            else if (Prn_Delay==0)
            {//打印机暂停结束
                Restart_Motor();
            }
            return;//打印机过热
        }
#if CASE_HANDUP
        if(Prn_Status.g_bHANDUP)
        {
            if (!Prn_Status.g_bWaitPaper)
                Wait_Paper();
            else
                Set_Motor_Speed( TIMESTART );
            return; //打印机压杆抬起
        }
        else
        {
            if (Prn_Status.g_bHANDDN)
            {//压杆抬起后,重新压下压杆,重新起动打印
                Prn_Status.g_bHANDDN = false;
                Prn_Status.g_bPaperIn = false;
                Prn_Status.g_bNOPAPER = false;
                Prn_Status.g_bWaitPaper = false;
                Prn_TaskNow = 0xff;
                Start_Motor(ApplVar.Prn_Command);
                return; //打印机错误处理完毕,需要重新启动打印
            }
        }
#endif
        //检测是否有纸
        if(Prn_Status.g_bNOPAPER)
        {//无纸
            if (!Prn_Status.g_bWaitPaper)//为第一次检测到无纸,关闭打印机
                Wait_Paper();
            else
                Set_Motor_Speed( TIMESTART );
            return; //没有装纸
        }
        else
        {//有纸
            if (Prn_Status.g_bPaperIn)
            {//为缺纸后插入了打印纸,重新起动打印
                Prn_FeedCount = Prn_Delay = 0;
                Prn_Status.g_bPaperIn = false;
                Prn_Status.g_bHANDDN = false;
                Prn_Status.g_bHANDUP = false;
                Prn_Status.g_bWaitPaper = false;
#if 1
                if ((ApplVar.ErrorNumber&0x7f)>=ERROR_ID(CWXXI39) && (ApplVar.ErrorNumber&0x7f)<=ERROR_ID(CWXXI41))
                    ApplVar.ErrorNumber = 0;//避免在CheckPrinter中重复执行Start_When_Ready(" ")

                Start_When_Ready(" ");
				//直接启动打印
//                Prn_TaskNow = 0xff;
//                Start_Motor(ApplVar.Prn_Command);
                return;
#else
				//启用4号任务,先走纸(会导致异常)

                Prn_PoolCurrent = ApplVar.PoolPopOut;
                Prn_TaskNow = TASK04_FeedPaper;
                Prn_BlankAppend = (LINE_BLANK + TEXT_HIGH) * STEPS1LINE * 10;//走十行空白
                PrnPowerON();
                Drive_Motor(PHSTART);//out
                Set_Motor_Speed( TIMESTART );
                Prn_Jphase = 0;
                Prn_AccStep = 0;
                //LPC_RIT->RICTRL |= RIT_CTRL_TEN;//Enable counter
                return;
#endif
            }
        }
    }

    //打印机无异常
    if(!Prn_Status.g_bPrintOver)//It's have data for print
    {

#if ENCUTTER
        if (BIT(ApplVar.Prn_Command,PRN_CUTRESET_Mode | PRN_CUTF_Mode | PRN_CUTH_Mode) && Prn_TaskNow!=TASK19_RevFeed8Line)
        {
            if(Prn_AccStep<CUTMAXSPEED)
                Prn_MotorTime = TAB_CUTTIME[Prn_AccStep];
        }
        else
#endif
        {
#if POWERCTRL //HJ 获取下次步进换相的时间
            if (TESTBIT(PowerFlags,BIT0))
            {//ccr20131015
                if(Prn_AccStep<11			//HJ 降低步进速度，保证电池加热时间
                   && !Prn_Status.g_bHotOver)//加热时间不超过步进时间,(当加热时间超过了步进时间时,右半步的步进时间不变)
                    Prn_MotorTime = TAB_PRTIME[Prn_AccStep];
            }
            else
#endif
            {
                if(Prn_AccStep<VMAXSPEED
                   && !Prn_Status.g_bHotOver)//加热时间不超过步进时间,(当加热时间超过了步进时间时,右半步的步进时间不变)
                    Prn_MotorTime = TAB_PRTIME[Prn_AccStep];
            }
            Prn_Status.g_bHotOver = 0;
        }
    }
    Prn_AccStep++;
    switch(Prn_TaskNow)
    {
        case TASK00_Stop:
            Prn_task00_Stop();
            break;
        case TASK01_Start:
            Prn_task01_Start();
            break;
        case TASK02_PrintChar:
            Prn_task02_PrintChar();
            break;
        case TASK03_PrintGraphic:
            Prn_task03_PrintGraphic();
            break;
        case TASK04_FeedPaper:
            Prn_task04_FeedPaper();
            break;
        case TASK05_FeedHalf:
            Prn_task05_FeedHalf();
            break;

        case 6://reserved
        case 7://reserved
        case 8://reserved
        case 9://reserved
            break;
#if ENCUTTER
                    //reset cutter
        case TASK10_StopCutter:
            Prn_task10_StopCutter();
            break;
        case TASK11_ResetCutter:
            Prn_task11_ResetCutter();
            break;
        case TASK12_FeedCutter:
            Prn_task12_FeedCutter();
            break;
        case TASK13_FeedStepCA:
            Prn_task13_FeedStepC();
            break;
        case TASK14_FeedStepE:
            Prn_task14_FeedStepE();
            break;
        case TASK15_FeedStepAlpha:
            Prn_task15_FeedStepAlpha();
            break;
                    //cut paper
        case TASK16_CutPaper:
            Prn_task16_CutPaper();
            break;
        case TASK17_FeedStepB:
            Prn_task17_FeedStepB();
            break;
        case TASK18_FeedStepC:
            Prn_task18_FeedStepC();
            break;
        case TASK19_RevFeed8Line:
            Prn_task19_RevFeed8Line();
            break;
#endif
        default:
            break;
    }
    return;
}

#if EN_PRINT
/*********************************************************************************************************
* Function Name:        TIMER3_IRQHandler
* Description:          TIMER3 中断处理函数,用于加热控制
* 当分组加热次数超过两次时，采用中断服务程序来控制分组加热
*********************************************************************************************************/
void TIM3_IRQHandler (void)
{
    /* Clear TIM2 update interrupt */
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    StopTimer(TIM3);

	PrnHotOFF();

	Prn_HotIndex++;
	if (Prn_HotIndex>=Prn_HotMAX)
		return;
	Prn_HotIndex %= Prn_HotMAX;

#if GROUPS>1

#if GROUPMODE==0
//此处只考虑了分 6组的情况。
   switch (Prn_HotIndex)
   {
       case 0:return;//加热结束
       case 1:Hot_On(HOT2_PIN);  break;
       case 2:Hot_On(HOT3_PIN);  break;
       case 3:Hot_On(HOT4_PIN);  break;
       case 5:Hot_On(HOT5_PIN);  break;
       case 6:Hot_On(HOT6_PIN);  break;
   }
   StartTimer(TIM3);    // 启动定时器

#else
//   if (Prn_HotIndex && Prn_GroupHot[Prn_HotIndex])
   if (Prn_GroupHot[Prn_HotIndex])
   {
       //if (Prn_HotIndex!=GROUPS*2-1)//!!!!@@@@@@@@@!!!!!!
       {
           Set_HotPIN();
           Set_HotTime(Prn_HotTime+10*uSECOND);//增加加热时间，以补偿第二组加热时的电压降低

       }
   }
#endif
#endif

}

#endif

