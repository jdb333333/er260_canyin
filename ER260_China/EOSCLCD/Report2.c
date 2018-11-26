#define REPORT2_C
#include "king.h"				/* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "message.h"

/*****************************************************************/

WORD SetUpReport()
{
    if (ApplVar.Report.Type > REPORTTYPE_MAX || ApplVar.Report.Period > 3 || ApplVar.Report.PointerType > (REPDEFMAX-1))
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI26);
        return 1;
    }
    switch(ApplVar.Report.PointerType)
    {
        case 1:     /* ApplVar.Clerk report */
            ApplVar.Report.PointerEnd = ApplVar.AP.Clerk.Number;
			if (ApplVar.ClerkNumber)				//ccr 040513
				ApplVar.Report.Pointer = ApplVar.ClerkNumber - 1;
			else							//ccr 040513
				ApplVar.Report.Pointer = 0;			//ccr 040513
            break;
        case 2:     /* Time ApplVar.Zone report report */
            ApplVar.Report.PointerEnd = ApplVar.AP.Zone.Number;
            ApplVar.Report.Pointer = ApplVar.Zone;
            break;
        case 3:     /* ApplVar.Day report */
            ApplVar.Report.PointerEnd = ApplVar.AP.Day.Number;
            ApplVar.Report.Pointer = ApplVar.Day;
            break;
        case 4:     /* ApplVar.Month report */
            ApplVar.Report.PointerEnd = ApplVar.AP.Month.Number;
            ApplVar.Report.Pointer = ApplVar.Month;
            break;
        case REPDEFMAX-1:     /* saler report */
#if (salNumber)
			if (ApplVar.AP.SalPer.Number)
			{
	            ApplVar.Report.PointerEnd = ApplVar.AP.SalPer.Number;
				if (ApplVar.SalPerNumber)				//ccr 040514
	    	        ApplVar.Report.Pointer = ApplVar.SalPerNumber - 1;
				else							//ccr 040514
	    	        ApplVar.Report.Pointer = 0;			//ccr 040514
			}
       	    break;
#endif
        default :  /* standard report */
            ApplVar.Report.PointerEnd = 1;
            break;
    }
    if (ApplVar.RepComputer)
        ApplVar.Report.Pointer = 0;
    if (ApplVar.Report.Pointer > ApplVar.Report.PointerEnd)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI26);
        return 1;
    }
	ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
    ApplVar.SaveClerk = ApplVar.ClerkNumber;
	ApplVar.SaveSalPer = ApplVar.SalPerNumber;
    return 0;
}


void MakeTime(char *to,WORD time)
{
    if (TESTBIT(AM_PM, BIT0)) // am - pm
    {
        *(to + 6) = 'M';
        if (time > 0x1159)
            *(to + 5) = 'P';
        else
            *(to + 5) = 'A';
        if (time > 0x1959 && time < 0x2200)
            time -= 0x1800;
		else if (time > 0x1259)
			time -= 0x1200;
        else if (!time)
            time = 0x1200;
    }
    HEXtoASC(to, ((BYTE *)&time) + 1, 1);
    *(to + 2) = ':';
    HEXtoASC(to + 3, (BYTE *)&time, 1);
    if (*to == '0')
        *to = ' ';
}

void PrintPointType()
{
    char *s;

    MemSet(SysBuf+7, 24, ' ');  /* clear record */
    switch(ApplVar.Report.PointerType)
    {
        case 0:
            s = 0;
            break;
        case 1:         /* clerk */
            ApplVar.ClerkNumber = ApplVar.Report.Pointer + 1;
            ReadClerk();
            s = ApplVar.Clerk.Name;
            break;
        case 2:         /* hour */
            SysBuf[25] = 0;
            SysBuf[15] = '-';
            SysBuf[16] = '-';
            MakeTime(SysBuf + 7, ApplVar.AP.Zone.Start[ApplVar.Report.Pointer]);
            if (ApplVar.Report.Pointer == (ApplVar.AP.Zone.Number - 1))
                MakeTime(SysBuf + 18, ApplVar.AP.Zone.Start[0]);
            else
                MakeTime(SysBuf + 18, ApplVar.AP.Zone.Start[ApplVar.Report.Pointer + 1]);
            s = SysBuf + 7;
            break;
        case 3:         /* day */
            s = (char*)Msg[ApplVar.Report.Pointer+MONDAY].str;
            break;
        case 4:         /* month */
            s = (char*)Msg[ApplVar.Report.Pointer+JANUARY].str;
            break;
        case REPDEFMAX-1:         /* saler */
#if (salNumber)
			if (ApplVar.AP.SalPer.Number)
			{
	            ApplVar.SalPerNumber = ApplVar.Report.Pointer + 1;
    	        ReadSalPer();
        	    s = ApplVar.SalPer.Name;
			}
            break;
#else
            s = 0;
#endif
		default:
            return;
    }
    if (ApplVar.RepComputer)
    {
        *((WORD *)SysBuf) = 3;
        SysBuf[2] = 1;      /* indicate text */
        SysBuf[3] = ApplVar.Report.Period;
        SysBuf[4] = ApplVar.Report.PointerType;
        *((WORD *)(SysBuf + 5)) = ApplVar.Report.Pointer;
        if (s)
            CopyFrStr(SysBuf + 7, s);
    }
    else
    {
        if (s)
        {
            PrintStr(s);
            PrintLine('=');
        }
    }
}


