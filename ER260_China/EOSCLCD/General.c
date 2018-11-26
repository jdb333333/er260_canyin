#define GENERAL_C 4
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"


void CheckBreakKey()
{
	BYTE key;


	if (KbHit())
    {
		key = Getch();
		if (key > 65)       /* central or clerk lock */
		{
		   	if (key < NOCLERK)      /* central Lock */
		   	{
			    ApplVar.CentralLock = key;
				while(ApplVar.CentralLock == OFF)       /* lock off then wait */
				{
				    key = Getch();
				    if (key > 65 && key < NOCLERK)
					    ApplVar.CentralLock = key;
				}
		     }
		     else
		     {
			     if (key == NOCLERK)
				 {
					 if (ApplVar.ClerkLock)
				     {
						 ApplVar.ClerkLock = 0;
						 ApplVar.SaveClerk = 0;
				     }
				 }
				else if (key != ApplVar.ClerkLock)
				{
				     ApplVar.ClerkLock = key;
				     ApplVar.SaveClerk = key - NOCLERK;
				     if (ApplVar.SaveClerk > ApplVar.AP.Clerk.Number)
				     {
						 ApplVar.SaveClerk = 0;
						 ApplVar.ClerkLock = 0;
				     }
				 }
		    }
		 }
		 else if (key == ApplVar.AP.FirmKeys[ID_CLEAR] && ApplVar.FReport != Z && !ApplVar.RepComputer)  /* clear is break */
		 {
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI27);   /* user break*/
		    return;
		 }
    }
}

