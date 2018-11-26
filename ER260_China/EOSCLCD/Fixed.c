#define FIXED_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

/***************************************************************/

/* Routine for performing fixed function of type 1
    KEYCODE	Function
    CLEAR      Clear ApplVar.Key
    RPF      RPF receipt paper feed
    JPF      JPF journal paper feed
    RON      RON Receipt On/Off key
    MULT      Multiply ApplVar.Key
    SUB      Subtotal
    NEWRECEIPT	Only Issue receipt
    LEVEL1	plu price level 1
    LEVEL2	plu price level 2
    LEVEL3	plu price level 3
    LEVEL4	plu price level 4
    NPRICE	new plu price
    NUMBER	number entry
    ODRAW      drawer open
*/


void CheckMultiply()
{
    switch (ApplVar.MultiplyCount)
    {
    case 0:
        ApplVar.Qty1 = ApplVar.Entry;
        ApplVar.Qty = ApplVar.Qty1;
        break;
    case 1:
        ApplVar.Qty2 = ApplVar.Entry;
        Multiply(&ApplVar.Qty, &ApplVar.Qty2);
        break;
    case 2:
        ApplVar.Qty3 = ApplVar.Entry;
        Multiply(&ApplVar.Qty, &ApplVar.Qty3);
        break;
    default:
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        return;
    }
    if ((ApplVar.Qty.Sign & 0x0f) > 0x03)
        RoundBcd(&ApplVar.Qty, 3);
    ApplVar.MultiplyCount++;
    ClearEntry(false);
}


void NewReceipt()
{
    if (ApplVar.FReceipt)    /* Receipt Used ? */
    {
        ApplVar.BufKp = 2;  /* set so no feed on issue */
        ApplVar.PrintLayOut = 0x06; /* set double size for table number */
        KpEnd(0);   /* issue internal kp receipt */
        ApplVar.BufKp = 0;
        ApplVar.FReceipt = 0;
        ApplVar.FRecIssue = 1;  /* set receipt issue used */
    }
}

