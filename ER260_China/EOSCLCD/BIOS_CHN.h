
//---------------------------------------------------------------------------
#ifndef BIOS_CHNH
#define BIOS_CHNH
//---------------------------------------------------------------------------
#define LCDINITIAL	0
#define LCDDISPCH	1
#define LCDREFRESHL	2
#define	LCDCLEARSCR	3
#define LCDDISPCURS	4
#define LCDENCURSOR	5
//#define LCDLIGHT		6
#define LCDLIGHTON	6
#define LCDLIGHTOFF	7
//#define LCDDISPBLOCK	8
#define LCDCONTRAST		8
//---------------------------------------------------------------------------
#if defined(DEBUGBYPC)
extern void DisplayLcd(short lcdLine);
extern void mClearScreen();
extern void mBalnkScreen(void);
#define mInitialLCD() 		{memset(ApplVar.ScreenMap,' ',sizeof(ApplVar.ScreenMap));DisplayLcd(0);DisplayLcd(1);DisplayLcd(2);DisplayLcd(3);}
#define mDispCharXY(ch,x,y) {} //DispCharXY(ch , x, y)

#define mEnCursor(on)     {}

#define mDispCursor()		{}
#define mRefreshLine(LinNum) DisplayLcd(LinNum)
#define mSetState(on_off)    {(on_off==0)?RESETBIT(ApplVar.ScreenStat,STATEON):SETBIT(ApplVar.ScreenStat,STATEON);}

#define mLightLCDOn()		{}
#define mLightLCDOff()		{}

#define mSetContrast(a)		{}
#define mContrastDown()	{}

#else

void mInitialLCD(void);//初始化汉显模块
void mClearScreen(void);//清除屏幕内容
void mDispCharXY(WORD ch,BYTE x,BYTE y) ;//在第y(0~3)行,第x(0~15)列位置显示字符ch,ch可以是ASCII或者汉字
void mEnCursor(BYTE on)       ;//允许显示光标
void mDispCursor(void) ;//显示光标
void mRefreshLine(BYTE LinNum);//刷新屏幕上指定的行LinNum(0~3),即把屏幕上的第LinNum行重新显示一遍
void mLightLCDOn(void) ;//打开显示背光
void mLightLCDOff(void) ;//关闭显示背光
void mSetContrast(uint8_t contrast);//设置显示亮度

#endif

#define mClearInvAttr()		RESETBIT(ApplVar.ScreenStat, INVATTR)//清除反显属性
#define mSetInvAttr()		SETBIT(ApplVar.ScreenStat, INVATTR)//设置反显属性,此后的所有显示将以反显显示
//#define mScreenOn(char sta) {};
#endif
