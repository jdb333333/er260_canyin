#define PROGOPT_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"


/****************************************************************/

//Get byte value from input
void GetByte(BYTE *to,BYTE min,BYTE max)
{
    Editor.DispX = SETUPDISPX;//new SETUPDISPX-SETINPUTWD
    Editor.DispW = SETINPUTWD;
    Editor.DispY = CursY;

    if (Appl_EntryCounter)
    {
        StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
        GetWordEntry();

        if (ApplVar.NumberEntry < min || ApplVar.NumberEntry > max)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return;
        }
        *to = ApplVar.NumberEntry;
    }
}

//Get hex data from input(BCD)
void GetHexBytes(BYTE *to,BYTE max)
{
    if (Appl_EntryCounter > (max * 2 + ((ApplVar.DecimalPoint>0) ? 1 : 0 )))//ccr20131015
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
    else
        StrToBCDValue(to, &EntryBuffer[ENTRYSIZE - 2], max);
}


//void InvertBit(BYTE type,void *flag)
void InvertBit(WORD type,void *flag)
{
    BYTE *f, bit;

    bit = (0x01 << ((Appl_BitNumber - 2) & 0x07));
    f = (BYTE *) flag;
    f += ((Appl_BitNumber - 2) / 8);
    if (Appl_EntryCounter)
    {
        *f ^= bit;
    }
    DispStrXY(Msg[type].str,0,0xff);
    WORDtoASC(ProgLineMes + 2, Appl_BitNumber - 1);
    if (*f & bit)
        strcpy(ProgLineMes + SETINPUTWD - 4, Msg[KAI].str);
    else
        strcpy(ProgLineMes + SETINPUTWD - 4, Msg[GUAN].str);
}

//Chang the Binary data to string "11010011"
void Btoa(BYTE type,void *flag)
{
    short i;

    for (i = 0; i < 8; i++)
    {
        if (*((BYTE *)flag) & (0x01 << i))
            ProgLineMes[5 + i] = '1';
        else
            ProgLineMes[5 + i] = '0';
    }
}

//convert "12345678" to byte data
void strToBin(BYTE type,BYTE *byte)
{
    short i;
    BYTE sByte;

    *byte = 0;
    for (i=0;i<Appl_EntryCounter;i++)
    {
        sByte =EntryBuffer[sizeof(EntryBuffer) - 2 - i];
        if (sByte==0x30)
            continue;
        else if (sByte>0x30 && sByte <= 0x38)
            (*byte) |= (1<<(sByte - 0x31));
        else
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
			break;
		}
	}
}

/* return 1 if option exist else return 0 */

