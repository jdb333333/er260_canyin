#include "king.h"
#include "exthead.h"
#include "exth.h"

#if (CASE_EPOS)

#include "ePosCard.h"

TePOSRECORD ePosLastRec;


/* 从ePos读取一个字节，如果有错，可以按清除键强制结束  */
BYTE ReadePos(BYTE *frePos)
{
  	short fr,lp;

	for (lp=0;lp<5;lp++)
	{

		if(KbHit() && Getch() == ApplVar.AP.FirmKeys[ID_CLEAR])
				return false;

		fr = ReadComm(ApplVar.ePosPort-1);
		if (fr != -1)
		{
			*frePos = fr & 0xff;
			return true;
		}
	}
	return false;
}

void StringToBytes( BYTE *strSource, BYTE *ucTarget )
{
	WORD i;
	BYTE sByte,sByte1;

	for( i = 0; i < sizeof(TePOSRECORD); i++ )
	{
		sByte = *strSource ;
		switch( sByte & 0xF0)
		{
		case 0x30:
			sByte1 = (sByte & 0x0F);
			break;
		case 0x40:
		case 0x60:
			sByte1 = (( sByte & 0x07 ) + 0x09);
			break;
		default:
			sByte1 = 0x00;
		}
		sByte1 <<= 4;
		strSource++;
		sByte = *strSource;
		switch( sByte  & 0xF0)
		{
		case 0x30:
			sByte1 |=  ( sByte & 0x0F );
			break;
		case 0x40:
		case 0x60:
			sByte1 |=  ( ( sByte & 0x07 ) + 0x09 );
			break;
		}
		strSource++;
		*ucTarget = sByte1;
		ucTarget++;
	}
}

