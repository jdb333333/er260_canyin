#ifndef MESSAGEH_EN_P
#define MESSAGEH_EN_P

// Printer
#if (PRTLEN>40)
                       //.123456789.123456789.123456789.123456789.1
#define Header1         "*       THANKS    FOR    SHOPPING        *"        //" 	 创 造 优 秀  优 创 造 就   *"
#define Header2         "*     Eutron      Cash     Register      *"         //" 	 创 造 优 秀     优 创 造 就   *"

#else
                       //.123456789.123456789.123456789.1
#define Header1         "*     THANKS FOR SHOPPING      *"        //" 	 创 造 优 秀  优 创 造 就   *"
#if defined(CASE_INDONESIA)
#define Header2         "*   Posibolt Cash Register     *"         //" 	 创 造 优 秀     优 创 造 就   *"
#else
#define Header2         "*     Eutron Cash Register     *"         //" 	 创 造 优 秀     优 创 造 就   *"
#endif
#endif
// Printer
#define ReportType0     "  GRAND TOTAL   "        //"   总  销  售   ",
#define ReportType1     "  GROUP         "        //" 柜  组  销  售 ",//1
#define ReportType2     "  DEPARTMENT    "        //" 部  类  销  售 ",//2
#define ReportType3     "  PLU           "        //" 商  品  销  售 ",/* 3 Used with ApplVar.Plu ApplVar.Report */
#define ReportType4     "  TENDER        "        //" 付  款  信  息 ",//4
#define ReportType5     "  PORA          "        //" 出 入 金 信 息 ",//5
#define ReportType6     "  DRAWER        "        //" 钱  箱  信  息 ",//6
#define ReportType7     "  CORRECTION    "        //" 更  改  信  息 ",//7
#define ReportType8     "  DISCOUNT      "        //" 折  扣  信  息 ",//8
#define ReportType9     "  CURRENCY      "        //" 外  币  信  息 ",//9
#define ReportType10    "  TAX           "        //" 计  税  信  息 ",//10
#define ReportType11    " PB FUNCTIONS   "        //" 桌  帐  信  息 ",//11
#define ReportType12    "PREVIOUS BALANCE"        //" 开  台  信  息 ",//12
#define ReportType13    "  TABLES        "        //" 桌  台  信  息 ",//13
#define ReportType14    "  INVENTORY     "        //" 商  品  库  存 ",//14
#define ReportType15    "  PB INVOICES   "   //
#define ReportType16    "  TENDER BUFFER "    //
// Printer
#define ReportType17    "  CLERK TOTAL   "  //"    收 款 员 统 计   ",
#define ReportType18    "  SALPER TOTAL  "  //"    营 业 员 统 计   ",
// Printer
#define Trailer1        "        THANK YOU       "        //"     ~多  ~谢  ~光  ~临     ",
#define Trailer2        "        GOODBYE       "        //"          ~再  ~见         ",


#define ReportList1     "CLERK DAILY"        //" 收 款 员 日 报 ",
#define ReportList2     "CLERK PERIOD"        //" 收 款 员 周 报 ",
#define ReportList3     "DAILY REPORT"        //" 销  售  日  报 ",
#define ReportList4     "PERIOD REPORT"        //" 销  售  周  报 ",
#define ReportList5     "PLU  REPORT"        //" 商  品  报  表 ",
#define ReportList6     "TABLE REPORT"        //" 桌  帐  报  表 ",
#define ReportList7     "TIME ZONE"        //" 时  段  报  表 ",
#define ReportList8     "ALL CLERK DAILY"        //"  全收款员日报  ",
#define ReportList9     "ALL CLERK PERIOD"        //"  全收款员月报  ",
#define ReportList10    "SALESPER.DAILY"        //" 营 业 员 日 报 "
#define ReportList11    "SALPR.PERIODICAL"        //" 营 业 员 周 报 "
#define ReportList12    "ALL SALPR.DAILY"        //"  全营业员日报  ",
#define ReportList13    "ALL SALP.PERIOD"        //"  全营业员月报  ",

