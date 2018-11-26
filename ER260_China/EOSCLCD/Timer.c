#define TIMER_C	1
#include "king.h"				/* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

/***************************************************************/
//pAll==true,强制进行日期时间转换
//pAll bit7=1,不读取日期时间,直接使用Now进行转换
void CheckTime(BYTE pAll)
{
    static struct TimeDate priDate={0xff};
    short  d;
    BYTE t;
    char dispBuff[SCREENWD+1];//sizeof(dispBuff)必须>14

    if (pAll<0x80)
        GetTimeDate(&Now);  /* read timer */
//    if (ApplVar.FRegi)   /* in transaction ? */
//        return;

    if (pAll)
        priDate.day = priDate.sec = 0xff;
    else if (!memcmp(&priDate,&Now,sizeof(priDate)))
        return;

    if (priDate.sec != Now.sec || priDate.min != Now.min || priDate.hour != Now.hour)
    {
        t = Now.hour;

        if (TESTBIT(AM_PM, BIT0))   /* AM-PM */
        {
            //hh:mm:ssP
    //		if (Now.hour > 0x11)
            if (Now.am_pm)   //cc 20070730
                TimeAsci[8] = 'P';
            else
                TimeAsci[8] = 'A';
            if (Now.hour == 0x20 || Now.hour == 0x21)
                t -= 0x18;
            else if (Now.hour > 0x12)
                t -= 0x12;
            else if (!Now.hour)
                t = 0x12;
        }
        else
            TimeAsci[8] = ' ';
        TimeAsci[0] = (t >> 4)+'0';
        TimeAsci[1] = (t & 0xf)+'0';
        TimeAsci[2] = ':';
        TimeAsci[3] = (Now.min >> 4)+'0';
        TimeAsci[4] = (Now.min & 0x0f)+'0';
        TimeAsci[5] = ':';      /* use '-' for MA  numeric display */
        TimeAsci[6] = (Now.sec >> 4)+'0';
        TimeAsci[7] = (Now.sec & 0xf)+'0';
        TimeAsci[9] = 0;

        if (TESTBIT(AM_PM, BIT0))   /*AM-PM */
        {
            TimeAsci[5] = ' ';
            TimeAsci[6] = TimeAsci[8];
            TimeAsci[7] = 'M';
            TimeAsci[8] = 0;
        }
    }

    if (priDate.day != Now.day || priDate.month != Now.month || priDate.year != Now.year)
    {
        MemSet(DateAsci, sizeof(DateAsci), ' ');
        //copy WeekDay to DateAsci
#if defined(CASE_FORHANZI)
        DateAsci[0] = Msg[Now.dow+MONDAY-1].str[4];      /* Day of week */
        DateAsci[1] = Msg[Now.dow+MONDAY-1].str[5];      /* Day of week */
        DateAsci[2]=',';
#else
        DateAsci[0] = Msg[Now.dow+MONDAY-1].str[0];      /* Day of week */
        DateAsci[1] = Msg[Now.dow+MONDAY-1].str[1];      /* Day of week */
        DateAsci[2] = Msg[Now.dow+MONDAY-1].str[2];      /* Day of week */
        DateAsci[3]=',';
#endif
        if (TIME_DATE == 1)          /*"mon,MM DD YYYY" */
            d = 7;
        else if (TIME_DATE == 2)     /*"mon,YYYY MM DD" */
            d = 12;
        else
            d = 4;            /* default"mon,DD MM YYYY" */
        //copy DAY to DateAsci
        DateAsci[d] = (Now.day>>4)+'0';
        DateAsci[d+1] = (Now.day & 0x0f)+'0';
        if (d>4)
            DateAsci[d-1]='-';/* -dd */

        Now.year &= 0x00ff;
        Now.year |= YEARFrHEX;

        if (TIME_DATE == 1)          /*"mon,MM DD YYYY" */
            d = 4;
        else if (TIME_DATE == 2)     /*"mon,YYYY MM DD" */
            d = 9;
        else
            d = 7;                   /* default"mon,DD MM YYYY" */
        //copy MONTH to DateAsci
        DateAsci[d] = (Now.month>>4)+'0';
        DateAsci[d+1] = (Now.month & 0x0f)+'0';
        if (d>4)
            DateAsci[d-1]='-';/* -mm */

        if (TIME_DATE == 2)     /*"mon,YYYY MM DD" */
            d = 4;
        else
        {
            d = 10;            /* default"mon,DD MM YYYY" or "mon,MM DD YYYY"*/
            DateAsci[9] = '-';/* -yyyy */
        }
        //copy YEAR to DateAsci
        DateAsci[d++] = (Now.year>>12)+'0';
        DateAsci[d++] = ((Now.year>>8) & 0x0f)+'0';
        DateAsci[d++] = ((Now.year>>4) & 0x0f)+'0';
        DateAsci[d++] = (Now.year & 0x0f)+'0';
    }

    if (ACTIVE && InActive > ACTIVE
        || ApplVar.CentralLock == SET || ApplVar.CentralLock == Z || ApplVar.CentralLock == X)     /* Inactive longer then allowed */
    {

        if (ACTIVE && (InActive & 0x7f) > ACTIVE)
        {

            if (InActive & 0x80)
            {
                memcpy(dispBuff, DateAsci+4, 10);
                dispBuff[10]=0;
            }
            else
            {
                memcpy(dispBuff, TimeAsci, 8);
                dispBuff[8]=0;
            }

            //DispSt3(dispBuff);	//hf 20060105
#if (SCRSTATDBL)
            if (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG)
                DispOnStatDBL(dispBuff,0);
            else
#endif
            {
                ClsState12();//ccr2016-08-15
                DispStrXY(dispBuff,SCREENWD-strlen(dispBuff),STATELINE);//ccr2016-08-15 DispSt2(dispBuff);//RG-DISPLAY
            }

            if (TESTBIT(ApplVar.ScreenStat,LIGHTON))
            {
                mLightLCDOff();
#if !defined(CASE_ASCIIDISPLAY)  && !defined(DEBUGBYPC)
                LED_Off();
#endif
            }
        }
        memset(dispBuff,' ',DISLEN);
#ifdef CASE_ER260
        memcpy(dispBuff+DISLEN-8,TimeAsci,8);
#else
        memcpy(dispBuff+(DISLEN-8)/2,TimeAsci,8);
#endif
        PutsO(dispBuff);//PutsO(TimeAsci);
        //if (ACTIVE && InActive > ACTIVE)
    }
    //else if (min != Now.min)	//hf
    if (priDate.min != Now.min && (InActive & 0x7f)<0x70)
    {
            InActive++;
    }
    priDate = Now;
}

