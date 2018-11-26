#define EXTM_C 1
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "message.h"
//==============================================================

const BCD ZERO       =    {0,{0,0,0,0,0,0,0,0}};
const BCD ONE        = {0,{1,0,0,0,0,0,0,0}};
const BCD TEN        = {0,{0x10,0,0,0,0,0,0,0}};
const BCD HUNDRED    = {0,{0,0x01,0,0,0,0,0,0}};
const BCD THOUSAND   = {0,{0,0x10,0,0,0,0,0,0}};
const BCD THOUSAND10 = {0,{0,0,0x01,0,0,0,0,0}};
const BCD THOUSAND100= {0,{0,0,0x10,0,0,0,0,0}};
const BCD MILLION    = {0,{0,0,0,0x01,0,0,0,0}};
const BCD MILLION10  = {0,{0,0,0,0x10,0,0,0,0}};
const BCD MAXWORD    = {0,{0x35,0x55,0x06,0,0,0,0,0}};
const BCD MAXBYTE    = {0,{0x55,0x02,0,0,0,0,0,0}};
const BCD MAXLONG    = {0,{0x48,0x36,0x48,0x47,0x21,0,0,0}};

BYTE Prefix1,Prefix2;

BYTE ArrowsRight;

UnLong RamOffSet;


#if (PC_EMUKEY)

#include "FEcrTest.h"
#if defined(DEBUGBYPC)
#include "FEcrTest.cpp"
#else
#include "FEcrTest.c"
#endif
#endif

void EmptyComm(BYTE port);
void SendComm(BYTE port,BYTE byte);

extern short Bios_PortRead(BYTE NumCom, void *Dest, short NumBytes, ULONG TimeOut, WORD *Status);

//calculate the keys of DEPT&PLU on the current KeyTable
void DeptPluKeysCount()
{
    short i;

    ApplVar.DeptKeys = ApplVar.PluKeys = 0;
#if (PLUCODE_X==1)
    PLU_Key_ID = MAXKEYB;
    DEPT_Key_ID = PLU_Key_ID+1;
#endif
    for (i=0;i<MAXKEYB-1;i++)
    {
#if (PLUCODE_X==1)
        if (ApplVar.AP.KeyTable[i]==PLU1)
            PLU_Key_ID=i;// PLU1 在键盘中的位置,=0xff时,键盘上无此功能键 //
        else if (ApplVar.AP.KeyTable[i]==DEPT)
            DEPT_Key_ID=i;// DEPT 在键盘中的位置,=0xff时,键盘上无此功能键 //
        else
#endif
        if (ApplVar.AP.KeyTable[i]>=DEPT+1 && ApplVar.AP.KeyTable[i]<PLU1)
            ApplVar.DeptKeys++;
        else if (ApplVar.AP.KeyTable[i]>=PLU1+1 && ApplVar.AP.KeyTable[i]<PLU3)
            ApplVar.PluKeys++;
    }
}

//Test if BARCode reader is online or not?
BYTE CheckOnLine(BYTE port)
{
#if !defined(DEBUGBYPC)
    switch (port)
    {
    case 0:
    case 1:
    default:
        return 1;
    }
#endif
}

void PrintDotEpson()
{
    RJPrint(0,"Print Dot Epson");
}

//if pBCD==0 return false else return true
//
short CheckNotZero(BCD *pBCD)
{
    register short sLp;

    for (sLp=0;sLp<BCDLEN;sLp++)
        if (pBCD->Value[sLp] != 0)
            return 1;
    return 0;
}
//
void WaitOnLine(BYTE port)
{
    char rBuf[1];

    Bios_PortRead(port+1, rBuf, 0, 1000/5, NULL);//time out=1's
}



short RFeed(short line)
{
    short sLp;

    for (sLp=0;sLp<line;sLp++)
        OutPrint(CMDP_LFR, NULL);                               //Line Feed standard

}

short JFeed()
{
    OutPrint(CMDP_LFR, NULL);                               //Line Feed standard
}

//==================================================================
//port=1,2,3....
BYTE TestCom123(BYTE port)
{
    BYTE    tChar;

    port--;

    EmptyComm(port);
    tChar=0x00;
    while (tChar<0x80)
    {
        SendComm(port,tChar);
        if (ReadComm(port)!=tChar)
            return 1;
        tChar++;
    }
    return 0;

}

//port=0,1,2...
WORD CheckComm(BYTE port)
{
#if defined(DEBUGBYPC)
    return Bios_PortRead(port+1, NULL, 0, 0, NULL);
#else
    return UARTGet(port+1,NULL,NULL);//Bios_PortRead(port+1, NULL, 0, 0, NULL);
#endif
}

//port=0,1,2...
void EmptyComm(BYTE port)
{
    if (Bios_PortRead(port+1, NULL, 0, 0, NULL))
        Bios_PortRead(port+1, NULL, -1, 0, NULL);
    return;
}

/*  When CheckComm return SOH, system can read data from rs232 use readcomm.
    the first byte is the length of data-L, then are the Byte data-B;
    the last is verify byte-V;
    (L + B)=V

*/
//Read one byte from port
//in: port=0,1,2
//out:(receive data)
// return -1 if no data
short ReadComm(BYTE port)
{
    char rBuf[1];
    WORD sSta;

    sSta = 0;
#if (defined(CASE_INDONESIA) || defined(CASE_INDIA))//yahyakammukutty
    if (!Bios_PortRead(port+1, rBuf, 1, 1200, &sSta) ||   //印度机器需要加长延迟时间
        (sSta & (1<<B_SIOF_OVERRUN | 1<<B_SIOF_FRAME | 1<<B_SIOF_PARITY | 1<<B_SIOF_ERRORS)))
#else
    if (!Bios_PortRead(port+1, rBuf, 1, ASECOND/2, &sSta) ||
        (sSta & (1<<B_SIOF_OVERRUN | 1<<B_SIOF_FRAME | 1<<B_SIOF_PARITY | 1<<B_SIOF_ERRORS)))
#endif
        return -1;
    else
        return(rBuf[0] & 0xff);
}

/********************************************************************
 * 向指定串口发送字节数据
 *
 * @author EutronSoftware (2017-08-31)
 *
 * @param port :=0,1,2
 * @param byte
 *******************************************************************/
void SendComm(BYTE port,BYTE byte)
{
    Bios_PortWrite(port+1, &byte, 1, 1);
}
//Speed:'1'-1200,'2'-2400,'3'-4800,'4'-9600,'5'-19200,'6'-38400,'7'-57600,'8'-115200
//Parity: '0'-NONE,'1' = EVEN,'2'-ODD
//StopBit: '1'= 1 stop bit;'2'-2 stop bit
//BitCar: '8' = 8 bit per carattere;'7'-7 bit per carattere
//Com=0,1,2...
CONST ULONG Baud[]={1200,2400,4800,9600,19200,38400,57600,115200};

void SetComm(BYTE Com)
{
    BYTE pSet = ApplVar.PortList[Com].Prot[ApplVar.PortList[Com].Type - 0x31];
#if defined(DEBUGBYPC)
//    if (Com==0)
//会减慢程序的运行        SetupCom232(Baud[pSet & 0x07], ((pSet>>6)&1) + 7,((pSet>>5)&1) + 1, ((pSet>>3)&0x03),10);
#else
#if defined(CASE_INNERDOTPRN)
    if (Com!=DOTPRNPORT)//testonly
#endif
        UARTInit(Com+1,
             Baud[pSet & 0x07], //Speed:'1'-1200,'2'-2400,'3'-4800,'4'-9600,'5'-19200,'6'-38400,'7'-57600,'8'-115200
             ((pSet>>6)&1) + 7, //BitCar: '8' = 8 bit per carattere;'7'-7 bit per carattere
             ((pSet>>3)&0x03),  //Parity: '0'-NONE,'1' = EVEN,'2'-ODD
             ((pSet>>5)&1) + 1);//StopBit: '1'= 1 stop bit;'2'-2 stop bit

#endif
}

//
BYTE TestRtc()
{
    BYTE sERR;

    Bios_1(BiosCmd_GetDate);
    Bios_1(BiosCmd_GetTime);

    sERR = 0;
    if (rtc_date.dd<1 || rtc_date.dd>0x31)
    {
        rtc_date.dd = 1;
        sERR = 1;
    }
    if (rtc_date.mm<1 || rtc_date.mm>0x12)
    {
        rtc_date.mm = 1;
        sERR = 1;
    }
    if (rtc_date.w>6)
    {
        rtc_date.w = 1;
        sERR = 1;
    }

    if (rtc_time.hh>0x23)
    {
        rtc_time.hh=1;
        sERR = 1;
    }
    if (rtc_time.mm>0x59)
    {
        rtc_time.mm = 1;
        sERR = 01;
    }
    if (rtc_time.ss>0x59)
    {
        rtc_time.ss = 1;
        sERR = 01;
    }

    if (sERR)
    {
//		rtc_date.yy = 0;
        Bios_1(BiosCmd_SetDate);
        Bios_1(BiosCmd_SetTime);
    }
    return(sERR);
}
//pDateTime=0,set date;=1,set time;=2,Set date and time
void SetTimeDate(BYTE pDateTime,struct TimeDate *pTimeDate)
{
    switch (pDateTime)
    {
    case 2:
    case 0:
        rtc_date.dd = pTimeDate->day;
        rtc_date.mm = pTimeDate->month;
        rtc_date.yy = pTimeDate->year;
        rtc_date.w = pTimeDate->dow =  GetWeekDay(pTimeDate->year,pTimeDate->month,pTimeDate->day);

        Bios_1(BiosCmd_SetDate);
        if (pDateTime==0) break;
    case 1:
        if (TESTBIT(AM_PM,BIT0)) //cc 20070730
            rtc_time.hh = (pTimeDate->am_pm * 0x12) + pTimeDate->hour;
        else
            rtc_time.hh = pTimeDate->hour;
        rtc_time.mm = pTimeDate->min;
        rtc_time.ss = pTimeDate->sec;
        Bios_1(BiosCmd_SetTime);
        break;
    }
}

//1-星期一,...,6-星期六,7-星期日
void GetTimeDate(struct TimeDate *pTimeDate)
{

    Bios_1(BiosCmd_GetDate);

    pTimeDate->day  =   rtc_date.dd;
    pTimeDate->month=   rtc_date.mm;
    pTimeDate->year =   rtc_date.yy+YEARFrHEX;
    pTimeDate->dow  =   rtc_date.w;

    Bios_1(BiosCmd_GetTime);
    if (TESTBIT(AM_PM,BIT0)) //cc 20070730
    {
        pTimeDate->am_pm = (rtc_time.hh / 0x12);
        pTimeDate->hour = (rtc_time.hh % 0x12);
    }
    else
        pTimeDate->hour=rtc_time.hh ;
    pTimeDate->min =rtc_time.mm ;
    pTimeDate->sec =rtc_time.ss;
}

WORD BCD4toWORD(WORD R2R0)
{
    WORD tmp=(R2R0>>8),tmp2=(R2R0 & 0xff);
    tmp = BCDtoDEC(tmp)*100;
    return(tmp + BCDtoDEC(tmp2));
}

short BCDWidth(BCD *bcd)
{
    short i;

    i = BCDLEN-1;
    while (i>=0)
    {
        if (bcd->Value[i] !=0)
            break;
        i--;
    }
    i++;
    if (i>0)
    {
        if (bcd->Value[i-1]<0x0f)
            i = (i <<1) - 1;
        else
            i<<=1;
    }
    if (TESTBIT(bcd->Sign,BIT7))//ccr2016-08-03 为负数
        i++;
    return i;
}

//
void SetBCDPoint(BCD *bcd,BYTE point)
{
    short   sLp;

    sLp = bcd->Sign & 0x07;
    if (sLp>point)
    {
        sLp -= point;
        while (sLp)
        {
            BcdDiv10(bcd);
            sLp--;
        }
    }
    else if (sLp<point)
    {
        sLp = point-sLp;
        while (sLp)
        {
            BcdMul10(bcd);
            sLp--;
        }
    }
    bcd->Sign &= 0xf8;
    bcd->Sign |= (point & 0x07);
}
//将当前日期,时间复制到sBuf中
//放回日期字符串长度
// pTime:=0,由系统设置控制
//    =1,只转换日期
//    =2,只转换时间
//    =3,转换日期和时间
//pTime & 0xf0:=0,使用系统设置控制
//             >0,禁止使用系统设置控制
int DateTimeToStr(char *sBuf,BYTE pTime)
{
    int sLen=0;
    if ((pTime & 1) || ((!TESTBIT(SLIPINFO, BIT1) || !TESTBIT(COMMA, BIT6)) && !(pTime & 0xf0)))        /* print date on slip */
    {
//012345678901234567890123456789012345
/*2003yy08mm09dd 12:12:12 */
        CheckTime(TRUE);
#if defined(CASE_FORHANZI)
        switch (TIME_DATE)
        {
        case 1:/*"Mon,MM-DD-YYYY" */
            memcpy(sBuf,DateAsci+10,4);
            sBuf[6]=DateAsci[4];sBuf[7]=DateAsci[5];
            sBuf[10]=DateAsci[7];sBuf[11]=DateAsci[8];
            break;
        case 2:/*"Mon,YYYY-MM-DD" */
            memcpy(sBuf,DateAsci+4,4);
            sBuf[6]=DateAsci[9];sBuf[7]=DateAsci[10];
            sBuf[10]=DateAsci[12];sBuf[11]=DateAsci[13];
            break;
        default:/*"Mon,DD-MM-YYYY" */
            memcpy(sBuf,DateAsci+10,4);
            sBuf[6]=DateAsci[7];sBuf[7]=DateAsci[8];
            sBuf[10]=DateAsci[4];sBuf[11]=DateAsci[5];
            break;
        }
        CopyFrStr(sBuf+4,Msg[NIAN].str);
        CopyFrStr(sBuf+8,Msg[YUE].str);
        CopyFrStr(sBuf+12,Msg[RI].str);
        sLen = 12+strlen(Msg[RI].str);
#else
        memcpy(sBuf,DateAsci+4,10);
        sLen = 10;
#endif
    }
    if ((pTime & 2) || ((!TESTBIT(SLIPINFO, BIT2)||!TESTBIT(COMMA, BIT7)) && !(pTime & 0xf0)))// && !ApplVar.FNoTime)        /* print time on slip */
    {
        sBuf[sLen++] = ' ';
        memcpy(sBuf + sLen, TimeAsci, 8);
        sLen += 8;
    }
    return sLen;
}