void PrintNumber(BCD *num)
{
    BCD temp;

    CONSTCHAR *str;

    temp = *num;
    temp.Sign &= 0x03;
    str = FormatQtyStr(Msg[HAOMA].str, &temp, PRTLEN);
    PrintStr(str);      /* print first part */
//	PrintStr(str+11);	/* print second */
    if (!ApplVar.BufRec)
    {
        ApplVar.RGRec.Qty = *num;
        ApplVar.RGRec.Key.Code = NUMBER;    /* number entry */
        StoreInBuffer();
        ApplVar.RGRec.Key.Code = 0;
    }
}
#if (1)//ccr20130426
void SuspendReceipt()
{//挂起收据(新程序)

    int i;

    if (ApplVar.FPb)
    {// 餐饮功能下禁止挂台操作 //
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        return;
    }
    if (!RGSuspend.SuspendItems.RGNumber)
    {//无挂起的收据
        if (!ApplVar.FRegi && !ApplVar.FSale)
        {//没有销售时,禁止此操作
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return;
        }

        RGSuspend.SaleAmt = ApplVar.SaleAmt;
        RGSuspend.SaleQty = ApplVar.SaleQty;
        RGSuspend.ClerkNumber = ApplVar.ClerkNumber;
        ApplVar.Key.Code = xSUSPEND;
        Correction();//执行交易挂起操作=交易取消

#if (SCRSTATDBL)
        DispOnStatDBL((char *)Msg[YOUGUADAN].str,0);
#else
        ClsState1();
//        ClsState2();
//        DispSt2(ApplVar.Clerk.Name);
        DispSt0(ApplVar.ModeHead);//RG-DISPLAY
        DispStrXY(Msg[YOUGUADAN].str,SCREENWD-6,STATELINE);//在状态行显示有挂单 //RG-DISPLAY
#endif
    }
    else    /* recall 打开挂起的收据 */
    {
        if (ApplVar.FRegi)
        {// 已经在销售状态,禁止此操作 //
            ApplVar.ErrorNumber=ERROR_ID(CWXXI73);
            return;
        }
//		if (ApplVar.ClerkNumber != RGSuspend.ClerkNumber)
//		{//收款员不一致,禁止操作
//			ApplVar.ErrorNumber=ERROR_ID(CWXXI34);
//			return;
//		}

        //利用保存在RGSuspend.SuspendItems.RGBuf中销售项目信息进行销售
        FRegSuspend = 1;
        for (i=0;i<RGSuspend.SuspendItems.RGNumber;i++)
        {
            ApplVar.Key.Code = RGSuspend.SuspendItems.RGBuf[i].Key.Code;

            //if ((ApplVar.Key.Code > CORREC && ApplVar.Key.Code < CORREC + 100) ||
            /*  为退货项目时,Qty为负数;为取消的项目时,Qty=ZERO;
                因此,无须处理CORREC项目
            */
            if ((ApplVar.Key.Code > DISC && ApplVar.Key.Code < DISC + 100) ||
                (ApplVar.Key.Code > MODI && ApplVar.Key.Code < MODI + 1000) ||
                (ApplVar.Key.Code > DEPT && ApplVar.Key.Code < PLU3) ||
                (ApplVar.Key.Code == SUB)
               )
            {
                if (ApplVar.Key.Code > DEPT && ApplVar.Key.Code < PLU3)
                {
                    ApplVar.Qty = RGSuspend.SuspendItems.RGBuf[i].Qty ;

                    if (ApplVar.Key.Code > PLU1)
                        ApplVar.Qty.Value[BCDLEN-1] = 0;//清除价格级别标示//

                    if (CheckNotZero(&ApplVar.Qty))
                    {
                        ApplVar.Qty1 = ApplVar.Qty ;
                        ApplVar.Price = RGSuspend.SuspendItems.RGBuf[i].Amt;
                        ApplVar.FPrice = 1;
                        ApplVar.MultiplyCount=1;
                        Appl_EntryCounter = 0;
                        ApplVar.OldKey.Code = 0;
                    }
                    else//当Qty=ZERO时,为已经取消的项目
                        continue;
                }
                else if (ApplVar.Key.Code >= DISC_PERINC && ApplVar.Key.Code <= DISC_SUB_NETDEC)    //ccr20130806 for SubDisc
                {
                    //ccr20130806 for SubDisc >>>>>>>>>
                    if (ApplVar.Key.Code>=DISC_SUB_PERINC && ApplVar.Key.Code<=DISC_SUB_NETDEC)
                        ApplVar.Key.Code-=4;//转换为非小计折扣功能码
                    //<<<<<<<<<<<<

                    if (ApplVar.Key.Code==DISC_PERDEC || ApplVar.Key.Code==DISC_PERINC)
                        ApplVar.Entry = RGSuspend.SuspendItems.RGBuf[i].Qty ;
                    else //if (ApplVar.Key.Code==DISC_NETDEC || ApplVar.Key.Code==DISC_NETINC)
                        ApplVar.Entry = RGSuspend.SuspendItems.RGBuf[i].Amt ;
                    if (CheckNotZero(&ApplVar.Entry))
                    {
                        ApplVar.Entry.Sign = 0;
                        ApplVar.DecimalPoint=2;
                        Appl_EntryCounter = 1;
                    }
                    else
                        continue;
                }
                ProcessFunctions();
            }
        }
        //PutsO(DispAmtStr(Msg[HOLD].str, &RGSuspend.SaleAmt,DISLEN));
        PrintAmt(Msg[GUAQI].str, &RGSuspend.SaleAmt);//打印挂单标记
        FRegSuspend = 0;
        RGSuspend.SuspendItems.RGNumber = 0;
    }
}
#else
// 老程序的挂起功能函数,收据账单被挂起过后,将无法执行此收据的交易取消操作
void SuspendReceipt()
{

    if (ApplVar.FPb)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        return;
    }
    if (!ApplVar.FSuspend)
    {
        if (!ApplVar.FRegi && !ApplVar.FSale)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return;
        }
        StoreInBuffer();// 把最后一条销售项目存入RGBuf中 //
        ApplVar.FSuspend = 1;
        RGSuspend.SaleAmt = ApplVar.SaleAmt;
        MemCopy(RGSuspend.TaxItem, ApplVar.TaxItem, sizeof(ApplVar.TaxItem));
        RGSuspend.SaleQty = ApplVar.SaleQty;
        RGSuspend.ClerkNumber = ApplVar.ClerkNumber;
        ApplVar.RGRec.Key.Code = SUSPEND;
        ApplVar.RGRec.Qty = ZERO;
        ApplVar.RGRec.Amt = ApplVar.SaleAmt;
        StoreInBuffer();// RGBuf中存入"挂起"项目(SUSPEND) //
//ccr2016-03-02        PutsO(DispAmtStr(Msg[HOLD].str, &ApplVar.SaleAmt,DISLEN));//C-DISPLAY
        ApplVar.PrintLayOut = 0x0b;                     /* print R/J and Slip */
        PrintLine('-');
        ApplVar.PrintLayOut = 0x0f;         /* double height */
        RESETBIT(ARROWS, BIT1); /* reset RG led */
        PrintAmt(Msg[GUAQI].str, &ApplVar.SaleAmt);
        ApplVar.FRegi = 0;
        ApplVar.FSale = 0;
        RegiEnd();
        ReceiptIssue(1);
        ApplVar.BufCC = 1;         /* set add Customer Count */
        ApplVar.BufKp = 1;
        ProcessBuffer();//生成统计数据
    }
    else    /* recall */
    {
        if (ApplVar.FRegi)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return;
        }
        if (ApplVar.ClerkNumber != RGSuspend.ClerkNumber)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI34);
            return;
        }
        ApplVar.PrintLayOut = 0x0f;         /* double height */
        if (RegiStart())
            return;
        ApplVar.SaleAmt = RGSuspend.SaleAmt;
        MemCopy(ApplVar.TaxItem, RGSuspend.TaxItem, sizeof(ApplVar.TaxItem));
        StoreInBuffer();
        ApplVar.RGRec.Key.Code = SUSPEND;
        ApplVar.RGRec.Qty = ZERO;
        ApplVar.RGRec.Amt = ApplVar.SaleAmt;

