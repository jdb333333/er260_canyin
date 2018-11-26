#define OFF_C 4
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

/***************************************************************/

void GetOFFPriceOffSet()
{
	RamOffSet = (UnLong)ApplVar.OFFNumber * ApplVar.AP.OFFPrice.RecordSize + ApplVar.AP.StartAddress[AddrOFF];
}

void WriteOFFPrice()
{
    if (ApplVar.OFFNumber < ApplVar.AP.OFFPrice.Number)
    {
		GetOFFPriceOffSet();
		WriteRam(ApplVar.OFFPrice.Name, ApplVar.AP.OFFPrice.CapSize);
		WriteRam(&ApplVar.OFFPrice.Type, sizeof(ApplVar.OFFPrice.Type));
		WriteRam(ApplVar.OFFPrice.DateFrom, sizeof(ApplVar.OFFPrice.DateFrom));
		WriteRam(ApplVar.OFFPrice.DateTo, sizeof(ApplVar.OFFPrice.DateTo));
		WriteRam(ApplVar.OFFPrice.TimeFrom, sizeof(ApplVar.OFFPrice.TimeFrom));
		WriteRam(ApplVar.OFFPrice.TimeTo, sizeof(ApplVar.OFFPrice.TimeTo));
		WriteRam(&ApplVar.OFFPrice.WeekDay, sizeof(ApplVar.OFFPrice.WeekDay));
		switch (ApplVar.OFFPrice.Type){
		case 1:
//lyq modify 2003\10\20 start
//			ApplVar.OFFPrice.OFFVal.TYPE1.NumItems = 0;
//			*((WORD *)ApplVar.OFFPrice.OFFVal.TYPE1.PriceU) = 0x0000;
//			*((WORD *)ApplVar.OFFPrice.OFFVal.TYPE1.PriceP) = 0x0000;
//			ApplVar.OFFPrice.OFFVal.TYPE1.NumItems = 4;
//			*((WORD *)ApplVar.OFFPrice.OFFVal.TYPE1.PriceU) = 0x0400;
//			*((WORD *)ApplVar.OFFPrice.OFFVal.TYPE1.PriceP) = 0x0800;
//lyq modify 2003\10\20 end
			WriteRam(&ApplVar.OFFPrice.OFFVal.TYPE1.NumItems, sizeof(ApplVar.OFFPrice.OFFVal.TYPE1.NumItems));
			WriteRam(ApplVar.OFFPrice.OFFVal.TYPE1.PriceU, ApplVar.AP.OFFPrice.PriUSize);
			WriteRam(ApplVar.OFFPrice.OFFVal.TYPE1.PriceP, sizeof(ApplVar.OFFPrice.OFFVal.TYPE1.PriceP));
			break;
		case 2:
//			*((WORD *)ApplVar.OFFPrice.OFFVal.Discount) = 0x1500;
			WriteRam(ApplVar.OFFPrice.OFFVal.Discount, 2);
			break;
		}
    }
}

void ReadOFFPrice()
{
    if (ApplVar.OFFNumber < ApplVar.AP.OFFPrice.Number)
    {
	    GetOFFPriceOffSet();
		ReadRam(ApplVar.OFFPrice.Name, ApplVar.AP.OFFPrice.CapSize);
		ReadRam(&ApplVar.OFFPrice.Type, sizeof(ApplVar.OFFPrice.Type));
		ReadRam(ApplVar.OFFPrice.DateFrom, sizeof(ApplVar.OFFPrice.DateFrom));
		ReadRam(ApplVar.OFFPrice.DateTo, sizeof(ApplVar.OFFPrice.DateTo));
		ReadRam(ApplVar.OFFPrice.TimeFrom, sizeof(ApplVar.OFFPrice.TimeFrom));
		ReadRam(ApplVar.OFFPrice.TimeTo, sizeof(ApplVar.OFFPrice.TimeTo));
		ReadRam(&ApplVar.OFFPrice.WeekDay, sizeof(ApplVar.OFFPrice.WeekDay));
		switch (ApplVar.OFFPrice.Type){
		case 1:
			ReadRam(&ApplVar.OFFPrice.OFFVal.TYPE1.NumItems, sizeof(ApplVar.OFFPrice.OFFVal.TYPE1.NumItems));
			ReadRam(ApplVar.OFFPrice.OFFVal.TYPE1.PriceU, ApplVar.AP.OFFPrice.PriUSize);
			ReadRam(ApplVar.OFFPrice.OFFVal.TYPE1.PriceP, sizeof(ApplVar.OFFPrice.OFFVal.TYPE1.PriceP));
			break;
		case 2:
			ReadRam(ApplVar.OFFPrice.OFFVal.Discount, 2);
			break;
		}
	}
}


void Print_Modifiers()
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

void ProcessOFFPrice()
{
}
