#ifndef MESSAGEH_CH_D
#define MESSAGEH_CH_D


#define Modifier1             "说明"
#define ClerkRec1             "收款员"
#define GroupRec1             "柜组"
#define DeptRec1              "部类"
#define PLURec1               "商品"
#define SalesPersonRec1       "营业员"
#define OffRec1               "特殊出价"

#define XZReportList1     "销售日报表"
#define XZReportList2     "销售月报表"
#define XZReportList3     "商品报表"
#define XZReportList4     "桌帐报表"
#define XZReportList5     "时段报表"
#define XZReportList6     "收款员日报表"
#define XZReportList7     "收款员月报表"
#define XZReportList8     "全收款员日报表"
#define XZReportList9     "全收款员月报表"
#define XZReportList10    "营业员日报表"
#define XZReportList11    "营业员月报表"
#define XZReportList12    "全营业员日报表"
#define XZReportList13    "全营业员月报表"
#define XZReportList14    "条码会员报表"


#define PortType1               "计算机:"
#if (1)
#define PortType2               "条码枪:"
#else
#define PortType2               "GPRS"
#endif
#define PortType3               "电子秤:"
#define PortType4               "厨打串口"
#define PortType5               "票据串口"
#define PortType6               "射频卡:"
#define PortType7               "E通卡:"
#define PortEXTPRN              "外打串口"//ccr2016-12-14 ExtPrinter串口

#define KClose                  "   关 "



#define SysFlags0		        "店铺编号:"
#define SysFlags1               "收款机号:"
#define SysFlags2               "舍入方式:"
#define SysFlags3               "商品变价:"
#define SysFlags4               "蜂鸣器:"
#define SysFlags5               "强制收款员:"
#define SysFlags6               "改变收款员:"
#define CLERKPASS               "收款员密码:"
#define SysFlags8               "打印营业员:"
#define SysFlags9               "打印收款员:"
#define SysFlags10              "打印收据号:"
#define SysFlags11              "高浓度打印:"
#define SysFlags12              "强制营业员:"
#define SysFlags13              "打印编码:"
#define SysFlags14              "按小计键:"
#define SysFlags15              "打印项目数:"
#define SysFlags16              "打印小票:"
#define SysFlags17              "小票票头:"
#define SysFlags18              "锁定SHIFT:"
#define SysFlags19              "显示时间:"
#define SysFlags20              "复制收据:"
#define SysFlags21              "特殊条码:"
#define SysFlags22              "操作日志:"
#define SysFlags23              "开台打印:"
#define SysFlags24              "打印库存:"
#define SysFlags25              "打印开关:"
#define SysFlags26              "必须开台:"
#define SysFlags27              "保持收据号"
#define SysFlags28              "复制厨打:"
#define SysFlags29              "收据号复位"
#define SysFlags30              "倍高打印:"
#define SysFlags31              "对比度:"
#define SysFlags32              "票头居中:"
#define SysFlags33              "票尾居中:"
#define SysFlags34              "票头倍高:"
#define SysFlags35              "票尾倍高:"
//ccr2016-03-21#define SysFlags36              ".CSV导出:"
#define SysFlags37              "打印日期:"
#define SysFlags38              "打印时间:"
#define SysFlags39              "打印取菜号:"
#define SysFlags40              "HEART BEAT"    //ccr2015-08-03"向服务器发送Heart beat"
#define SysFlags41              "货币符1:"      //ccr2016-08-02
#define SysFlags42              "货币符2:"      //ccr2016-08-02


#define GrapType1               "商店图标:"
#define GrapType2               "中奖图标:"
#define GrapType3               "票尾图像:"
#define GrapType4               "打印文字:"


#define GrapSet1                "中奖序号"
#define GrapSet2                "图像:"
#define GrapSet3                "开始日期:"
#define GrapSet4                "结束日期:"

///======================CHN msg========================//////

#define CurrentMode1            "营业"
#define CurrentMode2            "读帐"
#define CurrentMode3            "清帐"
#define CurrentMode4            "设置"
#define CurrentMode5            "经理"


#define SetupMode1              "系统设置"
#define SetupMode2              "资料管理"
#define SetupKeyboard          "键盘"   //DEFKEYBOARD  //
#define SetupMode3              "外接设备"
#define SetupMode4              "销售功能设置"
#define SetupMode5              "会员管理"
#define SetupMode6              "票头票尾"
#define msgCOMMHOST             "通讯"
#define SetupMode7              "报表管理"
#define SetupMode8              "餐饮设置"
#define msgDISCOUNT             "折扣促销"   //MENUDISC //

#define SetupMode9              "密码设置"
#define SetupMode10             "收款机维护"

#define SysSettings1            "系统参数设置"
#define SysSettings2            "设置日期"
#define SysSettings3            "设置时间"
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define msgSETEJOURNAL              "设置EJ"      //SETEJOURNAL //EJOURNAL
#endif


#define SysSettings4            "键盘定义"
#define MESSKEYMACRO            "键盘宏"
#define MESSKEYTYPE             "键盘模式"

#define SysSettings5            "餐饮键盘"
#define SysSettings6            "超市键盘"

#define SysSettings8            "GPRS通信"  //SETGPRS //ccr2014-11-11 NEWSETUP-step1

#define SetupCaption1           "柜组"
#define SetupCaption2           "部类"
#define SetupCaption3           "单品"

#define SetupCaption4           "打印商品清单"
#define msgSETTEST              "系统测试"            //SETTEST
#define msgSETCLEARPLU          "清除单品"        //SETCLEARPLU
#define msgSETPLULIST           "打印商品清单"      //SETPLULIST
#define msgSETTSTVER	        "打印版本"      //SETTSTVER
#define msgSETCONFIGPLU         "配置单品"          //SETCONFIGPLU
#define msgSETCONFIGDEPT        "配置部类"          //SETCONFIGDEPT
#define msgSETINSTALL           "安装/备份"        //SETINSTALL
#define msgSETPRINTFAT          "打印文件分配表"   //SETPRINTFAT



