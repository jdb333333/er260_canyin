//------------------------For Debug--------------------------
#ifndef KING_H
#define KING_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

#if (defined(CASE_GPRS) && defined(CASE_INNERDOTPRN))
#error "xx  CASE_GPRS & CASE_INNERDOTPRN  xx"
#endif

#if !defined(DEBUGBYPC)

#include "include.h"

#pragma pack(1)

#define Delay(msec) msDelay(msec)

#else

#include "TypeDef.h"

#define BIT0	0x01
#define BIT1	0x02
#define BIT2	0x04
#define BIT3	0x08
#define BIT4	0x10
#define BIT5	0x20
#define BIT6	0x40
#define BIT7	0x80

#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000

#define BIT16	0x010000l
#define BIT17	0x020000l
#define BIT18	0x040000l
#define BIT19	0x080000l
#define BIT20	0x100000l
#define BIT21	0x200000l
#define BIT22	0x400000l
#define BIT23	0x800000l

#define BIT24	0x01000000l
#define BIT25	0x02000000l
#define BIT26	0x04000000l
#define BIT27	0x08000000l
#define BIT28	0x10000000l
#define BIT29	0x20000000l
#define BIT30	0x40000000l
#define BIT31	0x80000000l

#define __disable_irq()  {}    //禁止中断，避免校验过程中的数据变化
#define __enable_irq()  {}    //禁止中断，避免校验过程中的数据变化


#pragma option -a1

extern void Delay(int msec);
//#define Delay(msec) Sleep(msec)

#endif


#include "types.h"		//definizione di tipi ausiliari
#include "bios.h"		//bios
#include "BIOS_CHN.h"
#include "CHNDisp.h"
#include "CHNInput.h"

#include "Menu.h"
#include "USBDISKAPP.H"

#include "SysTick.h"

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#include "EJournal.h"
#endif

#if !defined(DEBUGBYPC)
#include "print.h"
#endif

#define  DISPCOM	BiosCmd_PopFip0


//1-use team 42keyboard
//0-use macro 56 keybard

#define PARTNER  0		/* if set then test EPROM for PARTNER */
#define NETTEST 0		/* if set then no computer network */
#define CUBA1	0		/* If set then new option for SICO CUBA */
#define COST	0		/* if set then costprice system for Dansk */
#define PLUTEST 0		/* if set then test new Random ApplVar.Plu Routines */

/* DON'T FORGET TO SET THE CORRECT SWITCHES IN THE ASSEMBLER SOURCES*/

#define BSF	0		/* if set then eprom for BSF */
#define PLOGO	0		/* if PLOGO is 1 then check for graphic logo */
#define HIGHSPEED   (input(0x19) & 0x20)    /* if set then high speed installed */
#define PTEST	1	/* if set then printer controller with command for */
/* 1 DOT line also system flag KISCTR is checked */


#define BONUS	0		/* bonus system */
#define PBAFTER 0		/* pb transactions are added to reports when finalized */
#define PBTIME	0		/* store time on ApplVar.PB for Moretti */
#define BOTTLE	1		/* bottle link */
#define TTLPOS	0		/* programmable position of total on slip for Moretti */
#define PLUENQ	0		/* plu enquiry function */
#define PRCREP	0		/* print reports by computer also on journal */
#define INVCOM	0		/* change inventory by computer */
#define PLUREQ	0		/* plu request */
#define CCARD	0		/* credit card interface for Moretti */
#define NOBUGS	1		/* all bugs solved */
#define KWS	0		/* changes for KWS (ApplVar.PB) */
#define INDO	0		/* Indonesian fiscal version (also set FISCAL to 1) */

