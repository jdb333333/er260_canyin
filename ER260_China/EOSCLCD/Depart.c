#define DEPART_C 4
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "message.h"

/***************************************************************/

void GetDeptOffSet()
{
	RamOffSet = (UnLong)ApplVar.DeptNumber * ApplVar.AP.Dept.RecordSize + ApplVar.AP.StartAddress[AddrDept];
}

void AddDeptTotal()
{
    if (ApplVar.DeptNumber < ApplVar.AP.Dept.Number)	    /* just check dep sel. in plu */
    {
		GetDeptOffSet();
	    RamOffSet += ApplVar.AP.Dept.TotalOffSet;
		for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
		{
		    ApplVar.Size = ApplVar.AP.Dept.Size[ApplVar.PointerType];
		    AddPointerTotal();
		}
    }
}

void WriteDept()
{
    if (ApplVar.DeptNumber < ApplVar.AP.Dept.Number)
    {
		GetDeptOffSet();
		WriteRam(ApplVar.Dept.Random, ApplVar.AP.Dept.RandomSize);
		WriteRam(ApplVar.Dept.Name, ApplVar.AP.Dept.CapSize);
		WriteRam(&ApplVar.Dept.Group, sizeof(ApplVar.Dept.Group));
		WriteRam(&ApplVar.Dept.Print, sizeof(ApplVar.Dept.Print));
		WriteRam(&ApplVar.Dept.Options, sizeof(ApplVar.Dept.Options));
//		if (ApplVar.AP.Tax.Number)
		    WriteRam(&ApplVar.Dept.Tax, sizeof(ApplVar.Dept.Tax));
		WriteRam(&ApplVar.Dept.Kp, sizeof(ApplVar.Dept.Kp));
		WriteRam(ApplVar.Dept.Price, ApplVar.AP.Dept.PriceSize);
		WriteRam(ApplVar.Dept.PriceMax, ApplVar.AP.Dept.PriMaxSize);
    }
}

void ReadDept()
{
    GetDeptOffSet();

    ReadRam(ApplVar.Dept.Random, ApplVar.AP.Dept.RandomSize);
    ReadRam(ApplVar.Dept.Name, ApplVar.AP.Dept.CapSize);
    ApplVar.Dept.Name[ApplVar.AP.Dept.CapSize] = 0;
    ReadRam(&ApplVar.Dept.Group, sizeof(ApplVar.Dept.Group));
    ReadRam(&ApplVar.Dept.Print, sizeof(ApplVar.Dept.Print));
    ReadRam(&ApplVar.Dept.Options, sizeof(ApplVar.Dept.Options));
//	if (ApplVar.AP.Tax.Number)
		ReadRam(&ApplVar.Dept.Tax, sizeof(ApplVar.Dept.Tax));
	ReadRam(&ApplVar.Dept.Kp, sizeof(ApplVar.Dept.Kp));
	ReadRam(ApplVar.Dept.Price, ApplVar.AP.Dept.PriceSize);
	ReadRam(ApplVar.Dept.PriceMax, ApplVar.AP.Dept.PriMaxSize);
}


void PrintModifiers()
{
    BYTE j;

    if (!TESTBIT(KPPRICE, BIT5))
	return;
    for(j = 0; j < ApplVar.ModiCount; j++)
    {
		ApplVar.ModiNumber = ApplVar.ModNum[j];
		ReadModi();
		MemSet(SysBuf, sizeof(ApplVar.Modi.Name), ' ');
		strcpy(SysBuf + 5, ApplVar.Modi.Name);
		PrintStr(SysBuf + 3);
    }
}


/* temp points to string for number and if cmd = 0 the R & J & Slip */
/* when cmd = 1 then ApplVar.KP black if cmd == 2 then ApplVar.KP red */