void ReadTotal()
{
    if (ApplVar.BufCC)     /* reset Customer count added ?*/
    {//Z报表数据清0
		ReadRam(&ApplVar.Total.Qty.Sign, 1);
		return;
    }
    memset(&ApplVar.Total, 0, sizeof(ApplVar.Total));
    ReadRam((BYTE*)&ApplVar.Total.Qty, ApplVar.Size.Qty + 1);
    if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))    /* already used then clear by not read */
    {//BIT2=0,各项统计数据为0
		ApplVar.Total.Qty = ZERO;
		return;
    }
    if (ApplVar.Size.Cc)
		ReadRam(ApplVar.Total.Cc.Value, ApplVar.Size.Cc);
    if (ApplVar.Size.Amt)
    {
		ReadRam(ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
		if (TESTBIT(ApplVar.Total.Qty.Sign, BIT6))     /* qty negative ? */
		    SETBIT(ApplVar.Total.Amt.Sign, BIT7);
    }
    if (ApplVar.Size.RetQty)
    {
		ReadRam(ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
		ApplVar.Total.RetQty.Sign = ApplVar.Total.Qty.Sign & 0x03;    /* same decimal */
    }
    if (ApplVar.Size.Disc)
    {
		ReadRam(ApplVar.Total.Disc.Value, ApplVar.Size.Disc);
		if (TESTBIT(ApplVar.Total.Qty.Sign, BIT4))     /* disc negative ? */
		    SETBIT(ApplVar.Total.Disc.Sign, BIT7);
    }
    if (ApplVar.Size.Cost)
    {
		ReadRam(ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
		if (TESTBIT(ApplVar.Total.Qty.Sign, BIT5))     /* cost  negative ? */
		    SETBIT(ApplVar.Total.Cost.Sign, BIT7);
    }
}


void WriteTotal()
{
    if (ApplVar.BufCC)     /* reset Customer count added ?*/
    {
		RESETBIT(ApplVar.Total.Qty.Sign, BIT3);
		WriteRam(&ApplVar.Total.Qty.Sign, 1);    /* write back sign */
		return;
    }
    if (TESTBIT(ApplVar.Total.Amt.Sign, BIT7))
		SETBIT(ApplVar.Total.Qty.Sign, BIT6);
    else
		RESETBIT(ApplVar.Total.Qty.Sign, BIT6);
    if (TESTBIT(ApplVar.Total.Cost.Sign, BIT7))
		SETBIT(ApplVar.Total.Qty.Sign, BIT5);
    else
		RESETBIT(ApplVar.Total.Qty.Sign, BIT5);
    if (TESTBIT(ApplVar.Total.Disc.Sign, BIT7))
		SETBIT(ApplVar.Total.Qty.Sign, BIT4);
    else
		RESETBIT(ApplVar.Total.Qty.Sign, BIT4);
    SETBIT(ApplVar.Total.Qty.Sign, BIT2 + BIT3);   /* set bit 4 & 5 to indicate used */
    WriteRam((BYTE*)&ApplVar.Total.Qty, ApplVar.Size.Qty + 1);
    if (ApplVar.Size.Cc)
		WriteRam(ApplVar.Total.Cc.Value, ApplVar.Size.Cc);
    if (ApplVar.Size.Amt)
		WriteRam(ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
    if (ApplVar.Size.RetQty)
		WriteRam(ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
    if (ApplVar.Size.Disc)
		WriteRam(ApplVar.Total.Disc.Value, ApplVar.Size.Disc);
    if (ApplVar.Size.Cost)
		WriteRam(ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
}

void PrintMultiply()
{
    BYTE saveprint;

    if (!ApplVar.MultiplyCount)     /* if not used no print */
		return;
    if (ApplVar.MultiplyCount == 1 && !CompareBCD(&ApplVar.Qty, (BCD*)&ONE))
		return;     /* don't print qty of one */
    saveprint = ApplVar.PrintLayOut;
    if (!TESTBIT(SLIPINFO, BIT0))
		ApplVar.PrintLayOut &= 0xe7;
    MemSet(SysBuf, sizeof(SysBuf), ' ');
    SysBuf[sizeof(SysBuf) - 1] = 0;
    if (ApplVar.MultiplyCount > 2)
    {
#if PRTLEN == 18
		FormatQty(SysBuf + sizeof(SysBuf) - 12, &ApplVar.Qty1);
		SysBuf[sizeof(SysBuf) - 10] = 'X';
		PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN);
		MemSet(SysBuf, sizeof(SysBuf), ' ');
		SysBuf[sizeof(SysBuf) - 1] = 0;
#endif
		FormatQty(SysBuf + sizeof(SysBuf) - 4, &ApplVar.Qty3);
		SysBuf[sizeof(SysBuf) - 10] = 'X';
    }
	if (ApplVar.MultiplyCount > 1)
	{
		FormatQty(SysBuf + sizeof(SysBuf) - 12, &ApplVar.Qty2);
#if PRTLEN == 18
		if (ApplVar.MultiplyCount == 2)
#endif
		{
		    SysBuf[sizeof(SysBuf) - 18] = 'X';
		    FormatQty(SysBuf + sizeof(SysBuf) - 20, &ApplVar.Qty1);
		}
		if (ApplVar.MultiplyCount > 2)
		    SysBuf[sizeof(SysBuf) - 2] = '=';
		else
		    SysBuf[sizeof(SysBuf) - 10] = '=';

#if PRTLEN == 18
		if (ApplVar.MultiplyCount ==2)
		    PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN - 8);
		else
#endif
			PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN);
		MemSet(SysBuf, sizeof(SysBuf), ' ');
		SysBuf[sizeof(SysBuf) - 1] = 0;
    }
#if PRTLEN == 18
    FormatQty(SysBuf + sizeof(SysBuf) - 14, &ApplVar.Qty);
    FormatAmt(SysBuf + sizeof(SysBuf) - 3, &ApplVar.Price);
    SysBuf[sizeof(SysBuf) - 12] = 'X';
#else
    FormatQty(SysBuf + sizeof(SysBuf) - 16, &ApplVar.Qty);
    FormatAmt(SysBuf + sizeof(SysBuf) - 4, &ApplVar.Price);
    SysBuf[sizeof(SysBuf) - 14] = 'X';
#endif
	if (SysBuf[sizeof(SysBuf) - 1 - PRTLEN + 4] == ' ')
	    PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN + 5);
	else
	    PrintStr(SysBuf + sizeof(SysBuf) - 1 - PRTLEN);
   	 ApplVar.PrintLayOut = saveprint;
}


void PrintSaleQty()
{
    if (!PRN_QTY_ITEMS || !ApplVar.FReceipt)
		return;
    if (CheckNotZero(&ApplVar.SaleQty))    /* print item counter */
    {
		if (ApplVar.FPb)
		    ApplVar.PrintLayOut &= 0x02;
		else
		    ApplVar.PrintLayOut = 0x02;     /* set receipt */
 //		FormatQtyStr(ApplVar.TXT.Caption[25], &ApplVar.SaleQty, 14);
		PrintStr(FormatQtyStr(Msg[XIANGSHU].str, &ApplVar.SaleQty, PRTLEN));
//ccr2016-03-03		DispStrLF(FormatQtyStr(Msg[XIANGSHU].str, &ApplVar.SaleQty, SCREENWD));
    }
}

void PrintMessage(WORD number)
{
    if (Msg[number].str[0])
		PrintStr(Msg[number].str);
}
// add by lqw ,to print GPRS Messages...
#if(defined(CASE_GPRS))
void PrintGprsMessage(WORD number)
{
	if(GPRSMess[number].str[0])
		PrintStr(GPRSMess[number].str);
}
#endif
void PrintPbTrailer()
{
    BYTE saveprint;
    short i;

    if (ApplVar.PbNumber && TESTBIT(ApplVar.AP.Pb.Random, BIT7)
		    && TESTBIT(ApplVar.PrintLayOut, BIT1)
			&& TESTBIT(ApplVar.FNoPb, BIT2))
    {
		saveprint = ApplVar.PrintLayOut;
		ApplVar.PrintLayOut = 0x02;
	  	RamOffSet = (ApplVar.AP.Pb.Random & 0x0f) + ApplVar.AP.Pb.Text + ApplVar.AP.Pb.AmtSize + 6;
	    if (TESTBIT(PBINFO, BIT7))  /* Discount item stored ? */
			RamOffSet += ((ApplVar.AP.Pb.AmtSize + 1) * 2);
	    RamOffSet = RamOffSet * ApplVar.AP.Pb.NumberOfPb + ApplVar.AP.StartAddress[AddrPBt];      /*Start Pb Trailer */
		SysBuf[24] = 0;
		for (i = 0; i < 50; i++)
		{
		    ReadRam(SysBuf, 24);     /* read Line */
		    if (!SysBuf[0])     /* end line ? */
				break;
		    PrintRJ(SysBuf);
		}
		ApplVar.PrintLayOut = saveprint;
    }
}

void GetWordEntry()
{
	UnLong TempL ;
//ccr2014-09-09    ApplVar.NumberEntry = 0;
    if (CompareBCD(&ApplVar.Entry, (BCD*)&MAXWORD) < 1)   /* entry < 65535 */
		ApplVar.NumberEntry = (WORD)BCDValueToULong(ApplVar.Entry.Value, &TempL);
    else
        ApplVar.NumberEntry = 65535;//ccr2014-09-09
}

/* add corresponding totals */

void AddTotal()
{
	UnLong    save ;             /* unsigned long save;  */

    if (ApplVar.FTrain && ApplVar.PointerType != 1)
    {
   		RamOffSet += ApplVar.Size.Length; /* next period or total */
		return;         /* only add in clerk when training */
    }
    save = RamOffSet;      /* save OffSet for Writing */
    ReadTotal();
    RamOffSet = save;
    if (ApplVar.BufCC)             /* reset customer added ? customer count ? */
    {
		WriteTotal();
	    RamOffSet = save + ApplVar.Size.Length;  /* next period or total */
    }
    else
    {
		if (ApplVar.Size.Cc)
		{
	    	if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT3))
			{
				if (!ApplVar.FTrvoid && !ApplVar.FCanc) /* already added ?*/
					Add(&ApplVar.Total.Cc, (BCD*)&ONE);
			}
		}
		if (ApplVar.Size.Qty)
		{
		    ApplVar.Total.Qty.Sign &= 0x83;         /* skip bits */
		    Add(&ApplVar.Total.Qty, &ApplVar.Qty);
		}
		if (ApplVar.Size.Amt)
		{
	    	Add(&ApplVar.Total.Amt, &ApplVar.Amt);
//			PrintAmt(ApplVar.TXT.Caption[47], &ApplVar.Total.Amt);
		}
		if (ApplVar.Size.RetQty)
		{
		    Subtract(&ApplVar.Total.RetQty, &ApplVar.RetQty);   /* no Sign in memory so always subtract !!*/
		    if (TESTBIT(ApplVar.Total.RetQty.Sign, BIT7))       /* Result negative? ? */
				ApplVar.Total.RetQty = ZERO;
		}
		if (ApplVar.Size.Disc)
		    Add(&ApplVar.Total.Disc, &ApplVar.DiscAmt);
		if (ApplVar.Size.Cost)
		    Add(&ApplVar.Total.Cost, &ApplVar.Cost);
		WriteTotal();
    }
}


