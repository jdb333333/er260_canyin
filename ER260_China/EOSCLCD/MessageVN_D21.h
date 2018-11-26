#ifndef MESSAGEH_VN_D
#define MESSAGEH_VN_D


#define Modifier1             "GHI CHU"//"说明    "
#define ClerkRec1             "THU NGAN"//"收款员  "
#define GroupRec1             "NHOM CHU"//"柜组    "
#define DeptRec1              "NHOM"//"部类    "
#define PLURec1               "MA HANG"//"商品    "
#define SalesPersonRec1       "PHUC VU"//"营业员  "
#define OffRec1               "DE NGHI"//"特殊出价"

#define XZReportList1     "BC NGAY"//"销售日报表",
#define XZReportList2     "BC TICH LUY"//"销售月报表",
#define XZReportList3     "TICH LUY M.HANG"//"商品报表",
#define XZReportList4     "TICH LUY BAN"//"桌帐报表",
#define XZReportList5     "MUI GIO"//"时段报表",
#define XZReportList6     "N.VIEN-NGAY"//"收款员日报表",
#define XZReportList7     "N.VIEN-T.LUY"//"收款员月报表",
#define XZReportList8     "TC.BC NV-NGAY"//"全收款员日报表",
#define XZReportList9     "TC.BC NV-T.LUY"//"全收款员月报表",
#if (salNumber)
#define XZReportList10    "PHUC VU THEO NGAY"//"营业员日报表"
#define XZReportList11    "N.V PH.VU-T.LUY"//"营业员月报表"
#define XZReportList12    "TC.BC NV NGAY "//"全营业员日报表",
#define XZReportList13    "T N.V P.V-T.LUY"//"全营业员月报表",
#endif
#if (BARCUSTOMER==1)
#define XZReportList14    "VIP"//"条码会员报表",
#endif

#define PortType1               "MAY VI TINH"//"  主机",
#define PortType2               "DOC MA VACH"//"条码枪",
#define PortType3               "CAN DIEN TU"//"电子秤",
#define PortType4               "IN BEP"//"厨  打",
#define PortType5               "IN PHIEU"//'票打'，20040322
#define PortEXTPRN              "EP-PORT"//ccr2016-12-14 ExtPrinter????
#define PortType6               "THE CHIP"//-------090421 BY ZY//"/"射频卡"
#define PortType7               "THE E"//"E通卡"

#define KClose                  "TAT MAY"//"   关 ",

#define SysFlags0		        "CUA HANG SO:"// "店铺编号"
#define SysFlags1               "MAY SO:"//"收款机号:"
#define SysFlags2               "LAM TRON"//"舍入方式:"
#define SysFlags3               "GIA MO"//"商品变价:"
#define SysFlags4               "TIENG Bip"//"蜂鸣器:"
#define SysFlags5               "BUOC VAO CA"//"强制收款员"
#define SysFlags6               "THAY N.VIEN"//"改变收款员"
#define CLERKPASS               "MAT MA N.VIEN"//"收款员密码"
#define SysFlags8               "IN N.VIEN PHUC VU"//"打印营业员"
#define SysFlags9               "IN N.VIEN"//"打印收款员"
#define SysFlags10              "IN SO H.DON"//"打印收据号"
#define SysFlags11              "IN DAM"//"高浓度打印"
#define SysFlags12              "BAC BUOC VAO MA PHUC VU"//"强制营业员"
#define SysFlags13              "IN MA"//"打印编码:"
#define SysFlags14              "BAM T.CONG"//"按小计键:"
#define SysFlags15              "SO BAN IN"//"打印项目数"
#define SysFlags16              "IN TONG CONG"//"打印小票:"
#define SysFlags17              "DAU PHIEU NHO"//"小票票头:"
#define SysFlags18              "KHOA SHIFT"//"锁定SHIFT:"
#define SysFlags19              "HIEN THI THOI GIAN"//"显示时间:"
#define SysFlags20              "BAN SAO"//"复制收据:"
#define SysFlags21              "MA CAN"//"特殊条码:"
#define SysFlags22              "NHAT KY DANG NHAP"//-----090421 BY ZY//"操作日志"
#define SysFlags23              "IN MO BAN"//"开台打印:"
#define SysFlags24              "IN KHO"//"打印库存"
#define SysFlags25              "TAT/MO H.DON"//"打印开关:"
#define SysFlags26              "BUOC MO BAN"//"必须开台:"
#define SysFlags27              "GIU SO PHIEU"//"保持收据号"
#define SysFlags28              "BAN SAO IN BEP"//"复制厨打:"
#define SysFlags29              "RESET SO PHIEU"//"收据号复位"
#define SysFlags30              "IN GAP DOI"//-----090421 BY ZY//"倍高打印"
#define SysFlags31              "TUONG PHAN"//-----090421 BY ZY//"对比度:"
#define SysFlags32              "TIEU DE DAU"// "票头居中打印"
#define SysFlags33              "TIEU DE CUOI"// "票尾居中打印"
#define SysFlags34              "GAP DOI T.DE DAU"// "票头倍高打印"
#define SysFlags35              "GAP DOI T.DE CUOI"// "票尾倍高打印"
//ccr2016-03-21#define SysFlags36              "DATA FORMAT"       /DINH DANG DU LIEU/ 输出CSV or BINARY
#define SysFlags37              "IN NGAY"
#define SysFlags38              "IN GIO"
#define SysFlags39              "IN LAY SO"//"打印取菜号"
#define SysFlags40              "GUI TIN.H MCHU"//ccr2015-08-03"向服务器发送Heart beat"
#define SysFlags41              "PreFix1:"      //ccr2016-08-02
#define SysFlags42              "PreFix2:"      //ccr2016-08-02

#define GrapType1               "KIEU LOGO"//"商店图标",
#define GrapType2               "H.KH.MAI"//"中奖图标:",
#define GrapType3               "LOGO CUOI"//"票尾图像:",
#define GrapType4               "VAN BAN"//"打印文字?",

#define GrapSet1                "STT"//"序号:",
#define GrapSet2                "HINH"//"图像编号:",
#define GrapSet3                "TU"//"开始日期:",
#define GrapSet4                "DEN"//"结束日期:",

  /////==============CHN msg===============//////// Display
#define CurrentMode1            "BAN HANG"//"营业",
#define CurrentMode2            "XEM BC"//"读帐",
#define CurrentMode3            "LAY BC"//"清帐",
#define CurrentMode4            "CAI DAT"//"设置",
#define CurrentMode5            "CHU"//"经理",

#define SetupMode1              "CAI DAT CHUNG"//---090421 BY ZY//" "综合功能",
#define SetupMode2              "NHOM/MA"// "商品管理",
#define SetupKeyboard           "BAN PHIM/DEFKEYBOARD"  //
#define msgCOMMHOST             "COMM-HOST"
#define SetupMode3              "THIET BI NGOAI VI"// "通讯与外设",
#define SetupMode4              "QUAN LY"// "销售功能设置",
#define SetupMode5              "TAI KHOAN"// "会员卡管理",
#define SetupMode6              "DAU/CUOI"// "票据样式",
#define SetupMode7              "BAO CAO"// "报表功能",
#define msgDISCOUNT             "GIAM GIA/MENUDISC" //
#define SetupMode8              "CHUC NANG TAM"// "餐饮功能",
#define SetupMode9              "CAI MAT MA"// "密码,
#define SetupMode10             "DICH VU"// "收款机维护",

