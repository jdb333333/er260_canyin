#ifndef MESSAGEH_EN_D
#define MESSAGEH_EN_D


#define Modifier1             "MODIFIER"        //"说明    "
#define ClerkRec1             "CLERK"        //"收款员  "
#define GroupRec1             "GROUP"        //"柜组    "
#define DeptRec1              "DEPT"        //"部类    "
#define PLURec1               "PLU"        //"商品    "
#define SalesPersonRec1       "SALPER"        //"营业员  "
#define OffRec1               "OFFER"        //"特殊出价"

#define XZReportList1     "DAILY REPORT"        //"销售日报表",
#define XZReportList2     "PERIOD REPORT"        //"销售月报表",
#define XZReportList3     "PERIOD PLU"          //"商品报表",
#define XZReportList4     "PERIOD TABLES"        //"桌帐报表",
#define XZReportList5     "TIME ZONE"           //"时段报表",
#define XZReportList6     "CLERK DAILY"        //"收款员日报表",
#define XZReportList7     "CLERK PERIOD"        //"收款员月报表",
#define XZReportList8     "CLERK DAILY-*"        //"全收款员日报表",
#define XZReportList9     "CLER.PERIOD-*"        //"全收款员月报表",
#if (salNumber)
#define XZReportList10    "SALPR.DAILY"        //"营业员日报表"
#define XZReportList11    "SALPR. PERIOD"        //"营业员月报表"
#define XZReportList12    "SALPR.DAILY-*"        //"全营业员日报表",
#define XZReportList13    "SALP.PERIOD-*"        //"全营业员月报表",
#endif
#if (BARCUSTOMER==1)
#define XZReportList14    "MEMBERSHIP"        //"条码会员报表",
#endif
//Display
#define PortType1               "COMPUTER"        //"  主机",
#define PortType2               "SCANNER"        //"条码枪",
#define PortType3               "SCALE"        //"电子秤",
#define PortType4               "KITCHEN PORT"        //"厨  打",
#define PortType5               "SLIP PORT"        //'票打'，20040322
#define PortEXTPRN      "EXTPrint-PORT"//ccr2016-12-14 ExtPrinter串口
#define PortType6               "CHIP CARD"        //-------090421 BY ZY//"射频卡"
#define PortType7               "E-CARD"        //"E通卡"

#define KClose                  "   OFF"        //"   关 ",//

#define SysFlags0		        "SHOP#"	// "店铺编号"
#define SysFlags1               "ECR#"        //"收款机号:"
#define SysFlags2               "ROUNDING"        //"舍入方式:"
#define SysFlags3               "FREE PRICE"        //"商品变价:"
#define SysFlags4               "BUZZER"        //"蜂鸣器:"
#define SysFlags5               "CLERK REQUIRED"        //"强制收款员"
#define SysFlags6               "CLERK LOCKED"        //"改变收款员"
#define CLERKPASS               "CLERK PASSWORD"        //"收款员密码"
#define SysFlags8               "PRINT SALESMAN"        //"打印营业员"
#define SysFlags9               "PRINT CLERK"        //"打印收款员"
#define SysFlags10              "PRNT RECEIPT#"        //"打印收据号"
#define SysFlags11              "HEAVY PRINTING"        //"高浓度打印"
#define SysFlags12              "SALESMAN REQUIRED"        //"强制营业员"
#define SysFlags13              "PRINT CODE"        //"打印编码:"
#define SysFlags14              "SUBT. REQUIRED"        //"按小计键:"
#define SysFlags15              "PRNINT ITEMS"        //"打印项目数"
#define SysFlags16              "STUB"        //"打印小票:"
#define SysFlags17              "STUB HEADER"        //"小票票头:"
#define SysFlags18              "SHIFT LOCK"        //"锁定SHIFT:"
#define SysFlags19              "IDLE TIME"        //"显示时间:"
#define SysFlags20              "RECEIPT COPY"        //"复制收据:"
#define SysFlags21              "SCALE CODE"        //"特殊条码:"
#define SysFlags22              "OPR. LOG" //-----090421 BY ZY//"操作日志"
#define SysFlags23              "PRINT PB"        //"开台打印:"
#define SysFlags24              "LIST STOCK"        //"打印库存"
#define SysFlags25              "RECEIPT ON/OFF"        //"打印开关:"
#define SysFlags26              "CHECK REQUIRED"        //"必须开台:"
#define SysFlags27              "KEEP R-NO#"        //"保持收据号"
#define SysFlags28              "K-PRINT COPY"        //"复制厨打:"
#define SysFlags29              "RECEIPT# RESET"        //"收据号复位"
#define SysFlags30              "DOUBLE PRT"    //-----090421 BY ZY//"倍高打印"
#define SysFlags31              "LCD CONTRAST"   //-----090421 BY ZY//"对比度:"
#define SysFlags32              "->HEAD<-"  // "票头居中打印"
#define SysFlags33              "->TRAIL<-"  // "票尾居中打印"
#define SysFlags34              "HEADER D-HEIGHT"  // "票头倍高打印"
#define SysFlags35              "TRAILER D-HEIGHT"  // "票尾倍高打印"
//ccr2016-03-21#define SysFlags36              "DATA FORMAT"       // 输出CSV or BINARY
#define SysFlags37              "PRINT DATE"                                    //
#define SysFlags38              "PRINT TIME"                                    //
#define SysFlags39              "PRINT CALL#"    //"打印取菜号"
#define SysFlags40              "HEART BEAT"    //ccr2015-08-03"向服务器发送Heart beat"
#define SysFlags41              "PreFix1:"      //ccr2016-08-02
#define SysFlags42              "PreFix2:"      //ccr2016-08-02

#define GrapType1               "LOGO TYPE"        //"商店图标",
#define GrapType2               "PRIZE"        //"中奖图标:",
#define GrapType3               "TRAIL LOGO"        //"票尾图像:",
#define GrapType4               "TEXT"        //"打印文字?",

#define GrapSet1                "SEQUENCE#"        //"序号:",
#define GrapSet2                "PICTURE#"        //"图像编号:",
#define GrapSet3                "FROM"        //"开始日期:",
#define GrapSet4                "TO"        //"结束日期:",
  /////==============CHN msg===============//////
// Display
#define CurrentMode1            "RG  "  //"营业",
#define CurrentMode2            "X   "  //"读帐",
#define CurrentMode3            "Z   "  //"清帐",
#define CurrentMode4            "SET "  //"设置",
#define CurrentMode5            "MG  "  //"经理",

#define SetupMode1              "GENERAL"  //---090421 BY ZY// "综合功能",
#define SetupMode2              "DEPT/PLU..."  // "商品管理",
#define SetupKeyboard           "KEYBOARD"      //DEFKEYBOARD  //
#define msgCOMMHOST             "NETWORK"      // 这项不需要???ZWQ
#define SetupMode3              "PERIPHERALS"    // "通讯与外设",
#define SetupMode4              "MANAGEMENT"    // "销售功能设置",
#define SetupMode5              "ACCOUNT"   // "会员卡管理",
#define SetupMode6              "HEADER/TRAILER"  // "票据样式",
#define SetupMode7              "REPORT"    // "报表功能",
#define msgDISCOUNT             "DISCOUNT"      //MENUDISC //
#define SetupMode8              "PB FUNCTION"   // "餐饮功能",
#define SetupMode9              "PASSWORD"    // "密码,
#define SetupMode10             "SERVICE"       // "收款机维护",


#define SysSettings1            "SYSTEM OPTIONS"     // "系统参数设置",
#define SysSettings2            "DATE"     // "日期",
#define SysSettings3            "TIME"     // "时间",
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define msgSETEJOURNAL          "SET EJ"      //SETEJOURNAL //EJOURNAL
#endif

