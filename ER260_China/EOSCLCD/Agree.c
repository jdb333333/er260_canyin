#define AGREE_C 7
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"


/********************************************************************/

void GetAgreeOffSet()
{
#if (agreeNumber)
	RamOffSet = (UnLong)ApplVar.AgreeNumber * ApplVar.AP.Agree.RecordSize + ApplVar.AP.StartAddress[AddrAgree];
#endif
}


void WriteAgree()
{
#if (agreeNumber)
    if (ApplVar.AgreeNumber < ApplVar.AP.Agree.Number)
    {
		GetAgreeOffSet();
		WriteRam(ApplVar.Agree.Name, ApplVar.AP.Agree.CapSize);
		WriteRam(ApplVar.Agree.Addr, sizeof(ApplVar.Agree.Addr));
		WriteRam(ApplVar.Agree.Tax, sizeof(ApplVar.Agree.Tax));
		WriteRam(ApplVar.Agree.Total, sizeof(ApplVar.Agree.Total));
    }
#endif
}

void ReadAgree()
{
#if (agreeNumber)
    if (ApplVar.AgreeNumber < ApplVar.AP.Agree.Number)
    {
	    GetAgreeOffSet();
		ReadRam(ApplVar.Agree.Name, ApplVar.AP.Agree.CapSize);
		ReadRam(ApplVar.Agree.Addr, sizeof(ApplVar.Agree.Addr));
		ReadRam(ApplVar.Agree.Tax, sizeof(ApplVar.Agree.Tax));
		ReadRam(ApplVar.Agree.Total, sizeof(ApplVar.Agree.Total));
	}
#endif
}
