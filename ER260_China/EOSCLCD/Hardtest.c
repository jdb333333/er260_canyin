#define HARDTEST_C 7
#include "king.h"               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "Message.h"
#include "chnmess.h"
#include "Flashb.h"

extern void TestSendMess(void);

void PrintConfInf();		   //lyq added 20040303

long TestLMM(WORD lmmPage);


void TestPrinter()
{
	short i,j;

	OutPrint(CMDP_R, (char*)cPRTStyleN1);		//Normale
	OutPrint(CMDP_DR, (char*)cPRTStyleN2);				//Doppia altezza
	OutPrint(CMDP_R, (char*)cPRTStyleN3);	//Raddoppio caratteri
	OutPrint(CMDP_DR, (char*)cPRTStyleN4);		//Dopia altezza + Raddoppio caratteri
	OutPrint(CMDP_LFR, NULL);								//Line Feed standard
	OutPrintEx(((1 << B_PRI_RON) | (1 << B_PRI_FCUT)), NULL, 50); //taglio carta + 100 dot-lines bianche

	if (ApplVar.Graph[0].PictNo>0)
    {
		i = ApplVar.Graph[0].PictNo;
        if (i>=GRAPHMAX-5)
            i = GRAPHMAX-5;
    }
	else
		i = 1;

#if !defined(CASE_INNERDOTPRN)
	RJPrint(0,"Graphic 1")
	Bios(BiosCmd_PrintGraph, (void*)i, 0 , 10);i++; //Stampa msg ram
	RJPrint(0,"Graphic 2")
	Bios(BiosCmd_PrintGraph, (void*)i, 2 , 20);i++;  //Get info+reload default
	RJPrint(0,"Graphic 3")
	Bios(BiosCmd_PrintGraph, (void*)i, 1, 10); //Stampa msg ram
#endif
    //正常字符打印
#if defined(CASE_FORHANZI)
    for (j=0,i=33;i<128;i++)
#else
    for (j=0,i=33;i<256;i++)
#endif
    {
        SysBuf[j++] = i;
        if (j == PRTLEN)
        {
            SysBuf[j] = 0;
            RJPrint(0,SysBuf);
            j = 0;
        }
    }
    if (j>0)
    {
        SysBuf[j] = 0;
        RJPrint(0,SysBuf);
    }

    //倍宽倍高打印
    SysBuf[0] = '@';
#if defined(CASE_FORHANZI)
    for (j=1,i=33;i<128;i++)
#else
    for (j=1,i=33;i<256;i++)
#endif
    {
        SysBuf[j++] = '~';
        SysBuf[j++] = i;
        if (j >= PRTLEN)
        {
            SysBuf[j] = 0;
            RJPrint(0,SysBuf);
            j = 1;
        }
    }
    if (j>1)
    {
        SysBuf[j] = 0;
        RJPrint(0,SysBuf);
    }

}

void TestDisplay()
{
	short i,j;

	GotoXY(0,0);
	for (j=32;j<127;j++)
	{
		memset(SysBuf,j,SCREENWD);
		DispStrLF(SysBuf);
		Delay(ASECOND/100);
	}
#if defined(CASE_FORHANZI)
	mClearScreen();
	for (i=0;i<SCREENWD;i+=2)
	{
		SysBuf[i]=0xb0;		SysBuf[i+1]=0xa1+i;
	}
	for (j=16;j<88;j++)
	{
		DispStrLF(SysBuf);
		for (i=0;i<SCREENWD;i+=2)
			SysBuf[i]++;
		Delay(ASECOND/100);
	}
#endif
}

void TestCustomer()
{
	short i,j;

	MemSet(SysBuf,DISLEN,' ');

	for (j=0;j<2;j++)
		for (i=0;i<DISLEN;i++)
		{
            LCD10_SetFlag(i);
			SysBuf[i]='8' + 0x80;
			PutsO(SysBuf);//C-DISPLAY
			Delay(ASECOND/10);
			SysBuf[i]=' ';
		}
}

//------------------------------------------------------------------------------------
void PrintLine2(char c)          /* print line of characters */
{
    MemSet(SysBuf, PRTLEN, c);
    SysBuf[PRTLEN]=0;
	RJPrint(0, SysBuf);
}