#define SysSettings4            "DEFINE KEYBOARD"     // "键盘",
#define MESSKEYMACRO            "KEYB. MACRO" // "键盘宏"
#define MESSKEYTYPE             "KEYBOARD TYPE"             // "键盘模式"
#define SysSettings5            "HOSPITALITY"      // "餐饮键盘切换",
#define SysSettings6            "RETAIL"    // "超市键盘切换",
#define SysSettings8            "WIRELESS"          //"GPRS通信"  //SETGPRS //ccr2014-11-11 NEWSETUP

#define SetupCaption1           "GROUP"			//"柜组"
#define SetupCaption2           "DEPARTMENT"	//"部类",
#define SetupCaption3           "PLU"			//"单品",
#define SetupCaption4           "PRINT PLU LIST"			//"打印商品清单",
#define msgSETTEST              "TEST ECR"//"系统测试"            //SETTEST
#define msgSETCLEARPLU          "CLEAR PLU"//"清除单品"        //SETCLEARPLU
#define msgSETPLULIST           "PRINT DEPT/PLU"//"打印商品清单"      //SETPLULIST
#define msgSETTSTVER	        "PRINT VERSION"//"打印版本"      //SETTSTVER
#define msgSETCONFIGPLU         "FORM PLU"//"配置单品"          //SETCONFIGPLU
#define msgSETCONFIGDEPT        "FORM DEPT" //配置部类         //SETCONFIGDEPT
#define msgSETINSTALL           "INSTALL/BACKUP"//"安装/备份"        //SETINSTALL
#define msgSETPRINTFAT          "PRINT MAT"//"打印文件分配表"   //SETPRINTFAT

#define SetupCaption5           "COMPUTER"			// "通讯口1",
#define SetupCaption6           "SCANNER"			// "通讯口2",
#define SetupCaption7           "SCALE"			// "通讯口 3",
//#define SetupCaption8           "PORT-4"			// "通讯口 4",
#define SetupCaption9           "ETHERNET"			// "网络设置",

#define SetupCaption10          "SLIP PRINTER"			// "票据打印",
#define SetupCaption11          "KITCHEN PRINTER"			// "厨房打印",
#define SetupEXTPrinter         "EXTERNAL PRINT"    //SETEXTPRINTER

//Display
#define SetupCaption12          "OFFER"				// "优惠价",
#define SetupCaption13          "PROMOTION"			// "中奖设置",
#define SetupCaption14          "DISCOUNT"				// "折扣",
#define SetupCaption15          "PO.RA"				// "出金入金",
#define SetupCaption16          "CORRECT"			// "更正功能",
#define SetupCaption17          "TENDER"			// "付款方式",
#define SetupCaption18          "CURRENCY"        // "外币",
#define SetupCaption19          "DRAWER"        // "钱箱",
#define SetupCaption20          "TAX"        // "税率",

#define SetupCaption21          "SETUP ACCOUNT"  // "IC卡设置",
#define SetAddNewECRVIP         "ADD NEW ACCOUNT"
#define SetPrintECRVIP          "PRINT ACCOUNT"
#define SetCleanALLVIP          "CLEAN ALL ACCOUNT"

#define SetupCaption22          "LOST CARD"        // "IC卡挂失",
#define SetupCaption23          "PRINT CARD"        // "打印卡信息",
#define SetupCaption24          "HEADER"        // "票头",
#define SetupCaption25          "TRAILER"        // "票尾",
#define SetupCaption26          "SLIP HEADER"        // "票据票头",
#define SetupCaption27          "GRAPHIC"        // "图形",
#define SetupCaption28          "PRINT GRAPHICS"  // "打印图形",
#define SetupCaption29          "REPORT"        // "设置报表内容",
#define SetupCaption30          "REPORT UNIT"  // "报表单元",
#define SetupCaption31          "TIME ZONE"        // "报表时段设置",
#define SetupCaption32          "CLEAR ALL"     // "总清报表",
#define SetupCaption33          "PRINT LOG"         // "打印流水",

#define SetupCaption34          "TABLE"  // "桌台",
#define SetupCaption35          "PB FUNCTION"  // "餐饮功能",
#define SetupCaption36          "MODIFIER"      //ZY  // "菜单说明",
#define SetupCaption37          "AGREEMENT"          //ZY  // "签单",
#define SetupCaption38          "CLERK"  // "收款员",
#define SetupCaption39          "SALESMAN"  // "营业员",
#define SetupCaption40          "PG-PASSWORD"          // "设置档密码",
#define SetupCaption41          "MG-PASSWORD"          // "经理档密码",
#define SetupCaption42          "X-PASSWORD"      // "只读报表密码",
#define SetupCaption43          "Z-PASSWORD"      // "清帐报表密码",
#define SetupCaption44          "MEMORY"        // "内存测试",
#define SetupCaption45          "DISPLAY"       //"显示测试",
#define SetupCaption451         "CUSTOMER"      //"客显测试"
#define SetupCaption46          "PRINTER"      //"打印测试",
#define SetupTestRAM           "CHECK RAM"        //SETTSTMEM	       //RAM测试
#define SetupCaption47          "CLOCK"         //"时钟测试",
#define SetupCaption48          "BUZZER"        //"蜂鸣器测试",
#define SetupCaption49          "DRAWER"        //"钱箱测试",
#define SetupCaption50          "KEYBOARD"        //"键盘测试",
#define SetupCaption51          "SERIAL PORTS"     //"串口测试",
#define SetupCaption52          "SELF-TEST"        //"自动测试",

#define msgSETPRNMENU           "PRINT MENU"      //SETPRNMENU   //

#define OperationN1             "RECORD#"  // "记录#",
#define OperationN2             "NOTE?"           //ZY  // "记录?",

#define OperationN12            "PASSWORD:"  // "密码",
#define OperationN13            " CONFIRM:"  // "密码确认",
#define OperationN16            "STORE"  // "超市版",
#define OperationN17            "RESTAURANT"  // "餐饮版",
#define OperationN18            "TEST"  // "测试",                 //


//Dispaly
#define CaptionN39              "ADD PLU?"  // "增加?",
#define CaptionN40              "DELETE PLU?"  // "删除?",
#define CaptionN47              "OPTIONS"  // "选项:",
#define CaptionN48              "FIXED"  // "固定",不要超过SETINPUTWD的宽度
#define CaptionN49              "FREE"  // "浮动",不要超过SETINPUTWD的宽度
#define CaptionN50              "FIX/FREE"  // "固/浮",不要超过SETINPUTWD的宽度
#define CaptionN62              "JOLLY"  // "中奖设定:",
#define CaptionN63              "ACCOUNT POINT"  // "消费积分:",
#define CaptionN64              "PRICE CHANGE"  // "改变单品价格",
#define CaptionN65              "K-PRINTER GROUP"  // "厨房打印机组"，
#define CaptionN66              "INC. INVEN"  // "增加库存",
#define CaptionN67              "DEC. INVEN"  // "减少库存",
#define CaptionN68              "GET "  // "输入",
#define CaptionN69              "PAY "  // "待付",


#define CaptionE1               "RS232 ERROR"  // "RS232错",
#define CaptionE2               "RS232 OK"  // "RS232正确",
#define CaptionE3               "RTC ERROR!"  // "RTC 错!",
#define CaptionE4               "CTC ERROR!"  // "CTC 错!",
#define CaptionE5               "FPROM ERROR!"  // "FPROM错!",
#define CaptionE6               "INITIALIZING."  // "初始化...",显示
#define CaptionE7               "INIT. COMPLETED"  // "初始化结束.",
#define CaptionE8               "ERROR"  // "出错-",//长度<SCREENWD

