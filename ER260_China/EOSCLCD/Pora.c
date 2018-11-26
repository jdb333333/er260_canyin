#define PORA_C 7
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "CHNMess.h"

/********************************************************************/

void GetPoRaOffSet()
{
	RamOffSet = ApplVar.PoRaNumber * ApplVar.AP.PoRa.RecordSize + ApplVar.AP.StartAddress[AddrPoRa];
}

void AddPoRaTotal()
{
    GetPoRaOffSet();
   	RamOffSet += ApplVar.AP.PoRa.TotalOffSet;
    for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
    {
		ApplVar.Size = ApplVar.AP.PoRa.Size[ApplVar.PointerType];
		AddPointerTotal();
    }
}

void WritePoRa()
{
    if (ApplVar.PoRaNumber < ApplVar.AP.PoRa.Number)
    {
		GetPoRaOffSet();

		WriteRam((BYTE*)&ApplVar.PoRa, ApplVar.AP.PoRa.TotalOffSet);	   /* write function options */
    }
}

void ReadPoRa()
{
    GetPoRaOffSet();
    ReadRam((BYTE*)&ApplVar.PoRa, ApplVar.AP.PoRa.TotalOffSet);      /* write function options */
}


void PaidOutRa()
{
#if defined(CASE_INDIA)
    //BCD sSave;// = ApplVar.Entry;
    //WORD sKeyCode;// = ApplVar.Key.Code;
    //BYTE sEntryCou;// = Appl_EntryCounter;
#endif

    ApplVar.FCurr = 0;
    if (!Appl_EntryCounter)
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI43);
		return;
    }
    if (AmtInputMask())
		return;

#if defined(CASE_INDIA)//ccr2016-06-14>>>>>>>
    if (ApplVar.Key.Code == PORA)
    {
        if (!Appl_EntryCounter || ApplVar.NumberEntry>ApplVar.AP.PoRa.Number)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return;
        }
        ApplVar.PoRaNumber = ApplVar.NumberEntry-1;
        if (!ApplVar.FPrice)	    /* amount entered? */
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI43);
            return;
        }
        ApplVar.FPrice=0;
        ApplVar.Entry=ApplVar.Price;
    }
    else
#endif//ccr2016-06-14<<<<<<<
        ApplVar.PoRaNumber = ApplVar.Key.Code - PORA - 1;
    if (ApplVar.PoRaNumber < ApplVar.AP.PoRa.Number)
        ReadPoRa();	      /* read function */
    else
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
        return;
    }
    if (ApplVar.FPb)
	ApplVar.PoRa.Print = ApplVar.PbF.Print;
    if (TESTBIT(ApplVar.PoRa.Options, BIT2))
    {
		if (!ApplVar.FPb)  /* pb related*/
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI10);	/* pb required */
		    return;
		}
    }
    else if (!TESTBIT(ApplVar.PoRa.Options, BIT1))  /* single item */
    {
		if (ApplVar.FRegi)  /* single item */
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI14);	    /* not in registration */
		    return;
		}
    }
    else if (ApplVar.FPb)
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI07);
		return;
    }
    ApplVar.Amt = ApplVar.Entry;
	if (ApplVar.MultiplyCount)
	{
		if (ApplVar.FBarcode!=2 && ApplVar.FBarcode!=3)
		{
		    Multiply(&ApplVar.Amt, &ApplVar.Qty);
			RoundBcd(&ApplVar.Amt, 0);
		}
		ApplVar.MultiplyCount = 0;
	}
    if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund)
		ApplVar.Amt.Sign ^= 0x80;
    if (ApplVar.FCanc)
		ApplVar.Amt.Sign = 0x80;
    if (!TESTBIT(ApplVar.PoRa.Options, BIT3))
		ApplVar.Amt.Sign ^= 0x80;
    ApplVar.PrintLayOut = ApplVar.PoRa.Print;
    if (RegiStart())
		return;  /* check if start of registration */
    if (!TESTBIT(ApplVar.PoRa.Options, BIT1) && !ApplVar.FPb)  /* single item */
    {
		RESETBIT(ARROWS, BIT1);
		ApplVar.FRegi = 0;
    }
    StoreInBuffer();
#if (0)//ccr2016-03-31
    if (!TESTBIT(ApplVar.PoRa.Options, BIT3))
		DispStrLF(DispAmtStr(Msg[PUTOUT].str, &ApplVar.Amt,SCREENWD))//ccr2016-03-01
    else
		DispStrLF(DispAmtStr(Msg[RECIEVIN].str, &ApplVar.Amt,SCREENWD))//ccr2016-03-01
