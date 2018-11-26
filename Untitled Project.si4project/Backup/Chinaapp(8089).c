#include "king.h"
#include "message.h"



/* comment of Release:
	Release[0..4]:  ECR-Type name
	Release[5]:		-
	Release[6..7]:	M1-STM32F10X,M2-STM32F20X
	Release[8]:		'D'-DOT Printer;'T'-Thermal printer
	Release[9]:		2-IC4442,8-IC4428,M-ICmifare,X-no IC
	Release[10]:	Keyboard type N-7x8,L-7x6,W-8x7,C-6x7
*/


#if (DD_MU110)
#define PRNTYPE	'D'
#else
#define PRNTYPE	'T'
#endif

#if (DD_CHIPC==1)
#if (CASE_MFRIC==1)
#define MYIC	'M'
#elif (DD_4442IC==1)
#define MYIC	'2'
#else
#define MYIC	'8'
#endif
#else
#define MYIC	'X'
#endif

#if defined(STM32F10X_HD)
#define MCUTYPE     '1'//STM32F10X
#else
#define MCUTYPE     '2'//STM32F20X
#endif

//Relase 不得超过16字节
#if (CASE_GIADA3000)
CONST char Release[12] = {'G','3','0','0','0','-','M',MCUTYPE,PRNTYPE,MYIC,76};
#elif (CASE_ER220)
CONST char Release[12] = {'E','R','2','2','0','-','M',MCUTYPE,PRNTYPE,MYIC,57};
#elif (CASE_ER100)
CONST char Release[12] = {'E','R','1','0','0','-','M',MCUTYPE,PRNTYPE,MYIC,57};
#elif (CASE_ER520U)
CONST char Release[12] = {'E','R','5','2','0','-','M',MCUTYPE,PRNTYPE,MYIC,50};
#elif defined(CASE_INDONESIA)
CONST char Release[12] = {'P','B','P','2','6','0','M',MCUTYPE,PRNTYPE,MYIC,58};
#elif (CASE_ER260)
CONST char Release[12] = {'E','R','2','6','0','-','M',MCUTYPE,PRNTYPE,MYIC,58};
#elif (CASE_MACRO5150)
CONST char Release[12] = {'M','5','1','5','0','-','M',MCUTYPE,PRNTYPE,MYIC,87};
#else
CONST char Release[12] = {'G','3','0','0','0','-','M',MCUTYPE,PRNTYPE,MYIC,87};
#endif

CONST WORD SORT[] = {17, 9, 5, 3, 2, 1} ;


CONST struct APPTEXT DefaultText = {
    {       /* Header[8][37] */
#if CASE_ER220
	"  ",//用来推一行空行.
#endif
#if (PRTLEN<36)
/*	 .123456789.123456789.123456789.1 */
	"********************************",
	Header1,        /*Ccr" 	 创 造 优 秀  优 创 造 就   *" */
	"********************************",
#else
/*   .123456789.123456789.123456789.123456789.1*/
	"******************************************",
	Header2,        /*Ccr" 	 创 造 优 秀     优 创 造 就   *"*/
	"******************************************",
#endif
	"",
	"",
	"",
	"",
	},
    {    /* ReportType[16][16] */
        ReportType0,    //"  GRAND TOTAL   "//"   总  销  售   "
        ReportType1,    //"  GROUP         "//" 柜  组  销  售 "//1
        ReportType2,    //"  DEPARTMENT    "//" 部  类  销  售 "//2
        ReportType3,    //"  PLU           "//" 商  品  销  售 "      /* 3 Used with Plu Report */
        ReportType4,    //"  TENDER        "//" 付  款  信  息 "//4
        ReportType5,    //"  PORA          "//" 出 入 金 信 息 "//5
        ReportType6,    //"  DRAWER        "//" 钱  箱  信  息 "//6
        ReportType7,    //"  CORRECTION    "//" 更  改  信  息 "//7
        ReportType8,    //"  DISCOUNT      "//" 折  扣  信  息 "//8
        ReportType9,    //"  CURRENCY      "//" 外  币  信  息 "//9
        ReportType10,   //"  TAX           "//" 计  税  信  息 "//10
        ReportType11,   //" PB FUNCTIONS   "//" 桌  帐  信  息 "//11
        ReportType12,   //"PREVIOUS BALANCE"//" 开  台  信  息 "//12
        ReportType13,   //"  TABLES        "//" 桌  台  信  息 "//13
        ReportType14,   //"  INVENTORY     "//" 商  品  库  存 "//14
        ReportType15,   //"  PB INVOICES   "// 这个具体指什么? ZWQ 没有用
        ReportType16,   //"  TENDER BUFFER "// 这个具体指什么? ZWQ 没有用
    },
    {   /* SlipHeader[6][35] */
	"",
	"",
	"",
	"",
	"",
	"",
    },
    {       /* Trailer[6][24] */
	Trailer1,       //Ccr 	"     ~多  ~谢  ~光  ~临     ",
	Trailer2,       //Ccr 	"          ~再  ~见         ",
	"",
	"",
	"",
	"",
    },
};