void AddPointerTotal()
{
    WORD skip, next;


    if (ApplVar.FProforma)
		return;
    if (!ApplVar.Size.Periods)
		return;
    if (!ApplVar.Size.Length)     /* active ? */
		return;
    if (ApplVar.FSplit)
		return;             /* don't add when split bill */
    skip = 0;
    if (TESTBIT(ApplVar.Size.Periods, BIT0))      /* calculate size of 1 pointer */
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT1))
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT2))
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT3))
		skip += ApplVar.Size.Length;
    next = skip;
    switch (ApplVar.PointerType)
    {
	case 0://统计总销售
	    skip = 0;
	    next = 0;
	    break;
	case 1:         /* Clerks */
	    if (!ApplVar.AP.Clerk.Number)
			return;
	    next *= (ApplVar.AP.Clerk.Number - ApplVar.ClerkNumber);
	    skip *= (ApplVar.ClerkNumber - 1);
	    break;
	case 2:         /* Hour */
	    if (!ApplVar.AP.Zone.Number)
			return;
	    next *= (ApplVar.AP.Zone.Number - ApplVar.Zone - 1);
	    skip *= ApplVar.Zone;
	    break;
	case 3:         /* day of week */
	    if (!ApplVar.AP.Day.Number)
			return;
	    next *= (ApplVar.AP.Day.Number - ApplVar.Day - 1);
	    skip *= ApplVar.Day;
	    break;
	case 4:         /* ApplVar.Month of Year */
	    if (!ApplVar.AP.Month.Number)
			return;
	    next *= (ApplVar.AP.Month.Number - ApplVar.Month - 1);
	    skip *= ApplVar.Month;
	    break;
	case REPDEFMAX-1:
#if (salNumber)
	    if (ApplVar.AP.SalPer.Number)
		{
			next *= (ApplVar.AP.SalPer.Number - ApplVar.SalPerNumber);
		    skip *= (ApplVar.SalPerNumber - 1);
            break;
		}
        else
            return;
#else
        return;
#endif
	default:
	    break;
    }
   	RamOffSet += skip;
    if (TESTBIT(ApplVar.Size.Periods, BIT0))
		AddTotal();
    if (TESTBIT(ApplVar.Size.Periods, BIT1))
		AddTotal();
    if (TESTBIT(ApplVar.Size.Periods, BIT2))
		AddTotal();
    if (TESTBIT(ApplVar.Size.Periods, BIT3))
		AddTotal();
	RamOffSet += next;     /* start next pointer totals */
}

void ClearEntry(BYTE disp)
{
    MemSet(EntryBuffer, ENTRYSIZE, ' ');
    if (!TESTBIT(ApplVar.MyFlags,HANZIMODE))
	{
      if (ApplVar.CentralLock==RG ||  ApplVar.CentralLock==MG)
      {
        AtEntryBuffer(4) = '0';
        AtEntryBuffer(3) = '.';
        AtEntryBuffer(2) = '0';
      }
      AtEntryBuffer(1) = '0';
	}

	AtEntryBuffer(0) = 0;
    Appl_EntryCounter = 0;
	Editor.EditCount = 0;
	Editor.EditOffs = 0;
    ApplVar.DecimalPoint = 0;
    if (disp)
    {
        LCD10_SetFlag(FLAG_NULL);
    	PutsO(EntryBuffer+ENTRYSIZE-DISLEN-1);//C-DISPLAY
#if (SCRSTATDBL)
        if (ApplVar.CentralLock==RG ||ApplVar.CentralLock==MG)
            DispOnStatDBL(EntryBuffer+ENTRYSIZE-SCRDBLWD-1,0);
        else
#endif
            DispSt2(EntryBuffer+ENTRYSIZE-INPUTWIDTH-1);//RG-DISPLAY

    }
}

/**********************************************************
@Para: (max / 10) is digit and (max % 10) is number of zeroes:
    89:表示最大数据为8000000000
@return: 0-Data in the range;1-data out of range
********************************************************/
short CheckMaxEntry(BCD *amt, BYTE max)
{
    BCD mx;
    BYTE i;

    if (!max)       /* not active */
		return 0;

    mx = ZERO;
    i = (max % 10) / 2;
    mx.Value[i] = max / 10;
    if (max % 2)
		mx.Value[i] *= 0x10;
    if (CompareBCD(amt, &mx) < 1)        /* amt <= max */
		return 0;
    else
		return 1;
}


