#define EJOURNAL_C

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    #include "king.h"
    #include "exthead.h"
    #include "exth.h"
    #include "Message.h"

    #define EJ_DEBUG   0 //ccr2016-12-05testonly 用来控制发送调试信息

    #if (PC_EMUKEY==1)
        #include "FEcrTest.h"
    #endif

#if (defined(CASE_EJSD))
    #if defined   (__CC_ARM)      /* ARM Compiler */
BYTE MMC_Buff[SD_BLOCKSIZE*2] __attribute__ ((aligned (4)));
    #elif defined (__ICCARM__)    /* IAR Compiler */
        #pragma data_alignment=4
BYTE MMC_Buff[SD_BLOCKSIZE*2];
    #else
BYTE MMC_Buff[SD_BLOCKSIZE*2];
    #endif

    #if defined(DEBUGBYPC)
        #if (sizeof(ApplVar.EJHeader)>sizeof(MMC_Buff))
            #error "EJHeader is Too large!"
        #endif
    #endif
#else
#include "flashb.h"
#endif

BYTE DataFromEJ[SD_BLOCKSIZE];

/**
 * 计算一块数据的CRC
 *
 * @author EutronSoftware (2016-11-28)
 *
 * @param Area
 * @param pLen
 *
 * @return BYTE
 */
BYTE GetCRC(BYTE *Area,WORD pLen)
{
    BYTE sCRC;
    WORD i;

    sCRC = 0;
    for (i=0; i<pLen; i++)
        sCRC ^= Area[i];

    return sCRC;
}

//ccr2016-10-31>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*********************************************
 *  Get the real size of MMC
 *
 * @author EutronSoftware (2016-10-29)
 */
void GetMMCSize()
{
    short i;

#if defined(CASE_EJFLASH)
    ApplVar.EJVar.MMCSIZEMAX = BLOCKSFOREJ*0x10000;
#else
#if (!defined(DEBUGBYPC))// && !defined(CASE_MALTA))

    i=Bios(BiosCmd_SD_ReadInfo,0,MMC_SEND_CSD,16);

    if (i==0)
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI94);
        ApplVar.EJVar.MMCSIZEMAX =0;
        return;
    }
   //ccr2016-11-01受管理限制,最大使用4G SD卡容量
    if (SDCardInfo.CardCapacity>4294967200l)
        ApplVar.EJVar.MMCSIZEMAX =4294967200l;
    else
        ApplVar.EJVar.MMCSIZEMAX =SDCardInfo.CardCapacity;
#else
    ApplVar.EJVar.MMCSIZEMAX    = 0x1EA0000L;       //    32M MMC卡可操作的实际容量 //
#endif
#endif
}

/*******************************************************
 * 输入查询条件,然后查询打印EJ流水数据
 *
 * @author EutronSoftware (2016-20-32)
 */
void ExploreEJournal()
{
    WORD sInput;
    ULONG temp;

    while (true)
    {
        memset((char *)&LogDefine,0,sizeof(LogDefine));
        LogDefine.DateTo=0xffffffff;
        LogDefine.RecNumTo=0xffffffff;

        sInput = ListItems(MsgEJMESSAGES,0,It_SELEEJ,true);
        switch (sInput)
        {
        case (MsgEJMESSAGES-MsgEJMESSAGES+1)://   MsgEJMESSAGES         "EJ存储信息"
#if (SCREENLN>6)
            DisplayEJInfo1BY1();
#else
            if (TESTBIT(ApplVar.EJVar.ContFlag,EJDOCHEAD))
            {
                RefreshEJStart(true);
                StoreEJData(CEJ_DOCEND,(BYTE*)cMsg_EJXINXI);
                //StoreEJEnd();
            }
            RESETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
            PrintEJInfo(true);
            SETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
            SETBIT(ApplVar.MyFlags,PRNHEADMUST);
            PrePrintHead(PREHEADER);
#endif
            continue;
        case (MsgEJALLEJ-MsgEJMESSAGES+1)://   MsgEJALLEJ         "全部EJ记录"
            //可以指定收据号,输入收据号
            DispStrXY((char*)cMsg_RECNUMFR,0,0);
            if (GetStrFrKBD('9',SCREENWD-7,0,6,0)<0)
                continue;
            ScreenFeed(1);
            LogDefine.RecNum=0;
            if (Appl_EntryCounter)
            {
                StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
                BCDValueToULong(ApplVar.Entry.Value,&temp);
                LogDefine.RecNum=temp;
                if (LogDefine.RecNum)
                {
                    DispStrXY((char*)cMsg_RECNUMTO,0,0xff);
                    if (GetStrFrKBD('9',SCREENWD-7,0xff,6,0)<0)
                        continue;
                    ScreenFeed(1);
                    if (Appl_EntryCounter)
                    {
                        StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
                        BCDValueToULong(ApplVar.Entry.Value,&temp);
                        LogDefine.RecNumTo=temp;
                        if (LogDefine.RecNumTo<LogDefine.RecNum)
                            LogDefine.RecNumTo=LogDefine.RecNum;
                    } else
                        LogDefine.RecNumTo=LogDefine.RecNum;
                }
            }
            break;
        case (MsgEJDATERANGE-MsgEJMESSAGES+1)://   MsgEJDATERANGE     "日期时间范围"
            break;
        case (MsgEJRECEIPT-MsgEJMESSAGES+1)://MsgEJRECEIPT        "销售收据记录"
            LogDefine.LogType='R';
            break;
        case (MsgEJXREPORT-MsgEJMESSAGES+1)://   MsgEJXREPORT     "X报表记录"
            LogDefine.LogType='X';
            break;
        case (MsgEJZREPORT-MsgEJMESSAGES+1)://   MsgEJZREPORT       "Z报表记录"
            LogDefine.LogType='Z';
            break;
        case (MsgEJCLERK-MsgEJMESSAGES+1)://MsgEJCLERK        "收款员记录"
            //可以指定会员号
            DispStrXY(Msg[SHKYUANS].str,0,0);
            if (GetStrFrKBD('9',SCREENWD-3,0,3,0)<0)
                continue;
            ScreenFeed(1);
            if (Appl_EntryCounter)
            {//ccr2016-06-02会员号只取9位,因此最大为999999999
                StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
                BCDValueToULong(ApplVar.Entry.Value,&temp);
                LogDefine.FuncCode=temp;
            }
            break;
        case 0:
        case It_EXIT:
        default:
            return;
        }
        if (sInput!=(MsgEJALLEJ-MsgEJMESSAGES+1) || (LogDefine.RecNum==0))
        {//指定日期范围,输入开始日期
            DispStrXY(Msg[KSHRQIF].str,0,0xff);
#if SCREENWD<18
            ScreenFeed(1);
            DispStrXY("YYMMDDHH",0,0xff);
#else
            DispStrXY("YYMMDDHH",SCREENWD-8,0xff);
#endif
            if (GetStrFrKBD('9',SCREENWD-8,0xff,8,0)<0)
                continue;
            if (Appl_EntryCounter)
            {
                StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
                if (Appl_EntryCounter==6)
                    BcdMul100(&ApplVar.Entry);
                LogDefine.DateFr = (((UnLong)ApplVar.Entry.Value[3])<<24) + (((UnLong)ApplVar.Entry.Value[2])<<16) + ((UnLong)ApplVar.Entry.Value[1]<<8) + ApplVar.Entry.Value[0];
            }
            ScreenFeed(1);
            //指定日期范围,输入结束日期
            DispStrXY(Msg[JSHRQIF].str,0,0xff);
#if SCREENWD<18
            ScreenFeed(1);
            DispStrXY("YYMMDDHH",0,0xff);
#else
            DispStrXY("YYMMDDHH",SCREENWD-8,0xff);
#endif
            if (GetStrFrKBD('9',SCREENWD-8,0xff,8,0)<0)
                continue;
            if (Appl_EntryCounter)
            {
                StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
                if (Appl_EntryCounter==6)
                {
                    BcdMul100(&ApplVar.Entry);
                    ApplVar.Entry.Value[0]=0x24;
                }
                LogDefine.DateTo = (((UnLong)ApplVar.Entry.Value[3])<<24) + (((UnLong)ApplVar.Entry.Value[2])<<16) + ((UnLong)ApplVar.Entry.Value[1]<<8) + ApplVar.Entry.Value[0];
            } else if (LogDefine.DateFr)
            {//查询指定天里的数据
                LogDefine.DateTo = (LogDefine.DateFr | 0x24);
            }

        }
        if (LogDefine.DateTo!=0 && LogDefine.DateTo>=LogDefine.DateFr)
            PrintEJLog(false);
        else
            ApplVar.ErrorNumber=ERROR_ID(CWXXI02);

    }
    return ;
}
//ccr2016-10-31<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#if (defined(CASE_EJSD))
BYTE MMC_CheckPresence()
{
    if (!Bios(BiosCmd_SD_CheckPresence, 0,0,0))
    {
        Save_Config(true);
        return false;
    } else
        return true;
}

/**
 * 测试FM和EJ是否存在,不存在时,机器挂起
 *
 * @author EutronSoftware (2014-07-29)
 *
 * @return NONE
 */
void EJ_Exist(void)
{
    if (MMC_CheckPresence())//ccr2014-07-29
    {
        RESETBIT(ApplVar.EJVar.ContFlag,EJNOTEXIST);
    } else
    {
        if (!TESTBIT(ApplVar.EJVar.ContFlag,EJNOTEXIST))
        {//当在EJ存在的情况下,检测到无EJ
            ApplVar.ErrorNumber=ERROR_ID(CWXXI93);
            SETBIT(ApplVar.EJVar.ContFlag,EJNOTEXIST);
            //while (!MMC_CheckPresence())//block the ECR
            //    CheckError(0);
            CheckEJ();
        }
    }
}

// 利用MMC的读数据缓存区,读取所要的数据 //
    #define MMC_ReadBuff MMC_Buff
//bytes must less than 512
BYTE ReadFromMMC(char *to,ULONG pFrom,WORD bytes)
{

    UnLong sOffset,sBlock,BaseRam;
    BYTE s2Sect=0;//所要读取的数据在两个区

    sBlock = pFrom / SD_BLOCKSIZE;
    sOffset = pFrom % SD_BLOCKSIZE;

    BaseRam = (sBlock+1) * SD_BLOCKSIZE;
    if ((pFrom+bytes) > BaseRam)
    {
        if ((pFrom+bytes) > EJDOCADDRMAX)
            return 0;

        if (!MMC_ReadBlock(MMC_ReadBuff+SD_BLOCKSIZE,BaseRam))
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI94);//ccr2014-07-29
            return false;
        }
        s2Sect=1;
    }
/*ccr2014-08-06>>>>
    else if ((BaseRam-SD_BLOCKSIZE)==ApplVar.EJVar.MMCLast)
    {//利用CACH功能从MMC_Buff读取数据
        memcpy(to,MMC_Buff,bytes);
        RamOffSet += bytes;
        return true;
    }
<<<<<<<<<<<<<<<<< */

    BaseRam -= SD_BLOCKSIZE;

    if (MMC_ReadBlock(MMC_ReadBuff,BaseRam))
        memcpy(to,&MMC_ReadBuff[sOffset],bytes);
    else
    {
        ApplVar.EJVar.MMCLast = -1;
        ApplVar.ErrorNumber=ERROR_ID(CWXXI94);//ccr2014-07-29
        return false;
    }

    ApplVar.EJVar.MMCLast = BaseRam;

    RamOffSet += bytes;
    return true;
}

