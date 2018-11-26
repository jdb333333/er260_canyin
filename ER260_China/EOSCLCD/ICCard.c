#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "flashb.h"
#if (DD_CHIPC==1 && CASE_MFRIC==1)
    #include "MFRCard.h"
    #include "MFRCard.c"
#endif

#if (DD_CHIPC==1)
//---------------------------------------------------------------------------
#if (DD_4442IC==0 || CASE_MFRIC==1)
CONST char EUTRON[7]="EUTRON";
#else
CONST char EUTRON[9]="EUTRONHN";
#endif

//----------------------------------------------------------------------------
CONST BYTE TTbPswd[10][3]={// Table of passwords
    {0xE9,0x18,0x82},// 0
    {0x29,0xB1,0x40},// 1
    {0xAB,0x5D,0x26},// 2
    {0x7F,0xC7,0x64},// 3
    {0xAA,0x13,0x86},// 4
    {0x64,0x2F,0x17},// 5
    {0xF3,0x7A,0x1A},// 6
    {0x1C,0x2C,0x75},// 7
    {0x5A,0x3A,0xCB},// 8
    {0x43,0xD5,0xB9}// 9
};
//----------------------------------------------------------------------------
CONST BYTE TbCript[40]={// Table of codified areas decodification
    0x0D,0x79,0xAB,0xBA,0x96,0xCE,0xF7,0x89,0xF6,0xA1,
    0x79,0xDA,0x72,0x24,0xD3,0xA7,0x06,0x32,0xAB,0x12,
    0x30,0x01,0x1F,0x53,0xB3,0x52,0xEA,0x51,0x04,0x92,
    0xD4,0x17,0x4A,0x9B,0x05,0x5C,0x59,0x85,0x54,0x97
};
//------------------------------------------------------------------------------
#if (CASE_MFRIC==1)
    #define  SCC_Ver() (1)
#else
    #define  SCC_Ver() CC_VerifyPWD(ApplVar.IC.CHIP_PSWD)
#endif
#define  SCC_NewPwd(pPwd) CC_WritePWD(pPwd)
#define ReadFLAGS() CC_Read(ApplVar.IC.REC_Flags,ICR_FLAGS,sizeof(ApplVar.IC.REC_Flags))
#define ReadCUSTOMER() CC_Read(ApplVar.IC.REC_Customer,ICR_CLI,sizeof(ApplVar.IC.REC_Customer))
#define ReadCHARGE() CC_Read(ApplVar.IC.REC_VALATT,ICR_VALATT,sizeof(ApplVar.IC.REC_VALATT));//*2) //
#define ReadInit() CC_Read(ApplVar.IC.REC_INIT,ICR_INIT,28)
//------------------------------------------------------------------------------
void PrintChipCard(BYTE pSel);
void PayByChipCard(BCD *pValue,BCD *pIC_AMT,BYTE pay);
BYTE ChipCard();
short ReadFactory();         //OK
char RD_ChipCard();
short Clear_CHIP();
signed char Initial_CHIP();
short Charge_CHIP();
void ProgClearChip();
void ProgChargeChip();
WORD EncodeDateSelf(char *frDate);
BYTE SCC_Ccrc(BYTE *pHL,short pB);
short SCC_Cript(BYTE pDE[],short pC); //OK
void SCC_SetPswd(short pType);
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// 读卡号，并比较。用来判断操作过程中，IC卡是否换过 //
// ccr050316
BYTE CheckCardNo()
{

    short sLp,sInt;
    short sOk;
    UnLong sCardNo;
#if(CASE_MFRIC==1)
	BYTE sREC_Factory[MFR_DSCAD+4]; 	//16 bytes

	sOk = CC_ReadMFR(sREC_Factory,RW_Factory);
#else
	BYTE sREC_Factory[CC_FACTORY];	//38 bytes

   sOk = CC_Read(sREC_Factory,ICR_FACT,sizeof(sREC_Factory));
#endif
    if (sOk)
    {

#if(CASE_MFRIC==1)
		for (sLp=0;sLp<4;sLp++)
			sREC_Factory[MFRCC_SERIE+sLp] = sREC_Factory[MFRCC_SERIE+sLp] ^ TbCript[sLp*sLp+10];
		memcpy	(&sCardNo,&sREC_Factory[MFRCC_SERIE],sizeof(sCardNo));
#elif(DD_4442IC == 1)
		for (sLp=0;sLp<4;sLp++)
			sREC_Factory[CC_SERIE+sLp] = sREC_Factory[CC_SERIE+sLp] ^ TbCript[sLp*sLp+10];
		memcpy(&sCardNo,&sREC_Factory[CC_SERIE],sizeof(sCardNo));
#else//4428
		memcpy(&sCardNo,&sREC_Factory[CC_SERIE],sizeof(sCardNo));
#endif
        sOk = (sCardNo==ApplVar.IC.CardNo);
    }
    return sOk;
}

//------------------------------------------------------------------------------
// 检测是否插入，读取IC卡并检测数据的正确性   //

BYTE ChipCard()
{
    BYTE sBuf[6];
    WORD sPINH;
    short i;

#if(CASE_MFRIC==1)
    if(TESTBIT(ApplVar.ICCardSet.Options,IC_CHIPCARD) && CC_MFRCard())
    {
        if(!TESTBIT(ApplVar.IC.ICState,IC_INSERT))//MFR卡的当前状态为未放置
        {
            SETBIT(ApplVar.IC.ICState,IC_INSERT);//标示MFRIC放置好
#else
	if (TESTBIT(ApplVar.ICCardSet.Options,IC_CHIPCARD) && CC_Insert())
	{
		if (!TESTBIT(ApplVar.IC.ICState,IC_INSERT) && CC_Open(sBuf))
		{
			Delay(10);
#endif
            ECRVIP.ECRVIPState=0;//ccr2016-05-23使用IC卡时,禁止ECRVIP
            ApplVar.ErrorNumber=0;
            ApplVar.IC.CHIP_Flag = RD_ChipCard();

            RESETBIT(ApplVar.IC.ICState,IC_NEWCARD);
            switch (ApplVar.IC.CHIP_Flag)
            {
            default://其他未知错误
                ApplVar.IC.CHIP_Flag = IC_ILLERR;
            case  IC_ILLERR ://读卡出错
#if(CASE_MFRIC==1)
                if(!CC_MFRCard())
                {//读卡时,卡移除了
                    RESETBIT(ApplVar.IC.ICState,IC_INSERT);//标示MFRIC未放置好
                    return false;
                }
#endif
                ApplVar.ErrorNumber=ERROR_ID(CWXXI50);
                break;
            case  IC_POSERR :
                ApplVar.ErrorNumber=ERROR_ID(CWXXI51);
                break;
            case  IC_DATAERR:
                ApplVar.ErrorNumber=ERROR_ID(CWXXI52);
                break;
            case  IC_DATEERR:
                ApplVar.ErrorNumber=ERROR_ID(CWXXI53);
                break;
            case  IC_CANERR :
                ApplVar.ErrorNumber=ERROR_ID(CWXXI54);
                break;
            case  IC_NEWERR :
                ApplVar.ErrorNumber=ERROR_ID(CWXXI56);
                break;
/*
                case  IC_CUSTERR:
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI55);
                    break;
                case  IC_TYPEERR:
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI57);
                    break;
                case  IC_WRITERR:
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI58);
                    break;
                case  IC_NUMERR	:
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI59);
                    break;
*/
            case  IC_TYPE0  :
                if (!TESTBIT(ApplVar.ICCardSet.Options, IC_TYPE_0))
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI60);
                break;
            case  IC_TYPE1  :
                if (!TESTBIT(ApplVar.ICCardSet.Options, IC_TYPE_1))
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI61);
                    break;
                }
            case  IC_TYPE2  :
                if (ApplVar.IC.CHIP_Flag==IC_TYPE2 && !TESTBIT(ApplVar.ICCardSet.Options, IC_TYPE_2))
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI62);
                }
                else
#if(CASE_MFRIC==1)
				if(ApplVar.IC.REC_INIT[CC_FTIPO] && (ApplVar.IC.REC_INIT[CC_PINL] || ApplVar.IC.REC_INIT[CC_PINH]) && TESTBIT(ApplVar.IC.ICState,IC_INSERT))
#else
                if (ApplVar.IC.REC_INIT[CC_FTIPO] && (ApplVar.IC.REC_INIT[CC_PINL] || ApplVar.IC.REC_INIT[CC_PINH]) && !TESTBIT(ApplVar.IC.ICState,IC_NOTREMOVED))
#endif
                {//校验卡的密码(PIN)
#if (SCRSTATDBL)
                    DispSt0C(Msg[MIMA].str);//RG-DISPLAY
#else
                    ClsState2();
                    DispSt1C(Msg[MIMA].str);//RG-DISPLAY
#endif
                    GotoXY(SCREENWD-1,STATELINE);
                    memset(sBuf,0,sizeof(sBuf));
                    SETBIT(ApplVar.MyFlags,PWDMODE);
                    ApplVar.CentralLock |= 0x100;
                    if (GetStringCHN(sBuf,4,false)>0)//Get PIN //RG-DISPLAY
                    {//输入了密码
                        sPINH = (sBuf[0] - '0')*10 + (sBuf[1] - '0');
                        sPINH = (sPINH * 100)+(sBuf[2] - '0')*10 + (sBuf[3] - '0');
#if(CASE_MFRIC==1)
                        if ((sPINH>>8) != ApplVar.IC.REC_INIT[CC_PINH] || (sPINH & 0xff) !=ApplVar.IC.REC_INIT[CC_PINL]
							 || !CheckCardNo())
						{//密码错
							RESETBIT(ApplVar.IC.ICState,IC_INSERT);
                            ApplVar.IC.CHIP_Flag = IC_ILLERR;
                            ApplVar.ErrorNumber=ERROR_ID(CWXXI36);//error PIN
                        }
#else
                       if ((sPINH>>8) != ApplVar.IC.REC_INIT[CC_PINH] || (sPINH & 0xff) !=ApplVar.IC.REC_INIT[CC_PINL]
                            || !CC_Insert() || !CheckCardNo())//ccr050316
                        {//密码错
                            RESETBIT(ApplVar.IC.ICState,IC_NOTREMOVED);

                            ApplVar.IC.CHIP_Flag = IC_ILLERR;
                            ApplVar.ErrorNumber=ERROR_ID(CWXXI36);//error PIN
                        }
#endif
                    }
                    else
                    {//没有输入密码
#if(CASE_MFRIC==1)
						RESETBIT(ApplVar.IC.ICState,IC_INSERT);
#else
                        RESETBIT(ApplVar.IC.ICState,IC_NOTREMOVED);
#endif
                        ApplVar.IC.CHIP_Flag = IC_ILLERR;
                        ApplVar.ErrorNumber=ERROR_ID(CWXXI36);//error PIN
                    }
                    ApplVar.CentralLock &= 0xfeff;
                    ClsState12();
                    RESETBIT(ApplVar.MyFlags,PWDMODE);
                }
                break;
            }
//			if (ApplVar.ErrorNumber ==0 && ApplVar.IC.CHIP_Flag>=0)
//				Collect_Data(INSERTICLOG);
#if(CASE_MFRIC==1)
			//if(Appl_ProgType)
            //     ApplVar.ErrorNumber = 0;
			if (ApplVar.ErrorNumber && ApplVar.ErrorNumber!=ERROR_ID(CWXXI56))
                RESETBIT(ApplVar.IC.ICState,IC_INSERT);
#else
			if (!ApplVar.ErrorNumber)
				SETBIT(ApplVar.IC.ICState,IC_NOTREMOVED);
#endif
            return TRUE;
        }
#if (CASE_MFRIC==0)
        else
            return false;
#endif
    }
    else
    {//卡不存在或者被拔出
        ApplVar.IC.CHIP_Flag = IC_ILLERR;
#if (CASE_MFRIC==0)
        if (TESTBIT(ApplVar.IC.ICState,IC_INSERT))
            CC_Close();
#endif
        RESETBIT(ApplVar.IC.ICState,IC_INSERT | IC_NOTREMOVED);
    }
    return false;
}
//------------------------------------------------------------------------------
char RD_ChipCard()