BYTE GetOpt(BYTE type,void *opt,BYTE  size)
{
    short   sLp,sP;

    MemSet(ProgLineMes, sizeof(ProgLineMes), ' ');

    Editor.DispX = SETUPDISPX;//new SETUPDISPX-SETINPUTWD
    Editor.DispW = SETINPUTWD;
    Editor.DispY = CursY;

    switch (type)
    {
    case 1:     /* dept number (plu) */
        if (Appl_EntryCounter)
        {
            StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
            GetWordEntry();
            if (!ApplVar.NumberEntry || ApplVar.NumberEntry > ApplVar.AP.Dept.Number)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            else
            {
                ApplVar.NumberEntry--;
                *((WORD *) opt) = ApplVar.NumberEntry;
            }
        }
        DispStrXY(Msg[BULEIF].str, 0,0xff);
        WORDtoASC(ProgLineMes + SETINPUTWD-1, *((WORD *) opt) + 1);
        break;
    case 32:
        if (Appl_EntryCounter)
        {
            StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
            GetWordEntry();
            if (!ApplVar.AP.Plu.RandomSize && ApplVar.NumberEntry > ApplVar.AP.Plu.Number)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            else
                *((WORD *) opt) = ApplVar.NumberEntry;
        }
        DispStrXY(Msg[LIANJIEF].str, 0,0xff);
        WORDtoASC(ProgLineMes + SETINPUTWD-1, *((WORD *) opt));
        break;
    case 33:    /* ApplVar.OFFPrice index,added by ccr */
        if (Appl_EntryCounter)
        {
            StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
            GetWordEntry();
            if (ApplVar.NumberEntry > ApplVar.AP.OFFPrice.Number)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            else
                *((WORD *) opt) = ApplVar.NumberEntry;
        }
        DispStrXY(Msg[YHJIAF].str, 0,0xff);
        WORDtoASC(ProgLineMes + SETINPUTWD-1, *((WORD *) opt));
        break;
    case 2:     /* dept group number */
        GetByte((BYTE *)opt, 1, size);
        if (Appl_EntryCounter)
            *(BYTE *)opt = *(BYTE *)opt - 1;
        DispStrXY(Msg[GUIZUF].str, 0,0xff);
        WORDtoASC(ProgLineMes + SETINPUTWD-1, *((BYTE *) opt) + 1);
        break;
    case 3:     /* print */
    case 4:     /* options */
    case 5:     /* kp */
    case 6:     /* tax */
    case 7:     /* disable key switch */
        if (!size)
        {
            Appl_ProgLine++;
            return 0;
        }
        if (!Appl_BitNumber)
            Appl_BitNumber = 2;
        if (Appl_BitNumber > size + 1)
        {
            Appl_BitNumber = 0;
            Appl_ProgLine++;
            return 0;
        }
        if (Appl_ProgStart == 2)
        {
            DispStrXY(Msg[DAYINF+type-3].str, 0,0xff);
            Btoa(type, (BYTE *) opt);       /* print bit map */
            return 1;
        }
        else
            InvertBit(type-3+DAYINF, (BYTE *) opt); /* type 1 is print */
        break;
    case 8:     /* price1 */
    case 9:     /* price2 */
    case 10:    /* price3 */
    case 11:    /* price4 */
    case 12:    /* costprice */

    case 13:    /* fixed amt */
    case 14:    /* max amount */

    case 15:    /* fixed perc */
    case 16:    /* max perc */
    case 17:    /* Tax Rate */

    case 18:    /* buyrate */
    case 19:    /* sell rate */
    case 31:    /* Random PB Number */
        if (Appl_EntryCounter)
        {
            if (ApplVar.DecimalPoint)
                ApplVar.DecimalPoint--;
            GetHexBytes((BYTE *) opt, size);
        }
        if (type == 31)
        {
            DispStrXY(Msg[DAIMA].str, 0,0xff);
            HEXtoASC(ProgLineMes + SETINPUTWD-size*2, (BYTE *) opt, size);
            if (Appl_ProgStart == 2) /* dump ? */
                return 1;
            else
                break;
        }
        ApplVar.Entry = ZERO;
        memcpy(ApplVar.Entry.Value, opt, size);

        if (Appl_MaxEntry>SETINPUTWD)//ccr20131015>>>>
        {
            if (Appl_EntryCounter==0)
            {
                DispStrXY(Msg[JIAGE1+type-8].str, 0,0xff);
#if (DOUBLELINE)
                if (CursY>=SCREENLN-1-(ApplVar.ScreenStat & STATEON))
                {//在屏幕的最后一行,屏幕内容上滚一行
                    ScrollScreen(1);
                }
                else
                {
                    CursY++;
                }
#endif
            }
#if (DOUBLELINE)
            sP = 2;
#else
            sP = strlen(Msg[JIAGE1+type-8].str);//SETUPDISPX;//new SETUPDISPX-SETINPUTWD
#endif
            Editor.DispX = sP;//SETUPDISPX;//new SETUPDISPX-SETINPUTWD
            Editor.DispW = SCREENWD-sP;
            Editor.DispY = CursY;
            sP = Editor.DispW;
        }
        else
        {
            DispStrXY(Msg[JIAGE1+type-8].str, 0,0xff);
            sP = SETINPUTWD;
        }//<<<<<<<<<<
        if (type < 15)  /* amount */
        {
            if (Appl_EntryCounter)
            {
                ChangePoint();
                memcpy(opt,ApplVar.Entry.Value, size);
            }
            Appl_EntryCounter=0;  //add by lqw for avoiding show the next line,and instead to wait for a line.
//#if(defined(CASE_GPRS))
            //strncpy(ProgLineMes, DispAmtStr((char*)0, &ApplVar.Entry,SETUPDISPX),SETUPDISPX);
//#else
            strncpy(ProgLineMes, DispAmtStr((char*)0, &ApplVar.Entry,sP),sP);   //ccr20131015
//#endif
        }
        else
        {
            if (type < 18)
            {
                if (Appl_EntryCounter)
                {
                    ChangePoint2();//ccr2016-11-07
                    memcpy(opt,ApplVar.Entry.Value, size);
                }
                ApplVar.Entry.Sign = 2; // perc
            }
            else
            {
                if (Appl_EntryCounter)
                {
                    switch (ApplVar.DecimalPoint)
                    {
                    case 0:
                        BcdMul10(&ApplVar.Entry);
                    case 1:
                        BcdMul10(&ApplVar.Entry);
                    case 2:
                        BcdMul10(&ApplVar.Entry);
                    default:
                        break;
                    }
                    memcpy(opt,ApplVar.Entry.Value, size);
                }
                ApplVar.Entry.Sign = 3;
//lyq modify 2003\10\22 end
            }
            strncpy(ProgLineMes, DispQtyStr(ProgLineMes, &ApplVar.Entry,sP), sP); //ccr20131015
            //DispStrXY(ProgLineMes,SETUPDISPX,1);
        }
        break;

    case 46://get DEPT TAX
        if (Appl_EntryCounter)
            strToBin(type,(BYTE *)opt);
        DispStrXY(Msg[SHUILVF].str, 0,0xff);
        sP = 0;
        for (sLp = 0; sLp < 7; sLp++)
        {
            if (*((BYTE *)opt) & (0x01 << sLp))
            {
                ProgLineMes[sP] = sLp+0x31;
                sP++;
            }
        }
        break;
    case 47:
        break;
//lqw end>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    case 20:    /* start time zone */
        if (Appl_EntryCounter)
        {
            GetHexBytes(SysBuf, 2);
            if (!ApplVar.ErrorNumber &&
                SysBuf[0] < 0x59 && SysBuf[1] < 0x24)
            {
                *((BYTE *) opt) = SysBuf[0];
                *(((BYTE *) opt) + 1) = SysBuf[1];
            }
            else
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI03);
                break;
            }
        }
        DispStrXY(Msg[SETZONES].str, 0,0xff);//state
