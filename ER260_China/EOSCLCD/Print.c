#define PRINT_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

#define LQ300K  PRTLEN

CONST char SlipCmd1[8][5] = {
    { 4, 27, 64, 0x1c, 0x26},            /* init printer command ESC '@', select chinese */
    { 2, 27, 118},           /* ask status byte from printer ESC 'v' */
    { 3, 27, 33, 0x20},        /* set double width with 1 space advance ' ' ESC '!' 0x20*/
    { 3, 27, 33, 0x00},        /* reset double width  ESC '!' 0x00 */
    { 3, 12, 27, 113},          /* eject and release FF ESC 'q' */
    { 3, 27, 70, 0},           /* set normal feed ESC 'F' 0 */
    { 3, 27, 70, 1},           /* set reverse feed ESC 'F' 1 */
    { 2, 27, 100},           /* fast feed paper N lines ESC 'd' n*/
};

CONST char SlipCmd2[8][5] = {
    { 2, 27, 64},            /* init printer command ESC '@' */
    { 2, 27, 118},           /* ask status byte from printer ESC 'v' */
    { 3, 27, 33, 0x20},        /* set double width with 1 space advance ' ' ESC '!' 0x20*/
    { 3, 27, 33, 0x00},        /* reset double width  ESC '!' 0x00 */
    { 3, 12, 27, 113},          /* eject and release FF ESC 'q' */
    { 3, 27, 70, 0},           /* set normal feed ESC 'F' 0 */
    { 3, 27, 70, 1},           /* set reverse feed ESC 'F' 1 */
    { 2, 27, 100},           /* fast feed paper N lines ESC 'd' n*/
};

CONST char SlipCmd3[8][5] = {
    { 0},            /* init printer command ESC '@' */
    { 3, 28, 33,12},           /* ask status byte from printer ESC 'v' 1 */
    { 3, 28, 33, 8},        /* set double width with 1 space advance ' ' ESC '!' 0x20*/
    { 3, 28, 33, 0},        /* reset double width  ESC '!' 0x00 */
    { 1, 12},          /* eject and release FF ESC 'q' */
    { 3, 27, 87, 1},           /* set double high ESC 'W' 1*/
    { 3, 27, 87, 0},           /* set double high ESC 'W' 1*/
    { 0},           /* fast feed paper N lines ESC 'd' n*/
};

CONST char SlipCmd4[8][5] = {
    { 5, 27, 64},   /* init printer command ESC '@'and 40 col  */
    { 2, 27, 118},           /* ask status byte from printer ESC 'v' */
    { 4, 27, 33, 0x21},        /* set double width with 1 space advance ' ' ESC '!' 0x20*/
    { 3, 27, 33, 0x01},        /* reset double width  ESC '!' 0x00 */
    { 3, 12, 27, 113},          /* eject and release FF ESC 'q' */
    { 3, 27, 70, 0},           /* set normal feed ESC 'F' 0 */
    { 3, 27, 70, 1},           /* set reverse feed ESC 'F' 1 */
    { 2, 27, 100},           /* fast feed paper N lines ESC 'd' n*/
};

CONST char SlipCmd5[8][5] = {            /* Sprint */
    { 2, 27, 103},   /* init printer font 2, 32 col  */
    { 0},           /* ask status byte from printer ESC 'v' */
    { 3, 27, 87, 49, 0},        /* set double width ESC 'W' '1'*/
    { 3, 27, 87, 48, 0},        /* reset double width  ESC 'W' '0' */
    { 1, 12},          /* eject and release FF */
    { 0},           /* set normal feed ESC 'F' 0 */
    { 0},           /* set reverse feed ESC 'F' 1 */
    { 0},           /* fast feed paper N lines ESC 'd' n*/
};

CONST BYTE CharTable[48] = {
    0x5c,0x7e,0x60,0x61,0x7b,0x40,0x7d,0x5c,
    0x65,0x65,0x7d,0x69,0x69,0x7e,0x5b,0x5d,
    0x40,0x7b,0x5b,0x6f,0x7c,0x7c,0x75,0x7c,
    0x79,0x5c,0x5e,0x7c,0x23,0x5c,0x23,0x66,
    0x61,0x69,0x6f,0x75,0x7c,0x5c,0x61,0x6f,
    0x5d,0x20,0x20,0x20,0x20,0x5b,0x3c,0x3e,
};

void PrintRJ(CONSTCHAR *str);
void PrintAllons(void);



/* check character for external printer */
BYTE CheckChar(BYTE ch)
{
    BYTE chset;

    chset = CHARSET & 0x0f;
    if (chset) /* not standard */
    {
        switch (ch)
        {
        case 0x15:
            if (chset == 1)
                ch = 0x5d;
            else
                ch = 0x40;
            break;
        case 0x81:
            if (chset == 2)
                ch = 0x7d;
            else
                ch = 0x7e;
            break;
        case 0x82:
            if (chset == 1)
                ch = 0x7b;
            else if (chset == 6)
                ch = 0x5d;
            else
                ch = 0x60;
            break;
        case 0x85:
            if (chset == 6)
                ch = 0x7b;
            else
                ch = 0x40;
            break;
        case 0x97:
            if (chset == 6)
                ch = 0x60;
            else
                ch = 0x7c;
            break;
        case 0x9a:
            if (chset == 2)
                ch = 0x5d;
            else
                ch = 0x5e;
            break;
        case 0xcf:
            ch = 0x24;
            break;
        case 0xe1:
            ch = 0x7e;
            break;
        case 0xf8:
            ch = 0x5b;
            break;
        default:
            if (ch > 0x7f && ch < 0xb0)
                ch = CharTable[ch & 0x7f];
            break;
        }
    }
    return ch;
}


void CmdSlip(char cmd)
{
    CONSTCHAR *p;
    BYTE l;

    if (SLIP == 1)
        p = (char*)SlipCmd1[cmd];
    else if (SLIP == 2)
        p = (char*)SlipCmd2[cmd];
    else if (SLIP == 5)     /* citizen printer */
        p = (char*)SlipCmd4[cmd];
    else if (SLIP == 6)     /* DITRON Sprint */
        p = (char*)SlipCmd5[cmd];
    else
        p = (char*)SlipCmd3[cmd];

    l = *p;        /* length of data string */
    p++;
    while (l)
    {
        SendComm(SLIPPORT_1, *p);
        p++;
        l--;
    }
}

void ReleaseSlip()
{
    if (SLIP != 3)   // LYQ ADDED FOR ESP\C 20040324
        CmdSlip(6);     /* set reverse line feed */
    CmdSlip(4);
    if (SLIP != 3)   // LYQ ADDED FOR ESP\C 20040324
        CmdSlip(5);     /* set normal line feed */
}