/**
 *
 * 将BCD日期年月日压缩到WORD中
 * @author EutronSoftware (2016-11-30)
 *
 * @param sYear
 * @param sMonth
 * @param sDay
 *
 * @return WORD
 * bitNum:fedcba9876543210
 * result:0000000000000000
          |     ||  |+---+----ApplVar.Day(1..31)
          |     |+--+---------ApplVar.Month(1..12)
          +-----+-------------Year(0..99)2000..2099
  */
WORD EncordDate(WORD sYear,BYTE sMonth,BYTE sDay)
{
    return(((((sYear & 0xf0)>>4)*10 + (sYear & 0x0f))<<9) |
           ((((sMonth & 0xf0)>>4)*10 + (sMonth & 0x0f))<<5) |
           ((((sDay & 0xf0)>>4)*10 + (sDay & 0x0f))));
}
/**
 * 从WORD型大压缩日期中解析年月日
 *
 * @author EutronSoftware (2016-11-30)
 *
 * @param sDate
 * @param sYear
 * @param sMonth
 * @param sDay
 */
void DecordDate(WORD sDate,WORD *sYear,BYTE *sMonth,BYTE *sDay)
{
    WORD s;
    s = sDate>>9;
    *sYear = YEARFrHEX + ((s/10)<<4) + s % 10;

    s = (sDate>>5) & 0x0f;
    *sMonth = ((s/10)<<4) + s % 10;

    s = sDate & 0x1f;
    *sDay = ((s/10)<<4) + s % 10;
}

/******************************************************
//  大小月天数处理  //
//  闰年二月的天数处理 //
******************************************************/
BYTE  GetMonthMaxDay(BYTE  tyear,BYTE  tmonth)
{
		short iyear;

		switch (tmonth){
		case 2:
			{
	         iyear=2000+(tyear/16)*10+tyear%16;
    	     if((!(iyear%4) && (iyear%100)) || !(iyear%400))
              return 0x29;
           else
              return 0x28;
			}
		case 4:
		case 6:
		case 9:
		case 11:
			return 0x30;
		default:
			return 0x31;
		}
}
/**
 * 检查压缩日期的正确性
 *
 * @author EutronSoftware (2016-11-01)
 *
 * @param sDate
 *
 * @return BYTE
 */
BYTE CheckBCDDate(BYTE *sDate)
{
    BYTE sYear,sMonth,sDay;

    sYear = BCDtoDEC(sDate[2]);
    sMonth = BCDtoDEC(sDate[1]);
    sDay = BCDtoDEC(sDate[0]);

    return (sMonth && sMonth<=12 && sDay && sDay <= GetMonthMaxDay(sYear,sMonth));
}

/**
 * 将压缩格式的日期转换为字符串
 *
 * @author EutronSoftware (2016-05-17)
 *
 * @param sDate
 * @param strDate
 */
void DeDateToStr(char *strDate,WORD sDate)
{
    WORD sYear,sMonth,sDay;

    sYear = YEARFrDEC + (sDate>>9);
    sMonth = (sDate>>5) & 0x0f;
    sDay = sDate & 0x1f;


    switch (TIME_DATE)
    {
    case 1:/*"Mon,MM-DD-YYYY" */
        sprintf(strDate,"%02d-%02d-%4d",sMonth,sDay,sYear);
        break;
    case 2:/*"Mon,YYYY-MM-DD" */
        sprintf(strDate,"%4d-%02d-%02d",sYear,sMonth,sDay);
        break;
    default:/*"Mon,DD-MM-YYYY" */
        sprintf(strDate,"%02d-%02d-%4d",sDay,sMonth,sYear);
        break;
    }
}
/**
 * 将指定的年月日时分秒转换为字符串
 *
 * @author EutronSoftware (2016-11-01)
 *
 * @param strDate
 * @param FYear:为BCD格式的日期和时间
 * @param FMonth
 * @param FDay
 * @param FHour
 * @param FMinute
 * @param FSecond
 */
void BCDDateTimeToStr(char *strDate,BYTE FYear,BYTE FMonth,BYTE FDay,BYTE FHour,BYTE FMinute,BYTE FSecond)
{

    switch (TIME_DATE)
    {
    case 1:/*"Mon,MM-DD-YYYY" */
        sprintf(strDate,"%02x-%02x-20%02x %02x:%02x:%02x",FMonth,FDay,FYear,FHour,FMinute,FSecond);
        break;
    case 2:/*"Mon,YYYY-MM-DD" */
#if (1)//!defined(CASE_FORHANZI)
        sprintf(strDate,"20%02x-%02x-%02x %02x:%02x:%02x",FYear,FMonth,FDay,FHour,FMinute,FSecond);
#else
        sprintf(strDate,"20%02x%s%02x%s%02x%s %02x:%02x:%02x",FYear,Msg[NIAN].str,FMonth,Msg[YUE].str,FDay,Msg[RI].str,FHour,FMinute,FSecond);
#endif
        break;
    default:/*"Mon,DD-MM-YYYY" */
        sprintf(strDate,"%02x-%02x-20%02x %02x:%02x:%02x",FDay,FMonth,FYear,FHour,FMinute,FSecond);
        break;
    }

}

/**
 * 将按指定格式输入的日期转换为固定的YYMMDD格式
 *
 * @author EutronSoftware (2016-11-01)
 *
 * @param Value :输入的日期
 *
 * @return ULONG :输出日期
 */
ULONG PackageDate(BYTE *Value)
{
    switch (TIME_DATE)
    {
    case 1:     //wMMDDYYYYHH//
        return((((UnLong)Value[0])<<16) +     //YY
                ((UnLong)Value[2]<<8) +      //MM
                ((UnLong)Value[1]));       //DD
        break;
    case 2:     //wyyYYMMDDHH//
        return (((UnLong)Value[2])<<16) +     //YY
                ((UnLong)Value[1]<<8) +      //MM
                ((UnLong)Value[0]);       //DD
        break;
    default:         //wDDMMYYYYHH//
        return (((UnLong)Value[0])<<16) +     //YY
                ((UnLong)Value[1]<<8) +      //MM
                ((UnLong)Value[2]);       //DD

        break;
    }

}
//将金额数据转换为无小数形式存放(即去掉Sign中的小数位数),
//没有输入小数时,金额数据默认为两位小数
BYTE ChangePoint()//ccr2016-04-05
{

    switch (NO_DECIMAL)//ccr2016-11-07>>>>>>>>>>>>>>>>
    {
    case 0://系统为两位小数
        {
            switch (ApplVar.DecimalPoint)
            {
            case 0:BcdMul100(&ApplVar.Entry); break;//没有输入小数点
            case 1:BcdMul10(&ApplVar.Entry);break;//输入了一位小数
            case 2:break;//输入了两位小数
            case 3:BcdDiv10(&ApplVar.Entry); break;//输入了三位小数
            default:return 0;
            }
        }
        break;
    case 1://系统为无小数
        {
            switch (ApplVar.DecimalPoint)
            {
            case 0:break;//没有输入小数点
            case 1:BcdDiv10(&ApplVar.Entry);break;//输入了一位小数
            case 3:BcdDiv10(&ApplVar.Entry); //输入了三位小数
            case 2:BcdDiv100(&ApplVar.Entry);break;//输入了两位小数
            default:return 0;
            }
        }
        break;
    case 2://系统为三位小数
        {
            switch (ApplVar.DecimalPoint)
            {
            case 0:BcdMul10(&ApplVar.Entry); //没有输入小数点
            case 1:BcdMul100(&ApplVar.Entry);break;//输入了一位小数
            case 2:BcdMul10(&ApplVar.Entry);break;//输入了两位小数
            case 3:break;//输入了三位小数
            default:return 0;
            }
        }
        break;
    case 3://系统为1位小数
        {
            switch (ApplVar.DecimalPoint)
            {
            case 0:BcdMul10(&ApplVar.Entry); break;//没有输入小数点
            case 1:break;//输入了一位小数
            case 2:BcdDiv10(&ApplVar.Entry);break;//输入了两位小数
            case 3:BcdDiv100(&ApplVar.Entry);break;//输入了三位小数
            default:return 0;
            }
        }
        break;
    default:return 0;
    }//ccr2016-11-07<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    ApplVar.Entry.Sign &= 0xf8;
    ApplVar.DecimalPoint = 0;
    return 1;
}


//将金额数据转换为无小数形式存放(即去掉Sign中的小数位数),
//没有输入小数时,金额数据默认为两位小数
BYTE ChangePoint2()//ccr2016-04-05
{
    switch (ApplVar.DecimalPoint)
    {
    case 0:BcdMul100(&ApplVar.Entry); break;//没有输入小数点
    case 1:BcdMul10(&ApplVar.Entry);break;//输入了一位小数
    case 2:break;//输入了两位小数
    case 3:BcdDiv10(&ApplVar.Entry); break;//输入了三位小数
    default:return 0;
    }
    ApplVar.Entry.Sign = 2;
    ApplVar.DecimalPoint = 0;
    return 1;
}

#define PWDFLAG     '.'     //表示处在输入密码
#define PWD_X       ((SCREENWD-MAXPWD-1)/2)
/**
 * 验证密码,以窗口方式显示密码验证输入
 *
 * @author EutronSoftware (2016-02-15)
 *
 * @param pass
 *
 * @return BYTE:=true,密码验证正确;=false,密码验证失败
 */
BYTE CheckPWD(const char* PWDType,char *pass)
{
    short pwdY,kCode,sCounter;
    BYTE keyno;
    char sPass[SCREENWD+1];

    if (pass[0]==0)
        return true;

    pwdY = (SCREENLN-1-3)/2;
    mDrawABox(pwdY,pwdY+2);


    //在屏幕中央显示验证密码框
    if (PWDType)
        mDispStringXY((BYTE*)PWDType,CenterForDisplay((SCREENWD-2),strlen(PWDType))+1,pwdY);
    else
        mDispStringXY((BYTE*)Msg[MIMA].str,CenterForDisplay((SCREENWD-2),strlen(Msg[MIMA].str))+1,pwdY);


    pwdY++;//指向密码输入行

    memset(SysBuf,PWDFLAG,MAXPWD);
    sCounter = 0;
    SysBuf[MAXPWD-1] = 0;

    SetLoop();
    for (;;)
    {
        mDispStringXY(SysBuf,PWD_X,pwdY);

        while (!KbHit());
        keyno = Getch();
        kCode = ApplVar.AP.KeyTable[keyno];
        if (keyno == ApplVar.AP.FirmKeys[ID_CLEAR])
        {
            memset(SysBuf,PWDFLAG,MAXPWD);
            sCounter = 0;
            SysBuf[MAXPWD-1] = 0;
            continue;
        }
        if (keyno == ApplVar.AP.FirmKeys[ID_ENTER])
        {
            //恢复屏幕内容
            mDrawABox(0,0);//mRefreshLine(pwdY-1); mRefreshLine(pwdY++); mRefreshLine(pwdY);
            ResetLoop();
            pwdY = strlen(pass);
            return((pwdY==sCounter) && !strncmp(pass,sPass,sCounter));
        }
        if (keyno == ApplVar.AP.FirmKeys[ID_CANCEL])
        {
            //恢复屏幕内容
            mDrawABox(0,0);//mRefreshLine(pwdY-1); mRefreshLine(pwdY++); mRefreshLine(pwdY);
            ResetLoop();
            return FALSE;
        }
        if (kCode>='0' && kCode<='9' && sCounter<MAXPWD-1)
        {
            SysBuf[sCounter] = '*';
            sPass[sCounter++] = kCode & 0xff;
        }
    }
}
/**
 * 设置密码,以窗口方式显示密码验证输入
 *
 * @author EutronSoftware (2016-02-15)
 *
 * @param pass:存放设置的密码
 *
 * @return BYTE:=true,密码验证正确;=false,密码验证失败
 */
BYTE SetPassword(const char* PWDType,char *pass)
{
#define Pwd1st  SysBuf+MAXPWD+2

    short pwdY,sLen,kCode,sCounter,sCou1st,mmL,oldL,pwdLp;
    BYTE keyno;
    char sPass[SCREENWD+1];


    //在屏幕中央显示验证密码框
    pwdY = (SCREENLN-1-3)/2;
    mDrawABox(pwdY,pwdY+3);

    if (PWDType)
    {
        mDispStringXY((BYTE*)PWDType,CenterForDisplay((SCREENWD-2),strlen(PWDType))+1,pwdY);
    }
    else
    {
        mDispStringXY((BYTE*)Msg[MIMA].str,CenterForDisplay((SCREENWD-2),strlen(Msg[MIMA].str))+1,pwdY);
    }
    pwdY++;//指向密码输入行
    mDispStringXY((BYTE*)Msg[MIMA].str,1,pwdY);
    mDispStringXY((BYTE*)Msg[MMQREN].str,1,pwdY+1);
    mmL=strlen(Msg[MIMA].str)+1;

    SetLoop();

    sCounter = strlen(pass);
    if (sCounter>MAXPWD-1)
        sCounter=0;
    memset(SysBuf,PWDFLAG,MAXPWD);
    SysBuf[MAXPWD-1] = 0;
    memset(sPass,0,MAXPWD);
    if (sCounter)
    {
        memset(SysBuf,'+',sCounter);//表示有旧密码,必须先清除才能输入新密码
        strcpy(sPass,pass);
    }
    else
    {
        memset(Pwd1st,0,MAXPWD);
    }
    for (pwdLp=0;pwdLp<2;)
    {
        mDispStringXY(SysBuf,mmL,pwdY+pwdLp);

        while (!KbHit());
        keyno = Getch();
        kCode = ApplVar.AP.KeyTable[keyno];
        if (keyno == ApplVar.AP.FirmKeys[ID_CLEAR])
        {
            memset(SysBuf,PWDFLAG,MAXPWD);
            sCounter = 0;
            SysBuf[MAXPWD-1] = 0;
            continue;
        }
        if (keyno == ApplVar.AP.FirmKeys[ID_ENTER])
        {
            //恢复屏幕内容
            if (pwdLp==0)
            {//对密码进行确认
                if (sCounter)
                    strncpy(Pwd1st,sPass,sCounter);
                memset(SysBuf,PWDFLAG,MAXPWD);
                sCou1st=sCounter;
                sCounter = 0;
                SysBuf[MAXPWD-1] = 0;
                pwdLp++;
                mmL=strlen(Msg[MMQREN].str)+1;
                continue;
            }
            else
            {
                mDrawABox(0,0);//
//                mRefreshLine(pwdY-1); mRefreshLine(pwdY++); mRefreshLine(pwdY++);
#if (SCREENLN>4)
//                mRefreshLine(pwdY);
#endif
                ResetLoop();
                if (sCou1st==sCounter && (sCou1st==0 || !strncmp(Pwd1st,sPass,sCounter)))
                {//密码确认正确
                    if (sCounter)
                        memcpy(pass,Pwd1st,sCounter);

                    pass[sCounter]=0;
                    return true;
                }
                else
                    return false;
            }
        }
        else if (keyno == ApplVar.AP.FirmKeys[ID_CANCEL])
        {
            //恢复屏幕内容
            mDrawABox(0,0);//
//            mRefreshLine(pwdY-1); mRefreshLine(pwdY++); mRefreshLine(pwdY++);
#if (SCREENLN>4)
//             mRefreshLine(pwdY);
#endif
            ResetLoop();
            return true;//取消设置
        }
        else if (kCode>='0' && kCode<='9' && sCounter<MAXPWD-1)
        {
            SysBuf[sCounter] = '*';
            sPass[sCounter++] = kCode & 0xff;
        }
    }
}
//============================================================================
void DispSubTotal()
{
    char *strTotal;
    int subLen;

#if (SCRSTATDBL)
    DispOnStatDBL(FormatAmtStr(Msg[ZONGJI].str,&ApplVar.SaleAmt,SCRDBLWD),0);
#else
    DispSt0(Msg[XIAOJI].str);//显示小计 //RG-DISPLAY
    subLen = strlen(Msg[XIAOJI].str);
    strTotal = FormatAmtStr(0,&ApplVar.SaleAmt,SCREENWD);
    strTotal+=subLen;

    for (;subLen && *strTotal!=' ';subLen--) strTotal--;

    if (*strTotal==' ') {//剔除行首空格
        subLen++;
        strTotal++;
    }

//ccr2016-03-03    mSetInvAttr();
    DispStrXY(strTotal,subLen,STATELINE);//显示小计  //RG-DISPLAY
//ccr2016-03-03    mClearInvAttr();
#endif
//ccr2016-03-03    StrForST0 = (char*)Msg[SHURU].str;
}


