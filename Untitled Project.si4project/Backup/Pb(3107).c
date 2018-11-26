#define PB_C 1
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"


/****************************************************************/

short StorePbInvoice(short check)
{
    char str[7];
    WORD used, invoice;
    UnLong offset;
    BCD temp;

    if (!TESTBIT(ApplVar.AP.Pb.Random, BIT6) || ApplVar.FTrain)
        return 0;
    RamOffSet =  (ApplVar.AP.Pb.Random & 0x0f) + ApplVar.AP.Pb.Text + ApplVar.AP.Pb.AmtSize + 6 ;//????????????????????????
    if (TESTBIT(PBINFO, BIT7))  /* Discount item stored ? */
        RamOffSet += ((ApplVar.AP.Pb.AmtSize + 1) << 1) ;
    RamOffSet = RamOffSet * ApplVar.AP.Pb.NumberOfPb + ApplVar.AP.StartAddress[AddrPBt] ;
    if (TESTBIT(ApplVar.AP.Pb.Random, BIT7))     /* Pb Trailer ? */
        RamOffSet += 50*24 ;//???????????????????????????
    offset = RamOffSet ;     /* save start */
    ReadRam((char*)&used, 2);      /* read entries used */
    if (check == 1)     /* check if full */
    {
        if (used < 200)
            return 0;
        else        /* buffer full */
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI15);
            return 1;
        }
    }
    if (check == 2)  /* print report */
    {
        temp = ZERO;
        while (used)     /* print report */
        {
            MemSet(str, sizeof(str), ' ');
            str[sizeof(str) - 1] = 0;
            ReadRam((char*)&invoice, 2);
            WORDtoASC(str + 5, invoice);
            if ((CHARSET & 0xf0) == 0x10 || (SLIP != 4 &&
                ((CHARSET & 0x0f) == 3 || (CHARSET & 0x0f) == 7)))  /* number sign not available */
                str[0] = 'N';
            else
                str[0] = '#';
            ReadRam((char*)temp.Value, 4);
            if (TESTBIT(temp.Value[3], BIT7))
            {
                temp.Sign = 0x80;
                temp.Value[3] &= 0x0f;
            }
            else
                temp.Sign = 0;
            PrintAmt(str, &temp);
            used--;
        }
        PrintLine('-');
        if (ApplVar.FReport == Z)
        {
            RamOffSet = offset ;
            WriteRam((char*)&used, 2);
        }
        return 0;
    }
    used++;
    RamOffSet = offset;
    WriteRam((char*)&used, 2);
    used--;
    used = (used<<2)+(used<<1);//*6;
    RamOffSet += used ;        /* calculate offset */
    temp = ApplVar.SubTotal;
    temp.Value[3] &= 0x0f;  /* digit 8 is sign used for sign */
    if (TESTBIT(ApplVar.SubTotal.Sign, BIT7))   /* negative ? */
        temp.Value[3] |= 0x80;
    WriteRam((char*)&ApplVar.PbInvoice, 2);        /* write ApplVar.Total */
    WriteRam((char*)temp.Value, 4);
    ApplVar.PbInvoice++;
    if (ApplVar.PbInvoice > 9999)
        ApplVar.PbInvoice = 1;
    return 0;
}

void GetPbFOffSet()
{
    RamOffSet =  (UnLong)ApplVar.PbFNumber* ApplVar.AP.Pb.RecordSize + ApplVar.AP.StartAddress[AddrPBf] ;
}

void AddPbFTotal()
{

    GetPbFOffSet();
    RamOffSet += ApplVar.AP.Pb.TotalOffSet ;
    for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
    {
        ApplVar.Size = ApplVar.AP.Pb.Size[ApplVar.PointerType];
        AddPointerTotal();
    }
}

void WritePbF()
{
    if (ApplVar.PbFNumber < ApplVar.AP.Pb.Number)
    {
        GetPbFOffSet();

        WriteRam((char*)&ApplVar.PbF, ApplVar.AP.Pb.TotalOffSet);      /* write function options */
/*      WriteRam(ApplVar.PbF.Name, ApplVar.AP.Pb.CapSize);*/
    }
}

void ReadPbF()
{
    GetPbFOffSet();

    ReadRam((char*)&ApplVar.PbF,ApplVar.AP.Pb.TotalOffSet);      /* write function options */
/*        ReadRam(ApplVar.PbF.Name, ApplVar.AP.Pb.CapSize);*/
}

void GetPbtOffSet(short pbnum)  //ccr 2003-10-31
{
    if (pbnum<0)
        pbnum = (ApplVar.PbNumber-1);
    RamOffSet =  (UnLong)pbnum* ApplVar.AP.Pb.PBTRecordSize + ApplVar.AP.StartAddress[AddrPBt] ;
}

void AddPbtTotal()  //ccr 2003-10-31
{
    GetPbtOffSet(-1);
    RamOffSet += ApplVar.AP.Pb.PBTTotalOffset ;
    for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
    {
        ApplVar.Size = ApplVar.AP.Pb.PBTSize[ApplVar.PointerType];
        AddPointerTotal();
    }
}


/* cmd 0 = read PB total */
/* cmd 1 = write PB total */
/* cmd 2 = reset PB total */
/* cmd 3 = Write PB info (random + text) */
/* cmd 4 = Only get Offset */
/* cmd 5 = Only reset ApplVar.PB Extra Count for CUBA */

void PbTotal(WORD pbnum, BYTE cmd)
{
    WORD newblock;
    UnLong save;

    if (ApplVar.FProforma && cmd > 0  && cmd < 4) /* when proforma only read */
        return;
    pbnum--;
    GetPbtOffSet(pbnum);
    if (cmd == 4)   /* only get offset */
        return;
    if (cmd == 2 && ApplVar.AP.StartAddress[AddrTrack]>0)       /* reset track buffer ?*/
    {
        save = RamOffSet;
        ReadRam((char*)&ApplVar.PB.Block, sizeof(ApplVar.PB.Block));   /* read start block */
        if (ApplVar.PB.Block)   /* used so link to free */
        {
            newblock = ApplVar.PB.Block;    /* save start */
            for (;;)
            {
                ApplVar.PB.Block--;             /* minus 1 for offset calculation */
                RamOffSet = ApplVar.AP.StartAddress[AddrTrack] + (UnLong)ApplVar.PB.Block * ApplVar.AP.Pb.BlockSize ;
                ReadRam((char*)&ApplVar.PB.Block, sizeof(ApplVar.PB.Block));   /* next block ? */
                if (!ApplVar.PB.Block)  /* end block reached */
                {
                    RamOffSet -=  sizeof(ApplVar.PB.Block);
                    ApplVar.EmptyBlock++;
                    WriteRam((char*)&ApplVar.EmptyBlock, sizeof(ApplVar.PB.Block));    /* link to other */
                    ApplVar.EmptyBlock = newblock - 1;  /* free pb block */
                    break;
                }
            }
        }
        RamOffSet = save ;
    }
    if (!cmd || cmd == 2)   /* read or reset */
        MemSet(&ApplVar.PB, sizeof(ApplVar.PB), 0);
    if (!cmd) /* read */
        ReadRam((char*)&ApplVar.PB.Block, sizeof(ApplVar.PB.Block));
    else if (cmd == 3)
        RamOffSet += sizeof(ApplVar.PB.Block);
    else
        WriteRam((char*)&ApplVar.PB.Block, sizeof(ApplVar.PB.Block));
    if (ApplVar.AP.Pb.Random & 0x0f)
    {
        if (!cmd || cmd == 2)   /* read */
            ReadRam((char*)ApplVar.PB.Random, ApplVar.AP.Pb.Random & 0x0f);
        else if (cmd == 3)  /* write new random number */
            WriteRam((char*)ApplVar.PB.Random, ApplVar.AP.Pb.Random & 0x0f);
        else
            RamOffSet += ApplVar.AP.Pb.Random & 0x0f;
    }
    if (ApplVar.AP.Pb.Text)
    {
        if (!cmd || cmd == 2)   /* read */
            ReadRam((char*)ApplVar.PB.Text, ApplVar.AP.Pb.Text);
        else if (cmd == 3)  /* write new text */
            WriteRam((char*)ApplVar.PB.Text, ApplVar.AP.Pb.Text);
        else
            RamOffSet += ApplVar.AP.Pb.Text ;
    }

    if (!cmd) /* read */
    {
        ReadRam((char*)&ApplVar.PB.Clerk, sizeof(ApplVar.PB.Clerk) + sizeof(ApplVar.PB.Lines) /*+ sizeof(ApplVar.PB.Pages)*/ + sizeof(ApplVar.PB.Covers));
        ReadRam((char*)&ApplVar.PB.Amt, ApplVar.AP.Pb.AmtSize + 1);
        if (TESTBIT(PBINFO, BIT7))      /* ApplVar.Disc Item stored ? */
        {
            ReadRam((char*)&ApplVar.PB.Disc1, ApplVar.AP.Pb.AmtSize + 1);
            ReadRam((char*)&ApplVar.PB.Disc2, ApplVar.AP.Pb.AmtSize + 1);
        }
    }
    else if (cmd == 3)  /* only write info */
    {
        RamOffSet += ( sizeof(ApplVar.PB.Clerk) + sizeof(ApplVar.PB.Lines) /*+ sizeof(ApplVar.PB.Pages)*/ + sizeof(ApplVar.PB.Covers) + ApplVar.AP.Pb.AmtSize + 1) ;
        if (TESTBIT(PBINFO, BIT7))  /* Discount item stored ? */
            RamOffSet += ((ApplVar.AP.Pb.AmtSize + 1) << 1);
    }
    else
    {
        WriteRam((char*)&ApplVar.PB.Clerk, sizeof(ApplVar.PB.Clerk)
                 + sizeof(ApplVar.PB.Lines) /*+ sizeof(ApplVar.PB.Pages)*/
                 + sizeof(ApplVar.PB.Covers));
        WriteRam((char*)&ApplVar.PB.Amt, ApplVar.AP.Pb.AmtSize + 1);
        if (TESTBIT(PBINFO, BIT7))      /* ApplVar.Disc Item stored ? */
        {
            WriteRam((char*)&ApplVar.PB.Disc1, ApplVar.AP.Pb.AmtSize + 1);
            WriteRam((char*)&ApplVar.PB.Disc2, ApplVar.AP.Pb.AmtSize + 1);
        }
    }
}


