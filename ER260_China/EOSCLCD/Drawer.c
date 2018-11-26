#define DRAWER_C 1
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"


/***************************************************************/

void GetDrawerOffSet()
{
	RamOffSet = ApplVar.DrawNumber * ApplVar.AP.Draw.RecordSize + ApplVar.AP.StartAddress[AddrDrawer];
}

void AddDrawerTotal()
{
    if (ApplVar.DrawNumber < ApplVar.AP.Draw.Number)
    {
		GetDrawerOffSet();
	    RamOffSet += ApplVar.AP.Draw.TotalOffSet;
		for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
		{
		    ApplVar.Size = ApplVar.AP.Draw.Size[ApplVar.PointerType];
		    AddPointerTotal();
		}
    }
}

void WriteDrawer()
{
    if (ApplVar.DrawNumber < ApplVar.AP.Draw.Number)
    {
		GetDrawerOffSet();
		WriteRam((BYTE*)&ApplVar.Draw, ApplVar.AP.Draw.TotalOffSet);
    }
}

void ReadDrawer()
{
    GetDrawerOffSet();
    ReadRam((BYTE*)&ApplVar.Draw, ApplVar.AP.Draw.TotalOffSet);
//ccr2015-08-14 >>>>>>小数点与系统设置一致
//    if (TESTBIT(NO_DECIMAL,BIT0))
//        SETBIT(ApplVar.Draw.Options,BIT0);
//    else
//        RESETBIT(ApplVar.Draw.Options,BIT0);
//    if (TESTBIT(NO_DECIMAL,BIT1))
//        SETBIT(ApplVar.Draw.Options,BIT2);
//    else
//        RESETBIT(ApplVar.Draw.Options,BIT2);
//ccr2016-11-07<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void OpenDrawer()
{
	DrawerOpen();
    ApplVar.OpDraw = 0;
}

/* DRAW OPTIONS */
/*
	If BIT 1 is not set then Drawer Open Total
	BIT0	BIT2	-> Decimal point position
	0		0		1,00
	1		0		1
	0		1		1,000
	1		1		1,0
*/
void Drawer()
{

    ApplVar.DrawNumber = 0;
    if (ApplVar.Key.Code == DRAW)
    {
		if (!Appl_EntryCounter || ApplVar.NumberEntry > 99)
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
		    return;
		}
		ApplVar.DrawNumber = ApplVar.NumberEntry;
    }
    else
		ApplVar.DrawNumber = ApplVar.Key.Code - DRAW;
    if (!ApplVar.DrawNumber || ApplVar.DrawNumber > ApplVar.AP.Draw.Number ||
	!(ApplVar.AP.Draw.Size[0].Periods & 0x01) || !ApplVar.AP.Draw.Size[0].Amt)
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
		return;
    }

    ApplVar.DrawNumber--;
	ReadDrawer();
    if (TESTBIT(ApplVar.Draw.Options, BIT1))	    /* open drawer counter? */
    {
		if (!ApplVar.ClerkNumber)
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI33);	    /* select clerk */
		    return;
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
			    return;
			}
		}
#endif
		OpenDrawer();
        ApplVar.Amt = ONE;
        AddDrawerTotal();
	    if (!ApplVar.FRegi)
		{
			if (TESTBIT(CLERKFIX, BIT0) && !ApplVar.ClerkLock)    /* clerk compulsory, no key */
			{
				ApplVar.ClerkNumber = 0;
				ApplVar.FTrain = 0;
			}
#if (salNumber)
			if (TESTBIT(KEYTONE, BIT6))    /* Salesp compulsory */
				ApplVar.SalPerNumber = 0;
			else if (TESTBIT(AM_PM, BIT4))	  /* Salesp reset to 1*/
			{
				ApplVar.SalPerNumber = 1;
				ReadSalPer();
			}
#endif
		}
    }
    else
    {
		ApplVar.Size = ApplVar.AP.Draw.Size[0];
		GetDrawerOffSet();
		RamOffSet += ApplVar.AP.Draw.TotalOffSet;
		ReadTotal();
#if (0) //ccr2016-11-07>>>小数点与系统设置一致
		if (TESTBIT(ApplVar.Draw.Options, BIT2))
			ApplVar.AmtDecimal = 2;
		else
			ApplVar.AmtDecimal = 0;
		ApplVar.AmtDecimal += ApplVar.Draw.Options & BIT0;
#endif //ccr2016-11-07<<<<<<<<<<<<<<<<<
//ccr2016-03-02		PutsO(DispAmtStr(Msg[OPENDRAW].str, &ApplVar.Total.Amt,DISLEN));//C-DISPLAY
		DispStrLF(FormatAmtStr(Msg[DKQXIANG].str, &ApplVar.Total.Amt,SCREENWD));
		ApplVar.AmtDecimal = NO_DECIMAL;		/* restore amount format */
		Appl_EntryCounter = 1;   /* force clear of entry */
    }
    if (Appl_EntryCounter)
		ClearEntry(false);
}
//ccr 050301>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void DispDrawerTotal()
{
    if (ApplVar.DrawNumber < ApplVar.AP.Draw.Number)
    {
    	ReadDrawer();
		ApplVar.Draw.Name[ApplVar.AP.Draw.CapSize] = 0;
		GetDrawerOffSet();
	    RamOffSet += ApplVar.AP.Draw.TotalOffSet;
		ApplVar.PointerType = 0;
//		for (; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
		{
		    ApplVar.Size = ApplVar.AP.Draw.Size[ApplVar.PointerType];
		    DisplayPointerTotal(ApplVar.Draw.Name);
		}
    }
}
//ccr 050301<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