//------------------------------------------------------------------------------------
void PrintVersion()
{
	WORD i,j;

    i = CheckSum();

	RJPrint(0,Msg[FXBBEN].str);
	memset(SysBuf, 0, sizeof(SysBuf));
	//SysBuf[0] = SysBuf[1] = ' ';
	//strcpy(SysBuf+2,Release);
	strcpy(SysBuf,Release);
#if (SCREENWD>16)
	strcpy(SysBuf+strlen(SysBuf),"-Sum:");
	j = strlen(SysBuf);
#else
    j = strlen(SysBuf);
    SysBuf[j++]='-';
#endif
	HEXtoASC(SysBuf+j, (char *)&i, 2);

	SysBuf[j+4] = 0;

	//PrnBuffer(SysBuf, j+5);

    DispStrLF(SysBuf);
    RJPrint(0, SysBuf);

#if defined(CASE_FORHANZI)
	//将Apr 16 2013转换为2013-4-16格式

	i = strlen(__DATE__)-1;
	SysBuf[3]=__DATE__[i--];SysBuf[2]=__DATE__[i--];SysBuf[1]=__DATE__[i--];SysBuf[0]=__DATE__[i--];
	SysBuf[4]='-';
	j = 5;
	//将英文缩写月份转换为数字
	switch (__DATE__[0])
	{
	case 'J':
		if (__DATE__[1]=='a')
			SysBuf[j++]='1';
		else if (__DATE__[2]=='n')
			SysBuf[j++]='6';
		else if (__DATE__[2]=='l')
			SysBuf[j++]='7';
		break;
	case 'F':
		SysBuf[j++]='2';
		break;
	case 'M':
		switch(__DATE__[2])
		{
		case 'r':SysBuf[j++]='3';break;
		case 'y':SysBuf[j++]='5';break;
		}
		break;
	case 'A':
		switch(__DATE__[1])
		{
		case 'p':SysBuf[j++]='4';break;
		case 'u':SysBuf[j++]='8';break;
		}
		break;
	case 'S':
		SysBuf[j++]='9';
		break;
	case 'O':
		SysBuf[j++]='1';SysBuf[j++]='0';
		break;
	case 'N':
		SysBuf[j++]='1';SysBuf[j++]='1';
		break;
	case 'D':
		SysBuf[j++]='1';SysBuf[j++]='2';
		break;
	}
	SysBuf[j++]='-';
	i-=2;//point to day
	if (__DATE__[i]!=' ' && __DATE__[i]!='0')
		SysBuf[j++]=__DATE__[i];
	i++;
	if (__DATE__[i]!=' ')
		SysBuf[j++]=__DATE__[i];

	i = j;
#else
	strcpy(SysBuf,__DATE__);//Apr 16 2013
	i = strlen(SysBuf);
#endif
    SysBuf[i++]=',';
    strcpy(SysBuf+i,__TIME__);
    DispStrLF(SysBuf);
    RJPrint(0,SysBuf);
}
//在BlockOffset[15]到BlockOffset[FLASH_BLOCKS]之间按字节写入数据,
//判断是否会破坏BlockOffset[0];sAddr<BlockOffset[15]之间的数据
#define TESTLEN     251
void TestFlashBWrite(BYTE t)
{
#if (defined(CASE_EJFLASH) && defined(FOR_DEBUG))
    int i,j;
    uint32_t sAddr;
    uint32_t sAdd1,sAdd2;

    sAdd1=BlockOffset[4];
    sAdd2=BlockOffset[FLASH_BLOCKS-1]+0x10000;

    DispStrXY("CHECK FLASH:",0,2);
    sprintf(SysBuf,">FLASH R/W Test(%d)!",(int)t);
    RJPrint(0,SysBuf);
    bFlashMBlockErase(4,31);xputs(">Flash Block Erase finished!\n");

    for (i=0;i<TESTLEN;i++)
        PCBuffer[i]='A'+i;
    for (sAddr=sAdd1;sAddr<sAdd2;sAddr+=TESTLEN)
    {
        HEXtoASC(SysBuf,(char*)&sAddr,3); DispStrXY(SysBuf,SCREENWD-6,2);
        if (sAddr+TESTLEN>sAdd2)
            j=sAdd2-sAddr;
        else
            j=TESTLEN;
        if (t)
        {//部分字节为0xff
            memcpy(SysBuf,PCBuffer,TESTLEN);
            for (i=15;i<20;i++)  SysBuf[i]=0xff;
            i = bFlashProgram(sAddr,j,SysBuf);
            if (i!=FLASH_SUCCESS)
            {
                sprintf(SysBuf,"Error AT:60%06LX-Program-1(%d) ",sAddr,i);
                RJPrint(0,SysBuf);
                j=0; sAdd1 = sAddr;
                break;
            }
        }
        i = bFlashProgram(sAddr,j,PCBuffer);
        if (i!=FLASH_SUCCESS)
        {//在原地址上覆盖写入,改写部分0xff的数据
            sprintf(SysBuf,"ERROR AT:60%06LX-Program-0(%d) ",sAddr,i);
            RJPrint(0,SysBuf);
            j=0; sAdd1 = sAddr;
            break;
        }
        bFlashReadBuffer(SysBuf,sAddr,j);
        for (i=0;i<j;i++)
        {
            if ((BYTE)SysBuf[i]!=(BYTE)PCBuffer[i])
            {
                sprintf(SysBuf,"60%06LX-W%02x-R%02x,(%d)\n",sAddr+i,PCBuffer[i] & 0xff,SysBuf[i] & 0xff,i);
                RJPrint(0,SysBuf);
                sAdd1 = sAddr+i;
				j = 0;//标示出错
                break;
            }
        }
        if (!j)
            break;
        //for (i=0;i<TESTLEN;i++) PCBuffer[i] ^= 0xff;
    }
    xprintf(">FLASH写测试完毕\n");
    memset(SysBuf,' ',SCREENWD);
    if (!j)
    {
        CopyFrStr(SysBuf,"FLASH Error:");
        sprintf(SysBuf,"FLASH Error:60%06LX",sAdd1);
        RJPrint(0,SysBuf);
        sAdd2=sAdd1;
    }
    else
    {
        CopyFrStr(SysBuf,"FLASH CHECKED:");
        sAdd2 -= sAdd1;
    }
    SysBuf[SCREENWD]=0;
    HEXtoASC(SysBuf+SCREENWD-6,(char*)&sAdd2,3); DispStrXY(SysBuf,0,2);


#endif
}
void TestRomRam()
{
	WORD i,j;
	long lmmAddr,ramAddr;

#if !defined(DEBUGBYPC)    //ccr20131112>>>>>>>>>>>
	while( TestPrintGoingOn() )	{};//打印时,禁止TestLMM
#endif
    ramAddr = TestRam();// 测试扩展RAM (10000H-27fff) //
    lmmAddr = TestLMM(0);// 测试扩展RAM (会影响打印！！！！)
    //<<<<<<<<<<<<<<<<<<<<<

	MemSet(SysBuf,PRTLEN+1,' ');
	strcpy(SysBuf,"RAM:");
	j = strlen(SysBuf);
	i = 1;
	if (ramAddr<0)
	{
		ramAddr = -ramAddr;
		strcpy(SysBuf+j-5,"Error at:");
		j = strlen(SysBuf);
		i = 0;
	}
	HEXtoASC(SysBuf+j, (char *)&ramAddr, 3);

	SysBuf[j+6] = 'H';
	SysBuf[j+7] = 0;
	DispStrLF(SysBuf);
    RJPrint(0, SysBuf);

	MemSet(SysBuf,PRTLEN+1,' ');
	strcpy(SysBuf,"EXT_RAM:");
	j = strlen(SysBuf);
	if (lmmAddr<0)
	{
		lmmAddr = -lmmAddr;
		strcpy(SysBuf+j-5,"Error at:");
		j = strlen(SysBuf);
	}
	HEXtoASC(SysBuf+j, (char *)&lmmAddr, 3);

	SysBuf[j+6] = 'H';
	SysBuf[j+7] = 0;
	DispStrLF(SysBuf);
    RJPrint(0, SysBuf);

    PrintLine2('=');

#if (defined(FOR_DEBUG) && !defined(DEBUGBYPC))//ccr2015-11-30>>>>>>>>>>
    Beep(2);
    if (WaitForYesNo("TEST EJ-FLASH?",false))
    {
         TestFlashBWrite(0);
         TestFlashBWrite(1);
    }
#endif//ccr2015-11-30<<<<<<<<<<<<<

    PrintStr_Center("Memory for Receipt LOG",1);
    sprintf(SysBuf,"Size(BYTES)=      %lu",ApplVar.FlowHeader.MAXNUMB);//日志空间大小,=EndAddr-FromAddr
    RJPrint(0, SysBuf);
    sprintf(SysBuf,"Address From=     %lu",ApplVar.FlowHeader.FromAddr);//日志存放开始地址
    RJPrint(0, SysBuf);
    sprintf(SysBuf,"Address Last=     %lu",ApplVar.FlowHeader.EndAddr);//日志存放结束地址+1,日志的存储地址为FromAddr<=LOG<EndAddr
    RJPrint(0, SysBuf);
    sprintf(SysBuf,"First record=     %lu",ApplVar.FlowHeader.HeadP);//第一条有效日志的存放地址
    RJPrint(0, SysBuf);
    sprintf(SysBuf,"Record for Read=  %lu",ApplVar.FlowHeader.ReadP);//下一条要发送给主机的日志数据地址,ReadP>=HeadP
    RJPrint(0, SysBuf);
    sprintf(SysBuf,"Address for Write=%lu",ApplVar.FlowHeader.NextNP);//下一条日志的存放地址
    RJPrint(0, SysBuf);

}