/* return 1 incase of error slip etc */

void PrintPbHeader()
{
    BYTE saveprint, str[17];
    BCD temp;

    saveprint = ApplVar.PrintLayOut;
    if (TESTBIT(ApplVar.PrintLayOut, BIT3)) /* print on slip */
    {
        if (ApplVar.SlipLines)      /* paper in slip ? */
        {
            if (ApplVar.PB.Lines)
            {
                SETBIT(ApplVar.MyFlags, ENSLIPPB);
                if (!TESTBIT(DOT, BIT7) && SLIP == 2)
                    FeedSlip(ApplVar.PB.Lines*2 + SLIP_TOP-2);
                else
                    FeedSlip(ApplVar.PB.Lines + SLIP_TOP);
                ApplVar.SlipLines += ApplVar.PB.Lines;
                RESETBIT(ApplVar.PrintLayOut, BIT3); /* reset print on slip */
            }
        }
    }
    SETBIT(ApplVar.PrintLayOut, BIT2);  /* double size */
    if ((ApplVar.PbF.Options & 0x0f) < 2)
    {
        ApplVar.SlipDouble = 1;     /* double size on slip */       //20040326
        PrintQty(Msg[ZUOHAO].str, &ApplVar.Entry);
        ApplVar.PrintLayOut = saveprint;
    }
//	ApplVar.SlipPage = ApplVar.PB.Pages;  // 20040326
    RESETBIT(ApplVar.PrintLayOut, BIT3); /* reset print on slip */   //20040326

    if (TESTBIT(ApplVar.FNoPb, BIT2) && TESTBIT(ApplVar.AP.Pb.Random, BIT6)) /* print Invoice number ? */
    {
        if (!ApplVar.PbInvoice)
            ApplVar.PbInvoice = 1;
        temp = ZERO;
        WORDtoBCD(temp.Value, ApplVar.PbInvoice);
        strncpy(str, Msg[CWXXI22].str,sizeof(str));
#if (SCRSTATDBL)
        DispOnStatDBL((char *)Msg[CWXXI33].str,0);
#else
        DispSt1C(Msg[CWXXI33].str); //RG-DISPLAY
#endif
        PrintQty(str, &temp);
    }
    if ((ApplVar.PbF.Options & 0x0f) > 2)
    {
        PrintQty(ApplVar.PbF.Name, &ApplVar.Entry);
    }
    RESETBIT(ApplVar.PrintLayOut, BIT2);
    if (ApplVar.AP.Pb.Text && ApplVar.PB.Text[0])
        PrintStr(ApplVar.PB.Text);
    PrintLine('=');
    ApplVar.FPb = 1;
}


short GetPbNumber()
{
    WORD free, pbsave;

    if (ApplVar.AP.Pb.Random & 0x0f)  /* random pb# */
    {
        if (Appl_EntryCounter > ((ApplVar.AP.Pb.Random & 0x0f) * 2))
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI09);
            return 0;
        }
        ApplVar.NumberEntry = 0;
        free = 0;
        pbsave = ApplVar.PbNumber;
        for (ApplVar.PbNumber = 1; ApplVar.PbNumber <= ApplVar.AP.Pb.NumberOfPb; ApplVar.PbNumber++)
        {
            PbTotal(ApplVar.PbNumber, 0);   /* read new */
            if (!ApplVar.PB.Block && ApplVar.PbNumber != pbsave)       /* free */
            {
                if (!free)
                    free = ApplVar.PbNumber;    /* first free ApplVar.PB */
                if (TESTBIT(PBINFO, BIT2) || TESTBIT(PBPRINT, BIT7))  /* check number ? */
                    continue;
            }
            if (!CompareBCDValue(ApplVar.Entry.Value, ApplVar.PB.Random, ApplVar.AP.Pb.Random & 0x0f)) /* found */
            {
                ApplVar.NumberEntry = ApplVar.PbNumber;
                break;
            }
        }
        ApplVar.PbNumber = pbsave;
        if (!ApplVar.NumberEntry && free &&
            (TESTBIT(PBINFO, BIT2) || TESTBIT(PBPRINT, BIT7)))  /* check# ? */
        {
            ApplVar.NumberEntry = free;
            PbTotal(free, 4);   /* only get offset */
            RamOffSet += sizeof(ApplVar.PB.Block);
            memcpy(ApplVar.PB.Random, ApplVar.Entry.Value, ApplVar.AP.Pb.Random & 0x0f);
            WriteRam((char*)ApplVar.PB.Random, ApplVar.AP.Pb.Random & 0x0f);  /* write new number */
        }
    }
    if (!ApplVar.NumberEntry || ApplVar.NumberEntry > ApplVar.AP.Pb.NumberOfPb)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI09);
        return 0;
    }
    return 1;
}

