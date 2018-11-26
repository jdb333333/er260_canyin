#if defined(DEBUGBYPC)
#pragma option -a1
#else
#pragma pack(1)
#endif

#ifndef ICCARD_H
#define ICCARD_H

#define OFFSET 0		//offset for store, for 4428 card, must be zero
//------------------------------------------------------------------------------
//typedef char BYTE
//; Arithmetic constants
#define LTS 5 //size of standard totalizators
#define LTL 6 //size of long totalizators
#define LCS 2 //size of standard counters
#define LCL 3 //size of long counters
//------------------------------------------------------------------------------
// Values for CHIP_TOPER
// 0 = sale without delta-value
// 1 = sale without delta-value
// 2 = charge-value of chip-card (advance or draw-payment)
// 3 = discharging of points

// Values for Operation-Code into the operations list
// (note: only the first nibble is valid, while the second nibble is fixed to
//  5, as marker of valid operation-record)
#define CoOp_VS 055H //sale by receipt
#define CoOp_VSLIP 015H //sale by fiscal doc.-slip/bill
#define CoOp_CAR 0D5H //charging
//------------------------------------------------------------------------------
// Definition of chip-card structure :
#define IC_PSWD428 1022 //address of password for 428
//------------------------------------------------------------------------------
//Definition of length of chip-card
#define IC_MAX 1024
//------------------------------------------------------------------------------
// "FACTORY" area: 38 bytes
#define ICR_FACT    (0 +OFFSET)		//start of record
#define IC_ATR      (0 +OFFSET)		//ATR
#define IC_ICM      (6 +OFFSET)		//Id of chip-cards producer
#define IC_ICT      (7 +OFFSET)		//Id of used chip
#define IC_SERIE    (13+OFFSET)		 //serial number of the chip-card
#define IC_PATT     (30+OFFSET)		 //recognition pattern
//------------------------------------------------------------------------------
// "FLAGS" area:  14 bytes
#define ICR_FLAGS   (38+OFFSET)		//start of record
#define IC_AGGINIZ  (38+OFFSET)		//Flag of initialization in execution
#define IC_PFIFO    (39+OFFSET)		//Pointer to next record of fifo
#define IC_NFIFO    (40+OFFSET)		//Number of records that are present into the fifo
#define IC_AGGFIFO 	(41+OFFSET)		//Flag of update of fifo-data in execution
#define IC_AGGVAL   (42+OFFSET)		//Flag of value update in execution
#define IC_DSCAD    (43+OFFSET)		//expiration date (packed, 2 bytes)
#define IC_VERG1    (50+OFFSET)		//Flag : FF = no initialized
#define IC_VERG2    (51+OFFSET)		//Flag : FF = no initialized
//------------------------------------------------------------------------------
// "INIT" area : 28 byte
#define ICR_INIT    (52 +OFFSET)	//start of record
#define IC_TIPO     (52 +OFFSET)	//Type of chip-card (operation)
#define IC_FTIPO    (53 +OFFSET)	//安全级别 chip-card (operation)
#define IC_DATAI    (54 +OFFSET)	//Date of initialization
#define IC_ECR      (57 +OFFSET)	//mark id of ecr
#define IC_ECRSW    (58 +OFFSET)	//Release of ecr firmware
#define IC_ECRMATR  (59 +OFFSET)	//fiscal number of ecr
#define IC_CODPOS   (70 +OFFSET)	//POS code
#define IC_PINH     (74 +OFFSET)	//PIN (low part)
#define IC_AUTOC    (75 +OFFSET)
#define IC_PINL     (77 +OFFSET)	//PIN (high part)
#define IC_CRCINIT 	(79 +OFFSET)	//Checksum of initialization area
//------------------------------------------------------------------------------
// "CUSTOMER" area: 64 bytes
#define ICR_CLI     (80 +OFFSET)	//start of record (1)
#define IC_CCF      (80 +OFFSET)	//fiscal code of customer
#define IC_CCOD     (96 +OFFSET)	//Code of customer
#define IC_CFLAGS   (106+OFFSET)	 //Flags of customer
#define IC_CPERC    (108+OFFSET)	 //discount percent
#define IC_CLIST    (110+OFFSET)	 //Code of automatic prices-list
#define IC_CNOME    (112+OFFSET)	 //Name of customer
#define IC_CRCCLI   (143+OFFSET)	 //Checksum of fixed customer data
//------------------------------------------------------------------------------
// "CURRENT CHARGE-VALUE" area: 32bytes
#define ICR_VALATT  (144+OFFSET)	//start of record
#define IC_NOPER    (144+OFFSET)	//Number of (current) operation
#define IC_VAL      (146+OFFSET)	//charge-value
#define IC_PRGVEN   (151+OFFSET)	//Progressive number of sale
#define IC_PRGCAR   (156+OFFSET)	//Progressive number of chargings
#define IC_PUNTI    (161+OFFSET)	//Points
#define IC_FORE     (164+OFFSET)	//Foregift
//cc 2006-06-29 for CateSir>>>>>>>>>>>>>>>>>>>>>
#define IC_ICTYPE			(170+OFFSET)	//ICTYPE cc 2006-06-05
//cc 2006-06-29 for CateSir>>>>>>>>>>>>>>>>>>>>>
#define IC_CRCVAL   (174+OFFSET)	//Checksum of "current value" record
//------------------------------------------------------------------------------
// "PREVIOUS CHARGE-VALUE " area :32 bytes
#define ICR_VALPRE (176+OFFSET)	 //start of record
// "PASSWORDS" area (for Operator/Supervisor type)
#define ICR_PSW 80 //start of record (1)
//------------------------------------------------------------------------------