//ccr2016-01-25>>>>>>>>>>>>>>>>>
//ccr2016-01-25        ApplVar.Entry = ZERO;
        memcpy(ApplVar.Entry.Value, opt, size);
        ProgLineMes[0]=(ApplVar.Entry.Value[1]>>4)+'0';
        ProgLineMes[1]=(ApplVar.Entry.Value[1] & 0x0f)+'0';
        ProgLineMes[2]=':';
        ProgLineMes[3]=(ApplVar.Entry.Value[0]>>4)+'0';
        ProgLineMes[4]=(ApplVar.Entry.Value[0] & 0x0f)+'0';
        ProgLineMes[5]=0;

//        ApplVar.Entry.Sign = 2;
//        strncpy(ProgLineMes, DispQtyStr((char*)0, &ApplVar.Entry,SETINPUTWD), SETINPUTWD);
//ccr2016-01-25<<<<<<<<<<<<<<<<<
        break;
    case 21:     /* drawer 1 */
    case 22:     /* drawer 2 OTD */
    case 23:     /* tip */
    case 24:     /* period */
    case 25:    /* pointer */
    case 26:    /* amount prefix1 */
    case 27:     /* amount prefix2 */
    case 28:    /* link */
    case 30:    /* max entry */
        if (!size)
            break;
        if (Appl_EntryCounter)
            GetByte((BYTE *) opt, 0, size);
        if (type == 30)     /* max entry ? */
            DispStrXY(Msg[ZDZHIF].str, 0,0xff);//MAX
        else if (type == 28)
        {
            DispStrXY(Msg[LIANJIEF].str, 0,0xff);//Lin
            WORDtoASC(ProgLineMes + 6, Appl_ProgLine - 5); // lyq added 2003\10\27
        }
        else if (type > 25)
        {//26,27
            DispStrXY(Msg[QIANZHUIFU].str, 0,0xff);//pre
            //ccr2016-08-03>>>>>>>>>>>>>>>>>>>>>>
            sLp=strlen(Msg[QIANZHUIFU].str);
            DispCharXY((type - 25)+'0',sLp,0xff);//pre
            if (*((BYTE *) opt))
            {
                DispCharXY(*((BYTE *) opt),sLp+2,0xff);//ccr2016-08-02
            }
            //ccr2016-08-03<<<<<<<<<<<<<<<<<<<<<<<
            //WORDtoASCL(ProgLineMes, type - 25);
        }
        else//21,22,23,24,25
            DispStrXY(Msg[type - 21 + QIANXIANGF].str, 0,0xff);
        WORDtoASC(ProgLineMes + SETINPUTWD -1, *((BYTE *) opt));
        if ((type < 24 || type > 25) && !*((BYTE *) opt))   /* copy off if zero */
            memcpy(ProgLineMes + SETINPUTWD -3, Msg[GUAN].str, lCAPWIDTH);
        break;

    case 34:    //OFF type
        if (Appl_EntryCounter)
        {
            StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
            GetWordEntry();
            if (ApplVar.NumberEntry > size)
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                break;
            }
            else
                *((BYTE *)opt) = ApplVar.NumberEntry;
        }
        DispStrXY(Msg[LEIXING].str, 0,0xff);
        WORDtoASC(ProgLineMes + SETINPUTWD-1, *((BYTE *)opt));
        break;
    case 35:    //DateFrom
    case 36:    //DateTo
        if (Appl_EntryCounter)
        {
            NewTimeDate(0xf1);
            if (!ApplVar.ErrorNumber)
            {
                *((WORD *) opt) = ((WORD)Now.month<<8) | Now.day;
                if (size==4)
                    *((WORD *)((char *)opt+2)) = Now.year;
            }
            else
                break;
        }
        if (size>2)
            DispStrXY(Msg[KSHSHJIANF-35+type].str, 0,0xff);
        else
            DispStrXY(Msg[KSHRQIF-35+type].str, 0,0xff);
        HEXtoASC(ProgLineMes + SETINPUTWD - size * 2 ,(char *)opt,size);
        break;

    case 37:    //TimeFrom
    case 38:    //TimeTo
        if (Appl_EntryCounter)
        {
            NewTimeDate(0xf2);
            if (!ApplVar.ErrorNumber)
                *((WORD *) opt) = ((WORD)Now.hour<<8) | Now.min;
            else
                break;
        }
        DispStrXY(Msg[type-37+KSHSHJIANF].str, 0,0xff);
        HEXtoASC(ProgLineMes + SETINPUTWD-4, (char *)opt,2);
        break;
    case 39:    //WeekDay
        if (Appl_EntryCounter)
            strToBin(type,(BYTE *)opt);
        DispStrXY(Msg[XINGQIFF].str, 0,0xff);
        sP = 0;
        for (sLp = 0; sLp < 7; sLp++)
        {
            if (*((BYTE *)opt) & (0x01 << sLp))
            {
                ProgLineMes[sP] = sLp+0x31;
                sP++;
            }
        }
        break;
    case 41:        //OFF NumItems
        if (!size)
            break;
        GetByte((BYTE *)opt, 0, size);
        DispStrXY(Msg[DBSHLIANGF].str, 0,0xff);//MAX
        WORDtoASC(ProgLineMes + SETINPUTWD -1, *((BYTE *)opt));
        break;
    case 40:        //OFF Discount
    case 42:
    case 43:
        if (Appl_EntryCounter)
        {
            if (ApplVar.DecimalPoint)
                ApplVar.DecimalPoint--;
            GetHexBytes((BYTE *) opt, size);
        }
        DispStrXY(Msg[type-40+ZHEKOUF].str, 0,0xff);
        ProgLineMes[lCAPWIDTH+1] = 0;
        ApplVar.Entry = ZERO;
        memcpy(ApplVar.Entry.Value, opt, size);