/* When "err" is non zero only display "err" else */
/* display ApplVar.ErrorNumber */
void CheckError(BYTE err)
{
    static BYTE errLast=0;
    char dbuf[SCREENWD+1];
    short  erX,erY;


	if (!err)
    {
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
        if (!ApplVar.ErrorNumber)
        {
            if (TESTBIT(ApplVar.EJVar.ContFlag,EJWILLFULL))
            {
                if (!ApplVar.FRegi)//没有打开收据时
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI96);              // 40 KB left, only warn once
                    RESETBIT(ApplVar.EJVar.ContFlag,EJWILLFULL);
                }
            }
            else if (TESTBIT(ApplVar.EJVar.ContFlag,EJISFULL))
            {
                //启用此行会导致无法继续操作收款机 ApplVar.ErrorNumber=ERROR_ID(CWXXI97);
                if (ApplVar.FRegi)//允许完成结账
                    RESETBIT(ApplVar.EJVar.ContFlag,EJISFULL);
            }
        }
#endif
		err = ApplVar.ErrorNumber;
    }

	if (err==255)
	{//报警
		Bell(0);
		if (ApplVar.ErrorNumber ==255)
			ApplVar.ErrorNumber=0;
	}
	else
    {
        if (errLast==err)
        {
            if (err && GetTimerDelay1()==0)
            {//相同的错误号,只报警
                SetTimerDelay1(500);
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
                //EJ的错误信息被菜单信息覆盖,因此在状态行显示一次
                if (ECRMode()!='R')
                {
                    ClsState12();
                    DispSt2((char*)Msg[CWXXI01+err-1].str);
                }
#endif
                Bell(1);
            }
        }
        else
        {
            erY = (SCREENLN-1-3)/2;
            if (err==0)
            {
                if (errLast)
                {//恢复屏幕内容
                    mDrawABox(0,0);//mRefreshLine(erY++); mRefreshLine(erY++); mRefreshLine(erY);
                }
            }
            else
            {//显示出错信息,占用屏幕三行
                mDrawABox(erY,erY+2);

                mDispStringXY((char*)Msg[CHUCUO].str,CenterForDisplay(SCREENWD-2,strlen(Msg[CHUCUO].str))+1,erY);
                if (err>0 && err<=ERROR_ID(CWXXIMAX))
                {
                    mDispStringXY((char*)Msg[CWXXI01+err-1].str,CenterForDisplay(SCREENWD-2,strlen(Msg[CWXXI01+err-1].str))+1,erY+1);
                }
                else
                {//没有对应的错误代码,显示错误号
                    memset(dbuf,0,SCREENWD);
                    CopyFrStr(dbuf,Msg[CHUCUO].str);
                    WORDtoASCL(dbuf, err);
                    dbuf[SCREENWD-2]=0;
                    mDispStringXY(dbuf,SCREENWD/2-1,erY+1);
                }
                SetTimerDelay1(500);
                Bell(1);
            }
            errLast = err;
        }
    }

}
/**
 * 显示提示信息
 *
 * @author EutronSoftware (2016-03-11)
 *
 * @param msg
 */
void DisplayPrompt(char *msg)
{
    short  erY;

    erY = (SCREENLN-1-3)/2;
    mDrawABox(erY,erY+2);

    mDispStringXY((char*)Msg[CHUCUO].str,CenterForDisplay(SCREENWD-2,strlen(Msg[CHUCUO].str))+1,erY);
    mDispStringXY(msg,CenterForDisplay(SCREENWD-2,strlen(msg))+1,erY+1);
    Bell(1);
    WAIT_INPUT(erY);
    // mDrawABox(0,0);
}

WORD RegiInit()
{
    if (ApplVar.FCanc)
		ApplVar.PrintLayOut &= ApplVar.CancPrint;
    if (!ApplVar.FRegi)   /* already in registration ? */
    {
		if (!ApplVar.ClerkNumber)
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI33);       /* select clerk */
		    return 1;
		}
#if (salNumber)
		if (ApplVar.AP.SalPer.Number && !ApplVar.SalPerNumber)
		{
			if (!TESTBIT(KEYTONE, BIT6))
			{
				ApplVar.SalPerNumber = 1;
			}
			else
			{
			    ApplVar.ErrorNumber=ERROR_ID(CWXXI34);	    /* select salesperson */
			    return 1;
			}
		}
#endif
		ApplVar.SlipPage = 0;
		if (CheckSlip())
		    return 1;
		Now.sec = 0x61;     /* force read of new time */
		CheckTime(FALSE);        /* read correct time */
		GetTimeZones();      /* set up correct time, day, month */
		if (ApplVar.FPb && TESTBIT(PBINFO, BIT4))
		{
		}
		else
		    AddReceiptNumber();
		ApplVar.SaleAmt = ZERO;
		ApplVar.SaleQty = ZERO;
		MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);
		MemSet(ApplVar.DiscItem, sizeof(ApplVar.DiscItem), 0);
		ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
#if (salNumber)
		if (ApplVar.AP.SalPer.Number)
		{//1.将营业员存入RGBuf中
		    ApplVar.RGRec.Key.Code = SALPER + ApplVar.SalPerNumber;  /* store in buffer for correct CC update */
			StoreInBuffer();
		}
#endif
		ApplVar.RGRec.Key.Code = CLERK + ApplVar.ClerkNumber;
		ApplVar.FTend = 0;  /* reset incase post-tendering */
		ApplVar.FRegi = 1;
		ApplVar.FNoTax = 0;
		ApplVar.SubDiscFlag = 0;			//cc 2006-10-11 for SubDisc
		SETBIT(ARROWS, BIT1);
		Collect_Data(REGISLOG);
    }
    else if (ApplVar.FTend)  /* in tender ? */
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI17);
		return 1;
    }
    else if (!ApplVar.SlipLines && CheckSlip())
		return 1;
    else if (ApplVar.FBuffer)       /* regi buffer full */
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI15);
		return 1;
    }
    if (!ApplVar.FReceipt && !ApplVar.FInv)   /* receipt already used ? */
    {
		if (TESTBIT(ARROWS, BIT0) &&     /* receipt on ? */
		    TESTBIT(ApplVar.PrintLayOut, BIT1))
		{
			PrintHead1(PREHEADER);
			//ccr2016-11-15 RFeed(1);
		}
    }
    return 0;
}