#define SetupCaption5           "计算机"//ccr2016-01-22"通讯口1"
#define SetupCaption6           "条码枪"//ccr2016-01-22"通讯口2"
#define SetupCaption7           "电子秤"//ccr2016-01-22"通讯口3"
#define SetupCaption8           "通讯口4"
#define SetupCaption9           "网络设置"

#if defined(CASE_QRCODE)
#define InputQRCode           "二维码:"  //MsgQRCODE
#endif

#define GPRSServerIP		"服务器IP"
#define GPRSServerPort		"端口号"

#if (defined(CASE_GPRS))
#define GPRSAPNName        "APN"           //GPRSAPNNAME
#define GPRSUserName        "用户名"           //GPRSUSERNAME
#define GPRSPassword        "密码"           //GPRSPASSWORD

#define GPRSSendMESS        "GPRS通信测试"    //gprsSENDMESS //ccr2014-11-11 NEWSETUP-step2
#define GPRSSetMode         "设置发送方式"    //gprsSETMODE //ccr2014-11-11 NEWSETUP-step2
#define GPRSSendECRLogAll   "发送全部收据"    //gprsSendECRLogAll //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadPLU     "下载单品"    //gprsDownloadPLU //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadDEPT    "下载部类"    //gprsDownloadDEPT //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadCLERK   "下载收款员"    //gprsDownloadCLERK //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadHEAD    "下载票头"       //gprsDownloadHEAD //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadTRAIL   "下载票尾"      //gprsDownloadTRAIL //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadALL     "下载全部数据"      //gprsDownloadALL //ccr2016-08-18
#define GPRSRestart         "复位GPRS"      //gprsRestart //ccr2016-08-26
#endif

#define SetupCaption10          "票据打印"
#define SetupCaption11          "厨房打印"
#define SetupEXTPrinter         "外置打印机"    //SETEXTPRINTER

#define SetupCaption12          "优惠价"
#define SetupCaption13          "中奖设置"
#define SetupCaption14          "折扣"
#define SetupCaption15          "出金入金"
#define SetupCaption16          "更正功能"
#define SetupCaption17          "付款方式"
#define SetupCaption18          "外币"
#define SetupCaption19          "钱箱"
#define SetupCaption20          "税率"

#define SetupCaption21          "VIP设置"
#define SetAddNewECRVIP         "增加新会员"
#define SetPrintECRVIP          "打印会员信息"
#define SetCleanALLVIP          "清除全部会员"
#define SetupCaption22          "IC卡挂失"
#define SetupCaption23          "打印卡信息"

#define SetupCaption24          "收据票头"
#define SetupCaption25          "收据票尾"
#define SetupCaption26          "票打票头"
#define SetupCaption27          "中奖图片"
#define SetupCaption28          "打印图片"

#define SetupCaption29          "设置报表内容"
#define SetupCaption30          "报表单元"
#define SetupCaption31          "报表时段"
#define SetupCaption32          "总清报表"
#define SetupCaption33          "打印流水帐"

#define SetupCaption34          "桌台"
#define SetupCaption35          "餐饮功能"
#define SetupCaption36          "菜单说明"
#define SetupCaption37          "签单"
#define SetupCaption38          "收款员"
#define SetupCaption39          "营业员"
#define SetupCaption40          "设置档密码"
#define SetupCaption41          "经理档密码"
#define SetupCaption42          "只读报表密码"
#define SetupCaption43          "清帐报表密码"

#define SetupCaption44          "内存测试"
#define SetupCaption45          "显示测试"
#define SetupCaption451         "客显测试"
#define SetupCaption46          "打印测试"
#define SetupTestRAM            "RAM测试"        //SETTSTMEM	       //RAM测试
#define SetupCaption47          "时钟测试"
#define SetupCaption48          "蜂鸣器测试"
#define SetupCaption49          "钱箱测试"
#define SetupCaption50          "键盘测试"
#define SetupCaption51          "串口测试"
#define SetupCaption52          "自动测试"

#define msgSETPRNMENU           "打印设置菜单"      //SETPRNMENU   //

#define OperationN1             "记录#"
#define OperationN2             "记录?"

#define OperationN12            "密码:"
#define OperationN13            "确认:"
#define OperationN16            "超市版"
#define OperationN17            "餐饮版"
#define OperationN18            "测试"




#define CaptionN39              "增加单品?"
#define CaptionN40              "删除单品?"
#define CaptionN46              "特价优惠"
#define CaptionN47              "选项:"
#define CaptionN48              "固定"
#define CaptionN49              "浮动"
#define CaptionN50              "固/浮"


#define CaptionN62              "中奖设定:"
#define CaptionN63              "会员积分"

#define CaptionN64              "改变单品价格"
#define CaptionN65              "厨房打印机组"
#define CaptionN66              "增加库存"
#define CaptionN67              "减少库存"
#define CaptionN68              "输入"
#define CaptionN69              "待付"


#define CaptionE1               "RS232错"
#define CaptionE2               "RS232正确"
#define CaptionE3               "RTC 错!"
#define CaptionE4               "CTC 错!"
#define CaptionE5               "FPROM错!"
#define CaptionE6               "初始化..."
#define CaptionE7               "初始化结束."
#define CaptionE8               "出错" //长度<SCREENWD

#define CaptionE9               "密码错 ！"
#define CaptionE10              "密码已保存"
#define CaptionE15              "确认?"
#define CaptionE16              "完成"
#define CaptionE25              "正在更新程序！客\n显闪烁时切勿断电"
#define CaptionE26              "文件空间越界"
#define CaptionE27              "内存溢出！"
#define CaptionE28              "流水帐存储满"
#define CaptionE29              "请更换打印纸"
#define CaptionE30              "扩展内存出错！"


