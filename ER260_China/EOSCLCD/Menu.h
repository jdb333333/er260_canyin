#ifndef MENU_H
#define MENU_H

#if CASE_USBDISK
#define MENUUSBFUNSMAX    (3+26+3) 			//ccr 菜单功能表数目
//.......................
//#define MENUXZREPIDX 	0
#define MENUUSBMAINIDX 	0
#define MENUUSBFILEIDX 	1
#define MENUUSBFLOWIDX 	2

#define MENUUSBITEMSMAX	3			//ccr 系统中开始用的菜单数目//
#else
#define MENUUSBFUNSMAX  0 			//ccr 菜单功能表数目
#define MENUUSBITEMSMAX	0			//ccr 系统中开始用的菜单数目//
#endif
//........................

#if defined(CASE_GPRS)  //ccr2014-11-11 NEWSETUP Step-7>>>>>>>>>>>
#define MENUGPRSSEND 	    MENUUSBITEMSMAX

#define MENUGPRSFUNSMAX     (MENUUSBFUNSMAX+10)//gprsMAINITEMS)			//ccr 菜单功能表数目
#define MENUGPRSITEMSMAX	(MENUUSBITEMSMAX+1)			//ccr 系统中开始用的菜单数目//

#else
#define MENUGPRSFUNSMAX     MENUUSBFUNSMAX 			//ccr 菜单功能表数目
#define MENUGPRSITEMSMAX	MENUUSBITEMSMAX			//ccr 系统中开始用的菜单数目//
#endif //ccr2014-11-11<<<<<<<<<<<<<<<<<<<<<<<<
//.................................
#define MENUFUNSMAX     (MENUGPRSFUNSMAX) 			//ccr 菜单功能表数目
#define MENUITEMSMAX	(MENUGPRSITEMSMAX)			//ccr 系统中开始用的菜单数目//


#if (MENUITEMSMAX)
extern CONST struct SETUPLIST MenuList[MENUITEMSMAX+1];
#endif

extern void	DisplaySetup(BYTE keyno);

extern void ExitFile_USB(void);

extern BYTE MenuMain(int pMenuIDX);

extern void usbExportMenu(void);//Ccr导出收款机文件//
extern void usbInportMenu(void);    //Ccr导入收款机文件 //

extern BYTE   MenuFuncIdx;

extern void TestSendMess(void);//通过GPRS模块发送短信息
extern void GPRSSendECR_LOG(void);
extern void GPRSSendECR_LOG_All(void);
extern void GPRS_SetSendMode(void);

extern void ExitGPRS_SUB(void);

extern void GPRS_DownloadPLU(void)	;      //"下载单品文件"
extern void GPRS_DownloadDEPT(void)	;      //"下载部类文件"
extern void GPRS_DownloadCLERK(void)	;      //"下载收款员"
extern void GPRS_DownloadHEAD(void)	;      //"下载票头"
extern void GPRS_DownloadPTRAIL(void);  	    //"下载票尾"
extern void GPRS_DownloadALL(void);  	    //"下载全部数据"
extern void GPRS_Restart(void);             //复位GPRS

//ccr2016-01-12>>>>>>>>>>>>>>>>>>>>>>>>
//定义文件列表菜单
typedef struct{
    WORD     SetupIDX;    //文件的设置编号 //
    void    (*ReadRecord)();    //读取记录数据的功能函数 //
	char	*Caption;			//名称项目数据
	WORD	*RecordIdx;			//记录序号
	WORD	*RecordMax;			//记录最大数目
    WORD    MsgIDX;             //>0时,使用Msg[]中的信息作为列表项目,同时直接使用RecordMax的值作为最大记录数
    BYTE    (*PorcessRecord)(BYTE y,WORD recNo);    //对应当前记录的处理函数,=null时,无处理函数,函数返回0xff时,按键被处理 //
}    TListDef;

//为那些没有Record定义的文件列表增加此内容
#define SIZENAME    35//(PRTLEN+1)  //应该大于票头和显示的宽度

typedef struct{
	char	Name[SIZENAME];//数据名称名称
	WORD	Number; //数据记录序号
} TListRecord;

//虚拟输入
typedef struct{
    BYTE    vKey;//为按键;=0xff时无虚拟输入
    BYTE    vLen;//虚拟输入内容长度
	char	vInput[SIZENAME];//虚拟输入内容
    char    vDispRecNo;     //=1:Item前面显示记录号;=0:在状态行显示记录号
} TVirtualInput;
//ccr2016-01-12<<<<<<<<<<<<<<<<<<<<<<<<


extern TListRecord ListRecord;//对于无Name的文件,存放其没陈述句

extern CONST TListDef ListDef[];

extern void ListICBlock(void);
extern void ReadKP(void);
extern void ReadHeader(void);
extern void ReadTrail(void);
extern void ReadSlipHeader(void);
extern void ReadGraphic(void);
extern void ReadXZReport(void);
extern void ReadRepUnit(void);
extern void ReadZone(void);
extern void ReadKP(void);
extern void ReadPBTable(void);
extern void ReadMGList(void);
extern void ReadFlagsList(void);

extern WORD ListItems(WORD fileType,BYTE saveSCR,const char *caption,BYTE vKey);
extern WORD ReadItemCaption(WORD fileType,WORD idx,char *caption);
extern WORD GetInputByListItems(void);
extern void VirtualInputKey(BYTE key);
extern void ClearVirtualInput(void);
extern void DeleteHeader(BYTE hIdx);
extern void DeleteTrailer(BYTE hIdx);
extern void DeleteSlipHead(BYTE hIdx);
extern void VirtualInputStr(BYTE key,char *val,BYTE sLen);
extern char* GetActiveItemStr(void);
BYTE NewProgSysFlag(BYTE y,WORD recNo);
#endif
