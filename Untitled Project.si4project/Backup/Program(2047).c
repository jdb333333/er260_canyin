#define PROGRAM_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "Message.h"

BYTE ProgDevice(BYTE dev,BYTE *port,BYTE LineFr);

#if defined(CASE_FORHANZI)
CONST char Number0[]="无记录";
#else
CONST char Number0[]="COUNT=0";
#endif


/**
 * 对设置位进行编程
 *
 * @author EutronSoftware (2016-11-10)
 *
 * @param MsgIdx :要显示的信息
 * @param Opt :存放位数据的字节
 * @param BitNo 1..8  if (BitNo.Bit7=1),display it with caption
 * @param sInv :=0时,位值1有效;=YN时,位值0有效
 */
void CheckBitValue(short MsgIdx,char *Opt,BYTE BitNo,BYTE sInv)
{
    BYTE    BitV;
    int     slen;

    BitV = BIT0<<((BitNo-1) & 0x0f);

    Appl_MaxEntry=1;      //hf 20060105
    if (!Appl_BitNumber)
        Appl_BitNumber = (TESTBIT(*Opt,BitV)!=0)+1;
    if (Appl_BitNumber > 2)
        Appl_BitNumber = 1;
    if (Appl_BitNumber ==2)
    {
        ProgLineMes[SETINPUTWD-1] = 'y' ^ sInv;
        SETBIT(*Opt,BitV);
    }
    else
    {
        ProgLineMes[SETINPUTWD-1] = 'n' ^ sInv;
        RESETBIT(*Opt,BitV);
    }

    if (ProgLineMes[SETINPUTWD-1] == 'y')
        strcpy(ProgLineMes+SETINPUTWD-3,Msg[KAI].str);
    else
        strcpy(ProgLineMes+SETINPUTWD-3,Msg[GUAN].str);

    SetEditPosition(SETUPDISPX,CursY,0,NULL);
    DispStrXY(Msg[MsgIdx].str,0,0xff);
    if (TESTBIT(BitNo,BIT7))
    {
        slen =strlen(Msg[MsgIdx].str);
        DisplayDecXY(BitNo & 0x0f,slen,0xff,1);  //ccr2016-04-20
        //WORDtoASC(ProgLineMes+2,BitNo & 0x0f);
        //WORDtoASC(ProgLineMes+1,BitNo & 0x0f);      //dept kp disp 20060712
    }
}

void GetBCDValue(short MsgIdx,char *val,short BCDBytes,char IsQty)
{
    BYTE    sLp;
    BYTE    sWidth=SETINPUTWD;//ccr2015-05-21

    sLp = strlen(Msg[MsgIdx].str);
    if (sLp<SCREENWD)
        sWidth = SCREENWD-sLp;
    else
        sLp = SETUPDISPX;

    if (Appl_EntryCounter)
    {
        if (Appl_EntryCounter > (BCDBytes * 2))
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        else
            StrToBCDValue(val, &EntryBuffer[ENTRYSIZE - 2], BCDBytes);
    }
//ccr2015-05-21    else
    {
//ccr2015-05-21        SetEditPosition(SETUPDISPX,CursY,0,NULL);
        SetEditPosition(sLp,CursY,0,NULL);
    }

    ApplVar.Entry = ZERO;
    memcpy(ApplVar.Entry.Value, val, BCDBytes);
    DispStrXY(Msg[MsgIdx].str,0,0xff);
    if (IsQty)
    {
        strncpy(ProgLineMes, DispQtyStr((char*)0, &ApplVar.Entry,sWidth),sWidth);//ccr2015-05-21
        if (TESTBIT(ApplVar.MyFlags,PWDMODE) && CompareBCDValue(ApplVar.Entry.Value,(void*)ZERO.Value,4))//ccr2016-02-16
        {
            for (sLp=0;sLp<sizeof(ProgLineMes);sLp++)//ccr2015-05-21
            {
                if (ProgLineMes[sLp]==0)
                    break;
                if (ProgLineMes[sLp]!=' ')
                    ProgLineMes[sLp] ='*';
            }
        }
    }
    else
    {
        if (Appl_EntryCounter)
        {
            if (ApplVar.DecimalPoint)
                ApplVar.DecimalPoint--;
            ChangePoint();
            memcpy(val,ApplVar.Entry.Value,BCDBytes);
        }
        strncpy(ProgLineMes, DispAmtStr((char*)0, &ApplVar.Entry,sWidth),sWidth);//ccr2015-05-21
    }
}
/**
 * 两行方式输入HEX数据
 *
 * @author EutronSoftware (2014-12-25)
 *
 * @param Messg
 * @param val
 * @param BCDBytes
 */
void GetHEXValue(const char *Messg,char *val,short BCDBytes)
{
    int sP,i,sLen;

    sP = Appl_MaxEntry;
    if (Appl_EntryCounter)
    {
        if (Appl_EntryCounter < sP)
            sP = Appl_EntryCounter;
        for (i=0;i<sP;i++)
        {
            if (EntryBuffer[ENTRYSIZE - 2-i]>='a' && EntryBuffer[ENTRYSIZE - 2-i]<='f')
            {
                EntryBuffer[ENTRYSIZE - 2-i] -= 0x20;
                continue;
            }
            if (EntryBuffer[ENTRYSIZE - 2-i]>='0' && EntryBuffer[ENTRYSIZE - 2-i]<='9' ||
                EntryBuffer[ENTRYSIZE - 2-i]>='A' && EntryBuffer[ENTRYSIZE - 2-i]<='F')
                continue;
            else
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                break;
            }
        }
        if (ApplVar.ErrorNumber==0)
        {
            StrToBCDValue(val, &EntryBuffer[ENTRYSIZE - 2], BCDBytes);
            memset(ProgLineMes,' ',sP);
            memcpy(ProgLineMes, EntryBuffer + ENTRYSIZE - sP - 1, sP);
        }
    }
    else
    {
        DispStrXY((char*)Messg,0,0xff);
        HEXtoASC(ProgLineMes,val,BCDBytes);;
        sLen=strlen(Messg);

        if (sLen>4)
        {
            if (CursY>=SCREENLN-1-(ApplVar.ScreenStat & STATEON))
                //ccr2014-11-10在屏幕的最后一行,屏幕内容上滚一行，IP输入单独一行
                ScrollScreen(1);
            else
                CursY++;//ccr2014-11-10IP输入单独一行
            sLen=0;
        }

        SetEditPosition(sLen,CursY,sP,NULL);
    }

    ProgLineMes[Editor.DispW]=0; //SETINPUTWD=SCREENWD-SETUPDISPX=24-10=14;
}

/**
 * 为设置项输入WORD数据
 *
 * @author EutronSoftware (2016-03-24)
 *
 * @param MsgIdx :设置项名称
 * @param val :存放输入的内容
 * @param max :输入最大值
 */
void GetWordValue(short MsgIdx,WORD *val,WORD max)
{
    BYTE width;

    width = WidthofWORD(max);
    if (Appl_EntryCounter)
    {
        StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
        GetWordEntry();
        if (ApplVar.NumberEntry>max)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        else
            *val = ApplVar.NumberEntry;
    }

    SetEditPosition(SCREENWD-width,CursY,0,NULL);
    DispStrXY(Msg[MsgIdx].str,0,0xff);
    WORDtoASC(ProgLineMes+width-1, *val);
}

/**
 * 为设置项输入BYTE数据
 *
 * @author EutronSoftware (2016-03-24)
 *
 * @param MsgIdx :设置项名称
 * @param val :存放输入的内容
 * @param max :输入最大值
 */

void GetByteValue(short MsgIdx,BYTE *val,BYTE max)
{
    WORD sVal;

    sVal = *val;
    GetWordValue(MsgIdx,&sVal,max);
    if (ApplVar.ErrorNumber==0 && Appl_EntryCounter)
        *val = sVal;
}

/**
 *
 *
 * @author EutronSoftware (2015-09-30)
 *
 * @param MsgIdx :提示信息. =0时,无信息
 * @param val
 * @param size
 * @param num :>0时,显示序号
 *
 * @return BYTE
 */
BYTE GetString(short MsgIdx,BYTE *val,WORD size,WORD num) //ccr2015-09-30
{
    short   sP,mLen,i;
    char    numbuf[8];

    if (MsgIdx)//ccr2016-01-19
        mLen = strlen(Msg[MsgIdx].str);
    else
        mLen = 0;

    if (size)
    {
        sP=size;
        if (Appl_EntryCounter)
        {
            if (Appl_EntryCounter<sP)
                sP = Appl_EntryCounter;
            memset(val,0,size);
            ClearLine(CursY);   //ccr2016-01-18
            if (mLen)   //ccr2016-06-12
                DispStrXY(Msg[MsgIdx].str,0,0xff);

            if (sP>1 || EntryBuffer[ENTRYSIZE - 2]!=' ')//如果只输入了一个空格,就认为NULL
                memcpy(val, EntryBuffer + ENTRYSIZE - Appl_EntryCounter - 1, sP);
        }
        else
        {
            if (!TESTBIT(ApplVar.ScreenStat,HZINPUTMODE))
                ChnInput(2);//自动进入汉字输入方式

//			sP = strlen(val);
//			if (sP>size) sP = size;
//cc 2006-08-14 for Caption>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            //memcpy(EntryBuffer+ENTRYSIZE-size-1,val,sP);

            sP = strlen(val);
            while (sP)
            {
                if (*(val+sP-1) != ' ')
                    break;
                sP--;
            }
            if (sP>size)
                sP = size;
            memcpy(EntryBuffer+ENTRYSIZE-sP-1,val,sP);
//cc 2006-08-14 for Caption>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//			if (sP<size) sP = size;
            if (mLen)   //ccr2016-01-19
                DispStrXY(Msg[MsgIdx].str,0,0xff);

            if (num)//ccr2015-09-30>>>>>>>>>
            {
                memset(numbuf,' ',8);
                i = WORDtoASCL(numbuf,num);
                numbuf[i] = 0;
                DispStrXY(numbuf,mLen,0xff);
                mLen += i;//ccr2016-01-19
            }//<<<<<<<<<<<<<<<<<
#if (DOUBLELINE)
            if (CursY>=SCREENLN-1-(ApplVar.ScreenStat & STATEON))
                ScrollScreen(1);//ccr2015-03-10在屏幕的最后一行,屏幕内容上滚一行，输入单独一行
            else
                CursY++;//ccr2015-03-10 在单独一行上输入文字
#endif
        }

        SetEditPosition(mLen,CursY,sP,val);//ccr2016-01-19
        if (TESTBIT(ApplVar.MyFlags,PWDMODE))//ccr2015-03-10
            memset(ProgLineMes,'*',sP);//ccr2015-03-10
        else
            memcpy(ProgLineMes,val,sP);

        ProgLineMes[Editor.DispW]=0;

        return true;
    }
    else
    {
        Appl_ProgLine++;
        return false;
    }
}

//============ hf added 20040908 ================
/*****************************************
* 将数字格式转换为IP地址格式 *
* Num:待转换的IP 数组，4 个字节 *
* IPstr:转换后的IP 地址字符串，   *
*         格式为: 192.168.000.001，16 个    *
*        字节(加上'\0')                                  *
******************************************/
BYTE NumToIP(char *IPstr,BYTE *Num)
{
    BYTE ip,tip;
    BYTE i,j,k;

    for (i = 0,j = 0;i < 4;i++)
    {
        tip = ip = Num[i];

        if (ip>=100)        k=100;
        else if (ip>=10)    k=10;
        else                k = 1;

        for (;k > 0;k = k / 10)
        {
            IPstr[j] = ((ip / k) & 0x0f) + 48;
            if (IPstr[j] != '0' || tip)
                j++;
            else //if(tip == 0)
            {
                j++;
                break;
            }
            ip = ip % k;
        }
        if (i < 3)
            IPstr[j++] = '.';
    }
    IPstr[j] = '\0';
    return j;
}

/*****************************************
* 将IP 地址格式转换为数字格式*
* IPstr:待转换的IP 地址字符串，   *
*         格式如:192.168.0.1，最大16 个 *
*         字节(加上'\0')                                 *
* Num:转换后的IP 数组，4 个字节 *
******************************************/
BYTE IPToNum(BYTE *Num,char *IPstr,short Counter)
{
    short ip = 0;
    BYTE i,j;

    for (i = 0,j = 0;i < Counter;i++)
    {
        /*是空格则跳过*/
        if (IPstr[i] == ' ')
            i++;
        if (IPstr[i] != '.')
            ip = (IPstr[i] & 0x0f) + ip * 10;
        else
        {
            if (ip < 256)
            {
                Num[j] = ip;
                ip = 0;
                j++;
            }
            else
                return CWXXI71 - CWXXI01 + 1;
        }
    }
    if (j == 3 && ip < 256)
    {
        Num[j] = ip;
        return 0;
    }
    else
        return CWXXI71 - CWXXI01 + 1;
}

void GetIP(short MsgIdx,BYTE *val)
{
    short sP;

    sP = Appl_MaxEntry;
    if (Appl_EntryCounter)
    {
        if (Appl_EntryCounter < sP)
            sP = Appl_EntryCounter;
        memset(ProgLineMes,' ',sP);
        memcpy(ProgLineMes, EntryBuffer + ENTRYSIZE - sP - 1, sP);
        ApplVar.ErrorNumber=IPToNum(val, ProgLineMes, sP);
    }
    else
    {
        sP = NumToIP(ProgLineMes, val);
        memcpy(EntryBuffer + ENTRYSIZE - sP- 1,ProgLineMes,sP);
        DispStrXY(Msg[MsgIdx].str,0,0xff);
        if (CursY>=SCREENLN-1-(ApplVar.ScreenStat & STATEON))
            ScrollScreen(1);//ccr2014-11-10在屏幕的最后一行,屏幕内容上滚一行，IP输入单独一行
        else
            CursY++;//ccr2014-11-10IP输入单独一行
    }

    SetEditPosition(1,CursY,sP,NULL);

    ProgLineMes[Editor.DispW]=0; //SETINPUTWD=SCREENWD-SETUPDISPX=24-10=14;

}
//====================== hf added end ==================