//CWXXI01 >>>>>>>长度<SCREENWD
#define CaptionE32              "无效输入!"
#define CaptionE33              "无效日期!"
#define CaptionE34              "无效时间!"
#define CaptionE35              "禁止输入!"
#define CaptionE36              "PLU代码为空!"
#define CaptionE37              "PLU文件溢出!"
#define CaptionE38              "桌台被占用!"
#define CaptionE39              "桌台已打开!"
#define CaptionE40              "桌台号不对!"
#define CaptionE41              "请输入桌台号!"
#define CaptionE42              "请先开台!"
#define CaptionE43              "桌台桌帐溢出!"
#define CaptionE44              "禁止修改时间!"
#define CaptionE45              "仍在销售中!"
#define CaptionE46              "销售缓存区满!"
#define CaptionE47              "商品没有销售!"
#define CaptionE48              "正在结算中!"
#define CaptionE49              "输入数据超长!"
#define CaptionE50              "正在付帐!"
#define CaptionE51              "人数溢出!"
#define CaptionE52              "没有确认结帐!"
#define CaptionE53              "禁止取消付款!"
#define CaptionE54              "无菜名!"
#define CaptionE55              "无此收款员!"
#define CaptionE56              "收款员改变!"
#define CaptionE57              "无此类报表!"
#define CaptionE58              "报表打印中断!"
#define CaptionE59              "必须在经理档!"
#define CaptionE60              "不能分帐时用!"
#define CaptionE61              "必须输入号码!"
#define CaptionE62              "钥匙档位错"
#define CaptionE63              "未授权!"
#define CaptionE64              "请指定收款员!"
#define CaptionE65              "请指定营业员!"
#define CaptionE66              "禁止PLU变价！"
#define CaptionE67              "密码不对!"
#define CaptionE68              "厨房打印机错!"
#define CaptionE69              "打印机错!" //ccr2017-08-31
#define CaptionE70              "打印压杆抬起!"
#define CaptionE71              "打印纸用完!"
#define CaptionE72              "打印温度太高!"
#define CaptionE73              "按键未定义!"
#define CaptionE74              "必须输入金额!"/* zero price not allowed enter amount */
#define CaptionE75              "禁止多种付款!"
#define CaptionE76              "记帐无此功能!"
#define CaptionE77              "未按小计键!"
#define CaptionE78              "正在管理库存!"
#define CaptionE79              "保存流水帐错!"
#define CaptionE80              "MODEM通讯错!"

#define CaptionE81              "卡操作出错!"
#define CaptionE82              "POS代码错!"
#define CaptionE83              "卡数据错!"
#define CaptionE84              "为过期卡!"
#define CaptionE85              "为挂失卡!"
#define CaptionE86              "客户类别错!"
#define CaptionE87              "为新卡!"
#define CaptionE88              "不是购物卡!"
#define CaptionE89              "写卡出错!"
#define CaptionE90              "卡号不对!"
#define CaptionE91              "禁用折扣卡!"
#define CaptionE92              "禁用现金卡!"
#define CaptionE93              "禁用赊帐卡!"
#define CaptionE94              "禁用IC卡!"
#define CaptionE95              "清卡出错!"
#define CaptionE96              "数据溢出!"
#define CaptionE97              "IC充值出错!"
#define CaptionE98              "IC初始化出错!"
#define CaptionE99              "禁止初始化!"
#define CaptionE100             "IC金额不足!"
#define CaptionE101             "IC付款出错!"
#define CaptionE102             "IP地址错!"
#define CaptionE103             "无IC卡!"
#define CaptionE105		        "已有挂单"

#define CaptionERR105             "电池电量低!"  // "电池电量低",
#define CaptionERR106             "未开启库存!"	//20070806"没激活库存"
//ccr090715>>>>>>>>>>>>>>>>>>>>>>
#define CaptionERR76			  "请插入U盘!"	//
#define CaptionERR77			  "打开文件错!"		//
#define CaptionERR78			  "读U盘出错!"		//
#define CaptionERR79			  "写U盘出错!"		//
#define CaptionERR80			  "创建文件错!"		//
#define CaptionERR81			  "关闭文件错!"		//
#define CaptionERR82			  "须发送流水!"		//
#define CaptionERR83              "GPRS出错!"
#define CaptionERR84              "发送出错!"
#define CaptionERR85              "接收数据错!"
#define CaptionERR86              "无服务器!"
#define CaptionERR87              "服务器配置错!"
#define CaptionERR88              "无SIM卡!"
#define CaptionERR89              "GPRS未就绪!"
#define CaptionERR90              "记录数溢出" //CWXXI90  //
#define CaptionERR91              "无此会员!"  //CWXXI91

//ccr2016-10-24 EJ >>>>>>>>>>>>>>>>>>>
#define EJ_CWXXI92  "EJ DATA ERROR"   	//CWXXI92
#define EJ_CWXXI93  "NO SD CARD"        	//CWXXI93
#define EJ_CWXXI94  "EJ R/W ERROR"   	//CWXXI94
#define EJ_CWXXI95  "NO EJ RECORDS"   	//CWXXI95
#define EJ_CWXXI96  "EJ NEARLY FULL"   	//CWXXI96
#define EJ_CWXXI97  "EJ IS FULL"   	//CWXXI97
#define EJ_CWXXI98  "NEW EJ"        	//CWXXI98
#define EJ_CWXXI99  "RESET EJ FIRST"   	//CWXXI99
#define EJ_CWXXI100 "EJ NOT MATCHED"     	//CWXXI100

//CWXXI <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#define CaptionE104             "打开钱箱"
#define CaptionE106             "有挂单"
#define CaptionE107             "  输入商品数量\n  和商品价格"
#define CaptionE108             "转入到"    //ZHUANRUDAO

#define MsgPUTOUT       "出金"   	                //PUTOUT
#define MsgRECIEVIN     "入金" 		            //RECIEVIN


#define DownLoad1               "下载字库"
#define DownLoad2               "下载图形"

#define OptionN1                "是"
#define OptionN2                "否"
#define OptionN3                "名称:"
#define OptionN4                "部类:"
#define OptionN5                "柜组:"
#define OptionN6                "打印:"
#define OptionN7                "选项:"
#define OptionN8                "链接:"
#define OptionN9                "税率:"
#define OptionN10               "位置编号:"

#define OptionN11               "价格1:"
#define OptionN12               "价格2:"
#define OptionN13               "价格3:"
#define OptionN14               "价格4:"

#define OptionN15               "进价:"
#define OptionN16               "折扣率(%):"
#define OptionN17               "最大值:"
#define OptionN18               "折扣率:"
#define OptionN19               "最大值:"
#define OptionN20               "税率:"
#define OptionN21               "买入价:"
#define OptionN22               "卖出价:"
#define OptionN23               "状态:"
#define OptionN24               "钱箱:"
#define MSGOTD                  "OTD"                    //OTDMSG
#define OptionN25               "厨房打印"
#define MSGPERIOD               "周期:"                      //PERMSG
#define MSGPOINTER              "指针:"                     //POIMSG