WORD RegiStart()           /* registration start */
{
    BYTE saveprint;
    int l;

    saveprint = ApplVar.PrintLayOut;
    ApplVar.PrintLayOut &= 0xfb;        /* skip double height */
    if (RegiInit())
		return 1;
    if (!ApplVar.FReceipt)
    {
		if (ApplVar.FCanc == 1)
	    	PrintMessage(QUXIAOP);
    }
#if (salNumber)
    if (ApplVar.AP.SalPer.Number && ApplVar.SalPerNumber != ApplVar.OldSalPer)    /* print salp when changed ? */
    {
//		if (TESTBIT(ApplVar.SalPer.Options, BIT0))      /* double heigth ? */
//			SETBIT(ApplVar.PrintLayOut, BIT2);
//		PrintStr(ApplVar.SalPer.Name);
	    ApplVar.PrintLayOut &= 0xfb;        /* reset double height */
		ApplVar.OldSalPer = ApplVar.SalPerNumber;
    }
#endif
    if (TESTBIT(CLERKFIX, BIT4) && ApplVar.ClerkNumber != ApplVar.OldClerk)    /* print clerk when changed ? */
    {
		if(TESTBIT(ApplVar.MyFlags, ENSLIPPB))
		{
			RESETBIT(ApplVar.MyFlags, ENSLIPPB);	 //lyq added for slip 20040324
			RESETBIT(ApplVar.PrintLayOut,BIT3);
		}
		if((ApplVar.FPb==0 && (ApplVar.PbF.Options & 0x0f) != 3) ||(ApplVar.PbF.Options & 0x0f) == 0 || ApplVar.PbNumber==0)
		{
#if !defined(DEBUGBYPC)//  打印票头图象  //
#if !defined(CASE_INNERDOTPRN)
			if ((ApplVar.Graph[0].PictNo>0 && ApplVar.Graph[0].PictNo<=GRAPHMAX) && !TESTBIT(ApplVar.MyFlags, PRNONPB+ENPRINTER))
			{
				if (!TESTBIT(ApplVar.MyFlags, ENPRINTER))
					Bios(BiosCmd_PrintGraph, (void*)(ApplVar.Graph[0].PictNo), 1 , 0); //Stampa msg ram
		    	PrintLine('=');
			}
#endif
#endif
//ccr2014-03-31>>>>>>>>>>>>
            if (PRNINDEX)
            {//打印取菜号
                memset(ProgLineMes,' ',PRTLEN);
                CopyFrStr(ProgLineMes,Msg[mPRNINDEX].str);
                l = strlen(Msg[mPRNINDEX].str)+1;
//ccr2016-04-20                l+=WORDtoASCL(ProgLineMes+l,REGISTER);
//ccr2016-04-20                ProgLineMes[l]='-';
//ccr2016-04-20                l++;
                HEXtoASC(ProgLineMes+l, ApplVar.ReceiptNumber, 3);
                ProgLineMes[l+6]=0;
                SETBIT(ApplVar.PrintLayOut, BIT2);//倍高打印
                PrintStr_Center(ProgLineMes,true);
            }
//<<<<<<<<<<<<<<<<<<<<<<<<<
#if defined(CASE_PERU)//ccr2016-10-24>>>>>>>>>>>>>>>
            PrintCustomerMessage();
#endif//ccr2016-10-24<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
            if (TestSaveEJ())
                RefreshEJStart(true);
#endif
			PrintStr(Msg[TITLE].str);
//20130709分割线没有多大的意义，客户反馈也浪费纸	    	PrintLine('=');
		}
		ApplVar.OldClerk = ApplVar.ClerkNumber;
    }
    ApplVar.PrintLayOut = saveprint;    /* restore print construction */

    return 0;
}

void PromotionBeep()
{
	short i;
	for (i=0;i<5;i++)
	{
		memset(ProgLineMes,'8',DISLEN+1);
		PutsO(ProgLineMes);//C-DISPLAY
		Bell(1);
		Delay(ASECOND/2);
		memset(ProgLineMes,' ',DISLEN+1);
		PutsO(ProgLineMes);//C-DISPLAY
		Delay(ASECOND/2);
	}
	memset(ProgLineMes,'8',DISLEN+1);
	PutsO(ProgLineMes);//C-DISPLAY
}

