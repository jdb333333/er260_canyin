/*********************************************************************************
 菜单显示和操作处理程序
  程序编写:陈初荣
  编写日期:2004年2月至7月
  发布日期:2004年10月
  修改日期:2013年6月
*********************************************************************************/
#define MENU_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "Message.h"

#if (!defined(DEBUGBYPC))
//#include "USBDriver.h"
#endif
//============================================================

BYTE   MenuFuncIdx;//用来记录所选择的菜单功能号

#if (MENUITEMSMAX && MENUFUNSMAX)
/****************************************************************
    当要增加新菜单时,需要进行以下编程:
    1.在 Msg[]中增加菜单所对应菜单项的信息表
    2.在CHNMess.h中增加菜单项在Msg[]中索引序号
    3.在MenuTaxTab[]中添加菜单管理信息
    4.在 MenuTaxTab[]中增加菜单项所对应的功能函数
****************************************************************/
CONST KbdTabStruct  MenuTaxTab[MENUFUNSMAX]=
{//菜单功能表:
#if (CASE_USBDISK)//SETUP
/*   菜单功能号    按确认时执行功能     按退出时执行的功能        */
    {usbDAOCHU     ,(*usbExportMenu),   (0)},                   //Ccr导出收款机文件//
    {usbDAORU      ,(*usbInportMenu),   (0)},                   //Ccr导入收款机文件//
    //以下菜单功能没有指定确认函数,自动进入MenuList[]指定的下一级菜单
    {usbFLOW       ,(0),                (*ExitFile_USB)},       //Ccr导出销售流水//

    {usbALL        ,(*usbALLTheFile),    (*ExitFile_USB)},       //Ccr全部文件//
    {usbBULEI      ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"部类",
    {usbDANPIN     ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"单品",
    {usbSHUILV     ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"税率",
    {usbPIAOTOU    ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"票头",
    {usbZHEKOU     ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"折扣",
#if (salNumber)
    {usbYYYUAN     ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"营业员",
#endif
    {usbSTSHZHI    ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"系统设置",
    {usbTXKOU1     ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"通讯口1",
    {usbTUXIN      ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"图形打印",
    {usbGUIZU      ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"柜组",
    {usbICKA       ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"IC卡",
    {usbZHJIANG    ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"中奖设置",
    {usbCANYINGN   ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"餐饮功能",
    {usbSHKYUAN    ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"收款员",
    {usbSHUOMIN    ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"说明文字",
    {usbZHUOTAI    ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"桌台",
    {usbSHIDUANG   ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"时段",
    {usbJIANPAN    ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"键盘",
    {usbTSHCHJIA   ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"特殊出价",
    {usbCHJRJIN    ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"出金入金",
    {usbPIAOWEI    ,(*usbALLTheFile),   (*ExitFile_USB)},       //Ccr"票尾",
    {usbPIAOJTOU   ,(*usbALLTheFile),   (*ExitFile_USB)},       //Lyq"票据票头",
    {usbICCAGUASHI ,(*usbALLTheFile),   (*ExitFile_USB)},       //ccr"IC卡挂失",
    //以下菜单功能没有指定退出函数,自动回到MenuList[]指定的上一级菜单
    {usbFLOWNEW    ,(*usbExportNew),   (0)},                    // 导出新数据,
    {usbFLOWALL    ,(*usbExportALL),   (0)},                    // 导出全部数据
    {usbFLOWALLDEL ,(*usbExportDel),   (0)},                    // 导出全部数据后清空
#endif
#if defined(CASE_GPRS)//ccr2014-11-11 NEWSETUP Step-5
    {gprsSETMODE   ,   (*GPRS_SetSendMode),   (0)},                 //Ccr设置发送方式//
    {gprsSendECRLogAll,(*GPRSSendECR_LOG_All),   (0)},                  //Ccr发送全部流水//

    {gprsDownloadPLU,  (*GPRS_DownloadPLU),   (0)},         //"下载单品文件"
    {gprsDownloadDEPT, (*GPRS_DownloadDEPT),   (0)},            //"下载部类文件"
    {gprsDownloadCLERK,(*GPRS_DownloadCLERK),   (0)},           //"下载收款员"
    {gprsDownloadHEAD, (*GPRS_DownloadHEAD),   (0)},            //"下载票头"
    {gprsDownloadTRAIL,(*GPRS_DownloadPTRAIL),   (0)},          //"下载票尾"
    {gprsDownloadALL,   (*GPRS_DownloadALL),   (0)},          //"下载全部数据"
    {gprsRestart,      (*GPRS_Restart),   (0)},          //"复位GPRS"    //gprsRestart //ccr2016-08-26
    {gprsSENDMESS  ,   (*TestSendMess),   (0)},                    //Ccr发送短消息//

#endif

};


// MenuList为菜单描述表,每行对应一个菜单列表:MsgIdx=菜单的第一个信息,usbMAINITEMS为菜单项目数
// 每个菜单项目对应的处理功能函数在MenuTaxTab中定义:
//    当确认菜单项时,执行MenuTaxTab.EnterOperate
//    当取消菜单项时,执行MenuTaxTab.ExitOperate
// MenuList.*Var指示当前选择的菜单序号;*Var>=Rec1st && *Var<=RecMax
// 当按确认键或退出键时，MenuMain将根据MenuList中的MsgIdx值，从MenuTaxTab中找对应的功能函数执行
// MainMenu=0xff时,无上级菜单,否则按退出时,回到上一级菜单
CONST struct SETUPLIST MenuList[MENUITEMSMAX+1]={
//	    MsgIdx;   Cols;Rows;     RecMax;    Rec1st;  *Var;      SubNo;SubMax;isSub;	MainVar;MainMenu;       SubMenu;
#if (CASE_USBDISK)//SETUP
    {usbDAOCHU,   1,SCREENLN-1,usbMAINITEMS,0,      &MenuFuncIdx,0,     0,      0,      0,      0xff,           MENUUSBFLOWIDX},    //ccr MENUUSBMAINIDX //
    {usbALL,      1,SCREENLN-1,usbFILEITEMS,0,      &MenuFuncIdx,0,     0,      0,      0,      0xff,           0xff},              //ccr MENUUSBFILEIDX //
    {usbFLOWNEW,  1,SCREENLN-1,usbFLOWITEMS,0,      &MenuFuncIdx,0,     0,      1,      3,      MENUUSBMAINIDX, 0xff},              //ccr MENUUSBFLOWIDX //
#endif

#if defined(CASE_GPRS)//ccr2014-11-11 NEWSETUP Step-5
    {gprsSETMODE,1,SCREENLN-1,gprsMAINITEMS,0,     &MenuFuncIdx,0,     0,      0,      0,      0xff,           0xff},  //ccr MENUGPRSSEND //
#endif
    {SETSYSFLAG,  1,SCREENLN-1,MENUSYSMAX,    0,    &Appl_ProgType,0,     0,      0,      0,      0xff,      0xff},     //ccr MENUSETUPIDX //
//		{XZTITLE,	  1,SCREENLN-1,XZNUM,       0,      &ApplVar.ReportNumber,0,    0,     0,0,      0xff,      0xff},	//ccr MENUXZREPIDX //
};


/******************************************************************
菜单处理程序，pMenuIDX＝MenuList中的一个记录
进入MenuMain后,将不会处理
*******************************************************************/
BYTE MenuMain(int pMenuIDX)
{
    BYTE    NumricInput;// 输入的数字键，为0时，没有输入， 为‘0’。。‘9’时为第一个数字。当输入了第二个数子时，与其组合生成菜单序号 //

    unsigned int    sMsgID,j;
    void    (*ExitFuncPtr)();    //ccr 按键功能指针*///
    void    (*EnterFuncPtr)();    //ccr 按键功能指针*///
    BYTE i,keycode;

    keycode=0;


    SetList = MenuList[pMenuIDX];
    *SetList.Var = 1;
    DisplaySetup(0xff);
    NumricInput = 0;

//	if (ApplVar.FTrain)	  //ccr080728
//		DispSt1C(Msg[PEIXUN].str);


    SetLoop();

    while (1)//keycode<190) ccr080319
    {
        if (!KbHit())
        {

#if !defined(DEBUGBYPC)
            if (!ApplVar.ErrorNumber)
                CheckTime(true);
#endif
#if defined(CASE_GPRS)
            if (pMenuIDX==gprsSENDMESS)
                  GPRSWaitForReady(false);
#endif

            CheckError(0);
            continue;
        }
        keycode=Getch();InActive = 0;

        if (keycode<128)
        {
            i = GetFirmkeyID(keycode);
        }
#if defined(DD_LOCK)
#ifdef TR
        else if (keycode==SET || keycode==RG || keycode==TR || keycode==MG || keycode==X || keycode==Z || keycode==OFF)// ccr080403>>>>
#else
        else if (keycode==SET || keycode==RG || keycode==MG || keycode==X || keycode==Z || keycode==OFF)// ccr080403>>>>
#endif

        {
            KeyFrHost = keycode;
            i = ID_CANCEL;
        }
#endif
        else
            continue;//<<<<<<<<<<

        if (i<FIRMKEYS)
        {
            switch (i)
            {
            case ID_CLEAR:
                ApplVar.ErrorNumber=0;
                ClsState2();
                break;
            case ID_RJFEED:
                RFeed(1);
                break;
            case ID_ENTER:
                if (ApplVar.ErrorNumber)
                    break;
            case ID_CANCEL:
                sMsgID = MenuFuncIdx-1+SetList.MsgIdx;
                for (j=0;j<MENUFUNSMAX;j++)
                    if (sMsgID==MenuTaxTab[j].MsgIndex)
                        break;
                if (j<MENUFUNSMAX)
                {
                    ExitFuncPtr = MenuTaxTab[j].ExitOperate;
                    EnterFuncPtr = MenuTaxTab[j].EnterOperate;
                } else
                {
                    ExitFuncPtr = 0;
                    EnterFuncPtr = 0;
                }
                if (i==ID_CANCEL)
                {
                    ApplVar.ErrorNumber=0;
                    ClsState2();
                    if (ExitFuncPtr)
                    {
                        (*ExitFuncPtr)();//ccr 执行退出操作*///
                        NumricInput = 0;
                    } else
                    {// 无退出处理对象时，直接返回到上一级调用函数 //
                        if (SetList.MainMenu<MENUITEMSMAX)
                        {// 返回到上一级菜单 //
                            SetList = MenuList[SetList.MainMenu];
                            *SetList.Var = 1;
                            DisplaySetup(0xff);
                            NumricInput = 0;
                        } else
                        {//无上一级菜单(SetList.MainMenu=0xff),退出
                            ResetLoop();
                            mClearScreen();
                            return 0;
                        }
                    }
                } else
                {
                    mClearScreen();
                    MemSet(SysBuf,6,' ');
                    SysBuf[6] = 0;
                    DispStrXY(SysBuf,SCREENWD-6, SCREENLN-1);

                    if (EnterFuncPtr)
                        (*EnterFuncPtr)();//ccr 执行确认的操作*///
                    else if (SetList.SubMenu<MENUITEMSMAX)
                    {//进入下一级菜单
                        SetList = MenuList[SetList.SubMenu];
                        *SetList.Var = 1;
                    }
                    NumricInput = 0;
                    DisplaySetup(0xff);
                }
                break;
            case ID_UP:
            case ID_DOWN:
            case ID_LEFT:
            case ID_RIGHT:
            case ID_SELECT:
                if (ApplVar.ErrorNumber)
                    break;
                DisplaySetup(i);
                break;
            default:
                Bell(1);
                break;
            }
        } else
        {
            keycode = ApplVar.AP.KeyTable[keycode];//			i = GetNumric(keycode);
            if (keycode>='0' && keycode <='9')
            {
                switch (NumricInput)
                {
                default://与上一次输入的数字组合
                    i = NumricInput * 10 + keycode-'0';
                    if (i<=SetList.RecMax)
                    {
                        keycode=i+'0';
                        NumricInput = 0;
                        break;
                    }
                case 0:
                    NumricInput = keycode-'0';
                    break;
                }

                DisplaySetup(keycode);
            } else
                Bell(1);
        }
    }

    ResetLoop();
    return 0;

}

//ccr /******************************************************************
//ccr 显示报表二级菜单//
//ccr *******************************************************************/

    #if (CASE_USBDISK)

//ccr /*退出报表菜单*///
void ExitFile_USB()
{
    SetList = MenuList[MENUUSBMAINIDX];
    *SetList.Var = 1;
    DisplaySetup(0xff);
}


////////////////////////////////////////////////////////////////
void usbExportMenu()//Ccr导出收款机文件//
{
    SetList = MenuList[MENUUSBFILEIDX];
    *SetList.Var = 1;
    Backup_Restore= BACKUPECR;
}
//////////////////////////////////////////////
void usbInportMenu()    //Ccr导入收款机文件 //
{
    SetList = MenuList[MENUUSBFILEIDX];
    *SetList.Var = 1;
    Backup_Restore = RESTOREECR;
}

    #endif

    #if defined(CASE_GPRS)
//ccr /*退出报表菜单*///
void ExitGPRS_SUB()
{
    SetList = MenuList[MENUITEMSMAX];
    *SetList.Var = 1;
    DisplaySetup(0xff);
}
    #endif
#else
BYTE MenuMain(int pMenuIDX)
{
    ResetLoop();
    mClearScreen();
    return 0;
}
#endif

//>>>>>>>>>>>>>>>ccr2016-01-11>>>新增加的功能>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/**
 * 输入和验证密码
 *
 * @author EutronSoftware (2016-01-11)
 *
 * @param pass :为需要验证的密码
 *
 * @return char :返回true-验证成功,false-验证失败
 */
char CheckPassWord(char *pass)
{

}
//.......................................................................

extern void ReadKP();
void ReadFromMsg();
//......................................................................
TListRecord ListRecord;//对于无Name的文件,存放其没陈述句


TVirtualInput VirtualInput;

CONST WORD  MaxNum8=8,//厨房打印,收据票头
            MaxNum6=6,//收据票尾,票据票头
            MaxNumFL=FUNCLOOK_KEYS,
            MaxNumGR=GRAFESTMAX,
            MaxNumMG=MGFUNCS,//=7
            MaxNumXZ=XZNUM,
            MaxNumFlags=SYSUSED,
            MaxKeyDes=KEYDESMAX,
            MaxNum10=10,//十个数字
            MaxRepType=15,//报表单元
            MaxNumKM=KEYMACROMAX;
#define EJInfoMAX   14
//...............................................................................
int ListID;       //指向ListDef中的当前记录

CONST TListDef ListDef[]={
//对于不存在于此表中的项目,表示无记录概念,直接进入设置界面
//***********************************************************************
    { SETGROUP  ,(*ReadGroup),  (char*)ApplVar.Group.Name, (WORD*)&ApplVar.GroupNumber,  (WORD*)&ApplVar.AP.Group.Number,0,0},   //柜组 //ccr2014-11-11 NEWSETUP Step-3<<<<<<
    { SETDEPT   ,(*ReadDept),   (char*)ApplVar.Dept.Name,  (WORD*)&ApplVar.DeptNumber,   (WORD*)&ApplVar.AP.Dept.Number,0,0},    //部类
    { SETPLU    ,(*ReadPlu),    (char*)ApplVar.Plu.Name,   (WORD*)&ApplVar.PluNumber,    (WORD*)&ApplVar.AP.Plu.RNumber,0,0},     //单品

/* MENUCOMMAX:通讯与外设菜单*/
//{ SETCOMPUTER  ,(*),ApplVar..Name,&number,&Max,0,0},	//通讯口1
//{ SETBARCODE  ,(*),ApplVar..Name,&number,&Max,0,0},//通讯口2
//{ SETBALANCE  ,(*),ApplVar..Name,&number,&Max,0,0},//通讯口3
//{ SETPORT4  ,(*),ApplVar..Name,&number,&Max,0,0},//通讯口4
//{ SETIP		,(*),ApplVar..Name,&number,&Max,0,0},//IP地址
//{ SETSP     ,(*),ApplVar..Name,&number,&Max,0,0},		//票据打印
    { SETKP     ,(*ReadKP),     (char*)ListRecord.Name,     (WORD*)&ListRecord.Number,      (WORD*)&MaxNum8,0,0},//厨房打印

/* MENUMANMAX:销售功能设置*/
    { SETOFF    ,(*ReadOFFPrice),(char*)ApplVar.OFFPrice.Name,(WORD*)&ApplVar.OFFNumber,    (WORD*)&ApplVar.AP.OFFPrice.Number,0,0},//优惠价
//{ SETPROM   ,(*),ApplVar..Name,&number,&Max,0,0},//中奖设置
    { SETDISC   ,(*ReadDisc),   (char*)ApplVar.Disc.Name,   (WORD*)&ApplVar.DiscNumber,     (WORD*)&ApplVar.AP.Disc.Number,0,0},          //***BYTE //折扣
    { SETPORA   ,(*ReadPoRa),   (char*)ApplVar.PoRa.Name,   (WORD*)&ApplVar.PoRaNumber,     (WORD*)&ApplVar.AP.PoRa.Number,0,0},          //***BYTE //出金入金
    { SETCORR   ,(*ReadCorrec), (char*)ApplVar.Correc.Name, (WORD*)&ApplVar.CorrecNumber,   (WORD*)&ApplVar.AP.Correc.Number,0,0},  //***BYTE //更正功能
    { SETTEND   ,(*ReadTender), (char*)ApplVar.Tend.Name,   (WORD*)&ApplVar.TendNumber,     (WORD*)&ApplVar.AP.Tend.Number,0,0},            //***BYTE //付款方式
    { SETCURR   ,(*ReadCurr),   (char*)ApplVar.Curr.Name,   (WORD*)&ApplVar.CurrNumber,     (WORD*)&ApplVar.AP.Curr.Number,0,0},         //***BYTE //外币
    { SETDRAWER ,(*ReadDrawer), (char*)ApplVar.Draw.Name,   (WORD*)&ApplVar.DrawNumber ,    (WORD*)&ApplVar.AP.Draw.Number,0,0},           //***BYTE //钱箱
#if !defined(CASE_FORHANZI)
    { SETTAX    ,(*ReadTax),    (char*)ApplVar.Tax.Name,    (WORD*)&ApplVar.TaxNumber,      (WORD*)&ApplVar.AP.Tax.Number,0,0},  //***BYTE//税率
#endif
/* MENUICCMAX:IC卡管理*/
//{ SETVIPIC     ,(*),ApplVar..Name,&number,&Max,0,0},//IC卡设置
    { SETBLOCKIC,(*ListICBlock),(char*)ListRecord.Name,     (WORD*)&ApplVar.ICBlockNumber,  (WORD*)&ApplVar.AP.ICBlock.Number,0,0},//IC卡挂失
//{ SETPRINTIC,(*),ApplVar..Name,&number,&Max,0,0},	//打印卡信息

/* MENUTICMAX:票头票尾*/
    { SETHEAD   ,(*ReadHeader), (char*)ListRecord.Name,     (WORD*)&ListRecord.Number,      (WORD*)&MaxNum8,0,0},    //收据票头
    { SETTRAIL  ,(*ReadTrail),  (char*)ListRecord.Name,     (WORD*)&ListRecord.Number,      (WORD*)&MaxNum6,0,0},//收据票尾
    { SETSHEAD  ,(*ReadSlipHeader),(char*)ListRecord.Name,  (WORD*)&ListRecord.Number,      (WORD*)&MaxNum6,0,0},    //票据票头
    { SETGRAP   ,(*ReadGraphic),(char*)ListRecord.Name,     (WORD*)&ListRecord.Number,      (WORD*)&MaxNumGR,0,0},  //票尾图片
//{ SETPRTGRAP,(*),ApplVar..Name,&number,&Max,0,0},		//打印图片

/* MENUREPMAX:报表功能*/
    { SETREPORT ,(*ReadXZReport),(char*)ListRecord.Name,    (WORD*)&ListRecord.Number,      (WORD*)&MaxNumXZ,0,0},   //设置报表内容
    { SETREPTYPE,(*ReadRepUnit),(char*)ListRecord.Name,     (WORD*)&ListRecord.Number,      (WORD*)&MaxRepType,0,0},  //报表单元
    { SETZONES  ,(*ReadZone),   (char*)ListRecord.Name,     (WORD*)&ListRecord.Number,      (WORD*)&ApplVar.AP.Zone.Number,0,0},    //***BYTE  //报表时段设置
//{ SETCLRREP ,(*),ApplVar..Name,&number,&Max,0,0},	//总清报表
//{ SETPRTJOUR,(*),ApplVar..Name,&number,&Max,0,0},	//打印流水

/* MENUTBLMAX:餐饮功能*/
    { SETPBINF  ,(*ReadPBTable),(char*)ApplVar.PB.Text,     (WORD*)&ApplVar.PbNumber,       (WORD*)&ApplVar.AP.Pb.NumberOfPb,0,0},   //桌台
    { SETPBF    ,(*ReadPbF),    (char*)ApplVar.PbF.Name,    (WORD*)&ApplVar.PbFNumber,      (WORD*)&ApplVar.AP.Pb.Number,0,0},   //***BYTE //餐饮功能
    { SETMODIF  ,(*ReadModi),   (char*)ApplVar.Modi.Name,   (WORD*)&ApplVar.ModiNumber ,    (WORD*)&ApplVar.AP.Modi.Number,0,0},           //菜单说明
#if (agreeNumber)
    { SETAGREE  ,(*ReadAgree),  (char*)ApplVar.Agree.Name,  (WORD*)&ApplVar.AgreeNumber ,   (WORD*)&ApplVar.AP.Agree.Number,0,0},//签单
#endif

/* MENUPWDMAX:权限设置*/
    { SETCLERK  ,(*ReadClerk),  (char*)ApplVar.Clerk.Name,  (WORD*)&ApplVar.ClerkNumber ,   (WORD*)&ApplVar.AP.Clerk.Number,0,0},   //(ApplVar.ClerkNumber>=1)收款员
#if (salNumber)
    { SETSALER  ,(*ReadSalPer), (char*)ApplVar.SalPer.Name, (WORD*)&ApplVar.SalPerNumber ,  (WORD*)&ApplVar.AP.SalPer.Number,0,0},  //(ApplVar.SalPerNumber>=1)营业员
#endif
    { SETSYSFLAG,(*ReadFlagsList),(char*)ListRecord.Name,   (WORD*)&ListRecord.Number,      (WORD*)&MaxNumFlags,0,(*NewProgSysFlag)},  //系统设置
    { SETPOINTTYPE,(*ReadFromMsg),(char*)ListRecord.Name,   (WORD*)&ListRecord.Number,      (WORD*)MsgPOINTMAX,MsgPOINTTYPE1,0},  //小数格式选择
    { LISTMGFUNC,(*ReadFromMsg), (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,      (WORD*)MsgMGMMAX,MsgMGADDINV,0},  //经理挡下功能菜单
    { LISTKEYDES,(*ReadKeyDesList),(char*)ListRecord.Name,  (WORD*)&ListRecord.Number,      (WORD*)&MaxKeyDes,0,0},  //键盘功能码列表
    { SETTEST,   (*ReadFromMsg), (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,      (WORD*)MENUTSTMAX,SETTSTDISP,0},  //系统测试功能
    { LISTNUMRIC,(*ReadNumric),  (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,      (WORD*)&MaxNum10,0,0},  //十个数字
/* 销售日志查询打印 */
    { MsgALLLOG,  (*ReadFromMsg), (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,      (WORD*)MsgPRNLOGMAX,MsgALLLOG,0},  //打印销售记录
    { MsgALLCASH,(*ReadFromMsg), (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,      (WORD*)2,MsgALLCASH,0},  //选择付款方式
    { MsgALLDEPT,(*ReadFromMsg), (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,      (WORD*)2,MsgALLDEPT,0},  //选择部类
    { MsgALLPLU, (*ReadFromMsg), (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,      (WORD*)2,MsgALLPLU,0},  //选择单品
    { SETREST,   (*ReadFromMsg), (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,      (WORD*)KEYTYPEMAX,SETREST,0},  //键盘模式选择
    { SETKEYMACRO,(*ReadKeyMacro),(char*)ListRecord.Name,    (WORD*)&ListRecord.Number,     (WORD*)&MaxNumKM,0,0},  //键盘宏定义
/* 选择功能表中的功能 */
    { FUNCLOOK1, (*ReadFuncLook),  (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,    (WORD*)&MaxNumFL,0,0},  //功能表
/* EJ查询打印 */
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    { MsgEJMESSAGES, (*ReadFromMsg), (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,  (WORD*)MsgPRNEJMAX,MsgEJMESSAGES,0},  //打印EJ记录
    { MENUXEJOURNAL, (*ReadEJInfo), (char*)ListRecord.Name,    (WORD*)&ListRecord.Number,   (WORD*)EJInfoMAX,MENUXEJOURNAL,0},  //打印EJ记录
#endif
    { MsgMGPRINTSTOCK,(*ReadPluStock), (char*)ListRecord.Name,   (WORD*)&ApplVar.PluNumber,    (WORD*)&ApplVar.AP.Plu.RNumber,0,0},     //调阅单品库存

    { 0xffff  ,0,0,0,0,0,0},        //结束
};
//****************************************************************************************
/**
 * 设置要读取的记录号
 *
 * @author EutronSoftware (2016-01-14)
 *
 * @param idx :记录号指针
 * @param val :记录号
 * @param b_w :=true为BYTE类型,=false为WORD类型
 */
void SetRecIndex(WORD *idx,WORD val,BYTE b_w)
{
    if (b_w)
        *(BYTE*)idx=val;
    else
        *idx=val;
}

void IncRecIndex(WORD *idx,BYTE b_w)
{
    if (b_w)
        (*(BYTE*)idx)++;
    else
        (*idx)++;
}
void DecRecIndex(WORD *idx,BYTE b_w)
{
    if (b_w)
        (*(BYTE*)idx)--;
    else
        (*idx)--;
}

WORD ReadRecIndex(WORD *idx,BYTE b_w)
{
    if (b_w)
        return (*(BYTE*)idx);
    else
        return (*idx);
}

void ClearVirtualInput()
{
    VirtualInput.vLen=0;
    VirtualInput.vKey=0xff;
}
/**
 * 设置虚拟按键
 *
 * @author EutronSoftware (2016-01-28)
 *
 * @param key :=0xff,清除清除虚拟输入
 */
void VirtualInputKey(BYTE key)
{
    VirtualInput.vLen = 0;
    VirtualInput.vKey=key;
}
/**
 *
 *
 * @author EutronSoftware (2016-01-28)
 *
 * @param key :=0xff,清除清除虚拟输入
 * @param val :=0xffff时,无数据输入
 */
void VirtualInputWord(BYTE key,WORD val)
{
    if (val!=0xffff)
        VirtualInput.vLen=WORDtoASCL(VirtualInput.vInput,val);
    else
        VirtualInput.vLen = 0;
    VirtualInput.vKey=key;
}
/**
 *
 *
 * @author EutronSoftware (2016-01-28)
 *
 * @param key :=0xff,清除清除虚拟输入
 * @param val :虚拟输入的数据
 * @param sLen :虚拟输入的数据长度.
 */
void VirtualInputStr(BYTE key,char *val,BYTE sLen)
{
    if (val && sLen)
    {
        if (sLen>sizeof(VirtualInput.vInput))
            sLen=sizeof(VirtualInput.vInput);
        memcpy(VirtualInput.vInput,val,sLen);
        VirtualInput.vLen=sLen;
    }
    else
        VirtualInput.vLen=0;
    VirtualInput.vKey=key;
}

/**
 *
 *
 * @author EutronSoftware (2016-01-28)
 *
 * @return BYTE:返回虚拟按键,=0xff时,无虚拟按键
 */
BYTE CheckVirtualKey()
{
    return VirtualInput.vKey;
}

/**
 * 读取由ListItems设置的选择数据
 *
 * @author EutronSoftware (2016-01-14)
 *
 * @return WORD :=0,无数据;否则为(键号+1),同时设置好了输入数据
 */
WORD GetInputByListItems()
{
    if (VirtualInput.vKey!=0xff)
    {
        if (VirtualInput.vLen)
        {
            ClearEntry(0);
            Appl_EntryCounter = VirtualInput.vLen;
            memcpy(EntryBuffer + ENTRYSIZE - Appl_EntryCounter - 1,VirtualInput.vInput, Appl_EntryCounter);
        }
        KeyFrHost=VirtualInput.vKey;
        VirtualInput.vKey=0xff;
        VirtualInput.vLen=0;
        return (KeyFrHost+1);
    }
    else
        return 0;
}
/**
 * 读取所选择的条目名称串
 *
 * @author EutronSoftware (2016-02-15)
 *
 * @return char*
 */
char* GetActiveItemStr()
{
    return VirtualInput.vInput;
}
/**
 *
 *
 * @author EutronSoftware (2016-01-26)
 *
 * @param x :序号显示位置
 * @param y
 * @param val :序号
 *
 * @return int :显示的记录序号位数
 */
int DispIndex(BYTE x,BYTE y,WORD val)
{
    WORD temp;
    char decBuff[7];

    if (VirtualInput.vDispRecNo)//是否显示序号
    {
        temp = WORDtoASCL(decBuff,val);
        decBuff[temp++]='.';decBuff[temp]=0;
        DispStrXY(decBuff,x,y);
        return temp;
    }
    else
        return 0;
}
/**
 * 直接从Msg[]中读取信息生成列表
 *
 * @author EutronSoftware (2016-02-17)
 */
void ReadFromMsg()
{
    strcpy(ListRecord.Name,Msg[ListRecord.Number+ListDef[ListID].MsgIDX].str);
}
/***************************************************************
 * 以列表的显示列举指定设置文件下已有的数据并选择其中一个用来编辑
 *
 * @author EutronSoftware (2016-01-11)
 *
 * @param fileType:单品/部类/柜组等设置ID
 * @param saveSCR: 是否保护屏幕内容
 * @param caption: 在状态行显示菜单标题信息
 * @param vKey: =true,退出时保存虚拟按键
 *
 *
 * @return int:=0无列表数据
 *             =It_PLU_MG(-1)为按确认键,确认PLU输入和MG选择功能;
 *             =It_EXIT(-2)为按退出键.
 *             其它值,返回所选择的记录序号(1..0xfffd);
 ***************************************************************/

#define LISTLINES   (SCREENLN-1)    //屏幕上可显示的行数
#define ACTIVEITEM  TESTBIT(ApplVar.ScreenStat, INVATTR)  //为当前记录

WORD ListItems(WORD fileType,BYTE saveSCR,const char* caption,BYTE vKey)
{
    static WORD fileLast=0; //用于下次进入时,直接定为到当前记录
    static WORD recLast=0;  //用于下次进入时,直接定为到当前记录

    BYTE byteNum;
    WORD *recIDX;
    char *sStr;
    WORD recFrom,recMAX;//记录号开始编号和最大数目
    WORD recCurrent;    //当前选择的记录好
    WORD recList[LISTLINES];//保存当前找到的记录 (recFrom..n)
    WORD recCount,recCountX;          //recList中的记录数目
    BYTE keyID;
    WORD keyCode,temp;
    int  sLp;

    void    (*ReadFromFile)();
    BYTE    (*PorcessRecord)(BYTE y,WORD recNo);    //对应当前记录的处理函数,=null时,无处理函数,函数返回0时,没有处理按键 //

    //...................................................

    VirtualInputStr(0xff,0,0);//清除所有数据
    ListID=0;
    do
    {
        if (ListDef[ListID].SetupIDX==0xffff)
            return 0;
        if (ListDef[ListID].SetupIDX==fileType)
            break;
        ListID++;
    } while (true);

    recFrom=0;
    VirtualInput.vDispRecNo=1;
    switch (fileType)
    {
    case SETDISC:          //***BYTE //折扣
    case SETPORA:          //***BYTE //出金入金
    case SETCORR:          //***BYTE //更正功能
    case SETTEND:          //***BYTE //付款方式
    case SETCURR:          //***BYTE //外币
    case SETDRAWER:        //***BYTE //钱箱
#if !defined(CASE_FORHANZI)
    case SETTAX:           //***BYTE//税率
#endif                           //
    case SETZONES:         //***BYTE  //报表时段设置
    case SETPBF:           //***BYTE //餐饮功能
        byteNum=true;
        break;
    case SETPLU:
    case MsgMGPRINTSTOCK://ccr2017-01-10
        if (ApplVar.AP.Plu.RandomSize==0)//ccr2016-06-06
            ApplVar.AP.Plu.RNumber = ApplVar.AP.Plu.Number;
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    case MENUXEJOURNAL:
#endif
    case SETSYSFLAG:
        VirtualInput.vDispRecNo=0;
        byteNum=false;
        break;
    case SETGRAP:
        VirtualInput.vDispRecNo=0;
    case SETPBINF: //编号从1开始
    case SETCLERK://收款员和营业员的开始编号从1开始
#if (salNumber)
    case SETSALER:
#endif
        recFrom=1;
    default:
        byteNum=false;
        break;
    }
    if (ListDef[ListID].MsgIDX==0)
        recMAX=ReadRecIndex(ListDef[ListID].RecordMax,byteNum)+recFrom;
    else
        recMAX=(WORD)ListDef[ListID].RecordMax+recFrom;

    //处理列报表数据

    ReadFromFile=ListDef[ListID].ReadRecord;
    PorcessRecord=ListDef[ListID].PorcessRecord;
    recIDX=ListDef[ListID].RecordIdx;
    sStr=ListDef[ListID].Caption;


    keyCode=ID_DOWN;
    if (fileType==fileLast && recLast<recMAX)
        SetRecIndex(recIDX,recLast,byteNum);
    else
        SetRecIndex(recIDX,recFrom,byteNum);
    recCount=0;
    Appl_EntryCounter=0;
    memset((BYTE*)recList,0,sizeof(recList));

    if (saveSCR)
        BackupScreen(1);
    ClsState12();
    if (caption && VirtualInput.vDispRecNo==1)
        DispSt1((BYTE *)caption);
//    else
//        ClsArea(PROMDISPX,STATELINE,SCREENWD-PROMDISPX);
    while (true)//keycode<190) ccr080319
    {
        //读取一组记录数据并在屏幕上显示
        //DispSt0(ApplVar.ModeHead);
        mClearScreen();
        mSetInvAttr();
        recCountX=recCount;
        recCurrent=0;
        recCount=0;
        switch (keyCode)
        {
        case ID_UP://向前查找记录
        case ID_LEFT:
            if (recList[0]>recFrom)
            {
                SetRecIndex(recIDX,recList[0]-1,byteNum);
                while (recCount<LISTLINES)
                {
                    if (ReadRecIndex(recIDX,byteNum)>=recFrom)
                    {
                        //(*ReadFromFile)();
                        //DispStrXY(sStr,0,recCount);
                        //mClearInvAttr();
                            recList[recCount++]=ReadRecIndex(recIDX,byteNum);
                            if (ReadRecIndex(recIDX,byteNum)==recFrom)
                                break;
                            DecRecIndex(recIDX,byteNum);
                    }
                    else
                        break;
                }
                //对找到的数据记录顺序进行反转
                for (sLp=0;sLp<recCount / 2;sLp++)
                {
                    temp=recList[sLp];
                    recList[sLp]=recList[recCount-sLp-1];
                    recList[recCount-sLp-1]=temp;
                }
                //显示记录数据
                for (sLp=0;sLp<recCount;sLp++)
                {
                    SetRecIndex(recIDX,recList[sLp],byteNum);
                    (*ReadFromFile)();
                    DispStrXY(sStr,DispIndex(0,sLp,recList[sLp]+(1-recFrom)),sLp);
                    if (ACTIVEITEM)
                        strncpy(VirtualInput.vInput,sStr,sizeof(VirtualInput.vInput));//ccr2016-02-15
                    mClearInvAttr();
                }
                //避免DOWN查找时重复最好一条记录
                IncRecIndex(recIDX,byteNum);
                break;
            }
//            if (recCount<LISTLINES && recMAX>=LISTLINES)
            {
                SetRecIndex(recIDX,recFrom,byteNum);
                recCount=0;
            }
        case ID_DOWN://向后查找后续记录
        case ID_RIGHT:
        case ID_SELECT:
            while (recCount<LISTLINES)
            {
                if (ReadRecIndex(recIDX,byteNum)<recMAX)
                {
                    (*ReadFromFile)();

                    recList[recCount]=ReadRecIndex(recIDX,byteNum);
                    DispStrXY(sStr,DispIndex(0,recCount,recList[recCount]+(1-recFrom)),recCount);
                    if (ACTIVEITEM)
                        strncpy(VirtualInput.vInput,sStr,sizeof(VirtualInput.vInput));//ccr2016-02-15

                    mClearInvAttr();
                    recCount++;
                    IncRecIndex(recIDX,byteNum);
                }
                else if (recCount==0 && recCountX>0)//显示上一次查找到的内容
                {
                    recCount=recCountX;
                    //显示记录数据
                    //mSetInvAttr();
                    for (sLp=0;sLp<recCount;sLp++)
                    {
                        SetRecIndex (recIDX,recList[sLp],byteNum);
                        (*ReadFromFile)();
                        if (ACTIVEITEM)
                            strncpy(VirtualInput.vInput,sStr,sizeof(VirtualInput.vInput));//ccr2016-02-15

                        DispStrXY(sStr,DispIndex(0,sLp,recList[sLp]+(1-recFrom)),sLp);
                        mClearInvAttr();
                    }
                    break;
                }
                else
                    break;
            }
            break;
        }
        //以下处理选择记录
        recCurrent=0;
        sLp=true;
        while (sLp)
        {//在一组记录上选择
            if (Appl_EntryCounter==0 && VirtualInput.vDispRecNo==0)
                DisplayDecXY(recList[recCurrent]+(1-recFrom),SCREENWD-6,SCREENLN,6);//在状态行显示记录号
            if (PorcessRecord)
            {//反显????
                keyID=keyCode=(*PorcessRecord)(recCurrent,recList[recCurrent]-recFrom);
                if (keyID==0)
                    break;
            }
            else
            {
                SetLoop();
                while (!KbHit())
                {

    #if !defined(DEBUGBYPC)
                    if (!ApplVar.ErrorNumber)
                        CheckTime(true);
                    if (fileType==SETPLU && BarCode())
                        break;
                    Computer();//communicate with computer
    #endif
                    CheckError(0);
                    //continue;
                }
                keyID=keyCode=Getch();
                ResetLoop();
            }
            InActive = 0;

            if (keyCode<127)
            {
                keyCode = GetFirmkeyID(keyCode);
            }

#if defined(DD_LOCK)
#ifdef TR
            else if (keyCode==SET || keyCode==RG || keyCode==TR || keyCode==MG || keyCode==X || keyCode==Z || keyCode==OFF)// ccr080403>>>>
#else
            else if (keyCode==SET || keyCode==RG || keyCode==MG || keyCode==X || keyCode==Z || keyCode==OFF)// ccr080403>>>>
#endif
            {
                KeyFrHost = keyCode;
                //ccr2016-06-16>>>>>>>>>>>>>>
                ClsArea(PROMDISPX,SCREENLN,PROMPTWD);
                if (saveSCR)
                    RecallScreen();
                else
                    mClearScreen();
                mClearInvAttr();

                return It_EXIT;//为退出键标志
                //ccr2016-06-16<<<<<<<<<<<<<<<<<
            }
#endif
            else
                continue;//<<<<<<<<<<

            if (keyCode<FIRMKEYS)
            {
                switch (keyCode)
                {
                case ID_CLEAR:
                    ApplVar.ErrorNumber=0;
                    ClearEntry(true);
                    break;
                case ID_DELETE://与 ID_RJFEED 相同
                    if ((fileType==SETHEAD || fileType==SETTRAIL || fileType==SETSHEAD || fileType==SETPLU)
                        && InputYesNo(Msg[SHANCHU].str,PROMDISPX+1,SCREENLN,1)=='Y')
                    {//删除单品/票头等

                        SetRecIndex(recIDX,recList[recCurrent],byteNum);
                        if (fileType==SETHEAD)
                            DeleteHeader(recList[recCurrent]);
                        else if (fileType==SETTRAIL)
                            DeleteTrailer(recList[recCurrent]);
                        else if (fileType==SETSHEAD)
                            DeleteSlipHead(recList[recCurrent]);
                        else if (fileType==SETPLU)
                        {
                            GetPluOffSet();
                            CheckRandomPlu(1,1);
                        }

                        keyCode=ID_DOWN;
                        sLp=false;//中断循环
                    }
                    else
                        RFeed(1);
                    break;
                case ID_ENTER:
                    ClsArea(PROMDISPX,SCREENLN,PROMPTWD);
                    if (saveSCR)
                        RecallScreen();
                    else
                        mClearScreen();
                    fileLast=fileType;
                    recLast=recList[recCurrent];

                    temp = recLast+(1-recFrom);//收款员等的记录号从1开始
                    if (fileType==LISTMGFUNC && temp<=7)
                    {
                        if ((keyID = PosOfFuncKey(SUB))!=0xff)
                        {//虚拟1/2/3/4/5/6/7+小计
                            VirtualInputWord(keyID,temp);
                            temp = It_PLU_MG;
                        }
                    }
                    else if (fileType==SETPLU && Appl_EntryCounter)
                    {//虚拟输入条码编辑单品
                        VirtualInputStr(ApplVar.AP.FirmKeys[ID_ENTER],EntryBuffer + ENTRYSIZE - Appl_EntryCounter - 1,Appl_EntryCounter);
                        ClearEntry(false);
                        mClearInvAttr();
                        return It_PLU_MG;
                    }
                    if (Appl_EntryCounter)
                        ClearEntry(false);

                    if (recMAX)
                    {
                        mClearInvAttr();
                        return temp;
                    }
                case ID_CANCEL:
                case ID_LOCK://ccr2016-11-15
                    if (keyCode==ID_LOCK &&
                        Appl_EntryCounter && ApplVar.NumberEntry>0 && ApplVar.NumberEntry<=5 &&
                        (ApplVar.CentralLock==SET || ApplVar.CentralLock==X || ApplVar.CentralLock==Z ))
                        VirtualInputStr(ApplVar.AP.FirmKeys[ID_LOCK],EntryBuffer + ENTRYSIZE - Appl_EntryCounter - 1,Appl_EntryCounter);//虚拟按方式锁键
                    else if (vKey)
                    {
                         VirtualInputStr(ApplVar.AP.FirmKeys[ID_CANCEL],0,0);//虚拟按取消键
                    }

                    ClsArea(PROMDISPX,SCREENLN,PROMPTWD);
                    if (saveSCR)
                        RecallScreen();
                    else
                        mClearScreen();
                    mClearInvAttr();
                    return It_EXIT;//为退出键标志
                case ID_UP://向上显示
                case ID_LEFT:
                    if (recCurrent>0)
                    {//显示上一条记录数据
                        SetRecIndex(recIDX,recList[recCurrent],byteNum);
                        (*ReadFromFile)();
                        mClearInvAttr();

                        DispStrXY(sStr,DispIndex(0,recCurrent,recList[recCurrent]+(1-recFrom)),recCurrent);
                        recCurrent--;

                        SetRecIndex(recIDX,recList[recCurrent],byteNum);
                        (*ReadFromFile)();
                        mSetInvAttr();

                        strncpy(VirtualInput.vInput,sStr,sizeof(VirtualInput.vInput));//ccr2016-02-15
                        DispStrXY(sStr,DispIndex(0,recCurrent,recList[recCurrent]+(1-recFrom)),recCurrent);
                        mClearInvAttr();
                    }
                    else
                        sLp=false;//中断循环
                    break;
                case ID_SELECT:
                    sLp=false;//翻页显示,中断循环//recCurrent=recCount-1;
                    break;
                case ID_DOWN:
                case ID_RIGHT:
                    if (recCurrent<recCount-1)
                    {//显示下一条记录数据
                        SetRecIndex(recIDX,recList[recCurrent],byteNum);

                        (*ReadFromFile)();
                        mClearInvAttr();

                        DispStrXY(sStr,DispIndex(0,recCurrent,recList[recCurrent]+(1-recFrom)),recCurrent);
                        recCurrent++;

                        SetRecIndex(recIDX,recList[recCurrent],byteNum);
                        (*ReadFromFile)();
                        mSetInvAttr();

                        strncpy(VirtualInput.vInput,sStr,sizeof(VirtualInput.vInput));//ccr2016-02-15
                        DispStrXY(sStr,DispIndex(0,recCurrent,recList[recCurrent]+(1-recFrom)),recCurrent);
                        mClearInvAttr();
                    }
                    else
                    {
                        SetRecIndex(recIDX,recList[recCurrent],byteNum);
                        IncRecIndex(recIDX,byteNum);
                        sLp=false;//中断循环
                    }
                    break;
                default:
                    Bell(1);
                    break;
                }
                Appl_EntryCounter=0;
            }
            else
            {
                keyCode = ApplVar.AP.KeyTable[keyID];//			i = GetNumric(keycode);
                if (keyCode>='0' && keyCode <='9' && Appl_EntryCounter<13)
                {
                    AppendEntry(keyCode);
#if defined(CASE_FORHANZI)
                    DispStrXY(EntryBuffer + ENTRYSIZE - 12 - 1 ,SCREENWD-12,STATELINE);
#else
                    DispStrXY(EntryBuffer + ENTRYSIZE - 13 - 1 ,SCREENWD-13,STATELINE);
#endif
                    GetEntry();
                    if ((WORD)ApplVar.NumberEntry>=recFrom && (WORD)ApplVar.NumberEntry<=recMAX)
                    {
                        SetRecIndex(recIDX,(WORD)ApplVar.NumberEntry-(1-recFrom),byteNum);
                        keyCode=ID_DOWN;
                        sLp=false;
                    }
                }
                else
                {
                    if (keyCode>DEPT && keyCode<PLU1 && fileType==SETDEPT)
                    {//按部类键时,直接进入对应的部类
                        keyCode -= DEPT;
                    }
                    else if (keyCode>PLU1 && keyCode<PLU3 && fileType==SETPLU)
                    {//单品,键盘上通常无直接单品按键
                        keyCode -= PLU1;
                    }
                    /*else if (keyCode>DISC && keyCode<DRAW && fileType==SETDISC)
                    {//折扣
                        keyCode -= DISC;
                    }
                    else if (keyCode>TEND && keyCode<MODI && fileType==SETTEND)
                    {//付款方式
                        keyCode -= TEND;
                    }*/
                    else
                        keyCode=0;
                    if (keyCode)
                    {
                        ClsArea(PROMDISPX,SCREENLN,PROMPTWD);
                        if (saveSCR)
                            RecallScreen();
                        else
                            mClearScreen();
                        return keyCode;
                    }
                    else
                        Bell(1);
                }
            }
        }//while (sLp)
    }//while (true)
}

/**
 * 根据文件类型和指定的记录号,从收款机文件中获取记录名称
 *
 * @author EutronSoftware (2016-02-25)
 *
 * @param fileType:文件类型
 * @param idx:记录序号
 * @param caption:返回的记录中的名称
 *
 * @return word:true-有对应的名称;false-无对应的名称
 */
WORD ReadItemCaption(WORD fileType,WORD idx,char *caption)
{
    BYTE byteNum;
    WORD *recIDX;
    char *sStr;
    int  sListIdx;
    WORD recFrom,recMAX;//记录号开始编号和最大数目
    void    (*ReadFromFile)();
    //...................................................

    sListIdx=0;
    do
    {
        if (ListDef[sListIdx].SetupIDX==0xffff)
        {
            caption[0]=0;
            return 0;
        }
        if (ListDef[sListIdx].SetupIDX==fileType)
            break;
        sListIdx++;
    } while (true);

    recFrom=0;

    switch (fileType)
    {
    case SETDISC:          //***BYTE //折扣
    case SETPORA:          //***BYTE //出金入金
    case SETCORR:          //***BYTE //更正功能
    case SETTEND:          //***BYTE //付款方式
    case SETCURR:          //***BYTE //外币
    case SETDRAWER:        //***BYTE //钱箱
#if !defined(CASE_FORHANZI)
    case SETTAX:           //***BYTE//税率
#endif                           //
    case SETZONES:         //***BYTE  //报表时段设置
    case SETPBF:           //***BYTE //餐饮功能
        byteNum=true;
        break;
    case SETGRAP:
    case SETCLERK://收款员和营业员的开始编号从1开始
#if (salNumber)
    case SETSALER:
#endif
        recFrom=1;
    default:
        byteNum=false;
        break;
    }
    if (ListDef[sListIdx].MsgIDX==0)
        recMAX=ReadRecIndex(ListDef[sListIdx].RecordMax,byteNum)+recFrom;
    else
        recMAX=(WORD)ListDef[sListIdx].RecordMax+recFrom;

    if (idx<recMAX)
    {
        ReadFromFile=ListDef[sListIdx].ReadRecord;
        recIDX=ListDef[sListIdx].RecordIdx;
        sStr=ListDef[sListIdx].Caption;
        SetRecIndex(recIDX,idx,byteNum);
        (*ReadFromFile)();
        strcpy(caption,sStr);
        return true;
    }
    else
    {
        caption[0]=0;
        return false;
    }

}
/**
 * 打印XZ功能菜单
 *
 * @author EutronSoftware (2016-11-16)
 *
 * @param xz
 */
void PrintXZMenu(BYTE xz)
{
    MenuDefine *ItemXZReport;
    int msgFr,msgTo,msgLp;
    WORD recFrom,recMAX;//记录号开始编号和最大数目
    WORD recCurrent;    //当前选择的记录好
    WORD *recIDX;
    char *sStr,pBuf[PRTLEN+3];
    WORD recCount,recCountX;          //recList中的记录数目
    void    (*ReadFromFile)();


    if (xz==X)
    {
        msgFr = MENUXREPORT;
        msgTo = msgFr+MENUXITEMMAX-1;
        ItemXZReport = (MenuDefine *)&ItemXReport;
        pBuf[0]='X';
    }
    else if (xz==Z)
    {
        msgFr = MENUZREPORT;
        msgTo = msgFr+MENUZITEMMAX-1;
        ItemXZReport = (MenuDefine *)&ItemZReport;
        pBuf[0]='Z';
    }
    else
        return;

    pBuf[1]=' ';
    strcpy(pBuf+2,cMESSSETMENU);
    PrintLine2('.');
    PrintStr_Center(pBuf,true);
    PrintLine2('=');
    for (msgLp=msgFr;msgLp<=msgTo;msgLp++)
    {

        //打印上机菜单名称
        recFrom = msgLp-msgFr+1;

        memset(pBuf,' ',sizeof(pBuf));
        if (recFrom>9) pBuf[3]=recFrom/10+'0';
        pBuf[4]=recFrom % 10+'0';
        pBuf[5]='.';
        strncpy(pBuf+6,Msg[msgLp].str,sizeof(pBuf)-6);
        pBuf[PRTLEN+3-1]=0;
        RJPrint(0,pBuf+3);


        recFrom=0;
        do
        {
            if (ItemXZReport[recFrom].MenuFrom==0)
            {
                recFrom = 0xffff;
                break;
            }
            else if (ItemXZReport[recFrom].MenuFrom==msgLp)
                break;
            recFrom++;
        } while (true);
        if (recFrom == 0xffff || ((recFrom = ItemXZReport[recFrom].MenuItems)==0))
            continue;

        ListID=0;
        do
        {
            if (ListDef[ListID].SetupIDX==0xffff)
            {
                ListID = 0xffff;
                break;
            }
            else if (ListDef[ListID].SetupIDX==recFrom)
                break;
            ListID++;
        } while (true);
        if (ListID == 0xffff)
            continue;

        if (ListDef[ListID].MsgIDX==0)
            recMAX=ReadRecIndex(ListDef[ListID].RecordMax,true);
        else
            recMAX=(WORD)ListDef[ListID].RecordMax;

        //处理列报表数据

        recIDX=ListDef[ListID].RecordIdx;
        ReadFromFile=ListDef[ListID].ReadRecord;
        sStr=ListDef[ListID].Caption;

        recFrom=1;
        SetRecIndex(recIDX,0,true);

        while (recFrom<=recMAX)
        {
            if (ReadRecIndex(recIDX,true)<recMAX)
            {
                (*ReadFromFile)();
                if (recFrom>9)
                    pBuf[6]=recFrom/10+'0';
                else
                    pBuf[6]=' ';
                pBuf[7]=recFrom % 10+'0';
                pBuf[8]='.';
                strncpy(pBuf+9,sStr,sizeof(pBuf)-9);
                pBuf[PRTLEN-1]=0;
                RJPrint(0,pBuf);
                recFrom++;
                IncRecIndex(recIDX,true);
            }
        }
    }
}
/**
 * 打印经理档下的操作功能列表
 *
 * @author EutronSoftware (2016-11-18)
 */
void PrintMGMenu()
{
    int i;
    char pBuf[PRTLEN+3];

    PrintLine2('.');
    PrintStr_Center(Msg_MANAGEMENT,true);
    PrintLine2('=');
    memset(pBuf,' ',PRTLEN);
    for (i=1;i<=MsgMGMMAX;i++)
    {
        if (i>9)
            pBuf[0]=i/10+'0';
        else
            pBuf[0]=' ';
        pBuf[1]=i % 10+'0';
        pBuf[2]='.';
        strncpy(pBuf+3,Msg[i+MsgMGADDINV-1].str,PRTLEN-3);
        RJPrint(0,pBuf);
    }
}