//ccr2016-03-02        PutsO(DispAmtStr(Msg[HOLD].str, &ApplVar.SaleAmt,DISLEN));//C-DISPLAY
        PrintAmt(Msg[GUAQI].str, &ApplVar.SaleAmt);
        ApplVar.FSale = 1;
        ApplVar.FSuspend = 0;
    }
}
#endif

void Fixed()
{
    short saveclerk ;
    BYTE SaveRGNumber,saveCp, SaveSP,sLp;
    char temp[18];
    ULONG lastEJ;
    CONSTCHAR *p;

    switch (ApplVar.Key.Code)
    {
#if (DD_ECRVIP==1)
    case VIPLOGIN:
        ECRVIPLogin();
        break;
#endif
    case NEWRECEIPT:
        NewReceipt();
        break;
    case RON:
        if (ApplVar.FRegi)   /* registration */
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
            break;
        }
        else
        {
            INVERTBIT(ARROWS, BIT0);    /* invert RON */
        }
    case CLEAR: /* clear function */
        if (ApplVar.ErrorNumber)// && TESTBIT(ApplVar.MyFlags,HANZIMODE))
        {
            mRefreshLine(STATELINE);
            ApplVar.ErrorNumber=0;
//            CheckError(0xff);//ccr2016-12-01
            break;
        }
        ClearEntry(false);
        if (Appl_ProgType && Appl_ProgLine)  /* not programming */
        {
//hf 20060105 >>>>>>>>>>>>>>>
/*			if (Appl_ProgLine>1 && (ApplVar.CentralLock==SET || ApplVar.CentralLock==SETUPMG | MG))//ccr chipcard
                    ClsArea(SETUPDISPX,CursY,SETINPUTWD);
*/
            if (Appl_ProgLine>1 && (ApplVar.CentralLock==SET || ApplVar.CentralLock==SETUPMG | MG))//ccr chipcard
            {//Appl_ProgLine>1时,输入的内容显示在屏幕内容区
                if (Appl_ProgType==SETDATE || Appl_ProgType==SETTIME)
                    ClsArea(SETDATEDISPX,CursY,SCREENWD-SETDATEDISPX);
                else
                    ClsArea(Editor.DispX,CursY,SCREENWD-Editor.DispX);//ccr20140218 SETUPDISPX,SETINPUTWD
            }
//hf end <<<<<<<<<<<<<<<<<<
            ClsArea(PROMDISPX,SCREENLN,PROMPTWD);   //hf 20060105 ClsArea(INPUTDISPX,SCREENLN,INPUTWIDTH);
        }
        else
        {
            ClsArea(PROMDISPX,SCREENLN,PROMPTWD);//hf 20060105
            if (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG)
            {
#if (SCRSTATDBL)
                DispOnStatDBL(EntryBuffer + ENTRYSIZE - SCRDBLWD - 1,0);
                DispOnStatDBL(ApplVar.ModeHead,0);
#else
                DispSt2(EntryBuffer + ENTRYSIZE - INPUTWIDTH - 1);
#endif
                LCD10_SetFlag(FLAG_NULL);
                PutsO(EntryBuffer + ENTRYSIZE - DISLEN - 1);//C-DISPLAY
            }
            else
                if (!(ApplVar.CentralLock & 0xff00))
                    ClsState1();
            Appl_ProgType = 0;
        }
        if (ApplVar.Key.Code > FIXED ) /* RON key ?  */
            break;
        if (TESTBIT(KPPRICE, BIT5))     /* german system then clear modifiers */
            ApplVar.ModiCount = 0;
        ApplVar.FPrice = 0;
        if (ApplVar.FVoid || ApplVar.FCorr || ApplVar.FRefund)
            ApplVar.RGRec.Key.Code = 0; /* clear temporary record */
        ApplVar.FVoid = 0;
        ApplVar.FCorr = 0;
        ApplVar.FRefund = 0;
        ApplVar.FCurr = 0;
        ApplVar.ErrorNumber=0;
        ApplVar.MultiplyCount = 0;

        //ccr20130809(清除操作后不能折扣问题)>>>>>>>>>>>
        if (ApplVar.FRegi)
        {
            if (ApplVar.RGRec.Key.Code<DEPT || ApplVar.RGRec.Key.Code>PLU3)
                ApplVar.RepeatCount = 0;
            if (ApplVar.RGRec.Key.Code!=SUB)
                ApplVar.FSub = 0;
        }
        else/* if not used clear inventory flag */
        {
            ApplVar.FSub = 0;
            ApplVar.RepeatCount = 0;
            ApplVar.FNFolio = 0;
            ApplVar.TaxPerm = 0;
            ApplVar.FCanc = 0;
            ApplVar.FProforma = 0;
            ApplVar.FKpGroup = 0;
        }
        //<<<<<<<<<<<<<<<<<<<<<

        ApplVar.TaxTemp = 0;
        ApplVar.FBalance = 0;   //cc 2006-06-30 for Balance
        break;
    case RPF:
        RFeed(1);
        break;
    case JPF:
        JFeed();
        break;
    case TAXST1:        /* tax shift transaction */
        if (ApplVar.FRegi)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
        else
            ApplVar.TaxPerm = 0x03;
        break;
    case TAXSA1:        /* tax shift article */
        if (ApplVar.FPb)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        else
            ApplVar.TaxTemp = 0x03;
        break;
    case MULT:     /* multiply key */
        if (!Appl_EntryCounter)
        {
#if ((defined(CASE_EJSD) || defined(CASE_EJFLASH)))
            saveCp = (TESTBIT(ApplVar.EJVar.ContFlag,EJVIEWPRINT)!=0);
            lastEJ = ApplVar.EJHeader.AddrLast2EJ;//保存最后一个有效的EJ
            DisplayEJInfo(saveCp);
            if (saveCp)
                RESETBIT(ApplVar.EJVar.ContFlag,EJVIEWPRINT);
            else
                SETBIT(ApplVar.EJVar.ContFlag,EJVIEWPRINT);
            ApplVar.EJHeader.AddrLast2EJ =lastEJ;

#else
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
#endif
            break;
        }
        CheckMultiply();
        if (ApplVar.CentralLock==MG && ApplVar.FInv==3)
            p=DispAmtStr(Msg[JIAGE].str, &ApplVar.Qty,SCREENWD);  //ccr2017-03-17 DISLEN
        else
            p=DispQtyStr(Msg[SHULIANG].str, &ApplVar.Qty,SCREENWD);//ccr2017-03-17 DISLEN
        DispStrXY(p,0,0xff);//ccr2017-03-17
//ccr2016-02-29        PutsO(p);
//ccr2016-02-29        DispSt2C(p);//RG-DISPLAY
        break;
    case SUB:     /* Subtotal function */
    case SUB1:  /* Subtotal with Print */
        if (Appl_EntryCounter)
        {
            if (!ApplVar.FRegi && !ApplVar.FInv && ApplVar.AP.Plu.Number)
            {
                if (ApplVar.CentralLock != MG)
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI28);   /* only manager */
                    break;
                }

#if	(DD_CHIPC==1 || DD_ECRVIP==1)
                if (ApplVar.NumberEntry &&  ApplVar.NumberEntry <= 14)//ccr chipcard
                {
                    ClearEntry(false);
                    //ccr chipcard>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    Appl_ProgType = ApplVar.NumberEntry-4+MGSETUPIC;
#if (CASE_MFRIC==1)
                    if ((ApplVar.NumberEntry>=4 && ApplVar.NumberEntry<=7)
											  && (!TESTBIT(ECRVIP.ECRVIPState,VIP_LOGIN) || Appl_ProgType==SETCHIPPOINT))//ccr2016-05-23
                    {
#if	(DD_CHIPC==1)
                        ChipCard();
#endif
#else
                    //经理档下的IC卡设置
                    if (ApplVar.NumberEntry>=4 && ApplVar.NumberEntry<=7 &&
                        ((ApplVar.IC.CHIP_Flag>=0 || ApplVar.IC.CHIP_Flag==IC_NEWERR || ApplVar.IC.CHIP_Flag==IC_DATEERR)
                         || (!TESTBIT(ECRVIP.ECRVIPState,VIP_LOGIN) || Appl_ProgType==SETCHIPPOINT))//ccr2016-05-23
                        )
                    {
#endif

                        /*if (ApplVar.IC.CHIP_Flag!=1 && Appl_ProgType==SETCHARGIC ||
                            ApplVar.IC.CHIP_Flag!=IC_NEWERR && Appl_ProgType==SETINITIC ||
                            ApplVar.IC.CHIP_Flag==IC_DATEERR && Appl_ProgType!=SETCLEARIC)//ccr040805*/
                        if (((ApplVar.IC.CHIP_Flag!=1 && ApplVar.IC.CHIP_Flag!=2) && Appl_ProgType==SETCHARGIC
                            || ApplVar.IC.CHIP_Flag!=IC_NEWERR && Appl_ProgType==SETINITIC //不是新卡,不能初始化
                            || ApplVar.IC.CHIP_Flag==IC_DATEERR && Appl_ProgType!=SETCLEARIC)
                            && (!TESTBIT(ECRVIP.ECRVIPState,VIP_LOGIN) || Appl_ProgType!=SETCHIPPOINT))//ccr2016-05-23
                        {
                            if (!ApplVar.ErrorNumber)
                                ApplVar.ErrorNumber=ERROR_ID(CWXXI50);
                        }
                        else
                        {
                            ApplVar.CentralLock = SETUPMG | MG;
                            Appl_ProgStart = 0;
                            Appl_ProgNumber = 0;
                            Appl_ProgLine = 0;
                            Appl_BitNumber = 0;
                            ApplVar.KeyNo = ApplVar.AP.FirmKeys[ID_ENTER];
                            ApplVar.ErrorNumber=0;
                            CheckFirmKey();
                        }
                        break;
                    }
#else
                if (ApplVar.NumberEntry &&  ApplVar.NumberEntry <= 3)//ccr chipcard
                {
                    ClearEntry(false);
#endif
                    //ccr chipcard<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//					ApplVar.PrintLayOut = 0x07;//double high ccr040805

                    if (ApplVar.NumberEntry == 3)
                    {
                        ApplVar.FInv = 3;
#if (SCRSTATDBL)
                        DispOnStatDBL((char *)Msg[GBDPJGE].str,0);
#else
                        DispSt1(Msg[GBDPJGE].str);  //RG-DISPLAY
#endif
                        if (TESTBIT(PBPRINT,BIT5))
                        {
                            PrintMessage(GBPLUJGE);
                            DispStrXY(Msg[GBPLUJGE].str,0,0);//ccr2016-01-18
                        }
                        break;
                    }
                    else if (ApplVar.AP.Plu.InvSize)
                    {
                        if (ApplVar.NumberEntry == 1)
                        {
                            ApplVar.FInv = 1;
#if (SCRSTATDBL)
                            DispOnStatDBL((char *)Msg[ZJKCUN].str,0);
#else
                            DispSt1(Msg[ZJKCUN].str);//RG-DISPLAY
#endif
                        }
                        else if (ApplVar.NumberEntry == 2)
                        {
                            ApplVar.FInv = 2;
#if (SCRSTATDBL)
                            DispOnStatDBL((char *)Msg[JSHKCUN].str,0);
#else
                            DispSt1(Msg[JSHKCUN].str);//RG-DISPLAY
#endif
                        }
                        else
                            ApplVar.ErrorNumber=ERROR_ID(CWXXI04);
                        if (TESTBIT(PBPRINT,BIT5) && ApplVar.FInv)//ccr chipcard
                        {
                            PrintMessage(ZJIAKUCUN+ApplVar.FInv-1);//ccr2016-01-18
                            DispStrXY(Msg[ZJIAKUCUN+ApplVar.FInv-1].str,0,0);//ccr2016-01-18
                        }
                        break;
                    }
                }
            }
            ApplVar.ErrorNumber=ERROR_ID(CWXXI04);
            break;
        }
        else if (ApplVar.FInv)  /* inventory adjustment ? */
        {
            if (TESTBIT(PBPRINT,BIT5))    //lyq20031230
                ReceiptIssue(1);
            ApplVar.FInv = 0;
            ApplVar.FRegi = 0;
            ApplVar.FSale = 0;
            ApplVar.OldClerk = 0;     /* force clerk print start transaction */
            ApplVar.OldSalPer = 0;

            RESETBIT(ARROWS, BIT1); /* reset RG led */
//ccr2016-03-02            PutsO(&EntryBuffer[ENTRYSIZE - DISLEN - 1]);//C-DISPLAY
            ClsState2();
            break;
        }
        else
        {
            if (!ApplVar.FRegi && !ApplVar.FTend && (COPYRECEIP & 0x0f) && ApplVar.CopyReceipt<(COPYRECEIP & 0x0f))
            {// 复制收据 //
                if (ApplVar.RGItems.RGNumber)
                {
                    saveCp = ApplVar.CopyReceipt+1;
                    saveclerk = ApplVar.ClerkNumber ;
                    SaveSP = SLIP;          //20040330
                    SLIP = 0;              //20040330
                    ApplVar.BufRec = 1;// 为复制收据 //
                    SaveRGNumber = ApplVar.RGItems.RGNumber;
                    ProcessBuffer();    /* second receipt */
                    ApplVar.RGItems.RGNumber = SaveRGNumber;
                    ApplVar.ClerkNumber = saveclerk ;
                    ApplVar.CopyReceipt = saveCp;
                    SLIP = SaveSP;                   //20040330
                    break;
                }
                ApplVar.SubTotal = ZERO;
            }
            if (ApplVar.FRegi && !ApplVar.FTend)   /* not tendering */
            {
                CalculateTax(1);        /* calculate and not add report */
                ApplVar.FSub = 1;       /* set subtotal pressed */
                Collect_Data(SUBTOTALLOG);     //cc050816
            }
            //Collect_Data(SUBTOTALLOG);    //cc050816  ApplVar.CopyReceipt
        }
        ApplVar.RepeatCount = 0;
#if defined(LCD10_DISP)
        LCD10_SetFlag(FLAG_TOTAL);
        PutsO(DispAmtStr(0, &ApplVar.SubTotal,DISLEN));//C-DISPLAY
#else
        PutsO(DispAmtStr(Msg[SUBTOTAL].str, &ApplVar.SubTotal,DISLEN));//C-DISPLAY
#endif
        RESETBIT(ApplVar.MyFlags,(HANZIMODE | PWDMODE));//ccr20140514 清除汉字输入和密码输入标志  //
        RESETBIT(ApplVar.ScreenStat,HZINPUTMODE);     //ccr20140514

        ApplVar.FCurr = 0;
        if (ApplVar.Key.Code == SUB && ApplVar.FRegi && !ApplVar.FTend) //ccr101111
        {
            StoreInBuffer();//ccr20130425 将最后销售的项目存入RGBuf中
            ApplVar.PrintLayOut = 0x0b; /* print R/J and Slip */
            PrintAmt(Msg[XIAOJI].str, &ApplVar.SubTotal);
            //ccr20130425 为了在打开挂起的账单时保持小计操作>>>>>>>>
            ApplVar.RGRec.Key.Code = SUB;	// store overtender
            ApplVar.RGRec.Amt = ApplVar.SubTotal;
            ApplVar.BufCmd = 0;
//增加以下两个语句,会引起取消交易时,折扣统计出错
//????            StoreInBuffer();
//????            ApplVar.RGRec.Key.Code = 0;
            //<<<<<<<<<<<<<<<<<<
        }
        if (ApplVar.FBuffer)    /* Trans buffer full ? */
        {
            ApplVar.BufKp = 1;
            ApplVar.BufCC = 1;
            temp[0] = ApplVar.FReceipt;
            ProcessBuffer();
            ApplVar.BufKp = 0;
            ApplVar.BufCC = 0;
            ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
            ApplVar.FReceipt = temp[0];
#if (salNumber)
            if (ApplVar.AP.SalPer.Number)
            {
                ApplVar.RGRec.Key.Code = SALPER + ApplVar.SalPerNumber;  /* store in buffer for correct CC update */
                StoreInBuffer();
            }
#endif
            ApplVar.RGRec.Key.Code = CLERK + ApplVar.ClerkNumber;
        }
        break;
    case NPRICE:      /* new price entry for PLU*/
        if (AmtInputMask())
            return;
        if (Appl_EntryCounter && !ApplVar.DecimalPoint)
        {
            ApplVar.Price = ApplVar.Entry;
            ApplVar.FPrice = 1;
            ClearEntry(false);
        }
        else
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        break;
    case NUMBER2:   /* same as NUMBER1 but when no entry Open Drawer */
    case NUMBER1:   /* number entry with direct print */
    case NUMBER:  /* number entry */
        if (Appl_EntryCounter)
        {
            if (Appl_EntryCounter < 17)
            {
                ApplVar.NumberEntered = ApplVar.Entry;
                Collect_Data(NOADDLOG);
                ApplVar.NumberEntered.Sign |= Appl_EntryCounter * 4;
                if (ApplVar.Key.Code != NUMBER)
                {
                    ApplVar.PrintLayOut = 0x0b; /* Slip, R, J */
                    if (RegiStart())
                        return;
                    StoreInBuffer();
                    PrintNumber(&ApplVar.NumberEntered);
                    ApplVar.FNum = 2;
                }
                else
                    ApplVar.FNum = 1;
                ClearEntry(false);
            }
            else
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            break;
        }
        else if (ApplVar.Key.Code != NUMBER2)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            break;
        }
    case ODRAW:  /* drawer open */
        if (!ApplVar.ClerkNumber)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI33);   /* select clerk */