/**************************************************************
 * 将bytes个字节的数据写入MMC卡中的指定位置,bytes<=512
 * 向MMC卡写数据的规则是按块进行,WriteStream将自动处理块边界问题
 *
 * @author EutronSoftware (2014-08-19)
 *
 * @param from :需要写入的数据
 * @param bytes :写入的数据字节数,<=512
 * @param erase :是否擦除新块,=true时,将擦除写入区的数据
 *        RamOffSet:指定数据写入地址
 * @return BYTE
 */
BYTE WriteStream(char *from,WORD bytes,BYTE erase)
{
    UnLong sBlock,sOffset;
    BYTE s2Sect=0;//标示写在两个区

    sBlock = RamOffSet / SD_BLOCKSIZE;
    sOffset = RamOffSet % SD_BLOCKSIZE;

    if ((RamOffSet+bytes) >= ApplVar.EJVar.MMCSIZEMAX)
    {
        ApplVar.EJVar.MMCLast = -1;
        return 0;//ApplVar.EJVar.MMCLast = 0;
    }

    ApplVar.EJVar.MMCLast = (sBlock+1) * SD_BLOCKSIZE;
    if ((RamOffSet+bytes) > ApplVar.EJVar.MMCLast)
    {
        s2Sect = 1;
        if (erase)
        {//直接擦除第二块
            memset(MMC_Buff+SD_BLOCKSIZE,0xff,SD_BLOCKSIZE);
        } else if (!MMC_ReadBlock(MMC_Buff+SD_BLOCKSIZE,ApplVar.EJVar.MMCLast))//先读取写入区的数据
        {
            ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
            s2Sect = 0;
        }
    }

    ApplVar.EJVar.MMCLast -= SD_BLOCKSIZE;
    if (erase && (RamOffSet & (SD_BLOCKSIZE-1))==0)
    {//在块的首地址写入数据,直接擦除要写入的块
        memset(MMC_Buff,0xff,SD_BLOCKSIZE);
    } else if (!MMC_ReadBlock(MMC_Buff,ApplVar.EJVar.MMCLast))
    {//先读入写入区的数据
        ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
        ApplVar.EJVar.MMCLast=-1;
    }
    if (ApplVar.ErrorNumber != ERROR_ID(CWXXI94))
    {
        memcpy(MMC_Buff+sOffset,from,bytes);//将要写入的数据复制到缓存
        //将数据写入SD卡
        if (MMC_WriteBlock(MMC_Buff,ApplVar.EJVar.MMCLast))
        {
            if (s2Sect)
            {
                if (!MMC_WriteBlock(MMC_Buff+SD_BLOCKSIZE,ApplVar.EJVar.MMCLast+SD_BLOCKSIZE))
                    ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
            }
        } else
            ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
    }

    RamOffSet += bytes;
    return(ApplVar.ErrorNumber != ERROR_ID(CWXXI94));
}
//==========================================================

/**
 * 将EJ读写管理指针EJHeader写入MMC中的EJHEADERADDR位置
 * 注意:此数据占用一个存储块,快大小为SDBLOCKSIZE字节
 *
 * @author EutronSoftware (2016-11-14)
 */
void WriteEJHead()
{

    memset(MMC_Buff,0xff,sizeof(MMC_Buff));
    memcpy(&MMC_Buff,(BYTE *)&ApplVar.EJHeader,HEADSIZE);

    if (!MMC_WriteBlock(MMC_Buff,EJHEADERADDR))
        ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
    else
        ApplVar.EJVar.MMCLast = EJHEADERADDR;
    ApplVar.EJVar.MMCLast = 0xffffffff;

}

/**
 * 将EJ的查找索引pDate写入EJ的索引区 INDEXADDR //
 *
 * @author EutronSoftware (2016-11-01)
 *
 * @param pDate :压缩型日期
 * @param inc :true=增加记录;false=刷新记录
 *
 * @return ULONG :索引记录数目
 * 数据的写入位置由
 *
 */
ULONG WriteEJIndex(WORD pDate,BYTE inc)
{
    TEJINDEX IndexBuff;

    if (inc)
        IndexBuff.EJAddress = ApplVar.EJHeader.AddrNextEJ;
    else
        IndexBuff.EJAddress = ApplVar.EJHeader.AddrLastEJ;
    RamOffSet = ApplVar.EJHeader.IndexCount * INDEXRECORDSIZE + INDEXADDR;

    if ((RamOffSet+INDEXRECORDSIZE) >= IDXENDADDRESS)
    {
        //ccr2016-11-30ApplVar.ErrorNumber = ERROR_ID(CWXXI97);
        return 0;
    }
    IndexBuff.EJDate = pDate;
    IndexBuff.EJNumber = ApplVar.EJHeader.EJDocNumber;//ccr2016-10-31
    if (WriteStream((char*)&IndexBuff,INDEXRECORDSIZE,true))
        ApplVar.EJHeader.IndexCount++;

    return ApplVar.EJHeader.IndexCount;
}
#else//>>>>For CASE_EJFLASH
/**************************************************************
 * 将bytes个字节的数据写入MMC卡中的指定位置,bytes<=512
 * 向MMC卡写数据的规则是按块进行,WriteStream将自动处理块边界问题
 *
 * @author EutronSoftware (2014-08-19)
 *
 * @param from :需要写入的数据
 * @param bytes :写入的数据字节数,<=512
 * @param erase :是否擦除新块,=true时,将擦除写入区的数据
 *        RamOffSet:指定数据写入地址
 * @return BYTE
 */
BYTE WriteStream(char *from,WORD bytes,BYTE erase)
{
    int  i=bFlashProgram(RamOffSet,bytes,from);
    if (i!=FLASH_SUCCESS)
    {
#if defined(FOR_DEBUG)
        xprintf("Err%06LX,%u-(%d)\n",RamOffSet,bytes,i);//testonly
#endif
        ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
    }

    RamOffSet += bytes;
    return(ApplVar.ErrorNumber != ERROR_ID(CWXXI94));
}
//==========================================================
BYTE ReadFromMMC(char *to,ULONG pFrom,WORD bytes)
{
    bFlashReadBuffer(to,pFrom,bytes);
    return true;
}

/**
 * 将EJ读写管理指针EJHeader写入MMC中的EJHEADERADDR位置
 * 注意:此数据占用一个存储块,快大小为SDBLOCKSIZE字节
 *
 * @author EutronSoftware (2016-11-14)
 */
void WriteEJHead()
{
	int i=bFlashProgram(EJHEADERADDR,HEADSIZE,(BYTE *)&ApplVar.EJHeader);
    if (i!=FLASH_SUCCESS)
        ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
}

/**
 * 将EJ的查找索引pDate写入EJ的索引区 INDEXADDR //
 *
 * @author EutronSoftware (2016-11-01)
 *
 * @param pDate :压缩型日期
 * @param inc :true=增加记录;false=刷新记录
 *
 * @return ULONG :索引记录数目
 * 数据的写入位置由
 *
 */
ULONG WriteEJIndex(WORD pDate,BYTE inc)
{
    TEJINDEX IndexBuff;

    RamOffSet = ApplVar.EJHeader.IndexCount * INDEXRECORDSIZE + INDEXADDR;

    if ((RamOffSet+INDEXRECORDSIZE) >= IDXENDADDRESS)
    {
        //ccr2016-11-30 ApplVar.ErrorNumber = ERROR_ID(CWXXI97);
        return 0;
    }

    if (inc)
        IndexBuff.EJAddress = ApplVar.EJHeader.AddrNextEJ;
    else
        IndexBuff.EJAddress = ApplVar.EJHeader.AddrLastEJ;

    IndexBuff.EJDate = pDate;
    IndexBuff.EJNumber = ApplVar.EJHeader.EJDocNumber;//ccr2016-10-31
#if   EJ_DEBUG
    SendString("EI\n",3);//ccr2016-12-05testonly
#endif
    if (WriteStream((char*)&IndexBuff,INDEXRECORDSIZE,true))
        ApplVar.EJHeader.IndexCount++;

    return ApplVar.EJHeader.IndexCount;
}
#endif


/**
 * 将一行数据写入EJ,写入地址为EJHeader.NextNP
 *
 * @author EutronSoftware (2016-11-01)
 *
 * @param pFrom :需要写入的数据
 * @param pSize :数据长度
 *
 * @return BYTE :true=写入成功;false=写入失败
 * 写入完成后,EJHeader.NextNP将指向下一个写入位置
 *
 */
BYTE WriteEJLog(char *pFrom,WORD pSize)
{
    ULONG sAddr;

    sAddr=(PRTLEN>pSize)?(PRTLEN):(pSize);
    //check whether EJ is full
    if (ApplVar.EJHeader.AddrNextEJ+sAddr >= ApplVar.EJHeader.AddrEJMax)
    {
        SETBIT(ApplVar.EJVar.ContFlag,EJISFULL);//ccr2016-11-29
        ApplVar.ErrorNumber=ERROR_ID(CWXXI97);
        return false;
    }

    RamOffSet = ApplVar.EJHeader.AddrNextEJ;
#if   EJ_DEBUG
    SendString("EL\n",3);//ccr2016-12-05testonly
#endif

//ccr2016-12-05 避免在写错的位置再写入数据    if (WriteStream(pFrom,pSize,true))
    WriteStream(pFrom,pSize,true);
    {
        ApplVar.EJHeader.AddrNextEJ = ApplVar.EJHeader.AddrNextEJ + pSize;

        if (ApplVar.EJHeader.AddrNextEJ >= ApplVar.EJHeader.AddrEJMax-MMC_FULL)
        {
            SETBIT(ApplVar.EJVar.ContFlag,EJISFULL);//ccr2016-11-29
        }
        else if ((ApplVar.EJHeader.AddrNextEJ >= ApplVar.EJHeader.AddrEJMax-MMC_LESS) && ApplVar.EJHeader.AddrNextEJ < (ApplVar.EJHeader.AddrEJMax-MMC_LESS+PRTLEN*3)) //ccr2016-11-29
        {
            SETBIT(ApplVar.EJVar.ContFlag,EJWILLFULL);//ccr2016-11-29
            //ccr2016-11-29 ApplVar.ErrorNumber=ERROR_ID(CWXXI96);              // 40 KB left, only warn once
        }
        return true;
    }
//    else
//    {
//        return false;
//    }
}


//==========================================================
/**
 * 根据压缩型日期检索索引，获得EJ地址
 *
 * @author EutronSoftware (2016-11-10)
 *
 * @param pDate:压缩型日期
 *
 * @return ULONG
 */