#define Promotion1      "  CONGRATULATIONS   "        //"  ~恭  ~喜  ~中  ~奖",

#define Correc1         "CORRECT "        //"更改    "//ccr2016-07-14
#define Correc2         "VOID    "        //"取消    "//ccr2016-07-14
#define Correc3         "REFUND  "        //"退货    "
#define Correc4         "CANCEL  "        //"取消交易"
#define Correc5         "SUSPEND "      //"挂账"


#define CURRENCY1         "DOLLAR  "        //"美元    "L打印/显示
#define CURRENCY2         "YEN     "        //"日元    "L打印/显示
#define CURRENCY3         "HK $    "        //"港币    "L打印/显示
#define CURRENCY4         "CURR   1"        //"外币1   "L打印/显示
#define CURRENCY5         "CURR   2"        //"外币2   "L打印/显示

#define DISCOUNT1          "+%"        //"+%加成"     L打印/显示
#define DISCOUNT2          "-%"        //"(-%)折扣"   L打印/显示
#define DISCOUNT3          "+NET"        //"金额加成"    L打印/显示
#define DISCOUNT4          "-NET"        //"金额折扣"    L打印/显示
//#define DISCOUNT5          "+% ON PLU"   // zy //"单项加成"
//ccr090812 for SubDisc>>>>>>>>>>>>>>>>>>>>>>>>>
#define DISCOUNT6          "SUBTOTAL+%"   //  "+%小计加成"L打印/显示
#define DISCOUNT7          "SUBTOTAL-%"   //  "-%小计折扣"L打印/显示
#define DISCOUNT8          "SUBTOTAL+"   // "小计金额加成"L打印/显示
#define DISCOUNT9          "SUBTOTAL-"   // "小计金额折扣"L打印/显示

#define DRAWER1            "CASH    "        //"现金    "L打印/显示
#define DRAWER2            "CHECK   "        //"支票    "L打印/显示
#define DRAWER3            "CREDCARD"        //"信用卡  "L打印/显示
#define DRAWER4            "COUPON  "        //"购物券  "L打印/显示
#define DRAWER5            "CREDIT  "        //"赊账    "L打印/显示
#define DRAWER6            "CHIPCARD"        //"IC卡    "L打印/显示
#if  defined(CASE_INDONESIA)
#define DRAWER7            "DEBIT   "        //"小费    "L打印/显示
#else
#define DRAWER7            "FEE     "        //"小费    "L打印/显示
#endif
#define DRAWER8            "DRAW-8  "//打印/显示

#define PBFunction0         "OPEN    "        //   "开台挂帐"
#define PBFunction1         "ADD     "        //   "开台增加"
#define PBFunction2         "SERVICE "        //   "暂结"
#define PBFunction3         "FINALIZE"        //   "确认结帐"
#define PBFunction4         "PRINT PB"        //   "打印开台"
#define PBFunction5         "PRT BILL"        //   "打印帐单"
#define PBFunction6         "CANCEL  "        //   "取消确认"
#define PBFunction7         "SPLIT   "        //   "分帐"
#define PBFunction8         "TRANSFER"        //   "转帐"
#define PBFunction9         "MOVE TO "        //   "转桌帐"
#define PBFunction10        "COVER   "        //   "人数"


#define PORAType1            "PO      "        //"出金    "
#define PORAType2            "RA      "        //"入金    "
#define PORAType3            "RETURN"        //"IC卡退款"
#define PORAType4            "CHARGE  "        //"IC卡充值"
#define PORAType5            "DEPOSIT "        //"退IC押金"
#define PORAType6            "FOREGIFT"        //"收IC押金"

#define TendType1             "CASH       "        //"现金    "
#define TendType2             "CHECK      "        //"支票    "
#define TendType3             "CREDIT CARD"        //"信用卡  "
#define TendType4             "COUPON     "        //"购物券  "
#define TendType5             "CREDIT     "        //"赊账"
#define TendType6             "CHIPCARD   "        //"ICCard"
#define TendType7             "DEBIT CARD "        //"DEBIT CARD"
#define TendType8             "CASH-8     "        //"CASH-8"


