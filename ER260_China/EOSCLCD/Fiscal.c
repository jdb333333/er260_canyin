#define FISCAL_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

/***************************************************************/

void AddReceiptNumber()
{
	BCD temp;
	short i;

    if (!ApplVar.FInv && ApplVar.FCanc != 2 && !ApplVar.FProforma)
    {
		if(ApplVar.FTrain)
			i = RECNUM;
		else
			i = 0;
		temp = ZERO;
		memcpy(temp.Value, ApplVar.ReceiptNumber+i, 3);
		Add(&temp, (BCD*)&ONE);
		memcpy(ApplVar.ReceiptNumber+i, temp.Value, 3);
    }
}
//Get the index of clerk and convert it into ascii string for print.
void GetReceiptNumber(char *to)
{
	BYTE l;

	{
	    if ((CHARSET & 0xf0) == 0x10 || (SLIP != 4 && ((CHARSET & 0x0f) == 3 || (CHARSET & 0x0f) == 7)))  /* number sign not available */
			*to = 'N';
	    else
	    	*to = '#';
		if (TESTBIT(KPPRICE, BIT7))
			l = 1;
		else
			l = 3;
		if (ApplVar.FTrain)
		    HEXtoASC(to + 1, ApplVar.ReceiptNumber+RECNUM, l);
		else
		    HEXtoASC(to + 1, ApplVar.ReceiptNumber, l);
	}
}