#if(CASE_MFRIC==1)
// "FACTORY_FLAGS" area: 16 bytes
#define MFRR_FACT			(0 +OFFSET)		//start of record
#define MFR_SERIE    			(0 +OFFSET)		 //serial number of the chip-card
#define MFR_PATT     			(4 +OFFSET)		 //recognition pattern
#define MFR_AGGINIZ  		(10 +OFFSET)		//Flag of initialization in execution
#define MFR_AGGVAL   		(11 +OFFSET)		//Flag of value update in execution
#define MFR_DSCAD    		(12 +OFFSET)		//expiration date (packed, 2 bytes)
#define MFR_CRCFACT			(15 +OFFSET)		//cc 20070825
//------------------------------------------------------------------------------
// "INIT" area : 16 byte
#define MFRR_INIT    			(16 +OFFSET)	//start of record
#define MFR_TIPO     			(16 +OFFSET)	//Type of chip-card (operation)
#define MFR_FTIPO    			(17 +OFFSET)	//Bit-flags of chip-card (operation)
#define MFR_DATAI    			(18 +OFFSET)	//Date of initialization
#define MFR_ECR      			(21 +OFFSET)	//mark id of ecr
#define MFR_ECRSW    		(22 +OFFSET)	//Release of ecr firmware
#define MFR_CODPOS   		(23 +OFFSET)	//POS code
#define MFR_PINH     			(27 +OFFSET)	//PIN (low part)
#define MFR_AUTOC    		(28 +OFFSET)
#define MFR_PINL     			(30 +OFFSET)	//PIN (high part)
#define MFR_CRCINIT  		(31 +OFFSET)	//Checksum of initialization area
//------------------------------------------------------------------------------
// "CUSTOMER" area: 32 bytes
#define MFRR_CLI     			(32 +OFFSET)	//start of record (1)
#define MFR_CPERC    			(32 +OFFSET)	 	//discount percent
#define MFR_CLIST    			(34 +OFFSET)	 //Code of automatic prices-list
#define MFR_CNOME    		(36 +OFFSET)	 //Name of customer
#define MFR_CRCCLI   			(63 +OFFSET)	 //Checksum of fixed customer data
//------------------------------------------------------------------------------
// "CURRENT CHARGE-VALUE" area: 16 bytes
#define MFRR_VALATT  		(64 +OFFSET)	//start of record
#define MFR_NOPER    		(64 +OFFSET)	//Number of (current) operation
#define MFR_VAL      			(66 +OFFSET)	//charge-value
#define MFR_PRGVEN   		(70 +OFFSET)	//ccr090310 Progressive number of sale
#define MFR_PUNTI    			(74 +OFFSET)	//ccr090310 Points
#define MFR_FORE     			(77 +OFFSET)	//Foregift
#define MFR_ICTYPE			(78 +OFFSET)
#define MFR_CRCVAL   		(79 +OFFSET)	//Checksum of "current value" record
//------------------------------------------------------------------------------
// "FACTORY_FLAGS" area: 16 bytes
#define MFRR_FACTPRE		(80 +OFFSET)		//start of record	//cc 20070825
//------------------------------------------------------------------------------
// "PREVIOUS CHARGE-VALUE " area :16 bytes
#define MFRR_VALPRE 		(96 +OFFSET)	 //start of record 		//cc 20070825
//------------------------------------------------------------------------------
// "FACTORY" area (dinamic record, REC_TESTA=CHIPBUF)
#define MFRCC_SERIE    		(MFR_SERIE-MFRR_FACT)	//serial number of chip-card
#define MFRCC_PATT     		(MFR_PATT-MFRR_FACT)	//recognition pattern "EUTRONHN"
#define MFRCC_AGGINIZ  		(MFR_AGGINIZ-MFRR_FACT)	//initialization in execution
#define MFRCC_AGGVAL   		(MFR_AGGVAL-MFRR_FACT)	//update value
#define MFRCC_DSCAD    		(MFR_DSCAD-MFRR_FACT)	//expiration date (packed)
#define MFRCC_CRCFACT		(MFR_CRCFACT-MFRR_FACT)
//------------------------------------------------------------------------------
// "INIT" area (dinamic REC_INIT=CHIPBUF record)
#define MFRCC_TIPO     		(MFR_TIPO-MFRR_INIT)	//Type of chip-card (operation)
#define MFRCC_FTIPO    		(MFR_FTIPO-MFRR_INIT)	//Bit-flags of chip-card (operation)
#define MFRCC_DATAI    		(MFR_DATAI-MFRR_INIT)	//Date of initialization
#define MFRCC_ECR      		(MFR_ECR-MFRR_INIT)	//mark Id of ecr
#define MFRCC_ECRSW    		(MFR_ECRSW-MFRR_INIT)	//Release of ecr firmware
#define MFRCC_CODPOS   		(MFR_CODPOS-MFRR_INIT )		//POS code
#define MFRCC_PINL     		(MFR_PINL-MFRR_INIT 	)	//PIN (low part)
#define MFRCC_PINH     		(MFR_PINH-MFRR_INIT 	)	//PIN (high part)
#define MFRCC_AUTOC    		(MFR_AUTOC-MFRR_INIT 	)	//#Function of automatic closing
#define MFRCC_CRCINIT  		(MFR_CRCINIT-MFRR_INIT)	 //Checksum of initialization area
//------------------------------------------------------------------------------
// "CUSTOMER" area	(dinamic REC_CLI=CHIPBUF record)
#define MFRCC_CPERC    		(MFR_CPERC-MFRR_CLI 	)	//Automatic discount percent
#define MFRCC_CLIST    		(MFR_CLIST-MFRR_CLI 	)	//Code of prices-list
#define MFRCC_CNOME    		(MFR_CNOME-MFRR_CLI 	)	//Name of customer
#define MFRCC_CRCCLI   		(MFR_CRCCLI-MFRR_CLI 	)	//Checksum of fixed customer data
//------------------------------------------------------------------------------
// "CHARGE-VALUE" area  (static REC_VALATT record)
#define MFRCC_NOPER    		(MFR_NOPER-MFRR_VALATT )	//Number of (current) operation
#define MFRCC_VAL      		(MFR_VAL-MFRR_VALATT 	)	//Charge-value
#define MFRCC_PRGVEN   		(MFR_PRGVEN-MFRR_VALATT) 	//Progressive number of sales
#define MFRCC_PUNTI    		(MFR_PUNTI-MFRR_VALATT )	//Points
#define MFRCC_FORE     		(MFR_FORE-MFRR_VALATT )		//Points
#define MFRCC_ICTYPE		(MFR_ICTYPE-MFRR_VALATT)
#define MFRCC_CRCVAL   		(MFR_CRCVAL-MFRR_VALATT) 	//Checksum of "current value" record
//------------------------------------------------------------------------------
#define SERIELEN				4
#define PATTLEN				6
#define DSCADLEN			2
#define DATAILEN			3
#define CODPOSLEN			4
#define AUTOCLEN			2
#define CPERCLEN			2
#define CLISTLEN				2
//#define CNOMELEN			27	//cc 20070811
#define CNOMELEN			26
#define NOPERLEN			2
#define VALLEN				(MFR_PRGVEN-MFR_VAL)	//ccr090310
#define PRGVENLEN			(MFR_PUNTI-MFR_PRGVEN)	//ccr090310
#define PUNTILEN			(MFR_FORE-MFR_PUNTI)	//ccr090310
#define VALATTLEN			32
#endif
// The CC_* definitions are offsets relative to records
// "FACTORY" area (dinamic record, REC_TESTA=CHIPBUF)
#define CC_ATR      (IC_ATR-ICR_FACT)	//ATR
#define CC_ICM      (IC_ICM-ICR_FACT)	//Id chip-cards producer
#define CC_ICT      (IC_ICT-ICR_FACT)	//Id used chip
#define CC_SERIE    (IC_SERIE-ICR_FACT)	//serial number of chip-card
#define CC_PATT     (IC_PATT-ICR_FACT)	//recognition pattern "EUTRONHN"
#define CC_FACTORY	 38 					//size of factory
//------------------------------------------------------------------------------
// "FLAGS" area (static REC_FLAGS record)
#define CC_AGGINIZ  (IC_AGGINIZ-ICR_FLAGS 	)	//initialization in execution
#define CC_PFIFO    (IC_PFIFO-ICR_FLAGS 	)	//Pointer to next record of fifo
#define CC_NFIFO    (IC_NFIFO-ICR_FLAGS 	)	//Number of records present into the fifo
#define CC_AGGFIFO  (IC_AGGFIFO-ICR_FLAGS 	)	//update of fifo data in execution
#define CC_AGGVAL   (IC_AGGVAL-ICR_FLAGS 	)	//update value
#define CC_DSCAD    (IC_DSCAD-ICR_FLAGS 	)	//expiration date (packed)
#define CC_VERG1    (IC_VERG1-ICR_FLAGS 	)	//Flag : FF = not initialized
#define CC_VERG2    (IC_VERG2-ICR_FLAGS 	)	//Flag : FF = not initialized
//------------------------------------------------------------------------------
// "INIT" area (dinamic REC_INIT=CHIPBUF record)
#define CC_TIPO     (IC_TIPO-ICR_INIT 	)	//Type of chip-card (operation)
#define CC_FTIPO    (IC_FTIPO-ICR_INIT 	)	//安全级别0,1,2 chip-card (operation)
#define CC_DATAI    (IC_DATAI-ICR_INIT 	)	//Date of initialization
#define CC_ECR      (IC_ECR-ICR_INIT 	)	//mark Id of ecr
#define CC_ECRSW    (IC_ECRSW-ICR_INIT 	)	//Release of ecr firmware
#define CC_ECRMATR  (IC_ECRMATR-ICR_INIT)	 	//fiscal number of ecr
#define CC_CODPOS   (IC_CODPOS-ICR_INIT )		//POS code
#define CC_PINL     (IC_PINL-ICR_INIT 	)	//PIN (low part)
#define CC_PINH     (IC_PINH-ICR_INIT 	)	//PIN (high part)
#define CC_AUTOC    (IC_AUTOC-ICR_INIT 	)	//#Function of automatic closing
#define CC_CRCINIT  (IC_CRCINIT-ICR_INIT)	 //Checksum of initialization area
//------------------------------------------------------------------------------
// "CUSTOMER" area	(dinamic REC_CLI=CHIPBUF record)
#define CC_CCF      (IC_CCF-ICR_CLI 	)	//Fiscal code of customer
#define CC_CCOD     (IC_CCOD-ICR_CLI 	)	//Code of customer
#define CC_CFLAGS   (IC_CFLAGS-ICR_CLI 	)	//Flags of customer
#define CC_CPERC    (IC_CPERC-ICR_CLI 	)	//Automatic discount percent
#define CC_CLIST    (IC_CLIST-ICR_CLI 	)	//Code of prices-list
#define CC_CNOME    (IC_CNOME-ICR_CLI 	)	//Name of customer
#define CC_CRCCLI   (IC_CRCCLI-ICR_CLI 	)	//Checksum of fixed customer data
//------------------------------------------------------------------------------
// "CHARGE-VALUE" area  (static REC_VALATT record)
#define CC_NOPER    (IC_NOPER-ICR_VALATT )	//Number of (current) operation
#define CC_VAL      (IC_VAL-ICR_VALATT 	)	//Charge-value
#define CC_PRGVEN   (IC_PRGVEN-ICR_VALATT) 	//Progressive number of sales
#define CC_PRGCAR   (IC_PRGCAR-ICR_VALATT )	//Progressive number of chargings
#define CC_PUNTI    (IC_PUNTI-ICR_VALATT )	//Points
#define CC_FORE     (IC_FORE-ICR_VALATT )	//Points
//cc 2006-06-29 for CateSir>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define CC_ICTYPE			(IC_ICTYPE-ICR_VALATT)	//cc 2006-06-05
//cc 2006-06-29 for CateSir>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define CC_CRCVAL   (IC_CRCVAL-ICR_VALATT) 	//Checksum of "current value" record
//------------------------------------------------------------------------------
//  Definition of chip-cards-types  (NO CHANGE !!!!) :
#define TIPO_CLI 0 //0 = "CUSTOMER", only ID and promotions
#define TIPO_CLIS 1 //1 = "CUSTOMER", with pre-payed
#define TIPO_CLIC 2
#define TIPO_CAT 3 //3 = "CHAIN" (UF)	4..9;//(UF)
#define TIPO_OPE 10 //10 = "OPERATOR" (UF)	11..97;//(UF)
#define TIPO_BAK 98 //98 = "BAKUP" (UF)
#define TIPO_TEST 99 //99 =	Reserved for TEST
//------------------------------------------------------------------------------
// Definition of flags of type of customer chip-card (NO CHANGE !!!) :
#define FLT0_ProtM 0 //bit 0 : 0 = NO-protection,  1=middle protection
#define FLT0_ProtT 1 //bit 1 : 1 = Full-Protected
#define FLT0_Scad 2 //bit 2 : 0 = NO expiration  1 = YES expiration
//------------------------------------------------------------------------------
// Definition of some constants
#define MaxFifo442 2 //Max number of records for 442
#define MaxFifo428 48 //Max number of records for 428

