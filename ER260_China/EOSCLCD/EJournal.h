#ifndef EJOURNAL_H
#define EJOURNAL_H

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))

#if defined(DEBUGBYPC)
#pragma option -a1
#else
#pragma pack(1)
#endif
//-------------------------------------------------------------

#if defined(CASE_EJSD)
//EJ存储在SD卡 中
#define GETINDEXSIZE()   ( (ApplVar.EJVar.MMCSIZEMAX<=0x2000000L)?(10*1024):(ApplVar.EJVar.MMCSIZEMAX<=0x8000000L)?(40*1024):(128*1024) )

#define MMC_LESS			0x2000          // 当EJ剩余空间少于MMC_LESS时,报警EJ将满.
#define MMC_FULL			(1024)        // 当EJ剩余空间少于MMC_FULL时,认为EJ满了.
#define SD_BLOCKSIZE			512				// 每块字节,必须为512//

//     头指针保存地址 IDXENDADDRESS,此地址保存EJ读写管理数据EJHeader,考虑到兼容性，必须占1024字节 //
#define IDXENDADDRESS			((ApplVar.EJVar.MMCSIZEMAX-1024) & 0xfffffE00)//0x1E9FE00L

#define EJMEMORYFROM				0

#define EJHEADERADDR		EJMEMORYFROM  //EJ初始化信息EJHeader存放位置,占用1024字节
#define HEADSIZE			sizeof(ApplVar.EJHeader)		//实际占用1024字节

#define EJDOCADDRFROM			(EJMEMORYFROM+1024)    //第一个EJ记录数据存放位置考虑到兼容性，必须占1024字节 //

#define INDEXADDR			(IDXENDADDRESS - ApplVar.EJHeader.SizeEJIndex - SD_BLOCKSIZE)    //记录索引存放位置,存放按日期顺序的索引
#define INDEXRECORDSIZE	    10   //ccr2016-10-31//索引记录大小:2字节日期+4字节地址+四字节EJ#
#define INDEXRECORDCOUNT	(ApplVar.EJHeader.SizeEJIndex / INDEXRECORDSIZE)   //索引记录数

#define EJDOCADDRMAX			(INDEXADDR-SD_BLOCKSIZE)		// MMC卡实际可使用容量 //

#elif defined(CASE_EJFLASH)//----------------------------------------
//EJ存储在FLASH中
#define FLASHFOREJ       0x10000    //EJ数据在FLASH中存储开始地址
#define BLOCK1STFOREJ    4          //EJ数据在FLASH中存储开始块
#define BLOCKSFOREJ      31         //EJ数据在FLASH中存储占用块数

#define GETINDEXSIZE()   900    //可以存储90天的索引数据

#define MMC_LESS			0x2000          // 当EJ剩余空间少于MMC_LESS时,报警EJ将满.
#define MMC_FULL			(1024)        // 当EJ剩余空间少于MMC_FULL时,认为EJ满了.
#define SD_BLOCKSIZE			512				// 每块字节,必须为512//

#define EJMEMORYFROM		0x10000    //EJ数据在FLASH中存储开始地址

#define IDXENDADDRESS		(EJMEMORYFROM+1024)//索引区结束地址

//头指针保存地址 EJHEADERADDR,此地址保存EJ读写管理数据EJHeader,考虑到兼容性，必须占1024字节 //
#define EJHEADERADDR		EJMEMORYFROM  //EJ初始化信息EJHeader存放位置,占用1024字节
#define HEADSIZE			sizeof(TEJHEADER)		//实际占用1024字节

#define EJDOCADDRFROM			(EJMEMORYFROM+1024)    //第一个EJ记录数据存放位置考虑到兼容性，必须占1024字节 //

#define INDEXADDR			((HEADSIZE + 0x10 + EJMEMORYFROM) & 0xfffffff0)    //记录索引存放位置,存放按日期顺序的索引
#define INDEXRECORDSIZE	    10   //ccr2016-10-31//索引记录大小:2字节日期+4字节地址+四字节EJ#
#if defined(DEBUGBYPC)
#if (IDXENDADDRESS>INDEXADDR+INDEXRECORDSIZE*2)
#define INDEXRECORDCOUNT	((IDXENDADDRESS-INDEXADDR) / INDEXRECORDSIZE)   //索引记录数
#else
#error "Error:Size of EJ INDEX"
#endif
#else
#define INDEXRECORDCOUNT	((IDXENDADDRESS-INDEXADDR) / INDEXRECORDSIZE)   //索引记录数
#endif
#define EJDOCADDRMAX			(BLOCKSFOREJ*0x10000+EJMEMORYFROM)		// MMC卡实际可使用容量 //

#endif

//For ContFlag-------------------------------------------
#define EJSTOREMUST		BIT0	// Must Store into EJ
#define EJHEADMUST		BIT1	// Must Store EJ head first
#define EJDOCHEAD		BIT2	// EJ head has be store
#define EJENDMUST       BIT3    // 必须存入EJ文档结束标示