#define CaptionE9               "PASSWORD ERR!"  // "密码错 ！",
#define CaptionE10              "PASSWORD SAVED"  // "密码已保存",
#define CaptionE15              "CONFIRM?"  // "确认?",
#define CaptionE16              "COMPLETE"  // "完成",

#define CaptionE25              "@UPDATING!\nDON'T SHUTDOWN!"  // "更新程序切勿断电",
#define CaptionE26              "FILE OVERFLOW!"  // "文件空间越界",
#define CaptionE27              "MEM OVERFLOW,\nDEC PLU!"  // 内存溢出！
#define CaptionE28              "LOG FULL"  // "流水存储区满",
#define CaptionE29              "PAPER END"  // "请更换打印纸",
#define CaptionE30              "EXTD MEM ERR"  // "扩展内存出错！",



#define CaptionE32              "INVALID INPUT"  // "无效输入!",
#define CaptionE33              "INVALID DATE"  // "无效日期!",
#define CaptionE34              "INVALID TIME"  // "无效时间!",
#define CaptionE35              "NOT ALLOWED!"  // "禁止输入!",
#define CaptionE36              "NULL PLU!"  // "PLU代码为空!",
#define CaptionE37              "PLU FULL"  // "PLU文件溢出!",
#define CaptionE38              "TABLE EXIST!"  // "桌台被占用!",
#define CaptionE39              "TABLE OPENED"  // "桌台已打开!",
#define CaptionE40              "WRONG TABLE#"  // "桌台号不对!",
#define CaptionE41              "INPUT TABLE#"  // "请输入桌台号",
#define CaptionE42              "NULL TABLE"  // "没有开台",
#define CaptionE43              "TABLE FULL"  // "桌台桌帐溢出",
#define CaptionE44              "NOT ALLOWED!"  // "禁止修改时间",
#define CaptionE45              "SALES NOT CLOSED"  // "仍在销售中!",
#define CaptionE46              "BUFFER FULL"  // "销售缓冲区满",
#define CaptionE47              "ITEM NOT SOLD"  // "商品没有售出",
#define CaptionE48              "SALES NOT CLOSED"  // "正在结算中!",
#define CaptionE49              "INVALID NUMBER!"  // "输入数据超长",
#define CaptionE50              "TENDERING!"  // "正在付帐!",
#define CaptionE51              "WRONG COVER"  // "人数溢出!",
#define CaptionE52              "CONFIRM TABLE"  // "没有确认结帐",
#define CaptionE53              "NOT ALLOWED!"  // "禁止取消付款",
#define CaptionE54              "NULL ITEM!"  // "无菜名!",
#define CaptionE55              "NULL CLERK"  // "无此收款员!",
#define CaptionE56              "CLERK LOCK"  // "收款员改变",
#define CaptionE57              "NULL REPORT!"  // "无此类报表!",
#define CaptionE58              "PRINT INTERRUPT"  // "报表打印中断",
#define CaptionE59              "MG-MODE REQUIRED"  // "必须在经理档",
#define CaptionE60              "SPLIT NOT ALLOWED"  // "不能分帐时用",
#define CaptionE61              "NOADD# REQUIRED!"  // "必须输入号码",
#define CaptionE62              "KEY POSITION ERROR"  // "钥匙档位错",
#define CaptionE63              "UNAUTHORIZED"  // "未授权!",
#define CaptionE64              "CLERK REQUIRED"  // "请指定收款员",
#define CaptionE65              "SALESMAN REQUIRED"  // "请指定营业员",显示
#define CaptionE66              "NOT ALLOWED!"  // "禁止PLU变价！",
#define CaptionE67              "PASSWORD ERROR!"  //  "密码不对!",
#define CaptionE68              "K-PRINT ERROR"  // "厨房打印机错",
#define CaptionE69              "PRINT ERROR"  // "票据打印机错", //ccr2017-08-31
#if defined(CASE_INNERDOTPRN)
#define CaptionE70              "PRINTER BUSY!"  // "打印压杆抬起",
#else
#define CaptionE70              "PAPER END!"  // "打印压杆抬起",
#endif
#define CaptionE71              "PAPER END!"  // "打印纸用完!",
#define CaptionE72              "PRT OVERHEATING!"  // "打印温度太高",
#define CaptionE73              "FUNC UNDEFINED"  // "按键未定义!",
#define CaptionE74              "VALUE REQUIRED"  // "必须输入金额",
#define CaptionE75              "POST TENDER!"  // "禁止多种付款",
#define CaptionE76              "NULL FUNC."  // "记帐无此功能"
#define CaptionE77              "SUBT REQUIRED!"  // "未按小计键!",
#define CaptionE78              "INVENTORY TAKING"  // "正在管理库存",
#define CaptionE79              "LOG ERROR"  // "保存流水错",
#define CaptionE80              "MODEM ERROR"  // "MODEM通讯错!",


#define CaptionE81              "CHIPCARD EROR"  // "卡操作出错!"
#define CaptionE82              "POS CODE ERROR!"  // "POS代码错!"
#define CaptionE83              "DATA ERROR!"  // "卡数据错!"
#define CaptionE84              "EXPIRED CARD!"  // "为过期卡!"
#define CaptionE85              "LOST CARD"  // "为挂失卡!"
#define CaptionE86              "WRONG TYPE!"  // "客户类别错!"显示
#define CaptionE87              "NEW CARD!"  // "为新卡!"
#define CaptionE88              "NO CASH CARD!"  // "不是购物卡!"
#define CaptionE89              "WRITING ERROR!"  // "写卡出错!"
#define CaptionE90              "WRONG SERIAL#!"  // "卡号不对!"
#define CaptionE91              "DISC. CARD!"  // "禁用折扣卡!"
#define CaptionE92              "CASH CARD!"  // "禁用现金卡!"
#define CaptionE93              "CREDIT CARD!"  // "禁用赊帐卡!"
#define CaptionE94              "DISABLE!"  // "禁用IC卡!"
#define CaptionE95              "DISCHARGEERR"  // "清卡出错!",
#define CaptionE96              "OVERFLOW!"  // "数据溢出!",
#define CaptionE97              "CHARGE ERR!"  // "IC充值出错!",
#define CaptionE98              "INIT ERR"  // "IC初始化出错",
#define CaptionE99              "DISABLED"  // "禁止初始化!",
#define CaptionE100             "LOW VALUE!"  // "IC金额不足!",
#define CaptionE101             "PAY ERR!"  // "IC付款出错!",
#define CaptionE102             "IP-ERR!"  // "IP地址错"
#define CaptionE103             "NO CHIP CARD!"  //  ,
#define CaptionE105		        "ON HOLD"    //CWXXI73//Ccr

