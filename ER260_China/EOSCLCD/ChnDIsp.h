//---------------------------------------------------------------------------
#if defined(DEBUGBYPC)
#pragma option -a1
#endif

#ifndef CHNDispH
#define CHNDispH

//#define CHAR_6X8  0 //ccr2016-01-18 使用8x16点阵ASCII字符

#if (!defined(CHAR_6X8) || defined(CASE_FORHANZI))
#define SCREENLN	4	//Screen lines
#define SCREENWD	16	//20050105 24	//Screen width
#else
#define SCREENLN	8	//Screen lines
#define SCREENWD	21	//20050105 24	//Screen width
#define SCRDBLWD    11  //ccr2016-03-01倍宽时的屏幕宽度
//#define SCRSTATDBL   1   //ccr2016-03-01状态行倍宽显示
#endif

#define STATELN		1	//Statues lines

#define STATELINE	SCREENLN


//ApplVar.ScreenStat:

//显示状态,各位对应如下:
#define STATEON		BIT0    //STATEON必须为第一位(结果值用于计算)
#define CURSOREN	BIT1    //光标使能标志(是否在屏幕上显示光标)
#define CURSORON	BIT2    //屏幕上显示有光标标志
#define SCREENON	BIT3    //屏幕为显示状态标志
#define LIGHTON		BIT4    //背光打开标志
#define HZINPUTMODE BIT5    //为汉字输入模式
#define HZSELECT    BIT6    //输入了一个汉字
#define INVATTR		BIT7    //反显
#define CRITICAL	BIT8	//互斥控制
#define SCREENCOPY  BIT9    //屏幕内容被重码覆盖
#define DOUBLEWIDTH  BIT10    //双倍宽度，将5x8字符显示为10x8

//ccr2016-03-01 倍宽设置
#define mClearDBLDisp() RESETBIT(ApplVar.ScreenStat, DOUBLEWIDTH)
#define mSetDBLDisp()   SETBIT(ApplVar.ScreenStat, DOUBLEWIDTH)

typedef struct {
    char y;
    char x;
    } CURSOR;
//---------------------------------------------------------------------------
#if defined(DEBUGBYPC)
#define CONST

#define mDrawPower( ctrl,  ColNum, LinNum) {}
#define  mDrawGPRSSignal(Signal,ColNum,LinNum) {}

#pragma option -a1

#else

#define CONST const

void mDrawPower(BYTE ctrl, BYTE ColNum,BYTE LinNum);
void mDrawGPRSSignal(BYTE Signal,uint8_t ColNum,uint8_t LinNum);

#endif

#if defined(CASE_FORHANZI)
#define SIGNAL_X     4  //GPRS信号显示位置
#else
#define SIGNAL_X     3
#endif

#if defined(CASE_GPRS)
#define POWER_X    (SIGNAL_X+1)  //电源强度显示位置
#else
#define POWER_X    (SIGNAL_X)  //电源强度显示位置
#endif


struct EDITSTRUCT {
    short   DispFrom;//当前显示开始位置,即从EditBuff+DispFrom显示内容
    BYTE    DispX;//编辑区域在屏幕上的显示位置
    BYTE    DispY;//编辑区的显示位置
    BYTE    DispW;//内容编辑区显示宽度
    BYTE    EditOffs;//当前编辑位置
    BYTE    EditCount;//已经输入的字符数目
    BYTE    BuffSize;//=sizeof(EditBuff)-1
    BYTE    *EditBuff;//must terminated by NULL(\0)
    char    *Source; //ccr2015-03-11所输入的数据将要存入的存储区
};
//---------------------------------------------------------------------------

extern void SetEditPosition(BYTE x,BYTE y,BYTE len,char *source);
extern void InitLineEditor(short x,short y);
extern BYTE DispCharXY(WORD ch ,BYTE x,BYTE y);
extern void DispStrXY(CONST BYTE *str ,BYTE x,BYTE y);
extern BYTE GotoXY(BYTE x,BYTE y);
extern WORD Getxy(void);
extern void GetStrFr(char *str);
extern WORD GetChFr(BYTE x,BYTE y);
extern void OpenMess(char on);
extern void ClearScreen(void);
extern void ScrollScreen(BYTE sLines);
extern void ClearLine(char LinNum);

extern void ScreenFeed(char LinNum);
extern BYTE LineEditor(union KEY ch);
extern BYTE TestHZRight(BYTE *str,short pos);
extern void DisplayChar(BYTE ch);
extern void	mDispStringXY(BYTE *dbuf,BYTE DISPX,BYTE LN);

extern void DispStr_Right(char *str,BYTE y);

extern void BackupScreen(BYTE savCur);
extern void RecallScreen();
extern void  DispLFStr(char *str);
//---------------------------------------------------------------------------

extern struct EDITSTRUCT Editor;

extern WORD    HANZI;

void DispOnStatDBL(char *str,BYTE x);//以倍宽的形式在状态行显示内容


//---------------------------------------------------------------------------
#define DispSt0(a)	DispStrXY(a,0,STATELINE)			/* Display from 0 */
#define DispSt1(a)  DispStrXY(a,PROMDISPX,STATELINE)	/* Display from 4 */
#define DispSt2(a)  DispStrXY(a,INPUTDISPX,STATELINE)	/* Display from 10 */

#define DispSt0C(a)	{ClsState0();DispStrXY(a,0,STATELINE);}	/* Display from 0 */
#define DispSt1C(a) {ClsState1();DispStrXY(a,PROMDISPX,STATELINE);}	/* Display from 4 */
#define DispSt2C(a) {ClsState2();DispStrXY(a,INPUTDISPX,STATELINE);}	/* Display from 10 */

#define DispStrLF(a) {DispStrXY(a,0,0xff);ScreenFeed(1);}

extern void ClsState0(void);	/* Clear first 4 Bytes 0->3 */
extern void ClsState1(void);  /* Clear first 10 Bytes 4->14 */
extern void ClsState2(void);  /* Clear first 6 Bytes 10->15*/
extern void ClsState12(void);
extern void ClsArea(BYTE x,BYTE y,BYTE l);
extern void SuspendScreen(void);

#endif