#define EJISNEW         BIT4    // 为新EJ,必须进行初始化
#define EJNOTEXIST      BIT5    // 没有EJ,必须插入EJ
#define EJxECR          BIT6    // EJ与收款机不匹配,须重新初始化
#define EJISFULL        BIT7    // EJ存满,必须更换EJ
#define EJWILLFULL      BIT8    // =1时,表示EJ将满

#define EJVIEWPRINT     BIT9    // =0,显示EJ;=1时,打印EJ
//----------------------------------------------------------------
//-------------EJ记录类型定义(EJContent)-----------------------
#define EJMARK	0xC5A3	//MARKER 'Ｅ' for EJ flag
//-------------EJ文档标志定义-----------------------------
#define CEJ_STARTDOC	0x0A	//\n = start new document
#define CEJ_STARTLN		0x0D	//\r = 为一行EJ文本数据
#define CEJ_STARTLND	0x0E	//0E = 为一行倍高的EJ文本数据
#define CEJ_DOCEND		0x1A	//^Z = EJ文档结束标志,其后一个字节为校验码
//CONST BYTE ZIPList[ZIPLISTMAX] ={' ','*','-','.','='};//最多5个
#define CEJ_ESC			0x1B	//ESC = number of spaces to be decompressed
#define CEJ_ESC1		0x1C	//ESC = number of * to be decompressed
#define CEJ_ESC2		0x1d	//ESC = number of - to be decompressed
#define CEJ_ESC3		0x1e	//ESC = number of . to be decompressed
#define CEJ_ESC4		0x1f	//ESC = number of = to be decompressed
//---------------------------------------------------------------
typedef struct 				//EJ文档头(CEJ_STARTDOC),大小为EJDOCHEADSIZE
{
    BYTE    EJStart;          //文档开始标志=CEJ_STARTDOC,也是上一个文档结束标志
    ULONG   EJDocNumber;      //文档序号
    BYTE    EJLocation;         //ccr2016-10-29门店号
    BYTE    EJRegister;         //ccr2016-10-29收款机号
    //^^^^以上数据不变^^^^^^^^
    BYTE    EJDocType;        //文档类型('R/Z/X/M/S/O'),不要动其定义位置
    TDATETIME  EJDateTime;     //文档日期时间:0Xyymmddhhmmss,不要随意更改顺序
    WORD    EJClerk;           //ccr2016-10-28收款员序号
} TEJDOCHEAD;

#define EJDOCHEADSIZE	sizeof(TEJDOCHEAD)  //16 每个EJ文档的记录头数据大小,=sizeof(EJDOCHEAD) //

#define EJBUFFSIZE  (PRTLEN+5)

//EJ初始化信息,共98字节,
//当为CASE_EJFLASH时,最大不得超过124字节
//当为CASE_EJSD时,最大不得超过1024字节
typedef struct
{
	WORD 	EJMark;              //EJ初始化标志
	BYTE	EJID[32];           //初始化后,存放税控序列号等信息,也可以用来存放客户名称
	TDATETIME InitDateTime;    //EJ初始化日期时间
	WORD	Release;            //(双字节对齐)发布版本

    //SD卡基本信息
    ULONG   SizeOfSD;           //SD卡的容量,=ApplVar.EJVar.MMCSIZEMAX;
    ULONG   AddrEJFirst;          //第一条EJ记录的地址,也是EJHeader的分配空间,=EJDOCADDRFROM
    ULONG   AddrEJMax;          //EJ记录数据空间的最大地址,=EJDOCADDRMAX
    ULONG   AddrEJIndex;        //EJ数据索引存放位置,=INDEXADDR
    ULONG   SizeEJIndex;        //EJ数据索引索引空间大小
    WORD    MaxEJIndex;         //EJ数据索引记录最大数,=INDEXRECORDCOUNT
    ULONG   AddrEJHeader;       //存放EJHeader数据的地址,=EJHEADERADDR
    WORD    SizeEJHeader;       //EJHeader的大小,实际分配空间为1024字节

    //EJ文档管理数据
    BYTE    EJLocation;         //ccr2016-10-29门店号
    BYTE    EJRegister;         //ccr2016-10-29收款机号
    BYTE    EJType;             //1/2/3分别对应不同的容量,0xef时,为使用flash存储EJ
    BYTE    EJClerk;            //初始化此卡的收款员
    //以上为固定不变信息-----------------
    ULONG   AddrLastEJ;		    //最后一个文档的位置,即最后一个EJ开始文档的存储位置
    ULONG   AddrLast2EJ;		//倒数第二个文档的位置,即最后一个有效的EJ文档
    ULONG   AddrNextEJ;		    //新的EJ数据将要写入的地址
    ULONG   AddrEJRead;         //需要发送的EJ数据开始地址,为相对地址
    WORD    DateLastEJ;         //当前写入EJ文档的日期,用来控制索引数据
    WORD    IndexCount;         //索引记录数
    ULONG   EJDocNumber;		// 最后一个EJ文档的序号
}TEJHEADER;
//ccr2016-11-01 EJ变量定义
typedef struct {
    WORD ContFlag;      //EJ控制标志
    BYTE EJContent; 	//content type of EJ Log
    BYTE EJcrc;         //EJ文档的校验码(受RefreshEJStart的影响,此校验没有意义)
    ULONG MMCSIZEMAX;   //SD卡的容量(最大4G)
    ULONG MMCLast;      //EJ最后一次读写数据的地址
}TEJVAR;