/* print z-count and or message at report end */

void ReportEnd(BYTE AddZ)
{
    ApplVar.ClerkNumber = ApplVar.SaveClerk;        /* restore clerk */
    ReadClerk();

#if (salNumber)
	ApplVar.SalPerNumber = ApplVar.SaveSalPer;
	ReadSalPer();
#endif
    if (!TESTBIT(KEYTONE, BIT3))   /* print always on Journal ? */
        SETBIT(ApplVar.PrintLayOut, BIT0);  /* always on journal */

    if (ApplVar.RepComputer)
        ApplVar.PrintLayOut &= 0x01;        /* only journal */
    if (ApplVar.ErrorNumber == ERROR_ID(CWXXI27))
        PrintMessage(CWXXI27);

    if (ApplVar.Report.Type < 12 && AddZ)
    {
        ApplVar.SaleQty = ZERO;
        memcpy(ApplVar.SaleQty.Value, ApplVar.ZCount, ZSIZE);
        if (ApplVar.FReport == Z)
        {
            if (ApplVar.Report.PointerType == 1 && TRAINING)   /* training clerk ? */
            {
            }/* not add with training clerk */
            else
            {
               Add(&ApplVar.SaleQty, (BCD*)&ONE);        /* add to z -count */
               memcpy(ApplVar.ZCount, ApplVar.SaleQty.Value, ZSIZE);
            }
        }

        if (ApplVar.FReport==Z)//ccr2016-04-11
            PrintQty(Msg[QZHJSHU].str, &ApplVar.SaleQty);        /* print z-count */

        ApplVar.SaleQty = ZERO;
    }

    MemSet(SysBuf, PRTLEN, ' ');

    if (ApplVar.Report.System & 0x01)
        CopyFrStr(SysBuf, Msg[SBAOBIAO].str);
    else
        CopyFrStr(SysBuf, Msg[BAOBIAO].str);
//ccr2016-03-29    SysBuf[8] = ':';

    WORDtoASC(SysBuf + 9,ApplVar.ReportNumber);//ccr2014-09-05 PLUCODE_X
//ccr2014-09-05    memcpy(SysBuf + 9, &EntryBuffer[ENTRYSIZE - 6], 5);

    if (ApplVar.FReport == Z && ApplVar.Report.Type < 12 && ApplVar.ErrorNumber != ERROR_ID(CWXXI27))
    {
        CopyFrStr(SysBuf+16, Msg[QINGCHU].str);
    }
    else
        SysBuf[16] = 'X';
    PrintStr(SysBuf);

    ReceiptIssue(1);
	if (!ApplVar.RepComputer || (ApplVar.RepComputer && ApplVar.FReport == Z))
	{

	    if (TESTBIT(CLERKFIX, BIT0+BIT5) && !ApplVar.ClerkLock)    /* clerk compulsory or secret, no key */
		{
    	    ApplVar.ClerkNumber = 0;
			DispSt1C(Msg[CWXXI33].str); //hf 20060105 DispSt2C(Msg[CWXXI33].str);
		}
	}
#if (salNumber)
	if (TESTBIT(KEYTONE, BIT6))		/* sales person compulsory */
		ApplVar.SalPerNumber = 0;
	else if (TESTBIT(AM_PM, BIT4))		/* sales person to 1 */
	{
		ApplVar.SalPerNumber = 1;
		ReadSalPer();
	}
#endif
    RESETBIT(ApplVar.MyFlags,CLSSCREEN);//ccr2014-09-09
}

void PrintReportHeader()
{
    BYTE save;

    PrintLine(' ');

    if (ApplVar.Report.Type >= 12)
        memcpy(SysBuf + 4, ApplVar.TXT.ReportType[ApplVar.Report.Type], sizeof(ApplVar.TXT.ReportType[0]));
    else
    {
        CopyFrStr(SysBuf+1, Msg[ApplVar.Report.Period + RI].str);
		if (!(ApplVar.Report.PointerType == 5 && ApplVar.AP.SalPer.Number))
        	CopyFrStr(SysBuf + 8, Msg[ApplVar.Report.PointerType + BIAOZHUN].str);
        CopyFrStr(SysBuf + 18, Msg[BAOBIAO].str);
    }
    save = ApplVar.PrintLayOut;
    ApplVar.PrintLayOut &= 0xf8;
    PrintStr(SysBuf);   /* not double on slip */
    ApplVar.PrintLayOut = save & 0xe7;
    SETBIT(ApplVar.PrintLayOut, BIT2);      /* Double size */
    PrintStr(SysBuf);
    ApplVar.PrintLayOut = save;
    PrintLine('=');
}