ULONG ReadEJIndexByDate(WORD pDate)
{
    ULONG sRamOffsetFr,sRamOffsetTo,sRamOffset,sRecordOffset,sRamOffsetLast;
    TEJINDEX IndexBuff;

    if (ApplVar.EJHeader.IndexCount==0)
        return 0xffffffff;

    if (!ReadFromMMC((char*)&IndexBuff,INDEXADDR,INDEXRECORDSIZE))
        ApplVar.ErrorNumber = ERROR_ID(CWXXI94);

    if (IndexBuff.EJDate >= pDate)
        return IndexBuff.EJAddress;

    sRamOffsetLast = IndexBuff.EJAddress;
    sRamOffsetFr = 1;
    sRamOffsetTo = ApplVar.EJHeader.IndexCount;

    while (sRamOffsetFr <= sRamOffsetTo)
    {

        sRecordOffset = (sRamOffsetTo + sRamOffsetFr) / 2;
        sRamOffset = (sRecordOffset-1) * INDEXRECORDSIZE + INDEXADDR;

        if (!ReadFromMMC((char*)&IndexBuff,sRamOffset,INDEXRECORDSIZE))
        {
            ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
            break;
        }
        if (IndexBuff.EJDate == pDate)
        {
            return(IndexBuff.EJAddress);
        } else if (IndexBuff.EJDate > pDate)
            sRamOffsetTo = sRecordOffset - 1;
        else
        {
            sRamOffsetLast = IndexBuff.EJAddress;
            sRamOffsetFr = sRecordOffset + 1;
        }
    }
//			if(sRamOffsetFr > sRamOffsetTo)
    return sRamOffsetLast;//没有记录,取最小地址

}

/**
 * 根据压缩型日期检索索引，获得EJ地址
 *
 * @author EutronSoftware (2016-11-10)
 *
 * @param pNumber:EJ文档序号
 *
 * @return ULONG
 */
ULONG ReadEJIndexByNumber(ULONG pNumber)
{
    ULONG sRamOffsetFr,sRamOffsetTo,sRamOffset,sRecordOffset,sRamOffsetLast;
    TEJINDEX IndexBuff;

    if (ApplVar.EJHeader.IndexCount==0)
        return 0xffffffff;

    if (!ReadFromMMC((char*)&IndexBuff,INDEXADDR,INDEXRECORDSIZE))
        ApplVar.ErrorNumber = ERROR_ID(CWXXI94);

    if (IndexBuff.EJNumber >= pNumber)
        return IndexBuff.EJAddress;

    sRamOffsetLast = IndexBuff.EJAddress;

    sRamOffsetFr = 1;
    sRamOffsetTo = ApplVar.EJHeader.IndexCount;

    while (sRamOffsetFr <= sRamOffsetTo)
    {

        sRecordOffset = (sRamOffsetTo + sRamOffsetFr) / 2;
        sRamOffset = (sRecordOffset-1) * INDEXRECORDSIZE + INDEXADDR;

        if (!ReadFromMMC((char*)&IndexBuff,sRamOffset,INDEXRECORDSIZE))
        {
            ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
            break;
        }
        if (IndexBuff.EJNumber == pNumber)
        {
            return(IndexBuff.EJAddress);
            //break;
        } else if (IndexBuff.EJNumber > pNumber)
        {
            sRamOffsetTo = sRecordOffset - 1;
        } else
        {
            sRamOffsetLast = IndexBuff.EJAddress;
            sRamOffsetFr = sRecordOffset + 1;
        }
    }
    return sRamOffsetLast;//没有记录,取最小地址
}


//=====================================================
// 保存EJ文档开始标示,即:保存LogHeader数据  //
void StoreEJStart()
{
    WORD    sDate;
    ULONG   sTmp;

    if (TESTBIT(ApplVar.EJVar.ContFlag, (EJSTOREMUST+EJHEADMUST))==(EJSTOREMUST+EJHEADMUST))
    {
        if (TESTBIT(ApplVar.EJVar.ContFlag, EJENDMUST))
            StoreEJEnd();
        ApplVar.EJHeader.EJDocNumber++;
        memset((char*)&ApplVar.EJDocHeader,0xff, EJDOCHEADSIZE);

        GetTimeDate(&Now);

        ApplVar.EJDocHeader.EJStart= CEJ_STARTDOC;          //文档开始标志=CEJ_STARTDOC,也是上一个文档结束标志
        ApplVar.EJDocHeader.EJDocNumber=ApplVar.EJHeader.EJDocNumber;      //文档序号
        ApplVar.EJDocHeader.EJLocation=LOCATION;         //ccr2016-10-29门店号
        ApplVar.EJDocHeader.EJRegister=REGISTER;         //ccr2016-10-29收款机号
#if defined(CASE_EJSD)
        ApplVar.EJDocHeader.EJDocType= ECRMode();        //文档类型('R/Z/X/M/S/O')
        ApplVar.EJDocHeader.EJDateTime.year  = (Now.year & 0xff);       // YY  //文档日期时间:yymmddhhmmss
        ApplVar.EJDocHeader.EJDateTime.month = Now.month;               // MM
        ApplVar.EJDocHeader.EJDateTime.day   = Now.day;             // DD
        ApplVar.EJDocHeader.EJDateTime.hour  = Now.hour;                // HH
        ApplVar.EJDocHeader.EJDateTime.minute = Now.min;                // MM
        ApplVar.EJDocHeader.EJDateTime.second = Now.sec;                // SS
        ApplVar.EJDocHeader.EJClerk=ApplVar.ClerkNumber;           //ccr2016-10-28收款员序号
#endif

        sDate = EncordDate(Now.year & 0xff,Now.month,Now.day);
        if (sDate > ApplVar.EJHeader.DateLastEJ)
        {
            ApplVar.EJHeader.DateLastEJ = sDate;
            WriteEJIndex(ApplVar.EJHeader.DateLastEJ,true);//ccr2008
        }
        sTmp = ApplVar.EJHeader.AddrNextEJ;
//ccr2016-12-05 避免在写错的位置再写入数据       if (WriteEJLog((char *)&ApplVar.EJDocHeader, EJDOCHEADSIZE))
        WriteEJLog((char *)&ApplVar.EJDocHeader, EJDOCHEADSIZE);
        {
#if (EJ_DEBUG)
            SendString("EH\n",3);//ccr2016-12-05testonly
#endif
#if (PC_EMUKEY==1)// && PCREMOTE==0)

            //发送收据号和收据的存储结束地址
            if (COMPUTER)
            {
                SendComm(COMPUTER_1,'E');SendALong(sTmp);SendComm(COMPUTER_1,'#');SendAWord(ApplVar.EJHeader.EJDocNumber);SendComm(COMPUTER_1,'\n');
            }
#endif

            ApplVar.EJHeader.AddrLast2EJ = ApplVar.EJHeader.AddrLastEJ;
            ApplVar.EJHeader.AddrLastEJ = sTmp;//最后一个文档的位置
#if defined(CASE_EJSD)
            if (ApplVar.ErrorNumber  != ERROR_ID(CWXXI97))  //liuj 0611
                WriteEJHead();
#endif
            ApplVar.EJVar.EJcrc = GetCRC((BYTE *)&ApplVar.EJDocHeader, EJDOCHEADSIZE);
            RESETBIT(ApplVar.EJVar.ContFlag,(EJHEADMUST+EJVIEWPRINT));
            SETBIT(ApplVar.EJVar.ContFlag,(EJDOCHEAD+EJENDMUST));
        }
    }
}
/**
 * 打印EJ编号
 *
 * @author EutronSoftware (2017-01-13)
 */
void PrintEJNumber()
{
    char    strEJNum[PRTLEN+1];

    memset(strEJNum,' ',PRTLEN);
    sprintf(strEJNum,"#E%08lu",ApplVar.EJHeader.EJDocNumber);
    strEJNum[strlen(strEJNum)]=' ';
    BCDDateTimeToStr(strEJNum+PRTLEN-20,//2016-01-01 01:01:01
                     ApplVar.EJDocHeader.EJDateTime.year,   /* current year BCD*/
                     ApplVar.EJDocHeader.EJDateTime.month,  /* month (1 = Jan) BCD*/
                     ApplVar.EJDocHeader.EJDateTime.day,    /* day of month BCD*/
                     ApplVar.EJDocHeader.EJDateTime.hour,   /* hours BCD*/
                     ApplVar.EJDocHeader.EJDateTime.minute, /* minutes BCD*/
                     ApplVar.EJDocHeader.EJDateTime.second);    /* seconds BCD*/
    strEJNum[PRTLEN]=0;
    OutPrintEx(CMDP_R,strEJNum,0);//避免重入  RJPrint(0, strEJNum);
}

/**
 * 根据日期变化,刷新EJ文档开始标示,同时打印EJ信息,即:EJDocHeader数据
 *
 * @author EutronSoftware (2016-11-11)
 *
 * @param refresh :=true,刷新数据后打印EJ#;
 *                 =false,只打印EJ#不刷新数据
 * 将使用SysBuf作为打印缓存
 */
void RefreshEJStart(BYTE refresh)
{
    WORD    sDate;

    if (TESTBIT(ApplVar.EJVar.ContFlag,EJDOCHEAD))//表示写入了EJ头
    {
        RESETBIT(ApplVar.EJVar.ContFlag,EJDOCHEAD);
        if (refresh)
        {
#if (EJ_DEBUG)
            SendString("RH\n",3);//testonly
#endif
            GetTimeDate(&Now);
            ApplVar.EJVar.EJcrc ^= GetCRC((BYTE *)&ApplVar.EJDocHeader, EJDOCHEADSIZE);
#if defined(CASE_EJSD)
            ApplVar.EJDocHeader.EJStart= CEJ_STARTDOC;          //文档开始标志=CEJ_STARTDOC,也是上一个文档结束标志
            ApplVar.EJDocHeader.EJLocation=LOCATION;         //ccr2016-10-29门店号
            ApplVar.EJDocHeader.EJRegister=REGISTER;         //ccr2016-10-29收款机号
            //ApplVar.EJDocHeader.EJDocNumber=ApplVar.EJHeader.EJDocNumber;      //文档序号
#endif
            ApplVar.EJDocHeader.EJDocType= ECRMode();        //文档类型('R/Z/X/M/S/O')
            ApplVar.EJDocHeader.EJDateTime.year  = (Now.year & 0xff);       // YY  //文档日期时间:yymmddhhmmss
            ApplVar.EJDocHeader.EJDateTime.month = Now.month;               // MM
            ApplVar.EJDocHeader.EJDateTime.day   = Now.day;             // DD
            ApplVar.EJDocHeader.EJDateTime.hour  = Now.hour;                // HH
            ApplVar.EJDocHeader.EJDateTime.minute = Now.min;                // MM
            ApplVar.EJDocHeader.EJDateTime.second = Now.sec;                // SS
            ApplVar.EJDocHeader.EJClerk=ApplVar.ClerkNumber;           //ccr2016-10-28收款员序号


            sDate = EncordDate(Now.year & 0xff,Now.month,Now.day);
            if (sDate > ApplVar.EJHeader.DateLastEJ)
            {
                ApplVar.EJHeader.DateLastEJ = sDate;
                WriteEJIndex(ApplVar.EJHeader.DateLastEJ,false);//可能出现不同日期的索引地址相同的情况
            }

            ApplVar.EJVar.EJcrc ^= GetCRC((BYTE *)&ApplVar.EJDocHeader, EJDOCHEADSIZE);
            //RamOffSet = ApplVar.EJHeader.AddrLastEJ;
            //WriteStream((char *)&ApplVar.EJDocHeader, EJDOCHEADSIZE,false);
            RamOffSet = ApplVar.EJHeader.AddrLastEJ+(sizeof(ULONG)+3);
            WriteStream((char *)&ApplVar.EJDocHeader.EJDocType, EJDOCHEADSIZE-(sizeof(ULONG)+3),false);
        }
        PrintEJNumber();
    }
}
#define ZIPLISTMAX   5 ///最多5个
CONST BYTE ZIPList[ZIPLISTMAX] ={' ','*','-','.','='};//最多5个

