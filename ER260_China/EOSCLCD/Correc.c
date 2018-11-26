#define CORREC_C 7
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#if (PC_EMUKEY)
#include "FEcrTest.h"
#endif

#define CANCELF 0

extern char *ClearFlashChar(char *str,short w);

/****************************************************************/


void GetCorrecOffSet()
{
	RamOffSet = ApplVar.CorrecNumber * ApplVar.AP.Correc.RecordSize + ApplVar.AP.StartAddress[AddrCorr];
}

void AddCorrecTotal()
{
    GetCorrecOffSet();
	RamOffSet += ApplVar.AP.Correc.TotalOffSet;
    for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
    {
		ApplVar.Size = ApplVar.AP.Correc.Size[ApplVar.PointerType];
		AddPointerTotal();
    }
}

void WriteCorrec()
{
    if (ApplVar.CorrecNumber < ApplVar.AP.Correc.Number)
    {
		GetCorrecOffSet();
		WriteRam((BYTE*)&ApplVar.Correc, ApplVar.AP.Correc.TotalOffSet);	 /* write function options */
    }
}

void ReadCorrec()
{
    GetCorrecOffSet();
    ReadRam((BYTE*)&ApplVar.Correc, ApplVar.AP.Correc.TotalOffSet);	    /* write function options */
    ApplVar.Correc.Name[ApplVar.AP.Correc.CapSize]=0 ;
}

void TransactionVoid(WORD corrType)
{
	BYTE savenumber;
	WORD saveclerk;
	short i;

	if (!ApplVar.RGItems.RGNumber || ApplVar.RGItems.RGBuf[ApplVar.RGItems.RGNumber - 1].Key.Code == SUSPEND  || !ApplVar.FRegi || ApplVar.FTend)	/* items in buffer ? */
	{

#if (BARCUSTOMER==1)
		if (ApplVar.BarCustomer.OFFIndex)
			ApplVar.BarCustomer.OFFIndex = 0;// 标识没有会员登录  //
		else
#endif
			ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
		return;
	}


//cc 2006-10-11 for SubDisc>>>>>>>>>>>>>>>>>
	if(TESTBIT(ApplVar.SubDiscFlag,BIT0))
		CalculateSubDisc(1);
//cc 2006-10-11 for SubDisc<<<<<<<<<<<<<<<<<

#if (BARCUSTOMER==1)
	ApplVar.BarCustomer.OFFIndex = 0;// 标识没有会员登录  //
#endif


	if (ApplVar.FTend)//ccr 050111
		ApplVar.BufKp = 0;

	ApplVar.PrintLayOut = ApplVar.Correc.Print;
    if (CheckSlip())		/* slip compulsory */
	    return;
	if (ApplVar.FRegi)
	{//将最后销售的项目存入RGBuf中
		if (ApplVar.FPb)							/* ApplVar.PB transaction ? */
		{
			ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
			return;
		}
		StoreInBuffer();
		ApplVar.PrintLayOut = 0;
		RegiEnd();
		ApplVar.SubTotal = ApplVar.SaleAmt;
	}
	else
		CalculateTax(1);						/* calculate only */

    mClearScreen();
	ApplVar.FTrvoid = 1;
	ApplVar.SubTotal.Sign ^= 0x80;
    RESETBIT(ARROWS, BIT1); /* reset RG led */
	if (corrType==xRECEIPT)
    {
#if defined(LCD10_DISP)
        LCD10_SetFlag(FLAG_TOTAL);
        PutsO(DispAmtStr(0, &ApplVar.SubTotal,DISLEN));//C-DISPLAY
#else
		PutsO(DispAmtStr(Msg[CANCEL].str, &ApplVar.SubTotal,DISLEN));//C-DISPLAY
#endif
    }
	else if (corrType==xSUSPEND)
    {
        //保存挂起的账单数据
        memcpy(&RGSuspend.SuspendItems,&ApplVar.RGItems,
           ApplVar.RGItems.RGNumber*sizeof(struct TRANSRECORD)+sizeof(ApplVar.RGItems.RGNumber));
#if defined(LCD10_DISP)
        LCD10_SetFlag(FLAG_TOTAL);
        PutsO(DispAmtStr(0, &ApplVar.SubTotal,DISLEN));//C-DISPLAY
#else
		PutsO(DispAmtStr(Msg[GUAQI].str, &ApplVar.SubTotal,DISLEN));//C-DISPLAY
#endif
    }

	Collect_Data(TVOIDLOG);
//	DispSt2C(DispAmtStr(Msg[QUXIAO].str, &ApplVar.SubTotal));
	savenumber = ApplVar.CorrecNumber;
	saveclerk = ApplVar.ClerkNumber;
	ProcessBuffer();
	ApplVar.CorrecNumber = savenumber;
	ReadCorrec();							/* read function */
	ApplVar.ClerkNumber = saveclerk;
	ReadClerk();
	if (ApplVar.ErrorNumber)
	{
		ApplVar.FTrvoid = 0;
		return;
	}
	ApplVar.PrintLayOut = ApplVar.Correc.Print;
	if (!ApplVar.FRegi)
	{
		for (i = 0;i < 8;i++)
			ApplVar.TaxItem[i].Sign ^= 0x80;
		ApplVar.SaleAmt.Sign ^= 0x80;
		if (ApplVar.FTrvoid != 2)
			CalculateTax(0);						/* calculate and add in report */
	}
	else
		PrintLine('-');
// lyq   ApplVar.SaleAmt = ZERO;					 //lyq modified for print the total of Fvoid 2003\11\3 start
    ApplVar.SaleQty = ZERO;
    MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);
	memcpy(&ApplVar.SaleAmt.Value,&ApplVar.SubTotal.Value,sizeof(BCD)-1);
	ApplVar.SaleAmt.Sign ^=0x80;