void TestDateTime()
{
	if (TestRtc())
	{
		RJPrint(0,"Format error on DateTime.");
	}
	else
	{
		memset(SysBuf,' ',PRTLEN);
		DateTimeToStr(SysBuf,3);
		SysBuf[PRTLEN] = 0;
		DispStrLF(SysBuf);
		RJPrint(0,SysBuf);
	}
}

void TestBEEP()
{
	short i;

	Delay(ASECOND);
    strcpy(SysBuf,"Beep:");
	for (i = 0; i < 4; i++)
	{
        SysBuf[5]=i+'1';SysBuf[6]=0;
        DispStrLF(SysBuf);
		Bell(i);
		Delay(ASECOND*4);
	}
}

void TestOpenDrawer()
{
#if ((DD_ECRVIP==1) && defined(FOR_DEBUG))//testonly
    int i;
    Bell(0);
    DispStrXY("Create ECR-VIP:",0,0);
    for (i=0;i<1024+16;i++)
    {
        if (ApplVar.ICCardSet.ECRVIPCount<ECRVIPMAX)
        {
            memset(ECRVIP.VipFix.VipTEL,0,sizeof(ECRVIP.VipFix.VipTEL));
            ECRVIP.VipFix.VipTEL[0]=DECtoBCD((ApplVar.ICCardSet.ECRVIPCount+1)%100);
            ECRVIP.VipFix.VipTEL[1]=DECtoBCD((ApplVar.ICCardSet.ECRVIPCount+1)/100);
            ECRVIP.VipFix.VipTEL[2]=0x01;
            sprintf(ECRVIP.VipFix.VipName,"VIP%d",ApplVar.ICCardSet.ECRVIPCount+1);
            AddNewECRVIP();
        }
        if (i==1024) Bell(0);
        ECRVIP.ECRVIPNumber=ApplVar.ICCardSet.ECRVIPCount-1;
        ReadECRVIPVar();
        ECRVIP.VipVar[ECRVIP.ECRVIPNumber%(ECRVIPMAX/2)].VipPoints=ApplVar.ICCardSet.ECRVIPCount+Now.sec;
        ECRVIP.VipVar[ECRVIP.ECRVIPNumber%(ECRVIPMAX/2)].VipAmt=ApplVar.ICCardSet.ECRVIPCount*256+Now.sec;
        WriteECRVIPVar();
        DisplayDecXY(i+1,10,1,4);
    }
#endif

	OpenDrawer();
}