// It reads the chip-card, Supposes that the chip-card is inserted.
//
// ATTENTION : IT MUST BE PATCHED FOR THE POWER FAIL PROBLEM
//
// NOTE : It reads the chip-card and NOT verifies the password.
//        The password verification must be performed by the routine of
//        more high level during the PAYMENT, CHARGING, CREATION phases !!!
//
// It verifies the password re-calculating the password to applied
// in operation of chip-card type, of heading data preceding readed
// and of the eventual PIN code entered by the keyboard.
//
// Input  :
//        pDispTo:0-Skip display
//               :1-display data on GroupBoxNew;
//               :2-display data on GroupBoxVerify;
// Result<0---> NOT OK ,error code:
//Ccr "卡类型错!"          //-1
//Ccr "POS代码错!"         //-2
//Ccr "卡数据错!"          //-3
//Ccr "为过期卡!"          //-4
//Ccr "为挂失卡!"          //-5
//Ccr "客户类别错!"        //-6
//Ccr "为新卡!"            //-7
//Ccr "不是购物卡!"        //-8
//Ccr "写卡出错!"          //-9
//Ccr "卡号不对!"          //-10
//Result>=0:---> OK ,  A=0 !
//          0		Type0 	"折扣卡:",           //
//          1		Type1	"现金卡:",           //
//          2		Type2	"赊帐卡:",           //
//          3       Type3   "考勤卡:",             // cc added 2005-03-18
//
// 		  CHIP_TIPO  = type of chip-card
// 		  CHIP_FTIPO = Flags of type of chip-card
//
{
    WORD sTmp;
	short sRet;
    char sBuf[12];

//ccr090310>>>>>>>>>>>>>>>>>>>>
#if(CASE_MFRIC==1)	//cc 20070717
	memset(&ApplVar.IC,0,	sizeof(ApplVar.IC.REC_Factory)+
		sizeof(ApplVar.IC.REC_Flags)+
		sizeof(ApplVar.IC.REC_INIT)+
		sizeof(ApplVar.IC.REC_Customer)+
		sizeof(ApplVar.IC.REC_VALATT) +

		sizeof(ApplVar.IC.MFR_Factory) +
		sizeof(ApplVar.IC.MFR_Facpre) +
		sizeof(ApplVar.IC.MFR_INIT) +
		sizeof(ApplVar.IC.MFR_Customer) +
		sizeof(ApplVar.IC.MFR_VALATT) +
		sizeof(ApplVar.IC.MFR_VALPRE));//*2);
#else
	memset(&ApplVar.IC,0,	sizeof(ApplVar.IC.REC_Factory)+
		sizeof(ApplVar.IC.REC_Flags)+
		sizeof(ApplVar.IC.REC_INIT)+
		sizeof(ApplVar.IC.REC_Customer)+
		sizeof(ApplVar.IC.REC_VALATT));//+
#endif
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    SETBIT(ApplVar.IC.ICState,IC_INSERT);


    //test of application recognition pattern（read the "FACTORY" area）
#if(CASE_MFRIC==1)
	if((sRet = ReadFactory())==0 || sRet==IC_NEWERR)
	{
		if (!sRet)
			return IC_ILLERR;
		else
		{
			CC_ReadMFR(ApplVar.IC.MFR_INIT,RW_Init);
			CC_ReadMFR(ApplVar.IC.MFR_Customer,RW_Customer);
			return IC_NEWERR;
		}
	}
	if(!CC_ReadMFR(ApplVar.IC.MFR_INIT,RW_Init) || !CC_ReadMFR(ApplVar.IC.MFR_Customer,RW_Customer) ||
	   !CC_ReadMFR(ApplVar.IC.MFR_VALATT,RW_Valatt) || !CC_ReadMFR(ApplVar.IC.MFR_VALPRE,RW_Valpre))
	{
		return IC_ILLERR;
	}
	else
		MFRToIC(RW_Flags);
#else
    if (!ReadFactory() ||
        !CC_Read(ApplVar.IC.REC_Flags, ICR_FLAGS, sizeof(ApplVar.IC.REC_Flags)+sizeof(ApplVar.IC.REC_INIT)+sizeof(ApplVar.IC.REC_Customer)+sizeof(ApplVar.IC.REC_VALATT)*2))
    {
        return IC_ILLERR;
    }
#endif

    if (QueryICBlock(ApplVar.IC.CardNo))
    {
        return IC_CANERR;
    }

    //test if the chip-card has been initialized
#if(CASE_MFRIC==1)
	if(ApplVar.IC.REC_Flags[CC_AGGINIZ] == 0x55)
#else
    if ((ApplVar.IC.REC_Flags[CC_VERG1]==0xff)&&(ApplVar.IC.REC_Flags[CC_VERG1+1]==0xff)||ApplVar.IC.REC_Flags[IC_AGGINIZ] == 0x55)
#endif
    {
        return IC_NEWERR;
    }

    ApplVar.IC.CriptCnt=ApplVar.IC.CardNo%10;
    //decodify the 'INIT' record

#if(CASE_MFRIC==1)
	if(!MFRToIC(RW_Init))
	{
		return IC_DATAERR;
	}
#else
    SCC_Cript(ApplVar.IC.REC_INIT,28); //Decode of INIT record
    if (ApplVar.IC.REC_INIT[CC_CRCINIT]!=SCC_Ccrc(ApplVar.IC.REC_INIT,sizeof(ApplVar.IC.REC_INIT)-1))
    {
        return IC_DATAERR;
    }
#endif
    if (ApplVar.IC.REC_INIT[CC_TIPO]==TIPO_OPE)
    {
        return IC_TYPEERR; //Operator/Supervisor type
    }
    else
    {
        if (ApplVar.IC.REC_INIT[CC_TIPO]>TIPO_CLIC) //Is it "Customer" (0,1, or 2) type ?
        {
            return IC_CUSTERR;
        }

        ApplVar.IC.PosCode=CLONG(ApplVar.IC.REC_INIT[CC_CODPOS]);
        //memcpy(&ApplVar.IC.PosCode,&ApplVar.IC.REC_INIT[CC_CODPOS],4);

        ApplVar.IC.CHIP_TIPO=ApplVar.IC.REC_INIT[CC_TIPO];
        ApplVar.IC.CHIP_FTIPO=ApplVar.IC.REC_INIT[CC_FTIPO];
        ApplVar.IC.CHIP_PROT=ApplVar.IC.CHIP_FTIPO&3; //Set the protection type

        SCC_SetPswd(0);
        //read and decodify the customer data
#if(CASE_MFRIC==1)
		if(!MFRToIC(RW_Customer))
		{
			return IC_DATAERR;
		}
#else
        SCC_Cript(ApplVar.IC.REC_Customer,64); //Decode of Customer Area
#endif

        if (ApplVar.IC.PosCode!=ApplVar.ICCardSet.PosCode)
        {
            return IC_POSERR;
        }

        if ((ApplVar.IC.REC_INIT[CC_FTIPO] & (1 << FLT0_Scad)) && TESTBIT(ApplVar.ICCardSet.Options,IC_DEAD))
        {
            memset(sBuf,' ',10);
            HEXtoASC(sBuf,(char *)&Now.year,2);
            HEXtoASC(sBuf+5,&Now.month,1);
            HEXtoASC(sBuf+8,&Now.day,1);//2005-02-04
            sTmp = EncodeDateSelf(sBuf);
            if (sTmp>*((WORD *)(ApplVar.IC.REC_Flags+ CC_DSCAD)))
            {
                return IC_DATEERR;
            }
        }
#if(CASE_MFRIC==1)
		// MFRToIC对金额数据进行验证并恢复和转储 //
		if(!MFRToIC(RW_Valatt))
		{
			return IC_DATAERR;
		}
#else
        if (ApplVar.IC.REC_Customer[CC_CRCCLI]!=SCC_Ccrc(ApplVar.IC.REC_Customer,sizeof(ApplVar.IC.REC_Customer)-1))
        {
            return IC_DATAERR;
        }
        //read and decodify the 'CHARGE-VALUE' data
        SCC_Cript(ApplVar.IC.REC_VALATT,sizeof(ApplVar.IC.REC_VALATT));//*2);

        if (SCC_Ccrc(ApplVar.IC.REC_VALATT,sizeof(ApplVar.IC.REC_VALATT)-2)!=ApplVar.IC.REC_VALATT[CC_CRCVAL])//checksum of charge-value field
        {
            return IC_DATAERR;
        }
#endif
//                if(SCC_Ccrc(ApplVar.IC.REC_VALPRE,sizeof(ApplVar.IC.REC_VALPRE)-2)!=ApplVar.IC.REC_VALPRE[CC_CRCVAL])//checksum of charge-value field
//                    return IC_DATAERR;
        if (ApplVar.IC.REC_Flags[CC_AGGINIZ] == 0x55)
        {
            return IC_DATAERR;
        }
    }
    return ApplVar.IC.CHIP_TIPO;
}



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void icBCD2EcrBCD(BCD *ecrBCDTo,char *icBCDFr,BYTE pLenFr)
{
    short sLp,sLen;

    memset(ecrBCDTo,0,sizeof(BCD));
    sLen = pLenFr-1;

    for (sLp=0;sLp<pLenFr;sLp++)
    {
        ecrBCDTo->Value[sLp] = icBCDFr[sLen];
        sLen--;
    }
}

//------------------------------------------------------------------------------
void EcrBCD2icBCD(char *icBCDTo,BCD *ecrBCDFr,BYTE pLenTo)
{
    short sLp,sLen;

    memset(icBCDTo,0,pLenTo);
    sLen = pLenTo-1;

    for (sLp=0;sLp<pLenTo;sLp++)
    {
        icBCDTo[sLen] = ecrBCDFr->Value[sLp];
        sLen--;
    }
}

//------------------------------------------------------------------------------
void DecodeDateSelf(WORD pD0,char toDate[])
/*
; 01/01/92 (1992) <--- 0000000-0001-00001  = 0021 : min value
; 31/12/91 (2091) <--- 1100011-1100-11111  = C79F : max value
; toDate ="2003-01-01"
*/
{
    WORD  sYear, sDay, sMonth;
    memset(toDate,' ',10);

    sYear = (pD0>>9) + 1992;
    sMonth = (pD0>>5) & 0x0f;
    if (sMonth<1 || sMonth>12)
        sMonth = 1;
    sDay = pD0 & 0x1F;
    if (sDay<1 || sDay>31)
        sDay=1;
    WORDtoASC(toDate+3, sYear);
    WORDtoASC(toDate+6, sMonth);
    WORDtoASC(toDate+9, sDay);
    if (toDate[5]==' ')
        toDate[5]='0';
    if (toDate[8]==' ')
        toDate[8]='0';
    toDate[4]=toDate[7]='-';
    toDate[10] = 0;
}


WORD EncodeDateSelf(char *frDate)
/*
; It converts a date from the TDate format in a binary number of two bytes.
; The year is denoted by only the last two digit. (0..99)
;
; NOTE : the packing starts from the date 01/01/92
;
; The resultant number is so greater as greater is the date
;
; 01/01/92 (1992) --> 0000000-0001-00001  = 0021 : min value
; 31/12/91 (2091) --> 1100011-1100-11111  = C79F : max value
;
; Input : frDate="2001-01-01"
; Output: BC = value calculated
*/
{

    WORD sYear, sDay, sMonth;

    sYear = (frDate[0] & 0x0f)*1000 + (frDate[1] & 0x0f)*100+(frDate[2] & 0x0f)*10+(frDate[3] & 0x0f) - 1992;
    sMonth = (frDate[5] & 0x0f)*10+(frDate[6] & 0x0f);
    sDay = (frDate[8] & 0x0f)*10+(frDate[9] & 0x0f);;
    return(sYear<<9) + (sMonth<<5) + sDay;
}

//------------------------------------------------------------------------------
BYTE SCC_Ccrc(BYTE *pHL,short pB)  //OK
//; Subroutine SCC_Ccrc
//;
//; It calculates the checksum to control a certain area.
//;
//; Input : HL -> source area
//;          B =  size of area
//; Result :  CRC calculated (16 bits)
//;
{
    short sLp ;
    BYTE sSum;
    sSum=0;
    for (sLp=0;sLp<pB;sLp++)
        sSum=sSum+pHL[sLp];
    sSum++;
    return sSum;
}
//------------------------------------------------------------------------------
short SCC_Cript(BYTE pDE[],short pC) //OK
//; Subroutine SCC_Cript
//;
//; It codifies / decodifies a certain area
//;
//; Input : DE -> area in ram to codify / decodify
//;          C =  #bytes (1..N)
//;
//;         CriptCnt = current counter on codify table
{
    short sLp,sBX;
    sBX=ApplVar.IC.CriptCnt;
    for (sLp=0;sLp<pC;sLp++)
    {
        pDE[sLp]=pDE[sLp]^TbCript[sBX];
        sBX=(sBX+1)%40;
    }
    ApplVar.IC.CriptCnt=sBX;
    return 1;
}
//------------------------------------------------------------------------------
void SCC_SetPswd(short pType)     //OK
/*{; Subroutine SCC_SetPswd
; Set the current password in operation of data readed from the chip-card,
; and store it into the CHIP_PSWD buffer.
;
; If CHIP_PROT=1 or 2 (protec. level)--> CHIP_PIN must contains the PIN-code !!!
;}*/
{
    short sLp,sInt;
    sInt=ApplVar.IC.CardNo%10;
    for (sLp=0;sLp<=2;sLp++)
        ApplVar.IC.CHIP_PSWD[sLp]=TTbPswd[sInt][sLp];
    if ((ApplVar.IC.CHIP_PROT>0)&&(pType==0))
    {
        sInt=(ApplVar.IC.CHIP_PSWD[0]+((short)ApplVar.IC.CHIP_PSWD[1]<<8))
             +(ApplVar.IC.REC_INIT[CC_PINL]+((short)ApplVar.IC.REC_INIT[CC_PINH]<<8));
        ApplVar.IC.CHIP_PSWD[0]=sInt&0xff;
        ApplVar.IC.CHIP_PSWD[1]=(sInt>>8)&0xff;
    }
}
//------------------------------------------------------------------------------
short ReadFactory()         //OK
{
    short sLp,sInt,sFlag;
    short sOk,sNew=0;
#if(CASE_MFRIC==1)
    BYTE CRCFactory,CRCFacpre;//ccr2014-04-03

	sOk = CC_ReadMFR(ApplVar.IC.MFR_Factory,RW_Factory) | CC_ReadMFR(ApplVar.IC.MFR_Facpre,RW_Facpre);

	if(sOk)		//cc 20070825
	{
        //ccr2014-04-03>>>>>>>>
		if (ApplVar.IC.MFR_Factory[MFRCC_AGGINIZ] == 0x55)//ccr090311
			sNew = IC_NEWERR;
        else
        {
            CRCFactory = SCC_Ccrc(ApplVar.IC.MFR_Factory,sizeof(ApplVar.IC.MFR_Factory)-1);
            CRCFacpre = SCC_Ccrc(ApplVar.IC.MFR_Facpre,sizeof(ApplVar.IC.MFR_Facpre)-1);
            if(ApplVar.IC.MFR_Factory[MFRCC_CRCFACT] != 0xff &&
                ApplVar.IC.MFR_Factory[MFRCC_CRCFACT] != CRCFactory)
            {// 如果Factory被破坏,使用Facpre中的数据进行恢复 //
                if (ApplVar.IC.MFR_Facpre[MFRCC_CRCFACT] == CRCFacpre)
                {
                    memcpy(ApplVar.IC.MFR_Factory,ApplVar.IC.MFR_Facpre,sizeof(ApplVar.IC.MFR_Facpre));
                    sOk = CC_WriteMFR(ApplVar.IC.MFR_Factory,RW_Factory);
                }
                else
                    sOk = 0;
            }
            else if(ApplVar.IC.MFR_Facpre[MFRCC_CRCFACT] != 0xff &&
                    ApplVar.IC.MFR_Facpre[MFRCC_CRCFACT] != CRCFacpre)
            {// 如果Facpre被破坏,使用Factory中的数据进行恢复 //
                if (ApplVar.IC.MFR_Factory[MFRCC_CRCFACT] == CRCFactory)
                {
                    memcpy(ApplVar.IC.MFR_Facpre,ApplVar.IC.MFR_Factory,sizeof(ApplVar.IC.MFR_Facpre));
                    sOk = CC_WriteMFR(ApplVar.IC.MFR_Facpre,RW_Facpre);
                }
                else
                    sOk = 0;
            }else  if(ApplVar.IC.MFR_Facpre[MFRCC_CRCFACT] != 0xff && memcmp(ApplVar.IC.MFR_Factory,ApplVar.IC.MFR_Facpre,sizeof(ApplVar.IC.MFR_Facpre)))
            {// 如果Factory被破坏,使用Facpre中的数据进行恢复 //
                if (ApplVar.IC.MFR_Facpre[MFRCC_CRCFACT] == CRCFacpre)
                {
                    memcpy(ApplVar.IC.MFR_Factory,ApplVar.IC.MFR_Facpre,sizeof(ApplVar.IC.MFR_Facpre));
                    sOk = CC_WriteMFR(ApplVar.IC.MFR_Factory,RW_Factory);
                }
                else
                    sOk = 0;
            }
        }
        if (sOk)
            MFRToIC(RW_Factory);
        //ccr2014-04-03<<<<<<<<<<<<<<<
	}
#else
    sOk=CC_Read(ApplVar.IC.REC_Factory,ICR_FACT,sizeof(ApplVar.IC.REC_Factory));
#endif
    if (sOk)
    {

        ApplVar.IC.CardNo=0;
#if(DD_4442IC==1 || CASE_MFRIC==1)
        for (sLp=0;sLp<4;sLp++)
            ApplVar.IC.REC_Factory[CC_SERIE+sLp] = ApplVar.IC.REC_Factory[CC_SERIE+sLp] ^ TbCript[sLp*sLp+10];
#endif
        ApplVar.IC.CardNo=CLONG(ApplVar.IC.REC_Factory[CC_SERIE]);
        //memcpy(&ApplVar.IC.CardNo,&ApplVar.IC.REC_Factory[CC_SERIE],4);

        sInt = ApplVar.IC.CardNo % 40;

#if(DD_4442IC==1 || CASE_MFRIC==1)
        for (sLp=0;sLp<sizeof(EUTRON)-1;sLp++)
        {
            ApplVar.IC.REC_Factory[sLp+CC_PATT] = ApplVar.IC.REC_Factory[sLp+CC_PATT] ^ TbCript[sInt];
            sInt=(sInt+1) % 40;
        }
#endif

        for (sLp=0;sLp<sizeof(EUTRON)-1;sLp++)
        { //verify "EUTRON"
            sOk=sOk&(ApplVar.IC.REC_Factory[sLp+CC_PATT]==EUTRON[sLp]);
            if (!sOk)
                break;
        }
    }
    //ccr2014-04-03>>>>>
    if (sOk && sNew==IC_NEWERR)
        return IC_NEWERR;
	else
    	return sOk;
    //ccr2014-04-03<<<<<
}