// 显示16进制数据pHex,pWidth为pHex的字节数   //
void DisplayHex(char *pHex, short pWidth)
{
    char sDisp[30];

    memset(sDisp,' ',sizeof(sDisp));
    HEXtoASC(sDisp, pHex, pWidth);
    sDisp[pWidth*2] = 0;
    DispSt0C(sDisp);//ScreenFeed(1);
}
/**
 * 返回WORD数据的位数
 *
 * @author EutronSoftware (2016-03-25)
 *
 * @param pDec
 *
 * @return BYTE
 */
BYTE WidthofWORD(WORD pDec)
{
    if (pDec>9999)      return 5;
    else if (pDec>999)  return 4;
    else if (pDec>99)   return 3;
    else if (pDec>9)    return 2;
    else                return 1;
}
// 在(x,y)位置显示10进制数据pDec,pWidth为显示宽度  //
void DisplayDecXY(WORD pDec,int x,int y,int pWdith)
{
	char sDisp[9];
	int i;

    i = WidthofWORD(pDec);

	if (pWdith>=sizeof(sDisp))
        pWdith = sizeof(sDisp)-1;
    else if (pWdith<i)
        pWdith = i;
    memset(sDisp,' ',sizeof(sDisp));

	WORDtoASC(sDisp+pWdith-1,pDec);
	sDisp[pWdith] = 0;
	DispStrXY(sDisp,x,y);
}
// 在(x,y)位置显示10进制数据pDec,pWidth为显示宽度  //
void DisplayULongXY(ULONG pULong,int x,int y)
{
	char sDisp[12];
    memset(sDisp,' ',sizeof(sDisp));

	sDisp[ULongtoASCL(sDisp,pULong)] = 0;
	DispStrXY(sDisp,x,y);
}

//============================================================================
BYTE TestDencity()
{
    return(DENSITY & 0x80);
}
//============================================================================
short InitPrinterChar(BYTE *dots, short number)
{
}


/********************* Subroutines for BCD data *****************************/

BYTE   BCDValue1[BCDLEN*2+1];
BYTE   BCDValue2[BCDLEN*2+1];

BYTE    SaveBCDDec1,SaveBCDDec2,SaveBCDSign1;
BYTE    BCDSign1,BCDSign2,BCDDecimal1,BCDDecimal2;

BYTE   BcdLongFlag;
//**** BCD.Value/10=>BCD.Value **********
//size of BCD.Value must be 16 bytes
void  BCDValue_DIV_10(BYTE *res)
{
    int  i;
    BYTE tmp1,tmp2,tmp;

    tmp=0;
    res += 15;

    for (i=0;(i<16) && (*res==0);i++)
        res--;

    for (;i<16;i++)
    {
        tmp1=(*res &  0x0f ) << 4;
        tmp2=(*res &  0xf0)  >> 4;
        *res=tmp2+tmp;
        tmp=tmp1;
        res--;
    }
}

//*******BCD.Value * 10 =>BCD.Value *************
//size of BCD.Value must be 16 bytes
void  BCDValue_MUL_10( BYTE *res)
{
    int      i;
    BYTE tmp1,tmp2,tmp;

    tmp=0;
    for (i=0;i<16;i++)
    {
        tmp1=(*res &  0x0f)  << 4;
        tmp2=(*res &  0xf0)  >> 4;
        *res=tmp1+tmp;
        tmp=tmp2;
        res++;
    }
}

// aadjust BCD.Value: max 3 decimal/
void  Correct_BCDValue1(void)
{
    while (BCDDecimal1>3)
    {//只保留3位小数
        BCDValue_DIV_10(BCDValue1);
        BCDDecimal1--;
    }

    while ( (BCDDecimal1 > 0)  &&  ((BCDValue1[0] & 0x0f)  == 0) )
    {//去掉小数部分末尾的0
        BCDDecimal1--;
        BCDValue_DIV_10(BCDValue1);
    }


    while ( BCDDecimal1 > 0    &&  ((BCDValue1[8] & 0x0f)  != 0) )
    {//如果有小数,而且BCD的值超过16位(8 bytes),那么舍弃小数,只保留8位BCD的值
        BCDDecimal1--;
        BCDValue_DIV_10(BCDValue1);
    }
    BCDDecimal1 &=0x07;
    SaveBCDSign1 += BCDDecimal1;
}

// aadjust UINT64: max 3 decimal/
void  Correct_UINT64(UINT64 *pUInt64,BYTE *dec)
{
    while (*dec>3)
    {//只保留3位小数
        *pUInt64/=10;
        *dec--;
    }

    while ( (*dec > 0)  &&  (*pUInt64%10 == 0) )
    {//去掉小数部分末尾的0
    }


    while ( *dec > 0  &&  (*pUInt64>0xffffffff) )
    {//如果有小数,而且BCD的值超过16位(8 bytes),那么舍弃小数,只保留8位BCD的值
        *pUInt64/=10;
        *dec--;
    }
}


void SubBCDValue12(void)
{//BCDValue1=BCDValue1-BCDValue2
    short      i;
    BYTE tmp1,tmp2,tmp3,tmp,cfg;

    tmp=0;
    cfg=0;

    for (i=0;i<16;i++)
    {
#if (1)//借助十进制数来进行减法计算
        tmp1=BCDtoDEC(BCDValue1[i]);
        tmp2=BCDtoDEC(BCDValue2[i]);
        tmp2 += cfg;
        if (tmp1>=tmp2)
        {
            tmp=tmp1-tmp2;
            cfg=0;
        }
        else
        {
            tmp=tmp1 + 100 - tmp2;
            cfg=1;
        }
        BCDValue1[i]=DECtoBCD(tmp);
#else//直接对BCD数据进行减法计算
        tmp1=BCDValue1[i] & 0x0f;
        tmp2=BCDValue2[i] & 0x0f;
        tmp2 += cfg;
        if (tmp1>=tmp2)
        {
            tmp=tmp1-tmp2;
            cfg=0;
        }
        else
        {
            tmp=tmp1 + 10 - tmp2;
            cfg=1;
        }

        tmp1=(BCDValue1[i] & 0xf0) >> 4;
        tmp2=(BCDValue2[i] & 0xf0) >> 4;
        tmp2=tmp2+cfg;
        if (tmp1>=tmp2)
        {
            tmp3=tmp1-tmp2;
            cfg=0;
        }
        else
        {
            tmp3=tmp1 + 10 - tmp2;
            cfg=1;
        }

        tmp3 = tmp3 << 4;
        BCDValue1[i]=tmp  + tmp3;
#endif
    }
}


void AddBCDValue12(void)
{
    short      i;
    BYTE tmp1,tmp2,tmp3,tmp,cfg;

    tmp=0;
    cfg=0;

    for (i=0;i<16;i++)
    {
#if (1)//借助十进制数来进行加法计算
        tmp1=BCDtoDEC(BCDValue1[i]);
        tmp2=BCDtoDEC(BCDValue2[i]);
        tmp=tmp1+tmp2+cfg;
        if (tmp>99)
        {//有进位
            tmp -=100;
            cfg = 1;
        }
        else
            cfg=0;

        BCDValue1[i]=DECtoBCD(tmp);
#else//直接对BCD数据进行加法计算
        tmp1=BCDValue1[i] & 0x0f;
        tmp2=BCDValue2[i] & 0x0f;
        tmp=tmp1+tmp2+cfg;
        if (tmp>9)
        {//有进位
            tmp -=10;
            cfg = 1;
        }
        else
            cfg=0;

        tmp1=(BCDValue1[i] & 0xf0) >> 4;
        tmp2=(BCDValue2[i] & 0xf0) >> 4;
        tmp3=tmp1+tmp2+cfg;

        if (tmp3>9)
        {
            tmp3 -=10;
            cfg = 1;
        }
        else
            cfg=0;

        tmp3 = tmp3 << 4;
        BCDValue1[i]=tmp  + tmp3;
#endif
    }
}

/***************************************************************
"1234.56"=>{0x56,0x34,0x12}
 from--^  to-^
**************************************************************/
/**
 * 从字符串中转换BCD数据
 *
 * @author EutronSoftware (2014-12-18)
 *
 * @param to :BCD数据存储区
 * @param from :字符串('0'-'9','A'-'F'),从后向前开始转换
 * @param len :to所指BCD存储区的长度
 */
void    StrToBCDValue(BYTE *to, char *from, short len)
{
    BYTE      t1;
    BYTE    flag;//用来标示有效数据是否取完(是否遇到了空格)
    short      j,k;
    BYTE      *too,l;

    too=(BYTE  *)to;

    l=flag=0;
    for (j=k=0;j<len*2+k;j++)
    {
        if (*from=='.')
        {//剔除数据中的小数点
            from -- ;
            k++;//踢出小数点
            continue;
        }
        if (!flag)
        {//取有效数据
            t1=*from;
            from--;
        }
        if (flag || t1==' ')
        {//遇到空格后,有效数据已经转换完毕,BCD的剩余字节置0
            flag = 1;
            t1 = 0;
        }
        else
        {
            if (t1>='A')  t1 -= 7;
            t1 &= 0x0f;
        }
        if ((l & 1)==0)//得到1位BCD(4Bit)
            *too = t1;
        else
        {//得到2位BCD(8Bit)
            *too |= (t1<<4);
            too++;
        }
        l++;
    }
}