//把str转储到EJData+1中,对其中的空格进行压缩,行尾的空格被自动清除
//返回压缩后的长度

int CompressSpace(char *EJData,char *str)
{
    int sCount,i,j;
    char sCh,sCh1=0;
    BYTE sCC,sCCC;

    //compress space
    sCount = 0;
    sCC = sCCC = 5;

    for (i = 0, j = 1; i < PRTLEN; i++)
    {
        sCh = str[i];
        if (!sCh)
            break;//数据结束
        else
        {
            switch (sCh)
            {
            case ' ':sCC = 0;break;
            case '*':sCC = 1;break;
            case '-':sCC = 2;break;
            case '.':sCC = 3;break;
            case '=':sCC = 4;break;
            default:sCC = 5;break;//遇到非可压缩字符,
            }
            if (sCC!=sCCC)
            {
                if (sCh1)//检查之前字符是否压缩
                {
                    //如果此字符前有要压缩的空格,先保存压缩数据
                    if (sCount > 2)
                    {//只有当空格数大于2时,才进行压缩
                        EJData[j++] = CEJ_ESC+sCCC;
                        EJData[j++] = sCount;
                    } else
                    {//否则不进行压缩
                        switch (sCount)
                        {
                        case 2:
                            EJData[j]=sCh1;j++;
                        case 1:
                            EJData[j]=sCh1;j++;
                        }
                    }
                    sCount = 0;
                    sCh1 = 0;
                }
            }
            sCCC = sCC;
            if (sCC<5)
            {
                sCh1 = sCh;
                sCount++;
            } else
            {
                sCount = 0;
                sCh1 = 0;
                EJData[j] = sCh;//保存非空格字符
                j++;
            }
        }
    }
    if (sCount && sCh1!=' ')//如果行尾是空格,则不保存
    {
        //如果此字符前有要压缩的空格,先保存压缩数据
        if (sCount > 2)
        {//只有当空格数大于2时,才进行压缩
            EJData[j++] = CEJ_ESC+sCCC;
            EJData[j++] = sCount;
        } else
        {//否则不进行压缩
            switch (sCount)
            {
            case 2:
                EJData[j]=sCh1;j++;
            case 1:
                EJData[j]=sCh1;j++;
            }
        }
    }

    return j;
}


// 将要打印的一串字符写入EJ中,对于串中的空格字符进行压缩保存 //
// Store a string to MMC.
// cmd:Type of str,=CEJ_DOCEND时,在写入字符串后,同时写入结束标志
void StoreEJData(BYTE cmd, BYTE *str)
{
    BYTE ejLEN,//计算需要存储的数据长度
    rollEJcrc;
    BYTE EJData[EJBUFFSIZE];


    if (!TESTBIT(ApplVar.EJVar.ContFlag, EJSTOREMUST))
    {// 在打印了EJ头之后，又打印了其它内容，但是没有存EJ //
        ApplVar.EJRoll.Flag = ApplVar.EJRoll.FlagS = 0;
        //       if (TESTBIT(ApplVar.EJVar.ContFlag, EJDOCHEAD))
        //           SETBIT(ApplVar.EJVar.ContFlag,PRINTONLY);
        return;
    }

    if (ApplVar.EJRoll.Flag == 0x69 && ApplVar.EJRoll.FlagS == 0x5a
        && ApplVar.EJRoll.Address == ApplVar.EJHeader.AddrNextEJ)
    {//将上次写入失败的数据再次写入
        if (WriteEJLog(ApplVar.EJRoll.EJData, ApplVar.EJRoll.LogLen))
            ApplVar.EJVar.EJcrc = ApplVar.EJRoll.EJcrc;
    }

    rollEJcrc = ApplVar.EJVar.EJcrc;
    if (str != NULL)
    {
        memset(EJData, 0, sizeof(EJData));
        if (cmd == CEJ_DOCEND)
            EJData[0] = CEJ_STARTLN;//type of thie line
        else
            EJData[0] = cmd;//type of thie line

        ejLEN = CompressSpace(EJData,str);

        rollEJcrc  ^= (GetCRC(EJData,(WORD) ejLEN));
    } else if (cmd == CEJ_DOCEND)
        ejLEN = 0;
    else
        return;
    if (cmd == CEJ_DOCEND && TESTBIT(ApplVar.EJVar.ContFlag,EJENDMUST))
    {//同时写入文档结束标示(含校验码)
#if (EJ_DEBUG)
        SendString("EE\n",3);//testonly
#endif
        EJData[ejLEN++] = CEJ_DOCEND;//EJ文档结束标志
        EJData[ejLEN++] = rollEJcrc ;   //保存校验数据;
        ApplVar.EJVar.EJcrc = rollEJcrc = 0;
    }

    if (ApplVar.CentralLock == RG || ApplVar.CentralLock == MG)
    {//将EJ数据存入回滚区,只回滚销售模式下的EJ日志
        ApplVar.EJRoll.Flag = 0x69;
        ApplVar.EJRoll.FlagS = 0x5a;
        ApplVar.EJRoll.EJcrc = rollEJcrc;
        memcpy(ApplVar.EJRoll.EJData,EJData, ejLEN);
        ApplVar.EJRoll.EJContent = ApplVar.EJVar.EJContent;
        ApplVar.EJRoll.LogLen = ejLEN;
        ApplVar.EJRoll.Address = ApplVar.EJHeader.AddrNextEJ;
    }

//ccr2016-12-05 避免在写错的位置再写入数据if (WriteEJLog((char *)&EJData, ejLEN))
    WriteEJLog((char *)&EJData, ejLEN);
    {
        SETBIT(ApplVar.EJVar.ContFlag,(EJENDMUST));
        ApplVar.EJRoll.Flag = ApplVar.EJRoll.FlagS = 0;

        ApplVar.EJVar.EJcrc = rollEJcrc;
        //if(ApplVar.EJContent == CEJ_DOCEND)//ccr070608!!!!!!!!!!!!!!!!!!!!!
#if defined(CASE_EJSD)
        if (ApplVar.ErrorNumber  != ERROR_ID(CWXXI97))
            WriteEJHead();
#endif
    }
    if (cmd == CEJ_DOCEND)
        RESETBIT(ApplVar.EJVar.ContFlag,(EJDOCHEAD+EJENDMUST));
    SETBIT(ApplVar.EJVar.ContFlag,(EJHEADMUST));
}



/**
 *
 * 根据指定的日期范围和收据号记录号来查询打印EJ中的数据 //
 * 当没有指定收据号时,打印日期范围内的全部收据      //
 *
 * @author EutronSoftware (2016-11-11)
 *
 * @param prnLast:=true 打印EJ中的最后一个文档,
 *                =false,根据指定的日期查找符合条件的文档
 * 从EJ中解析出来的数据将存入SysBuf中
 */