#define OptionN26               "货币符"  // "货币符",6个字符,
#define OptionN27               "连接:"  // "连接:",
#define OptionN28               "功能:"  // "输入功能码:",
#define OptionN29               "按设定键:"  // "按功能键:",
#define OptionN30               "类型:"  // "类型:",
#define OptionN31               "开始日期:"  // "开始日期:",
#define OptionN32               "结束日期:"  // "结束日期:",
#define OptionN33               "开始时间:"  // "开始时间:",
#define OptionN34               "结束时间:"  // "结束时间:",
#define OptionN35               "星期:"  // "星期:",
#define OptionN36               "折扣:"  // "折扣:",
#define OptionN37               "大包数量:"  // "大包数量:",
#define OptionN38               "单价:"  // "单价:",
#define OptionN39               "优惠价:"  // "优惠价:",
#define OptionN40               "通讯协议:"  // "通讯协议:",
#define OptionN41               "电话号码:"  // "电话号码:",
#define OptionN42               "密码:"  // "密码:",
#define OptionN43               "频率:"  // "频率:",
#define OptionN44               "最小金额:"  // "最小金额:",
#define OptionN45               "通讯口:"  // "通讯口:",
#define OptionN46               "计点金额:"  // "计点金额:",
#define OptionN47               "图象序号:"  // "图象序号:",
#define OptionN48               "时间:"  // "时间:",
#define OptionN49               "日期:"  // "日期:",
#define OptionN50               "打印机:"
#define OptionN51               "秤型号:"      //DZCHXHAO//"秤型号:",

//#define WelcomeN1                "创造优秀优创造就\n优创MACRO收款机!"
#define WelcomeN1                "优创MACRO收款机!"
#define WelcomeN2                "创造优秀优创造就\n欢迎使用优创 ECR"

#define MessagesN1                "收款机已锁定"  // "  收款机已锁定",
#define MessagesN2                "按【方式锁】开机"  // "按【方式锁】开机",
#define MessagesN3                "形式发票"  // "形式发票",																							//XSHFPIAO
#define MessagesN4                "厨房打印机#"  // "厨房打印机#",																						//CHFDYJHAO
#define MessagesN13               "按<选择>或<确认>"  // "按<选择>或<确认>",
#define MessagesN14               "总计"  //


#define CaptionM1                 "一月"  // "一月",
#define CaptionM2                 "二月"  // "二月",
#define CaptionM3                 "三月"  //  "三月",
#define CaptionM4                 "四月"  //  "四月",
#define CaptionM5                 "五月"  // "五月",
#define CaptionM6                 "六月"  // "六月",
#define CaptionM7                 "七月"  // "七月",
#define CaptionM8                 "八月"  // "八月",
#define CaptionM9                 "九月"  // "九月",
#define CaptionM10                "十月"  // "十月",
#define CaptionM11                "十一月"  // "十一月",
#define CaptionM12                "十二月"  // "十二月",

#define CaptionW1                 "星期一"  // "星期一",			//MONDAY
#define CaptionW2                 "星期二"  // "星期二",			//TUESDAY
#define CaptionW3                 "星期三"  // "星期三",			//WEDNSDAY
#define CaptionW4                 "星期四"  // "星期四",			//THURSDAY
#define CaptionW5                 "星期五"  // "星期五",			//FRIDAY
#define CaptionW6                 "星期六"  // "星期六",			//SATURDAY
#define CaptionW7                 "星期日"  // "星期日",			//SUNDAY

#define ClueN1                    "按确认键..."  // "按确认键...",

  //Option for KP
#define OptionS1                  "打印总额:"  // "打印总额:",
#define OptionS2                  "菜合并打印"  // "菜相同打印",
#define OptionS3                  "菜分类打印"  // "菜不同打印",
  //Option for dept
#define OptionS4                  "禁止零价格"  // "禁止零价格",
#define OptionS5                  "打印分隔线"  // "打印分隔线",
#define OptionS6                  "单项销售:"  // "单项销售:",
#define OptionS7                  "折扣项1:"  // "折扣项 1:",
#define OptionS8                  "折扣项2:"  // "折扣项 2:",
  //Options for tend
#define OptionS9                  "打开钱箱:"  // "打开钱箱:",
#define OptionS10                 "必须输金额:"  // "输入金额:",
#define OptionS11                 "禁止输金额:"  // "禁止输入:",
#define OptionS12                 "必须输号码:"  // "输入号码:",
#define OptionS13                 "找零为小费:"  // "找零为小费",
  //Options for PoRa
#define OptionS14                 "打开钱箱:"  // "打开钱箱:",
#define OptionS15                 "付款方式键"  // "付款方式键",
#define OptionS16                 "存入金额:"  // "存入金额:",
#define OptionS17                 "入金/出金"  // "入金/出金",
  //Option for Disc
#define OptionS18                 "打印折扣额"  // "打印折扣额",
#define OptionS19                 "使用折扣1:"  // "使用折扣1:",
#define OptionS20                 "使用折扣2:"  // "使用折扣2:",
  //Options for currency
#define OptionS21                 "卖出价找零"  // "卖出价找零",
  //Options for Draw
#define OptionS22                 "无小数点:"  // "无小数点:",
#define OptionS23                 "打开钱箱:"  // "打开钱箱:",
  //Options for Tax
#define OptionS24                 "营业/增值:"  // "营业/增值:",
#define OptionS25                 "打印税率项"  // "打印税率项",
#define OptionS26                 "打印0税项"  // "打印0税项",
#define OptionS27                 "为GST类税"  // "为GST类税",
#define OptionS28                 "打印税额:"  // "打印税额:",
  //Options for clerk
