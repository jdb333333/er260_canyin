//---------------------------------------------------------------------------
#include "king.h"  /* Kingtron library and I/O routines */

#include "message.h"

//#include "types.h"

CONST char SPACE[2]=" ";

//---------------------------------------------------------------------------
extern CONST struct XZREPORT XZTitle[XZNUM];
//---------------------------------------------------------------------------

CONST FSTRING Msg[]={//HZMESSMAX
//captions for current mode
"WAITING....",                       //LING
CurrentMode1,                 //SHOUKUAN//"营业",
CurrentMode2,                 //XBBIAO//"读帐",
CurrentMode3,                 //ZBBIAO//"清帐",
CurrentMode4,                 //SHEZHI//"设置",
CurrentMode5,                 //JINLI//"经理",
//vvvvvvvvvvvvvvvv设置档菜单相关的信息描述vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
/* 设置档一级菜单(主菜单) */
SetupMode2,					//MENUFILES  // "资料管理",
SetupMode1,					//ZHGNSZ  // "系统设置",
SetupKeyboard,               //DEFKEYBOARD  //键盘定义
SetupMode6,					//PJYSSZ  // "票头票尾",
msgCOMMHOST,                //COMMHOST //通讯
SetupMode3,					//TXKYWS  // "外接设备",
SetupMode8,					//CYGNSZ  // "餐饮功能",
SetupMode7,					//BBGNSZ  // "报表功能",
msgDISCOUNT,                //MENUDISC //折扣促销
SetupMode4, 				//JYGLSZ  // "销售功能设置",
SetupMode5,					//ICKGLI  // "IC卡管理",
#if !defined(DD_LOCK)
SetupMode9,					//QXSZHI  // "收银员和密码",
#endif
SetupMode10,				//MENUSERVICE // "收款机测试",

//设置档二级菜单>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/* SETECRFILES下的综合功能设置菜单 */
SysSettings1, 			         //SETSYSFLAG //"系统参数设置",
POINTTYPE,                       //SETPOINTTYPE "设置小数类型"
SysSettings2,            	     //SETDATE    //"日期",
SysSettings3,                 	 //SETTIME    //"时间",
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
msgSETEJOURNAL,                  //SETEJOURNAL //EJOURNAL
#endif

/* DEFKEYBOARD下的综合功能设置菜单 */
SysSettings4,                 	 //SETKEYB    //"键盘",
MESSKEYMACRO,                   //SETKEYMACRO  "键盘宏"
MESSKEYTYPE,                    //SETKEYTYPE   "键盘模式"
PRINTKEYBOARD,                  //SETPRINTKEYB "打印键盘布局"

SysSettings5,	                 //SETREST    //"餐饮键盘切换",
SysSettings6,	                 //SETMARKET  //"超市键盘切换",

SetupCaption1,                 	//SETGROUP  //"柜组",
SetupCaption39,                 //SETSALER  //"营业员",

/* SPGLSZ下的商品管理菜单 */
SetupCaption3,                	//SETPLU    //"单品",
SetupCaption2,                 	//SETDEPT   //"部类",
#if !defined(CASE_FORHANZI)
SetupCaption20,                 //SETTAX    // "税率",
#endif
SetupCaption38,                 //SETCLERK  //"收款员",

/* COMMHOST下的通讯与外设菜单 */
SetupCaption5,                  //SETCOMPUTER//"计算机",
SetupCaption9,			        //SETIP  // "网络设置",
#if defined(CASE_GPRS)
SysSettings8,					//SETGPRS //"GPRS通信"  /ccr2014-11-11 NEWSETUP
#endif

/* TXKYWS下的通讯与外设菜单 */
SetupCaption6,                  //SETBARCODE//"条码枪",
SetupCaption7,                  //SETBALANCE//"电子秤",
//ccr2016-01-22SetupCaption8,                  //SETPORT4//"通讯口 4",
SetupCaption10,                 //SETSP// "票据打印",
SetupCaption11,                 //SETKP// "厨房打印",
SetupEXTPrinter,                //SETEXTPRINTER //"EXTERNAL PRINT"

SetupCaption13,                 //SETPROM   // "中奖设置",

/* MENUDISC下的销售功能设置*/
SetupCaption12,                 //SETOFF    //"优惠价",
SetupCaption14,                 //SETDISC   // "折扣",

/* JYGLSZ下的销售功能设置*/
SetupCaption17,                 //SETTEND   // "付款方式",
SetupCaption16,                 //SETCORR   // "更正功能",
SetupCaption19,                 //SETDRAWER // "钱箱",
SetupCaption15,                 //SETPORA   // "出金入金",
SetupCaption18,                 //SETCURR   // "外币",

/* ICKGLI下的IC卡管理*/
SetupCaption21,                 //SETVIPIC         //"IC卡设置",
SetAddNewECRVIP,         //SETADDNEWVIP //"ADD NEW VIP"
SetPrintECRVIP,          //SETPRINTVIP //"PRINT VIP"
SetCleanALLVIP,          //SETCLEANVIP //"CLEAN VIP"

SetupCaption22,                 //SETBLOCKIC    //"IC卡挂失",
SetupCaption23,	                //SETPRINTIC    //"打印卡信息",

/* PJYSSZ下的票头票尾*/
SetupCaption24,                 //SETHEAD       // "收据票头",
SetupCaption25,                 //SETTRAIL      // "收据票尾",
SetupCaption26,                 //SETSHEAD      // "票据票头",
SetupCaption27, 		        //SETGRAP       // "图形",
SetupCaption28,			        //SETPRTGRAP    //"打印图形",

/* BBGNSZ下的报表功能*/
SetupCaption29,                 //SETREPORT     //"设置报表内容",
SetupCaption30,                 //SETREPTYPE    //"报表单元",
SetupCaption31,                 //SETZONES      //"报表时段",
SetupCaption32,			        //SETCLRREP    //"总清报表",
SetupCaption33,			        //SETPRTJOUR   //"打印流水",

/* CYGNSZ下的餐饮功能*/
SetupCaption34,                 //SETPBINF  //"桌台",
SetupCaption35,                 //SETPBF    //"餐饮功能",
SetupCaption36,                 //SETMODIF  //"菜单说明",
#if (agreeNumber)
SetupCaption37,                 //SETAGREE  //"签单",
#endif
/* QXSZHI下的密码权限设置*/
#if !defined(DD_LOCK)
SetupCaption40,		            //SETSETPWD  //"设置档密码",
SetupCaption42,                 //SETXPWD    //"只读报表密码",
SetupCaption43,	                //SETZPWD    //"清帐报表密码",
#endif
SetupCaption41,		            //SETMGPWD   //"经理档密码",

/* MENUSERVICE下的系统测试功能 */
msgSETTEST,                     //SETTEST    //"系统测试"
msgSETCLEARPLU,                 //SETCLEARPLU    //"清除单品"
msgSETPLULIST,                  //SETPLULIST   //"打印商品清单"
msgSETTSTVER,                  //SETTSTVER        //"打印版本"
msgSETCONFIGPLU,                //SETCONFIGPLU //"配置单品"
msgSETCONFIGDEPT,               //SETCONFIGDEPT //"配置部类"
#if CASE_USBDISK
msgSETINSTALL,                  //SETINSTALL    //"安装/备份"
#endif
msgSETPRINTFAT,                 //SETPRINTFAT   //"打印文件分配表"
msgSETPRNMENU,                  //SETPRNMENU   //打印设置菜单


/* SKJCSHI下的系统测试功能 */
//SetupCaption44,	                //SETTSTVER   //"内存测试",
SetupCaption45,                 //SETTSTDISP  //"显示测试",
SetupCaption451,                 //SETTSTCUST  //"客显示测试",
SetupCaption46,	                //SETTSTPRT   //"打印版本",
SetupCaption47,	                //SETTSTCLK   //"时钟测试",
SetupTestRAM,                   //SETTSTMEM	       //RAM测试
SetupCaption48,	                //SETTSTBELL  //"蜂鸣器测试",
SetupCaption49,	                //SETTSTDRAW  //"钱箱测试",
SetupCaption50,                 //SETTSTKBD   //"键盘测试",
SetupCaption51,	                //SETTSTCOMM  //"串口测试",
SetupCaption52,	                //SETTSTAUTO  //"自动测试",

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

OperationN1,			        //JLHAO//"记录#",
OperationN2,			        //SHRDMA//"记录?",

OperationN3,                    //ZONGJI//"总计",
OperationN4,                    //ZHHUAN//"转换",
OperationN5,                    //XIAOJI//"小计",
OperationN6,                    //CHUJIN//"出金",
OperationN7,                    //RUJIN//"入金",
OperationN8,                    //BAOBIAO//"报表",
OperationN9,                    //ZENJIA//"增加",
OperationN10,                   //GUAQI//"挂起",
OperationN11,                   //QUXIAO//"取消",

OperationN12,                   //MIMA//"密码",
OperationN13,                   //MMQREN//"密码确认",
OperationN14,                   //DANJIA//"单价:",
OperationN15,                   //JINEF//"金额:",
OperationN16,                   //CHAOSHI//"超市版",
OperationN17,                   //CANYIN//"餐饮版",
OperationN18,                   //CEISHI//"测试",
"  RAM:",                       //RAM

//captions
CaptionN1,                      //YINGFU//"应付",
CaptionN2,                      //JINGE//"净额",
CaptionN3,                      //JIAGE//"价格",
CaptionN4,                      //JIAOYISHU//"交易数",
CaptionN5,                      //SHULIANG//"数量",
CaptionN6,                      //JINE//"金额",
CaptionN7,                      //SHOUQU//"收取",
CaptionN8,                      //ZHAOLING//"找零",
CaptionN9,			            //RI//"日",
CaptionN10,                     //ZHOU//"周",
CaptionN11,			            //YUE//"月",
CaptionN12,			            //NIAN//"年",
CaptionN13,                     //BIAOZHUN//"标准",
CaptionN14,                     //SHKYUANS//"收款员",
CaptionN15,                     //SHI//"时",
CaptionN16,			            //RIS//"   日",
CaptionN17,			            //YUES//"   月",
CaptionN18,                     //TUIHUO//"退货",
CaptionN19,                     //CHENBEN//"成本",
CaptionN20,                     //ZUOHAO//"桌号#",
CaptionN21,                     //ZHPHAO//"支票号#",
CaptionN22,                     //XINAGSHU//"项目数",
CaptionN23,                     //HSHXSHOU//"含税销售",
CaptionN24,                     //SHUIE//"税额",
CaptionN25,                     //YEHAO//"页号",
CaptionN26,                     //HAOMA//"号码:",
CaptionN27,                     //GUAZHE//"挂帐",
CaptionN28,                     //KAITAI//"开台",
CaptionN29,                     //QZHJSHU//"清帐计数",
CaptionN30,                     //QINGCHU//"清除",
CaptionN31,                     //SBAOBIAO//"S-报表",
CaptionN32,                     //MAOLI//"毛利",
CaptionN33,                     //KCZJIA//"增加库存",
CaptionN331,                     //KCJIANSHAO//"减少库存",
CaptionN34,                     //YZHZHANG//"原桌帐:",
CaptionN35,                     //XYKHAO//"信用卡#",
CaptionN36,                     //XZHZHANG//"新桌帐:",
CaptionN37,                     //RENSHU//"人数",
CaptionN38,                     //HUANHUO//"换货",

CaptionN39,                     //ZENJIAA//"增加?",
CaptionN40,                     //SHANCHU//"删除?",
CaptionN41,                     //ZHKOU1//"折扣项1",
CaptionN42,                     //ZHKOU1//"折扣项2",
CaptionN43,                     //XSHOUE//"销售额",,
CaptionN44,                     //DBYHUI//"大包优惠",
CaptionN45,                     //KCSHLIANG//"库存数量",
CaptionN46,                     //TJYHUI//"特价优惠",
CaptionN47,                     //XUANXIANG//"选项:",
CaptionN48,                     //GDZHKOU//"固定",
CaptionN49,                     //"浮动",
CaptionN50,                     //"固/浮",

CaptionN51,                     //KTZHJIE//"开台暂结",
CaptionN55,                     //KTZJIA//"开台增加",
CaptionN53,                     //ZHANJIE//"暂结",
CaptionN54,                     //QRJZHANG//"确认结帐",
CaptionN55,                     //DYKTAI//"打印开台",
CaptionN56,                     //DYZHDAN//"打印帐单",
CaptionN57,                     //QXQREN//"取消确认",
CaptionN58,                     //FENZHANG//"分帐",
CaptionN59,                     //ZHUANZHANG//"转帐",
CaptionN60,                     //GHSHKYUAN//"更换收款员",
CaptionN61,                     //GKRSHU//"顾客人数",

CaptionN62,                     //ZHJSHDING//"中奖设定:",
CaptionN63,                     //JIFEN//"积分",

CaptionN64,                     //GBDPJGE//"改变单品价格",
CaptionN65,			            //CHFDYJZU//"厨房打印机组"，
CaptionN66,                     //ZJKCUN//"增加库存",
CaptionN67,                     //JSHKCUN//"减少库存",
CaptionN68,                     //SHURU//"输入",
CaptionN69,                     //DAIFU//"待付",

CaptionE1,                      //RS232ERR//"RS232错",
CaptionE2,                      //RS232OK//"RS232正确",
CaptionE3,                      //RTCERR//"RTC 错!",
CaptionE4,                      //CTCERR//"CTC 错!",
CaptionE5,                      //FPROMERR//"FPROM错!",
CaptionE6,                      //CHSHHUA//"初始化...",
CaptionE7,                      //SHSHHJSHU//"初始化结束.",
CaptionE8,                      //CHUCUO//"出错-",

CaptionE9,                      //MMACUO//"密码错 ！",
CaptionE10,                     //BCMMA//"密码已保存",
CaptionE11,                     //SHKYUANF//"收款员:",
CaptionE12,                     //YYYUANF//"营业员:",
CaptionE13,                     //SHKJHAO//"收款机号:",
CaptionE14,                     //WEIZHI//"位置:",
CaptionE15,                     //QUERENF//"确认?",
CaptionE16,                     //WANCHENG//"完成",

CaptionE17,                     //FXBBEN//"发行版本:",
CaptionE18,                     //BCXFEI//"本次消费点",
CaptionE19,                     //ZJDPIN//"新增单品:",
CaptionE20,                     //SHCHDP//"删除单品:",
CaptionE21,                     //KSHJCE//"开始检测>>",
CaptionE22,                     //JCJSHU//"<<检测结束",
CaptionE23,                     //KUCUN//"库存",
CaptionE24,                     //BBQQING//"报表已全清!",
CaptionE25,                     //GXBIOS//"更新程序切勿断电",
CaptionE26,                     //WJKJYJIE//"文件空间越界",
CaptionE27,	                    //JSHDPSHL//CCr 内存溢出！
CaptionE28,			            //RZQMAN//"流水存储区满",
CaptionE29,			            //GHDYZHI//"请更换打印纸",
CaptionE30,			            //KZNCCUO//"扩展内存出错！",

CaptionE31,                     //SHUOMINF//"说明文字",

CaptionE32,                     //CWXXI01//"无效输入!",
CaptionE33,                     //CWXXI02//"无效日期!",
CaptionE34,                     //CWXXI03//"无效时间!",
CaptionE35,                     //CWXXI04//"禁止输入!",
CaptionE36,                     //CWXXI05//"PLU代码为空!",
CaptionE37,                     //CWXXI06//"PLU文件溢出!",
CaptionE38,                     //CWXXI07//"桌台被占用!",
CaptionE39,                     //CWXXI08//"桌台已打开!",
CaptionE40,                     //CWXXI09//"桌台号不对!",
CaptionE41,                     //CWXXI10//"请输入桌台号",
CaptionE42,                     //CWXXI11//"没有开台",
CaptionE43,                     //CWXXI12//"桌台桌帐溢出",
CaptionE44,                     //CWXXI13//"禁止修改时间",
CaptionE45,                     //CWXXI14//"仍在销售中!",
CaptionE46,                     //CWXXI15//"销售缓冲区满",
CaptionE47,                     //CWXXI16//"商品没有售出",
CaptionE48,                     //CWXXI17//"正在结算中!",
CaptionE49,                     //CWXXI18//"输入数据超长",
CaptionE50,                     //CWXXI19//"正在付帐!",
CaptionE51,                     //CWXXI20//"人数溢出!",
CaptionE52,                     //CWXXI21//"没有确认结帐",
CaptionE53,                     //CWXXI22//"禁止取消付款",
CaptionE54,                     //CWXXI23//"无菜名!",
CaptionE55,                     //CWXXI24//"无此收款员!",
CaptionE56,                     //CWXXI25//"收款员改变",
CaptionE57,                     //CWXXI26//"无此类报表!",
CaptionE58,                     //CWXXI27//"报表打印中断",
CaptionE59,                     //CWXXI28//"必须在经理档",
CaptionE60,                     //CWXXI29//"不能分帐时用",
CaptionE61,                     //CWXXI30//"必须输入号码",
CaptionE62,                     //CWXXI31//"钥匙档位错",
CaptionE63,                     //CWXXI32//"未授权!",
CaptionE64,                     //CWXXI33//"请指定收款员",
CaptionE65,                     //CWXXI34//"请指定营业员",
CaptionE66,                     //CWXXI35//"禁止PLU变价！",
CaptionE67,                     //CWXXI36//"密码不对!",
CaptionE68,                     //CWXXI37//"厨房打印机错",
CaptionE69,                     //CWXXI38//"票据打印机错",
CaptionE70,                     //CWXXI39//"打印压杆抬起",/打印机忙
CaptionE71,                     //CWXXI40//"打印纸用完!",
CaptionE72,                     //CWXXI41//"打印温度太高",
CaptionE73,                     //CWXXI42//"功能未定义!",
CaptionE74,                     //CWXXI43//"必须输入金额",/* zero price not allowed enter amount */
CaptionE75,                     //CWXXI44//"禁止多种付款",
CaptionE76,                     //CWXXI45//"记帐无此功能",
CaptionE77,                     //CWXXI46//"未按小计键!",
CaptionE78,                     //CWXXI47//"正在管理库存",
CaptionE79,                     //CWXXI48//"保存流水错",
CaptionE80,			            //CWXXI49//"MODEM通讯错!",


CaptionE81,                     //CWXXI50//"卡操作出错!"
CaptionE82,                     //CWXXI51//"POS代码错!"
CaptionE83,                     //CWXXI52//"卡数据错!"
CaptionE84,                     //CWXXI53//"为过期卡!"
CaptionE85,                     //CWXXI54//"为挂失卡!"
CaptionE86,                     //CWXXI55//"客户类别错!"
CaptionE87,                     //CWXXI56//"为新卡!"
CaptionE88,                     //CWXXI57//"不是购物卡!"
CaptionE89,                     //CWXXI58//"写卡出错!"
CaptionE90,                     //CWXXI59//"卡号不对!"
CaptionE91,			            //CWXXI60//"禁用折扣卡!"
CaptionE92,			            //CWXXI61//"禁用现金卡!"
CaptionE93,			            //CWXXI62//"禁用赊帐卡!"
CaptionE94,			            //CWXXI63//"禁用IC卡!"
CaptionE95,			            //CWXXI64//"清卡出错!",
CaptionE96,			            //CWXXI65//"数据溢出!",
CaptionE97,			            //CWXXI66//"IC充值出错!",
CaptionE98,			            //CWXXI67//"IC初始化出错",
CaptionE99,			            //CWXXI68//"禁止初始化!",
CaptionE100,			        //CWXXI69//"IC金额不足!",
CaptionE101,			        //CWXXI70//"IC付款出错!",
CaptionE102,			        //CWXXT71//Hf "IP地址错"
CaptionE103,			        //CWXXI72// ccr "无IC卡!",
CaptionE105,			        //CWXXI73//"有挂单",

CaptionERR105,     				// CWXXI74     //  20070803"电池电量低"
CaptionERR106,     				// CWXXI75     //  20070806"没激活库存"

//ccr090715>>>>>>>>>>>>>>>>>>>>>>>>>
CaptionERR76,			     //  CWXXI76 无U盘!     //
CaptionERR77,			     //  CWXXI77 打开文件出错     //
CaptionERR78,			     //  CWXXI78 读文件出错!     //
CaptionERR79,			     //  CWXXI79 写文件出错!     //
CaptionERR80,			     //  CWXXI80 建新文件出错     //
CaptionERR81,			     //  CWXXI81 关闭文件出错     //
CaptionERR82,                //  CWXXI82 须发送流水     //

CaptionERR83,                 //CWXXI83  //GPRS出错
CaptionERR84,                 //CWXXI84  //发送数据出错
CaptionERR85,                 //CWXXI85  //接收数据错
CaptionERR86,                 //CWXXI86  //无法连接服务器
CaptionERR87,                 //CWXXI87  //无服务器IP地址
CaptionERR88,                 //CWXXI88  //无SIM卡
CaptionERR89,                 //CWXXI89  //GPRS未就绪
CaptionERR90,                //CWXXI90  //已到最大数
CaptionERR91,                //CWXXI91          //无此会员

//ccr2016-10-24 EJ >>>>>>>>>>>>>>>>>>>
EJ_CWXXI92  ,              	//CWXXI92 	"EJ DATA ERROR"
EJ_CWXXI93  ,              	//CWXXI93 	"NO SD CARD"
EJ_CWXXI94  ,              	//CWXXI94 	"EJ R/W ERROR"
EJ_CWXXI95  ,              	//CWXXI95 	"NO EJ RECORDS"
EJ_CWXXI96  ,              	//CWXXI96 	"EJ NEARLY FULL"
EJ_CWXXI97  ,              	//CWXXI97 	"EJ IS FULL"
EJ_CWXXI98  ,              	//CWXXI98 	"NEW EJ"
EJ_CWXXI99  ,              	//CWXXI99 	"RESET EJ FIRST"
EJ_CWXXI100 ,              	//CWXXI100	"EJ NOT MATCHED"

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

CaptionE104,			        //DKQXIANG//"打开钱箱",
CaptionE106,					//YOUGUADAN "有挂单"    //
CaptionE107,					//SHURUQTYAMT
CaptionE108,                    //ZHUANRUDAO "转入到"

"Error-",			            //ERRORMSG
"CUR",				            //CURRENCY
"TOT",				            //TOTALMSG
"CHG",				            //CHANGE
"SUB",				            //SUBTOTAL
"Dis",				            //DISCOUNT
MsgPUTOUT,   	                //PUTOUT
MsgRECIEVIN, 		            //RECIEVIN
"DRAW",  			            //OPENDRAW
"CANC",  			            //CANCEL
"MODI",				            //ADDMODI

"TABL",          		        //TABLENUM
"SER",          		        //SERVICE
"CPA",          		        //CHECKPAID
"CHK",          		        //CHECKCPAID
"COV",          		        //COVERS

"COR",    			            //CORRECT
"RET",    			            //REFUND
"CAN1",    			        //CANCEL1
"CAN2",    			        //CANCEL2

DownLoad1,			            //ZZXZZKU//"正在下载字库",
DownLoad2,			            //ZZXZTXING//"正在下载图形",
SpecialSymbol1,			        //DOWNLOAD//"★",
SpecialSymbol2,			        //UPLOAD//"☆",

OptionN1,                       //KAI//"是",
OptionN2,                       //GUAN//"否",
OptionN3,                       //MINCHENF//"名称:",
OptionN4,                       //BULEIF//"部类:",
OptionN5,                       //GUIZUF//"柜组:",

"HOLD",                          //HOLD
OptionN6,                       //DAYINF//"打印:",
OptionN7,                       //XUANXIANGF//"选项:",
OptionN8,                       //LIANJIE//"链接:",
OptionN9,                       //SHUILVF//"税率:",
OptionN10,                      //WZHBHAOF//"位置编号:",
OptionN11,                      //JIAGE1//"价格 1:",
OptionN12,                      //JIAGE2//"价格 2:",
OptionN13,                      //JIAGE3//"价格 3:",
OptionN14,                      //JIAGE4//"价格 4:",
OptionN15,                      //JINJIA//"进价:",
OptionN16,                      //ZHKLVF//"折扣率(%):",
OptionN17,                      //ZDZHIF//"最大值:",
OptionN18,                      //FIXMSG//"折扣率:",
OptionN19,                      //MAXMSG//"最大值:",
OptionN20,                      //HUILVF//"汇率:",
OptionN21,                      //MRJIAF//"买入价:",
OptionN22,                      //MCHJIAF//"卖出价:",
OptionN23,                      //ZHUANGTAIF//"状态:",
OptionN24,                      //QIANXIANGF//"钱箱:",
MSGOTD,                          //OTDMSG
OptionN25,                      //CHFDYINF//"厨房打印:",

MSGPERIOD,                      //PERMSG  PERIOD 周期
MSGPOINTER,                     //POIMSG  POINTER 指针
OptionN26,                      //QIANZHUIFU//"前缀符",,6个字符
OptionN27,                      //LIANJIEF//"连接:",
OptionN28,                      //GNMA//"输入功能码:",
OptionN29,	                    //AGNJIAN//"按功能键:",
OptionN30,                      //LEIXING//"类型:",
OptionN31,                      //KSHRQIF//"开始日期:",
OptionN32,                      //JSHRQIF//"结束日期:",
OptionN33,                      //KSHSHJIANF//"开始时间:"
OptionN34,                      //JSHSHJIANF//"结束时间:"
OptionN35,	                    //XINGQIFF//"星期:",
OptionN36,	                    //ZHEKOUF//"折扣:",
OptionN37,                      //DBSHLIANGF//"大包数量:",
OptionN38,                      //DANJIAF//"单价:",
OptionN39,                      //YHJIAF//"优惠价:",
OptionN40,                      //TXXYIF//"通讯协议:",
OptionN41,                      //DHHMAF//"电话号码:",
OptionN42,	                    //MIMAF//"密码:",
OptionN43,	                    //PINLV//"频率:",
OptionN44,                      //ZXJERF//"最小金额:",
OptionN45,	                    //TXKHAOF//"通讯口:",
OptionN46,                      //"计点金额:",
OptionN47,                      //TXXHAO//"图象序号:",

OptionN48,                      //SHIJIANF//"时间:",
OptionN49,                      //RIQIF//"日期:",

OptionN50,                     //DYJXHAO//"打印型号:",
OptionN51,                     //DZCHXHAO//"秤型号:",

#if(CASE_MACRO5150==1)
WelcomeN1,                      //"创 造 优 秀 优 创 造 就\n    欢迎使用优创收款机" */
#else
WelcomeN2,                      //"创造优秀优创造就\n优创蓝宝石收款机" */
#endif

MessagesN1,			//SHKJSDING//"  收款机已锁定",
MessagesN2,		        //AFSHSUO//"按【方式锁】开机",
MessagesN3,		        //XSHFPIAO//"形式发票",																							//XSHFPIAO
MessagesN4,	                //CHFDYJHAO//"厨房打印机#",																						//CHFDYJHAO
MessagesN5,                     //PXZHTAI//	"*** 培训状态 ***",
MessagesN6,                     //JIADIAN//	"***** 加电 *****",
MessagesN7,		        //FKFZHANG//"*** 分开付帐 ***",
MessagesN8,                     //FZHSHJU//	"*** 复制收据 ***",
MessagesN9,                     //GBPLUJGE//	"*** 商品变价 ***",
MessagesN10,                    //ZJIAKUCUN//	"*** 增加库存 ***",
MessagesN101,                   //JSHAOKUCUN//	"*** 减少库存 ***",
MessagesN11,                    //QUXIAOP//	"***** 取消 *****",
MessagesN12,                    //CHXCHSHHUA//	"** 程序初始化 **",
MessagesN13,			//SELECT1HZ "按<选择>或<确认>"
MessagesN14,			//KPTOTAL 厨房打印汇总


TitleN1,			//TITLE
//品名   数量   单价   金额      /*123456789012345678901234567890123456*/
TitleN3,			//TITLEQTYAMT
TitleN4,			//TITLEAMT

/* ApplVar.Month Captions */
CaptionM1,			//JANUARY //"一月",
CaptionM2,			//FEBRUARY//"二月",
CaptionM3,			//MARCH   //"三月",
CaptionM4,			//APRIL   //"四月",
CaptionM5,			//MAY     //"五月",
CaptionM6,			//JUNE    //"六月",
CaptionM7,			//JULY    //"七月",
CaptionM8,			//AUGUST  //"八月",
CaptionM9,			//SEPTMBER//"九月",
CaptionM10,			//OCTOBER //"十月",
CaptionM11,			//NOVEMBER//"十一月",
CaptionM12,			//DECEMBER//"十二月",

/* WeekDay Captios */
CaptionW1,			//MONDAY  //"星期一",
CaptionW2,			//TUESDAY //"星期二",
CaptionW3,			//WEDNSDAY//"星期三",
CaptionW4,			//THURSDAY//"星期四",
CaptionW5,			//FRIDAY  //"星期五",
CaptionW6,			//SATURDAY//"星期六",
CaptionW7,			//SUNDAY  //"星期日",

ClueN1,			        //AQRJIAN//"按确认键...",

//Option for KP
OptionS1,		        //KPDYZE//"打印总额:",
OptionS2,		        //KPCXTDY//"菜相同打印",
OptionS3,		        //KPCBTDY//"菜不同打印",

//Option for dept
OptionS4,		        //JZHLJGE//"禁止零价格",
OptionS5,		        //DYFGXIAN//"打印分隔线",
OptionS6,		        //DXXSHOU//"单项销售:",
OptionS7,		        //ZHKXIANG1//"折扣项 1:",
OptionS8,		        //ZHKXIANG2//"折扣项 2:",

//Options for tend
OptionS9,		        //DKQXIANGF//"打开钱箱:",
OptionS10,		        //SHRJE	//"输入金额:",
OptionS11,		        //JZHSHRU//"禁止输入:",
OptionS12,		        //SHRHMA//"输入号码:",
OptionS13,		        //ZHLXFEI//"找零为小费",

//Options for PoRa
OptionS14,		        //DKQXIANGP//"打开钱箱:",
OptionS15,		        //FKFSHJIAN//"付款方式键",
OptionS16,		        //CRJE//"存入金额:",
OptionS17,		        //RJCHJIN//"入金/出金",
#if defined(CASE_INDIA)
OptionS12,		        //PORANUMBER// "输入号码:",
#endif

//Option for ApplVar.Disc
OptionS18,		        //DYZHKE//"打印折扣额",
OptionS19,		        //SHYZHK1//"使用折扣1:",
OptionS20,		        //SHYZHK2//"使用折扣2:",

//Options for currency
OptionS21,		        //MCHJZHLIN//"卖出价找零",

//Options for Draw
OptionS22,		        //WXSHDIAN//"无小数点:",
OptionS23,		        //DKQXIANGD//"打开钱箱:",

//Options for Tax
OptionS24,		        //YYZZHI//"营业/增值:",
OptionS25,		        //DYSHLXIANG//"打印税率项",
OptionS26,		        //DYLSHXXI//"打印0税项",
OptionS27,		        //WGSTSHUI//"为GST类税",
OptionS28,		        //DYSHE//"打印税额:",


//Options for clerk
OptionS29,		        //QXXZHI//"取消限制:",
OptionS30,		        //GGXZHI//"更改限制:",
OptionS31,		        //QXJYIXZHI//"取消交易:",
OptionS32,		        //THXZHI//"退货限制:",
OptionS33,		        //BFBZHKXZHI//"%折扣限制:",
OptionS34,		        //BFBJCHXZHI//"%加成限制:",
OptionS35,		        //JJJEXZHI//"+-金额限制",
OptionS36,		        //PSHMSHI//"培训模式:",
OptionS37,		        //mPRNINDEX//"取菜号:",
                        //
MESS_ECR_IP,            //ECRIPADDRESS         "IP Address:"
MESS_ECR_ID,            //"收款机ID号:"   //ECRNETID

EditN1,                         //SHRLJHAO "<记录号>  新纪录\n<确认>  当前纪录",
EditN2,                         //SHRDPDMA "<条码>增删改商品\n<确认>  当前商品\n",

EditN3,                         //TCHBJI//"<退出>      退出",
EditN4,                         //BJDQJLU//"<确认>      继续\n",
EditN5,                         //ANXZJIAN	//"按<选择>报表",
EditN6,				//XFJE//"消费金额",				//XFJE

EditN7,										//AJGNDYI  // "键码     ->键   ",
EditN8,				//JLSHYONG//"只能在经理档用",
EditN9,				//YYSHYONG//"可在营业档用",
EditN10,		        //DYWZI//"打印文字",

EditN11,			//BAOLIU//"保留",
EditN12,	                //SHGNENG//"文件密码测试",


EditN13,			//ZHZBHAO//CCr "拨号",
EditN14,			//MODEMLINK//CCr "MODEM 连接!",
EditN15,			//DAIMA//"代码:",

EditN16,			//PZHEKOU//"%折扣",
EditN17,			//PJIACHEN//"%加成",
EditN18,			//SHJHAOF//"收据号:",

EditN19,			//WENJSHU//	"文件数："，0
EditN20,  		        //WENJKJ//	"文件空间："
EditN21,		  	//TJWENJ// "统计文件"2
EditN22,			//JISHUI// “计税”8
EditN23,			//LIUSKJ
"RAM",                      //RAMKB
EditN24,                        //CONIFOT0    // "文件名      记录数      空间大小"
EditN25,                        //CONIFOT1    // "串口名称    通信类别   通信协议"

EditN26,                        //DBKHSHU//  '顶部空行数', //	DBKHSHU
EditN27,                        //MYEHSHU//  '每页行数',//	MYEHSHU
EditN28,                        //DAYXXI//	'打印信息',//	DAYXXI
EditN29,                        //DEZPJU			//	'第二张票据', //	DEZPJU
EditN30,                        //ZUOBJU			//	'左边距',//	ZUOBJU
EditN31,                        //PDJIXING	//	'票打机型',//	PDJIXING


EditN32,		        //DYJGUAN//"打印机关",
EditN33,		        //DYJKAI//"打印机开",


//ChipCard>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
ChipCardN1,                     //ICSZMIMA//"密码:",
ChipCardN2,                     //ICSZMIMAF//"密码确认:",
ChipCardN3,                     //POSDAIMA//"POS代码:",
ChipCardN4,                     //POSDIAMAF//"代码确认:",
ChipCardN5,                     //BCDMA//"代码已保存",
ChipCardN6,                     //DMACUO//"代码错 ！",
ChipCardN7,                     //ICCA//"启用IC卡:",
ChipCardN8,                     //ZHEKOUCA//"折扣卡:",
ChipCardN9,                     //XIANJINKA//"现金卡:",
ChipCardN10,                    //SHEZHANGKA//"赊帐卡:",
ChipCardN11,                    //DAYINICKA//"打印IC卡",
ChipCardN12,                    //ZDZHEKOU//"自动折扣:",
ChipCardN13,                    //XFJIFEN//"消费积分:",
ChipCardN14,                    //XJJIAN//"用现金键:",
ChipCardN15,                    // QYXYQI	"启用有效期", //
ChipCardN16,                    //GUASHIIC//"挂失卡号",      //ICCard Part ccr chipcard 2004-06-28
ChipCardN17,			//KAHAO//	"卡号:",
ChipCardN18,			//KLXING//	"卡类型:",
ChipCardN19,		        //KNJE 	//hf"余额"
ChipCardN20,			//KYJIN//	"卡押金:",
ChipCardN21,	                //XFZE//	"消费:",
ChipCardN22,			//CHZHZE//	"充值总额:",
ChipCardN23,			//SHYCSHU//	"使用次数:",
ChipCardN24,			//JGLBIE//	"价格级别:",
ChipCardN25,			//PINMA//	"PIN码:",
ChipCardN26,			//BHJBIE//	"保护级别:",
ChipCardN27,			//ZDJZHANG//	"自动结帐:",
ChipCardN28,			//ZHKRQI//	"制卡日期:",
ChipCardN29,			//KYXQI//	"卡有效期:",
ChipCardN30,			//KHMCHEN//	"客户名称:",
ChipCardN31,			//CHSHHICKA//ccr	"初始化IC",
ChipCardN32,			//ICKCHZHI//ccr	"IC卡充值",
ChipCardN33,			//QCHICKA//ccr	"清除IC卡",
ChipCardN34,		        //CHZHTKUAN//ccr	"充值/退款?",
ChipCardN35,		        //JDSDIAN//ccr	"兑奖/送分?",
ChipCardN36,			//QRCHZHI//"确认充值?",
ChipCardN37,			//QRTKUAN//"确认退款?",
ChipCardN38,			//QRSDIAN//"确认送点?",
ChipCardN39,			//DJQREN//"兑奖确认?",
ChipCardN40,			//XFJDDJIANG//"积分兑奖",
ChipCardN41,			//ICKTKUAN//"IC卡退款",
ChipCardN42,			//ICKDJDIANG//"兑奖分数",
ChipCardN43,			//ICKSDIAN//"兑送分数:",
ChipCardN44,		        //QDDAI//"请等待......",
ChipCardN45,		        //KNYJE//"卡内有金额,请先退款....",
ChipCardN46,			//TMHYHAO//"会员号:",
ChipCardN47,            //BARREPORT// 会 员 消 费
ChipCardN48,			//CHXKJI//"要启用新的IP,请重新开机."
ChipCardN49,			//ZQBBIAO//"确认清报表?",
ChipCardN50,			//WAITICARD//"等待IC卡....",
ChipCardN51,                    //BGDYIN "倍高打印",

//ccr2015-06-15>>>>>>>>>>>>>>>>>>>>>
ChipCardN52,             //CLEARIC "清卡",
ChipCardN53,             //REFUNDtoIC "退款",
ChipCardN54,             //PAYMENTbyIC "付款",
ChipCardN55,             //CHARGEtoIC "充值",
ChipCardN56,             //INITIALIC   "初始化",
ChipCardN57,             //TAKECAHSfrIC "取现"
ChipCardN58,            //ADDVIPPOINTS "奖励积分"
ChipCardN59,            //DECVIPPOINTS "减分兑奖"
ChipCardN60,            //VIPPOINTS "累计积分"
//ccr2015-06-15<<<<<<<<<<<<<<<<<<<<<

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
MESSEJ_X_STORE  ,       //"SAVE X DATA"//MsgEJ_X_STORE
MESSEJ_Z_STORE  ,       //"SAVE Z DATA"//MsgEJ_Z_STORE
MESSEJ_TR_STORE ,       //"SAVE TRAIN"//MsgEJ_TR_STORE
#endif

//ccr2016-02-22>>>>>>>>>>>>>>>>>>>>>
MsgMENUXREPORT    ,     //MENUXREPORT    "打印X报表"    "X REPORT"
#if (CASE_USBDISK)
MsgMENUXEXPORT    ,     //MENUXEXPORT    "导出报表数据" "EXPORT(X)"
#endif
MsgMENUQUICKVIEW  ,     //MENUQUICKVIEW  "调阅报表"     "VIEW REPORT"
MsgMENUXLOGFUNC    ,     //MENUXEJFUNC    "调阅流水帐"   "VIEW LOG"
#if defined(CASE_GPRS)
MsgMENUSENDNEWLOG,      //MENUSENDNEWLOG  //发送新流水数据
#endif
MsgMENUXPASSWORD  ,     //MENUXPASSWORD  "设置X密码"    "X-PASSWORD"
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
MsgMENUXEJOURNAL  ,     //MENUXEJOURNAL   "E Journal"
#endif
//.......................................................

MsgMENUZREPORT    ,     //MENUZREPORT    "打印Z报表"    "Z REPORT"
#if (CASE_USBDISK)
MsgMENUZEXPORT    ,     //MENUZEXPORT    "导出报表数据" "EXPORT(Z)"
#endif
MsgMENUCLEARLOG    ,     //MENUCLEARLOG    "清除流水帐"   "CLEAR LOG"
MsgMENUZGRAND     ,     //MENUZGRAND     "清除总账选择" "GRAND TOTAL"
MsgMENUZPASSWORD  ,     //MENUZPASSWORD  "设置Z密码"    "Z-PASSWORD"
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
MsgMENUZRESETEJ  ,     //MENUZRESETEJ   "RESET EJ"
#endif
//...................................

#if defined(DEBUGBYPC)
XZReportList1,                  //		{3, "销售日报表"},
XZReportList2,                  //		{4, "销售月报表"},
XZReportList3,                  //		{5, "商品报表"},
XZReportList4,                  //		{5, "商品报表"},
XZReportList5,                  //		{7, "时段报表"},
XZReportList6,                  //		{1, "收款员日报表"},
XZReportList7,                  //		{2, "收款员月报表"},
XZReportList8,                  //		{8, "全收款员日报表"},
XZReportList9,                  //		{9, "全收款员月报表"},
#if (salNumber)
XZReportList10,                 //		{10,"营业员日报表"},
XZReportList11,                 //		{11,"营业员月报表"},
XZReportList12,                 //		{12,"全营业员日报表"},
XZReportList13,                 //		{13,"全营业员月报表"},
#endif
#if (BARCUSTOMER==1)
XZReportList14,                 //		{14,"条目会员报表"},
#endif
#else
XZTitle[0].Name,	        //XZTITLE
XZTitle[1].Name,	        //XZTITLE
XZTitle[2].Name,	        //XZTITLE
XZTitle[3].Name,	        //XZTITLE
XZTitle[4].Name,	        //XZTITLE
XZTitle[5].Name,	        //XZTITLE
XZTitle[6].Name,	        //XZTITLE
XZTitle[7].Name,	        //XZTITLE
XZTitle[8].Name,	        //XZTITLE
#if (salNumber)
XZTitle[9].Name,	        //XZTITLE
XZTitle[10].Name,	        //XZTITLE
XZTitle[11].Name,	        //XZTITLE
XZTitle[12].Name,	        //XZTITLE
#endif
#if (BARCUSTOMER==1)
XZTitle[13].Name,	        //XZTITLE
#endif
#endif

#if (CASE_EPOS)
EPosN1,                         //ePOSTAC //cc epos//cc "ePOS认证码:",
EPosN2,			        //WAITEPOS"等待易通卡"
EPosN3,			        //ePOSCARNO//cc "易通卡号:",
EPosN4,	                        //eCARREMAIN//cc "易通卡余额:",
#endif

//ccr090715>>>> captions for USB Applications >>>>>>>>>>>>>>>>
#if (CASE_USBDISK)//SETUP

usbMAIN1,     // usbDAOCHU	导出收款机文件     //
usbMAIN2,     // usbDAORU 导入收款机文件     //
usbMAIN3,     // usbFLOW 导出销售流水     //
// ------------

usbALLFile,     	// usbALL 全部文件     //
SetupCaption2,     //  usbBULEI     //  "部类",
SetupCaption3,     //  usbDANPIN     //  "单品",
SetupCaption20,     //  usbSHUILV     //  "税率",
SetupCaption24,     //  usbPIAOTOU     //  "票头",
SetupCaption14,     //  usbZHEKOU     //  "折扣",
#if (salNumber)
SetupCaption39,     //  usbYYYUAN     //  "营业员",
#endif
SysSettings1,     	//  usbSTSHZHI     //  "系统设置",
SetupCaption5,     //  usbTXKOU1     //  "通讯口1",
SetupCaption28,     //  usbTUXIN     //  "图形打印",
SetupCaption1,     //  usbGUIZU     //  "柜组",
DRAWER6,     		//  usbICKA     //  "IC卡",
SetupCaption13,     //  usbZHJIANG     //  "中奖设置",
SetupMode8,     	//  usbCANYINGN     //  "餐饮功能",
SetupCaption38,     //  usbSHKYUAN     //  "收款员",
CaptionE31,     	//  usbSHUOMIN     //  "说明文字",
SetupCaption34,     //  usbZHUOTAI     //  "桌台",
SetupCaption31,     //  usbSHIDUANG     //  "时段",
SysSettings4,     	//  usbJIANPAN     //  "键盘",
OffRec1,     		//  usbTSHCHJIA     //  "特殊出价",
SetupCaption15,     //  usbCHJRJIN     //  "出金入金",
SetupCaption25,     //  usbPIAOWEI     //  "票尾",
SetupCaption26,     //  usbPIAOJTOU    // "票据票头",
SetupCaption22,     //  usbICCAGUASHI     //  "IC卡挂失",

usbFLOWNEWMESS,     //usbFLOWNEW    导出新数据   //
usbFLOWALLMESS,     //usbFLOWALL    导出全部数据
usbFLOWDELMESS,     //usbFLOWALLDEL 导出全部数据后清空

//<<<<<<<<<<<<<<<
#endif

DEPTREPORTTITLE,       //HDEPTREPORT   //zengy "部名称     数量     金额 "
PLUREPORTTITLE,        //HPLUREPORT    //zengy "单品名称     数量    金额  "
MESS_PRESSKEY,         //PRESSANYKEY
SysFlags21,            //PRICEBARCODE //ccr2016-01-22

SysFlags32,             // HEADCENTER "票头居中打印"
SysFlags33,             // TRAILCENTER "票尾居中打印"
SysFlags34,             // HEADDBLHIGH "票头倍高打印"
SysFlags35,             // TRAILDBLHIGH "票尾倍高打印"
//ccr2016-02-17>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//经理档功能定义...................
MGADDINV    ,//MsgMGADDINV     "增加单品库存"
MGDECINV    ,//MsgMGDECINV     "减少单品库存"
MGSETPRICE  ,//MsgMGSETPRICE   "修改单品价格"
MGCLEARIC   ,//MsgMGCLEARIC    "会员退卡"
MGINITIC    ,//MsgMGINITIC     "初始化会员卡"
MGCHARGE    ,//MsgMGCHARGE     "会员卡充值"
MGADDPOINT  ,//MsgMGADDPOINT   "会员兑送积分"
MGPRINTSTOCK ,//MsgMGPRINTSTOCK   "打印单品库存"
MGPASSWORD  ,//MsgMGPASSWORD    "设置经理密码"
MGCLOSEPRN  ,//MsgMGCLOSEPRN    "关闭打印"
#if (!defined(DD_LOCK) || !defined(TR))
MGTRAINING  ,//MsgMGTRAINING    "培训模式"
#endif
MGGOTOMG    ,//MsgMGGOTOMG      "经理档"

//ccr2016-03-09>>>>流水查询打印>>>>>>>
MESSALLLOG       ,//MsgALLLOG        "全部收据"
MESSDATERANGE   ,//MsgDATERANGE    "日期范围"
MESSVIPLOG,     //MsgVIPLOG        "会员消费记录"
MESSPRNCASH     ,//MsgPRNCASH      "打印付款记录"
MESSPRNDEPART   ,//MsgPRNDEPART    "打印部类记录"
MESSPRNPLU      ,//MsgPRNPLU       "打印单品记录"
MESSPRNDISCOUNT ,//MsgPRNDISCOUNT  "打印折扣加成"
MESSPRNRAPO     ,//MsgPRNRAPO      "打印出入金"
MESSPRNCORRECT  ,//MsgPRNCORRECT   "打印更正操作"
MESSPRNCANCEL   ,//MsgPRNCANCEL    "打印取消交易"
MESSPRNNOSALE   ,//MsgPRNNOSALE    "打印非销售"
                 //
MESSALLCASH     ,//MsgALLCASH      "全部付款方式"
MESSSELCASH     ,//MsgSELCASH      "选择付款方式"
MESSALLDEPT     ,//MsgALLDEPT      "打印全部部类"
MESSSELDEPT     ,//MsgSELDEPT      "选择一个部类"
MESSALLPLU      ,//MsgALLPLU       "打印全部单品"
MESSSELPLU      ,//MsgSELPLU       "选择一个单品"

PLUNUMBER,       //MsgPLUNUMBER    "记录数:"
PLURANDOMSIZE   ,//MsgPLURANDOMSIZE    	"编码位数:"
PLUNAMESIZE		,//MsgPLUNAMESIZE    	    "名称长度:"
PLUPRICESIZE    ,//MsgPLUPRICESIZE       "价格位数:"
PLUPRICELEVEL   ,//MsgPLUPRICELEVEL      "会员价级别:"
PLUCOSTSIZE     ,//MsgPLUCOSTSIZE        "进价位数:"
PLUINVENTSIZE   ,//MsgPLUINVENTSIZE      "库存位数:"
PLUREPQTYSIZE   ,//MsgPLUREPQTYSIZE      "报表数量位数:"
PLUREPAMTSIZE   ,//MsgPLUREPAMTSIZE      "报表金额位数:"
PLUREPDISCSIZE  ,//MsgPLUREPDISCSIZE     "统计折扣位数:"
PLUREPCOSTSIZE  ,//MsgPLUREPCOSTSIZE     "统计进价位数:"
//ccr2016-02-17<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
DATETYPE ,        //MsgDATETYPE"FORMAT "
DDMMYYYY ,        //MsgDDMMYYYY"DDMMYYYY"
MMDDYYYY ,        //MsgMMDDYYYY"MMDDYYYY"
YYYYMMDD ,        //MsgYYYYMMDD"YYYYMMDD"

//ccr2016-04-05>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
POINTTYPE1          ,//MsgPOINTTYPE1 "1.000,00"  //两位小数
POINTTYPE2          ,//MsgPOINTTYPE2 "1,000.00"  //两位小数
POINTTYPE3          ,//MsgPOINTTYPE3 "1.000"     //无小数
POINTTYPE4          ,//MsgPOINTTYPE4 "1,000"     //无小数
#if !defined(CASE_FORHANZI)
POINTTYPE5          ,//MsgPOINTTYPE5 "1.000,000"  //三位小数
POINTTYPE6          ,//MsgPOINTTYPE6 "1,000.000"  //三位小数
//POINTTYPE7          ,//MsgPOINTTYPE7 "1.000,0"  //一位小数
//POINTTYPE8          ,//MsgPOINTTYPE8 "1,000.0"  //一位小数
#endif
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
MESSEJMESSAGES,  //MsgEJMESSAGES   //"EJ存储信息"
MESSEJALLEJ     ,//MsgEJALLEJ      //"打印全部EJ"
MESSEJDATERANGE ,//MsgEJDATERANGE  // "日期范围EJ"
MESSEJRECEIPT   ,//MsgEJRECEIPT    // "打印收据EJ"
MESSEJXREPORT   ,//MsgEJXREPORT    // "打印X-EJ"
MESSEJZREPORT   ,//MsgEJZREPORT    // "打印Z-EJ"
MESSEJCLERK     ,//MsgEJCLERK      //"打印收款员EJ"
#endif

#if defined(CASE_QRCODE)
InputQRCode,           //MsgQRCODE "二维码"
#endif

GPRSServerIP,			//GPRSSERVERIP
GPRSServerPort,			//GPRSSERVERPORT


#if(defined(CASE_GPRS))
//ccr2014-11-11 NEWSETUP>>>>>GPRS 功能菜单 >>>>>>>>>>
GPRSAPNName,           //GPRSAPNNAME
GPRSUserName,           //GPRSUSERNAME
GPRSPassword,           //GPRSPASSWORD

GPRSSetMode,        //gprsSETMODE //ccr2014-11-11 NEWSETUP-step2
GPRSSendECRLogAll,        //gprsSENDECRLOGAll //ccr2014-11-11 NEWSETUP-step2
GPRSDownloadPLU   ,     //  "下载单品"    //gprsDownloadPLU //ccr2014-11-11 NEWSETUP-step2
GPRSDownloadDEPT  ,     //  "下载部类"    //gprsDownloadDEPT //ccr2014-11-11 NEWSETUP-step2
GPRSDownloadCLERK ,     //  "下载收款员"    //gprsDownloadCLERK //ccr2014-11-11 NEWSETUP-step2
GPRSDownloadHEAD  ,     //  "下载票头"       //gprsDownloadHEAD //ccr2014-11-11 NEWSETUP-step2
GPRSDownloadTRAIL ,     //  "下载票尾"      //gprsDownloadTRAIL //ccr2014-11-11 NEWSETUP-step2
GPRSDownloadALL ,     //  "下载全部数据"      //gprsDownloadALL //ccr2016-08-18
GPRSRestart,            //  "复位GPRS模块"      //gprsRestart //ccr2016-08-26
GPRSSendMESS,			//gprsSENDMESS
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#endif
0
};
//add by lqw 090802
#if(defined(CASE_GPRS))
CONST GPRSSTR GPRSMess[G_GPRSMessMax]={
	GPRSstr1,						//G_xDATATYPE 0            ccr"数据类型错"
	GPRSstr2,						//G_GPRSOK                 ccr"GPRS READY"
	GPRSstr3,						//G_GPRSNOTOK              ccr"GPRS 出错"
	GPRSstr4,						//G_CONFIRM                ccr"请确认"
	GPRSstr5,						//G_SERVERIP_NULL          ccr"服务器IP为空"
	GPRSstr6,						//G_X_IPPORT               ccr"服务器IP错"
	GPRSstr7,						//G_WAITFOR                ccr"等待GPRS"
	GPRSstr10,						//G_SHUJUFASONGZHONG       ccr"数据发送中.."
	GPRSstr11,						//G_SHUJUYIFACHU           "数据已发送.."
	GPRSstr28,                      //G_FASONGCHENGGONG        "发送成功.."
	GPRSstr31,                      //G_LIANJIESERVERSHIBAI    "连接服务器失败"
	GPRSstr32,                      //G_JIESHOUSHIBAI          "接收失败"
	GPRSstr34,                      //G_FASONGSHIBAI           "发送失败"
	GPRSServerIP,                   //G_SERVERIP
	GPRSServerPort,                 //G_SERVERPORT
	GPRSstr58,                      //G_CONNECTING              "正在建立连接.."
	GPRSstr59,                      //G_TOCONNECTING            "准备建立连接.."
	GPRSstr60,                      //G_FUWEIMOKUAI             "正在复位模块.."
	GPRSstr61                       //G_CHENGGONGTUICHU         "发送完毕,请退出.."
};
#endif
//----------字符串常数------------------------------------------------------
const char
            cMsg_NOSTOCK[]=Msg_NOSTOCK,  //无库存数据
            cMsg_DISPSTOCK[]=Msg_DISPSTOCK,//显示/打印?
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
            cMsg_RECNUMFR[]= Msg_RECNUMFR	,//"Rec_FROM"                  //开始收据号	//RECNUMFR
            cMsg_RECNUMTO[]= Msg_RECNUMTO	,//"Rec_TO"                    //结束收据号	//RECNUMTO
            cMsg_EJBBIAO[]= Msg_EJBBIAO	,//	"~E~J ~R~E~P~O~R~T"		//EJ 报表			//EJBBIAO
            cMsg_EJXINXI[]= Msg_EJXINXI	,//	"EJ INFORMATIONS"            //EJ信息			//EJXINXI
            cMsg_EJBHAO[]= Msg_EJBHAO		,//"EJ MEMORY ID"						//EJ编号			//EJBHAO
            cMsg_SIZEEJ[]= Msg_SIZEEJ		,//"Size of LOG"				//SIZEEJ
            cMsg_EJLEFT[]=Msg_EJLEFT,        //剩余空间
            cMsg_EJCSHUA[]= Msg_EJCSHUA	,//	"EJ INITIALISATION:"		//EJ初始化		//EJCSHUA
            cMsg_FORMAT[] = Msg_FORMAT,//      "FAST FORMAT"
            cMsg_EJDATEEND[]= Msg_EJDATEEND,// "DATE LAST EJ"
            cMsg_EJRECORDS[]= Msg_EJRECORDS,// "LAST EJ#"