//---------------------------------------------------------------------------
//
//
short Clear_CHIP()
{
    BCD newValue;

#if(CASE_MFRIC==1)
	BYTE sFactory[MFR_DSCAD+4];
#endif
    if (ApplVar.IC.CHIP_Flag==IC_ILLERR)//
        return ApplVar.IC.CHIP_Flag;
    else //it must be not initialized
    {

        if (SCC_Ver())
        {
            ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_CLEAR;
            icBCD2EcrBCD(&newValue,&ApplVar.IC.REC_VALATT[CC_VAL],CC_PRGVEN-CC_VAL);
            memcpy(ApplVar.FlowBuff.CDC_refreshic.RValue,newValue.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RValue));
            icBCD2EcrBCD(&newValue,&ApplVar.IC.REC_VALATT[CC_PUNTI],CC_FORE-CC_PUNTI);
            memcpy(ApplVar.FlowBuff.CDC_refreshic.RPoint,newValue.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RPoint));
            Collect_Data(REFRESHICLOG);

#if(CASE_MFRIC==1)
			memset(ApplVar.IC.MFR_INIT,0xff,sizeof(ApplVar.IC.MFR_INIT));
			memset(ApplVar.IC.MFR_Customer,0xff,sizeof(ApplVar.IC.MFR_Customer));
			memset(ApplVar.IC.MFR_VALATT,0xff,sizeof(ApplVar.IC.MFR_VALATT));
			ApplVar.IC.MFR_Factory[MFRCC_AGGINIZ] = 0x55;
			ApplVar.IC.MFR_Factory[MFRCC_AGGVAL] = 0xff;
			memset(&ApplVar.IC.MFR_Factory[MFRCC_DSCAD],0x00,4);
			ApplVar.IC.MFR_Factory[MFRCC_CRCFACT] = SCC_Ccrc(ApplVar.IC.MFR_Factory, 15);	//cc 20070825
			if(!CC_WriteMFR(ApplVar.IC.MFR_Factory,RW_Factory) ||!CC_WriteMFR(ApplVar.IC.MFR_INIT,RW_Init) ||
		    	    !CC_WriteMFR(ApplVar.IC.MFR_Customer,RW_Customer) || !CC_WriteMFR(ApplVar.IC.MFR_VALATT,RW_Valatt) ||
		    	    !CC_WriteMFR(ApplVar.IC.MFR_Factory, RW_Facpre) || !CC_WriteMFR(ApplVar.IC.MFR_VALATT,RW_Valpre))
#else
            memset(ApplVar.IC.REC_Flags,0xff,   sizeof(ApplVar.IC.REC_Flags)+
                   sizeof(ApplVar.IC.REC_INIT)+
                   sizeof(ApplVar.IC.REC_Customer)+
                   sizeof(ApplVar.IC.REC_VALATT));//*2);

            ApplVar.IC.REC_Flags[CC_AGGINIZ] = 0x55;

            if (!CC_Write(ApplVar.IC.REC_Flags,ICR_FLAGS,sizeof(ApplVar.IC.REC_Flags)+
                          sizeof(ApplVar.IC.REC_INIT)+
                          sizeof(ApplVar.IC.REC_Customer)+
                          sizeof(ApplVar.IC.REC_VALATT)))//*2);
#endif
                return -1;

            ApplVar.IC.REC_INIT[CC_PINL] = 0; //set PIN-L (olny if Protection= 0 or 1)
            ApplVar.IC.REC_INIT[CC_PINH] = 0; //set PIN-H (olny if Protection= 0 or 1)

            SCC_SetPswd(1);
#if(CASE_MFRIC==0)
            if (!SCC_NewPwd(ApplVar.IC.CHIP_PSWD))
                return -1;

            ApplVar.IC.REC_Flags[CC_AGGINIZ] = 0xff;
            if (!CC_Write(ApplVar.IC.REC_Flags,IC_AGGINIZ,1))
                return -1;
#endif

#if(CASE_MFRIC==1)
			if(!CC_ReadMFR(sFactory,RW_Factory) || sFactory[MFRCC_AGGINIZ] != 0x55)
			{
				return -1;
			}

//			ApplVar.IC.CHIP_Flag = 1;
//			RJPrint(0,Msg[QCHICKA].str);
//			PrintChipCard(2);
			return IC_NEWERR;
#else
            ApplVar.IC.CHIP_Flag = RD_ChipCard();
            return ApplVar.IC.CHIP_Flag;
#endif
        }
        else
            return -1;
    }

}

//---------------------------------------------------------------------------

signed char Initial_CHIP()
{
#if (CASE_MFRIC==1)
    BYTE    cpPin[2],icProt;
#endif
    if (ApplVar.IC.CHIP_Flag!=IC_NEWERR)//
        return -1;
    else
    {
        SCC_SetPswd(1);

        if (SCC_Ver())
        {
            ApplVar.IC.CriptCnt = ApplVar.IC.CardNo % 10;
			ApplVar.IC.REC_Flags[CC_AGGINIZ] = 0x55;

#if(CASE_MFRIC==1)
			cpPin[0] = ApplVar.IC.REC_INIT[CC_PINL];
			cpPin[1] = ApplVar.IC.REC_INIT[CC_PINH];
			icProt = ApplVar.IC.CHIP_PROT;
			ICToMFR(RW_ALL);
			ApplVar.IC.MFR_Factory[MFRCC_CRCFACT] = SCC_Ccrc(ApplVar.IC.MFR_Factory, 15);	//cc 20070825
			if(!CC_WriteMFR(ApplVar.IC.MFR_Factory,RW_Factory) ||!CC_WriteMFR(ApplVar.IC.MFR_INIT,RW_Init) ||
	           	    !CC_WriteMFR(ApplVar.IC.MFR_Customer,RW_Customer) || !CC_WriteMFR(ApplVar.IC.MFR_VALATT,RW_Valatt))
			{
				return IC_WRITERR;
			}

			ApplVar.IC.REC_Flags[CC_AGGINIZ] = 0xff;

			ICToMFR(RW_Flags);
			ApplVar.IC.MFR_Factory[MFRCC_CRCFACT] = SCC_Ccrc(ApplVar.IC.MFR_Factory, 15);	//cc 20070825
	 		if(!CC_WriteMFR(ApplVar.IC.MFR_Factory,RW_Factory) ||
				!CC_WriteMFR(ApplVar.IC.MFR_VALATT,RW_Valpre) ||
				!CC_WriteMFR(ApplVar.IC.MFR_Factory,RW_Facpre))
			{
				return IC_WRITERR;
			}
			ApplVar.IC.CHIP_Flag = RD_ChipCard();

#else
            SCC_SetPswd(0);
            ApplVar.IC.REC_INIT[CC_CRCINIT] = SCC_Ccrc(ApplVar.IC.REC_INIT, sizeof(ApplVar.IC.REC_INIT)-1);
            SCC_Cript(ApplVar.IC.REC_INIT, sizeof(ApplVar.IC.REC_INIT));
            ApplVar.IC.REC_Customer[CC_CRCCLI] = SCC_Ccrc(ApplVar.IC.REC_Customer, sizeof(ApplVar.IC.REC_Customer)-1);
            SCC_Cript(ApplVar.IC.REC_Customer, sizeof(ApplVar.IC.REC_Customer));
            ApplVar.IC.REC_VALATT[CC_CRCVAL] = SCC_Ccrc(ApplVar.IC.REC_VALATT, sizeof(ApplVar.IC.REC_VALATT)-2);
            memcpy(ApplVar.IC.REC_VALPRE,ApplVar.IC.REC_VALATT,sizeof(ApplVar.IC.REC_VALATT));
            SCC_Cript(ApplVar.IC.REC_VALATT, sizeof(ApplVar.IC.REC_VALATT));//*2);


            ApplVar.IC.REC_Flags[CC_AGGINIZ] = 0x55;
            //ApplVar.IC.REC_Flags[CC_AGGINIZ] = 0xff;    //cc20050819
            if (!CC_Write(ApplVar.IC.REC_Flags, ICR_FLAGS, sizeof(ApplVar.IC.REC_Flags)+sizeof(ApplVar.IC.REC_INIT)+sizeof(ApplVar.IC.REC_Customer)+sizeof(ApplVar.IC.REC_VALATT)*2))
                return IC_WRITERR;

            ApplVar.IC.REC_Flags[CC_AGGINIZ] = 0xff;
            if (!CC_Write(ApplVar.IC.REC_Flags, IC_AGGINIZ, 1)) //Clear area $55
                return IC_WRITERR;


            if (SCC_NewPwd(ApplVar.IC.CHIP_PSWD))
                ApplVar.IC.CHIP_Flag = RD_ChipCard();
            else
                ApplVar.IC.CHIP_Flag = IC_ILLERR;
#endif
            if (ApplVar.IC.CHIP_Flag>=0)
            {
                ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_INIT;
                Collect_Data(REFRESHICLOG);
            }

            return ApplVar.IC.CHIP_Flag;
        }
        else
            return -1;
    }
}
//---------------------------------------------------------------------------

short Charge_CHIP()
{
    BCD sInt;
    char sResult;
#if(CASE_MFRIC==1)
//ccr090622   	BYTE sVALATT[MFRCC_CRCVAL+1]; // 32 bytes  Area of the "current charge-value"
#else
	BYTE sVALATT[CC_CRCVAL+1]; // 32 bytes  Area of the "current charge-value"
#endif

    if (ApplVar.IC.CHIP_Flag<0)//
        return -1;
    else
    {
        SCC_SetPswd(0);

        if (SCC_Ver())
        {
            ApplVar.IC.CriptCnt = ApplVar.IC.CardNo % 10;

#if(CASE_MFRIC==1)
//ccr090622			if (!CC_ReadMFR(sVALATT,RW_Valatt))//ccr090619 备份卡中的老数据 //
//ccr090622				return -1;
			ICToMFR(RW_Init);
			ICToMFR(RW_Customer);
#else
            ApplVar.IC.REC_INIT[CC_CRCINIT] = SCC_Ccrc(ApplVar.IC.REC_INIT, sizeof(ApplVar.IC.REC_INIT)-1);
            SCC_Cript(ApplVar.IC.REC_INIT, sizeof(ApplVar.IC.REC_INIT));
            ApplVar.IC.REC_Customer[CC_CRCCLI] = SCC_Ccrc(ApplVar.IC.REC_Customer, sizeof(ApplVar.IC.REC_Customer)-1);
            SCC_Cript(ApplVar.IC.REC_Customer, sizeof(ApplVar.IC.REC_Customer));
#endif

            icBCD2EcrBCD(&sInt, &ApplVar.IC.REC_VALATT[CC_NOPER],2);
            Add(&sInt, (BCD*)&ONE);
            EcrBCD2icBCD( &ApplVar.IC.REC_VALATT[CC_NOPER], &sInt,2);

// 首先把数据写入工作区 //
#if(CASE_MFRIC==1)

			ICToMFR(RW_Valatt);
			ApplVar.IC.REC_Flags[CC_AGGVAL] = 0xff;// 先预置工作区数据写入出错 //
			ICToMFR(RW_Flags);
			ApplVar.IC.MFR_Factory[MFRCC_CRCFACT] = SCC_Ccrc(ApplVar.IC.MFR_Factory, 15);	//cc 20070825
/*ccr090622			sResult = CC_WriteMFR(ApplVar.IC.MFR_Factory,RW_Factory);
			if(sResult)
*/
			{
				if(!CC_WriteMFR(ApplVar.IC.MFR_VALATT,RW_Valatt))
				{// 卡操作出错时,恢复卡中的老数据 //
//ccr090622					CC_WriteMFR(sVALATT,RW_Valatt);
					return IC_WRITERR;
				}
				if(!CC_WriteMFR(ApplVar.IC.MFR_VALATT,RW_Valpre))	//cc 20070825
					Bell(1);// 写备份区出错时,仅仅报警,可以通过再次消费来更正备份区数据 //
				return 1;

			}
//ccr090622			else
//ccr090622				return IC_WRITERR;
#else
            ApplVar.IC.REC_VALATT[CC_CRCVAL] = SCC_Ccrc(ApplVar.IC.REC_VALATT, sizeof(ApplVar.IC.REC_VALATT)-2);
            memcpy(ApplVar.IC.REC_VALPRE,ApplVar.IC.REC_VALATT,sizeof(ApplVar.IC.REC_VALATT));
            SCC_Cript(ApplVar.IC.REC_VALATT, sizeof(ApplVar.IC.REC_VALATT));//*2);


            CC_Read(sVALATT, ICR_VALATT, sizeof(ApplVar.IC.REC_VALATT));

            ApplVar.IC.REC_Flags[CC_AGGVAL] = 0x55;

            if (sResult = CC_Write(ApplVar.IC.REC_Flags+CC_AGGVAL, IC_AGGVAL, 1)) //Set area $55
            {
                if (sResult = CC_Write(ApplVar.IC.REC_VALATT, ICR_VALATT, sizeof(ApplVar.IC.REC_VALATT)))//*2))
                {
                    ApplVar.IC.REC_Flags[CC_AGGVAL] = 0xff;
                    sResult = CC_Write(ApplVar.IC.REC_Flags+CC_AGGVAL, IC_AGGVAL, 1); //Clear area $ff
                }
                if (!sResult)//*2))
                {
                    CC_Write(sVALATT, ICR_VALATT, sizeof(ApplVar.IC.REC_VALATT));
                    return IC_WRITERR;
                }
            }
            else
                return IC_WRITERR;

/*            if (sResult=CC_Write(ApplVar.IC.REC_Flags,ICR_FLAGS,sizeof(ApplVar.IC.REC_Flags)+
                                sizeof(ApplVar.IC.REC_INIT)+
                                sizeof(ApplVar.IC.REC_Customer)+
                                sizeof(ApplVar.IC.REC_VALATT)))//*2);
            {
                ApplVar.IC.REC_Flags[CC_AGGVAL] = 0xff;
                sResult = CC_Write(ApplVar.IC.REC_Flags+CC_AGGVAL, IC_AGGVAL, 1); //Clear area $ff
            }
*/

            if (!sResult)
            {
                CC_Write(sVALATT, ICR_VALATT, sizeof(ApplVar.IC.REC_VALATT));
                return IC_WRITERR;
            }

            ApplVar.IC.CHIP_Flag = RD_ChipCard();
            if (ApplVar.IC.CHIP_Flag<0)
                CC_Write(sVALATT, ICR_VALATT, sizeof(ApplVar.IC.REC_VALATT));
            return ApplVar.IC.CHIP_Flag;
#endif
        }
        else
            return -1;
    }
}


//---------------------------------------------------------------------------
//Print Chip card on printer
//pSel:select the message for print
//	=0,display only
//	=1,print selected
//  =2,print all the message