CONST struct APPLICATION Default = {
    {   /* FirmKeys[FIRMKEYS],Can't exchange position of the list */
		fCLEAR		,		//ID_CLEAR  0,firmkey clear. It must be the first byte!!!!!!.
		fOCPRINTER	,		//ID_xPRINT 1,Set Time
		fDATE		,		//ID_DATE   2,Display Date
		cRIGHT		,		//ID_RIGHT  3,
		fPRGTYPE	,       //ID_SELECT 4 programming Type key
		cLEFT		,       //ID_LEFT   5 Programming number
		fPRGENTER	,       //ID_ENTER  6 programming Enter
		fPRGOPT		,       //ID_PRGOPT 7 Programming option invert
		cUP		    ,       //ID_UP     8 system report trigger
		cDOWN		,       //ID_DOWN   9
		fDUMP		,       //ID_DUMP   10
		fSHIFTDEPT	,       //ID_xDEPT  11 Alfa Shift 1
		fSHIFTHZ_ASC,       //ID_xINPUT 12 Alfa Shift 2
		fLOCK		,      	//ID_LOCK   13 mode lock
		fCANCEL		,		//ID_CANCEL 14 Mode lock
		fRJFEED		,		//ID_RJFEED 15 confirm OK

    },
    {   /* Manager[16] */
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    },
    {       /* Flag[SYSFLAGS] */
//NO_DECIMAL[0] COMMA[1]     AM_PM[2]     TIME_DATE[3]  KEYTONE[4]    CSV_FORMAT[5] SLIP_TOP[6]  SLIP_MAX[7]
//SLIPINFO[8]   CLERKFIX[9]  PREFIX_1[10] PREFIX_2[11]  PRI_LEVEL[12] PVAT[13]      LOCATION[14] REGISTER[15]
//SECOND[16]    CONTRAST[17] PBINFO[18]   PBPRINT[19]   COMPUTER[20]  ACTIVE[21]    CUTTER[22]   ART_LEVEL[23]
//PLU_PRINT[24] DOT[25]      DBLHIPRN[26] PROMOTION[27] BALANCE[28]   LMARGE[29]    BARCODE[30]  SLIP[31]
//SLIPPORT_1[32]  CHARSET[33]  KPSTART[34]  KPPRICE[58]   EXTREMAIN[59] SLFLAG[60]    EXTRAMN[61]  COPYRECEIP[62] ROUNDMASK[63]
   	//   0	 1     2    3  	 4	  5    6	7	 8	   9   10   11   12   13   14   15
#if (0)//ccr2015-08-14 defined(CASE_INDONESIA)
        --0x01,0x01,0x00,0x02,0x28,0x00,0x00,0x00,//ccr2015-04-20打印金额无小数
        0x06,0x10,0x20,0x20,0x00,0x00,0x00,0x00,
        0x03,--0x05,0x9a,0x30,0x02,0x05,0x00,0x58,
        0x42,0x01,--0x20,0x03,0x00,0x00,0xd1,0x00,//ccr2015-08-03发送HeartBeat
#endif
        FLAG00,		FLAG01,		FLAG02,		FLAG03,		FLAG04,		FLAG05,		FLAG06,		FLAG07,
        FLAG08,		FLAG09,		FLAG10,		FLAG11,		FLAG12,		FLAG13,		FLAG14,		FLAG15,
        FLAG16,		FLAG17,		FLAG18,		FLAG19,		FLAG20,		FLAG21,		FLAG22,		FLAG23,
        FLAG24,		FLAG25,		FLAG26,		FLAG27,		FLAG28,		FLAG29,		FLAG30,		FLAG31,
        FLAG32,		FLAG33,		FLAG34,		FLAG35,		FLAG36,		FLAG37,		FLAG38,		FLAG39,
        FLAG40,		FLAG41,		FLAG42,		FLAG43,		FLAG44,		FLAG45,		FLAG46,		FLAG47,
        FLAG48,		FLAG49,		FLAG50,		FLAG51,		FLAG52,		FLAG53,		FLAG54,		FLAG55,
        FLAG56,		FLAG57,		FLAG58,		FLAG59,		FLAG60,		FLAG61,		FLAG62,		FLAG63
    },

#if (CASE_GIADA3000)
	{// KeyTable[128]
			JPF,		SHIFT1,	    CLERK,		xLASTONE,	xREFUND,	DISC_PERDEC,	//00-05
			MODELOCK,	0,			NPRICE,		xBEFOR,     xRECEIPT,	DISC_NETDEC,		//07-11
#if (FOR_DEBUG)
            CLEAR,		MULT,		PLU1,		DEPT+5,		DEPT+8,		DISC_PERINC,		//12-17
            '7',		'8',		'9',		DEPT+4,		DEPT+7,		DISC_NETINC,		//18-23
#else
			CLEAR,		MULT,		PLU1,		DEPT+5,		DEPT+8,		PORA+1,		//12-17
			'7',		'8',		'9',		DEPT+4,		DEPT+7,		PORA+2,		//18-23
#endif
			'4',		'5',		'6',		DEPT+3,		DEPT+6,		SUB,		//24-29
			'1',		'2',		'3',		DEPT+2,		0,			TEND+1,		//30-35
			'0',		ZERO2,		'.',		DEPT+1,		0,			0,		//36-41
			0,			0,			0,			0,			0,			0,			//42-47
			0,			0,			0,			0,			0,			0,			//48-53
			0,			0,			0,			0,			0,			0,			//54-59
			0,			0,			0,			0,									//60-63
	},
#elif (defined(CASE_ER100) || defined(CASE_ER220))//ccr2016-05-25
#if defined(CASE_FORHANZI)
	{// KeyTable[128]
#if (FOR_DEBUG)
            MODELOCK,	JPF,	CLEAR,	MULT,		FUNCLOOK1,	xREFUND,	DISC_PERINC,
			CLERK,		'7',	'8',	'9',		DEPT+4,		DISC_NETDEC,DISC_PERDEC,
			xLASTONE,	'4',	'5',	'6',		DEPT+3,		xRECEIPT,	DISC_NETINC,
#else
            MODELOCK,	JPF,	CLEAR,	MULT,		SHIFT1,     xLASTONE,   xREFUND,
			CLERK,		'7',	'8',	'9',		DEPT+4,		DEPT+8,		DISC_PERDEC,
			FUNCLOOK1,	'4',	'5',	'6',		DEPT+3,		DEPT+7,		TEND_COUPON,
#endif
			NPRICE, 	'1',	'2',	'3',		DEPT+2,		DEPT+6,		SUB,
			PLU1,	    '0',	ZERO2,  '.',		DEPT+1,		DEPT+5,		TEND+1,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,
	},
#else
	{// KeyTable[128]
			MODELOCK,	JPF,	CLEAR,	MULT,		PLU1,		NPRICE,		DISC_PERDEC,
#if (FOR_DEBUG)
			CLERK,		'7',	'8',	'9',		DEPT+4,		DISC_NETDEC,DISC_PERINC,
			VIPLOGIN,	'4',	'5',	'6',		DEPT+3,		xRECEIPT,	DISC_NETINC,
#else
			CLERK,		'7',	'8',	'9',		DEPT+4,		DEPT+8,		DISC_PERINC,
			FUNCLOOK1,	'4',	'5',	'6',		DEPT+3,		DEPT+7,		TEND_COUPON,
#endif
			xBEFOR,   '1',	'2',	'3',		DEPT+2,		DEPT+6,		SUB,
			xLASTONE,	'0',	ZERO2,  '.',		DEPT+1,		DEPT+5,		TEND+1,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,
	},
#endif
#elif defined(CASE_ER520U)
	{// KeyTable[128]
			JPF,		SHIFT1,	    FUNCLOOK1,	DEPT+10,	DEPT+5,		MULT,	PLU1,	NPRICE,	ODRAW,	DISC_NETDEC,
			PORA+1,		CLERK,		DEPT+14,	DEPT+9,		DEPT+4,		'7',	'8',	'9',	DISC_PERDEC,	DISC_PERINC,
			PORA+2,		xBEFOR,	    DEPT+13,	DEPT+8,		DEPT+3,		'4',	'5',	'6',	TEND+3,	TEND+4,
			xREFUND,	xLASTONE,	DEPT+12,	DEPT+7,		DEPT+2,		'1',	'2',	'3',	SUB,
			CLEAR,		xRECEIPT,	DEPT+11,	DEPT+6,		DEPT+1,		'0',	ZERO2,	'.',	TEND+1,
			0,			0,			0,			0,			0,			0,		0,		0,			0,		0,
			0,			0,			0,			0,			0,			0,
	},
#elif defined(CASE_ER260)
	{
#if defined(CASE_FORHANZI)
#if defined(FOR_DEBUG)
        JPF,		MULT,	NPRICE,	PLU1,	DISC_PERINC,DISC_NETINC,SHIFT1,	DISC_NETDEC,		//00-07
        MODELOCK,	'7',	'8',	'9',	FUNCLOOK1,VIPLOGIN,	xRECEIPT,	PORA+1,		//08-15
#else
        JPF,		MULT,	NPRICE,	PLU1,	DEPT+9,	DEPT+10,	SHIFT1,		PORA+2,		//00-07
        MODELOCK,	'7',	'8',	'9',	DEPT+7,	DEPT+8,		xRECEIPT,	PORA+1,		//08-15
#endif
        CLERK,		'4',	'5',	'6',	DEPT+5,	DEPT+6,		xLASTONE,	SUSPEND,	//16-23
        DISC_PERDEC,'1',	'2',	'3',	DEPT+3,	DEPT+4,		xBEFOR,	    SUB,		//24-31
        CLEAR,		'0',	ZERO2,	'.',	DEPT+1,	DEPT+2,		xREFUND,	TEND+1,		//32-39

#else
//ccr2017-09-27        JPF,		MULT,	NPRICE,	PLU1,	SHIFT1,	FUNCLOOK1,	DISC_PERDEC,PORA+2,		//00-07
        JPF,		MULT,	NPRICE,	PLU1,	DEPT+9,	DEPT+10,	DISC_PERDEC,PORA+2,		//00-07
        MODELOCK,	'7',	'8',	'9',	DEPT+7,	DEPT+8,		DISC_NETDEC,PORA+1,		//08-15
        CLERK,		'4',	'5',	'6',	DEPT+5,	DEPT+6,		xLASTONE,	xRECEIPT,	//16-23
        DISC_PERINC,'1',	'2',	'3',	DEPT+3,	DEPT+4,		xBEFOR,	    SUB,		//24-31
        CLEAR,		'0',	ZERO2,	'.',	DEPT+1,	DEPT+2,		TEND+4,		TEND+1,		//32-39
#endif
        0,			0,		0,		0,		0,		0,			0,			0,			//40-47
        0,			0,		0,		0,		0,		0,			0,			0,			//48-55
        0,			0,		0,		0,		0,		0,			0,			0,			//56-63
	},

#endif

    {   /* StartAddress[AddrMAX] */
		RamOfTotal,                      /* Total Sales start address */
		RamOfGroup,                     /* Group Start Address */
		RamOfDept,                      /* Dept start address */
		RamOfPLU,                      	/* Plu start address */
		RamOfPort,						/* Port start address */
		RamOfTend,                      /* tendering function start address */
		RamOfPoRa,                      /* Po Ra functions */
		RamOfDrawer,                    /* Drawer start */
		RamOfCorr,                      /* correc start */
		RamOfDisc,                      /* discount start */
		RamOfCurr,                      /* Foreign currency start */
		RamOfTax,                      	/* Tax start */
		RamOfPBf,                      	/* PB-Function start */
		RamOfModi,                      /* modifier start */
		RamOfClerk,                     /* clerk start */
		RamOfOFF,						/* OFFPrice start*/
		RamOfICBlock,                  	/*   //ccr chipcard 2004-07-01 */
		RamOFSalPer,                  	/*  */
		RamOFAgree,						/*  */
		RamOfRPLU,                  	/* Random PLU's used (low word) */
		RamOfEndP,                      /* PROGRAM END Address */
		RamOfPBt,                      	/* PB table start */
		RamOfTrack                      /* track buffer start */
    },

    {	//struct PASSWORD ModePwd
    	{0,0,0,0,0,0,0},	//PwdX[MAXPWD];
    	{0,0,0,0,0,0,0},    //PwdZ[MAXPWD];
    	{0,0,0,0,0,0,0},    //PwdSET[MAXPWD];
    	{0,0,0,0,0,0,0}     //PwdMG[MAXPWD];
    },

    {   /* struct CCONFIG Config */
        0,        /* country code (0 = Europe) */
        2,        /* display (2 = 9 digit numeric) */
        3,      /* lock (2 = Central Lock, 3 = central & clerk) */
        128,    /* Journal density */
        128,    /* Receipt density */
        2,        /* Keyboard (2 = MA  60 keys) */
        15,        /* Clear key */
        -1,        /* Key Click 0 = disabled -1 = enabled) */
        {
            0x88,   /* disabled key locations each bit one key */
            0xff,
            0xff,
            0xff,
            0xff,
            0xff,
            0xff,
            0xbf,
            0xff,
            0xff,
            0xff,
            0xff,
        }
    },

    {   /* struct CLERKTABLE Clerk */
        clrkNumber      ,
        clrkRecordSize  ,
        clrkCapSize
    },

    {/* struct SALPERTABLE SalPer  */
        salNumber       ,
        salRecordSize   ,
        salCapSize
    },
    {       /* struct FIXEDREPORT ReportList[13] */
        {
            {ReportList1},//Ccr REPID_CLERKDAILY  {" 收 款 员 日 报 "},
            PLAYOUT,   /* print */
            0x00,   /* options */
            0x00,  /* number */
            0x00,   /* period */
            0x01,   /* pointer type */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_PLU+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList2},//Ccr REPID_CLERKPERIOD   {" 收 款 员 周 报 "},
            PLAYOUT,   /* print */
            0x00,   /* options=0,指定收款员 */
            0x00,  /* number */
            0x01,   /* period */
            0x01,   /* pointer type */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_PLU+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList3},//ccr REPID_DAILYREPORT   {" 销  售  日  报 "},
            PLAYOUT,   /* print */
#if defined(FISCAL)
            0x06,   /* options */
#else
            0x02,
#endif
            0x00,  /* number */
            0x00,   /* period=0,标示打印日统计数据 */
            0x00,   /* pointer type */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList4},//ccr REPID_PERIODREPORT  {" 销  售  周  报 "},
            PLAYOUT,   /* print */
            0x02,   /* options=2,全部记录 */
            0x00,  /* number */
            0x01,   /* period=1,标示打印周统计数据 */
            0x00,   /* pointer type */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList5},//ccr REPID_PLUDAILY {" 商  品  报  表 "},
            PLAYOUT,   /* print */
            0x02,   /* options=2,全部商品 */
            0x00,  /* number */
            0x00,   /* period=0,日统计数据 */
            0x00,   /* pointer type */
            {REPORT_PLU+1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList6},//ccr REPID_TABLEREPORT {" 桌  帐  报  表 "},
            PLAYOUT,   /* print */
            0x02,   /* options */
            0x00,  /* number */
            0x00,   /* period */
            0x00,   /* pointer type */
            {REPORT_BALANCE+1, REPORT_TABLES+1, REPORT_PBFUNC+1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList7},//ccr REPID_TIMEZONE {" 时  段  报  表 "},
            PLAYOUT,   /* print */
            0x02,   /* options=2,为全部记录 */
            0x00,  /* number */
            0x00,   /* period=0,为日报表 */
            0x02,   /* pointer type=2,为时段报表 */
            {REPORT_GRANDTOTAL+1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList8},//ccr REPID_ALLCLERKDAILY {"  全收款员日报  "},
            PLAYOUT,   /* print */
            0x02,   /* options=2,全部收款员 */
            0x00,  /* number */
            0x00,   /* period=0,为日报表 */
            0x01,   /* pointer type=1,为收款员报表 */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_PLU+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList9},//ccr REPID_ALLCLERKPERIOD {"  全收款员周报  "},
            PLAYOUT,   /* print */
            0x02,   /* options=2;为全部收款员 */
            0x00,  /* number */
            0x01,   /* period=1,为周报表 */
            0x01,   /* pointer type=1,为收款员报表 */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_PLU+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0}  /* Report type link */
        },
