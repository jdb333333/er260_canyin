#define  PLU_C 2
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

void Add_New_PLU(WORD pPluNumber);

/***************************************************************/

void GetPluOffSet()
{
    RamOffSet = (UnLong)ApplVar.PluNumber*ApplVar.AP.Plu.RecordSize+ApplVar.AP.StartAddress[AddrPLU];
}


void AddPluTotal()
{
    GetPluOffSet();
    RamOffSet += ApplVar.AP.Plu.TotalOffSet;
    for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
    {
        ApplVar.Size = ApplVar.AP.Plu.Size[ApplVar.PointerType];
        AddPointerTotal();
    }
}


void WritePluInventory()
{
    GetPluOffSet();
    RamOffSet += (ApplVar.AP.Plu.TotalOffSet - ApplVar.AP.Plu.InvSize);
    if (ApplVar.AP.Plu.InvSize)
        WriteRam((BYTE*)&ApplVar.PluInventory, ApplVar.AP.Plu.InvSize);
}


/**
 * 在指定的地址RamOffSet上写入单品数据
 *
 * @author EutronSoftware (2014-12-10)
 */
void WritePlu_At()
{
    ULONG i,begin;

    {
        begin = RamOffSet;

        WriteRam(ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
        WriteRam(ApplVar.Plu.Name, ApplVar.AP.Plu.CapSize);
        if (ApplVar.AP.OFFPrice.Number)
            WriteRam((BYTE*)&ApplVar.Plu.OFFIndex,sizeof(ApplVar.Plu.OFFIndex));
        WriteRam(&(ApplVar.Plu.Dept), sizeof(ApplVar.Plu.Dept));
        if (TESTBIT(ApplVar.AP.Options.Plu, BIT1))      /* Large ApplVar.Dept ? */
            WriteRam((BYTE*)&ApplVar.Plu.DeptHi, sizeof(ApplVar.Plu.DeptHi));
        for (i = 0; i < ApplVar.AP.Plu.Level; i++)
            WriteRam(ApplVar.Plu.Price[i], ApplVar.AP.Plu.PriceSize);
        if (ApplVar.AP.Plu.Cost)
            WriteRam(ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
        if (TESTBIT(ApplVar.AP.Options.Plu, BIT0))              /* ApplVar.Plu Link */
            WriteRam((BYTE*)&ApplVar.Plu.Link, sizeof(ApplVar.Plu.Link));
        if (ApplVar.AP.Plu.InvSize)
            WriteRam((BYTE*)&ApplVar.Plu.Inventory, ApplVar.AP.Plu.InvSize);
        begin = RamOffSet - begin;
        if (begin<ApplVar.AP.Plu.RecordSize)
        {
            MemSet(SysBuf, ApplVar.AP.Plu.RecordSize, 0);   /* clear plu record */
            WriteRam(SysBuf, ApplVar.AP.Plu.RecordSize-begin);    /* clear new or last record */
        }
    }
}

void WritePlu()
{
    ULONG i,begin;

    if (ApplVar.PluNumber < ApplVar.AP.Plu.Number)
    {
        GetPluOffSet();

        WriteRam(ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
        WriteRam(ApplVar.Plu.Name, ApplVar.AP.Plu.CapSize);
        if (ApplVar.AP.OFFPrice.Number)
            WriteRam((BYTE*)&ApplVar.Plu.OFFIndex,sizeof(ApplVar.Plu.OFFIndex));
        WriteRam(&(ApplVar.Plu.Dept), sizeof(ApplVar.Plu.Dept));
        if (TESTBIT(ApplVar.AP.Options.Plu, BIT1))      /* Large ApplVar.Dept ? */
            WriteRam((BYTE*)&ApplVar.Plu.DeptHi, sizeof(ApplVar.Plu.DeptHi));
        for (i = 0; i < ApplVar.AP.Plu.Level; i++)
            WriteRam(ApplVar.Plu.Price[i], ApplVar.AP.Plu.PriceSize);
        if (ApplVar.AP.Plu.Cost)
            WriteRam(ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
        if (TESTBIT(ApplVar.AP.Options.Plu, BIT0))              /* ApplVar.Plu Link */
            WriteRam((BYTE*)&ApplVar.Plu.Link, sizeof(ApplVar.Plu.Link));
        if (ApplVar.AP.Plu.InvSize)
            WriteRam((BYTE*)&ApplVar.Plu.Inventory, ApplVar.AP.Plu.InvSize);
    }
}

void ReadPlu()
{
    BYTE i;
    BYTE temp[BCDLEN];

    GetPluOffSet();
    ReadRam(ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
    ReadRam(ApplVar.Plu.Name, ApplVar.AP.Plu.CapSize);
    if (ApplVar.AP.OFFPrice.Number)
        ReadRam((BYTE*)&ApplVar.Plu.OFFIndex,sizeof(ApplVar.Plu.OFFIndex));
    else
        ApplVar.Plu.OFFIndex = 0;
    ApplVar.Plu.Name[ApplVar.AP.Plu.CapSize] = 0;
    ReadRam((BYTE*)&ApplVar.Plu.Dept, sizeof(ApplVar.Plu.Dept));
    ApplVar.PluDept = ApplVar.Plu.Dept;
    if (TESTBIT(ApplVar.AP.Options.Plu, BIT1))      /* Large ApplVar.Dept ? */
    {
        ReadRam((BYTE*)&ApplVar.Plu.DeptHi, sizeof(ApplVar.Plu.DeptHi));
        *(((BYTE *)&ApplVar.PluDept)+1) = ApplVar.Plu.DeptHi;
    }
    for (i = 0; i < ApplVar.AP.Plu.Level; i++)
        ReadRam(ApplVar.Plu.Price[i], ApplVar.AP.Plu.PriceSize);
    if (ApplVar.AP.Plu.Cost)
        ReadRam(ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
    if (TESTBIT(ApplVar.AP.Options.Plu, BIT0))              /* ApplVar.Plu Link */
        ReadRam((BYTE*)&ApplVar.Plu.Link, sizeof(ApplVar.Plu.Link));

    if (ApplVar.AP.Plu.InvSize)
    {
        ApplVar.PluInventory = ZERO;
        ReadRam((BYTE*)&ApplVar.PluInventory, ApplVar.AP.Plu.InvSize);
        ApplVar.Plu.Inventory = ApplVar.PluInventory;
    }
    RESETBIT(ApplVar.MyFlags,PLUOFF);//PLUPACK);//ccr 050613
    RESETBIT(ART_LEVEL, BIT2);

#if (BARCUSTOMER==1)
    if (ApplVar.FBarcode==6)
        return;
#endif

    if ((ApplVar.CentralLock == MG || ApplVar.CentralLock == RG))//add by ccr,for OFF
    {

#if (BARCUSTOMER==1)
        if (ApplVar.BarCustomer.OFFIndex>0 && ApplVar.BarCustomer.DeptHi>0 && ApplVar.BarCustomer.DeptHi<=ApplVar.AP.Plu.Level)
            ApplVar.PluPriceLevel = ApplVar.BarCustomer.DeptHi-1;       //ccr chipcard
        else
#endif
            if (ApplVar.Plu.OFFIndex && ApplVar.Plu.OFFIndex<=ApplVar.AP.OFFPrice.Number)
        {
            ApplVar.OFFNumber = ApplVar.Plu.OFFIndex-1;
            ReadOFFPrice();
            if (ApplVar.OFFPrice.Type)
            {
                GetTimeDate(&Now);
                temp[3] = Now.month;    temp[2]=Now.day;
                temp[1] = Now.hour;     temp[0]=Now.min;

                if (CWORD(temp[2]) >= CWORD(ApplVar.OFFPrice.DateFrom[0]) &&
                    CWORD(temp[0]) >= CWORD(ApplVar.OFFPrice.TimeFrom[0]) &&
                    CWORD(temp[2]) <= CWORD(ApplVar.OFFPrice.DateTo[0]) &&
                    CWORD(temp[0]) <= CWORD(ApplVar.OFFPrice.TimeTo[0]) &&
                    (ApplVar.OFFPrice.WeekDay & (1<<(Now.dow-1))))
                {
                    memset(ApplVar.Plu.Price[1], 0, sizeof(ApplVar.Plu.Price[0]));
                    SETBIT(ART_LEVEL, BIT2);
                    if (ApplVar.OFFPrice.Type == 1)
                    {
                        SETBIT(ApplVar.MyFlags,PLUPACK);
                        memset(temp,0,sizeof(temp));
                        ULongToBCDValue(temp,ApplVar.OFFPrice.OFFVal.TYPE1.NumItems);
                        memcpy(ApplVar.Plu.Cost, temp,sizeof(ApplVar.Plu.Cost));
                        memset(ApplVar.Plu.Price[0], 0, sizeof(ApplVar.Plu.Price[0]));
                        memcpy(ApplVar.Plu.Price[0],ApplVar.OFFPrice.OFFVal.TYPE1.PriceU, ApplVar.AP.OFFPrice.PriUSize);
                        memcpy(ApplVar.Plu.Price[1],ApplVar.OFFPrice.OFFVal.TYPE1.PriceP, sizeof(ApplVar.OFFPrice.OFFVal.TYPE1.PriceP));
                    }
                    else
                    {
                        SETBIT(ApplVar.MyFlags,PLUDISC);
                        memcpy(ApplVar.Plu.Price[1],ApplVar.OFFPrice.OFFVal.Discount, 2);
                    }
                }
            }
        }
        else if (ApplVar.IC.CHIP_Flag>=0 && (ApplVar.IC.REC_Customer[CC_CLIST] & 3)>0 && (ApplVar.IC.REC_Customer[CC_CLIST] & 3)<ApplVar.AP.Plu.Level)//ccr chipcard
        {
            //ccr040805>>>>>>
            ApplVar.PluPriceLevel = (ApplVar.IC.REC_Customer[CC_CLIST] & 3);//-1;		//ccr chipcard
/*			i = (ApplVar.IC.REC_Customer[CC_CLIST] & 3)-1;		//ccr chipcard
            if (i)
                memcpy(ApplVar.Plu.Price[0],ApplVar.Plu.Price[i], sizeof(ApplVar.Plu.Price[0]));*/
        }//ccr040805<<<<<<<<<<<<
    }
}

/**
 *
 *
 * @author EutronSoftware (2014-12-10)
 *
 * @param update :if update == 1 then also return already deleted PLU
 * @param number :BARCode of plu
 *
 * @return WORD :return 0 if not found
 *               else ENTRYNUMBER + 1 and set RamOffSet to Start location
 */
WORD GetPluNumber(BYTE update,BYTE *number)
{
    RamOffSet = ApplVar.AP.StartAddress[AddrPLU];    /* start plu table */
    return  BinarySearch(number, ApplVar.AP.Plu.RecordSize, ApplVar.AP.Plu.RandomSize, ApplVar.AP.Plu.RNumber);

}
/**
 *
 * @author EutronSoftware (2014-12-10)
 *
 * 入口数据要求: RamOffSet指向单品所在的位置
 * 当增加一个新单品时,单品数据将被写入单品文件,同时返回其PluNumber
 *
 * @param fdelete: 0-add plu if Random not found,
 *                 1-delete plu if Random found
 * @param frHost 1-data from computer,0-data from keyboard
 *
 * @return short
 */
short CheckRandomPlu(BYTE fdelete,BYTE frHost)
{
    WORD mcount, c, r;
    BYTE i;
    UnLong bcount, endaddr ;   /* unsigned long bcount, endaddr; */

    if (!frHost)      /* via keyboard then confirm */
    {//提示是否增加不存在的单品或删除已存在的单品
        memset(SysBuf,0,INPUTWIDTH);
        strcpy(SysBuf, Msg[ZENJIAA+ fdelete].str);
        c=strlen(SysBuf);
        ApplVar.PrintLayOut = 0x03;     /* receipt and journal */
        if (!WaitForYesNo(SysBuf,true))
            return 0;
    }
    if (fdelete)
    {
        ApplVar.AP.Plu.RNumber--;
        MemSet(&ApplVar.Plu, sizeof(ApplVar.Plu), 0);   /* clear plu record */
    }
    else if (ApplVar.AP.Plu.RNumber >= ApplVar.AP.Plu.Number) /* full ? */
    {
        if (!frHost)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI06);
        return 0;
    }

    bcount = ApplVar.AP.Plu.RNumber;     /* calculate bytes to move  1st part */
    bcount = bcount * ApplVar.AP.Plu.RecordSize+ ApplVar.AP.StartAddress[AddrPLU];
    endaddr = bcount;       /* save endaddress for insertion */
    bcount -= RamOffSet;    //bcount为需要移动的存储区大小
    c = bcount / 128 ;        /* /128L number of 128 byte blocks */
    r = bcount % 128 ;        /*  %128L remainder */
    mcount = 128;           //一次移动128字节
    i = 0;
    if (fdelete)
    {//删除单品

        for (;;)     /* move <-- length bcount */
        {
            if (!c)
            {
                if (!r)
                    break;
                mcount = r;     /* remainder */
                r = 0;
            }
            else
                c--;

            //移动单品数据:删除一个单品时,被删除单品之后的单品数据向前移动;
            //存储区地址连续时可以考虑采用SRAM_Move来完成
            RamOffSet += ApplVar.AP.Plu.RecordSize;
            ReadRam(SysBuf, mcount);
            RamOffSet -= (ApplVar.AP.Plu.RecordSize + mcount);
            WriteRam(SysBuf, mcount);
        }
        RamOffSet = endaddr;
        bcount = RamOffSet;
    }
    else
    {//增加单品
        bcount = RamOffSet;     /* save insert address */
        for (;;)     /* move --> length bcount */
        {
            if (!c)
            {
                if (!r)
                    break;
                mcount = r;     /* remainder */
                r = 0;
            }
            else
                c--;
            //移动单品数据:增加一个单品时,把由RamOffSet所指位置之后的所有单品向后移动一个单品的位置
            RamOffSet = endaddr - mcount;
            endaddr = RamOffSet;
            ReadRam(SysBuf, mcount);
            RamOffSet = endaddr + ApplVar.AP.Plu.RecordSize; /* move over length 1 record */;
            WriteRam(SysBuf, mcount);
        }
        RamOffSet = bcount;
        if (!frHost)//data from keyboard
        {
            memcpy(ApplVar.Plu.Random, ApplVar.Entry.Value, ApplVar.AP.Plu.RandomSize);
            WriteRam(ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);    /* clear new or last record */
            MemSet(SysBuf, sizeof(SysBuf), 0);   /* clear plu record */
            WriteRam(SysBuf, ApplVar.AP.Plu.RecordSize - ApplVar.AP.Plu.RandomSize);    /* clear new or last record */
        }
        else
        {//ccr2014-12-10 将新单品数据写入
            ApplVar.PluNumber = (RamOffSet-ApplVar.AP.StartAddress[AddrPLU])/ApplVar.AP.Plu.RecordSize;
            WritePlu_At();
        }

        ApplVar.AP.Plu.RNumber++;
    }
    if (!frHost
        && ApplVar.CentralLock != RG)//SetList.isSub:SETPLU在销售过程中增加单品时,不打印此信息
    {
        if (fdelete)
            strcpy(ProgLineMes,Msg[SHCHDP].str);
        else
            strcpy(ProgLineMes,Msg[ZJDPIN].str);
        if (!TESTBIT(PBPRINT,BIT5))
        {
            DispStrLF(FormatQtyStr(ProgLineMes, &ApplVar.Entry, SCREENWD));
        }
        else
        {
            if (TESTBIT(PBPRINT,BIT5))
            {
                PrintQty(ProgLineMes, &ApplVar.Entry);
                SETBIT(ApplVar.MyFlags,PRNTRAI);
            }
        }
    }
    return 1;
}

void ProcessOnePlu()
{
    WORD    sPluNumber=0;//不为0时,表示为销售过程中新增加的单品
    int     i;
    BCD     sSave;

    if ((ApplVar.Key.Code == ApplVar.OldKey.Code && !ApplVar.OldMultiCount) || ApplVar.FVoid)
        ProcessDept();
    else
    {
#if (defined(CASE_INDONESIA) && (defined(CASE_GPRS)))
        //在没有打开收据的情况下，检查是否必须发送流水
        if (!ApplVar.FRegi && !TESTBIT(ApplVar.DaysMust,BIT7) && ApplVar.DaysMust && //ccr2015-05-06 不是自动发送数据，检查是否超过天数
            ((GetDaysFrom1900(Now.year,Now.month,Now.day) - ApplVar.LastDaysLog)>=ApplVar.DaysMust))
        {
            if (ApplVar.FlowHeader.ReadP==ApplVar.FlowHeader.NextNP)//ccr2015-05-06无流水数据，
                ApplVar.LastDaysLog = GetDaysFrom1900(Now.year,Now.month,Now.day);
            else
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI82);
                return;
            }
        }
#endif

        if (ApplVar.Key.Code == PLU1)
        {//为间接单品销售
            ApplVar.PluNumber = 0;
            if (!ApplVar.FBarcode)          /* id code scanned then already checked */
            {
                CheckInStore(); /* also in store codes trhough keyboard */
                GetWordEntry();
            }
            if (Appl_EntryCounter)
            {
                if (ApplVar.AP.Plu.RandomSize)// && Appl_EntryCounter>4)//============================
                {//根据单品条码读取单品数据
                    if (Appl_EntryCounter <= (ApplVar.AP.Plu.RandomSize * 2))
                        ApplVar.PluNumber = GetPluNumber(0, ApplVar.Entry.Value);
                }
                else if (ApplVar.FBarcode != 2)
                {//根据单品序号读取单品数据
                    ApplVar.PluNumber = ApplVar.NumberEntry;
#if defined(ARTLEVEL1) //ccr2016-01-27>>>>
                    if (ApplVar.PluNumber && TESTBIT(ART_LEVEL, BIT4) && ApplVar.FPluEnq != 2)
                        ApplVar.PluNumber += ApplVar.PluArtLevel;       /* add article shift ?*/
#endif
                }
            }
        }
        else
        {//为直接单品销售
            if (Appl_EntryCounter)
            {
                if (TESTBIT(PLU_PRINT, BIT7) && !ApplVar.FInv && ApplVar.FPluEnq != 2)    /* no entry allowed */
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                else
                    CheckMultiply();
                if (ApplVar.ErrorNumber)
                    return;
            }
            ApplVar.PluNumber = ApplVar.Key.Code - PLU1;
#if defined(ARTLEVEL1) //ccr2016-01-27>>>>
            if (ApplVar.FPluEnq != 2)
                ApplVar.PluNumber += ApplVar.PluArtLevel;       /* add article shift */
#endif
        }

        if (!ApplVar.PluNumber || ApplVar.PluNumber > ApplVar.AP.Plu.Number)
        {
            if (ApplVar.FBarcode == 2 || ApplVar.FBarcode == 3)
            {
                ApplVar.Key.Code = DEPT + 1;    /* always in dept 1 */
                ApplVar.Entry = ApplVar.Price;
                ApplVar.FPrice = 0;
                ProcessDept();
                return;
            }
            else
            {//ccr20130508 所要销售的单品不存在,是否增补到数据库中
#if defined(CASE_INDIA)//ccr2016-01-22>>>>>
                ApplVar.ErrorNumber=ERROR_ID(CWXXI05);//"PLU代码为空!"
                return;
#else
                if (!Appl_EntryCounter || !ApplVar.AP.Plu.RandomSize ||
                    !CheckRandomPlu(0, 0))/* 不存在时提示是否增加add */
                {//不增加
                        if (!ApplVar.ErrorNumber)
                            ApplVar.ErrorNumber=ERROR_ID(CWXXI05);//"PLU代码为空!"
                        return;
                 }
                 //增加单品
                 ApplVar.PluNumber = GetPluNumber(0,ApplVar.Entry.Value);
                 sPluNumber = ApplVar.PluNumber;

                 Add_New_PLU(sPluNumber);

                 ApplVar.PluNumber = sPluNumber;

                 //if (sPluNumber>0)//为新增加的单品
                 {//在收据上打印新增单品信息
                     sPluNumber = strlen(Msg[ZJDPIN].str)+1;
                     memset(ProgLineMes,' ',PRTLEN);
                     CopyFrStr(ProgLineMes+1,Msg[ZJDPIN].str);

                     i = strlen(ApplVar.Plu.Name);
                     if (i>PRTLEN-sPluNumber-1)
                         i = PRTLEN-sPluNumber-1;

                     memcpy(ProgLineMes+(PRTLEN-i-1),ApplVar.Plu.Name,i);//单品名称靠右对齐
                     ProgLineMes[PRTLEN]=0;
                     RJPrint(0,ProgLineMes);
                     if (!ApplVar.FRegi)
                         return;//如果没有打开收据,新增加的单品将不引起销售
                 }//如果开始销售,新增加的单品会自动进行销售
#endif
            }
        }
        ApplVar.PluNumber--;        /* plunumber 1 is absolute number 0 */
        ReadPlu();

#if (BARCUSTOMER==1)
        //条码会员功能:检查是否为条码会员码
        if (ApplVar.FBarcode==6)
        {
            memset(&ApplVar.BarCustomer,0,sizeof(ApplVar.BarCustomer));
            ApplVar.PluDept = 0;
            ApplVar.BarCustomer.OFFIndex = ApplVar.PluNumber+1;
//			ApplVar.BarCustomer.Dept = 0;
            memcpy(&ApplVar.BarCustomer.Random,&ApplVar.Plu.Random,sizeof(ApplVar.Plu.Random));
            memcpy(&ApplVar.BarCustomer.Name,&ApplVar.Plu.Name,sizeof(ApplVar.Plu.Name));
            memcpy(&ApplVar.BarCustomer.Price[0],&ApplVar.Plu.Price[0],5);
            ApplVar.BarCustomer.DeptHi = (ApplVar.Entry.Value[0]>>4);
            if (!ApplVar.BarCustomer.DeptHi)
            {
                ApplVar.BarCustomer.Price[1][0] = ApplVar.Entry.Value[1];
                ApplVar.BarCustomer.Price[1][1] = ApplVar.Entry.Value[2];
            }

            sSave = ZERO;
            ApplVar.Price = ZERO;
            memcpy(&ApplVar.Price.Value,ApplVar.Plu.Price[0],5);
            memcpy(sSave.Value,ApplVar.Plu.Random,ApplVar.AP.Plu.RandomSize);
            DispStrLF(FormatQtyStr(Msg[TMHYHAO].str,&sSave,SCREENWD));
            DispStrLF(FormatAmtStr(Msg[XFJE].str,&ApplVar.Price,SCREENWD));

            strcpy(ProgLineMes,FormatQtyStr(0,&sSave,13));
            ProgLineMes[7] = 0;
            ProgLineMes[2] = 'C';
#if defined(LCD10_DISP)
            LCD10_SetFlag(FLAG_PRICE);
            PutsO(FormatAmtStr(0, &ApplVar.Price,DISLEN));//C-DISPLAY
#else
            PutsO(FormatAmtStr(ProgLineMes+2,&ApplVar.Price,DISLEN));//C-DISPLAY
#endif
            return;
        }
#endif

        for (i = 0; i < ApplVar.AP.Plu.PriceSize; i++)
        {
            if ((BYTE)ApplVar.Plu.Price[ApplVar.PluPriceLevel][i] != 0x99 || ApplVar.FInv)
            {
                if ((ApplVar.FBarcode == 2 || ApplVar.FBarcode == 3))
                {//ccr20120131>>>>>>>>>>根据条码中的金额数据自动计算单品的数量
                    ApplVar.Entry = ApplVar.Price;
                    ApplVar.Price = ZERO;
                    memcpy(ApplVar.Price.Value, ApplVar.Plu.Price[ApplVar.PluPriceLevel], ApplVar.AP.Plu.PriceSize);
                    if (CheckNotZero(&ApplVar.Price))
                    {
                        ApplVar.Qty = ApplVar.Entry;
                        Divide(&ApplVar.Qty,&ApplVar.Price);
                        RoundBcd(&ApplVar.Qty,3);
                        ApplVar.MultiplyCount = 1;
                    }
                    else
                    {
                        ApplVar.Qty = ONE;
                        ApplVar.Price = ApplVar.Entry;
                    }
                }//<<<<<<<<<<<<<<<

                ProcessDept();            // LYQ ADDED for divide pricecode into ApplVar.Qty and ApplVar.Price end 20040402
                return;
            }           /* when price is 9999999999 then not active */
        }
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);    /* invalid entry */
    }
}

/* bufchk == 0 then print and calculate */
/* bufchk == 1 then print and check */
/* bufchk == 2 then only calculate ApplVar.DiscAmt */
void CheckMixMatch(short bufchk)
{
    BYTE scmd, sign;
    BCD matchqty, freeqty, soldqty;

    if (TESTBIT(ART_LEVEL, BIT2) && (ApplVar.AP.Plu.Cost || TESTBIT(ApplVar.MyFlags,PLUOFF)))// && !ApplVar.FPb) /* mix match active ? */
    {
        scmd = 0;
        if (TESTBIT(ApplVar.MyFlags,PLUPACK))
        {// for PACKAGE sales use OFF Type-1
            matchqty = ZERO;

            memcpy(matchqty.Value, ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
            if (!CheckNotZero(&matchqty))    /* not active */
                return;
            if (bufchk == 1)
            {
                ApplVar.MixQty = ZERO;
                sign = ApplVar.BufCmd;
                ApplVar.BufCmd = 3;     /* Count the PLU number has input */
                StoreInBuffer();
                ApplVar.BufCmd = sign;
            }
            freeqty = ApplVar.MixQty;
            freeqty.Sign = 0;
            soldqty = ZERO;
            while (CompareBCD(&freeqty, &matchqty) >= 0)
            {
                Subtract(&freeqty, &matchqty);        /* get remainder already sold  */
                Add(&soldqty, (BCD*)&ONE);            /* check number of free items */
            }//soldqty = ApplVar.MixQty - matchqty + 1;
            soldqty.Sign = ApplVar.MixQty.Sign;
            Add(&ApplVar.MixQty, &ApplVar.Qty);     /* add current sale qty */
            sign = ApplVar.MixQty.Sign;
            ApplVar.MixQty.Sign = 0;
            freeqty = ZERO;
            while (CompareBCD(&ApplVar.MixQty, &matchqty) >= 0)
            {
                Subtract(&ApplVar.MixQty, &matchqty);        /* get remainder already sold  */
                Add(&freeqty, (BCD*)&ONE);            /* check number of free items */
            }//soldqty = ApplVar.MixQty - matchqty + 1;
            freeqty.Sign = sign;
            Subtract(&freeqty, &soldqty);
            RoundBcd(&freeqty,0);     //lyq added 2003\10\28
            if (CheckNotZero(&freeqty))    /* not zero */
            {
                ApplVar.DiscAmt = ZERO;
                memcpy(ApplVar.DiscAmt.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);
                Multiply(&ApplVar.DiscAmt, &freeqty);
                ApplVar.DiscAmt.Sign ^= 0x80;
                if (bufchk != 2)
                    scmd = 0x05;//Discount value
            }
        }
        else if (TESTBIT(ApplVar.MyFlags,PLUDISC))//ccr 050613
        {
            // for a PLU sales use OFF 	Type-2

            ApplVar.DiscAmt = ZERO;
            memcpy(ApplVar.DiscAmt.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);
            ApplVar.DiscAmt.Sign = 4;
            Multiply(&ApplVar.DiscAmt, &ApplVar.Price);
            Multiply(&ApplVar.DiscAmt, &ApplVar.Qty);
            RoundBcd(&ApplVar.DiscAmt,0);
            ApplVar.DiscAmt.Sign ^= 0x80;
            if (bufchk != 2)
                scmd = 0x11;//Discount %

            /* set item was match */
        }
        if (scmd != 0)
        {
            PrintAmt(ApplVar.OFFPrice.Name, &ApplVar.DiscAmt);
            for (ApplVar.DiscNumber=0;ApplVar.DiscNumber<ApplVar.AP.Disc.Number;ApplVar.DiscNumber++)
            {
                GetDiscOffSet();
                ReadDisc();
                if (ApplVar.Disc.Options==scmd)
                    break;
            }
            if (ApplVar.DiscNumber < ApplVar.AP.Disc.Number);
            {

                SETBIT(ApplVar.MyFlags,PLUDISC);//ccr 050613
                matchqty = ApplVar.Amt;
                ApplVar.Amt = ApplVar.DiscAmt;

                memcpy(ApplVar.Entry.Value, ApplVar.Plu.Price[1], ApplVar.AP.Plu.PriceSize);

                AddDiscTotal();

                ApplVar.Amt = matchqty;
            }
            if (bufchk != 0)
                Add(&ApplVar.Amt, &ApplVar.DiscAmt);
            ApplVar.FMixMatch = 1;          /* set item was match */
        }
    }
}


WORD GetLinkNumber()
{
    WORD keycode;
    BCD     temp;

    if (!ApplVar.AP.Plu.RandomSize)
        keycode = ApplVar.Plu.Link;
    else
    {
        temp = ZERO;
        WORDtoBCD(temp.Value, ApplVar.Plu.Link);
        keycode = GetPluNumber(0, temp.Value);
    }
    if (keycode)    /* not found */
        keycode += PLU1;

    return(keycode);
}

void ProcessPlu()
{
    BYTE ecnt, multicount, rcnt, correc;
    WORD saveplu, keycode;
    BCD     entr, price, qty1;


    ecnt = Appl_EntryCounter;
    entr = ApplVar.Entry;
    if (ApplVar.Key.Code == ApplVar.OldKey.Code || ApplVar.FVoid)
        multicount = ApplVar.OldMultiCount;//Select the same PLU
    else
        multicount = ApplVar.MultiplyCount;
    keycode = ApplVar.Key.Code;

    if (ApplVar.FVoid)
        correc = 1;
    else if (ApplVar.FCorr || ApplVar.FRefund)
        correc = 2;
    else
        correc = 0;
    ProcessOnePlu();

    rcnt = ApplVar.RepeatCount;
    price = ApplVar.Price;          /* Save data for repeat and void */

    //处理单品的链接销售(捆绑销售)
    if (!TESTBIT(ApplVar.AP.Options.Plu, BIT0) || ApplVar.FInv /* no link with inventory */
        || !ApplVar.Plu.Link)
        return;

    saveplu = ApplVar.PluNumber;    /* not link to itself again !! */
    qty1 = ApplVar.Qty1;
    if (correc == 1)        /* Void ? */
        ApplVar.Qty.Sign ^= 0x80;               /* Invert sign when void */
    ApplVar.FPluEnq = 2;            /* Indicate linked plu */
    while (!ApplVar.ErrorNumber && ApplVar.Plu.Link)
    {
        ApplVar.Key.Code = GetLinkNumber();     /* number in ApplVar.Plu.Link */
        if (!ApplVar.Key.Code)  /* Random Code not found */
            break;
        if (ApplVar.Key.Code == (saveplu + PLU1 + 1)) /*not link to itself */
            break;
        ApplVar.Entry = ApplVar.Qty;
        Appl_EntryCounter = 1;
        ProcessOnePlu();
        if (correc)
            AddCorrecTotal();               /* update correc report */
        ApplVar.RGRec.Key.Code = 0;             /* don't store link in ApplVar.PB and Trans buffer */
    }
    ApplVar.FPluEnq = 0;
    if (correc == 1)        /* Void ? */
        ApplVar.Qty.Sign ^= 0x80;               /* Invert sign when void */
    ApplVar.Qty1 = qty1;     /*     restore */
    ApplVar.RepeatCount = rcnt;
    ApplVar.Price = price;          /* restore data for repeat and void */
    ApplVar.Entry = entr;
    Appl_EntryCounter = ecnt;
    if (correc == 2)
        ApplVar.Key.Code = 0;
    else
        ApplVar.Key.Code = keycode;
    ApplVar.OldMultiCount = multicount;
    ApplVar.PluNumber = saveplu;
    ReadPlu();              /* Restore first PLU recrod */
    ApplVar.DeptNumber = ApplVar.PluDept;
    ReadDept();             /* Restore Department */
}

void LookPlu(BYTE pType)//pType=0,look price,pType>0 look inv
{
    BCD sPrice;
    short i;

    if (ApplVar.PluNumber>ApplVar.AP.Plu.Number)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        return;
    }
    else
    {
        ReadPlu();
        sPrice = ZERO;
        mClearScreen();
        DispStrXY(ApplVar.Plu.Name,0,0);

        memcpy(&sPrice.Value,&ApplVar.Plu.Price[0],ApplVar.AP.Plu.PriceSize);
        DispStrXY(DispAmtStr(Msg[JIAGE].str ,&sPrice,SCREENWD),0,1);

        if (ApplVar.AP.Plu.Cost)
        {
            sPrice = ZERO;
            memcpy(&sPrice.Value,&ApplVar.Plu.Cost,ApplVar.AP.Plu.PriceSize);
            DispStrXY(DispAmtStr(Msg[CHENBEN].str, &sPrice,SCREENWD),0,2);
        }
        else if (ApplVar.AP.Plu.InvSize)
        {
//			sPrice.Sign = ApplVar.Plu.Inventory.Sign;
//			for (i=0;i<ApplVar.AP.Plu.InvSize-1;i++)
//				sPrice.Value[i] = ApplVar.PluInventory.Value[i];
//			PutsO(FormatAmtStr(DMes[33] ,&sPrice,DISLEN));
            ApplVar.Entry = ZERO;
            memcpy(&ApplVar.Entry,&ApplVar.Plu.Inventory,sizeof(ApplVar.Plu.Inventory));
            switch (ApplVar.Entry.Sign &0x7f)
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
            ApplVar.Entry.Sign = (ApplVar.Plu.Inventory.Sign & 0x80) | 3;//ccr20110721
            DispStrXY(DispQtyStr(Msg[KUCUN].str, &ApplVar.Entry,SCREENWD),0,2);
        }
    }
}
/*************************************************************************************
  在销售过程中,发现有不存在的单品时,调用此功能函数进行增补
*************************************************************************************/
void Add_New_PLU(WORD pPluNumber)
{
    BYTE   saveRet;
    WORD  keyno;

	Appl_ProgNumber = pPluNumber - 1;
    Appl_BitNumber = 0;
    ApplVar.ErrorNumber=0;
    ApplVar.FuncOnEnter = 0;
	Appl_ProgLine = 2;//直接进入输入单品名称
    Appl_ProgStart = 1;
    Appl_ProgType = SETPLU;
    ApplVar.CentralLock=SET;
    SetList.isSub = SETPLU;//用来控制CheckFirmkey运行的流程
    ClearEntry(false);
    mClearScreen();
    mEnCursor(ENABLE);
    ProgPlu(); //DisplayOption();

    for (;;)
    {
        keyno = ChnInput(0);
				CheckError(0);//ccr2015-01-29
        if (!keyno)
            continue;

        saveRet = GetFirmkeyID(keyno & 0xff);
        if (saveRet==ID_CANCEL)
            break;//退出编辑
        else if (saveRet==ID_xINPUT)
        {//切换输入方法
            if (TESTBIT(ApplVar.MyFlags,HANZIMODE))
            {
                TESTBIT(ApplVar.ScreenStat,HZINPUTMODE)?ChnInput(3):ChnInput(2);
                continue;
            }
        }
        else if (saveRet==ID_CLEAR)
        {//清除输入
            LineEditor(*((union KEY *)&keyno));//编译按键输入
            ClearEntry(true);
            continue;
        }

        if ((keyno & 0xff00)==0x0100)
        {//为特殊功能按键
            if (!TESTBIT(ApplVar.ScreenStat,HZINPUTMODE))
            {//已经选择了一个汉字 时 //

                if ((keyno & 0x00ff) < MAXKEYB)
                {
                    ApplVar.KeyNo = keyno & 0x00ff;
                    ApplVar.Key.Code = ApplVar.AP.KeyTable[ApplVar.KeyNo];
                    if (CheckFirmKey())
                    {
                        if (Appl_ProgLine==0)//新增单品记录的所有数据输入完毕
                            break;
                        else
                            continue;//继续编辑新单品的数据
                    }
                }
                else
                    continue;
            }
        }
        LineEditor(*((union KEY *)&keyno));//编译按键输入
        Appl_EntryCounter = Editor.EditCount;
    }

    RESETBIT(ApplVar.MyFlags,HANZIMODE + CLSSCREEN);
    ChnInput(3);
    mClearScreen();

    ApplVar.CentralLock=RG;//回到RG状态
#if (SCRSTATDBL)
    DispOnStatDBL(ApplVar.ModeHead,0);
#else
    DispSt0(ApplVar.ModeHead);//RG-DISPLAY
#endif

    Appl_BitNumber = 0;
    ApplVar.ErrorNumber=0;
    SetList.isSub = 0;
    Appl_ProgLine = 0;
    Appl_ProgStart = 0;
    Appl_ProgType = 0;
    ClearEntry(false);
    ApplVar.Key.Code = PLU1;//自动销售此单品
    ApplVar.FPrice = 0;//强制使用新增加单品的价格进行销售
    Appl_MaxEntry=ENTRYSIZE-2;
    mEnCursor(DISABLE);
}

/**
 * 打印单品和部类清单
 *
 * @author EutronSoftware (2016-02-19)
 */
void PrintDept_PLU()
{
    WORD i;

#if defined(CASE_INDIA)
    //打印单品和部类修改日志
    PrintLine2('=');
    memset(&LogDefine,0,sizeof(LogDefine));
    LogDefine.LogType=EDITPLULOG;
    LoadReceipLog();
#endif
    if (ApplVar.AP.Dept.Number)
    {//先打印部类信息
        PrintLine2('=');
        SETBIT(ApplVar.PrintLayOut, BIT2);
        RJPrint(0,cDEPTLIST);
        //"         单 品 信 息 表"
        RESETBIT(ApplVar.PrintLayOut, BIT2);
        RFeed(1);
        PrintLine2('=');
        RJPrint(0,cDEPTTITLE);
        PrintLine2('=');
        for (ApplVar.DeptNumber=0;ApplVar.DeptNumber<ApplVar.AP.Dept.Number;ApplVar.DeptNumber++)
        {
            ApplVar.Price = ZERO;
            ReadDept();
            memcpy(&ApplVar.Price.Value,ApplVar.Dept.Price,sizeof(ApplVar.Dept.Price));
            ApplVar.GroupNumber = ApplVar.Dept.Group;
            ReadGroup();
            ApplVar.Group.Name[ApplVar.AP.Group.CapSize-1]=0;
            memset(ProgLineMes,' ',sizeof(ProgLineMes));
            memcpy(ProgLineMes,ApplVar.Dept.Name,strlen(ApplVar.Dept.Name));
            strcpy(ProgLineMes+14,ApplVar.Group.Name);
            RJPrint(0,FormatAmtStr(ProgLineMes,&ApplVar.Price,PRTLEN));
        }

    }
#if (1)//!defined(DEBUGBYPC)
    if (ApplVar.AP.Plu.RandomSize != 0)
        i = ApplVar.AP.Plu.RNumber;
    else
        i = ApplVar.AP.Plu.Number;
    if ( i !=0 )
    {
        SETBIT(ApplVar.PrintLayOut, BIT2);
        PrintLine2('=');
        RJPrint(0,cPLULIST);
        //"         单 品 信 息 表"
        RESETBIT(ApplVar.PrintLayOut, BIT2);
        PrintLine2('=');
        RJPrint(0,cPLUTITLE1);
        RJPrint(0,cPLUTITLE2);
        PrintLine2('=');
        SetLoop();

        for (ApplVar.PluNumber = 0; ApplVar.PluNumber < i; ApplVar.PluNumber++)
        {
            ApplVar.Qty = ZERO;
            ApplVar.Amt = ZERO;
            ApplVar.Price = ZERO;
            memset(&ApplVar.Plu,0,sizeof(ApplVar.Plu));
            ReadPlu();
            memcpy(&ApplVar.Price.Value,ApplVar.Plu.Price[0],5);
            memcpy(&ApplVar.Amt,&ApplVar.Plu.Inventory,ApplVar.AP.Plu.InvSize);
            ApplVar.Price.Sign = 2;

            if (ApplVar.AP.Plu.RandomSize)
            {
#if (BARCUSTOMER==1)
                if (ApplVar.Plu.Random[6]==0x09 && ApplVar.Plu.Random[5]==0x99 && TESTBIT(ART_LEVEL, BIT3))
                    continue;
#endif
                memcpy(ApplVar.Qty.Value,ApplVar.Plu.Random,ApplVar.AP.Plu.RandomSize);
            }
            else
            {
                ULongToBCDValue(ApplVar.Qty.Value,ApplVar.PluNumber+1);
            }
            RJPrint(0,FormatQtyStr(ApplVar.Plu.Name,&ApplVar.Qty,PRTLEN));
            ApplVar.DeptNumber = ApplVar.Plu.Dept;
            ReadDept();

            memset(ProgLineMes,' ',sizeof(ProgLineMes));
            strcpy(ProgLineMes+4,ApplVar.Dept.Name);
            RJPrint(0,FormatStrQtyPriAmt(ProgLineMes,&ApplVar.Amt,(BCD *)0,&ApplVar.Price,PRTLEN));
            if (KbHit())
            {//any key for stop
                Getch();
                break;
            }
        }
        ResetLoop();
        ApplVar.BufKp = 0;
        SETBIT(ApplVar.MyFlags,PRNTRAI);
        PrintLine2('=');
    }
#endif
    ClearEntry(false);
    SETBIT(ApplVar.MyFlags,PRNHEADMUST);
    //用来控制返回到菜单,并且按确认时,不出现输入记录号>>>>>>

}

/**
 * 生成单品库存数据信息
 *
 * @author EutronSoftware (2017-01-10)
 */
void ReadPluStock()
{
    ReadPlu();
    //memset(ListRecord.Name,' ',sizeof(ListRecord.Name));
    strcpy(ListRecord.Name,DispQtyStr(ApplVar.Plu.Name, &ApplVar.Plu.Inventory,SCREENWD));
}