char *GetPluPrint(char *temp,BYTE  cmd)
{
    if (TESTBIT(PLU_PRINT,BIT0))     /* print PLU #? */
    {
		if ((CHARSET & 0xf0) == 0x10 || (SLIP != 4 &&
		    ((CHARSET & 0x0f) == 3 || (CHARSET & 0x0f) == 7)))  /* number sign not available */
		    *temp = 'N';
		else
		    *temp = '#';
		if (ApplVar.AP.Plu.RandomSize)
		{
		    HEXtoASC(temp + 1, ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
		    *(temp + ApplVar.AP.Plu.RandomSize * 2 + 1) = 0;
		}
		else
		{
		    WORDtoASCZero(temp + 5, ApplVar.PluNumber + 1);
		    *(temp + 6) = 0;
		}
		if ((PLU_PRINT & 0x0f) == 1)	 /* only for plu# */
		    return temp;
		if (!cmd)	   /* R & J & SLIP */
		    PrintStr(temp);
		else		    /* type = 1 then black if type == 2 then red */
		    PrintKp(cmd, temp);
    }
	if (cmd && ApplVar.AP.Plu.CapSize)
		return ApplVar.Plu.Name;			/* on ApplVar.KP always PLU name when active */
    switch (PLU_PRINT & 0x0f)
    {
	case 4: 	    /* print depart caption */
	case 5 :	    /* print PLU# + Depart name */
	    return ApplVar.Dept.Name;
	default:
	    return ApplVar.Plu.Name;
    }
}


/*
    ApplVar.Dept.Options
    BIT 0 - If set then price is negative
    BIT 1 - if set then Zero ApplVar.Price not allowed
    BIT 2 - if set Single Ticket
    BIT 3 - if set then Double ticket (also Single set)
    BIT 4 - Print Line after article
    BIT 5 - if Set single item when start of sale on TEND#1
    BIT 6 - Add to Discount Itemizer 1
    BIT 7 - Add to Discount Itemizer 2
*/
void ProcessDept()
{
    static BYTE ln1st;//ccr2016-01-26存放部类的第行显示位置
    char temp[PRTLEN];	 /* used for converting a number */
    char *str;

	WORD	i;
    BCD  sMaxPrice,sMulti;

    ApplVar.FCurr = 0;

	RESETBIT(ApplVar.SubDiscFlag,BIT2);	/* cc 2006-10-28 for SubDisc */
	if(ApplVar.CentralLock != RG && ApplVar.CentralLock != MG)
		return;

    RESETBIT(ApplVar.MyFlags,(HANZIMODE | PWDMODE));//ccr20140514 清除汉字输入和密码输入标志  //
    RESETBIT(ApplVar.ScreenStat,HZINPUTMODE);     //ccr20140514

#if (defined(CASE_INDONESIA) && (defined(CASE_GPRS)))
    //在没有打开收据的情况下，检查是否必须发送流水
    if (!ApplVar.FRegi && !TESTBIT(ApplVar.DaysMust,BIT7) && ApplVar.DaysMust && //ccr2015-05-06 不是自动发送数据，检查是否超过天数
        ((GetDaysFrom1900(Now.year,Now.month,Now.day) - ApplVar.LastDaysLog)>=ApplVar.DaysMust))
    {
        if (ApplVar.FlowHeader.ReadP==ApplVar.FlowHeader.NextNP)//ccr2015-05-06无流水数据，
            ApplVar.LastDaysLog = GetDaysFrom1900(Now.year,Now.month,Now.day);
        else
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI82);
            return;
        }
    }