#if (salNumber)
        {
            {ReportList10},//ccr2017-05-26 REPID_SALERDAILY /*营 业 员 日 报*/
            PLAYOUT,   /* print */
            0x00,   /* options */
            0x00,  /* number */
            0x00,   /* period */
            0x05,   /* pointer type */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_PLU+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList11},//Ccr REPID_SALERPERIOD 营 业 员 周 报
            PLAYOUT,   /* print */
            0x00,   /* options */
            0x00,  /* number */
            0x01,   /* period */
            0x05,   /* pointer type */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_PLU+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList12},//ccr REPID_ALLSALERDAILY {"  全营业员日报  "},
            PLAYOUT,   /* print */
            0x02,   /* options */
            0x00,  /* number */
            0x00,   /* period */
            0x05,   /* pointer type */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_PLU+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0}  /* Report type link */
        },
        {
            {ReportList13},//ccr REPID_ALLSALERPERIOD {"  全营业员周报  "},
            PLAYOUT,   /* print */
            0x02,   /* options */
            0x00,  /* number */
            0x01,   /* period */
            0x05,   /* pointer type */
            {REPORT_GRANDTOTAL+1,
                REPORT_DEPARTMENT+1,
                REPORT_PLU+1,
                REPORT_TENDER+1,
                REPORT_PORA+1,
                REPORT_DRAWER+1,
                REPORT_CORRECTION+1,
                REPORT_DISCOUNT+1,
                REPORT_CURRENCY+1,
                REPORT_TAX+1,
                REPORT_PBFUNC+1,
                REPORT_BALANCE+1,
                0, 0, 0, 0}  /* Report type link */
        },