//---------------------------------------------------
signed char RecvACK()
{
	BYTE Resp[2],lp=0;

	Resp[1] = 0x00;
	do
	{
		Resp[0] = Resp[1];
		if(!ReadePos(Resp+1) || (lp++)>10)
			return FAILURE;
	}while( ( Resp[0] != DLE ) || ( Resp[1] != ACK ) );

	return SUCCESS;
}
//---------------------------------------------------------------
void SendENQ()
{
	BYTE ucENQ[2];

	ucENQ[0] = DLE;
	ucENQ[1] = ENQ;

	Bios_PortWrite(ApplVar.ePosPort, ucENQ, 2, 1);
}
//---------------------------------------------------------------
signed char RecvEOT()
{
	BYTE Resp[2],lp=0;

	Resp[1] = 0x00;
	do
	{
		Resp[0] = Resp[1];
        if(!ReadePos(Resp+1) || (lp++)>10)
        	return FAILURE;
	}while( ( Resp[0] != DLE ) || ( Resp[1] != EOT ) );

	return SUCCESS;
}
//---------------------------------------------------------------
void SendEOT()
{
	BYTE ucEOT[2];

	ucEOT[0] = DLE;
	ucEOT[1] = EOT;

	Bios_PortWrite(ApplVar.ePosPort, ucEOT, 2, 1);
}
//---------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  接收截取数据包中的信息                                                   //
//                                                                           //
//  BYTE OMLRecvData( BYTE *RealMessage, WORD RLen )                         //
//                                                                           //
//  RealMessage	数据包中的有效数据                                           //
//	RLen		数据包的期望长度，可以为RealMessage的大小，0表示按ETX接收    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
signed char RecvData(BYTE *RealMessage, WORD *RLen )
{
	short i, PLen;
	BYTE VByte, PByte;

	PLen = 0;
	VByte = 0x00;
	for(i=0;i<10;i++)
	{
        if (!ReadePos(&PByte))
			return FAILURE;

		if (VByte==DLE && PByte==STX)
			break;
		else if (VByte==DLE && PByte==NAK)
			return RETRY;
		else
			VByte = PByte;
	}
	if (i>=10)
		return FAILURE;

	PLen = 0;
	while( 1 )
	{
		if (PLen>*RLen || !ReadePos(RealMessage+PLen))
			return FAILURE;

		if( RealMessage[PLen] == DLE )
		{
            if (!ReadePos(RealMessage+PLen + 1))
				return FAILURE;

			if( RealMessage[PLen + 1] == ETX )
				break;
			else if( RealMessage[PLen + 1] == DLE )
				{ PLen++; continue; }
			else PLen += 2;
		}
		else PLen++;
	}

    if(!ReadePos(&PByte))
		return FAILURE;

	VByte = 0x00;		/* Check Bit */
	for ( i = 0; i < PLen; i++ )
		VByte ^= RealMessage[i];
	VByte ^= ETX;

	if ( VByte != PByte )
		return CHECK_ERROR;

	*RLen = PLen;

	return SUCCESS;
}
//---------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  打包发送数据                                                             //
//                                                                           //
//  void OMLSendData( BYTE *RealMessage, short RLen )                          //
//                                                                           //
//  入口参数：RealMessage	发送的数据                                       //
//  	      RLen			发送的数据长度                                   //
//  出口参数：DataPacket	打包后的数据                                     //
//  返 回 值：PLen			数据包长度                                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
void SendDataePos(BYTE *RealMessage, short RLen )
{
	short i, PLen;
	BYTE VByte;
	BYTE DataPacket[24];

	DataPacket[0] = DLE;
	DataPacket[1] = STX;

	PLen = 2;
	VByte = 0x00;		/* Check Bit */
	for( i = 0; i < RLen; i++ )
	{
		if( RealMessage[i] == 0x10 )
		{
			DataPacket[PLen++] = RealMessage[i];
			DataPacket[PLen++] = RealMessage[i];
		}
		else
		{
			DataPacket[PLen++] = RealMessage[i];
		}
		VByte ^= RealMessage[i];
	}
	VByte ^= ETX;

	DataPacket[PLen++] = DLE;
	DataPacket[PLen++] = ETX;
	DataPacket[PLen++] = VByte;

	Bios_PortWrite(ApplVar.ePosPort, DataPacket, PLen, 1);
}
//---------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Command Format( HOST -> C/R )                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
signed char SendCMD(char *CMD, BYTE *Parameter, WORD Len, BYTE *Response, WORD *RespLen )
{
	BYTE ucSendBuffer[30];//, ucRecvBuffer[120];
	WORD i, lResult, lSendLength, lRecvLength,maxLen=*RespLen;


	ucSendBuffer[0] = 0x43;		// 'C'
	ucSendBuffer[1] = CMD[0];
	ucSendBuffer[2] = CMD[1];

	if( Len != 0 )
		memcpy(	ucSendBuffer + 3, Parameter, Len );

	lSendLength = Len + 3;

	EmptyComm(ApplVar.ePosPort-1);

	for( i = 0; i < 3; i++ )
	{
		SendENQ();

		if( RecvACK() == SUCCESS )
				break;
	}
	if (i>=3)
		return FAILURE;

	EmptyComm(ApplVar.ePosPort-1);

	while( 1 )
	{
		SendDataePos(ucSendBuffer,lSendLength);
        lRecvLength = maxLen;
		if( ( lResult = RecvData(SysBuf, &lRecvLength ) ) == SUCCESS )
			break;
		else if( lResult == RETRY )
			continue;
		else
			return FAILURE;
	}

	switch( SysBuf[0] )
	{
	case 'N':
		*RespLen = 0;
		return CMD_NEGATIVE;
	case 'P':
		memcpy( Response, SysBuf + 5, lRecvLength - 5 );	//Return Response data
		*RespLen = lRecvLength - 5;
		return CMD_POSITIVE;
	default:
		*RespLen = 0;
		return FAILURE;
	}
}
//------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  取POS功能状态                                                            //
//                                                                           //
//  BYTE GetPosStatus( HANDLE PortHandle, WORD *Status )                     //
//                                                                           //
//  入口参数：                                                               //
//		PortHandle;  端口句柄                                                //
//  出口参数：                                                               //
//		Status:	系统返回的POS状态                                            //
//			0	系统空闲                                                     //
//			1	等待读卡                                                     //
//			2	读卡完成，可以确认交易                                       //
//			3	卡余额不足                                                   //
//			4 	卡已经过期                                                   //
//			5	黑名单卡                                                     //
//			6	非法卡                                                       //
//			7	损坏卡                                                       //
//			8	交易完成，等待取卡                                           //
//  返 回 值：                                                               //
//		0	正常                                                             //
//  	<>0	失败                                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
signed char GetPosStatus(BYTE *Status )
{
	WORD ulRespLen;
	BYTE ucRespData[20];

	ulRespLen = 15;
	switch( SendCMD("00", NULL, 0, ucRespData, &ulRespLen ) )
	{
	case CMD_POSITIVE:
		break;
	case CMD_NEGATIVE:
	default:
		return FAILURE;
	}

	*Status = ( ucRespData[0] - 0x30 ) * 10 + ( ucRespData[1] - 0x30 );

	return SUCCESS;
}
//------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  取交易的卡号                                                             //
//                                                                           //
//  BYTE GetTransCardNo( HANDLE PortHandle, BYTE *CardNo )                   //
//                                                                           //
//  入口参数：                                                               //
//		PortHandle;  端口句柄                                                //
//  出口参数：                                                               //
//		CardNo; 10 位为卡面号                                                //
//  返 回 值：                                                               //
//		0	正常                                                             //
//		-1	系统超时                                                         //
//		1	系统忙                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
/*
BYTE GetTransCardNo(BYTE *CardNo )
{
	WORD ulRespLen;
	BYTE ucRespData[20];

	ulRespLen = 18;
	switch( SendCMD("01", NULL, 0, ucRespData, &ulRespLen ) )
	{
	case CMD_POSITIVE: break;
	case CMD_NEGATIVE:
	default: return FAILURE;
	}

	memset( CardNo, 0x00, 11 );
	memcpy( CardNo, ucRespData, ulRespLen );

	return SUCCESS;
}
*/
//------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  POS从收银机获取金额，然后等待用户刷卡                                    //
//                                                                           //
//  BYTE UpdateTransAmount( HANDLE PortHandle, WORD *Amount )                //
//                                                                           //
//  入口参数：                                                               //
//		PortHandle;  端口句柄                                                //
//  出口参数：                                                               //
//		Amount;	收银机收取金额                                               //
//  返 回 值：                                                               //
//		0	正常                                                             //
//		-1	失败                                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
signed char UpdateTransAmount(BCD *Amount )
{
	WORD ulRespLen;
	BYTE sSing = Amount->Sign;
	BYTE ucParamData[40], ucRespData[40];

	memset(ucParamData,'0',sizeof(ucParamData));
	Amount->Value[7] = Amount->Value[6] = Amount->Value[5] = Amount->Value[4] = 0;
	Amount->Sign &= 0x7f;
	FormatQty(ucParamData+7, Amount);
	Amount->Sign &= sSing;

	ulRespLen = 15;
	switch( SendCMD("02", ucParamData, 8, ucRespData, &ulRespLen ) )
	{
	case CMD_POSITIVE:
		break;
	case CMD_NEGATIVE:
	default:
		return FAILURE;
	}

	return SUCCESS;
}
//------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  通过获取POS状态，如果已经读卡正常，可以确认此交易                        //
//                                                                           //
//  BYTE UpdateTransComfirm( HANDLE PortHandle )                             //
//                                                                           //
//  入口参数：                                                               //
//		PortHandle;  端口句柄                                                //
//  出口参数：无                                                             //
//  返 回 值：                                                               //
//		0	正常                                                             //
//		-1	失败                                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
signed char UpdateTransComfirm()
{
	WORD ulRespLen;
	BYTE ucRespData[20];

	ulRespLen = 15;
	switch( SendCMD("04", NULL, 0, ucRespData, &ulRespLen ) )
	{
	case CMD_POSITIVE:
		break;
	case CMD_NEGATIVE:
	default:
		return FAILURE;
	}

	return SUCCESS;
}
//------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  当交易未确认时,可以使用此交易取消交易                                    //
//                                                                           //
//  BYTE CancelTrans( HANDLE PortHandle )                                    //
//                                                                           //
//  入口参数：                                                               //
//		PortHandle;  端口句柄                                                //
//  出口参数：无                                                             //
//  返 回 值：                                                               //
//		0	撤销成功                                                         //
//		-1	撤销失败                                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
signed char CancelTrans()
{
	WORD ulRespLen;
	BYTE ucRespData[20];

	ulRespLen = 15;
	switch( SendCMD("05", NULL, 0, ucRespData, &ulRespLen ) )
	{
	case CMD_POSITIVE:
		break;
	case CMD_NEGATIVE:
	default:
		return FAILURE;
	}

	return SUCCESS;
}
//------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  取最后一笔交易记录                                                       //
//                                                                           //
//  BYTE GetLastTrans( HANDLE PortHandle, TRECORD *Rec )                     //
//                                                                           //
//  入口参数：                                                               //
//		PortHandle;  端口句柄                                                //
//  出口参数：                                                               //
//		Rec;	上送数据结构                                                 //
//  返 回 值：                                                               //
//		0	成功                                                             //
//		-1	失败                                                             //
//		1	无交易记录                                                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
signed char GetLastTrans()
{
	WORD ulRespLen;
	BYTE ucRespData[150];

	memset(ucRespData,0,sizeof(ucRespData));

	ulRespLen = sizeof(ePosLastRec)*2 + 15;
	switch( SendCMD("03", NULL, 0, ucRespData, &ulRespLen ) )
	{
	case CMD_POSITIVE:
		break;
	case CMD_NEGATIVE:
	default:
		return FAILURE;
	}

	StringToBytes( ( char * )ucRespData,(char *)&ePosLastRec);
	return SUCCESS;
}
//--------------------------------------------------
/**************************************************************************/
/* 接口函数 BYTE PayByECard(BCD *Amount) */
/* 执行一次完整的销售                                                     */
/* Amount :BCD   付款金额                                                 */
/* 成功返回 SUCCESS,由 ePosLastRec送出最后交易数据                        */
/* 失败返回                                                               */
/* BUSY          POS忙                                                    */
/* TRANSAMTERR   传送金额错                                               */
/* TRANSCOMFERR  确认交易错                                               */
/* NOCARD        交易完成,读取余额错（卡已经取走）                        */
/* OVERTIME      等待超时                                                 */
/* OUTTIME		 过期卡													  */
/* BLOCKCARD	 黑名单卡                                                 */
/* WRONGCARD	 非E卡通卡                                                */
/* BROKENCARD	 损坏卡                                                   */
/* SIGNONNEEDED	 未签到                                                   */
/**************************************************************************/
signed char PayByePosCard(BCD *Amount)
{
    	BYTE retry,Status;

//ccr2016-03-02		PutsO(Msg[WAITEPOS].str);//C-DISPLAY

        if(GetPosStatus(&Status) == FAILURE || Status != VACANCY)//free?
        	switch(Status)
        	{
        		case NEEDSIGNON:
        			return SIGNONNEEDED;
        		default:
        			return BUSY;
        	}

        if(UpdateTransAmount(Amount) == FAILURE)
            return TRANSAMTERR;

        Delay(ePOSDELAY);
		for (retry=0;;)
		{
	        if (GetPosStatus(&Status) == FAILURE || Status != READFINISH)
	        {
				if (Status==LOWAMT || ++retry>50 || KbHit() && (Getch() == ApplVar.AP.FirmKeys[ID_CLEAR]))
				{
					Bell(2);//长叫，出错   //
					CancelTrans();
					return FAILURE;
				}
	        }
			else
				break;
		}

        if(UpdateTransComfirm() == FAILURE)
       	{
			CancelTrans();
            return TRANSCOMFERR;
       	}

        if(GetPosStatus(&Status) == SUCCESS && (Status == VDFINISH || Status == BALANCEFAILD))//complete
        {
	        Delay(ePOSDELAY);
//			memset(&ePosLastRec,0,sizeof(ePosLastRec));
            if (GetLastTrans()!=SUCCESS)
				memset(&ePosLastRec,0,sizeof(ePosLastRec));
			else
				Bell(0);
		    return SUCCESS;
        }
        else
		    return FAILURE;
}
/**************************************************************************/
/* 打印ePos交易认证码 													  */
/**************************************************************************/
void PrintePosTAC()
{
	BYTE i;
	BCD	theLast,theCurr;


	memset(SysBuf,' ',sizeof(SysBuf));
//	CopyFrStr(SysBuf, Msg[ePOSTAC].str);
	HEXtoASC(SysBuf+6,ePosLastRec.TAC,4);
	RJPrint(0,SysBuf);


	memset(SysBuf,' ',sizeof(SysBuf));
	CopyFrStr(SysBuf, Msg[ePOSCARNO].str);

	i = ePosLastRec.CardNum[0];	ePosLastRec.CardNum[0] = ePosLastRec.CardNum[4];ePosLastRec.CardNum[4] = i;
	i = ePosLastRec.CardNum[1];	ePosLastRec.CardNum[1] = ePosLastRec.CardNum[3];ePosLastRec.CardNum[3] = i;

	HEXtoASC(SysBuf+10,ePosLastRec.CardNum,5);
	i = SysBuf[10];	SysBuf[10] = SysBuf[14];SysBuf[14] = i;
	i = SysBuf[11];	SysBuf[11] = SysBuf[13];SysBuf[13] = i;
	i = SysBuf[12];	SysBuf[12] = SysBuf[17];SysBuf[17] = i;
	i = SysBuf[15];	SysBuf[15] = SysBuf[19];SysBuf[19] = i;
	i = SysBuf[16];	SysBuf[16] = SysBuf[18];SysBuf[18] = i;
	for (i=10;i<10+10;i++)
	{
		if (SysBuf[i]=='9')
			SysBuf[i] = '0';
		else if (SysBuf[i]=='0')
			SysBuf[i] = '9';
		else if (SysBuf[i] & 1)
			SysBuf[i]++;
		else
			SysBuf[i]--;
	}
	RJPrint(0,SysBuf);

    theLast = theCurr = ZERO;

	theCurr.Value[0] = ePosLastRec.BalBef[3];//消费前金额 //
	theCurr.Value[1] = ePosLastRec.BalBef[2];//消费前金额 //
	theCurr.Value[2] = ePosLastRec.BalBef[1];//消费前金额 //
	theCurr.Value[3] = ePosLastRec.BalBef[0];//消费前金额 //

	theLast.Value[0] = ePosLastRec.TxnAmt[3];//交易金额 //
	theLast.Value[1] = ePosLastRec.TxnAmt[2];//交易金额 //
	theLast.Value[2] = ePosLastRec.TxnAmt[1];//交易金额 //
	theLast.Value[3] = ePosLastRec.TxnAmt[0];//交易金额 //

	Subtract(&theCurr, &theLast);
	RJPrint(0,FormatAmtStr(Msg[eCARREMAIN].str, &theCurr, PRTLEN));

}
#endif