//ccr2016-11-01 EJ索引定义
typedef struct {
    WORD  EJDate;      //EJ日期
    ULONG EJAddress;   //在SD卡中的地址
    ULONG EJNumber;    //EJ文档编号
}TEJINDEX;

#if (defined(DEBUGBYPC))
#if (INDEXRECORDSIZE!=sizeof(TEJINDEX))
#error "INDEXRECORDSIZE error!"
#endif
#endif

//ccr2014-08-07>>>>>>>>>>>>>>>>>>>>>>
/*TEJROLL用来保存最后要写入的EJ LOG*/
typedef struct
{
  BYTE  Flag;       //=0x69,说明EJ数据写入失败
  BYTE  FlagS;      //必须为0x5a,其中的数据才有效
  BYTE  EJContent;
  ULONG Address;    //EJ数据的写入地址
  BYTE  LogLen;     //EJ数据长度
  BYTE  EJcrc;      //EJ数据的校验
  BYTE  EJData[EJBUFFSIZE];//EJ数据
} TEJROLL;
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


extern void WriteEJHead(void);
extern ULONG ReadEJIndexByDate(WORD pDate);
extern ULONG ReadEJIndexByNumber(ULONG pNumber);
extern ULONG WriteEJIndex(WORD pDate,BYTE inc);
extern ULONG RecallEJHead(void);

#if (defined(CASE_EJSD))
extern BYTE MMC_CheckPresence(void);
void EJ_Exist(void);
#else//>>>for CASE_EJFLASH
#define MMC_CheckPresence() true
#define EJ_Exist()  {}
#endif

extern BYTE ReadFromMMC(char *to,ULONG pFrom,WORD bytes);
extern void StoreEJStart(void);
extern void StoreEJData(BYTE cmd, BYTE *str);
extern void StoreLogType(BYTE type, BYTE *str);
extern void PrintEJLog(BYTE prnLast);
extern BYTE Initial_EJ(void);

#define StoreEJEnd() StoreEJData(CEJ_DOCEND,0)  //存储EJ文档结束标志

extern BYTE CheckEJ(void);

extern  BYTE  MMC_Buff[SD_BLOCKSIZE*2];
extern void ExploreEJournal();

BYTE WriteStream(char *from,WORD bytes,BYTE erase);
BYTE WriteEJLog(char *pFrom,WORD pSize);
BYTE ECRMode(void);

BYTE CheckEJError(BYTE disp);
void PrintEJInfo(BYTE oldEJ);
BYTE TestSaveEJ(void);
void RefreshEJStart(BYTE refresh);
void DisplayEJInfo1BY1(void);

//------------ defined for ReadEJInfo--------------
#define READEJINI_T    	    0	//读取初始化标题
#define READEJINIT_D   		(READEJINI_T    +1)	//读取初始化日期
#define READEJEND_T    		(READEJINIT_D   +1)	//读取最后一条EJ的日期标题
#define READEJEND_D    		(READEJEND_T    +1)	//读取最后一条EJ的日期
#define READEJSIZE_T   		(READEJEND_D    +1)	 //读取EJ大小标题
#define READEJSIZE_D   		(READEJSIZE_T   +1)	 //读取EJ大小
#define READEJLEFT_T   		(READEJSIZE_D   +1)	 //读取EJ剩余大小标题
#define READEJLEFT_D   		(READEJLEFT_T   +1)	 //读取EJ剩余大小
#define READEJRECORDS_TD     (READEJLEFT_D +1)       	//读取EJ记录数
#define READEJREGISTER_TD   (READEJRECORDS_TD +1)         //读取EJ收款机号 /* REGISTER number */
#define READEJLOCATION_TD   (READEJREGISTER_TD+1)         //读取EJ商店号 /* LOCATION number */
#define READEJCLERK_T    	(READEJLOCATION_TD+1)      //读取EJ收款员标题/* CLERK */
#define READEJCLERK_D    	(READEJCLERK_T    +1)      //读取EJ收款员
void ReadEJInfo(void);

void DisplayEJInfo(BYTE prnEJLast);
void PrintEJNumber(void);
#endif
#endif