#define CaptionERR105             "BATTERY LOW!"  // "电池电量低",
#define CaptionERR106             "STOCK NOT ACTIVE"	//20070806"没激活库存"
//ccr090715>>>>>>>>>>>>>>>>>>>>>>
#define CaptionERR76			  "INSERT DISK"	//
#define CaptionERR77			  "OPEN ERROR!"		//
#define CaptionERR78			  "READ ERROR!"		//
#define CaptionERR79			  "WRITE ERROR!"		//
#define CaptionERR80			  "CREATE ERROR"		//
#define CaptionERR81			  "CLOSE ERROR!"		//
#define CaptionERR82			  "SEND LOG!"		//
#define CaptionERR83              "GPRS ERROR!"                       //
#define CaptionERR84              "SEND ERROR!"//
#define CaptionERR85              "RECEIVE ERROR"//
#define CaptionERR86              "SERVER NOT FOUND!"//
#define CaptionERR87              "IP/PORT INVALID"    //没有设置服务器IP和端口
#define CaptionERR88              "NO SIM CARD"//
#define CaptionERR89              "GPRS NOT OK"//
#define CaptionERR90              "OUT OF RANGE" //CWXXI90  //
#define CaptionERR91              "NULL ACCOUNT"  //CWXXI91
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

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#define CaptionE104             "OPEN DRAWER"  // "打开钱箱",
#define CaptionE106             "RECEIPT ON HOLD"    //YOUGUADAN
#define CaptionE107             "  ENTER QUANTITY\n AND IT'S PRICE"    //SHURUQTYAMT
#define CaptionE108             "TRANFER TO"    //ZHUANRUDAO

#define MsgPUTOUT               "PO"   	                //PUTOUT
#define MsgRECIEVIN             "RA" 		            //RECIEVIN

#define DownLoad1               "DOWNLOAD FONT"  // "正在下载字库",
#define DownLoad2               "DOWNLOAD PICS"  // "正在下载图形",


#define OptionN1                "YES"  // "是",
#define OptionN2                " NO"  // "否",
#define OptionN3                "NAME "  // "名称:",
#define OptionN4                "DEPARTMENT"  // "部类:",
#define OptionN5                "GROUP"  // "柜组:",
#define OptionN6                "PRINT"  // "打印:",
#define OptionN7                "OPTION"  // "选项:",
#define OptionN8                "LINKED"  // "链接:",
#define OptionN9                "TAX"  // "税率:",
#define OptionN10               "POS CODE"  // "位置编号:",
#define OptionN11               "PRICE1"  // "价格 1:",
#define OptionN12               "PRICE2"  // "价格 2:",
#define OptionN13               "PRICE3"  // "价格 3:",
#define OptionN14               "PRICE4"  // "价格 4:",

#define OptionN15               "BUYING PRICE"  // "进价:",
#define OptionN16               "DISCOUNT(%)"  // "折扣率(%):",
#define OptionN17               "MAX"  // "最大值:",
#define OptionN18               "DISC. RATE"  // "折扣率:",
#define OptionN19               "MAX"  // "最大值:",
#define OptionN20               "RATE"  // "汇率:",
#define OptionN21               "BUYING"  // "买入价:",
#define OptionN22               "SELLING"  // "卖出价:",
#define OptionN23               "STATUS"  // "状态:",
#define OptionN24               "DRAWER"  // "钱箱:",
#define MSGOTD                  "OTD"                    //OTDMSG
#define OptionN25               "K-PRINTER"  // "厨房打印:",
#define MSGPERIOD               "PERIOD"                      //PERMSG
#define MSGPOINTER              "POINTER"                     //POIMSG

#define OptionN26               "PREFIX"  // "前缀符",6个字符
#define OptionN27               "LINK"  // "连接:",
#define OptionN28               "FUNCTION "  // "输入功能码:",
#define OptionN29               "ASSIGN TO"  // "按功能键:",
#define OptionN30               "TYPE"  // "类型:",
#define OptionN31               "DATE FROM"  // "开始日期:",
#define OptionN32               "DATE TO"  // "结束日期:",
#define OptionN33               "START TIME"  // "开始时间:",
#define OptionN34               "END TIME"  // "结束时间:",
#define OptionN35               "WEEK"  // "星期:",
#define OptionN36               "DISCOUNT"  // "折扣:",
#define OptionN37               "QUANTITY"  // "大包数量:",
#define OptionN38               "PRICE"  // "单价:",
#define OptionN39               "OFFER"  // "优惠价:",
#define OptionN40               "PROTOCOL"  // "通讯协议:",
#define OptionN41               "PHONE#"  // "电话号码:",
#define OptionN42               "PASSWORD"  // "密码:",
#define OptionN43               "FREQUENCY"  // "频率:",
#define OptionN44               "THRESHOLD"  // "最小金额:",
#define OptionN45               "PORT"  // "通讯口:",
#define OptionN46               "POINT VALUE"  // "计点金额:",
#define OptionN47               "PICTURE#"  // "图象序号:",
#define OptionN48               "TIME"  // "时间:",
#define OptionN49               "DATE"  // "日期:",
#define OptionN50               "PRINTER"  //厨房打印机类型
#define OptionN51               "TYPE:"      //DZCHXHAO//"秤型号:",

#define WelcomeN1                "THANKS CHOOSING\nEUTRON ECR"  // "创 造 优 秀 优 创 造 就\n    欢迎使用优创收款机" */
#define WelcomeN2                "THANKS CHOOSING \nEUTRON ECR"  // "创造优秀优创造就\n优创蓝宝石收款机" */

#define MessagesN1                "ECR CLOSED"  // "  收款机已锁定",//
#define MessagesN2                "PRESS <KEY>"  // "按【方式锁】开机",//
#define MessagesN3                "FORMAL INVOICE"  // "形式发票",		     																					//XSHFPIAO
#define MessagesN4                "KITCHEN #"  // "厨房L打印/显示机#",																						//CHFDYJHAO
#define MessagesN13               "<>"                                 //
#define MessagesN14               "TOTAL"  //KPTOTAL

//PRINT

#define CaptionM1                 "JANUARY"  // "一月",
#define CaptionM2                 "FEBRUARY"  // "二月",
#define CaptionM3                 "MARCH"  //  "三月",
#define CaptionM4                 "APRIL"  //  "四月",
#define CaptionM5                 "MAY"  // "五月",
#define CaptionM6                 "JUNE"  // "六月",
#define CaptionM7                 "JULY"  // "七月",
#define CaptionM8                 "AUGUST"  // "八月",
#define CaptionM9                 "SEPTEMBER"  // "九月",
#define CaptionM10                "OCTOBER"  // "十月",
#define CaptionM11                "NOVEMBER"  // "十一月",
#define CaptionM12                "DECEMBER"  // "十二月",

#define CaptionW1                 "MONDAY"  // "星期一",
#define CaptionW2                 "TUESDAY"  // "星期二",
#define CaptionW3                 "WEDNSDAY"  // "星期三",
#define CaptionW4                 "THURSDAY"  // "星期四",
#define CaptionW5                 "FRIDAY"  // "星期五",
#define CaptionW6                 "SATURDAY"  // "星期六",
#define CaptionW7                 "SUNDAY"  // "星期日",


#define ClueN1                    "PRESS ENTER..."  // "按确认键...",

  //Option for KP
#define OptionS1                  "PRINT TOTAL"  // "打印总额:",
#define OptionS2                  "COMBINED"  // "菜相同打印",
#define OptionS3                  "CLASSIFIED"  // "菜不同打印",
  //Option for dept
#define OptionS4                  "ZERO PRICE"  // "禁止零价格",
#define OptionS5                  "SEPARATRIX"  // "打印分隔线",
#define OptionS6                  "SINGLE"  // "单项销售:",
#define OptionS7                  "DISC. 1"  // "折扣项 1:",
#define OptionS8                  "DISC. 2"  // "折扣项 2:",
  //Options for tend
#define OptionS9                  "OPEN DRAWER"  // "打开钱箱:",
#define OptionS10                 "AMOUNT REQUIRED"  // "输入金额:",
#define OptionS11                 "AMOUNT FORBIDDEN"  // "禁止输入:",
#define OptionS12                 "NUMBER REQUIRED"  // "输入号码:",
#define OptionS13                 "TIP"  // "找零为小费",
  //Options for PoRa