#define OptionS29                 "取消限制:"  // "取消限制:",
#define OptionS30                 "更改限制:"  // "更改限制:",
#define OptionS31                 "取消交易:"  // "取消交易:",
#define OptionS32                 "退货限制:"  // "退货限制:",
#define OptionS33                 "%折扣限制:"  // "%折扣限制:",
#define OptionS34                 "%加成限制:"  // "%加成限制:",
#define OptionS35                 "+-金额限制"  // "+-金额限制",
#define OptionS36                 "培训模式:"  // "培训模式:",

#define MESS_ECR_IP         "本机IP地址:" //ECRIPADDRESS
#define MESS_ECR_ID         "本机ID(0-9,A-F):"   //ECRNETID

#define EditN1                    "<记录号>选择纪录\n<确认>  当前纪录\n"  //ccr "<记录号>  新纪录\n<确认>  当前纪录",
#define EditN2                    "<条码>增删改商品\n<确认>  当前商品\n"  //ccr "<记录号>增删商品\n<确认>  当前商品\n",
#define EditN3                    "<退出>      退出"  // "<退出>      退出",
#define EditN4                    "<确认>      继续\n"  // "<确认>      继续\n",
#define EditN5                    "选择打印报表"  // "按<选择>报表",
#define EditN7                    "定义到键位"  //"定义到键位",
#define EditN8                    "只能在经理档用"  // "只能在经理档用",
#define EditN9                    "可在营业档用"  // "可在营业档用",
#define EditN10                   "打印文字"  // "打印文字",
#define EditN11                   "(保留)"  // "保留",
#define EditN12                   "文件密码测试"  // "文件密码测试",
#define EditN13                   "拨号"  // "拨号",
#define EditN14                   "MODEM 连接!"  // "MODEM 连接!",
#define EditN15                   "代码:"  // "代码:",
#define EditN16                   "%折扣"  // "%折扣",
#define EditN17                   "%加成"  // "%加成",
#define EditN18                   "收据号:"  // "收据号:",
#define EditN19                   "文件数"  // "文件数："，0
#define EditN20                   "文件空间"  // "文件空间："?
#define EditN21                   "统计文件"  // "统计文件"2
#define EditN22                   "计税"  // “计税”8
#define EditN23                   "流水帐空间"  // "流水空间"
#define EditN26                   "顶部空行数:"  // '顶部空行数',
#define EditN27                   "每页行数:"  // '每页行数',
#define EditN28                   "打印信息:"  // '打印信息',
#define EditN29                   "第二章票据:"  // '第二张票据',
#define EditN30                   "左边距:"  // '左边距',
#define EditN31                   "票打机型:"  // '票打机型',
#define EditN32                   "打印机关"  // "打印机关",
#define EditN33                   "打印机开"  // "打印机开",

  //ChipCard>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN1                "密码:"  //I "密码:",
#define ChipCardN2                "密码确认:"  // "密码确认:",
#define ChipCardN3                "POS码:"  // "POS代码:",
#define ChipCardN4                "POS确认:"  // "代码确认:",
#define ChipCardN5                "代码已保存"  // "代码已保存",
#define ChipCardN6                "代码错 ！"  // "代码错 ！",
#define ChipCardN7                "启用IC卡:"  // "启用IC卡:",
#define ChipCardN8                "折扣卡:"  // "折扣卡:",
#define ChipCardN9                "现金卡:"  // "现金卡:",
#define ChipCardN10               "赊帐卡:"  // "赊帐卡:",
#define ChipCardN11               "打印IC卡"  // "打印IC卡",
#define ChipCardN12               "自动折扣:"  // "自动折扣:",
#define ChipCardN13               "消费积分:"  // "消费积分:",
#define ChipCardN14               "用现金键:"  // "用现金键:",
#define ChipCardN15               "启用有效期"  // "启用有效期", //
#define ChipCardN16               "挂失卡"  // "挂失卡号",
#define ChipCardN17               "卡号:"  // "卡号:",
#define ChipCardN18               "卡类型:"  // "卡类型:",
#define ChipCardN19               "余额:"  // "余额"
#define ChipCardN20               "卡押金:"  // "卡押金:",
#define ChipCardN21               "消费:"  // "消费:",
#define ChipCardN22               "充值总额:"  // "充值总额:",
#define ChipCardN23               "使用次数:"  // "使用次数:",
#define ChipCardN24               "价格级别:"  // "价格级别:",
#define ChipCardN25               "PIN码:"  // "PIN码:",
#define ChipCardN26               "保护级别:"  // "保护级别:",
#define ChipCardN27               "自动结帐:"  // "自动结帐:",
#define ChipCardN28               "制卡日期:"  // "制卡日期:",
#define ChipCardN29               "卡有效期:"  // "卡有效期:",
#define ChipCardN30               "客户:"  // "客户:",
#define ChipCardN31               "初始化IC"  // "初始化IC",
#define ChipCardN32               "IC卡充值"  // "IC卡充值",
#define ChipCardN33               "清除IC卡"  // "清除IC卡",
#define ChipCardN34               "充值/退款?"  // "充值/退款?",
#define ChipCardN35               "兑奖/送分?"  // "兑奖/送分?",
#define ChipCardN36               "确认充值?"  // "确认充值?",
#define ChipCardN37               "确认退款?"  // "确认退款?",
#define ChipCardN38               "确认送分?"  // "确认送点?",
#define ChipCardN39               "兑奖确认?"  // "兑奖确认?",减点
#define ChipCardN40               "积分兑奖"  // "积分兑奖",
#define ChipCardN41               "IC卡退款"  // "IC卡退款",
#define ChipCardN42               "兑奖分数"  // "兑奖分数",
#define ChipCardN43               "兑送分数:"  // "兑送分数:",
#define ChipCardN44               "请等待......"  // "请等待......",
#define ChipCardN45               "请先退款..."  // "卡内有金额,请先退款....",
#define ChipCardN48               "要启用新的IP,请重新开机."  // "要启用新的IP,请重新开机.",
#define ChipCardN49               "确认清报表?"  //  "确认清报表?",
#define ChipCardN50               "等待IC卡...."  // "等待IC卡....",
#define ChipCardN51               "倍高打印"  // "倍高打印",
//ccr2015-06-15>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN52             "清卡"
#define ChipCardN53             "退款"
#define ChipCardN54             "付款"
#define ChipCardN55             "充值"
#define ChipCardN56             "初始化"
#define ChipCardN57             "取现"
#define ChipCardN58            "增加积分"   //ADDVIPPOINTS
#define ChipCardN59            "减分兑奖"   //DECVIPPOINTS
#define ChipCardN60            "累计积分"  //VIPPOINTS
//ccr2015-06-15<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MESSEJ_X_STORE      "存储X数据"//MsgEJ_X_STORE
#define MESSEJ_Z_STORE      "存储Z数据"//MsgEJ_Z_STORE
#define MESSEJ_TR_STORE     "存储TR数据"//MsgEJ_TR_STORE
#endif
//ccr2016-02-22>>>>>>>>>>>>>>>>>>>>>
#define MsgMENUXREPORT     "打印X报表"//     显示
#define MsgMENUXEXPORT     "导出报表(X)"//     显示
#define MsgMENUQUICKVIEW   "调阅汇总数据"//     显示
#define MsgMENUXLOGFUNC    "调阅流水帐"//     显示
#define MsgMENUSENDNEWLOG  "发送新流水帐"      //MENUSENDNEWLOG  //发送新流水数据
#define MsgMENUXPASSWORD   "设置X密码"//     显示
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUXEJOURNAL    "调阅EJ"     //MENUXEJOURNAL   "E Journal"
#endif
//.................................
#define MsgMENUZREPORT     "打印Z报表"//     显示
#define MsgMENUZEXPORT     "导出报表(Z)"//     显示
#define MsgMENUCLEARLOG    "清除流水帐"//     显示
#define MsgMENUZGRAND      "清总账选项"//     显示
#define MsgMENUZPASSWORD   "设置Z密码"//     显示
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUZRESETEJ    "初始化EJ"     //MENUZRESETEJ   "初始化EJ"
#endif
//...................................


