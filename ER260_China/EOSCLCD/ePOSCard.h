/*****************************************************/
/********************* ePosCard.h ***********************/
/*****************************************************/
//---------------------------------------------------------------------------
#if (CASE_EPOS)

#if defined(DEBUGBYPC)
#pragma option -a1
#else
#pragma pack(1)
#endif


#ifndef EPOS_H
#define EPOS_H

#define ePOSDELAY		800

#define DLE				0x10
#define STX				0x02
#define ETX				0x03
#define ENQ				0x05
#define ACK				0x06
#define NAK				0x15
#define EOT				0x04

//Define the return type of Command
#define CMD_POSITIVE	0
#define CMD_NEGATIVE	1

#define SUCCESS			0
#define FAILURE			-1
#define RETRY			-2
#define CHECK_ERROR		-3

//error
#define	BUSY 	        73					/* POS忙 */
#define TRANSAMTERR     (BUSY + 1)			/* 传送金额错 */
#define TRANSCOMFERR    (TRANSAMTERR + 1)	/* 确认交易错 */
#define NOECARD          (TRANSCOMFERR + 1)	/* 交易完成，读取余额错（卡已经取走）*/
#define OVERTIME        (NOECARD + 1)		/* 等待超时 */
#define OUTTIME			(OVERTIME + 1)		/* 过期卡 */
#define BLOCKCARD		(OUTTIME + 1)		/* 黑名单卡 */
#define WRONGECARD		(BLOCKCARD + 1)		/* 非E卡通卡 */
#define BROKENCARD		(WRONGECARD + 1)		/* 损坏卡 */
#define SIGNONNEEDED	(BROKENCARD + 1)	/* 未签到 */
#define LOWAMTERR		(SIGNONNEEDED + 1)	/* 未签到 */

#define WAITTIME		2 * 1000

//pos state
#define	VACANCY		0x00	/* 系统空闲 */
#define	WAITFORCARD	0x01	/* 等待读卡 */
#define	READFINISH	0x02	/* 读卡完成，可以确认交易 */
#define	LOWAMT		0x03	/* 卡余额不足 */
#define	OUTDATE		0x04	/* 卡已经过期 */
#define	BLOCKED		0x05	/* 黑名单卡 */
#define NOTECARD	0x06    /* 非法卡，非E卡通卡 */
#define	BROKEN		0x07	/* 损坏卡，无法修复的卡 */
#define	VDFINISH	0x08	/* 交易完成，等待取卡 */
#define	BALANCEFAILD	0x09/* 交易完成，最后读余额失败，可能产生新点卡 */
#define	NEEDSIGNON 	0x10	/* 未签到 */

//error
#define	BUSY 	        73
#define TRANSAMTERR     (BUSY + 1)
#define TRANSCOMFERR    (TRANSAMTERR + 1)
#define NOCARDE          (TRANSCOMFERR + 1)
#define OVERTIMER        (NOCARDE + 1)


typedef struct
{
	unsigned char Corpld[6];		//运营单位代码 //
	unsigned char Brhld[3];			//网点编号 //
	unsigned char Oprld[5];			//操作员号 //
	unsigned char TxnType[1];		//交易类型 //
	unsigned char Posld[4];			//POS机器号码 //
	unsigned char PosSeq[3];		//POS 流水号 //
	unsigned char CardNum[5];		//卡号 //
	unsigned char CardKind[1];		//卡类型 //
	unsigned char BalBef[4];		//消费前金额 //
	unsigned char TxnAmt[4];		//交易金额 //
	unsigned char TxnDate[4];		//交易日期 //
	unsigned char TxnTime[3];		//交易时间 //
	unsigned char TxnCounter[3];		//交易计数器 //
	unsigned char TAC[4];			//交易认证码 //
} TePOSRECORD; //上送数据结构 //

extern signed char PayByePosCard(BCD *Amount);
extern void PrintePosTAC();

#endif
#endif