void PromtionCheck()
{
	BCD	sVal1,sVal2;

	if (!TESTBIT(PROMOTION,BIT0))
	{
		sVal1 = ZERO;
		memcpy(sVal1.Value,ApplVar.AP.Promotion.JollyMin,sizeof(ApplVar.AP.Promotion.JollyMin));
	    if (CompareBCD(&ApplVar.SubTotal,&sVal1) >= 0)
		    ApplVar.PromCounter++;
	    if (ApplVar.PromCounter==ApplVar.AP.Promotion.Freq)
	    {
#if !defined(CASE_INNERDOTPRN)
	    	if (ApplVar.AP.Promotion.GrapIdx>0 && !TESTBIT(ApplVar.MyFlags, ENPRINTER))
				Bios(BiosCmd_PrintGraph, (void*)(ApplVar.AP.Promotion.GrapIdx), 1 , 0); //Stampa msg ram
#endif
			RJPrint(0,ApplVar.AP.Promotion.Memo);
	    	ApplVar.PromCounter = 0;
	    	PromotionBeep();
	    }
    }
	if (!TESTBIT(PROMOTION,BIT1))
	{
		sVal1 = ZERO;
		memcpy(sVal1.Value,ApplVar.AP.Promotion.PointMin,sizeof(ApplVar.AP.Promotion.PointMin));
	    if (CompareBCD(&ApplVar.SubTotal,&sVal1) >= 0)
	    {
			sVal1 = ZERO;
			memcpy(sVal1.Value,ApplVar.AP.Promotion.PointVal,sizeof(ApplVar.AP.Promotion.PointVal));
			sVal2 = ApplVar.SubTotal;
			if (CheckNotZero(&sVal1))
			{
				Divide(&sVal2,&sVal1);
				SetBCDPoint(&sVal2,0);
				PrintRJ(FormatQtyStr(Msg[BCXFEI].str, &sVal2, PRTLEN));

/*				if (CompareBCD(&sVal2,&TEN) >= 0)
				{
			    	if (ApplVar.AP.Promotion.GrapIdx>0 && !TESTBIT(ApplVar.MyFlags, ENPRINTER))
						Bios(BiosCmd_PrintGraph, ApplVar.AP.Promotion.GrapIdx-1, 0 , 0); //Stampa msg ram
					RJPrint(0,ApplVar.AP.Promotion.Memo);
			    	PromotionBeep();
		    	}*/
		    }
	    }
    }
}

void RegiEnd()
{

    if (PRICE_LEVEL & 0x02)       /* to zero at end of trans action ? */
		ApplVar.PluPriceLevel = 0;
#if defined(ARTLEVEL1) //ccr2016-01-27>>>>
    if ((ART_LEVEL & 0x03) == 2)         /* to zero at end of transaction ? */
		ApplVar.PluArtLevel = 0;
#endif
    ApplVar.TaxPerm = 0;            /* reset perm tax shift */
	ApplVar.FNFolio = 0;
    if (!ApplVar.FSale)   /* no sale then don't print total */
	{
		if (!ApplVar.PbNumber || (ApplVar.PbNumber && !TESTBIT(PBINFO, BIT3)))
		{
			return;
		}
	}
    CalculateTax(2);        /* only print and calculate tax */
    PrintLine('.');//20130709分割线没有多大的意义，客户反馈也浪费纸
	if (!ApplVar.FPb) /* also when no check out function */
	{
		SETBIT(ApplVar.PrintLayOut, BIT2);  /* double size */
		ApplVar.SlipDouble = 1;
		PrintAmt(Msg[YINGFU].str, &ApplVar.SubTotal);	//ApplVar.Total
#if (SCREENLN<5)
#if (SCRSTATDBL)
        DispStrXY(DispAmtStr(Msg[YINGFU].str, &ApplVar.SubTotal,SCRDBLWD),0,STATELINE);//ccr2016-03-02
#else
        DispStrXY(DispAmtStr(Msg[YINGFU].str, &ApplVar.SubTotal,SCREENWD),0,STATELINE);//ccr2016-03-02
#endif
#endif
		RESETBIT(ApplVar.PrintLayOut, BIT2);  /* double size */
	}
	ApplVar.SubDiscFlag = 0;	/* cc 2006-10-11 for SubDisc */
}


void PrintTotal()
{

#if (HOR_REPORT)
    //>>>>>>>> for new report(ApplVar.Dept & PLU only)
	if (!ApplVar.RepComputer)//ccr100702>>>>>>>>>
	{
	    if(ApplVar.Report.Type==2)  // dept report
	    {
	    	if (ApplVar.Size.Qty)
	           FormatQty(SysBuf+19+pSHIFTx,&ApplVar.Total.Qty);
	        else if (ApplVar.Size.Cc)
	           FormatQty(SysBuf+19+pSHIFTx,&ApplVar.Total.Cc);
	        if (ApplVar.Size.Amt)
	        {
	            ApplVar.Amt = ApplVar.Total.Amt;
				if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
				{
				    Subtract(&ApplVar.Amt, &ApplVar.Total.Disc);       /* calculate gross */
					if(((ApplVar.Total.Amt.Sign & 0x80)==0) && ((ApplVar.Total.Disc.Sign & 0x80)!=0))
						ApplVar.Amt.Sign = ApplVar.Total.Amt.Sign;
				}
				FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Amt);
	        }
	        RJPrint(0,SysBuf);
			if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
			{
		    	memset(SysBuf,' ',PRTLEN);
				CopyFrStr(SysBuf+12,Msg[ZHEKOUF].str);FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Total.Disc);
		        RJPrint(0,SysBuf);
			}
	        return;
	    }
	    else if(ApplVar.Report.Type==3)       // plu report
	    {
	    	if (ApplVar.Size.Qty)
		        FormatQty(SysBuf+19+pSHIFTx,&ApplVar.Total.Qty);
	        else if (ApplVar.Size.Cc)
	           FormatQty(SysBuf+19+pSHIFTx,&ApplVar.Total.Cc);

	        if (ApplVar.Size.Amt)
	        {
	            ApplVar.Amt = ApplVar.Total.Amt;
				if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
				{
				    Subtract(&ApplVar.Amt, &ApplVar.Total.Disc);
					if(((ApplVar.Total.Amt.Sign & 0x80)==0) && ((ApplVar.Total.Disc.Sign & 0x80)!=0))
						ApplVar.Amt.Sign = ApplVar.Total.Amt.Sign;
				}
				FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Amt);
	        }
	        RJPrint(0,SysBuf);

		    if (ApplVar.Size.Cost && ApplVar.Size.Amt && ApplVar.AP.Plu.Cost && CheckNotZero(&ApplVar.Total.Cost))//
		    {
		    	memset(SysBuf,' ',PRTLEN);
				if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
				{
					CopyFrStr(SysBuf+2,Msg[ZHEKOUF].str);
                    FormatAmt(SysBuf+14+(pSHIFTx>>1), &ApplVar.Total.Disc);
				}
				else
				{
					CopyFrStr(SysBuf+2,Msg[CHENBEN].str);//SysBuf[2+4]=':';
                    FormatAmt(SysBuf+14+(pSHIFTx>>1), &ApplVar.Total.Cost);
				}
				ApplVar.Amt = ApplVar.Total.Amt;
				Subtract(&ApplVar.Amt, &ApplVar.Total.Cost);
				CopyFrStr(SysBuf+16+(pSHIFTx>>1),Msg[MAOLI].str);//SysBuf[16+4]=':';
                FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Amt);
		        RJPrint(0,SysBuf);
		    }
			else if (ApplVar.Size.Disc && CheckNotZero(&ApplVar.Total.Disc))
			{
		    	memset(SysBuf,' ',PRTLEN);
				CopyFrStr(SysBuf+12,Msg[ZHEKOUF].str);FormatAmt(SysBuf+PRTLEN-1, &ApplVar.Total.Disc);
		        RJPrint(0,SysBuf);
			}
	        return;
	    }
    }//<<<<<<<<<<       end
