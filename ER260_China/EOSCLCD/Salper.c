#define SALPER_C 2
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"


/***************************************************************/

void GetSalPerOffSet()
{
#if (salNumber)
	RamOffSet = (ApplVar.SalPerNumber - 1) * ApplVar.AP.SalPer.RecordSize + ApplVar.AP.StartAddress[AddrSalPer];
#endif
}

void WriteSalPer()
{
#if (salNumber)
    if (ApplVar.SalPerNumber <= ApplVar.AP.SalPer.Number)
    {
		GetSalPerOffSet();
		WriteRam(ApplVar.SalPer.Name, ApplVar.AP.SalPer.CapSize);
		WriteRam(&ApplVar.SalPer.Options, sizeof(ApplVar.SalPer.Options));
    }
#endif
}

void ReadSalPer()
{
#if (salNumber)
    if (ApplVar.AP.SalPer.Number)
    {
        GetSalPerOffSet();
		ReadRam(ApplVar.SalPer.Name, ApplVar.AP.SalPer.CapSize);
		ReadRam(&ApplVar.SalPer.Options, sizeof(ApplVar.SalPer.Options));
		ApplVar.SalPer.Name[ApplVar.AP.SalPer.CapSize] = 0 ;
    }
#endif
}


void SelectSalPer()
{
#if (salNumber)
    WORD newsalper;

    if (ApplVar.AP.SalPer.Number)
    {

        if (ApplVar.CentralLock != RG && ApplVar.CentralLock != MG)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return;
        }

        newsalper = 0;
        if (ApplVar.Key.Code == SALPER && ApplVar.NumberEntry < 256)
            newsalper = ApplVar.NumberEntry;
        else if (!Appl_EntryCounter)
        {
            newsalper = ApplVar.Key.Code - SALPER;
            ApplVar.Entry = ZERO;
            WORDtoBCD(ApplVar.Entry.Value, newsalper);
        }
        if (!newsalper || newsalper > ApplVar.AP.SalPer.Number)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return;
        }
        if (ApplVar.FRegi)    /* still in regi no change salesperson */
        {
            if (TESTBIT(KEYTONE, BIT7))
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI14);   /* still in registration */
                return;
            }
        }
        if (ApplVar.RGRec.Key.Code < SALPER || ApplVar.RGRec.Key.Code > SALPER + 255)
            StoreInBuffer();
        ApplVar.RGRec.Key.Code = SALPER + newsalper;  /* store in buffer for correct CC update */
        ApplVar.SalPerNumber = newsalper;
        ReadSalPer();

#if (SCRSTATDBL)
        DispOnStatDBL(ApplVar.SalPer.Name,0);
#else
        ClsState2();
        //DispSt3(ApplVar.SalPer.Name);	//hf 20060105
        DispSt2(ApplVar.SalPer.Name);//RG-DISPLAY
#endif
    }
#endif
}

