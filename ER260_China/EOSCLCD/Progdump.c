#define PROGDUMP_C
#include "king.h"				/* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"




void ProgramDump()
{
    BYTE s_type;
    WORD s_number;

    if (Appl_ProgType == SETKEYB && Appl_ProgType == SETKEYMASK)  /* don't print keyboard */
        return;
    if (!Appl_ProgStart) /* dump all */
    {
        Appl_ProgNumber = 0;
        ApplVar.NumberEntry = 0xffff;
    }
    Appl_ProgLine = 1;
    ApplVar.PrintLayOut = 0x02;
    Appl_ProgStart = 2;      /* indicate program dump */
    Appl_EntryCounter = 0;
    s_number = Appl_ProgNumber;
    s_type = Appl_ProgType;
    if (ApplVar.NumberEntry)
        ApplVar.NumberEntry--;
    else
        ApplVar.NumberEntry = Appl_ProgNumber;
    while(Appl_ProgNumber <= ApplVar.NumberEntry)
    {
        if (DisplayOption() && !ApplVar.ErrorNumber)
        {
            if (Appl_ProgNumber > ApplVar.NumberEntry)
                break;
            if (Appl_ProgLine > 2)
            {
                MemSet(SysBuf, 6, ' ');
                memcpy(SysBuf + 6, ProgLineMes, sizeof(ProgLineMes));
                PrintStr(SysBuf);
            }
            else
            {
                PrintStr(ProgLineMes);
                if (Appl_ProgType == SETPLU)      /* PLU ? */
                {
                    if (Appl_ProgLine == 1 && ApplVar.AP.Plu.RandomSize) /* random number ?*/
                    {
                        SysBuf[0] = 'R';
                        SysBuf[1] = ':';
                        HEXtoASC(SysBuf+2, ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
                        SysBuf[ApplVar.AP.Plu.RandomSize * 2 + 2] = 0;
                        PrintStr(SysBuf);
                    }
                }
            }
        }
        else
            break;
        if (Appl_ProgType == SETSYSFLAG)
            Appl_ProgNumber++;
        else
            Appl_ProgLine++;
        if (KbHit() && Getch() == ApplVar.AP.FirmKeys[ID_CLEAR])
            break;      /* clear key means break */
    }
	if(TESTBIT(ARROWS, BIT0))        /* receipt on ?*/
	    RFeed(2);
    Appl_ProgStart = 1;
    Appl_ProgNumber = s_number;
    Appl_ProgType = s_type;
    Appl_ProgLine = 1;
    Appl_BitNumber = 0;
    ApplVar.ErrorNumber=0;
    DisplayOption();
//ccr2016-03-01    PutsO(ProgLineMes);
}