#if (salNumber)
        else if (ApplVar.AP.SalPer.Number && !ApplVar.SalPerNumber)
        {
            if (!TESTBIT(KEYTONE, BIT6))
            {
                ApplVar.SalPerNumber = 1;
            }
            else
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI34);       /* select salesperson */
                return;
            }
        }
#endif
        else
        {
            OpenDrawer();
            if (!ApplVar.FRegi)
            {
                if (TESTBIT(CLERKFIX, BIT0) && !ApplVar.ClerkLock)    /* clerk compulsory, no key */
                {
                    ApplVar.ClerkNumber = 0;
                    ApplVar.FTrain = 0;
                }
#if (salNumber)
                if (TESTBIT(KEYTONE, BIT6))    /* Salesp compulsory */
                    ApplVar.SalPerNumber = 0;
                else if (TESTBIT(AM_PM, BIT4))    /* Salesp to 1 */
                {
                    ApplVar.SalPerNumber = 1;
                    ReadSalPer();
                }
#endif
            }
        }
        break;
    case KPGROUP :
        ClsState2();
        DispSt2(Msg[CHFDYJZU].str);
        ApplVar.FKpGroup = 1;
        break;
    case PROFORMA:  /* proforma invoice only receipt */
    case PROFORMA1: /* print on receipt and journal */
        if (ApplVar.FRegi)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            break;
        }
        strncpy(temp + 1, Msg[XSHFPIAO].str,sizeof(temp)-1);
        temp[0] = ' ';
        if (!Appl_EntryCounter)
        {
#if (SCRSTATDBL)
            DispOnStatDBL(temp+1,0);
#else
            DispSt2C(temp);//RG-DISPLAY
#endif
            if (ApplVar.Key.Code == PROFORMA)
                ApplVar.FProforma = 1;
            else
                ApplVar.FProforma = 2;
            break;          /* set proforma receipt */
        }
        if (AmtInputMask())
            break;
        ApplVar.PrintLayOut = 0x08;     /* print receipt or slip */
        CheckSlip();    /* slip active ? */
        if (!ApplVar.SlipLines)
        {
            RFeed(1);
            ApplVar.PrintLayOut = 0x02;
        }
        if (ApplVar.Key.Code == PROFORMA1)
            ApplVar.PrintLayOut |= 0x01;        /* also journal */
        else
        {
            ApplVar.FCanc = 1;  /* skip journal printing */
            ApplVar.CancPrint = 0xfe;
        }
        MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);
        ApplVar.SaleAmt = ApplVar.Entry;
        ApplVar.TaxItem[0] = ApplVar.Entry;     /* always tax 0 */
        PrintAmt(temp + 1, &ApplVar.SaleAmt);
        ApplVar.FSale = 1;
        RegiEnd();

        ApplVar.FSale = 0;
        ApplVar.TendNumber = 0;
        ReadTender();
        PrintAmt(ApplVar.Tend.Name, &ApplVar.SubTotal);

        if (PVAT)
            CalculateTax(3);    /* print inclusive tax */
        ReceiptIssue(1);

        //PromtionCheck();

        ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
        ApplVar.FCanc = 0;
        ClearEntry(false);
        break;
    case NFOLIO:        /* new Folio key only outside transaction */
        if (ApplVar.FRegi)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        else
        {
            //ccr2016-03-01 PutsO(" NFOLIO ");
            ApplVar.FNFolio = 1;
        }
        break;