//------------------------------------------------------------------------------------------
//**WORD FormatBCD( char*  to, BCD *bcd, WORD Format)***
/* Convert BCD data to ascii string for display and printer*/
//将BCD中数据转换为字符串
//参数说明:to指向字符串存储区,转换后的数据由后向前存储
//"                 12345.67"
//                   <--to_^
// 返回转换后有效数据的长度
// 说明：该转换函数按format规定的格式将from所指的BCD数串的每位数转换成一个ASCII字符，
//       并将结果存于to指定的串中。
// 参数:
// Format:高8位为格式；低8位为ASCII码长度。
//  '-'是放在币种符号前还是后.bit7=1时,放在币种前面(-$12.55),bit7=0时,放在币种后面($-12.55)
//  '-'放到数字前面:bit8=0,'-'放到数字后面:Bit8=1
// 数字头部的'0'用SPACE取代:bit9=0,不用空格取代:bit9=1
//  默认为两位小数: bit15.bit11=00 (or 0x0000)
//          无小数：bit15.bit11=01 (or 0x0800);
//        三位小数: bit15.bit11=10 (or 0x8000);
//        一位小数：bit15.bit11=11 (or 0x8800);
//    无前缀（￥）：bit14=1 (or 0x4000);
//不对数据进行分割:  bit13=1 (or 0x2000);
//   数量打印(Qty)：bit10=1 (or 0x0400), 数量打印时,只用'.'作小数点,数据不分隔,小数点位数由BCD.Sign决定 ;
//用逗号代替小数点：bit12=1 (or 0x1000)
//当有小数时,bit12=1时,使用'.'做小数点,使用','分隔数据,bit12=0时,使用','做小数点,使用'.'分隔数据
//当无小数点时( bit15.bit11=01),bit12=1时,使用','分隔数据,bit12=0时,使用'.'分隔数据;
///////////////////////////////////////////////////////////////////////////////////////////
WORD FormatBCD( char*  to, BCD *bcd, WORD Format)
{
    short i;

    BYTE *q = to;
    BYTE *p = bcd->Value;

    WORD count;
    BYTE *WorkPtr = BCDValue1 + (2*BCDLEN)-1;
    BYTE index = 0;
    BYTE DecimalPartLen = 0;
    BYTE decimal;


    /* BCD To Asci*/
    for (i=0;i<BCDLEN;i++)
    {
        BCDValue1[2*(BCDLEN-1-i)+1] = ((*p)&0x0f)+'0';
        BCDValue1[2*(BCDLEN-1-i)] = ((*p)>>4)+'0';
        p++;
    }
    /* Prefix '0' ---->  ' '*/
    if (!TestBit(Format,9))
    {//将数据头部的0换为空格
        /* Get Integer Part Length*/
        if (TestBit(Format,10))
        {//为QTY
            count = 2*BCDLEN -1;//去掉小数点
            count = count - ((bcd->Sign)&0x0F);//去掉小数位数
        }
        else
        {//为AMT
            count = 2*BCDLEN -3;//去掉2位小数+小数点
            if (TestBit(Format,15))
            {
                if (TestBit(Format,11))
                    count++;
                else
                    count--;
            }
            else
            {
                if (TestBit(Format,11))
                    count+=2;
            }
        }
        /*Integer Part Prefix '0' --> ' '*/
        for (i=0;(i<count)&&(BCDValue1[i]=='0');i++)
            BCDValue1[i]=' ';
    }

    count = 0;//统计转换后的有效BCD长度
    /* Add Postfix Sign. Sign In bcd.Sign.7 */
    if (TestBit(Format,8))
    {
        if (TestBit(bcd->Sign,7)) *to = '-';
        else *to = ' ';
        to--;
        count++;
    }

    if (TestBit(Format,10)) /* QTY:Decimal Length In bcd.Sign 2..0 bits
                               And Don't Need Comma List Separator.
                               And decimal Dot Must be Dot*/
        DecimalPartLen = bcd->Sign&0x0f;
    else    /*Decimal Length depend on the Format.15 and Format.11
            .15   .11   Decimal Length    Need Comma List Separator.
             1     1   1                    Y
             1     0   3                    Y
             0     0   2                    Y
             0     1   0                    N
            */
    {
        DecimalPartLen = 2;
        if (TestBit(Format,15))
        {
            if (TestBit(Format,11))
                DecimalPartLen--; //Decimal Length=1
            else
                DecimalPartLen++;//Decimal Length=3
        }
        else if (TestBit(Format,11))
                DecimalPartLen = 0;//Decimal Length=0
    }

    do
    {
        if (*WorkPtr == ' ') break;
        /* Add decimal dot*/
        if ((index == DecimalPartLen)&&(DecimalPartLen>0))
        {
            if (TestBit(Format,10))//为QTY打印
                *to = '.';
            else
            {
                *to = ',';
                /*FormatInfo.4 .<--->,*/
                if (TestBit(Format,12))
                {
                    if (*to == '.') *to = ',';
                    else *to = '.';
                }
            }
            to--;
            count++;
        }
        else   if (!TestBit(Format,13)&&(!TestBit(Format,10)))// Amt
        {
            /* Add Comma List Separator  (将数据中添加分隔符号",")
                FormatInfo.5 sum print but not need list separator
                FormatInfo.2 quantity print            */
            /*Add Decimal Part List Separator*/
            if (index < DecimalPartLen)
            {
                if ((((DecimalPartLen-index)%3)==0)&&(index >0))
                {
                    /*FormatInfo.4 .<--->,*/
                    if (TestBit(Format,12))
                        *to = ',';
                    else
                        *to = '.';
                    to--;
                    count++;
                }
            }
            /*Add Integer Part List Separator*/
            else
            {
                if ((((index-DecimalPartLen)%3)==0)&&(index >0))
                {
                    /*FormatInfo.4 .<--->,*/
                    if (TestBit(Format,12))
                        *to = ',';
                    else
                        *to = '.';
                    to--;
                    count++;
                }
            }
        }
        /* Copy Digital*/
        *to = *WorkPtr;
        to--;
        count++;
        index++;

        WorkPtr--;
    }
    while (index != 2*BCDLEN);

    /*Add Prefix Sign. Sign In bcd.Sign.7*/
//ccr070424>>>>>>>>>>>>>>>>>>>>>>
    if (!TestBit(Format,8))
    {
        if (!TestBit(Format,7))
        {
            if (TestBit(bcd->Sign,7))
                *to = '-';
            else
                *to = ' ';
            to--;
            count++;
        }
        if (!TestBit(Format,10) && !TestBit(Format,14))
        {
            if (Prefix2)
            {
                *to = Prefix2;
                to--;
                count++;
            }
            if (Prefix1)
            {
                *to = Prefix1;
                to--;
                count++;
            }
        }
        if (TestBit(Format,7))
        {
            if (TestBit(bcd->Sign,7))
                *to = '-';
            else
                *to = ' ';
            to--;
            count++;
        }

    }
    else
    {//追加前缀
        if (!TestBit(Format,10) && !TestBit(Format,14))
        {
            if (Prefix2)
            {
                *to = Prefix2;
                to--;
                count++;
            }
            if (Prefix1)
            {
                *to = Prefix1;
                to--;
                count++;
            }
        }
    }
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    /*remove 0 of the tail after Decimal*/
    if ((DecimalPartLen > 0))
    {
        if (TestBit(Format,12)||TestBit(Format,10))
            decimal='.';
        else
            decimal=',';
        while (q!= to)
        {
            /* if decimal point equal '.' */
            if ((*(q-2)==decimal)||(*(q-1)==decimal)
                ||(((Format & 0x8800)==0x8000) && (*(q-3)==decimal))) //ccr2016-11-08
                break;
            else
                if ((*q > '0')&&(*q <= '9')) break;
            else
                *q = ' ';

            q--;
        }
    }
    return count;
}

//
/* make unsigned long from bcd number */
WORD  BCDValueToULong(BYTE  *p,UnLong  *Lp)       /* (void *p, UnLong *Lp) max is 999.999.999 */
{
    short       i;
    unsigned  long     re=0;

    for (i=BCDLEN-2;i>=0;i--)
    {
        if (p[i]<=0x99)
            re=re*100+BCDtoDEC(p[i]);
    }
    *Lp=re;
    return(re & 0xffff);
}
//-----------------------------------------------------------------------------
UnLong Bcd2Long(BYTE *p, BYTE BcdBytes)
{
    UnLong  sTmp;
    short  sLp;

    sTmp=0;
    for (sLp=BcdBytes-1;sLp>=0;sLp--)
    {
        sTmp=(sTmp<<3)+(sTmp<<1)+(p[sLp] >> 4);
        sTmp=(sTmp<<3)+(sTmp<<1)+(p[sLp] & 0x0f);
    }
    return(sTmp);
}
/* make unsigned long from bcd number */
UINT64  BCDValueToLong64(BYTE  *p)       /* (void *p, UnLong *Lp) max is 999.999.999 */
{
    short       i;
    UINT64     re=0;

    for (i=BCDLEN-2;i>=0;i--)
    {
        if (p[i]<=0x99)
            re=re*100+BCDtoDEC(p[i]);
    }
    return(re);
}
//*************比较指定长度的两个BCD.Value数据的大小*******************
short  CompareBCDValue(void  *a,void  *b, WORD  length)
{
    BYTE   *tmp1,*tmp2;
    WORD  i;

    tmp1=(BYTE  *) a;
    tmp2=(BYTE  *) b;
    tmp1 += (length-1);
    tmp2 += (length-1);

    for (i=0;i<length;i++)
    {
        if (*tmp1 >  *tmp2) return 1;
        if ( *tmp1 < *tmp2) return -1;
        tmp1--;
        tmp2--;
    }
    return 0;
}

/********** compare two BCD numbers **********/
/* result   returnvalue    */
/* a = b    0		   */
/* a < b    -1		   */
/* a > b    1		   */

short     CompareBCD(BCD *a, BCD *b)   /* (BCD *a, BCD *b) */
{
    short      i;

    BCDDecimal1=a->Sign  &  0x0f;//小数位数
    BCDDecimal2=b->Sign  &  0x0f;//小数位数

    memset(BCDValue1,0,sizeof(BCDValue1));
    memset(BCDValue2,0,sizeof(BCDValue2));
    memcpy(BCDValue1,a->Value,BCDLEN);
    memcpy(BCDValue2,b->Value,BCDLEN);

    i = BCDLEN-1;
    while (BCDDecimal1>BCDDecimal2)
    {
        BCDValue_MUL_10(BCDValue2);
        BCDDecimal2++;
        i++;
    }
    while (BCDDecimal2>BCDDecimal1)
    {
        BCDValue_MUL_10(BCDValue1);
        BCDDecimal1++;
        i++;
    }
    for (;i>=0;i--)
    {
        if (BCDValue1[i]>BCDValue2[i])
            return 1;
        if (BCDValue1[i]<BCDValue2[i])
            return -1;
    }

    return 0;
}

//result1/with1-> result1
void    Divide(BCD *result1, BCD *with1)      /* (BCD *result, BCD *with) */
{
    short    decmi,tmp;
    BYTE     *result,*with;
    BYTE     temp3[17],temp4[17],temp5[17];

    if (CheckNotZero(result1))
    {
      with=(BYTE *)with1;
      result=(BYTE *)result1;
      SaveBCDDec1=*(BYTE *)result;
      SaveBCDDec2=*(BYTE *)with;
      BCDSign1=SaveBCDDec1&0x80;
      BCDSign2=SaveBCDDec2&0x80;
      BCDDecimal1=SaveBCDDec1&0x07;
      BCDDecimal2=SaveBCDDec2&0x07;
      result++;
      with++;

      SaveBCDSign1=0;
      if (BCDSign1!=BCDSign2)
          SaveBCDSign1=0x80;


      memset(temp3,0,sizeof(temp3));
      memset(temp4,0,sizeof(temp4));
      memset(temp5,0,sizeof(temp5));

      memcpy(BCDValue1,result1->Value,BCDLEN);
      memcpy(BCDValue2,with1->Value,BCDLEN);

      decmi=BCDDecimal1-BCDDecimal2;
      BCDDecimal2=0;
      while ( CompareBCDValue(BCDValue1,BCDValue2,BCDLEN*2) > 0)
      {
          BCDValue_MUL_10(BCDValue2);
          decmi--;
      }
      while ( CompareBCDValue(BCDValue1,BCDValue2,BCDLEN*2) < 0)
      {
          BCDValue_MUL_10(BCDValue1);
          decmi++;
      }

      for (;;)
      {
          if (decmi>6)
              break;
          tmp=CompareBCDValue(BCDValue1,BCDValue2,BCDLEN*2);
          if (tmp == 0)
          {

              memcpy(BCDValue1,temp5,sizeof(BCDValue1));
              memset(BCDValue2,0,sizeof(BCDValue2));
              BCDValue2[0]=1;
              AddBCDValue12();
              memcpy(temp5,BCDValue1,sizeof(temp5));

              break;
          }
          else if (tmp<0)
          {
              BCDValue_MUL_10(BCDValue1);
              BCDValue_MUL_10(temp5);
              decmi++;
          }
          else
          {
              while (tmp>0)
              {
                  SubBCDValue12();
                  BCDDecimal2++;
                  tmp=CompareBCDValue(BCDValue1,BCDValue2,BCDLEN*2);
              }

              memcpy(temp3,BCDValue1,sizeof(temp3));
              memcpy(BCDValue1,temp5,sizeof(BCDValue1));
              memcpy(temp4,BCDValue2,sizeof(temp4));
              memset(BCDValue2,0,sizeof(BCDValue2));
              BCDValue2[0]=BCDDecimal2;
              BCDDecimal2=0;
              AddBCDValue12();
              memcpy(temp5,BCDValue1,sizeof(temp5));
              memcpy(BCDValue1,temp3,sizeof(BCDValue1));
              memcpy(BCDValue2,temp4,sizeof(BCDValue2));
          }
      }

      while (decmi<0)
      {
          BCDValue_MUL_10(temp5);
          decmi++;
      }



      memcpy(BCDValue1,temp5,sizeof(BCDValue1));

      BCDDecimal1=decmi;
      Correct_BCDValue1();

      result1->Sign=SaveBCDSign1;
      memcpy(result1->Value,BCDValue1,8);
    }
    else
    {
      result1->Sign=0;
    }
}

/**********************************************************
 * convert hex to asci
 *
 * @author EutronSoftware (2016-02-16)
 *
 * @param to :  "123456"
 * @param from: 0x123456 ("\x56\x34\x12")
 * @param len : 3,为from长度
 */
void    HEXtoASC(char *to, char *from, short len)
{
    short      i;
    BYTE      *too=to;
    BYTE      *from1=from;
    char      t1;

    too += (2*len-1);//指向末尾

    for (i=0;i<len;i++,from1++)
    {
        t1=(*from1 &  0x0f)+'0';
        if (t1>=0x3a) t1 +=7;
        *too--=t1;

        t1=((*from1) >>  4)+'0';
        if (t1 >=0x3a) t1  += 7;
        *too-- = t1;
    }
}
/**
 * 功能同HEXtoASC,去掉头部的0
 *
 * @author EutronSoftware (2016-05-22)
 *
 * @param to
 * @param from
 * @param len:为from长度
 * @return BYTE:放回ASC字符个数
 */
BYTE  HEXtoASCX0(char *to, char *from, short len)
{
    short      i;
    BYTE      *too=to;
    BYTE      *from1=from;
    BYTE      t1;

    too += (2*len-1);//指向末尾

    for (i=0;i<len;i++,from1++)
    {
        t1=(*from1 &  0x0f)+'0';
        if (t1>=0x3a) t1 +=7;
        *too--=t1;

        t1=((*from1) >>  4)+'0';
        if (t1 >=0x3a) t1  += 7;
        *too-- = t1;
    }
    t1 = i*2;
    too++;
    //清除头部的0
    for (i=0;i<(2*len-1) && *too=='0';i++)
        *too++=' ';
    return t1;
}

/* (void *to, WORD) make BCD from WORD */
void    WORDtoBCD(char *to, WORD num1)       /* (void *to, WORD) make BCD from hex */
{
    to[0]=DECtoBCD(num1%100);num1/=100;
    to[1]=DECtoBCD(num1%100);num1/=100;
    to[2]=num1;
}
//以右对齐方式将WORD转为字符串
// '1234'
//    to^
//返回有效数字位数
BYTE    WORDtoASC(char *to, WORD num1)        /* (void *to, WORD) make ascii string from WORD */
{
    short     i;
    BYTE      *tmp;

    tmp= (BYTE *)to;

    for (i=0;i<5;i++)
    {
        *tmp--=(num1 % 10) + '0';
        num1/=10;
        if (num1==0)
            break;
    }
    return i+1;
}


//将WORD数据转换为字符串,以左对齐方式存储在to中
// no leading zero's & space
//返回:转换后的字符串长度
// '1234'
//  ^to

short WORDtoASCL(char *to, WORD num)
{
    short i,j;
    char ch;

    if (num>9999)       i = 10000;
    else if (num>999)   i = 1000;
    else if (num>99)    i = 100;
    else if (num>9)     i = 10;
    else                i = 1;

    j = 0;

    for (;i;)
    {
        ch = (num / i);
        if (ch || j)
        {
            to[j] = ch+'0';
            j++;
        }
        num %= i;
        i /= 10;
    }
    if (j==0)
    {
        to[0]='0';
        j=1;
    }
    return j;
}
// '01234'
//    to^
void    WORDtoASCZero(char *to, WORD num)     /* (void *to, WORD) make asci string from WORD with */
{
    short     i;
    BYTE      *tmp;
    WORD      num1=num;

    tmp= (BYTE *)to;

    for (i=0;i<5;i++)
    {
        *tmp--=(num1 % 10) + '0';
        num1/=10;
    }
}/* leading zero's */

//将UNLONG数据转换为字符串,以'\0\结束,以左对齐方式存储在to中
// no leading zero's & space
//返回:转换后的字符串长度
// '1234'
//  ^to

