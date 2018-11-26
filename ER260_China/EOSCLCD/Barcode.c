#define BARCODE_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

#if 0
CONST BYTE digbar [10][3] = {
/*      set A, set B, set C             digit           code (7 bit) <-- 0 */
	{0x0d, 0x27,  0x72},   /* '0' -> 0001101, 0100111, 1110010 */
	{0x19, 0x33,  0x66},   /* '1' -> 0011001, 0110011, 1100110 */
	{0x13, 0x1b,  0x6c},   /* '2' -> 0010011, 0011011, 1101100 */
	{0x3d, 0x21,  0x42},   /* '3' -> 0111101, 0100001, 1000010 */
	{0x23, 0x1d,  0x5c},   /* '4' -> 0100011, 0011101, 1011100 */
	{0x31, 0x39,  0x4e},   /* '5' -> 0110001, 0111001, 1001110 */
	{0x2f, 0x05,  0x50},   /* '6' -> 0101111, 0000101, 1010000 */
	{0x3b, 0x11,  0x44},   /* '7' -> 0111011, 0010001, 1000100 */
	{0x37, 0x09,  0x48},   /* '8' -> 0110111, 0001001, 1001000 */
	{0x0b, 0x17,  0x74},   /* '9' -> 0001011, 0010111, 1110100 */
/* Guard Pattern 101, center pattern 01010 */
};

CONST BYTE dig13[10] = {
/*      13th digit is formed through pattern of digit 7 to 12
	according to following table, 0 = set A, 1 = set B,
	first 6 digit always use set C (right to left)
	     digit 12 11 10  9  8  7
*/
	0x00,   /*  A, A, A, A, A, A -> 13th digit 0 */
	0x0b,   /*  A, A, B, A, B, B -> 13th digit 1 */
	0x0d,   /*  A, A, B, B, A, B -> 13th digit 2 */
	0x0e,   /*  A, A, B, B, B, A -> 13th digit 3 */
	0x13,   /*  A, B, A, A, B, B -> 13th digit 4 */
	0x19,   /*  A, B, B, A, A, B -> 13th digit 5 */
	0x1c,   /*  A, B, B, B, A, A -> 13th digit 6 */
	0x15,   /*  A, B, A, B, A, B -> 13th digit 7 */
	0x16,   /*  A, B, A, B, B, A -> 13th digit 8 */
	0x1a,   /*  A, B, B, A, B, A -> 13th digit 9 */
};
#endif
int CCDCounter=0;
BYTE CCDBuff[CCDBUFMAX];

/***************************************************************/

/*
    IN STORE MARKING active when ARTLEVEL bit 3 is set

	    format FFRRRRRAAAAAC fixed 13 digits when BARCODE BIT 7 not set
		then
			 ONLY  FF = 21 Identifier, for Dansk it is 20
		else when BARCODE BIT 7 is set also
			FF = 02, 20, 26, 28


		When BARCODE BIT 6 is set also the weight can be decoded

		FF = 02, 20, 21, 26, 28 then format FFRRRRRAAAAAC
		FF = 25, 27, 29 the format FFRRRRRWWWWWC

    RRRRR = Random ApplVar.Plu Number
    AAAAA = Amount (qty is 1)
	WWWWW = ApplVar.Qty (xx.xxx fixed)
    C = check digit

	IN STORE MARKING for magazines is active when bit 4 of BARCODE is set
	format is fixed to FFFRRRRAAAAAC fixed 13 digits
	FFF = 379 Identifier
	RRRR = Standard Random ApplVar.Plu Number
	379RRRR = Random ApplVar.Plu number for Celigny France (BSF == 0)
	AAAAA = Amount
	ApplVar.Qty is ONE or ApplVar.Qty which is entered


	ApplVar.FBarcode
	1                       Standard Random Code
	2                       Code & ApplVar.Price for Presse (France) 379####PPPPPC
	3                       Code & ApplVar.Price XX#####PPPPPC
	4                       Code & Weight XX#####WWWWWC
	5                       depart, cost price, sales price  ###CCCCCPPPPPP
#if BARCUSTOMER==1
	6						Barcode customer 999CCCCDDDDPV
#endif
*/