BYTE CheckSlip()
{
    ApplVar.SlipLines = 0;
    if (!SLIP)
        return 0;
    if (!TESTBIT(ApplVar.PrintLayOut, BIT3)) /* print on slip ? */
        return 0;

    if (!CheckOnLine(SLIPPORT_1))
    {
        if (ApplVar.SlipPage || TESTBIT(ApplVar.PrintLayOut, BIT4)) /* slip compulsory */
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI38);   /* not on line */
            return 1;
        }
        else
            return 0;
    }
/*       while(Check232(SLIPPORT_1)); */
    if (SLIP < 3 || SLIP == 5)       /* only CTM-290 & 290 II */
    {
        CmdSlip(0);     /* init printer */
        if (SLIP == 5)      /* also set columns */
            CmdSlip(3);
        CmdSlip(1);     /* ask status */
        ApplVar.SlipLines = ReadComm(SLIPPORT_1);
        if (ApplVar.SlipLines & (BIT0 + BIT1))
        {
            ApplVar.SlipLines = 0;
            if ((ApplVar.SlipPage && TESTBIT(ApplVar.PrintLayOut, BIT3))
                || TESTBIT(ApplVar.PrintLayOut, BIT4))
            {

                ApplVar.ErrorNumber=ERROR_ID(CWXXI35);
                return 1;
            }
            else
                return 0;
        }
    }
    if (SLIP != 4 && SLIP != 6)       /* Epson compatible ? then select char set */
    {
        SendComm(SLIPPORT_1, 0x1b);
        SendComm(SLIPPORT_1, 'R');
        SendComm(SLIPPORT_1, CHARSET & 0x0f);
        if (SLIP == 3)
        {
            SendComm(SLIPPORT_1, 0x1c);
            SendComm(SLIPPORT_1, '&');
        }
    }
    ApplVar.SlipLines = 1;
    return 0;
}

void FeedSlip(BYTE lines)

{
    if (SLIP > 2 && SLIP != 5)
    {
        while (lines)
        {
            SendComm(SLIPPORT_1, LF);        /* line feed */
            lines--;
        }
    }
    else
    {
        CmdSlip(7);
        SendComm(SLIPPORT_1, lines);
    }
}

void SlipMargin()
{
    BYTE l;

    l = LMARGE;     /* left margin */
    while (l)
    {
        SendComm(SLIPPORT_1, ' ');
        l--;
    }
}

void PrintSlip(CONSTCHAR *str)
{
#if !defined(DEBUGBYPC)

    BYTE l, d ,r = 0;

    l = LQ300K ;  /* 31; */
    if (SLIP == 6)
    {
        CmdSlip(0);                        /* set font 2 */
        l = 32;
        if (SLIP == 6)             /* sprint */
        {
            SendComm(SLIPPORT_1, 27);
            SendComm(SLIPPORT_1, 65);
            SendComm(SLIPPORT_1, CHARSET & 0x0f);
        }
    }
    d = 0;
    while (*str == ' ')
    {
        r++;
        if (r<LQ300K+1)
            str++;
        else
            break;

    }
    r = 0;
    while (l && *str)
    {
        if (*str == '~'&&SLIP == 3)        /* double size char lyq modified for double high*/
        {
            CmdSlip(5);
            d = 1;              /* set double width */
            r = 0;
        }
        else if (*str == '@')
        {
            if (SLIP == 3 && d == 1)
                CmdSlip(1);
            else
                CmdSlip(2);
            d = 2;
            r = 0;
        }
        else
        {
            if (SLIP != 4)     /* Epson Compatible */
                SendComm(SLIPPORT_1, CheckChar(*str));
            else
                SendComm(SLIPPORT_1, *str);
            if (ApplVar.SlipDouble==0 && d )
            {
                if (*str < 0x81 || r==1)
                {
                    if (SLIP != 3 && d==2)
                        CmdSlip(3);                /* reset double high */
                    else
                        CmdSlip(6);
                }
            }
            r = 1;
        }
        str++;
        l--;
    }
    if (d==2)
    {
        ApplVar.SlipLines ++ ;
//    	CmdSlip(3);                /* reset double high */
//    	d = 0;
    }
    SendComm(SLIPPORT_1, CRET);
    SendComm(SLIPPORT_1, LF);
#endif
}


void PrintSlipPage(short sub)
{
    char line[LQ300K+1], l;   /* 36 */
    BYTE temp1,temp2,temp3;

    MemSet(line, sizeof(line), ' ');
    if (SLIP == 2)
        l = 24;
    else
        if (SLIP == 6)
        l = 32;
    else
        l = PRTLEN ; /* 35; */
    line[l] = 0;
    l--;
    CopyFrStr(line, Msg[YEHAO].str);
    WORDtoASC(line + 8, ApplVar.SlipPage);
    if (sub && (ApplVar.FRegi || ApplVar.FTend || ApplVar.BufRec))
    {
        if (SLIP != 2)
            CopyFrStr(line + 11, Msg[XIAOJI].str);
        temp1 = ApplVar.AmtDecimal; /* save */
        temp2 = Prefix1;
        temp3 = Prefix2;
        ApplVar.AmtDecimal = NO_DECIMAL; /* restore */
        Prefix1 = PREFIX_1;
        Prefix2 = PREFIX_2;
        if (ApplVar.FRegi)
            FormatAmt(line + l, &ApplVar.SaleAmt);
        else
            FormatAmt(line + l, &ApplVar.SubTotal);
        ApplVar.AmtDecimal = temp1; /* restore */
        Prefix1 = temp2;
        Prefix2 = temp3;
    }
    SlipMargin();
//    PrintSlip(line);	  // delete for
}

/* ApplVar.PrintLayOut contains the print construction */
/* BIT 0 if set journal */
/* BIT 1 if set receipt */
/* BIT 2 if set DOUBLE HEIGHT (XT thermal) */
/* BIT 3 if set then print on slip, if set in command to xt printer then */
/* no inter line spacing so skip bit */

void PrintRJ(CONSTCHAR *str)
{
    BYTE    print;
#if !defined(DEBUGBYPC)
    if (ApplVar.FReport || Appl_ProgStart == 2)        /* report or dump in progress? */
    {
#if !defined(CASE_INNERDOTPRN)
        if (!ApplVar.RepComputer)
        {
            if (Get_Printer_status().g_bNOPAPER)          /* out of paper ? */
            {
                CheckError(39);
                while (Get_Printer_status().g_bNOPAPER) ;
            }
        }
#endif
        CheckBreakKey();
    }
#endif
    print = ApplVar.PrintLayOut & 0x07;

    if (TESTBIT(ARROWS, BIT0) || ApplVar.FReport)        /* receipt on or report ? */
    {
        if (TESTBIT(ApplVar.PrintLayOut, BIT1))     /* print on receipt ? */
            ApplVar.FReceipt = 1;
    }
    else
        RESETBIT(print, BIT1);
    if (TESTBIT(ApplVar.PrintLayOut, BIT0))     /* print on journal ? */
        ApplVar.FJournal = 1;   /* set journal used */

    if (TESTBIT(print, BIT0 + BIT1))    /* still print ? */
    {
        if ( !RJPRN )
            print &= 0xfe ;
        else if ( RJPRN &&( ApplVar.CentralLock == RG || ApplVar.CentralLock == MG ) )
            print &= 0xfd ;
        RJPrint(print,str) ;
    }
}

