#ifndef MESSAGEH_VN_P
#define MESSAGEH_VN_P

// Printer
#define Header1         "CAM ON DA CHON"            //" 	 创 造 优 秀  优 创 造 就   *"
#if defined(CASE_INDONESIA)
#define Header2         "MAY TINH TIEN TICH CUC"           //" 	 创 造 优 秀     优 创 造 就   *"
#else
#define Header2         "MAY TINH TIEN EUTRON"      //" 	 创 造 优 秀     优 创 造 就   *"
#endif


#define ReportType0     "DOANH THU"//"   总  销  售   ",
#define ReportType1     "NHOM CHU"//" 柜  组  销  售 ",//"/1
#define ReportType2     "NHOM"//" 部  类  销  售 ",//2
#define ReportType3     "MA HANG"//" 商  品  销  售 ",      /* 3 Used with ApplVar.Plu ApplVar.Report */
#define ReportType4     "TIEN MAT"//" 付  款  信  息 ",//4
#define ReportType5     "CHI-THU"//" 出 入 金 信 息 ",//5
#define ReportType6     "KET TIEN"//" 钱  箱  信  息 ",//6
#define ReportType7     "SUA SAI"//" 更  改  信  息 ",//7
#define ReportType8     "CHIET KHAU"//" 折  扣  信  息 ",//8
#define ReportType9     "NGOAI TE"//" 外  币  信  息 ",//9
#define ReportType10    "THUE"//" 计  税  信  息 ",//10
#define ReportType11    "CHUC NANG TRUOC"//" 桌  帐  信  息 ",//11
#define ReportType12    "TAM TINH TRUOC"//" 开  台  信  息 ",//12
#define ReportType13    "BAN"//" 桌  台  信  息 ",//13
#define ReportType14    "TON KHO"//" 商  品  库  存 ",//14
#define ReportType15    "HOA DON TAM TINH"
#define ReportType16    "THANH TOAN PHU"

#define ReportType17    "TONG NHAN VIEN"//"    收 款 员 统 计   ",
#define ReportType18    "TONG PHUC VU"//"    营 业 员 统 计   ",

#define Trailer1        "CAM ON"//"     ~多  ~谢  ~光  ~临     ",
#define Trailer2        "TAM BIET"//"          ~再  ~见         ",


#define ReportList1     "N.Vien-Ngay"//" 收 款 员 日 报 ",
#define ReportList2     "N.Vien-Tich Luy"//" 收 款 员 月 报 ",
#define ReportList3     "BC NGAY"//" 销  售  日  报 ",
#define ReportList4     "BC TICH LUY"//" 销  售  月  报 ",
#define ReportList5     "BC MA HANG"//" 商  品  报  表 ",
#define ReportList6     "BC BAN"//" 桌  帐  报  表 ",
#define ReportList7     "BC MUI GIO"//" 时  段  报  表 ",
#define ReportList8     "TC.BC NV-NGAY"//"  全收款员日报  ",
#define ReportList9     "TC.BC NV-T.LUY"//"  全收款员月报  ",
#define ReportList10    "PHUC VU THEO NGAY"//" 营 业 员 日 报 "
#define ReportList11    "N.V PH.VU-T.LUY"//" 营 业 员 月 报 "
#define ReportList12    "TC.BC NV NGAY "//"  全营业员日报  ",
#define ReportList13    "T N.V P.V-T.LUY"//"  全营业员月报  ",

#define Promotion1      "XIN CHUC MUNG"//"  ~恭  ~喜  ~中  ~奖",

#define Correc1         "SUA SAI"//"取消    "//ccr2016-07-14
#define Correc2         "XOA"//"更改    "//ccr2016-07-14
#define Correc3         "TRA LAI"//"退货    "
#define Correc4         "HUY"//"取消交易"
#define Correc5         "TAM DUNG"//"挂账"


#define CURRENCY1       "USD"//"美元    "
#define CURRENCY2       "YEN"//"日元    "
#define CURRENCY3       "HK"//"港币    "
#define CURRENCY4       "Ngoai Te 1"//"外币1   "
#define CURRENCY5       "Ngoai Te 2"//"外币2   "