#endif

    if (ApplVar.Size.Cc)
    {
		if (!SendComp(2, &ApplVar.Total.Cc))
		    PrintQty(Msg[JIAOYISHU].str, &ApplVar.Total.Cc);
    }
    if (ApplVar.Size.Qty)
    {
		if (!SendComp(3, &ApplVar.Total.Qty))
		    PrintQty(Msg[SHULIANG].str, &ApplVar.Total.Qty);
    }
    if (ApplVar.Size.RetQty)
    {
		if (!SendComp(4, &ApplVar.Total.RetQty))
		    PrintQty(Msg[TUIHUO].str, &ApplVar.Total.RetQty);
    }
    if (ApplVar.Size.Amt)
    {
		if (ApplVar.Report.Type == REPORT_TAX)       /* tax report ? */
		{
		    if (!SendComp(9, &ApplVar.Total.Amt))       /* taxable */
				PrintAmt(Msg[HSHXSHOU].str, &ApplVar.Total.Amt);
		}
		else if (ApplVar.Report.Type == REPORT_PBFUNC && (ApplVar.PbF.Options & 0x0f) == 10)       /* PB info report */
		{
		    if (!SendComp(3, &ApplVar.Total.Amt))
				PrintQty(Msg[SHULIANG].str, &ApplVar.Total.Amt);   /* and cover function ?*/
		}
		else if (!ApplVar.Size.Disc)
		{
		    if (ApplVar.Report.Type == REPORT_DRAWER)    /* drawer report */
		    {
//				if (TESTBIT(ApplVar.Draw.Options, BIT1))
//				{//打开钱箱次数统计
//				    if (!SendComp(3, &ApplVar.Total.Amt))
//						PrintQty(Msg[SHULIANG].str, &ApplVar.Total.Amt);
//				}
//				else
                if (!SendComp(7, &ApplVar.Total.Amt))
				{
#if (0) //ccr2016-11-07>>>>>>>>小数点与系统设置一致
					if (TESTBIT(ApplVar.Draw.Options, BIT2))
						ApplVar.AmtDecimal = 2;
					else
						ApplVar.AmtDecimal = 0;
					ApplVar.AmtDecimal += ApplVar.Draw.Options & BIT0;
				    Prefix1 = 0;
				    Prefix2 = 0;    /* no prefix */
#endif //ccr2016-11-07<<<<<<<<<<
				    PrintAmt(Msg[JINE].str, &ApplVar.Total.Amt);
				    ApplVar.AmtDecimal = NO_DECIMAL;
				    Prefix1 = PREFIX_1;
				    Prefix2 = PREFIX_2;
				}
		    }
		    else if (!SendComp(7, &ApplVar.Total.Amt))
				PrintAmt(Msg[JINE].str, &ApplVar.Total.Amt);
		}
		else
		{
		    ApplVar.Amt = ApplVar.Total.Amt;
		    Subtract(&ApplVar.Amt, &ApplVar.Total.Disc);       /* calculate gross */
//lyq added 2003\10\27 start
			if(((ApplVar.Total.Amt.Sign & 0x80)==0) && ((ApplVar.Total.Disc.Sign & 0x80)!=0))
				ApplVar.Amt.Sign = ApplVar.Total.Amt.Sign;
		    if (!SendComp(5, &ApplVar.Amt))
				PrintAmt(Msg[JINE].str, &ApplVar.Amt);
		}
		if (ApplVar.Report.Type == REPORT_CURRENCY && TESTBIT(ApplVar.Curr.Options, BIT2))   /* currency */
		{
		    GetCurrency(0, &ApplVar.Total.Amt);
		    if (!SendComp(7, &ApplVar.Total.Amt))
		    {
				ApplVar.AmtDecimal = ApplVar.Curr.Options & BIT0;
				Prefix1 = ApplVar.Curr.Prefix1;
				Prefix2 = ApplVar.Curr.Prefix2;
				PrintAmt(Msg[JINE].str, &ApplVar.Total.Amt);
				ApplVar.AmtDecimal = NO_DECIMAL;
				Prefix1 = PREFIX_1;
				Prefix2 = PREFIX_2;
		    }
		}
    }
    if (ApplVar.Size.Disc)
    {
		if (!SendComp(6, &ApplVar.Total.Disc))
		    PrintAmt(Msg[SETDISC].str, &ApplVar.Total.Disc);
		if (!SendComp(7, &ApplVar.Total.Amt))
		    PrintAmt(Msg[JINGE].str, &ApplVar.Total.Amt);
    }
    if (ApplVar.Size.Cost)
    {
		if (ApplVar.Report.Type == REPORT_TAX)
		{
			if (!SendComp(7, &ApplVar.Total.Disc))       /* NET tax report ? */
			    PrintAmt(Msg[XSHOUE].str, &ApplVar.Total.Disc);
		    if (!SendComp(10, &ApplVar.Total.Cost))       /* tax report ? */
				PrintAmt(Msg[SHUIE].str, &ApplVar.Total.Cost);
		}
		else
		{
			if (!SendComp(8, &ApplVar.Total.Cost))
			    PrintAmt(Msg[CHENBEN].str, &ApplVar.Total.Cost);
			ApplVar.Amt = ApplVar.Total.Amt;
			Subtract(&ApplVar.Amt, &ApplVar.Total.Cost);
			if (!SendComp(12, &ApplVar.Amt))
				PrintAmt(Msg[MAOLI].str, &ApplVar.Amt);
		}
    }
}