#define OptionS14                 "OPEN DRAWER"  // "打开钱箱:",
#define OptionS15                 "<TENDER>"  // "付款方式键",
#define OptionS16                 "DEPOSIT"  // "存入金额:",
#define OptionS17                 "PO/RA"  // "入金/出金",
  //Option for Disc
#define OptionS18                 "PRINT DISCOUNT"  // "打印折扣额",
#define OptionS19                 "DISCOUNT 1"  // "使用折扣1:",
#define OptionS20                 "DISCOUNT 2"  // "使用折扣2:",
  //Options for currency
  #define OptionS21                 "CHANGE"  // "卖出价找零",
  //Options for Draw
#define OptionS22                 "NO DECIMAL"  // "无小数点:",
#define OptionS23                 "OPEN DRAWER"  // "打开钱箱:",
  //Options for Tax
#define OptionS24                 "TAX ADDON"  // "营业/增值:",
#define OptionS25                 "PRINT VAT"  // "打印税率项",
#define OptionS26                 "PRINT ZERO"  // "打印0税项",
#define OptionS27                 "GST"  // "为GST类税",
#define OptionS28                 "PRINT TAX"  // "打印税额:",
  //Options for clerk
#define OptionS29                 "CORRECT"  // "取消限制:",
#define OptionS30                 "VOID"  // "更改限制:",
#define OptionS31                 "CANCEL"  // "取消交易:",
#define OptionS32                 "REFUND"  // "退货限制:",
#define OptionS33                 "%DISCOUNT"  // "%折扣限制:",
#define OptionS34                 "%SURCHARGE"  // "%加成限制:",
#define OptionS35                 "AMOUNT LIMIT"  // "+-金额限制",
#define OptionS36                 "TRAINING"  // "培训模式:",

#define MESS_ECR_IP         "ECR IP ADDRESS"             //
#define MESS_ECR_ID         "ECR ID(0-9,A-F)"          //

#define EditN1                    "<REC.SQ>NEW REC.\n<ENTER> CUR.REC \n"  //---ZY  //ccr "<记录号>  新纪录\n<确认>  当前纪录\n",
#define EditN2                    "<REC.SQ>A/D ITEM\n<ENTER> CUR.REC \n"   //---ZY  //ccr "<记录号>增删商品\n<确认>  当前商品\n",
#define EditN3                    "<EXIT>      EXIT"                       //---ZY  // "<退出>      退出",
#define EditN4                    "<ENTER> CONTINUE\n"                 //---ZY  // "<确认>      继续\n",
#define EditN5                    "SELECT A REPORT"  // "按<选择>报表",
#define EditN7                    "ASSIGN TO"                    //---ZY  //"键码     ->键   ",
#define EditN8                    "MG REQUIRED"  // "只能在经理档用",
#define EditN9                    "RG REQUIRED"  // "可在营业档用",
#define EditN10                   "TEXT"  // "L打印/显示文字",
#define EditN11                   "(RESERVED)"  // "保留",
#define EditN12                   "FILE/PASSWORD/TEST"  // "文件密码测试",
#define EditN13                   "DIALING"  // "拨号",
#define EditN14                   "CONNECTING!"  // "MODEM 连接!",
#define EditN15                   "CODE "  // "代码:",
#define EditN16                   "%DISCOUNT"  // "%折扣",
#define EditN17                   "%SURCHARGE"  // "%加成",
#define EditN18                   "RECEIPT#:"  // "收据号:",
#define EditN19                   "OPTION"  // "文件数："，0
#define EditN20                   "TOTAL"  // "文件空间："?
#define EditN21                   "REPORT"  // "统计文件"2
#define EditN22                   "TAX"  // “计税”8
#define EditN23                   "LOG MEMORY"  // "流水空间"

#define EditN26                   "BLANKLINE"  // '顶部空行数',
#define EditN27                   "LINAGE"  // '每页行数',
#define EditN28                   "PRINT DATE"  // 'L打印/显示信息',
#define EditN29                   "SECOND RECEIPT"  // '第二张票据',
#define EditN30                   "LEFT MARGIN"  // '左边距',
#define EditN31                   "TYPE"  // '票打机型',
#define EditN32                   "PRINTER OFF"  // "打印机关",
#define EditN33                   "PRINTER ON"  // "打印机开",
  //ChipCard>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN1                "PASSWORD"  //I "密码:",
#define ChipCardN2                "CONFIRM"  // "密码确认:",
#define ChipCardN3                "POS-CODE"  // "POS代码:",
#define ChipCardN4                "CONFIRM"  // "代码确认:",
#define ChipCardN5                "POS-CODE SAVED"  // "代码已保存",
#define ChipCardN6                "POS-CODE ERROR"  // "代码错 ！",
#define ChipCardN7                "CHIP CARD"  // "启用IC卡:",
#define ChipCardN8                "DISCOUNT"  // "折扣卡:",
#define ChipCardN9                "CASH"  // "现金卡:",
#define ChipCardN10               "CREDIT"  // "赊帐卡:",
#define ChipCardN11               "PRINT"  // "打印IC卡",
#define ChipCardN12               "AUTO DISCOUNT"  // "自动折扣:",
#define ChipCardN13               "POINTS"  // "消费积分:",
#define ChipCardN14               "CASH=CC"  // "用现金键:",
#define ChipCardN15               "EXPIRED DATE"  // "启用有效期", //
#define ChipCardN16               "LOST LIST"  // "挂失卡号",
#define ChipCardN17               "SERIAL#"  // "卡号:",
#define ChipCardN18               "TYPE"  // "卡类型:",
#define ChipCardN19               "BALANCE"  // "余额"
#define ChipCardN20               "FOREGIFT"  // "卡押金:",
#define ChipCardN21               "SPENDING"  // "消费:",
#define ChipCardN22               "CHARGE"  // "充值总额:",
#define ChipCardN23               "RECEIPTS"  // "使用次数:",
#define ChipCardN24               "PRICE LEVEL"  // "价格级别:",
#define ChipCardN25               "PIN"  // "PIN码:",
#define ChipCardN26               "PROTECT LEVEL"  // "保护级别:",
#define ChipCardN27               "AUTO TENDER"  // "自动结帐:",
#define ChipCardN28               "INITIALIZED DATE"  // "制卡日期:",
#define ChipCardN29               "EXPIRED DATE"  // "卡有效期:",
#define ChipCardN30               "HOLDER"  // "客户:",
#define ChipCardN31               "CREATE"  // "初始化IC",
#define ChipCardN32               "CHARGE"  // "IC卡充值",
#define ChipCardN33               "CLEAR CARD"  // "清除IC卡",
#define ChipCardN34               "CHARGE?"  // "充值/退款?",
#define ChipCardN35               "REDEMPTION?"  // "减点/送点?",
#define ChipCardN36               "CONFIRM?"  // "确认充值?",
#define ChipCardN37               "CONFIRM?"  // "确认退款?",
#define ChipCardN38               "CONFIRM?"  // "确认送点?",
#define ChipCardN39               "CONFIRM?"  // "兑奖确认?",
#define ChipCardN40               "REDEMPTION PRIZE"  // "计点兑奖",
#define ChipCardN41               "WITHDRAWAL"  // "IC卡退款",
#define ChipCardN42               "REDEMPTION PRIZE"  // "IC卡兑奖点数",
#define ChipCardN43               "PONITS"  // "IC卡送点数",
#define ChipCardN44               "PLEASE WAIT......"  // "请等待......",
#define ChipCardN45               "PLEASE REFUND FIRST...."  // "卡内有金额,请先退款....",
#define ChipCardN48               "RESTART ECR AFTER NEW IP"  // "要启用新的IP,请重新开机.",
#define ChipCardN49               "CONFIRM CLEAR?"  //  "确认清报表?",
#define ChipCardN50               "WAITTING..."  // "等待IC卡....",
#define ChipCardN51               "DOUBLE HEIGHT" //---zy  // "倍高L打印/显示",