#define OperationN3             "TOTAL"  // "总计",
#define OperationN4             "CONVERT"  // "转换",
#define OperationN5             "SUBTOTAL"  // "小计",
#define OperationN6             "PO"  // "出金",
#define OperationN7             "RA"  // "入金",
#define OperationN8             "REPORT"  // "报表",
#define OperationN9             "ADD"  // "增加",
#define OperationN10            "HOLD"  // "挂单,
#define OperationN11            "VOID"  // "取消",

#define OperationN14            "PRICE"  // "单价:",     //
#define OperationN15            "AMOUNT"  // "金额:",//

#define CaptionN2               "NET AMOUNT"  // "净额",
#define CaptionN4               "TOTAL TRADE"  // "交易数",
#define CaptionN5               "QUANTITY"  // "数量",
#define CaptionN6               "AMOUNT"  // "金额",
#define CaptionN7               "COLLECT"  // "收取",
#define CaptionN9               " "  // "日",
#define CaptionN10              " "  // "周",
#define CaptionN11              "-"  // "月",
#define CaptionN12              "-"  // "年",
#define CaptionN13              "STANDARD"  // "标准",
#define CaptionN14              "CLERK"  // "收款员",
#define CaptionN15              "   HOUR"  // "时",
#define CaptionN16              "  "  // "   日",
#define CaptionN17              " --"  // "   月",
#define CaptionN18              "REFUND"  // "退货",
#define CaptionN19              "COST"  // "成本",
#define CaptionN20              "TABLE# "  // "桌号#",
#define CaptionN21              "CHEQUE#"  // "支票号#",
#define CaptionN22              "ITEMS"  // "项数:",
#define CaptionN23              "TAXABLES"  // "含税销售",
#define CaptionN24              "TAX "  // "税额",
#define CaptionN25              "PAGE"  //  "页号",
#define CaptionN26              "NOADD#"  // "号码:",
#define CaptionN27              "SERVICE"  // "挂帐",
#define CaptionN28              "PREVIOUS BALANCE"  // "开台",
#define CaptionN29              "Z-COUNT"  // "清帐计数",
#define CaptionN30              "CLEAR"  // "清除",
#define CaptionN31              "S-REPORT"  // "S-报表",什么是S-REPORT? ZWQ
#if HOR_REPORT==1
#define CaptionN32              "GROSS PROFIT"  // "毛利",
#else
#define CaptionN32              "GROSS PROFIT"  // "毛利",
#endif
#define CaptionN33              "STOCK INCREASED"  // "库存增减",//
#define CaptionN331             "STOCK DECREASED"  // "库存增减",//
#define CaptionN34              "FORMER PB"  // "原桌帐:",
#define CaptionN35              "CREDIT CARD#"  // "信用卡#",
#define CaptionN36              "NEW TABLE"  // "新桌帐:",
#define CaptionN37              "GUESTS"  // "人数",
#define CaptionN38              "EXCHANGE"  // "换货",

//Dispaly
#define CaptionN41              "DISCOUNT 1"  // "折扣项1",
#define CaptionN42              "DISCOUNT 2"  // "折扣项2",
#define CaptionN43              "SALES AMOUNT"  // "销售额",
#define CaptionN44              "MIX-MATCH"  // "大包优惠",
#define CaptionN45              "STOCK"  // "库存数量",
#define CaptionN46              "OFFER"  // "特价优惠",
#define CaptionN51              "OPEN"  // "开台暂结",
#define CaptionN52              "ADD"  // "开台增加",
#define CaptionN53              "SERVICE"  // "暂结",
#define CaptionN54              "FINALIZE"  // "确认结帐",
#define CaptionN55              "PRINT"  // "打印开台",
#define CaptionN56              "PRINT BILL"  // "打印帐单",
#define CaptionN57              "CANCEL"  // "取消确认",
#define CaptionN58              "SPLIT"  // "分帐",
#define CaptionN59              "TRANSFER"  // "转帐",
#define CaptionN60              "MOVE TO"  // "更换收款员",信息为"MOV"
#define CaptionN61              "GUSET"  // "顾客人数",


