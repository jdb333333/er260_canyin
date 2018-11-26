#define CURRENCY_C 6
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

/****************** Define used Global variables ***********************/

void GetCurrOffSet()
{
	RamOffSet = ApplVar.CurrNumber * ApplVar.AP.Curr.RecordSize + ApplVar.AP.StartAddress[AddrCurr];
}

void AddCurrTotal()
{
    GetCurrOffSet();
	RamOffSet += ApplVar.AP.Curr.TotalOffSet;

    for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
    {
		ApplVar.Size = ApplVar.AP.Curr.Size[ApplVar.PointerType];
		AddPointerTotal();
    }
}

void WriteCurr()
{
    if (ApplVar.CurrNumber < ApplVar.AP.Curr.Number)
    {
		GetCurrOffSet();
		WriteRam((BYTE*)&ApplVar.Curr, ApplVar.AP.Curr.TotalOffSet);      /* write function options */
    }
}

void ReadCurr()
{
    GetCurrOffSet();
    ReadRam((BYTE*)&ApplVar.Curr, ApplVar.AP.Curr.TotalOffSet);	  /* write function options */
}

/* if sell = 0 then local to currency with buyrate */
/* if sell = 1 then currency to local with buyrate */
/* if sell = 2 then local to currency with sell rate */

void GetCurrency(BYTE sell, BCD *amt)
{
    BCD rate;

    rate = ZERO;
    if (sell > 1)   /* selling ? */
		memcpy(rate.Value, ApplVar.Curr.SellRate, sizeof(ApplVar.Curr.SellRate));
    else
		memcpy(rate.Value, ApplVar.Curr.BuyRate, sizeof(ApplVar.Curr.BuyRate));
    rate.Sign = 0x03;	 /* 4 decimals */
    if (sell == 1)
    {
		Divide(amt, &rate);
		if (NO_DECIMAL) /* local with decimal ? */
		    RoundBcd(amt, 0);
		else
		{
		    if (TESTBIT(ApplVar.Curr.Options, BIT0))	/* if set no decimals */
				RoundBcd(amt, 2);
		    else
				RoundBcd(amt, 0);
		}
		amt->Sign &= 0xfc;
		AmtRound(0, amt);
		return;
    }
    else
    {
		Multiply(amt, &rate);
		RoundBcd(amt, 0);
    }
    if (TESTBIT(ApplVar.Curr.Options, BIT0))	/* if set no decimals */
    {
		if (!NO_DECIMAL)     /* local is with decimal ? */
		{
		    amt->Sign |= 0x02;
		    RoundBcd(amt, 0);
		}
    }
}

void Currency()
{
    if (Appl_EntryCounter)
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
		return;
    }
    if (ApplVar.FPb)	   /* pb used ? */
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI07);
		return;
    }
    ApplVar.CurrNumber = ApplVar.Key.Code - CURR - 1;
    if (ApplVar.CurrNumber < ApplVar.AP.Curr.Number)
		ReadCurr();
    else
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
		return;
    }
    if (!ApplVar.FTend)     /* not in tender */
		CalculateTax(1);	/* calulate and not add report */
    ApplVar.Amt = ApplVar.SubTotal;
    GetCurrency(0, &ApplVar.Amt);
    ApplVar.AmtDecimal = ApplVar.Curr.Options & BIT0;
#if defined(LCD10_DISP)
    LCD10_SetFlag(FLAG_NULL);
    PutsO(DispAmtStr(0, &ApplVar.Amt,DISLEN));//C-DISPLAY
#else
    PutsO(DispAmtStr(Msg[CURRENCY].str, &ApplVar.Amt,DISLEN));//C-DISPLAY
#endif
//    DispSt2C(DispAmtStr(Msg[SETCURR].str, &ApplVar.Amt));
    ApplVar.AmtDecimal = NO_DECIMAL;		    /* restore amount format */
    ApplVar.FCurr = 1;
}