void ClearPb()
{
    if (!TESTBIT(PBINFO,BIT3)) /* if split and new system */
    {
        PbTotal(ApplVar.PbNumber, 0);       /* read total */
        if (ApplVar.FSplit)
        {
            ApplVar.Amt = ApplVar.SubTotal;
            Subtract(&ApplVar.PB.Amt, &ApplVar.Amt);
            Subtract(&ApplVar.PB.Disc1, &ApplVar.DiscItem[0]);
            Subtract(&ApplVar.PB.Disc2, &ApplVar.DiscItem[1]);
            PbTotal(ApplVar.PbNumber, 1);   /* write new total */
            ApplVar.FSplit = 0; /* reset otherwise checkout not added */
            ApplVar.BufKp = 0;      /* don't print on ApplVar.KP */
        }
        else
        {
            ApplVar.Amt = ApplVar.PB.Amt;
            Add(&ApplVar.SaleAmt, &ApplVar.PB.Amt);
            Add(&ApplVar.SubTotal, &ApplVar.PB.Amt);
            Add(&ApplVar.DiscItem[0], &ApplVar.PB.Disc1);
            Add(&ApplVar.DiscItem[1], &ApplVar.PB.Disc2);
            PbTotal(ApplVar.PbNumber, 2);       /* reset total */
        }
    }
    else
    {
        PbTotal(ApplVar.PbNumber, 2);       /* reset total */
    }
    ApplVar.FPb = 0;
}

void PrintNewBal()
{
    BCD amt;

    if (TESTBIT(PBPRINT,BIT2))   /* print new balance */
    {
        ApplVar.PrintLayOut = ApplVar.PbF.Print & 0x0b;  /* set slip & R&J */
        amt = ApplVar.PB.Amt;
        AmtRound(0, &amt);              /* Sales Amount rounding */
        PrintAmt(Msg[GUAZHE].str, &amt);
    }
}

//jdb2018-10-24餐饮结账不需要按FINALZE，加入TENDER按键功能前面执行
void PbTender()
{
    BYTE autochk;
    BYTE ENP = 1,SPFG;

	SETBIT(ApplVar.PrintFlag, BIT0);//jdb2018-10-23不打印

    ApplVar.PbFNumber = 3;
    ApplVar.PrintLayOut = ApplVar.PbF.Print;    /* save previous print */
    if (ApplVar.PbFNumber < ApplVar.AP.Pb.Number)
        ReadPbF();           /* read function */
    else
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
        return;
    }
    autochk = 0;
    ApplVar.Qty = ZERO;

	if (ApplVar.TaxPerm)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        return;
    }

        if (!TESTBIT(ApplVar.MyFlags, PRNONPB))
            ENP = 1;
        else
        {
            ENP = 0;
            RESETBIT(ApplVar.MyFlags, PRNONPB);   //  |
        }                                 //  |___>// lyq added 20040224
        
        if (!TESTBIT(PBINFO,BIT3))
            ApplVar.ErrorNumber=ERROR_ID(CWXXI42);        /* not allowed with new system */
        else if (!ApplVar.PbNumber)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI10);
        else if (!ApplVar.FPb)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI19);
        else
        {
            ApplVar.PrintLayOut &= ApplVar.PbF.Print;   /* and open ApplVar.PB print */
            PbTotal(ApplVar.PbNumber, 0);       /* read total */
            if (ApplVar.FSplit)
            {
                RegiEnd();
                ApplVar.FSale = 0;
                ApplVar.Amt = ApplVar.SubTotal;
                Subtract(&ApplVar.PB.Amt, &ApplVar.Amt);
                Subtract(&ApplVar.PB.Disc1, &ApplVar.DiscItem[0]);
                Subtract(&ApplVar.PB.Disc2, &ApplVar.DiscItem[1]);
                PbTotal(ApplVar.PbNumber, 1);   /* write new total */
                ApplVar.FSplit = 0;
                ApplVar.PbNumber = 0;
                AmtRound(0, &ApplVar.Amt);              /* Sales Amount rounding */
                PrintAmt(Msg[YINGFU].str, &ApplVar.Amt);
                ApplVar.SaleAmt = ApplVar.Amt;
                if (PVAT)
                {
                    CalculateTax(3);        /* print VAT */
                    ApplVar.Amt = ApplVar.SaleAmt;          /* restore amount */
                }
                MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);
                ApplVar.RGItems.RGNumber = 2;
            }
            else
            {
                StoreInBuffer();
                ApplVar.Amt = ApplVar.PB.Amt;
                AmtRound(0, &ApplVar.Amt);              /* Sales Amount rounding */
/* JB Modify */
/*                    ApplVar.DiscItem[0] = ApplVar.PB.Disc1;
            ApplVar.DiscItem[1] = ApplVar.PB.Disc2;      */
                Add(&ApplVar.SaleAmt, &ApplVar.Amt);
            }

#if defined(LCD10_DISP)
                LCD10_SetFlag(FLAG_TOTAL);
                PutsO(DispAmtStr(0, &ApplVar.Amt,DISLEN));//C-DISPLAY
#else
                PutsO(DispAmtStr(Msg[CHECKPAID].str, &ApplVar.Amt,DISLEN));//C-DISPLAY
#endif
//			    DispStrLF(DispAmtStr(Msg[QRJZHANG].str, &ApplVar.Amt,DISLEN));
                ApplVar.FPb = 0;

            if (ENP == 0)     //lyq added 20040224 ,added enp varialbe
            {
                SETBIT(ApplVar.PrintLayOut, BIT2);
                ApplVar.Entry = ZERO;
                ULongToBCDValue(ApplVar.Entry.Value, ApplVar.PbNumber);
                PrintQty(Msg[ZUOHAO].str, &ApplVar.Entry);
                RESETBIT(ApplVar.PrintLayOut, BIT2);
                PrintLine('=');
            }
            ApplVar.RGRec.Amt = ApplVar.Amt;
            ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
            SPFG = ApplVar.PrintLayOut;        //
            RESETBIT(ApplVar.PrintLayOut, BIT3);//20040326
            if  (CheckNotZero(&ApplVar.Amt))   //ccr2016-07-20为0时不打印"确认结账"/FINALIZE
                PrintAmt(ApplVar.PbF.Name, &ApplVar.Amt);
            ApplVar.PrintLayOut = SPFG;       //20040326

            AddPbFTotal();
            RESETBIT(ApplVar.FNoPb, BIT0+BIT1);      /* reset no tender */
        }

	if (ApplVar.ErrorNumber)
    {
        if (ApplVar.RGRec.Key.Code == ApplVar.Key.Code)
            ApplVar.RGRec.Key.Code = 0;
    }

	if (TESTBIT(ApplVar.PbF.Options, BIT7))
		ApplVar.FNoTime = 1;	/* set don't print time */

}