//ccr2015-06-15>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN52             "CLEAR"//
#define ChipCardN53             "REFUND" //
#define ChipCardN54             "PAYMENT"  //
#define ChipCardN55             "CHARGE"//
#define ChipCardN56             "INITIALIZATION"//
#define ChipCardN57             "CASH OUT"
#define ChipCardN58             "ADD POINTS"   //ADDVIPPOINTS
#define ChipCardN59             "DEC POINTS"   //DECVIPPOINTS
#define ChipCardN60             "SUM POINTS"  //VIPPOINTS
//ccr2015-06-15<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MESSEJ_X_STORE      "SAVE X DATA"//MsgEJ_X_STORE
#define MESSEJ_Z_STORE      "SAVE Z DATA"//MsgEJ_Z_STORE
#define MESSEJ_TR_STORE     "SAVE TRAIN"//MsgEJ_TR_STORE
#endif
//ccr2016-02-22>>>>>>>>>>>>>>>>>>>>>
#define MsgMENUXREPORT     "X-REPORT"               //
#define MsgMENUXEXPORT     "EXPORT(X)"               //
#define MsgMENUQUICKVIEW   "GRAND TOTAL"               //
#define MsgMENUXLOGFUNC    "RECEIPT LOG"               //
#define MsgMENUSENDNEWLOG  "SEND NEW LOG"      //MENUSENDNEWLOG  //发送新流水数据
#define MsgMENUXPASSWORD   "X-PASSWORD"               //
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUXEJOURNAL    "EJ"     //MENUXEJOURNAL   "E Journal"
#endif
//.................................
#define MsgMENUZREPORT     "Z-REPORT"               //
#define MsgMENUZEXPORT     "EXPORT(Z)"               //
#define MsgMENUCLEARLOG    "CLEAR LOG"               //
#define MsgMENUZGRAND      "CLEAR TOTAL"               //显示
#define MsgMENUZPASSWORD   "Z-PASSWORD"               //
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUZRESETEJ   "RESET EJ"     //MENUZRESETEJ   "Clear EJ"
#endif
//...................................


#define EPosN1                    "EPOS CODE:"  // "ePOS认证码:",
#define EPosN2                    "WAITING..."  // "等待易通卡"
#define EPosN3                    "CARD NUM:"  // "易通卡号:",
#define EPosN4                    "BALANCE:"  // "易通卡余额:",


#define ErrInfoN1                 "DATA OVERSIZE"  // "通讯数据过长"
#define ErrInfoN2                 "**** PRINT INTERRUPTED *****"  // " ***打印被中断、重新打印 *** "
#define ErrInfoN3                 "CRC ERROR!"  // "数据校验错"
//PRINT



#define FisTextN1                 "TAXPAYER ID"   //"纳税人号:"
#define FisTextN2                 "PRINT INVOICE"   //"开出税票"
#define FisTextN3                 "AUTHORIZED NUMBER "   //"授权号码"
#define FisTextN4                 "SERIAL NUMBER"   //"写机器号"
#define FisTextN5                 "SERIAL# CONFIRM "   //"写入确认"
#define FisTextN6                 "DATA      PRINT "   //"数据印出"
#define FisTextN7                 "USER        ID  "   //"用户号码"
#define FisTextN8                 "TAX-EXEMPT      "   //"免税状态"
#define FisTextN9                 "Z-COUNT         "   //"Z-计数:"
#define FisTextN10                "TAXABLE    SUM  "   //"累计销售"
#define FisTextN11                "TAX        SUM  "   //"累计税"
#define FisTextN12                "TOTAL TAX-EXEMPT"   //"总免税额"
#define FisTextN13                "TOTAL   TAXABLE "   //"总销售额"
#define FisTextN14                "TOTAL    TAX    "   //"总税额"
#define FisTextN15                "DAILY RECEIPTS"   //"日税票数"
#define FisTextN16                "TAX RECPT. SUM  "   //"收据总数"
#define FisTextN17                "REPORT   SUM    "   //"余报表数:"





#define CurrentTimeN1                    "TIME:"      //  "当前时间"
#define InputMode1				"A:" // "A："

#define SpecialSymbol1			"<<"			//DOWNLOAD//Ccr "★",
#define SpecialSymbol2			">>"			//UPLOAD//Ccr "☆",
#define SpecialSymbol3			"->  <-"		// "↑  ↓",
#define MESS_PRESSKEY   "PRESS ANY KEY..."//
#if defined(CASE_QRCODE)
#define InputQRCode           "QRCODE"  //MsgQRCODE
#endif

#define GPRSServerIP			"SERVER IP"//"服务器IP"//
#define GPRSServerPort		    "PORT"//"端口号"显示

/////////////////////////////////////////////////////////
//added by lqw 090802
#if  (defined(CASE_GPRS))
#define GPRSAPNName        "APN"           //GPRSAPNNAME
#define GPRSUserName        "User Name"           //GPRSUSERNAME
#define GPRSPassword        "Password"           //GPRSPASSWORD

#define GPRSSendMESS        "Check GPRS" //ccr2014-11-11 NEWSETUP
#define GPRSSetMode        "Set Sent Mode"    //gprsSETMODE //ccr2014-11-11 NEWSETUP-step2
#define GPRSSendECRLogAll   "Send All LOG"    //gprsSendECRLogAll //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadPLU     "Update PLU"    //gprsDownloadPLU //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadDEPT    "Update DEPT"    //gprsDownloadDEPT //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadCLERK   "Update CLERK"    //gprsDownloadCLERK //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadHEAD    "Update HEAD"       //gprsDownloadHEAD //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadTRAIL   "Update TRAIL"      //gprsDownloadTRAIL //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadALL     "Update ALL"      //gprsDownloadALL //ccr2016-08-18
#define GPRSRestart         "RESET GPRS UNIT"      //gprsRestart //ccr2016-08-26

#define GPRSstr1 "DATA ERROR!"  // "数据类型错"
#define GPRSstr2 "GPRS OK"//
#define GPRSstr3 "GPRS NOT OK"        //
#define GPRSstr4 "CONFIRM?"//
#define GPRSstr5 "IP/PORT:NULL"//
#define GPRSstr6 "ERROR:IP/PORT"//
#define GPRSstr7 "WAITING GPRS"//
#define GPRSstr8 "YES"   // "是"
#define GPRSstr9  " NO"    // "否 "
#define GPRSstr10 "TRANSMITTING..."  // "数据发送中.. "
#define GPRSstr11 "TRANSMISSION FINISHED"  // "数据已发出.. "
#define GPRSstr28 "SUCCESS........"		// "发送成功.."
#define GPRSstr31 "FAILURE ON CONNECT"	//	// "连接失败"
#define GPRSstr32 "FAILURE ON READ"         // "接受失败"
#define GPRSstr33 "CONFIRMA ERR"	// "发送确认失败"
#define GPRSstr34 "FAILURE........"			// "发送失败"
#define GPRSstr58 "CONNECT........"		// "正在建立连接.."
#define GPRSstr59 "CONNECT........"		// " 准备建立连接.."
#define GPRSstr60 "RESTART GPRS"	// "正在复位模块.."
#define GPRSstr61 "END OF SEND." // "发送完毕,请退出"
#define GPRSxACK  "ACK NOT RECEIVED"
#endif //end defined(CASE_GPRS)
///////////////////////////////////////////////////////////////////
#define InsertUSB		"<<Insert USB"//CHRUSB  插入U盘 //

