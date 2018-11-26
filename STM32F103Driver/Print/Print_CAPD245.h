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

void   Prn_task11_ResetCutter();
void   Prn_task16_CutPaper();

BYTE emuState=0;//testonly
/****************************************************************
  Name: TESTKEYS  检测按键状态;
 return: true-按键按下; false-按键没有按下
*****************************************************************/
BYTE TESTKEYS(void)
{
    ULONG t1,t2;

    do {
        t1 = (LPC_GPIO0->FIOPIN & KEY1);
        t2 = t1;
        t2 = (LPC_GPIO0->FIOPIN & KEY1);
    } while (t1!=t2);

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
        t1 = (LPC_GPIO0->FIOPIN & CUTS);
        t2 = t1;
        t2 = (LPC_GPIO0->FIOPIN & CUTS);
    } while (t1!=t2);

   	if (t1)
        return true;
   	else
        return false;

}
#endif

/****************************************************************
  Name: TESTPAPER  检测是否有纸;
 return: true-有纸; false-无纸
*****************************************************************/
BYTE TESTPAPER(void)
{
    ULONG t1,t2;


    do {
        t1 = (LPC_GPIO0->FIOPIN & RPE);
        t2 = t1;
        t2 = (LPC_GPIO0->FIOPIN & RPE);
    } while (t1!=t2);

   	if (t1)
        return false;  // 无纸
   	else
        return true;  // 有纸
}