//jdb2018-10-24
void PbPrint()
{
	BYTE autochk;
	BYTE ENP = 1,SPFG;

	RESETBIT(ApplVar.PrintFlag, BIT0);

	ApplVar.PbFNumber = 4;
	ApplVar.PrintLayOut = ApplVar.PbF.Print;    /* save previous print */
	ReadPbF();           /* read function */

	autochk = 0;
    ApplVar.Qty = ZERO;
	
	if (ApplVar.TaxPerm)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        return;
    }

	ApplVar.NumberEntry = ApplVar.PbNumber;
	memset(&ApplVar.Entry, 0, sizeof(ApplVar.Entry));
	WORDtoBCD(ApplVar.Entry.Value, ApplVar.PbNumber);

	PbTotal(ApplVar.NumberEntry, 0);   /* read pb total */

	if (!ApplVar.AP.StartAddress[AddrTrack])    /* no track buffer */
    {
     	ApplVar.ErrorNumber=ERROR_ID(CWXXI45);
                return;
    }
    if (!ApplVar.PB.Block && !ApplVar.FProforma)  /* pb open ? */
    {
     	/* if proforma and print bill then also when not open */
      	ApplVar.ErrorNumber=ERROR_ID(CWXXI11);
       	return;
  	}

	if (ApplVar.PB.Lines && (TESTBIT(PBPRINT,BIT0)))	/* already printed ? */
	{
		ApplVar.ErrorNumber=ERROR_ID(CWXXI28);
		return;
	}

	ApplVar.PrintLayOut = ApplVar.PbF.Print;
	if (ApplVar.PB.Lines && TESTBIT(PBPRINT,BIT1))	 /*already printed then insert bill */
		SETBIT(ApplVar.PrintLayOut,BIT4);	  /* set slip compulsory */
	if (TESTBIT(CLERKFIX, BIT1) && ApplVar.PB.Block && ApplVar.PB.Clerk != ApplVar.ClerkNumber)
	{
		ApplVar.ErrorNumber=ERROR_ID(CWXXI25);
		return;
	}

	if (TESTBIT(ApplVar.PbF.Options, BIT4)) 	/* Invoice# PBtrail ? */
	{
		if (StorePbInvoice(1))
			return; 	/* buffer full */
		SETBIT(ApplVar.FNoPb, BIT2);
	}

	ApplVar.FPb = 1;		/* indicate ApplVar.PB incase no receipt# increment */
	if (!TESTBIT(PBPRINT, BIT4) && !((ApplVar.PB.Block)))// ccr 040727
		SETBIT(ApplVar.MyFlags, PRNONPB);// 开台打印
	if (RegiInit())
	{
		ApplVar.FPb = 0;
		RESETBIT(ApplVar.FNoPb, BIT2);
		RESETBIT(ApplVar.MyFlags, PRNONPB);// ccr 040727
		return;
	}
	ApplVar.FPb = 0;

				if (TESTBIT(PBINFO, BIT1))		/* display amount ? */
				{
					ApplVar.Amt = ApplVar.PB.Amt;
					AmtRound(0, &ApplVar.Amt);				/* total sales rounding */
#if defined(LCD10_DISP)
					LCD10_SetFlag(FLAG_TOTAL);
					PutsO(DispAmtStr(0, &ApplVar.Amt,DISLEN));//C-DISPLAY
#else
					PutsO(DispAmtStr(Msg[TABLENUM].str, &ApplVar.Amt,DISLEN));//C-DISPLAY
#endif
#if (SCRSTATDBL)
					DispOnStatDBL(DispAmtStr(Msg[GUAZHE].str, &ApplVar.PB.Amt,SCRDBLWD),0);
#else
					DispSt2(DispAmtStr(Msg[GUAZHE].str, &ApplVar.PB.Amt,INPUTWIDTH));//RG-DISPLAY
#endif
				}
				else
				{
#if defined(LCD10_DISP)
					LCD10_SetFlag(FLAG_NULL);
					PutsO(DispAmtStr(0, &ApplVar.Entry,DISLEN));//C-DISPLAY
#else
					PutsO(DispQtyStr(Msg[TABLENUM].str, &ApplVar.Entry,DISLEN));//C-DISPLAY
#endif
#if (SCRSTATDBL)
					DispOnStatDBL(DispAmtStr(Msg[GUAZHE].str, &ApplVar.Entry,SCRDBLWD),0);
#else
					DispSt2(DispAmtStr(Msg[GUAZHE].str, &ApplVar.Entry,INPUTWIDTH));//RG-DISPLAY
#endif
				}

				if (TESTBIT(KPPRICE, BIT4) && TESTBIT(ApplVar.PrintLayOut, BIT1)) /* print or split function ? */
					PrintHead1(1);
				
				ApplVar.PbNumber = ApplVar.NumberEntry; 			//20040325

					PrintPbHeader();
				
				if (ApplVar.FCanc == 1)
					PrintMessage(QUXIAOP);
				StoreInBuffer();
				ApplVar.Amt = ZERO;

						if (ApplVar.PB.Block)	/* print ? */
						{
							/* only print total with 4 (not SI) when bill available */
							SPFG = ApplVar.PrintLayOut;
							RESETBIT(ApplVar.PrintLayOut, BIT3);	  //20040326
							if (ApplVar.PB.Covers)
							{
								WORDtoBCD(ApplVar.Amt.Value, ApplVar.PB.Covers);
								PrintQty(Msg[RENSHU].str, &ApplVar.Amt);
								PrintLine('=');
							}

#if !defined(DEBUGBYPC)//  打印票头图象  //
#if !defined(CASE_INNERDOTPRN)
								if ((ApplVar.Graph[0].PictNo>0 && ApplVar.Graph[0].PictNo<=GRAPHMAX) && !TESTBIT(ApplVar.MyFlags, PRNONPB+ENPRINTER))
								{
									Bios(BiosCmd_PrintGraph, (void*)(ApplVar.Graph[0].PictNo), 1 , 0); //Stampa msg ram
				//20130709分割线没有多大的意义，客户反馈也浪费纸 				   PrintLine('=');
								}
#endif
#endif
#if defined(CASE_PERU)//ccr2016-10-24>>>>>>>>>>>>>>>
								PrintCustomerMessage();
#endif//ccr2016-10-24<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
								RefreshEJStart(true);
#endif
								PrintStr(Msg[TITLE].str);
				//20130709分割线没有多大的意义，客户反馈也浪费纸 			   PrintLine('=');

							ProcessPbBuffer(0); 	/* print buffer */
							ApplVar.PrintLayOut &= 0xdf;	/* reset two line */
							ApplVar.FSale = 1;
							RegiEnd();
							ApplVar.FSale = 0;
							if (!TESTBIT(PBINFO, BIT3)) /* only without check-out system */
							{
								SETBIT(ApplVar.PrintLayOut, BIT2);	/* double size */
								ApplVar.SlipDouble = 1;
							}
							AmtRound(0, &ApplVar.SubTotal); 		/* total sales rounding */

							RESETBIT(ApplVar.PrintLayOut, BIT2);
							if (PVAT)		/* print VAT ? */
								CalculateTax(3);


								MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);
								ApplVar.SaleAmt = ZERO;

							ApplVar.PrintLayOut = SPFG;  //20040326
						}

						ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
						ApplVar.RGRec.Amt = ApplVar.Entry;
						*((WORD *) ApplVar.RGRec.Qty.Value) = ApplVar.PbNumber;

						if (autochk)
						{
							ApplVar.AutoCheck++;
						}

						if (ApplVar.ErrorNumber)
						{
							if (ApplVar.RGRec.Key.Code == ApplVar.Key.Code)
								ApplVar.RGRec.Key.Code = 0;
						}

}

void PbFunction()
{
    BYTE type;
    BYTE autochk;
    BYTE ENP = 1,SPFG;

	SETBIT(ApplVar.PrintFlag, BIT0);//jdb2018-10-23不打印

    ApplVar.PbFNumber = ApplVar.Key.Code - PBF - 1;
    ApplVar.PrintLayOut = ApplVar.PbF.Print;    /* save previous print */
    if (ApplVar.PbFNumber < ApplVar.AP.Pb.Number)
        ReadPbF();           /* read function */
    else
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
        return;
    }
    autochk = 0;
    ApplVar.Qty = ZERO;
    type = ApplVar.PbF.Options & 0x0f;
    if ((ApplVar.FCorr || ApplVar.FRefund) && type != 10)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        return;
    }
	
	if(type == 4){//jdb2018-10-23 PRINT PB
		RESETBIT(ApplVar.PrintFlag, BIT0);
    }
	
    if (type < 2 && !ApplVar.FPb && !ApplVar.FRegi)
    {
        if (!Appl_EntryCounter)
        {
            if (TESTBIT(PBPRINT, BIT7) && (ApplVar.AP.Pb.Random & 0x0f))    /* Auto Check Number ? */
            {
                //if (!ApplVar.AutoCheck.l && ApplVar.AutoCheck.h)
                if (!(ApplVar.AutoCheck & 0xffff) && (ApplVar.AutoCheck & 0xffff0000))
                    ApplVar.AutoCheck = 1 ;
                ApplVar.Entry=ZERO;
                ULongToBCDValue(ApplVar.Entry.Value, ApplVar.AutoCheck);
                Appl_EntryCounter = 5;
                autochk = 1;
            }
            else if (TESTBIT(ApplVar.PbF.Options, BIT5))
            {
                if (!TESTBIT(ApplVar.FNoPb, BIT0) && TESTBIT(KPPRICE, BIT4))
                    PrintHead1(1);
                SETBIT(ApplVar.FNoPb, BIT0);  /* set disable ApplVar.PB compulsion for 1 transaction */
                return;
            }
        }
    }
    if (!type && !Appl_EntryCounter)  /* OPEN without entry then service */
        type = 2;
    else if (ApplVar.TaxPerm)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        return;
    }