#endif
    },
    {       /* struct TOTALSALES Sales,REPORT_GRANDTOTAL */
        totalRecordSize, //Size[0].Length*2+Size[1].Length*clerkNumber + Size[2].Length * zoneNumber
        {/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
            {0x03, 	(toCC+toQTY+toAMT+toRET+toDISC+toCOST+1), toCC, toQTY, toAMT, toRET, toDISC, toCOST},       /*ccr090812 for SubDisc standard Total lay-out */
		    {0x03, 	toAMT+1, 		0, 0, toAMT, 	0, 		0, 0},   /* Clerk Total lay-out */
		    {0x01, 	(toCC+toAMT+1), toCC, 0, toAMT, 	0, 		0, 0},   /* Time zones Total lay-out */
		    {0x00, 	0, 		0, 0, 0, 	0, 		0, 0},   /* Day Total lay-out */
		    {0x00, 	0, 		0, 0, 0, 	0, 		0, 0},   /* Month Total lay-out */
		    {0x03, 	toAMT+1, 		0, 0, toAMT, 	0, 		0, 0},   /* Saler Total lay-out */
		}
    },

    {   /* struct CORRECTABLE Correc,REPORT_CORRECTION */
		corrNumber		,          /* number of correction functions */
		corrRecordSize	,
		corrTotalOffSet	,
		corrCapSize		,
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
		    {0x03, (crCC+crQTY+crAMT+crRET+crDISC+crCOST+1), crCC, crQTY, crAMT, crRET, crDISC, crCOST},   /* standard tlzr lay-out */
		    {0x03, (crCC+crQTY+crAMT+crRET+crDISC+crCOST+1), crCC, crQTY, crAMT, crRET, crDISC, crCOST},   /* clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x03, (crCC+crQTY+crAMT+crRET+crDISC+crCOST+1), crCC, crQTY, crAMT, crRET, crDISC, crCOST},   /* saler tlzr lay-out */
		}
    },

    {   /* struct CURRTABLE Curr,REPORT_CURRENCY */
		currNumber		,          /* number of foreign currencies */
		currRecordSize	,
		currTotalOffSet	,
		currCapSize		,
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
		    {0x03, (cuCC+cuAMT+1), cuCC, 0, cuAMT, 0, 0, 0},   /* standard tlzr lay-out */
		    {0x03, cuAMT+1, 0, 0, cuAMT, 0, 0, 0},   /* clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x03, cuAMT+1, 0, 0, cuAMT, 0, 0, 0},   /* saler tlzr lay-out */
		}
    },


    {   /* struct DEPTTABLE Dept,REPORT_DEPARTMENT */
		depNumber		,        /* number of Dept */
		depRecordSize	,         /* Record size */
		depTotalOffSet	,         /* Total offset */
		depRandomSize	,          /* Random Number Size */
		depCapSize		,            /* Caption Length */
		depPriceSize	,		/* max size is 4 */
		depPriMaxSize	,		/* max size is 4 */
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
            /*|次数|数量|销售额|退货项数|折扣汇总|COST汇总 */
		    {0x03, (dCC+dQTY+dAMT+dRET+dDISC+dCOST+1), dCC, dQTY, dAMT, dRET, dDISC, dCOST},   /* standard tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* saler tlzr lay-out */
		}
    },
    {       /* struct DISCTABLE Disc ,REPORT_DISCOUNT */
		discNumber		,          /* number of discount functions */
		discRecordSize	,
		discTotalOffSet	,
		discCapSize		,
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
		    {0x03, (xCC+xQTY+xAMT+xRET+xDISC+xCOST+1), xCC, xQTY, xAMT, xRET, xDISC, xCOST},   /* standard tlzr lay-out */
		    {0x03, (xCC+xQTY+xAMT+xRET+xDISC+xCOST+1), xCC, xQTY, xAMT, xRET, xDISC, xCOST},   /* clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},    /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x03, (xCC+xQTY+xAMT+xRET+xDISC+xCOST+1), xCC, xQTY, xAMT, xRET, xDISC, xCOST},   /* saler tlzr lay-out */
		}
    },
    {       /* struct DRAWERTABLE Draw,REPORT_DRAWER */
		drawNumber		,          /* number of drawer tlzr */
		drawRecordSize	,
		drawTotalOffSet	,          /* 1 byte for options, 1 for print */
		drawCapSize		,
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
		    {0x03, drAMT+1, 0, 0, drAMT, 0, 0, 0},   /* standard tlzr lay-out */
		    {0x01, drAMT+1, 0, 0, drAMT, 0, 0, 0},   /* clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x01, drAMT+1, 0, 0, drAMT, 0, 0, 0},   /* saler tlzr lay-out */
		}
    },

    {       /* struct GROUPTABLE Group,REPORT_GROUP */
		grpNumber,
		grpRecordSize,
		grpTotalOffSet,
		grpCapSize,
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
		    {0x03, (gCC+gQTY+gAMT+gRET+gDISC+gCOST+1), gCC, gQTY, gAMT, gRET, gDISC, gCOST},   /* standard tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* saler tlzr lay-out */
		}
    },
    {   /* struct MODITABLE Modi */
		modiNumber	,
		modiCapSize ,
    },

    {       /* struct PBTABLE Pb */
        pbNumber        ,          /* number of PB functions */
        pbRecordSize    ,            /* record size PB function */
        pbTotalOffSet   ,         /* total offset */
        pbCapSize       ,          /* caption length */

		pbNumberOfPb	,        /* number of PB's */
		pbRandom		,          /* Pb random */
		pbText			,          /* Pb text */
		pbAmtSize		,          /* PB amount size is 5 (table contains also block clerk and lines */
		pbArtAmt		,          /* max art amount 999999 */
		pbQty			,          /* max qty 999999 */
		pbArtSize		,          /* amt + qty + sign + keycode */
		pbNumberOfItems	,         /* number of items in block */
		pbNumberOfBlocks,         /* number of blocks */
		pbBlockSize		,        /* block size maximum is 128 bytes */
        pbtRecordSize,          // Record size for PBTable report ccr 2003-10-31
        pbtTotalOffset,         //ccr 2003-10-31
        {/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*///REPORT_PBFUNC, For pb function//ccr2017-05-26 删除统计
		    {0x03, (pfCC+pfQTY+pfAMT+pfRET+pfDISC+pfCOST+1), pfCC, pfQTY, pfAMT, pfRET, pfDISC, pfCOST},   /* standard tlzr lay-out */
		    {0x03, (pfCC+pfQTY+pfAMT+pfRET+pfDISC+pfCOST+1), pfCC, pfQTY, pfAMT, pfRET, pfDISC, pfCOST},   /* clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x03, (pfCC+pfQTY+pfAMT+pfRET+pfDISC+pfCOST+1), pfCC, pfQTY, pfAMT, pfRET, pfDISC, pfCOST},   /* saler tlzr lay-out */
		},
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*///REPORT_TABLES,for pb table //ccr2017-05-26 删除统计
		    {0x01, (ptCC+ptQTY+ptAMT+ptRET+ptDISC+ptCOST+1), ptCC, ptQTY, ptAMT, ptRET, ptDISC, ptCOST},   /* standard tlzr lay-out */
		    {0x01, (ptAMT+1), 0, 0, ptAMT, 0, 0, 0},   /* clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x01, (ptAMT+1), 0, 0, ptAMT, 0, 0, 0},   /* saler tlzr lay-out */
		}
    },
    {       /*    struct PLUTABLE Plu,REPORT_PLU     */
		pluNumber		,           /* Number:number of plus */
		0,						    /* RNumber:number of RandomPLU */
		pluRecordSize	,           /* RecordSize:Record size (+ Dept selection)*/
		pluTotalOffSet	,           /* TotalOffSet:Total offset */
		pluRandomSize	,           /* RandomSize:Random Number Size */
		pluCapSize		,           /* CapSize:Caption Length */
		pluLevel		,           /* Level:Number of Price Levels */
		pluPriceSize	,           /* PriceSize:Price size in bytes */
		pluCost			,           /* Cost:if not zero then cost price */
		pluInvSize		,           /* InvSize:inventory size (sign + size) */
        //struct  REPSIZE Size[REPDEFMAX]
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
		    {0x01, (pCC+pQTY+pAMT+pRET+pDISC+pCOST+1), pCC, pQTY, pAMT, pRET, pDISC, pCOST},       /* standard tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   		/* Clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},    	/* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   		/* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},    	/* Month tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   		/* saler tlzr lay-out */
		}
    },
    {       /* struct PORATABLE PoRa,REPORT_PORA */
		poraNumber		,          /* number of PO  & RA functions */
		poraRecordSize	,
		poraTotalOffSet	,
		poraCapSize		,
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
		    {0x03, (prCC+prQTY+prAMT+prRET+prDISC+prCOST+1), prCC, prQTY, prAMT, prRET, prDISC, prCOST},   /* standard tlzr lay-out */
		    {0x03, prAMT+1, 0, 0, prAMT, 0, 0, 0},   /* clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x03, prAMT+1, 0, 0, prAMT, 0, 0, 0},   /* saler tlzr lay-out */
		}
    },
    {       /* struct TAXTABLE Tax,REPORT_TAX */
		taxNumber		,              /* (3) number of tax itemizers max 8 */
		taxRecordSize	,
		taxTotalOffSet	,
		taxCapSize		,
		{/*Periods,	Length,            Cc, Qty,  Amt,RetQty,Disc,Cost*/
		    {0x03, (tQTY+tAMT+tCOST+1), 0, tQTY, tAMT, 0, 0, tCOST},   /* standard tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Clerks TLZR layout */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Saler TLZR layout */
		}
    },
    {       /* struct TENDTABLE Tend,REPORT_TENDER */
		tendNumber		,          /* number of tendering functions */
		tendRecordSize	,
		tendTotalOffSet	,
		tendCapSize		,
		{/*Periods,	Length,Cc,Qty,Amt,RetQty,Disc,Cost*/
		    {0x03, (cCC+cQTY+cAMT+cRET+cDISC+cCOST+1), cCC, cQTY, cAMT, cRET, cDISC, cCOST},   /* standard tlzr lay-out */
		    {0x03, cAMT+1, 0, 0, cAMT, 0, 0, 0},   /* clerk tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Time zones tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Day tlzr lay-out */
		    {0x00, 0, 0, 0, 0, 0, 0, 0},   /* Month tlzr lay-out */
		    {0x03, cAMT+1, 0, 0, cAMT, 0, 0, 0},   /* clerk tlzr lay-out */
		}
    },
    {       /* struct ZONETABLE Zone */
		zoneNumber,
		{
		    0x0000,         /* zone start */
		    0x0200,
		    0x0400,
		    0x0600,
		    0x0800,
		    0x1000,
		    0x1200,
		    0x1400,
		    0x1600,
		    0x1800,
		    0x2000,
		    0x2200,
		    0,0,0,0,0,0,0,0,0,0,0,0     /* notused */
		},
	},
    {   /* struct DAYTABLE Day */
        dayNumber
    },

    {       /* struct MONTHTABLE Month */
        monthNumber
    },
	{//struct OPTIONTABLE                                        /* Option */
		0,0,0,0,0,0,0,0,"",0,
	},
	{// struct OFFTABLE OFFPrice
		offNumber,
		offRecordSize,
		offCapSize,
		offPriUSize,
	},
	{// struct PORTTABLE Port
		portNumber,
		portRecordSize,
		portCapSize,
	},

	{//struct PROMRECORD 	Promotion;
		1000,
		{0x00,0x00,0x01,0x00},
		{0x00,0x00,0x01,0x00},
		{0x00,0x00,0x01,0x00},
		2,
		Promotion1,     //	 "  ~恭  ~喜  ~中  ~奖",
	},

	{//strcut AGREETABLE Agree
		agreeNumber,
		agreeRecordSize,
		agreeTotalOffSet,
		agreeCapSize,
	},
	{   /* struct ICBLOCKTABLE ICBlock */  //ccr chipcard 2004-07-01
		icblockNumber	,
		icblockSize ,
     },
};