#define NEWPORT 0	/* if set then extra serial port for Moretti */
#define LCDLEN	16		/* Length of LCD Display */
#define JAPAN	0	/* if set the Japanese standard app texts */
/* and special controller used also make*/
/* eprom with XJ1219 application */
#define DANKORT 0      /* if set then dankort interface active */
#define EFT	0		/* if set EFT active for Holland */
#define EFTD	0		/* if set EFT active for Germany, also set EFT */
#define REMOTE	0      /* if set invoice printing for argentina */
/* Also GST is calculated on NET Sales Amount !!*/
/* Special Validation lay-out */
/* ApplVar.Total not double heigth/width */
#define CUBA	0		/* if set then special Eprom for CUBA, SICO with secret code
and special print on SPRINT only used for LAUNDRY system  */
#define CHINA	1	/* chinese printing  */
/* If CHINA is set to 2 it is compiled for Europe */
/* This means graphic print on slip only when Slip */
/* is set to CTM 290, Reset GRAPHIC */
/* DON'T forget XTGEN/XTMOD/XTDRIVER/XTC/MAKEPROM !!!! */
/* For CHINA = 1 also skip LOGO, BONUS, VALIDATION */
/* SUSPEND */
#define SLOVAK	0	/* if set the program for Slovakia Fiscal Prom 32K !! */
/* if set to 2 then version without Fiscal Eprom */
/* Always set FISCAL to 1 */
#define GRILLO	0		/* Note that SUPER must also be 1 !! */
/* used for header print  */
/* if 1 then 24 character print */
/* if 2 then 11 character print */
#define PRINTTYPE   0	    /* define printer type used */
/* 0 is thermal, 1 is dot matrix */
#define CHAN	0      /* if CHAN is 1 then PORT 1 is High Speed */
/* Channel */
#define APPL	1     /* 1 -> Standard application */
/* 2 -> GREEK (and Cyprus) application */
/* 3 -> OPALE RIVA Germany appl */
/* 4 -> Moretti Spanish appl (based on appl 1) */
/* 5 -> Cyprus English appl (based on appl 2) */
/* 6 -> Chinese Application base on appl 1 thermal */
/* 7 -> Chinese Application base on appl 1 for */
/* Super Micro dot matrix */
/* 8 -> Standard MALTA application */
/*  also make special applic eproms !! */
/* 9 -> Standard Frontiera Application */
/* 10 -> Standard Spanish Frontiera Application */
/* 11 -> Slovakian appl (based on appl 1) */
/* 12 -> Application for Holland based on 1 */
/* 13 -> test application for new init routines */
#define OPALE	0	/* if 1 then compile for OPALE (RHS Germany)*/
/* skip ApplVar.PB, ApplVar.KP, ReceiptIssue, Proforma & KPGroup */
/* note that SKIP must be set to S_PB */
#define DEBET	0   /* if set then debet card reader active */
/* if set to 1 then QUARTO */
/* if set to 2 then Standard (SCAS), IPC */
#define CRT	0   /* if set then KT + CRT version */

#define MA   1
#define NEWCTR	1 /* if set then NEW controller is used (SM Main board) */
/* MUST be SET also for SUPER, GRILLO, FRONT */
#define SUPER 0
/* if SUPER is 1 and MA  == 1 then compile  for SUPER MICRO */
#define FRONT	0   /* if SUPER is 1 and FRONT is 1 then compile for FRONTIERA */
#define DITRON	0
/* if set then DITRON LANGUAGE active */
/* Don't set FISCAL because only serial */
/* number is written */
//#define FISCAL	0   /* if set then FISCAL ROUTINES ACTIVE */
/* DON'T FORGET TO SET THE CORRECT EPROM TYPE */
/* IN XTBOOT.S01 !!!!*/
#define ARGENT	0   /* if set then Argentine fiscal logo */
#define CYPRUS	0   /* if set and GREEK is also set fiscal O.S. for Cyprus */
/* there are two versions APPL = 2 (Greek) APPL = 5 (English) */
#define GREEK	0   /* if set and FISCAL set then Greek fiscal O.S. */
/* don't forget POWERFAIL !!*/
/* Please Also NOTE the Programming Texts when using */
/* MAKEPROM !! because they are not stored in XTGEN */
#define EMPORIKI	0	/* If Set Then EPROM for EMPORIKI */
#define ROUNDTAXUP  0	/* if set round tax always UP, especially for GREECE */
/* not required any more */
#define MALTA	0   /* if set then fiscal O.S. for MALTA (no powerfail) */

#define FISCALERROR 100     /* fiscal error message start */
#define GRAPHIC 0   /* If set then print Graphic picture */
/* MUST be disabled for TECHNOEMPORIKI GREECE !!! */

/* if SKIP is set to 1 then the report, */
/* program, barcode and pbbuffer module */
/* are empty because of the size of the */
/* eprom emulator */
#define S_PB	0x01   /* Bit 0 then skip ApplVar.PB */
#define S_REP	0x02	/* bit 1 then skip report */
#define S_PROG	0x04	    /* if bit 2 then skip programming */
#define S_BAR	0x08	    /* if bit 3 then skip barcode */
#define S_COMP	0x10	    /* if bit 4 set then skip computer */
#define S_DISC	0x20	    /* if bit 5 set then skip discount */

/* 厨房打印机的命令 */
#define KP_NORMAL   0   //正常打印
#define KP_DBLWIDTH 1   //倍宽打印
#define KP_BLACK    2   //黑色字体
#define KP_RED      3   //红色字体
#define KP_CUTF     4   //全切纸
#define KP_CUTH     5   //半切纸
#define KP_DBLHIGH  6   //倍高打印
#define KP_FEED     LF   //走纸一行