int ULongtoASCL(char *to, ULONG num)
{
    ULONG i,j;
    char ch;

    i = 10;
    for (;num>=i;)
            i*=10;
    i/=10;

    j = 0;

    for (;i;)
    {
        ch = (num / i);
        if (ch || j)
        {
            to[j] = ch+'0';
            j++;
        }
        num %= i;
        i /= 10;
    }
    if (j==0)
    {
        to[0]='0';
        j=1;
    }
    to[j] = 0;
    return j;
}


/**
 *以右对齐方式将ULONG转为字符串
 * '1234'
 *    to^
 *
 * @author EutronSoftware (2016-05-23)
 *
 * @param to
 * @param num
 *
 * @return BYTE:返回有效数字位数
 */
BYTE    ULongtoASC(char *to, ULONG num)        /* (void *to, WORD) make ascii string from WORD */
{
    short     i;

    BYTE      *tmp;
    ULONG      num1=num;

    tmp= (BYTE *)to;

    for (i=0;;i++)
    {
        *tmp--=(num1 % 10) + '0';
        num1/=10;
        if (num1==0)
            break;
    }
    return i+1;
}

/**
 *以右对齐方式将UINT64转为字符串
 * '1234'
 *    to^
 *
 * @author EutronSoftware (2016-05-23)
 *
 * @param to
 * @param num
 *
 * @return BYTE:返回有效数字位数
 */
BYTE ULong64toASC(char *to, UINT64 num)        /* (void *to, UINT64) make ascii string from WORD */
{
    short     i;

    BYTE      *tmp;
    UINT64    num1=num;

    tmp= (BYTE *)to;

    for (i=0;;i++)
    {
        *tmp--=(num1 % 10) + '0';
        num1/=10;
        if (num1==0)
            break;
    }
    return i+1;
}


void  ULongToBCDValue(BYTE  *to, unsigned long re)
{
    short     i;
    unsigned long     relong;
    relong=re;

    for (i=0;relong;i++)
    {
        to[i]=DECtoBCD(relong%100);
        relong/=100;
    }
}

void LeftShiftBCD(BCD *result,BYTE shiftnum)
{
    BYTE validshiftnum;
    BYTE i,j,src,dest;
    validshiftnum = result->Sign&0x0F;
    validshiftnum = 2*BCDLEN - validshiftnum;

    result->Sign &= 0xF0;
    if (shiftnum > validshiftnum)
    {
        for (i=0;i<BCDLEN;i++)
            result->Value[i] = 0x0;
        return;
    }
    else
    {
        j = shiftnum/2;
        if (j>0)
        {
            for (i=j;i<BCDLEN;i++)
                result->Value[i-j] = result->Value[i];
            for (i=0;i<j;i++)
                result->Value[BCDLEN -1- i] = 0x0;
        }

        if (shiftnum & 1)//
        {
            for (i= 0;i<BCDLEN-1;i++)
            {
                src = result->Value[i];
                dest = result->Value[i+1];
                result->Value[i] = (src>>4) | (dest<<4);
            }
            result->Value[BCDLEN-1] >>= 4;
        }
    }
}


void    Add(BCD *result1, BCD *with1)       /* (BCD *result, BCD *with) add two BCD numbers */
{
    short       i;
    BYTE    tmp;

    BCDSign1=result1->Sign & 0x80;
    BCDSign2=with1->Sign & 0x80;
    BCDDecimal1=result1->Sign & 0x07;
    BCDDecimal2=with1->Sign & 0x07;


    memset(BCDValue1,0,sizeof(BCDValue1));
    memset(BCDValue2,0,sizeof(BCDValue2));

    memcpy(BCDValue1,result1->Value,BCDLEN);
    memcpy(BCDValue2,with1->Value,BCDLEN) ;

    if (BCDDecimal1>BCDDecimal2)
        while (BCDDecimal1>BCDDecimal2)
        {
            BCDDecimal2++;
            BCDValue_MUL_10(BCDValue2);
        }
    else if (BCDDecimal1<BCDDecimal2)
        while (BCDDecimal1<BCDDecimal2)
        {
            BCDDecimal1++;
            BCDValue_MUL_10(BCDValue1);
        }

    for (i=7;i>=0;i--)
    {
        if (BCDValue1[i]>BCDValue2[i])
            break;
        if (BCDValue1[i]<BCDValue2[i])
        {
            for (i=0;i<8;i++)
            {
                tmp=BCDValue1[i];
                BCDValue1[i]=BCDValue2[i];
                BCDValue2[i]=tmp;
            }
            tmp=BCDSign2;
            BCDSign2=BCDSign1;
            BCDSign1=tmp;
            break;
        }
    }
    if (BCDSign1==BCDSign2)
        AddBCDValue12();
    else
        SubBCDValue12();
    SaveBCDSign1=BCDSign1;
    Correct_BCDValue1();

    result1->Sign=SaveBCDSign1;
    memcpy(result1->Value,BCDValue1,BCDLEN);
}

void    Multiply(BCD *result1, BCD *with1)     /* (BCD *result, BCD *with) */
{
#if (0)
    //利用长正数来进行乘法运算
    ULONG       multiFor,multiWith;
    UINT64      result;

    BCDSign1=result1->Sign & 0x80;
    BCDSign2=with1->Sign & 0x80;

    BCDDecimal1=(with1->Sign & 0x07) + (result1->Sign & 0x07);

    if (BCDSign1==BCDSign2)
        BCDSign1=0;//符号相同,结果为正数
    else
        BCDSign1=0x80;//符号不同,结果为负数

    BCDValueToULong(result1->Value,&multiFor);
    BCDValueToULong(with1->Value,&multiWith);
    result = multiFor * multiWith;
    Correct_UINT64(&result,&BCDDecimal1);
    multiFor = result;
    memset(result1->Value,0,BCDLEN);
    ULongToBCDValue(result1->Value,multiFor);
    result1->Sign =BCDSign1 |  BCDDecimal1;

#else //利用移位加法实现BCD乘法运算
    short   i,i1,len;

    BYTE    temp3[BCDLEN*2+1],
            temp4[BCDLEN*2+1],
            temp5[BCDLEN*2+1],
            tmp;

    SaveBCDDec1=result1->Sign;
    SaveBCDDec2=with1->Sign;

    BCDSign1=SaveBCDDec1&0x80;
    BCDSign2=SaveBCDDec2&0x80;
    BCDDecimal1=SaveBCDDec1&0x07;
    BCDDecimal2=SaveBCDDec2&0x07;

    memset(BCDValue1,0,sizeof(BCDValue1));
    memset(BCDValue2,0,sizeof(BCDValue2));

    memcpy(BCDValue1,result1->Value,BCDLEN);
    memcpy(BCDValue2,with1->Value,BCDLEN);

    i1=7;
    //对其小数点
    if (BCDDecimal1>BCDDecimal2)
        while (BCDDecimal1>BCDDecimal2)
        {
            i1++;
            BCDDecimal2++;
            BCDValue_MUL_10(BCDValue2);
        }
    else if (BCDDecimal1<BCDDecimal2)
        while (BCDDecimal1<BCDDecimal2)
        {
            i1++;
            BCDDecimal1++;
            BCDValue_MUL_10(BCDValue1);
        }

    for (i=7;i>=0;i--)
    {
        if (BCDValue1[i]>BCDValue2[i])
            break;
        if (BCDValue1[i]<BCDValue2[i])
        {//让较小的数做乘数

            memcpy(temp3,BCDValue1,BCDLEN);//辆数交换
            memcpy(BCDValue1,BCDValue2,BCDLEN);
            memcpy(BCDValue2,temp3,BCDLEN);
            tmp=BCDSign2;
            BCDSign2=BCDSign1;
            BCDSign1=tmp;
            break;
        }
    }

    if (BCDSign1==BCDSign2)
        BCDSign1=0;//符号相同,结果为正数
    else
        BCDSign1=0x80;//符号不同,结果为负数
    BCDDecimal1 *=2;

    memset(temp3,0,sizeof(temp3));
    memset(temp4,0,sizeof(temp4));
    memset(temp5,0,sizeof(temp5));

    //以下实现 BCDValue1 * BCDValue2
    memcpy(temp3,BCDValue1,sizeof(temp3));//被乘数
    memcpy(temp4,BCDValue2,sizeof(temp4));//乘数
    memcpy(BCDValue2,BCDValue1,sizeof(BCDValue1));

    for (; i1>=0 && temp4[i1]!=0;i1--);//寻找最高有效位
    //temp5用来保存中间结果
    for (;i1>=0;i1--)
    {
        //p*qn
        memset(BCDValue1,0,sizeof(BCDValue1));

        len=(temp4[i1] & 0xf0) >> 4;

        while (len>0)
        {
            AddBCDValue12();
            len--;
        }
        //r=r*10
        BCDValue_MUL_10(temp5);
        //r=r+p*qn
        memcpy(BCDValue2,temp5,sizeof(BCDValue2));
        AddBCDValue12();

        memcpy(temp5,BCDValue1,sizeof(temp5));
        memcpy(BCDValue2,temp3,sizeof(BCDValue2));
        memset(BCDValue1,0,sizeof(BCDValue1));
        len=(temp4[i1] & 0x0f)   ;

        while (len>0)
        {
            AddBCDValue12();
            len--;
        }

        //r=r*10
        BCDValue_MUL_10(temp5);
        //r=r+p*qn
        memcpy(BCDValue2,temp5,sizeof(BCDValue2));

        AddBCDValue12();

        memcpy(temp5,BCDValue1,sizeof(temp5));
        memcpy(BCDValue2,temp3,sizeof(BCDValue2));
    }
    memcpy(BCDValue1,temp5,sizeof(BCDValue1));

    SaveBCDSign1=BCDSign1;
    Correct_BCDValue1();
    result1->Sign=SaveBCDSign1;
    memcpy(result1->Value,BCDValue1,BCDLEN);
#endif
}

void  RoundBcd(BCD *result, WORD decimal)    /* (BCD *n, BYTE decimal) round BCD to fixed number of decimals */
{
    short decimalnum = 0;
    BYTE CFlag = 0,i,ch;

    decimalnum = (result->Sign&0x0F) - decimal;
    if (decimalnum <= 0) return;

//	i = decimalnum/2;
    i=(decimalnum+1)/2-1;/*change by nick 2006.8.28*/
    if (!(decimalnum%2))/*change by nick 2006.8.9*/
        CFlag = (result->Value[i] >= 0x50);
    else
        CFlag = ((result->Value[i]&0x0F) >= 0x05);

    LeftShiftBCD(result,decimalnum);

    for (i=0;(i<2*BCDLEN)&&CFlag;i++)
    {
        if (i & 1)//(i%2)
            ch = result->Value[i/2]>>4;
        else
            ch = result->Value[i/2]&0x0F;

        ch += CFlag;
        if (ch>=10)
        {
            CFlag = 1;
            ch %= 10;
        }
        else
            CFlag = 0;

        if (i & 1)//(i%2)
        {
            result->Value[i/2] &= 0x0F;
            result->Value[i/2] |= ch<<4;
        }
        else
        {
            result->Value[i/2] &= 0xF0;
            result->Value[i/2] |= ch;
        }
    }
}

void    Subtract(BCD *result1, BCD *with1)     /* (BCD *result, BCD *with) */
{
    short       i;
    BYTE    tmp;

    SaveBCDDec1=result1->Sign;
    SaveBCDDec2=with1->Sign;
    BCDSign1=SaveBCDDec1&0x80;
    BCDSign2=SaveBCDDec2&0x80;
    BCDDecimal1=SaveBCDDec1&0x07;
    BCDDecimal2=SaveBCDDec2&0x07;

    memset(BCDValue1,0,sizeof(BCDValue1));
    memset(BCDValue2,0,sizeof(BCDValue2));

    memcpy(BCDValue1,result1->Value,BCDLEN);
    memcpy(BCDValue2,with1->Value,BCDLEN);

    if (BCDDecimal1>BCDDecimal2)
        while (BCDDecimal1>BCDDecimal2)
        {
            BCDDecimal2++;
            BCDValue_MUL_10(BCDValue2);
        }
    else if (BCDDecimal1<BCDDecimal2)
        while (BCDDecimal1<BCDDecimal2)
        {
            BCDDecimal1++;
            BCDValue_MUL_10(BCDValue1);
        }

    for (i=7;i>=0;i--)
    {
        if (BCDValue1[i]>BCDValue2[i])
            break;
        if (BCDValue1[i]<BCDValue2[i])
        {
            for (i=0;i<8;i++)
            {
                tmp=BCDValue1[i];
                BCDValue1[i]=BCDValue2[i];
                BCDValue2[i]=tmp;
            }
            tmp=BCDSign2;
            BCDSign2=BCDSign1^0x80;
            BCDSign1=tmp^0x80;
            break;
        }
    }

    if (BCDSign1==BCDSign2)
        SubBCDValue12();
    else
        AddBCDValue12();
    SaveBCDSign1=BCDSign1;
    Correct_BCDValue1();

    result1->Sign=SaveBCDSign1;
    memcpy(result1->Value,BCDValue1,BCDLEN);
}

void BcdMul100(BCD *A0)
{
    int i;

    for (i=BCDLEN-1;i>0;i--)
    {
        A0->Value[i]=A0->Value[i-1];
    }
    A0->Value[0]=0;
}


void BcdMul10(BCD *A0)
{
    memset(BCDValue1,0,sizeof(BCDValue1));
    memcpy(BCDValue1,A0->Value,BCDLEN);
    BCDValue_MUL_10(BCDValue1);
    memcpy(A0->Value,BCDValue1,BCDLEN);
}

void BcdDiv10(BCD *A0)
{
    memset(BCDValue1,0,sizeof(BCDValue1));
    memcpy(BCDValue1,A0->Value,8);
    BCDValue_DIV_10(BCDValue1);
    memcpy(A0->Value,BCDValue1,8);

}

/**
 * 快速实现BCD/100的操作
 *
 * @author EutronSoftware (2016-05-28)
 *
 * @param A0
 */
void BcdDiv100(BCD *A0)
{
    memcpy(A0->Value,A0->Value+1,BCDLEN-1);
    A0->Value[BCDLEN-1]=0;
}