#endif

    if ((ApplVar.Key.Code == ApplVar.OldKey.Code && !ApplVar.OldMultiCount) || ApplVar.FVoid)	 /* same or void */
    {	//Multi Input of the same DEPT
		ApplVar.MultiplyCount = ApplVar.OldMultiCount;
		ApplVar.TaxTemp = ApplVar.TaxTemp1;	    /* restore Temp ApplVar.Tax Shift */
		if (!ApplVar.FVoid) /* no void */
		{
		    if (ApplVar.RepeatCount < 200)
				ApplVar.RepeatCount++;
		    else
				ApplVar.RepeatCount = 1;
		}
		else
		{
		    ApplVar.RepeatCount--;
		    ApplVar.ModiCount = ApplVar.OldModiCount;
		}
    }//end of if ((ApplVar.Key.Code == ApplVar.OldKey.Code && !ApplVar.OldMultiCount) || ApplVar.FVoid)
    else
    {				 /* not same DEPT and no void */
		ApplVar.OldModiCount = 0;
		if (ApplVar.Key.Code < PLU1)
		{//为部类销售
			if (ApplVar.Key.Code == DEPT)
		    {
				if (!Appl_EntryCounter)
				{
				    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
				    return;
				}
				ApplVar.DeptNumber = ApplVar.NumberEntry;
				if (!ApplVar.FPrice)	    /* amount entered? */
				{
					ApplVar.DeptNumber--;
			    	ReadDept();
					ApplVar.Price = ZERO;
					memcpy(ApplVar.Price.Value,ApplVar.Dept.Price,ApplVar.AP.Dept.PriceSize);
					ApplVar.DeptNumber++;
				}
		    }
		    else
		    {
				ApplVar.DeptNumber = ApplVar.Key.Code - DEPT;
				if (TESTBIT(ApplVar.ArrowsAlfa,BIT7))
					  	ApplVar.DeptNumber +=ApplVar.DeptKeys;

				if (!TESTBIT(SLFLAG,BIT7))
					RESETBIT(ApplVar.ArrowsAlfa,BIT7);
				if (!ApplVar.FPrice)
				{
				    if (AmtInputMask())
						return;
			    	//ReadDept();
				    if (Appl_EntryCounter)
						ApplVar.Price = ApplVar.Entry;
				    else
				    	ApplVar.Price = ZERO;
				}
				else if (Appl_EntryCounter)
				{
				    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
				    return;
				}
		    }
#if defined(ARTLEVEL1) //ccr2016-01-27>>>>
            if (TESTBIT(ARTLEVEL, BIT5))	/* Article shift also for DEPART? */
				ApplVar.DeptNumber += ApplVar.PluArtLevel;
#endif
		    if (!ApplVar.DeptNumber || ApplVar.DeptNumber > ApplVar.AP.Dept.Number)
		    {
				ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
				return;
		    }
		    ApplVar.FPlu = 0;
		    ApplVar.DeptNumber--;
		}
		else	 /* must be ApplVar.Plu? */
		{//为单品销售
			if (ApplVar.PluDept < ApplVar.AP.Dept.Number)
			    ApplVar.DeptNumber = ApplVar.PluDept;
			else
				ApplVar.DeptNumber = 0;
		    if (!ApplVar.FPrice)
		    {
				ApplVar.Price = ZERO;	/* clear previous price */
				memcpy(ApplVar.Price.Value, ApplVar.Plu.Price[ApplVar.PluPriceLevel], ApplVar.AP.Plu.PriceSize);
		    }
		    else if (TESTBIT(PLU_PRINT,BIT5))
		    {
		    	ApplVar.ErrorNumber=ERROR_ID(CWXXI35);
//		    	ApplVar.FPrice = 0;
		    	return;
		    }
		    ApplVar.FPlu = 1;
		}
		ReadDept();
		if (!CheckNotZero(&ApplVar.Price))
			for (i=0;i<ApplVar.AP.Dept.PriceSize;i++)
				ApplVar.Price.Value[i] = ApplVar.Dept.Price[i];

		sMaxPrice =ZERO;
		ApplVar.ErrorNumber=0;
		for (i=0;i<ApplVar.AP.Dept.PriMaxSize;i++)
		{
			ApplVar.ErrorNumber |= (ApplVar.Dept.PriceMax[i] !=0);
			sMaxPrice.Value[i] = ApplVar.Dept.PriceMax[i];
		}

		if (ApplVar.ErrorNumber && CompareBCD(&ApplVar.Price,&sMaxPrice)>0)
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI18);	/* zero price not allowed enter amount */
		    return;
		}
		ApplVar.ErrorNumber=0;
		if (!ApplVar.FInv && TESTBIT(ApplVar.Dept.Options, BIT1) &&
			!CheckNotZero(&ApplVar.Price))   /* ZERO price allowed ? */
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI43);	/* zero price not allowed enter amount */
		    return;
		}
		if (!ApplVar.MultiplyCount)
		    ApplVar.Qty = ONE;
		ApplVar.RepeatCount = 1;
		if (TESTBIT(ApplVar.Dept.Options, BIT0))   /* negative ? */
		    ApplVar.Price.Sign = 0x80;	   /* make price negative */
		if (ApplVar.FPb)
		    ApplVar.Dept.Print = ApplVar.PbF.Print;
		if (ApplVar.Dept.Group < ApplVar.AP.Group.Number)
			ApplVar.GroupNumber = ApplVar.Dept.Group;
		else
			ApplVar.GroupNumber = 0;
		ReadGroup();
    }
    if (ApplVar.FInv && ApplVar.FInv < 3 && !ApplVar.MultiplyCount)    /* inventory and no entry then not print */
		ApplVar.PrintLayOut = 0;
    else
    {
		if (!ApplVar.FInv)
		    ApplVar.PrintLayOut = ApplVar.Dept.Print;
		else
		{
		    ApplVar.PrintLayOut = 0x03;     /* R & J */
		    ApplVar.Price = ZERO;	    /* clear current price */
		}
    }
    if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund) /* void, corr, refund */
    {
		SETBIT(ApplVar.SubDiscFlag,BIT2);	/* cc 2006-10-28 for SubDisc */
		ApplVar.Qty.Sign ^= 0x80;
    }
    if (ApplVar.FCanc)
		ApplVar.Qty.Sign ^= 0x80;

	if (ApplVar.FBarcode==2 || ApplVar.FBarcode==3)	//ccr 040412
	{
	    ApplVar.Amt = ApplVar.Entry;
	    ApplVar.Amt.Sign |= (ApplVar.Qty.Sign & 0x80);
	}
	else
	{
	    ApplVar.Amt = ApplVar.Price;
	    Multiply(&ApplVar.Amt, &ApplVar.Qty);
	    RoundBcd(&ApplVar.Amt, 0);
	    if (ApplVar.Qty.Sign & 0x03)     /* ApplVar.Qty with decimal point? */
			AmtRound(1, &ApplVar.Amt);
	}
    if (!ApplVar.FInv  && CheckMaxEntry(&ApplVar.Amt, ApplVar.Group.Family))
    /*
	     CompareBCD(&ApplVar.Amt, &MILLION) > -1)) */
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI18);
		return;
    }
	if (!ApplVar.FInv)
	{
/*		if ((ApplVar.Qty.Sign & 0x03) && CompareBCD(&ApplVar.Qty, &THOUSAND10) > -1)
		{
			ApplVar.ErrorNumber=ERROR_ID(CWXXI18);
		    return;
		}
		else*/
		if (CompareBCD(&ApplVar.Qty, (BCD*)&THOUSAND10) > -1)
	    {
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI18);
			return;
		}
    }
	if (TESTBIT(PBINFO, BIT0) && !ApplVar.PbNumber && !TESTBIT(ApplVar.FNoPb, BIT0) && !ApplVar.FInv)  /* pb Compulsory */
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI10);
		return;
    }