#define CaptionE11              "CLERK"  // "收款员:",
#define CaptionE12              "SALESMAN"  // "营业员:",
#define CaptionE13              "ECR#"  // "收款机号:",
#define CaptionE14              "SHOP#"  // "位置:",

#define CaptionE17              "VERSION"  // "发行版本:",
#define CaptionE18              "POINTS"  // "本次消费点",
#define CaptionE19              "ADD PLU"  // "新增单品:",
#define CaptionE20              "DELETE PLU"  // "删除单品:",
#define CaptionE21              "TEST START"  // "开始检测>>",
#define CaptionE22              "TEST COMPLETED"  // "<<检测结束",
#define CaptionE23              "INVENTORY"  // "库存",
#define CaptionE24              "REPORT CLEARD!"  // "报表已全清!",


#define CaptionE31              "MODIFIER"  // "说明文字",


#define MessagesN5                "*** TRAINING MODE  ***"  // "*** 培训状态 ***",
#define MessagesN6                "***** POWER ON*****"  // "***** 加电 *****",
#define MessagesN7                "*** SPLIT TABLE ***"  // "*** 分开付帐 ***",
#define MessagesN8                "**DUPLICATE RECEIPT ***"  // "*** 复制收据 ***",
#define MessagesN9                "**** SET PRICE PLU ****"  // "*** 商品变价 ***",//
#define MessagesN10               "**INVENTORY+ **"  // "*** 增加库存 ***",
#define MessagesN101              "**INVENTORY- **"  // "*** 减少库存 ***",
#define MessagesN11               "***** CANCEL *****"  // "***** 取消 *****",
#define MessagesN12               "** INITIALIZING **"  // "** 程序初始化 **",

#define EditN24                   "FILE           RECORDS      RAM"  // "文件名      记录数      空间大小"
#define EditN25                   "PORT           TYPE    PROTOCOL"  // "串口名称    通信类别   通信协议"

//PRINT                          /*123456789012345678901234567890123456*/
#if PRTLEN>40
#define TitleN1                   "NAME     QUANTITY       PRICE      AMOUNT"
#define TitleN3	                  "NAME            QTY.      AMOUNT"  //--品名   数量   单价   金额
#define TitleN4	                  "NAME                      AMOUNT "  //

#define DEPTREPORTTITLE           "DEPARTMENT         QUANTITY        AMOUNT"
#define PLUREPORTTITLE            "PLU                QUANTITY        AMOUNT"//

#else

#define TitleN1	                  "NAME      QTY.    PRICE   AMOUNT"  //--品名   数量   单价   金额
#define TitleN3	                  "NAME            QTY.      AMOUNT"  //--品名   数量   单价   金额
#define TitleN4	                  "NAME                      AMOUNT "  //
#define DEPTREPORTTITLE         "DEPARTMENT   QUANTITY     AMOUNT"
#define PLUREPORTTITLE          "PLU          QUANTITY     AMOUNT"//

#endif

#define OptionS37                 "CALL#"    //mPRNINDEX//Ccr "取菜号:",


#define EditN6                    "CONSUMED"  // "消费金额",
#define ChipCardN46               "CARD#"  // "会员号:",
#define ChipCardN47               "MEMBERSHIP"  // 会 员 消 费


#define PLULIST                  "         PLU LIST"  // "         单 品 信 息 表"
#define PLUTITLE1                "Name                         ID"  //
#define PLUTITLE2                "   >>Depart   STOCK       Price"//

#define DEPTLIST               "         Depart List"  // "         单 品 信 息 表"
#define DEPTTITLE                "Name          Group       Price"        //



#define FisTextN18                "    ~FISCAL ~ PRINT~    "   //"    _计 _税 _印 _出     "
#define FisTextN19                "    ~TRAIL ~ ENDING~    "   //"    _试 _装 _结 _束     "
#define FisTextN20                "    ~NON   ~ FISCAL~    "   //"    _非   _计   _税     "