void PrintChipCard(BYTE pSel)
{

    BCD sVal1;
    short sInt;
    BYTE  sICOpt;


    sICOpt = ApplVar.ICCardSet.Options;
    if (pSel==0)
        RESETBIT(sICOpt, IC_REPORT);

    if (ApplVar.IC.CHIP_Flag>=0)
    {
        if (pSel==0)
            mClearScreen();
        if (TESTBIT(sICOpt,IC_REPORT))
            PrintLine2('-');
        sVal1=ZERO;
        ULongToBCDValue(sVal1.Value, ApplVar.IC.CardNo);
        if (TESTBIT(sICOpt,IC_REPORT))
        {//ccr2016-09-12
            FormatQtyStr(Msg[KAHAO].str, &sVal1, PRTLEN);
            RJPrint(0, SysBuf);
        }

        memset(SysBuf,' ',PRTLEN);
        CopyFrStr(SysBuf, Msg[KLXING].str);
        CopyFrStr(SysBuf+PRTLEN-8, Msg[ZHEKOUCA+ApplVar.IC.REC_INIT[CC_TIPO]].str);
        SysBuf[PRTLEN-2] = 0;//delete :
        if (TESTBIT(sICOpt,IC_REPORT) && pSel==2)
            RJPrint(0, SysBuf);
        if (pSel==0)
        {//ccr2016-09-12
            FormatQtyStr(Msg[ZHEKOUCA+ApplVar.IC.REC_INIT[CC_TIPO]].str, &sVal1, SCREENWD);
            DispStrXY(SysBuf,0,0xff);
        }

/*		memset(&sVal1,0,sizeof(BCD));
        ULongToBCDValue(sVal1.Value, ApplVar.IC.PosCode);
        RJPrint(0, FormatQtyStr(Msg[POSDAIMA].str, &sVal1, PRTLEN));
*/
        sInt = 1;
        if (ApplVar.IC.CHIP_Flag>0)
        {
            icBCD2EcrBCD(&sVal1,(char*)&ApplVar.IC.REC_VALATT[CC_VAL], 5);
            if (ApplVar.IC.CHIP_Flag==2)
                SETBIT(sVal1.Sign,BIT7);
            if (TESTBIT(sICOpt,IC_REPORT))
            {//ccr2016-09-12
                FormatAmtStr(Msg[KNJE].str, &sVal1, PRTLEN);
                RJPrint(0, SysBuf);
            }
            if (pSel==0)
            {//ccr2016-09-12
                GotoXY(0,sInt++);
                FormatAmtStr(Msg[KNJE].str, &sVal1, SCREENWD);
                DispStrXY(SysBuf,0,0xff); //hf 20060622 DispStrXY(SysBuf+PRTLEN-10,10);
            }
        }

        if (ApplVar.IC.REC_VALATT[CC_FORE]!=0 && TESTBIT(sICOpt,IC_REPORT))
        {
            icBCD2EcrBCD(&sVal1,(char*)&ApplVar.IC.REC_VALATT[CC_FORE], 1);
            RJPrint(0, FormatQtyStr(Msg[KYJIN].str, &sVal1, PRTLEN));
        }

        icBCD2EcrBCD(&sVal1,(char*)&ApplVar.IC.REC_VALATT[CC_PRGVEN], 5);
        if (TESTBIT(sICOpt,IC_REPORT) && pSel==2)
        {//ccr2016-09-12
            FormatAmtStr(Msg[XFZE].str, &sVal1, PRTLEN);
            RJPrint(0, SysBuf);
        }
        if (pSel==0)
        {//ccr2016-09-12
            GotoXY(0,sInt++);
            FormatAmtStr(Msg[XFZE].str, &sVal1, SCREENWD);
            DispStrXY(SysBuf,0,0xff); //hf 20060622 DispStrXY(SysBuf+PRTLEN-10,10);
        }

        if (TESTBIT(sICOpt,IC_REPORT))
        {
            if (pSel==2)
            {
#if(CASE_MFRIC==0)
                icBCD2EcrBCD(&sVal1,(char*)&ApplVar.IC.REC_VALATT[CC_PRGCAR], 5);
                RJPrint(0, FormatAmtStr(Msg[CHZHZE].str, &sVal1, PRTLEN));
#endif
                icBCD2EcrBCD(&sVal1,(char*)&ApplVar.IC.REC_VALATT[CC_NOPER], 2);
                RJPrint(0, FormatQtyStr(Msg[SHYCSHU].str, &sVal1, PRTLEN));
            }

#if(CASE_MFRIC==1)
			icBCD2EcrBCD(&sVal1,(char*)&ApplVar.IC.REC_VALATT[CC_PUNTI], MFRCC_FORE-MFRCC_PUNTI);
#else
            icBCD2EcrBCD(&sVal1,(char*)&ApplVar.IC.REC_VALATT[CC_PUNTI], CC_FORE-CC_PUNTI);
#endif
            if (CheckNotZero(&sVal1))
                RJPrint(0, FormatQtyStr(Msg[XFJIFEN].str, &sVal1, PRTLEN));

            if (pSel==2)
            {
                memset(SysBuf,' ',PRTLEN);
                CopyFrStr(SysBuf, Msg[JGLBIE].str);
                WORDtoASC(&SysBuf[PRTLEN-1], (ApplVar.IC.REC_Customer[CC_CLIST] & 3)+1);//ccr040809
                SysBuf[PRTLEN] = 0;
                RJPrint(0, SysBuf);
            }
        }

        if (CWORD(ApplVar.IC.REC_Customer[CC_CPERC]) != 0)
        {
            icBCD2EcrBCD(&sVal1,(char*)&ApplVar.IC.REC_Customer[CC_CPERC], 2);
            if (TESTBIT(sICOpt,IC_REPORT))
            {//ccr2016-09-12
                FormatAmtStr(Msg[ZHKLVF].str, &sVal1, PRTLEN);
                RJPrint(0, SysBuf);
            }
            if (pSel==0)
            {//ccr2016-09-12
                GotoXY(0,sInt++);
                FormatAmtStr(Msg[ZHKLVF].str, &sVal1, SCREENWD);
                DispStrXY(SysBuf,0,0xff);
            }
        }

        if (TESTBIT(sICOpt,IC_REPORT))
        {
            if (pSel==2)
            {
                //打印卡日期
                memset(SysBuf,' ',PRTLEN);
                CopyFrStr(SysBuf, Msg[ZHKRQI].str);
#if defined(CASE_FORHANZI)
                //yyyy-mm-dd格式
                SysBuf[PRTLEN-12] = '2';    SysBuf[PRTLEN-12+1] = '0';
                HEXtoASC(SysBuf+PRTLEN-12+2, &ApplVar.IC.REC_INIT[CC_DATAI + 2], 1);
                SysBuf[PRTLEN-12+4] = '-';
                HEXtoASC(SysBuf+PRTLEN-12+5, &ApplVar.IC.REC_INIT[CC_DATAI + 1], 1);
                SysBuf[PRTLEN-12+7] = '-';
                HEXtoASC(SysBuf+PRTLEN-12+8, &ApplVar.IC.REC_INIT[CC_DATAI], 1);
#else
                //dd/mm/yyyy格式
                HEXtoASC(SysBuf+PRTLEN-12, &ApplVar.IC.REC_INIT[CC_DATAI], 1);//日
                SysBuf[PRTLEN-12+2] = '/';
                HEXtoASC(SysBuf+PRTLEN-12+3, &ApplVar.IC.REC_INIT[CC_DATAI + 1], 1);//月
                SysBuf[PRTLEN-12+5] = '/';
                SysBuf[PRTLEN-12+6] = '2';    SysBuf[PRTLEN-12+7] = '0';//年
                HEXtoASC(SysBuf+PRTLEN-12+8, &ApplVar.IC.REC_INIT[CC_DATAI + 2], 1);
#endif
                SysBuf[PRTLEN-1]=0;
                RJPrint(0, SysBuf);

                if (CWORD(ApplVar.IC.REC_Flags[CC_DSCAD]))
                {
                    memset(SysBuf,' ',PRTLEN);
                    DecodeDateSelf(((WORD)(ApplVar.IC.REC_Flags[1 + CC_DSCAD]) << 8) + ApplVar.IC.REC_Flags[CC_DSCAD], SysBuf+PRTLEN-12);
                    SysBuf[PRTLEN-1] = 0;
                    CopyFrStr(SysBuf, Msg[KYXQI].str);
                    RJPrint(0, SysBuf);
                }

                memset(SysBuf,' ',PRTLEN);
                strncpy(SysBuf+12,&ApplVar.IC.REC_Customer[CC_CNOME],PRTLEN-12);
                SysBuf[PRTLEN-1] = 0;
                CopyFrStr(SysBuf, Msg[KHMCHEN].str);
                RJPrint(0, SysBuf);
                PrintLine2('-');
                RFeed(1);
                PrintHead1(0);
            }
            else
                PrintLine2('-');
        }

    }
}

//------------------------------------------------------------------------------
/**
 *
 *
 * @author EutronSoftware (2014-12-26)
 *
 * @param pay :false=仅进行判断，=true，付款
 * @param pValue ：需要支付的金额
 * @param pIC_AMT ：送出IC卡内金额。
 */
void PayByChipCard(BCD *pValue,BCD *pIC_AMT,BYTE pay)
{
    BCD newValue;

    if (ApplVar.IC.CHIP_Flag>0)
    {
        icBCD2EcrBCD(&newValue, &ApplVar.IC.REC_VALATT[CC_VAL],CC_PRGVEN-CC_VAL);
        if (pIC_AMT)
            *pIC_AMT = newValue;
        switch (ApplVar.IC.CHIP_Flag)
        {
        case IC_TYPE1:
            Subtract(&newValue, pValue);
            if (newValue.Sign & 0x80)
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI69);
                return;
            }
            if (pay)
            {
                if (pValue->Sign & 0x80)
                    ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_ADD;
                else
                    ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_SUB;
            }
            break;
        case IC_TYPE2:
            Add(&newValue, pValue);
            if (newValue.Sign & 0x80)
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI69);
                return;
            }
            if (pay)
            {
                if (pValue->Sign & 0x80)
                    ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_ADD;
                else
                    ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_SUB;
            }
            break;
        default:
            return;
        }
        if (pay)
        {
            EcrBCD2icBCD(&ApplVar.IC.REC_VALATT[CC_VAL], (BCD *)&newValue, CC_PRGVEN-CC_VAL);
            memcpy(ApplVar.FlowBuff.CDC_refreshic.RValue,pValue->Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RValue));

            icBCD2EcrBCD(&newValue,&ApplVar.IC.REC_VALATT[CC_PRGVEN],CC_PRGCAR-CC_PRGVEN);
            Add(&newValue, pValue);
            EcrBCD2icBCD(&ApplVar.IC.REC_VALATT[CC_PRGVEN],&newValue, CC_PRGCAR-CC_PRGVEN);

            if (Charge_CHIP()<0)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI70);
            else
                Collect_Data(REFRESHICLOG);
        }
    }
}

//------------------------------------------------------------------------------
void PointsByChipCard(BCD *pValue)
{
    BCD newValue;
    BCD oldValue;
    BYTE sUpdate;

    if (ApplVar.IC.CHIP_Flag>=0)
    {
        if (TESTBIT(ApplVar.ICCardSet.Options,IC_POINTS))
        {
            memset(&oldValue,0,sizeof(BCD));
            memcpy(oldValue.Value,&ApplVar.ICCardSet.Value,sizeof(ApplVar.ICCardSet.Value));//ccr090311

            memset(&newValue,0,sizeof(BCD));
            memcpy(newValue.Value,&ApplVar.ICCardSet.MiniNum,sizeof(ApplVar.ICCardSet.MiniNum));//ccr090311

            if (CheckNotZero(&oldValue) &&
                CompareBCD(pValue,&newValue)>=0)//ccr090311
            {
                newValue = *pValue;
                Divide(&newValue, &oldValue);
                //ccr090311 trunc BCD>>>>>>>>>>>
                if (newValue.Sign & 1)
                    BcdDiv10(&newValue);
                newValue.Sign &= 0x06;
                if (newValue.Sign)
                {
                    sUpdate = newValue.Sign/2;
                    memcpy(newValue.Value,newValue.Value+sUpdate,BCDLEN-sUpdate);
                    memset(newValue.Value+BCDLEN-sUpdate,0,sUpdate);
                    newValue.Sign = 0;
                }
                //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

                memcpy(ApplVar.FlowBuff.CDC_refreshic.RPoint,newValue.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RPoint));

                icBCD2EcrBCD(&oldValue,&ApplVar.IC.REC_VALATT[CC_PUNTI], CC_FORE-CC_PUNTI);
                Add(&newValue, &oldValue);
                EcrBCD2icBCD(&ApplVar.IC.REC_VALATT[CC_PUNTI],&newValue, CC_FORE-CC_PUNTI);
                SETBIT(ApplVar.IC.ICState,IC_POINTS);

            }
        }
        if (ApplVar.IC.CHIP_Flag==0)
        {

            icBCD2EcrBCD(&newValue,&ApplVar.IC.REC_VALATT[CC_PRGVEN],CC_PRGCAR-CC_PRGVEN);
            Add(&newValue, pValue);
            EcrBCD2icBCD(&ApplVar.IC.REC_VALATT[CC_PRGVEN],&newValue, CC_PRGCAR-CC_PRGVEN);
            if (Charge_CHIP()<0)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI70);
            else//ccr2016-05-23>>>>>>>>>>
            {
                ApplVar.FlowBuff.CDC_refreshic.OpType=IC_OP_ADDPOINT;
                Collect_Data(REFRESHICLOG);
            }//ccr2016-05-23<<<<<<<<<<<<<
        }
    }
}
//---------------------------------------------------------------------------

void ProgClearChip()
{
    BCD sValue,sCopy;//ccr040809
    BYTE sFore;
    signed char sType;

/*
    if ((ApplVar.IC.ICState & (IC_NOTREMOVED | IC_INSERT))!=(IC_NOTREMOVED | IC_INSERT))
    {//卡被拔出过，终止设置   ccr 050316//
        ApplVar.ErrorNumber=ERROR_ID(CWXXI50);
        return;
    }
*/

    switch (Appl_ProgLine)
    {
    case 1:
        RESETBIT(ApplVar.IC.ICState,IC_FLAG);
#if (SCRSTATDBL)
        DispOnStatDBL((char *)Msg[QCHICKA].str,0);
#else
        DispSt1C(Msg[QCHICKA].str);//RG-DISPLAY
#endif
        sType = 0;
        for (sFore=0;sFore<CC_PRGVEN-CC_VAL;sFore++)//ccr040809
            if (ApplVar.IC.REC_VALATT[CC_VAL+sFore])
            {
                sType = 1;
                break;
            }
        if (sType)
        {
            DispStrXY(Msg[KNYJE].str, 0,2);
            break;
        }
        Appl_ProgLine=2;//Like Date
    case 2:
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(QUERENF,&ApplVar.IC.ICState,1,0);
        break;
    case 3:
        if (TESTBIT(ApplVar.IC.ICState, IC_FLAG))
        {
            RJPrint(0,Msg[QCHICKA].str);
            PrintChipCard(2);

            sFore = ApplVar.IC.REC_VALATT[CC_FORE];//ccr050316
            sType = ApplVar.IC.CHIP_Flag;
#if (CASE_MFRIC==1)
            if (!CheckCardNo() || Clear_CHIP()!=IC_NEWERR)
#else
            if (Clear_CHIP()!=IC_NEWERR)
#endif
                ApplVar.ErrorNumber=ERROR_ID(CWXXI64);
            else//ccr040809
            {

                sCopy = ApplVar.Amt;
                if (sFore!=0)
                {
                    memset(&ApplVar.Amt,0,sizeof(BCD));
                    ApplVar.Amt.Value[1]=sFore;
                    ApplVar.Amt.Sign = 0x80;
                    ApplVar.PoRaNumber = 4;
                    AddPoRaTotal();
                    ApplVar.DrawNumber = 0;
                    AddDrawerTotal();
                }
                ApplVar.Amt = sCopy;
            }
        }
        Appl_MaxEntry=ENTRYSIZE-2;
        Appl_ProgLine = 0;
        ApplVar.IC.ICState &= IC_NOTREMOVED;
        ClsState1();
        if (ApplVar.CentralLock == (SETUPMG | MG))
        {
            ApplVar.CentralLock = MG;
            mClearScreen();
        }
        Bell(2);
        break;
    }

}

//---------------------------------------------------------------------------