#define StartFifo 208 //fix start address of fifo of chip-card

#define LCC_PUNTI 3 //size of 'POINTS' area

#define StartMacro 976 //fix address of macros area
#define LN_CCMACRO 20 //Max size of automatic macro

//------------------------------------------------------------------------------
#define IC_ILLERR  	-1	//
#define IC_POSERR	-2	//PosCode Error
#define IC_DATAERR 	-3
#define IC_DATEERR 	-4
#define IC_CANERR 	-5
#define IC_CUSTERR 	-6
#define IC_NEWERR  	-7
#define IC_TYPEERR	-8
#define IC_WRITERR	-9
#define IC_NUMERR	-10
#define IC_NULLERR		-11 //无卡

#define IC_TYPE0 	0		//Type0
#define IC_TYPE1 	1		//Type1
#define IC_TYPE2	       2		//Type2
#define IC_TYPE3				3
//IC卡操作类型(OpType).........................................
#define IC_OP_NOVAT         0   //O VAT TRANSLATE
#define IC_OP_CLEAR         1   //Clear Chipcard
#define IC_OP_ADD           2   //ADD
#define IC_OP_SUB           3   //SUB
#define IC_OP_CHARGE        4   //Charge value
#define IC_OP_INIT          5   // Initital chipcard
#define IC_OP_DECHARGE      6   //ccr2015-05-22
#define IC_OP_ADDPOINT      7   //ccr2016-05-13//加点操作
#define IC_OP_DECPOINT      8   //ccr2016-05-13//减点操作