//   if (RegiStart())  //ccr050411
//		return;  /* check if start of registration */
    if (!ApplVar.FInv)
    {
	   if (RegiStart())
			return;  /* 增加库存时，不打印票头 check if start of registration */

		if (ApplVar.FVoid)
		{
			ApplVar.RGRec.Qty = ApplVar.Qty;	/* store ApplVar.Qty */
			ApplVar.RGRec.Amt = ApplVar.Amt;	/* store amount not price */
			ApplVar.RGRec.DiscAmt = ApplVar.Amt;	/* cc 2006-10-20 for SubDisc */
		}
		StoreInBuffer(); /* 先将上一次销售项目的数据存入 RGBuf */
		if (ApplVar.RepeatCount > 1)
		    ApplVar.ModiCount = ApplVar.OldModiCount;
		else
		    ApplVar.OldModiCount = 0;
    }


    ApplVar.Cost = ZERO;
    if (!ApplVar.FPlu)	 /* ApplVar.Dept ? */
		ApplVar.RGRec.Key.Code = DEPT + 1 + ApplVar.DeptNumber;
    else
    {
		if (ApplVar.AP.Plu.Cost && !TESTBIT(ART_LEVEL, BIT2))
		{
		    memcpy(ApplVar.Cost.Value, ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
		    Multiply(&ApplVar.Cost, &ApplVar.Qty);
		    RoundBcd(&ApplVar.Cost, 0);
		}
		ApplVar.RGRec.Key.Code = PLU1 + 1 + ApplVar.PluNumber;
    }
    //保存最后一个销售项目的数据
    ApplVar.RGRec.Qty = ApplVar.Qty;
    ApplVar.RGRec.Amt = ApplVar.Price;
	ApplVar.RGRec.DiscAmt = ApplVar.Price;		/* cc 2006-10-20 for SubDisc */
    Multiply(&ApplVar.RGRec.DiscAmt,&ApplVar.Qty);		//cc 2007-02-13 for SubDisc
    ApplVar.RGRec.Group = ApplVar.GroupNumber;
	if (ApplVar.FPlu)
	{
		ApplVar.RGRec.Amt.Value[BCDLEN-1] = ApplVar.PluPriceLevel;
	}

	if (ApplVar.FCorr && (!ApplVar.PbNumber || (!ApplVar.AP.StartAddress[AddrTrack])))
    {
		ApplVar.BufCmd = 2;	    /* only check if used */
		StoreInBuffer();    /* check if article used */
		if (ApplVar.ErrorNumber)//== ERROR_ID(CWXXI16)
        {
            ApplVar.RGRec.Key.Code = 0;//ccr20130815,避免将输错的销售项目存入ApplVar.RGItems.RGBuf中
		    return;
        }
    }
    if (ApplVar.FPluEnq != 2 || 		/* if Link not store */
			!ApplVar.AP.Pb.ArtAmt ||	/* when no amount stores the always store */
				TESTBIT(ApplVar.AP.Options.Plu, BIT2)) /* always store link ? */
	{
		if (StorePbBuffer())	 /* store in ApplVar.PB buffer */
	    {
		    ApplVar.RGRec.Key.Code = 0;
			return;
	    }
	}
    if (ApplVar.FCorr || ApplVar.FRefund)
    {
		ApplVar.RGRec.Key.Code = CORREC + 1 + ApplVar.CorrecNumber;
		ApplVar.RGRec.Amt = ApplVar.Amt;		/* store total amount not price */
		ApplVar.RGRec.DiscAmt = ApplVar.Amt;		/* cc 2006-10-20 for SubDisc */
		if (ApplVar.FRefund)
			ApplVar.BufCmd = 0;				/* no consolidation for refund */
		StoreInBuffer();      /* store correction or refund function */
		if (ApplVar.FRefund)
			ApplVar.BufCmd = 0;				/* no consolidation for refund */
	    ApplVar.RGRec.Amt = ApplVar.Price;
		ApplVar.RGRec.DiscAmt = ApplVar.Price;		/* cc 2006-10-20 for SubDisc */
		Multiply(&ApplVar.RGRec.DiscAmt,&ApplVar.Qty);		//cc 2007-02-13  for SubDisc

		if (!ApplVar.FPlu)   /* ApplVar.Dept ? */
	    	ApplVar.RGRec.Key.Code = DEPT + 1 + ApplVar.DeptNumber;
		else
		{
	    	ApplVar.RGRec.Key.Code = PLU1 + 1 + ApplVar.PluNumber;
			ApplVar.RGRec.Amt.Value[BCDLEN-1] = ApplVar.PluPriceLevel;
			ApplVar.RGRec.DiscAmt.Value[BCDLEN-1] = ApplVar.PluPriceLevel;		/* cc 2006-10-20 for SubDisc */
		}
    }

    if (ApplVar.RepeatCount > 1 || (ApplVar.FVoid && ApplVar.RepeatCount))
    {
		if (ApplVar.RepeatCount > 9)
		{
			if (ApplVar.RepeatCount<=99)
				temp[3] = '0'+ (ApplVar.RepeatCount / 10);
			else
				temp[3] = 'A';
		}
		else
		    temp[3] = ' ';
		temp[4] = '0'+(ApplVar.RepeatCount % 10);
		temp[5] = 0;
    }
    else
	    if (!ApplVar.FPlu || ApplVar.AP.Plu.RandomSize)
	    {
			WORDtoASCZero(temp + 5, ApplVar.DeptNumber + 1);
			temp[3] = 'D';
    		temp[6] = 0;       /* terminate string */
	    }
    	else
    	{
			WORDtoASCZero(temp + 6, ApplVar.PluNumber + 1);
    		temp[7] = 0;       /* terminate string */
    	}
    if (!ApplVar.FInv)
   	{
#if defined(LCD10_DISP)
        LCD10_SetFlag(FLAG_PRICE);
        PutsO(DispAmtStr(0, &ApplVar.Amt,DISLEN));//C-DISPLAY
#else
        str = DispAmtStr(temp + 3, &ApplVar.Amt,DISLEN);//ccr 050601
		PutsO(str);   /* 在客显上显示销售项目 *///C-DISPLAY
#endif
		//ccr200130425 DispSt1C(sstr)/* 在主显的状态行显示销售项目 */
   	}
    if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund) /* void, corr, refund */
    {
		ApplVar.PrintLayOut = ApplVar.Correc.Print;
			PrintStr(ApplVar.Correc.Name);
		if (ApplVar.FRefund)   /* refund ? */
		    ApplVar.RetQty = ApplVar.Qty;
    }
    else if (!CheckNotZero(&ApplVar.RetQty))	/* already zero ? */
		ApplVar.RetQty.Sign = ApplVar.Qty.Sign & 0x03;	/* decimal point must remain same */
    ApplVar.PrintLayOut = ApplVar.Dept.Print | (DBLHIPRN & BIT2);