/* 厨房打印机消耗(0-无厨房打印机,1-内置打印机) */
#define	T58A_C	(0+2)       //（58mm A字体，带切刀,32字符）
#define	T80A_C	(T58A_C+1)  //（80mm，A字体，带切刀)
#define	T58B_C	(T80A_C+1)  //（58mm B字体，带切刀,42字符）
#define	D76A_C	(T58B_C+1)  // 76mm针打，32字符，A字体，带切刀。增加的型号
#define	CITIZEN	(D76A_C+1)  //西铁城打印机, 30字符宽度
#define	T58A	(CITIZEN+1) //（58mm A字体，无切刀）
#define	KPMAX	(T58A-T58A_C+1)

#define DOTPRNTYPE D76A_C

/*
#define SKIP	(S_PB+S_REP+S_DISC)
*/


#define SKIP	0


/* Define default print Layout */

#define 	PLAYOUT 	0x0b

#if (CASE_ER520U)
#define AFTERTRILER	5// 打印完票尾后走纸行数 //
#else
#define AFTERTRILER	2// 打印完票尾后走纸行数 //
#endif

//#define WORD unsigned short
//#define UnLong unsigned int

#define BASEITEMS  98

#if COST == 1
#define TRANSITEMS	(BASEITEMS - 8)
#else
#if BONUS == 1
#define TRANSITEMS	(BASEITEMS - 7)
#else
#define TRANSITEMS	(BASEITEMS - 6)
#endif
#endif

#if GREEK == 0

#define ARROWS	ArrowsRight  /* if ARROWS is ArrowsRight then the left	     */
/* markers are used for #(Training), REGI, RON */
#else

#define ARROWS	ApplVar.ArrowsFlag  /* if ARROWS is defined ApplVar.ArrowsFlag then the left */
/* markers are used for SUB, TTL, CHANGE */

#endif

#if FISCAL == 1

#define RECNUM	12
/* when fiscal then Z-counter used for */
/* battery down status */
#define TRAINNUM	0	/* training receipt number */
#define ZSIZE	9

#else
#if CUBA == 1
#define RECNUM	9	/* 3 counter of 6 digits */
#else
#define RECNUM	3
#endif
#define TRAINNUM	3	/* training receipt number */
#define ZSIZE	3

#endif

#define SOH		0x01
#define STX		0x02
#define ETX		0x03
#define EOT		0x04
#define ENQ		0x05
#define ACK		0x06
#define BEL		0x07
#define LF		0x0A
#define VT		0x0B
#define FF		0x0C
#define CRET	0x0D
#define SO		0x0E
#define SI		0x0F
#define DLE		0x10
#define DC1		0x11
#define XON     0x11
#define DC2		0x12
#define DC3		0x13
#define XOFF    0x13
#define DC4		0x14
#define NAK	0x15
#define ETB	0x17
#define DEL	0x18
#define CAN		0x18
#define WAIT	0x1b
#define FS		0x1C
#define GS		0x1D


#define TESTBIT(w,i)	    (w & (i))
#define SETBIT(w,i)	    (w |= (i))
#define RESETBIT(w,i)	    (w &= ~(i))
#define INVERTBIT(w,i)	    (w ^= (i))

///////////////////////////////////////////////////////////////////////
#define SETDECIMAL(sign,num)  {sign = (sign & (~0x07)) | num;}//设置BCD数据的sign的小数位数


//lyq added for modify flash driver 2003\12\08 start
#define CCLIB_ADDR ((unsigned CONSTCHAR*)0x40000)
//lyq added for modify flash driver 2003\12\08 end
#if NETTEST == 1
extern  BYTE ToMc;
#endif

//===========================================
extern BYTE PrintAmt(CONSTCHAR *str, BCD *amt);
extern void SetPrintLayOut(BYTE playout);
extern void KpEnd(BYTE kp);
extern void PrintKp(BYTE cmd, CONSTCHAR *str);
extern void PrintMessage(WORD number);
extern void ReadClerk(void);
extern void ReadPlu(void);
extern void StrToBCDValue(BYTE *to, char *from, short len);

//===========================================

extern void DrawerOpen(void);

extern void BcdDiv10(BCD *A0);
extern void BcdMul10(BCD *A0);
extern void BcdMul100(BCD *A0);
extern void ULongToBCDValue(BYTE *toA1, unsigned long R2R0);	    /* () make BCD from hex long */
extern WORD BCD4toWORD(WORD R2R0);