#define SysSettings1            "TUY CHON HE THONG"// "系统参数设置",
#define SysSettings2            "CAI DAT NGAY"// "日期",
#define SysSettings3            "CAI DAT GIO"// "时间",
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define msgSETEJOURNAL          "SET EJ"      //SETEJOURNAL //EJOURNAL
#endif

#define SysSettings4            "BAN PHIM"// "键盘",
#define MESSKEYMACRO            "PHIM TICH HOP"// "键盘宏"
#define MESSKEYTYPE             "LOAI PHIM"// "键盘模式"
#define SysSettings5            "MO HINH NHA HANG"// "餐饮键盘切换",
#define SysSettings6            "BAN LE"// "超市键盘切换",
#define SysSettings8          "TIN HIEU KHONG DAY"//"GPRS通信"  //SETGPRS //ccr2014-11-11 NEWSETUP

#define SetupCaption1           "NHOM CHU"//"柜组"
#define SetupCaption2           "NHOM"//"部类",
#define SetupCaption3           "MA HANG"//"单品",
#define SetupCaption4           "IN DANH SACH HANG"//"打印商品清单",
#define msgSETTEST              "KIEM TRA ECR"//"系统测试"            //SETTEST
#define msgSETCLEARPLU          "XOA MA HANG"//"清除单品"        //SETCLEARPLU
#define msgSETPLULIST           "IN NHOM/MA HANG"//"打印商品清单"      //SETPLULIST
#define msgSETTSTVER	        "IN PHIEN BAN"//"打印版本"      //SETTSTVER
#define msgSETCONFIGPLU         "TU MA"//"配置单品"          //SETCONFIGPLU
#define msgSETCONFIGDEPT        "TU NHOM"//配置部类         //SETCONFIGDEPT
#define msgSETINSTALL           "CAI DAT/SAO LUU"//"安装/备份"        //SETINSTALL
#define msgSETPRINTFAT          "IN PHAN BO"//"打印文件分配表"   //SETPRINTFAT


#define SetupCaption5           "MAY TINH"// "通讯口1",
#define SetupCaption6           "DOC MA VACH"// "通讯口2",
#define SetupCaption7           "CAN DIEN TU"// "通讯口 3",
//#define SetupCaption8           "PORT-4"			//"CONG 4"// "通讯口 4",
#define SetupCaption9           "CONG MANG"// "网络设置"

#define SetupCaption10          "IN PHIEU"// "票据打印",
#define SetupCaption11          "IN BEP"// "厨房打印",
#define SetupEXTPrinter         "EXTERNAL PRINT"    //SETEXTPRINTER

//Display
#define SetupCaption12          "DE NGHI"// "优惠价",
#define SetupCaption13          "KHUYEN MAI"// "中奖设置",
#define SetupCaption14          "GIAM GIA"// "折扣",
#define SetupCaption15          "CHI.THU"// "出金入金",
#define SetupCaption16          "SUA SAI"// "更正功能",
#define SetupCaption17          "HINH THUC THANH TOAN"// "付款方式",
#define SetupCaption18          "NGOAI TE"// "外币",
#define SetupCaption19          "KET TIEN"// "钱箱",
#define SetupCaption20          "THUE"// "税率",

#define SetupCaption21          "DAT TAI KHOAN"// "IC卡设置",
#define SetAddNewECRVIP         "THEM TAI KHOAN"
#define SetPrintECRVIP          "IN TAI KHOAN"
#define SetCleanALLVIP          "XOA TAI KHOAN"

#define SetupCaption22          "MAT THE"// "IC卡挂失",
#define SetupCaption23          "IN THE"// "打印卡信息",
#define SetupCaption24          "TIEU DE DAU"// "票头",
#define SetupCaption25          "TIEU DE CUOI"// "票尾",
#define SetupCaption26          "DAU PHIEU"// "票据票头",
#define SetupCaption27          "LOGO HINH"// "图形",
#define SetupCaption28          "IN LOGO HINH"// "打印图形",
#define SetupCaption29          "BAO CAO"// "设置报表内容",
#define SetupCaption30          "DON VI BAO CAO"// "报表单元",
#define SetupCaption31          "MUI GIO"// "报表时段设置",
#define SetupCaption32          "XOA TAT CA"// "总清报表",
#define SetupCaption33          "IN CUON LUU"// "打印流水",

#define SetupCaption34          "BAN"// "桌台",
#define SetupCaption35          "PB FUNCTION"// "餐饮功能",
#define SetupCaption36          "GHI CHU"//ZY  // "菜单说明",
#define SetupCaption37          "XAC NHAN"//ZY  // "签单",
#define SetupCaption38         "THU NGAN"// "收款员",
#define SetupCaption39          "NHAN VIEN BAN HANG"// "营业员",
#define SetupCaption40          "MAT MA CHUONG TRINH"//ZY  // "设置档密码",
#define SetupCaption41          "MAT MA QUAN" //LY/ZY  // "经理档密码",
#define SetupCaption42          "MAT MA XEM BAO CAO"//ZY  // "只读报表密码",
#define SetupCaption43          "MAT MA LAY BAO CAO"//ZY  // "清帐报表密码",
#define SetupCaption44          "BO NHO"//ZY  // "打印测试",
#define SetupCaption45          "HIEN THI"//ZY  //"显示测试",
#define SetupCaption451         "KHACH HANG"// 客显测试
#define SetupCaption46          "MAY IN"//ZY  //"打印版本",
#define SetupTestRAM           "KIEM TRA RAM"//SETTSTMEM	       //RAM测试
#define SetupCaption47          "DONG HO"//ZY  //"时钟测试",
#define SetupCaption48          "COI BAO"//ZY  //"蜂鸣器测试",
#define SetupCaption49          "KET TIEN"//ZY  //"钱箱测试",
#define SetupCaption50          "BAN PHIM"//ZY  //"键盘测试",
#define SetupCaption51          "CONG NOI TIEP"//ZY  //"串口测试",
#define SetupCaption52          "TU KIEM TRA"//ZY  //"自动测试",

#define msgSETPRNMENU           "IN MENU"//SETPRNMENU   //

#define OperationN1             "GHI LAI"// "记录#",
#define OperationN2             "LUU Y?"//ZY  // "记录?",

#define OperationN12            "MAT MA"// "密码",
#define OperationN13            "XAC NHAN"// "密码确认",
#define OperationN16            "CUA HANG"// "超市版",
#define OperationN17            "NHA HANG"// "餐饮版",
#define OperationN18            "KIEM TRA"// "测试",


//Dispaly