//清除字符串str中的~(倍宽标示符),由SysBuf送出清除~后的字符串//
CONSTCHAR *ClearFlashChar(char *str,short w)
{
    short   i=0;

    if (str)
    {
        for (;i<w && str[i] ;i++)
        {
            if (str[i]=='~')
                SysBuf[i]=' ';
            else
                SysBuf[i]=str[i];
        }
    }
    SysBuf[i]=0;
    return SysBuf;
}

/*****************************************************************************
 * 打印销售项目
 *
 * @author EutronSoftware (2016-01-26)
 *
 * @param qty
 * @param str
 * @param amt
 * @param disp:是否在屏幕上显示//ccr2016-03-04
 *
 * @return BYTE :在屏幕上显示的行数
 *****************************************************************************/
BYTE PrintQtyAmt(BCD *qty,CONSTCHAR *str,BCD *amt,BYTE disp)
{
    BYTE l, sliplen,rlen;
    BYTE lnDisp=0;//ccr2016-01-26

    if (ApplVar.FCanc)
        ApplVar.PrintLayOut &= ApplVar.CancPrint;
    if (ApplVar.FTrain || ApplVar.FProforma == 1)
        ApplVar.PrintLayOut &= 0xfe;    /* skip journal */

    if (amt)
    {/* 含有金额数据 */
        if (TESTBIT(ApplVar.PrintLayOut, BIT5))  // amount on seperate line ?
        {/* 金额数据独立一行 */
            if (str)
            {
                PrintRJ(str);
                if (disp)
                {
                    DispStrLF(ClearFlashChar((char*)str,SCREENWD)); lnDisp++;
                }
            }
            PrintRJ(FormatAmtStr((char*)0, amt, PRTLEN));

            if (disp)
            {
                DispStrLF(FormatAmtStr(0,amt,SCREENWD));lnDisp++ ;
            }
        }
        else
        {
            if (qty)
            {
                if (str)
                {
                    if (ApplVar.FRegi && disp)
                    {
                        //ccr2015-09-06>>>>>>>>>>>>>>>>>>>>>
                        l = 1;//标示显示了项目名称
                        if (ApplVar.sCursor.y>(SCREENLN-2))
                            GotoXY(0,0);
                        else if (ApplVar.sCursor.y==(SCREENLN-2))
                            ScreenFeed(1);
                        //ccr2015-09-06<<<<<<<<<<<<<<<<<<
                        DispStrLF(ClearFlashChar((char*)str,SCREENWD)); lnDisp++;/* 显示名称 */
                    }
                    else
                        l=0;//ccr2015-09-06标示没有显示项目名称
                    rlen = strlen(str);
                    while (*(str+rlen -1)==' ' && rlen)
                        rlen--;
                }
                else
                    rlen = 0;

                if (ApplVar.FRegi && disp)
                {
                    DispStrLF(FormatQtyStr(Msg[SHULIANG].str,qty,SCREENWD)); lnDisp++;/* 显示数量*/
                    //ccr2015-09-06>>>>>>>>>>>>>>>>>>
                    if (l && ApplVar.sCursor.y==(SCREENLN-2))//显示了项目名称
                        DispStrXY(FormatAmtStr(Msg[JINE].str,amt,SCREENWD),0,0xff);/* 显示金额后不滚动屏幕 */
                    else
                        DispStrLF(FormatAmtStr(Msg[JINE].str,amt,SCREENWD));/* 显示金额 */
                     lnDisp++;
                    //ccr2015-09-06<<<<<<<<<<<<<<<<<<
                }

//                MemSet(SysBuf, sizeof(SysBuf), ' ');
                //ccr2016-08-03>>>>>>>>>>>>>>>>>>>>>>>>
                l = BCDWidth(&ApplVar.Price);
                sliplen=BCDWidth(amt);
                if (Prefix1) {l++;sliplen++;}
                if (Prefix2) {l++;sliplen++;}
                //ccr2016-08-03<<<<<<<<<<<<<<<<<<<<<<<<
                if (l>6+pSHIFTx || sliplen>6+pSHIFTx || ((rlen+BCDWidth(qty))>13+(pSHIFTx>>1)))
                {
                    PrintRJ(str);
                    MemSet(SysBuf, sizeof(SysBuf), ' ');
                    FormatQty(SysBuf + 8+pSHIFTx, qty);
                    FormatAmt(SysBuf + 19+pSHIFTx, &ApplVar.Price);
                    FormatAmt(SysBuf +PRTLEN - 1, amt);
                    PrintRJ(SysBuf);
                }
/*ccr2015-08-21 else if ((rlen+BCDWidth(qty))>13)
                {
                    PrintRJ(str);
                    MemSet(SysBuf, sizeof(SysBuf), ' ');
                    FormatAmt(SysBuf +PRTLEN - 1, amt);
                    FormatAmt(SysBuf + 20+(PRTLEN>32)*2, &ApplVar.Price);
                    FormatQty(SysBuf + 7+(PRTLEN>32)*2, qty);
                    PrintRJ(SysBuf);
                }*/
                else
                    PrintRJ(FormatStrQtyPriAmt(str, qty, &ApplVar.Price, amt, PRTLEN));
            }
            else
            {
                if (ApplVar.FRegi && disp)
                {
                    DispStrLF(FormatAmtStr(str,amt,SCREENWD));/* 显示金额 */
                     lnDisp++;
                }
                PrintRJ(FormatAmtStr(str, amt, PRTLEN));
            }
        }
    }
    else if (qty)
    {
        if (ApplVar.FRegi && disp)
        {
            DispStrLF(ClearFlashChar(FormatQtyStr(str, qty, SCREENWD),SCREENWD));
            lnDisp++;
        }
        PrintRJ(FormatQtyStr(str, qty, PRTLEN));
    }
    else
        PrintRJ(str);
    if (ApplVar.SlipLines && TESTBIT(ApplVar.PrintLayOut, BIT3) && ApplVar.FRegi && SLIP)     /* print on slip ? */
    {
        if (SLIP_MAX)     /* max lines on slip ? */
        {
            if ((ApplVar.SlipLines >= SLIP_MAX) || (SLIP == 6 && ApplVar.SlipLines >= 26))
            {
                ApplVar.SlipPage++;
                PrintSlipPage(1);
                ReleaseSlip();
                if (SLIP == 6)
                {
                    ApplVar.SlipLines = 1;
                }
                else
                {
                    DispStrLF(Msg[GHDYZHI].str); lnDisp++;
                    Bell(3);
                    for (;;)
                    {
                        if (KbHit()&&ApplVar.AP.KeyTable[Getch()] == CLEAR)
                        {
                            l = CheckSlip();  /* clear key and paper */
                            if (!l || ApplVar.ErrorNumber == 38)
                            {
                                if (ApplVar.ErrorNumber == 38)
                                    return lnDisp;
                                ApplVar.ErrorNumber=0;
                                break;
                            }
                        }

                    }
                }
            }
        }
        if (ApplVar.SlipLines == 1)
        {
            if (SLIP_TOP)     /* start of slip and feed ? */
                FeedSlip(SLIP_TOP);
            ApplVar.SlipLines = 1;
            if (!ApplVar.SlipPage &&
                ((ApplVar.Correc.Options & 0x07)!=(xRECEIPT-CORREC-1)) &&
                ((ApplVar.Correc.Options & 0x07)!=(xSUSPEND-CORREC-1)))     //20040325
            {
                for (l = 0; l < 6; l++)
                {
                    if (ApplVar.TXT.SlipHeader[l][0])
                    {
                        SlipMargin();
                        //MemSet(SysBuf, 41, ' ');
                        //memcpy(SysBuf, ApplVar.TXT.SlipHeader[l],PRTLEN);	   20040325
                        PrintSlip(ApplVar.TXT.SlipHeader[l]);
                        ApplVar.SlipLines++;
                    }
                    else
                        break;
                }
            }
            if (ApplVar.SlipPage)
            {
                PrintSlipPage(1);
                ApplVar.SlipLines++;
            }
        }
        SlipMargin();
        if (SLIP != 2)             /* Chinese with TM-290II */
        {
            if (ApplVar.SlipDouble)    /* Double size ? */
            {
                if (SLIP !=  4)
                {
                    CmdSlip(2);                /* set double width */
                }
                l = LQ300K ; /* 31; */
                if (SLIP == 6)
                    l--;
            }
            else
            {
                if (TESTBIT(SLIPINFO, BIT0) && amt && qty)
                    l = LQ300K ;  /* 31; */
                else
                    l = LQ300K ;  /* 31; */

                if (SLIP == 6)
                    l -= 3;
            }
        }
        if (!amt && !qty && str)
            PrintSlip(str);
        else
        {
            if (SLIP == 2)
            {
                if (amt)
                {
                    if (TESTBIT(ApplVar.PrintLayOut, BIT5)) /* amount on seperate line ? */
                    {
                        PrintSlip(str);
                        ApplVar.SlipLines++;
                        PrintSlip(FormatStrQtyPriAmt((char*)0, qty, &ApplVar.Price, amt, PRTLEN));

                    }
                    else
                    {
                        PrintSlip(FormatStrQtyPriAmt(str, qty, &ApplVar.Price, amt, PRTLEN));
                    }
                }
                else if (qty)
                    PrintSlip(FormatQtyStr(str, qty, PRTLEN));
                else
                    PrintSlip(str);
            }
            else
            {
                if (SLIP == 6)
                    sliplen = 26;
                else
                    sliplen = 25;
                if (amt)
                {
                    FormatAmtStr(str, amt, l);
                    if (qty /*&& l == sliplen*/)
                        FormatStrQtyPriAmt(str, qty, &ApplVar.Price, amt, PRTLEN);//????????????????????????
                }
                else
                    FormatQtyStr(str, qty, l);      /* not print if > 999 or with decimal */
                if (l == sliplen)
                    l += 6;
                if (SLIP == 4 && l == 17)               /* double size */
                {
                    SysBuf[sizeof(SysBuf) - 36] = ' ';//????????????????????????
                    for (l = 0; l < 17; l++)
                    {
                        SysBuf[sizeof(SysBuf) + l * 2] = ' ';//????????????????????????
                        SysBuf[sizeof(SysBuf) + l * 2] = SysBuf[sizeof(SysBuf) + l];//????????????????????????
                    }
                    PrintSlip(SysBuf);//????????????????????????
                }
                else
                    PrintSlip(SysBuf);//????????????????????????
            }
        }
        if (ApplVar.SlipDouble && SLIP == 3)
            ApplVar.SlipLines ++ ;
        ApplVar.SlipLines++;
        if (SLIP != 4)
            CmdSlip(3);        /* set normal width */
    }
    ApplVar.SlipDouble = 0;
    return  lnDisp;
}