#if (SCREENWD>=24)
	PrintAmt(ApplVar.Correc.Name, &ApplVar.SaleAmt);//ccr &ApplVar.SubTotal);
#else
	DispStrLF(ClearFlashChar(ApplVar.Correc.Name,SCREENWD));
	DispStrLF(FormatAmtStr(0, &ApplVar.SaleAmt, SCREENWD));
	PrintRJ(FormatAmtStr(ApplVar.Correc.Name, &ApplVar.SaleAmt, PRTLEN));
#endif
   	ApplVar.SaleAmt = ZERO;							//lyq modified for print the total of Fvoid 2003\11\3 start
    RESETBIT(ARROWS, BIT1); /* reset RG led */
	ApplVar.FRegi = 0;
	ApplVar.FSale = 0;
	RegiEnd();
	ReceiptIssue(1);
	ApplVar.FTrvoid = 0;
	ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
	ApplVar.Amt = ApplVar.SubTotal;
	ApplVar.Qty = ONE;
	ApplVar.RetQty = ZERO;
	ApplVar.DiscAmt = ZERO;
	ApplVar.Cost = ZERO;
	AddCorrecTotal();
	ApplVar.BufCC = 1;
	AddCorrecTotal();			/* reset CC */
	ApplVar.BufCC= 0;
    if (TESTBIT(CLERKFIX, BIT0) && !ApplVar.ClerkLock)	  /* clerk compulsory, no key */
    {
		ApplVar.ClerkNumber = 0;
		ApplVar.FTrain = 0;
    }
	// DispSubTotal();
}

void Correction()
{
    if (Appl_EntryCounter)
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI04);
		return;
    }
    //ccr2016-03-01>>>>>>>>>>>>>>>>>>>>>>>>
#if (PC_EMUKEY)
    if (!FisTestTask.Going)