extern void    Multiply(BCD *result, BCD *with);     /* () */
extern void RoundBcd(BCD *A0, WORD decimalR3);     /* () round BCD to fixed number of decimals */
extern void Add(BCD *result, BCD *with);	    /* () add two BCD numbers */
extern void Subtract(BCD *result, BCD *with);     /* () */
extern WORD FormatBCD(char *toA0, BCD *fromA1, WORD fmtR1);	    /* () make ascii from BCD */
extern short Bell(short len);	    /* () sound (len = 0, 1, 2, 3)  buzzer */
extern WORD BCDValueToULong(BYTE *pA1, UnLong *A0) ;		/* () max is 999.999.999 */
extern UINT64  BCDValueToLong64(BYTE  *p);       /* (void *p, UnLong *Lp) max is 999.999.999 */
extern void WORDtoBCD(char *to, WORD R3);	    /* () make BCD from hex */
extern BYTE WORDtoASC(char *to, WORD R3);	    /* () make ascii string from unsigned short with zero suppression*/
extern void WORDtoASCZero(char *to, WORD R3);     /* () make asci string from unsigned short with leading zero's*/
extern void HEXtoASC(char *to, char *from, short len);	    /* () convert hex to asci */
extern BYTE  HEXtoASCX0(char *to, char *from, short len);
extern void Divide(BCD *result, BCD *with);	    /* () */
extern short CompareBCDValue(void *a, void *b, WORD length);	    /* () */
extern short CompareBCD(BCD *a, BCD *b);   /* () */
/* compare two BCD numbers */
/* result   returnvalue    */
/* a = b    0		   */
/* a < b    -1		   */
/* a > b    1		   */

/* make unsigned long from bcd number */
extern BYTE    CheckOnLine(BYTE port);
extern WORD    Check232(BYTE port);
extern short     CheckNotZero(BCD * bcd);
/* return 0 if zero else 1 */

extern short BCDWidth(BCD  *bcd);
extern void SetBCDPoint(BCD *bcd,BYTE point);

extern void    DrawerOpen(void);   /* (BYTE draw) number number from 0 to 3 */
extern BYTE     Getch(void);	    /* wait and read keynumber */
extern void    GetTimeDate(struct TimeDate *);  /* () get system time, date */

#if NEWPORT == 1
extern void InitLcd(BYTE cmd, BYTE *line1, BYTE *line2);	    /* () */
extern BYTE	DisplayType;
#endif
extern short     JFeed(void);	    /* (short line) feed number of lines on journal */

extern short     KbHit(void);	    /* check if key available */
extern short     KbTone(short cmd);	    /* () keytone on,off */

extern void     PutsO( CONSTCHAR *str);	    /* () put string on operator display */

//extern void    ReadRam();	    /* () read bytes from ram banks */
extern short     RFeed(short line);	    /* () feed number of lines on receipt */
/* Command 0 only returns status */
//extern short     RJPrint(short cmd, char *str);	    /* () print string on printer */

extern void SetRts(BYTE port, BYTE OnOff);	    /* () */
extern void SetTimeDate(BYTE pDateTime,struct TimeDate *);  /* () set system time */

extern void	WaitOnLine(BYTE port);   /* () set PortTimeOut[X] to 1 if time out */
//extern void WriteRam();     /* () write bytes to ram banks */
extern WORD WriteSL(BYTE port, BYTE b);	    /* () return 1 if time out */


/***********************************************************************/
/********** Define MA  specific variables and functions *****************/
/***********************************************************************/


/******** Functions **********/
extern void    Density(WORD printer, WORD density);	    /* () */
extern short     InitConfig(void *config, short len);   /* () init controller */
extern void    SetCountry(BYTE country);   /* () set character set for printer */



//=====================================================================================
#define RJPrint(a, s) OutPrint(TESTBIT(ApplVar.PrintLayOut,BIT2)?CMDP_DR:CMDP_R, (char *)(s));	//Lancia stampa (standard)

#define  NearEnd() Bios_1(BiosCmd_PrintCheck)	    /* check nearend R & J */
/* 0 = ok, 1 = J nearend, 2 = R nearend */
/* 3 = R & J nearend */


#define MemSet(a,b,c) memset(a,c,b)

#define CopyFrStr(a,b)   memcpy(a,b,strlen(b))
#define DECtoBCD(p) ((((p)/10)<<4)+((p)%10))    //DEC to BCD
#define BCDtoDEC(p) (((p)>>4)*10+((p)&0x0f))   //BCD to DEC
#define TestBit(ch,BitNo) (ch & (1<< BitNo))
#define MemCopy(to,from ,len)  memcpy(to,from,len)
#define StrCopy(to,from) strcpy(to,from)
#define StrLen(str) strlen(str)

//ccr2016-02-04>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#if defined(CASE_GPRS)
#define KEYDESMAX	    46  //ccr2016-08-23
#else
#define KEYDESMAX	    39 //ccr2016-09-13
#endif
//ccr2016-02-04<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//客显标示显示控制常数
#define	FLAG_NULL		0x00
#define	FLAG_TOTAL		0x01
#define	FLAG_COLLECT	0x02
#define	FLAG_CHANGE		0x04
#define	FLAG_PRICE		0x08


#include "appl.h"

#define TRAINING    (!TESTBIT(ApplVar.Clerk.Options, BIT7)) //ccr2017-02-27收款机处在培训模式下

#endif//KING_H