#if (BARCUSTOMER==1)
    if (type!=3 && type!=5 && ApplVar.BarCustomer.OFFIndex)
    {
        if (ApplVar.BarCustomer.DeptHi)
            ApplVar.PluPriceLevel = 0;
        ApplVar.BarCustomer.OFFIndex = 0;// 标识没有会员登录  //
    }
#endif

    switch (type)
    {
    case 0:     /* open ApplVar.PB and feed if already open */// 开台挂帐  PB-F 1  //
    case 1:     // 开台增加 PB-F 2 //
    case 4:     /* Print Pb */// 打印开台PB-F 5 //
    case 5:     /* print pb and release */// 打印帐单 PB-F 6 //
    case 7:     /* split bill */// 分帐 PB-F 8 //
    	if(Appl_EntryCounter && type == 4)//jdb2018-10-23PRINT PB打印不需要输入数字
		{
			ApplVar.ErrorNumber=ERROR_ID(CWXXI10);
            	break;
		}

		if (!Appl_EntryCounter)
        {
        	if(type == 4)
//jdb2018-10-23PRINT PB打印
        	{
				ApplVar.NumberEntry = ApplVar.PbNumber;
				memset(&ApplVar.Entry, 0, sizeof(ApplVar.Entry));
				WORDtoBCD(ApplVar.Entry.Value, ApplVar.PbNumber);
        	}else{
            	ApplVar.ErrorNumber=ERROR_ID(CWXXI10);
            	break;
        	}
        }
        else if (ApplVar.FPb)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI08);
            break;
        }
        else if (ApplVar.FRegi)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
            break;
        }
        if (!GetPbNumber())
            break;

        PbTotal(ApplVar.NumberEntry, 0);   /* read pb total */
        if (type > 1)      /* print or split ? */
        {
            if (!ApplVar.AP.StartAddress[AddrTrack])    /* no track buffer */
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI45);
                return;
            }
            if (!ApplVar.PB.Block && !ApplVar.FProforma)  /* pb open ? */
            {
                /* if proforma and print bill then also when not open */
                ApplVar.ErrorNumber=ERROR_ID(CWXXI11);
                return;
            }
            if (type != 7 && ApplVar.PB.Lines &&
                (TESTBIT(PBPRINT,BIT0) && ApplVar.CentralLock != MG))   /* already printed ? */
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI28);
                return;
            }
//			SPFG = ApplVar.PB.Lines;	//20040326
//			ApplVar.PB.Lines = 0;   /* always print */
        }
        ApplVar.PrintLayOut = ApplVar.PbF.Print;
        if (ApplVar.PB.Lines && TESTBIT(PBPRINT,BIT1))   /*already printed then insert bill */
            SETBIT(ApplVar.PrintLayOut,BIT4);     /* set slip compulsory */
        if (TESTBIT(CLERKFIX, BIT1) && ApplVar.PB.Block && ApplVar.PB.Clerk != ApplVar.ClerkNumber)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI25);
            return;
        }
        if (type == 4 || type == 7)   /* print not SI or split打印开台,分帐 */
        {
            if (TESTBIT(ApplVar.PbF.Options, BIT4))     /* Invoice# PBtrail ? */
            {
                if (StorePbInvoice(1))
                    return;     /* buffer full */
                SETBIT(ApplVar.FNoPb, BIT2);
            }
        }
        ApplVar.FPb = 1;        /* indicate ApplVar.PB incase no receipt# increment */
        if (!TESTBIT(PBPRINT, BIT4) && !((type == 4  && ApplVar.PB.Block) || type == 5 || type == 7))// ccr 040727
            SETBIT(ApplVar.MyFlags, PRNONPB);// 开台打印
//		if ((type == 4  && ApplVar.PB.Block) || type == 5 || type == 7)  /* lyq added 20040224 */
//				RESETBIT(ApplVar.MyFlags, PRNONPB);
        if (RegiInit())
        {
            ApplVar.FPb = 0;
            RESETBIT(ApplVar.FNoPb, BIT2);
            RESETBIT(ApplVar.MyFlags, PRNONPB);// ccr 040727
            return;
        }
        ApplVar.FPb = 0;
        if (type != 5)
        {
            if (TESTBIT(PBINFO, BIT1))      /* display amount ? */
            {
                ApplVar.Amt = ApplVar.PB.Amt;
                AmtRound(0, &ApplVar.Amt);              /* total sales rounding */
#if defined(LCD10_DISP)
                LCD10_SetFlag(FLAG_TOTAL);
                PutsO(DispAmtStr(0, &ApplVar.Amt,DISLEN));//C-DISPLAY
#else
                PutsO(DispAmtStr(Msg[TABLENUM].str, &ApplVar.Amt,DISLEN));//C-DISPLAY
#endif
#if (SCRSTATDBL)
                DispOnStatDBL(DispAmtStr(Msg[GUAZHE].str, &ApplVar.PB.Amt,SCRDBLWD),0);
#else
                DispSt2(DispAmtStr(Msg[GUAZHE].str, &ApplVar.PB.Amt,INPUTWIDTH));//RG-DISPLAY
#endif
            }
            else
            {
#if defined(LCD10_DISP)
                LCD10_SetFlag(FLAG_NULL);
                PutsO(DispAmtStr(0, &ApplVar.Entry,DISLEN));//C-DISPLAY
#else
                PutsO(DispQtyStr(Msg[TABLENUM].str, &ApplVar.Entry,DISLEN));//C-DISPLAY
#endif
#if (SCRSTATDBL)
                DispOnStatDBL(DispAmtStr(Msg[GUAZHE].str, &ApplVar.Entry,SCRDBLWD),0);
#else
                DispSt2(DispAmtStr(Msg[GUAZHE].str, &ApplVar.Entry,INPUTWIDTH));//RG-DISPLAY
#endif
            }
            if (type==0)
                Collect_Data(PBOPENLOG);
        }
        else
            RESETBIT(ARROWS, BIT1);

        if (TESTBIT(KPPRICE, BIT4)
            && type > 1
            && TESTBIT(ApplVar.PrintLayOut, BIT1)) /* print or split function ? */
            PrintHead1(1);

        ApplVar.PbNumber = ApplVar.NumberEntry;             //20040325

        if (type < 2 && ApplVar.FNFolio)                /* new folio key used ? */
        {
            ApplVar.PB.Lines = 0;
            PrintPbHeader();
            if (ApplVar.PB.Block)
            {
                PrintAmt(Msg[GUAZHE].str, &ApplVar.PB.Amt);     /* balance */
                DispStrLF(DispAmtStr(Msg[GUAZHE].str, &ApplVar.PB.Amt,SCREENWD));
            }
        }
        else
            PrintPbHeader();
//		if(type >1)
//			ApplVar.PB.Lines = SPFG;	//20040326
        if (ApplVar.FCanc == 1)
            PrintMessage(QUXIAOP);
        StoreInBuffer();