BYTE PrintStr(CONSTCHAR *str)
{
    return(PrintQtyAmt(0, str, 0,true));
}

BYTE PrintLine(char c)          /* print line of characters */
{
    MemSet(SysBuf, PRTLEN, c);
    SysBuf[PRTLEN]=0;
    return(PrintStr(SysBuf));
}

BYTE PrintAmt(CONSTCHAR *str,BCD *amt)
{
    return(PrintQtyAmt(0, str, amt,true));
}

BYTE PrintQty(CONSTCHAR *str,BCD *qty)
{
    return(PrintQtyAmt(qty, str, 0,true));
}
//
/**
 * 预打印票头,预打印的行数由PREHEADER指定
 *
 * @author EutronSoftware (2016-11-10)
 *
 * @param line
 */
void PrePrintHead(short line)
{
    BYTE saveprint;
    int i;

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    if (TestSaveEJ())
        StoreEJStart();
#endif
    saveprint = ApplVar.PrintLayOut;

    if (TESTBIT(AM_PM, BIT2))//票头为倍高打印方式
        ApplVar.PrintLayOut = 0x06;
    else
        ApplVar.PrintLayOut = 0x02;

    {
        CutExtPrinter();        //ccr2017-02-07外置打印机切纸>>>>>>>
        for (i=0;i<line; i++)
        {
            if (ApplVar.TXT.Header[i][0])
                PrintStr_Center(ApplVar.TXT.Header[i],DBLHIPRN & BIT3);
            else
                RFeed(1);//ccr2016-11-11
        }
    }
    ApplVar.PrintLayOut = saveprint;
    RESETBIT(ApplVar.MyFlags,PRNHEADMUST);
}
/**
 * 打印票头剩余行,即:打印PrePrintHead未打印的行
 *
 * @author EutronSoftware (2016-11-10)
 *
 * @param line
 */
void PrintHead1(short line)
{
    BYTE saveprint;

    if (line<PREHEADER)
        RESETBIT(ApplVar.MyFlags,PRNHEADMUST);

    saveprint = ApplVar.PrintLayOut;

    if (TESTBIT(AM_PM, BIT2))//票头为倍高打印方式
        ApplVar.PrintLayOut = 0x06;
    else
        ApplVar.PrintLayOut = 0x02;
    //if( RJPRN )
    //ApplVar.PrintLayOut = 0 ;    ----0905 Zengy Logout
    {
        for (; line < 8; line++)
        {
            if (ApplVar.TXT.Header[line][0])
                PrintStr_Center(ApplVar.TXT.Header[line],DBLHIPRN & BIT3);
            else
                break;
        }
    }
    ApplVar.PrintLayOut = saveprint;
}