void TestKeyboard()
{
	short i;
	do{
		while (!KbHit());
		i = Getch();            /* wait keyhit */
		MemSet(SysBuf, SCREENWD, ' ');
		SysBuf[0] = 'K';
		SysBuf[1] = 'e';
		SysBuf[2] = 'y';

		WORDtoASC(SysBuf + 7, i);
//ccr2016-03-01		PutsO(SysBuf);
		DispStrLF(SysBuf);
	} while ((BYTE)i != ApplVar.AP.FirmKeys[ID_CLEAR]);    /* until clear firmkey */
}

void TestPrnPower()
{

}

void TestSerial123()
{

	if (TestCom123(1))
		strcpy(SysBuf,cComTestN1);
                //"串口 1 测试出错"
	else
		strcpy(SysBuf,cComTestN2);
                //"串口 1 测试正确"
	DispStrLF(SysBuf);
	RJPrint(0,SysBuf);

	if (TestCom123(2))
		strcpy(SysBuf,cComTestN3);
                //"串口 2 测试出错"
	else
		strcpy(SysBuf,cComTestN4);
                //"串口 2 测试正确"
	DispStrLF(SysBuf);
	RJPrint(0,SysBuf);

#if (NUMPORT==3)
    if (TestCom123(3))
        strcpy(SysBuf,cComTestN3ERR);
                //"串口 3 测试出错"
    else
        strcpy(SysBuf,cComTestN3OK);
                //"串口 3 测试正确"
    DispStrLF(SysBuf);
    RJPrint(0,SysBuf);
#endif

}