void ProgChargeChip()
{
    BCD newValue;
    short   i;

/*
    if ((ApplVar.IC.ICState & (IC_NOTREMOVED | IC_INSERT))!=(IC_NOTREMOVED | IC_INSERT))
    {//卡被拔出过，终止设置   ccr 050316//
        ApplVar.ErrorNumber=ERROR_ID(CWXXI50);
        return;
    }
*/

    switch (Appl_ProgLine)
    {
    case 1:
#if (SCRSTATDBL)
        DispOnStatDBL((char *)Msg[ICKCHZHI].str,0);
#else
        DispSt1C(Msg[ICKCHZHI].str);//RG-DISPLAY
#endif
        SETBIT(ApplVar.IC.ICState,IC_FLAG + IC_FLAG1);
        ApplVar.NumberEntered = ZERO;
        Appl_ProgLine = 2;//Like Date
    case 2:
        Appl_MaxEntry=10;
        Appl_BitNumber = 0;
        GetBCDValue(JINEF,ApplVar.NumberEntered.Value,IC_PRGVEN-IC_VAL,FALSE);
        break;
    case 3:
        ClearLine(SCREENLN-2);
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(CHZHTKUAN,&ApplVar.IC.ICState,2,0);//IC_FLAG1
        break;
    case 4:
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        if (TESTBIT(ApplVar.IC.ICState,IC_FLAG1))
            CheckBitValue(QRCHZHI,&ApplVar.IC.ICState,1,0);
        else
            CheckBitValue(QRTKUAN,&ApplVar.IC.ICState,1,0);
        break;
    case 5:
        if (TESTBIT(ApplVar.IC.ICState, IC_FLAG))
        {
            icBCD2EcrBCD(&newValue,&ApplVar.IC.REC_VALATT[CC_VAL],IC_PRGVEN-IC_VAL);

            memcpy(ApplVar.FlowBuff.CDC_refreshic.RValue,ApplVar.NumberEntered.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RValue));

            //if (TESTBIT(ApplVar.IC.ICState, IC_FLAG1) ^ (ApplVar.IC.CHIP_Flag==2))//ccr040809
            if (TESTBIT(ApplVar.IC.ICState, IC_FLAG1))      //cc20050817>>>>>>>>>>>>>>>>>>>>
            {
                if (ApplVar.IC.CHIP_Flag==1)
                {
                    ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_CHARGE;
                    Add(&newValue, &ApplVar.NumberEntered);
                }
                else
                {
                    ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_DECHARGE;
                    if (CompareBCD(&newValue, &ApplVar.NumberEntered)>=0)
                        Subtract(&newValue, &ApplVar.NumberEntered);
                    else
                        ApplVar.ErrorNumber=ERROR_ID(CWXXI65);
                }
            }
            else
            {
                //if (CompareBCD(&newValue, &ApplVar.NumberEntered)>=0)
                //{
                if (ApplVar.IC.CHIP_Flag==1)
                {
                    ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_DECHARGE;
                    if (CompareBCD(&newValue, &ApplVar.NumberEntered)>=0)
                        Subtract(&newValue, &ApplVar.NumberEntered);
                    else
                        ApplVar.ErrorNumber=ERROR_ID(CWXXI65);
                }
                else
                {
                    ApplVar.FlowBuff.CDC_refreshic.OpType = IC_OP_CHARGE;
                    Add(&newValue, &ApplVar.NumberEntered);
                }
                //}
                //else
                //	ApplVar.ErrorNumber=ERROR_ID(CWXXI65);
            }//cc20050817>>>>>>>>>>>>>>>>>>>>
            if (BCDWidth(&newValue)>(IC_PRGVEN-IC_VAL)*2)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI65);
            if (!ApplVar.ErrorNumber)
            {
                EcrBCD2icBCD(&ApplVar.IC.REC_VALATT[CC_VAL],&newValue, IC_PRGVEN-IC_VAL);
                icBCD2EcrBCD(&newValue,&ApplVar.IC.REC_VALATT[CC_PRGCAR],IC_PRGCAR-IC_PRGVEN);
                if (TESTBIT(ApplVar.IC.ICState, IC_FLAG1))
                {
                    i = ICKCHZHI;
                    Add(&newValue, &ApplVar.NumberEntered);
                }
                else
                {
                    i = ICKTKUAN;
                    Subtract(&newValue, &ApplVar.NumberEntered);
                }
                EcrBCD2icBCD(&ApplVar.IC.REC_VALATT[CC_PRGCAR],&newValue, IC_PRGCAR-IC_PRGVEN);


#if (CASE_MFRIC==1)
				if (!CheckCardNo() || Charge_CHIP()<0)
#else
                if (Charge_CHIP()<0)
#endif
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI66);
                else
                {
                    newValue = ApplVar.Amt;
                    ApplVar.Amt = ApplVar.NumberEntered;
                    if (!TESTBIT(ApplVar.IC.ICState, IC_FLAG1))//ccr040809
                    {
                        ApplVar.Amt.Sign ^= 0x80;
                        ApplVar.PoRaNumber = 2;//charge ApplVar.IC
                    }
                    else
                        ApplVar.PoRaNumber = 3;//decrease ApplVar.IC
                    AddPoRaTotal();
                    ApplVar.DrawNumber = 0;
                    AddDrawerTotal();
                    ApplVar.Amt = newValue;

                    RJPrint(0, FormatAmtStr(Msg[i].str, &ApplVar.NumberEntered,PRTLEN));
                    PrintChipCard(2);
                    Collect_Data(REFRESHICLOG);
                }
            }

        }
        Appl_MaxEntry=ENTRYSIZE-2;
        Appl_ProgLine = 0;
        ApplVar.IC.ICState &= IC_NOTREMOVED;
        ClsState1();
        if (ApplVar.CentralLock == (SETUPMG | MG))
        {
            ApplVar.CentralLock = MG;
            mClearScreen();
        }
        Bell(2);
        break;
    }
//
}

