#define MODIFIER_C 2
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "message.h"

/***************************************************************/

void GetModiOffSet()
{
	RamOffSet = (UnLong)ApplVar.ModiNumber * ApplVar.AP.Modi.CapSize + ApplVar.AP.StartAddress[AddrModi] ;
}

void WriteModi()
{
    if (ApplVar.ModiNumber < ApplVar.AP.Modi.Number)
    {
		GetModiOffSet();
		WriteRam(ApplVar.Modi.Name, ApplVar.AP.Modi.CapSize);
    }
}

void ReadModi()
{
    GetModiOffSet();
    ReadRam(ApplVar.Modi.Name, ApplVar.AP.Modi.CapSize);
    ApplVar.Modi.Name[ApplVar.AP.Modi.CapSize] = 0;
}


void GetModifier()
{
    if (ApplVar.ModiCount > 3)
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI46);
		return;
    }
    ApplVar.ModiNumber = 0;
    //ccr2016-01-26>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //    if (ApplVar.Key.Code == MODI && ApplVar.NumberEntry < 1000)
    //		ApplVar.ModiNumber = ApplVar.NumberEntry;
    if (ApplVar.Key.Code == MODI)
    {
         switch (ApplVar.ModiNumber = ListItems(SETMODIF,1,It_SELEMODI,true))
         {
         case It_EXIT:
         case 0:
             return;
         default:
            ApplVar.NumberEntry=ApplVar.ModiNumber;
            break;
         }
    }//ccr2016-01-26<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    else if (!Appl_EntryCounter)
    {
        ApplVar.ModiNumber = ApplVar.Key.Code - MODI;
		WORDtoBCD(ApplVar.Entry.Value, ApplVar.ModiNumber);
    }
    if (!ApplVar.ModiNumber || ApplVar.ModiNumber > ApplVar.AP.Modi.Number)
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
		return;
    }
    if (TESTBIT(KPPRICE, BIT5))     /* german system then enter first */
    {
		StoreInBuffer();
		ApplVar.RGRec.Key.Code = 0;
    }
    else if (ApplVar.RGRec.Key.Code < MODI || (ApplVar.RGRec.Key.Code > MODI + 999 && ApplVar.RGRec.Key.Code < DEPT))
    {
		ApplVar.ErrorNumber=ERROR_ID(CWXXI23);
		return;
    }
    ApplVar.ModiNumber--;
    ReadModi();
//ccr2016-01-26    PutsO(FormatQtyStr(Msg[ADDMODI].str, &ApplVar.Entry, DISLEN));
    if (!TESTBIT(KPPRICE, BIT5))     /* german system then enter first */
    {                       /* and print with department */
		if (!TESTBIT(PLU_PRINT,BIT2)
			|| !ApplVar.AP.Plu.CapSize)     /* print depart name on receipt */
		{
			MemSet(SysBuf, sizeof(SysBuf), ' ');
		    strcpy(SysBuf + 2, ApplVar.Modi.Name);
			PrintStr(SysBuf);
		}
    }
    DispLFStr(ApplVar.Modi.Name);//ccr2016-01-26
    ApplVar.ModNum[ApplVar.ModiCount] = ApplVar.ModiNumber;
    ApplVar.ModiCount++;
    ApplVar.OldModiCount = 0;
}