//	    ApplVar.PbNumber = ApplVar.NumberEntry;
        ApplVar.Amt = ZERO;
        if (type == 4 || type == 5 || type == 7)  /* print or split */
        {
            if (TESTBIT(ApplVar.PbF.Options, BIT7))
                ApplVar.FNoTime = 1;    /* set don't print time */
        }
        if ((type == 4  && ApplVar.PB.Block) || type == 5)  /* print ? */
        {
            /* only print total with 4 (not SI) when bill available */
            SPFG = ApplVar.PrintLayOut;
            RESETBIT(ApplVar.PrintLayOut, BIT3);      //20040326
            if (ApplVar.PB.Covers)
            {
                WORDtoBCD(ApplVar.Amt.Value, ApplVar.PB.Covers);
                PrintQty(Msg[RENSHU].str, &ApplVar.Amt);
                PrintLine('=');
            }
            if (type == 4)
            {
#if !defined(DEBUGBYPC)//  打印票头图象  //
#if !defined(CASE_INNERDOTPRN)
                if ((ApplVar.Graph[0].PictNo>0 && ApplVar.Graph[0].PictNo<=GRAPHMAX) && !TESTBIT(ApplVar.MyFlags, PRNONPB+ENPRINTER))
                {
                    Bios(BiosCmd_PrintGraph, (void*)(ApplVar.Graph[0].PictNo), 1 , 0); //Stampa msg ram
//20130709分割线没有多大的意义，客户反馈也浪费纸                    PrintLine('=');
                }
#endif
#endif
#if defined(CASE_PERU)//ccr2016-10-24>>>>>>>>>>>>>>>
                PrintCustomerMessage();
#endif//ccr2016-10-24<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
                RefreshEJStart(true);
#endif
                PrintStr(Msg[TITLE].str);
//20130709分割线没有多大的意义，客户反馈也浪费纸                PrintLine('=');
            }
            ProcessPbBuffer(0);     /* print buffer */
            ApplVar.PrintLayOut &= 0xdf;    /* reset two line */
            ApplVar.FSale = 1;
            RegiEnd();
            ApplVar.FSale = 0;
            if (!TESTBIT(PBINFO, BIT3)) /* only without check-out system */
            {
                SETBIT(ApplVar.PrintLayOut, BIT2);  /* double size */
                ApplVar.SlipDouble = 1;
            }
            AmtRound(0, &ApplVar.SubTotal);         /* total sales rounding */
            if (type ==5)
                PrintAmt(Msg[YINGFU].str, &ApplVar.SubTotal);//20040402
            RESETBIT(ApplVar.PrintLayOut, BIT2);
            if (PVAT)       /* print VAT ? */
                CalculateTax(3);
            if (type == 5)  /* single item */
            {
                ApplVar.FRegi = 0;
                ReceiptIssue(TESTBIT(ApplVar.PbF.Options, BIT4) ? 1 : 0); /* print time date ? */
//			    if (ApplVar.SlipLines > 1)
//				ApplVar.PB.Lines = ApplVar.SlipLines - 1;	  // delete 20040326
                ApplVar.PB.Clerk = ApplVar.ClerkNumber;
                PbTotal(ApplVar.PbNumber, 1);   /* write line count */

                ApplVar.FPb = 0;
                ApplVar.PbNumber = 0;
                ApplVar.FNoPb = 0;
//				if(!TESTBIT(PBPRINT, BIT4))
//			    	RESETBIT(ApplVar.MyFlags, PRNONPB);
            }
            else
            {
                MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);
                ApplVar.SaleAmt = ZERO;
            }
            ApplVar.PrintLayOut = SPFG;  //20040326
        }
        ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
        ApplVar.RGRec.Amt = ApplVar.Entry;
        *((WORD *) ApplVar.RGRec.Qty.Value) = ApplVar.PbNumber;
        if (type == 7)      /* split ?*/
        {
            ApplVar.FSplit = 1;
            PrintMessage(FKFZHANG);
        }
        if (type < 2 && TESTBIT(ApplVar.PbF.Options, BIT6)) /* NO tender allowed */
        {
            if (!ApplVar.PB.Lines && !ApplVar.SlipLines)    /* bill not printed yet */
                SETBIT(ApplVar.FNoPb, BIT1);
        }
        if (autochk)
        {
            ApplVar.AutoCheck++;
        }
        break;
    case 2:     /* service *///  暂结  PB-F 3 //
        RESETBIT(ApplVar.PrintLayOut, BIT3);//20040326
        SysBuf[0] = ApplVar.PbF.Print;
        ApplVar.PbF.Print = ApplVar.PrintLayOut;        /* print of open function */
        ApplVar.PrintLayOut = SysBuf[0] & ApplVar.PbF.Print;    /* print of service */
        if (Appl_EntryCounter)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI04);
        else if (!ApplVar.FPb)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI10);
        else if (ApplVar.FSplit)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI21);
        else
        {
            CalculateTax(0);        /* add and report */
            RESETBIT(ARROWS, BIT1);
#if defined(LCD10_DISP)
            LCD10_SetFlag(FLAG_TOTAL);
            PutsO(DispAmtStr(0, &ApplVar.SubTotal,DISLEN));//C-DISPLAY
#else
            PutsO(DispAmtStr(Msg[SERVICE].str, &ApplVar.SubTotal,DISLEN));//C-DISPLAY
#endif
//			DispStrLF(DispAmtStr(Msg[XFJE].str, &ApplVar.SubTotal,SCREENWD));
            if (ApplVar.FRecIssue)
            {
                NewReceipt();
                RESETBIT(ApplVar.PrintLayOut, BIT1);
            }
            ApplVar.FRecIssue = 0;
            StoreInBuffer();
            RegiEnd();
            ApplVar.Amt = ApplVar.SubTotal;
            ApplVar.RGRec.Key.Code = 299;    /* subtotal */
            StoreInBuffer();
            SPFG = ApplVar.PrintLayOut;        //
            PrintAmt(ApplVar.PbF.Name, &ApplVar.Amt);
            ApplVar.PrintLayOut = SPFG;       //20040326
            ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
            ApplVar.RGRec.Qty = ZERO;
            ApplVar.RGRec.Amt = ApplVar.Amt;
            StoreInBuffer();
            ApplVar.FRegi = 0;
            PbTotal(ApplVar.PbNumber, 0);   /* read current total */
            ApplVar.PB.Clerk = ApplVar.ClerkNumber;
            Add(&ApplVar.PB.Amt, &ApplVar.Amt);     /* add total */
            Add(&ApplVar.PB.Disc1, &ApplVar.DiscItem[0]);     /* add discount total */
            Add(&ApplVar.PB.Disc2, &ApplVar.DiscItem[1]);     /* add discount total */
            Collect_Data(PBCLOSELOG);
            PrintNewBal();
            PrintSaleQty();
            ApplVar.FNoPb = 0;      /* reset Invoice number */
//			ApplVar.PB.Pages = ApplVar.SlipPage; //20040326
            ReceiptIssue(TESTBIT(ApplVar.PbF.Options, BIT4) ? 1 : 0); /* print time date ? */
            if (ApplVar.SlipLines > 1)      /*  */
                ApplVar.PB.Lines = ApplVar.SlipLines - 1;
            else
                ApplVar.PB.Lines = 0;
            if (!ApplVar.AP.StartAddress[AddrTrack] && ApplVar.FSale)   /* no track buffer and sale */
//			if (ApplVar.FSale)   /* no track buffer and sale */
                ApplVar.PB.Block = 1;       /* then indicate used */
            PbTotal(ApplVar.PbNumber, 1);   /* write new total */
            AddPbFTotal();
            ApplVar.FSale = 0;
            ApplVar.BufCC = 1;                 /* set add Customer Count */
            ApplVar.BufKp = 1;
            ProcessBuffer();    /* reset Customer count and print kp*/

            if (TESTBIT(PBPRINT,BIT6))//ccr040810
            {
                ApplVar.BufCC = 1;                 /* set add Customer Count */
                ApplVar.BufKp = 1;
                ProcessBuffer();    /* reset Customer count and print kp*/
            }

            ApplVar.FPb = 0;
            ApplVar.PbNumber = 0;
        }
        RESETBIT(ApplVar.MyFlags, PRNONPB);  // lyq added 20040224
        SETBIT(ApplVar.MyFlags, CLSSCREEN);        //|__>lyq20040115 added for control the printing pb message
        break;
    case 3:     /* check out */// 确认结帐  PB-F 4 //
        if (!TESTBIT(ApplVar.MyFlags, PRNONPB))
            ENP = 1;
        else
        {
            ENP = 0;
            RESETBIT(ApplVar.MyFlags, PRNONPB);   //  |
        }                                 //  |___>// lyq added 20040224
    case 6:// 取消确认 PB-F 7 //
        if (!TESTBIT(PBINFO,BIT3))
            ApplVar.ErrorNumber=ERROR_ID(CWXXI42);        /* not allowed with new system */
        else if (!ApplVar.PbNumber)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI10);
        else if (Appl_EntryCounter)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI04);
        else if (type == 3 && !ApplVar.FPb)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI19);
        else if (type == 6 && ApplVar.FPb)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI22);
        else
        {
            ApplVar.PrintLayOut &= ApplVar.PbF.Print;   /* and open ApplVar.PB print */
            PbTotal(ApplVar.PbNumber, 0);       /* read total */
            if (ApplVar.FSplit)
            {
                RegiEnd();
                ApplVar.FSale = 0;
                ApplVar.Amt = ApplVar.SubTotal;
                Subtract(&ApplVar.PB.Amt, &ApplVar.Amt);
                Subtract(&ApplVar.PB.Disc1, &ApplVar.DiscItem[0]);
                Subtract(&ApplVar.PB.Disc2, &ApplVar.DiscItem[1]);
                PbTotal(ApplVar.PbNumber, 1);   /* write new total */
                ApplVar.FSplit = 0;
                ApplVar.PbNumber = 0;
                AmtRound(0, &ApplVar.Amt);              /* Sales Amount rounding */
                PrintAmt(Msg[YINGFU].str, &ApplVar.Amt);
                ApplVar.SaleAmt = ApplVar.Amt;
                if (PVAT)
                {
                    CalculateTax(3);        /* print VAT */
                    ApplVar.Amt = ApplVar.SaleAmt;          /* restore amount */
                }
                MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);
                ApplVar.RGItems.RGNumber = 2;
            }
            else
            {
                StoreInBuffer();
                ApplVar.Amt = ApplVar.PB.Amt;
                AmtRound(0, &ApplVar.Amt);              /* Sales Amount rounding */
/* JB Modify */
/*                    ApplVar.DiscItem[0] = ApplVar.PB.Disc1;
            ApplVar.DiscItem[1] = ApplVar.PB.Disc2;      */
                if (type == 6)   /* cancel pay */
                    ApplVar.Amt.Sign ^= 0x80;
                Add(&ApplVar.SaleAmt, &ApplVar.Amt);
            }
            if (type == 3)
            {
#if defined(LCD10_DISP)
                LCD10_SetFlag(FLAG_TOTAL);
                PutsO(DispAmtStr(0, &ApplVar.Amt,DISLEN));//C-DISPLAY
#else
                PutsO(DispAmtStr(Msg[CHECKPAID].str, &ApplVar.Amt,DISLEN));//C-DISPLAY
#endif
//			    DispStrLF(DispAmtStr(Msg[QRJZHANG].str, &ApplVar.Amt,DISLEN));
                ApplVar.FPb = 0;
            }
            else
            {
#if defined(LCD10_DISP)
                LCD10_SetFlag(FLAG_TOTAL);
                PutsO(DispAmtStr(0, &ApplVar.Amt,DISLEN));//C-DISPLAY
#else
                PutsO(DispAmtStr(Msg[CHECKCPAID].str, &ApplVar.Amt,DISLEN));//C-DISPLAY
#endif
//			    DispStrLF(DispAmtStr(Msg[QXQREN].str, &ApplVar.Amt,DISLEN));
                ApplVar.FPb = 1;
            }
            if (type ==3 && ENP == 0)     //lyq added 20040224 ,added enp varialbe
            {
                SETBIT(ApplVar.PrintLayOut, BIT2);
                ApplVar.Entry = ZERO;
                ULongToBCDValue(ApplVar.Entry.Value, ApplVar.PbNumber);
                PrintQty(Msg[ZUOHAO].str, &ApplVar.Entry);
                RESETBIT(ApplVar.PrintLayOut, BIT2);
                PrintLine('=');
            }
            ApplVar.RGRec.Amt = ApplVar.Amt;
            ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
            SPFG = ApplVar.PrintLayOut;        //
            RESETBIT(ApplVar.PrintLayOut, BIT3);//20040326
            if  (CheckNotZero(&ApplVar.Amt))   //ccr2016-07-20为0时不打印"确认结账"/FINALIZE
                PrintAmt(ApplVar.PbF.Name, &ApplVar.Amt);
            ApplVar.PrintLayOut = SPFG;       //20040326
            if (type == 6)                //lyq added 2003\11\1
                ApplVar.Amt.Sign ^= 0x80;
            AddPbFTotal();
            RESETBIT(ApplVar.FNoPb, BIT0+BIT1);      /* reset no tender */
        }
        break;
    case 8:     /* transfer table *///  转帐 PB-F 9 //
        if (!ApplVar.FPb)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI10);
            break;
        }
        else if (ApplVar.FSplit)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI29);
            break;
        }
        else if (!Appl_EntryCounter || ApplVar.FProforma)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            break;
        }
        else if (!GetPbNumber())
            break;
        if (ApplVar.NumberEntry == ApplVar.PbNumber)    /* transfer to same table */
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            break;
        }
        PbTotal(ApplVar.NumberEntry, 0);   /* read new pb total */
        if (ApplVar.PB.Block)  /* pb  open ? */
        {
            if (TESTBIT(ApplVar.PbF.Options, BIT4)) /* open allowed ? */
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI08);   /* already open */
                break;
            }
            if (TESTBIT(ApplVar.PbF.Options, BIT5) && ApplVar.ClerkNumber != ApplVar.PB.Clerk) /* other clerk allowed ? */
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI25);   /* wrong clerk */
                break;
            }
            if (ApplVar.FSale && ApplVar.ClerkNumber != ApplVar.PB.Clerk)
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI14);   /* already in sale */
                break;
            }
            PbTotal(ApplVar.PbNumber, 0);   /* read old */
            ProcessPbBuffer(ApplVar.NumberEntry);   /* transfer records */
            if (ApplVar.ErrorNumber)
                break;
            PbTotal(ApplVar.PbNumber, 0);   /* read old */
            ApplVar.Amt = ApplVar.PB.Amt;
            ApplVar.DiscItem[0] = ApplVar.PB.Disc1;
            ApplVar.DiscItem[1] = ApplVar.PB.Disc2;
            type = ApplVar.PB.Covers;
            PbTotal(ApplVar.NumberEntry, 0);   /* read new */
            ApplVar.PB.Lines = 0;   /* reset printed */
            Add(&ApplVar.PB.Amt, &ApplVar.Amt);
            Add(&ApplVar.PB.Disc1, &ApplVar.DiscItem[0]);
            Add(&ApplVar.PB.Disc2, &ApplVar.DiscItem[1]);