void HardTest(WORD idx)
{
	WORD tstIdx;
	BYTE sKey,hz[2];
	short i;
	short sSecOld,sSecNow;

	tstIdx = idx;

    BackupScreen(1);

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
//    if (TESTBIT(tstIdx,BIT0+BIT2+BIT3+BIT10+BIT11+BIT12+BIT13))
//    {
//        if (TESTBIT(ApplVar.EJVar.ContFlag,EJDOCHEAD))
//        {
//            RefreshEJStart(true);
//            StoreEJEnd();//ccr070609pm
//        }
//        RESETBIT(ApplVar.EJVar.ContFlag,EJSTOREMUST);
//    }
#endif

	i = 0;
	if (TESTBIT(tstIdx,BIT0+BIT2+BIT10+BIT11+BIT3))
		RJPrint(0,Msg[KSHJCE].str);
	if (TESTBIT(tstIdx,BIT11))//2048
		InActive = ACTIVE+5;
	sKey = 0xff;
	sSecOld = 0;
	hz[0]=' ';hz[1]=' '+1;
	do {
		if (sKey == ApplVar.AP.FirmKeys[ID_RJFEED])
		{
			sKey = 0xff;
			JFeed();
		}
		if (TESTBIT(tstIdx,BIT0))//1
			TestPrinter();
		if (TESTBIT(tstIdx,BIT1))//2
			TestDisplay();
		if (TESTBIT(tstIdx,BIT2))//4
			TestRomRam();
		if (TESTBIT(tstIdx,BIT3))//8
			TestDateTime();
		if (TESTBIT(tstIdx,BIT4))//16
			TestBEEP();
		if (TESTBIT(tstIdx,BIT5))//32
			TestOpenDrawer();
		if (TESTBIT(tstIdx,BIT6))//64
			TestKeyboard();
		if (TESTBIT(tstIdx,BIT9))//512
			TestPrnPower();
		if (TESTBIT(tstIdx,BIT10))//1024
			TestSerial123();
        if (TESTBIT(tstIdx,BIT12))//4096
            PrintVersion();
        if (TESTBIT(tstIdx,BIT13))//8192
            PrintConfInf();
        if (TESTBIT(tstIdx,BIT14))//16384
            TestCustomer();
#if POWERCTRL
        if (!DC_DET_GET())
            mDrawPower(2,POWER_X,SCREENLN-1);//显示电池
#endif
		if (TESTBIT(tstIdx,BIT11))//2048
		{
			CheckTime(FALSE);
			sSecNow = BCDtoDEC(Now.min)*60+BCDtoDEC(Now.sec);
			if(sSecOld != sSecNow)
			{
				sSecOld = sSecNow-sSecOld;
				if (sSecOld<0)
					sSecOld += (60*60);
				i += sSecOld;
				if (i >= 300)
				{
					memset(SysBuf,' ',PRTLEN);
					DateTimeToStr(SysBuf,3);
					SysBuf[PRTLEN] = 0;
					RJPrint(0,SysBuf);
					i = 0;
				}
				if (tstIdx==BIT11)
				{
					for (sSecOld=0;sSecOld<SCREENWD;sSecOld+=2)
					{
						SysBuf[sSecOld]=hz[0];SysBuf[sSecOld+1]=hz[1];
						if (hz[0]<128)
						{
							hz[0]+=2;hz[1]+=2;
							if (hz[0]>=128)
							{
#if defined(CASE_FORHANZI)
								hz[0]=0xa1;hz[1]=0xa1;
#else
								hz[0]=' ';hz[1]=' '+1;
#endif
							}
						}
#if defined(CASE_FORHANZI)
						else
						{
							if (++hz[1]==(0xa0+95))
							{//切换显示汉字
								hz[1]=0xa1;
								if (++hz[0]==0xa0+88)
								{//切换显示ASCII字符
									hz[0]=' ';hz[1]=' '+1;
								}
							}
							else if (hz[1]==0xa0+15)
								hz[1]=0xb1;
						}
#endif
					}
					SysBuf[sSecOld]=0;
					DispStrLF(SysBuf);
				}

				sSecOld = sSecNow ;
			}
		}
	} while (TESTBIT(tstIdx,BIT11) && (!KbHit() || (sKey = Getch()) !=ApplVar.AP.FirmKeys[ID_CLEAR]));

	if (TESTBIT(tstIdx,BIT0+BIT2+BIT10+BIT11+BIT3))
	{
		SETBIT(ApplVar.MyFlags,PRNTRAI);  //lyq added 20040227
		RJPrint(0,Msg[JCJSHU].str);
        RFeed(4);
	}
//	if (!TESTBIT(tstIdx,BIT1))
//		DispStrLF(Msg[JCJSHU].str);

    PRESS_ANY_KEY(SCREENLN-2);
    RecallScreen();
	SETBIT(ApplVar.MyFlags,PRNHEADMUST);

    ClearEntry(true);
}

void RWRand(BYTE read,WORD  num, BYTE *to)
{
	RamOffSet = (UnLong)num * ApplVar.AP.Plu.RecordSize + ApplVar.AP.StartAddress[AddrPLU] ;
	if (!read)
		WriteRam(to, ApplVar.AP.Plu.RecordSize);
	else if (read == 1)
		ReadRam(to, ApplVar.AP.Plu.RandomSize);
	else if (read == 2)             /* read rest of record */
		ReadRam(to, ApplVar.AP.Plu.RecordSize);
}