void PrintEJLog(BYTE prnLast)
{
    short  i, j;
    UnLong sSaveP,sRecno;
    UnLong sDateLog;
    BYTE sCh;   //cc 2006-07-07 for MMC
    WORD sDate;     //cc 2006-07-07 for MMC
    BYTE sActLog,           //=0,Document is not for printe,(文档不符合查找条件)
    //=1,Document must be printed,(文档符合查找条件)
    //=2, At the end of document
    sEJcrc,             //EJ校验
    sLogStart,          //=1,文档开始;=0,文件结束
    sPrintIt,           // 解析出来的数据是否打印
    sFlag,              //处理标志
    sEJFound=false,           //是否找到符合条件的EJ
    sDbH;//=0,为正常字符;=1,为倍高打印

    ApplVar.ErrorNumber = 0;
    if (!prnLast)
    {//根据条件查询数据
        if (LogDefine.RecNum)
        {
            sSaveP = ReadEJIndexByNumber(LogDefine.RecNum);//     通过索引来定为记录的存储位置 //
        } else if (LogDefine.DateFr)
        {
            sDate = EncordDate((WORD)(LogDefine.DateFr >> 24),(BYTE)(LogDefine.DateFr >> 16),(BYTE)(LogDefine.DateFr>>8)); //    day

            if (sDate > ApplVar.EJHeader.DateLastEJ)
            {
                return;
            }

            sSaveP = ReadEJIndexByDate(sDate);//     通过索引来定为记录的存储位置 //
        } else
            sSaveP = ApplVar.EJHeader.AddrEJFirst;

        if (sSaveP == 0xffffffff || ApplVar.ErrorNumber)
        {
            return;
        }
        DispSt1C(Msg[WAITING].str);
    } else
    {
        sSaveP = ApplVar.EJHeader.AddrLast2EJ;
    }

    sActLog = sLogStart = sDbH = 0;
    j= 0;

    //while (sSaveP<ApplVar.EJHeader.AddrNextEJ)/*    && !KbHit()    */
    while (sSaveP<ApplVar.EJHeader.AddrLastEJ)//最后一个文档读取完毕
    {
#if (!defined(DEBUGBYPC))//ccr091104>>>>>>>>
        if (KbHit())
        {//打印过程中，可以随时按清除键中断
            if (Getch() == ApplVar.AP.FirmKeys[ID_CLEAR])
                break;//while (sSaveP<ApplVar.EJHeader.AddrNextEJ)
        }
#endif//<<<<<<<<<<<<<<<<<<<<<

        //read log header
        if (!ReadFromMMC(DataFromEJ,sSaveP,sizeof(DataFromEJ)))//ccr2016-11-14EJDOCHEADSIZE))
            break;
        i=0;
        if (!sLogStart)
        {
            if (DataFromEJ[0] == CEJ_STARTDOC)
            {// 为一个日志文档开始标示  YYMMDDHH
                sEJcrc =  GetCRC(DataFromEJ, EJDOCHEADSIZE);
                sLogStart = 1;
                sPrintIt = true;

                if (!prnLast)
                {
                    sDateLog = CLONG(DataFromEJ[10]);//(((TEJDOCHEAD*)DataFromEJ)->EJDateTime); //hour.day.month.year

                    sRecno = ((TEJDOCHEAD*)DataFromEJ)->EJDocNumber;

                    if (sDateLog > LogDefine.DateTo ||  sRecno>LogDefine.RecNumTo)//ccr091203
                        break;
                }

                if (prnLast  //prnLast==true时,打印最后一个记录
                    || (sDateLog!=0xffffffff && sDateLog>=LogDefine.DateFr && sDateLog<=LogDefine.DateTo)//日期范围
                    && (!LogDefine.RecNum || sRecno>=LogDefine.RecNum && sRecno<=LogDefine.RecNumTo)//指定收据号
                    && (!LogDefine.LogType || LogDefine.LogType==((TEJDOCHEAD*)DataFromEJ)->EJDocType)//指定EJ类型
                    && (!LogDefine.FuncCode || LogDefine.FuncCode==((TEJDOCHEAD*)DataFromEJ)->EJClerk)//指定收款员
                   )
                {

                    if (!sEJFound)
                    {
                        sEJFound = true;
                        if (TESTBIT(ApplVar.EJVar.ContFlag,EJDOCHEAD))
                        {
                            RefreshEJStart(true);
                            StoreEJData(CEJ_DOCEND,Msg_PRNEJURNAL);
                            //StoreEJEnd();
                        }
                        RESETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
                        PrintStr_Center((char*)cMsg_EJBBIAO,true);
                        //PrintLine('-');
                    }

                    if (!prnLast)
                        PrintLine2('=');// first line must print
                    sActLog = 1;// 找到符合条件的文档,开始一个EJ日志文档 //
                    memset(EntryBuffer,0,PRTLEN);
                    j = 0;
                } else
                    sActLog = 0;// 文档不符合条件 //
                i = EJDOCHEADSIZE;   //ccr2016-11-14
            }
        }

        for (; i < (sizeof(DataFromEJ)) && (sActLog!=2) ; i++)
        {//逐字节解析从MMC中读取的数据
            sCh = DataFromEJ[i];
            sFlag = 0;//设置未处理标志

            switch (sCh)
            {//当遇到一个文档标示符时,打印此前已经生成的数据
            case CEJ_STARTLND:  //0E = printing line in double heigh
                if (!sFlag)
                {
                    sDbH= CMDP_DR;
                    sFlag = 2;
                }
            case CEJ_STARTLN:   //\r = printing line
                if (!sFlag)
                {
                    sDbH= 0;
                    sFlag = 3;
                }
                sEJcrc ^= sCh;
            case CEJ_DOCEND:   //^Z = 文档结束
                if (!sFlag)
                {
                    i++;
                    if (i==sizeof(DataFromEJ))
                    {
                        sSaveP+=i;
                        ReadFromMMC(DataFromEJ,sSaveP,sizeof(DataFromEJ));
                        i = 0;
#if (EJ_DEBUG)
                        SendString("APP_END\n",8);
#endif
                    }
#if (0)//ccr2017-01-13>>>>>因为加电初始化时无法正确写入校验,因此不检查错误
                    if (DataFromEJ[i] != sEJcrc)
                    {
#if (EJ_DEBUG)
                        sprintf(PCBuffer, "CRC Error at:0x%04x,Data=0x%02x,CRC=0x%02x\n",sSaveP+i,(BYTE)DataFromEJ[i],sEJcrc);
                        SendString(PCBuffer,strlen(PCBuffer));
#endif

                        ApplVar.ErrorNumber=ERROR_ID(CWXXI92);     //EJ CRC error
                    }
#endif//ccr2017-01-13<<<<<<<<<<
                }
            case 0:             //Epmty flags EJ存储结束
            case 0xff:          //Epmty flags EJ存储结束
                if (!sFlag)
                {
                    sActLog = 2;//文档结束
                    sLogStart = 0;
                }
            case CEJ_STARTDOC:
                if (sActLog && j)
                {//打印已取到的一条EJ
                    if (sPrintIt)
                    {
                        EntryBuffer[PRTLEN] = 0;
                        RJPrint(sDbH, EntryBuffer);
                    }
                    memset(EntryBuffer,0,PRTLEN);
                    sDbH = 0;
                }
                j = 0;
                sPrintIt = (sFlag!=1);//为数据类型时,不打印数据
                break;
            case CEJ_ESC:   //ESC=number of spaces to be decompressed
            case CEJ_ESC1:   //ESC=number of spaces to be decompressed
            case CEJ_ESC2:   //ESC=number of spaces to be decompressed
            case CEJ_ESC3:   //ESC=number of spaces to be decompressed
            case CEJ_ESC4:   //ESC=number of spaces to be decompressed
                sEJcrc ^= sCh;
                i++;
                if (i==sizeof(DataFromEJ))
                {
                    sSaveP+=i;
                    ReadFromMMC(DataFromEJ,sSaveP,sizeof(DataFromEJ));
                    i = 0;
#if (EJ_DEBUG)
                    SendString("APP_ESC\n",8);
#endif
                }
                sFlag = DataFromEJ[i];
                sEJcrc ^= sFlag;
                if (sActLog)//为符合条件的数据
                {
                    //解压空格数据
                    if (j<PRTLEN)
                    {
                        if (j+sFlag>PRTLEN)
                            sFlag = PRTLEN-j;
                        sCh = ZIPList[sCh-CEJ_ESC];
                        memset(EntryBuffer+j, sCh, sFlag);
                        j += sFlag;
                    }
                }
                break;
            default:
                sEJcrc ^= sCh;
                if (sActLog && j < PRTLEN)
                {//为符合条件的数据
                    EntryBuffer[j] = sCh;
                    j++;
                }
                break;
            }
            if (sCh==CEJ_STARTDOC || sCh==0xff || sCh==0)
            {// a new EJ document found
                sLogStart = sActLog = 0;
                break;//for (i = 0; i < (sizeof(DataFromEJ)-1) && (sActLog!=2) ; i++)
            }
        }

        sActLog &= 1;
        sSaveP = sSaveP +i;

        if (sCh==0xff || sCh==0) //EJ数据存储结束
            break;

    }
    if (sEJFound)
    {
        if (!prnLast)
        {
            PrintLine('=');
            RFeed(1);
        }

        SETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
        SETBIT(ApplVar.MyFlags,PRNHEADMUST);
        PrePrintHead(PREHEADER);
    }
}

//-----------------------------------------------------------------------------
/**
 * 加电初始化时,以枚举方式从EJ中生成EJHeader
 *
 * @author EutronSoftware (2016-11-28)
 *
 * @return UnLong :返回最后一个EJ文档的记录号EJ#
 */
UnLong RecallEJHead()
{
    WORD i;
    UnLong sSaveP,sRecno;
    BYTE sCh,sEJcrc;
    BYTE sActLog,sLogStart,j;           //=0,Document is not for printe,=1Document must be printed,=2 At the end of document
    TEJINDEX IndexBuff;

    GetMMCSize();
    if (!ReadFromMMC((char*)&ApplVar.EJHeader,EJHEADERADDR,HEADSIZE))
    {
        ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
        return 0;
    }
    if (ApplVar.EJHeader.EJMark!=EJMARK)
    {//EJ 未初始化
        ApplVar.ErrorNumber = ERROR_ID(CWXXI98);
        return 0;
    }
    //搜寻最后一个索引记录
    sSaveP = ApplVar.EJHeader.AddrEJIndex;

    for (ApplVar.EJHeader.IndexCount=0;ApplVar.EJHeader.IndexCount<ApplVar.EJHeader.MaxEJIndex;ApplVar.EJHeader.IndexCount++)
    {
        if (!ReadFromMMC((char*)&IndexBuff,sSaveP,INDEXRECORDSIZE))
        {
            ApplVar.ErrorNumber = ERROR_ID(CWXXI94);
            return 0;
        }
        else if (IndexBuff.EJDate==0xffff)
            break;
        else
        {
            ApplVar.EJHeader.DateLastEJ = IndexBuff.EJDate;     //索引的最后日期,不一定是最后一个EJ DOC的日期
            ApplVar.EJHeader.AddrLastEJ = IndexBuff.EJAddress;  //不一定是最后一个EJ DOC的地址
            ApplVar.EJHeader.EJDocNumber = IndexBuff.EJNumber;  //不一定是最后一个EJ#
            sSaveP+=INDEXRECORDSIZE;
        }
    }

    ApplVar.EJHeader.AddrLast2EJ = ApplVar.EJHeader.AddrLastEJ;
    ApplVar.EJHeader.AddrEJRead=0;         //需要发送的EJ数据开始地址,为相对地址
    LOCATION = ApplVar.EJHeader.EJLocation;
    REGISTER = ApplVar.EJHeader.EJRegister;

    if (ApplVar.EJHeader.IndexCount==0 || ApplVar.EJHeader.AddrLastEJ==0xffffffff)
        return 0;//无索引记录

    sRecno = 0;
    sSaveP = ApplVar.EJHeader.AddrLastEJ;

    sActLog = sLogStart = j = 0;

    DispSt2C(Msg_RECALLEJ);//ccr091210
    while (sSaveP<EJDOCADDRMAX)
    {
        i = 0;
        //read log header
        ReadFromMMC(DataFromEJ,sSaveP,sizeof(DataFromEJ));
        if (!sLogStart)
        {
            if (DataFromEJ[0] == CEJ_STARTDOC)
            {// 为一个日志文档开始标示  YYMMDDHH
                memcpy(&ApplVar.EJDocHeader,DataFromEJ,EJDOCHEADSIZE);
                sEJcrc =  (GetCRC(DataFromEJ, EJDOCHEADSIZE));
                sLogStart = 1;
//                sRecno = ((TEJDOCHEAD*)DataFromEJ)->EJDocNumber;
                sRecno = ApplVar.EJDocHeader.EJDocNumber;
                sActLog = 1;// 开始一个EJ日志文档 //
                i = EJDOCHEADSIZE;
                ApplVar.EJHeader.EJDocNumber = sRecno;// 最后一个EJ文档的序号
//                ApplVar.EJHeader.DateLastEJ = EncordDate(ApplVar.EJDocHeader.EJDateTime.year,
//                                                            ApplVar.EJDocHeader.EJDateTime.month,
//                                                            ApplVar.EJDocHeader.EJDateTime.day);
                ApplVar.EJHeader.AddrLast2EJ = ApplVar.EJHeader.AddrLastEJ;		//倒数第二个文档的位置,即最后一个有效的EJ文档
                ApplVar.EJHeader.AddrLastEJ =sSaveP;		    //最后一个文档的位置,即最后一个EJ开始文档的存储位置
                if (ApplVar.EJDocHeader.EJDateTime.day==0xff)
                    SETBIT(ApplVar.EJVar.ContFlag,EJDOCHEAD);
                else
                    RESETBIT(ApplVar.EJVar.ContFlag,EJDOCHEAD);
            }
        }

        for (; i < (sizeof(DataFromEJ)) && (sActLog!=2) ; i++)
        {
            sCh = DataFromEJ[i];

            if (sCh==CEJ_STARTDOC)
            {// a new EJ document found
                sLogStart = sActLog = 0;
                break;
            }

            switch (sCh)
            {
            case CEJ_DOCEND:    //^Z	= seal
                i++;
                if (i==sizeof(DataFromEJ))
                {
                    sSaveP+=i;
                    ReadFromMMC(DataFromEJ,sSaveP,sizeof(DataFromEJ));
                    i = 0;
#if (EJ_DEBUG)
                    SendString("APP_END\n",8);
#endif
                }
#if (0)//ccr2017-01-13>>>>>因为加电初始化时无法正确写入校验,因此不检查错误
                if (DataFromEJ[i] != sEJcrc)
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI92);      //EJ CRC error
                }
#endif//ccr2017-01-13<<<<<<
            case 0:             //Epmty flags (正常情况下无此值)
            case 0xff:          //Epmty flags (EJ数据结束标志)
                sActLog = 2;
                sLogStart = 0;
                ApplVar.EJHeader.AddrNextEJ =sSaveP+i;   //新的EJ数据将要写入的地址
                break;
            case CEJ_ESC:   //ESC=number of spaces to be decompressed
            case CEJ_ESC1:   //ESC=number of spaces to be decompressed
            case CEJ_ESC2:   //ESC=number of spaces to be decompressed
            case CEJ_ESC3:   //ESC=number of spaces to be decompressed
            case CEJ_ESC4:   //ESC=number of spaces to be decompressed
                sEJcrc ^= sCh;
                i++;
                if (i==sizeof(DataFromEJ))
                {
                    sSaveP+=i;
                    ReadFromMMC(DataFromEJ,sSaveP,sizeof(DataFromEJ));
                    i = 0;
#if (EJ_DEBUG)
                    SendString("APP_ESC\n",8);
#endif
                }

                sCh = DataFromEJ[i];
            default:
                sEJcrc ^= sCh;
                break;
            }
        }
        sActLog &= 1;
        sSaveP = sSaveP +i;
        if (sCh==0x00 || sCh==0xff)
            break;//无EJ数据
    }
    GetTimeDate(&Now);  /* read timer */
    if (EncordDate(Now.year & 0xff,Now.month,Now.day)<ApplVar.EJHeader.DateLastEJ)
    {
        DisplayPrompt(Msg_ECRDATEltEJ);
    }
    ClsState2();//ccr091210
    return sRecno;      //no Log record
}