#else
    DispStrLF(DispAmtStr(ApplVar.PoRa.Name, &ApplVar.Amt,SCREENWD));//ccr2016-03-31
#endif
    ApplVar.RGRec.Key.Code = PORA + 1 + ApplVar.PoRaNumber;
    ApplVar.RGRec.Qty = ONE;
    ApplVar.RGRec.Amt = ApplVar.Amt;
    if (ApplVar.FPb)
		ApplVar.RGRec.Amt.Sign ^= 0x80;
    ApplVar.Qty = ONE;
    if (ApplVar.FCorr)
    {
		ApplVar.BufCmd = 2;	    /* only check if used */
		StoreInBuffer();    /* check if discount used */
		if (ApplVar.ErrorNumber)
		    return;
    }
    if (!TESTBIT(ApplVar.PoRa.Options, BIT1) || ApplVar.FPb)  /* single item or pb */
    {
		if (StorePbBuffer())	 /* store in ApplVar.PB buffer */
		{
		    ApplVar.RGRec.Key.Code = 0;
		    return;
		}
		if (ApplVar.PoRa.Drawer)
		{
		    ApplVar.DrawNumber = ApplVar.PoRa.Drawer - 1;
		    AddDrawerTotal();
		}
    }
    if (ApplVar.FCorr || ApplVar.FRefund)
    {
		ApplVar.RGRec.Key.Code = CORREC + 1 + ApplVar.CorrecNumber;
		StoreInBuffer();      /* store correction or refund function */
		ApplVar.RGRec.Key.Code = ApplVar.Key.Code;
    }
    if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund)
    {
		ApplVar.Qty.Sign ^= 0x80;
		AddCorrecTotal();
    }
    AddPoRaTotal();

#if defined(CASE_INDIA)//ccr2016-05-31>>>>>>>>>>>>>
    if (TESTBIT(ApplVar.PoRa.Options,BIT4))//是否需要输入号码
    {
        if (!ApplVar.FNum)
        {
                //需要输入号码,存入ApplVar.NumberEntered中
                //sSave = ApplVar.Entry;sKeyCode = ApplVar.Key.Code;sEntryCou = Appl_EntryCounter;
                mClearScreen();
                DispStrXY(Msg[SHRHMA].str,0,0);
                if  (GetStrFrKBD('9',SCREENWD-12,1,12,0)>0)
                {
                    StrToBCDValue(ApplVar.NumberEntered.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
                    Collect_Data(NOADDLOG);
                    ApplVar.FNum=1;
                }
                //Appl_EntryCounter=sEntryCou;ApplVar.Entry=sSave;ApplVar.Key.Code=sKeyCode;
                GotoXY(0,2);
                DispStrLF(DispAmtStr(ApplVar.PoRa.Name, &ApplVar.RGRec.Amt,SCREENWD));//ccr2016-03-31
        }
        if (ApplVar.FNum = 1)
            PrintNumber(&ApplVar.NumberEntered);
        ApplVar.FNum = 0;
    }
#endif//ccr2016-05-31<<<<<<<<<<<<<<<<<

	Collect_Data(PORALOG);

	if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund) /* void */
    {
		ApplVar.PrintLayOut = ApplVar.Correc.Print;
		PrintStr(ApplVar.Correc.Name);
		DispStrLF(ApplVar.Correc.Name);
		ApplVar.PrintLayOut = ApplVar.PoRa.Print;
	}

#if defined(TEND_PREFIX)//ccr2016-08-08
    Prefix1 = PREFIX_1;
    Prefix2 = PREFIX_2;
#endif

	PrintAmt(ApplVar.PoRa.Name, &ApplVar.RGRec.Amt);
    if (TESTBIT(ApplVar.PoRa.Options, BIT0))
		OpenDrawer();
    Add(&ApplVar.SaleAmt, &ApplVar.RGRec.Amt);

    if (!TESTBIT(ApplVar.PoRa.Options, BIT1) && !ApplVar.FPb)  /* single item */
    {
		StoreInBuffer();
		ReceiptIssue(1);
		ApplVar.BufCC = 1;		   /* set add Customer Count */
		ProcessBuffer();
    }
    else
		ApplVar.FSale = 1;
    ApplVar.FVoid = 0;
    ApplVar.FCorr = 0;
    ApplVar.FRefund = 0;
    ApplVar.RepeatCount = 0;
}

