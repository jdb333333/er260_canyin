#define BALANCE_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

#define BALBUFMAX  30
BYTE WeightFrom,WeightEnd;    //ccr2017-03-16
BYTE PS1X_BCC;    //ccr2017-03-17,PS1X
short   BalCounter=0;
BYTE    BalBuff[BALBUFMAX];

#define BALPOINTS	10  //DIBAL

/*********************************************************************
 * @author EutronSoftware (2017-03-16)
 *1. TOLEDO电子称的数据格式为"\x02  0.120 KG    0.00    0.00\x0d\x0a"
 *  托利多电子秤(0.012){2,7}: 02 20 20 30 2E 30 31 32 20 4B 47 20 20 20 20 30 2E 30 30 20 20 20 20 30 2E 30 30 0D 0A
 *2. DIBAL迪宝电子秤(0.0954){8,13}: 46 30 30 30 30 30 30 30 30 30 30 39 35 34 0D
 *3. PS1X读取重量数据(Read current weight):
 * PC sends 0X05, after receiving scale response 0x06, sends 0x11, scale will response weight package.
 * Weight Package formats:SOH STX STA SIGN WEIGHT_ASCII WEIGHT_UNIT BCC ETX EOT
 * SOH: start to sending symbol, one byte, fixed at 01H.
 * STX: start symbol of data item, one byte, fixed at 02H.
 * STA: words to describe weight status, one byte, only possible to be 53H or 55H46H.
 *     53H –weight is stable,
 *     55H – weight is unstable,
 *     46H –weight is abnormal.
 * SIGN: sign of weight, one byte, only possible to be 2DH or 20H.
 *     2DH-weight is negative,
 *     20H-weight is positive.
 * WEIGHT_ASCII: character string of weight. 5~6 byte, only possible to be 30H~39H, decimal (2EH), space (20H)
 * WEIGHT_UNIT : character string of weight unit. 1~2 byte, only possible to be :
 *     ‘TJ’: catty of Taiwan,
 *     ‘TL’: tael of Taiwan,
 *     ‘SJ’: jin,
 *     ‘LB’: pound,
 *     ‘kg’: kilogram,
 *     ‘g’: gram.
 * BCC: character of data verify. One byte. Set STA to D1, SIGN to D2, byte before BCC to DN, so
 *     BCC=D1^D2^. .^DN.'^’ is a logic XOR operator.
 * ETX: symbol of ending data item, one byte, fixed at 03H.
 * EOT: symbol of ending sending, one byte, fixed at 04H.
 * Example(0.316kg):06 01 02 53 20 20 30 2E 33 31 36 4B 47 75 03 04
 *
 * @return short
 *********************************************************************/