/* JB Modify */
            ApplVar.DiscItem[0] = ZERO;
            ApplVar.DiscItem[1] = ZERO;

            ApplVar.PB.Covers += type;
        }
        else
        {
            PbTotal(ApplVar.PbNumber, 0);   /* read old */
            ApplVar.PB.Clerk = ApplVar.ClerkNumber; /* current clerk */
        }
        Collect_Data(TRTABLELOG);

        ApplVar.PrintLayOut &= ApplVar.PbF.Print;   /* and open ApplVar.PB print */

        if (ApplVar.PB.Clerk != ApplVar.ClerkNumber)
            PrintStr(ApplVar.Clerk.Name);
        PrintMessage(ZHUANRUDAO);
        PrintQty(Msg[ZUOHAO].str, &ApplVar.Entry);
        PbTotal(ApplVar.NumberEntry, 1);    /* write new */
        PbTotal(ApplVar.NumberEntry, 0);   /* read new for ApplVar.PB text */
        if (ApplVar.AP.Pb.Text && ApplVar.PB.Text[0])
            PrintStr(ApplVar.PB.Text);
        MemSet(&ApplVar.PB, sizeof(ApplVar.PB), 0);
        PbTotal(ApplVar.PbNumber, 1);    /* reset old */
        ApplVar.PbNumber = ApplVar.NumberEntry;
        PbTotal(ApplVar.PbNumber, 0);   /* read new */
        StoreInBuffer();
        ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
        ApplVar.RGRec.Amt = ApplVar.Entry;
        ApplVar.RGRec.Qty = ZERO;
        if (ApplVar.PB.Clerk != ApplVar.ClerkNumber)
        {
            ApplVar.Amt.Sign ^= 0x80;       /* invert sign */
            AddPbFTotal();
            ApplVar.NumberEntry = ApplVar.ClerkNumber;      /* save active clerk */
            ApplVar.ClerkNumber = ApplVar.PB.Clerk;
            ReadClerk();
            PrintStr(ApplVar.Clerk.Name);
            ApplVar.Amt.Sign ^= 0x80;       /* invert sign back */
            AddPbFTotal();          /* add in transfer total */
            ApplVar.ClerkNumber = ApplVar.NumberEntry;
            ReadClerk();
            SPFG = ApplVar.PrintLayOut;        //
            RESETBIT(ApplVar.PrintLayOut, BIT3);//20040326
            PrintAmt(ApplVar.PbF.Name, &ApplVar.Amt);
            ApplVar.PrintLayOut = SPFG;       //20040326
            StoreInBuffer();
            ApplVar.FRegi = 0;
            ReceiptIssue(1);
            ApplVar.BufCC = 1;
            ProcessBuffer();
            ApplVar.FPb = 0;
            ApplVar.PbNumber = 0;
        }
        break;
    case 9:     /* transfer clerk when pb open only 1 pb else all */// 转桌帐  PB-F 10 //
        if (!Appl_EntryCounter || Appl_EntryCounter > 3 || ApplVar.FProforma)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            break;
        }
        if (!ApplVar.NumberEntry || ApplVar.NumberEntry > ApplVar.AP.Clerk.Number)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI24);
            break;
        }
        ApplVar.Amt = ZERO;
        if (ApplVar.FPb)
        {
            if (ApplVar.FSplit)
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI29);
                break;
            }
            if (ApplVar.FSale)  /* already in transaction */
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
                break;
            }
            ApplVar.PrintLayOut &= ApplVar.PbF.Print;
            PbTotal(ApplVar.PbNumber, 0);   /* read new */
            ApplVar.PB.Clerk = ApplVar.NumberEntry;
            PbTotal(ApplVar.PbNumber, 1);   /* write back */
            ApplVar.Amt = ApplVar.PB.Amt;
        }
        else
        {
            ApplVar.PrintLayOut = ApplVar.PbF.Print;
            ApplVar.FPb = 1;
            if (RegiStart())
            {
                RESETBIT(ApplVar.MyFlags, PRNONPB);// |
                ApplVar.FPb = 0;
                return;  /* check if start of registration */
            }
            for (ApplVar.PbNumber = 1; ApplVar.PbNumber <= ApplVar.AP.Pb.NumberOfPb; ApplVar.PbNumber++)
            {
                PbTotal(ApplVar.PbNumber, 0);   /* read new */
                if (ApplVar.PB.Clerk == ApplVar.ClerkNumber)
                {
                    ApplVar.PB.Clerk = ApplVar.NumberEntry;
                    PbTotal(ApplVar.PbNumber, 1);   /* write back */
                    Add(&ApplVar.Amt, &ApplVar.PB.Amt);
                    if (ApplVar.AP.Pb.Random & 0x0f)
                        memcpy(ApplVar.Entry.Value, ApplVar.PB.Random, 7);
                    else
                        WORDtoBCD(ApplVar.Entry.Value, ApplVar.PbNumber);
                    PrintQty(Msg[ZUOHAO].str, &ApplVar.Entry);
                    if (ApplVar.AP.Pb.Text && ApplVar.PB.Text[0])
                        PrintStr(ApplVar.PB.Text);
                }
            }
            ApplVar.PB.Clerk = ApplVar.NumberEntry;
        }
        PrintMessage(ZHUANRUDAO);
        ApplVar.Amt.Sign ^= 0x80;       /* invert sign */
        AddPbFTotal();
        ApplVar.NumberEntry = ApplVar.ClerkNumber;      /* save active clerk */
        ApplVar.ClerkNumber = ApplVar.PB.Clerk;
        ReadClerk();
        PrintStr(ApplVar.Clerk.Name);
        ApplVar.Amt.Sign ^= 0x80;       /* invert sign back */
        AddPbFTotal();          /* add in transfer total */
        ApplVar.ClerkNumber = ApplVar.NumberEntry;
        ReadClerk();
        SPFG = ApplVar.PrintLayOut;        //
        RESETBIT(ApplVar.PrintLayOut, BIT3);//20040326
        PrintAmt(ApplVar.PbF.Name, &ApplVar.Amt);
        ApplVar.PrintLayOut = SPFG;       //20040326
        ApplVar.FRegi = 0;
        ReceiptIssue(1);
        ApplVar.BufCC = 1;
        AddPbFTotal();
        ApplVar.NumberEntry = ApplVar.ClerkNumber;      /* save active clerk */
        ApplVar.ClerkNumber = ApplVar.PB.Clerk;
        AddPbFTotal();          /* add in transfer total */