void CheckInStore()
{
	BYTE id;

	if (TESTBIT (ART_LEVEL, BIT3) && (Appl_EntryCounter == 13 || Appl_EntryCounter == 12))
		/* In Store Marking ? */
	{
		if (Appl_EntryCounter == 12) /* if length 12 then EAN 13 with */
		{                                               /* preceding ZERO, add preceding ZERO */
	    	EntryBuffer[ENTRYSIZE - 14] = '0';
		}
		id = ((EntryBuffer[ENTRYSIZE - 14] - '0') * 0x10) +
			   (EntryBuffer[ENTRYSIZE - 13] - '0');

		switch(id)
		{
			case 0x02:
			case 0x20:
			case 0x26:
			case 0x28:
				if (!TESTBIT(BARCODE, BIT7))
					break;
			case 0x21:
				ApplVar.FBarcode = 3;
				break;
			case 0x37:
				if (TESTBIT(BARCODE, BIT4) && EntryBuffer[ENTRYSIZE - 12] == '9')
					ApplVar.FBarcode = 2;
				break;
			case 0x25:
			case 0x27:
			case 0x29:
				if (TESTBIT(BARCODE, BIT6))
					ApplVar.FBarcode = 4;
				break;
#if (BARCUSTOMER==1)
			case 0x99:
				if (EntryBuffer[ENTRYSIZE - 12] == '9')
					ApplVar.FBarcode = 6;
				break;
#endif
			default:
				break;
		}
		if (ApplVar.FBarcode > 1)
		{
			ApplVar.Entry = ZERO;
#if (BARCUSTOMER==1)
			if (ApplVar.FBarcode!=6)
#endif
			{
				ApplVar.Entry.Value[0] = (EntryBuffer[ENTRYSIZE - 3] - '0')+
					((EntryBuffer[ENTRYSIZE - 4] - '0') * 0x10);
			    ApplVar.Entry.Value[1] = (EntryBuffer[ENTRYSIZE - 5] - '0')+
			    	((EntryBuffer[ENTRYSIZE - 6] - '0') * 0x10);
			    ApplVar.Entry.Value[2] =
			    	(EntryBuffer[ENTRYSIZE - 7] - '0');
				if (ApplVar.FBarcode == 4)      /* Weight xx.xxx in code */
				{
					ApplVar.Qty = ApplVar.Entry;
					ApplVar.Qty.Sign = 0x03;
					ApplVar.MultiplyCount = 1;
				}
				else if (!ApplVar.FPrice)
				{
					ApplVar.Price = ApplVar.Entry;
				    ApplVar.FPrice = 1;
				}
				if (ApplVar.FBarcode == 2)      /* price encoded on magazine ?*/
					Appl_EntryCounter = 7;
				else
				{
					EntryBuffer[ENTRYSIZE - 13] = ' ';
					EntryBuffer[ENTRYSIZE - 14] = ' ';
					if (ApplVar.FBarcode != 4)
						ApplVar.MultiplyCount = 0; /* reset multiply count */
					Appl_EntryCounter = 5;
				}
				for (id = 0; id < 7; id++)
			    {
				    EntryBuffer[ENTRYSIZE - 2 - id] =
					    EntryBuffer[ENTRYSIZE - 8 - id];
				    EntryBuffer[ENTRYSIZE - 8 - id] = ' ';
			    }
			}
			StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
		}
    }
}

///////////////////////////////////////////////////////////////////////////
extern BYTE USB_Detect(void);//检测USB设备
extern BYTE CheckUSBKey(void);//检测USB键盘是否有输入
extern BYTE GetUSBKey(BYTE clean);//获取USB键盘输入

WORD BarCode()
{
    BYTE bport,sOK;
    short c;

	bport = BARCODE & 0x07;

    if (ApplVar.ErrorNumber
        || ApplVar.CentralLock==OFF
        || ApplVar.CentralLock==Z
#if (PLUCODE_X==0)
        || ApplVar.CentralLock==X
#endif
        || (ApplVar.CentralLock==SET && Appl_ProgType!=SETPLU)
        )
    {
#if !(defined(DEBUGBYPC) || defined(STM32F10X_HD))
        if (USB_Detect()==USB_KEYBOARD)
            GetUSBKey(true);
#endif
		return false;
    }

	sOK = FALSE;
	while (!sOK)
	{
        //先检查USB扫描枪
#if !(defined(DEBUGBYPC) || defined(STM32F10X_HD))
		if (USB_Detect()==USB_KEYBOARD && CheckUSBKey())
			c = GetUSBKey(false);
		else
#endif
        if (!bport || !CheckComm(bport-1) || (c = ReadComm(bport-1))==-1)//在检查串口扫描枪
				return false;
        c &= 0xff;
		CCDBuff[CCDCounter++] = c;
		if (CCDCounter>=CCDBUFMAX)
		{
			CCDCounter = 0;
			return false;
		}
		switch (c){
		case STX:
		case '$':
			CCDCounter = 0;
			return false;
		case LF:
			if (CCDBuff[CCDCounter-2] == CRET)
			{
				CCDCounter -=2;//Skip the end flag 0x0a
				CCDBuff[CCDCounter]=0;
			}
			else
			{
				CCDCounter --;//Skip the end flag 0x0a
				CCDBuff[CCDCounter]=0;
				if (!CCDCounter)
					return false;
			}
			sOK = TRUE;
			break;

		case CRET:				//End of a Bar code
		case 0x09:				//End of a Bar code
		case ETX:				//End of a Bar code
			CCDCounter --;
			CCDBuff[CCDCounter]=0;
			if (!CCDCounter)
				return false;
			sOK = TRUE;
			break;
        default:
            if (c<0x20 || c>0x7e)//去掉带电拔插时的干扰数据
            {
                CCDCounter --;//Skip the end flag 0x0a
                CCDBuff[CCDCounter]=0;
            }
            break;
		}
	}
    ClearEntry(false);
	InActive = 0;//Disable display datetime
    if (ApplVar.CentralLock == SET && !Appl_ProgType)
    {//Display BarCode only
//ccr2016-03-01		PutsO(CCDBuff);
	    RJPrint(0x02, CCDBuff);
#if (SCRSTATDBL)
        DispOnStatDBL(CCDBuff,0);
#else
		ClsState2();
		DispSt1C(CCDBuff);//RG-DISPLAY
#endif
		CCDCounter = 0;
		return false;
    }
	Appl_EntryCounter = CCDCounter;
	CCDCounter = 0;
    memcpy(EntryBuffer + ENTRYSIZE - Appl_EntryCounter - 1,CCDBuff, Appl_EntryCounter);
	ApplVar.FBarcode = 1;//standard BarCode
	CheckInStore();
    if (ApplVar.CentralLock != RG && ApplVar.CentralLock != MG)
    {//设置方式下,将输入的单品编码作为输入
     //输入的数据在EntryBuffer中
		ApplVar.Key.Code = 0;
        ApplVar.FPrice = 0;
//		ApplVar.KeyNo = ApplVar.AP.FirmKeys[ID_ENTER]; /* simulate number key */
//		mClearScreen();
//		CheckFirmKey();
#if (PLUCODE_X==1)
        if (ApplVar.CentralLock == X)
            KeyFrHost = PLU_Key_ID;
        else
#endif
            KeyFrHost = ApplVar.AP.FirmKeys[ID_ENTER];//模拟按确认键
    }
    else
    {//自动进行销售扫描枪读入的单品
    	ApplVar.OldKey.Code = 0;
	    ApplVar.Key.Code = PLU1;
        ProcessKey();
    }
    return true;
}