short BalanceWeight()
{
    static BYTE sType=0;   //电子秤型号,默认为0(TOLEDO)

    BYTE bport,sOK;
    short c;

    bport = BALANCE & 0x07;//获得电子秤所连串口

    if (!bport || ApplVar.ErrorNumber)//ccr2017-03-17 || ApplVar.CentralLock == OFF)
        return false;

    //采用自动识别方式 sType = BALANCE>>4;//获得电子秤型号
    //*sType :0-"TOLEDO",电子秤主动发送重量数据
    //*      1-DIBAL,收款机必须向电子秤发送0x05后,再读取重量数据
    //*      2-PS1X,收款机必须向电子秤发送0x05后,再读取重量数据

    while (true)
    {
        if (!CheckComm(bport-1) || (c = ReadComm(bport-1))==-1)
                return false;

        //自动识别电子秤型号>>>>>>>>>>>>>
        if (BalCounter==0)
        {
            WeightFrom=WeightEnd=PS1X_BCC=0;
            switch (c)
            {
            case ACK:
                    SendComm(bport-1,XON);
                    sType = BAL_PS1X;//也可以是其他型号
                    return false;
            case SOH:
                    sType = BAL_PS1X;
                    break;
            case STX:
                    sType = BAL_TOLEDO;
                    break;
            case 'F':
                    sType = BAL_DIBAL;
                    break;
            default://为错误数据
                    return false;
            }
            BALANCE = (BALANCE & 0x07) | ((sType & 0x0f)<<4);
        }
        else  if (sType==BAL_PS1X)
        {//收到电子秤的应答
            if (c==EOT)
            {//数据收据完毕
                PS1X_BCC ^= ETX;
                if (PS1X_BCC || BalBuff[2]!=0x53 || CWORD(BalBuff[0])!=0x0201)
                {//数据校验出错
                    EmptyComm(bport-1);
                    ApplVar.ErrorNumber = (ERROR_ID(CWXXI85));
                    BalCounter = 0;
                    return false;
                }
                else//数据收取正确
                    break;
            }
            if (BalCounter>1)
                PS1X_BCC ^= c;

        }
        //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        if (WeightEnd==0 || BalCounter-WeightEnd<=1) //只存储有效的重量数据,其后的数据只计数不存储
            BalBuff[BalCounter] = c;

        if (c>='0' && c<='9' || c=='.')
        {//标记重量数据的存储区域
            if (WeightFrom==0)
                WeightFrom=BalCounter;
            WeightEnd=BalCounter;
        }
        BalCounter++;

        if (sType==BAL_DIBAL && BalCounter == BALPOINTS)
        {//ccr2017-03-17 DIBAL>>>小数点位置固定
            if (WeightFrom==0)
                WeightFrom=BalCounter;
            WeightEnd=BalCounter;
            BalBuff[BalCounter++] = '.';
        }//ccr2017-03-17<<<<<<<<<<<<<<<<<<<<<

        if (BalCounter>=BALBUFMAX || c==LF)
        {
            BalCounter = 0;
            return false;
        }
        if (c==CRET)
        {
            if (sType==BAL_TOLEDO && BalBuff[0]==STX ||
                sType==BAL_DIBAL && BalBuff[0]=='F')
                 break;//数据收取正确
            else
            {//数据出错
                EmptyComm(bport-1);
                ApplVar.ErrorNumber = (ERROR_ID(CWXXI85));
                BalCounter = 0;
                return false;
            }
        }
    }
    ClearEntry(false);
    EmptyComm(bport-1);//清除后续无用的数据

    InActive = 0;//Disable display datetime
    BalCounter = 0;
    sOK = FALSE;
    bport=0;
    if (WeightFrom<WeightEnd)
    {
        for (c = WeightFrom;c<=WeightEnd;c++)
        {
            if (!sOK)
                sOK=(CWORD(BalBuff[c])!='00');

            if (sOK)
            {
                ApplVar.OldKey.Code = 0;
                bport=BalBuff[c];
                if (bport=='.' && BalCounter==0)
                    BalCounter=1;
                else  if (BalCounter)
                    BalCounter++;

                if (BalCounter<=4)
                {//最多只允许3位小数
                    ApplVar.Key.Code = bport;
                    KeyInput(ApplVar.Key.Code); //ccr 040420
                }
            }
        }

        BalCounter = 0;
        Bell(1);//ccr2017-03-16
        if (ApplVar.CentralLock == RG || ApplVar.CentralLock == MG)
        {
            ApplVar.OldKey.Code = 0;
            ApplVar.Key.Code = MULT;
            //ccr2016-02-17>>>>>>>>>
            {
                ApplVar.FBalance = 1;
                ProcessKey();
            }
            //ccr2016-02-17<<<<<<<<
            return true;
        }
    }
    return false;
}

/**
 * 向电子秤发送获取重量的命令
 *
 * @author EutronSoftware (2017-03-16)
 *
 * @return short
 */
BYTE BalanceCMD()
{
    BYTE bport,sType;

    bport = BALANCE & 0x07;
    if (!bport || ApplVar.ErrorNumber)//ccr2017-03-17 || ApplVar.CentralLock == OFF)
        return false;

//    sType=BALANCE>>4;

//    if (sType)
    {//DIBAL,PS1X
        EmptyComm(bport-1);
        SendComm(bport-1,0x05);
        BalCounter=WeightFrom=WeightEnd=0;
        return true;
    }
//    else
//        return false;
}