/* JB Modify */
        ReadClerk();
/*            ApplVar.ClerkNumber = ApplVar.NumberEntry; */

        ApplVar.FPb = 0;
        ApplVar.BufCC = 0;
        ApplVar.PbNumber = 0;
        ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
        break;
    case 10:        /* covers *///  人数 PB-F 11 //
        if (!ApplVar.FPb)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI10);
            break;
        }
        if (!Appl_EntryCounter)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            break;
        }
        if (ApplVar.FSplit || ApplVar.FCanc || ApplVar.FCorr || ApplVar.FRefund)
            ApplVar.Entry.Sign = 0x80;  /* negativ */
        if (Appl_EntryCounter > 3 || ApplVar.NumberEntry > 100)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI18);
            break;
        }
        PbTotal(ApplVar.PbNumber, 0);   /* read pb total */
        if ((ApplVar.PB.Covers + ApplVar.NumberEntry) > 255)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI20);
            break;
        }
        if (ApplVar.Entry.Sign & 0x80)
        {
            if (ApplVar.NumberEntry > ApplVar.PB.Covers)
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                break;
            }
            ApplVar.PB.Covers -= ApplVar.NumberEntry;
        }
        else
            ApplVar.PB.Covers += ApplVar.NumberEntry;       /* add covers */
        ApplVar.PrintLayOut &= ApplVar.PbF.Print;   /* and open ApplVar.PB print */
#if defined(LCD10_DISP)
        LCD10_SetFlag(FLAG_NULL);
        PutsO(DispQtyStr(0, &ApplVar.Entry,DISLEN));//C-DISPLAY
#else
        PutsO(DispQtyStr(Msg[COVERS].str, &ApplVar.Entry,DISLEN));//C-DISPLAY
#endif
//ccr2016-09-02        DispStrLF(DispQtyStr(Msg[RENSHU].str, &ApplVar.Entry,DISLEN));
        PrintQty(Msg[RENSHU].str, &ApplVar.Entry);
        PbTotal(ApplVar.PbNumber, 1);   /* write new total */
        StoreInBuffer();
        ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
        ApplVar.RGRec.Amt = ApplVar.Entry;
        ApplVar.RGRec.Qty = ZERO;
        ApplVar.Amt = ApplVar.Entry;            /* add in report */
        AddPbFTotal();
        ApplVar.FRefund = 0;
        ApplVar.FCorr = 0;
        break;
    default:
        ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
        break;
    }
    if (ApplVar.ErrorNumber)
    {
        if (ApplVar.RGRec.Key.Code == ApplVar.Key.Code)
            ApplVar.RGRec.Key.Code = 0;
    }
}

//ccr2016-01-13>>>>>>>>>>>>>>>>>>>>>
void ReadPBTable()
{
    PbTotal(ApplVar.PbNumber, 0);
}
