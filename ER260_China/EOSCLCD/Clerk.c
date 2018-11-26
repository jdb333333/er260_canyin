#define CLERK_C 5

#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"


/****************************************************************/

void GetClerkOffSet()
{
    RamOffSet = (ApplVar.ClerkNumber - 1) * ApplVar.AP.Clerk.RecordSize + ApplVar.AP.StartAddress[AddrClerk];
}

void WriteClerk()
{
    if (ApplVar.ClerkNumber <= ApplVar.AP.Clerk.Number)
    {
		GetClerkOffSet();
		WriteRam(ApplVar.Clerk.Name, ApplVar.AP.Clerk.CapSize);
		WriteRam(&ApplVar.Clerk.Options, sizeof(ApplVar.Clerk.Options));
	    WriteRam(ApplVar.Clerk.Passwd,sizeof(ApplVar.Clerk.Passwd));
    }
}

void ReadClerk()
{
    //if (!ApplVar.ClerkNumber || ApplVar.ClerkNumber > ApplVar.AP.Clerk.Number)
    //		ApplVar.ClerkNumber = 1;
    //cc 2006-01-11>>>>>>>>>>>>>>>
    if(!ApplVar.ClerkNumber || ApplVar.ClerkNumber > ApplVar.AP.Clerk.Number)
    {
        ApplVar.ClerkNumber = 0;
        return;
    }
    //cc 2006-01-11>>>>>>>>>>>>>>>
    GetClerkOffSet();
    ReadRam(ApplVar.Clerk.Name, ApplVar.AP.Clerk.CapSize);
    ApplVar.Clerk.Name[ApplVar.AP.Clerk.CapSize] = 0 ;
    ReadRam(&ApplVar.Clerk.Options, sizeof(ApplVar.Clerk.Options));
    ReadRam(ApplVar.Clerk.Passwd,sizeof(ApplVar.Clerk.Passwd));
    //ccr2017-02-27>>>>>>
    if (TESTBIT(DOT,BIT1))
        RESETBIT(ApplVar.Clerk.Options, BIT7);//training mode
    else
        SETBIT(ApplVar.Clerk.Options, BIT7);
    //ccr2017-02-27<<<<<<<

}


void SelectClerk(BYTE lock)
{
    WORD newclerk;
    short sSave;

    char cPass[7];

	if (ApplVar.CentralLock != RG && ApplVar.CentralLock != MG)
	{
		ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
		return;
	}

    newclerk = 0;
	if (ApplVar.Key.Code == CLERK && ApplVar.NumberEntry < (OFFER-CLERK))
		newclerk = ApplVar.NumberEntry;
	else if (!Appl_EntryCounter)
	{
		newclerk = ApplVar.Key.Code - CLERK;
		ApplVar.Entry = ZERO;
		WORDtoBCD(ApplVar.Entry.Value, newclerk);
	}
	if (newclerk > ApplVar.AP.Clerk.Number)
	{
	    ApplVar.ErrorNumber=ERROR_ID(CWXXI24);
		return;
	}
	if (ApplVar.FRegi)    /* still in regi and train no change clerk */
    {
	    if ((ApplVar.FPb && TESTBIT(CLERKFIX, BIT1)) ||
		    TESTBIT(CLERKFIX, BIT2) || ApplVar.FTrain || !newclerk)
	    {
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI14);	/* still in registration */
			return;
		}
	}
	if (!lock)
	{
		if (!newclerk)
		{
	      	ApplVar.ClerkNumber = 0;
#if (SCRSTATDBL)
            DispOnStatDBL(Editor.EditBuff+Editor.BuffSize-SCRDBLWD,0);
#else
			DispSt2C((BYTE*)Msg[CWXXI33].str);//RG-DISPLAY
#endif
			return;
		}
		if(TESTBIT(CLERKFIX, BIT5))		/* secret code ? */
		{
			sSave = ApplVar.ClerkNumber;
			ApplVar.ClerkNumber = newclerk;
			ReadClerk();
			ApplVar.ClerkNumber = sSave;
			HEXtoASC(cPass,ApplVar.Clerk.Passwd,3);
			cPass[6] = 0;
			if (!CheckPWD(cCLERKPASS,cPass))
			{
			       //cc 2006-01-09>>>>>>>>>
		              ApplVar.ClerkNumber = 0;
                            ReadClerk();
                            //cc 2006-01-09>>>>>>>>>
				ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
				return;
			}
		}
	}
    if (ApplVar.RGRec.Key.Code < CLERK || ApplVar.RGRec.Key.Code >=OFFER)
		StoreInBuffer();
    ApplVar.RGRec.Key.Code = CLERK + newclerk;	/* store in buffer for correct CC update */
    ApplVar.ClerkNumber = newclerk;

    if(ApplVar.ClerkNumber)
    		ReadClerk();

    //ccr2017-02-27>>>>>>>>

    if (TRAINING)	/* training clerk ? */
    {
		ApplVar.FTrain = 1;	  /* training mode */
    }
    else
    {
		ApplVar.FTrain = 0;	/* reset training */
	}

	Collect_Data(OPERLOG);

#if (SCRSTATDBL)
    DispOnStatDBL(ApplVar.Clerk.Name,0);
#else
    ClsState2();   //RG-DISPLAY
    DispSt2(ApplVar.Clerk.Name);  //RG-DISPLAY
#endif

}