#define CaptionN39              "THEM MA HANG?"// "增加?",
#define CaptionN40              "XOA MA HANG?"// "删除?",
#define CaptionN47              "TUY CHON:"// "选项:",
#define CaptionN48              "CO DINH"// "固定",
#define CaptionN49              "TU DO"// "浮动",
#define CaptionN50              "DINH/TU DO"// "固//浮",
#define CaptionN62              "DIEM THUONG:"// "中奖设定:",
#define CaptionN63              "DIEM THUONG"// "消费积分:",
#define CaptionN64              "THAY DOI GIA"// "改变单品价格",
#define CaptionN65              "NHOM IN BEP"// "厨房打印机组"，
#define CaptionN66              "CONG KHO"// "增加库存",
#define CaptionN67              "XUAT KHO"// "减少库存",
#define CaptionN68              "NHAN"// "输入",
#define CaptionN69              "TRA"// "待付",

#define CaptionE1               "LOI RS-232"// "RS232错",
#define CaptionE2               "RS232 OK"// "RS232正确",
#define CaptionE3               "LOI RTC"// "RTC 错!",
#define CaptionE4               "LOI CTC"// "CTC 错!",
#define CaptionE5               "LOI FPROM"// "FPROM错!",
#define CaptionE6               "KHOI TAO"// "初始化...",
#define CaptionE7               "KHOI TAO THANH CONG"// "初始化结束.",
#define CaptionE8               "LOI"// "出错-",////长度<SCREENWD

#define CaptionE9               "SAI MAT MA"// "密码错 ！",
#define CaptionE10              "LUU MAT MA"// "密码已保存",

#define CaptionE15              "XAC NHAN ?"// "确认?",
#define CaptionE16              "HOAN THANH"// "完成",

#define CaptionE25              "CAP NHAT!\nKHONG THOAT!"// "更新程序切勿断电",
#define CaptionE26              "TRAN TAP TIN"// "文件空间越界",
#define CaptionE27              "TRAN BO NHO"// 内存溢出！
#define CaptionE28              "DAY CUON LUU"// "流水存储区满",
#define CaptionE29              "HET GIAY"// "请更换打印纸",
#define CaptionE30              "LOI BO NHO MO RONG"// "扩展内存出错！",



#define CaptionE32              "NHAP KHONG HOP LE"// "无效输入!",
#define CaptionE33              "NGAY KHONG HOP LE"// "无效日期!",
#define CaptionE34              "GIO KHONG HOP LE"// "无效时间!",
#define CaptionE35              "KHONG DUOC PHEP"// "禁止输入!",
#define CaptionE36              "KHONG CO MA HANG"// "PLU代码为空!",
#define CaptionE37              "DAY MA HANG"// "PLU文件溢出!",
#define CaptionE38              "BAN DA TON TAI"////" "桌台被占用!",
#define CaptionE39              "BAN DA MO"// "桌台已打开!",
#define CaptionE40              "SAI BAN"// "桌台号不对!",
#define CaptionE41              "NHAP SO BAN"// "请输入桌台号",
#define CaptionE42              "KHONG MO BAN"// "没有开台",
#define CaptionE43              "DAY BAN"// "桌台桌帐溢出",
#define CaptionE44              "KHONG DUOC PHEP"// "禁止修改时间",
#define CaptionE45              "DANG BAN HANG"// "仍在销售中!",
#define CaptionE46              "DAY NHO DEM"// "销售缓冲区满",
#define CaptionE47              "HANG TON"// "商品没有售出",
#define CaptionE48              "DANG BAN HANG"// "正在结算中!",
#define CaptionE49              "SO KHONG HOP LE"// "输入数据超长",
#define CaptionE50              "DANG THANH TOAN"// "正在付帐!",
#define CaptionE51              "NHAP SAI KHACH"// "人数溢出!",
#define CaptionE52              "XAC NHAN BAN"// "没有确认结帐",
#define CaptionE53              "KHONG DUOC PHEP"// "禁止取消付款",
#define CaptionE54              "KHONG CO HANG"// "无菜名!",
#define CaptionE55              "KHONG CO NHAN VIEN"// "无此收款员!",
#define CaptionE56              "KHOA NHAN VIEN"// "收款员改变",
#define CaptionE57              "BAO CAO-RONG"// "无此类报表!",
#define CaptionE58              "IN BI GIAN DOAN"// "报表打印中断",
#define CaptionE59              "BAT BUOC CHE DO QUAN LY"// "必须在经理档",
#define CaptionE60              "KHONG THE TACH"// "不能分帐时用",
#define CaptionE61              "NHAP SO#"// "必须输入号码",
#define CaptionE62              "KEY POSITION ERROR"// "钥匙档位错",
#define CaptionE63              "KHONG DUOC PHEP"// "未授权!",
#define CaptionE64              "XIN NHAP NHAN VIEN"// "请指定收款员",
#define CaptionE65              "XIN NHAP N.VIEN PHUC VU"// "请指定营业员",
#define CaptionE66              "KHONG DUOC PHEP"// "禁止PLU变价！",
#define CaptionE67              "SAI MAT MA"//  "密码不对!",
#define CaptionE68              "LOI IN BEP"// "厨房打印机错",
#define CaptionE69              "LOI ERROR"// "票据打印机错",
#define CaptionE70              "HET GIAY"// "打印压杆抬起",
#define CaptionE71              "HET GIAY"// "打印纸用完!",
#define CaptionE72              "NONG THANH NHIET"// "打印温度太高",
#define CaptionE73              "CHUA CO CHUC NANG"// "功能未定义!",
#define CaptionE74              "XIN NHAP TIEN"// "必须输入金额",
#define CaptionE75              "CAM THANH TOAN HINH THUC KHAC"// "禁止多种付款",
#define CaptionE76              "CHUC NANG RONG"// "记帐无此功能",
#define CaptionE77              "BAM TONG CONG"// "未按小计键!",
#define CaptionE78              "TON KHO"// "正在管理库存",
#define CaptionE79              "LOI CUON LUU"// "保存流水错",
#define CaptionE80              "LOI MODEM"// "MODEM通讯错!",

#define CaptionE81              "LOI CHIPCARD"// "卡操作出错!"
#define CaptionE82              "LOI MA POS"// "POS代码错!"
#define CaptionE83              "LOI DU LIEU THE"// "卡数据错!"
#define CaptionE84              "THE HET HAN"// "为过期卡!"
#define CaptionE85              "MAT THE"// "为挂失卡!"
#define CaptionE86              "SAI LOAI"// "客户类别错!"
#define CaptionE87              "THE MOI"// "为新卡!"
#define CaptionE88              "KHONG THANH TOAN THE"// "不是购物卡!"
#define CaptionE89              "LOI GHI THE"// "写卡出错!"
#define CaptionE90              "LOI CONG"// "卡号不对!"
#define CaptionE91              "THE GIAM GIA"// "禁用折扣卡!"
#define CaptionE92              "THANH TOAN THE!"// "禁用现金卡!"
#define CaptionE93              "THE TIN DUNG!"// "禁用赊帐卡!"
#define CaptionE94              "KHONG SU DUNG"// "禁用IC卡!"
#define CaptionE95              "LOI KHOA THE"// "清卡出错!",
#define CaptionE96              "TRAN DU LIEU"// "数据溢出!",
#define CaptionE97              "LOI CHARGE"// "IC充值出错!",
#define CaptionE98              "NAP THE LOI"// "IC初始化出错",
#define CaptionE99              "CAM KHOI TAO"// "禁止初始化!",
#define CaptionE100             "KHONG DU TIEN"// "IC金额不足!",
#define CaptionE101             "LOI THANH TOAN"// "IC付款出错!",
#define CaptionE102             "LOI DIA CHI IP"// "IP地址错"
#define CaptionE103             "KHONG CO THE"//  ,
#define CaptionE105		"GIU"//CWXXI73////Ccr							////CWXXI73