/* set up correct hour, day and month pointer */

void GetTimeZones()
{
    WORD now;

    now = (Now.hour * 0x0100) + Now.min;
    if (ApplVar.AP.Zone.Number)
        ApplVar.Zone = ApplVar.AP.Zone.Number - 1;
    else
        ApplVar.Zone = 0;
    while (ApplVar.Zone)
    {
        if (now < ApplVar.AP.Zone.Start[ApplVar.Zone])
            ApplVar.Zone--;
        else
            break;
    }
    ApplVar.Day = Now.dow;
    if (ApplVar.Day >= ApplVar.AP.Day.Number)
        ApplVar.Day = 0;
    /* used for salesperson */
    ApplVar.Month = Now.month;
    if (ApplVar.Month > 9)
        ApplVar.Month -= 6;
    if (ApplVar.Month > ApplVar.AP.Month.Number)
        ApplVar.Month = 0;
    else
        ApplVar.Month--;

}
//------------------------------------------------------
//type=1,Set date;=2,set time;
//type =0xf?, get Now from input only
//type =0x0?, get Now from input only and set date time
void NewTimeDate(BYTE type)
{
    short i;
    //BYTE active = ACTIVE & 0x7f;	//test

    if (!(type & 0xf0))
        GetTimeDate(&Now);      /* restore old values */
    StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);

    if ((type & 0x0f) == 1)   /* set date */
    {
        //Format for date string:'ddmmyyyy' or 'mmddyyyy' or 'yyyymmdd'

        if (Appl_EntryCounter == 8)
        {
            switch (TIME_DATE)
            {
            case 1:/*"Mon,MM-DD-YYYY" */
                Now.year = ((WORD)ApplVar.Entry.Value[1] << 8) |  ApplVar.Entry.Value[0];
                Now.month = ApplVar.Entry.Value[3];
                Now.day = ApplVar.Entry.Value[2];
                break;
            case 2:/*"Mon,YYYY-MM-DD" */
                Now.year = ((WORD)ApplVar.Entry.Value[3] << 8) |  ApplVar.Entry.Value[2];
                Now.month = ApplVar.Entry.Value[1];
                Now.day = ApplVar.Entry.Value[0];
                break;
            default:/*"Mon,DD-MM-YYYY" */
                Now.year = ((WORD)ApplVar.Entry.Value[1] << 8) |  ApplVar.Entry.Value[0];
                Now.month = ApplVar.Entry.Value[2];
                Now.day = ApplVar.Entry.Value[3];
                break;
            }

            if (Now.month==0 || Now.month > 0x12
                || BCDtoDEC(Now.day) > DaysAMonth(BCD4toWORD(Now.year),BCDtoDEC(Now.month)))
            {
                GetTimeDate(&Now);
                ApplVar.ErrorNumber=ERROR_ID(CWXXI02);
                return;
            }

    //        Now.dow = GetWeekDay(Now.year,Now.month,Now.day);
            if (type<0x0f)
                InActive = 0x80 ;   //hf 20060406 0x80 + ACTIVE + 1
        }
#if (1)
        else if (Appl_EntryCounter == 4)
        {//ccr2016-06-01 只输入月和日>>>>>
            switch (TIME_DATE)
            {
            case 1:/*"Mon,MM-DD-YYYY" */
                Now.month = ApplVar.Entry.Value[2];
                Now.day = ApplVar.Entry.Value[1];
                break;
            case 2:/*"Mon,YYYY-MM-DD" */
                Now.month = ApplVar.Entry.Value[1];
                Now.day = ApplVar.Entry.Value[0];
                break;
            default:/*"Mon,DD-MM-YYYY" */
                Now.month = ApplVar.Entry.Value[1];
                Now.day = ApplVar.Entry.Value[2];
                break;
            }

            if (Now.month==0 || Now.month > 0x12
                || BCDtoDEC(Now.day) > DaysAMonth(BCD4toWORD(Now.year),BCDtoDEC(Now.month)))
            {
                GetTimeDate(&Now);
                ApplVar.ErrorNumber=ERROR_ID(CWXXI02);
                return;
            }
        }//ccr2016-06-01<<<<<<<<<<<<<<<<
#endif
        else
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI02);
            return;
        }
    }
    else if ((type & 0x0f) == 2)//Set time
    {
        switch (Appl_EntryCounter)
        {
        case 3:
        case 4:
        case 5:
            i = 2;
            Now.sec = 0;
            Now.min = ApplVar.Entry.Value[0];
            Now.hour = ApplVar.Entry.Value[1];
            break;
        case 6:
        case 7:
            Now.sec = ApplVar.Entry.Value[0];
            Now.min = ApplVar.Entry.Value[1];
            Now.hour = ApplVar.Entry.Value[2];
            i = 3;
            break;
        default:
            ApplVar.ErrorNumber=ERROR_ID(CWXXI03);
        }
        if (TESTBIT(AM_PM, BIT0))     /* am - pm */
        {
            if (!Now.hour || Now.hour > 0x12)        /* hour must be 1 - 12 */
                ApplVar.ErrorNumber=ERROR_ID(CWXXI03);
            else if (ApplVar.Entry.Value[i] && ApplVar.Entry.Value[i] != 0x01)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI03);
            else if (Now.hour == 0x12)
            {
                if (!ApplVar.Entry.Value[i])
                    Now.hour = 0;
            }
            else if (ApplVar.Entry.Value[i])
            {
                if (Now.hour == 0x08 || Now.hour == 0x09)
                    Now.hour += 0x18;
                else
                    Now.hour += 0x12;
            }
        }
        else if (Now.hour > 0x23)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI03);
        if (Now.min > 0x59 || Now.sec > 0x59)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI03);
        if (type<0x0f)
            InActive = 0;   //hf 20060406InActive = ACTIVE + 1;
    }
    else
        return;

    if (!ApplVar.ErrorNumber && type <0x0f)
        SetTimeDate((type & 0x0f)-1,&Now);
    else if (!(type & 0xf0))
        GetTimeDate(&Now);
}

//<<<<<<<<<<<<<<<<<<<<