//------------------------------------------------------------------------------
struct ICStruct
{
        // Definition of variables
        BYTE REC_Factory[CC_FACTORY]; //38 bytes
        BYTE REC_Flags[CC_VERG2+1]; //"FLAGS" Area 14Bytes
        BYTE REC_INIT[CC_CRCINIT+1]; //28 bytes
        BYTE REC_Customer[CC_CRCCLI+1]; //64 bytes
        BYTE REC_VALATT[CC_CRCVAL+1]; // 32 bytes  Area of the "current charge-value"
        BYTE REC_VALPRE[CC_CRCVAL+1]; //=REC_VALATT[32..63] Area of the "previous charge-value"
#if(CASE_MFRIC==1)
	BYTE MFR_Factory[MFR_DSCAD+4];
	BYTE MFR_Facpre[MFR_DSCAD+4];
	BYTE MFR_INIT[MFRCC_CRCINIT+1]; //16 bytes
	BYTE MFR_Customer[MFRCC_CRCCLI+1]; //32 bytes
	BYTE MFR_VALATT[MFRCC_CRCVAL+1]; // 16 bytes  Area of the "current charge-value"
	BYTE MFR_VALPRE[MFRCC_CRCVAL+1]; //=REC_VALATT[16..32] Area of the "previous charge-value"
 #endif
        //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        unsigned long  PosCode;//Post code for ICCard
		unsigned long CardNo; //Serio number of ICCard
        BYTE CHIP_PSWD[3]; //auxiliary buffer for password
        BYTE CriptCnt; 	//current counter of area codify/decodify