#endif
    if (ApplVar.Key.Code == xRECEIPT)
    {// 确认取消交易  //
        if (!WaitForYesNo(Msg[QXJYIXZHI].str,false))
            return;
    }
    //ccr2016-03-01<<<<<<<<<<<<<<<<<<<<<<<<<
    ApplVar.CorrecNumber = ApplVar.Key.Code - CORREC - 1;
    if (ApplVar.CorrecNumber < ApplVar.AP.Correc.Number)
		ReadCorrec();		/* read function */
    else
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
		return;
    }
    if (ApplVar.FPb)
		ApplVar.Correc.Print = ApplVar.PbF.Print;
    ApplVar.PrintLayOut = ApplVar.Correc.Print;
    switch(ApplVar.Correc.Options & 0x07) {
    case 0:     /* void */
//cc 2006-10-19 for SubDisc>>>>>>>>>>>>>>>>>>>
        if(TESTBIT(ApplVar.SubDiscFlag,BIT0))
        {
            ApplVar.ErrorNumber = ERROR_ID(CWXXI01);
            return;
        }
//cc 2006-10-19 for SubDisc<<<<<<<<<<<<<<<<<<<<<
		if (!TESTBIT(ApplVar.Clerk.Options,BIT0) && !CheckPWD(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG))
		{
			ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
			return;
		}

	    if (ApplVar.FRegi && ApplVar.RepeatCount)
	    {
            ApplVar.FVoid = 1;
            SETBIT(ApplVar.SubDiscFlag,BIT1);//cc 2006-10-19 for SubDisc
            StoreInBuffer();
			ApplVar.RGRec.Key.Code = CORREC + 1 + ApplVar.CorrecNumber;
			if (ApplVar.FPlu)		/* Previous PLU */
				ProcessPlu();
			else
				ProcessDept();
	    }
	    else if (ApplVar.RGRec.Key.Code > DISC && ApplVar.RGRec.Key.Code < DISC + 100)
	    {
            ApplVar.FVoid = 1;
            SETBIT(ApplVar.SubDiscFlag,BIT1);//cc 2006-10-19 for SubDisc
            StoreInBuffer();
			ApplVar.RGRec.Key.Code = CORREC + 1 + ApplVar.CorrecNumber;
			Discount();
	    }
	    else if (ApplVar.RGRec.Key.Code > PORA && ApplVar.RGRec.Key.Code < PORA + 100)
	    {
            ApplVar.FVoid = 1;
            SETBIT(ApplVar.SubDiscFlag,BIT1);//cc 2006-10-19 for SubDisc
            StoreInBuffer();
			ApplVar.RGRec.Key.Code = CORREC + 1 + ApplVar.CorrecNumber;
			PaidOutRa();
	    }
	    else
	    {
			ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
			ApplVar.RGRec.Key.Code = 0;
	    }
	    return;
	case 1:     /* correction */
//cc 2006-10-19 for SubDisc>>>>>>>>>>>>>>>>>>>
        if(TESTBIT(ApplVar.SubDiscFlag,BIT0))
        {
            ApplVar.ErrorNumber = ERROR_ID(CWXXI01);
            return;
        }
//cc 2006-10-19 for SubDisc<<<<<<<<<<<<<<<<<<
		if (!TESTBIT(ApplVar.Clerk.Options,BIT1) && !CheckPWD(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG))
		{
			ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
			return;
		}

	    if (!ApplVar.FRegi)
			ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
	    else
	    {
			mClearScreen();
			DispStrXY(ClearFlashChar(ApplVar.Correc.Name,SCREENWD),0,0);
			DispStrXY(Msg[SHURUQTYAMT].str,0,1);
            SETBIT(ApplVar.MyFlags,CLSSCREEN);
            LCD10_SetFlag(FLAG_NULL);
			PutsO(Msg[CORRECT].str);//C-DISPLAY
			ApplVar.FCorr = 1;
	    }
	    StoreInBuffer();
	    ApplVar.RGRec.Key.Code = 0;
	    break;
	case 2:     /* Refund */
//cc 2006-10-19 for SubDisc>>>>>>>>>>>>>>>>>>>
        if(TESTBIT(ApplVar.SubDiscFlag,BIT0))
        {
            ApplVar.ErrorNumber = 1;
            return;
        }
/* ccr100917>>>>>> cc为什么加此限制 >>>>>>>>>
        if(ApplVar.FRefund || ApplVar.FRegi)
        {
            ApplVar.ErrorNumber = 1;
            return;
        }
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
//cc 2006-10-19 for SubDisc<<<<<<<<<<<<<<<<<<
		if (!TESTBIT(ApplVar.Clerk.Options,BIT3) && !CheckPWD(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG))
		{
			ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
			return;
		}
	    if (ApplVar.FSplit)
			ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
	    else
	    {
            StoreInBuffer();
            ApplVar.RGRec.Key.Code = 0;
			mClearScreen();
			DispStrXY(ClearFlashChar(ApplVar.Correc.Name,SCREENWD),0,0);
			DispStrXY(Msg[SHURUQTYAMT].str,0,1);
            SETBIT(ApplVar.MyFlags,CLSSCREEN);
            LCD10_SetFlag(FLAG_NULL);
			PutsO(Msg[REFUND].str);//C-DISPLAY
			ApplVar.FRefund = 1;
	    }
	    break;
	case 3:
		if (!TESTBIT(ApplVar.Clerk.Options,BIT2) && !CheckPWD(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG))
		{
			ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
			return;
		}
    case 4:					// 挂账=取消交易  //
        ApplVar.FRefund = 0;
		TransactionVoid(ApplVar.Key.Code);// 取消交易  //
        ApplVar.SubDiscFlag = 0;		/* cc 2006-10-11 for SubDisc */
		break;
#if (0)
	case 5 :     /* cancel 1 */
	    if (ApplVar.FRegi)
			ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
	    else
	    {
			PutsO(Msg[CANCEL1].str);
			ApplVar.FCanc = 1;
			ApplVar.CancelNumber = ApplVar.Key.Code - CORREC - 1;
			ApplVar.CancPrint = ApplVar.Correc.Print | 0x04;    /* set double size */
	    }
	    break;
	case 6 :    /* cancel 2 */
	    if (ApplVar.FRegi)
			ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
	    else
	    {
			PutsO(Msg[CANCEL2].str);
			ApplVar.FCanc = 2;
			ApplVar.CancPrint = 0xfe;   /* not on journal */
	    }
	    break;
#endif
	default:
	    ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
	    break;
    }
}

