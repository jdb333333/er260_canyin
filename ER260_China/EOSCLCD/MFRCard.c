//MFRCard.c-------------------

BYTE MFRBuffer[32];
BYTE BlockStart,BlockCnt;

CONST BYTE MFRKeyA[6] = {'E','U','T','R','O','N'};					//ApplVar.Key A
CONST BYTE MFRKeyA1[6] = {0xff,0xff,0xff,0xff,0xff,0xff};		//ApplVar.Key Acc 20070825
CONST BYTE MFRKeyB[6] = {0xff,0xff,0xff,0xff,0xff,0xff};		//ApplVar.Key B
//------------------------------------------------------------------------------
//extern CONST BYTE TTbPswd[10][3];// Table of passwords
//----------------------------------------------------------------------------
//extern CONST BYTE TbCript[40];// Table of codified areas decodification
//----------------------------------------------------------------------------
BYTE CC_ReadMFR(char *Dest,BYTE ChipAdr)
{
	int i,j,retry;
	short bytesRead;
	BYTE sum;

	GetValue(ChipAdr);

	for(j=0;j<BlockCnt;j++)
	{
		memset(MFRBuffer,0,sizeof(MFRBuffer));
   		MFRBuffer[0] = CommandBit;
   		MFRBuffer[1] = CMD_Read_Block;
   		MFRBuffer[2] = BlockStart + j;

		if(ChipAdr == RW_Facpre || ChipAdr == RW_Valpre)
			memcpy(&MFRBuffer[3],&MFRKeyA1[0],sizeof(MFRKeyA1));
		else
			memcpy(&MFRBuffer[3],&MFRKeyA[0],sizeof(MFRKeyA));
		memcpy(&MFRBuffer[9],&MFRKeyB[0],sizeof(MFRKeyB));

   		for(i=2,sum=0;i<MFRBuffer[1]+1;i++)
       		sum += MFRBuffer[i];
   		MFRBuffer[MFRBuffer[1]+1] = sum;
		EmptyComm(MFRPORT-1);
		Bios_PortWrite(MFRPORT,&MFRBuffer,16,0);
		//ccr2014-08-27 从串口读取射频卡模块19字节的回送数据
		for(retry=0,bytesRead=0;retry<10 && bytesRead<19;retry++)
		{
   			bytesRead+=Bios_PortRead(MFRPORT,&MFRBuffer[bytesRead],19-bytesRead, 50,0);
		}

       	for(i=2,sum=0;i<18;i++)
			sum += MFRBuffer[i];
       	if(MFRBuffer[0] != CommandBit ||MFRBuffer[1] != REC_Read_Block || MFRBuffer[18] != sum || bytesRead<19)
       	{
       		sum = 0;
			memset(Dest,0,sizeof(Dest));
			break;
       	}
		else
		{
			sum = 1;
			memcpy(&Dest[j*MFRBLOCKSIZE],&MFRBuffer[2],MFRBLOCKSIZE);
		}
	}
	return sum;
}
//---------------------------------------------------------------------------
BYTE CC_WriteMFR(char *Source,BYTE ChipAdr)
{
	int i,j;
	BYTE sum = 0;

	GetValue(ChipAdr);


	for(j=0;j<BlockCnt;j++)
	{
		memset(MFRBuffer,0,sizeof(MFRBuffer));
    	MFRBuffer[0] = CommandBit;
    	MFRBuffer[1] = CMD_Write_Block;
		MFRBuffer[2] = BlockStart + j;

		if(ChipAdr == RW_Facpre || ChipAdr == RW_Valpre)
			memcpy(&MFRBuffer[3],&MFRKeyA1[0],sizeof(MFRKeyA1));
		else
			memcpy(&MFRBuffer[3],&MFRKeyA[0],sizeof(MFRKeyA));

		memcpy(&MFRBuffer[9],&MFRKeyB[0],sizeof(MFRKeyB));
		memcpy(&MFRBuffer[15],&Source[j*MFRBLOCKSIZE],MFRBLOCKSIZE);

   		for(i=2,sum=0;i<MFRBuffer[1]+1;i++)
       		sum += MFRBuffer[i];

   		MFRBuffer[MFRBuffer[1]+1] = sum;
		EmptyComm(MFRPORT-1);
		Bios_PortWrite(MFRPORT,&MFRBuffer,32,0);
		for(sum=10;sum;sum--)
		{
			Bios_PortRead(MFRPORT,&MFRBuffer,3, 50,0);
			if(MFRBuffer[0] == CommandBit && MFRBuffer[1] == REC_Write_Block1 && MFRBuffer[2] == REC_Write_Block2)
				break;
		}
//		if(!sum)
//			break;//ccr090622
	}
	return sum;
}
//---------------------------------------------------------------------------
/**
 * 测试射频卡是否放置在读卡器上
 *
 * @author EutronSoftware (2014-08-22)
 * @return BYTE :true-有射频卡;false-无射频卡
 */