#endif//ccr2016-10-25<<<<<<<<
            cMESS_PAYALSO[]=MESS_PAYALSO,
            cMESSSETMENU[]=MESSSETMENU,
            cDEPTTITLE[]=DEPTTITLE,
            cPLULIST[]=PLULIST,
            cPLUTITLE1[]=PLUTITLE1,
            cPLUTITLE2[]=PLUTITLE2,
            cMESSPLUPRINTEND[]=MESSPLUPRINTEND,
            cMESS_TABLEHOLD[]=MESS_TABLEHOLD,
            cMESSPRNMENU[]=MESSPRNMENU,
            cPRTStyleN1[]=PRTStyleN1,
            cPRTStyleN2[]=PRTStyleN2,
            cPRTStyleN3[]=PRTStyleN3,
            cPRTStyleN4[]=PRTStyleN4,
            cComTestN1[]=ComTestN1,
            cComTestN2[]=ComTestN2,
            cComTestN3[]=ComTestN3,
            cComTestN4[]=ComTestN4,
            cCurrentTimeN1[]=CurrentTimeN1,
            cMESSCONFIGERR[]=MESSCONFIGERR,
            cMESSCLEANFILES[]=MESSCLEANFILES,
            cMESSRAMOUT[]=MESSRAMOUT,
            cMESSDELPLU[]=MESSDELPLU,
            cMESSCLEANLOG[]=MESSCLEANLOG,
            cMESSCLEANLOGA[]=MESSCLEANLOGA,
            cSpecialSymbol3[]=SpecialSymbol3,
            cRECALLFLOW[]=RECALLFLOW,
            cErrInfoN1[]=ErrInfoN1,
            cErrInfoN3[]=ErrInfoN3,
            cMessageE18[]=MessageE18,
            cEXPORTCSV[]=EXPORTCSV,
			cDEPTCAP[]=DEPTCAP,
			cPLUCAP[]=PLUCAP     ,
			cTAXCAP[]=TAXCAP     ,
			cHEADERCAP[]=HEADERCAP  ,
			cDISCCAP[]=DISCCAP    ,
			cSALPERCAP[]=SALPERCAP  ,
			cSYSTEMCAP[]=SYSTEMCAP  ,
			cCOMMCAP[]=COMMCAP    ,
			cGRAPPRNCAP[]=GRAPPRNCAP ,
			cGROUPCAP[]=GROUPCAP   ,
			cICCARDCAP[]=ICCARDCAP  ,
			cPROMCAP[]=PROMCAP    ,
			cPBFUNCCAP[]=PBFUNCCAP  ,
			cCLERKCAP[]=CLERKCAP   ,
			cMODICAP[]=MODICAP    ,
			cPBTOTALCAP[]=PBTOTALCAP ,
			cZONECAP[]=ZONECAP    ,
			cKEYBOARDCAP[]=KEYBOARDCAP,
			cOFFCAP[]=OFFCAP     ,
			cPORACAP[]=PORACAP    ,
			cTRAILERCAP[]=TRAILERCAP ,
			cSLIPHEADCAP[]=SLIPHEADCAP,
			cICBLOCKCAP[]=ICBLOCKCAP ,
            cUSBREPNAME[]=USBREPNAME,
            cSetupCaption38[]=SetupCaption38,
            cSetupCaption39[]=SetupCaption39,
            cCSVREPTITL[]=CSVREPTITL,
            cReportType0[]=ReportType0,
            cReportType1[]=ReportType1,
            cReportType2[]=ReportType2,
            cReportType3[]=ReportType3,
            cReportType7[]=ReportType7,
            cReportType8[]=ReportType8,
            cReportType13[]=ReportType13,
            cReportType17[]=ReportType17,
            cReportType18[]=ReportType18,