        BYTE CHIP_TIPO; 	//0,Type of chip-card
//cc 2006-06-29 for CateSir>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 BYTE CHIP_ICTYPE;	//0-A,1-B,2-C cc 2006-06-05
//cc 2006-06-29 for CateSir>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        BYTE CHIP_FTIPO; 	//Flags of the type
        BYTE CHIP_PROT; 	//0,Aux : protection level
        signed char CHIP_Flag;	//
		BYTE ICState;

};
//------------------------------------------------------------------------------
//for ICState
#define IC_FLAG			BIT0		//Confirm flag
#define IC_FLAG1		BIT1		//Confirm flag1
#define IC_INSERT		BIT2		//ChipCard inserted
#define IC_NOTREMOVED	BIT3		//=0,卡拔出过，用来判断是否要重新验证密码 ；＝1，卡没有被拔出 (射频卡不使用此标志)//
#define IC_NEWCARD		BIT4		//为新卡,必须充值才能使用

//#define IC_DISCOUNT BIT5
//#define IC_POINTS   BIT6

//ApplVar.ICCardSet.Options--------------------------------
#define IC_CHIPCARD BIT0
#define IC_TYPE_0   BIT1
#define IC_TYPE_1   BIT2
#define IC_TYPE_2   BIT3
#define IC_REPORT   BIT4
#define IC_DISCOUNT BIT5
#define IC_POINTS   BIT6
#define IC_TOT_CC   BIT7
#define IC_DEAD 	 BIT8			//有效期是否有效