void PrintRegiInfo(BYTE prnGrp)
{
    BYTE saveprint;
    BYTE flags;//ccr20130923 用来表示是否已经打印收据号
    WORD i,sP;
    struct TimeDate pTimeDate;

    if (ApplVar.FTrain)
        PrintStr_Center((char*)Msg[PXZHTAI].str,true);        /* print training */
    if (ApplVar.FProforma)
        PrintStr_Center((char*)Msg[XSHFPIAO].str,true);   /* print proforma */

    memset(SysBuf,' ', PRTLEN);
    flags = 0;
    sP = 0;
    if (!TESTBIT(COMMA, BIT5))         /* print CLERK name? */
    {
//012345678901234567890123456789012345
/*ApplVar.Clerk :********    Saler :********    */
        strcpy(SysBuf+sP,Msg[SHKYUANF].str);
        sP += strlen(Msg[SHKYUANF].str);
        SysBuf[sP++]=' ';
        i = strlen(ApplVar.Clerk.Name);
        //if (i>8)  i = 8;
        memcpy(SysBuf + sP, ApplVar.Clerk.Name,i);

        sP = (PRTLEN<36)?17:19;
#if (salNumber)
        if (ApplVar.SalPerNumber && TESTBIT(PLU_PRINT,BIT4))//lyq adde 2003\10\23
        {
            strcpy(SysBuf+sP,Msg[YYYUANF].str);
            sP += strlen(Msg[YYYUANF].str);
            SysBuf[sP]=' ';
            i = strlen(ApplVar.SalPer.Name);
            if (i>8)
                i = 8;
            memcpy(SysBuf + sP, ApplVar.SalPer.Name,i);
            SysBuf[PRTLEN]=0;
        }
        else
#endif
        if (!TESTBIT(COMMA, BIT3))         /* print receipt number ? */
        {
            GetReceiptNumber(SysBuf +  PRTLEN -7);
            flags = 1;//收据号与收款员名称打印在同一行
        }
        PrintStr(SysBuf);
    }

    memset(SysBuf,' ', PRTLEN);
    sP = 0;
    if (TESTBIT(CLERKFIX, BIT3))         /* print CLERK number? */
    {
//012345678901234567890123456789012345
/*ApplVar.Clerk :********    Saler :********    */
        strcpy(SysBuf+sP,Msg[SHKYUANF].str);
        sP += strlen(Msg[SHKYUANF].str);
        SysBuf[sP]='#';
        WORDtoASC(SysBuf + sP + 2, ApplVar.ClerkNumber);
        sP = 17;
#if (salNumber)
        if (ApplVar.SalPerNumber && TESTBIT(PLU_PRINT,BIT4))//lyq adde 2003\10\23
        {
            strcpy(SysBuf+sP,Msg[YYYUANF].str);
            sP += strlen(Msg[YYYUANF].str);
            SysBuf[sP]='#';
            WORDtoASC(SysBuf + sP + 2, ApplVar.SalPerNumber);
            SysBuf[PRTLEN]=0;
        }
        else
#endif
        if (!TESTBIT(COMMA, BIT3) && !flags)         /* print receipt number ? */
        {
            GetReceiptNumber(SysBuf +  PRTLEN -7);
            flags = 1;//收据号与收款员编号打印在同一行
        }

        PrintStr(SysBuf);
    }

    sP = 0;
    if (!TESTBIT(COMMA, BIT4))         /* print LXX RXX ? */
    {
//012345678901234567890123456789012345
/*RegiterN: 9999        POSI: 9999   */
        memset(SysBuf,' ', PRTLEN);
        i = 0;
//ccr2017-09-05        if (REGISTER)
        {/* REGISTER number */
            sP = strlen(Msg[SHKJHAO].str);
            memcpy(SysBuf,Msg[SHKJHAO].str,sP);
            sP++;
            sP += WORDtoASCL(SysBuf + sP , REGISTER);
            i = sP;
        }
        if (LOCATION)
        { /* LOCATION number */
            i = strlen(Msg[WEIZHI].str);
            memcpy(SysBuf + sP, Msg[WEIZHI].str,i);
            sP += i;
            sP++;
            sP+=WORDtoASCL(SysBuf + sP, LOCATION);
        }
#if (!defined(CASE_EJSD) && !defined(CASE_EJFLASH))//有EJ功能时,此处不打印日期
        if (sP<(PRTLEN-23))//2016年12月12日 12:12:12
        {
            i = DateTimeToStr(SysBuf+60,false);//ccr20130923
            memcpy(SysBuf+PRTLEN-i,SysBuf+60,i);
            sP = 1;//日期已打印
        }
        else
        {
            sP=0;//日期未打印
        }
#endif
        if (i)
        {
            SysBuf[PRTLEN]=0;
            PrintStr(SysBuf);
        }
    }

    RESETBIT(ApplVar.PrintLayOut, BIT2);

    memset(SysBuf,' ', sizeof(SysBuf));
#if (!defined(CASE_EJSD) && !defined(CASE_EJFLASH))//有EJ功能时,此处不打印日期
    if (sP==0)
        i = DateTimeToStr(SysBuf,false);//ccr20130923
    else
#endif
        i = 0;
    if (!TESTBIT(COMMA, BIT3) && !flags)         /* 收据号未打印 print receipt number ? */
    {
        GetReceiptNumber(SysBuf +  PRTLEN -7);
        i = PRTLEN;//ccr20130923
    }
    //ccr20130923if (((SLIPINFO & (BIT1+BIT2)) != BIT1+BIT2) ||
        //ccr20130923(COMMA & (BIT3+BIT6+BIT7)) != (BIT3+BIT6+BIT7))  /* print something */
    if (i)//ccr20130923
    {
        if (TESTBIT(COMMA, BIT2))          /* double heigth ? */
            SETBIT(ApplVar.PrintLayOut, BIT2);
        SysBuf[PRTLEN] = 0;
//		saveprint = ApplVar.PrintLayOut;
//		ApplVar.PrintLayOut &= 0xfc;        /* skip R & J */
        PrintStr(SysBuf);
//		ApplVar.PrintLayOut = saveprint;        /* restore ApplVar.PrintLayOut */
        if (TESTBIT(SLIPINFO, BIT3) && (ApplVar.Key.Code>PORA + 100 && ApplVar.Key.Code < TEND + 100))     /* print on slip ? */ //lyq added for slip  start 20040324
        {
            SlipMargin();
            PrintSlip(SysBuf);
//			RESETBIT(ApplVar.MyFlags, ENSLIPPB);	 //lyq added for slip 20040324
        }                               //lyq added for slip end 20040324
        RESETBIT(ApplVar.PrintLayOut, BIT2);
    }

#if !defined(DEBUGBYPC)
#if !defined(CASE_INNERDOTPRN)
    if (prnGrp && !TESTBIT(ApplVar.MyFlags, PRNONPB+ENPRINTER))
    {//   打印票尾图形    //
        GetTimeDate(&pTimeDate);
        sP = EncordDate(pTimeDate.year,pTimeDate.month,pTimeDate.day);
        for (i=1;i<GRAFESTMAX+1;i++)
        {
            if (ApplVar.Graph[i].PictNo>0 && ApplVar.Graph[i].PictNo<=GRAPHMAX)
            {
                if (sP>=EncordDate(*((WORD *)(ApplVar.Graph[i].DateFrom+2)),ApplVar.Graph[i].DateFrom[1],ApplVar.Graph[i].DateFrom[0]) &&
                    sP<=EncordDate(*((WORD *)(ApplVar.Graph[i].DateTo+2)),ApplVar.Graph[i].DateTo[1],ApplVar.Graph[i].DateTo[0]))
                {
                    Bios(BiosCmd_PrintGraph, (void*)(ApplVar.Graph[i].PictNo), 1 , 0); //Stampa msg ram
                    break;
                }
            }
        }
        if (ApplVar.Graph[GRAFESTMAX+1].PictNo>0 && ApplVar.Graph[GRAFESTMAX+1].PictNo<=GRAPHMAX && i==GRAFESTMAX+1)
            Bios(BiosCmd_PrintGraph, (void*)(ApplVar.Graph[GRAFESTMAX+1].PictNo), 1 , 0); //Stampa msg ram
    }
#endif
#endif

#if (SCRSTATDBL)
    if (RGSuspend.SuspendItems.RGNumber)
    {
        ClearLine(STATELINE);
        DispOnStatDBL((char *)Msg[YOUGUADAN].str,0);//在状态行显示有挂单 //RG-DISPLAY
    }
    else
    {
        if (ApplVar.CentralLock==RG ||ApplVar.CentralLock==MG)
            DispOnStatDBL(EntryBuffer + ENTRYSIZE - SCRDBLWD - 1,0);
        DispOnStatDBL(ApplVar.ModeHead,0);
    }
#else
//ccr2016-03-03>>>以下注释的程序:结账后,保留显示小计金额
//ccr2016-03-03    ClsState2();  //RG-DISPLAY
//ccr2016-03-03    ClsState1();  //RG-DISPLAY
//ccr2016-03-03    mSetInvAttr();
//ccr2016-03-03    DispSt2(ApplVar.Clerk.Name); //RG-DISPLAY
//ccr2016-03-03    mClearInvAttr();
//ccr2016-03-03    DispSt0(ApplVar.ModeHead);  //RG-DISPLAY
    if (RGSuspend.SuspendItems.RGNumber)
    {
        DispSt0(ApplVar.ModeHead);  //RG-DISPLAY
        ClsState12();
        DispStrXY(Msg[YOUGUADAN].str,SCREENWD-6,STATELINE);//在状态行显示有挂单 //RG-DISPLAY
    }
    else
        StrForST0 = ApplVar.ModeHead;
#endif
    SETBIT(ApplVar.MyFlags,CLSSCREEN);
}