/********************************************************************************/
//将输入的字符加入到输入存储区EntryBuffer中
//成功加入,返回输入区的字符数
//出错时,返回0;
BYTE AppendEntry(char chKey)
{
    if (!Appl_EntryCounter)
    {
        memset(EntryBuffer, ' ', ENTRYSIZE-1);
        ClsState12();//ccr2016-08-15
    }

    if (!TESTBIT(ApplVar.MyFlags,HANZIMODE))//销售模式下
    {
        //为数字键
        if (chKey==ZERO2)
        {
            AppendEntry('0');
            chKey='0';
        }
        if (chKey == '.')     //      小数点  //
        {
            if (!ApplVar.DecimalPoint)
            {
                ApplVar.DecimalPoint = 1;
                if (Appl_EntryCounter==0)
                {//输入的数据为".",自动变为"0."
                    AtEntryBuffer(2) = '0';
                    AtEntryBuffer(1) = '.';
                    AtEntryBuffer(0) = 0;
                    Appl_EntryCounter = 2;
                    return 2;
                }
            }
            else
            {
//				ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                return false;
            }
        }
        else if (ApplVar.DecimalPoint && chKey>='0' && chKey<='9')//     Set ApplVar.IP address
        {
            if (ApplVar.DecimalPoint > 3)    // 小数点个数大于4 //
            {
//				ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                return false;
            }
            ApplVar.DecimalPoint++;
        }
    }

    if (Appl_EntryCounter)   //      Shift entry 将按键值移入接收缓冲区  PenGH //
        memcpy(EntryBuffer, EntryBuffer + 1,ENTRYSIZE - 2);
    AtEntryBuffer(1) = chKey;
    AtEntryBuffer(0) = 0;
    Appl_EntryCounter++;
    return Appl_EntryCounter;
}
//从EntryBuffer删除最后输入的字符
void DeleteEntry()
{
    int i,k;

    if (Appl_EntryCounter==1)
    {
        ClearEntry(false);
    }
    else if (Appl_EntryCounter>1)
    {
        k = ENTRYSIZE - 2;
        for (i=0;i<(Appl_EntryCounter-1);i++,k--)
            EntryBuffer[k] = EntryBuffer[k-1];
        EntryBuffer[k] = ' ';
        Appl_EntryCounter --;
        if (ApplVar.DecimalPoint)
            ApplVar.DecimalPoint--;
    }
}
//////////////////////////////////////////////////////////////////////////////
void Copy_LCD(BYTE ctrl)
{
}
/////////////////////////////////////////////////////////////////////////////
/***************************************************************************
  显示存放在RGBuf中的数据,每次显示三条
  Appl_dispRGNumber指示当前要显示的记录号
 UpDown=-1:显示前一批,当前一批不足三条时,重新从第一条开始向后显示;
       =1显示后一批
 返回:true-有可显示的数据
      false-无可显示的数据
  ***************************************************************************/
#define scrollBuf(ip)    &CCDBuff[ip*SCREENWD]
BYTE Display_RGBuf(short UpDown)
{
    short items=0,scanIDX;
    BCD sAmt;
    struct TRANSRECORD  RGRec;          /* temporary record */
    BYTE isTrue;/* 标示是否为有效的销售项目*/
    BYTE firstFound=false;  /* 未找到第一个有小项目 */

    if (ApplVar.FRegi)
    {
        if ((ApplVar.RGRec.Key.Code>DEPT && ApplVar.RGRec.Key.Code<PLU3
             || ApplVar.RGRec.Key.Code>DISC && ApplVar.RGRec.Key.Code<=DISC+discNumber)
            && Appl_DispRGBuf.RGCount == ApplVar.RGItems.RGNumber
            && ApplVar.FRegi)
        {//最后一个项目为单品或者部类时
            Appl_DispRGBuf.RGCount = ApplVar.RGItems.RGNumber+1;
            ApplVar.RGItems.RGBuf[ApplVar.RGItems.RGNumber] =  ApplVar.RGRec;
            Appl_DispRGBuf.FirstRGNum++;
            Appl_DispRGBuf.LastRGNum = Appl_DispRGBuf.FirstRGNum;
        }

        if (UpDown==1)
            scanIDX = Appl_DispRGBuf.LastRGNum;
        else
            scanIDX = Appl_DispRGBuf.FirstRGNum;

        memset(CCDBuff,' ',(SCREENLN-1)*SCREENWD);
        /* 从RGBuf中挑选销售项目,最多挑选SCREENLN-1条 */
        while (items<SCREENLN-1)
        {
            isTrue = false;
            scanIDX+=UpDown;
            if (scanIDX>=0 && scanIDX<Appl_DispRGBuf.RGCount)
            {
                RGRec = ApplVar.RGItems.RGBuf[scanIDX];

                //if ((RGRec.Key.Code>DEPT && RGRec.Key.Code<PLU3))//it must be the PLU or DEPT record
                {
                    if (CheckNotZero(&RGRec.Qty))//Must not be the cancel record
                    {
                        memset(ProgLineMes,' ',sizeof(ProgLineMes));
                        sAmt = RGRec.Amt;

                        if (RGRec.Key.Code>PLU1 && RGRec.Key.Code<PLU3)
                        {
                            ApplVar.PluNumber = RGRec.Key.Code - PLU1 - 1;
                            ReadPlu();

                            strcpy(ProgLineMes,ApplVar.Plu.Name);
                            sAmt.Value[BCDLEN-1]=0;/* 清除价格级别标示 */
                            Multiply(&sAmt, &RGRec.Qty);
                            RoundBcd(&sAmt, 0);

                            isTrue=true;
                        }
                        else if (RGRec.Key.Code>DEPT && RGRec.Key.Code<DEPT+5000)
                        {
                            ApplVar.DeptNumber = RGRec.Key.Code - DEPT - 1;
                            ReadDept();

                            strcpy(ProgLineMes,ApplVar.Dept.Name);
                            Multiply(&sAmt, &RGRec.Qty);
                            RoundBcd(&sAmt, 0);
                            isTrue=true;
                        }
                        else if (RGRec.Key.Code>DISC && RGRec.Key.Code<DISC+5)
                        {
                            ApplVar.DiscNumber = RGRec.Key.Code - DISC - 1;
                            ReadDisc();
                            ApplVar.Disc.Name[ApplVar.AP.Disc.CapSize] = 0;
                            strcpy(ProgLineMes,ApplVar.Disc.Name);
                            isTrue=true;
                        }
                        else
                        {
                            //WORDtoASCL(ProgLineMes,RGRec.Key.Code);
                            //ProgLineMes[6]=0;
                        }
                        if (isTrue)
                        {
                            if (!firstFound)
                            {
                                Appl_DispRGBuf.FirstRGNum = scanIDX;
                                firstFound=true;
                            }

                            Appl_DispRGBuf.LastRGNum = scanIDX;
                            strcpy(scrollBuf(items),FormatAmtStr(ProgLineMes, &sAmt, SCREENWD));
                            items++;
                            //RJPrint(0,FormatStrQtyPriAmt(ProgLineMes, 0, 0, &sAmt, SCREENWD));
                        }
                    }
                }
            }
            else
            {
                if (UpDown<0 && items<SCREENLN-1 && Appl_DispRGBuf.RGCount>=SCREENLN-1)
                {/* 从开始位置重新查找,以便显示满屏幕 */
                    UpDown = 1;
                    items = 0;
                    memset(CCDBuff,' ',(SCREENLN-1)*SCREENWD);
                    scanIDX = -1;
                    firstFound = false;
                }
                else
                {
                    break;
                }
            }
        }
        if (items)
        {
            mClearScreen();
            isTrue = 0;
            if (UpDown<0)
            {/* 为从后向前挑选项目(即先挑选的是后销售的项目)*/
                scanIDX = Appl_DispRGBuf.LastRGNum;
                Appl_DispRGBuf.LastRGNum = Appl_DispRGBuf.FirstRGNum;
                Appl_DispRGBuf.FirstRGNum = scanIDX;
                for (UpDown=items-1;UpDown>=0;UpDown--)
                {
                    memcpy(ApplVar.ScreenMap[isTrue],scrollBuf(UpDown),SCREENWD);
                    mRefreshLine(isTrue++);

                }
            }
            else
            {/* 为从前向后挑选项目(即依销售先后顺序挑选销售项目)*/
                for (UpDown=0;UpDown<items;UpDown++)
                {
                    memcpy(ApplVar.ScreenMap[isTrue],scrollBuf(UpDown),SCREENWD);
                    mRefreshLine(isTrue++);
                }
            }
            /* 剩余行清空 */
            /*
            for (;isTrue<SCREENLN-1;isTrue++)
            {
                memset(ApplVar.ScreenMap[isTrue],' ',SCREENWD);
                mRefreshLine(isTrue);
            }
            */
        }
    }
    return (items>0);
}
//-----------------------------------------------------------------------
//将字符串打印在中间位置
// BYTE isMidPrt   == true  打印居中格式
// DBLHIPRN  BIT3  if set 票头居中
//           BIT4  if set 票尾居中

void PrintStr_Center(char *str,BYTE alCenter)
{

    int j,k;

    char strBuff[PRTLEN+1];

    k = strlen(str)-1;
    if ( alCenter && k>=0 && k<PRTLEN-1 ) // Middle print Trailer
    {
        for ( j = 0 ; j<=k && str[j]==' ';j++ ) {}//j=第一个非空字符
        for (; j<k && str[k] == ' ';k-- ) {} //k=最后一个非空字符
        k -= j;//有效的字符串长度
        k++;
        memset(strBuff,' ',sizeof(strBuff)) ;
        memcpy( strBuff + (PRTLEN - k)/2 , str + j , k);
        strBuff[ PRTLEN  ] = '\0';
        RJPrint(0,strBuff);
    } else
        RJPrint(0,str);
}
//-----------------------------------------------------------------------
//将字符串打印在右端位置
void PrintStr_Right(char *str)
{
    char sBuf[PRTLEN+1];
    int len,i;
    len = strlen(str);
    if (len>=PRTLEN)
    {
        RJPrint(0,str);
    }
    else
    {
        memset(sBuf,' ',PRTLEN);
        strcpy(sBuf+(PRTLEN-len),str);
        RJPrint(0,sBuf);
    }
}
/********通过日期算星期 *************************************************
如计算2010年12月05日是星期几：假设 uchar   year,month,day;
            year=0x10;     //年份：10年
            month=0x12;     //月份：12月
            day=0x05;     //日：    05日
则星期为：week1=GetWeekDay((year>>4)*10+(year&0x0f)+2000,
                           (month>>4)*10+(month&0x0f),
                           (day>>4)*10+(day&0x0f));
***********************************************************************/

CONST BYTE Month_Day[12]={31,28,31,30,31,30,31,31,30,31,30,31};

BYTE DaysAMonth(WORD y,BYTE m)  //算出某年某月有多少天
{
       if(m==2)
          return(((y%4 == 0) && (y%100 != 0) || (y%400 == 0))? 29: 28);
       else
          return(Month_Day[m-1]);
}

//************************************************
//计算从1900年开始到某年某月某日的天数
//year, month, day 均为BCD数据
WORD GetDaysFrom1900(WORD year,BYTE month,BYTE day)
{
    WORD nday=0;
    WORD i;

    year = BCD4toWORD(year);
    month = BCDtoDEC(month);
    day = BCDtoDEC(day);

    if (year<1900) year+=2000;

    for (i=1900;i<year;i++)
         nday +=((((i%4 == 0) && (i%100 != 0) || (i%400 == 0))? 366: 365));
    for (i=1;i<month;i++)
         nday+=DaysAMonth(year,i);
    return(nday);
}

//************************************************
//计算某年某月某日是星期几（1900年开始）
//1-星期一,...,6-星期六,7-星期日
//year, month, day 均为BCD数据
BYTE GetWeekDay(WORD year,BYTE month,BYTE day)
{
    WORD nday = GetDaysFrom1900(year, month, day);
    nday = (nday + BCDtoDEC(day))%7;
	if (nday==0) nday = 7;

    return(nday);
}


/**
 *
 * 从键盘或者串口port读取数字字符串,按确认键结束, //
 * 输入的字符串存入EntryBuffer高地址区域 //
 *
 * @author EutronSoftware (2014-09-05)
 *
 * @param type :为输入的字符类型,'9'-数字,'A'-字符和数字,'*'
 * @param x,y 显示位置 //
 * @param maxLen 输入长度
 * @param port :>0,同时从串口输入数据,以0x0d结束 //
 *
 * @return short:=-1,取消输入;其他值为输入的字符串长度 //
 */

short GetStrFrKBD(BYTE type,BYTE x,BYTE y,BYTE maxLen,BYTE port)
{
    BYTE kCode,keyno;
    short cht;


    SetLoop();

    MemSet(EntryBuffer, ENTRYSIZE, ' ');
    AtEntryBuffer(0) = 0;
    Appl_EntryCounter = 0;
    for (;;)
    {
        if (KbHit())
        {
            keyno = Getch();
            cht=0;// 键盘输入 //
        }
        else if (port && CheckComm(port-1) && (cht=ReadComm(port-1))!=-1)
        {
            keyno=0xff;
            kCode = cht & 0xff;
            if (kCode == 0x0d)
                break;
        }
        else
            continue;

        if (keyno == ApplVar.AP.FirmKeys[ID_ENTER])
        {
            break;
        }
        else if (keyno == ApplVar.AP.FirmKeys[ID_CANCEL])
        {
            ClearEntry(0);
            //Appl_EntryCounter = 0;
            ResetLoop();
            return -1;
        }
        else if (keyno == ApplVar.AP.FirmKeys[ID_CLEAR])
            ClearEntry(0);
        else if (Appl_EntryCounter<maxLen)
        {
            if (type=='9')//输入数字
            {
                if (!cht)
                    kCode = NUMASCII[keyno];
                if (kCode>='0' && kCode<='9')
                    AppendEntry(kCode);
            }
            else if (type=='A' || type=='a' || type=='*')//输入字符
            {
                if (!cht)
                    kCode = ASCIIKEY[keyno];
                if ((kCode>='A' && kCode<='Z')||(kCode>='a' && kCode<='z'))
                    AppendEntry(kCode);
                else if (type=='*')
                {
                    if (!cht)
                        kCode = NUMASCII[keyno];
                    if (kCode>=0x20 && kCode<=0x7f)
                        AppendEntry(kCode);
                }
            }
        }
        DispStrXY(EntryBuffer + ENTRYSIZE - SCREENWD - 1 + x,x,y);
    }
    ResetLoop();
     return Appl_EntryCounter;

}

/**
 * 在指定位置输入Y或N,按确认结束
 *
 * @author EutronSoftware (2014-12-12)
 *
 * @param pMessage:提示信息
 * @param x ,y :信息显示位置
 * @param pDefault :默认值1-Yes;0-No
 *
 * @return BYTE :返回'Y'或'N',返回时,表示按CANCEL(退出)键
 */