/**
 * 检测EJ卡是否正常,在切换方式时,要CHeckEJ,以防替换EJ卡
 *
 * @author EutronSoftware (2016-10-29)
 *
 */
BYTE CheckEJ()
{
    int i;
    TEJHEADER sEJHeader;

    RESETBIT(ApplVar.EJVar.ContFlag,(EJISNEW+EJNOTEXIST+EJxECR+EJISFULL+EJWILLFULL));
#if defined(CASE_EJSD)
#if !defined(DEBUGBYPC)
    if (SD_Init()!=SD_OK || !MMC_CheckPresence())
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI93);     // no EJ //
        SETBIT(ApplVar.EJVar.ContFlag,EJNOTEXIST);
        return ERROR_ID(CWXXI93);
    }

    GetMMCSize();
    if (ApplVar.ErrorNumber==ERROR_ID(CWXXI94)) //read or write ERROR
    {
        SETBIT(ApplVar.EJVar.ContFlag,EJNOTEXIST);
        return ERROR_ID(CWXXI94);
    }
#endif
#else//>>>For CASE_EJFLASH
    GetMMCSize();
#endif

    {
        if (ReadFromMMC((BYTE *)&sEJHeader,EJHEADERADDR, HEADSIZE))
        {// The EJ is the current used by this ECR
            if (sEJHeader.EJMark != EJMARK || CWORD(sEJHeader)==0xffff || CWORD(sEJHeader)==0x0000)
            {
                SETBIT(ApplVar.EJVar.ContFlag,EJISNEW);
                ApplVar.ErrorNumber = ERROR_ID(CWXXI98);// 新的EJ //
            } else if (ApplVar.EJHeader.EJMark==EJMARK)
            {
                for (i=0;i<16;i++)
                {
                    if (sEJHeader.EJID[i]!=ApplVar.EJHeader.EJID[i])
                    {
                        ApplVar.ErrorNumber = ERROR_ID(CWXXI100);// EJ与收款机不匹配 //
                        SETBIT(ApplVar.EJVar.ContFlag,EJxECR);
                        return ERROR_ID(CWXXI100);
                    }
                }
                if (sEJHeader.EJLocation!=ApplVar.EJHeader.EJLocation || sEJHeader.EJRegister!=ApplVar.EJHeader.EJRegister
                    || sEJHeader.EJLocation!=LOCATION || sEJHeader.EJRegister!=REGISTER)
                {
                    ApplVar.ErrorNumber = ERROR_ID(CWXXI100);// EJ与收款机不匹配 //
                    SETBIT(ApplVar.EJVar.ContFlag,EJxECR);
                    return ERROR_ID(CWXXI100);
                }
                SETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);

            } else
            {
#if (0)
                if (sEJHeader.EJLocation!=LOCATION || sEJHeader.EJRegister!=REGISTER)
                {
                    ApplVar.ErrorNumber = ERROR_ID(CWXXI100);// EJ与收款机不匹配 //
                    SETBIT(ApplVar.EJVar.ContFlag,EJxECR);
                    // 需要在设置收款机号后,重新开机
                    return ERROR_ID(CWXXI100);
                } else
                {
                    ApplVar.EJHeader = sEJHeader;//加电初始化后,恢复EJ数据
                    SETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
                }
#else
                //加电初始化后,恢复EJ数据
#if defined(CASE_EJSD)
                ApplVar.EJHeader = sEJHeader;
                LOCATION = sEJHeader.EJLocation;
                REGISTER = sEJHeader.EJRegister;
#else
                RecallEJHead();
#endif
                SETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
#endif
            }
        } else
        {
            SETBIT(ApplVar.EJVar.ContFlag,EJNOTEXIST);
            ApplVar.ErrorNumber=ERROR_ID(CWXXI94);// EJ is damaged,can't write and read //
        }
    }
    if (ApplVar.EJHeader.AddrNextEJ >= ApplVar.EJHeader.AddrEJMax-MMC_FULL)
    {
        SETBIT(ApplVar.EJVar.ContFlag,EJISFULL);//ccr2016-11-29
        if (!ApplVar.ErrorNumber)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI97);//需要按两次清除才能清除此报错
    }
    else  if (ApplVar.EJHeader.AddrNextEJ >= ApplVar.EJHeader.AddrEJMax-MMC_LESS)
    {
        SETBIT(ApplVar.EJVar.ContFlag,EJWILLFULL);//ccr2016-11-29
        //if (!ApplVar.ErrorNumber)
        //    ApplVar.ErrorNumber=ERROR_ID(CWXXI96);              // 40 KB left, only warn once
    }

    return ApplVar.ErrorNumber;

}

/**
 * 当检测到的EJ为新的EJ时,必须执行此函数,对EJ进行初始化
 *
 * @author EutronSoftware (2016-10-29)
 */
void FormatSDForEJ()
{
    ULONG sAddr,sPer,sDiv100,sEJNumber;
    int sLp;
    BYTE sEJID[16];

    ApplVar.ErrorNumber=0;

#if defined(CASE_EJSD)
    //read EJ ID from MMC card
    if (!Bios(BiosCmd_SD_ReadInfo, sEJID,MMC_SEND_CID,16))
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI94);
        return;
    }
    ApplVar.EJVar.MMCLast = 0xffffffff;//ccr091206
#else//>>>for CASE_EJFLASH
    memset(sEJID,0xff,16);
    memcpy(sEJID,Release,sizeof(Release));
#endif
#if defined(CASE_PERU)//ccr2017-01-12秘鲁客户要求初始化EJ时不重新编号
    sEJNumber=ApplVar.EJHeader.EJDocNumber;
    if (sEJNumber) sEJNumber--;
#endif
    //Test if this EJ has been used by other ECR?
    if (ReadFromMMC((BYTE *)&ApplVar.EJHeader,EJHEADERADDR, HEADSIZE))
    {
#if defined(CASE_PERU)//ccr2017-01-12秘鲁客户要求初始化EJ时不重新编号
        if (CWORD(ApplVar.EJHeader)==0xffff)//为新机器
            sEJNumber=0;
#endif
        if (ApplVar.EJHeader.EJMark == EJMARK)
        {//EJ使用过
            for (sLp=0;sLp<16;sLp++)
                if (ApplVar.EJHeader.EJID[sLp] != sEJID[sLp])
                    break;
            if (sLp>=16)
            {// this EJ has been used by other Fiscal ECR
                sprintf(SysBuf,"ECR:#%03u/%03u",ApplVar.EJHeader.EJRegister,ApplVar.EJHeader.EJLocation);
                DispSt1(SysBuf);
                if (!WaitForYesNo(Msg_EJNOTEMPTY,false))
                {//在本机上恢复使用此SD卡继续存储数据
#if 0
                    ApplVar.EJHeader.EJMark=0;
#else
#if defined(CASE_EJSD)
                    LOCATION = ApplVar.EJHeader.EJLocation;
                    REGISTER = ApplVar.EJHeader.EJRegister;
#else //>>>CASE_EJFLASH
                    RecallEJHead();
#endif
                    DispSt1C(Msg_RECALLEJ);
#endif
                    return;
                } else
                    DispSt1C(Msg[MENUZRESETEJ].str);
                //ApplVar.ErrorNumber=ERROR_ID(CWXXI100);
            }
        }
    } else
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI94);
        return;
    }

    GetMMCSize();//ccr091102
    if (ApplVar.ErrorNumber==ERROR_ID(CWXXI94))
    {
        ApplVar.EJHeader.EJMark=0;
        return;
    }

    //count

    //Init Date & Time
    GetTimeDate(&Now);
    memset(&ApplVar.EJHeader, 0, HEADSIZE);
    ApplVar.EJHeader.InitDateTime.year = (Now.year & 0xff); // DD
    ApplVar.EJHeader.InitDateTime.month = Now.month;            // MM
    ApplVar.EJHeader.InitDateTime.day = Now.day;                // YY
    ApplVar.EJHeader.InitDateTime.hour = Now.hour;          // HH
    ApplVar.EJHeader.InitDateTime.minute = Now.min;             // MM
    ApplVar.EJHeader.InitDateTime.second = Now.sec;             // SS
    //Type of EJ
#if defined(CASE_EJSD)
    (ApplVar.EJVar.MMCSIZEMAX<=0x2000000L)?(ApplVar.EJHeader.EJType=1)
    :(ApplVar.EJVar.MMCSIZEMAX<=0x8000000L)?(ApplVar.EJHeader.EJType =2)
    :(ApplVar.EJHeader.EJType =3);
#else
    ApplVar.EJHeader.EJType =0xEF;//标示为EPROM FLASH
#endif


    ApplVar.EJHeader.EJMark = EJMARK;
    memcpy(ApplVar.EJHeader.EJID,sEJID,16);

#if defined(STM32F10X_HD)
    ApplVar.EJHeader.Release = 0xF103;//STM32F10X
#else
    ApplVar.EJHeader.Release = 0xF205;//STM32F20X
#endif

    //EJ卡的基本信息
    ApplVar.EJHeader.SizeOfSD=ApplVar.EJVar.MMCSIZEMAX;   //SD卡的容量,=ApplVar.EJVar.MMCSIZEMAX;
    ApplVar.EJHeader.SizeEJIndex=GETINDEXSIZE();
    ApplVar.EJHeader.AddrEJFirst=EJDOCADDRFROM;  //第一条EJ记录的地址,也是EJHeader的分配空间,=EJDOCADDRFROM
    ApplVar.EJHeader.AddrEJMax=EJDOCADDRMAX;  //EJ记录数据空间的最大地址,=EJDOCADDRMAX
    ApplVar.EJHeader.AddrEJIndex=INDEXADDR;    //EJ数据索引存放位置,=INDEXADDR
    ApplVar.EJHeader.MaxEJIndex=INDEXRECORDCOUNT;    //EJ数据索引记录最大数
    ApplVar.EJHeader.AddrEJHeader=EJHEADERADDR;    //存放EJHeader数据的地址,=EJHEADERADDR
    ApplVar.EJHeader.SizeEJHeader=HEADSIZE;    //EJHeader的大小,实际分配空间为1024字节

    //EJ读写管理数据
    ApplVar.EJHeader.EJLocation = LOCATION;
    ApplVar.EJHeader.EJRegister = REGISTER;

    ApplVar.EJHeader.AddrEJRead=0;  //从0开始发送数据,为相对地址
    ApplVar.EJHeader.AddrNextEJ = EJDOCADDRFROM; //0;
    ApplVar.EJHeader.AddrLastEJ =  EJDOCADDRFROM;  //0;
    ApplVar.EJHeader.AddrLast2EJ = ApplVar.EJHeader.AddrLastEJ;

    ApplVar.EJHeader.DateLastEJ = 0;  //当前存储EJ的日期(最后一天的EJ日期)
    ApplVar.EJHeader.IndexCount = 0;