//预打印收据票头,预打印行数与倍高相关
void ReceiptIssue(short sliptimedate)
/*
    if bit 0 set then print time data on slip
    if bit 1 set then calculate bonus points
*/
{
    short i;
    BYTE saveprint;

    RESETBIT(ApplVar.IC.ICState,IC_DISCOUNT | IC_POINTS);//ccr chipcard

    Collect_Data(ENDLOG);
    Collect_Data(TENDLOG);
    if (ApplVar.BufKp != 2)
    {
        if (ApplVar.FJournal && !ApplVar.BufRec && (ApplVar.CentralLock == RG || ApplVar.CentralLock == MG))
            ApplVar.PrintLayOut = 0x01;
        if (ApplVar.SlipLines > 1 && sliptimedate)
            ApplVar.PrintLayOut |= 0x08;        /* set print slip */
        if (ApplVar.FReceipt)    /* Receipt Used ? */
        {
            ApplVar.PrintLayOut |= 0x02;
//ccr2017-09-05  CLERK 前不空行          RFeed(1);
        }
    }
    PrintRegiInfo(true);
    saveprint = ApplVar.PrintLayOut;
    if (ApplVar.FReceipt)    /* Receipt Used ? */
    {
        //if(RJPRN)
        //ApplVar.PrintLayOut = 0 ;    ----0905 Zengy Logout
        if (TESTBIT(AM_PM, BIT3))
            ApplVar.PrintLayOut = 0x06;        /* Double Height TRAILER */
        else
            ApplVar.PrintLayOut = 0x02;
        if (ApplVar.BufKp != 2)
        {
            if (ApplVar.TXT.Trailer[0][0])
            {
                RFeed(1);
                for (i = 0; i < 6; i++)
                {
                    if (ApplVar.TXT.Trailer[i][0])
                    {
                        PrintStr_Center(ApplVar.TXT.Trailer[i],DBLHIPRN & BIT4);
                    }
                    else
                        break;
                }
            }
            PrintPbTrailer();
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
//            if (TestSaveEJ())
//                StoreEJEnd();
#endif
        }
    }

    if (!TESTBIT(PLU_PRINT,BIT6) && ApplVar.FTrvoid==0)
    {
        ApplVar.PrintLayOut=saveprint;
        PrintAllons();
    }

    if (ApplVar.FReceipt)    /* Receipt Used ? */
    {
        //if(RJPRN)
        //ApplVar.PrintLayOut = 0 ;  ----0905 Zengy Logout
        if (TESTBIT(AM_PM, BIT2))//票头为倍高打印方式
            ApplVar.PrintLayOut = 0x06;        /* Double Heigth HEADER*/
        else
            ApplVar.PrintLayOut = 0x02;
        if (CUTTER == 1 || CUTTER == 2)
            RFeed(AFTERTRILER+1);
        else
            RFeed(AFTERTRILER);

        if (TESTBIT(KPPRICE, BIT4))
        {
            if (TESTBIT(AM_PM, BIT2) && CUTTER != 1 && CUTTER != 2)//票头为倍高打印方式
                RFeed(4);
            else
                RFeed(5);
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
            if (TestSaveEJ())
                StoreEJStart();
#endif
            if (ApplVar.TXT.Header[0][0])//预打印一行票头
                PrintStr_Center(ApplVar.TXT.Header[0],DBLHIPRN & BIT3);
            else
                RFeed(1);
        }
        else
        {
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
            if (TestSaveEJ())
                StoreEJStart();
#endif
            //预打印收据票头,预打印行数与倍高相关
            CutExtPrinter();        //ccr2017-02-07外置打印机切纸>>>>>>>
            //ccr2017-02-07<<<<<<<<<<<<<<<<<<<<<
            for (i = 0; i < PREHEADER; i++)
            {
                if (ApplVar.TXT.Header[i][0])
                    PrintStr_Center(ApplVar.TXT.Header[i],DBLHIPRN & BIT3);
                else
                    RFeed(1);

                if (CUTTER != 1 && CUTTER !=2)
                {
                    if (i > 4 || (TESTBIT(AM_PM, BIT2) && i > 1))//票头为倍高打印方式
                        break;
                }
            }
            RESETBIT(ApplVar.MyFlags,(PRNHEADMUST));
        }
        if (CUTTER)
        {
            if (CUTTER == 1 || CUTTER == 2)
            {
                //Delay(300);
                DrawerOpen();
            }
            else
                Delay(CUTTER * 100);
        }
    }

    ApplVar.PrintLayOut = saveprint;
    ApplVar.FNoTime = 0;
    if (ApplVar.BufKp == 2)
        return;
    if (ApplVar.SlipPage)
    {
        ApplVar.SlipPage++;
        if (!ApplVar.FPb)
            PrintSlipPage(0);           /* no Subtotal */
    }
    if (ApplVar.SlipLines > 1)
    {
        ReleaseSlip();

        ApplVar.Correc.Options = 0;
    }
    ApplVar.SlipPage = 0;
    ApplVar.PrintLayOut &= 0x01;
    /*PrintLine('+');*/
    ApplVar.FReceipt = 0;
    ApplVar.FJournal = 0;
}