CONST struct DEFAULT Def = {
	{               /* CORRECRECORD */
		{ PLAYOUT, 0x00, Correc1},//Ccr "更改    "},
		{ PLAYOUT, 0x01, Correc2},//Ccr "取消    "},
		{ PLAYOUT, 0x02, Correc3},//Ccr "退货    "},
		{ PLAYOUT, 0x03, Correc4},//Ccr "取消交易" //
		{ PLAYOUT, 0x04, Correc5},//Ccr "挂账" //
	},
	{       /* CURRENCY */
		{0, 8, 0, 0, {0,0,0,0}, {0,0,0,0}, CURRENCY1},//"美元    "
#if defined(DEBUGBYPC)
		{0, 0, 0, 0, {0,0,0,0}, {0,0,0,0}, CURRENCY2},//Ccr"日元    "},
		{0, 0, 0, 0, {0,0,0,0}, {0,0,0,0}, CURRENCY3},//Ccr"港币    "},
		{0, 0, 0, 0, {0,0,0,0}, {0,0,0,0}, CURRENCY4},//Ccr"外币1   "},
		{0, 0, 0, 0, {0,0,0,0}, {0,0,0,0}, CURRENCY5},//Ccr"外币2   "},
#endif
	},
	{       /* DISCOUNT */
		{PLAYOUT, 0x19, {0, 0, 0}, 0, {0x00,0x99,0x00,0}, DISCOUNT1},         /*  DISC 1  "%加成"},    */
		{PLAYOUT, 0x11, {0, 0, 0}, 0, {0x00,0x99,0x00,0}, DISCOUNT2},         /*  DISC 2 "%折扣"},  */
//ccr090812 for SubDisc>>>>>>>>>>>>>>>>>
		{PLAYOUT, 0x1d, {0, 0, 0}, 0, {0x00,0x99,0x99,0}, DISCOUNT3},         /*  DISC 3 , "金额加成"}  */
		{PLAYOUT, 0x15, {0, 0, 0}, 0, {0x00,0x99,0x99,0}, DISCOUNT4},         /* DISC 4 "金额折扣"}, */

        {PLAYOUT, 0x09, {0, 0, 0}, 0xff, {0x00,0x99,0x00,0}, DISCOUNT6},	  /* DISC 5 "小计%加成"},*/
		{PLAYOUT, 0x01, {0, 0, 0}, 0xff, {0x00,0x99,0x00,0}, DISCOUNT7},	  /* DISC 6 "小计%折扣"}, */
		{PLAYOUT, 0x0d, {0, 0, 0}, 0xff, {0x00,0x99,0x99,0}, DISCOUNT8},	  /*  DISC 7 "小计金额+"}*/
		{PLAYOUT, 0x05, {0, 0, 0}, 0xff, {0x00,0x99,0x99,0}, DISCOUNT9},	  /* DISC 8 "小计金额-"}, */
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    },
	{       /* DRAWER */
		{PLAYOUT, 0, DRAWER1},//Ccr 		{PLAYOUT, 0, "现金    "},
		{PLAYOUT, 0, DRAWER2},//Ccr 		{PLAYOUT, 0, "支票    "},
		{PLAYOUT, 0, DRAWER3},//Ccr 		{PLAYOUT, 0, "信用卡  "},
		{PLAYOUT, 0, DRAWER4},//	{PLAYOUT, 0, "购物券  "},
		{PLAYOUT, 0, DRAWER5},//cc epos//		{PLAYOUT, 0, "赊账    "},
		{PLAYOUT, 0, DRAWER6},//		{PLAYOUT, 0, "IC卡    "},
		{PLAYOUT, 0, DRAWER7},//		{PLAYOUT, 0, "小费    "},
		{PLAYOUT, 0, DRAWER8},
	},
	{       /* PB FUNCTIONS */
		{PLAYOUT, 0x00, PBFunction0},    // 开台挂帐  PB-F 1  //
		{PLAYOUT, 0x01, PBFunction1},    // 开台增加 PB-F 2 //
		{PLAYOUT, 0x02, PBFunction2},    //  暂结  PB-F 3 //
		{PLAYOUT, 0x03, PBFunction3},    // 确认结帐  PB-F 4 //
		{PLAYOUT, 0x04, PBFunction4},    // 打印开台PB-F 5 //
		{PLAYOUT, 0x05, PBFunction5},    // 打印帐单 PB-F 6 //
		{PLAYOUT, 0x06, PBFunction6},    // 取消确认 PB-F 7 //
		{PLAYOUT, 0x07, PBFunction7},    // 分帐 PB-F 8 //
		{PLAYOUT, 0x08, PBFunction8},    //  转帐 PB-F 9 //
		{PLAYOUT, 0x09, PBFunction9},    // 转桌帐  PB-F 10 //
		{PLAYOUT, 0x0A, PBFunction10},   //  人数 PB-F 11 //
	},
	{       /* PORA */
		{PLAYOUT, 0x01, 1, 0, PORAType1},         /* PORA 1 "出金    "*///ccr040809
		{PLAYOUT, 0x09, 1, 0, PORAType2},         /* PORA 2 "入金    "*/
		{PLAYOUT, 0x01, 1, 0, PORAType3},         /* PORA 3 "IC卡退款"*/
		{PLAYOUT, 0x09, 1, 0, PORAType4},         /* PORA 4 "IC卡充值"*/
		{PLAYOUT, 0x01, 1, 0, PORAType5},         /* PORA 5退IC押金" */
		{PLAYOUT, 0x09, 1, 0, PORAType6},         /* PORA 6"收IC押金" */
	},
	{       /* TEND */
		{PLAYOUT, 0x01, 1,  0, 0, TendType1},         /* TEND 1"现金    " */
		{PLAYOUT, 0x05, 2,  0, 0, TendType2},         /* TEND 2 "支票    "*/
		{PLAYOUT, 0x05, 3,  0, 0, TendType3},         /* TEND 3 "信用卡  "*/
		{PLAYOUT, 0x11, 4,  7, 0, TendType4},         /* TEND 4 "购物券  "*/
		{PLAYOUT, 0x05, 5,  0, 0, TendType5},       //cc epos"赊账"
		{PLAYOUT, 0x05, 6,  0, 0, TendType6},         /* TEND 6 "ICCard"*/
        {PLAYOUT, 0x05, 7,  0, 0, TendType7},         /* TEND 7"微信   " */
        {PLAYOUT, 0x01, 8,  0, 0, TendType8},         /* TEND 8"支付宝   " */
	},
	{       /* TAX */
		{"TAX_A", {0,0,0}, 0},
		{"TAX_B", {0,0,0}, 0},
		{"TAX_C", {0,0,0}, 0},
//		{"TAX_D", {0,0,0}, 0},
//		{"TAX_E", {0,0,0}, 0},
//		{"TAX_F", {0,0,0}, 0}

	},
	{
		Modifier1,             /* Modifier */
//Ccr 		"说明    ",             /* Modifier */
	},
	{
		ClerkRec1, 0xff,{0,0,0},                /* Clerk */
//Ccr 		"收款员  ", 0x10                /* Clerk */
	},
	{
		99, GroupRec1,             /* Group GROUPRECORD */
//Ccr 		0x0000, "柜组    ",             /* Group */
	},
	{ /* DEPTRECORD */
		{0,0,0,0,0,0,0}, 	//Random
		DeptRec1,	//name
		01,PLAYOUT,0xc0,0,0,
		{0,0,0,0,0},		//Price
		{0,0,0,0,0}         //PriceMax
//Ccr 		{0,0,0,0,0,0,0}, "部类    ",0,PLAYOUT,0,0,0,0           /* Department */
	},
	{	/* PLU */
		{0,0,0,0,0,0,0}, 			//Random
		PLURec1,		//Name
		0,							//OFFIndex
		2,							//Dept
		0,							/* High Dept Number */
//Ccr 		{0,0,0,0,0,0,0}, "商品    ",0,
		{							//Price
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{0,0,0,0,0},				//Cost
		0,							/* PLU Link */
		{0,0,0,0,0,0}				//NotUsed
	},
	{
		SalesPersonRec1  , 0x00                /* Salesperson */
//Ccr 		"营业员  ", 0x00                /* Salesperson */
	},
	{// struct OFFRECORD OFFPrice
		OffRec1,
                //"特殊出价"
		0,								//Type
		{0x01,0x01},							//Date From
		{0x01,0x01},							//Date to
		{0x01,0x01},							//Time from
		{0x01,0x01},							//Time to
		0,								//Week day
		0,//{0,{0,0,0,0},{0,0,0,0}},			//discount
	},
	{// struct PORTRECORD Port
		"PORT ",
		'1',								//Type,host
#if (CASE_EPOS)
		{0x47,0x43,0x43,0x43,0x43,0x43,0x43},//80181 //ccr2016-12-14 ExtPrinter串口
#else
		{0x47,0x43,0x43,0x43,0x43,0x43},//80181 //ccr2016-12-14 ExtPrinter串口
#endif
	},
	{//struct GRAPFORPRN Graph
		0,{0,0,0,0},{0,0,0,0},
	},
	{//strcut AGREERECORD Agree
		"Agree",
		"TELE",
		"10000000",
		0,
	},
    {//struct ICBLOCKRECORD ICBlock;  //ccr chipcard 2004-07-01
        0
    },
    {//struct IPSETRECORD IP;      //ccr //ccr2014-12-25
        {0,0,0,0},0,
        {0,0,0,0},0,
    }
};

CONST char UPCOMM[][4]={
    "UOK",			//0
    "UCA",			//1
    "UEN",			//2
    "UER",			//3
};


//Captions for XZ report
CONST struct XZREPORT XZTitle[XZNUM]={
		{REPID_DAILYREPORT+1,   XZReportList1},//3, "销售日报表"},
		{REPID_PERIODREPORT+1,  XZReportList2},//4, "销售月报表"},
		{REPID_PLUREPORT+1,     XZReportList3},//5, "商品报表"},
		{REPID_TABLEREPORT+1,   XZReportList4},//6, "桌帐报表"},
		{REPID_TIMEZONE+1,      XZReportList5},//7, "时段报表"},
		{REPID_CLERKDAILY+1,    XZReportList6},//1, "收款员日报表"},
		{REPID_CLERKPERIOD+1,   XZReportList7},//2, "收款员月报表"},
		{REPID_ALLCLERKDAILY+1, XZReportList8},//8, "全收款员日报"},
		{REPID_ALLCLERKPERIOD+1,XZReportList9},//9, "全收款员月报"},
#if (salNumber)
		{REPID_SALERDAILY+1,    XZReportList10},//10,"营业员日报表"},
		{REPID_SALERPERIOD+1,   XZReportList11},//11,"营业员月报表"},
		{REPID_ALLSALERDAILY+1, XZReportList12},//12,"全营业员日报表"},
		{REPID_ALLSALERPERIOD+1,XZReportList13},//13,"全营业员月报表"},
#endif
#if (BARCUSTOMER==1)
		{REPID_BARCUSTOMER+1,XZReportList14},//Ccr 		{14,"条码会员报表"},
#endif
	};

CONST char *PortType[portTypeNum]={//用于显示和打印
		PortType1,//Ccr   "  主机",
		PortType2,//Ccr   "条码枪",
		PortType3,//Ccr   "电子秤",
		PortType4,//Ccr   "厨  打",
		PortType5,//ccr   "票  打",
        PortEXTPRN,//ccr2016-12-14 ExtPrinter串口
#if (CASE_EPOS)
        PortType7,//cc    "E通卡",//ePos
#endif
	};


CONST char *KPType[KPTypeNum]={
		KClose,       //"   关 ",
		"INTERNAL",   //   "内置打印机",
		"  T58A-C",   //（58mm A字体，带切刀,32字符）
		"  T80A-C",   //（80mm，A字体，应该带切刀，但是程序里没切刀，测试确认一下)
        "  T58B-C",   //（58mm B字体，带切刀,42字符）
		"  D76A-C",   // 76mm针打，32字符，A字体，带切刀。增加的型号
		" CITIZEN",   //西铁城打印机, 30字符宽度
		"  T58A  ",   //（58mm A字体，无切刀）
};

CONST char *SPType[SPTypeNum]={
		KClose,      //Ccr "   关 ",
		"CTM290",
		"TM2902",
		" ESP-C",
		"ESP-NC",
};


#if (CASE_GIADA3000)
CONST WORD  KeyTablePBF[MAXKEYB] ={// KeyTable[128]
			JPF,		SHIFT1,	    CLERK,		xBEFOR,	xREFUND,	DISC_PERDEC,	//00-05
			MODELOCK,	0,			NPRICE,		xLASTONE,	xRECEIPT,	DISC_NETDEC,		//07-11
			CLEAR,		MULT,		PLU1,		DEPT+5,		PBF+4,		PBF+1,		//12-17
			'7',		'8',		'9',		DEPT+4,		PBF+7,		PBF+5, 		// 2,		//18-23
			'4',		'5',		'6',		DEPT+3,		DEPT+6,		SUB,		//24-29
			'1',		'2',		'3',		DEPT+2,		0,			TEND+1,		//30-35
			'0',		ZERO2,		'.',		DEPT+1,		0,			0,			//36-41
			0,			0,			0,			0,			0,			0,			//42-47
			0,			0,			0,			0,			0,			0,			//48-53
			0,			0,			0,			0,			0,			0,			//54-59
			0,			0,			0,			0xffff,									//60-63
	};
#elif (defined(CASE_ER100)||defined(CASE_FORHANZI))
CONST WORD  KeyTablePBF[MAXKEYB] ={// KeyTable[128]
            MODELOCK,	JPF,	CLEAR,	MULT,		VIPLOGIN,	xLASTONE,   xREFUND,
            CLERK,		'7',	'8',	'9',		PBF+4,		PBF+1,		DISC_PERDEC,
            FUNCLOOK1,	'4',	'5',	'6',		PBF+7,		PBF+5,		TEND_COUPON,
            NPRICE, 	'1',	'2',	'3',		DEPT+2,		DEPT+4,		SUB,
            PLU1,	    '0',	ZERO2,  '.',		DEPT+1,		DEPT+3,		TEND+1,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0xffff,
	};
#elif (defined(CASE_ER220)||defined(CASE_ER100))
CONST WORD  KeyTablePBF[MAXKEYB] ={// KeyTable[128]
			MODELOCK,	JPF,	CLEAR,	MULT,		PLU1,		NPRICE,		DISC_PERDEC,
			CLERK,		'7',	'8',	'9',		PBF+4,		PBF+1,		DISC_PERINC,
			FUNCLOOK1,	'4',	'5',	'6',		PBF+7,		PBF+5,		TEND_COUPON,
			xBEFOR,	    '1',	'2',	'3',		DEPT+2,		DEPT+4,		SUB,
			xLASTONE,	'0',	ZERO2,  '.',		DEPT+1,		DEPT+3,		TEND+1,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0,			0,		0,		0,			0,			0,			0,
			0xffff,
	};
#elif CASE_ER520U
CONST WORD  KeyTablePBF[MAXKEYB] ={// KeyTable[128]
			JPF,		SHIFT1,		PBF+1,	DEPT+10,	DEPT+5,		MULT,	PLU1,	NPRICE,	ODRAW,	DISC_NETDEC,
			PORA+1,		CLERK,		PBF+6,	DEPT+9,		DEPT+4,		'7',	'8',	'9',	DISC_PERDEC,	DISC_PERINC,
			PORA+2,		xLASTONE,	PBF+5,	DEPT+8,		DEPT+3,		'4',	'5',	'6',	TEND+3,	TEND+4,
			PBF+11,		xBEFOR,	    PBF+4,	DEPT+7,		DEPT+2,		'1',	'2',	'3',	SUB,
			CLEAR,		xRECEIPT,	PBF+7,	DEPT+6,		DEPT+1,		'0',	ZERO2,	'.',	TEND+1,
			0,			0,			0,			0,			0,		0,		0,		0,			0,		0,
			0,			0,			0,			0,			0,		0xffff,
	};

#elif CASE_ER260

CONST WORD  KeyTablePBF[MAXKEYB] ={// KeyTable[128]
			JPF,		MULT,	NPRICE,	PLU1,	PBF+4,	PBF+1,	    DISC_NETDEC,DISC_PERDEC,		//00-07
			MODELOCK,	'7',	'8',	'9',	PBF+7,	PBF+5,		xREFUND,	xBEFOR,	//08-15
			CLERK,		'4',	'5',	'6',	DEPT+5,	FUNCLOOK1,	PORA+1,		xLASTONE,	//16-23
			ODRAW,		'1',	'2',	'3',	DEPT+3,	DEPT+4,		PORA+2,		SUB,		//24-31
			CLEAR,		'0',	ZERO2,	'.',	DEPT+1,	DEPT+2,		TEND+4,		TEND+1,		//32-39
			0,			0,		0,		0,		0,		0,			0,			0,			//40-47
			0,			0,		0,		0,		0,		0,			0,			0,			//48-55
			0,			0,		0,		0,		0,		0,			0,			0,		//56-63
	};
#endif

//!!!!  YN-表示0为true,1为false;无YN时0为false,1为true; !!!!
CONST struct SYSFLAGIDX SysFlagUsed[SYSUSED] ={
		{14,255,0,YN,SysFlags0},         //LOCATION "店铺编号"
		{15,255,0,YN,SysFlags1},	    //REGISTER 0-255"ECR_no    ""收款机号:"
#if (defined(CASE_GPRS) && defined(CASE_INDONESIA)) //ccr2015-08-03
        {26,0,  6, 0,SysFlags40},       //DBLHIPRN bit4 "向服务器发送Heart beat"
#endif
        {63,4,  0,YN,SysFlags2},	    //ROUNDMASK 0,1,2,3,4"Round_    ""舍入方式:"
		{24,0,  6,YN,SysFlags3},	    //PLU_PRINT  bit1"PLU_Free  "商品变价:"
		{4, 0,  1,YN,SysFlags4},	    //KEYTONE  bit0"beep      ""蜂鸣器:"
		{9, 0,  1, 0,SysFlags5},		//CLERKFIX  bit0"Obl_Oper  "   强制收款员"
//ccr20131015		{9, 0,  3, 0,SysFlags6},		//CLERKFIX  bit0"DisX_Oper "   改变收款员"
		{9, 0,  6, 0,CLERKPASS},		//CLERKFIX  bit0"Pass_Oper "   收款员密码"
#if (salNumber>0)
		{24,0,  5, 0,SysFlags8},		//PLU_PRINT  bit0"Saleper_Num  "  打印营业员"
#endif
		{9, 0,  4, 0,SysFlags9},		//CLERKFIX  bit0"Prin_Oper ""打印收款员"
		{1, 0,  4,YN,SysFlags10},		//COMMA bit3"Prin_Recn ""打印收据号"
//ccr20131111		{17,0,  8, 0,SysFlags11},		//DENSITY bit5"高浓度打印"
#if (salNumber>0)
		{4, 0,  7, 0,SysFlags12},		//KEYTONE bit6"Obl_Saleper_ ""强制营业员"
#endif
		{24,0,  1, 0,SysFlags13},		//PLU_PRINT bit0"ArtCode   ""打印编码:"
		{9 ,0,  8, 0,SysFlags14},		//CLERKFIX bit0"Obl_Subt_ ""按小计键:"
		{5, 0,  2, 0,SysFlags15},		//APPLCTRL bit0"Art_no_   ""打印销售量"
		{24,0,  7,YN,SysFlags16},		//PLU_PRINT bit7"TALLONS   ""打印小票:"
		{25,0,  5, 0,SysFlags17},		//DOT bit0"TALLON HEADER AND TAILER ""小票票头"
		{60,0,  8, 0,SysFlags18},		//SLFLAG bit7"En_Shift""锁定SHIFT:"
		{21,99, 0,YN,SysFlags19},		//ACTIVE 0..99"Time_IDLE ""显示时间:"
		{62,15, 0,YN,SysFlags20},        //COPYRECEIP "复制收据:"
		{25,0,  3, 0,SysFlags22},		//DOT bit4"select collect flow ""操作日志"
		{19,0,  5, 0,SysFlags23},		//PBPRINT bit4"select pb print " "开台打印:"//lyq2003
		{19,0,  6, 0,SysFlags24},		//PBPRINT bit4"select pb print ""打印库存 //lyq2003
		{25,0,  6, 0,SysFlags25},		//DOT bit5"allow press operator key to close printer" "打印开关:"
		{18,0,  1, 0,SysFlags26},		//PBINFO bit0"必须开台:
		{18,0,  5, 0,SysFlags27},		//PBINFO 保持收据号"
		{19,0,  7, 0,SysFlags28},		//PBPRINT "复制厨打:" //ccr040810
		{62,0,  8, 0,SysFlags29},		//COPYRECEIP "收据号复位" //ccr040810
		{26,0,  3, 0,SysFlags30},		//DBLHIPRN "倍高打印"bit2
        {1, 0,  7,YN,SysFlags37},		//COMMA  bit6"打印日期"
        {1, 0,  8,YN,SysFlags38},		//COMMA   bit7""打印时间"
        {10,255,0, 0,SysFlags41},       //PREFIX_1 货币符1
        {11,255,0, 0,SysFlags42},       //PREFIX_2 货币符2
        {27,0,  8, 0,SysFlags39},		//PROMOTION   bit7 打印取菜号，收款机号＋收据号
		{CONTRAST,15,  0,YN,SysFlags31},// DENSITY	 CONTRAST      ""对比度:"
//        {5, 0,  1,YN,SysFlags36},     // APPLCTRL bit1 "CSV格式导出"
};

CONST char GrapType[GRASETMAX][tCAPWIDTH]={
//		"SHOP TYPE ",
		GrapType1,//Ccr 	"商店图标",
		GrapType2,//Ccr 	"中奖图标:",
		GrapType3,//Ccr     "票尾图像:",
		GrapType4,//Ccr     "打印文字?",
};

CONST char GrapSet[4][tCAPWIDTH]={
		GrapSet1,//Ccr 		"序号:",
		GrapSet2,//Ccr 		"图像编号:",
		GrapSet3,//Ccr 		"开始日期:",
		GrapSet4,//Ccr 		"结束日期:",
};


CONST BYTE	ASCIIKEY[MAXKEYB]={
#if (CASE_GIADA3000)
            0,      0,	    'a',	'b',	'c',	0,	//00-05
            0,	    0,	    'd',	'e',	'f',	0,	//06-11
            0,	    0,	    'g',	'h',	'i',	'~',	//12-17
            'j',	'k',	'l',	'm',	'n',	0,		//18-23
            'o',	'p',	'q',	'r',	's',	0,		//24-29
            't',	'u',	'v',	'w',    0,	    0,		//30-35
            'x',	'y',    '.',	'z',    0,	    0,		//36-41

            0,	0,	0,	0,	0,	0,			//42-47
            0,	0,	0,	0,	0,	0,			//48-53
            0,	0,	0,	0,	0,	0,			//54-59
            0,	0,	0,	0,							//60-63
#elif (defined(CASE_ER220)||defined(CASE_ER100))
			0,		0,		0,		0,		'a',	'b',		0,
			'c',	'd',	'e',	'f',	'g',	'h',		0,
			'i',	'j',	'k',	'l',	'm',	'n',		0,
			'o',	'p',	'q',	'r',	's',	't',		0,
			'u',	'v',	'w', 	'x',	'y',	'z',		0,
			0,		0,		0,		0,		0,		0,			0,
			0,		0,		0,		0,		0,		0,			0,
			0,		0,		0,		0,		0,		0,			0,
			0,		0,		0,		0,		0,		0,			0,
			0,
#elif CASE_ER520U
			0,		0,		'a',	'b',	'c',	0,		'w',	'x',	'y',	'z',
			'd',	'e',	'f',	'g',	'h',	'7',	'8',	'9',	0,		0,
			'i',	'j',	'k',	'l',	'm',	'4',	'5',	'6',	' ',	0,
			'n',	'o',	'p',	'q',	'r',	'1',	'2',	'3',	0,
			0,		's',	't',	'u',	'v',	'0', 	0,		'.',	0,
			0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
			0,		0,		0,		0,		0,		0,

#elif CASE_ER260
            0,		0,	    'a',	'b',	'c',	'd',	    '~',	0,		//00-07
            0,	    'e',	'f',	'g',	'h',	'i',		'j',	0,	//08-15
            'k',	'l',	'm',	'n',	'o',	'p',		'q',	0,	//16-23
            'r',	's',	't',	'u',	'v',	'w',	    'x',    0,		//24-31
            0,		',',	' ',    '.',	'@',	'y',		'z',	0,		//32-39
                                                                                            //
            0,		0,		0,		0,		0,		0,			0,		0,			//40-47
            0,		0,		0,		0,		0,		0,			0,		0,			//48-55
            0,		0,		0,		0,		0,		0,			0,		0,		//56-63

#elif(CASE_WISE158==1)
        							                 0 , 0, 0,   0, 0 ,0 ,
        	'@','~', 0 , 0 ,	 0 , 0 , 0 ,0 ,	    'p','7','8','9',0,0 ,
        	'q','w','e','r',	't','y','u','i',	'o','4','5','6',0,0,
        	'a','s','d','f',	'g','h','j','k',	'l','1','2','3',0,0,
        	'z','x','c','v',	'b','n','m', 0,	     0, '0', 0, '.',0,0,
#else
			0,	's',	't',	'u',	'm',	'n',	'o',	'p',	//00-07
			0,	0,	'v',	'w',	'k',	'l',	'q',	'r',	//08-15
			'x',	'y',	'z',	0,	'i',	'j',	0,	0,		//16-23
			'@',	'7',	'8',	'9',	'g',	'h',	0,	0,		//24-31
			'~',	'4',	'5',	'6',	'e',	'f',	0,	0,		//32-39
			0,	'1',	'2',	'3',	'c',	'd',	0,	0,		//40-47
			0,	'0',	0,	'.',	'a',	'b',	0,	0,		//48-55

			0,	0,	0,	0,	0,	0,	0,	0,		//56-63
#endif
};


CONST BYTE	NUMASCII[MAXKEYB]={
#if (CASE_GIADA3000)
    0,	    0,	    '$',	'%',	'^',	0,	//00-05
    0,	    0,	    '(',	')',	'-',	0,	//06-11
    0,	    0,	    '{',	'}',	'@',	'~',	//12-17
    '7',	'8',	'9',	'\'',	'?',	0,		//18-23
    '4',	'5',	'6',	':',	';',	0,		//24-29
    '1',	'2',	'3',	'[',    0,	    0,		//30-35
    '0',	'#',    '.',	']',    0,	    0,		//36-41

    0,	0,	0,	0,	0,	0,			//42-47
    0,	0,	0,	0,	0,	0,			//48-53
    0,	0,	0,	0,	0,	0,			//54-59
    0,	0,	0,	0,							//60-63
#elif (defined(CASE_ER220)||defined(CASE_ER100))
    0,	    0,	    0,	    0,		'!',	 '&',	'~',
    '(',	'7',	'8',	'9',	'$',	 '*',	0,
    ')',	'4',	'5',	'6',	'-',	 ' ',	0,
    '%',	'1',	'2',	'3',	'?',	 '@',	0,
    '+',	'0',	';',   '.',		':',	 0x27,	0,
    0,	    0,	    0,	    0,		0,		0,		0,
    0,	    0,	    0,	    0,		0,		0,		0,
    0,	    0,	    0,	    0,		0,		0,		0,
    0,	    0,	    0,	    0,		0,		0,		0,
    0,
#elif CASE_ER520U
    0,		0,		'~',	'!',	'@',	0,		'#',	'$',	'%',	'&',
    '(',	')',	'_',	'+',	'^',	'7',	'8',	'9',	0,		0,
    '-',	'=',	'{',	'}',	'|',	'4',	'5',	'6',	'/',	0,
    '[',	']',	':',	';',	'"',	'1',	'2',	'3',	0,
    0,		'\'',	',',	'<',	'>',	'0', 	0,		'.',	0,
    0,		0,		0,		0,		0,		0,		0,		0,		0,		0,
    0,		0,		0,		0,		0,		0,

#elif CASE_ER260
    0,		0,	    '!',	':',	'#',	'$',	    '~',	0,		//00-07
    0,	    '7',	'8',	'9',	'%',	'^',		'&',	0,	//08-15
    '*',	'4',	'5',	'6',	'[',	']',		'/',	0,	//16-23
    '=',	'1',	'2',	'3',	'(',	')',		'-',	0,		//24-31
    0,		'0',    ' ',    '.',	'@',	'"',		'\'',	0,		//32-39
                                                                                    //
    0,		0,		0,		0,		0,		0,			0,		0,			//40-47
    0,		0,		0,		0,		0,		0,			0,		0,			//48-55
    0,		0,		0,		0,		0,		0,			0,		0,		//56-63

#elif CASE_WISE158
        0,	0,	0,	0,	0,	0,
    '@','~', '!',	0x22,'#','$',0,	 0,	 0,	'7','8','9',0,0,
    '%','&', 0x27,	'(', ')','*','+',',',0,	'4','5','6',0,0,
    '-','/', ':',	';', '<','=','>','?',0,	'1','2','3',0,0,
    '[',0x5c,']',	'^', '_','{','|','}',0,	'0',0,	'.',0,0,
    0,	0,

#else
    0,	's',	't',	'u',	'm',	'n',	'o',	'p',	//00-07
    0,	0,	'v',	'w',	'k',	'l',	'q',	'r',	//08-15
    'x',	'y',	'z',	0,	'i',	'j',	0,	0,		//16-23
    '@',	'7',	'8',	'9',	'g',	'h',	0,	0,		//24-31
    '~',	'4',	'5',	'6',	'e',	'f',	0,	0,		//32-39
    0,	'1',	'2',	'3',	'c',	'd',	0,	0,		//40-47
    0,	'0',	0,	'.',	'a',	'b',	0,	0,		//48-55

    0,	0,	0,	0,	0,	0,	0,	0,		//56-63
#endif
};


//ccr2016-01-21>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/**
 * 外设连接的端口号,字符串长度必须小余SETINPUTWD
 *
 * @author EutronSoftware (2016-01-21)
 */

CONST char PortForDevice[LINKTYPENUM][7]={
		DEVICENULL ,// "    无",
		DEVICEPORT1,// " 串口1",
        DEVICEPORT2,// " 串口2",
//      DEVICEPORT3,//   " 串口3",
//      DEVICEPORT4,//   " 串口4",
	};
//Speed:'1'-1200,'2'-2400,'3'-4800,'4'-9600,'5'-19200,'6'-38400,'7'-57600,'8'-115200
//长度必须小余SETINPUTWD
CONST char PortRate[PORTRATENUM][7]={
        "  1200",
        "  2400",
        "  4800",
        "  9600",
        " 19200",
        " 38400",
        " 57600",
        "115200"
	};


//ccr2016-03-17>>>>FuncLook>>>>>
//"开台关帐\0开台增加\0暂结\0确认结账\0打印开台\0打印帐单\0取消确认\0分帐\0转帐\0桌帐转移\0"
CONST WORD FuncLook[FUNCLOOKMAX][FUNCLOOK_KEYS]={
#if defined(CASE_FORHANZI)
    {
        PORA+2,     //PO:出金
        PORA+1,     //RA:入金
        VIPLOGIN,   //  会员登录
        TEND_WXPAY,     //微信支付
        TEND_ALPAY,     //支付宝
        SUSPEND,   //  (CORREC+5)   //为挂起操作
    },
    {
        TEND_CHECK,     //支票
        TEND_VISA,     //信用卡
        xRECEIPT,   //  (CORREC+4)    //为取消交易
        DISC_NETINC,    //	(DISC+3)      //为金额加成
        PBF+6,          //打印帐单
        PBF+7,          //取消确认
    }
#else
    {
        PORA+2,     //PO:出金
        PORA+1,     //RA:入金
        VIPLOGIN,   //  会员登录
#if defined(CASE_PERU)
        CUSTOMER,
#else
        TEND_VISA,     //微信支付
#endif
        xREFUND,     //退货
        SUSPEND,   //  (CORREC+5)   //为挂起操作
    },
    {
        TEND_CHECK,     //支票
        TEND_CREDIT,     //赊账
        xRECEIPT,   //  (CORREC+4)    //为取消交易
        DISC_NETINC,    //	(DISC+3)      //为金额加成
        PBF+6,          //打印帐单
        PBF+7,          //取消确认
    }
#endif
};
//ccr2017-03-16>>>>>>>>>>>>>>>>>>
CONST char *BALType[BALTYPEMAX]={
		"TOLEDO",
		" DIBAL",
        "  PS1X",
};
//ccr2017-03-16<<<<<<<<<<<<<<<<