//ccr 050301 显示汇总数据>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //

void DisplayTotal()
{
	UnLong    save ;             /* unsigned long save;  */

    if (ApplVar.FTrain && ApplVar.PointerType != 1)
    {
   		RamOffSet += ApplVar.Size.Length; /* next period or total */
		return;         /* only add in clerk when training */
    }
    save = RamOffSet;      /* save OffSet for Writing */
    ReadTotal();
    RamOffSet = save;


//    SETBIT(ApplVar.Total.Qty.Sign, BIT2 + BIT3);   /* set bit 4 & 5 to indicate used */
//    WriteRam((BYTE*)&ApplVar.Total.Qty, ApplVar.Size.Qty + 1);
//    if (ApplVar.Size.Cc)
//   		DispSt2C(DispQtyStr(Msg[JIAOYISHU].str, &ApplVar.Total.Cc));
    if (ApplVar.Size.Amt)
   		DispSt2C(DispAmtStr(Msg[JINE].str, &ApplVar.Total.Amt,DISLEN));
//    if (ApplVar.Size.RetQty)
//   		DispSt2C(DispQtyStr(Msg[SHULIANG].str, &ApplVar.Total.RetQty));
//    if (ApplVar.Size.Disc)
//   		DispSt2C(DispAmtStr(Msg[SETDISC].str, &ApplVar.Total.Disc,DISLEN));
//    if (ApplVar.Size.Cost)
//   		DispSt2C(DispAmtStr(Msg[CHENBEN].str, &ApplVar.Total.Cost,DISLEN));
}


void DisplayPointerTotal(char *sFile)
{
    WORD skip, next;

    if (ApplVar.FProforma)
		return;
    if (!ApplVar.Size.Periods)
		return;
    if (!ApplVar.Size.Length)     /* active ? */
		return;
    if (ApplVar.FSplit)
		return;             /* don't add when split bill */
    skip = 0;
    if (TESTBIT(ApplVar.Size.Periods, BIT0))      /* calculate size of 1 pointer */
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT1))
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT2))
		skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT3))
		skip += ApplVar.Size.Length;
    next = skip;
    switch (ApplVar.PointerType)
    {
	case 0:
	    skip = 0;
	    next = 0;
	    break;
	case 1:         /* Clerks */
	    if (!ApplVar.AP.Clerk.Number)
			return;
	    next *= (ApplVar.AP.Clerk.Number - ApplVar.ClerkNumber);
	    skip *= (ApplVar.ClerkNumber - 1);
	    break;
	case 2:         /* Hour */
	    if (!ApplVar.AP.Zone.Number)
			return;
	    next *= (ApplVar.AP.Zone.Number - ApplVar.Zone - 1);
	    skip *= ApplVar.Zone;
	    break;
	case 3:         /* day of week */
	    if (!ApplVar.AP.Day.Number)
			return;
	    next *= (ApplVar.AP.Day.Number - ApplVar.Day - 1);
	    skip *= ApplVar.Day;
	    break;
	case 4:         /* ApplVar.Month of Year */
	    if (!ApplVar.AP.Month.Number)
			return;
	    next *= (ApplVar.AP.Month.Number - ApplVar.Month - 1);
	    skip *= ApplVar.Month;
	    break;
	case REPDEFMAX-1:
#if (salNumber)
	    if (ApplVar.AP.SalPer.Number)
		{
			next *= (ApplVar.AP.SalPer.Number - ApplVar.SalPerNumber);
		    skip *= (ApplVar.SalPerNumber - 1);
            break;
		}
        else
            return;
#else
        return;
#endif
	default:
	    break;
    }
   	RamOffSet += skip;
	DispSt1C(sFile);
    if (TESTBIT(ApplVar.Size.Periods, BIT0))
		DisplayTotal();
	else if (TESTBIT(ApplVar.Size.Periods, BIT1))
		DisplayTotal();
	else if (TESTBIT(ApplVar.Size.Periods, BIT2))
		DisplayTotal();
    else if (TESTBIT(ApplVar.Size.Periods, BIT3))
		DisplayTotal();
	RamOffSet += next;     /* start next pointer totals */
}
//ccr 050301 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< //

#if defined(CASE_PERU)//ccr2016-10-24>>>>>>>>>>>>>>>
//打印顾客信息
void PrintCustomerMessage()
{
    int i,sP;

    if (ApplVar.CustomerMessage.Messages[0][0])//ApplVar.CustomerMessage.CopyOfMess)
    {
		PrintLine('.');
        for (i=0;i<CUSTLINES && ApplVar.CustomerMessage.Messages[i][0];i++)
        {
            RJPrint(0,ApplVar.CustomerMessage.Messages[i]);
        }
    }
    ApplVar.CustomerMessage.CopyOfMess = ApplVar.CustomerMessage.Messages[0][0];
    ApplVar.CustomerMessage.Messages[0][0]=0;
}
#endif//ccr2016-10-24<<<<<<<<<<<<<