#define CaptionERR105            "PIN YEU"// "电池电量低",
#define CaptionERR106            "KHONG DUNG KHO"//20070806"没激活库存"
//ccr090715>>>>>>>>>>>>>>>>>>>>>>
#define CaptionERR76		"CHON DIA"//
#define CaptionERR77		"LOI MO"//
#define CaptionERR78		"LOI DOC"//
#define CaptionERR79		"LOI GHI"//
#define CaptionERR80		"LOI TAO"//
#define CaptionERR81		"LOI DONG"//
#define CaptionERR82		"GUI LOG!"//
#define CaptionERR83            "LOI WIRELESS"//
#define CaptionERR84            "LOI GUI"//
#define CaptionERR85            "LOI NHAN"//
#define CaptionERR86            "KHONG CO MAY CHU"//
#define CaptionERR87            "IP/CONG: RONG"//没有设置服务器IP和端口
#define CaptionERR88            "KHONG CO THE SIM"//
#define CaptionERR89            "WIRELESS KHONG OK"//
#define CaptionERR90            "VUOT GIOI HAN"//CWXXI90  ////
#define CaptionERR91            "TAI KHOANG RONG"//CWXXI91
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
#define CaptionE104             "MO KET"// "打开钱箱",
#define CaptionE106             "GIU PHIEU"//
#define CaptionE107             "NHAP SL\nVA GIA"//
#define CaptionE108             "CHUYEN DEN"    //ZHUANRUDAO

#define MsgPUTOUT               "CHI"//PUTOUT
#define MsgRECIEVIN             "THU"//RECIEVIN

#define DownLoad1               ">>TAI FONT CHU"// "正在下载字库",
#define DownLoad2               ">>TAI LOGO HINH"// "正在下载图形",


#define OptionN1                "DONG" ////Y// "是",
#define OptionN2               "KHONG DONG" ////Y// "否",
#define OptionN3                "TEN"// "名称:",
#define OptionN4                "NHOM"// "部类:",
#define OptionN5                "NHOM CHU"// "柜组:",
#define OptionN6                "IN"// "打印:",
#define OptionN7                "TUY CHON"// "选项:",
#define OptionN8                "LIEN KET"// "链接:",
#define OptionN9                "THUE"// "税率:",
#define OptionN10               "MA POS"// "位置编号:",
#define OptionN11               "GIA 1"// "价格 1:",
#define OptionN12               "GIA 2"// "价格 2:",
#define OptionN13               "GIA 3"// "价格 3:",
#define OptionN14               "GIA 4"// "价格 4:",
#define OptionN15               "MUA"// "进价:",
#define OptionN16               "GIAM %"// "折扣率(%):",
#define OptionN17               "LON NHAT"// "最大值:",
#define OptionN18               "GIAM GIA"// "折扣率:",
#define OptionN19               "LON NHAT"// "最大值:",
#define OptionN20               "TI LE"// "汇率:",
#define OptionN21               "MUA"// "买入价:",
#define OptionN22               "BAN"// "卖出价:",
#define OptionN23               "TRANG THAI"// "状态:",
#define OptionN24               "KET TIEN"// "钱箱:",
#define MSGOTD                  "OTD"//OTDMSG
#define OptionN25               "IN BEP"// "厨房打印:",
#define MSGPERIOD               "THOI KHOANG"//PERMSG
#define MSGPOINTER              "CON TRO"//POIMSG

#define OptionN26               "TIEN TO"// "前缀符",,6个字符
#define OptionN27               "LIEN KET"// "连接:",
#define OptionN28               "CHUC NANG"// "输入功能码:",
#define OptionN29               "CHON PHIM"// "按功能键:",
#define OptionN30               "LOAI"// "类型:",
#define OptionN31               "TU NGAY"// "开始日期:",
#define OptionN32               "DEN NGAY"// "结束日期:",
#define OptionN33               "THOI GIAN BAT DAU"// "开始时间:",
#define OptionN34               "THOI GIAN KET THUC"// "结束时间:",
#define OptionN35               "TUAN"// "星期:",
#define OptionN36               "GIAM"// "折扣:",
#define OptionN37               "SO LUONG"// "大包数量:",
#define OptionN38               "GIA"// "单价:",
#define OptionN39               "DE NGHI"// "优惠价:",
#define OptionN40               "GIAO THUC TRUYEN"// "通讯协议:",
#define OptionN41               "SO DIEN THOAI"// "电话号码:",
#define OptionN42               "MAT MA"// "密码:",
#define OptionN43               "TAN SUAT"// "频率:",
#define OptionN44               "TOI THIEU"// "最小金额:",
#define OptionN45               "CONG"// "通讯口:",
#define OptionN46               "GIA TRI"// "计点金额:",
#define OptionN47               "HINH ANH"// "图象序号:",
#define OptionN48               "GIO:"// "时间:",
#define OptionN49               "NGAY:"// "日期:",
#define OptionN50               "IN BEP LOAI:"//
#define OptionN51               "TYPE:"      //DZCHXHAO//"秤型号",

#define WelcomeN1               "CAM ON DA CHON"// "创 造 优 秀 优 创 造 就\n    欢迎使用优创收款机" *////"
#define WelcomeN2               "CAM ON DA CHON"// "创造优秀优创造就\n优创蓝宝石收款机" *//

#define MessagesN1              "DONG MAY"// "  收款机已锁定",
#define MessagesN2              "BAM<PHIM>"// "按【方式锁】开机",
#define MessagesN3              "HINH THUC HOA DON"// "形式发票",																							////XSHFPIAO
#define MessagesN4              "IN BEP#"// "厨房打印机#",																						////CHFDYJHAO
#define MessagesN13               "<>"
#define MessagesN14               "TONG CONG"//////KPTOTAL


#define CaptionM1                 "THANG 1"// "一月",
#define CaptionM2                 "THANG 2"// "二月",
#define CaptionM3                 "THANG 3"//  "三月",
#define CaptionM4                 "THANG 4"//  "四月",
#define CaptionM5                 "THANG 5"// "五月",
#define CaptionM6                 "THANG 6"// "六月",
#define CaptionM7                 "THANG 7"// "七月",
#define CaptionM8                 "THANG 8"// "八月",
#define CaptionM9                 "THANG 9"// "九月",
#define CaptionM10                "THANG 10"// "十月",
#define CaptionM11                "THANG 11"// "十一月",
#define CaptionM12                "THANG 12"// "十二月",

