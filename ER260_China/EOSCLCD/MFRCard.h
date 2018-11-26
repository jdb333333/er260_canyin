/********************* M1Card.h ************************/

#ifndef MFRCARD_H
#define MFRCARD_H
//---------------------------------------------------------------------------
#define MFR_CTS             1   //ccr2014-08-26 使用串口CTS来检测是否有射频卡

#define MFRPORT				3			//cc 060321 MF 卡串口

#define MFRBLOCKSIZE		16	//  每次读取的数据块的字节数   //

#define CommandBit			0x42	//发送命令之前先发'B'
                                    //
#define CMD_Read_SN1		1		//读卡序列号
#define CMD_Read_SN2		0x59	//读卡序列号
                                    //
#define CMD_Read_Block		14		//读数据
#define CMD_Write_Block		30		//写数据
                                    //
#define REC_Read_SN			5		//读卡序列号返回
                                    //
#define REC_Read_Block		17		//读数据返回
#define REC_Write_Block1	1		//写数据返回
#define REC_Write_Block2	0x59	//写数据返回

#define CMD_Read_MFR1		1		//读卡状态
#define CMD_Read_MFR2		0xC0	//读卡状态


#define RW_Factory			0
#define RW_Flags			1
#define RW_Init				2
#define RW_Customer			3
#define RW_Valatt			4
#define RW_Valpre			5
#define RW_Facpre			6		//cc 20070825
#define RW_ALL				7

#define CLOGINCARD  		82641937L	// 经理卡 //
#define MAKECARD			0
#define CHARGECARD			1
#define CLEARCARD			2

#define EMIDHEAD			0x02


BYTE SendRecordBySPI(BYTE *p,BYTE length);
BYTE ReadRecordBySPI(BYTE *p,BYTE length);
BYTE SendByteBySPI(BYTE p);
WORD ReadByteBySPI();
BYTE CC_WriteMFR(char *Source,BYTE ChipAdr);
BYTE CC_ReadMFR(char *Dest,BYTE ChipAdr);
BYTE CC_Request(void);
BYTE CC_MFRCard(void);
void GetValue(BYTE ChipAdr);

#endif