struct  ICCardSetTable
{
    WORD Options;
    WORD ECRVIPCount;//ccr2016-05-13
    BYTE Flags;      //ccr2016-05-13
    UnLong PosCode;
    BYTE Value[3];  //计点金额
    BYTE MiniNum[3];//计点起始金额
};

extern void SCC_SetPswd(short pType);
extern void PrintICMessage(void);

//ccr2016-05-04........ECR会员开发...............................



struct  TECRVIPFix
{//会员固定信息
    WORD  VipDate;//会员创建日期
    BYTE  VipTEL[6];//电话号码(12位)
    BYTE  VipName[8];//姓名
};

struct  TECRVIPVar
{//会员动态信息
    UnLong VipPoints;  //会员积分
    UnLong VipAmt;   //会员消费总额(固定两位小数)
};


#define ECRVIPMAX   2048    //可管理的会员最大数目

#define VIP_LOGIN   BIT0    //会员是否登录
#define VIP_POINTS  BIT1    //登录会员是否已经积分
#define VIP_DEC_ADD BIT2    //减点或加点
#define VIP_CONFIRM BIT3    //确认减点或加点

struct TECRVIP
{
    WORD ECRVIPNumber;  // ECRVIPNumber当前会员逻辑序号,[0..ECRVIPMAX-1)]
    WORD ECRVIPState;   // ECRVIPState操作标志:
                        //      VIP_LOGIN,BI0:表示是否登录了会员
                        //      VIP_POINTS,bit1:表示是否已经计点
    UnLong VipPoints;  //会员积分
    WORD CurrentVIPVar;       //当前保存在VipVar中的数据的Flash地址,=0时,没有保存数据.
    struct TECRVIPFix   VipFix;
    struct TECRVIPVar   VipVar[ECRVIPMAX/2];//记录当前登录的会员,数据掉电保护,因此关机后数据会丢失

};

void ReadECRVIPVar(void);	//根据ECRVIPNumber读取会员数据,实际上是将一个8K的数据从Flash读入到ECRVipVar区
void WriteECRVIPVar(void);	//根据ECRVIPNumber将会员数据ECRVipVar写入Flash,在写入前先擦除对应FLASH区
void InitECRVIP(void);	//初始化ECRVIP,即将ECRVIP的flash空间全部擦除,并将ECRVIPCount
void RecallECRVIP(void);//恢复会员数据计数器
void AddNewECRVIP(void);
void PrintECRVIP(BYTE *vipCode);
void ECRVIPLogin(void);
void DisplayECRVIP(void);
void PointsByECRVIP(BCD *pValue);
void ProgECRVIPPoints(void);
#endif