#define CaptionW1                 "THU 2"// "星期一",
#define CaptionW2                 "THU 3"// "星期二",
#define CaptionW3                 "THU 4"// "星期三",
#define CaptionW4                 "THU 5"// "星期四",
#define CaptionW5                 "THU 6"// "星期五",
#define CaptionW6                 "THU 7"// "星期六",
#define CaptionW7                 "CHU NHAT"// "星期日",


#define ClueN1                    "BAM ENTER"// "按确认键...",

  ////"//Option for KP
#define OptionS1                  "IN TONG"// "打印总额:",
#define OptionS2                  "GOP MON GIONG NHAU"// "菜相同打印",
#define OptionS3                  "KHONG GOP"// "菜不同打印",
  ////"//Option for dept
#define OptionS4                  "GIA BANG 0"// "禁止零价格",
#define OptionS5                  "LUOI PHAN CHIA"// "打印分隔线",
#define OptionS6                  "DOC LAP"// "单项销售:",
#define OptionS7                  "GIAM GIA 1"// "折扣项 1:",
#define OptionS8                  "GIAM GIA 2"// "折扣项 2:",
  ////Options for tend
#define OptionS9                  "MO KET"// "打开钱箱:",
#define OptionS10                 "NHAP TIEN"// "输入金额:",
#define OptionS11                 "CAM"// "禁止输入:",
#define OptionS12                 "NHAP SO:"// "输入号码:",
#define OptionS13                 "TIEN BOA"// "找零为小费",
  ////Options for PoRa
#define OptionS14                 "MO KET"// "打开钱箱:",
#define OptionS15                 "THANH TOAN"// "付款方式键",
#define OptionS16                 "DAT COC"// "存入金额:",
#define OptionS17                 "CHI//THU"// "入金//出金",
  ////Option for Disc
#define OptionS18                 "IN GIAM GIA"// "打印折扣额",
#define OptionS19                 "GIAM GIA 1"// "使用折扣1:",
#define OptionS20                 "GIAM GIA 2"// "使用折扣2:",
  ////Options for currency
#define OptionS21                 "THOI LAI"// "卖出价找零",
  ////Options for Draw
#define OptionS22                 "KHONG THAP PHAN"// "无小数点:",
#define OptionS23                 "MO KET"// "打开钱箱:",
  ////Options for Tax
#define OptionS24                 "CO THUE"// "营业//增值:",
#define OptionS25                 "IN THUE VAT"// "打印税率项",
#define OptionS26                 "IN 0"// "打印0税项",
#define OptionS27                 "CAC LOP THUE"// "为GST类税",
#define OptionS28                 "IN THUE"// "打印税额:",
  ////Options for clerk
#define OptionS29                 "GIOI HAN SUA SAI"// "取消限制:",
#define OptionS30                 "GIOI HAN XOA"// "更改限制:",
#define OptionS31                "GIOI HAN HUY"// "取消交易:",
#define OptionS32                 "GIOI HAN TRA LAI"// "退货限制:",
#define OptionS33                 "GIOI HAN GIAM %"// "%折扣限制:",
#define OptionS34                 "GIOI HAN CONG %"// "%加成限制:",
#define OptionS35                 "GIOI HAN +- TIEN"// "+-金额限制",
#define OptionS36                 "THUC TAP"// "培训模式:",


#define MESS_ECR_IP         "DIA CHI MAY"//
#define MESS_ECR_ID         "SO MAY(0-9,A-F):"//

#define EditN1                    "<GHI SO>GHI MOI \n<XAC NHAN>GHI SO GOC \n"////---ZY  ////ccr "<记录号>  新纪录\n<确认>  当前纪录\n",
#define EditN2                    "<GHI SO>A//D ITEM \n<XAC NHAN>GHI SO GOC \n"//---ZY  ////ccr "<记录号>增删商品\n<确认>  当前商品\n",
#define EditN3                    "<THOAT>        THOAT"//---ZY  //// "<退出>      退出",
#define EditN4                    "<XAC NHAN> TIEP TUC\n"//---ZY  //// "<确认>      继续\n",
#define EditN5                    "CHON BAO CAO"// "按<选择>报表",

#define EditN7                    "BAM PHIM"//---ZY  ////"键码     ->键   ",
#define EditN8                    "CHI O CHUC NANG QUAN LY"// "只能在经理档用",
#define EditN9                    "O CHE DO BAN HANG"// "可在营业档用",
#define EditN10                   "VAN BAN"// "打印文字",
#define EditN11                   "(BAO LUU)"// "保留",
#define EditN12                   "TAP TIN//MAT MA//KIEM TRA"// "文件密码测试",
#define EditN13                   "QUAY SO"// "拨号",
#define EditN14                   "DANG KET NOI"// "MODEM 连接!",
#define EditN15                   "MA:"// "代码:",
#define EditN16                   "GIAM %"// "%折扣",
#define EditN17                   "CONG %"// "%加成",
#define EditN18                   "SO HOA DON:"// "收据号:",
#define EditN19                   "NHUNG TAP TIN"// "文件数："，0
#define EditN20                   "TONG"// "文件空间："?
#define EditN21                   "BAO CAO"// "统计文件"2
#define EditN22                   "THUE"// “计税”8
#define EditN23                   "BO NHO CUON LUU"// "流水空间"
#define EditN26                   "DONG TRONG"// '顶部空行数',
#define EditN27                   "SO DONG TREN TRANG"// '每页行数',
#define EditN28                   "IN NGAY"// '打印信息',
#define EditN29                   "NHAN SEC"// '第二张票据',
#define EditN30                   "LE TRAI"// '左边距',
#define EditN31                   "LOAI"// '票打机型',
#define EditN32                   "TAT HOA DON"// "打印机关",
#define EditN33                   "MO HOA DON"// "打印机开",
  ////ChipCard>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN1                "MAT MA"//I "密码:",