//    if (!ApplVar.FInv)
//		PrintMultiply();//print ApplVar.Qty * ApplVar.Amt(12 x 12.50)
    if (ApplVar.FPlu)
		str = GetPluPrint(temp, KP_NORMAL);   /* get plu print receipt & journal & slip */
    else
		str = ApplVar.Dept.Name;

    if (ApplVar.FInv)	    /* inventory ? */
    {
		if (ApplVar.FInv == 2)
	    	ApplVar.Qty.Sign ^= 0x80;	/* invert sign if subtract */
		if (ApplVar.MultiplyCount)	/* new entry ? */
		{
		    if (ApplVar.FInv == 3)	/* easy price set ? */
		    {
				ApplVar.Price = ApplVar.Qty;
				memcpy(ApplVar.Plu.Price[ApplVar.PluPriceLevel], ApplVar.Price.Value, ApplVar.AP.Plu.PriceSize);
				WritePlu();
				if(TESTBIT(PBPRINT,BIT5))
					PrintAmt(str, &ApplVar.Price);
				else
				    DispStrLF(FormatQtyStr(str, &ApplVar.Price, SCREENWD));
		    }
		    else
		    {
				Add(&ApplVar.PluInventory, &ApplVar.Qty);
				if(!ApplVar.FTrain)
					WritePluInventory();
				if(!TESTBIT(PBPRINT,BIT5))
				{
					DispStrLF(FormatQtyStr(Msg[KCZJIA+ApplVar.FInv-1].str, &ApplVar.Qty, SCREENWD));
					DispStrLF(FormatQtyStr(str, &ApplVar.PluInventory, SCREENWD));
				}
				else
				{
					PrintQty(Msg[KCZJIA+ApplVar.FInv-1].str, &ApplVar.Qty);	/* change inventory with */
					PrintQty(str, &ApplVar.PluInventory);
				}
		    }
		}
        else//没有输入库存数量
        {//ccr2017-01-09>>>>>>显示库存或价格>>>>>>>>
            mClearScreen();
            DispStrXY(ApplVar.Plu.Name,0,0);
            if (ApplVar.FInv == 3)
                DispStrXY(DispAmtStr(0, &ApplVar.Price,SCREENWD),0,1);
            else
                DispStrXY(DispQtyStr(0, &ApplVar.PluInventory,SCREENWD),0,1);
            DispStrXY(INVQTYMUST,0,2);
        }//ccr2017-01-09<<<<<<<<<<<<<<
    }
    else// if (!ApplVar.FInv)
    {
//		if (!ApplVar.FCanc && TESTBIT(ApplVar.Dept.Options, BIT2))   /* Single Ticket */
//		    ApplVar.PrintLayOut &= 0x0fd;   /* skip receipt */
        //ccr2016-03-04>>>>>>>>>>>>>>
		if (ApplVar.RepeatCount<=1)
        {
//ccr2016-03-04        if (SCREENLN-(TESTBIT(ApplVar.ScreenStat,STATEON))<CursY+3 || ApplVar.RepeatCount>1)//屏幕上空余行数<3
            mClearScreen();
#if (0)//SCREENLN>6)
            if (ApplVar.Key.Code>=PLU1 && ApplVar.AP.Plu.RandomSize)
            {//显示单品代码
                ln1st=strlen(Msg[DAIMA].str);
                strcpy(temp,Msg[DAIMA].str);
                HEXtoASC(temp + ln1st, ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
                temp[ln1st+ApplVar.AP.Plu.RandomSize*2]=0;
                DispStrLF(temp);
            }
#endif
           ln1st=CursY;
        }
        else//ccr2016-03-04
		{//显示同一个商品的重复次数
            i=WORDtoASCL(temp+1,ApplVar.RepeatCount)+1;
    		temp[0] = ' ';
	    	DispStrXY(temp,SCREENWD-i,ln1st+1);
		}
        PrintQtyAmt(&ApplVar.Qty, str, (BCD *)&ApplVar.Amt,(ApplVar.RepeatCount<=1));   /* 打印和显示销售项目 */

        //ccr2016-03-04<<<<<<<<<<<<<<<<<<<<

		if (!TESTBIT(PLU_PRINT,BIT2) || !ApplVar.AP.Plu.CapSize)	/* print depart name on receipt */
			PrintModifiers();
		if (TESTBIT(ApplVar.Dept.Options, BIT4))    /* print line ? */
		{
		    MemSet(SysBuf, sizeof(SysBuf), '-');
		    PrintStr(SysBuf);
		}

		ApplVar.DiscAmt = ZERO;
		ApplVar.FMixMatch = 0;
		if (ApplVar.FPlu)
		{
			Collect_Data(PLULOG);
		    CheckMixMatch(1);
		}
		else
			Collect_Data(DEPTLOG);

		AddGroupTotal();

		AddDeptTotal();

		if (ApplVar.FPlu)   /* ApplVar.Plu ? */
		{

			if (TESTBIT(ApplVar.MyFlags,PLUDISC) && CheckNotZero(&ApplVar.DiscAmt))// 当为由特殊出价进行打折时，保存折扣流水  //
			{//ccr 050613
				sMaxPrice = ApplVar.Amt;
				sMulti=ApplVar.Entry;
//				Subtract(&ApplVar.Amt, &ApplVar.DiscAmt);// 将特殊出价折扣金额追加到单品价格上，恢复单品的原始价格 //
//				Collect_Data(PLULOG);

				ApplVar.Amt = ApplVar.DiscAmt;
				ApplVar.Entry = ZERO;
				memcpy(ApplVar.Entry.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);
				Collect_Data(DISCADDLOG);

				ApplVar.Entry = sMulti;
				ApplVar.Amt = sMaxPrice;

			}
//			else
//				Collect_Data(PLULOG);

		    AddPluTotal();
			if (ApplVar.AP.Plu.InvSize)	/* inventory ? */
		    {
				if (ApplVar.FCanc != 2) /* Not update inventory when cancel 2 */
				{
				    Subtract(&ApplVar.PluInventory, &ApplVar.Qty);
				    if(!ApplVar.FTrain)
						WritePluInventory();
				}
		    }
		}
		AddTaxItem(ApplVar.Dept.Tax);	    /* add to tax itemizers */
		if (TESTBIT(ApplVar.Dept.Options, BIT6))
		    Add(&ApplVar.DiscItem[0], &ApplVar.Amt);
		if (TESTBIT(ApplVar.Dept.Options, BIT7))
		    Add(&ApplVar.DiscItem[1], &ApplVar.Amt);
		if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund) /* void, corr, refund */
		{
		    AddCorrecTotal();
		    ApplVar.Key.Code = 0;	/* no repeat */
		}
		if (ApplVar.FCanc == 1)
		{
		    ApplVar.CorrecNumber = ApplVar.CancelNumber;
		    AddCorrecTotal();		/* add in cancel */
		}
    }//end if
    if (ApplVar.FCanc)		/* invert sign back */
		ApplVar.Qty.Sign ^= 0x80;
    ApplVar.FPrice = 0;
    ApplVar.OldMultiCount = ApplVar.MultiplyCount;
    ApplVar.MultiplyCount = 0;
    if (ApplVar.FVoid)
		ApplVar.Qty.Sign ^= 0x80;   /* invert sign back */
	ApplVar.FVoid = 0;
    ApplVar.FCorr = 0;
    ApplVar.FRefund = 0;
	if (ApplVar.FPlu && ApplVar.Plu.Link)		/* Linked ApplVar.Plu ? */
		return;
    ApplVar.RetQty = ZERO;
    if (PRICE_LEVEL & 0x01)
		ApplVar.PluPriceLevel = 0;
#if defined(ARTLEVEL1) //ccr2016-01-27>>>>
    if ((ART_LEVEL & 0x03) == 1)
		ApplVar.PluArtLevel = 0;
#endif
	if (!ApplVar.FSale && !ApplVar.FPb && ApplVar.AP.Tend.Number && TESTBIT(ApplVar.Dept.Options,BIT5))
	{
		ClearEntry(false);
		ApplVar.FSale = 1;
		ApplVar.Key.Code = TEND+1;
		Tender();
	}
	else
	    ApplVar.FSale = 1;
	if (!ApplVar.FInv)	//ccr2017-01-09    /* inventory ? */
		DispSubTotal();
	ApplVar.FBalance = 0;		//cc 2006-06-29 for Balance
}