#if defined(CASE_PERU)//ccr2017-01-12秘鲁客户要求初始化EJ时不重新编号
    //ccr2017-01-24>>>>输入EJ#
    mClearScreen();
    DispStrXY("INPUT EJ#:",0,0);

    memset(SysBuf,' ',SCREENWD);SysBuf[SCREENWD]=0;
    ULongtoASC(SysBuf+SCREENWD-1,sEJNumber+1);
    DispStrXY(SysBuf,0,1);

    if (GetStrFrKBD('9',SCREENWD-7,1,12,0)>0 && Appl_EntryCounter)
    {
        StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
        BCDValueToULong(ApplVar.Entry.Value,&sEJNumber);
        if (sEJNumber) sEJNumber--;
    }
    //ccr2017-01-24<<<<<<<<<<<<<<<<<<
    ApplVar.EJHeader.EJDocNumber = sEJNumber;

//ccr2017-02-08临时版本>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    mClearScreen();
    DispStrXY("Receipt #:",0,0);

    memset(SysBuf,' ',SCREENWD);SysBuf[SCREENWD]=0;
    HEXtoASC(SysBuf+SCREENWD-1-RECNUM*2,ApplVar.ReceiptNumber,RECNUM);
    DispStrXY(SysBuf,0,1);

    if (GetStrFrKBD('9',SCREENWD-7,1,RECNUM*2,0)>0 && Appl_EntryCounter)
    {
        StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
        ApplVar.Entry.Sign=0;
        if (CheckNotZero(&ApplVar.Entry))
            Subtract(&ApplVar.Entry,(BCD*)&ONE);
        memcpy(ApplVar.ReceiptNumber,ApplVar.Entry.Value,RECNUM);
    }

    mClearScreen();
    DispStrXY("Z-Count #:",0,0);

    memset(SysBuf,' ',SCREENWD);SysBuf[SCREENWD]=0;
    HEXtoASC(SysBuf+SCREENWD-1-ZSIZE*2,ApplVar.ZCount,ZSIZE);
    DispStrXY(SysBuf,0,1);

    if (GetStrFrKBD('9',SCREENWD-7,1,ZSIZE*2,0)>0 && Appl_EntryCounter)
    {
        StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
        if (CheckNotZero(&ApplVar.Entry))
            Subtract(&ApplVar.Entry,(BCD*)&ONE);
        memcpy(ApplVar.ZCount,ApplVar.Entry.Value,ZSIZE);
    }
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#else
    ApplVar.EJHeader.EJDocNumber = 0;
#endif
    ApplVar.EJHeader.EJClerk = ApplVar.ClerkNumber;//初始化EJ的操作员

    //<<<<<<<<<<<<<<<<<<<<<<<<<
#if defined(CASE_EJSD)
    if (WaitForYesNo(cMsg_FORMAT,false))
    {//对SD卡进行完整格式化
        ClsState2();

        DispSt1(Msg_FORMATED);
        memset(DataFromEJ,0xff,SD_BLOCKSIZE);
        sDiv100 = ApplVar.EJVar.MMCSIZEMAX / 100;
        if (sDiv100==0)
            sDiv100 = 100;
        for (sAddr = 0;(sAddr<ApplVar.EJVar.MMCSIZEMAX-SD_BLOCKSIZE*2) && !KbHit();sAddr+=SD_BLOCKSIZE)
        {
            RamOffSet = sAddr;
            sEJID[0]=WriteStream(DataFromEJ,SD_BLOCKSIZE,true);
            sPer = (sAddr/sDiv100);
            sPer=WORDtoASCL(DataFromEJ,sPer);DataFromEJ[sPer++]='%';DataFromEJ[sPer]=0;
            DispStrXY(DataFromEJ,SCREENWD-3,STATELINE);

            if (sEJID[0])
            {
                sEJID[0] = ReadFromMMC(DataFromEJ,sAddr,SD_BLOCKSIZE);
                if (sEJID[0])//读写成功,比较数据的正确性
                {
                    for (sLp=0;sLp<SD_BLOCKSIZE;sLp++)
                        if (DataFromEJ[sLp]!=0xff)
                        {
                            sEJID[0]=0;
                            break;
                        }
                }
            }
            if (!sEJID[0])
            {//SD卡出错
                ApplVar.EJVar.MMCLast = 0xffffffff;
                ApplVar.ErrorNumber=ERROR_ID(CWXXI94);         //read or write EJ error
                ApplVar.EJHeader.EJMark=0;
                return;
            }
        }
    } else
    {
        memset(MMC_Buff,0xff,SD_BLOCKSIZE);
        if (!MMC_WriteBlock(MMC_Buff, EJDOCADDRFROM))
        {//清除第一块
            ApplVar.EJVar.MMCLast = 0xffffffff;
            ApplVar.ErrorNumber=ERROR_ID(CWXXI94);         //read or write EJ error
            ApplVar.EJHeader.EJMark=0;
            return;
        }
    }

    memcpy(&MMC_Buff, (BYTE *)&ApplVar.EJHeader, HEADSIZE);
    if (!MMC_WriteBlock(MMC_Buff, EJHEADERADDR))
    {
        ApplVar.EJVar.MMCLast = 0xffffffff;
        ApplVar.ErrorNumber=ERROR_ID(CWXXI94);         //read or write EJ error
        ApplVar.EJHeader.EJMark=0;
        return;
    }


    ApplVar.EJVar.MMCLast = INDEXADDR;
    memset(MMC_Buff,0,SD_BLOCKSIZE);
    for (sLp=0;sLp<ApplVar.EJHeader.MaxEJIndex;sLp++)
    {
        if (!MMC_WriteBlock(MMC_Buff,ApplVar.EJVar.MMCLast))
        {
            ApplVar.EJVar.MMCLast = 0xffffffff;
            ApplVar.ErrorNumber=ERROR_ID(CWXXI94);     //read or write EJ error
            ApplVar.EJHeader.EJMark=0;
            return;
        }
        ApplVar.EJVar.MMCLast += SD_BLOCKSIZE;
    }

    ApplVar.EJVar.MMCLast = 0xffffffff;
#else//>>>for CASE_EJFLASH
    //擦除FLASH
    DispStrXY(Msg[WAITING].str,0,1);
    bFlashMBlockErase(BLOCK1STFOREJ,BLOCKSFOREJ);
    WriteEJHead();
    if (ApplVar.ErrorNumber == ERROR_ID(CWXXI94))
    {   //写数据出错
        ApplVar.EJHeader.EJMark=0;
    }
#endif
}

/**
 * 打印税控机已经使用过和正在使用的EJ的相关信息
 * 将使用SysBuf作为工作区
 * @author EutronSoftware (2016-11-11)
 * @param
 *              oldEJ:=true,打印用过的EJ信息;=false,打印当前EJ
 */
void PrintEJInfo(BYTE oldEJ)
{
    int sP,i;
    WORD saveClerk;

    if (ApplVar.EJHeader.EJMark==EJMARK)
    {
        if (oldEJ)
        {
            RFeed(1);
            PrintStr_Center((char*)cMsg_EJXINXI,true);
        }
        //else
        //    PrintStr_Center(Msg_NEWEJXINXI,true);
        PrintLine('-');

        memset(SysBuf,' ',PRTLEN);
        CopyFrStr(SysBuf, cMsg_EJCSHUA);
        //2016-01-01 01:01:01
        BCDDateTimeToStr(SysBuf+PRTLEN-20,
                         ApplVar.EJHeader.InitDateTime.year,    /* current year BCD*/
                         ApplVar.EJHeader.InitDateTime.month,   /* month (1 = Jan) BCD*/
                         ApplVar.EJHeader.InitDateTime.day, /* day of month BCD*/
                         ApplVar.EJHeader.InitDateTime.hour,    /* hours BCD*/
                         ApplVar.EJHeader.InitDateTime.minute,  /* minutes BCD*/
                         ApplVar.EJHeader.InitDateTime.second); /* seconds BCD*/
        RJPrint(0, SysBuf);

        memset(SysBuf,' ',PRTLEN);SysBuf[PRTLEN]=0;
        CopyFrStr(SysBuf, cMsg_EJDATEEND);
        //2016-01-01 01:01:01
        DeDateToStr(SysBuf+PRTLEN-15,ApplVar.EJHeader.DateLastEJ); /* */
        RJPrint(0, SysBuf);

        memset(SysBuf,' ',PRTLEN);
        CopyFrStr(SysBuf, cMsg_SIZEEJ);
        ULongtoASC(SysBuf+PRTLEN-1,ApplVar.EJHeader.SizeOfSD);
        RJPrint(0,SysBuf);
        memset(SysBuf,' ',PRTLEN);
        CopyFrStr(SysBuf, cMsg_EJLEFT);
        ULongtoASC(SysBuf+PRTLEN-1,ApplVar.EJHeader.AddrEJMax-ApplVar.EJHeader.AddrNextEJ);
        RJPrint(0,SysBuf);

        memset(SysBuf,' ',PRTLEN);
        CopyFrStr(SysBuf, cMsg_EJRECORDS);
        ULongtoASC(SysBuf+PRTLEN-1,ApplVar.EJHeader.EJDocNumber);
        RJPrint(0,SysBuf);

        memset(SysBuf,' ', PRTLEN);
        {/* REGISTER number */
            sP = strlen(Msg[SHKJHAO].str);
            memcpy(SysBuf,Msg[SHKJHAO].str,sP);
            sP+=WORDtoASCL(SysBuf + sP , REGISTER);
        }
        { /* LOCATION number */
            i = strlen(Msg[WEIZHI].str);
            if (sP) sP=PRTLEN-i-3;
            memcpy(SysBuf + sP, Msg[WEIZHI].str,i);
            WORDtoASC(SysBuf + PRTLEN-1, LOCATION);
        }
        RJPrint(0,SysBuf);

        {/* CLERK */
            saveClerk = ApplVar.ClerkNumber;
            ApplVar.ClerkNumber = ApplVar.EJHeader.EJClerk;
            ReadClerk() ;
            sprintf(SysBuf,"%s %s (%d)",Msg[SHKYUANF].str,ApplVar.Clerk.Name,ApplVar.ClerkNumber);
            RJPrint(0,SysBuf);
            ApplVar.ClerkNumber = saveClerk ;
            ReadClerk() ;
        }

        PrintLine('-');
        RFeed(1);
    }
}


/**
 * 初始化EJ,只能在X/Z下调用,不能在SET下调用
 * 将使用SysBuf作为工作区
 *
 * @author EutronSoftware (2016-11-09)
 *
 * @return BYTE
 */
BYTE Initial_EJ()
{
    BYTE saveEJCtrl=APPLCTRL;

    FormatSDForEJ();
    if (ApplVar.ErrorNumber)
        return false;

    SETBIT(APPLCTRL,((BIT_EJ_X_STORE+BIT_EJ_Z_STORE+BIT_EJ_TR_STORE)));//强制保存初始化信息

    ApplVar.EJVar.ContFlag = 0;
    //RESETBIT(ApplVar.EJVar.ContFlag,(EJDOCHEAD+EJENDMUST+EJISNEW));
    SETBIT(ApplVar.EJVar.ContFlag,(EJSTOREMUST+EJHEADMUST));
    StoreEJStart();
    PrintStr_Center(Msg_EJINITIAL,true);
#if defined(CASE_EJSD)
    RefreshEJStart(false);
#else
    RefreshEJStart(true);
#endif
    PrintEJInfo(false);
    PrePrintHead(PREHEADER);
    APPLCTRL = saveEJCtrl;
    return true;
}

