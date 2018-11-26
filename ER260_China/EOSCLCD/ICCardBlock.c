#define ICBLOCK_C 2
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

/***************************************************************/
  //ccr chipcard 2004-07-01
void GetICBlockOffSet()
{
	//ApplVar.AP.ICBlock.CapSize = 10;
	RamOffSet = (UnLong)ApplVar.ICBlockNumber * icblockSize + ApplVar.AP.StartAddress[AddrICBlock] ;
}

void WriteICBlock()
{


    if (ApplVar.ICBlockNumber < ApplVar.AP.ICBlock.Number)
    {
     GetICBlockOffSet();
     WriteRam((BYTE*)&ApplVar.ICBlock.ICCardNO, 4);
    }
}

void ReadICBlock()
{

    GetICBlockOffSet();
    ReadRam((BYTE*)&ApplVar.ICBlock.ICCardNO, 4);

}
//ccr2016-01-12>>>>>>>>>>>
void ListICBlock()
{
    ReadICBlock();
    ULongtoASCL(ListRecord.Name,ApplVar.ICBlock.ICCardNO);
}

/*//---------------------------------------------------
BYTE StrToUnlong(unsigned long Data,char *Str)
{
unsigned long Data;

if (sizeof(Str)>11)  return -1;
Data = (Str[0]-0x30)*10000000 +
       (Str[1]-0x30)*1000000 +
       (Str[2]-0x30)*100000 +
       (Str[3]-0x30)*10000 +
       (Str[4]-0x30)*1000 +
       (Str[5]-0x30)*100 +
       (Str[6]-0x30)*10 +
       (Str[7]-0x30);

return 0;
}*/

/*//---------------------------------------------------
BYTE UnlongToStr(char *Str,unsigned long Data)
{
if (sizeof(Str)>9)  return -1;

Str[7] = Data%10+0x30;
Str[6] = (Data%100)/10+0x30;
Str[5] = (Data%1000)/100+0x30;
Str[4] = (Data%10000)/1000+0x30;
Str[3] = (Data%100000)/10000+0x30;
Str[2] = (Data%1000000)/100000+0x30;
Str[1] = (Data%10000000)/1000000+0x30;
Str[0] = (Data%100000000)/10000000+0x30;

Str[8]=0x00;

return 0;
}*/

//----------------------------------------------------
void PrintICBlock()
{
BCD TempBCD;
unsigned char TempStr[15];
unsigned char TempStr1[15];

RJPrint(print,Msg[GUASHIIC].str);

for(ApplVar.ICBlockNumber=0;ApplVar.ICBlockNumber<100;ApplVar.ICBlockNumber++)
{
 ReadICBlock();
 if (ApplVar.ICBlock.ICCardNO!=0)
 {
  WORDtoASC(TempStr1+2,ApplVar.ICBlockNumber+1);
  TempStr1[3]=0;
  strcat(TempStr1,":    ");
  TempBCD=ZERO;
  ULongToBCDValue(TempBCD.Value,ApplVar.ICBlock.ICCardNO);
  strcpy(TempStr,"        ");
  strcat(TempStr1,FormatQtyStr(TempStr,&TempBCD,8));
  RJPrint(0,TempStr1);
 }
}
}

//-----------------------------------------------------
BYTE QueryICBlock(unsigned long pICCardNO)
{
	BYTE IsBlock = 0;

	for(ApplVar.ICBlockNumber=0;ApplVar.ICBlockNumber<ApplVar.AP.ICBlock.Number;ApplVar.ICBlockNumber++)
	{
		 ReadICBlock();
		 if (ApplVar.ICBlock.ICCardNO == pICCardNO)
		 {
		  IsBlock = 1;
		  break;
		 }
	}
	return IsBlock;
}