BYTE InputYesNo(const char *pMessage,BYTE x,BYTE y,BYTE pDefault)
{
    char Yes_No,key;

    ClsArea(x,y,SCREENWD-x);
    DispStrXY((BYTE*)pMessage,x,y);
    Yes_No=pDefault?'Y':'N';
    do
    {
        if (Yes_No=='Y')
            DispStrXY(Msg[KAI].str ,SCREENWD-3,y);
        else
            DispStrXY(Msg[GUAN].str ,SCREENWD-3,y);
        WAIT_INPUT(key);
        if (key==ApplVar.AP.FirmKeys[ID_CANCEL])
        {
            Yes_No = 0;
            break;
        }
        else if (key==ApplVar.AP.FirmKeys[ID_SELECT])
            Yes_No ^= YN;
        else if (ASCIIKEY[key]=='y')
            Yes_No = 'Y';
        else if (ASCIIKEY[key]=='n')
            Yes_No = 'N';
    } while (key!=ApplVar.AP.FirmKeys[ID_ENTER]);
//ccr2016-08-18    ClsArea(x,y,SCREENWD-x);
    return Yes_No;
}
//ccr2016-01-12>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void ReadHeader()
{
    strcpy(ListRecord.Name,ApplVar.TXT.Header[ListRecord.Number]);
}
void ReadTrail()
{
    strcpy(ListRecord.Name,ApplVar.TXT.Trailer[ListRecord.Number]);
}
void ReadSlipHeader()
{
    strcpy(ListRecord.Name,ApplVar.TXT.SlipHeader[ListRecord.Number]);
}

CONST char *EcrGraphics[GRAPHMAX+1]={
        DEVICENULL,     //"无"
        GRAPHICSELF    ,//   "自制图片",
        GRAPHICCAFFE   ,//   "咖啡",
        GRAPHICDRESS   ,//   "服装",
        GRAPHICKNIFE   ,//   "刀叉",
        GRAPHICHIPPY   ,//   "嬉皮士",
        GRAPHICBREAD   ,//   "牛角面包",
        GRAPHICCHICKEN ,//   "烤鸡",
        GRAPHICROSE1   ,//   "玫瑰花",
        GRAPHICMUM     ,//   "菊花",
        GRAPHICICE     ,//   "冰淇淋",
        GRAPHICGIFT    ,//   "礼品",
        GRAPHICROSE2   ,//   "大玫瑰花",
        GRAPHICTREE    ,//   "圣诞树",
        GRAPHICHBREAD  ,//   "热面包",
        GRAPHICJOY     ,//   "游乐",
        GRAPHICFISH    ,//   "鲜鱼",
        GRAPHICCARTOON ,//   "卡通鸡",
        GRAPHICMONEY   ,//   "零花钱",
        GRAPHICEAT     ,//   "用餐",
        GRAPHICKITE    ,//   "放风筝",
        GRAPHICRUN     ,//   "跑步",
        GRAPHICREST    ,//   "户外休息"
};

void ReadGraphic()
{
    int sLen=strlen(GrapSet[0]);

    strcpy(ListRecord.Name,GrapSet[0]);
    sLen+=WORDtoASCL(ListRecord.Name+sLen,ListRecord.Number);
    //ListRecord.Name[sLen]=ListRecord.Number+'0';
    strcpy(ListRecord.Name+sLen,EcrGraphics[ApplVar.Graph[ListRecord.Number].PictNo]);
}
void ReadXZReport()
{
    strcpy(ListRecord.Name,XZTitle[ListRecord.Number].Name);
}
void ReadRepUnit()
{
    strcpy(ListRecord.Name,ApplVar.TXT.ReportType[ListRecord.Number]);
}
void ReadZone()
{
    BYTE sBCD[2];

    CWORD(sBCD[0])=ApplVar.AP.Zone.Start[ListRecord.Number];
    ListRecord.Name[0]=(sBCD[1]>>4)+'0';
    ListRecord.Name[1]=(sBCD[1] & 0x0f)+'0';
    ListRecord.Name[2]=':';
    ListRecord.Name[3]=(sBCD[0]>>4)+'0';
    ListRecord.Name[4]=(sBCD[0] & 0x0f)+'0';
    ListRecord.Name[5]=0;
}

//..........................................................
/**
 * 处理进入MG的功能
 *
 * @author EutronSoftware (2016-02-16)
 */
void MGFunctions(void)
{
    WORD sItems=ListItems(LISTMGFUNC,0,It_MGITEMS,false);

    switch (sItems)
    {
    case It_EXIT://ccr2016-01-18
        ClearVirtualInput();
        break;
    case (MsgMGPASSWORD-MsgMGADDINV+1): //MGPASSWORD 设置经理密码
        if (!SetPassword(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG))
            ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
        break;
    case (MsgMGCLOSEPRN-MsgMGADDINV+1): //MGCLOSEPRN 关闭打印
        if (WaitForYesNo(cCLOSEPRINTER,!TESTBIT(ApplVar.MyFlags,ENPRINTER)))
        {
#if (SCRSTATDBL)
            DispOnStatDBL((char *)Msg[DYJGUAN].str,0);
#else
            DispSt1C(Msg[DYJGUAN].str);//RG-DISPLAY
#endif
            SETBIT(ApplVar.MyFlags,ENPRINTER);
        }
        else
        {
#if (SCRSTATDBL)
            DispOnStatDBL((char *)Msg[DYJKAI].str,0);
#else
            DispSt1C(Msg[DYJKAI].str);//RG-DISPLAY
#endif
            RESETBIT(ApplVar.MyFlags,ENPRINTER);
        }
        break;
#if (!defined(DD_LOCK) || !defined(TR))
    case (MsgMGTRAINING-MsgMGADDINV+1)://MGTRAINING 培训模式
        if (WaitForYesNo(Msg[PSHMSHI].str,!TESTBIT(DOT,BIT1)))
        {//进入培训模式
#if (SCRSTATDBL)
            DispOnStatDBL((char *)Msg[PSHMSHI].str,0);
#else
            DispSt1C(Msg[PSHMSHI].str);//RG-DISPLAY
#endif
            SETBIT(DOT,BIT1);
            ApplVar.FTrain = 1;
            RESETBIT(ApplVar.Clerk.Options, BIT7);//training mode
        }
        else
        {//退出培训模式
            SETBIT(ApplVar.Clerk.Options, BIT7);
            RESETBIT(DOT,BIT1);
            ApplVar.FTrain = 0;
        }

        break;
#endif//ccr2017-02-27<<<<<<<<<<<<<<<
    case (MsgMGPRINTSTOCK-MsgMGADDINV+1)://MGPRINTSTOCK   打印单品库存
        if (ApplVar.AP.Plu.InvSize)
        {
            if (WaitForYesNo(Msg_DISPSTOCK,true))
            {//显示库存数据
                ListItems(MsgMGPRINTSTOCK,0,0,false);
            }
            else
            {//打印库存数据
                if (ApplVar.AP.Plu.RandomSize)
                    sItems=ApplVar.AP.Plu.RNumber;
                else
                    sItems=ApplVar.AP.Plu.Number;

                for (ApplVar.PluNumber=0;ApplVar.PluNumber<sItems;ApplVar.PluNumber++)
                {
                    ReadPlu();
                    RJPrint(0,DispQtyStr(ApplVar.Plu.Name, &ApplVar.Plu.Inventory,PRTLEN));
                }
            }
        }
        else
            DisplayPrompt((char*)cMsg_NOSTOCK);
        break;
    case (MsgMGGOTOMG-MsgMGADDINV+1)://MGGOTOMG   经理档
        break;
    }
}

/** ccr2016-01-18
 * 获取funcKey功能码在键盘中的位置
 *
 * @author EutronSoftware (2016-01-18)
 *
 * @param funcKey:功能吗
 *
 * @return BYTE:=0xff,没有发现此功能码,
 *              否则返回funcKey在键盘中的位置(0~n)
 */
BYTE PosOfFuncKey(WORD funcKey)
{
    short i;

    for (i=0;i<MAXKEYB-1;i++)
    {
        if (ApplVar.AP.KeyTable[i]==funcKey)
            return i;
    }
    return 0xff;
}


void ReadFlagsList()
{
    BYTE BitV, i,BitN;
    BYTE updownkey;
    short   sIdx;

    if (ListRecord.Number<SYSUSED)
    {
        sIdx = SysFlagUsed[ListRecord.Number].Index;
        memset(ListRecord.Name,' ',sizeof(ListRecord.Name));
        CopyFrStr(ListRecord.Name,SysFlagUsed[ListRecord.Number].Name);

        if (SysFlagUsed[ListRecord.Number].Bit)
        {
            BitV = BIT0<<(SysFlagUsed[ListRecord.Number].Bit-1);

            if ((ApplVar.AP.Flag[sIdx] & BitV))
                BitN = 'y' ^ SysFlagUsed[ListRecord.Number].Invert;
            else
                BitN = 'n' ^ SysFlagUsed[ListRecord.Number].Invert;

            if (BitN == 'y')
                strcpy(ListRecord.Name+SCREENWD-3,Msg[KAI].str);
            else
                strcpy(ListRecord.Name+SCREENWD-3,Msg[GUAN].str);
        }
        else
        {
            if (sIdx==62 || sIdx==CONTRAST)//复制收据次数或对比度
            {//高4位为标志,低4为数值  //
                i = ApplVar.AP.Flag[sIdx] & 0x0f;   //备份标志位
            }
            else
                i = ApplVar.AP.Flag[sIdx];
            WORDtoASC(ListRecord.Name+SCREENWD-1, i);
            //ccr2016-08-03>>>>>>>>>>>>>>>>>>
            if ((sIdx==10 || sIdx==11) && i && ListRecord.Name[SCREENWD-6]==' ')//PREFIX_1,PREFIX_2
                ListRecord.Name[SCREENWD-5]=i;
            //ccr2016-08-03<<<<<<<<<<<<<<<<<

            ListRecord.Name[SCREENWD] = 0;
        }
    }
}

//ccr2016-01-12<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

CONST TKEYDES EcrKeyDes[KEYDESMAX]={
		{PBF+1	  ,PBF+pbNumber     ,MsgKEYPBFUN  ,SETPBF},//"开台关帐\0开台增加\0暂结\0确认结账\0打印开台\0打印帐单\0取消确认\0分帐\0转帐\0桌帐转移\0人数"},
        {PORA	  ,PORA             ,MsgKEYPORA   ,0},//"出金入金"},
		{PORA+1	  ,PORA+poraNumber  ,MsgKEYPO_RA  ,SETPORA},//"出金\0入金"},
		{TEND+1	  ,TEND+tendNumber  ,MsgKEYTEND   ,SETTEND},//"现金\0支票\0信用卡\0购物券\0易通卡\0IC卡"},
        {WEIGHT   ,WEIGHT           ,MsgKEYWEIGHT ,0},//"称重"},       //cc 20060713
		{CLERK	  ,CLERK	        ,MsgKEYCLERKN ,0},//"收款员#"},
        {DEPT	  ,DEPT             ,MsgKEYDEPTNo ,0},//"部类#"},
		{DEPT+1	  ,DEPT+199         ,MsgKEYDEPT   ,SETDEPT},//"*"},
		{PLU1	  ,PLU1	            ,MsgKEYPLUNo  ,0},//"PLU"},
        {PLU1+1	  ,PLU1+39990       ,MsgKEYPLUDIR ,SETPLU},//"*"},
        {MODI	  ,MODI	            ,MsgKEYMEMO   ,0},//"说明"},
        {CORREC+1 ,CORREC+corrNumber,MsgKEYCORR   ,SETCORR},//"更改\0取消\0退货\0取消交易"},
        {CURR+1	  ,CURR+currNumber  ,MsgKEYCURR   ,SETCURR},//"美元\0日元\0港币"},
        {DISC+1	  ,DISC+discNumber  ,MsgKEYDISC   ,SETDISC},//"+%加成\0-%折扣\0金额加成\0金额折扣"},
        {SUSPEND  ,SUSPEND          ,MsgKEYSUSPEND,0},
        {MACROFUNC1,MACROFUNC20     ,MESSKEYMACRO ,SETKEYMACRO},
        {FUNCLOOK1,FUNCLOOK1        ,MsgFUNCLOOK1,0},
        {FUNCLOOK2,FUNCLOOK2        ,MsgFUNCLOOK2,0},
        {ODRAW    ,ODRAW	        ,MsgKEYDRAW   ,0},//"打开钱箱"},
        {CLEAR	  ,CLEAR	        ,MsgKEYCLEAR  ,0},//"清除"},
        {JPF	  ,JPF		        ,MsgKEYFEED   ,0},//"进纸"},
        {MULT	  ,MULT	            ,MsgKEYMULT   ,0},//"X"},
        {POINT    ,POINT            ,MsgKEYDOT    ,0},
        {POINT+1  ,POINT+1          ,MsgKEYZERO2  ,0},
        {POINT+2  ,POINT+11         ,MsgKEYNUMBER ,LISTNUMRIC},
        {SHIFT1	  ,SHIFT1	        ,MsgKEYSHIFT  ,0},//"SHIFT"},
        {DATETIME ,DATETIME	        ,MsgKEYDATE   ,0},//"日期"},
        {SUB	  ,SUB		        ,MsgKEYSUBTOT ,0},//"小计"},
        {NPRICE	  ,NPRICE	        ,MsgKEYPRICE  ,0},//"价格\0价格1\0价格2\0价格3\0价格4"},
        {LEVEL    ,LEVEL            ,MsgKEYPRICEN ,0},//"价格#"},      //cc 20060713
        {LEVEL1   ,LEVEL1           ,MsgKEYPRICE1 ,0},//"价格#"},      //cc 20060713
        {LEVEL2   ,LEVEL2           ,MsgKEYPRICE2 ,0},//"价格#"},      //cc 20060713
        {LEVEL3   ,LEVEL3           ,MsgKEYPRICE3 ,0},//"价格#"},      //cc 20060713
        {LEVEL4   ,LEVEL4           ,MsgKEYPRICE4 ,0},//"价格#"},      //cc 20060713
        {MODELOCK ,MODELOCK	        ,MsgMODELOCK  ,0},//""},
        {VIPLOGIN ,VIPLOGIN	        ,MsgVIPLOGIN  ,0},//""},
        {NUMBER2  ,NUMBER2          ,MsgINPUTNUM  ,0},//输入号码
        {CUSTOMER ,CUSTOMER         ,MsgCUSTOMER ,0},//输入顾客信息 ccr2016-09-13
#if defined(CASE_GPRS)
        {CLERK+1  ,CLERK+90         ,MsgKEYCLERK  ,0},//ccr2016-08-23>>>>>供显示下载的数据类型名称
        {DRAW+1	  ,DRAW+7           ,MsgKEYDRAWFUN,0},//"非销售\0支票\0信用卡\0购物券\0易通卡\0IC卡\0小费"},
        {RHEAD1	  ,RHEAD2           ,MsgRHEADER   ,0},//票头,
        {RTRAIL1  ,RTRAIL2          ,MsgRTRAILER  ,0},//票尾,
        {KEY0     ,KEY32            ,MsgKEYBOARD  ,0},//键盘,//ccr2016-08-23<<<<<<<<<
        {SYSFLG   ,SYSFLG           ,MsgSYSFLAGS  ,0},//系统设置
        {REMOTESETTIME,REMOTESETTIME,MsgSETDATE   ,0},//系统设置
#endif
        {0        ,0                ,MsgKEYNULL   ,0}

        //		{MODELOCK ,MODELOCK	        ,MsgKEYLOCK   ,0},//"方式锁"},
//		{SALPER	  ,SALPER	        ,MsgKEYSALPN  ,0},//"营业员#"},
//      {SALPER+1 ,SALPER+90        ,MsgKEYSALP   ,0},//"*"},          //cc 20060713
};
/**
 * 生成POINT+2 到POINT+11之间的数字
 *
 * @author EutronSoftware (2016-02-25)
 */
