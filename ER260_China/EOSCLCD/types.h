//
//
// File : TYPES.H
//        -------
//
// Cont : Definizione di tipi di dati comuni a TUTTI I  moduli
//        ----------------------------------------------------
//
// In questo file vengono definiti alcuni tipi di dati ausiliari.
// Questo file deve essere incluso all'inizio di tutti i sorgenti
//
#ifndef TYPES_H
#define TYPES_H


#define BCDLEN	8
#define SIGN	1

#define STRZ char 			//stringhe ascii-z
#define STR char			//stringhe ascii


#if defined(DEBUGBYPC)

#pragma option -a1

#define BYTE 	unsigned char
#define WORD 	unsigned short
#define ULONG 	unsigned int
#define UnLong 	unsigned int

#define  uint8_t       BYTE
#define  uint16_t      WORD
#define  uint32_t      ULONG


#define UINT64  unsigned __int64

#define CONSTCHAR  char
#define CONSTBYTE  BYTE
#define CONSTWORD  WORD
#define BOOL     bool
#define CONST

#else

#include <stdint.h>

#pragma pack(1)

#define BYTE 	uint8_t
#define WORD 	uint16_t
#define ULONG 	uint32_t
#define UnLong 	uint32_t

#define UINT64 uint64_t

#define CONSTCHAR  const char
#define CONSTBYTE  const BYTE
#define BOOL      BYTE

#define CONST    const
#define CONSTWORD  const WORD
#endif


//ccr090715>>>>>>>>>>>>>
typedef struct{
    unsigned int     MsgIndex;    //ccr 当前功能号的信息编号 //
    void    (*EnterOperate)();    //ccr 按下"回车"键时需要执行的函数,=NULL时,返回上一级菜单 //
    void    (*ExitOperate)();    //ccr 按下"退出"键时需要执行的函数 //
}    KbdTabStruct;


typedef struct {
	WORD	MenuFrom;   //菜单信息在Msg[]中的位置
	WORD	MenuItems;  //菜单项目数
} MenuDefine;


struct SETUPLIST {
	WORD MsgIdx;    // 为主菜单的信息序号
	BYTE Cols;
	BYTE Rows;
	BYTE RecMax;    // 为主菜单的项目数
	BYTE Rec1st;
	BYTE *Var;
	BYTE SubNo; 	/*>0时,表示有下级菜单,此时SubNo为子菜单的信息序号 */
	BYTE SubMax;	/* 子菜单的项目数目 */
	BYTE isSub;		/* =true,表示为子主菜单; =false,为主菜单*/
	BYTE MainVar;	/*>0,为 上一级主菜单的菜单项序号 */
	BYTE MainMenu;  /*<0xff,表示有上级菜单*/
	BYTE SubMenu;	/*<0xff,表示有下级菜单*/
};

//印度尼西亚：当Index，Max,Bit均为0时，输入unitID。
struct SYSFLAGIDX {
	BYTE	Index;	// index of system flag(0-63)
	BYTE	Max;	// Max value for flag,=0:非数据域
	BYTE	Bit;	// bit number of flag(1-8),=0:非bit域
	BYTE	Invert; //=YN,标示1代表否,0代表是
	CONSTCHAR *Name;
};


struct XZREPORT {
	BYTE	Index;
	CONSTCHAR Name[15];
};

typedef struct {
	CONSTCHAR* str;
} FSTRING;

//add by lqw 090802
typedef struct {
	CONSTCHAR* str;
}GPRSSTR;


typedef struct
{
    BYTE    Sign;
    BYTE    Value[BCDLEN];
} BCD;

struct word {
    BYTE    Low;
    BYTE    High;
};

union KEY {
    WORD    Code;
    struct word C;
};

struct	TimeDate {
	WORD year;	/* current year */
	BYTE month;	/* month (1 = Jan) */
	BYTE day;	/* day of month */
	BYTE dow;	/* day of week (0 = Monday) */
	BYTE hour;	/* hours */
	BYTE min;	/* minutes */
	BYTE sec;	/* seconds */
	BYTE am_pm;	/*0=am,1=pm*/	//cc 20070730
	};

struct CCONFIG {	/* default value between brackets */
	BYTE	Country;	/* (0) country code for printer */
	BYTE	Display;	/* (2) 0 -- 3 */
				/* 0 = not present or not enabled */
				/* 1 = alphanumeric 20 char 1 line */
				/* 2 = numeric 9 digit + 6 arrows */
				/* 3 = not used */
	BYTE	Lock;		/* (2) 0 -- 3 */
				/* 0 = not present */
				/* 1 = badge (not implemented) */
				/* 2 = Central lock */
				/* 3 = Central & ApplVar.Clerk Lock */
	BYTE	JDensity;	/* (128) 0 - 255, density on journal printer */
	BYTE	RDensity;	/* (128) 0 - 255, density on receipt printer */
	BYTE	KeyBoard;	/* (2) 0 -- 3  not used by controller,
			    used by XTGEN */
				/* 0 =	*/
				/* 1 = Super Micro */
				/* 2 = MA  - 60 keys */
				/* 3 =	*/
	BYTE	ClearKey;	/* (0) keynumber for the clear key */
	BYTE	Click;		/* (-1) key click, 0 is disabled */
	BYTE	KeyMask[12];	/* (0) disable keynumber on keyboard 12 */
	};