void ClearAllReport()
{
    WORD  saveclerk ;

    saveclerk = ApplVar.ClerkNumber ;
    ApplVar.FReport = Z;
    ApplVar.PrintLayOut = 0;
    MemSet(&ApplVar.Report, sizeof(ApplVar.Report), 0);
    for (ApplVar.Report.Period = 0;ApplVar.Report.Period < 4;ApplVar.Report.Period++)
      for (ApplVar.Report.PointerType = 0;ApplVar.Report.PointerType < REPDEFMAX;	ApplVar.Report.PointerType++)
	    for (ApplVar.Report.Type = 0;ApplVar.Report.Type < REPORTTYPE_MAX; ApplVar.Report.Type++)	//lyq 13
	    {
		    if (ApplVar.Report.Type == 12 && //lyq
			    (ApplVar.Report.Period || ApplVar.Report.PointerType))
			    break;
		    SysBuf[0] = ' ';
		    SysBuf[1] = ApplVar.Report.Period + '0';
		    SysBuf[2] = ApplVar.Report.PointerType + '0';
		    SysBuf[3] = ApplVar.Report.Type / 10 + '0';
		    SysBuf[4] = (ApplVar.Report.Type % 10) + '0';
		    SysBuf[5] = 0;
		    ClsState2();
		    DispSt2(SysBuf);
		    GetSystemReport(1); /* clear all */
	    }

    ApplVar.ClerkNumber = saveclerk ;
    ReadClerk() ;

    MemSet(ApplVar.ZCount, ZSIZE, 0);
    MemSet(ApplVar.ReceiptNumber, RECNUM, 0);     /* reset receiptnumber ?*/
    ApplVar.FReport = 0;
    ApplVar.PrintLayOut = 0x05;
    if (ApplVar.AP.Plu.Number && ApplVar.AP.Plu.InvSize)	/* reset inventory */
    {
        ApplVar.PluInventory = ZERO;
		for (ApplVar.PluNumber = 0; ApplVar.PluNumber < ApplVar.AP.Plu.Number; ApplVar.PluNumber++)
		    WritePluInventory();	/* write zero */
    }
    ClsState2();
    DispSt1(Msg[BBQQING].str);//hf 20060105 DispSt2(Msg[BBQQING].str);
    PrintStr(Msg[BBQQING].str);
	SETBIT(ApplVar.MyFlags,PRNTRAI);
	ApplVar.PrintLayOut = 0x02;				//force single heigth
	RESETBIT(ApplVar.MyFlags,ZREPORT);
}
//ccr2016-02-22>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/**
 * 显示XZ档下的菜单
 *
 * @author EutronSoftware (2016-02-22)
 * @param xzType :X or Z
 */
void DisplayXZMenu(BYTE xzType)
{
    if (xzType == X)
    {
        SetList.MsgIdx = MENUXREPORT;
        SetList.RecMax = MENUXITEMMAX;
    }
    else
    {
        SetList.MsgIdx = MENUZREPORT;
        SetList.RecMax = MENUZITEMMAX;
    }
    SetList.Cols = 1;
    SetList.Rows = SCREENLN-TESTBIT(ApplVar.ScreenStat,STATEON);
    SetList.Rec1st = 0;
    SetList.Var = &ApplVar.ReportNumber;
    SetList.isSub = false;   // main menu
    ApplVar.ReportNumber = 1;
    SetupList = &DisplaySetup;
    DisplaySetup(0xff);//将默认的菜单项显示在第一行
}

CONST MenuDefine ItemXReport[]={
    {MENUXREPORT,SETREPORT},
#if (CASE_USBDISK)
    {MENUXEXPORT,0},  //导出报表数据
#endif
    {MENUQUICKVIEW,0},      //Quick View
    {MENUXEJFUNC,MsgALLLOG},       //LOG. OPERATION
#if defined(CASE_GPRS)
    {MENUSENDNEWLOG,0},     //发送新流水数据
#endif
#if !defined(DD_LOCK)
    {MENUXPASSWORD,0},     //设置X密码
#endif
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    {MENUXEJOURNAL,MsgEJMESSAGES}, // 打印EJ报表
#endif
    {0,0}
};