#define DISCOUNT1       "+% M.HANG"//"+%加成"
#define DISCOUNT2       "-% M.HANG"//"(-%)折扣"
#define DISCOUNT3       "CONG TIEN"//"金额加成"
#define DISCOUNT4       "GIAM TIEN"//"金额折扣"
//#define DISCOUNT5     "CONG % M.HANG"// zy //"单项加成"
//ccr090812 for SubDisc>>>>>>>>>>>>>>>>>>>>>>>>>
#define DISCOUNT6       "+% Bill"//  "+%小计加成"
#define DISCOUNT7       "-% Bill"//  "-%小计折扣"
#define DISCOUNT8       "PHU THU"// "小计金额加成"
#define DISCOUNT9       "GIAM TIEN"// "小计金额折扣"
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define DRAWER1         "TIEN MAT"//"现金    "
#define DRAWER2         "CHI PHIEU"//"支票    "
#define DRAWER3         "THE TIN DUNG"//"信用卡  "
#define DRAWER4         "Voucher"//"购物券  "
#define DRAWER5         "TIN DUNG"//"赊账    "
#define DRAWER6         "THE CHIP"//"IC卡    "
#if  defined(CASE_INDONESIA)
#define DRAWER7         "GHI NO"//"小费    "
#else
#define DRAWER7         "PHI"//"小费    "
#endif
#define DRAWER8         "KET-8"//打印/显示

#define PBFunction0     "THEM"//   "开台挂帐"
#define PBFunction1     "CONG THEM"//   "开台增加"
#define PBFunction2         "TAM TINH"//   "暂结"
#define PBFunction3         "KTR HTAT"//   "确认结帐"
#define PBFunction4         "BAN"//   "打印开台"
#define PBFunction5         "IN BILL"//   "打印帐单"
#define PBFunction6         "HUY"//   "取消确认"
#define PBFunction7         "TACH MON"//   "分帐"
#define PBFunction8         "CHUYEN BILL"//   "转帐"
#define PBFunction9         "CHUUYEN BAN"//  "转桌帐'
#define PBFunction10        "SL KHACH"//   "人数"

#define PORAType1            "CHI TIEN"//"出金    "
#define PORAType2            "THU TIEN"//"入金    "
#define PORAType3            "TRA LAI"//"IC卡退款"
#define PORAType4            "NAP"//"IC卡充值"
#define PORAType5            "DAT COC"//"退IC押金"
#define PORAType6            "TCOC IC"//"收IC押金"

#define TendType1             "TIEN MAT"//"现金    "
#define TendType2             "PHIEU"//"支票    "
#define TendType3             "THE TIN DUNG"//"信用卡  "
#define TendType4             "Voucher"//"购物券  "
#define TendType5             "TIN DUNG"//"赊账"
#define TendType6             "THE CHIP"//"ICCard"
#define TendType7             "THE NO"//"DEBIT CARD"
#define TendType8             "TIEN MAT-8"//"CASH-8"


#define OperationN3             "TONG CONG"// "总计",
#define OperationN4             "CHUYEN DOI"// "转换",
#define OperationN5             "TONG"// "小计",
#define OperationN6             "CHI TIEN"// "出金",
#define OperationN7             "THU TIEN"// "入金",
#define OperationN8             "BAO CAO"// "报表",
#define OperationN9             "THEM"// "增加",
#define OperationN10            "GIU"// "挂单,
#define OperationN11            "XOA"// "取消",

#define OperationN14            "GIA"// "单价:",
#define OperationN15            "SO TIEN"// "金额:",

#define CaptionN2               "TONG TIEN"// "净额",
#define CaptionN3               "GIA"// "价格",
#define CaptionN4               "TONG SO GIAO DICH"// "交易数",
#define CaptionN5               "SO LUONG"// "数量",
#define CaptionN6               "SO TIEN"// "金额",
#define CaptionN7               "TIEN MAT"// "收取",
#define CaptionN8               "THOI LAI"// "找零",
#define CaptionN9               "NGAY"// "日",
#define CaptionN10              "TUAN"// "周",
#define CaptionN11              "THANG"// "月",
#define CaptionN12              "NAM"// "年",
#define CaptionN13              "CHUAN"// "标准",
#define CaptionN14              "DIEU HANH"// "收款员",
#define CaptionN15              "GIO"// "时",
#define CaptionN16              "NGAY"// "   日",
#define CaptionN17              "THANG"// "   月",
#define CaptionN18              "TRA LAI"// "退货",
#define CaptionN19              "GIA"// "成本",
#define CaptionN20              "BAN SO:"// "桌号#",
#define CaptionN21              "SEC SO:"// "支票号#",
#define CaptionN22              "SL"// "项数:",
#define CaptionN23              "THUE BAN HANG"// "含税销售",
#define CaptionN24              "THUE"// "税额",
#define CaptionN25              "TRANG"//  "页号",
#define CaptionN26              "SO:"// "号码:",
#define CaptionN27              "TAM TINH"// "挂帐",
#define CaptionN28              "MO BAN"// "开台",
#define CaptionN29              "LAN LAY BAO CAO"// "清帐计数",
#define CaptionN30              "XOA"// "清除",
#define CaptionN31              "S-BAO CAO"// "S-报表",
#if HOR_REPORT==1
#define CaptionN32              "LAI RONG"// "毛利",
#else
#define CaptionN32              "LAI GOP"// "毛利",
#endif
#define CaptionN33              "NHAP KHO"// "库存增减",
#define CaptionN331             "TRU KHO"// "库存增减",
#define CaptionN34              "NGUYEN KHO"// "原桌帐:",
#define CaptionN35              "THE TIN DUNG"// "信用卡#",
#define CaptionN36              "BAN MOI:"// "新桌帐:",
#define CaptionN37              "SL KHACH"// "人数",
#define CaptionN38              "TRAO DOI"// "换货",

