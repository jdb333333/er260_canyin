#define AUTOBUFMAX  2048
#define SENDTOHOST	1
#define FASTMODE	0   //快速测试方式
#define DEPT_TEST   1   //测试部类销售
#define PLU_TEST    1   //测试单品销售
//---------------------------------------
#define TEST_END        0
#define TEST_NEXTDAY    1
#define TEST_REPEAT1    2
#define TEST_REPEAT2    (TEST_REPEAT1+1)
#define TEST_REPEAT3    (TEST_REPEAT1+2)
#define TEST_REPEAT4    (TEST_REPEAT1+3)
#define TEST_REPEAT5    (TEST_REPEAT1+4)
#define TEST_REPEAT6    (TEST_REPEAT1+5)
#define TEST_REPEAT7    (TEST_REPEAT1+6)
#define TEST_REPEAT8    (TEST_REPEAT1+7)
#define TEST_REPEAT9    (TEST_REPEAT1+8)
#define TEST_REPEAT10   (TEST_REPEAT1+9)
#define TEST_ZREPORT    TEST_REPEAT10+1
//---------------------------------------

struct ECRTESTATTR {
	BYTE	Going;			// 是否进行测试,=0时,不进行测试 //
	BYTE	LastDay;
	BYTE	Repeat;		// 循环测试一次
	BYTE	PrnOFF;			//
	int     KeyEmuNum;		// 使用ROM中的数据进行测试
    int    TestCount;    //记录测试次数
#if FASTMODE
    int AutoKeyCurrent ;
    int AutoReceiptTag ;

    BYTE AutoKeyBuf[AUTOBUFMAX];
#endif

};


extern struct  ECRTESTATTR  FisTestTask;

void SendABCD(BCD *pBcd);

extern void FisTestProc();
extern void chkAutoReceiptKey();

void SendALong(long pLInt);
void SendAWord(WORD pLInt);