#if defined(ARTLEVEL1) //ccr2016-01-27>>>>
    case ARTLEVEL:
        if (ApplVar.NumberEntry && ApplVar.NumberEntry < 5)
        {
            ApplVar.PluArtLevel = (ApplVar.NumberEntry - 1)*ApplVar.PluKeys;
            memset(SysBuf,' ',(INPUTDISPX>DISLEN)?INPUTDISPX:DISLEN);
            CopyFrStr(SysBuf,Msg[SETPLU].str);
            WORDtoASC(SysBuf+7,ApplVar.PluArtLevel+1);  SysBuf[8] = '-'; WORDtoASC(SysBuf+10,ApplVar.PluArtLevel*2);
#if (SCRSTATDBL)
            DispOnStatDBL(SysBuf,0);
#else
            DispSt2(SysBuf);//RG-DISPLAY
#endif
            ClearEntry(false);
        }
        else
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        break;

    case ARTLEVEL1:      /* plu article levels */
    case ARTLEVEL2:
    case ARTLEVEL3:
    case ARTLEVEL4:
    case ARTLEVEL5:
    case ARTLEVEL6:
    case ARTLEVEL7:
    case ARTLEVEL8:
    case ARTLEVEL9:
        ApplVar.PluArtLevel = (ApplVar.Key.Code - ARTLEVEL1)*ApplVar.PluKeys;
        memset(SysBuf,' ',(INPUTDISPX>DISLEN)?INPUTDISPX:DISLEN);
        CopyFrStr(SysBuf,Msg[SETPLU].str);
        WORDtoASC(SysBuf+7,ApplVar.PluArtLevel+1);  SysBuf[8] = '-'; WORDtoASC(SysBuf+10,ApplVar.PluArtLevel*2);