//Dispaly
#define CaptionN41              "GIAM 1"// "折扣项1",
#define CaptionN42              "GIAM 2"// "折扣项2",
#define CaptionN43              "BAN HANG"// "销售额",
#define CaptionN44              "TRON GOI"// "大包优惠",
#define CaptionN45              "SL KHO"// "库存数量",
#define CaptionN46              "DE NGHI"// "特价优惠",

#define CaptionN51              "MO"// "开台暂结",
#define CaptionN52              "THEM"// "开台增加",
#define CaptionN53              "TAM TINH"// "暂结",
#define CaptionN54              "XAC NHAN THANH TOAN"// "确认结帐",
#define CaptionN55              "IN"// "打印开台",
#define CaptionN56              "IN BILL"// "打印帐单",
#define CaptionN57              "HUY"// "取消确认",
#define CaptionN58              "TACH"// "分帐",
#define CaptionN59              "CHUYEN"// "转帐",
#define CaptionN60              "CHUYEN DEN"// "更换收款员",
#define CaptionN61              "SL KHACH"// "顾客人数",


#define CaptionE11              "DIEU HANH:"// "收款员:",
#define CaptionE12              "N.VIEN PHUC VU:"// "营业员:",
#define CaptionE13              "MAY SO:"// "收款机号:",
#define CaptionE14              "CUA HANG SO:"// "位置:",

#define CaptionE17              "PHIEN BAN:"// "发行版本:",
#define CaptionE18              "DAU CHAM"// "本次消费点",
#define CaptionE19              "THEM MA HANG"// "新增单品:",
#define CaptionE20              "XOA MA HANG"// "删除单品:",
#define CaptionE21              "BAT DAU KIEM TRA"// "开始检测>>",
#define CaptionE22              "KIEM TRA HOAN TAT"// "<<检测结束",
#define CaptionE23              "TON KHO"// "库存",
#define CaptionE24              "XOA BAO CAO"// "报表已全清!",

#define CaptionE31              "CHU THICH"// "说明文字",


#define MessagesN5                "***CHE DO THUC TAP***"// "*** 培训状态 ***",
#define MessagesN6                "***** MO NGUON*****"// "***** 加电 *****",
#define MessagesN7                "***TACH BAN***"// "*** 分开付帐 ***",
#define MessagesN8                "***BAN SAO PHIEU***"// "*** 复制收据 ***",
#define MessagesN9                "***CAI GIA MAT HANG***"// "*** 商品变价 ***",
#define MessagesN10               "**NHAP KHO +**"// "*** 增加库存 ***",
#define MessagesN101              "**XUAT KHO -**"// "*** 减少库存 ***",
#define MessagesN11               "***HUY***"// "***** 取消 *****",
#define MessagesN12               "**KHOI TAO CHUONG TRINH**"// "** 程序初始化 **",


#define TitleN1	                  "TEN HANG   S.L      GIA   T.TIEN"//--品名   数量   单价   金额    //*123456789012345678901234567890123456*//

#define TitleN3	                  "TEN HANG        S.L       T.TIEN"//--品名   数量   单价   金额    //*123456789012345678901234567890123456*//
#define TitleN4	                  "TEN HANG                  T.TIEN"//

#define OptionS37                 "LAY SO"//mPRNINDEX////Ccr "取菜号:",


#define EditN6                    "TIEU THU"// "消费金额",
#define EditN24                   "TAP TIN       BAN GHI       RAM"// "文件名      记录数      空间大小"
#define EditN25                   "CONG       LOAI       GIAO THUC"// "串口名称    通信类别   通信协议"

#define ChipCardN46               "THE HOI VIEN:"// "会员号:",
#define ChipCardN47               "THANH VIEN"// 会 员 消 费


