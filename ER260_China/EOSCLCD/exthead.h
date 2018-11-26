#ifndef EXTHEAD_H
#define EXTHEAD_H

//------------------------For Debug--------------------------
#ifndef ForDebug
#include "debug.h"
#endif
//------------------------For Debug--------------------------

/******************* Working Lay Out with  ******************/
extern BYTE
    ApplVar_Restored,
	CopyReceipt,
	ARROWS,         /* define ARROWS ArrowsRight */
	Prefix1,
	Prefix2,
    InActive,
    Appl_ProgLinePre,                /* programming type line */
    Appl_ProgLine,                /* programming type line */
    Appl_ProgStart,               /* programming started */
    Appl_ProgType,               /* programming type status */
    Appl_BitNumber,              /* used during programming */
    Appl_EntryCounter,                   /* entry counter */
    ApplRamSaved,               //ApplVar 保存标志
    Appl_MaxEntry,               //Max length of input from keyboard
    MACSwitch ;

extern struct DISPLAY_RGBUF
    Appl_DispRGBuf;

extern WORD
     FuncLookNum,            //当前选用的FuncLook
     Appl_PortNumber;

extern short
     Appl_ProgNumber;             /* item  (plu or function #) number of programming */

extern UnLong RamOffSet;

extern struct SETUPLIST SetList;

extern CONST WORD SORT[7] ;
/******************* Working Memory Lay Out with C language ******************/

extern char
    FRegSuspend,
	PwdInput[MAXPWD],			//Save the password of input
	DateAsci[16],          /* holds asci date */
	EntryBuffer[ENTRYSIZE],        /* entry buffer is ((max caption size) * 2) + 1 */
	ProgLineMes[64],       /* display program option */
	SysBuf[128],            /* System buffer max 128 positions */
				/* Used for Display  and printer and ApplVar.PB */
 //      GprsBuf[64][128], //defined in memory.c
  //      GprsSendBuf[64][128];

	TimeAsci[10] ;            /* holds asci time*/

extern BYTE
    FromUSB,                    //=1,表示Computer收取的数据来源于USB口;=0,表述数据来源于串口
    PCBuffer[PCBUFMAX],
	CCDBuff[CCDBUFMAX];     /* 除用作扫描枪的存储区外,在与扫描枪不冲突的情况下,还用作临时存储区 */

extern int
	PCCounter,
	CCDCounter;


extern struct TimeDate     Now;


extern const BCD
	ZERO,
	ONE,
	TEN,
	HUNDRED,
	THOUSAND,
	THOUSAND10,
	THOUSAND100,
	MAXWORD,
	MAXBYTE,
	MILLION,
	MILLION10;

extern struct RGSUSPEND RGSuspend;/* 存放挂起的收据,RGNumber=0时,无挂起的收据 */

extern char    *StrForST0;         /* 需要显示在状态行第一个状态位置的字符串.=0时,无显示*/
////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
extern CONST BYTE	ASCIIKEY[MAXKEYB];

extern CONST BYTE NUMASCII[MAXKEYB];


extern CONST struct APPTEXT DefaultText ;
extern CONST struct APPLICATION Default ;
extern CONST struct DEFAULT Def ;
extern CONST char DMes[20][11] ;
extern CONST char DText[20][6] ;

extern CONST char UPCOMM[12][4];

extern CONST BYTE Month_Day[12];


extern CONST struct XZREPORT XZTitle[XZNUM];

extern CONST WORD  KeyTablePBF[MAXKEYB];
extern CONST char *PortType[portTypeNum];
extern CONST char *KPType[KPTypeNum];
extern CONST char *SPType[SPTypeNum];
extern CONST char *BALType[BALTYPEMAX];

extern CONST struct SYSFLAGIDX SysFlagUsed[SYSUSED];
extern CONST char GrapType[GRASETMAX][tCAPWIDTH];
extern CONST char GrapSet[4][tCAPWIDTH];


extern CONST char Release[12] ;

extern BYTE KeyFrHost;

extern APPLICATION_SAVE ApplVar;
#if (ApplVarInSRAM==0)
extern APPLICATION_SAVE ApplVarSave;//将ApplVar指定在地址ADDR_EXTRAM=0x64000000的SRAM区
#endif

//.............ECRVIP数据存储区....................
#if (DD_ECRVIP==1)
#if defined(DEBUGBYPC)
extern struct  TECRVIPVar   ECRVIPVar1[ECRVIPMAX/2];
extern struct  TECRVIPVar   ECRVIPVar2[ECRVIPMAX/2];
extern struct  TECRVIPFix   ECRVIPFix[ECRVIPMAX];
#else
extern struct  TECRVIPVar   ECRVIPVar1[];
extern struct  TECRVIPVar   ECRVIPVar2[];
extern struct  TECRVIPFix   ECRVIPFix[];
#endif
extern struct TECRVIP ECRVIP;                      //记录当前登录的会员,数据掉电保护,因此关机后数据会丢失
#endif


extern struct PORTRECORD   Appl_Port;

#if (PLUCODE_X==1)
extern WORD
    PLU_X_Report;// 标志,表示直接报表功能.=0时,不是直接报表,>0时,为直接报表,其值为报表中的记录号  //
extern BYTE
    DEPT_Key_ID,
    PLU_Key_ID;// PLU1 在键盘中的位置,=0xff时,键盘上无此功能键 //
#endif

#if (defined(DEBUGBYPC))
extern void ProcessMessages();
extern BYTE	LoopInput;
#define SetLoop()  LoopInput=1
#define ResetLoop()  LoopInput=0
#else
#define SetLoop()  {}
#define ResetLoop()  {}
#define ProcessMessages()   {}
#endif

//ccr2016-01-21>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
extern CONST char PortForDevice[LINKTYPENUM][7];
//Speed:'1'-1200,'2'-2400,'3'-4800,'4'-9600,'5'-19200,'6'-38400,'7'-57600,'8'-115200
extern CONST char PortRate[PORTRATENUM][7];
extern CONST char *EcrGraphics[GRAPHMAX+1];

extern CONST WORD FuncLook[FUNCLOOKMAX][FUNCLOOK_KEYS];

extern CONST MenuDefine ItemXReport[];
extern CONST MenuDefine ItemZReport[];
#endif//EXTHEAD_H