void ProgPoints()
{
    BCD newValue;

/*
    if ((ApplVar.IC.ICState & (IC_NOTREMOVED | IC_INSERT))!=(IC_NOTREMOVED | IC_INSERT))
    {//卡被拔出过，终止设置   ccr 050316//
        ApplVar.ErrorNumber=ERROR_ID(CWXXI50);
        return;
    }
*/

    switch (Appl_ProgLine)
    {
    case 1:
#if (SCRSTATDBL)
        DispOnStatDBL((char *)Msg[XFJDDJIANG].str,0);
#else
        DispSt1C(Msg[XFJDDJIANG].str);//RG-DISPLAY
#endif
        RESETBIT(ApplVar.IC.ICState,IC_FLAG);
        SETBIT(ApplVar.IC.ICState,IC_FLAG1);
        ApplVar.NumberEntered = ZERO;
        Appl_ProgLine=2;//Like Date
    case 2:
        Appl_MaxEntry=6;
        Appl_BitNumber = 0;
        GetBCDValue(ICKSDIAN,ApplVar.NumberEntered.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RPoint),TRUE);
        break;
    case 3:
        ClearLine(SCREENLN-2);
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(JDSDIAN,&ApplVar.IC.ICState,2,0);//IC_FLAG1
        //ccr2016-05-23>>>>>>>>>>>>>>>>
        if (TESTBIT(ApplVar.IC.ICState,IC_FLAG1))
            strcpy(ProgLineMes+SETINPUTWD-4,cDECVIPPOINT);
        else
            strcpy(ProgLineMes+SETINPUTWD-4,cADDVIPPOINT);
        //ccr2016-05-23<<<<<<<<<<<<<<<<<
        break;
    case 4:
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        if (TESTBIT(ApplVar.IC.ICState,IC_FLAG1))
            CheckBitValue(DJQREN,&ApplVar.IC.ICState,1,0);//IC_FLAG
        else
            CheckBitValue(QRSDIAN,&ApplVar.IC.ICState,1,0);//IC_FLAG
        break;
    case 5:
        if (TESTBIT(ApplVar.IC.ICState, IC_FLAG))
        {
            icBCD2EcrBCD(&newValue,&ApplVar.IC.REC_VALATT[CC_PUNTI], CC_FORE-CC_PUNTI);
            if (!TESTBIT(ApplVar.IC.ICState, IC_FLAG1))
            {//送分
                ApplVar.FlowBuff.CDC_refreshic.OpType=IC_OP_ADDPOINT;
                Add(&newValue, &ApplVar.NumberEntered);
            }
            else
            {//兑奖
                ApplVar.FlowBuff.CDC_refreshic.OpType=IC_OP_DECPOINT;
                if (CompareBCD(&newValue, &ApplVar.NumberEntered)>=0)
                    Subtract(&newValue, &ApplVar.NumberEntered);
                else
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI65);
            }
            if (BCDWidth(&newValue)>(CC_FORE-CC_PUNTI)*2)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI65);
            if (!ApplVar.ErrorNumber)
            {
                EcrBCD2icBCD(&ApplVar.IC.REC_VALATT[CC_PUNTI],&newValue, CC_FORE-CC_PUNTI);

#if (CASE_MFRIC==1)
				if (!CheckCardNo() ||  Charge_CHIP()<0)
#else
                if (Charge_CHIP()<0)
#endif
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI58);
                else//ccr2016-05-23
                {
                    memset(ApplVar.FlowBuff.CDC_refreshic.RValue,0,sizeof(ApplVar.FlowBuff.CDC_refreshic.RValue));
                    memcpy(ApplVar.FlowBuff.CDC_refreshic.RPoint,ApplVar.NumberEntered.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RPoint));
                    Collect_Data(REFRESHICLOG);
                }
            }

        }
        Appl_MaxEntry=ENTRYSIZE-2;
        Appl_ProgLine = 0;
        ApplVar.IC.ICState &= IC_NOTREMOVED;
        ClsState1();
        if (ApplVar.CentralLock == (SETUPMG | MG))
        {
            ApplVar.CentralLock = MG;
            mClearScreen();
        }
        if (!ApplVar.ErrorNumber)
            PrintChipCard(2);
        Bell(2);
        break;
    }
}
//---------------------------------------------------------------------------
void ProgInitialChip()
{
    WORD    sWord;
    short   sLp,sInt;
    BYTE    CHIP_FSCAD;
    UnLong  sLong;
    BCD     sValue;

/*
    if ((ApplVar.IC.ICState & (IC_NOTREMOVED | IC_INSERT))!=(IC_NOTREMOVED | IC_INSERT))
    {//卡被拔出过，终止设置   ccr 050316//
        ApplVar.ErrorNumber=ERROR_ID(CWXXI50);
        return;
    }
*/
    switch (Appl_ProgLine)
    {
    case 1 :
        RESETBIT(ApplVar.MyFlags,SETCONFIRM);//ccr040809
#if (SCRSTATDBL)
        DispOnStatDBL((char *)Msg[CHSHHICKA].str,0);
#else
        DispSt1C(Msg[CHSHHICKA].str);//RG-DISPLAY
#endif
        SETBIT(ApplVar.IC.ICState,IC_FLAG);
        ApplVar.NumberEntered = ZERO;
        memset(ApplVar.IC.REC_Flags,0,sizeof(ApplVar.IC.REC_Flags)+sizeof(ApplVar.IC.REC_INIT)+sizeof(ApplVar.IC.REC_Customer)+sizeof(ApplVar.IC.REC_VALATT)*2); //"FLAGS" Area
        memset(&ApplVar.IC.REC_Customer[CC_CNOME],0,19);
        CWORD(ApplVar.IC.CHIP_PSWD) = 0;
        ApplVar.IC.REC_Customer[CC_CLIST] = 1;
        Appl_ProgLine=2;//Like Date
    case 2 :   //DES
        Appl_MaxEntry=20;
        SETBIT(ApplVar.MyFlags,HANZIMODE);
        GetString(KHMCHEN,&ApplVar.IC.REC_Customer[CC_CNOME],20,0);
        break;
    case 3 :   //TYPE
        ClearLine(SCREENLN-2);
        Appl_MaxEntry=1;
        GetByteValue(KLXING,&ApplVar.IC.REC_INIT[CC_TIPO],2); //0,1,2
        memset(SysBuf,' ',SCREENWD);SysBuf[SCREENWD]=0;//清除最后一行
        DispStrXY(SysBuf,0,SCREENLN-2);
        break;
    case 4 :   //PROT_LEVL
        if (ApplVar.IC.REC_INIT[CC_TIPO]>0)
        {
            Appl_MaxEntry=1;
            GetByteValue(BHJBIE,&ApplVar.IC.REC_INIT[CC_FTIPO],1); //0,1,2
            break;
        }
        else
            Appl_ProgLine = 5;
    case 5 :  //PASSWORD(PIN)
        if (ApplVar.IC.REC_INIT[CC_FTIPO]>0)//ccr040809
        {
            Appl_MaxEntry=4;
            SETBIT(ApplVar.MyFlags,PWDMODE);
            Appl_BitNumber = 0;
            GetBCDValue(PINMA,ApplVar.IC.CHIP_PSWD,2,TRUE);
            ApplVar.IC.CHIP_PSWD[2] = 0xff;
            break;
        }
        else
            Appl_ProgLine = 7;
    case 6 :  //PASSWORD(PIN)
        if (ApplVar.IC.REC_INIT[CC_FTIPO]>0 && CWORD(ApplVar.IC.CHIP_PSWD)>0)//ccr040809
        {
            Appl_MaxEntry=4;
            SETBIT(ApplVar.MyFlags,PWDMODE+SETCONFIRM); //ccr040809
            Appl_BitNumber = 0;
            sWord = 0;
            GetBCDValue(PINMA,(char *)&sWord,2,TRUE);
            if (ApplVar.IC.CHIP_PSWD[2] != 0xff && sWord!=CWORD(ApplVar.IC.CHIP_PSWD))
                ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
            ApplVar.IC.CHIP_PSWD[2] = 0x00;
            break;
        }
        else
            Appl_ProgLine = 7;
    case 7:  //Discount
        RESETBIT(ApplVar.MyFlags,SETCONFIRM);//ccr040809
        Appl_MaxEntry=5;
        Appl_BitNumber = 0;
        icBCD2EcrBCD(&sValue, (char *)&ApplVar.IC.REC_Customer[CC_CPERC],2);
        GetOpt(40, sValue.Value, 2);
        EcrBCD2icBCD((char *)&ApplVar.IC.REC_Customer[CC_CPERC], &sValue, 2);
        break;
    case 8:  //Foregift
        Appl_MaxEntry=2;
        Appl_BitNumber = 0;
        GetBCDValue(KYJIN,&ApplVar.IC.REC_VALATT[CC_FORE],1,TRUE);
        break;
    case 9 :   //PriceLevl
        Appl_MaxEntry=1;          //ccr040809
        CHIP_FSCAD = 1;
        GetByteValue(JGLBIE,&CHIP_FSCAD,4);  //0/1/2/3/4
        if (CHIP_FSCAD>0)
            CHIP_FSCAD--;
        ApplVar.IC.REC_Customer[CC_CLIST] = CHIP_FSCAD & 0x03;
        break;
    case 10:  //Dead
        Appl_MaxEntry=8;//ccr040805>>>>>>>>
        if (Appl_EntryCounter>0 && Appl_EntryCounter!=6 && Appl_EntryCounter!=8)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
        else
        {
            GetBCDValue(KYXQI,&ApplVar.IC.REC_Flags[CC_DSCAD],4,TRUE);
            if (Appl_EntryCounter>0)
            {
                if (ApplVar.IC.REC_Flags[CC_DSCAD+3]<0x20)
                    ApplVar.IC.REC_Flags[CC_DSCAD+3]=0x20;
                if (ApplVar.IC.REC_Flags[CC_DSCAD]==0 || ApplVar.IC.REC_Flags[CC_DSCAD]>0x31 ||
                    ApplVar.IC.REC_Flags[CC_DSCAD+1]==0 || ApplVar.IC.REC_Flags[CC_DSCAD+1]>0x12)
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            }
        }                               //ccr040805<<<<<<<
        break;
    case 11:
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(QUERENF,&ApplVar.IC.ICState,1,0);
        break;
    case 12:
        if (TESTBIT(ApplVar.IC.ICState, IC_FLAG))
        {
            DispStrXY(Msg[QDDAI].str,0,3);

            ApplVar.IC.REC_Flags[CC_VERG1] = 0x55;
            ApplVar.IC.REC_Flags[CC_VERG2] = 0x55;
            sLong = Bcd2Long(&ApplVar.IC.REC_Flags[CC_DSCAD], 4);

            if (sLong>0)
            {
                CHIP_FSCAD = 1;
                sWord = (sLong / 10000);
                if (sWord<2000)
                    sWord += 2000;
                sWord-=1992;
                sWord = (sWord<<9) + (((sLong % 10000)/100)<<5) + (sLong % 100);
            }
            else
            {
                CHIP_FSCAD = 0;
                sWord = 0;
            }
            ApplVar.IC.REC_Flags[CC_DSCAD] = sWord & 0xFF;
            ApplVar.IC.REC_Flags[CC_DSCAD + 1] = (sWord>>8);

            //Create INIT area

            ApplVar.IC.REC_INIT[CC_FTIPO] |= (CHIP_FSCAD<<FLT0_Scad); //set the protection level and expiration

            sLong = ApplVar.ICCardSet.PosCode;
            for (sLp = CC_CODPOS;sLp<=CC_CODPOS + 3;sLp++)
            {
                ApplVar.IC.REC_INIT[sLp] = sLong & 0xFF;
                sLong >>= 8;
            }

            //Set date ICCard created
            ApplVar.IC.REC_INIT[CC_DATAI + 2] = Now.year; //BCD for year;
            ApplVar.IC.REC_INIT[CC_DATAI + 1] = Now.month; //BCD for ApplVar.Month;
            ApplVar.IC.REC_INIT[CC_DATAI] = Now.day; //BCD for ApplVar.Day;

            ApplVar.IC.REC_INIT[CC_ECR] = 0; //
            ApplVar.IC.REC_INIT[CC_ECRSW] = 0x10; //Set ECR version

            sWord = Bcd2Long(ApplVar.IC.CHIP_PSWD, 2);
            ApplVar.IC.REC_INIT[CC_PINL] = sWord & 0xFF; //set PIN-L (olny if Protection= 0 or 1)
            ApplVar.IC.REC_INIT[CC_PINH] = (sWord >> 8); //set PIN-H (olny if Protection= 0 or 1)


            ApplVar.IC.REC_INIT[CC_AUTOC] = 0; //set #function of automatic closing

            ApplVar.IC.CHIP_TIPO=ApplVar.IC.REC_INIT[CC_TIPO];
            ApplVar.IC.CHIP_FTIPO=ApplVar.IC.REC_INIT[CC_FTIPO];
            ApplVar.IC.CHIP_PROT=ApplVar.IC.CHIP_FTIPO&3; //Set the protection type

            //Create Cutomer area
//		        for (sLp = CC_CCF;sLp<=CC_CCOD - CC_CCF - 1;sLp++) //Clear
//			          ApplVar.IC.REC_Customer[sLp] = sLp+'0'; //' '
#if (CASE_MFRIC==1)
            if (!CheckCardNo() || Initial_CHIP()<0)
#else
            if (Initial_CHIP()<0)
#endif
                ApplVar.ErrorNumber=ERROR_ID(CWXXI67);
            else
            {
                if (ApplVar.IC.REC_VALATT[CC_FORE]!=0)//ccr040809
                {
                    sValue = ApplVar.Amt;
                    memset(&ApplVar.Amt,0,sizeof(BCD));
                    ApplVar.Amt.Value[1]=ApplVar.IC.REC_VALATT[CC_FORE];
                    ApplVar.PoRaNumber = 5;
                    AddPoRaTotal();
                    ApplVar.DrawNumber = 0;
                    AddDrawerTotal();
                    ApplVar.Amt = sValue;
                }
                PrintChipCard(2);
            }
        }
        Appl_MaxEntry=ENTRYSIZE-2;
        Appl_ProgLine = 0;
        ApplVar.IC.ICState &= IC_NOTREMOVED;
        ClsState1();
        if (ApplVar.CentralLock == (SETUPMG | MG))
        {
            ApplVar.CentralLock = MG;
            mClearScreen();
        }
        Bell(2);
        break;
    }
}
//---------------------------------------------------------------------------
#if(CASE_MFRIC==1)
// 把从MFRIC中读取的数据转换为标准的IC卡数据,如果MFR_VALATT中的数据有错,那么就用MFR_VALPRE中的数据恢复 //
BYTE MFRToIC(int pSel)
{
	BYTE sMFR_VALATT[MFRCC_CRCVAL+1],sMFR_VALPRE[MFRCC_CRCVAL+1];
	int sBx;

	if(pSel == RW_Factory || pSel == RW_ALL)
	{
		memset(ApplVar.IC.REC_Factory,0xff,sizeof(ApplVar.IC.REC_Factory));
		CLONG(ApplVar.IC.REC_Factory[CC_SERIE])=CLONG(ApplVar.IC.MFR_Factory[MFRCC_SERIE]);//ccr090310,SERIELEN);

		CLONG(ApplVar.IC.REC_Factory[CC_PATT])=CLONG(ApplVar.IC.MFR_Factory[MFRCC_PATT]);//ccr090310,PATTLEN);
		CWORD(ApplVar.IC.REC_Factory[CC_PATT+4])=CWORD(ApplVar.IC.MFR_Factory[MFRCC_PATT+4]);//ccr090310,PATTLEN);
	}
	if(pSel == RW_Flags || pSel == RW_ALL)
	{
		memset(ApplVar.IC.REC_Flags,0xff,sizeof(ApplVar.IC.REC_Flags));
		ApplVar.IC.REC_Flags[CC_AGGINIZ] = ApplVar.IC.MFR_Factory[MFRCC_AGGINIZ];
		ApplVar.IC.REC_Flags[CC_AGGVAL] = ApplVar.IC.MFR_Factory[MFRCC_AGGVAL];
		CWORD(ApplVar.IC.REC_Flags[CC_DSCAD])=CWORD(ApplVar.IC.MFR_Factory[MFRCC_DSCAD]);//ccr090310,DSCADLEN);
	}
	if(pSel == RW_Init || pSel == RW_ALL)
	{
		SCC_Cript(ApplVar.IC.MFR_INIT,16); //Decode of INIT record
		if(ApplVar.IC.MFR_INIT[MFRCC_CRCINIT] != SCC_Ccrc(ApplVar.IC.MFR_INIT,sizeof(ApplVar.IC.MFR_INIT)-1))
              	return 0;
		memset(ApplVar.IC.REC_INIT,0xff,sizeof(ApplVar.IC.REC_INIT));
		ApplVar.IC.REC_INIT[CC_TIPO] = ApplVar.IC.MFR_INIT[MFRCC_TIPO];
		ApplVar.IC.REC_INIT[CC_FTIPO] = ApplVar.IC.MFR_INIT[MFRCC_FTIPO];

		CWORD(ApplVar.IC.REC_INIT[CC_DATAI])=CWORD(ApplVar.IC.MFR_INIT[MFRCC_DATAI]);//ccr090310,DATAILEN);
		ApplVar.IC.REC_INIT[CC_DATAI+2]=ApplVar.IC.MFR_INIT[MFRCC_DATAI+2];//ccr090310,DATAILEN);

		ApplVar.IC.REC_INIT[CC_ECR] = ApplVar.IC.MFR_INIT[MFRCC_ECR];
		ApplVar.IC.REC_INIT[CC_ECRSW] = ApplVar.IC.MFR_INIT[MFRCC_ECRSW];

		CLONG(ApplVar.IC.REC_INIT[CC_CODPOS])=CLONG(ApplVar.IC.MFR_INIT[MFRCC_CODPOS]);//ccr090310,CODPOSLEN);

		ApplVar.IC.REC_INIT[CC_PINH] = ApplVar.IC.MFR_INIT[MFRCC_PINH];

		CWORD(ApplVar.IC.REC_INIT[CC_AUTOC])=CWORD(ApplVar.IC.MFR_INIT[MFRCC_AUTOC]);//ccr090310,AUTOCLEN);

		ApplVar.IC.REC_INIT[CC_PINL] = ApplVar.IC.MFR_INIT[MFRCC_PINL];
	}
	if(pSel == RW_Customer || pSel == RW_ALL)
	{
		SCC_Cript(ApplVar.IC.MFR_Customer,32); //Decode of Customer Area
		if(ApplVar.IC.MFR_Customer[MFRCC_CRCCLI] != SCC_Ccrc(ApplVar.IC.MFR_Customer,sizeof(ApplVar.IC.MFR_Customer)-1))
              	return 0;
		memset(ApplVar.IC.REC_Customer,0xff,sizeof(ApplVar.IC.REC_Customer));
		CWORD(ApplVar.IC.REC_Customer[CC_CPERC])=CWORD(ApplVar.IC.MFR_Customer[MFRCC_CPERC]);//ccr090310,CPERCLEN);
		CWORD(ApplVar.IC.REC_Customer[CC_CLIST])=CWORD(ApplVar.IC.MFR_Customer[MFRCC_CLIST]);//ccr090310,CLISTLEN);
		memcpy(&ApplVar.IC.REC_Customer[CC_CNOME],&ApplVar.IC.MFR_Customer[MFRCC_CNOME],CNOMELEN);
	}
	if(pSel == RW_Valatt || pSel == RW_ALL)
	{// 对金额数据进行验证并恢复和转储 //
		memcpy(sMFR_VALATT,ApplVar.IC.MFR_VALATT,sizeof(ApplVar.IC.MFR_VALATT));
		memcpy(sMFR_VALPRE,ApplVar.IC.MFR_VALPRE,sizeof(ApplVar.IC.MFR_VALPRE));

//ccr090622>>>>>>>>>>>>>>>>>>>>>>>>>
		sBx = ApplVar.IC.CriptCnt;
		SCC_Cript(ApplVar.IC.MFR_VALATT,sizeof(ApplVar.IC.MFR_VALATT));
		ApplVar.IC.CriptCnt = sBx;
		SCC_Cript(ApplVar.IC.MFR_VALPRE,sizeof(ApplVar.IC.MFR_VALPRE));

		if(SCC_Ccrc(ApplVar.IC.MFR_VALATT,sizeof(ApplVar.IC.MFR_VALATT)-2) != ApplVar.IC.MFR_VALATT[MFRCC_CRCVAL]) //checksum of charge-value field
		{
			if(SCC_Ccrc(ApplVar.IC.MFR_VALPRE,sizeof(ApplVar.IC.MFR_VALPRE)-2) == ApplVar.IC.MFR_VALPRE[MFRCC_CRCVAL])//checksum of charge-value field
			{
				memcpy(ApplVar.IC.MFR_VALATT,ApplVar.IC.MFR_VALPRE,sizeof(ApplVar.IC.MFR_VALATT));
//ccr090622				CC_WriteMFR(sMFR_VALPRE,RW_Valatt);
			}
			else
				return 0;
		}
		else if (memcmp(ApplVar.IC.MFR_VALATT,ApplVar.IC.MFR_VALPRE,sizeof(ApplVar.IC.MFR_VALATT)))
		{// 当备份区的数据有错时,把工作区中数据复制到备份区,并写卡 //
				memcpy(ApplVar.IC.MFR_VALPRE,ApplVar.IC.MFR_VALATT,sizeof(ApplVar.IC.MFR_VALATT));
				CC_WriteMFR(sMFR_VALPRE,RW_Valpre);
		}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		memset(ApplVar.IC.REC_VALATT,0,sizeof(ApplVar.IC.REC_VALATT));

		CWORD(ApplVar.IC.REC_VALATT[CC_NOPER])=CWORD(ApplVar.IC.MFR_VALATT[MFRCC_NOPER]);//ccr090310,,NOPERLEN);

		CLONG(ApplVar.IC.REC_VALATT[CC_VAL+1])=CLONG(ApplVar.IC.MFR_VALATT[MFRCC_VAL]);//ccr090310,,VALLEN);
		CLONG(ApplVar.IC.REC_VALATT[CC_PRGVEN+1])=CLONG(ApplVar.IC.MFR_VALATT[MFRCC_PRGVEN]);//ccr090310,PRGVENLEN);

		CWORD(ApplVar.IC.REC_VALATT[CC_PUNTI]) = CWORD(ApplVar.IC.MFR_VALATT[MFRCC_PUNTI]);//ccr090310,PUNTILEN
		ApplVar.IC.REC_VALATT[CC_PUNTI+2] = ApplVar.IC.MFR_VALATT[MFRCC_PUNTI+2];//ccr090310,PUNTILEN

		ApplVar.IC.REC_VALATT[CC_FORE] = ApplVar.IC.MFR_VALATT[MFRCC_FORE];
		ApplVar.IC.REC_VALATT[CC_ICTYPE] = ApplVar.IC.MFR_VALATT[MFRCC_ICTYPE];
	}

	return 1;
}
//------------------------------------------------------------------------
void ICToMFR(int pSel)
{
	if(pSel == RW_Flags || pSel == RW_ALL)
	{
		ApplVar.IC.MFR_Factory[MFRCC_AGGINIZ] = ApplVar.IC.REC_Flags[CC_AGGINIZ];
		ApplVar.IC.MFR_Factory[MFRCC_AGGVAL] = ApplVar.IC.REC_Flags[CC_AGGVAL];
		CWORD(ApplVar.IC.MFR_Factory[MFRCC_DSCAD])=CWORD(ApplVar.IC.REC_Flags[CC_DSCAD]);//ccr090310,DSCADLEN);
	}
	if(pSel == RW_Init || pSel == RW_ALL)
	{
		ApplVar.IC.MFR_INIT[MFRCC_TIPO] = ApplVar.IC.REC_INIT[CC_TIPO];
		ApplVar.IC.MFR_INIT[MFRCC_FTIPO] = ApplVar.IC.REC_INIT[CC_FTIPO];

		CWORD(ApplVar.IC.MFR_INIT[MFRCC_DATAI])=CWORD(ApplVar.IC.REC_INIT[CC_DATAI]);//ccr090310,DATAILEN);
		ApplVar.IC.MFR_INIT[MFRCC_DATAI+2]=ApplVar.IC.REC_INIT[CC_DATAI+2];//ccr090310,DATAILEN);

		ApplVar.IC.MFR_INIT[MFRCC_ECR] = ApplVar.IC.REC_INIT[CC_ECR];
		ApplVar.IC.MFR_INIT[MFRCC_ECRSW] = ApplVar.IC.REC_INIT[CC_ECRSW];

		CLONG(ApplVar.IC.MFR_INIT[MFRCC_CODPOS])=CLONG(ApplVar.IC.REC_INIT[CC_CODPOS]);//ccr090310,CODPOSLEN);

		ApplVar.IC.MFR_INIT[MFRCC_PINH] = ApplVar.IC.REC_INIT[CC_PINH];

		CWORD(ApplVar.IC.MFR_INIT[MFRCC_AUTOC])=CWORD(ApplVar.IC.REC_INIT[CC_AUTOC]);//ccr090310,AUTOCLEN);

		ApplVar.IC.MFR_INIT[MFRCC_PINL] = ApplVar.IC.REC_INIT[CC_PINL];
		ApplVar.IC.MFR_INIT[MFRCC_CRCINIT] = SCC_Ccrc(ApplVar.IC.MFR_INIT,sizeof(ApplVar.IC.MFR_INIT)-1);
		SCC_Cript(ApplVar.IC.MFR_INIT,16); //Decode of INIT record
	}
	if(pSel == RW_Customer || pSel == RW_ALL)
	{
		CWORD(ApplVar.IC.MFR_Customer[MFRCC_CPERC])=CWORD(ApplVar.IC.REC_Customer[CC_CPERC]);//ccr090310,CPERCLEN);
		CWORD(ApplVar.IC.MFR_Customer[MFRCC_CLIST])=CWORD(ApplVar.IC.REC_Customer[CC_CLIST]);//ccr090310,,CLISTLEN);

		memcpy(&ApplVar.IC.MFR_Customer[MFRCC_CNOME],&ApplVar.IC.REC_Customer[CC_CNOME],CNOMELEN);

		ApplVar.IC.MFR_Customer[MFRCC_CRCCLI] = SCC_Ccrc(ApplVar.IC.MFR_Customer,sizeof(ApplVar.IC.MFR_Customer)-1);
		SCC_Cript(ApplVar.IC.MFR_Customer,32); //Decode of Customer Area
	}
	if(pSel == RW_Valatt || pSel == RW_ALL)
	{

		CWORD(ApplVar.IC.MFR_VALATT[MFRCC_NOPER])=CWORD(ApplVar.IC.REC_VALATT[CC_NOPER]);//ccr090310,NOPERLEN);

		CLONG(ApplVar.IC.MFR_VALATT[MFRCC_VAL])=CLONG(ApplVar.IC.REC_VALATT[CC_VAL+1]);//ccr090310,VALLEN);
		CLONG(ApplVar.IC.MFR_VALATT[MFRCC_PRGVEN])=CLONG(ApplVar.IC.REC_VALATT[CC_PRGVEN+1]);//ccr090310,PRGVENLEN);

		CWORD(ApplVar.IC.MFR_VALATT[MFRCC_PUNTI])=CWORD(ApplVar.IC.REC_VALATT[CC_PUNTI]);//ccr090310,PUNTILEN);
		ApplVar.IC.MFR_VALATT[MFRCC_PUNTI+2] = ApplVar.IC.REC_VALATT[CC_PUNTI+2];

		ApplVar.IC.MFR_VALATT[MFRCC_FORE] = ApplVar.IC.REC_VALATT[CC_FORE];
		ApplVar.IC.MFR_VALATT[MFRCC_ICTYPE] = ApplVar.IC.REC_VALATT[CC_ICTYPE];
		ApplVar.IC.MFR_VALATT[MFRCC_CRCVAL] = SCC_Ccrc(ApplVar.IC.MFR_VALATT,sizeof(ApplVar.IC.MFR_VALATT)-2);
		SCC_Cript(ApplVar.IC.MFR_VALATT,sizeof(ApplVar.IC.MFR_VALATT));
	}
}