#if (SCRSTATDBL)
        DispOnStatDBL(SysBuf,0);
#else
        DispSt2(SysBuf);//RG-DISPLAY
#endif
        break;
#endif//ccr2016-01-27<<<<
    case LEVEL:
        if (ApplVar.NumberEntry && ApplVar.NumberEntry <= ApplVar.AP.Plu.Level)
        {
            ApplVar.PluPriceLevel = ApplVar.NumberEntry - 1;
            ClearEntry(false);
            Collect_Data(PRICELISTLOG);
        }
        else
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        break;
    case LEVEL1:      /* plu price levels */
    case LEVEL2:
    case LEVEL3:
    case LEVEL4:
        SysBuf[0] = ApplVar.Key.Code - LEVEL1;
        if ((BYTE)SysBuf[0] < ApplVar.AP.Plu.Level)
        {
            ApplVar.PluPriceLevel = SysBuf[0];
            Collect_Data(PRICELISTLOG);
            break;
        }
    case LOOKPLU:
        if (Appl_EntryCounter)
        {
            ApplVar.Entry.Sign =  0;
            StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);

            if (ApplVar.AP.Plu.RandomSize)
            {
                if (Appl_EntryCounter <= (ApplVar.AP.Plu.RandomSize * 2))
                    ApplVar.PluNumber = GetPluNumber(0, ApplVar.Entry.Value);
                if (!ApplVar.PluNumber)
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI05);
                else
                    ApplVar.PluNumber--;
            }
            else
                ApplVar.PluNumber = ApplVar.NumberEntry-1;
            RESETBIT(ApplVar.MyFlags,LOCKPLU);
        }
        else
            INVERTBIT(ApplVar.MyFlags,LOCKPLU);
        if (!ApplVar.ErrorNumber)
            LookPlu(ApplVar.MyFlags & LOCKPLU);
        ClearEntry(false);
        break;
    case WEIGHT:    //cc 20070918
        if (ApplVar.FBalance==1 || !BalanceCMD())//ccr2017-03-16
        {//DIBAL only
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return;
        }
        break;
    case SUSPEND:
        SuspendReceipt();// 挂起收据 //
        break;