#define EPosN1                    "ePOS认证码:"  // "ePOS认证码:",
#define EPosN2                    "等待易通卡"  // "等待易通卡"
#define EPosN3                    "易通卡号:"  // "易通卡号:",
#define EPosN4                    "易通卡余额:"  // "易通卡余额:",


#define ErrInfoN1                 "通讯数据太长"  // "通讯数据过长"
#define ErrInfoN2                 " *** 打印被中断、重新打印 *** "  // " *** 打印被中断、重新打印 *** "
#define ErrInfoN3                 "数据校验错"  // "数据校验错"

#define PLULIST               "         单 品 信 息 表"  // "         单 品 信 息 表"//打印
                                 //1234567890123456789012345678901234567890

#define FisTextN1                 "纳税人号:"  //"纳税人号:"
#define FisTextN2                 "开出税票      "  //"开出税票"
#define FisTextN3                 "授权号码        "  //"授权号码"
#define FisTextN4                 "写机器号        "  //"写机器号"
#define FisTextN5                 "写入确认        "  //"写入确认"
#define FisTextN6                 "数据印出        "  //"数据印出"
#define FisTextN7                 "用户号码        "  //"用户号码"
#define FisTextN8                 "免税状态        "  //"免税状态"
#define FisTextN9                 "Z-计数:         "  //"Z-计数:"
#define FisTextN10                 "累计销售        "  //"累计销售"
#define FisTextN11                 "累计税          "  //"累计税"
#define FisTextN12                 "总免税额        "  //"总免税额"
#define FisTextN13                 "总销售额        "  //"总销售额"
#define FisTextN14                 "总税额          "  //"总税额"
#define FisTextN15                 "日税票数        "  //"日税票数"
#define FisTextN16                 "收据总数        "  //"收据总数"
#define FisTextN17                 "余报表数 :      "  //"余报表数:"



#define CurrentTimeN1              "当前时间:"  // "当前时间"


#define InputMode1				"a: " // "A："
#define InputMode2				"区:" // "区："
#define InputMode3				"拼:" // "拼："
#define InputMode4				"笔:" // "笔："

#define SpecialSymbol1			"★"			//DOWNLOAD//Ccr "★",
#define SpecialSymbol2			"☆"			//UPLOAD//Ccr "☆",
#define SpecialSymbol3			"↑  ↓"		// "↑  ↓",
#define MESS_PRESSKEY   "....按任意键...."


//added by lqw 090802
#if  (defined(CASE_GPRS))
#define GPRSstr1  "数据类型错|"
#define GPRSstr2  "GPRS就绪"
#define GPRSstr3  "GPRS未就绪!"
#define GPRSstr4  "确认?"
#define GPRSstr5  "服务器IP或端口空"
#define GPRSstr6  "服务器IP/PORT错!"
#define GPRSstr7  "等待GPRS......."
#define GPRSstr8  "是"
#define GPRSstr9  "否"
#define GPRSstr10 "数据发送中.."
#define GPRSstr11 "数据已发送.."
#define GPRSstr28 "发送成功.."
#define GPRSstr31 "连接服务器失败"
#define GPRSstr32 "接收失败"
#define GPRSstr33 "发送确认失败"
#define GPRSstr34 "发送失败"
#define GPRSstr58 "正在建立连接.."
#define GPRSstr59 "准备建立连接.."
#define GPRSstr60 "正在复位模块.."
#define GPRSstr61 "发送完毕,请退出.."
#define GPRSxACK  "没有收到ACK"

#endif //end defined(CASE_GPRS)
//////////////////////////////////////////////////////////
//以下信息不存入Msg[]中
#define MESSPRNMENU     "<00>打印菜单"
#define MESSSETMENU     "功能菜单表"
#define MESSCLEANLOG    "    清除流水帐?"
#define MESSCLEANLOGA   "按确认时, 将清除\n全部存根数据!"
#define MESSRAMOUT      "  存储空间不足!"
#define MESSCLEANFILES  "  清除文件数据?"
#define MESSCONFIGERR   "配置数据已改变,\n保留文件数据可能\n引起销售错误!"
#define MESSDELPLU      "系统变量有变化,\n     单品被删除!"
#define MESSPLUPRINTEND "单品信息打印完毕\n按确认键继续!"

#define RECALLFLOW      "恢复销售流水帐"

/////////////////////////////////////////////////////////////

#define InsertUSB		"<<插入U盘>>" //
//ccr090715>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define usbMAIN1		"导出文件" //
#define usbMAIN2		"导入文件" //
#define usbMAIN3		"导出电子存根" //