void PrintAllons()
{

#define PRESSENTER 1	//   是否需要按确认键    //
#define PRNAMTON 	0	//  是否在小票上同时打印数量和金额 //
#define ALLPRNPOS	(6-PRNAMTON*2)  //   商品名称打印位置   //

#if (PRNAMTON)
    BCD sAmt;
#endif
    short i,j;
    BYTE sGroup;
    BYTE sLine;
    BYTE keyno = 0;
    WORD sFlags,sSavePBn;

//	if (ApplVar.RGItems.RGNumber<=0)
    for (i=0;i<ApplVar.RGItems.RGNumber;i++)
    {
        if (ApplVar.RGItems.RGBuf[i].Group!=0xff && (ApplVar.RGItems.RGBuf[i].Key.Code>PLU1 && ApplVar.RGItems.RGBuf[i].Key.Code<PLU3 ||
                                             ApplVar.RGItems.RGBuf[i].Key.Code>DEPT && ApplVar.RGItems.RGBuf[i].Key.Code<DEPT+5000))
            break;
    }
    if (i==ApplVar.RGItems.RGNumber)
        return;
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    RESETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
#endif

//ccr090323	if(ApplVar.AP.KeyTable[MAXKEYB-1]==0xffff && TESTBIT(ApplVar.MyFlags, PRNONPB) && (ApplVar.PbF.Options & 0x0f) < 3)
    if (TESTBIT(ApplVar.MyFlags, PRNONPB) && (ApplVar.PbF.Options & 0x0f) < 3)
    {
        RESETBIT(ApplVar.MyFlags, PRNONPB);
        keyno=1;
    }

    sSavePBn = ApplVar.PbNumber;
    sFlags = ApplVar.MyFlags;
    RESETBIT(ApplVar.MyFlags, ENPRINTER);

    RFeed(1);
    sLine = 0;
    do
    {
        sGroup = 0xff;
        for (i=0;i<ApplVar.RGItems.RGNumber;i++)
        {
//cc 20070723>>>>>>>>>>>>>>>
            if (i && ApplVar.RGRec.Key.Code > PLU1 &&
                TESTBIT(ApplVar.AP.Options.Plu, BIT0) && ApplVar.Plu.Link)
            /* ApplVar.Plu Link on previous item ? */
            {
                ApplVar.RGRec.Key.Code = GetLinkNumber();
                if (ApplVar.RGRec.Key.Code &&
                    ApplVar.RGRec.Key.Code != ApplVar.RGItems.RGBuf[i-1].Key.Code)  /* Random Code found ? */
                {
                    if (TESTBIT(DOT,BIT4) && sLine)
                    {
                        PrintLine('-');
                        PrintRegiInfo(FALSE);
                        RFeed(1);
                        PrintLine('-');
                        sLine = 0;
                    }
                    /*  ApplVar.Price Level is stored in ApplVar.RGRec.Amt.Value[BCDLEN-1] */
                    ApplVar.PluNumber = ApplVar.RGRec.Key.Code - PLU1 - 1;
                    ReadPlu();      /* get correct price */
                    MemSet(&ApplVar.RGRec.Amt, BCDLEN, 0);          /* Clear previous except Level*/
                    memcpy(ApplVar.RGRec.Amt.Value,
                           ApplVar.Plu.Price[ApplVar.RGRec.Amt.Value[BCDLEN-1]], ApplVar.AP.Plu.PriceSize);
                    ApplVar.DeptNumber = ApplVar.Plu.Dept;
                    ReadDept();

                    /*SysBuf[0] = ApplVar.DeptNumber;
                    SysBuf[1] = ApplVar.Dept.Group;
                    SysBuf[2] = sGroup;
                    SysBuf[3] = sLine;
                    PrnBuffer((char *)&SysBuf,4);*/
                    ApplVar.RGRec.Group = ApplVar.Dept.Group;
                    sGroup = 0xff;
                }
                else
                    ApplVar.RGRec = ApplVar.RGItems.RGBuf[i];
            }
            else
                ApplVar.RGRec = ApplVar.RGItems.RGBuf[i];

            if (ApplVar.RGRec.Key.Code > PBF && ApplVar.RGRec.Key.Code < (PBF + 100))   /* pb function */
            {
                ApplVar.PbFNumber = ApplVar.RGRec.Key.Code - PBF - 1;
                ReadPbF();
                switch (ApplVar.PbF.Options & 0x0f)
                {
                case 0:
                case 1:
                    ApplVar.PbNumber = *((WORD *) ApplVar.RGRec.Qty.Value);
                    break;
                }
            }


            if (sGroup==0xff && ApplVar.RGRec.Group!=0xff &&
                (ApplVar.RGRec.Key.Code>PLU1 && ApplVar.RGRec.Key.Code<PLU3 ||
                 ApplVar.RGRec.Key.Code>DEPT && ApplVar.RGRec.Key.Code<DEPT+5000)
               )
            {
                sGroup = ApplVar.RGRec.Group;
                ApplVar.GroupNumber = sGroup;
                ReadGroup();
                ApplVar.Group.Name[ApplVar.AP.Group.CapSize] = 0;
                if (!sLine)
                {
                    if (!keyno)
                    {
                        RFeed(4);
                        CutExtPrinter();        //ccr2017-02-07外置打印机切纸>>>>>>>

                        for (j = 0; j < PREHEADER; j++)
                        {
                            if (ApplVar.TXT.Header[j][0])
                                PrintStr_Center(ApplVar.TXT.Header[j],DBLHIPRN & BIT3);
                            else
                                break;
                        }
                        RFeed(PREHEADER-j);
                        RESETBIT(ApplVar.MyFlags,(PRNHEADMUST));
                    }

#if (PRESSENTER)
                    WaitForYesNo(Msg[AQRJIAN].str,true);
#endif
                    if (!keyno)
                    {
                        if (j>=PREHEADER)
                            for (j = PREHEADER; j < 8; j++)
                            {
                                if (ApplVar.TXT.Header[j][0])
                                    PrintStr_Center(ApplVar.TXT.Header[j],DBLHIPRN & BIT3);
                                else
                                    break;
                            }
                            RESETBIT(ApplVar.MyFlags,(PRNHEADMUST));
                    }
                    keyno = 0;
                    sLine = 1;
                }

                if (ApplVar.PbNumber)//ccr20110916 && ApplVar.AP.KeyTable[MAXKEYB-1] == 0xffff && )
                {
                    if (sizeof(ApplVar.PB)>sizeof(SysBuf))
                    {
                        RJPrint(0,"sizeof(ApplVar.PB)>sizeof(SysBuf)");
                    }
                    memcpy(SysBuf,&ApplVar.PB,sizeof(ApplVar.PB));//ccr20110916 save the ApplVar.PB
                    memset(ProgLineMes,' ',PRTLEN);
                    CopyFrStr(ProgLineMes,ApplVar.Group.Name);

                    PbTotal(ApplVar.PbNumber, 0);   /* read new for ApplVar.PB text */
                    if (ApplVar.AP.Pb.Text && ApplVar.PB.Text[0])//ccr20110913
                        memcpy(ProgLineMes+PRTLEN/2, ApplVar.PB.Text, ApplVar.AP.Pb.Text);
                    else if (ApplVar.AP.Pb.Random & 0x0f)
                        HEXtoASC(ProgLineMes+PRTLEN/2,ApplVar.PB.Random,7);
                    else
                    {
                        CopyFrStr(ProgLineMes+PRTLEN/2,Msg[SETPBINF].str);
                        WORDtoASC(ProgLineMes+PRTLEN-3,ApplVar.PbNumber);
                    }
                    memcpy(&ApplVar.PB,SysBuf,sizeof(ApplVar.PB));//ccr20110916 save the ApplVar.PB

                    ProgLineMes[PRTLEN]=0;
                    PrintStr(ProgLineMes);
                }
                else
                    PrintStr(ApplVar.Group.Name);
            }
            if (sGroup!=0xff && ApplVar.RGRec.Group==sGroup &&
                (ApplVar.RGRec.Key.Code>PLU1 && ApplVar.RGRec.Key.Code<PLU3 ||
                 ApplVar.RGRec.Key.Code>DEPT && ApplVar.RGRec.Key.Code<DEPT+5000)
               )//it must be the PLU or DEPT record
            {
                if (CheckNotZero(&ApplVar.RGRec.Qty))//Must not be the cancel record
                {
                    memset(ProgLineMes,' ',sizeof(ProgLineMes));
                    if (ApplVar.RGRec.Key.Code>PLU1 && ApplVar.RGRec.Key.Code<PLU3)
                    {
                        ApplVar.PluNumber = ApplVar.RGRec.Key.Code - PLU1 - 1;
                        ReadPlu();

                        strcpy(ProgLineMes+ALLPRNPOS,ApplVar.Plu.Name);
                        if (ProgLineMes[ALLPRNPOS]=='@')
                        {
                            ProgLineMes[ALLPRNPOS] = ' ';
                            ProgLineMes[0] = '@';
                        }
                    }
                    else
                    {
                        ApplVar.DeptNumber = ApplVar.RGRec.Key.Code - DEPT - 1;
                        ReadDept();
                        strcpy(ProgLineMes+ALLPRNPOS,ApplVar.Dept.Name);
                        if (ProgLineMes[ALLPRNPOS]=='@')
                        {
                            ProgLineMes[ALLPRNPOS] = ' ';
                            ProgLineMes[0] = '@';
                        }
                    }


#if (PRNAMTON)
                    sAmt = ApplVar.RGRec.Amt;
                    Multiply(&sAmt, &ApplVar.RGRec.Qty);
                    RoundBcd(&sAmt, 0);
                    RJPrint(0,FormatStrQtyPriAmt(ProgLineMes, &ApplVar.RGRec.Qty, 0, &sAmt, PRTLEN));
#else
                    RJPrint(0,FormatQtyStr(ProgLineMes,&ApplVar.RGRec.Qty,PRTLEN));
#endif
                }
                ApplVar.RGItems.RGBuf[i].Group = 0xff;//标示已经打印过

                if (ApplVar.RGItems.RGBuf[i+1].Key.Code>MODI && ApplVar.RGItems.RGBuf[i+1].Key.Code< MODI + 1000)
                {
                    ApplVar.ModiCount = ApplVar.RGItems.RGBuf[i+1].Key.Code - MODI;
                    memcpy(ApplVar.ModNum, &ApplVar.RGItems.RGBuf[i+1].Qty, sizeof(ApplVar.ModNum));
                    for (j=0;j<ApplVar.ModiCount;j++)
                    {
                        ApplVar.ModiNumber = ApplVar.ModNum[j];
                        ReadModi();
                        MemSet(SysBuf, sizeof(SysBuf), ' ');
                        strcpy(SysBuf + ALLPRNPOS, ApplVar.Modi.Name);
                        if (SysBuf[ALLPRNPOS]=='@')
                        {
                            SysBuf[ALLPRNPOS] = ' ';
                            SysBuf[0] = '@';
                        }
                        PrintStr(SysBuf);
                    }
                    ApplVar.ModiCount = 0;
                    memset(ApplVar.ModNum, 0, sizeof(ApplVar.ModNum));
                    i++;
                }
            }
        }
        if (TESTBIT(DOT,BIT4) && sLine)
        {
            PrintLine('-');
            PrintRegiInfo(FALSE);
            RFeed(1);
            PrintLine('-');
            sLine = 0;
        }
    }while (sGroup!=0xff);

    if (sLine)
    {
        if (!TESTBIT(DOT,BIT4))
        {
            PrintLine('-');
            PrintRegiInfo(FALSE);
            RFeed(1);
        }
        //if( RJPRN )
        //ApplVar.PrintLayOut = 0 ;    ----0905 Zengy Logout
        if (TESTBIT(AM_PM, BIT3))          /* Double Heigth Trailer ? */
            ApplVar.PrintLayOut = 0x06;
        else
            ApplVar.PrintLayOut = 0x02;
        if (ApplVar.BufKp != 2 && (ApplVar.TXT.Trailer[0][0]))
        {
            RFeed(1);
            for (i = 0; i < 6; i++)
            {
                if (ApplVar.TXT.Trailer[i][0])
                    PrintStr_Center(ApplVar.TXT.Trailer[i],DBLHIPRN & BIT4);
                else
                    break;
            }
        }
    }
#if (PRESSENTER)
    ClsState2();
#endif
    if (TESTBIT(sFlags, ENPRINTER))
        RFeed(9);
    ApplVar.MyFlags = sFlags;
    ApplVar.PbNumber =sSavePBn;
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    SETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
#endif

}

