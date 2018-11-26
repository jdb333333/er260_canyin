//---------------------------------------------------------------------------
#if defined(DEBUGBYPC)
#pragma option -a1
#endif

#ifndef CHNInputH
#define CHNInputH

#define INPUTPOS    3 //输入法下的编码显示位置
#define DISPPOSY    1 //重码汉字显示行
#define DISPPOSX	0 //输入法下的重码汉字显示位置
#define DISPMAX	    10 //重码汉字显示个数

#define CHECKINPUT	0
#define GETINPUT	1
#define OPENINPUT	2
#define CLOSEINPUT	3


//.................................................................

#define ASC_MODE    0   //当前为ASCII输入
#define QUWEI_MODE  1   //当前为区位输入
#define PYBH_MODE   2   //当前为拼音或壁画输入

//defined for ASCIIMode>>>>>>>>>>>>>>>
#define LOASCII     0   //小写ASCII
#define UPASCII     1   //大写ASCII
#define SPASCII     2   //特殊字符输入方式(!@#$%...)

#if EN_EXASCII
#define EXASCII     3   //扩展ASCII字符
#define MAXASCII    3   //ASCII方式最大数目
#else
#define MAXASCII    2   //ASCII方式最大数目
#define EXASCII     (MAXASCII+1)   //扩展ASCII字符
#endif
//<<<<<<<<<<<<<<<<<<<<<<<<<

typedef struct {
    const char *Name;
    BYTE MaxInput;
} MODEREC;

extern BYTE CHNMode;
extern WORD ChnInput(BYTE func);
//---------------------------------------------------------------------------
#endif