#endif
#endif

void    PrintICMessage()
{
#if (DD_CHIPC==1)
#if (CASE_MFRIC==1)
                ChipCard();
                if ((ApplVar.IC.CHIP_Flag>=0)&& TESTBIT(ApplVar.IC.ICState,IC_INSERT))//ccr2016-03-31
                {
                    if (ApplVar.ErrorNumber)
                        CheckError(0);
                    else
                        PrintChipCard(2);
                }
                RESETBIT(ApplVar.IC.ICState,IC_INSERT);
#else
                if ((ApplVar.IC.CHIP_Flag>=0)&& TESTBIT(ApplVar.IC.ICState,IC_INSERT))//ccr2016-03-31
                {
                    RESETBIT(ApplVar.IC.ICState,IC_INSERT | IC_NOTREMOVED);
                    ChipCard();
                    if (ApplVar.ErrorNumber)
                        CheckError(0);
                    else
                        PrintChipCard(2);
                }
#endif
#endif

}

#if (DD_ECRVIP==1)
/**..........................................................
 * 根据ECRVIPNumber读取会员数据,实际上是将一个8K的数据从Flash读入到ECRVipVar区
 *
 * @author EutronSoftware (2016-05-13)
 ..........................................................*/
void ReadECRVIPVar()
{
    if (ECRVIP.ECRVIPNumber<ECRVIPMAX/2)
    {
        if (ECRVIP.CurrentVIPVar!=FLASH_ECRVIPVAR1_ADDR)
        {
            memcpy(ECRVIP.VipVar,ECRVIPVar1,sizeof(ECRVIP.VipVar));//bFlashReadBuffer((BYTE*)&ECRVIP.VipVar,FLASH_ECRVIPVAR1_ADDR,sizeof(ECRVIP.VipVar));
            ECRVIP.CurrentVIPVar=FLASH_ECRVIPVAR1_ADDR;
        }
    }
    else if (ECRVIP.ECRVIPNumber<ECRVIPMAX)
    {
        if (ECRVIP.CurrentVIPVar!=FLASH_ECRVIPVAR2_ADDR)
        {
            memcpy(ECRVIP.VipVar,ECRVIPVar2,sizeof(ECRVIP.VipVar));//bFlashReadBuffer((BYTE*)&ECRVIP.VipVar,FLASH_ECRVIPVAR2_ADDR,sizeof(ECRVIP.VipVar));//FLASH_ECRVIPVAR2_SIZE
            ECRVIP.CurrentVIPVar=FLASH_ECRVIPVAR2_ADDR;
        }
    }
    else
        ApplVar.ErrorNumber=ERROR_ID(CWXXI90);
}
/**..........................................................
 * //根据ECRVIPNumber将会员数据ECRVipVar写入Flash,在写入前先擦除对应FLASH区
 *
 * @author EutronSoftware (2016-05-13)
 ..........................................................*/
void WriteECRVIPVar()
{
    if (ECRVIP.ECRVIPNumber<ECRVIPMAX/2)
    {
        bFlashMBlockErase(FLASH_ECRVIPVAR1_BLOCKFr,FLASH_ECRVIPVAR1_BLOCKS);
        bFlashProgram(FLASH_ECRVIPVAR1_ADDR, sizeof(ECRVIP.VipVar),(uint8_t *)&ECRVIP.VipVar );
#if defined(DEBUGBYPC)
        memcpy(ECRVIPVar1,ECRVIP.VipVar,sizeof(ECRVIPVar1));
#endif
    }
    else if (ECRVIP.ECRVIPNumber<ECRVIPMAX)
    {
        bFlashMBlockErase(FLASH_ECRVIPVAR2_BLOCKFr,FLASH_ECRVIPVAR2_BLOCKS);
        bFlashProgram(FLASH_ECRVIPVAR2_ADDR, sizeof(ECRVIP.VipVar),(uint8_t *)&ECRVIP.VipVar );
#if defined(DEBUGBYPC)
        memcpy(ECRVIPVar2,ECRVIP.VipVar,sizeof(ECRVIPVar1));
#endif
    }
    else
        ApplVar.ErrorNumber=ERROR_ID(CWXXI90);
}
/**..........................................................
 * 初始化ECRVIP,即将ECRVIP的flash空间全部擦除,并将ECRVIPCount
 *
 * @author EutronSoftware (2016-05-13)
 ..........................................................*/
void InitECRVIP()
{
     bFlashMBlockErase(FLASH_ECRVIPVAR1_BLOCKFr,FLASH_ECRVIPVAR1_BLOCKS+FLASH_ECRVIPVAR2_BLOCKS+FLASH_ECRVIPFIX_BLOCKS);
     ApplVar.ICCardSet.ECRVIPCount=0;
     ECRVIP.ECRVIPState=0; // ECRVIPState操作标志
     ECRVIP.CurrentVIPVar=0; // ECRVIPState操作标志
}
/**..........................................................
 * 恢复会员数据
 *
 * @author EutronSoftware (2016-05-13)
 ..........................................................*/
void RecallECRVIP()
{
    int i;

#if defined(DEBUGBYPC)
    bFlashReadBuffer((BYTE*)&ECRVIPVar1,FLASH_ECRVIPVAR1_ADDR,sizeof(ECRVIPVar1));
    bFlashReadBuffer((BYTE*)&ECRVIPVar2,FLASH_ECRVIPVAR2_ADDR,sizeof(ECRVIPVar1));
    bFlashReadBuffer((BYTE*)&ECRVIPFix,FLASH_ECRVIPFIX_ADDR,sizeof(ECRVIPFix));
#endif
    ApplVar.ICCardSet.ECRVIPCount=0;
    ECRVIP.ECRVIPState=0; // ECRVIPState操作标志
    for (i=0;i<ECRVIPMAX && (ECRVIPFix[i].VipDate!=0xffff)/* && CLONG(ECRVIPFix[i].VipTEL)!=0xFFFFFFFF)*/;i++)
        ApplVar.ICCardSet.ECRVIPCount++;
}

/******************************************************************
 * 增加一个新会员,将新增加的会员数据EcrVipFix写入FLASH
 *
 * @author EutronSoftware (2016-05-16)
 *****************************************************************/
void AddNewECRVIP()
{
    int vipNumber;
    struct  TECRVIPVar sVIPVar;

    vipNumber=ApplVar.ICCardSet.ECRVIPCount;

    GetTimeDate(&Now);              //* read timer
    ECRVIP.VipFix.VipDate=EncordDate(Now.year,Now.month,Now.day);
    if (vipNumber<ECRVIPMAX)
    {
#if defined(DEBUGBYPC)
        ECRVIPFix[vipNumber]=ECRVIP.VipFix;
#endif
        bFlashProgram(FLASH_ECRVIPFIX_ADDR+vipNumber*sizeof(struct  TECRVIPFix), sizeof(struct TECRVIPFix),(uint8_t *)&ECRVIP.VipFix);
        ECRVIP.ECRVIPNumber=vipNumber;
        ECRVIP.ECRVIPState=0;
        sVIPVar.VipPoints=0;
        sVIPVar.VipAmt=0;
        //将计点数据清零
        bFlashProgram(FLASH_ECRVIPVAR1_ADDR+vipNumber*sizeof(struct  TECRVIPVar), sizeof(struct TECRVIPVar),(uint8_t *)&sVIPVar);

        ApplVar.ICCardSet.ECRVIPCount++;

        vipNumber &= (ECRVIPMAX/2-1);
        if (ECRVIP.ECRVIPNumber<ECRVIPMAX/2 && ECRVIP.CurrentVIPVar==FLASH_ECRVIPVAR1_ADDR ||
            ECRVIP.ECRVIPNumber>=ECRVIPMAX/2 && ECRVIP.CurrentVIPVar==FLASH_ECRVIPVAR2_ADDR)
        {//同时更新已经读入到ECRVIP.VipVar中的数据
            ECRVIP.VipVar[vipNumber]=sVIPVar;
        }
#if defined(DEBUGBYPC)
        if (ECRVIP.ECRVIPNumber<ECRVIPMAX/2)
            ECRVIPVar1[vipNumber]=sVIPVar;
        else
            ECRVIPVar2[vipNumber]=sVIPVar;
#endif
    }
    else
        ApplVar.ErrorNumber=ERROR_ID(CWXXI90);
}

/**
 * 打印会员信息:号码,计点,总额(不打印名称)
 *
 * @author EutronSoftware (2016-05-17)
 *
 * @param vipCode :会员号码,=null时打印全部会员信息
 */
void PrintECRVIP(BYTE *vipCode)
{
    int i,j;
    struct TECRVIPVar   sECRVIPVar;
    char sTel[13];
    BYTE p=0;//标示是否打印了数据
    WORD sLogin;

    sLogin = TESTBIT(ECRVIP.ECRVIPState,VIP_LOGIN);
    if (!sLogin)
    {//没有会员登录时,提示输入会员号
        mClearScreen();
        DispStrXY(Msg[TMHYHAO].str,0,0);
        if (GetStrFrKBD('9',SCREENWD-7,0,12,0)>0)//(Appl_EntryCounter)
        {//ccr2016-06-02会员号只取9位,因此最大为999999999
            StrToBCDValue(ECRVIP.VipFix.VipTEL, &EntryBuffer[ENTRYSIZE - 2], sizeof(ECRVIP.VipFix.VipTEL));
            for (i=0;i<ECRVIPMAX && ECRVIPFix[i].VipDate!=0xffff;i++)
            {//有会员
                if (CompareBCDValue(ECRVIP.VipFix.VipTEL,(BYTE*)ECRVIPFix[i].VipTEL,sizeof(ECRVIP.VipFix.VipTEL))==0)
                {//找到会员
                    ECRVIP.ECRVIPNumber=i;
                    sLogin=VIP_LOGIN;
                    break;
                }
            }
        }
    }
    if (sLogin)
    {//指定会员打印格式
       p=1;
       i=ECRVIP.ECRVIPNumber;

       //名称
       memset(SysBuf,' ',PRTLEN);
       CopyFrStr(SysBuf,Msg[TMHYHAO].str);
       HEXtoASCX0(SysBuf+PRTLEN-13,(char*)ECRVIPFix[i].VipTEL,6);SysBuf[PRTLEN]=0;
       RJPrint(0,SysBuf);

       //会员号
       memset(SysBuf,' ',PRTLEN);
       CopyFrStr(SysBuf,Msg[MINCHENF].str);
       strncpy(SysBuf+PRTLEN-10,(char*)ECRVIPFix[i].VipName,8);SysBuf[PRTLEN]=0;
       RJPrint(0,SysBuf);

       //建档日期
       memset(SysBuf,' ',PRTLEN);
       CopyFrStr(SysBuf,cCREATEDATE);
       DeDateToStr(SysBuf+PRTLEN-11,(WORD)ECRVIPFix[i].VipDate);SysBuf[PRTLEN]=0;
       RJPrint(0,SysBuf);

       if (i<ECRVIPMAX/2)
           sECRVIPVar = ECRVIPVar1[i];
       else
           sECRVIPVar = ECRVIPVar2[i & (ECRVIPMAX/2-1)];

       //消费总金额
       memset(SysBuf,' ',PRTLEN);
       CopyFrStr(SysBuf,Msg[XFJE].str);
       sprintf(SysBuf+PRTLEN-11,"%7lu.%02d",sECRVIPVar.VipAmt/100,sECRVIPVar.VipAmt % 100);
       RJPrint(0,SysBuf);

       //消费积分
       memset(SysBuf,' ',PRTLEN);
       CopyFrStr(SysBuf,Msg[XFJIFEN].str);
       sprintf(SysBuf+PRTLEN-8,"%7lu",sECRVIPVar.VipPoints);
       RJPrint(0,SysBuf);
    }
#if defined(DEBUGBYPC)
    else
    {
        for (i=0;i<ECRVIPMAX && ECRVIPFix[i].VipDate!=0xffff;i++)
        {//有会员
            if (vipCode==NULL)
            {//全部会员打印格式
                if (p==0)
                {
                    p=1;
                    RJPrint(0,cECRVIPTITLE);
                }
                memset(SysBuf,' ',PRTLEN);
                HEXtoASC(SysBuf,(char*)ECRVIPFix[i].VipTEL,6);
                if (i<ECRVIPMAX/2)
                    sECRVIPVar = ECRVIPVar1[i];
                else
                    sECRVIPVar = ECRVIPVar2[i & (ECRVIPMAX/2-1)];
                sprintf(SysBuf+13,"%7lu.%02d%7lu",sECRVIPVar.VipAmt/100,sECRVIPVar.VipAmt % 100,sECRVIPVar.VipPoints);

                RJPrint(0,SysBuf);
            }
            else if (CompareBCDValue(vipCode,(BYTE*)ECRVIPFix[i].VipTEL,sizeof(ECRVIP.VipFix.VipTEL))==0)
            {//指定会员打印格式
                p=1;

                //名称
                memset(SysBuf,' ',PRTLEN);
                CopyFrStr(SysBuf,Msg[TMHYHAO].str);
                HEXtoASCX0(SysBuf+PRTLEN-12,(char*)ECRVIPFix[i].VipTEL,6);SysBuf[PRTLEN]=0;
                RJPrint(0,SysBuf);

                //会员号
                memset(SysBuf,' ',PRTLEN);
                CopyFrStr(SysBuf,Msg[MINCHENF].str);
                strncpy(SysBuf+PRTLEN-10,(char*)ECRVIPFix[i].VipName,8);SysBuf[PRTLEN]=0;
                RJPrint(0,SysBuf);

                //建档日期
                memset(SysBuf,' ',PRTLEN);
                CopyFrStr(SysBuf,cCREATEDATE);
                DeDateToStr(SysBuf+PRTLEN-11,(WORD)ECRVIPFix[i].VipDate);SysBuf[PRTLEN]=0;
                RJPrint(0,SysBuf);

                if (i<ECRVIPMAX/2)
                    sECRVIPVar = ECRVIPVar1[i];
                else
                    sECRVIPVar = ECRVIPVar2[i & (ECRVIPMAX/2-1)];

                //消费总金额
                memset(SysBuf,' ',PRTLEN);
                CopyFrStr(SysBuf,Msg[XFJE].str);
                sprintf(SysBuf+PRTLEN-11,"%7lu.%02d",sECRVIPVar.VipAmt/100,sECRVIPVar.VipAmt % 100);
                RJPrint(0,SysBuf);

                //消费积分
                memset(SysBuf,' ',PRTLEN);
                CopyFrStr(SysBuf,Msg[XFJIFEN].str);
                sprintf(SysBuf+PRTLEN-11,"%7lu",sECRVIPVar.VipPoints);
                RJPrint(0,SysBuf);
            }
        }
        ApplVar.ICCardSet.ECRVIPCount=i;
    }
#endif
    if (p)
    {
        RFeed(3);
        SETBIT(ApplVar.MyFlags,PRNHEADMUST);
    }
}
/**
 * 根据ECRVIP.ECRVIPNumber显示会员信息:名称,号码,计点,总额
 *
 * @author EutronSoftware (2016-05-19)
 */