//lyq added 2003\10\23 start
        if (Appl_EntryCounter)
        {
            ChangePoint();
            memcpy(opt,ApplVar.Entry.Value, size);
        }
//lyq added 2003\10\23 end
        strncpy(ProgLineMes, DispAmtStr(ProgLineMes, &ApplVar.Entry,SETINPUTWD),sizeof(ProgLineMes));
        break;
    default:
        break;
    }

//ccr20131015    ProgLineMes[SETINPUTWD] = 0;

    return 1;
}

//ccr2016-01-21>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/**
 * 选择连主机的方式
 *
 * @author EutronSoftware (2016-01-21)
 *
 * @param pType:0-主机,1-条码枪,2-电子秤,3-厨房打印机,4-票据打印机,5-外置打印机
 *
 */

CONST char NETWORK[]="ET-NET";//长度必须小余SETINPUTWD
void GetDevicePort(BYTE pType,void *val)
{
    short   sLp,sP;

    MemSet(ProgLineMes, sizeof(ProgLineMes), ' ');

    Editor.DispX = SETUPDISPX;//new SETUPDISPX-SETINPUTWD
    Editor.DispW = SETINPUTWD;
    Editor.DispY = CursY;

    if (Appl_BitNumber > LINKTYPENUM )
        Appl_BitNumber = 1;

    *(BYTE *)val = Appl_BitNumber-1;
    DispStrXY(PortType[pType],0,0xff);
#if (defined(TCPIP) || defined(CASE_GPRS))
    if (pType==FORHOST && Appl_BitNumber==1)
        memcpy(ProgLineMes+ SETINPUTWD + 1 - sizeof(NETWORK),NETWORK,sizeof(NETWORK));
    else
#endif
        memcpy(ProgLineMes+ SETINPUTWD + 1 - sizeof(PortForDevice[0]),PortForDevice[Appl_BitNumber-1],sizeof(PortForDevice[0]));
}

/**
 * 获取串口的通讯速率
 *
 * @author EutronSoftware (2016-01-21)
 *
 * @param val
 */
void GetPortRate(void *val)
{
    MemSet(ProgLineMes, sizeof(ProgLineMes), ' ');

    Editor.DispX = SETUPDISPX;//new SETUPDISPX-SETINPUTWD
    Editor.DispW = SETINPUTWD;
    Editor.DispY = CursY;

    if (Appl_BitNumber > PORTRATENUM )
        Appl_BitNumber = 1;

    *(BYTE *)val = EnCodeRate(Appl_BitNumber);
    DispStrXY(Msg[TXXYIF].str, 0,0xff);
    memcpy(ProgLineMes+ SETINPUTWD + 1 - sizeof(PortRate[0]),PortRate[Appl_BitNumber-1],sizeof(PortRate[0]));
}

//ccr2016-01-21<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