#define usbFLOWNEWMESS	"导出新数据"
#define usbFLOWALLMESS	"导出全部数据"
#define usbFLOWDELMESS	"导出全部+清空"

#define usbALLFile		"全部文件" //

#define USBREPNAME      "项目名称"

////////  CSV导出文件标题  /////////////////////////////////////////////
#define DEPTCAP       "编码,名称,归属柜组,对应选项,销售选项\r\n"
#define PLUCAP        "编码,名称,特殊优惠,归属部类,价格1,价格2,价格3,价格4,进价,捆绑销售,库存\r\n"

#define TAXCAP        "名称,税率,选项\r\n"
#define HEADERCAP     "票头文字\r\n"
#define DISCCAP       "名称,打印选项,选项,固定折扣,税率,最大值\r\n"


#define SALPERCAP     "名称,选项\r\n"
#define SYSTEMCAP     "系统控制标志\r\n"
#define COMMCAP       "名称,类型,同学协议,电话号码\r\n"
#define GRAPPRNCAP    "图像编号,开始日期,结束日期\r\n"
#define GROUPCAP      "名称,最大销售金额\r\n"
#define ICCARDCAP     "IC会员卡设置\r\n"
#define PROMCAP       "中奖频率,最小金额,计点,最小点数,图像序号,说明文字\r\n"
#define PBFUNCCAP     "名称,打印控制,选项\r\n"
#define CLERKCAP      "名称,选项,密码\r\n"
#define MODICAP       "口味说明\r\n"
#define PBTOTALCAP    "编码,名称\r\n"
#define ZONECAP       "时段1,时段2,时段3,时段4,时段5,时段6,时段7,时段8,时段9,时段10,时段11,时段12\r\n"
#define KEYBOARDCAP   "键盘定义\r\n"
#define OFFCAP        "名称,类型,开始日期,结束日期,开始时间,结束时间,折扣,整包数,单价,整包价给\r\n"
#define PORACAP       "名称,打印控制,选项,钱箱,最大金额\r\n"

#define TRAILERCAP    "票尾文字\r\n"
#define SLIPHEADCAP   "收据打印机票头\r\n"
#define ICBLOCKCAP    "IC会员卡黑名单\r\n"

#define EXPORTCSV    "1-CSV,2-BIN:"

#define CSVFLOETITL "项目名称,数量,金额,收据号,收款员,营业员,日期,收款机,店铺,数据类型\r\n"
#define CSVREPTITL  "日期,收款机,店铺\r\n"
#define BUTTONBATLOW "纽扣电池电压低!\n请更换电池!"

#define CHARGEBATNOW "供电电池电压低!\n请立即充电!"

#define SRAMNOTFOUND "没有装配SRAM!\n系统无法运行!"

#define INPUTPLUCODE "输入单品代码:"

#define MessageE18   "!!!! PLU文件空间溢出错 !!!!"

#define MESS_TABLEHOLD  "桌台      挂起!"

#define MESS_LOGREPET   "可能重复发送流水"

#define MESS_PAYALSO   "是否继续付款?"


//............................................
#define MGADDINV        "增加单品库存"  //1+SUBTOTAL
#define MGDECINV        "减少单品库存"  //2+SUBTOTAL
#define MGSETPRICE      "修改单品价格"  //3+SUBTOTAL

#define MGCLEARIC       "会员退卡"
#define MGINITIC        "初始化会员卡"
#define MGCHARGE        "会员卡充值"
#define MGADDPOINT      "会员兑送积分"
#define MGPRINTSTOCK    "调阅单品库存"
#define MGPASSWORD      "设置经理密码"
#define MGCLOSEPRN      "关闭打印"
#define MGTRAINING      "培训模式"
#define MGGOTOMG        "经理档"
//..............................................
#define PLUCODE     "PLU代码:"
#define INVQTYMUST  "输入库存数或价格"
#define ININVENTORY "请先按<小计>键\n退出库存管理"
//..............................................
#define DEVICENULL		"   无"
#define DEVICEPORT1     "串口1"
#define DEVICEPORT2     "串口2"
#define DEVICEPORT3     "串口3"
#define DEVICEPORT4     "串口4"
//................................................
#define CLOSEPRINTER    "关闭打印机"
#define CLEARGRANDTOTAL "Z日报清总计"
#define CLEANALLECRVIP  "清除会员数据?"
//.......功能码对应的名称.........................................
#define MsgKEYCORR      "更改功能"
#define MsgKEYCURR      "外币"
#define MsgKEYDISC      "折扣功能"
#define MsgKEYPBFUN     "餐饮功能"
#define MsgKEYPORA      "出入金"
#define MsgKEYPO_RA      "出金/入金"
#define MsgKEYTEND      "付款方式"
#define MsgKEYCLEAR     "清除"
#define MsgKEYFEED      "进纸"
#define MsgKEYMULT      "X"
#define MsgKEYSUBTOT    "小计"
#define MsgKEYPRICE     "价格"
#define MsgKEYPRICEN    "价格#"
#define MsgKEYPRICE1    "价格1"
#define MsgKEYPRICE2    "价格2"
#define MsgKEYPRICE3    "价格3"
#define MsgKEYPRICE4    "价格4"
#define MsgKEYSHIFT     "SHIFT"
#define MsgKEYDATE      "日期"
#define MsgKEYWEIGHT    "称重"
#define MsgKEYCLERKN    "收款员#"
#define MsgKEYDEPT      "直接部类"
#define MsgKEYDEPTNo    "部类#"
#define MsgKEYPLUNo     "单品#"
#define MsgKEYDRAW      "打开钱箱"
#define MsgKEYMEMO      "说明"
#define MsgKEYCLERK     "收款员*"   //加注'*'表示不能用于定义键盘
#define MsgKEYPLUDIR    "直接单品"
#define MsgKEYLOCK      "方式锁"
#define MsgKEYDRAWFUN   "钱箱功能*" //加注'*'表示不能用于定义键盘
#define MsgKEYSALPN     "营业员#"
#define MsgKEYSALP      "营业员"
#define MsgKEYDOT       "点'.'"
#define MsgKEYZERO2     "'00'"
#define MsgKEYNUMBER    "数字"
#define MsgKEYSUSPEND   "挂单"
#define MsgFUNCLOOK1    "功能表1"
#define MsgFUNCLOOK2    "功能表2"
#define MsgMODELOCK     "方式锁"
#define MsgVIPLOGIN     "会员登录"
#define MsgINPUTNUM     "输入号码"
#define MsgCUSTOMER     "顾客名称:"
#define MsgKEYNULL      "(其它)"