//ccr090715>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define usbMAIN1		"COPY TO USB"			//usbDAOCHU	导出收款机文件 //
#define usbMAIN2		"COPY FROM USB"			//usbDAORU 导入收款机文件 //
#define usbMAIN3		"LOG TO USB"		//usbFLOW 导出销售流水 //


#define usbFLOWNEWMESS	    "EXPORT UPDATES"      //1-导出新数据   //
#define usbFLOWALLMESS	    "EXPORT ALL"      //2-导出全部数据
#define usbFLOWDELMESS	    "RESET AFTER EXPORT"    //3-导出全部数据后清空

#define usbALLFile		"ALL FILES"		//usbALL 全部文件 //


#define MESSPRNMENU     "<00>Print IT"         //
#define MESSSETMENU     " MANUALS LIST"  //
#define MESSCLEANLOG    "Clear LOG?  "  //
#define MESSCLEANLOGA   "All of LOG\nwill be cleared!" //
#define MESSRAMOUT      "RAM Overflow!HLT"   //
#define MESSCLEANFILES  "Clear Files?"   //
#define MESSCONFIGERR   "Config changed,\nError will be if\nuse the old file"  //
#define MESSDELPLU      "Variable Changed\n     PLU deleted"  //
#define MESSPLUPRINTEND "PLU Printed O.K.\nPress <ENTER>!" //



#define RECALLFLOW      "Recall LOG"  //

#define USBREPNAME      "Name"  //

////////  CSV导出文件标题  ///////////////////////////////////////////////
#define DEPTCAP       "Random,Name,Group,Print,Options\r\n"          //
#define PLUCAP        "Random,Name,OFFIndex,Dept,DeptHi,Price1,Price2,Price3,Price4,,Cost,Link,Inventory\r\n"//

#define TAXCAP        "Name,Rate,Options\r\n"//
#define HEADERCAP     "Captions on the receipt head\r\n"//
#define DISCCAP       "Name,Print,Options,Fixed[3],Tax,Max\r\n"//

#define SALPERCAP     "Name,Options\r\n"//
#define SYSTEMCAP     "System Flags\r\n"//
#define COMMCAP       "Name,Type,Prot,Telephone\r\n"//
#define GRAPPRNCAP    "PictNo,DateFrom,DateTo\r\n"//
#define GROUPCAP      "Name,Max\r\n"//
#define ICCARDCAP     "Sets for Chip Card\r\n"//
#define PROMCAP       "Freq,JollyMin,PointVal,PointMin,GrapIdx,Memo\r\n"//
#define PBFUNCCAP     "Name,Print,Options\r\n"//
#define CLERKCAP      "Name,Options,Passwd\r\n"//
#define MODICAP       "Captions for modify\r\n"//
#define PBTOTALCAP    "Random,Name\r\n"//
#define ZONECAP       "Zone1,Zone2,Zone3,Zone4,Zone5,Zone6,Zone7,Zone8,Zone9,Zone10,Zone11,Zone12\r\n"//
#define KEYBOARDCAP   "Defitions for keyboard"//
#define OFFCAP        "Name,Type,DateFrom,DateTo,TimeFrom,TimeToWeekDay,Discount,NumItems,PriceU,PriceP\r\n"//
#define PORACAP       "Name,,Print,Options,Drawer,Max\r\n"//

#define TRAILERCAP    "Captions on the receipt trail\r\n"//
#define SLIPHEADCAP   "Captions on the receipt Slip\r\n"//
#define ICBLOCKCAP    "Chip card blocked\r\n"//


#define EXPORTCSV    "1-CSV,2-BIN:"//

#define CSVFLOETITL "Name,Quantity,Amount,Receipt,Clerk,SalesMan,DateTime,Register,ShopNo,Type\r\n"//
#define CSVREPTITL  "DateTime,Register,ShopNo\r\n"//

#define BUTTONBATLOW "S.Battery Low!"//
#define CHARGEBATNOW "O.Battery Low!\nCharge Now..."//

#define SRAMNOTFOUND "SRAM not found!\nSystem HALT!"//

#define INPUTPLUCODE "PLU CODE:"//

#define MessageE18      "!!!! FAT(PLU) ERROR !!!!"//

#define MESS_TABLEHOLD  "Table    Suspend"//

#define MESS_LOGREPET   "Re-Send All LOG!"//

#define MESS_PAYALSO   "Confirm Payment?"//


//ccr2016-01-12>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//...............................................
#define MGADDINV        "INVENTORY+"  //1+SUBTOTAL//
#define MGDECINV        "INVENTORY-"  //2+SUBTOTAL//
#define MGSETPRICE      "PRICE PLU"  //3+SUBTOTAL//

#define MGCLEARIC       "REFUND IC"//
#define MGINITIC        "INIT IC"//
#define MGCHARGE        "CHARGE IC"//
#define MGADDPOINT      "ADD POINT"//
#define MGPRINTSTOCK   "LIST STOCK"

#define MGPASSWORD      "SET PASSWORD"//
#define MGCLOSEPRN      "CLOSE PRINT"//
#define MGTRAINING      "TRAINING MODE"//
#define MGGOTOMG        "MANAGEMENT"//

//..............................................
#define PLUCODE     "PLU CODE"//
#define INVQTYMUST  "INPUT QUANTITY/PRICE"//
#define ININVENTORY "Press <SUBT> Key\n Exit Inventory!"//
//..............................................
#define DEVICENULL		" NULL"//
#define DEVICEPORT1     "PORT#1"//
#define DEVICEPORT2     "PORT#2"//
#define DEVICEPORT3     "PORT#3"//
#define DEVICEPORT4     "PORT#4"//

//................................................
#define CLOSEPRINTER    "CLOSE PRINTER"//
#define CLEARGRANDTOTAL "CLS G.T AFTER Z"//
#define CLEANALLECRVIP  "CLEAN ACCOUNT?"//"清除会员数据?"
//.......功能码对应的名称.........................................
#define MsgKEYCORR      "CORRECT"//
#define MsgKEYCURR      "CURRENCY"//
#define MsgKEYDISC      "DISCOUNT"//显示
#define MsgKEYPBFUN     "PB-FUNC"  //
#define MsgKEYPORA      "PORA"//
#define MsgKEYPO_RA      "PO/RA"
#define MsgKEYTEND      "TENDER"//
#define MsgKEYCLEAR     "CLEAR"//
#define MsgKEYFEED      "FEED"//
#define MsgKEYMULT      "X"//
#define MsgKEYSUBTOT    "SUBTOTAL"//
#define MsgKEYPRICE     "PRICE"//
#define MsgKEYPRICEN    "PRICE#"//
#define MsgKEYPRICE1    "PRICE1"//
#define MsgKEYPRICE2    "PRICE2"//
#define MsgKEYPRICE3    "PRICE3"//
#define MsgKEYPRICE4    "PRICE4"//
#define MsgKEYSHIFT     "SHIFT"//
#define MsgKEYDATE      "DATE"//
#define MsgKEYWEIGHT    "WEIGHT"//
#define MsgKEYCLERKN    "CLERK#"//
#define MsgKEYDEPT      "DEPT~"//
#define MsgKEYDEPTNo    "DEPT#"//
#define MsgKEYPLUNo     "PLU#"//
#define MsgKEYDRAW      "OPEN DRAWER"//
#define MsgKEYMEMO      "MODIFIER"//
#define MsgKEYCLERK     "CLERK*"              //显示
#define MsgKEYPLUDIR    "PLU~"//
#define MsgKEYLOCK      "MODE KEY"//
#define MsgKEYDRAWFUN   "DRAW FUNC*"//
#define MsgKEYSALPN     "SALESMAN#"//
#define MsgKEYSALP      "SALESMAN"//
#define MsgKEYDOT       "'.'"//
#define MsgKEYZERO2     "'00'"//
#define MsgKEYNUMBER    "0~9"//
#define MsgKEYSUSPEND   "HOLD"//
#define MsgFUNCLOOK1    "FUNC LOOK1"//
#define MsgFUNCLOOK2    "FUNC LOOK2"//
#define MsgMODELOCK     "MODE KEY"
#define MsgVIPLOGIN     "ACCOUNT LOGIN"
#define MsgINPUTNUM     "INPUT CODE"
#define MsgCUSTOMER     "CUSTOMER"
#define MsgKEYNULL      "(OTHERS)"//