#define ChipCardN2                "BUOC NHAP MAT MA"// "密码确认:",
#define ChipCardN3                "MA POS"// "POS代码:",
#define ChipCardN4                "XAC NHAN MA POS"// "代码确认:",
#define ChipCardN5                "LUU MA POS"// "代码已保存",
#define ChipCardN6                "LOI MA POS"// "代码错 ！",
#define ChipCardN7                "THE CHIP"// "启用IC卡:",
#define ChipCardN8                "GIAM GIA"// "折扣卡:",
#define ChipCardN9                "TIEN MAT"// "现金卡:",
#define ChipCardN10               "THE TIN DUNG"// "赊帐卡:",
#define ChipCardN11               "IN"// "打印IC卡",
#define ChipCardN12               "TU DONG GIAM GIA"// "自动折扣:",
#define ChipCardN13               "DIEM"// "消费积分:",
#define ChipCardN14               "TIEN MAT=CC"// "用现金键:",
#define ChipCardN15               "HAN SU DUNG"// "启用有效期", ////
#define ChipCardN16               "MAT DANH SACH"// "挂失卡号",
#define ChipCardN17               "SO THE"// "卡号:",
#define ChipCardN18               "LOAI THE"// "卡类型:",
#define ChipCardN19               "CAN BANG"// "余额"
#define ChipCardN20               "TIEN GIU THE"// "卡押金:",
#define ChipCardN21               "CHI TIEU"// "消费:",
#define ChipCardN22               "PHI NAP"// "充值总额:",
#define ChipCardN23               "HOA DON"// "使用次数:",
#define ChipCardN24               "MUC GIA"// "价格级别:",
#define ChipCardN25               "MA PIN"// "PIN码:",
#define ChipCardN26               "MUC BAO MAT"// "保护级别:",
#define ChipCardN27               "THANH TOAN TU DONG"// "自动结帐:",
#define ChipCardN28               "NGAY LAM THE"// "制卡日期:",
#define ChipCardN29               "NGAY HET HAN"// "卡有效期:",
#define ChipCardN30               "KHACH HANG"// "客户:",
#define ChipCardN31               "TAO"// "初始化IC",
#define ChipCardN32               "NAP THE IC"// "IC卡充值",
#define ChipCardN33               "XOA THE IC"// "清除IC卡",
#define ChipCardN34               "NAP ?"// "充值//退款?",
#define ChipCardN35               "HOAN TRA ?"// "减点//送点?",
#define ChipCardN36               "XAC NHAN ?"// "确认充值?",
#define ChipCardN37               "XAC NHAN ?"// "确认退款?",
#define ChipCardN38               "XAC NHAN ?"// "确认送点?",
#define ChipCardN39               "XAC NHAN ?"// "兑奖确认?",
#define ChipCardN40               "TONG DIEM DOI"// "计点兑奖",
#define ChipCardN41               "TRA THE"// "IC卡退款",
#define ChipCardN42               "DIEM DOI THE IC"// "IC卡兑奖点数",
#define ChipCardN43               "SO DIEM THE IC"// "IC卡送点数",
#define ChipCardN44               "XIN DOI"// "请等待......",
#define ChipCardN45               "TRUOC TIEN XIN TRA LAI..."// "卡内有金额,请先退款....",

#define ChipCardN48               "KHOI DONG LAI IP MOI"// "要启用新的IP,请重新开机.",
#define ChipCardN49               "XAC NHAN XOA ?"//  "确认清报表?",
#define ChipCardN50               "XIN DOI..."// "等待IC卡....",
#define ChipCardN51               "IN CAO GAP DOI"//---zy  //// "倍高打印",
////cr2015-06-15>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN52             "XOA"//
#define ChipCardN53             "TRA LAI"//
#define ChipCardN54             "THANH TOAN"//
#define ChipCardN55             "PHU THU"//
#define ChipCardN56             "KHOI TAO"//
#define ChipCardN57             "XUAT TIEN"//
#define ChipCardN58            "THEM DIEM//ADDVIPPOINTS"
#define ChipCardN59            "GIAM DIEM//DECVIPPOINTS"
#define ChipCardN60            "TONG DIEM//VIPPOINTS"
////ccr2015-06-15<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MESSEJ_X_STORE      "SAVE X DATA"//MsgEJ_X_STORE
#define MESSEJ_Z_STORE      "SAVE Z DATA"//MsgEJ_Z_STORE
#define MESSEJ_TR_STORE     "SAVE TRAIN"//MsgEJ_TR_STORE
#endif
////ccr2016-02-22>>>>>>>>>>>>>>>>>>>>>
#define MsgMENUXREPORT     "XEM BAO CAO"//
#define MsgMENUXEXPORT     "XUAT(X)"//
#define MsgMENUQUICKVIEW   "DOANH THU"//
#define MsgMENUXLOGFUNC     "XEM CUON LUU"//
#define MsgMENUSENDNEWLOG  "GUI DANG NHAP ECR"      //MENUSENDNEWLOG  //发送新流水数据
#define MsgMENUXPASSWORD   "MAT MA XEM"//
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUXEJOURNAL    "EJ"     //MENUXEJOURNAL   "E Journal"
#endif
//................................
#define MsgMENUZREPORT     "LAY BAO CAO"//
#define MsgMENUZEXPORT     "XUAT (Z)"//
#define MsgMENUCLEARLOG     "XOA CUON LUU"//
#define MsgMENUZGRAND      "CAI DAT XOA TONG"//
#define MsgMENUZPASSWORD   "MAT MA LAY BC"//
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUZRESETEJ   "RESET EJ"     //MENUZRESETEJ   "Clear EJ"
#endif
//...................................


#define EPosN1                    "MA EPOS:"// "ePOS认证码:",
#define EPosN2                    "XIN DOI..."// "等待易通卡"
#define EPosN3                    "SO THE:"// "易通卡号:",
#define EPosN4                    "CAN BANG:"// "易通卡余额:",

#define ErrInfoN1                 "VUOT GIOI HAN"// "通讯数据过长"
#define ErrInfoN2                 "***IN GIAN DOAN***"// " *** 打印被中断、重新打印 *** "
#define ErrInfoN3                 "LOI DU LIEU !"// "数据校验错"
////PRINT


#define FisTextN1              "NOP THUE SO:"//"纳税人号:"
#define FisTextN2                 "IN PHIEU"//"开出税票        "
#define FisTextN3                 "SO UY QUYEN"//"授权号码        "
#define FisTextN4                 "CONG SO:"//"写机器号        "
#define FisTextN5                 "XAC NHAN CONG"//"写入确认        "
#define FisTextN6                 "IN DU LIEU"//"数据印出        "
#define FisTextN7                 "MA SO NGUOI DUNG"//"用户号码        "
#define FisTextN8                 "MIEN THUE"//"免税状态        "
#define FisTextN9                 "SO LAN LAY Z"//"Z-计数:         "
#define FisTextN10                "TONG CHIU THUE"//"累计销售        "
#define FisTextN11                "THUE TICH LUY"//"累计税          "
#define FisTextN12                "TONG MIEN THUE"//"总免税额        "
#define FisTextN13                "TONG CHIU THUE"//"总销售额        "
#define FisTextN14                "TONG THUE"//"总税额          "
#define FisTextN15                "HOA DON NGAY"//"日税票数        "
#define FisTextN16                "TONG PHIEU THUE"//"收据总数        "
#define FisTextN17                "BAO CAO TONG"//"余报表数 :      "




#define CurrentTimeN1                    "GIO:"//  "当前时间"
#define InputMode1			"A:"// "A："

#define SpecialSymbol1			"<<"			////RUT//DOWNLOAD////Ccr "★",
#define SpecialSymbol2			">>"			////TAI//UPLOAD////Ccr "☆",
#define SpecialSymbol3			"->  <-"		////TRUYEN// "↑  ↓",
#define MESS_PRESSKEY          "BAM PHIM BAT KY"//