#define PLULIST                  "DANH SACH MA HANG"// "         单 品 信 息 表"
#define PLUTITLE1                "TEN HANG                   ID"//
#define PLUTITLE2                ">>NHOM     KHO     GIA"//

#define DEPTLIST               "DANH SACH NHOM"// "         单 品 信 息 表"
#define DEPTTITLE              "TEN HANG      NHOM CHU      GIA"//

#define FisTextN18                "~ IN ~ TAI ~CHINH ~"//"    _计 _税 _印 _出     "
#define FisTextN19                "~ KET ~THUC ~ THUC ~TAP ~"//"    _试 _装 _结 _束     "
#define FisTextN20                "~PHI ~THUE"//"    _非   _计   _税     "


#define PRTStyleN1                "IN THUONG"//Normale
#define PRTStyleN2                "CAO GAP DOI"//Doppia altezza
#define PRTStyleN3                "R~O~N~G ~G~A~P ~D~O~I"//Raddoppio caratteri
#define PRTStyleN4                "C~A~O ~R~O~N~G ~G~A~P ~D~O~I"//Dopia altezza + Raddoppio caratteri

#define ComTestN1                 "LOI CONG 1"//"串口 1 测试出错"
#define ComTestN2                 "CONG 1 OK"//"串口 1 测试正确"
#define ComTestN3                        "LOI CONG 2"//"串口 2 测试出错"
#define ComTestN4                        "CONG 2 OK"//"串口 2 测试正确"

#define ComTestN3ERR                     "LOI CONG 3"//"串口 3 测试出错"
#define ComTestN3OK                      "CONG 3 OK"//"串口 3 测试正确"

#define DEPTREPORTTITLE         "NHOM       S.L        THANH TIEN"//"部类名称     交易数量       金额"
#define PLUREPORTTITLE          "MA HANG     S.L        THANH TIEN"//

#define REPRN_PWON      "IN LAI KHI MO NGUON....."//
#define REPRN_PAPERON   "IN LAI KHI GIAY O.K...."//

//ccr2016-01-12>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define GRAPHICSELF     "  SELF GRAPHIC"
#define GRAPHICCAFFE    "  CAFFE"
#define GRAPHICDRESS    "  DRESS"
#define GRAPHICKNIFE    "  KNIFE"
#define GRAPHICHIPPY    "  HIPPY"
#define GRAPHICBREAD    "  BREAD"
#define GRAPHICCHICKEN  "  CHICKEN"
#define GRAPHICROSE1    "  ROSE1"
#define GRAPHICMUM      "  CHRYSANTHEMUM"
#define GRAPHICICE      "  ICE CREEN"
#define GRAPHICGIFT     "  GIFT"
#define GRAPHICROSE2    "  ROSE-BIG"
#define GRAPHICTREE     "  CHRISTMAS TREE"        //CHRISTMAS TREE
#define GRAPHICHBREAD   "  HOT BREAD"
#define GRAPHICJOY      "  JOY"
#define GRAPHICFISH     "  FISH"
#define GRAPHICCARTOON  "  CARTOON CHICKEN"        //CARTOON CHICKEN
#define GRAPHICMONEY    "  MONEY"
#define GRAPHICEAT      "  EATING"
#define GRAPHICKITE     "  FLY KITE"            //FLY A KITE
#define GRAPHICRUN      "  RUNNING"
#define GRAPHICREST     "  REST"

#define ECRVIPTITLE     "ACCOUNT          AMOUNT   POINTS"
                       //012345678901234567890123456789012
#define CREATEDATE      "CREATE DATE"
//ccr2016-06-06>>>>>>>>>>>INDIA>>>>>>>>>>>>>>>>
#define MsgEDITPLU      "CHINH SUA MA HANG"//
#define MsgEDITDEPT     "CHINH SUA NHOM"//
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define Msg_RECNUMFR	"EJ# FROM"                  //开始收据号	//RECNUMFR
#define Msg_RECNUMTO	"EJ# TO"                    //结束收据号	//RECNUMTO
#define Msg_EJBBIAO		"~E~J ~R~E~P~O~R~T"		//EJ 报表			//EJBBIAO
#define Msg_EJXINXI		"CURRENT MESSAGES OF EJ"            //EJ信息			//EJXINXI
#define Msg_NEWEJXINXI	"EJ AFTER INITIALIZED"            //EJ信息			//NEWEJXINXI
#define Msg_EJBHAO		"CODE"						//EJ编号			//EJBHAO
#define Msg_EJLEFT      "SPACE LEFT"        //剩余空间
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