#if (defined(CASE_PERU)) //ccr2016-10-24>>>>>>>>>>>>>>
    case CUSTOMER:
        if (ApplVar.CentralLock != MG && ApplVar.CentralLock != RG || ApplVar.FRegi)
        {
            ApplVar.ErrorNumber = 4;   /* only in MG and RG*/
            break;
        }
        mClearScreen();
        DispStrXY((BYTE*)cMsgCUSTOMER,0,0);
        GotoXY(0,1);
        for (sLp=0;sLp<CUSTLINES;sLp++)
        {
            saveclerk = GetStringCHN(SysBuf,CUSTWIDTH-1,true);
            if (saveclerk>0)
            {
                memset(ApplVar.CustomerMessage.Messages[sLp],0,CUSTWIDTH);
                memcpy(ApplVar.CustomerMessage.Messages[sLp],SysBuf+CUSTWIDTH-saveclerk-1,saveclerk);
                ApplVar.CustomerMessage.Messages[sLp][saveclerk]=0;
            }
            else
            {
                ApplVar.CustomerMessage.Messages[sLp][0]=0;
                break;
            }
        }
        ApplVar.CustomerMessage.CopyOfMess = 0;
        break;
#endif//ccr2016-10-24<<<<<<<<<<<<<<<<

    default:
        ApplVar.ErrorNumber=ERROR_ID(CWXXI42);
        break;
    }
}
/**
 * 处理键盘宏
 *
 * @author EutronSoftware (2016-03-16)
 *
 * @param keyCode
 */