#if defined(CASE_QRCODE)
#define InputQRCode           "MA QR"//MsgQRCODE
#endif

#define GPRSServerIP			"IP MAY CHU"//"服务器IP"
#define GPRSServerPort		    "CONG"//"端口号"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////added by lqw 090802
#if  (defined(CASE_GPRS))
#define GPRSAPNName        "APN//GPRSAPNNAME"
#define GPRSUserName       "TEN NGUOI DUNG//GPRSUSERNAME"
#define GPRSPassword       "MAT MA//GPRSPASSWORD"

#define GPRSSendMESS       "Check GPRS"//ccr2014-11-11 NEWSETUP
#define GPRSSetMode         "DAT CHE DO GUI"//gprsSETMODE ////ccr2014-11-11 NEWSETUP-step2
#define GPRSSendECRLogAll   "GUI TAT CA DANG NHAP"//gprsSendECRLogAll ////ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadPLU     "CAP NHAT MA HANG"//gprsDownloadPLU ////ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadDEPT    "CAP NHAT NHOM"//gprsDownloadDEPT ////ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadCLERK   "CAP NHAT NHAN VIEN"//gprsDownloadCLERK ////ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadHEAD    "CAP NHAT TIEU DE DAU"//gprsDownloadHEAD ////ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadTRAIL   "CAP NHAT TIEU DE CUOI"//gprsDownloadTRAIL ////ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadALL     "CAP NHAT TIEU ALL"      //gprsDownloadALL //ccr2016-08-18
#define GPRSRestart         "RESET GPRS UNIT"      //gprsRestart //ccr2016-08-26

#define GPRSstr1 "LOI DU LIEU"// "数据类型错"
#define GPRSstr2 "WIRELESS OK"//
#define GPRSstr3 "WIRELESS CHUA OK"//
#define GPRSstr4 "XAC NHAN"//
#define GPRSstr5 "IP//CONG BI RONG"//
#define GPRSstr6 "LOI IP//CONG"//
#define GPRSstr7 "DANG CHO WIRELESS"//
#define GPRSstr8 "DONG Y"// "是"
#define GPRSstr9 "KHONG DONG Y"// "fou "
#define GPRSstr10 "DANG TRUYEN..."// "数据发送中.. "
#define GPRSstr11 "HOAN TAT"// "数据已发出.. "
#define GPRSstr28 "THANH CONG...."// "发送成功.."
#define GPRSstr31 "LOI KET NOI"//						 "连接失败"
#define GPRSstr32 "DOC THAT BAI"// "接受失败"
#define GPRSstr33 "XAC NHAN THAT BAI"// "发送确认失败"
#define GPRSstr34 "THAT BAI"// "发送失败"
#define GPRSstr58 "KET NOI...."// "正在建立连接.."
#define GPRSstr59 "KET NOI...."// " 准备建立连接.."
#define GPRSstr60 "KHOI DONG WIRELESS"// "正在复位模块.."
#define GPRSstr61 "DA GUI XONG"// "发送完毕,请退出"
#define GPRSxACK  "ACK NOT RECEIVED"
#endif ////end defined(CASE_GPRS)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define InsertUSB		"<<XIN GAN USB"//CHRUSB  插入U盘 ////

////ccr090715>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define usbMAIN1		"SAO CHEP SANG USB"//usbDAOCHU	导出收款机文件 ////
#define usbMAIN2		"SAO CHEP TU USB"//usbDAORU 导入收款机文件 ////
#define usbMAIN3		"CUON LUU SANG USB"//usbFLOW 导出销售流水 ////


#define usbFLOWNEWMESS	    "XUAT DL MOI"//1-导出新数据   ////
#define usbFLOWALLMESS	    "XUAT TOAN BO"//2-导出全部数据
#define usbFLOWDELMESS	    "XUAT VA RESET"//3-导出全部数据后清空
#define usbALLFile		"TAT CA TAP TIN"//usbALL 全部文件 ////


#define MESSPRNMENU     "<00> IN MENU"//
#define MESSSETMENU     " MANUALS LIST"//
#define MESSCLEANLOG    "XOA CUON LUU DIEN TU?"//
#define MESSCLEANLOGA   "TAT CA CUON LUU\nSE BI XOA!"//
#define MESSRAMOUT      "BI TRAN RAM"//
#define MESSCLEANFILES  "XOA TAP TIN?"//
#define MESSCONFIGERR   "THAY DOI CAU HINH,\nSE BI LOI NEU\nDUNG TAP TIN CU"//
#define MESSDELPLU      "BIEN THAY DOI\n   XOA MA HANG"//
#define MESSPLUPRINTEND "IN MA HANG O.K.\nBAM <ENTER>"//

#define RECALLFLOW      "GOI LAI CUON LUU"//

#define USBREPNAME      "TEN"//

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

#define MESS_LOGREPET   "Re-Send All LOG!"
#define MESS_PAYALSO   "Confirm Payment?"//


//ccr2016-01-12>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//...............................................
#define MGADDINV        "NHAP KHO"//1+SUBTOTAL
#define MGDECINV        "TRU KHO"//2+SUBTOTAL
#define MGSETPRICE      "GIA"//3+SUBTOTAL

#define MGCLEARIC       "TRA THE IC"//
#define MGINITIC        "TAO THE IC"//
#define MGCHARGE        "NAP THE IC"//
#define MGADDPOINT      "THEM DIEM"//
#define MGPRINTSTOCK   "LIST STOCK"

#define MGPASSWORD      "CAI DAT MAT MA"//
#define MGCLOSEPRN      "TAT MAY IN"//
#define MGTRAINING      "CHE DO THUC TAP"//
#define MGGOTOMG        "CHE DO QUAN LY"//

//..............................................
#define PLUCODE     "PLU CODE"
#define INVQTYMUST  "INPUT QTY OR PRICE"
#define ININVENTORY "Press <SUBT> Key\n Exit Inventory!"
//..............................................
#define DEVICENULL	"RONG"//
#define DEVICEPORT1     "CONG 1"//
#define DEVICEPORT2     "CONG 2"//
#define DEVICEPORT3     "CONG 3"//
#define DEVICEPORT4     "CONG 4"//