void InitSysFlag()
{

    if (TESTBIT(COPYRECEIP, BIT7))      //打印清0日报表时,是否复位收据号   ///
        SETBIT(ApplVar.AP.ReportList[2].Options, BIT2);
    else
        RESETBIT(ApplVar.AP.ReportList[2].Options, BIT2);

    ApplVar.AmtDecimal = NO_DECIMAL;
    Prefix1 = PREFIX_1;
    Prefix2 = PREFIX_2;
    if (TESTBIT(KEYTONE,BIT0))
        Bios_1(BiosCmd_AutoClickOff);
    else
        Bios_1(BiosCmd_AutoClickOn);

//    Density(0, DENSITY);      /* Density Receipt */
//    ApplVar.AP.Config.RDensity = DENSITY;
//    Density(1, DENSITY);       /* Density Journal */
//    ApplVar.AP.Config.JDensity = DENSITY;
    ApplVar.AP.Config.Country = (CHARSET / 16);     /* set char set */
    if (ApplVar.AP.Config.Country > 4)
        ApplVar.AP.Config.Country = 0;
//    SetCountry(ApplVar.AP.Config.Country);      /* activate other character set */

}

void ProgPlu()
{
    WORD i;

    if (Appl_ProgNumber >= ApplVar.AP.Plu.Number)
    {
        if (ApplVar.AP.Plu.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    //ccr2016-07-05>>>>>>>>>>
    if (ApplVar.AP.Plu.RandomSize && ApplVar.AP.Plu.RNumber==0)
    {
        strcpy(ProgLineMes,Number0);
        return;
    }//ccr2016-07-05<<<<<<<<<<
    ApplVar.PluNumber = Appl_ProgNumber;
    ReadPlu();
    switch (Appl_ProgLine)
    {
    case 1:
        if (ApplVar.AP.Plu.RandomSize)
            Appl_MaxEntry=ApplVar.AP.Plu.RandomSize*2;
#if (0)//defined(CASE_INDIA)
        memcpy(&ApplVar.FlowBuff.CDC_EditPlu.Plu,&ApplVar.Plu,sizeof(ApplVar.Plu));
        ApplVar.FlowBuff.CDC_EditPlu.FunN=0x80+EDITPLULOG;//标示对PLU进行编辑修改
        ApplVar.FlowBuff.CDC_EditPlu.Code=Appl_ProgNumber+PLU1+1;
        ApplVar.FlowBuff.CDC_EditPlu.OperNo=ApplVar.ClerkNumber;
#endif
        break;
    case 2:     /* PLU name */
#if defined(CASE_INDIA)
        if (ApplVar.FlowBuff.CDC_EditPlu.FunN!=0x80+EDITPLULOG)
        {
            memcpy(&ApplVar.FlowBuff.CDC_EditPlu.Plu,&ApplVar.Plu,sizeof(ApplVar.Plu));
            ApplVar.FlowBuff.CDC_EditPlu.FunN=0x80+EDITPLULOG;//标示对PLU进行编辑修改
            ApplVar.FlowBuff.CDC_EditPlu.Code=Appl_ProgNumber+PLU1+1;
            ApplVar.FlowBuff.CDC_EditPlu.OperNo=ApplVar.ClerkNumber;
        }
#endif
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.Plu.CapSize;
        GetString(MINCHENF, ApplVar.Plu.Name, ApplVar.AP.Plu.CapSize,0);
        break;
    case 3:     /*Get PLU bar code*/
/*		if (ApplVar.AP.Plu.RandomSize)
        {
            Appl_MaxEntry=0;//ApplVar.AP.Plu.RandomSize * 2;
            if (Appl_EntryCounter)
            {
                StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
                memset(ApplVar.Plu.Random,0,sizeof(ApplVar.Plu.Random));
                memcpy(ApplVar.Plu.Random,ApplVar.Entry.Value,ApplVar.AP.Plu.RandomSize);
            }
            memcpy(ProgLineMes + 1, Msg[BULEIF].str, strlen(Msg[BULEIF].str));
            HEXtoASC(ProgLineMes + SETINPUTWD - Appl_MaxEntry,ApplVar.Plu.Random,ApplVar.AP.Plu.RandomSize);
            for (i=0;i<ApplVar.AP.Plu.RandomSize;i++)
            {
                if (ProgLineMes[SETINPUTWD - Appl_MaxEntry + i]=='0')
                    ProgLineMes[SETINPUTWD - Appl_MaxEntry + i]=' ';
                else
                    break;
            }
            break;
        }
        else */
        Appl_ProgLine = 4;
    case 4:     /* price 1 */
    case 5:     /* price 2 */
    case 6:     /* price 3 */
    case 7:     /* price 4 */
        if (ApplVar.AP.Plu.Level>Appl_ProgLine-4)
        {
            Appl_MaxEntry=10;
            if (GetOpt(4 + Appl_ProgLine, ApplVar.Plu.Price[Appl_ProgLine - 4], ApplVar.AP.Plu.PriceSize))
                break;
        }
        else
            Appl_ProgLine = 8;
    case 8:     /* plu dept */
        Appl_MaxEntry=4;

        GetOpt(1, (WORD*)&ApplVar.PluDept, ApplVar.AP.Dept.Number);
        ApplVar.Plu.Dept = *((BYTE *)&ApplVar.PluDept);
        ApplVar.Plu.DeptHi = *(((BYTE *)&ApplVar.PluDept)+1);
        break;
    case 9:     /* costprice */
        Appl_MaxEntry=10;
        if (ApplVar.AP.Plu.Cost && GetOpt(12, ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize))
            break;
        else
            Appl_ProgLine = 10;
    case 10:        //OFF index for PLU
        Appl_MaxEntry=4;
        if (ApplVar.AP.OFFPrice.Number && GetOpt(33, (WORD*)&ApplVar.Plu.OFFIndex, ApplVar.AP.OFFPrice.Number))
            break;
        else
            Appl_ProgLine = 11;
    case 11:         /* Linked ApplVar.Plu */
        Appl_MaxEntry=4;
        if (TESTBIT(ApplVar.AP.Options.Plu, BIT0) &&
            GetOpt(32, (WORD*)&ApplVar.Plu.Link, ApplVar.AP.Plu.Number))
            break;
    default:
#if defined(CASE_INDIA)
        SavePlu_DeptEditLOG();
#endif
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WritePlu();
}


void ProgDept()
{

    //ccr2014-07-26>>>>>>>判断厨房打印机
    struct PRINTER *SetKP;
    unsigned int i;
    //<<<<<<<
    if (Appl_ProgNumber >= ApplVar.AP.Dept.Number)
    {
        if (ApplVar.AP.Dept.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.DeptNumber = Appl_ProgNumber;
    ReadDept();
    switch (Appl_ProgLine)
    {
    case 1:
#if (0)//defined(CASE_INDIA)
        memcpy(&ApplVar.FlowBuff.CDC_EditDept.Dept,&ApplVar.Dept,sizeof(ApplVar.Dept));
        ApplVar.FlowBuff.CDC_EditDept.FunN=0x80+EDITDEPTLOG;//标示对DEPT行编辑修改
        ApplVar.FlowBuff.CDC_EditDept.Code=Appl_ProgNumber+DEPT+1;
        ApplVar.FlowBuff.CDC_EditDept.OperNo=ApplVar.ClerkNumber;
#endif
        break;
    case 2:     /* dept name */
#if defined(CASE_INDIA)
        if (ApplVar.FlowBuff.CDC_EditDept.FunN!=0x80+EDITDEPTLOG)
        {
            memcpy(&ApplVar.FlowBuff.CDC_EditDept.Dept,&ApplVar.Dept,sizeof(ApplVar.Dept));
            ApplVar.FlowBuff.CDC_EditDept.FunN=0x80+EDITDEPTLOG;//标示对DEPT行编辑修改
            ApplVar.FlowBuff.CDC_EditDept.Code=Appl_ProgNumber+DEPT+1;
            ApplVar.FlowBuff.CDC_EditDept.OperNo=ApplVar.ClerkNumber;
        }
#endif
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.Dept.CapSize ;
        if (GetString(MINCHENF, ApplVar.Dept.Name, ApplVar.AP.Dept.CapSize,0))
            break;
    case 3:     /* ApplVar.Dept price */
        Appl_MaxEntry=10;
        GetOpt(8, ApplVar.Dept.Price, ApplVar.AP.Dept.PriceSize);
        break;
    case 4:     /* ApplVar.Dept price */
        Appl_MaxEntry=10;
        GetOpt(14, ApplVar.Dept.PriceMax, ApplVar.AP.Dept.PriMaxSize);
        break;
    case 5:     /* tax */
        Appl_MaxEntry=8;
	    GetOpt(46, &ApplVar.Dept.Tax, ApplVar.AP.Tax.Number);
        break;
    case 6:     /* group */
        Appl_MaxEntry=3;
        Appl_BitNumber = 0;
        GetOpt(2, &ApplVar.Dept.Group, ApplVar.AP.Group.Number);
        break;
    case 7: //设置厨房打印机(1-8)
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        //ccr2014-07-26>>>>>>>>>>>
        i = Appl_ProgLine-7;
        SetKP = (struct PRINTER *) &KPSTART;
        SetKP += i;
        do {
            if (SetKP->type & 0x0f)
                break;
            else
            {//厨房打印机不存在
                i++;
                SetKP++;
            }
        } while (i<8);
        if (i<8)
        {
            Appl_ProgLine = i + 7;
            Appl_MaxEntry=0;
            //Appl_EntryCounter = 0;
            CheckBitValue(CHFDYINF,&ApplVar.Dept.Kp,1+(Appl_ProgLine-7) + 0x80,0);
            break;
        }
        else
            Appl_ProgLine = 15;
        //<<<<<<<<<<<
    case 15:     /* options */
        Appl_ProgLine = 16;
    case 16:
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(JZHLJGE,&ApplVar.Dept.Options,1+(Appl_ProgLine-16+1),0);
        break;
    case 17:
    case 18:
    case 19:
    case 20:
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(DYFGXIAN+Appl_ProgLine-17,&ApplVar.Dept.Options,1+(Appl_ProgLine-17+4),0);
        break;
    case 21:     /* print */
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(BGDYIN,&ApplVar.Dept.Print,3,0);
        break;
    default:
#if defined(CASE_INDIA)
        SavePlu_DeptEditLOG();
#endif
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteDept();
}


void ProgGroup()
{
    WORD tmpW;

    if (Appl_ProgNumber >= ApplVar.AP.Group.Number)
    {
        if (ApplVar.AP.Group.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.GroupNumber = Appl_ProgNumber;
    ReadGroup();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* group name */
        Appl_MaxEntry=ApplVar.AP.Group.CapSize ;
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        if (GetString(MINCHENF, ApplVar.Group.Name, ApplVar.AP.Group.CapSize,0))
            break;
    case 3:     /* Family now used for maxentry */
        Appl_MaxEntry=2;
        tmpW = ApplVar.Group.Family;

        if (GetOpt(30, &tmpW, 99))
        {
            ApplVar.Group.Family = tmpW;
            break;
        }
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteGroup();
}


void ProgTend()
{
    if (Appl_ProgNumber >= ApplVar.AP.Tend.Number)
    {
        if (ApplVar.AP.Tend.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.TendNumber = Appl_ProgNumber;
    ReadTender();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* tendering name */
        Appl_BitNumber = 0;
        Appl_MaxEntry=ApplVar.AP.Tend.CapSize ;
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        if (GetString(MINCHENF, ApplVar.Tend.Name, ApplVar.AP.Tend.CapSize,0))
            break;
    case 3:     /* options */
    case 4:     /* options */
    case 5:     /* options */
    case 6:     /* options */
    case 7:     /* options */
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(DKQXIANGF+Appl_ProgLine-3,&ApplVar.Tend.Options,1+(Appl_ProgLine-3),0);
        break;
//	case 4:     /* print */
//		Appl_MaxEntry=8;
//	    if (GetOpt(3, &ApplVar.Tend.Print, 8))
//		break;
    case 8:     /* Drawer */
        Appl_MaxEntry=8;
        GetOpt(21, &ApplVar.Tend.Drawer, ApplVar.AP.Draw.Number);
        break;
    case 9:     /* Over OTD */
        Appl_MaxEntry=8;
        GetOpt(22, &ApplVar.Tend.Over, ApplVar.AP.Draw.Number);
        break;
#if defined(CASE_QRCODE)
    case 10://输入二维码
        if (Appl_ProgNumber==(TEND_WXPAY-TEND-1) || (Appl_ProgNumber==TEND_ALPAY-TEND-1))
        {
            Appl_BitNumber = 0;
            Appl_MaxEntry=MAX_QRCODE-1;
            SETBIT(ApplVar.MyFlags,HANZIMODE);
            if (GetString(MsgQRCODE, ApplVar.QRCode[Appl_ProgNumber-(TEND_WXPAY-TEND-1)],Appl_MaxEntry,0))
            {
                ApplVar.QRCode[Appl_ProgNumber-(TEND_WXPAY-TEND-1)][MAX_QRCODE-1]=0;
                break;
            }
        }
#endif
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteTender();
}


void ProgPoRa()
{
    if (Appl_ProgNumber >= ApplVar.AP.PoRa.Number)
    {
        if (ApplVar.AP.PoRa.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.PoRaNumber = Appl_ProgNumber;
    ReadPoRa();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* pora name */
        Appl_MaxEntry=ApplVar.AP.PoRa.CapSize;
        Appl_BitNumber = 0;
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        if (GetString(MINCHENF, ApplVar.PoRa.Name, ApplVar.AP.PoRa.CapSize,0))
            break;
    case 3:     /* options */
    case 4:     /* options */
    case 5:     /* options */
    case 6:     /* options */
#if defined(CASE_INDIA)
    case 7:     /* Card # must */
#endif
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(DKQXIANGP+Appl_ProgLine-3,&ApplVar.PoRa.Options,1+(Appl_ProgLine-3),0);
        break;
//	case 4:     /* print */
//		Appl_MaxEntry=8;
//	    if (GetOpt(3, &ApplVar.PoRa.Print, 8))
//			break;
#if defined(CASE_INDIA)
    case 8:     /* Drawer 1 */
#else
    case 7:     /* Drawer 1 */
#endif
        Appl_MaxEntry=8;
        GetOpt(21, &ApplVar.PoRa.Drawer, ApplVar.AP.Draw.Number);
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WritePoRa();
}


void ProgCorrec()
{
    if (Appl_ProgNumber >= ApplVar.AP.Correc.Number)
    {
        if (ApplVar.AP.Correc.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.CorrecNumber = Appl_ProgNumber;
    ReadCorrec();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        Appl_MaxEntry=ApplVar.AP.Correc.CapSize ;
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_BitNumber = 0;
        if (GetString(MINCHENF, ApplVar.Correc.Name, ApplVar.AP.Correc.CapSize,0))
            break;
//	case 3:     /* options */
/*		Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        if (!Appl_BitNumber)
            Appl_BitNumber = ApplVar.Correc.Options+1;
        if (Appl_BitNumber > 4 )
            Appl_BitNumber = 1;
        ApplVar.Correc.Options = Appl_BitNumber - 1;
        memcpy(ProgLineMes , Msg[LEIXINGF].str,strlen(Msg[LEIXINGF].str));
        memcpy(ProgLineMes+ SETINPUTWD - 5,ApplVar.TXT.Caption[Appl_BitNumber - 1 + 80],5);
        break;*/
//	case 4:     /* print */
//		Appl_MaxEntry=8;
//	    if (GetOpt(3, &ApplVar.Correc.Print, 8))
//		break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteCorrec();
}

void ProgDisc()
{
    if (Appl_ProgNumber >= ApplVar.AP.Disc.Number)
    {
        if (ApplVar.AP.Disc.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.DiscNumber = Appl_ProgNumber;
    ReadDisc();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.Disc.CapSize;
        if (GetString(MINCHENF, ApplVar.Disc.Name, ApplVar.AP.Disc.CapSize,0))
            break;
    case 3:
        Appl_MaxEntry=8;
        if (TESTBIT(ApplVar.Disc.Options, BIT0))    /* amount ? */
            GetOpt(13, ApplVar.Disc.Fixed, sizeof(ApplVar.Disc.Fixed));
        else
            GetOpt(15, ApplVar.Disc.Fixed, sizeof(ApplVar.Disc.Fixed));
        break;
    case 4: /* max */
        Appl_BitNumber = 0;
        Appl_MaxEntry=8;
        if (TESTBIT(ApplVar.Disc.Options, BIT0))    /* amount ? */
            GetOpt(14, ApplVar.Disc.Max, sizeof(ApplVar.Disc.Max));
        else
            GetOpt(16, ApplVar.Disc.Max, sizeof(ApplVar.Disc.Max));
        break;
    case 5:     /* options */
        //Appl_EntryCounter = 0;
        if (!Appl_BitNumber)
            Appl_BitNumber = (ApplVar.Disc.Options & 0x3)+1;
        if (Appl_BitNumber > 3 )
            Appl_BitNumber = 1;
        ApplVar.Disc.Options &= 0xfc;
        ApplVar.Disc.Options |= (Appl_BitNumber - 1);
        DispStrXY(Msg[LEIXING].str,0,0xff);
        Appl_MaxEntry=strlen(Msg[GDZHKOU+Appl_BitNumber - 1].str);
        if (Appl_MaxEntry<SETINPUTWD-1)
        {
            Appl_MaxEntry=SETINPUTWD-Appl_MaxEntry-1;
            strcpy(ProgLineMes+ Appl_MaxEntry,Msg[GDZHKOU+Appl_BitNumber - 1].str);
        }
        else
            strcpy(ProgLineMes,Msg[GDZHKOU+Appl_BitNumber - 1].str);
        Appl_MaxEntry=0;
        break;
    case 6:
/*		Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        if (!Appl_BitNumber)
            Appl_BitNumber = ((ApplVar.Disc.Options>>2) & 0x07)+1;
        if (Appl_BitNumber > 5)
            Appl_BitNumber = 5;
        ApplVar.Disc.Options &= 0xc3;
        ApplVar.Disc.Options |= ((Appl_BitNumber - 1)<<2);
        memcpy(ProgLineMes , ApplVar.TXT.LineCap[33], lCAPWIDTH);
        memcpy(ProgLineMes+ SETINPUTWD - 6,ApplVar.TXT.Caption[Appl_BitNumber - 1 + 87],6);
        break;*/
        Appl_ProgLine = 7;
    case 7:
    case 8:
    case 9:
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(DYZHKE+Appl_ProgLine- 7,&ApplVar.Disc.Options,1+(Appl_ProgLine- 7 + 5),0);
        break;
//	case 6:     /* print */
//		Appl_MaxEntry=8;
//	    if (GetOpt(3, &ApplVar.Disc.Print, 8))
//		break;
    case 10:     /* tax */
        Appl_MaxEntry=8;
        if (GetOpt(6, &ApplVar.Disc.Tax, ApplVar.AP.Tax.Number))
            break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteDisc();
}



void ProgCurr()
{
    if (Appl_ProgNumber >= ApplVar.AP.Curr.Number)
    {
        if (ApplVar.AP.Curr.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.CurrNumber = Appl_ProgNumber;
    ReadCurr();

    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.Curr.CapSize;
        if (GetString(MINCHENF, ApplVar.Curr.Name, ApplVar.AP.Curr.CapSize,0))
            break;
    case 3:
        Appl_MaxEntry=8;
        GetOpt(18, ApplVar.Curr.BuyRate, sizeof(ApplVar.Curr.BuyRate));
        break;
    case 4:
        Appl_BitNumber = 0;
        Appl_MaxEntry=8;
        GetOpt(19, ApplVar.Curr.SellRate, sizeof(ApplVar.Curr.SellRate));
        break;
    case 5:     /* options */
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(MCHJZHLIN,&ApplVar.Curr.Options,1+(Appl_ProgLine- 4),0);
        break;
    case 6:
        Appl_MaxEntry=8;
        GetOpt(21, &ApplVar.Curr.Drawer, ApplVar.AP.Draw.Number);
        break;
    case 7:
        Appl_MaxEntry=8;
        GetOpt(26, &ApplVar.Curr.Prefix1, 255);
        break;
    case 8:
        Appl_MaxEntry=8;
        GetOpt(27, &ApplVar.Curr.Prefix2, 255);
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteCurr();
}

void ProgDraw()
{
    if (Appl_ProgNumber >= ApplVar.AP.Draw.Number)
    {
        if (ApplVar.AP.Draw.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.DrawNumber = Appl_ProgNumber;
    ReadDrawer();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        Appl_BitNumber = 0;
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.Draw.CapSize;
        if (GetString(MINCHENF, ApplVar.Draw.Name, ApplVar.AP.Draw.CapSize,0))
            break;
//    case 3:     /* options */
//        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
//        CheckBitValue(WXSHDIAN+1,&ApplVar.Draw.Options,2,0);
//        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteDrawer();
}


void ProgPbF()
{
    if (!ApplVar.AP.Pb.NumberOfPb || Appl_ProgNumber >= ApplVar.AP.Pb.Number)
    {
        if (ApplVar.AP.Pb.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.PbFNumber = Appl_ProgNumber;
    ReadPbF();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.Pb.CapSize;
        if (GetString(MINCHENF, ApplVar.PbF.Name, ApplVar.AP.Pb.CapSize,0))
            break;
    case 3:     /* options */
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        if (!Appl_BitNumber)
            Appl_BitNumber = ApplVar.PbF.Options+1;
        if (Appl_BitNumber > 11)
            Appl_BitNumber = 1;
        ApplVar.Disc.Options = Appl_BitNumber - 1;
        DispStrXY(Msg[LEIXING].str,0,0xff);
        strcpy(ProgLineMes+ 3,Msg[ApplVar.Disc.Options+KTZHJIE].str);
        break;
//	    case 4:     /* print */
//			Appl_MaxEntry=8;
//			if (GetOpt(3, &ApplVar.PbF.Print, 8))
//			    break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WritePbF();
}

void ProgPbInfo()
{
    if (Appl_ProgNumber > ApplVar.AP.Pb.NumberOfPb || !ApplVar.AP.Pb.Text)
    {
        if (ApplVar.AP.Pb.NumberOfPb==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.PbNumber = Appl_ProgNumber + 1;

    if (!ApplVar.AP.Pb.Text)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        return;
    }
    PbTotal(ApplVar.PbNumber, 0);     /* read */
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* ApplVar.PB text */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.Pb.Text;
        GetString(MINCHENF, ApplVar.PB.Text, ApplVar.AP.Pb.Text,0);
        break;
    case 3:
        Appl_MaxEntry=16;
        if (!TESTBIT(PBINFO, BIT2) && !(PBPRINT & 0x80)
            && (ApplVar.AP.Pb.Random & 0x0f))  /* random number ? */
        {
            if (GetOpt(31, ApplVar.PB.Random, ApplVar.AP.Pb.Random & 0x0f))
                break;
        }
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    PbTotal(ApplVar.PbNumber, 3);     /* write text + Random */
}

void ProgTax()
{
    if (Appl_ProgNumber >= ApplVar.AP.Tax.Number)
    {
        if (ApplVar.AP.Tax.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.TaxNumber = Appl_ProgNumber;
    ReadTax();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.Tax.CapSize;
        if (GetString(MINCHENF, ApplVar.Tax.Name, ApplVar.AP.Tax.CapSize,0))
            break;
    case 3:
        Appl_BitNumber = 0;
        Appl_MaxEntry=8;
        GetOpt(17, ApplVar.Tax.Rate, sizeof(ApplVar.Tax.Rate));
        break;

    case 7:     /* 为GST类税*/
        Appl_ProgLine = 8;//ccr2015-08-19
    case 4:     /* 营业/增值 */
    case 5:     /* 打印税率项 */
    case 6:     /* 打印0税项 */
    case 8:     /* 打印税额 */
//	    case 9:     /* options */
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(YYZZHI+Appl_ProgLine-4,&ApplVar.Tax.Options,1+(Appl_ProgLine-4),0);
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteTax();
}


void ProgClerk()
{
    if (Appl_ProgNumber >= ApplVar.AP.Clerk.Number)
    {
        if (ApplVar.AP.Clerk.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.ClerkNumber = Appl_ProgNumber + 1;
    ReadClerk();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        Appl_MaxEntry=ApplVar.AP.Clerk.CapSize;
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_BitNumber = 0;
        if (GetString(MINCHENF, ApplVar.Clerk.Name, ApplVar.AP.Clerk.CapSize,0))
            break;
    case 3:
        if (TESTBIT(CLERKFIX,BIT5))
        {
            Appl_MaxEntry=sizeof(ApplVar.Clerk.Passwd) * 2;
//				SETBIT(ApplVar.MyFlags,PWDMODE);
            Appl_BitNumber = 0;
            GetBCDValue(MIMAF,ApplVar.Clerk.Passwd,3,TRUE);
            break;
        }
        else
            Appl_ProgLine = 4;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
#if !defined(DD_LOCK)//ccr090526
//ccr2016-02-16    case 11:
#endif
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(QXXZHI+Appl_ProgLine-4,&ApplVar.Clerk.Options,1+(Appl_ProgLine-4),YN);
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteClerk();
}


void ProgSalPer()
{
#if (salNumber)
    if (Appl_ProgNumber >= ApplVar.AP.SalPer.Number)
    {
        if (ApplVar.AP.SalPer.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.SalPerNumber = Appl_ProgNumber + 1;
    ReadSalPer();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        Appl_MaxEntry=ApplVar.AP.SalPer.CapSize;
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        if (GetString(MINCHENF, ApplVar.SalPer.Name, ApplVar.AP.SalPer.CapSize,0))
            break;
    default:
        Appl_ProgLine = 0;
        Appl_MaxEntry=4;
        break;
    }
    WriteSalPer();
#endif
}

void ProgZone()
{
    if (Appl_ProgLine == 1)
        Appl_ProgLine = 2;//Like Date
    else if (Appl_ProgLine > 2)
    {
//        ClearLine(SCREENLN-2);
        Appl_ProgNumber++;
        Appl_ProgLine = 2;
    }
    if (Appl_ProgNumber >= ApplVar.AP.Zone.Number)
    {
        if (ApplVar.AP.Zone.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    GetOpt(20, &ApplVar.AP.Zone.Start[Appl_ProgNumber], 2);
    memset(SysBuf,0,5);
    SysBuf[8] = WORDtoASCL(SysBuf,Appl_ProgNumber+1);
    DispStrXY(SysBuf,10-SysBuf[8],0xff);
    Appl_MaxEntry=5;
}

void ProgModi()
{
    RESETBIT(ApplVar.MyFlags,HANZIMODE);
    if (Appl_ProgLine == 1)
        return;
    else if (Appl_ProgLine > 2)
    {
        Appl_ProgNumber++;
        Appl_ProgLine = 2;
    }
    if (Appl_ProgNumber >= ApplVar.AP.Modi.Number)
    {
        if (ApplVar.AP.Modi.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.ModiNumber = Appl_ProgNumber;
    ReadModi();

    SETBIT(ApplVar.MyFlags,HANZIMODE);

    Appl_MaxEntry=ApplVar.AP.Modi.CapSize;
    GetString(SETMODIF, ApplVar.Modi.Name, ApplVar.AP.Modi.CapSize,Appl_ProgNumber+1);
    WriteModi();
}

/**
 * 删除当前票头,其后续票头往前异动
 *
 * @author EutronSoftware (2016-01-19)
 *
 * @param hIdx :票头序号(0..7)
 */
void DeleteHeader(BYTE hIdx)
{
    if (hIdx<7)
        memcpy(ApplVar.TXT.Header[hIdx],ApplVar.TXT.Header[hIdx+1],(PRTLEN+1)*(8-hIdx));
    memset(ApplVar.TXT.Header[7],0,(PRTLEN+1));
}

void ProgHeader()
{
    RESETBIT(ApplVar.MyFlags,HANZIMODE);
    if (Appl_ProgLine == 1)
        return;
    else if (Appl_ProgLine > 2 && Appl_ProgNumber<8)
    {
        Appl_ProgNumber++;
        Appl_ProgLine = 2;
        //ccr2016-01-22>>>>>>>>>
        if (Appl_ProgNumber>0 && Appl_ProgNumber<8 && ApplVar.TXT.Header[Appl_ProgNumber-1][0]==0)
        {
            Appl_ProgNumber = 8;
        }//ccr2016-01-22<<<<<<<<<
    }

    //ccr2016-01-22>>>>>>>>>>>>>>>>>
    Editor.DispY = ApplVar.sCursor.y;//ccr20160601
    if (Appl_ProgNumber == 8)
    {
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        if (Appl_ProgLine==2)
        {
            CheckBitValue(HEADCENTER,&DBLHIPRN,4,0);
            return;
        }
        else if (Appl_ProgLine==3)
        {
            CheckBitValue(HEADDBLHIGH,&AM_PM,3,0);
            return;
        }
        else if (Appl_ProgLine==4)
        {//选择票头图片
            DispStrXY(GrapType[0],0,0xff);
            ApplVar.Graph[0].PictNo = SelectGraphic(ApplVar.Graph[0].PictNo);
            return;
        }
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
            return;
        }
    }
    //ccr2016-01-22<<<<<<<<<<<<<<<
    SETBIT(ApplVar.MyFlags,HANZIMODE);
    GetString(SETHEAD, ApplVar.TXT.Header[Appl_ProgNumber], PRTLEN,Appl_ProgNumber+1);
    ApplVar.TXT.Header[Appl_ProgNumber][PRTLEN]=0;
    Appl_MaxEntry=sizeof(ApplVar.TXT.Header[0]);
}
/**
 * 删除当前票尾,其后续票头往前异动
 *
 * @author EutronSoftware (2016-01-19)
 *
 * @param hIdx :票头序号(0..7)
 */
void DeleteTrailer(BYTE hIdx)
{
    if (hIdx<5)
        memcpy(ApplVar.TXT.Trailer[hIdx],ApplVar.TXT.Trailer[hIdx+1],(PRTLEN+1)*(6-hIdx));
    memset(ApplVar.TXT.Trailer[5],0,(PRTLEN+1));
}

void ProgTrailer()
{
    RESETBIT(ApplVar.MyFlags,HANZIMODE);
    if (Appl_ProgLine == 1)
        return;
    else if (Appl_ProgLine > 2 && Appl_ProgNumber<6)
    {
        Appl_ProgNumber++;
        Appl_ProgLine = 2;
        if (Appl_ProgNumber>0 && Appl_ProgNumber<6 && ApplVar.TXT.Trailer[Appl_ProgNumber-1][0]==0)
        {
            Appl_ProgNumber = 6;
        }
    }
    //ccr2016-01-22>>>>>>>>>>>>>>>>>>>>>>>>>
    Editor.DispY = ApplVar.sCursor.y;//ccr20160601
    if (Appl_ProgNumber == 6)
    {
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        if (Appl_ProgLine==2)
        {
            CheckBitValue(TRAILCENTER,&DBLHIPRN,5,0);
            return;
        }
        else if (Appl_ProgLine==3)
        {
            CheckBitValue(TRAILDBLHIGH,&AM_PM,4,0);
            return;
        }
        else if (Appl_ProgLine==4)
        {//选择票尾图片
            DispStrXY(GrapType[2],0,0xff);
            ApplVar.Graph[GRAFESTMAX+1].PictNo = SelectGraphic(ApplVar.Graph[GRAFESTMAX+1].PictNo);
            return;
        }
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
            return;
        }
    }
    //ccr2016-01-22<<<<<<<<<<<<<<<<<<<<
    SETBIT(ApplVar.MyFlags,HANZIMODE);
    GetString(SETTRAIL, ApplVar.TXT.Trailer[Appl_ProgNumber], PRTLEN,Appl_ProgNumber+1);
    ApplVar.TXT.Trailer[Appl_ProgNumber][PRTLEN]=0;
    Appl_MaxEntry=sizeof(ApplVar.TXT.Trailer[0]);
}
/**
 * 删除当前据收票尾,其后续票头往前异动
 *
 * @author EutronSoftware (2016-01-19)
 *
 * @param hIdx :票头序号(0..7)
 */
void DeleteSlipHead(BYTE hIdx)
{
    if (hIdx<5)
        memcpy(ApplVar.TXT.SlipHeader[hIdx],ApplVar.TXT.SlipHeader[hIdx+1],(SHEADWIDTH+1)*(6-hIdx));
    memset(ApplVar.TXT.SlipHeader[5],0,(SHEADWIDTH+1));
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void ProgSlipHead()            //lyq modified for ds_1000 20040324 start
{
    RESETBIT(ApplVar.MyFlags,HANZIMODE);
    if (Appl_ProgLine == 1)
        return;
    else if (Appl_ProgLine > 2)
    {
        Appl_ProgNumber++;
        Appl_ProgLine = 2;
        if (Appl_ProgNumber>0  && Appl_ProgNumber<6 && ApplVar.TXT.SlipHeader[Appl_ProgNumber-1][0]==0)
        {
            Appl_ProgNumber = 6;
        }
    }
    if (Appl_ProgNumber > 5)
    {
        Appl_ProgNumber = -1;
        Appl_ProgLine = 0;
        return;
    }
    SETBIT(ApplVar.MyFlags,HANZIMODE);
    GetString(SETSHEAD, ApplVar.TXT.SlipHeader[Appl_ProgNumber], sizeof(ApplVar.TXT.SlipHeader[0]),Appl_ProgNumber+1);
    Appl_MaxEntry=sizeof(ApplVar.TXT.SlipHeader[0]);
}                              //lyq modified for ds_1000 20040324 end

/**
 * 由NewProgSysFlag取代此设置函数
 *
 * @author EutronSoftware (2016-08-03)
 */
void ProgSysFlag()
{
    BYTE BitV, i;
    BYTE updownkey;
    short   sIdx;

    SetEditPosition(SETUPDISPX,CursY,0,NULL);
    if (Appl_ProgLine==1)// || Appl_ProgLine>=SYSUSED+2)
    {
        mClearScreen();
        return;//Appl_ProgLine = 2;
    }
    if (Appl_ProgLine>=2 && Appl_ProgLine<SYSUSED+2)
    {
        sIdx = SysFlagUsed[Appl_ProgLine-2].Index;
        DispStrXY(SysFlagUsed[Appl_ProgLine-2].Name,0,0xff);//SysOpt

        if (sIdx == CONTRAST && SysFlagUsed[Appl_ProgLine-2].Bit==0)// && SysFlagUsed[Appl_ProgLine-2].Max == 63)
        {//调对比度,//ApplVar.AP.Flag[CONTRAST]
            //set contrast by pageup & pagedown.
            memset(SysBuf,0,7);
            strcpy(SysBuf,cSpecialSymbol3);

            for (updownkey = 1;;)
            {
                i = (DENSITY & 0x0f);
                if (updownkey)
                {
                    SysBuf[3] = (i % 10)+'0';
                    if (i>9)
                        SysBuf[2] = (i / 10)+'0';
                    else
                        SysBuf[2] = ' ';
                    DispStrXY(SysBuf, SETUPDISPX ,0xff);
                }
                Save_Config(true);
                while (!KbHit());
                updownkey = Getch();

                switch (updownkey)
                {
                case cUP:
                    if((DENSITY & 0x0f)<14)// VolumeVal <= 0x3A)
                        DENSITY++;
                    mSetContrast((DENSITY & 0x0f));
                    break;
                case cDOWN:
                    if((DENSITY & 0x0f)>0)// VolumeVal <= 0x3A)
                        DENSITY--;
                    mSetContrast((DENSITY & 0x0f));
                    break;
                default:
                    if (updownkey == ApplVar.AP.FirmKeys[ID_CANCEL] ||
                        updownkey == ApplVar.AP.FirmKeys[ID_ENTER])
                    {
                        ApplVar.KeyNo = updownkey;
                        CheckFirmKey();
                        return;
                    }
                    updownkey = 0;
                    break;
                }
            }
        }

        if (SysFlagUsed[Appl_ProgLine-2].Bit)
        {
            BitV = BIT0<<(SysFlagUsed[Appl_ProgLine-2].Bit-1);
            Appl_MaxEntry=1;      //hf 20060105 0;
            //Appl_EntryCounter = 0;
            if (!Appl_BitNumber)
                Appl_BitNumber = ((ApplVar.AP.Flag[sIdx] & BitV)!=0)+1;
            if (Appl_BitNumber > 2)
                Appl_BitNumber = 1;

            if (Appl_BitNumber ==2)
            {
                ProgLineMes[SETINPUTWD-3] = 'y' ^ SysFlagUsed[Appl_ProgLine-2].Invert;
                SETBIT(ApplVar.AP.Flag[sIdx],BitV);
            }
            else
            {
                ProgLineMes[SETINPUTWD-3] = 'n' ^ SysFlagUsed[Appl_ProgLine-2].Invert;
                RESETBIT(ApplVar.AP.Flag[sIdx], BitV);
            }

            if (ProgLineMes[SETINPUTWD-3] == 'y')
                strcpy(ProgLineMes+SETINPUTWD-4,Msg[KAI].str);
            else
                strcpy(ProgLineMes+SETINPUTWD-4,Msg[GUAN].str);
        }
        else
        {
            Appl_MaxEntry=3;  Appl_BitNumber = 0;
            if (sIdx==62 || sIdx==CONTRAST)//复制收据次数或对比度
            {//高4位为标志,低4为数值  //
                i = ApplVar.AP.Flag[sIdx] & 0xf0;   //备份标志位
                ApplVar.AP.Flag[sIdx] &= 0x0f;      //为了显示数值，先清除高四位
            }
            else
                i = 0;
            if (Appl_EntryCounter)
            {
                if (ApplVar.NumberEntry>SysFlagUsed[Appl_ProgLine-2].Max)
                {
                    ApplVar.AP.Flag[sIdx] |= i;
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                    return;
                }
                ApplVar.AP.Flag[sIdx] = ApplVar.NumberEntry;
            }
            WORDtoASC(ProgLineMes + SETINPUTWD-1, ApplVar.AP.Flag[sIdx]);
            ApplVar.AP.Flag[sIdx] |= i;
        }
        Save_Config(true);
    }
    else
    {
        Appl_MaxEntry=8;
        Appl_ProgLine = 1;
    }

    InitSysFlag();
}


void ProgSysMes()
{
    if (Appl_ProgNumber > 15)
    {
        Appl_ProgNumber = -1;
        Appl_ProgLine = 0;
    }
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=16;
        GetString(MINCHENF, ApplVar.TXT.ReportType[Appl_ProgNumber], 16,0);
        /* caption changed */
        break;
    default:
        RESETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_ProgNumber++;
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}

void ProgReport()
{
    short i;

    if (Appl_ProgNumber > XZNUM-1)
    {
        Appl_ProgNumber = -1;
        Appl_ProgLine = 0;
        return;
    }
    i = XZTitle[Appl_ProgNumber].Index-1;

    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=16;
        GetString(MINCHENF, ApplVar.AP.ReportList[i].Name, 16,0);
        Appl_BitNumber = 0;
        break;
//	case 3:
//		Appl_MaxEntry=8;
//	    if (GetOpt(3, &ApplVar.AP.ReportList[i].PrintLayOut, 8))
//			break;
    case 3:     /* options */
//lyq delete 2003\10\27 start
//	case 4:     /* options */
//	case 5:     /* options */
//	case 6:     /* options */
//lyq delete 2003\10\27 end
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(DKQXIANGF,&ApplVar.AP.ReportList[i].Options,1+(Appl_ProgLine-3),0);
        break;
    case 4:    /* period */
        Appl_MaxEntry=8;
        GetOpt(24, &ApplVar.AP.ReportList[i].Period, 3);
        break;
    case 5:     /* pointer type */
        Appl_MaxEntry=8;
        GetOpt(25, &ApplVar.AP.ReportList[i].PointerType, 4);
        break;
    default:     /* link */
        if (Appl_ProgLine < (6+16))
            GetOpt(28, &ApplVar.AP.ReportList[i].Link[Appl_ProgLine - 6], REPORTTYPE_MAX+1);//9--6 lyq
        else
            Appl_ProgLine = 0;
        break;
    }
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#if defined(CASE_FORHANZI)
#define DATE_Y  0
#else
#define DATE_Y  1
#endif
void ProgDate()
{
    switch (Appl_ProgLine)
    {
    case 1:
        Appl_ProgLine = 2;
    case 2:
#if defined(CASE_FORHANZI)
        Appl_ProgLine = 3;
#else//英文版本增加日期格式设置
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        if (!Appl_BitNumber)
            Appl_BitNumber = (TIME_DATE & 0x3)+1;
        if (Appl_BitNumber > 3 )
            Appl_BitNumber = 1;
        TIME_DATE = (Appl_BitNumber - 1);
        DispStrXY(Msg[MsgDATETYPE].str,0,0xff);
//        strcpy(ProgLineMes+ SETINPUTWD - strlen(Msg[MsgDDMMYYYY+Appl_BitNumber - 1].str) - 1,Msg[MsgDDMMYYYY+Appl_BitNumber - 1].str);
        strcpy(ProgLineMes,Msg[MsgDDMMYYYY+Appl_BitNumber - 1].str);
        SetEditPosition(SETUPDISPX,0,0,NULL);
        break;
#endif
    case 3:
        Appl_MaxEntry=10;

        CheckTime(TRUE);
        MemSet(ProgLineMes, SCREENWD, ' ');
        CopyFrStr(ProgLineMes,Msg[Now.dow+MONDAY-1].str);
        DispStrXY(ProgLineMes,SCREENWD-9,DATE_Y+1);
        GotoXY(SETDATEDISPX, DATE_Y);       //GotoXY(SETUPDISPX,0);
        Appl_EntryCounter = 0;

        SetEditPosition(SETDATEDISPX,CursY,0,NULL);

        DispStrXY(Msg[RIQIF].str,0,0xff);
        MemSet(ProgLineMes, sizeof(ProgLineMes), ' ');
        memcpy(ProgLineMes,DateAsci+4, 10); //hf 20060105 memcpy(ProgLineMes+1,DateAsci+4, 10);
        Appl_ProgLine = 4;
        break;
    case 4:     /* set date */
        if (Appl_EntryCounter>0)
        {
            if (TESTBIT(ApplVar.MyFlags,ZREPORT))   /* z report taken ? */
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI13);
                KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];
            }
            else
            {
                NewTimeDate(1);

                CheckTime(TRUE | 0x80);//STM3无法立即读取新设置的日期

                MemSet(ProgLineMes, SCREENWD, ' ');
                CopyFrStr(ProgLineMes,Msg[Now.dow+MONDAY-1].str);
                DispStrXY(ProgLineMes,SCREENWD-9,DATE_Y+1);

                Appl_ProgLine = 2;//确保下一次输入的内容显示在屏幕内容区
                GotoXY(SETDATEDISPX, DATE_Y);       //GotoXY(SETUPDISPX,0);
                Appl_EntryCounter = 0;

                SetEditPosition(SETDATEDISPX,CursY,0,NULL);
                MemSet(ProgLineMes, sizeof(ProgLineMes), ' ');
                memcpy(ProgLineMes,DateAsci+4, 10); //hf 20060105 memcpy(ProgLineMes+1,DateAsci+4, 10);
                //KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];
            }
        }
        else
            KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}

void ProgTime()
{
    switch (Appl_ProgLine)
    {
    case 1:
        Appl_ProgLine = 2;
    case 2:
        Appl_MaxEntry=8;
        CheckTime(TRUE);
        GotoXY(SETDATEDISPX+2,0);      //GotoXY(SETUPDISPX,0);
        Appl_EntryCounter = 0;
        SetEditPosition(SETDATEDISPX+2,CursY,0,NULL);
        DispStrXY(Msg[SHIJIANF].str,0,0xff);
        MemSet(ProgLineMes, sizeof(ProgLineMes), ' ');
        memcpy(ProgLineMes,TimeAsci, 8);
        Appl_ProgLine = 3;
        break;
    case 3:     /* set time */
        if (Appl_EntryCounter>0)
        {
            if (TESTBIT(ApplVar.MyFlags,ZREPORT))   /* z report taken ? */
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI13);
            }
            else
            {
                Appl_ProgLine = 2;//确保下一次输入的内容显示在屏幕内容区
                NewTimeDate(2);
                CheckTime(TRUE);
                SetEditPosition(SETDATEDISPX+2,CursY,0,NULL);
                MemSet(ProgLineMes, sizeof(ProgLineMes), ' ');
                memcpy(ProgLineMes,TimeAsci, 8);            }
        }
        else
            KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];//
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}

#if 0 //ccr2016-12-14>>>>>>>>
void ProgPort(BYTE port)
{
    short i;
    BYTE oldICPort;


    if (port>=2)
    {
        if (Appl_ProgLine==1)
        {
            Appl_ProgLine = 2;
            //strcpy(ProgLineMes,Msg[BAOLIU].str);
            ProgLineMes[0]=0;
            DispStrXY(Msg[BAOLIU].str,0,0);
        }
        else
        {
            Appl_MaxEntry=4;
            Appl_ProgLine = 0;
        }
    }
    else
    {
        Appl_PortNumber = port;
        ReadPort();
        switch (Appl_ProgLine)
        {
        case 1:
            Appl_ProgLine = 2;//Like date
        case 2:     /* Appl_Port type (Option)*/
            Appl_MaxEntry=1;    //hf 20060105 0;
            GotoXY(SETUPDISPX,0);
            Appl_EntryCounter = 0;

            if (Appl_BitNumber==0)
                Appl_BitNumber = Appl_Port.Type-'0';

            if (COMPUTER && COMPUTER_1 != port && Appl_BitNumber==1)
            {// 当有一个串口类型为连主机时，禁止其他串口连主机  ccr 050227//
                Appl_Port.Type = '2';
                Appl_BitNumber = '2'-'0';
            }

            GetOpt(44, &Appl_Port.Type, portTypeNum);
            if (COMPUTER && COMPUTER_1 != port && Appl_Port.Type=='1')
            {// 当有一个串口类型为连主机时，禁止其他串口连主机  //
                Appl_Port.Type = '2';
                Appl_BitNumber = '2'-'0';
                GetOpt(44, &Appl_Port.Type, portTypeNum);
            }
            break;
        case 3:     /* Protocl string */
            Appl_MaxEntry=5;
            Appl_BitNumber = 0;
            GetOpt(45, &Appl_Port.Prot[Appl_Port.Type - 0x31], 5);
            break;
        default:
            Appl_MaxEntry=4;
            Appl_ProgLine = 0;
            break;
        }
        COMPUTER = 0;
        BALANCE &= 0xf8;
        BARCODE &= 0xf8;
        PCCounter = CCDCounter = BalCounter = 0;

        WritePort();
        //SetComm(Appl_PortNumber);
        EmptyComm(Appl_PortNumber);

        if (Appl_Port.Type == '1')
        {
            COMPUTER = Appl_PortNumber+1;
        }
        else if (Appl_Port.Type == '2')
            BARCODE = (BARCODE & 0xf8) |(Appl_PortNumber+1);
        else if (Appl_Port.Type == '3')
            BALANCE = (BALANCE & 0xf8) | (Appl_PortNumber+1);
#if (CASE_EPOS)
        else if (Appl_Port.Type == '6')  //cc epos
            ApplVar.ePosPort = Appl_PortNumber+1;
#endif

        for (Appl_PortNumber = 0;Appl_PortNumber < ApplVar.AP.Port.Number;Appl_PortNumber++)
        {
            ReadPort();
            switch (Appl_Port.Type)
            {
            case '1':
                if (!COMPUTER)
                    COMPUTER = Appl_PortNumber+1;
                break;
            case '2':
                if (!(BARCODE & 0x07))
                    BARCODE = (BARCODE & 0xf8) | (Appl_PortNumber+1);
                break;
            case '3':
                if (!(BALANCE & 0x07))
                    BALANCE = (BALANCE & 0xf8) | (Appl_PortNumber+1);
                break;
#if (CASE_EPOS)
            case '6'://ccr epos
                if (!ApplVar.ePosPort)
                    ApplVar.ePosPort = Appl_PortNumber+1;
                break;
#endif
            }
        }
    }
}
#endif
//设置中奖打印图象和图片上叠加的文字   //
void ProgPrnGraph()
{
    BYTE    sIdx;

    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2://Festival number
        //  为设置中奖打印图象  //
        if (!Appl_ProgNumber || Appl_ProgNumber>GRAFESTMAX)//设置中奖图象  //
            Appl_ProgNumber = 1;
        SetEditPosition(SETUPDISPX,CursY,0,NULL);
        DispStrXY(GrapSet[0], 0,0xff);
        WORDtoASC(ProgLineMes+SETINPUTWD-1,Appl_ProgNumber);
        break;
    case 3://Picture number
        sIdx = SelectGraphic(ApplVar.Graph[Appl_ProgNumber].PictNo);
        DispStrXY(GrapSet[1], 0,0xff);
        SetEditPosition(strlen(GrapSet[1])+1,CursY,0,NULL);
        ApplVar.Graph[Appl_ProgNumber].PictNo = sIdx;
        break;
    case 4://Date from
        Appl_MaxEntry=8;
        GetOpt(35, ApplVar.Graph[Appl_ProgNumber].DateFrom, sizeof(ApplVar.Graph[Appl_ProgNumber].DateFrom));
        break;
    case 5://date to
        Appl_MaxEntry=8;
        GetOpt(36, ApplVar.Graph[Appl_ProgNumber].DateTo, sizeof(ApplVar.Graph[Appl_ProgNumber].DateTo));
        break;
    default:
        if (Appl_ProgLine>=6 && Appl_ProgLine<GRAFESTMAX+6)
        {
            Appl_MaxEntry=PRTLEN;
            SETBIT(ApplVar.MyFlags,HANZIMODE);
            GetString(DYWZI,ApplVar.GrafTexts[Appl_ProgLine-6], PRTLEN,Appl_ProgLine-5);
        }
        else
        {
            Appl_ProgLine = 0;
            Appl_MaxEntry=4;
        }
        break;
    }
}
//IC会员卡功能设置
void ProgIC()  //ccr chipcard 2004-06-28
{
    unsigned char TempStr[15];
    static UnLong  sPOSCODE;


    switch (Appl_ProgLine)
    {
    case 1 :
        RESETBIT(ApplVar.MyFlags,SETCONFIRM);//ccr040809
        Appl_ProgLine=2;//break;
    case 2 ://ICCard Enable
        mClearScreen();
#if (DD_CHIPC==1)
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(ICCA,(BYTE *)&ApplVar.ICCardSet.Options,1,0);
        break;
#else
        Appl_ProgLine=2;
        RESETBIT(ApplVar.ICCardSet.Options,IC_CHIPCARD);
#endif
    case 3 ://Manager PASSWORD VERIFY
#if (DD_ECRVIP==0)
        if (!TESTBIT(ApplVar.ICCardSet.Options, IC_CHIPCARD))
        {
            Appl_MaxEntry=4;
            Appl_ProgLine = 0;
            break;
        }
        else
#endif
            Appl_ProgLine = 4;
    case 4 : //POSCODE SET
#if (DD_ECRVIP==1)
        if (!TESTBIT(ApplVar.ICCardSet.Options, IC_CHIPCARD))
        {
            Appl_ProgLine = 5;
        }
        else
#endif
        {
            Appl_MaxEntry=sizeof(ApplVar.ICCardSet.PosCode) * 2;
            Appl_BitNumber = 0;
            memset(TempStr,0,sizeof(TempStr));
            SETBIT(ApplVar.MyFlags,PWDMODE);
            if (!Appl_EntryCounter)
            {
                ULongToBCDValue(TempStr,ApplVar.ICCardSet.PosCode);
            }
            GetBCDValue(POSDAIMA,TempStr,4,TRUE);
            sPOSCODE = ApplVar.ICCardSet.PosCode;
            if (Appl_EntryCounter)
            {
                BCDValueToULong(TempStr,&sPOSCODE);
                ApplVar.ICCardSet.PosCode=sPOSCODE;
                sPOSCODE=0;
            }
            break;
        }
    case 5 : //POSCODE confirm
        if (ApplVar.ICCardSet.PosCode!=sPOSCODE)
        {//只有在输入了新的POS码后才进行校验
            Appl_MaxEntry=sizeof(ApplVar.ICCardSet.PosCode) * 2;
            Appl_BitNumber = 0;
            memset(TempStr,0,sizeof(TempStr));
            SETBIT(ApplVar.MyFlags,(PWDMODE | SETCONFIRM));
            GetBCDValue(POSDAIMA,TempStr,4,TRUE);
            sPOSCODE = 0;                              //ccr020809
            if (Appl_EntryCounter)
            {
                BCDValueToULong(TempStr,&sPOSCODE);
                if (sPOSCODE != ApplVar.ICCardSet.PosCode)
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI51);
            }
            break;
        }
        else
            Appl_ProgLine = 6;
    case 6 : //Type 0 Card Enable
    case 7 : //Type 1 Card Enable
    case 8 : //Type 2 Card Enable
    case 9 : //ApplVar.Report
    case 10 : //Discount
    case 11://Points
#if (DD_ECRVIP==1)
        if (!TESTBIT(ApplVar.ICCardSet.Options, IC_CHIPCARD))
        {
            Appl_ProgLine = 11;
        }
#endif
        RESETBIT(ApplVar.MyFlags,SETCONFIRM);//ccr040809
        Appl_MaxEntry=0;
        CheckBitValue(ZHEKOUCA+Appl_ProgLine-6,(BYTE *)&ApplVar.ICCardSet.Options,2+(Appl_ProgLine-6),0);
        break;
    case 12://Value
        if (TESTBIT(ApplVar.ICCardSet.Options,BIT6))
        {
            Appl_MaxEntry=6;
            Appl_BitNumber = 0;
            GetBCDValue(JDJER,ApplVar.ICCardSet.Value,3,FALSE);
            break;
        }
        else  Appl_ProgLine = 13;
    case 13://MIniNum
        if (TESTBIT(ApplVar.ICCardSet.Options,BIT6))
        {
            Appl_MaxEntry=6;
            Appl_BitNumber = 0;
            GetBCDValue(ZXJERF,ApplVar.ICCardSet.MiniNum,3,FALSE);
            break;
        }
        else
            Appl_ProgLine = 14;
    case 14://Toc_CC
#if (DD_ECRVIP==1)
        if (!TESTBIT(ApplVar.ICCardSet.Options, IC_CHIPCARD))
        {
            Appl_MaxEntry=4;
            Appl_ProgLine = 0;
        }
        else
#endif
        {
            Appl_MaxEntry=0;
            CheckBitValue(XJJIAN,(BYTE *)&ApplVar.ICCardSet.Options,8,0);
        }
        break;
    case 15:// 有效期  //
        Appl_MaxEntry=0;
        CheckBitValue(QYXYQI,(BYTE *)&ApplVar.ICCardSet.Options+1,1,0);//
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}



void ProgPromotion()
{
    WORD i;

    switch (Appl_ProgLine)
    {
    case 1:
        Appl_BitNumber = 0;
        break;//Appl_ProgLine = 2;
    case 2:     /* JOLLY ?*/
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(ZHJSHDING,&PROMOTION,1,YN);
        break;
    case 3:
        if (!TESTBIT(PROMOTION,BIT0))
        {
            Appl_MaxEntry=4;
            Appl_BitNumber = 0;
            i = ApplVar.AP.Promotion.Freq;
            GetWordValue(PINLV,&i,9999);
            ApplVar.AP.Promotion.Freq = i;
            break;
        }
        else
            Appl_ProgLine = 4;
    case 4:
        if (!TESTBIT(PROMOTION,BIT0))
        {
            Appl_MaxEntry=8;
            Appl_BitNumber = 0;
            GetBCDValue(ZXJERF,ApplVar.AP.Promotion.JollyMin,4,FALSE);
            break;
        }
        else
            Appl_ProgLine = 5;
    case 5:
        if (!TESTBIT(PROMOTION,BIT0))
        {
            Appl_MaxEntry=2;
            GetByteValue(TXXHAO,&ApplVar.AP.Promotion.GrapIdx,22);
            break;
        }
        else
            Appl_ProgLine = 6;
    case 6:
        if (!TESTBIT(PROMOTION,BIT0))
        {
            SETBIT(ApplVar.MyFlags,HANZIMODE);
            Appl_MaxEntry=sizeof(ApplVar.AP.Promotion.Memo)-1;
            GetString(SHUOMINF, ApplVar.AP.Promotion.Memo, sizeof(ApplVar.AP.Promotion.Memo)-1,0);
            break;
        }
        else
            Appl_ProgLine = 7;
    case 7:
        Appl_MaxEntry=0;
        //Appl_EntryCounter = 0;
        CheckBitValue(XFJIFEN,&PROMOTION,2,YN);
        break;
    case 8:
        if (!TESTBIT(PROMOTION,BIT1))
        {
            Appl_MaxEntry=8;
            Appl_BitNumber = 0;
            GetBCDValue(JDJER,ApplVar.AP.Promotion.PointVal,4,FALSE);
            break;
        }
        else
            Appl_ProgLine = 9;
    case 9:
        if (!TESTBIT(PROMOTION,BIT1))
        {
            Appl_MaxEntry=8;
            Appl_BitNumber = 0;
            GetBCDValue(ZXJERF,ApplVar.AP.Promotion.PointMin,4,FALSE);
            break;
        }
        else
            Appl_ProgLine = 9;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}

void ProgNULL()
{
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:
        break;
    default:
        Appl_ProgLine = 0;
        break;
    }
}

void ProgOFF()
{

    if (Appl_ProgNumber >= ApplVar.AP.OFFPrice.Number)
    {
        if (ApplVar.AP.OFFPrice.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.OFFNumber = Appl_ProgNumber;
    ReadOFFPrice();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* ApplVar.OFFPrice name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.OFFPrice.CapSize ;
        if (GetString(MINCHENF, ApplVar.OFFPrice.Name, ApplVar.AP.OFFPrice.CapSize,0))
            break;
    case 3:     /* OFF type */
        Appl_MaxEntry=1;
        if (GetOpt(34, &ApplVar.OFFPrice.Type, 3))
            break;
    case 4:     /* DateFrom */
        if (ApplVar.OFFPrice.Type>0)
        {
            Appl_MaxEntry=8;
            GetOpt(35, ApplVar.OFFPrice.DateFrom, sizeof(ApplVar.OFFPrice.DateFrom));
            break;
        }
    case 5:     /* DateTo */
        if (ApplVar.OFFPrice.Type>0)
        {
            Appl_MaxEntry=4;
            if (GetOpt(36, ApplVar.OFFPrice.DateTo, sizeof(ApplVar.OFFPrice.DateFrom)))
                break;
        }
    case 6:     /* TimeFrom */
        if (ApplVar.OFFPrice.Type>0)
        {
            Appl_MaxEntry=8;
            if (GetOpt(37, ApplVar.OFFPrice.TimeFrom, sizeof(ApplVar.OFFPrice.DateFrom)))
                break;
        }
    case 7:     /* TimeTo */
        if (ApplVar.OFFPrice.Type>0)
        {
            Appl_MaxEntry=4;
            if (GetOpt(38, ApplVar.OFFPrice.TimeTo, sizeof(ApplVar.OFFPrice.DateFrom)))
                break;
        }
    case 8:     /* WeekDay */
        if (ApplVar.OFFPrice.Type>0)
        {
            Appl_MaxEntry=7;
            GetOpt(39, &ApplVar.OFFPrice.WeekDay, 7);
            break;
        }
    case 9:     /* Discount or Package */
        if (ApplVar.OFFPrice.Type>0)
        {
            if (ApplVar.OFFPrice.Type==1)
            {
                Appl_MaxEntry=3;
                GetOpt(41, &ApplVar.OFFPrice.OFFVal.TYPE1.NumItems, 255);
                break;
            }
            else if (ApplVar.OFFPrice.Type==2)
            {
                Appl_MaxEntry=5;
                GetOpt(40, ApplVar.OFFPrice.OFFVal.Discount, 2);
                break;
            }
        }
        else
            Appl_ProgLine = 10;
    case 10:        //OFF ApplVar.Price for a unit
        if (ApplVar.OFFPrice.Type==1)
        {
            Appl_MaxEntry=10;
            GetOpt(42, ApplVar.OFFPrice.OFFVal.TYPE1.PriceU,ApplVar.AP.OFFPrice.PriUSize);
            break;
        }
        else
            Appl_ProgLine = 11;
    case 11:        //OFF ApplVar.Price for a package
        if (ApplVar.OFFPrice.Type==1)
        {
            Appl_MaxEntry=10;
            GetOpt(43, ApplVar.OFFPrice.OFFVal.TYPE1.PriceP,sizeof(ApplVar.OFFPrice.OFFVal.TYPE1.PriceP));
            break;
        }
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteOFFPrice();
}



void ProgAgree()
{

#if (agreeNumber)
    if (Appl_ProgNumber >= ApplVar.AP.Agree.Number)
    {
        if (ApplVar.AP.Agree.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }
    ApplVar.AgreeNumber = Appl_ProgNumber;
    ReadAgree();
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* ApplVar.Agree name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=ApplVar.AP.Agree.CapSize;
        if (GetString(MINCHENF, ApplVar.Agree.Name, ApplVar.AP.Agree.CapSize,0))
            break;
    case 3:     /* ApplVar.Agree name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=sizeof(ApplVar.Agree.Addr);
        GetString(DHHMAF,ApplVar.Agree.Addr, sizeof(ApplVar.Agree.Addr),0);
        break;
    case 4:     /* ApplVar.Agree ApplVar.Tax */
        Appl_MaxEntry=sizeof(ApplVar.Agree.Tax);
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        GetString(SHUILVF,ApplVar.Agree.Tax, sizeof(ApplVar.Agree.Tax),0);
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteAgree();
#endif
}

void ProgKPrn()
{
    struct PRINTER *SetKP;
    BYTE    sYN;

    SetKP = (struct PRINTER *) &KPSTART;
    if (Appl_ProgNumber > 8)
    {
        Appl_ProgNumber = -1;
        Appl_ProgLine = 0;
        return;
    }
    SetKP = SetKP + Appl_ProgNumber;
    switch (Appl_ProgLine)
    {
    case 1:
        Appl_ProgLine=2;
    case 2:
        Appl_MaxEntry=1;   //hf 20060105 0;
        //Appl_EntryCounter = 0;
        if (!Appl_BitNumber)
            Appl_BitNumber = (SetKP->type & 0x0f) + 1;
        if (Appl_BitNumber > KPTypeNum )
            Appl_BitNumber = 1;
        SetKP->type = (SetKP->type & 0xf0) + Appl_BitNumber-1;

        DispStrXY(Msg[DYJXHAO].str,0,0xff);

        strcpy(ProgLineMes,KPType[Appl_BitNumber-1]);//ccr2014-04-01
        Appl_EntryCounter = 0;
#if (SCREENWD<20)
        sYN = strlen(Msg[DYJXHAO].str)+1;
        if (sYN>SETUPDISPX)
#endif
            sYN=SETUPDISPX;
        SetEditPosition(sYN,CursY,1,NULL);//ccr2014-04-01 设定信息显示位置

        break;

    case 3:
    case 4:
    case 5:
        if (SetKP->type & 0x0f)
        {
            if (ProgDevice(FORKPRINTER,&SetKP->port,3))
                break;
        }
        else
        {//当关闭厨房打印机时,无须进行后续设置
            Appl_MaxEntry=4;
            Appl_ProgLine = 0;
            break;
        }
    case 6:
    case 7:
    case 8:
        if (SetKP->port==0)
        {//当关闭厨房打印机时,无须进行后续设置
            Appl_MaxEntry=4;
            Appl_ProgLine = 0;
            break;
        }
        else
        {
            Appl_MaxEntry=0;
            //Appl_EntryCounter = 0;
            sYN = (Appl_ProgLine==7)?(YN):(0);
            //标志位是从Bit5开始!!!!!
            CheckBitValue(KPDYZE+Appl_ProgLine-6,&SetKP->type,(Appl_ProgLine),sYN);
        }
        break;
    case 9:     /* link with another KP */
        Appl_BitNumber = 0;
        Appl_MaxEntry=1;
        SetKP->alter++;
        GetByteValue(LIANJIE,&SetKP->alter,8);
        if (SetKP->alter>0)
            SetKP->alter--;
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}

/**
 * 设置外置收款机
 *
 * @author EutronSoftware (2016-12-14)
 */
void ProgExtPrinter()
{

    BYTE    sYN;

    switch (Appl_ProgLine)
    {
    case 1:
        Appl_ProgLine=2;
    case 2:
        //if (ApplVar.ExtPrinter.port>0)//当无外置打印机时,无须进行后续设置
        {
            Appl_MaxEntry=1;   //hf 20060105 0;
            //Appl_EntryCounter = 0;
            if (!Appl_BitNumber)
                Appl_BitNumber = ApplVar.ExtPrinter.type + 1;
            if (Appl_BitNumber==2)
                Appl_BitNumber++;//跳开票据打印机
            if (Appl_BitNumber > KPTypeNum )
                Appl_BitNumber = 1;
            ApplVar.ExtPrinter.type =  Appl_BitNumber-1;

            DispStrXY(Msg[DYJXHAO].str,0,0xff);

            strcpy(ProgLineMes,KPType[Appl_BitNumber-1]);//ccr2014-04-01
            Appl_EntryCounter = 0;
#if (SCREENWD<20)
            sYN = strlen(Msg[DYJXHAO].str)+1;
            if (sYN>SETUPDISPX)
#endif
                sYN=SETUPDISPX;
            SetEditPosition(sYN,CursY,1,NULL);//ccr2014-04-01 设定信息显示位置
            break;
        }
    case 3:
    case 4:
    case 5:
        if (ProgDevice(FOREXTPRN,&ApplVar.ExtPrinter.port,3))
            break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}

void ProgSlip()
{

    switch (Appl_ProgLine)
    {
    case 1:
        Appl_ProgLine=2;
    case 2:

        //Appl_EntryCounter = 0;
        if (!Appl_BitNumber)
            Appl_BitNumber = (SLIP & 0x0f) + 1;
        if (Appl_BitNumber > SPTypeNum  )
            Appl_BitNumber = 1;
        SLIP = Appl_BitNumber - 1;

        DispStrXY(Msg[PDJIXING].str,0,0xff);
        strcpy(ProgLineMes,SPType[Appl_BitNumber - 1]);
        SetEditPosition(SETUPDISPX,CursY,1,NULL);//ccr2014-04-01 设定信息显示位置
        Appl_EntryCounter = 0;
        Appl_MaxEntry=1;
        break;
    case 3:
    case 4:
    case 5:
        if (SLIP>0)
        {
            if (ProgDevice(FORSLIPPRN,&SLIPPORT,3))
                break;
        }
    case 6:
        if (SLIP>0 && SLIPPORT>0)
        {
            Appl_BitNumber = 0;
            Appl_MaxEntry=2;
            GetByteValue(DBKHSHU,&SLIP_TOP, 10);
            break;
        }
        else
        {
            Appl_MaxEntry=4;
            Appl_ProgLine = 0;
            break;
        }
    case 7:
        {
            Appl_BitNumber = 0;
            Appl_MaxEntry=2;
            GetByteValue(MYEHSHU,&SLIP_MAX,30);
            break;
        }
    case 8:
        {
            if (Appl_BitNumber<2)
                Appl_BitNumber = 1;
            Appl_MaxEntry=0;
            DispStrXY(Msg[DAYXXI].str,0,0xff);
            if (Appl_BitNumber==2)
            {
                Appl_BitNumber = 1;
                SLIPINFO ^= 0x08;
            }
            if (TESTBIT(SLIPINFO, BIT3))
                strcpy(ProgLineMes+SETINPUTWD-4,Msg[KAI].str);
            else
                strcpy(ProgLineMes+SETINPUTWD-4,Msg[GUAN].str);
            break;
        }
    case 9:     /* Appl_Port */
//		{
//			Appl_BitNumber = 0;
//			Appl_MaxEntry=2;
//			SP = SECOND &0xf0;
//			SECOND &= 0x0f;
//			GetByteValue(DEZPJU,&SECOND);
//			SECOND |= SP;
//			break;
//		}
//	case 9:     /* link with another ApplVar.KP */
        {
            Appl_BitNumber = 0;
            Appl_MaxEntry=2;
            GetByteValue(ZUOBJU,&LMARGE, 50);
            break;
        }
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}


void ProgICBlock()  //ccr chipcard 2004-07-01
{
    unsigned char TempStr[15];
    ULONG ICCardNOBK;

    if (Appl_ProgLine == 1)
        Appl_ProgLine = 2;
    else if (Appl_ProgLine > 2)
    {
//        ClearLine(SCREENLN-2);
        Appl_ProgNumber++;
        Appl_ProgLine = 2;
    }
    if (Appl_ProgNumber >= ApplVar.AP.ICBlock.Number)
    {
        if (ApplVar.AP.ICBlock.Number==0)
            strcpy(ProgLineMes,Number0);
        else
        {
            Appl_ProgNumber = -1;
            Appl_ProgLine = 0;
        }
        return;
    }

    ApplVar.ICBlockNumber = Appl_ProgNumber;
    ReadICBlock();

    Appl_MaxEntry=8;
    memset(TempStr,0,sizeof(TempStr));
    if (!Appl_EntryCounter)
        ULongToBCDValue(TempStr,ApplVar.ICBlock.ICCardNO);
    GetBCDValue(GUASHIIC,TempStr,4,TRUE);

    BCDValueToULong(TempStr,&ICCardNOBK);
    ApplVar.ICBlock.ICCardNO = ICCardNOBK;

    memset(SysBuf,' ',8);                   //ccr040809
    WORDtoASCZero(SysBuf+4,Appl_ProgNumber+1);
    SysBuf[5] = ':';
    SysBuf[6] = 0;
    DispStrXY(SysBuf+2,6,0xff);
    if (Appl_EntryCounter)
    {
        if (ApplVar.ICBlock.ICCardNO == 99999999)
        {
            PrintICBlock();
            if (Appl_ProgNumber>0)
                Appl_ProgNumber--;

        }
        else
        {
            WriteICBlock();
//			Appl_ProgNumber++;
        }
    }
}

//hf added 20040903 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//ProgIP的处理流程与ProgDate的处理流程保持一致 //
//
void ProgIP()
{
	WORD port;

    switch (Appl_ProgLine)
    {
    case 1:
        Appl_ProgLine = 2;
    case 2:
        ClearLine(0);
        Editor.EditOffs=0;
        Appl_EntryCounter = 0;
        Editor.EditCount = 0;
        GotoXY(SETUPDISPX,0);
        Appl_ProgLine = 3;
    case 3:
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=16;
        GetHEXValue(Msg[ECRNETID].str,ApplVar.IP.ECR_ID,sizeof(ApplVar.IP.ECR_ID));
        break;
    case 4:
        ClearLine(SCREENLN-2);
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        ChnInput(3);
        Appl_MaxEntry=15;
        GetIP(ECRIPADDRESS,ApplVar.IP.IPAddress);
        if (Appl_EntryCounter>0 && REGISTER==0)
        {
            //Appl_ProgLine = 1;	//change by lqw..090827
            REGISTER = ApplVar.IP.IPAddress[3];//ccr 050111
        }
        break;
    case 5:
        memset(ProgLineMes,' ',sizeof(ProgLineMes));
        port = ApplVar.IP.ClientPort;
        GetWordValue(GPRSSERVERPORT, &port,65530);
        ApplVar.IP.ClientPort = port;
        break;
#if(defined(CASE_GPRS))
    case 6:
        ClearLine(SCREENLN-2);
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        ChnInput(3);
        Appl_MaxEntry=15;
        GetIP(GPRSSERVERIP,ApplVar.IP.ServerIP);
        break;
    case 7:
        memset(ProgLineMes,' ',sizeof(ProgLineMes));
		port = ApplVar.IP.ServerPort;
        GetWordValue(GPRSSERVERPORT, &port,65530);
		ApplVar.IP.ServerPort = port;
        break;
    case 8://ccr2015-03-24>>>>>>>>
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=sizeof(ApplVar.IP.APN)-1;
        ApplVar.IP.APN[Appl_MaxEntry]=0;
        if (GetString(GPRSAPNNAME, ApplVar.IP.APN, Appl_MaxEntry,0))
            break;
    case 9://ccr2015-03-11>>>>>>>>
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=sizeof(ApplVar.IP.UserName)-1;
        ApplVar.IP.UserName[Appl_MaxEntry]=0;
        if (GetString(GPRSUSERNAME, ApplVar.IP.UserName, Appl_MaxEntry,0))
            break;
    case 10:
        if (ApplVar.IP.UserName[0])
        {
            SETBIT(ApplVar.MyFlags,HANZIMODE | PWDMODE);
            Appl_MaxEntry=sizeof(ApplVar.IP.Password)-1;
            ApplVar.IP.Password[Appl_MaxEntry]=0;
            if (GetString(GPRSPASSWORD, ApplVar.IP.Password, Appl_MaxEntry,0))
                break;
        }//ccr2015-03-11<<<<<<<<<<<
#endif
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    default:
        RESETBIT(ApplVar.MyFlags,HANZIMODE | PWDMODE);
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}

//ccr2016-01-21>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
BYTE PortSelect;//保存设置前的端口号

/**
 * 将指定设备从串口portFr换到串口portTo上,然后将portTo上原来的设备改为portFr
 *
 * @author EutronSoftware (2016-01-21)
 *
 * @param setType :设备类型
 * @param portFr :两个交换的端口(1,2,3...)
 * @param portTo :
 */
void Exchange2Port(BYTE setType,BYTE portFr,BYTE portTo)
{
    short i;
    BYTE oldPort;
    struct PRINTER *SetKP;

    if (portTo && portTo<=portNumber)
    {
        if (setType!=FORHOST && portTo==COMPUTER)
            COMPUTER = portFr;
        else if (setType!=FORBALANCE && portTo==(BALANCE & 0x07))
            BALANCE =(BALANCE & 0xf8) | portFr;
        else if (setType!=FORBARCODE && portTo==(BARCODE & 0x07))
            BARCODE = (BARCODE & 0xf8) | (portFr);
        else if (setType!=FORSLIPPRN && portTo==(SLIPPORT))
            SLIPPORT = portFr;
        else if (setType!=FOREXTPRN && portTo==ApplVar.ExtPrinter.port)//ccr2016-12-14 ExtPrinter串口
            ApplVar.ExtPrinter.port = portFr;
        else if (setType!=FORKPRINTER)
        {
            SetKP = (struct PRINTER *) &KPSTART;
            for (i=0;i<8;i++)
            {
                SetKP = SetKP+i;
                if (SetKP->port==portTo)
                    SetKP->port = portFr;
            }
        }
        if (portFr && portFr<=portNumber)
        {
            Appl_Port = ApplVar.PortList[portFr -1];
            ApplVar.PortList[portFr -1] = ApplVar.PortList[portTo -1];
            Appl_Port.Type= setType+'1';
            ApplVar.PortList[portTo -1] = Appl_Port;
            SetComm(portFr -1);
        }
    }
}

/**
 * 设置与外设的的通讯方式 [网络,串口1,串口2.....]
 *
 * @author EutronSoftware (2016-01-21)
 * @param dev :设备类型:
 *            FORHOST,FORBARCODE,FORBALANCE,FORKPRINTER,FORSLIPPRN,FOREXTPRN
 *
 * @param port :设备的端口变量
 */
BYTE ProgDevice(BYTE dev,BYTE *port,BYTE LineFr)
{
    short i;
    BYTE DevPort;
    struct PRINTER *SetKP;


    DevPort=(*port) & 0x07;

    if (Appl_ProgLine==LineFr)
    {
        Appl_ProgLine = LineFr+1;//Like date
        PortSelect=DevPort;
    }
    if (Appl_ProgLine==LineFr+1)
    {     /* Appl_Port type (Option)*/
        Appl_MaxEntry=1;    //hf 20060105 0;
        GotoXY(SETUPDISPX,CursY);
        Appl_EntryCounter = 0;

        if (Appl_BitNumber==0)
            Appl_BitNumber = DevPort+1;
        GetDevicePort(dev,&PortSelect);
        return true;
    }
    if (Appl_ProgLine==LineFr+2)
    {     /* Protocl string */
        if (PortSelect!=DevPort)
        {   //将COMPUTER换到PortSelect上
            Exchange2Port(dev,DevPort,PortSelect);
            if (dev==FORBARCODE || dev==FORBALANCE)
                DevPort = (DevPort & 0xf8) + PortSelect;
            else
                DevPort=PortSelect;
            *port = DevPort;
        }
        if (PortSelect)
        {//为串口
            Appl_PortNumber = PortSelect-1;
            ReadPort();
            Appl_Port.Type = dev+'1';
            Appl_MaxEntry=1;
            if (Appl_BitNumber==0)
            {
                Appl_BitNumber = DeCodeRate(Appl_Port.Prot[Appl_Port.Type-0x31]);
                 if (Appl_BitNumber==0)  Appl_BitNumber=1;
            }
            GetPortRate(&Appl_Port.Prot[Appl_Port.Type-0x31]);
            WritePort();
            return true;
        }
    }
    return false;
}

/**
 * 设置与计算机的通讯方式 [网络,串口1,串口2.....]
 *
 * @author EutronSoftware (2016-01-22)
 */
void ProgHost()
{
    switch (Appl_ProgLine)
    {
    case 1:
    case 2:
    case 3:
        if (ProgDevice(FORHOST,&COMPUTER,1))
            break;
    default:
#if (defined(TCPIP) || defined(CASE_GPRS))
        if (COMPUTER==0)
        {//为网路
            ProgIP();
            break;
        }
#endif
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}

/**
 * 设置条码枪的通讯方式 [网络,串口1,串口2.....]
 *
 * @author EutronSoftware (2016-01-22)
 */
void ProgBarcodeReader()
{
    switch (Appl_ProgLine)
    {
    case 1:
    case 2:
    case 3:
        if (ProgDevice(FORBARCODE,&BARCODE,1))
            break;
    case 4:
        if (BARCODE & 0x07)
        {
            Appl_MaxEntry=0;
            Appl_EntryCounter = 0;
            CheckBitValue(PRICEBARCODE,&ART_LEVEL,4,0);
            break;
        }
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
}

/**
 * 设置电子秤的通讯方式 [网络,串口1,串口2.....]
 *
 * @author EutronSoftware (2016-01-22)
 */
void ProgBalance()
{
    BYTE type,sX;
    switch (Appl_ProgLine)
    {
        case 1:
        case 2:
        case 3:
            if (!ProgDevice(FORBALANCE,&BALANCE,1))
            {
                Appl_MaxEntry=4;
                Appl_ProgLine = 0;
            }
            break;
        case 4://ccr2017-03-16>>>>>选择电子秤类型
#if SCREENLN<5
            ClearLine(SCREENLN-2);
#endif
            type = (BALANCE >> 4)+1;

            Appl_MaxEntry=1;
            //Appl_EntryCounter = 0;
            if (!Appl_BitNumber)
                Appl_BitNumber = type;
            if (Appl_BitNumber > BALTYPEMAX )
                Appl_BitNumber = 1;
            type = Appl_BitNumber-1;

            DispStrXY(Msg[DZCHXHAO].str,0,0xff);

            strcpy(ProgLineMes,BALType[type]);//ccr2014-04-01
            Appl_EntryCounter = 0;
            sX = SCREENWD-strlen(ProgLineMes);
            SetEditPosition(sX,CursY,1,NULL);//ccr2014-04-01 设定信息显示位置

            BALANCE = (BALANCE & 0x07) | ((type & 0x0f)<<4);
            break;//ccr2017-03-16<<<<<<<<<<<<<<<<<<<<<
        default:
            Appl_MaxEntry=4;
            Appl_ProgLine = 0;
            break;
    }
}

/**
 * 选择要打印的图片
 *
 * @author EutronSoftware (2016-01-25)
 *
 * @return BYTE
 */
BYTE SelectGraphic(BYTE grpIdx)
{
    Appl_MaxEntry=1;
    Appl_EntryCounter = 0;
    if (Appl_BitNumber==0)
        Appl_BitNumber = grpIdx+1;

    if (Appl_BitNumber > GRAPHMAX+1 )
        Appl_BitNumber = 1;

    CopyFrStr(ProgLineMes,EcrGraphics[Appl_BitNumber-1]);
    return Appl_BitNumber-1;
}

/**
 * 新的系统参数设置功能,仅供ListItems调用
 *
 * @author EutronSoftware (2016-02-27)
 *
 * @param y :需要设置的信息在屏幕上的显示行
 * @param recNo :需要设置的系统参数序号
 *
 * @return BYTE :=0xff时,无按键输出;否则为按键输出
 */
BYTE NewProgSysFlag(BYTE y,WORD recNo)
{
    BYTE BitV, i;
    WORD BitN;
    BYTE updownkey;
    char sDispBuf[10];
    short   sIdx;

    if (recNo<SYSUSED)
    {
        sIdx = SysFlagUsed[recNo].Index;

        if (sIdx == CONTRAST && SysFlagUsed[recNo].Bit==0)// && SysFlagUsed[recNo-2].Max == 63)
        {//调对比度,//ApplVar.AP.Flag[CONTRAST]
            //set contrast by pageup & pagedown.
            sDispBuf[0]=' ';

            for (updownkey = 1;;)
            {
                i = (DENSITY & 0x0f);
                if (updownkey)
                {
                    sDispBuf[2] = (i % 10)+'0';
                    if (i>9)
                        sDispBuf[1] = (i / 10)+'0';
                    else
                        sDispBuf[1] = ' ';
                    sDispBuf[3] = 0;
                    DispStrXY(sDispBuf, SCREENWD-3 ,y);
                }
                WAIT_INPUT(updownkey);

                if (updownkey==ApplVar.AP.FirmKeys[ID_UP])
                {
                    if((DENSITY & 0x0f)<14)// VolumeVal <= 0x3A)
                        DENSITY++;
                    else
                        return updownkey;
                }
                else if (updownkey==ApplVar.AP.FirmKeys[ID_DOWN])
                {
                    if((DENSITY & 0x0f)>0)// VolumeVal <= 0x3A)
                        DENSITY--;
                    else
                        return updownkey;
                }
                else if (updownkey==ApplVar.AP.FirmKeys[ID_ENTER])
                    return ApplVar.AP.FirmKeys[ID_DOWN];
                else
                    return updownkey;
                mSetContrast((DENSITY & 0x0f));
                Save_Config(true);
            }
        }

        if (SysFlagUsed[recNo].Bit)
        {//为yes/no选择项
            BitV = BIT0<<(SysFlagUsed[recNo].Bit-1);
            BitN = (ApplVar.AP.Flag[sIdx] & BitV);
            while (true)
            {
                if (BitN)
                {
                    i = 'y' ^ SysFlagUsed[recNo].Invert;
                    SETBIT(ApplVar.AP.Flag[sIdx],BitV);
                }
                else
                {
                    i = 'n' ^ SysFlagUsed[recNo].Invert;
                    RESETBIT(ApplVar.AP.Flag[sIdx], BitV);
                }
                if (i == 'y')
                    DispStrXY(Msg[KAI].str, SCREENWD-3 ,y);
                else
                    DispStrXY(Msg[GUAN].str, SCREENWD-3 ,y);

                InitSysFlag();
                Save_Config(true);

                WAIT_INPUT(updownkey);
                if (updownkey==ApplVar.AP.FirmKeys[ID_SELECT])
                    BitN = !BitN;
                else if (updownkey==ApplVar.AP.FirmKeys[ID_ENTER])
                    return ApplVar.AP.FirmKeys[ID_DOWN];
                else
                    return updownkey;
            }

        }
        else
        {
            if (sIdx==62)//复制收据次数或对比度
            {//高4位为标志,低4为数值  //
                i = ApplVar.AP.Flag[sIdx] & 0xf0;   //备份标志位
                ApplVar.AP.Flag[sIdx] &= 0x0f;      //为了显示数值，先清除高四位
            }
            else
                i = 0;
            ApplVar.NumberEntry=ApplVar.AP.Flag[sIdx];
            DisplayDecXY(ApplVar.NumberEntry,SCREENWD-3,y,3);//加此显示的目的是去掉反显

            //ccr2016-08-03>>>>>>>>>>>>>>>>>>
//            if ((sIdx==10 || sIdx==11) && ApplVar.NumberEntry && ApplVar.ScreenMap[y][SCREENWD-5]==' ')//PREFIX_1,PREFIX_2
//                DispCharXY(ApplVar.NumberEntry,SCREENWD-5,y);
            //ccr2016-08-03<<<<<<<<<<<<<<<<<

            updownkey = InputWord(SCREENWD-3,y,SysFlagUsed[recNo].Max);
            ApplVar.AP.Flag[sIdx] = ApplVar.NumberEntry | i;
            Save_Config(true);
            InitSysFlag();
            if (updownkey==ApplVar.AP.FirmKeys[ID_ENTER])
                return ApplVar.AP.FirmKeys[ID_DOWN];
            else
                return updownkey;
        }
    }
}
//ccr2016-03-15>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void  ProgKeyMacro()
{
    WORD tmpNumber,keyFunc;
    int idx;

    if (Appl_ProgNumber >= KEYMACROMAX)
    {
        Appl_ProgNumber = -1;
        Appl_ProgLine = 0;
        return;
    }
    tmpNumber = Appl_ProgNumber;
    switch (Appl_ProgLine)
    {
    case 1:
        break;
    case 2:     /* group name */
        Appl_MaxEntry=MACRONAME-1 ;
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        if (GetString(MINCHENF, ApplVar.AP.KeyMacros[tmpNumber].Name,MACRONAME-1,0))
            break;
    default:
        idx = Appl_ProgLine-3;
        if (idx<(MACROFUNC20-MACROFUNC1))
        {
            if (idx==0 || ApplVar.AP.KeyMacros[tmpNumber].FuncList[idx-1]!=0)
            {//如果上一个功能码为空时,没有必要再继续设置其后的功能码
                Appl_MaxEntry=1;    //hf 20060105 0;
                Appl_EntryCounter = 0;
                if (Appl_BitNumber==0)
                {//显示当前定义的功能码
                    Appl_BitNumber = 1;
                    DispStrXY(Msg[GNMA].str,0,0xff);
                    ECRFuncCodeToName(ApplVar.AP.KeyMacros[tmpNumber].FuncList[idx],ProgLineMes);
                    SetEditPosition(SETUPDISPX,CursY,1,NULL);//ccr2014-04-01 设定信息显示位置
                    break;
                }
                else
                {
                    Appl_BitNumber = 1;
                    keyFunc = SelectKeyCode(ProgLineMes);//选择功能码
                    if (keyFunc<MACROFUNC1 || keyFunc>MACROFUNC20)
                        ApplVar.AP.KeyMacros[tmpNumber].FuncList[idx]=keyFunc;
                    else
                        ProgLineMes[0]=0;//宏中禁止包括宏定义
                    if (keyFunc!=0)//设置不为空时,才有必要在继续设置其后的功能码
                        break;
                }
            }
        }
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        break;
    }
    WriteGroup();
}




//ccr2016-03-24.....配置单品文件..........
ECRFILEDEF CopyECRTable;//复制单品配置数据

/************************************************************
 * 不改变分配给单品的总空间的情况下,配置单品数据
 *
 * @author EutronSoftware (2016-03-24)
 ************************************************************/
void ProgPLUFields()
{
    WORD tmpW;
    BYTE tmpB;
    ULONG tmpL;

    switch (Appl_ProgLine)
    {
    case 1:
        CopyECRTable.Plu=ApplVar.AP.Plu;//复制当前的数据结构
        Appl_ProgLine=2;
//        mClearScreen();
    case 2://RandomSize
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Plu.RandomSize*2;
        GetByteValue(MsgPLURANDOMSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Plu.RandomSize=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Plu.RandomSize++;
        }
        break;
    case 3:     /* PLU name size*/
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Plu.CapSize;
        GetByteValue(MsgPLUNAMESIZE, &tmpB, 24);
        if (Appl_EntryCounter)
            CopyECRTable.Plu.CapSize=tmpB;
        else
            ClearLine(SCREENLN-2);   //清除"按退出键 退出"
        break;
    case 4://price size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Plu.PriceSize*2;
        GetByteValue(MsgPLUPRICESIZE, &tmpB, 10);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Plu.PriceSize=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Plu.PriceSize++;
        }
        break;
    case 5://Price level
        Appl_MaxEntry=1;
        tmpB=CopyECRTable.Plu.Level-1;//至少包含一级价格
        GetByteValue(MsgPLUPRICELEVEL, &tmpB, 3);//最多三级会员价
        if (Appl_EntryCounter)
        {
            CopyECRTable.Plu.Level=tmpB+1;
        }
        break;
    case 6://Cost size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Plu.Cost*2;
        GetByteValue(MsgPLUCOSTSIZE, &tmpB, 10);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Plu.Cost=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Plu.Cost++;
        }
        break;
    case 7://Inventory size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Plu.InvSize*2;
        GetByteValue(MsgPLUINVENTSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Plu.InvSize=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Plu.InvSize++;
        }
        break;

    case 8://Report QTY size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Plu.Size[0].Qty*2;
        GetByteValue(MsgPLUREPQTYSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Plu.Size[0].Qty=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Plu.Size[0].Qty++;
        }
        break;
    case 9://Report AMT size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Plu.Size[0].Amt*2;
        GetByteValue(MsgPLUREPAMTSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Plu.Size[0].Amt=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Plu.Size[0].Amt++;
        }
        break;
    case 10://Report DISC size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Plu.Size[0].Disc*2;
        GetByteValue(MsgPLUREPDISCSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Plu.Size[0].Disc=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Plu.Size[0].Disc++;
        }
        break;
    case 11://Report COST size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Plu.Size[0].Cost*2;
        GetByteValue(MsgPLUREPCOSTSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Plu.Size[0].Cost=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Plu.Size[0].Cost++;
        }
        break;

    default://对新的配置进行确认
        if (memcmp(&ApplVar.AP.Plu,&CopyECRTable.Plu,sizeof(CopyECRTable.Plu)) && WaitForYesNo(Msg[SETCONFIGPLU].str,false))
        {//单品的数据有变化
            //1.重新计算PLU的记录大小
            ApplVar.AP.Plu=CopyECRTable.Plu;
            GetRecordSize(PLUIDX);
            //2.重新计算PLU数量
            ApplVar.AP.Plu.Number=(ApplVar.AP.StartAddress[AddrPort] - ApplVar.AP.StartAddress[AddrPLU])/ApplVar.AP.Plu.RecordSize;
            //3.对PLU进行初始化
            InitPlu(true);
        }
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];//强制退出设置
        break;
    }
}

/************************************************************
 * 可改变分配给部类的总空间的情况下,配置部类数据
 *
 * @author EutronSoftware (2016-03-28)
 ************************************************************/
void ProgDEPTFields()
{
    WORD tmpW;
    BYTE tmpB;
    ULONG tmpL;

    switch (Appl_ProgLine)
    {
    case 1:
        CopyECRTable.Dept=ApplVar.AP.Dept;//复制当前的数据结构
        Appl_ProgLine=2;
//        mClearScreen();
    case 2://部类数量
        Appl_MaxEntry=3;
        tmpW=CopyECRTable.Dept.Number;
        if (TESTBIT(ApplVar.AP.Options.Plu, BIT1))
            GetWordValue(MsgPLUNUMBER, &tmpW, 999);
        else
            GetWordValue(MsgPLUNUMBER, &tmpW, 250);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Dept.Number=tmpW;
        }
        break;
    case 3:     /* DEPT name size*/
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Dept.CapSize;
        GetByteValue(MsgPLUNAMESIZE, &tmpB, 24);
        if (Appl_EntryCounter)
            CopyECRTable.Dept.CapSize=tmpB;
        else
            ClearLine(SCREENLN-2);   //清除"按退出键 退出"
        break;
    case 4://price size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Dept.PriceSize*2;
        GetByteValue(MsgPLUPRICESIZE, &tmpB, 10);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Dept.PriceSize=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Dept.PriceSize++;
            CopyECRTable.Dept.PriMaxSize=CopyECRTable.Dept.PriceSize;
        }
        break;
    case 5://Report QTY size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Dept.Size[0].Qty*2;
        GetByteValue(MsgPLUREPQTYSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Dept.Size[0].Qty=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Dept.Size[0].Qty++;
        }
        break;
    case 6://Report AMT size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Dept.Size[0].Amt*2;
        GetByteValue(MsgPLUREPAMTSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Dept.Size[0].Amt=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Dept.Size[0].Amt++;
        }
        break;
    case 7://Report DISC size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Dept.Size[0].Disc*2;
        GetByteValue(MsgPLUREPDISCSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Dept.Size[0].Disc=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Dept.Size[0].Disc++;
        }
        break;
/*    case 8://Report COST size
        Appl_MaxEntry=2;
        tmpB=CopyECRTable.Dept.Size[0].Cost*2;
        GetByteValue(MsgPLUREPCOSTSIZE, &tmpB, 14);
        if (Appl_EntryCounter)
        {
            CopyECRTable.Dept.Size[0].Cost=tmpB/2;
            if (TESTBIT(tmpB,BIT0))
                CopyECRTable.Dept.Size[0].Cost++;
        }
        break;
*/
    default://对新的配置进行确认
        if (memcmp(&ApplVar.AP.Dept,&CopyECRTable.Dept,sizeof(CopyECRTable.Dept)) && WaitForYesNo(Msg[SETCONFIGDEPT].str,false))
        {//单品的数据有变化
            //1.重新计算Dept的记录大小
            tmpW=CopyECRTable.Dept.Number;
            CopyECRTable.Dept.Number=ApplVar.AP.Dept.Number;
            ApplVar.AP.Dept=CopyECRTable.Dept;
            GetRecordSize(DEPTIDX);
            //2.重新计算Dept数量
            ApplVar.AP.Dept.Number=(ApplVar.AP.StartAddress[AddrPLU] - ApplVar.AP.StartAddress[AddrDept])/ApplVar.AP.Dept.RecordSize;
            if (tmpW>ApplVar.AP.Dept.Number)//增加了部类数量,需要减少单品空间
            {
                tmpL=(tmpW-ApplVar.AP.Dept.Number)*ApplVar.AP.Dept.RecordSize;
                ApplVar.AP.StartAddress[AddrPLU]+=tmpL;
                ApplVar.AP.Plu.Number=(ApplVar.AP.StartAddress[AddrPort] - ApplVar.AP.StartAddress[AddrPLU])/ApplVar.AP.Plu.RecordSize;
                //3.对PLU进行初始化
                InitPlu(true);
                ApplVar.AP.Dept.Number=tmpW;
            }
            InitDept(true);
        }
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];//强制退出设置
        break;
    }
}
/**
 * 选择收款机的小数位数和小数点类型
 *
 * @author EutronSoftware (2016-04-05)
 */
void ProgPointType()
{
    WORD tmpW;
    BYTE tmpB;
    int tmpI;

    switch (Appl_ProgLine)
    {
    case 1:
        Appl_ProgLine=2;
    case 2://部类数量
        tmpI=(NO_DECIMAL & 0x03) * 2 + (COMMA & BIT0);
        Appl_MaxEntry=1;
        Appl_EntryCounter = 0;
        if (Appl_BitNumber==0)
        {//显示当前小数格式
            DispStrXY(Msg[SETPOINTTYPE].str,0,0xff);
            strcpy(ProgLineMes,Msg[MsgPOINTTYPE1+tmpI].str);
            tmpI=strlen(ProgLineMes);
            SetEditPosition(SCREENWD-tmpI,CursY,1,NULL);//ccr2014-04-01 设定信息显示位置
        }
        else
        {
            tmpW = ListItems(SETPOINTTYPE,true,It_SELEPOINT,true);
            if (tmpW>0 && tmpW<=MsgPOINTMAX)
            {
                tmpW--;
                strcpy(ProgLineMes,Msg[MsgPOINTTYPE1+tmpW].str);
                tmpI=strlen(ProgLineMes);
                SetEditPosition(SCREENWD-tmpI,CursY,1,NULL);//ccr2014-04-01 设定信息显示位置
                COMMA = (COMMA & (~BIT0)) | (tmpW & BIT0);
                NO_DECIMAL = (tmpW>>1);
                ApplVar.AmtDecimal=NO_DECIMAL;
                Save_Config(true);
            }
        }
        Appl_BitNumber = 1;
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];//强制退出设置
        break;
    }
}

/**
 * 增加新会员
 *
 * @author EutronSoftware (2016-05-17)
 */
void ProgNewVIP()
{
    WORD i;
    BYTE tmpB;
    ULONG tmpL;

    switch (Appl_ProgLine)
    {
    case 1:
        Appl_ProgLine=2;
        mClearScreen();
        ECRVIP.VipFix.VipName[0]=0;
    case 2:     /* VIP name */
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        Appl_MaxEntry=sizeof(ECRVIP.VipFix.VipName);
        GetString(MINCHENF, ECRVIP.VipFix.VipName, Appl_MaxEntry,0);
        break;
    case 3:     /*Get VIP TEL*/
        Appl_MaxEntry=sizeof(ECRVIP.VipFix.VipTEL)*2;
        DispStrXY(Msg[TMHYHAO].str,0,0xff);

        if (Appl_EntryCounter)
        {
            StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
            memset(ECRVIP.VipFix.VipTEL,0,sizeof(ECRVIP.VipFix.VipTEL));
            memcpy(ECRVIP.VipFix.VipTEL,ApplVar.Entry.Value,sizeof(ECRVIP.VipFix.VipTEL));
        }
        if (Appl_MaxEntry<=SETINPUTWD)
            HEXtoASCX0(ProgLineMes + SETINPUTWD - Appl_MaxEntry,ECRVIP.VipFix.VipTEL,sizeof(ECRVIP.VipFix.VipTEL));
        else
            HEXtoASCX0(ProgLineMes,ECRVIP.VipFix.VipTEL,SETINPUTWD/2);
        break;
    default://对新的配置进行确认
        if (WaitForYesNo(Msg[SETADDNEWVIP].str,false))
        {
            AddNewECRVIP();
        }
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];//强制退出设置
        break;
    }
}
//ccr2016-06-06>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#if defined(CASE_INDIA)
/**
 * 保存单品或者部类修改日志
 *
 * @author EutronSoftware (2016-06-06)
 */
void SavePlu_DeptEditLOG()
{
    switch (ApplVar.FlowBuff.CDC_EditDept.FunN)
    {
    case 0x80+EDITDEPTLOG://标示对DEPT进行了编辑修改
        if (memcmp(&ApplVar.FlowBuff.CDC_EditDept.Dept,&ApplVar.Dept,sizeof(ApplVar.Dept)))
        {//对DEPT进行了编辑修改
            memcpy(&ApplVar.FlowBuff.CDC_EditDept.Dept,&ApplVar.Dept,sizeof(ApplVar.Dept));
            Collect_Data(EDITDEPTLOG);
        }
        ApplVar.FlowBuff.CDC_EditDept.FunN=0;
        break;
    case 0x80+EDITPLULOG://标示对PLU进行了编辑修改
        if (memcmp(&ApplVar.FlowBuff.CDC_EditPlu.Plu,&ApplVar.Plu,sizeof(ApplVar.Plu)))
        {//对Plu进行了编辑修改
            memcpy(&ApplVar.FlowBuff.CDC_EditPlu.Plu,&ApplVar.Plu,sizeof(ApplVar.Plu));
            Collect_Data(EDITPLULOG);
        }
        ApplVar.FlowBuff.CDC_EditPlu.FunN=0;
        break;
    }
}
#endif
//ccr2016-06-06<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
/**
 * 设置EJ数据保存类型
 *
 * @author EutronSoftware (2016-11-10)
 */
void ProgEJournal()
{
    switch (Appl_ProgLine)
    {
    case 1:
        Appl_ProgLine=2;
    case 2://EJ_Z_STORE
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(MsgEJ_Z_STORE,&APPLCTRL,6,0);
        break;
    case 3://EJ_X_STORE
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(MsgEJ_X_STORE,&APPLCTRL,5,0);
        break;
    case 4://EJ_TR_STORE
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(MsgEJ_TR_STORE,&APPLCTRL,7,0);
        break;
    default:
        Appl_MaxEntry=4;
        Appl_ProgLine = 0;
        KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];//强制退出设置
        break;
    }
}
#endif