#if RHUP
/****************************************************************
  Name: TESTHAND  检测压杆状态
 return: true-压杆正常; false-压杆异常
*****************************************************************/
BYTE TESTHAND(void)
{
    ULONG t1,t2;


    do {
        t1 = (LPC_GPIO0->FIOPIN & RHUP);
        t2 = t1;
        t2 = (LPC_GPIO0->FIOPIN & RHUP);
    } while (t1!=t2);

   	if (t1)
        return false;  // 压杆抬起
   	else
        return true;  //hand down
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
void Prn_CalcHotTime()
{
	WORD DST_time,temp_value;
    uint32_t AD_value;


    AD_value = ADC0Read();
    AD_value /= 128;//change it into 0~32

#if 1
    if (AD_value<6)//Too hot of the printer
#else//testonly
    if (AD_value<16)//Too hot of the printer
#endif
    {
        Prn_Status.g_bTOOHOT = 1;
    }
    if      (Prn_AccStep>109) DST_time=12;     //2700 PPS
    else if (Prn_AccStep>93) DST_time=11;     //2600 PPS
    else if (Prn_AccStep>78) DST_time=10;     //2400 PPS
    else if (Prn_AccStep>65) DST_time=9;     //2200 PPS
    else if (Prn_AccStep>53) DST_time=8;     //2000 PPS
    else if (Prn_AccStep>42) DST_time=7;     //1800 PPS
    else if (Prn_AccStep>32) DST_time=6;     //1600 PPS
    else if (Prn_AccStep>24) DST_time=5;     //1400 PPS
    else if (Prn_AccStep>17) DST_time=4;     //1200 PPS
    else if (Prn_AccStep>11 ) DST_time=3;     //1000 PPS
    else if (Prn_AccStep>6 ) DST_time=2;     //800 PPS
    else if (Prn_AccStep>4 ) DST_time=0;     //600 PPS
    else  DST_time = 0;        //400 PPS

    Prn_HotTime = TABSTROBE24V[AD_value][DST_time];

#if CHINESE
	if(Prn_HotTime+Prn_HZTime >= Prn_MotorTime * STEPS1LINE)//不分组,两步一线
    {//如果加热时间大于步进电机时间,修正步进时间
        Prn_MotorTime = (Prn_HotTime+Prn_HZTime)/2 ;//+ 5 * uSECOND ;
#else
        if(Prn_HotTime >= Prn_MotorTime * STEPS1LINE)//不分组,两步一线
        {//如果加热时间大于步进电机时间,修正步进时间
            Prn_MotorTime = (Prn_HotTime)/2 ;//+ 5 * uSECOND ;
#endif
        //if (Prn_AccStep<VMAXSPEED)
        {
            //Prn_AccCopy = Prn_AccStep;
            Prn_AccStep = VMAXSPEED+1;
        }
        return;
/*
        temp_value = Prn_AccStep-1;
        if (temp_value>=VMAXSPEED)
            temp_value = VMAXSPEED-1;
        else if (temp_value<0)
            temp_value = 0;
        while (temp_value>0)
        {
            if (Prn_MotorTime> TAB_PRTIME[temp_value])
                break;
            temp_value --;
        }
        Prn_AccStep = temp_value;
 */
    }
}
/************************************************************************************
  Tasks for print
************************************************************************************/
//Stop motor===========================================
void Prn_task00_Stop()
{
#if DEBUGONPC==0
    LPC_RIT->RICTRL &= ~RIT_CTRL_TEN;			 //disable counter
#endif
    Drive_Off();//    stop print motor
	Set_Motor_Speed( TIMESTART );
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
    else if (BIT(Prn_Mode, PRN_CUTF_Mode | PRN_CUTH_Mode))
    {//cut paper
        Prn_task16_CutPaper();
    }
    else if (BIT(Prn_Mode,PRN_CUTRESET_Mode))
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

        if (Prn_LineChars)
            Prn_CalcHotTime();
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

            for(i=0;i<DOTSBUFF;i++)
            {
                Set_Hot_Data( *linep++ , i );
            }
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

    if (Prn_LineChars)
        Prn_CalcHotTime();

	Set_Motor_Speed( Prn_MotorTime );

	if(Prn_LineIndex<TEXT_HIGH)
	{
		Prn_TaskNow = TASK05_FeedHalf;
        Prn_TaskNext = TASK02_PrintChar;

        if (BIT(Prn_Mode,PRN_DBLH_Mode))
		{ //double high
			if(Prn_Status.g_bDoublHight)
			{
				Prn_Status.g_bDoublHight = false;
				return;//print current dots twice
			}
			Prn_Status.g_bDoublHight = true;
		}
        if (Prn_LineChars)
        {
            Create384Dots();
            linep = DotsOfCurrent;

            for(i=0;i<DOTSBUFF;i++)
            {
                Set_Hot_Data( *linep++ , i );
            }

            Start_Hot();
        }

		Prn_LineIndex++;
	}
	else
	{
#if CHINESE
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
		PoolPopOut = PoolCurrent;
	}
}
// Print graphic====================================
void Prn_task03_PrintGraphic()
{
	Move_Motor();

    Prn_CalcHotTime();
	Set_Motor_Speed( Prn_MotorTime );

	if(!Prn_Status.g_bGraphicOver)
	{
		if(Prn_GraphTextMax)
			GraphicDotsWithText();
		else
			CreateGraphicDots();
		Prn_TaskNow = TASK05_FeedHalf;
        Prn_TaskNext = TASK03_PrintGraphic;
	}
	else
	{
#if CHINESE
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
		PoolPopOut = PoolCurrent;
	}
}
//feed paper=======================================
void Prn_task04_FeedPaper()
{
	int i;
    BYTE *linep;

    Move_Motor();
	if(Prn_BlankAppend)
    {
        Set_Motor_Speed( Prn_MotorTime );
		Prn_BlankAppend--;
    }
	else
	{
		GetTaskFromPool();
        Prn_CalcHotTime();
        Set_Motor_Speed( Prn_MotorTime );

		if(Prn_Status.g_bPrintOver)//所有数据打印完毕
		{
			Stop_Motor();//关闭motor
		}
#if ENCUTTER
        else  if (BIT(Prn_Mode, PRN_CUTF_Mode | PRN_CUTH_Mode | PRN_CUTRESET_Mode))
        {//cut paper
            Prn_TaskNow = 0xff;
            Start_Motor(Prn_Mode);
        }
#endif
        else
		{
			if(Prn_Status.g_bPrintGraph)//打印图片
			{
				Prn_TaskNow = TASK05_FeedHalf;
                Prn_TaskNext = TASK03_PrintGraphic;
			}
            else if (Prn_LineChars)
            {
                Create384Dots();
                linep = DotsOfCurrent;
                for(i=0;i<DOTSBUFF;i++)
                {
                    Set_Hot_Data( *linep++ , i );
                }

                Start_Hot();

    			Prn_LineIndex=1;
    			Prn_TaskNow = TASK05_FeedHalf;
                Prn_TaskNext = TASK02_PrintChar;
            }
            else
            {
                Prn_BlankAppend = (LINE_BLANK + TEXT_HIGH) * STEPS1LINE;
                PoolPopOut = PoolCurrent;
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

#if (STEPS1LINE>2)
    StepS++;
    if(StepS<STEPS1LINE)
        return;
    StepS = 1;
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
        PoolPopOut = PoolCurrent;// 当前切纸任务处理完毕

        LPC_RIT->RICTRL &= ~RIT_CTRL_TEN;			 //disable counter
        if (PoolPushIn != PoolPopOut)
        {//A new task wait for service
            PoolCurrent = (PoolPopOut + 1);
            if (PoolCurrent>=MAXOFPOOL) PoolCurrent = 0;
            Prn_TaskNow = 0xff;
            Start_Motor(PoolForPrint[PoolCurrent].Prn_Mode);
        }
        else
        {// not any task in the pool
            Prn_TaskNow = 0xff;
        }
    }
    else if (BIT(Prn_Mode,PRN_CUTRESET_Mode))
    {// 停止切纸电机后,改变切纸电机的走向(反向.)
        Set_Motor_Speed( TIMESTART );
        Cut_Status.g_bReverse = 1;
        Prn_TaskNow = TASK15_FeedStepAlpha;
        Prn_TaskNext = TASK15_FeedStepAlpha;
        CutMotorMax = CUTSTEPALPHA;
    }
    else //if (BIT(Prn_Mode,PRN_CUTF_Mode | PRN_CUTH_Mode))
    {// 停止切纸电机后,改变切纸电机的走向(反向.),继续切纸任务
        Set_Motor_Speed( TIMESTART );
        Cut_Status.g_bReverse = 1;
        Prn_TaskNow = TASK18_FeedStepC;
        Prn_TaskNext = TASK18_FeedStepC;
        if (BIT(Prn_Mode,PRN_CUTF_Mode))
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
            if (BIT(Prn_Mode,PRN_CUTRESET_Mode))
            {//task for reset cutter
                Set_Motor_Speed( TIMESTOP );//stop motor first
                Cut_Status.g_bStop = 0;
                Prn_TaskNow = TASK10_StopCutter;//stop cut motor first
            }
            else
            {//task for cut paper

                if (BIT(Prn_Mode,PRN_CUTF_Mode))
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
        Prn_Jphase = 0;
        Prn_AccStep = 0;
        Prn_TaskNow = TASK18_FeedStepC;
        Prn_FeedCount=0;
    }

}

#endif
//////////////////////////////////////////////////////////
void Wait_Paper()
{
    Hot_Off();
    Drive_Off();//     stop print motor
    Set_Motor_Speed( TIMESTART );
    Prn_TaskNow = 0xff;

    Prn_Jphase = 0;
    Prn_AccStep = 0;

    Prn_LineIndex = 0;
	Prn_BlankAppend = LINE_BLANK*STEPS1LINE;
	Prn_Status.g_bPrintGraph = false;
    Prn_Status.g_bWaitPaper = true;

}
/*================================ main for print===============================*/
#if EN_PRINT
void RIT_IRQHandler(void)
#else
void timer2_interrupt()
#endif
{

#if DEBUGONPC==0 //testonly>>>>>>>>>>>>>>>>>>>>
    static BYTE onOff=0;

    RIT_GetIntStatus(LPC_RIT); //call this to clear interrupt flag

    if (onOff==128)
    {
            LightLED(2,1);
//            onOff = 0;
    } else if (onOff==0)
    {
        LightLED(2,0);
//        onOff = 1;
    }
    onOff++;
#endif//<<<<<<<<<<<<<<<<<<<<<<<<<<<


#if ENCUTTER
    // 切纸工作模式下,不检测无纸等状态
    if (!BIT(Prn_Mode,PRN_CUTRESET_Mode | PRN_CUTF_Mode | PRN_CUTH_Mode))
#endif
    {
        Check_Print();
        if(Prn_Status.g_bTOOHOT)
        {//
            if(!Prn_Status.g_bSuspend)
            {//打印机过热,暂停打印
                Suspend_Motor();
            }
            else if (Prn_Delay==0)
            {//打印机暂停结束
                Restart_Motor();
            }
            return;//打印机过热
        }
#if RHUP
        if(Prn_Status.g_bHANDUP)
        {
            if (!Prn_Status.g_bWaitPaper)
                Wait_Paper();
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
                Start_Motor(Prn_Mode);
                return; //打印机错误处理完毕,需要重新启动打印
            }
        }
#endif
        //检测是否有纸
        if(Prn_Status.g_bNOPAPER)
        {//无纸
            if (!Prn_Status.g_bWaitPaper)
                Wait_Paper();
            return; //没有装纸
        }
        else
        {//有纸
            if (Prn_Status.g_bPaperIn)
            {//缺纸后重新插入了打印纸,重新起动打印
                Prn_FeedCount = Prn_Delay = 0;
                Prn_Status.g_bPaperIn = false;
                Prn_Status.g_bHANDDN = false;
                Prn_Status.g_bHANDUP = false;
                Prn_Status.g_bWaitPaper = false;

                Prn_TaskNow = TASK04_FeedPaper;//启用4号任务,先走纸
                Prn_BlankAppend = (LINE_BLANK + TEXT_HIGH) * STEPS1LINE * 10;//走十行空白
                Drive_Motor(PHSTART);//out
                Set_Motor_Speed( TIMESTART );
                Prn_Jphase = 0;
                Prn_AccStep = 0;
                //LPC_RIT->RICTRL |= RIT_CTRL_TEN;//Enable counter
                return;
            }
        }
    }

    //打印机无异常
    if(!Prn_Status.g_bPrintOver)//It's have data for print
    {

#if ENCUTTER
        if (BIT(Prn_Mode,PRN_CUTRESET_Mode | PRN_CUTF_Mode | PRN_CUTH_Mode) && Prn_TaskNow!=TASK19_RevFeed8Line)
        {
            if(Prn_AccStep<CUTMAXSPEED)
                Prn_MotorTime = TAB_CUTTIME[Prn_AccStep];
        }
        else
#endif
        {
            if(Prn_AccStep<VMAXSPEED)
                Prn_MotorTime = TAB_PRTIME[Prn_AccStep];
        }
    }
    Prn_AccStep++;

#if DEBUGONPC==0
    if (Prn_TaskNow<6 && Prn_TaskNow>1)
    {
        //UARTSend(0,Prn_TaskNow);//testonly
    }
#endif
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
* Function Name:        TIMER0_IRQHandler
* Description:          TIMER0 中断处理函数,用于加热控制
*********************************************************************************************************/
void TIMER0_IRQHandler (void)
{

   LPC_TIM0->IR = 0x01;                    /* 清除中断标志,以允许其它中断  */

   Prn_HotIndex++;
   Prn_HotIndex &= 0x03;

   Hot_Off();

#if 0//testonly
   switch (Prn_HotIndex)
   {
       case 0:return;//加热结束
       case 1:Hot_On(HOT2);  break;
       case 2:Hot_On(HOT3);  break;
       case 3:Hot_On(HOT4);  break;
   }

//        LPC_TIM0->MR0  = SystemCoreClock/2;         /* 2秒中断1次 */
   LPC_TIM0->TCR  = 0x01;                          /* 启动定时器                   */
#endif
}

/*********************************************************************************************************
** Function name:      timer0Init
** Descriptions:      定时器0初始化程序
*********************************************************************************************************/

void Timer0_Initialize (void)
{

    LPC_TIM0->TCR  = 0x02;
    LPC_TIM0->IR   = 1;
    LPC_TIM0->CTCR = 0;
    LPC_TIM0->TC   = 0;
    LPC_TIM0->PR   = 0;
    LPC_TIM0->MR0  = SystemCoreClock/4/2;           /* 0.5S中断1次                  */
    LPC_TIM0->MCR  = 0x07;                          /* 匹配后产生中断,并停止计数               */

    NVIC_EnableIRQ(TIMER0_IRQn);                    /* 设置中断并使能               */
    NVIC_SetPriority(TIMER0_IRQn, 3);
//    LPC_TIM0->TCR  = 0x01;                          /* 启动定时器                   */

}
#endif