void ReadNumric()
{
    int i;

    i=WORDtoASCL(ListRecord.Name+1,ListRecord.Number)+1;
    ListRecord.Name[0]='\'';
    ListRecord.Name[i++]='\'';
    ListRecord.Name[i]=0;
}

void ReadKeyDesList()
{
    strcpy(ListRecord.Name,EcrKeyDes[ListRecord.Number].KeyType);
}

void ReadKeyMacro()
{
    strcpy(ListRecord.Name,ApplVar.AP.KeyMacros[ListRecord.Number].Name);
}
//
void ReadFuncLook()
{
    ECRFuncCodeToName(FuncLook[FuncLookNum][ListRecord.Number],ListRecord.Name);
}
/**ccr2016-02-04
 * 根据KeyDes述,获得一个键盘功能码
 *
 * @author EutronSoftware (2016-02-04)
 * @param keyName:用来保存功能码对应的名称
 * @return WORD :=0时,无键盘功能码,否则返回键盘功能码
 */
WORD SelectKeyCode(char *keyName)
{
    WORD sKeyCode,sKeyDes;

    sKeyDes = ListItems(LISTKEYDES,1,It_SELEFUNC,true);
    if (sKeyDes==It_EXIT || sKeyDes==0)
        return 0;
    else
    {
        sKeyDes--;
        if (EcrKeyDes[sKeyDes].SetupIDX==0)
        {
            if (keyName)
            {
                strcpy(keyName,EcrKeyDes[sKeyDes].KeyType);
            }
            return EcrKeyDes[sKeyDes].IndexS;
        }
        else
        {//列举键码明细
            sKeyCode = ListItems(EcrKeyDes[sKeyDes].SetupIDX,1,It_SELEFUNC,true);
            if (sKeyCode==It_EXIT || sKeyCode==0)
                return 0;
            else
            {
                sKeyCode--;
                if (keyName)
                {
                    strcpy(keyName,GetActiveItemStr());
                }
                return (EcrKeyDes[sKeyDes].IndexS+sKeyCode);
            }
        }
    }
}
/**
 * 打印键盘布局
 *
 * @author EutronSoftware (2016-05-25)
 */
void PrintKeyboard()
{
    WORD i;

    for (i=0;i<MAXKEYB && (ApplVar.AP.KeyTable[i] || ApplVar.AP.KeyTable[i+1]);i++)
    {
        memset(SysBuf,' ',PRTLEN);
        SysBuf[0]='K';SysBuf[1]='E';SysBuf[2]='Y';SysBuf[3]='-';
        WORDtoASCL(SysBuf+4,i+1);
        ECRFuncCodeToName(ApplVar.AP.KeyTable[i],SysBuf+7);
        RJPrint(0,SysBuf);
    }
    RFeed(3);
    SETBIT(ApplVar.MyFlags,PRNHEADMUST);
}
//自定义键盘
void SetKeyboard(void)
{
    WORD newKeyFunc;
    WORD keyID;
//    while (true)
    {
        mClearScreen();
        //DispSt1(Msg[GNMA].str);
        ApplVar.NumberEntry = SelectKeyCode(SysBuf);
        if (ApplVar.NumberEntry==0)
        {//是否打印键盘布局
           if  (WaitForYesNo(cPRINTKEYBOARD,false))
               PrintKeyboard();
           return;
        }

        DispStrXY(Msg[GNMA].str,0,0);
        DispStrXY(SysBuf,strlen(Msg[GNMA].str),0);//   DisplayDecXY(ApplVar.NumberEntry,SCREENWD-5,0,5);
        DispStrXY(Msg[AGNJIAN].str,0,1);
        WAIT_INPUT(ApplVar.KeyNo);
        ApplVar.Key.Code = ApplVar.AP.KeyTable[ApplVar.KeyNo];

        if (ApplVar.Key.Code < 200)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return;
        }

        if (ApplVar.NumberEntry != CLEAR && ApplVar.NumberEntry != MODELOCK)
        {
            keyID = ApplVar.KeyNo;
            memset(SysBuf,' ',SCREENWD);
            CopyFrStr(SysBuf,Msg[AJGNDYI].str);

            WORDtoASC(SysBuf+SCREENWD-1,keyID);
            //WORDtoASC(SysBuf+8,ApplVar.NumberEntry);
            ClearLine(1);       ClearLine(2);
            DispStrXY(SysBuf,0,1);
            DispStrXY(Msg[TCHBJI].str,0,SCREENLN-2);

            ApplVar.AP.KeyTable[ApplVar.KeyNo] = ApplVar.NumberEntry;


            if (ApplVar.AP.FirmKeys[ID_xPRINT] == ApplVar.KeyNo)
                ApplVar.AP.FirmKeys[ID_xPRINT] = 0xff;
            if (ApplVar.AP.FirmKeys[ID_xDEPT] == ApplVar.KeyNo)
                ApplVar.AP.FirmKeys[ID_xDEPT] = 0xff;
            else if (ApplVar.AP.FirmKeys[ID_DATE] == ApplVar.KeyNo)
                ApplVar.AP.FirmKeys[ID_DATE] = 0xff;
            else if (ApplVar.AP.FirmKeys[ID_RJFEED] == ApplVar.KeyNo)
                ApplVar.AP.FirmKeys[ID_RJFEED] = 0xff;
            else if (ApplVar.AP.FirmKeys[ID_CLEAR] == ApplVar.KeyNo)
                ApplVar.AP.FirmKeys[ID_CLEAR] = 0xff;
            else if (ApplVar.AP.FirmKeys[ID_LOCK] == ApplVar.KeyNo)
                ApplVar.AP.FirmKeys[ID_LOCK] = 0xff;

            switch (ApplVar.NumberEntry)
            {
            case CLERK:
                ApplVar.AP.FirmKeys[ID_xPRINT] = ApplVar.KeyNo;
                break;
            case SHIFT1:
                ApplVar.AP.FirmKeys[ID_xDEPT] = ApplVar.KeyNo;
                break;
            case DATETIME:
                ApplVar.AP.FirmKeys[ID_DATE] = ApplVar.KeyNo;
                break;
            case JPF:
            case RPF:
                ApplVar.AP.FirmKeys[ID_RJFEED] = ApplVar.KeyNo;
                break;
            case CLEAR:
                ApplVar.AP.FirmKeys[ID_CLEAR] = ApplVar.KeyNo;
                break;
            case MODELOCK:
                ApplVar.AP.FirmKeys[ID_LOCK] = ApplVar.KeyNo;
                break;
            }
            DeptPluKeysCount();
            Save_Config(true);
            Appl_ProgLine=0;
        }
    }
}
/**
 * 将收款机功能码转换为对应的功能名称
 *
 * @author EutronSoftware (2016-02-25)
 *
 * @param fCode :功能码
 * @param fName :用于存储功能名称
 */
void ECRFuncCodeToName(WORD fCode,char *fName)
{
    int sIdx;

    for (sIdx=0;sIdx<KEYDESMAX ;sIdx++)
    {
        if (fCode>=EcrKeyDes[sIdx].IndexS && fCode<=EcrKeyDes[sIdx].IndexE)
        {
            if (EcrKeyDes[sIdx].IndexS==EcrKeyDes[sIdx].IndexE || EcrKeyDes[sIdx].SetupIDX==0)
            {//只有一个独立名称
                strcpy(fName,EcrKeyDes[sIdx].KeyType);
            }
            else// if (KeyDes[sIdx].SetupIDX)
            {
                if (!ReadItemCaption(EcrKeyDes[sIdx].SetupIDX,fCode-EcrKeyDes[sIdx].IndexS,fName))
                {
                    strcpy(fName,EcrKeyDes[sIdx].KeyType);
                    sIdx=strlen(fName);
                    sIdx+=WORDtoASCL(fName+sIdx,fCode-EcrKeyDes[sIdx].IndexS+1);
                    fName[sIdx]=0;
                }
            }
            return;
        }
    }
    strcpy(fName,EcrKeyDes[KEYDESMAX-1].KeyType);//无对应名称
}


/**
 * 将收款机功能码转换为对应的功能类名称
 *
 * @author EutronSoftware (2016-02-25)
 *
 * @param fCode :功能码
 * @param fName :用于存储功能类名称
 *
 * @return WORD
 *         :=0,无对应功能名称;>0,为功能名称序号
 */
WORD ECRFuncCodeToTypeName(WORD fCode,char *fName)
{
    WORD sIdx;

    for (sIdx=0;sIdx<KEYDESMAX ;sIdx++)
    {
        if (fCode>=EcrKeyDes[sIdx].IndexS && fCode<=EcrKeyDes[sIdx].IndexE)
        {
            strcpy(fName,EcrKeyDes[sIdx].KeyType);
            return sIdx+1;
        }
    }
    strcpy(fName,EcrKeyDes[KEYDESMAX-1].KeyType);//无对应名称
    return 0;
}
/**
 * 等待输入Y或N,按确认结束
 *
 * @author EutronSoftware (2016-2-15)
 *
 * @param pMessage:提示信息
 * @param pDefault :默认值1-Yes;0-No
 *
 * @return BYTE :返回True-Y,返回False-N
 */
BYTE WaitForYesNo(const char *pMessage,BYTE pDefault)
{
    BYTE Yes_No,key,ynX;
    short pwdY;
    char mess[SCREENWD];

    //在屏幕中央显示验证密码框
    pwdY = (SCREENLN-1-3)/2;
    mDrawABox(pwdY,pwdY+2);
    mDispStringXY((BYTE*)pMessage,CenterForDisplay(SCREENWD-2,strlen(pMessage))+1,pwdY);
    pwdY++;//指向密码输入行

    Yes_No=strlen(MsgCONFIRM);
    if (Yes_No>=SCREENWD-2)
        Yes_No=SCREENWD-2;
    memcpy(mess,MsgCONFIRM,Yes_No);
		Yes_No--;
		key=mess[Yes_No];
    if (key==':' || key=='.' || key==' ')
			mess[Yes_No]='?';
		else if (key!='?')
			mess[++Yes_No]='?';
    mess[++Yes_No]=0;

    ynX=CenterForDisplay(SCREENWD-6,Yes_No)+1;

    mDispStringXY((BYTE*)mess,ynX,pwdY);
#if defined(CASE_FORHANZI)
        key=SCREENWD-3;
#else
        key=SCREENWD-4;
#endif
    ynX+=(Yes_No+1);
    if (ynX>key)
        ynX=key;
    //mDispStringXY((BYTE*)mess,1,pwdY);
    Yes_No=pDefault?'Y':'N';
    do
    {
        if (Yes_No=='Y')
            mDispStringXY((BYTE*)Msg[KAI].str ,ynX,pwdY);
        else
            mDispStringXY((BYTE*)Msg[GUAN].str ,ynX,pwdY);
        WAIT_INPUT(key);
        if (key==ApplVar.AP.FirmKeys[ID_CANCEL])
        {
            Yes_No = 'N';
            break;
        }
        else if (key==ApplVar.AP.FirmKeys[ID_SELECT])
            Yes_No ^= YN;
        else if (ASCIIKEY[key]=='y')
            Yes_No = 'Y';
        else if (ASCIIKEY[key]=='n')
            Yes_No = 'N';
    } while (key!=ApplVar.AP.FirmKeys[ID_ENTER]);
    mDrawABox(0,0);//mRefreshLine(pwdY-1); mRefreshLine(pwdY++); mRefreshLine(pwdY);
    return (Yes_No=='Y');
}

void PrintGraphics()
{
#if !defined(CASE_INNERDOTPRN)
    int i;
    for (i=1;i<GRAPHMAX+1;i++)
    {
        memset(SysBuf,' ',PRTLEN);
        strcpy(SysBuf,"Graphic:");
        SysBuf[8]=' ';
        WORDtoASC(SysBuf+12,i);
        SysBuf[13]=0;
        RJPrint(0,SysBuf)
        Bios(BiosCmd_PrintGraph, (void*)(i), 0 , 0); //Stampa msg ram
    }
#endif
}
/**
 * 输入WORD,输入数据由ApplVar.EntryNumber送出
 * 调用前设置ApplVar.EntryNumber作为输入初始值
 * @author EutronSoftware (2016-02-29)
 *
 * @param x ,y:输入数据显示位置,输入内容显示在屏幕的右边
 * @param vMax:最大值
 *
 * @return BYTE :为退出此输入时的按键
 */
BYTE InputWord(BYTE x,BYTE y,WORD vMax)
{
    WORD keyCode;
    BYTE keyNo;

    ClearEntry(0);
    while (true)
    {
        SetLoop();
        while (!KbHit()){CheckError(0);};
        keyNo=Getch();

        ResetLoop();
        keyCode=ApplVar.AP.KeyTable[keyNo];
        if (keyCode==CLEAR)
        {
            ApplVar.ErrorNumber=0;
            ClearEntry(0);
        }
        else if (ApplVar.ErrorNumber==0)
        {
            if (keyCode>='0' && keyCode<='9')
                AppendEntry(keyCode);
            else
                return keyNo;
            GetEntry();
            if (ApplVar.NumberEntry>vMax)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI18);
        }
        DispStrXY(EntryBuffer + ENTRYSIZE - SCREENWD - 1 + x,x,y);
    }
}
/**
 * 根据显示宽度,计算字符串的居中位置
 *
 * @author EutronSoftware (2016-03-03)
 *
 * @param width
 * @param strL
 *
 * @return BYTE
 */
BYTE CenterForDisplay(BYTE width,BYTE sLen)
{
    if (sLen>width)
        return 0;
    else
        return (width-sLen)/2;
}
//////////////////////////////////////////////////////////////////