#define RNEWL   127
void CheckPluFile(WORD type)
/* type == MERGEPLU(103) then merge file and update file */
/* type == REMOVEPLU(104) then remove deleted PLU from file */
/* type == REMMER(105) then merge and remove file */
/* type == TESTPLU(106) then check file sequence */
/* type == REPPLU(107) then check file sequence and repair */
{
	BYTE    rnew[RNEWL+1] ;      /* Read old in Sysbuf */

	if (!ApplVar.AP.Plu.RandomSize)
	{
		ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
		return;
	}
	MemSet(SysBuf, 24, ' ');
	memcpy(SysBuf, "PLUTEST", 7);
	WORDtoASC(SysBuf + 10, type);
	RJPrint(02, SysBuf);
	if (ApplVar.AP.Plu.RecordSize > RNEWL+1)
	{
		RJPrint(0x02, "PLU RECORD TO BIG");
		ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
		return;
	}
    ApplVar.omax = ApplVar.AP.Plu.RNumber;
	ApplVar.nmax = ApplVar.omax;
	ApplVar.errplu = 0;
	if (type != MERGEPLU) /* != 103 */
	{
		ApplVar.NewPlu = 0;
		if (type == TESTPLU || type == REPPLU) /* 106 & 107 */
		{
			rnew[RNEWL] = 0;
		}
		for (ApplVar.OldPlu = 0; ApplVar.OldPlu < ApplVar.nmax; ApplVar.OldPlu ++)
		{
			RWRand(2, ApplVar.OldPlu, SysBuf);
			if (type == TESTPLU || type == REPPLU)
			{
				if (ApplVar.OldPlu == ApplVar.omax)
					rnew[RNEWL] = 0;
				if(rnew[RNEWL] && CompareBCDValue(rnew, SysBuf, ApplVar.AP.Plu.RandomSize) >= 0)
					/* previous larger then current then delete current */
				{
					ApplVar.errplu++;
					if (type == REPPLU)
						SysBuf[ApplVar.AP.Plu.RandomSize] = 0xff;
				}
			}
			if((BYTE)SysBuf[ApplVar.AP.Plu.RandomSize] != 0xff)  /* active ? */
			{
				if (ApplVar.NewPlu != ApplVar.OldPlu)
					RWRand(0, ApplVar.NewPlu, SysBuf);         /* write back at correct position */
				ApplVar.NewPlu++;
				if (type == TESTPLU || type == REPPLU)
				{
					memcpy(rnew, SysBuf, ApplVar.AP.Plu.RandomSize);
					rnew[RNEWL] = 1;
				}
			}
			if (ApplVar.OldPlu == (ApplVar.omax - 1))  /* end of first file ? */
				ApplVar.AP.Plu.RNumber = ApplVar.NewPlu;    /* new end */
//			if (ApplVar.OldPlu == (ApplVar.nmax - 1))  /* end of second file ? */
//			{
//				ApplVar.AP.StartAddress[AddrRPLU] = ApplVar.NewPlu - (ApplVar.AP.StartAddress[AddrRPLU] & 0xffff) ;
//			}
			if (!(ApplVar.OldPlu % 10))
			{
				WORDtoASCZero(SysBuf + 7, ApplVar.OldPlu);
				SysBuf[2] = ' ';
				SysBuf[8] = 0;
				DispSt2C(SysBuf+2);
			}
		}
		if (type != TESTPLU)
		{
			MemSet(SysBuf, sizeof(SysBuf), 0);
			for (; ApplVar.NewPlu < ApplVar.nmax; ApplVar.NewPlu++)
				RWRand(0, ApplVar.NewPlu, SysBuf);         /* clear rest of file */
		}
		ApplVar.omax = ApplVar.AP.Plu.RNumber;
		ApplVar.nmax = 0;
		ApplVar.nmax += ApplVar.omax;
		if (type == TESTPLU || type == REPPLU)
		{
			MemSet(SysBuf, 24, ' ');
			CopyFrStr(SysBuf, Msg[ERRORMSG].str);
			WORDtoASC(SysBuf + 10, ApplVar.errplu);
				RJPrint(2, SysBuf);
		}
	}
	if (ApplVar.nmax != ApplVar.omax && (type == MERGEPLU || type == REMMER))
	{
		if(ApplVar.omax)
		{
			for(ApplVar.errplu = 0 ; ApplVar.errplu < 6 ; ApplVar.errplu++)
			{
				for(ApplVar.OldPlu = SORT[ApplVar.errplu] ; ApplVar.OldPlu < ApplVar.nmax ; ++ApplVar.OldPlu)
				{
					RWRand(2, ApplVar.OldPlu, SysBuf);
					for(ApplVar.NewPlu = ApplVar.OldPlu-SORT[ApplVar.errplu], RWRand(1, ApplVar.NewPlu, rnew) ;
						CompareBCDValue(SysBuf, rnew, ApplVar.AP.Plu.RandomSize) < 0 ; )
					{
						RWRand(2, ApplVar.NewPlu, rnew) ;
						RWRand(0, ApplVar.NewPlu+SORT[ApplVar.errplu], rnew) ;
						ApplVar.NewPlu -= SORT[ApplVar.errplu] ;
						if(ApplVar.NewPlu < 0)
							break ;
						RWRand(1, ApplVar.NewPlu, rnew) ;

					}
					RWRand(0, ApplVar.NewPlu+SORT[ApplVar.errplu], SysBuf) ;
				}
			}
		}
	}
	ApplVar.nmax = ApplVar.AP.Plu.RNumber;
	MemSet(SysBuf, 24, ' ');
	CopyFrStr(SysBuf, Msg[SETPLU].str);
	WORDtoASC(SysBuf + 10, ApplVar.nmax);
	RJPrint(2, SysBuf);
	ClearEntry(true);
}

#if defined(DEBUGBYPC)
long TestLMM(WORD lmmPage)
{
	return 0;
}
#endif



