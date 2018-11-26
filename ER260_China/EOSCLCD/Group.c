#define GROUP_C 2
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"


/***************************************************************/

void GetGroupOffSet()
{
	RamOffSet = (UnLong)ApplVar.GroupNumber * ApplVar.AP.Group.RecordSize + ApplVar.AP.StartAddress[AddrGroup] ;
}

void AddGroupTotal()
{
    //先统计总汇总数据
	RamOffSet = ApplVar.AP.StartAddress[AddrTotl];
    for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)	  /* add total sales */
    {
		ApplVar.Size = ApplVar.AP.Sales.Size[ApplVar.PointerType];
		AddPointerTotal();
    }
    //再统计柜组汇总数据
    if (ApplVar.GroupNumber < ApplVar.AP.Group.Number)	   /* when no groups return */
    {
		GetGroupOffSet();
		RamOffSet += ApplVar.AP.Group.TotalOffSet ;
		for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
		{
		    ApplVar.Size = ApplVar.AP.Group.Size[ApplVar.PointerType];
		    AddPointerTotal();
		}
    }
}


void WriteGroup()
{
    if (ApplVar.GroupNumber < ApplVar.AP.Group.Number)	   /* when no groups return */
    {
		GetGroupOffSet();
		WriteRam((BYTE*)&ApplVar.Group,ApplVar.AP.Group.TotalOffSet);//============================
    }
}

void ReadGroup()
{
    GetGroupOffSet();
    ReadRam((BYTE*)&ApplVar.Group, ApplVar.AP.Group.TotalOffSet);
    ApplVar.Group.Name[ApplVar.AP.Group.CapSize] = 0;
}