/**
 * 处理X下菜单所对应的功能
 *
 * @author EutronSoftware (2016-02-23)
 *
 * @param xFunc :X下的功能号(MENUXREPORT-MENUXPASSWORD)
 */
void ProcessXFunc(WORD xFunc)
{
    WORD idx;
    switch (xFunc)
    {
    case MENUXREPORT: // 打印X报表
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
        if (EJ_X_STORE && CheckEJError(true))
            break;
#endif
        do {
            idx = ListItems(SETREPORT,0,It_SELEREPORT,false);
            if (idx<=XZNUM && idx>0)
            {//打印X/Z报表
                ApplVar.ReportNumber=idx;
                ApplVar.RepComputer = 0;
                ApplVar.FReport = ApplVar.CentralLock;
                GetReport(ApplVar.ReportNumber);
                ApplVar.FReport = 0;
            }
            else
                break;
        } while (1);
        break;
#if (CASE_USBDISK)
    case MENUXEXPORT:   //导出报表数据
        ReportToUSB();
        break;
#endif
    case MENUQUICKVIEW:      //Quick View
        BackupScreen(true);
        DispGrandTotal();
        WAIT_INPUT(idx);
        RecallScreen();
        break;
    case MENUXEJFUNC:       //LOG. OPERATION
        ExploreLog();
        break;
#if defined(CASE_GPRS)
    case MENUSENDNEWLOG:     //发送新流水数据
        GPRSSendECR_LOG();
        break;
#endif
#if !defined(DD_LOCK)
    case MENUXPASSWORD:     //设置X密码
        if (!SetPassword(Msg[SETXPWD].str,ApplVar.AP.ModePwd.PwdX))
            ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
        break;
#endif
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    case MENUXEJOURNAL: // 打印EJ报表
        ExploreEJournal();
        break;
#endif
    }

}

CONST MenuDefine ItemZReport[]={
    {MENUZREPORT,SETREPORT},
#if (CASE_USBDISK)
    {MENUZEXPORT,0},  //导出报表数据
#endif
    {MENUCLEARLOG,0},      //清除LOG
    {MENUZGRAND,0},    //是否清除Grand TOTAL
#if !defined(DD_LOCK)
    {MENUZPASSWORD,0},     //设置X密码
#endif
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    {MENUZRESETEJ,0}, // 打印EJ报表
#endif
    {0,0}
};
/**
 * 处理Z下菜单所对应的功能
 *
 * @author EutronSoftware (2016-02-23)
 *
 * @param zFunc :z下的功能号(MENUZREPORT-MENUZPASSWORD)
 */
void ProcessZFunc(WORD zFunc)
{
    WORD idx;
    switch (zFunc)
    {
    case MENUZREPORT:  //打印Z报表
        do {
            idx = ListItems(SETREPORT,0,It_SELEREPORT,false);
            if (idx<=XZNUM && idx>0)
            {//打印X/Z报表
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
                if (EJ_X_STORE)
                {
                   // if (ApplVar.EJHeader.AddrNextEJ >= ApplVar.EJHeader.AddrEJMax-MMC_FULL)
                   //     SETBIT(ApplVar.EJVar.ContFlag,EJISFULL);//ccr2016-11-29
                    if (CheckEJError(true))
                        break;
                }
#endif
                ApplVar.ReportNumber=idx;
                ApplVar.RepComputer = 0;
                ApplVar.FReport = ApplVar.CentralLock;
                GetReport(ApplVar.ReportNumber);
                ApplVar.FReport = 0;
            }
            else
                break;
        } while (1);
        break;
#if (CASE_USBDISK)
    case MENUZEXPORT:   //导出报表数据
        ReportToUSB();
        break;
#endif
    case MENUCLEARLOG:      //清除LOG
        if (WaitForYesNo(Msg[MENUCLEARLOG].str,false))
        {
            Init_Flow();
        }
        break;
    case MENUZGRAND:    //是否清除Grand TOTAL
        if (WaitForYesNo(cCLEARGRANDTOTAL,!TESTBIT(DBLHIPRN,BIT6)))
            SETBIT(DBLHIPRN,BIT6);
        else
            RESETBIT(DBLHIPRN,BIT6);
        break;
#if !defined(DD_LOCK)
    case MENUZPASSWORD: //设置Z密码
        if (!SetPassword(Msg[SETZPWD].str,ApplVar.AP.ModePwd.PwdZ))
            ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
        break;
#endif
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    case MENUZRESETEJ:      //复位EJ
        if (CheckEJError(false)!=ERROR_ID(CWXXI93) && WaitForYesNo(Msg[MENUZRESETEJ].str,false))
        {//EJ is  exist
            RESETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
            PrintEJInfo(true);
            SETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
            Initial_EJ();
        }
        break;
#endif
    }
}

//ccr2016-02-22<<<<<<<<<<<<<<<<<<<<<<<<<<<<