#if defined(CASE_FORHANZI)
            cInputMode1[]=InputMode1,
            cInputMode2[]=InputMode2,
            cInputMode3[]=InputMode3,
            cInputMode4[]=InputMode4,
#endif
            cBUTTONBATLOW[]=BUTTONBATLOW,
            cCHARGEBATNOW[]=CHARGEBATNOW,
            cREPRN_PWON[]=REPRN_PWON,
#if (NUMPORT==3)
            cComTestN3ERR[]=ComTestN3ERR,
            cComTestN3OK[]=ComTestN3OK,
#endif
            cSRAMNOTFOUND[]=SRAMNOTFOUND,
            cCLERKPASS[]=CLERKPASS,
            cDEPTLIST[]=DEPTLIST,
            cCLEARGRANDTOTAL[]=CLEARGRANDTOTAL,
            cMsgNET[]=MsgNET,
            cCLOSEPRINTER[]=CLOSEPRINTER,//    "关闭打印机"
            cCLEANALLECRVIP[]= CLEANALLECRVIP,//  "CLEAN ECR-VIP?"//"清除会员数据?"
            cECRVIPTITLE[]=ECRVIPTITLE,//"会员号      消费金额     计点"
            cCREATEDATE[]=CREATEDATE,//创建日期:
            cVIPLOGOUT[]=VIPLOGOUT, // 无此会员
            cVIPTOTAL[]=VIPTOTAL,// 总额
            cVIPTOTALA[]=VIPTOTALA,// 消费总额
            cDECVIPPOINT[]=DECVIPPOINT,//兑奖
            cADDVIPPOINT[]=ADDVIPPOINT,//送分
            cINPUTVIPNAME[]=INPUTVIPNAME,//"输入会员姓名:"