void  ProcessKeyMacro(WORD keyCode)
{
    static int currKey=0;
    int macroIdx;


    if (currKey==0)//避免重入
    {
        macroIdx=keyCode-MACROFUNC1;
        for (;currKey<KEYMACROMAX;currKey++)
        {
            ApplVar.Key.Code=ApplVar.AP.KeyMacros[macroIdx].FuncList[currKey];
            if (ApplVar.Key.Code)
            {
                //ccr2016-08-25>>>>>>>>>>>>>>>>>
                if ((ApplVar.Key.Code>='0' && ApplVar.Key.Code<='9')
                    || ApplVar.Key.Code=='.' || ApplVar.Key.Code==ZERO2)
                {
                    if (!AppendEntry(ApplVar.Key.Code))
                        ApplVar.ErrorNumber = ERROR_ID(CWXXI01);
                    else
                    {
                        LCD10_SetFlag(FLAG_NULL);
                        PutsO(EntryBuffer+ENTRYSIZE-DISLEN-1);//C-DISPLAY
#if (SCRSTATDBL)
                        DispOnStatDBL(EntryBuffer+ENTRYSIZE-SCRDBLWD-1,0);
#else
                        DispSt2(EntryBuffer+ENTRYSIZE-INPUTWIDTH-1);//RG-DISPLAY
#endif
                    }
                }
                else
                {
                    if (Appl_EntryCounter)          /* ApplVar.Entry ? */
                    {
                        if (Appl_EntryCounter <= (BCDLEN * 2))
                        {
                            GetEntry();
                            if (TestChangePointMust())
                            {//对小数进行转换
                                if (!ChangePoint())
                                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                                else
                                    SETBIT(ApplVar.MyFlags,DOTCHANGED);
                            }
                            else
                                RESETBIT(ApplVar.MyFlags,DOTCHANGED);

                        }
                        else
                            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);                /* maximum 16 digits entry */
                    }
                    if (!ApplVar.ErrorNumber)
                        ProcessFunctions();
                }//ccr2016-08-25<<<<<<<<<<<<<<<<
                if (ApplVar.ErrorNumber)
                    break;
            }
            else
                break;
        }
        currKey=0;
    }
}

/**
 * 选择FuncLook中的一个功能码
 *
 * @author EutronSoftware (2016-03-18)
 *
 * @param keyCode
 */
void  ProcessFuncLook(WORD keyCode)
{

    BCD saveEntry=ApplVar.Entry;
    BYTE saveEntryCounter=Appl_EntryCounter;
    WORD saveNumber=ApplVar.NumberEntry;
    WORD macroIdx;


    FuncLookNum=keyCode-FUNCLOOK1;
    macroIdx=ListItems(FUNCLOOK1,TRUE,0,false);
    if (macroIdx>0 && macroIdx<=FUNCLOOK_KEYS)
    {
        ApplVar.Entry=saveEntry;
        Appl_EntryCounter=saveEntryCounter;
        ApplVar.NumberEntry=saveNumber;
        ApplVar.Key.Code=FuncLook[FuncLookNum][macroIdx-1];
        if (ApplVar.Key.Code && ApplVar.Key.Code != FUNCLOOK1 && ApplVar.Key.Code != FUNCLOOK2)
        {
            if (!TestChangePointMust() && TESTBIT(ApplVar.MyFlags,DOTCHANGED))
            {//之前对小数进行了转换
                BcdDiv100(&ApplVar.Entry);
            }
            ProcessFunctions();
        }
    }
}