#define MsgRHEADER      "Header*"
#define MsgRTRAILER     "Trail*"
#define MsgKEYBOARD     "Keyboard*"
#define MsgSYSFLAGS     "System*"
#define MsgSETDATE      "Datetime*"

//......................................
#define MsgCONFIRM      "CONFIRM"//
#define MsgNET          "NET AMOUNT"  // "净额",//
#define VIPLOGOUT       "ACCOUNT LOGOUT"
#define VIPTOTAL        "AMOUNT"
#define VIPTOTALA       "SUM-AMOUNT"
#define DECVIPPOINT     "DEC."//长度必须为4
#define ADDVIPPOINT     "ADD."//长度必须为4
#define INPUTVIPNAME    "NAME:"
#define PRINTKEYBOARD   "PRINT KEYBOARD"
//...........................................
#define MESSALLLOG         "LOG BY REC.NO"      //
#define MESSDATERANGE     "LOG BY DATE"         //
#define MESSVIPLOG        "ACCOUNT LOG"                            //
#define MESSPRNCASH       "PRINT CASH"		//"打印付款记录"
#define MESSPRNDEPART     "PRINT DEPART"	//"打印部类记录"
#define MESSPRNPLU        "PRINT PLU"		//"打印单品记录"
#define MESSPRNDISCOUNT   "PRINT DISCOUNT"	//"打印折扣加成"
#define MESSPRNRAPO       "PRINT RA/PO"		//"打印出入金"
#define MESSPRNCORRECT    "PRINT CORRECT"	//"打印更正操作"
#define MESSPRNCANCEL     "PRINT CANCEL"   //"打印交易取消"
#define MESSPRNNOSALE     "PRINT NOSALE"	//"打印非销售"

#define MESSALLCASH		  "ALL OF CASH"		//"全部付款方式"
#define MESSSELCASH		  "ONE OF CASH"		//"选择付款方式"
#define MESSALLDEPT		  "ALL OF DEPT"		//"打印全部部类"
#define MESSSELDEPT		  "ONE OF DEPT"		//"选择一个部类"
#define MESSALLPLU		  "ALL OF PLU"		//"打印全部单品"
#define MESSSELPLU		  "ONE OF PLU"		//"选择一个单品"
//........................................
//ccr2016-03-15>>>>定义键盘宏>>>>>
#define MESSMACRO         "MACRO"//  "宏定义"
//....ListItems提示信息....................................
#define It_MGITEMS      "MG FUNCTION"//
#define It_SELEFUNC     "KEY FUNCTION"  //
#define It_SELEITEM     "SELECT ITEM"                   //
#define It_SELEKEYTYPE  "KEYBOARD TYPE"                  //
#define It_SELETEST     "SELECT ITEM"                       //
#define It_SELELOG      "SELECT LOG"                //
#define It_SELETEND     "SELECT TYPE"//
#define It_SELEDEPT     "SELECT DEPT"//
#define It_SELEPLU      "SELECT PLU"//
#define It_SELEMODI     "SELECT MODIFIER"//
#define It_SELEREPORT   "SELECT REPORT"               //
#define It_SELEPOINT    "SELECT POINT"// 小数位数"
#define It_SELEEJ       " EJ      "
//.........................................................
#define MESSRAMVAR      "VARIABLE"//
//.........................................................
#define PLUNUMBER           "RECORDE COUNT:"//
#define PLURANDOMSIZE     	"SIZE OF CODE:"//
#define PLUNAMESIZE			"SIZE OF NAME:"//
#define PLUPRICESIZE        "SIZE OF PRICE:"//
#define PLUPRICELEVEL       "ACCOUNT PRICE:"//
#define PLUCOSTSIZE         "SIZE OF COST:"//
#define PLUINVENTSIZE       "SIZE OF STOCK:"//
#define PLUREPQTYSIZE       "SIZE OF REP-QTY:"//
#define PLUREPAMTSIZE       "SIZE OF REP-AMT:"//
#define PLUREPDISCSIZE      "SIZE OF REP-DISC:"//
#define PLUREPCOSTSIZE      "SIZE OF REP-COST:"//
//.........................................................
#define POINTTYPE           "POINT TYPE"
#define POINTTYPE1          "1.000,00"  //两位小数  ,为小数点  .为分隔符号
#define POINTTYPE2          "1,000.00"  //两位小数  .为小数点  ,为分隔符号
#define POINTTYPE3          "   1.000"     //无小数   .为分隔符号
#define POINTTYPE4          "   1,000"     //无小数   ,为分隔符号
#define POINTTYPE5          "1.000,000"  //三位小数  ,为小数点  .为分隔符号
#define POINTTYPE6          "1,000.000"  //三位小数  .为小数点  ,为分隔符号
//#define POINTTYPE7          "1.000,0"  //一位小数  ,为小数点  .为分隔符号
//#define POINTTYPE8          "1,000.0"  //一位小数  .为小数点  ,为分隔符号
//.......................................................
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define MESSEJMESSAGES      "ABOUT EJ"    //MsgEJMESSAGES
#define MESSEJALLEJ         "ALL OF EJ"
#define MESSEJDATERANGE     "EJ BY DATE"
#define MESSEJRECEIPT       "EJ OF RECEIPT"
#define MESSEJXREPORT       "EJ OF X-REPORT"
#define MESSEJZREPORT       "EJ OF Z-REPORT"
#define MESSEJCLERK         "EJ OF CLERK"
#endif
//.......................................................
#define DATETYPE        "FORMAT "
#define DDMMYYYY        "DDMMYYYY"
#define MMDDYYYY        "MMDDYYYY"
#define YYYYMMDD        "YYYYMMDD"
//.......................................................
#define XUPDATEBIOS     "Can't Update firmware\nby USB!"

//ccr2016-06-22..........................................
#if defined(CASE_QRCODE)
#define MESSWXPAY       "OK WX-PAYMENT"
#endif

#define SENDLOGAUTO     "SEND LOG AUTO:"
#define DAYSFORLOG      "DAYS FOR SEND:"
#define ACKMUSTNEED     "ACK NEEDED:"

#define CSV_TITLE       "CAPTION,QUANTITY,AMOUNT\r\n"

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define Msg_FORMAT      "ERASE EJ"
#define Msg_FORMATED    "SD ERASED"
#define Msg_RECALLEJ    "RECALL EJ"
#define Msg_EJNOTEMPTY  "EJ NOT EMPTY"
#define Msg_ECRDATEltEJ "ECR DATE < EJ DATE"

#endif

//ccr2017-01-10>>>>>>>>>>>>>>>>>>>>>>>>>
#define Msg_NOSTOCK     "NO STOCK FIELD"
#define Msg_DISPSTOCK   "DISPLAY/PRINT?"
//ccr2017-01-10<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