/**
 * 使用字母表示收款机的当前状态
 *
 * @author EutronSoftware (2016-10-28)
 *
 * @return BYTE
 */
BYTE ECRMode()
{
    switch (ApplVar.CentralLock)
    {
    case MG://return 'M';
    case RG:return 'R';
    case X: return 'X';
    case Z: return 'Z';
    case SET:return 'S';
    default:return 'O';
    }
}


/**
 * 检测EJ当前错误状态
 *
 * @author EutronSoftware (2016-11-09)
 *
 * @param disp :是否显示错误信息
 * @return BYTE :=0,无错误.否则为错误号
 */
BYTE CheckEJError(BYTE disp)
{
    BYTE sErr=0;

    if (TESTBIT(ApplVar.EJVar.ContFlag,EJISNEW))
        sErr=ERROR_ID(CWXXI98);
    else if (TESTBIT(ApplVar.EJVar.ContFlag,EJNOTEXIST))
        sErr=ERROR_ID(CWXXI93);
    else if (TESTBIT(ApplVar.EJVar.ContFlag,EJISFULL) && !ApplVar.FRegi)//进入销售时,不报错.
        sErr=ERROR_ID(CWXXI97);
    else if (TESTBIT(ApplVar.EJVar.ContFlag,EJxECR))
        sErr=ERROR_ID(CWXXI100);
    if (sErr)
    {
        ApplVar.ErrorNumber = sErr;
        if (disp)
            CheckError(0);
    }
    return sErr;
}

/**
 * 测试是否满足保存EJ数据的条件
 *
 * @author EutronSoftware (2016-11-11)
 *
 * @return BYTE:=true,保存EJ数据; =false,不保存EJ
 */
BYTE TestSaveEJ()
{
    return(!CheckEJError(true)
           && TESTBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST) //要保存EJ
           && (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG//保存营业模式下的全部数据
               || (ApplVar.CentralLock==Z && EJ_Z_STORE)
               || (ApplVar.CentralLock==X && EJ_X_STORE))
//               || (ApplVar.CentralLock==SET && ApplVar.FReceipt==1))//ccr2017-01-24确保增加PLU后重新打印票头时,有EJ#
           && (ApplVar.FTrain==0 || EJ_TR_STORE)//保存培训数据
          );
}


/**
 * 借助ListItems显示EJ信息
 *
 * @author EutronSoftware (2016-11-11)
 */
void DisplayEJInfo1BY1()
{
    ListItems(MENUXEJOURNAL,0,Msg[MENUXEJOURNAL].str,false);
}
//供ListItems使用,读取EJ信息内容
void ReadEJInfo()
{
    int sP,i;
    WORD saveClerk;

    if (ApplVar.EJHeader.EJMark==EJMARK)
    {
        memset(ListRecord.Name,' ',sizeof(ListRecord.Name));
        switch (ListRecord.Number)
        {
        case READEJINI_T:    //读取初始化标题
            strcpy(ListRecord.Name,cMsg_EJCSHUA);
            break;
        case READEJINIT_D:   //读取初始化日期
#if (SCREENWD>16)
            BCDDateTimeToStr(ListRecord.Name+SCREENWD-16,//2016-01-01 01:01:01
#else
            BCDDateTimeToStr(ListRecord.Name,//2016-01-01 01:01:01
#endif
                             ApplVar.EJHeader.InitDateTime.year,    /* current year BCD*/
                             ApplVar.EJHeader.InitDateTime.month,   /* month (1 = Jan) BCD*/
                             ApplVar.EJHeader.InitDateTime.day, /* day of month BCD*/
                             ApplVar.EJHeader.InitDateTime.hour,    /* hours BCD*/
                             ApplVar.EJHeader.InitDateTime.minute,  /* minutes BCD*/
                             ApplVar.EJHeader.InitDateTime.second); /* seconds BCD*/
            ListRecord.Name[SCREENWD]=0;
            break;
        case READEJEND_T:    //读取最后一条EJ的日期标题
            strcpy(ListRecord.Name,cMsg_EJDATEEND);
            break;
        case READEJEND_D:    //读取最后一条EJ的日期
            //2016-01-01
            DeDateToStr(ListRecord.Name+SCREENWD-10,ApplVar.EJHeader.DateLastEJ);
            ListRecord.Name[SCREENWD]=0;
            break;
        case READEJSIZE_T:    //读取EJ大小标题
            strcpy(ListRecord.Name,cMsg_SIZEEJ);
            break;
        case READEJSIZE_D:    //读取EJ大小
            ULongtoASC(ListRecord.Name+SCREENWD-1,ApplVar.EJHeader.SizeOfSD);
            break;
        case READEJLEFT_T:    //读取EJ剩余大小标题
            strcpy(ListRecord.Name,cMsg_EJLEFT);
            break;
        case READEJLEFT_D:    //读取EJ剩余大小
            ULongtoASC(ListRecord.Name+SCREENWD-1,ApplVar.EJHeader.AddrEJMax-ApplVar.EJHeader.AddrNextEJ);
            break;
        case READEJRECORDS_TD:    //读取EJ记录数
            CopyFrStr(ListRecord.Name,cMsg_EJRECORDS);
            ULongtoASC(ListRecord.Name+SCREENWD-1,ApplVar.EJHeader.EJDocNumber);
            break;
        case READEJREGISTER_TD:    //读取EJ收款机号 /* REGISTER number */
            CopyFrStr(ListRecord.Name,Msg[SHKJHAO].str);
            WORDtoASC(ListRecord.Name+SCREENWD-1 , REGISTER);
            ListRecord.Name[SCREENWD]=0;
            break;
        case READEJLOCATION_TD:    //读取EJ商店号 /* LOCATION number */
            CopyFrStr(ListRecord.Name,Msg[WEIZHI].str);
            WORDtoASC(ListRecord.Name+SCREENWD-1 , LOCATION);
            ListRecord.Name[SCREENWD]=0;
            break;
        case READEJCLERK_T:    //读取EJ收款员标题/* CLERK */
            strcpy(ListRecord.Name,Msg[SHKYUANF].str);
            break;
        case READEJCLERK_D:    //读取EJ收款员
            saveClerk = ApplVar.ClerkNumber;
            ApplVar.ClerkNumber = ApplVar.EJHeader.EJClerk;
            ReadClerk() ;
            sprintf(ListRecord.Name+4,"%s (%d)",ApplVar.Clerk.Name,ApplVar.ClerkNumber);
            ApplVar.ClerkNumber = saveClerk ;
            ReadClerk() ;
            break;
        }
        ListRecord.Name[SCREENWD]=0;
    }
}

/**
 * 显示或打印EJ数据
 *
 * @author EutronSoftware (2016-11-17)
 *
 * @param prnEJLast:=false,显示EJ信息;
 *                 =true,打印最后一个EJ
 */
void DisplayEJInfo(BYTE prnEJLast)
{
    int sP,i;
    char dispBuf[SCREENWD+1];
    WORD saveClerk;

    if (ApplVar.EJHeader.EJMark==EJMARK)
    {
        if (prnEJLast)
        {
            PrintEJLog(true);
        }
        //else
        {//显示EJ信息
            //读取EJ记录数
            memset(dispBuf,' ',sizeof(dispBuf));dispBuf[SCREENWD]=0;
            CopyFrStr(dispBuf,cMsg_EJRECORDS);
            ULongtoASC(dispBuf+SCREENWD-1,ApplVar.EJHeader.EJDocNumber-1);//-1后,为可见的EJ号.
            DispStrXY(dispBuf,0,0);

#if (SCREENLN<6)
            //读取EJ剩余大小
            DispStrXY((BYTE*)cMsg_EJLEFT,0,1);
            memset(dispBuf,' ',sizeof(dispBuf));dispBuf[SCREENWD]=0;
            ULongtoASC(dispBuf+SCREENWD-1,ApplVar.EJHeader.AddrEJMax-ApplVar.EJHeader.AddrNextEJ);
            DispStrXY(dispBuf,0,2);
#else
            //读取EJ大小
            memset(dispBuf,' ',sizeof(dispBuf));dispBuf[SCREENWD]=0;
            CopyFrStr(dispBuf,cMsg_SIZEEJ);
            ULongtoASC(dispBuf+SCREENWD-1,ApplVar.EJHeader.SizeOfSD);
            DispStrXY(dispBuf,0,1);

            //读取EJ剩余大小
            memset(dispBuf,' ',sizeof(dispBuf));dispBuf[SCREENWD]=0;
            CopyFrStr(dispBuf,cMsg_EJLEFT);
            ULongtoASC(dispBuf+SCREENWD-1,ApplVar.EJHeader.AddrEJMax-ApplVar.EJHeader.AddrNextEJ);
            DispStrXY(dispBuf,0,2);
            //读取EJ收款机号 /* REGISTER number */
            memset(dispBuf,' ',sizeof(dispBuf));
            CopyFrStr(dispBuf,Msg[SHKJHAO].str);
            WORDtoASC(dispBuf+SCREENWD-1 , REGISTER);
            dispBuf[SCREENWD]=0;
            DispStrXY(dispBuf,0,3);

            //读取EJ商店号 /* LOCATION number */
            memset(dispBuf,' ',sizeof(dispBuf));
            CopyFrStr(dispBuf,Msg[WEIZHI].str);
            WORDtoASC(dispBuf+SCREENWD-1 , LOCATION);
            dispBuf[SCREENWD]=0;
            DispStrXY(dispBuf,0,4);
            //读取EJ收款员
            memset(dispBuf,' ',sizeof(dispBuf));
            CopyFrStr(dispBuf,Msg[SHKYUANF].str);
            saveClerk = ApplVar.ClerkNumber;
            ApplVar.ClerkNumber = ApplVar.EJHeader.EJClerk;
            ReadClerk() ;
            strncpy(dispBuf+10,ApplVar.Clerk.Name,SCREENWD-10);
            dispBuf[SCREENWD]=0;
            DispStrXY(dispBuf,0,5);
            ApplVar.ClerkNumber = saveClerk ;
            ReadClerk() ;
            //读取初始化日期
            memset(dispBuf,' ',sizeof(dispBuf));
            DispStrXY((BYTE*)cMsg_EJCSHUA,0,6);
            BCDDateTimeToStr(dispBuf,//2016-01-01 01:01:01
                             ApplVar.EJHeader.InitDateTime.year,    /* current year BCD*/
                             ApplVar.EJHeader.InitDateTime.month,   /* month (1 = Jan) BCD*/
                             ApplVar.EJHeader.InitDateTime.day, /* day of month BCD*/
                             ApplVar.EJHeader.InitDateTime.hour,    /* hours BCD*/
                             ApplVar.EJHeader.InitDateTime.minute,  /* minutes BCD*/
                             ApplVar.EJHeader.InitDateTime.second); /* seconds BCD*/
            dispBuf[10]=0;
            DispStrXY(dispBuf,SCREENWD-10,6);

#endif
        }
    }
}

//=======================================================================================//
#endif