#define MsgRHEADER      "票头*"     //加注'*'表示不能用于定义键盘
#define MsgRTRAILER     "票尾*"     //加注'*'表示不能用于定义键盘
#define MsgKEYBOARD     "键盘*"     //加注'*'表示不能用于定义键盘
#define MsgSYSFLAGS     "系统参数*"//加注'*'表示不能用于定义键盘
#define MsgSETDATE      "日期时间*"//加注'*'表示不能用于定义键盘
//......................................
#define MsgCONFIRM      "请确认"
#define MsgNET          "净额"
#define VIPLOGOUT       "会员退出"
#define VIPTOTAL        "总金额:"
#define VIPTOTALA       "消费总额:"
#define DECVIPPOINT     "兑奖"//长度必须为4
#define ADDVIPPOINT     "送分"//长度必须为4
#define INPUTVIPNAME    "输入会员姓名:"
#define PRINTKEYBOARD   "打印键盘布局"
//........................................
#define MESSALLLOG         "打印全部收据"
#define MESSDATERANGE     "指定日期范围"
#define MESSVIPLOG        "会员消费记录"
#define MESSPRNCASH       "打印付款记录"
#define MESSPRNDEPART     "打印部类记录"
#define MESSPRNPLU        "打印单品记录"
#define MESSPRNDISCOUNT   "打印折扣加成"
#define MESSPRNRAPO       "打印出入金"
#define MESSPRNCORRECT    "打印更正退货"
#define MESSPRNCANCEL     "打印取消交易"
#define MESSPRNNOSALE     "打印非销售"

#define MESSALLCASH		  "全部付款方式"
#define MESSSELCASH		  "选择付款方式"

#define MESSALLDEPT		  "打印全部部类"
#define MESSSELDEPT		  "选择一个部类"

#define MESSALLPLU		  "打印全部单品"
#define MESSSELPLU		  "选择一个单品"
//........................................
//ccr2016-03-15>>>>定义键盘宏>>>>>
#define MESSMACRO       "宏定义"
//....ListItems提示信息....................................
#define It_MGITEMS      " 经理功能"
#define It_SELEFUNC     " 选择功能"
#define It_SELEITEM     " 选择项目"
#define It_SELEKEYTYPE  " 键盘模式"
#define It_SELETEST     " 测试项目"
#define It_SELELOG      " 选择类型"
#define It_SELETEND     " 付款方式"
#define It_SELEDEPT     " 选择部类"
#define It_SELEPLU      " 选择单品"
#define It_SELEMODI     " 口味说明"
#define It_SELEREPORT   " 打印报表"
#define It_SELEPOINT    " 小数位数"
#define It_SELEEJ       " EJ      "
//.........................................................
#define MESSRAMVAR      "运行数据"
//.........................................................
#define PLUNUMBER           "记录数:"
#define PLURANDOMSIZE     	"编码位数:"
#define PLUNAMESIZE			"名称长度:"
#define PLUPRICESIZE        "价格位数:"
#define PLUPRICELEVEL       "会员价级别:"
#define PLUCOSTSIZE         "进价位数:"
#define PLUINVENTSIZE       "库存位数:"
#define PLUREPQTYSIZE       "统计数量位数:"
#define PLUREPAMTSIZE       "统计金额位数:"
#define PLUREPDISCSIZE      "统计折扣位数:"
#define PLUREPCOSTSIZE      "统计进价位数:"
//.......................................................
#define POINTTYPE           "小数位数"
//以下字符串应该等长
#define POINTTYPE1          "1.000,00"  //两位小数
#define POINTTYPE2          "1,000.00"  //两位小数
#define POINTTYPE3          " 1.000无"     //无小数
#define POINTTYPE4          " 1,000无"     //无小数
#define POINTTYPE5          "1.000,000"  //三位小数
#define POINTTYPE6          "1,000.000"  //三位小数
//#define POINTTYPE7          "1.000,0"  //一位小数
//#define POINTTYPE8          "1,000.0"  //一位小数
//.......................................................
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MESSEJMESSAGES      "EJ存储信息"    //MsgEJMESSAGES
#define MESSEJALLEJ         "打印全部EJ"
#define MESSEJDATERANGE     "日期范围EJ"
#define MESSEJRECEIPT       "打印收据EJ"
#define MESSEJXREPORT       "打印X-EJ"
#define MESSEJZREPORT       "打印Z-EJ"
#define MESSEJCLERK         "收款员EJ"
#endif
//.......................................................
#define DATETYPE        "格式:"
#define DDMMYYYY        "DDMMYYYY"
#define MMDDYYYY        "MMDDYYYY"
#define YYYYMMDD        "YYYYMMDD"
//.........................................................
#define XUPDATEBIOS     "USB通讯不支持\n更新程序!"

//ccr2016-06-22..........................................
#if defined(CASE_QRCODE)
#define MESSWXPAY       "微信支付成功"
#endif

#define SENDLOGAUTO     "自动发送流水:"
#define DAYSFORLOG      "发送间隔天数:"
#define ACKMUSTNEED     "需要ACK应答:"

#define CSV_TITLE       "名称,数量,金额\r\n"

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define Msg_FORMAT      "ERASE EJ"
#define Msg_FORMATED    "ERASED"
#define Msg_RECALLEJ    "RECALL EJ"
#define Msg_EJNOTEMPTY  "EJ NOT EMPTY"

#define Msg_ECRDATEltEJ "ECR DATE < EJ"
#endif

//ccr2017-01-10>>>>>>>>>>>>>>>>>>>>>>>>>
#define Msg_NOSTOCK     "无库存数据"
#define Msg_DISPSTOCK   "显示/打印?"
//ccr2017-01-10<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