BYTE CC_Request()
{

	int i,retry;
	short bytesRead;
	BYTE sum;

	MFRBuffer[0] = CommandBit;
	MFRBuffer[1] = CMD_Read_SN1;
	MFRBuffer[2] = CMD_Read_SN2;

	//if(!TESTBIT(ApplVar.IC.ICState,IC_INSERT))
	{
		EmptyComm(MFRPORT-1);
		Bios_PortWrite(MFRPORT,&MFRBuffer,3,0);
		//读取7字节的应答数据
		for(retry=0,bytesRead=0;retry<5 && bytesRead<7;retry++)
		{
   			bytesRead+=Bios_PortRead(MFRPORT,&MFRBuffer[bytesRead],7-bytesRead, 50,0);
		}

		for(i=2,sum=0;i<6;i++)
       		sum += MFRBuffer[i];
       	if(MFRBuffer[0] != CommandBit || MFRBuffer[1] != REC_Read_SN || MFRBuffer[6] != sum || bytesRead<7)
       	{
			RESETBIT(ApplVar.IC.ICState,IC_INSERT | IC_NEWCARD);
			ApplVar.IC.CHIP_Flag = IC_NULLERR;
			return 0;
       	}
		else
			return 1;
	}
}

//---------------------------------------------------------------------------
/** ccr2014-08-26
 * 读取射频卡是否放置在读卡器上
 *
 * @author EutronSoftware (2014-08-22)
 * @return BYTE :true-有射频卡;false-无射频卡
 */
BYTE CC_MFRCard(void)
{
#if MFR_CTS
	//通过串口的CTS信号线检测射频卡
    if (GetCTS(MFRPORT))
        return 1;
    else
    {
        RESETBIT(ApplVar.IC.ICState,IC_INSERT | IC_NEWCARD);
        ApplVar.IC.CHIP_Flag = IC_NULLERR;
        return 0;
    }

#else
	//通过命令检测射频卡
	MFRBuffer[0] = CommandBit;
	MFRBuffer[1] = CMD_Read_MFR1;
	MFRBuffer[2] = CMD_Read_MFR2;


	{
		EmptyComm(MFRPORT-1);
		Bios_PortWrite(MFRPORT,&MFRBuffer,3,0);
        MFRBuffer[0]=MFRBuffer[1]=MFRBuffer[2] = 0;
		Bios_PortRead(MFRPORT,&MFRBuffer,3,25,0);

       	if(MFRBuffer[0] != CommandBit || MFRBuffer[1] != CMD_Read_MFR1 || MFRBuffer[2] <= 1)
       	{
			RESETBIT(ApplVar.IC.ICState,IC_INSERT | IC_NEWCARD);
			ApplVar.IC.CHIP_Flag = IC_NULLERR;
			return 0;
       	}
		else
			return 1;
	}
#endif
}
//-----------------------------------------------------------------------------
void GetValue(BYTE ChipAdr)
{
	switch(ChipAdr)
	{
		case RW_Factory://Sec 0:1Block
			BlockStart = 1;
			BlockCnt = 1;
			break;
		case RW_Init:		//Sec0:2Block
			BlockStart = 2;
			BlockCnt = 1;
			break;
		case RW_Customer:	//Sec1:0-1Block
			BlockStart = 4;
			BlockCnt = 2;
			break;
		case RW_Valatt:		//Sec1:2Block
			BlockStart = 6;
			BlockCnt = 1;
			break;
		case RW_Valpre:		//Sec2:0Block
			BlockStart = 8;
			BlockCnt = 1;
			break;
		case RW_Facpre:		//Sec2:1Block	//cc 20070825
			BlockStart = 9;
			BlockCnt = 1;
			break;
	}
}