#if 0
#define CODE    (SysBuf+80)
#define BARS    (SysBuf+96)
#define BCHAR   (SysBuf+110)

void PrintBarCode(char dump, char *n)
{
	BYTE b, p, l, m;
	short i, c;

	MemSet(CODE, 16, '0');
	CopyFrStr(CODE+1, n);
	*(CODE) = '*';
	*(CODE+14) = '*';
	*(CODE+15) = 0;         /* terminate */
	c = 0;
/* calculate check digit
	1 - add all even positions (2, 4, 6, 8, 10, 12 starting from right)
		in code.
	2 - Multiply by 3.
	3 - add all odd positions (3, 5, 7, 9, 11, 13 starting from right)
		in code.
	4 - check digit is number which should be added to the result to get
		a multiple of 10.
*/
	for( i = 12; i > 1; i-=2)               /* 1 */
		c += (*(CODE+i) & 0x0f);
	c *= 3;                                         /* 2 */
	for( i = 11; i > 0; i-=2)               /* 3 */
		c += (*(CODE+i) & 0x0f);
	c %= 10;                                        /* 4 */
	if (c)
		c = 10 - c;
	*(CODE+13) = c + '0';
	b = 0x01;
	for (i = 13; i > 1; i--)
	{
		p = *(CODE+i) & 0x0f;
		if (i > 7)
			*(BARS+i) = digbar[p][2];
		else
		{
			if (dig13[*(CODE+1) & 0x0f] & b)
				*(BARS+i) = digbar[p][1];
			else
				*(BARS+i) = digbar[p][0];
			b <<= 1;
		}
	}
	RFeed(1);
	MemSet(SysBuf, 8, 1);
	SysBuf[8] = ' ';
	*(BCHAR) = 0xff;                /* start pattern */
	*(BCHAR+1) = 0x00;
	*(BCHAR+2) = 0xff;
	l = 3;
	c = 3;
	m = 0;                          /* line counter for trailer text */
	for (i = 13; i > 1; i--)
	{
		b = 0x01;
		for(p = 0; p < 7; p++)
		{
			if (*(BARS+i) & b)              /* black ? */
				*(BCHAR+l) = 0xff;
			else
				*(BCHAR+l) = 0x00;
			b <<= 1;
			l++;
			c++;
			if (l > 8)              /* 9 lines in 1 character */
			{
				if (m < 10)             /* only 5 lines */
				{
						memcpy(SysBuf+9, ApplVar.TXT.Trailer[m/2], 16);
				}

			    if(RJPRN && (ApplVar.CentralLock == RG || ApplVar.CentralLock == MG))
			    {}
			    else
					RJPrint(0x0e, SysBuf);
				l = 0;
				m++;
			}
			if (c == 45)            /* center pattern ?*/
			{
				*(BCHAR) = 0x00;
				*(BCHAR+1) = 0xff;
				*(BCHAR+2) = 0x00;
				*(BCHAR+3) = 0xff;
				*(BCHAR+4) = 0x00;
				c += 5;
				l += 5;
			}
		}
	}
	*(BCHAR+l) = 0xff;
	*(BCHAR+l+1) = 0x00;
	*(BCHAR+l+2) = 0xff;
	l += 3;
	while(l < 9)
	{
		*(BCHAR+l)=0;
		l++;
	}
	memcpy(SysBuf+9, CODE, 16);

    if(RJPRN && (ApplVar.CentralLock == RG || ApplVar.CentralLock == MG))
    {}
    else
		RJPrint(0x0e, SysBuf);
	if (dump)
		RFeed(7);
}
#endif
