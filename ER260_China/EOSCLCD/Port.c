#define PORT_C 4
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

/***************************************************************/

void WritePort()
{
    if (Appl_PortNumber < ApplVar.AP.Port.Number)
    {
		ApplVar.PortList[Appl_PortNumber] = Appl_Port;
        SetComm(Appl_PortNumber) ;
    }
}

void ReadPort()
{
    if (Appl_PortNumber < ApplVar.AP.Port.Number)
		Appl_Port = ApplVar.PortList[Appl_PortNumber];
}

BYTE EnCodeProto(char *pro)
{
	 return((pro[0] - 0x31) |
 		   ((pro[1] - 0x30)<<3) |
 		   ((pro[2] - 0x31)<<5) |
 		   ((pro[3] - 0x37)<<6));
}


void DeCodeProto(BYTE In, char *out)
{
    CopyFrStr(out,PortRate[In&0x07]);//bit0..bit2:Speed
    out[6]=',';
    out[7]=((In>>6)&1) + 0x37;//bit6:Data length
    out[8]=',';
    out[9]=((In>>5)&1) + 0x31;//bit5:Stop bit
    out[10]=',';
    out[11]=((In>>3)&0x03)+ 0x30 ;	//bit3..bit4:Parity
}