//------------------------------------------------------------------------------
/*
BYTE SendRecordBySPI(BYTE *p,BYTE length)
{
	int sLp;

	for(sLp=0;sLp<length;sLp++)
	{
		if(!SendByteBySPI(p[sLp]))
			break;
		//SendByteBySPI(p[sLp]);
	}

	if(sLp == length)
		return 1;
	else
		return 0;
}
//------------------------------------------------------------------------------
BYTE ReadRecordBySPI(BYTE *p,BYTE length)
{
	int sLp;
	WORD sRead;

	for(sLp=0;sLp<length;sLp++)
	{
		if((sRead = ReadByteBySPI()) >= 1)
			p[sLp] = sRead & 0xff;
		else
			break;
		//sRead = ReadByteBySPI();
		//p[sLp] = sRead & 0xff;
	}

	if(sLp == length)
		return 1;
	else
		return 0;
}
//------------------------------------------------------------------------------
BYTE SendByteBySPI(BYTE p)
{
//	return SIO4_TX(p);
	return 0;
}
//------------------------------------------------------------------------------
WORD ReadByteBySPI()
{
//	return SIO4_RX();
	return 0;
}
//------------------------------------------------------------------------------
*/
#if 0//ccr2014-08-22>>>>>>>>>>>>>>>>>>>
	static int RFLen=0;
	static ULONG RFDelay=0;
	short i;
	BYTE  RFCh,RFCRC;

	//if(!TESTBIT(ApplVar.IC.ICState,IC_INSERT))
	{
		if(RFDelay==0)
		{//发送检测命令
			RFDelay = 1;
			RFLen = 0;
			MFRBuffer[0] = CommandBit;
			MFRBuffer[1] = CMD_Read_SN1;
			MFRBuffer[2] = CMD_Read_SN2;

			Bios_PortWrite(MFRPORT,&MFRBuffer,3,0);
		}


		while(1)
		{
//			if (CheckComm(MFRPORT-1) && (i = ReadComm(MFRPORT-1))!=-1)
            if ((i = ReadComm(MFRPORT-1))!=-1)
			{
				RFDelay = 1;
				RFCh = i & 0xff;
				MFRBuffer[RFLen++] = RFCh;
				if(RFLen==7)
				{//收到完整的命令

					RFDelay=0;//重新发送检测命令
					for(i=2,RFCRC=0;i<6;i++)
						RFCRC += MFRBuffer[i];
					if(MFRBuffer[0] != CommandBit || MFRBuffer[1] != REC_Read_SN || MFRBuffer[6] != RFCRC)
					{//无卡
						RESETBIT(ApplVar.IC.ICState,IC_INSERT | IC_NEWCARD);
						ApplVar.IC.CHIP_Flag = IC_NULLERR;
						return false;
					}
					else//有卡
						return true;
				}
			}
			else
			{
				RFDelay++;
				if(RFDelay>=(5000+25*PROG1ms*wait))
				{
					RFDelay=0;//重新发送检测命令
					return false;//无卡
				}
				else if (!wait)
					return TESTBIT(ApplVar.IC.ICState,IC_INSERT);//当前有卡状态
			}
		}
	}
#endif//<<<<<<<<<<<<<<<<<<<<<<