////................................................
#define CLOSEPRINTER    "TAT MAY IN"//
#define CLEARGRANDTOTAL "XOA TONG SAU KHI LAY BC Z"//
#define CLEANALLECRVIP  "XOA TAI KHOAN?"//"清除会员数据?"
////.......功能码对应的名称.........................................
#define MsgKEYCORR      "SUA SAI"//
#define MsgKEYCURR      "TIEN TE"//
#define MsgKEYDISC      "GIAM GIA"//
#define MsgKEYPBFUN     "CHUC NANG BAN"//
#define MsgKEYPORA      "CHI THU"//
#define MsgKEYPO_RA      "CHI/THU"
#define MsgKEYTEND      "THANH TOAN"//
#define MsgKEYCLEAR     "XOA"//
#define MsgKEYFEED      "CHAY GIAY"//
#define MsgKEYMULT      "X"//
#define MsgKEYSUBTOT    "TONG CONG"//
#define MsgKEYPRICE     "GIA"//
#define MsgKEYPRICEN    "GIA MO"//
#define MsgKEYPRICE1    "GIA 1"//
#define MsgKEYPRICE2    "GIA 2"//
#define MsgKEYPRICE3    "GIA 3"//
#define MsgKEYPRICE4    "GIA 4"//
#define MsgKEYSHIFT     "TANG"//
#define MsgKEYDATE      "NGAY"//
#define MsgKEYWEIGHT    "NANG"//
#define MsgKEYCLERKN    "CA"//
#define MsgKEYDEPT      "NHOM~"//
#define MsgKEYDEPTNo    "NHOM#"//
#define MsgKEYPLUNo     "MA HANG#"//
#define MsgKEYDRAW      "MO KET"//
#define MsgKEYMEMO      "BO NHO"//
#define MsgKEYCLERK     "CA*"//
#define MsgKEYPLUDIR    "MA HANG~"//
#define MsgKEYLOCK      "VI TRI KHOA"//
#define MsgKEYDRAWFUN   "CHUC NANG KET*"//
#define MsgKEYSALPN     "N.VIEN PHUC VU#"//
#define MsgKEYSALP      "N.VIEN PHUC VU"//
#define MsgKEYDOT       "'.'"
#define MsgKEYZERO2     "'00'"
#define MsgKEYNUMBER    "0~9"
#define MsgKEYSUSPEND   "GIU"//
#define MsgFUNCLOOK1    "CHUC NANG TIM KIEM 1"//
#define MsgFUNCLOOK2    "CHUC NANG TIM KIEM 2"//
#define MsgMODELOCK     "VI TRI KHOA"//
#define MsgVIPLOGIN     "VAO TAI KHOAN"//
#define MsgINPUTNUM     "NHAP MA VAO"//
#define MsgCUSTOMER     "CUSTOMER"
#define MsgKEYNULL      "RONG"//

#define MsgRHEADER      "Header*"
#define MsgRTRAILER     "Trail*"
#define MsgKEYBOARD     "Keyboard*"
#define MsgSYSFLAGS     "System*"
#define MsgSETDATE      "Datetime*"

////...........................................
#define MsgCONFIRM      "XAC NHAN"//
#define MsgNET          "TINH"// "净额",
#define VIPLOGOUT       "XUAT TAI CHINH"//
#define VIPTOTAL        "GIA TRI"//
#define VIPTOTALA       "TONG GIA TRI"//
#define DECVIPPOINT     "GIAM"//长度必须为4
#define ADDVIPPOINT     "TANG"//长度必须为4
#define INPUTVIPNAME    "TEN"//
#define PRINTKEYBOARD   "IN BAN PHIM"//
////..........................................
#define MESSALLLOG       "CUON LUU THEO SO "//
#define MESSDATERANGE   "CUON LUU THEO NGAY"//
#define MESSVIPLOG      "VAO TAI KHOAN"//
#define MESSPRNCASH     "IN TIEN MAT"//"打印付款记录"
#define MESSPRNDEPART   "IN NHOM"//"打印部类记录"
#define MESSPRNPLU      "IN MA HANG"//"打印单品记录"
#define MESSPRNDISCOUNT "IN GIAM GIA"//"打印折扣加成"
#define MESSPRNRAPO     "IN CHI//THU"//"打印出入金"
#define MESSPRNCORRECT  "IN SUA SAI"//"打印更正操作"
#define MESSPRNCANCEL   "IN HUY"//"打印交易取消"
#define MESSPRNNOSALE   "IN MO KET"//"打印非销售"

#define MESSALLCASH	"TAT CA TIEN MAT"//"全部付款方式"
#define MESSSELCASH	"CHON 1 TIEN MAT"//"选择付款方式"
#define MESSALLDEPT	"TAT CA NHOM"//"打印全部部类"
#define MESSSELDEPT	"CHON 1 NHOM"//"选择一个部类"
#define MESSALLPLU	"TAT CA MA HANG"//"打印全部单品"
#define MESSSELPLU	"CHON 1 MA HANG"//"选择一个单品"
////........................................
////ccr2016-03-15>>>>定义键盘宏>>>>>
#define MESSMACRO       "TICH HOP"//  "宏定义"
////....ListItems提示信息....................................
#define It_MGITEMS      "CHUC NANG QUAN LY"//
#define It_SELEFUNC     "PHIM CHUC NANG"//
#define It_SELEITEM     "CHON MAT HANG"//
#define It_SELEKEYTYPE  "LOAI BAN PHIM"//
#define It_SELETEST     "CHON MAT HANG"//
#define It_SELELOG      "CHON DANG NHAP"//
#define It_SELETEND     "CHON KIEU"//
#define It_SELEDEPT     "CHON NHOM"//
#define It_SELEPLU      "CHON MA HANG"//
#define It_SELEMODI     "CHON BO NHO"//
#define It_SELEREPORT   "CHON BAO CAO"//
#define It_SELEPOINT    "CHON DIEM"// 小数位数"
#define It_SELEEJ       " EJ      "
////........................................................
#define MESSRAMVAR      "VARIABLE"
////.........................................................
#define PLUNUMBER           "SO BANG GHI"//
#define PLURANDOMSIZE     "KICH CO CUA MA"//
#define PLUNAMESIZE	"KICH CO CUA TEN"//
#define PLUPRICESIZE      "KICH CO CUA GIA"//
#define PLUPRICELEVEL     "GIA TAI KHOAN"//
#define PLUCOSTSIZE       "KICH CO GIA CA"//
#define PLUINVENTSIZE     "KICH CO CUA KHO"//
#define PLUREPQTYSIZE     "KICH CO CUA COT S.L"//
#define PLUREPAMTSIZE     "KICH CO CUA COT THANH TIEN"//
#define PLUREPDISCSIZE    "KICH CO CUA COT GIAM GIA"//
#define PLUREPCOSTSIZE    "KICH CO CUA COT GIA"//
////"//........................................................
#define POINTTYPE         "DAU CHAM"//
#define POINTTYPE1        "1.000,00"////"2 SO THAP PHAN"//两位小数
#define POINTTYPE2        "1,000.00"////"2 SO THAP PHAN"//两位小数
#define POINTTYPE3        "   1.000"
#define POINTTYPE4          "   1,000"
#define POINTTYPE5          "1.000,000"  //"3 SO THAP PHAN"/三位小数
#define POINTTYPE6          "1,000.000"  //"3 SO THAP PHAN"/三位小数
//#define POINTTYPE7          "1.000,0"  //"1 SO THAP PHAN"/一位小数
//#define POINTTYPE8          "1,000.0"  //"1 SO THAP PHAN"/一位小数
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
#define DATETYPE        "DINH DANG"//
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

#define CSV_TITLE       "TEN HANG,SO LUONG,SO TIEN\r\n"

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