//Tipo per bytes da vedere a bit-flags
//------------------------------------
//
typedef struct{
		char	b0:1;
		char	b1:1;
		char	b2:1;
		char	b3:1;
		char	b4:1;
		char	b5:1;
		char	b6:1;
		char	b7:1;
} BIT_8;




// Tipo ausiliario per vedere un BYTE sia come BYTE che come bit-flags
//
typedef union {
	BIT_8	bit;
	BYTE	byte;
} BYTEBIT;




//Tipo per WORDs da vedere a bit-flags
//------------------------------------
//
typedef struct{
		char	b0:1;
		char	b1:1;
		char	b2:1;
		char	b3:1;
		char	b4:1;
		char	b5:1;
		char	b6:1;
		char	b7:1;
		char	b8:1;
		char	b9:1;
		char	b10:1;
		char	b11:1;
		char	b12:1;
		char	b13:1;
		char	b14:1;
		char	b15:1;
} BIT_16;




// Tipo ausiliario per vedere una WORD sia come WORD che come bit-flags
//
typedef union {
	BIT_16 bit;
	WORD  word;
} WORDBIT;



// Tipo usato come pointer a memoria fiscale :
typedef unsigned long	FMADR;	//Fiscal-Memory addresses (pointer)



//tipo per gestione date (BCD, 4 bytes) :
typedef struct{
	 	BYTE dd;	//giorno
	 	BYTE mm;	//mese
	 	BYTE yy;	//anno
	 	BYTE w;		//giorno della settimana
} DATE_BCD;


//tipo per gestione date (BCD, 4 bytes) :
typedef struct{
	 	BYTE hh;	//ora
	 	BYTE mm;	//minuti
	 	BYTE ss;	//secondi
	 	BYTE w;   	//dummy : used to have even align
} TIME_BCD;

//-----------------------




//Tipo per struttura di controllo I/O su/da porte
//
typedef struct {

	short		BytesRead;		//numero di bytes letti
	short		BytesToRead;	//numero di bytes da leggere
	WORD	TimeOutR;		//time-out per lettura
	short		BytesWritten;	//bytes scritti
	short		BytesToWrite;	//numero di bytes da scrivere
	WORD	TimeOutW;		//time-out per scrittura
	WORD	Status;			//bit-flags stato


} PORTCTRL_TYP;




// =============================================================================
// Definizione di Macro
// =============================================================================

//CBYTE : per vedere un BIT8 come un BYTE
#define CBYTE(a) (*(BYTE*)&a)

//CBIT8 : per vedere un BYTE come un BIT8
#define CBIT8(a) (*(BIT_8*)&a)


#define CWORD(a) (*(WORD*)&a)
#define CLONG(a) (*(ULONG*)&a)

// CVI :
#define CVI(a)  ( ( (unsigned short)*(a) & 255 ) + ( (unsigned short)(*(a+1)<<8) & 0xFF00 ) )


// COFFSET : torna l'offset di un membro "m" di una struttura "s"
#define COFFSET(s,m) ((WORD)((ULONG)&(s.m) - (ULONG)&(s)))


// Macro MAKEEVEN : rende pari un numero sommando 1 se il numero e' dispari
#define MAKEEVEN(n) (2*((n/2)+(n%2)))

// --------------------------------------------------------------------------------
typedef enum {
		Graph_GM=1,
		Graph_000,
		Graph_001,
		Graph_002,
		Graph_003,
		Graph_004,
		Graph_005,
		Graph_006,
		Graph_007,
		Graph_008,
		Graph_009,
		Graph_010,
		Graph_011,
		Graph_012,
		Graph_013,
		Graph_014,
		Graph_015,
   		Graph_016,
		Graph_017,
		Graph_018,
		Graph_019,
		Graph_020,
		Graph_021,
} GraphIndex;


typedef struct  {
	int		IndexS;//键码开始序号
	int		IndexE;//键码开始序号
	char	*KeyType;//键码类型名称
	WORD	SetupIDX;//设置序号,用来调用ListItem;=0时,不使用ListItem
}TKEYDES;

//ccr2016-11-01>>>>>>>>>>
typedef struct {//为便于比较处理,不要改变以下顺序
    BYTE second;	/* seconds BCD*/
    BYTE minute;	/* minutes BCD*/
    BYTE hour;	/* hours BCD*/
    BYTE day;	/* day of month BCD*/
    BYTE month;	/* month (1 = Jan) BCD*/
	BYTE year;	/* current year BCD*/
}TDATETIME;

#endif