void DisplayECRVIP()
{

#define sDispBuff ProgLineMes

    int i,j;
    struct TECRVIPVar   sECRVIPVar;
    //char sDispBuff[SCREENWD+1];

        i = ECRVIP.ECRVIPNumber;
        if (i<ECRVIPMAX)
        {
            mClearScreen();
            memset(sDispBuff,' ',sizeof(sDispBuff));
            //会员号
            ECRVIP.VipFix=ECRVIPFix[i];

            HEXtoASCX0(sDispBuff+SCREENWD-12,(char*)ECRVIP.VipFix.VipTEL,6);
            //名称
            for (j=0;j<sizeof(ECRVIP.VipFix.VipName);j++)
            {
                if (ECRVIP.VipFix.VipName[j]==0 || ECRVIP.VipFix.VipName[j]==0xff)
                    break;
            }
            if (j)
                memcpy(sDispBuff,(char*)ECRVIP.VipFix.VipName,j);sDispBuff[SCREENWD]=0;
            DispStrXY(sDispBuff,0,0);

            //建档日期
            //memset(sDispBuff,' ',sizeof(sDispBuff));
            //CopyFrStr(sDispBuff,cCREATEDATE);
            //DeDateToStr(sDispBuff+PRTLEN-11,(WORD)ECRVIPFix[i].VipDate);sDispBuff[PRTLEN]=0;
            //RJPrint(0,sDispBuff);

            sECRVIPVar = ECRVIP.VipVar[i & (ECRVIPMAX/2-1)];

            //消费总金额
            memset(sDispBuff,' ',sizeof(sDispBuff));
            CopyFrStr(sDispBuff,cVIPTOTAL);
            sprintf(sDispBuff+SCREENWD-9,"%6lu.%02d",sECRVIPVar.VipAmt/100,sECRVIPVar.VipAmt % 100);
            DispStrXY(sDispBuff,0,1);


            //消费积分
            memset(sDispBuff,' ',sizeof(sDispBuff));
            CopyFrStr(sDispBuff,Msg[XFJIFEN].str);
            sprintf(sDispBuff+SCREENWD-7,"%7lu",sECRVIPVar.VipPoints);
            DispStrXY(sDispBuff,0,2);
        }
}
/**
 * 根据ECRVIP.VipFix.VipTEL登录和显示会员信息:名称,号码,计点,总额
 *
 * @author EutronSoftware (2016-05-17)
 *
 * @param
 */
void ECRVIPLogin()
{
    int i,j;

    if (Appl_EntryCounter)
    {
        ECRVIP.ECRVIPState=0;
        memcpy(ECRVIP.VipFix.VipTEL,ApplVar.Entry.Value,sizeof(ECRVIP.VipFix.VipTEL));
        for (i=0;i<ECRVIPMAX && ECRVIPFix[i].VipDate!=0xffff;i++)
        {//有会员
            if (CompareBCDValue(ECRVIP.VipFix.VipTEL,(BYTE*)ECRVIPFix[i].VipTEL,sizeof(ECRVIP.VipFix.VipTEL))==0)
            {//指定会员打印格式
                ECRVIP.ECRVIPNumber=i;
                ReadECRVIPVar();
                DisplayECRVIP();
                ClearEntry(TRUE);
                SETBIT(ApplVar.MyFlags,CLSSCREEN);
                SETBIT(ECRVIP.ECRVIPState,VIP_LOGIN);//表示会员正确登录
                return;
            }
        }
        if (WaitForYesNo(Msg[SETADDNEWVIP].str,TRUE))//无此会员,是否增加会员
        {//增加新会员,输入会员名称
            mClearScreen();
            DispStrLF((BYTE*)cINPUTVIPNAME);
            RESETBIT(ApplVar.MyFlags,PWDMODE);
            if (GetStringCHN(SysBuf,sizeof(ECRVIP.VipFix.VipName),true)>0)
            {
                for (i=j=0;i<sizeof(ECRVIP.VipFix.VipName) && SysBuf[i];i++)
                {
                    if (j || SysBuf[i]!=' ')
                        ECRVIP.VipFix.VipName[j++]=SysBuf[i];
                }
                AddNewECRVIP();
                ECRVIP.ECRVIPNumber=ApplVar.ICCardSet.ECRVIPCount-1;
                ReadECRVIPVar();
                DisplayECRVIP();
                SETBIT(ECRVIP.ECRVIPState,VIP_LOGIN);//表示会员正确登录
            }
            ClearEntry(TRUE);
            SETBIT(ApplVar.MyFlags,CLSSCREEN);
            return;
        }
        else
            ApplVar.ErrorNumber=ERROR_ID(CWXXI91);//无此会员
    }
    else if (TESTBIT(ECRVIP.ECRVIPState,VIP_LOGIN))//表示有会员登录)
    {//退出登录
        ECRVIP.ECRVIPState=0;
        DispSt2C((char*)cVIPLOGOUT);//
    }
}

//------------------------------------------------------------------------------
/**
 * 根据付款金额对会员进行积分
 *
 * @author EutronSoftware (2016-05-22)
 *
 * @param pValue:付款金额;
 */
void PointsByECRVIP(BCD *pValue)
{
    BCD newValue;
    BCD oldValue;
    BYTE sUpdate;
    WORD iNum;
    ULONG sPoint,sAmt;

//    if (TESTBIT(ECRVIP.ECRVIPState,VIP_LOGIN+VIP_POINTS)==VIP_LOGIN && TESTBIT(ApplVar.ICCardSet.Options,IC_POINTS))
    {
            memset(&oldValue,0,sizeof(BCD));
            memcpy(oldValue.Value,&ApplVar.ICCardSet.Value,sizeof(ApplVar.ICCardSet.Value));//ccr090311

            memset(&newValue,0,sizeof(BCD));
            memcpy(newValue.Value,&ApplVar.ICCardSet.MiniNum,sizeof(ApplVar.ICCardSet.MiniNum));//ccr090311

            if (CheckNotZero(&oldValue) && CompareBCD(pValue,&newValue)>=0)//ccr090311
            {
                newValue = *pValue;
                Divide(&newValue, &oldValue);
                //ccr090311 trunc BCD>>>>>>>>>>>
                if (newValue.Sign & 1)
                    BcdDiv10(&newValue);
                newValue.Sign &= 0x06;
                if (newValue.Sign)
                {
                    sUpdate = newValue.Sign/2;
                    memcpy(newValue.Value,newValue.Value+sUpdate,BCDLEN-sUpdate);
                    memset(newValue.Value+BCDLEN-sUpdate,0,sUpdate);
                    newValue.Sign = 0;
                }
                //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

                BCDValueToULong(newValue.Value,&sPoint);//本次计点数
                BCDValueToULong(pValue->Value,&sAmt);//本次计点金额

                ECRVIP.VipPoints=sPoint;

                iNum = ECRVIP.ECRVIPNumber & (ECRVIPMAX/2-1);
                memcpy(ApplVar.FlowBuff.CDC_refreshic.RPoint,newValue.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RPoint));
                ECRVIP.VipVar[iNum].VipPoints+=sPoint;//累计点数

                memcpy(ApplVar.FlowBuff.CDC_refreshic.RValue,pValue->Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RValue));
                ECRVIP.VipVar[iNum].VipAmt+=sAmt;//累计金额
                WriteECRVIPVar();

                //累计计点和金额
                oldValue=ZERO;
                ULongToBCDValue(oldValue.Value,ECRVIP.VipVar[iNum].VipPoints);
                memcpy(ApplVar.FlowBuff.CDC_refreshic.CPoint,oldValue.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.CPoint));
                oldValue=ZERO;
                ULongToBCDValue(oldValue.Value,ECRVIP.VipVar[iNum].VipAmt);
                memcpy(ApplVar.FlowBuff.CDC_refreshic.CValue,oldValue.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.CValue));

                ApplVar.FlowBuff.CDC_refreshic.OpType=IC_OP_ADDPOINT;
                Collect_Data(REFRESHICLOG);

                SETBIT(ECRVIP.ECRVIPState,VIP_POINTS);

#if (0)//移动到付款后再打印
                memset(SysBuf,' ',PRTLEN);
#if (0)         //打印会员计点和消费总金额
                HEXtoASC(SysBuf+2,(char*)ECRVIPFix[ECRVIP.ECRVIPNumber].VipTEL,6);
                CopyFrStr(SysBuf,Msg[JIFEN].str);
                sprintf(SysBuf+PRTLEN-17,"%7lu.%02d%7lu",ECRVIP.VipVar[iNum].VipAmt/100,ECRVIP.VipVar[iNum].VipAmt % 100,ECRVIP.VipVar[iNum].VipPoints);
#else           //打印会员本次计点
                CopyFrStr(SysBuf,Msg[JIFEN].str);
                HEXtoASC(SysBuf+10,(char*)ECRVIPFix[ECRVIP.ECRVIPNumber].VipTEL,6);
                sprintf(SysBuf+PRTLEN-7,"%7lu",sPoint);
#endif
                RJPrint(0,SysBuf);
#endif
            }
    }
}
/**
 * 减除会员计点:计点兑奖 在会员登录的情况下调用此功能
 * @author EutronSoftware (2016-05-23)
 */
void ProgECRVIPPoints()
{
    BCD newValue;
    int iNum;
		ULONG sLong;

    switch (Appl_ProgLine)
    {
    case 1:
#if (SCRSTATDBL)
        DispOnStatDBL((char *)Msg[XFJDDJIANG].str,0);
#else
        DispSt1C(Msg[XFJDDJIANG].str);//RG-DISPLAY
#endif
        ApplVar.NumberEntered = ZERO;
        Appl_ProgLine=2;//Like Date
    case 2:
        Appl_MaxEntry=6;
        Appl_BitNumber = 0;
        GetBCDValue(ICKSDIAN,ApplVar.NumberEntered.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RPoint),TRUE);
        SETBIT(ECRVIP.ECRVIPState,VIP_DEC_ADD);//默认兑奖
        break;
    case 3:
        ClearLine(SCREENLN-2);
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        CheckBitValue(JDSDIAN,(char *)&ECRVIP.ECRVIPState,3,0);//VIP_DEC_ADD
        if (TESTBIT(ECRVIP.ECRVIPState,VIP_DEC_ADD))
            strcpy(ProgLineMes+SETINPUTWD-4,cDECVIPPOINT);
        else
            strcpy(ProgLineMes+SETINPUTWD-4,cADDVIPPOINT);

        break;
    case 4:
        Appl_MaxEntry=0;
        Appl_EntryCounter = 0;
        if (TESTBIT(ECRVIP.ECRVIPState,VIP_DEC_ADD))
            CheckBitValue(DJQREN,(char *)&ECRVIP.ECRVIPState,4,0);//VIP_CONFIRM
        else
            CheckBitValue(QRSDIAN,(char *)&ECRVIP.ECRVIPState,4,0);//VIP_CONFIRM
        break;
    case 5:
        if (TESTBIT(ECRVIP.ECRVIPState, VIP_CONFIRM))
        {
            iNum=ECRVIP.ECRVIPNumber & (ECRVIPMAX/2-1);
            BCDValueToULong(ApplVar.NumberEntered.Value,&sLong);
            ECRVIP.VipPoints=sLong;
            if (!TESTBIT(ECRVIP.ECRVIPState,VIP_DEC_ADD))
            {
                ECRVIP.VipVar[iNum].VipPoints+=ECRVIP.VipPoints;
                ApplVar.FlowBuff.CDC_refreshic.OpType=IC_OP_ADDPOINT;
            }
            else
            {//减积分
                ApplVar.FlowBuff.CDC_refreshic.OpType=IC_OP_DECPOINT;
                if (ECRVIP.VipVar[iNum].VipPoints>=ECRVIP.VipPoints)
                    ECRVIP.VipVar[iNum].VipPoints-=ECRVIP.VipPoints;
                else
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI65);
            }
            if (!ApplVar.ErrorNumber)
            {
                WriteECRVIPVar();

                newValue=ZERO;
                ULongToBCDValue(newValue.Value,ECRVIP.VipPoints);
                memcpy(ApplVar.FlowBuff.CDC_refreshic.RPoint,newValue.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.RPoint));

                memset(ApplVar.FlowBuff.CDC_refreshic.RValue,0,sizeof(ApplVar.FlowBuff.CDC_refreshic.RValue));

                //累计计点和金额
                newValue=ZERO;
                ULongToBCDValue(newValue.Value,ECRVIP.VipVar[iNum].VipPoints);
                memcpy(ApplVar.FlowBuff.CDC_refreshic.CPoint,newValue.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.CPoint));
                newValue=ZERO;
                ULongToBCDValue(newValue.Value,ECRVIP.VipVar[iNum].VipAmt);
                memcpy(ApplVar.FlowBuff.CDC_refreshic.CValue,newValue.Value,sizeof(ApplVar.FlowBuff.CDC_refreshic.CValue));
                Collect_Data(REFRESHICLOG);
                PrintECRVIP(0);
            }

        }
        Appl_MaxEntry=ENTRYSIZE-2;
        Appl_ProgLine = 0;
        ClsState1();
        if (ApplVar.CentralLock == (SETUPMG | MG))
        {
            ApplVar.CentralLock = MG;
            mClearScreen();
        }
        Bell(2);
        break;
    }
}

#endif