#if defined(CASE_PERU)
            cMsgCUSTOMER[]=MsgCUSTOMER,//     "CUSTOMER"
#endif
            cPRINTKEYBOARD[]=PRINTKEYBOARD;//打印键盘布局
//---------------------------------------------------------------------------

#if (defined(DEBUGBYPC))

#include "exthead.h"
#include "exth.h"

void PrintMsg(char* msgName,int idx)
{
    char sBuf[225];
    static int selfIDX;

    if (idx==0) selfIDX=0;
    if (idx!=selfIDX)
        sprintf(sBuf,">>>>>%s:\t%d",msgName,idx);
    else
    {
        selfIDX++;
        if (idx<HZMESSMAX)
            sprintf(sBuf,"%d:%s\t---->\t%s",idx,msgName,Msg[idx].str);
        else
            sprintf(sBuf,"%d:%s\t---->\t%s",idx,msgName,"XXXXXXXX");
    }
    RJPrint(0,sBuf);

}

void PrintChnMess()
{
// MSg[]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
PrintMsg(("LING"),			0);				//WAIT

PrintMsg(("SHOUKUAN"),(LING+1));		//营业
PrintMsg(("XBBIAO"),(SHOUKUAN+1));	//读帐
PrintMsg(("ZBBIAO"),(XBBIAO+1));		//清帐
PrintMsg(("SHEZHI"),(ZBBIAO+1));		//设置
PrintMsg(("JINLI"),(SHEZHI+1));		//经理

//vvvvvvvvvvvvvvvv设置档菜单相关的信息描述vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//设置档各级菜单的项目数存储在SubMenuItems中

/* 设置档一级菜单(主菜单) */
PrintMsg(("MENUFILES"),(JINLI+1));		//资料管理
PrintMsg(("ZHGNSZ"),(MENUFILES+1));	//系统设置
PrintMsg(("DEFKEYBOARD"),(ZHGNSZ+1));	    //键盘定义
PrintMsg(("PJYSSZ"),(DEFKEYBOARD+1));	//票头票尾
PrintMsg(("COMMHOST"),(PJYSSZ+1));      //通讯
PrintMsg(("TXKYWS"),(COMMHOST+1));    //外接设备
PrintMsg(("CYGNSZ"),(TXKYWS+1));		//餐饮设置
PrintMsg(("BBGNSZ"),(CYGNSZ+1));		//报表管理
PrintMsg(("MENUDISC"),(BBGNSZ+1));      //折扣促销
PrintMsg(("JYGLSZ"),(MENUDISC+1));	//销售功能设置
PrintMsg(("ICKGLI"),(JYGLSZ+1));		//IC卡管理
#if !defined(DD_LOCK)
PrintMsg(("QXSZHI"),(ICKGLI+1));		//密码权限
#else
PrintMsg(("QXSZHI"),(ICKGLI));		//密码权限
#endif
PrintMsg(("MENUSERVICE"),(QXSZHI+1));		//维护(SERVICE)
//用于菜单管理,Msg[]中无信息>>>>>>>>......
//PrintMsg(("MENUID_FILES"),(1)		//资料管理
//PrintMsg(("MENUID_SYS"),(MENUID_FILES+1));	//系统设置
//PrintMsg(("MENUID_DEFKB"),(MENUID_SYS+1));	    //键盘定义
//PrintMsg(("MENUID_HEAD"),(MENUID_DEFKB+1));	//票头票尾
//PrintMsg(("MENUID_COMM"),(MENUID_HEAD+1));      //通讯
//PrintMsg(("MENUID_DEV"),(MENUID_COMM+1));    //外接设备
//PrintMsg(("MENUID_RES"),(MENUID_DEV+1));		//餐饮设置
//PrintMsg(("MENUID_REP"),(MENUID_RES+1));		//报表管理
//PrintMsg(("MENUID_DISC"),(MENUID_REP+1));      //折扣促销
//PrintMsg(("MENUID_OP"),(MENUID_DISC+1));	//销售功能设置
//PrintMsg(("MENUID_VIP"),(MENUID_OP+1));		//IC卡管理
//#if !defined(DD_LOCK)
//PrintMsg(("MENUID_PWD"),(MENUID_VIP+1));		//密码权限
//#else
//PrintMsg(("MENUID_PWD"),(MENUID_VIP)		//密码权限
//#endif
//PrintMsg(("MENUID_SERVICE"),(MENUID_PWD+1));		//维护(SERVICE)
//PrintMsg(("SUBMENTMAX"),(MENUSERVICE-MENUFILES+1));
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<........

//设置档二级菜单>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//    PrintMsg(("SETIDXFROM"),(MENUSERVICE+1));           //设置项目的开始序号

    /* ZHGNSZ下的综合功能设置菜单 */
    PrintMsg(("SETSYSFLAG"),      SETIDXFROM);        //系统参数设置
    PrintMsg(("SETPOINTTYPE"),(SETSYSFLAG+1));    //设置小数类型
    PrintMsg(("SETDATE"),(SETPOINTTYPE+1));	  //设置日期
    PrintMsg(("SETTIME "),(SETDATE+1));  	  //设置时间
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    PrintMsg(("SETEJOURNAL"),(SETTIME+1));  	  //设置EJ
#endif

    /* DEFKEYBOARD下的综合功能设置菜单 */
    PrintMsg(("SETKEYB   "),(SETEJOURNAL+1)); 	  //设置键盘
    PrintMsg(("SETKEYMACRO"),(SETKEYB+1));         //  MESSKEYMACRO            "键盘宏"
    PrintMsg(("SETKEYTYPE"),(SETKEYMACRO+1));     //  MESSKEYTYPE             "键盘模式"
    PrintMsg(("SETPRINTKEYB"),(SETKEYTYPE+1));     //  MESSKEYTYPE             "键盘模式"
//    PrintMsg(("MENUKEYBMAX"),(SETKEYTYPE-SETKEYB+1));		/* ccr2014-11-11 NEWSETUP Step-3 total items of sys menu */

    /* 键盘模式选择　*/
    PrintMsg(("SETREST   "),(SETPRINTKEYB+1));		    //启用餐饮键盘
    PrintMsg(("SETMARKET"),(SETREST+1));		        //启用超市键盘
//    PrintMsg(("KEYTYPEMAX"),(SETMARKET-SETREST+1));   //键盘模式选择

    PrintMsg(("SETGROUP   "),(SETMARKET+1));	  //柜组 //ccr2014-11-11 NEWSETUP Step-3<<<<<<
    PrintMsg(("SETSALER   "),(SETGROUP+1));	    //营业员

    /* SPGLSZ下的商品管理菜单 */
    PrintMsg(("SETPLU   "),(SETSALER+1));		 //单品
    PrintMsg(("SETDEPT   "),(SETPLU+1)); 	 //部类
#if !defined(CASE_FORHANZI)
    PrintMsg(("SETTAX   "),(SETDEPT+1)); 	    //税率
    PrintMsg(("SETCLERK   "),(SETTAX+1));	    //收款员
#else
    PrintMsg(("SETCLERK   "),(SETDEPT+1));	    //收款员
#endif
//    PrintMsg(("MENUPLUMAX"),(SETCLERK-SETPLU+1));	/* total items of plu manages */

    /* COMMHOST下的通讯菜单 */
    PrintMsg(("SETCOMPUTER"),(MENUPLUMAX+SETPLU));	    //"计算机"
    PrintMsg(("SETIP"),(SETCOMPUTER+1));        //IP地址
    //ccr2014-11-11 NEWSETUP Step-3>>>>>>
    #if defined(CASE_GPRS)
    PrintMsg(("SETGPRS"),(SETIP+1));
    #else
    //PrintMsg(("SETGPRS         SETIP
    #endif

    /* TXKYWS下的外设菜单 */
    PrintMsg(("SETBARCODE   "),(SETGPRS+1));        //"条码枪"
    PrintMsg(("SETBALANCE   "),(SETBARCODE+1));        //"电子秤"
    //ccr2016-01-22PrintMsg(("SETPORT4   "),(SETBALANCE+1));        //通讯口4
    PrintMsg(("SETSP"),(SETBALANCE+1));		    //票据打印
    PrintMsg(("SETKP"),(SETSP+1));           //厨房打印
    PrintMsg(("SETEXTPRINTER"),(SETKP+1));          //中奖设置


    PrintMsg(("SETPROM"),(SETEXTPRINTER+1));          //中奖设置

    /* MENUDISC下的折扣促销功能设置*/
    PrintMsg(("SETOFF"),(SETPROM+1));           //优惠价
    PrintMsg(("SETDISC   "),(SETOFF+1));		    //折扣

    /* JYGLSZ下的销售功能设置*/
    PrintMsg(("SETTEND   "),(SETDISC+1));		    //付款方式
    PrintMsg(("SETCORR   "),(SETTEND+1));		    //更正功能
    PrintMsg(("SETDRAWER"),(SETCORR+1));		    //钱箱
    PrintMsg(("SETPORA   "),(SETDRAWER+1));		    //出金入金
    PrintMsg(("SETCURR   "),(SETPORA+1));		    //外币
//    PrintMsg(("MENUMANMAX"),(SETCURR-SETTEND+1));		/* total items of comsume manages */

    /* ICKGLI下的IC卡管理*/
    PrintMsg(("SETVIPIC"),(SETCURR+1));          //IC卡设置
    PrintMsg(("SETADDNEWVIP"),(SETVIPIC+1));          //"ADD NEW VIP"
    PrintMsg(("SETPRINTVIP"),(SETADDNEWVIP+1));          //"PRINT VIP"
    PrintMsg(("SETCLEANVIP"),(SETPRINTVIP+1));          //"CLEAN VIP"

    PrintMsg(("SETBLOCKIC"),(SETCLEANVIP+1));           //IC卡挂失
    PrintMsg(("SETPRINTIC"),(SETBLOCKIC+1));	    //打印卡信息
//    PrintMsg(("MENUICCMAX"),(SETPRINTIC-SETVIPIC+1));		/* total items of Chip Card */

    /* PJYSSZ下的票头票尾*/
    PrintMsg(("SETHEAD   "),(SETPRINTIC+1));	    //收据票头
    PrintMsg(("SETTRAIL   "),(SETHEAD+1));         //收据票尾
    PrintMsg(("SETSHEAD   "),(SETTRAIL+1)); 	    //票据票头
    PrintMsg(("SETGRAP"),(SETSHEAD+1));	    //票尾图片
    PrintMsg(("SETPRTGRAP"),(SETGRAP+1));		    //打印图片
//    PrintMsg(("MENUTICMAX"),(SETPRTGRAP-SETHEAD+1));		/* total items of tictes */

    /* BBGNSZ下的报表功能*/
    PrintMsg(("SETREPORT"),(SETPRTGRAP+1));	    //设置报表内容
    PrintMsg(("SETREPTYPE"),(SETREPORT+1));	    //报表单元
    PrintMsg(("SETZONES   "),(SETREPTYPE+1));	    //报表时段设置
    PrintMsg(("SETCLRREP"),(SETZONES+1));	    //总清报表
    PrintMsg(("SETPRTJOUR "),(SETCLRREP+1));	    //打印流水
//    PrintMsg(("MENUREPMAX"),(SETPRTJOUR-SETREPORT+1));		/* total items of report */

    /* CYGNSZ下的餐饮功能*/
    PrintMsg(("SETPBINF   "),(SETPRTJOUR+1));	    //桌台
    PrintMsg(("SETPBF   "),(SETPBINF+1));	    //餐饮功能
    PrintMsg(("SETMODIF   "),(SETPBF+1)); 	 	    //菜单说明
#if (agreeNumber)
    PrintMsg(("SETAGREE"),(SETMODIF+1));        //签单
#endif
//    PrintMsg(("MENUTBLMAX"),(SETMODIF-SETPBINF+1));		/* total items of table function */

    /* QXSZHI下的密码权限设置*/
    #if !defined(DD_LOCK)
    PrintMsg(("SETSETPWD"),(SETMODIF+1));	    //设置档密码
    PrintMsg(("SETXPWD   "),(SETSETPWD+1));	    //只读报表密码
    PrintMsg(("SETZPWD   "),(SETXPWD+1));		    //清帐报表密码
    #endif
    PrintMsg(("SETMGPWD   "),(SETMGPWD));	    //经理档密码
//    PrintMsg(("MENUPWDMAX"),(SETMGPWD-SETSETPWD+1));		/* total items of password setup */

    /* MENUSERVICE下的系统测试功能 */
    PrintMsg(("SETTEST"),(SETMGPWD+1));      //系统测试
    PrintMsg(("SETCLEARPLU"),(SETTEST+1));      //清除单品
    PrintMsg(("SETPLULIST"),(SETCLEARPLU+1));		 //打印商品清单
    PrintMsg(("SETTSTVER"),(SETPLULIST+1));         //打印版本
    PrintMsg(("SETCONFIGPLU"),(SETTSTVER+1));      //配置单品
    PrintMsg(("SETCONFIGDEPT"),(SETCONFIGPLU+1));              //配置部类
#if CASE_USBDISK
    PrintMsg(("SETINSTALL"),(SETCONFIGDEPT+1));    //安装/备份
    PrintMsg(("SETPRINTFAT"),(SETINSTALL+1));      //打印文件分配表
#else
    PrintMsg(("SETPRINTFAT"),(SETCONFIGDEPT+1));      //打印文件分配表
#endif
    PrintMsg(("SETPRNMENU"),(SETPRINTFAT+1));      //打印设置菜单
//    PrintMsg(("MENUSERVICEMAX"),(SETPRNMENU-SETTEST+1));

    /* SKJCSHI下的系统测试功能 */
    PrintMsg(("SETTSTDISP"),(SETPRNMENU+1));       //显示测试
    PrintMsg(("SETTSTCUST"),(SETTSTDISP+1));       //客显测试
    PrintMsg(("SETTSTPRT"),(SETTSTCUST+1));      //打印测试
    PrintMsg(("SETTSTCLK"),(SETTSTPRT+1));       //时钟测试
    PrintMsg(("SETTSTMEM"),(SETTSTCLK+1));       //蜂鸣器测试
    PrintMsg(("SETTSTBELL"),(SETTSTMEM+1));       //RAM测试
    PrintMsg(("SETTSTDRAW"),(SETTSTBELL+1));      //钱箱测试
    PrintMsg(("SETTSTKBD"),(SETTSTDRAW+1));      //键盘测试
    PrintMsg(("SETTSTCOMM"),(SETTSTKBD+1));       //串口测试
    PrintMsg(("SETTSTAUTO"),(SETTSTCOMM+1));      //自动测试

//    PrintMsg(("MENUTSTMAX"),(SETTSTAUTO-SETTSTDISP+1));		/* total items of test function */
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//    PrintMsg(("SETUPMAX	    SETTSTAUTO	//hf 20060105
    #if (SETUPMAX>254)
    #error "Error on setup IDX" //设置功能号不能>255
    #endif
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//ccr2016-02-04>>>>专用于ListMenu,Msg[]无信息>>>>>>>>>>>>>>>>>>>>>>>
//PrintMsg(("LISTMGFUNC"),(SETUPMAX+1));      //ccr2016-01-15 经理挡下选择小计功能ListItem序号
//PrintMsg(("LISTKEYDES"),(LISTMGFUNC+1));      //ccr2016-02-04 功能码描述ListItem序号
//PrintMsg(("LISTKEYSID"),(LISTKEYDES+1));  //ccr2016-02-04 功能码明细描述ListItem序号
//PrintMsg(("LISTNUMRIC"),(LISTKEYSID+1));  //ccr2016-02-25 数字码列表
//ccr2016-02-04<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

PrintMsg(("JLHAO"),(SETUPMAX+1));	//记录#
PrintMsg(("SHRDMA"),(JLHAO+1));		//记录?
PrintMsg(("ZONGJI"),(SHRDMA+1));		//总计
PrintMsg(("ZHHUAN"),(ZONGJI+1));		//转换
PrintMsg(("XIAOJI"),(ZHHUAN+1));		//小计
PrintMsg(("CHUJIN"),(XIAOJI+1));		//出金
PrintMsg(("RUJIN"),(CHUJIN+1));		//入金
PrintMsg(("BAOBIAO"),(RUJIN+1));		//报表
PrintMsg(("ZENJIA"),(BAOBIAO+1));		//增加
PrintMsg(("GUAQI"),(ZENJIA+1));		//挂单
PrintMsg(("QUXIAO"),(GUAQI+1));		//取消
PrintMsg(("MIMA"),(QUXIAO+1));		//密码
PrintMsg(("MMQREN"),(MIMA+1));		//密码确认
PrintMsg(("DANJIA"),(MMQREN+1));		//单价:
PrintMsg(("JINEF"),(DANJIA+1));		//金额:
PrintMsg(("CHAOSHI"),(JINEF+1));		//超市版
PrintMsg(("CANYIN"),(CHAOSHI+1));		//餐饮版
PrintMsg(("CEISHI"),(CANYIN+1));		//测试
PrintMsg(("RAM"),(CEISHI+1));		//  RAM:
PrintMsg(("YINGFU"),(RAM+1));		//应付
PrintMsg(("JINGE"),(YINGFU+1));		//净额
PrintMsg(("JIAGE"),(JINGE+1));		//价格
PrintMsg(("JIAOYISHU"),(JIAGE+1));		//交易数
PrintMsg(("SHULIANG"),(JIAOYISHU+1));		//数量
PrintMsg(("JINE"),(SHULIANG+1));		//金额
PrintMsg(("SHOUQU"),(JINE+1));		//收取
PrintMsg(("ZHAOLING"),(SHOUQU+1));		//找零
PrintMsg(("RI"),(ZHAOLING+1));		//日
PrintMsg(("ZHOU"),(RI+1));		//周
PrintMsg(("YUE"),(ZHOU+1));		//月
PrintMsg(("NIAN"),(YUE+1));		//年
PrintMsg(("BIAOZHUN"),(NIAN+1));		//标准
PrintMsg(("SHKYUANS"),(BIAOZHUN+1));		//收款员
PrintMsg(("SHI"),(SHKYUANS+1));		//   时
PrintMsg(("RIS"),(SHI+1));		//   日
PrintMsg(("YUES"),(RIS+1));		//   月
PrintMsg(("TUIHUO"),(YUES+1));		//退货
PrintMsg(("CHENBEN"),(TUIHUO+1));		//成本
PrintMsg(("ZUOHAO"),(CHENBEN+1));		//桌号#
PrintMsg(("ZHPHAO"),(ZUOHAO+1));		//支票号#
PrintMsg(("XIANGSHU"),(ZHPHAO+1));		//项数:
PrintMsg(("HSHXSHOU"),(XIANGSHU+1));		//含税销售
PrintMsg(("SHUIE"),(HSHXSHOU+1));		//税额
PrintMsg(("YEHAO"),(SHUIE+1));		//页号
PrintMsg(("HAOMA"),(YEHAO+1));		//号码:
PrintMsg(("GUAZHE"),(HAOMA+1));		//挂帐
PrintMsg(("KAITAI"),(GUAZHE+1));		//开台
PrintMsg(("QZHJSHU"),(KAITAI+1));		//清帐计数
PrintMsg(("QINGCHU"),(QZHJSHU+1));		//清除
PrintMsg(("SBAOBIAO"),(QINGCHU+1));		//S-报表
PrintMsg(("MAOLI"),(SBAOBIAO+1));		//毛利
PrintMsg(("KCZJIA"),(MAOLI+1));		//库存增减
PrintMsg(("KCJIANSHAO"),(KCZJIA+1));		//库存增减
PrintMsg(("YZHZHANG"),(KCJIANSHAO+1));		//原桌帐:
PrintMsg(("XYKHAO"),(YZHZHANG+1));		//信用卡#
PrintMsg(("XZHZHANG"),(XYKHAO+1));		//新桌帐:
PrintMsg(("RENSHU"),(XZHZHANG+1));		//人数
PrintMsg(("HUANHUO"),(RENSHU+1));		//换货
PrintMsg(("ZENJIAA"),(HUANHUO+1));		//增加?
PrintMsg(("SHANCHU"),(ZENJIAA+1));		//删除?ZENJIAA与SHANCHU不能间隔
PrintMsg(("ZHKOU1"),(SHANCHU+1));		//折扣项 1
PrintMsg(("ZHKOU2"),(ZHKOU1+1));		//折扣项 2
PrintMsg(("XSHOUE"),(ZHKOU2+1));		//销售额
PrintMsg(("DBYHUI"),(XSHOUE+1));		//大包优惠
PrintMsg(("KCSHLIANG"),(DBYHUI+1));		//库存数量
PrintMsg(("TJYHUI"),(KCSHLIANG+1));		//特价优惠
PrintMsg(("XUANXIANG"),(TJYHUI+1));		//选项:
PrintMsg(("GDZHKOU"),(XUANXIANG+1));		//固定
PrintMsg(("FDZHKOU1"),(GDZHKOU+1));		//浮动
PrintMsg(("GFZHKOU1"),(FDZHKOU1+1));		//固/浮
PrintMsg(("KTZHJIE"),(GFZHKOU1+1));		//开台暂结
PrintMsg(("KTZJIA"),(KTZHJIE+1));		//打印开台
PrintMsg(("ZHANJIE"),(KTZJIA+1));		//暂结
PrintMsg(("QRJZHANG"),(ZHANJIE+1));		//确认结帐
PrintMsg(("DYKTAI"),(QRJZHANG+1));		//打印开台
PrintMsg(("DYZHDAN"),(DYKTAI+1));		//打印帐单
PrintMsg(("QXQREN"),(DYZHDAN+1));		//取消确认
PrintMsg(("FENZHANG"),(QXQREN+1));		//分帐
PrintMsg(("ZHUANZHANG"),(FENZHANG+1));		//转帐
PrintMsg(("GHSHKYUAN"),(ZHUANZHANG+1));		//更换桌台
PrintMsg(("GKRSHU"),(GHSHKYUAN+1));		//顾客人数
PrintMsg(("ZHJSHDING"),(GKRSHU+1));		//中奖设定:
PrintMsg(("JIFEN"),(ZHJSHDING+1));		//消费积分:
PrintMsg(("GBDPJGE"),(JIFEN+1));		//改变单品价格
PrintMsg(("CHFDYJZU"),(GBDPJGE+1));		//厨房打印机组
PrintMsg(("ZJKCUN"),(CHFDYJZU+1));		//增加库存
PrintMsg(("JSHKCUN"),(ZJKCUN+1));		//减少库存
PrintMsg(("SHURU"),(JSHKCUN+1));		//输入
PrintMsg(("DAIFU"),(SHURU+1));		//待付
PrintMsg(("RS232ERR"),(DAIFU+1));		//RS232错
PrintMsg(("RS232OK"),(RS232ERR+1));		//RS232正确
PrintMsg(("RTCERR"),(RS232OK+1));		//RTC 错!
PrintMsg(("CTCERR"),(RTCERR+1));		//CTC 错!
PrintMsg(("FPROMERR"),(CTCERR+1));		//FPROM错!
PrintMsg(("CHSHHUA"),(FPROMERR+1));		//初始化...
PrintMsg(("SHSHHJSHU"),(CHSHHUA+1));		//初始化结束.
PrintMsg(("CHUCUO"),(SHSHHJSHU+1));		//出错-
PrintMsg(("MMACUO"),(CHUCUO+1));		//密码错 ！
PrintMsg(("BCMMA"),(MMACUO+1));		//密码已保存
PrintMsg(("SHKYUANF"),(BCMMA+1));		//收款员:
PrintMsg(("YYYUANF"),(SHKYUANF+1));		//营业员:
PrintMsg(("SHKJHAO"),(YYYUANF+1));		//收款机号:
PrintMsg(("WEIZHI"),(SHKJHAO+1));		//位置:
PrintMsg(("QUERENF"),(WEIZHI+1));		//确认?
PrintMsg(("WANCHENG"),(QUERENF+1));		//完成
PrintMsg(("FXBBEN"),(WANCHENG+1));		//发行版本:
PrintMsg(("BCXFEI"),(FXBBEN+1));		//本次消费点
PrintMsg(("ZJDPIN"),(BCXFEI+1));		//新增单品:
PrintMsg(("SHCHDP"),(ZJDPIN+1));		//删除单品:
PrintMsg(("KSHJCE"),(SHCHDP+1));		//开始检测>>
PrintMsg(("JCJSHU"),(KSHJCE+1));		//<<检测结束
PrintMsg(("KUCUN"),(JCJSHU+1));		//库存
PrintMsg(("BBQQING"),(KUCUN+1));		//报表已全清!
PrintMsg(("GXBIOS"),(BBQQING+1));		//更新程序切勿断电
PrintMsg(("WJKJYJIE"),(GXBIOS+1));		//文件空间越界
PrintMsg(("JSHDPSHL"),(WJKJYJIE+1));		//内存溢出！
PrintMsg(("RZQMAN"),(JSHDPSHL+1));		//流水存储区满
PrintMsg(("GHDYZHI"),(RZQMAN+1));		//请更换打印纸
PrintMsg(("KZNCCUO"),(GHDYZHI+1));		//扩展内存出错！
PrintMsg(("SHUOMINF"),(KZNCCUO+1));		//说明文字:

PrintMsg(("CWXXI01"),(SHUOMINF+1));		//无效输入!
PrintMsg(("CWXXI02"),(CWXXI01+1));		//无效日期!
PrintMsg(("CWXXI03"),(CWXXI02+1));		//无效时间!
PrintMsg(("CWXXI04"),(CWXXI03+1));		//禁止输入!
PrintMsg(("CWXXI05"),(CWXXI04+1));		//PLU代码为空!
PrintMsg(("CWXXI06"),(CWXXI05+1));		//PLU文件溢出!
PrintMsg(("CWXXI07"),(CWXXI06+1));		//桌台被占用!
PrintMsg(("CWXXI08"),(CWXXI07+1));		//桌台已打开!
PrintMsg(("CWXXI09"),(CWXXI08+1));		//桌台号不对!
PrintMsg(("CWXXI10"),(CWXXI09+1));		//请输入桌台号
PrintMsg(("CWXXI11"),(CWXXI10+1));		//桌台没有开台
PrintMsg(("CWXXI12"),(CWXXI11+1));		//桌台桌帐溢出
PrintMsg(("CWXXI13"),(CWXXI12+1));		//禁止修改时间
PrintMsg(("CWXXI14"),(CWXXI13+1));		//仍在销售中!
PrintMsg(("CWXXI15"),(CWXXI14+1));		//销售缓冲区满
PrintMsg(("CWXXI16"),(CWXXI15+1));		//商品没有售出
PrintMsg(("CWXXI17"),(CWXXI16+1));		//正在结算中!
PrintMsg(("CWXXI18"),(CWXXI17+1));		//输入数据超长
PrintMsg(("CWXXI19"),(CWXXI18+1));		//正在付帐!
PrintMsg(("CWXXI20"),(CWXXI19+1));		//人数溢出!
PrintMsg(("CWXXI21"),(CWXXI20+1));		//没有确认结帐
PrintMsg(("CWXXI22"),(CWXXI21+1));		//禁止取消付款
PrintMsg(("CWXXI23"),(CWXXI22+1));		//无菜名!
PrintMsg(("CWXXI24"),(CWXXI23+1));		//无此收款员!
PrintMsg(("CWXXI25"),(CWXXI24+1));		//收款员改变
PrintMsg(("CWXXI26"),(CWXXI25+1));		//无此类报表!
PrintMsg(("CWXXI27"),(CWXXI26+1));		//报表打印中断
PrintMsg(("CWXXI28"),(CWXXI27+1));		//必须在经理档
PrintMsg(("CWXXI29"),(CWXXI28+1));		//不能分帐时用
PrintMsg(("CWXXI30"),(CWXXI29+1));		//必须输入号码!
PrintMsg(("CWXXI31"),(CWXXI30+1));		//转入到
PrintMsg(("CWXXI32"),(CWXXI31+1));		//未授权!
PrintMsg(("CWXXI33"),(CWXXI32+1));		//请指定收款员
PrintMsg(("CWXXI34"),(CWXXI33+1));		//请指定营业员
PrintMsg(("CWXXI35"),(CWXXI34+1));		//禁止PLU变价！
PrintMsg(("CWXXI36"),(CWXXI35+1));		//密码不对!
PrintMsg(("CWXXI37"),(CWXXI36+1));		//厨房打印机错
PrintMsg(("CWXXI38"),(CWXXI37+1));		//票据打印机错
PrintMsg(("CWXXI39"),(CWXXI38+1));		//打印压杆抬起
PrintMsg(("CWXXI40"),(CWXXI39+1));		//打印纸用完!
PrintMsg(("CWXXI41"),(CWXXI40+1));		//打印温度太高
PrintMsg(("CWXXI42"),(CWXXI41+1));		//按键未定义!
PrintMsg(("CWXXI43"),(CWXXI42+1));		//必须输入金额
PrintMsg(("CWXXI44"),(CWXXI43+1));		//禁止多种付款
PrintMsg(("CWXXI45"),(CWXXI44+1));		//记帐无此功能
PrintMsg(("CWXXI46"),(CWXXI45+1));		//未按小计键!
PrintMsg(("CWXXI47"),(CWXXI46+1));		//正在管理库存
PrintMsg(("CWXXI48"),(CWXXI47+1));		//保存流水错
PrintMsg(("CWXXI49"),(CWXXI48+1));		//MODEM通讯错
PrintMsg(("CWXXI50"),(CWXXI49+1));		//卡操作出错!
PrintMsg(("CWXXI51"),(CWXXI50+1));		//POS代码错!
PrintMsg(("CWXXI52"),(CWXXI51+1));		//卡数据错!
PrintMsg(("CWXXI53"),(CWXXI52+1));		//为过期卡!
PrintMsg(("CWXXI54"),(CWXXI53+1));		//为挂失卡!
PrintMsg(("CWXXI55"),(CWXXI54+1));		//客户类别错!
PrintMsg(("CWXXI56"),(CWXXI55+1));		//为新卡!
PrintMsg(("CWXXI57"),(CWXXI56+1));		//不是购物卡!
PrintMsg(("CWXXI58"),(CWXXI57+1));		//写卡出错!
PrintMsg(("CWXXI59"),(CWXXI58+1));		//卡号不对!
PrintMsg(("CWXXI60"),(CWXXI59+1));		//禁用折扣卡!
PrintMsg(("CWXXI61"),(CWXXI60+1));		//禁用现金卡!
PrintMsg(("CWXXI62"),(CWXXI61+1));		//禁用赊帐卡!
PrintMsg(("CWXXI63"),(CWXXI62+1));		//禁 ApplVar.IC 卡!
PrintMsg(("CWXXI64"),(CWXXI63+1));		//清卡出错!
PrintMsg(("CWXXI65"),(CWXXI64+1));		//数据溢出!
PrintMsg(("CWXXI66"),(CWXXI65+1));		//IC充值出错!
PrintMsg(("CWXXI67"),(CWXXI66+1));		//IC初始化出错
PrintMsg(("CWXXI68"),(CWXXI67+1));		//禁止初始化!
PrintMsg(("CWXXI69"),(CWXXI68+1));		//IC金额不足!
PrintMsg(("CWXXI70"),(CWXXI69+1));		//IC付款出错!
PrintMsg(("CWXXI71"),(CWXXI70+1));		//IP地址错!
PrintMsg(("CWXXI72"),(CWXXI71+1));		//无IC卡!
PrintMsg(("CWXXI73"),(CWXXI72+1));		//已有挂单

PrintMsg(("CWXXI74"),(CWXXI73+1));         //  电池电量低"
PrintMsg(("CWXXI75"),(CWXXI74+1));         //  没激活库存"
PrintMsg(("CWXXI76"),(CWXXI75+1));         // 无U盘!     //
PrintMsg(("CWXXI77"),(CWXXI76+1));         // 打开文件出错     //
PrintMsg(("CWXXI78"),(CWXXI77+1));         // 读文件出错!     //
PrintMsg(("CWXXI79"),(CWXXI78+1));         // 写文件出错!     //
PrintMsg(("CWXXI80"),(CWXXI79+1));         //建新文件出错     //
PrintMsg(("CWXXI81"),(CWXXI80+1));         // 关闭文件出错     //
                                            //GPRS>>>>>
PrintMsg(("CWXXI82    "),(CWXXI81+1));         //必须发送流水数据
PrintMsg(("CWXXI83"),(CWXXI82+1));         //GPRS出错
PrintMsg(("CWXXI84"),(CWXXI83+1));         //发送数据出错
PrintMsg(("CWXXI85"),(CWXXI84+1));         //接收数据出错
PrintMsg(("CWXXI86"),(CWXXI85+1));         //无法连接服务器
PrintMsg(("CWXXI87"),(CWXXI86+1));         //无服务器IP地址
PrintMsg(("CWXXI88"),(CWXXI87+1));         //无SIM卡
PrintMsg(("CWXXI89"),(CWXXI88+1));          //GPRS未就绪
PrintMsg(("CWXXI90"),(CWXXI89+1));          //已到最大数
PrintMsg(("CWXXI91"),(CWXXI90+1));          //无此会员

PrintMsg("CWXXI92",   (CWXXI91 +1));	    //"EJ DATA ERROR"
PrintMsg("CWXXI93",   (CWXXI92 +1));         //"NO SD CARD"
PrintMsg("CWXXI94",   (CWXXI93 +1));         //"EJ R/W ERROR"
PrintMsg("CWXXI95",   (CWXXI94 +1));         //"NO EJ RECORDS"
PrintMsg("CWXXI96",   (CWXXI95 +1));         //"EJ NEARLY FULL"
PrintMsg("CWXXI97",   (CWXXI96 +1));         //"EJ IS FULL"
PrintMsg("CWXXI98",   (CWXXI97 +1));         //"NEW EJ"
PrintMsg("CWXXI99",   (CWXXI98 +1));         //"RESET EJ FIRST"
PrintMsg("CWXXI100",   (CWXXI99 +1));         //"EJ NOT MATCHED"

//PrintMsg(("CWXXIMAX    	CWXXI89

//PrintMsg(("SHPKZCUO		CWXXI72

PrintMsg(("DKQXIANG"),(CWXXIMAX+1));		//打开钱箱
PrintMsg(("YOUGUADAN"),(DKQXIANG+1));	//有挂单
PrintMsg(("SHURUQTYAMT"),(YOUGUADAN+1));	//  输入商品数量和商品价格
PrintMsg("ZHUANRUDAO",(SHURUQTYAMT+1));    //ZHUANRUDAO "转入到"
PrintMsg(("ERRORMSG"),(ZHUANRUDAO+1));	//Error-
PrintMsg(("CURRENCY"),(ERRORMSG+1));	//CUR
PrintMsg(("TOTALMSG"),(CURRENCY+1));	//TOT
PrintMsg(("CHANGE"),(TOTALMSG+1));	//CHG
PrintMsg(("SUBTOTAL"),(CHANGE+1));		//SUB
PrintMsg(("DISCOUNT"),(SUBTOTAL+1));	//Dis
PrintMsg(("PUTOUT"),(DISCOUNT+1));	//PO
PrintMsg(("RECIEVIN"),(PUTOUT+1));		//RA
PrintMsg(("OPENDRAW"),(RECIEVIN+1));	//DRAW
PrintMsg(("CANCEL"),(OPENDRAW+1));	//CANC
PrintMsg(("ADDMODI"),(CANCEL+1));		//ADD
PrintMsg(("TABLENUM"),(ADDMODI+1));		//TABLE
PrintMsg(("SERVICE"),(TABLENUM+1));	//SER
PrintMsg(("CHECKPAID"),(SERVICE+1));		//CPA
PrintMsg(("CHECKCPAID"),(CHECKPAID+1));	//CCP
PrintMsg(("COVERS"),(CHECKCPAID+1));	//COV
PrintMsg(("CORRECT"),(COVERS+1));		//COR
PrintMsg(("REFUND"),(CORRECT+1));		//RET
PrintMsg(("CANCEL1"),(REFUND+1));		//CANCEL 1
PrintMsg(("CANCEL2"),(CANCEL1+1));		//CANCEL 2
PrintMsg(("ZZXZZKU"),(CANCEL2+1));		//正在下载字库
PrintMsg(("ZZXZTXING"),(ZZXZZKU+1));		//正在下载图形
PrintMsg(("DOWNLOAD"),(ZZXZTXING+1));	//★
PrintMsg(("UPLOAD"),(DOWNLOAD+1));	//☆
PrintMsg(("KAI"),(UPLOAD+1));		//是
PrintMsg(("GUAN"),(KAI+1));		    //否
PrintMsg(("MINCHENF"),(GUAN+1));		//名称:
PrintMsg(("BULEIF"),(MINCHENF+1));	//部类:
PrintMsg(("GUIZUF"),(BULEIF+1));		//柜组:
PrintMsg(("HOLD"),(GUIZUF+1));		//HOLD
PrintMsg(("DAYINF"),(HOLD+1));		//打印:
PrintMsg(("XUANXIANGF"),(DAYINF+1));		//选项:
PrintMsg(("LIANJIE"),(XUANXIANGF+1));	//链接:
PrintMsg(("SHUILVF"),(LIANJIE+1));		//税率:
PrintMsg(("WZHBHAOF"),(SHUILVF+1));		//位置编号:
PrintMsg(("JIAGE1"),(WZHBHAOF+1));	//价格1:
PrintMsg(("JIAGE2"),(JIAGE1+1));		//价格2:
PrintMsg(("JIAGE3"),(JIAGE2+1));		//价格3:
PrintMsg(("JIAGE4"),(JIAGE3+1));		//价格4:
PrintMsg(("JINJIA"),(JIAGE4+1));		//进价:
PrintMsg(("ZHKLVF"),(JINJIA+1));		//折扣率(%):
PrintMsg(("ZDZHIF"),(ZHKLVF+1));		//最大值:
PrintMsg(("FIXMSG"),(ZDZHIF+1));		//折扣率:
PrintMsg(("MAXMSG"),(FIXMSG+1));		//最大值:
PrintMsg(("HUILVF"),(MAXMSG+1));		//税率:
PrintMsg(("MRJIAF"),(HUILVF+1));		//买入价:
PrintMsg(("MCHJIAF"),(MRJIAF+1));		//卖出价:
PrintMsg(("ZHUANGTAIF"),(MCHJIAF+1));		//状态:
PrintMsg(("QIANXIANGF"),(ZHUANGTAIF+1));		//钱箱:
PrintMsg(("OTDMSG"),(QIANXIANGF+1));		//OTD
PrintMsg(("CHFDYINF"),(OTDMSG+1));		//厨房打印:

PrintMsg(("POIMSG"),(CHFDYINF+1));		//POI
PrintMsg(("PREMSG"),(POIMSG+1));		//周期:
PrintMsg(("QIANZHUIFU"),(PREMSG+1));		//PER
PrintMsg(("LIANJIEF"),(QIANZHUIFU+1));		//连接:
PrintMsg(("GNMA"),(LIANJIEF+1));		//输入功能码
PrintMsg(("AGNJIAN"),(GNMA+1));		//按功能键:
PrintMsg(("LEIXING"),(AGNJIAN+1));		//类型:
PrintMsg(("KSHRQIF"),(LEIXING+1));		//开始日期:
PrintMsg(("JSHRQIF"),(KSHRQIF+1));		//结束日期:
PrintMsg(("KSHSHJIANF"),(JSHRQIF+1));		//开始时间:
PrintMsg(("JSHSHJIANF"),(KSHSHJIANF+1));		//结束时间:
PrintMsg(("XINGQIFF"),(JSHSHJIANF+1));		//星期:
PrintMsg(("ZHEKOUF"),(XINGQIFF+1));		//折扣:
PrintMsg(("DBSHLIANGF"),(ZHEKOUF+1));		//大包数量:
PrintMsg(("DANJIAF"),(DBSHLIANGF+1));		//单价:
PrintMsg(("YHJIAF"),(DANJIAF+1));		//优惠价:
PrintMsg(("TXXYIF"),(YHJIAF+1));		//通讯协议:
PrintMsg(("DHHMAF"),(TXXYIF+1));		//电话号码:
PrintMsg(("MIMAF"),(DHHMAF+1));		//密码:
PrintMsg(("PINLV"),(MIMAF+1));		//频率:
PrintMsg(("ZXJERF"),(PINLV+1));		//最小金额:
PrintMsg(("TXKHAOF"),(ZXJERF+1));		//通讯口:
PrintMsg(("JDJER"),(TXKHAOF+1));		//计点金额:
PrintMsg(("TXXHAO"),(JDJER+1));		//图象序号:
PrintMsg(("SHIJIANF"),(TXXHAO+1));		//时间:
PrintMsg(("RIQIF"),(SHIJIANF+1));	//日期:
                                        //
PrintMsg(("DYJXHAO"),(RIQIF+1));		//PER
PrintMsg(("DZCHXHAO"),(DYJXHAO+1));		//PER

PrintMsg(("EUTRONMSG"),(DZCHXHAO+1));		//创造优秀优创造就欢迎使用优创收款机
PrintMsg(("SHKJSDING"),(EUTRONMSG+1));	//  收款机已锁定
PrintMsg(("AFSHSUO"),(SHKJSDING+1));	//按【方式锁】开机
PrintMsg(("XSHFPIAO"),(AFSHSUO+1));		//形式发票
PrintMsg(("CHFDYJHAO"),(XSHFPIAO+1));	//厨房打印机#
PrintMsg(("PXZHTAI"),(CHFDYJHAO+1));	//*** 培训状态 ***
PrintMsg(("JIADIAN"),(PXZHTAI+1));		//***** 加电 *****
PrintMsg(("FKFZHANG"),(JIADIAN+1));		//*** 分开付帐 ***
PrintMsg(("FZHSHJU"),(FKFZHANG+1));	//*** 复制收据 ***
PrintMsg(("GBPLUJGE"),(FZHSHJU+1));		//*** 商品变价 ***
PrintMsg(("ZJIAKUCUN"),(GBPLUJGE+1));	//*** 增加减库存 ***
PrintMsg(("JSHAOKUCUN"),(ZJIAKUCUN+1));	//*** 增加减库存 ***
PrintMsg(("QUXIAOP"),(JSHAOKUCUN+1));		//***** 取消 *****
PrintMsg(("CHXCHSHHUA"),(QUXIAOP+1));		//** 程序初始化 **
PrintMsg(("SELECT1HZ"),(CHXCHSHHUA+1));  //"按<选择>或<确认>"
PrintMsg(("KPTOTAL"),(SELECT1HZ+1));	//KPTOTAL
PrintMsg(("TITLE"),(KPTOTAL+1));		//品名      数量     单价     金额
PrintMsg(("TITLEQTYAMT"),(TITLE+1));	    //品名      数量        金额
PrintMsg(("TITLEAMT"),(TITLEQTYAMT+1));	    //品名      数量        金额

PrintMsg(("JANUARY"),(TITLEAMT+1));	//一月
PrintMsg(("FEBRUARY"),(JANUARY+1));		//二月
PrintMsg(("MARCH"),(FEBRUARY+1));	//三月
PrintMsg(("APRIL"),(MARCH+1));		//四月
PrintMsg(("MAY"),(APRIL+1));		//五月
PrintMsg(("JUNE"),(MAY+1));		    //六月
PrintMsg(("JULY"),(JUNE+1));		//七月
PrintMsg(("AUGUST"),(JULY+1));		//八月
PrintMsg(("SEPTMBER"),(AUGUST+1));		//九月
PrintMsg(("OCTOBER"),(SEPTMBER+1));	//十月
PrintMsg(("NOVEMBER"),(OCTOBER+1));		//十一月
PrintMsg(("DECEMBER"),(NOVEMBER+1));	//十二月
PrintMsg(("MONDAY"),(DECEMBER+1));	//星期一
PrintMsg(("TUESDAY"),(MONDAY+1));		//星期二
PrintMsg(("WEDNSDAY"),(TUESDAY+1));		//星期三
PrintMsg(("THURSDAY"),(WEDNSDAY+1));	//星期四
PrintMsg(("FRIDAY"),(THURSDAY+1));	//星期五
PrintMsg(("SATURDAY"),(FRIDAY+1));		//星期六
PrintMsg(("SUNDAY"),(SATURDAY+1));		//星期日
PrintMsg(("AQRJIAN"),(SUNDAY+1));		//按确认键...
PrintMsg(("KPDYZE"),(AQRJIAN+1));		//打印总额:
PrintMsg(("KPCXTDY"),(KPDYZE+1));		//菜相同打印
PrintMsg(("KPCBTDY"),(KPCXTDY+1));		//菜不同打印
PrintMsg(("JZHLJGE"),(KPCBTDY+1));		//禁止零价格
PrintMsg(("DYFGXIAN"),(JZHLJGE+1));		//打印分隔线
PrintMsg(("DXXSHOU"),(DYFGXIAN+1));		//单项销售:
PrintMsg(("ZHKXIANG1"),(DXXSHOU+1));		//折扣项1:
PrintMsg(("ZHKXIANG2"),(ZHKXIANG1+1));		//折扣项2:
PrintMsg(("DKQXIANGF"),(ZHKXIANG2+1));		//打开钱箱:
PrintMsg(("SHRJE"),(DKQXIANGF+1));		//输入金额:
PrintMsg(("JZHSHRU"),(SHRJE+1));		//禁止输入:
PrintMsg(("SHRHMA"),(JZHSHRU+1));		//输入号码:
PrintMsg(("ZHLXFEI"),(SHRHMA+1));		//找零为小费
PrintMsg(("DKQXIANGP"),(ZHLXFEI+1));		//打开钱箱:
PrintMsg(("FKFSHJIAN"),(DKQXIANGP+1));		//付款方式键
PrintMsg(("CRJE"),(FKFSHJIAN+1));		//存入金额:
PrintMsg(("RJCHJIN"),(CRJE+1));		//入金/出金
#if defined(CASE_INDIA)
PrintMsg(("PORANUMBER"),(RJCHJIN+1));		//入金/出金
PrintMsg(("DYZHKE"),(PORANUMBER+1));		//打印折扣额
#else
PrintMsg(("DYZHKE"),(RJCHJIN+1));		//打印折扣额
#endif
PrintMsg(("SHYZHK1"),(DYZHKE+1));		//使用折扣1:
PrintMsg(("SHYZHK2"),(SHYZHK1+1));		//使用折扣2:
PrintMsg(("MCHJZHLIN"),(SHYZHK2+1));		//卖出价找零
PrintMsg(("WXSHDIAN"),(MCHJZHLIN+1));		//无小数点:
PrintMsg(("DKQXIANGD"),(WXSHDIAN+1));		//打开钱箱:
PrintMsg(("YYZZHI"),(DKQXIANGD+1));		//营业/增值:
PrintMsg(("DYSHLXIANG"),(YYZZHI+1));		//打印税率项
PrintMsg(("DYLSHXXI"),(DYSHLXIANG+1));		//打印0税项
PrintMsg(("WGSTSHUI"),(DYLSHXXI+1));		//为GST类税
PrintMsg(("DYSHE"),(WGSTSHUI+1));		//打印税额:
PrintMsg(("QXXZHI"),(DYSHE+1));		//取消限制:
PrintMsg(("GGXZHI"),(QXXZHI+1));		//更改限制:
PrintMsg(("QXJYIXZHI"),(GGXZHI+1));		//取消交易:
PrintMsg(("THXZHI"),(QXJYIXZHI+1));		//退货限制:
PrintMsg(("BFBZHKXZHI"),(THXZHI+1));		//%折扣限制:
PrintMsg(("BFBJCHXZHI"),(BFBZHKXZHI+1));		//%加成限制:
PrintMsg(("JJJEXZHI"),(BFBJCHXZHI+1));		//+-金额限制
PrintMsg(("PSHMSHI"),(JJJEXZHI+1));		//培训模式:
PrintMsg(("mPRNINDEX"),(PSHMSHI+1));         //Ccr "取菜号:",
                                            //
PrintMsg(("ECRIPADDRESS"),(mPRNINDEX+1));       //"IP Address:"
PrintMsg(("ECRNETID"),(ECRIPADDRESS+1));   //"收款机ID号:"   //

PrintMsg(("SHRLJHAO"),(ECRNETID+1));		//<记录号>  新纪录<确认>  当前纪录
PrintMsg(("SHRDPDMA"),(SHRLJHAO+1));		//<条码>增删改商品<确认>  当前商品
PrintMsg(("TCHBJI"),(SHRDPDMA+1));		//<退出>      退出
PrintMsg(("BJDQJLU"),(TCHBJI    +1));		//<确认>      继续
PrintMsg(("ANXZJIAN"),(BJDQJLU+1));		//选择打印报表
PrintMsg(("XFJE"),(ANXZJIAN+1));		//消费金额
PrintMsg(("AJGNDYI"),(XFJE    +1));		//键码     //键
PrintMsg(("JLSHYONG"),(AJGNDYI+1));		//只能在经理档用
PrintMsg(("YYSHYONG"),(JLSHYONG+1));		//可在营业档用
PrintMsg(("DYWZI"),(YYSHYONG+1));		//打印文字
PrintMsg(("BAOLIU"),(DYWZI    +1));		//保留
PrintMsg(("SHGNENG"),(BAOLIU    +1));		//文件密码测试
PrintMsg(("ZHZBHAO"),(SHGNENG+1));		//拨号
PrintMsg(("MODEMLINK"),(ZHZBHAO+1));		//MODEM 连接!
PrintMsg(("DAIMA"),(MODEMLINK+1));		//代码:
PrintMsg(("PZHEKOU"),(DAIMA+1));		//%折扣
PrintMsg(("PJIACHEN"),(PZHEKOU+1));		//%加成
PrintMsg(("SHJHAOF"),(PJIACHEN+1));		//收据号:
PrintMsg(("WENJSHU"),(SHJHAOF+1));		//文件数
PrintMsg(("WENJKJ"),(WENJSHU+1));		//文件空间
PrintMsg(("TJWENJ"),(WENJKJ+1));		//统计文件
PrintMsg(("JISHUI"),(TJWENJ+1));		//计税
PrintMsg(("LIUSKJ"),(JISHUI+1));		//流水空间
PrintMsg(("RAMKB"),(LIUSKJ+1));		//RAM(KB)
PrintMsg(("CONIFOT0"),(RAMKB+1));		//文件名      记录数      空间大小
PrintMsg(("CONIFOT1"),(CONIFOT0+1));		//串口名称    通信类别   通信协议
PrintMsg(("DBKHSHU"),(CONIFOT1+1));		//顶部空行数:
PrintMsg(("MYEHSHU"),(DBKHSHU+1));		//每页行数:
PrintMsg(("DAYXXI"),(MYEHSHU+1));		//打印信息:
PrintMsg(("DEZPJU"),(DAYXXI+1));		//第二章票据:
PrintMsg(("ZUOBJU"),(DEZPJU+1));		//左边距:
PrintMsg(("PDJIXING"),(ZUOBJU+1));		//票打机型:
PrintMsg(("DYJGUAN"),(PDJIXING+1));		//打印机关
PrintMsg(("DYJKAI"),(DYJGUAN+1));		//打印机开
PrintMsg(("ICSZMIMA"),(DYJKAI+1));		//密码:
PrintMsg(("ICSZMIMAF"),(ICSZMIMA+1));		//密码确认:
PrintMsg(("POSDAIMA"),(ICSZMIMAF+1));		//POS代码:
PrintMsg(("POSDIAMAF"),(POSDAIMA+1));		//代码确认:
PrintMsg(("BCDMA"),(POSDIAMAF+1));		//代码已保存
PrintMsg(("DMACUO"),(BCDMA+1));		//代码错 ！
PrintMsg(("ICCA"),(DMACUO+1));		//启用IC卡:
PrintMsg(("ZHEKOUCA"),(ICCA+1));		//折扣卡:
PrintMsg(("XIANJINKA"),(ZHEKOUCA+1));		//现金卡:
PrintMsg(("SHEZHANGKA"),(XIANJINKA+1));		//赊帐卡:
PrintMsg(("DAYINICKA"),(SHEZHANGKA+1));		//打印IC卡
PrintMsg(("ZDZHEKOU"),(DAYINICKA+1));		//自动折扣:
PrintMsg(("XFJIFEN"),(ZDZHEKOU+1));		//消费积分:
PrintMsg(("XJJIAN"),(XFJIFEN+1));		//用现金键:
PrintMsg(("QYXYQI"),(XJJIAN + 1));		//启用有效期
PrintMsg(("GUASHIIC"),(QYXYQI + 1));		//挂失卡
PrintMsg(("KAHAO"),(GUASHIIC+1));		//卡号:
PrintMsg(("KLXING"),(KAHAO+1));		//卡类型:
PrintMsg(("KNJE"),(KLXING+1));		//余额:
PrintMsg(("KYJIN"),(KNJE+1));		//卡押金:
PrintMsg(("XFZE"),(KYJIN+1));		//消费:
PrintMsg(("CHZHZE"),(XFZE+1));		//充值总额:
PrintMsg(("SHYCSHU"),(CHZHZE+1));		//使用次数:
PrintMsg(("JGLBIE"),(SHYCSHU+1));		//价格级别:
PrintMsg(("PINMA"),(JGLBIE+1));		//PIN码:
PrintMsg(("BHJBIE"),(PINMA+1));		//保护级别:
PrintMsg(("ZDJZHANG"),(BHJBIE+1));		//自动结帐:
PrintMsg(("ZHKRQI"),(ZDJZHANG+1));		//制卡日期:
PrintMsg(("KYXQI"),(ZHKRQI+1));		//卡有效期:
PrintMsg(("KHMCHEN"),(KYXQI+1));		//客户:
PrintMsg(("CHSHHICKA"),(KHMCHEN+1));		//初始化IC
PrintMsg(("ICKCHZHI"),(CHSHHICKA+1));		//IC卡充值
PrintMsg(("QCHICKA"),(ICKCHZHI+1));		//清除IC卡
PrintMsg(("CHZHTKUAN"),(QCHICKA+1));		//充值/退款?
PrintMsg(("JDSDIAN"),(CHZHTKUAN+1));		//减点/送点?
PrintMsg(("QRCHZHI"),(JDSDIAN+1));		//确认充值?
PrintMsg(("QRTKUAN"),(QRCHZHI+1));		//确认退款?
PrintMsg(("QRSDIAN"),(QRTKUAN+1));		//确认送点?
PrintMsg(("DJQREN"),(QRSDIAN+1));		//兑奖确认?
PrintMsg(("XFJDDJIANG"),(DJQREN+1));		//计点兑奖
PrintMsg(("ICKTKUAN"),(XFJDDJIANG+1));		//IC卡退款
PrintMsg(("ICKDJDIANG"),(ICKTKUAN+1));		//IC卡兑奖点数
PrintMsg(("ICKSDIAN"),(ICKDJDIANG+1));		//IC卡送点数
PrintMsg(("QDDAI"),(ICKSDIAN+1));		//请等待......
PrintMsg(("KNYJE"),(QDDAI+1));		//请先退款...
PrintMsg(("TMHYHAO"),(KNYJE+1));		//会员号
PrintMsg(("BARREPORT"),(TMHYHAO+1));		//         会 员 消 费 表
PrintMsg(("CHXKJI"),(BARREPORT+1));		//要启用新的IP,请重新开机.
PrintMsg(("ZQBBIAO"),(CHXKJI+1));		//确认清报表?
PrintMsg(("WAITICARD"),(ZQBBIAO+1));		//等待IC卡....
PrintMsg(("BGDYIN"),(WAITICARD+1));		//倍高打印

//ccr2015-06-15>>>>>>>>>>>>>>>>>>>>>
PrintMsg(("CLEARIC"),(BGDYIN+1));                //"清卡",
PrintMsg(("REFUNDtoIC"),(CLEARIC+1));               //"退款",
PrintMsg(("PAYMENTbyIC"),(REFUNDtoIC+1));            //"付款",
PrintMsg(("CHARGEtoIC"),(PAYMENTbyIC+1));           //"充值",
PrintMsg(("INITIALIC"),(CHARGEtoIC+1));            //"初始化",
PrintMsg(("TAKECAHSfrIC"),(INITIALIC+1));             //"取现"
PrintMsg(("ADDVIPPOINTS"),((TAKECAHSfrIC+1)));//ADDVIPPOINTS "奖励积分"
PrintMsg(("DECVIPPOINTS"),((ADDVIPPOINTS+1)));//DECVIPPOINTS "减分兑奖"
PrintMsg(("VIPPOINTS"),((DECVIPPOINTS+1)));//            "VIP POINTS"  //VIPPOINTS
//ccr2015-06-15<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
PrintMsg("MsgEJ_X_STORE ",     (VIPPOINTS+1));//EJ中存储X数据
PrintMsg("MsgEJ_Z_STORE ", (MsgEJ_X_STORE+1));//EJ中存储Z数据
PrintMsg("MsgEJ_TR_STORE", (MsgEJ_Z_STORE+1));//EJ中存储TR数据
#endif

//ccr2016-02-22>>>>>>>>>>>>>>>>>>>>>
//PrintMsg(("MENUOFXMODE"),(TAKECAHSfrIC  +1));
PrintMsg(("MENUXREPORT"),(MsgEJ_TR_STORE  +1));  // 打印X报表
#if(CASE_USBDISK)
PrintMsg(("MENUXEXPORT"),(MENUXREPORT   +1));  //导出报表数据
PrintMsg(("MENUQUICKVIEW"),(MENUXEXPORT   +1));  //Quick View
#else
PrintMsg(("MENUQUICKVIEW"),(MENUXREPORT   +1));  //Quick View
#endif
PrintMsg(("MENUXEJFUNC"),(MENUQUICKVIEW +1));  //LOG. OPERATION
#if defined(CASE_GPRS)
PrintMsg(("MENUSENDNEWLOG"),(MENUXEJFUNC +1));  //发送新流水数据
PrintMsg(("MENUXPASSWORD"),(MENUSENDNEWLOG   +1));  //设置X密码
#else
PrintMsg(("MENUXPASSWORD"),(MENUXEJFUNC   +1));  //设置X密码
#endif
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
PrintMsg(("MENUXEJOURNAL"),(MENUXPASSWORD   +1));  //打印EJ报表
#endif
//PrintMsg(("MENUXITEMMAX"),(MENUXPASSWORD-MENUXREPORT+1));
//.................................
PrintMsg(("MENUZREPORT"),(MENUXEJOURNAL +1));  //打印Z报表
#if (CASE_USBDISK)
PrintMsg(("MENUZEXPORT"),(MENUZREPORT   +1));  //导出报表数据
PrintMsg(("MENUCLEARLOG"),(MENUZEXPORT   +1));  //清除E.J
#else
PrintMsg(("MENUCLEARLOG"),(MENUZREPORT   +1));  //清除E.J
#endif
PrintMsg(("MENUZGRAND"),(MENUCLEARLOG   +1));  //是否清除Grand TOTAL
PrintMsg(("MENUZPASSWORD"),(MENUZGRAND    +1));  //设置Z密码
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
PrintMsg(("MENUXEJOURNAL"),(MENUZPASSWORD   +1));  //清EJ报表
#endif

//PrintMsg(("MENUZITEMMAX"),(MENUZPASSWORD-MENUZREPORT+1));
//...................................

//ccr2016-02-22<<<<<<<<<<<<<<<<<<<<<
//PrintMsg(("XZTITLE"),(MENUZPASSWORD  +1));
PrintMsg(("XZTITLE1"),		XZTITLE);		//销售日报表
PrintMsg(("XZTITLE2"),(XZTITLE1+1));      //销售月报表
PrintMsg(("XZTITLE3"),(XZTITLE2+1));      //商品报表
PrintMsg(("XZTITLE4"),(XZTITLE3+1));      //桌帐报表
PrintMsg(("XZTITLE5"),(XZTITLE4+1));      //时段报表
PrintMsg(("XZTITLE6"),(XZTITLE5+1));      //收款员日报表
PrintMsg(("XZTITLE7"),(XZTITLE6+1));      //收款员月报表
PrintMsg(("XZTITLE8"),(XZTITLE7+1));      //全收款员日报表
PrintMsg(("XZTITLE9"),(XZTITLE8+1));      //全收款员月报表
#if (salNumber)
PrintMsg(("XZTITLE10"),(XZTITLE9+1));      //营业员日报表
PrintMsg(("XZTITLE11"),(XZTITLE10+1));      //营业员月报表
PrintMsg(("XZTITLE12"),(XZTITLE11+1));      //全营业员日报表
PrintMsg(("XZTITLE13"),(XZTITLE12+1));      //全营业员月报表
#else
//PrintMsg(("XZTITLE13"),(XZTITLE9));      //全营业员月报表
#endif
#if (BARCUSTOMER==1)
PrintMsg(("XZTITLE14"),(XZTITLE13+1));      //条码会员报表
//PrintMsg(("XZITEMSMAX"),(XZTITLE14-XZTITLE1+1));
#else
//PrintMsg(("XZITEMSMAX"),(XZTITLE13-XZTITLE1+1));
#endif
//....................................
#if (CASE_EPOS)
PrintMsg(("ePOSTAC"),(XZTITLE1+XZNUM)  //ePOS认证码:
PrintMsg(("WAITEPOS"),(ePOSTAC+1));        //等待易通卡
PrintMsg(("ePOSCARNO"),(WAITEPOS+1));       //易通卡号:
PrintMsg(("eCARREMAIN"),(ePOSCARNO+1));      //易通卡余额:
#else
//PrintMsg(("eCARREMAIN"),(XZTITLE1+XZNUM-1)     //部类名称     交易数量       金额
#endif


#if (CASE_USBDISK)
//ccr090715>>> USB applications >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
PrintMsg(("usbDAOCHU"),(eCARREMAIN+1));		//Ccr导出收款机文件//
PrintMsg(("usbDAORU"),(usbDAOCHU+1));       //Ccr导入收款机文件 //
PrintMsg(("usbFLOW"),(usbDAORU+1));       //Ccr导出销售流水 //

//PrintMsg(("usbMAINITEMS"),(usbFLOW-usbDAOCHU+1));
//;;;;;;;;;;;;;;;;;;;;

PrintMsg(("usbALL"),(usbFLOW+1));         //Ccr全部文件  //
PrintMsg(("usbBULEI"),(usbALL+1));          //Ccr"部类",
PrintMsg(("usbDANPIN"),(usbBULEI+1));         //Ccr"单品",
PrintMsg(("usbSHUILV"),(usbDANPIN+1));        //Ccr"税率",
PrintMsg(("usbPIAOTOU"),(usbSHUILV+1));         //Ccr"票头",
PrintMsg(("usbZHEKOU"),(usbPIAOTOU+1));       //Ccr"折扣",
#if (salNumber)
PrintMsg(("usbYYYUAN"),(usbZHEKOU+1));        //Ccr"营业员",
PrintMsg(("usbSTSHZHI"),(usbYYYUAN+1));         //Ccr"系统设置",
#else
PrintMsg(("usbSTSHZHI"),(usbZHEKOU+1));         //Ccr"系统设置",
#endif
PrintMsg(("usbTXKOU1"),(usbSTSHZHI+1));       //Ccr"通讯口1",
PrintMsg(("usbTUXIN"),(usbTXKOU1+1));       //Ccr"图形打印",
PrintMsg(("usbGUIZU"),(usbTUXIN+1));        //Ccr"柜组",
PrintMsg(("usbICKA"),(usbGUIZU+1));       //Ccr"IC卡",
PrintMsg(("usbZHJIANG"),(usbICKA+1));           //Ccr"中奖设置",
PrintMsg(("usbCANYINGN"),(usbZHJIANG+1));         //Ccr"餐饮功能",
PrintMsg(("usbSHKYUAN"),(usbCANYINGN+1));        //Ccr"收款员",
PrintMsg(("usbSHUOMIN"),(usbSHKYUAN+1));        //Ccr"说明文字",
PrintMsg(("usbZHUOTAI"),(usbSHUOMIN+1));        //Ccr"桌台",
PrintMsg(("usbSHIDUANG"),(usbZHUOTAI+1));         //Ccr"时段",
PrintMsg(("usbJIANPAN"),(usbSHIDUANG+1));       //Ccr"键盘",
PrintMsg(("usbTSHCHJIA"),(usbJIANPAN+1));         //Ccr"特殊出价",
PrintMsg(("usbCHJRJIN"),(usbTSHCHJIA+1));       //Ccr"出金入金",
PrintMsg(("usbPIAOWEI"),(usbCHJRJIN+1));        //Ccr"票尾",
PrintMsg(("usbPIAOJTOU"),(usbPIAOWEI+1));  //Lyq"票据票头",
PrintMsg(("usbICCAGUASHI"),(usbPIAOJTOU+1));    //ccr"IC卡挂失",

//PrintMsg(("usbFILEITEMS"),(usbICCAGUASHI-usbALL+1));
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
PrintMsg(("usbFLOWNEW"),(usbICCAGUASHI+1));   // 导出新数据
PrintMsg(("usbFLOWALL"),(usbFLOWNEW+1));      // 导出全部数据
PrintMsg(("usbFLOWALLDEL"),(usbFLOWALL+1));      // 导出全部数据后清空 //

//PrintMsg(("usbFLOWITEMS"),(usbFLOWALLDEL-usbFLOWNEW+1));
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

PrintMsg(("HDEPTREPORT"),(usbFLOWALLDEL+1));   //部类名称     交易数量       金额
#else
PrintMsg(("HDEPTREPORT"),(eCARREMAIN+1));  //部类名称     交易数量       金额
#endif

PrintMsg(("HPLUREPORT"),(HDEPTREPORT+1));    //商品名称     交易数量       金额
PrintMsg(("PRESSANYKEY"),(HPLUREPORT+1));     //按任意键 MESS_PRESSKEY,         //

PrintMsg(("PRICEBARCODE"),(PRESSANYKEY+1));     //ccr2016-01-22 特殊条码

PrintMsg(("HEADCENTER"),(PRICEBARCODE+1));    //SysFlags32,  "票头居中打印"
PrintMsg(("TRAILCENTER"),(HEADCENTER  +1));    //SysFlags33, "票尾居中打印"
PrintMsg(("HEADDBLHIGH"),(TRAILCENTER +1));    //SysFlags34, "票头倍高打印"
PrintMsg(("TRAILDBLHIGH"),(HEADDBLHIGH +1));    //SysFlags35, "票尾倍高打印"

//ccr2016-02-17>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//经理档功能定义...................
PrintMsg(("MsgMGADDINV"),(TRAILDBLHIGH +1));//   "增加单品库存"
PrintMsg(("MsgMGDECINV"),(MsgMGADDINV  +1));//   "减少单品库存"
PrintMsg(("MsgMGSETPRICE"),(MsgMGDECINV  +1));//   "修改单品价格"
PrintMsg(("MsgMGCLEARIC"),(MsgMGSETPRICE+1));//   "会员退卡"
PrintMsg(("MsgMGINITIC"),(MsgMGCLEARIC +1));//   "初始化会员卡"
PrintMsg(("MsgMGCHARGE"),(MsgMGINITIC  +1));//   "会员卡充值"
PrintMsg(("MsgMGADDPOINT"),(MsgMGCHARGE  +1));//   "会员兑送积分"
PrintMsg(("MsgMGPRINTSTOCK"),(MsgMGADDPOINT+1));//   "打印单品库存"
PrintMsg(("MsgMGPASSWORD"),(MsgMGPRINTSTOCK+1));//    "设置经理密码"
PrintMsg(("MsgMGCLOSEPRN"),(MsgMGPASSWORD+1));//    "关闭打印"
#if (!defined(DD_LOCK) || !defined(TR))
PrintMsg(("MsgMGTRAINING"),(MsgMGCLOSEPRN+1));//    "培训模式"
#endif
PrintMsg(("MsgMGGOTOMG"),(MsgMGTRAINING+1));//    "经理档"
//PrintMsg(("MsgMGMMAX"),(MsgMGGOTOMG-MsgMGADDINV+1));
//ccr2016-03-09>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
PrintMsg(("MsgALLLOG"),(MsgMGGOTOMG  +1));//MESSALLLOG       "全部收据"
PrintMsg(("MsgDATERANGE"),(MsgALLLOG  +1));//MESSDATERANGE   "日期范围"
PrintMsg(("MsgMESSVIPLOG"),(MsgDATERANGE  +1));               //MESSVIPLOG        "会员消费记录"
PrintMsg(("MsgPRNCASH"),(MsgMESSVIPLOG  +1));      //"打印付款记录"
PrintMsg(("MsgPRNDEPART"),(MsgPRNCASH    +1));    //"打印部类记录"
PrintMsg(("MsgPRNPLU"),(MsgPRNDEPART  +1));    //"打印单品记录"
PrintMsg(("MsgPRNDISCOUNT"),(MsgPRNPLU     +1));    //"打印折扣加成"
PrintMsg(("MsgPRNRAPO"),(MsgPRNDISCOUNT+1));    //"打印出入金"
PrintMsg(("MsgPRNCORRECT"),(MsgPRNRAPO    +1));    //"打印更正操作"
PrintMsg(("MsgPRNCANCEL"),(MsgPRNCORRECT +1));  //"打印交易取消"
PrintMsg(("MsgPRNNOSALE"),(MsgPRNCANCEL +1));    //"打印非销售"
//PrintMsg(("MsgPRNLOGMAX"),(MsgPRNNOSALE-MsgALLLOG+1));

PrintMsg(("MsgALLCASH"),(MsgPRNNOSALE  +1));    //"全部付款方式"
PrintMsg(("MsgSELCASH"),(MsgALLCASH	+1));    //"选择付款方式"
                                               //
PrintMsg(("MsgALLDEPT"),(MsgSELCASH	+1));    //"打印全部部类"
PrintMsg(("MsgSELDEPT"),(MsgALLDEPT	+1));    //"选择一个部类"
                                               //
PrintMsg(("MsgALLPLU"),(MsgSELDEPT	+1));    //"打印全部单品"
PrintMsg(("MsgSELPLU"),(MsgALLPLU	    +1));    //"选择一个单品"

PrintMsg(("MsgPLUNUMBER"),(MsgSELPLU	+1));       // "记录数:"
PrintMsg(("MsgPLURANDOMSIZE"),(MsgPLUNUMBER	+1));   // "编码位数:"
PrintMsg(("MsgPLUNAMESIZE"),(MsgPLURANDOMSIZE +1)); // "名称长度:"
PrintMsg(("MsgPLUPRICESIZE"),(MsgPLUNAMESIZE   +1)); // "价格位数:"
PrintMsg(("MsgPLUPRICELEVEL"),(MsgPLUPRICESIZE  +1)); // "会员价级别:"
PrintMsg(("MsgPLUCOSTSIZE"),(MsgPLUPRICELEVEL +1)); // "进价位数:"
PrintMsg(("MsgPLUINVENTSIZE"),(MsgPLUCOSTSIZE   +1)); // "库存位数:"
PrintMsg(("MsgPLUREPQTYSIZE"),(MsgPLUINVENTSIZE +1)); // "统计数量位数:"
PrintMsg(("MsgPLUREPAMTSIZE"),(MsgPLUREPQTYSIZE +1)); // "统计金额位数:"
PrintMsg(("MsgPLUREPDISCSIZE"),(MsgPLUREPAMTSIZE +1)); // "统计折扣位数:"
PrintMsg(("MsgPLUREPCOSTSIZE"),(MsgPLUREPDISCSIZE+1)); // "统计进价位数:"
//ccr2016-03-09<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//.......................................................
PrintMsg(("MsgDATETYPE"),(MsgPLUREPCOSTSIZE+1));  //"FORMAT "
PrintMsg(("MsgDDMMYYYY"),(MsgDATETYPE+1));    //"DDMMYYYY"
PrintMsg(("MsgMMDDYYYY"),(MsgDDMMYYYY+1));    //"MMDDYYYY"
PrintMsg(("MsgYYYYMMDD"),(MsgMMDDYYYY+1));    //"YYYYMMDD"

//ccr2016-04-05>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
PrintMsg(("MsgPOINTTYPE1"),(MsgYYYYMMDD+1));//"1.000,00"  //两位小数
PrintMsg(("MsgPOINTTYPE2"),(MsgPOINTTYPE1+1));// "1,000.00"  //两位小数
PrintMsg(("MsgPOINTTYPE3"),(MsgPOINTTYPE2+1));// "1.000"     //无小数
PrintMsg(("MsgPOINTTYPE4"),(MsgPOINTTYPE3+1));// "1,000"     //无小数
#if !defined(CASE_FORHANZI)
PrintMsg(("MsgPOINTTYPE5"),(MsgPOINTTYPE4+1));// "1.000,000"  //三位小数
PrintMsg(("MsgPOINTTYPE6"),(MsgPOINTTYPE5+1));// "1,000.000"  //三位小数
//PrintMsg(("MsgPOINTTYPE7"),(MsgPOINTTYPE6+1));// "1.000,0"  //一位小数
//PrintMsg(("MsgPOINTTYPE8"),(MsgPOINTTYPE7+1));// "1,000.0"  //一位小数
#endif
//PrintMsg(("MsgPOINTMAX"),(MsgPOINTTYPE4-MsgPOINTTYPE1+1));
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
PrintMsg("MsgEJMESSAGES", (MsgPOINTTYPE6+1)); //"EJ存储信息"    //
PrintMsg("MsgEJALLEJ",   (MsgEJMESSAGES+1));  //"打印全部EJ"
PrintMsg("MsgEJDATERANGE",   (MsgEJALLEJ    +1));// "日期范围EJ"
PrintMsg("MsgEJRECEIPT",   (MsgEJDATERANGE+1));// "打印收据EJ"
PrintMsg("MsgEJXREPORT",   (MsgEJRECEIPT  +1));// "打印X-EJ"
PrintMsg("MsgEJZREPORT",   (MsgEJXREPORT  +1));// "打印Z-EJ"
PrintMsg("MsgEJCLERK",   (MsgEJZREPORT  +1));//"打印收款员EJ"    
#endif

#if defined(CASE_QRCODE)
PrintMsg(("MsgQRCODE"),(MsgEJCLERK+1));//
#endif

PrintMsg(("GPRSSERVERIP"),(MsgQRCODE+1));//
PrintMsg(("GPRSSERVERPORT"),(GPRSSERVERIP+1));

#if (!defined(CASE_GPRS))		// ADD BY LQW 090827
PrintMsg(("HZMESSMAX"),(GPRSSERVERPORT+1));
#else
//ccr2014-11-11  NEWSETUP Step-4 子菜单项目信息编号>>>>>>>>>>>>>>
PrintMsg(("GPRSAPNNAME"),(GPRSSERVERPORT+1));
PrintMsg(("GPRSUSERNAME"),(GPRSAPNNAME+1));
PrintMsg(("GPRSPASSWORD"),(GPRSUSERNAME+1));

PrintMsg(("gprsSETMODE"),(GPRSPASSWORD+1));
PrintMsg(("gprsSendECRLogAll"),(gprsSETMODE+1));

PrintMsg(("gprsDownloadPLU"),(gprsSendECRLogAll+1)); //"下载单品文件"
PrintMsg(("gprsDownloadDEPT"),(gprsDownloadPLU+1));     //"下载部类文件"
PrintMsg(("gprsDownloadCLERK"),(gprsDownloadDEPT+1)); //"下载收款员"
PrintMsg(("gprsDownloadHEAD"),(gprsDownloadCLERK+1)); //"下载票头"
PrintMsg(("gprsDownloadTRAIL"),(gprsDownloadHEAD+1)); //"下载票尾"
PrintMsg(("gprsDownloadALL"),(gprsDownloadTRAIL+1)); //"下载全部数据"
PrintMsg(("gprsRestart"),(gprsDownloadALL+1)); //"下载全部数据"
PrintMsg(("gprsSENDMESS"),(gprsRestart+1));
//PrintMsg(("gprsMAINITEMS"),(8));                 //子菜单项目数

PrintMsg(("HZMESSMAX"),(gprsSENDMESS+1)); //<<<<<<<<
// MSg[]<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
}
#endif