//-----------------------------------------------------------------------------------
void FormPrtConfInf(unsigned long n, unsigned long size, CONSTCHAR* str)
{
	if(n == 0 && size == 0)
		return;
	MemSet(SysBuf, PRTLEN, ' ');
	CopyFrStr(SysBuf, str);
	if(size)
	{
		ApplVar.Entry = ZERO;
		ULongToBCDValue(ApplVar.Entry.Value,size);
		FormatQty(SysBuf+PRTLEN-2,&ApplVar.Entry);
	}
	if(n)
	{
		ApplVar.Entry = ZERO;
		ULongToBCDValue(ApplVar.Entry.Value,n);
		FormatQty(SysBuf+18,&ApplVar.Entry);
	}
	SysBuf[PRTLEN] = 0;
	RJPrint(0,SysBuf);
}
//------------------------------------------------------------------------------------
void PrintConfInf()
{
	unsigned long Size,filesize;
    CONSTCHAR *sp;

	BYTE i,files;
	WORD Number;
	short n,j;

	files = 0;
	filesize = 0;
	PrintLine2('=');
	RJPrint(0, Msg[CONIFOT0].str);
	PrintLine2('=');
	for(i=0;i<AddrMAX+1;i++)
	{
		 Number = 0;
		 Size = 0;
		 switch(i)
		 {
			 case AddrTotl:
			 	sp = Msg[TJWENJ].str;
			 	Number = 0;
			 	Size = ApplVar.AP.StartAddress[AddrGroup]-ApplVar.AP.StartAddress[AddrTotl];
			 	break;
			 case AddrGroup:
			 	sp = Msg[SETGROUP].str;
			 	Number = ApplVar.AP.Group.Number;
			 	Size = ApplVar.AP.StartAddress[AddrDept] - ApplVar.AP.StartAddress[AddrGroup];
			 	break;
			 case AddrDept:
			 	sp = Msg[SETDEPT].str;
			 	Number = ApplVar.AP.Dept.Number;
			 	Size = ApplVar.AP.StartAddress[AddrPLU] - ApplVar.AP.StartAddress[AddrDept];
			 	break;
			 case AddrPLU:
			 	 Number = ApplVar.AP.Plu.Number;
				 sp = Msg[SETPLU].str;
				 Size =(ApplVar.AP.StartAddress[AddrPort] - ApplVar.AP.StartAddress[AddrPLU]);
				 if(ApplVar.AP.Plu.RandomSize)
				 {
				 	CopyFrStr(ProgLineMes,Msg[SETPLU].str);
				 	n = strlen(Msg[SETPLU].str);
                    ApplVar.Entry=ZERO;
				 	ULongToBCDValue(ApplVar.Entry.Value,ApplVar.AP.Plu.Number);
				 	MemSet(SysBuf, sizeof(SysBuf), ' ');
				 	ProgLineMes[n++] = '(';
				 	FormatQty(SysBuf+12,&ApplVar.Entry );
				 	for (j=0;j<13;j++)
				 	{
				 		if (SysBuf[j]!=' ')
				 			ProgLineMes[n++]=SysBuf[j];
				 	}
				 	ProgLineMes[n++] = ')';
				 	ProgLineMes[n] = 0;
				 	Number = ApplVar.AP.Plu.RNumber;
					sp = ProgLineMes;
				 }
			 	break;
			 case AddrTend:
				sp = Msg[SETTEND].str;
				Number = ApplVar.AP.Tend.Number;
				Size = ApplVar.AP.StartAddress[AddrPoRa] - ApplVar.AP.StartAddress[AddrTend];
				break;
			 case AddrPoRa:
			 	sp = Msg[SETPORA].str;
			 	Number = ApplVar.AP.PoRa.Number;
			 	Size = ApplVar.AP.StartAddress[AddrDrawer] - ApplVar.AP.StartAddress[AddrPoRa];
			 	break;
			 case AddrDrawer:
			 	sp = Msg[SETDRAWER].str;
			 	Number = ApplVar.AP.Draw.Number;
			 	Size = ApplVar.AP.StartAddress[AddrCorr] - ApplVar.AP.StartAddress[AddrDrawer];
			 	break;
			 case AddrCorr:
			 	sp = Msg[SETCORR].str;
			 	Number = ApplVar.AP.Correc.Number;
			 	Size = ApplVar.AP.StartAddress[AddrDisc] - ApplVar.AP.StartAddress[AddrCorr];
			 	break;
			 case AddrDisc:
			 	sp = Msg[SETDISC].str;
			 	Number = ApplVar.AP.Disc.Number;
			 	Size = ApplVar.AP.StartAddress[AddrCurr] - ApplVar.AP.StartAddress[AddrDisc];
			 	break;
			 case AddrCurr:
			 	sp = Msg[SETCURR].str;
			 	Number = ApplVar.AP.Curr.Number;
			 	Size = ApplVar.AP.StartAddress[AddrTax] - ApplVar.AP.StartAddress[AddrCurr];
			 	break;
			 case AddrTax:
			 	sp = Msg[JISHUI].str;
			 	Number = ApplVar.AP.Tax.Number;
			 	Size = ApplVar.AP.StartAddress[AddrPBf] - ApplVar.AP.StartAddress[AddrTax];
			 	break;
			 case AddrPBf:
			 	sp = Msg[SETPBF].str;
			 	Number = ApplVar.AP.Pb.Number;
			 	Size = ApplVar.AP.StartAddress[AddrModi] - ApplVar.AP.StartAddress[AddrPBf];
			 	break;
			 case AddrModi:
			 	sp = Msg[SETMODIF].str;
			 	Number = ApplVar.AP.Modi.Number;
			 	Size = ApplVar.AP.StartAddress[AddrClerk] - ApplVar.AP.StartAddress[AddrModi];
			 	break;
			 case AddrClerk:
			 	sp = Msg[SETCLERK].str;
			 	Number = ApplVar.AP.Clerk.Number;
			 	Size = ApplVar.AP.StartAddress[AddrOFF] - ApplVar.AP.StartAddress[AddrClerk];
			 	break;
			 case AddrOFF:
			 	sp = Msg[SETOFF].str;
			 	Number = ApplVar.AP.OFFPrice.Number;
			 	Size = ApplVar.AP.StartAddress[AddrICBlock] - ApplVar.AP.StartAddress[AddrOFF];  //Edit By Ranjer.Hu 2004-07-01
			 	break;
			 case AddrSalPer:
			 	sp = Msg[SETSALER].str;
			 	Number = ApplVar.AP.SalPer.Number;
			 	Size = ApplVar.AP.StartAddress[AddrAgree] - ApplVar.AP.StartAddress[AddrSalPer];
			 	break;
//			 case AddrAgree:
//			 	sp = Msg[SETAGREE].str;
//			 	Number = ApplVar.AP.Agree.Number;
//			 	Size = ApplVar.AP.StartAddress[AddrPBt] - ApplVar.AP.StartAddress[AddrAgree];
//			 	break;
//			 case AddrRPLU:
//			 	break;
			 case AddrPBt:
			 	sp = Msg[SETPBINF].str;
			 	Number = ApplVar.AP.Pb.NumberOfPb;
			 	Size = ApplVar.AP.StartAddress[AddrTrack] - ApplVar.AP.StartAddress[AddrPBt];
			 	break;
			 case AddrTrack:
			 	sp = Msg[KAITAI].str;
			 	Number = ApplVar.AP.Pb.NumberOfBlocks;
			 	Size = ApplVar.AP.StartAddress[AddrEndP] - ApplVar.AP.StartAddress[AddrTrack];
			 	break;
			 case AddrMAX:
			 	sp = Msg[LIUSKJ].str;
			 	Number = 0;
			 	Size = ApplVar.FlowHeader.MAXNUMB;
			 	break;
			 case AddrICBlock://ccr chipcard
			 	sp = Msg[GUASHIIC].str;
			 	Number = ApplVar.AP.ICBlock.Number;
			 	Size = ApplVar.AP.StartAddress[AddrSalPer] - ApplVar.AP.StartAddress[AddrICBlock];
			 	break;
            default:
                continue;
		 }
		 if (Size)
 	 	 {
		 	files++;
			filesize+=Size;
	 	 }
	     FormPrtConfInf(Number, Size, sp);
	}
    //FormPrtConfInf(0, sizeof(ApplVar), MESSRAMVAR);filesize+=sizeof(ApplVar);//变量空间

	PrintLine2('-');
//ccr2016-03-21	FormPrtConfInf(0, files, Msg[WENJSHU].str);
	FormPrtConfInf(0, filesize, Msg[WENJKJ].str);
	FormPrtConfInf(0, ApplVar.SIZE_EXTRAM, Msg[RAMKB].str);
	PrintLine2('=');
	RJPrint(0, Msg[CONIFOT1].str);
	PrintLine2('=');

//cc 20070528>>>>>>>>>>>>>>>>>
#if(CASE_MACRO5150==1)
	Number=0;
#else
	for(Number=0;Number<2;Number++)
#endif
	{
        n = ApplVar.PortList[Number].Type - 0x31;
		MemSet(SysBuf, sizeof(SysBuf), ' ');

        j = strlen(ApplVar.PortList[Number].Name);
		memcpy(SysBuf, ApplVar.PortList[Number].Name, j);
		SysBuf[j] = Number + 0x31;
		CopyFrStr(SysBuf + 8,PortType[n]);

        DeCodeProto(ApplVar.PortList[Number].Prot[n], SysBuf + 20);

		SysBuf[PRTLEN] = 0;
		RJPrint(0, SysBuf);
	}
	PrintLine2('=');
	PrintVersion();
	RJPrint(0,cCurrentTimeN1);		//“当前时间”
	Bios_1(BiosCmd_GetDate);
	Bios_1(BiosCmd_GetTime);
	memset(SysBuf,' ',PRTLEN);
	DateTimeToStr(SysBuf,3);
	SysBuf[PRTLEN] = 0;
	RJPrint(0,SysBuf);
}