#define PRTStyleN1                       "Normal Printing"   //Normale
#define PRTStyleN2                       "DblHigh"   //Doppia altezza
#define PRTStyleN3                       "D~b~l~W~t~h"   //Raddoppio caratteri
#define PRTStyleN4                       "D~b~l~H~i~g~h"   //Dopia altezza + Raddoppio caratteri

#define ComTestN1                        "PORT#1 ERROR"   //"串口 1 测试出错"
#define ComTestN2                        "PORT#1 OK"   //"串口 1 测试正确"
#define ComTestN3                        "PORT#2 ERROR"    //"串口 2 测试出错"
#define ComTestN4                        "PORT#2 OK"   //"串口 2 测试正确"

#define ComTestN3ERR                      "PORT#3 ERROR"    //"串口 3 测试出错"
#define ComTestN3OK                      "PORT#3 OK"   //"串口 3 测试正确"


#define REPRN_PWON      "Re-Print When Power ON........." //
#define REPRN_PAPERON   "Re-Print When Paper OK......"  //

//ccr2016-01-12>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define GRAPHICSELF     "  SELF GRAPHIC"//
#define GRAPHICCAFFE    "  CAFFE"//
#define GRAPHICDRESS    "  DRESS"//
#define GRAPHICKNIFE    "  KNIFE"//
#define GRAPHICHIPPY    "  HIPPY"//
#define GRAPHICBREAD    "  BREAD"//
#define GRAPHICCHICKEN  "  CHICKEN"//
#define GRAPHICROSE1    "  ROSE1"//
#define GRAPHICMUM      "  CHRYSANTHEMUM"//
#define GRAPHICICE      "  ICE CREEN"//
#define GRAPHICGIFT     "  GIFT"//
#define GRAPHICROSE2    "  ROSE-BIG"//
#define GRAPHICTREE     "  CHRISTMAS TREE"   //
#define GRAPHICHBREAD   "  HOT BREAD"//
#define GRAPHICJOY      "  JOY"//
#define GRAPHICFISH     "  FISH"//
#define GRAPHICCARTOON  "  CARTOON CHICKEN" //
#define GRAPHICMONEY    "  MONEY"//
#define GRAPHICEAT      "  EATING"//
#define GRAPHICKITE     "  FLY KITE"            //
#define GRAPHICRUN      "  RUNNING"//
#define GRAPHICREST     "  REST"//

#define ECRVIPTITLE     "ACCOUNT          AMOUNT   POINTS"
                       //012345678901234567890123456789012
#define CREATEDATE      "CREATE DATE"

//ccr2016-06-06>>>>>>>>>>>INDIA>>>>>>>>>>>>>>>>
#define MsgEDITPLU      "EDIT PLU"
#define MsgEDITDEPT     "EDIT DEPART"
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define Msg_RECNUMFR	"EJ# FROM"                  //开始收据号	//RECNUMFR
#define Msg_RECNUMTO	"EJ# TO"                    //结束收据号	//RECNUMTO
#define Msg_EJBBIAO		"~E~J ~R~E~P~O~R~T"		//EJ 报表			//EJBBIAO
#define Msg_EJXINXI		"CURRENT MESSAGES OF EJ"            //EJ信息			//EJXINXI
#define Msg_NEWEJXINXI	"EJ AFTER INITIALIZED"            //EJ信息			//NEWEJXINXI

#define Msg_EJBHAO		"CODE"						//EJ编号			//EJBHAO
#define Msg_EJLEFT    "SPACE LEFT"        //剩余空间
#define Msg_SIZEEJ		"SIZE OF EJ"				//SIZEEJ
#define Msg_EJCSHUA		"EJ INITIAL"		//EJ初始化		//EJCSHUA

#define Msg_PRNEJURNAL  ">>>>PRINT ELECTRONIC JOURNAL<<<<"
#define Msg_ENTERSETUP  ">>>>>    PRINT ON SETUP    <<<<<"
#define Msg_EJDATEEND   "DATE LAST EJ"
#define Msg_EJRECORDS   "LAST EJ#"

#define Msg_EJINITIAL   "EJ INITIALIZATION"
#endif

#define Msg_MANAGEMENT "MANAGEMENT LIST "
#endif
