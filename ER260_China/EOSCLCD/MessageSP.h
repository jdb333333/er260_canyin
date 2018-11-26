#ifndef MESSAGE_SP
#define MESSAGE_SP


//----------Length<=32------------------
#define Header1         "*       LO MEJOR PARA USTED   *" // "*   创 造 优 秀  优 创 造 就  *"
#define Header2         "*       LO MEJOR PARA USTED  *" // "*   创 造 优 秀  优 创 造 就  *"

#define ReportType0     "TOTAL VENTA  "// " 销  售  总  计 "
#define ReportType1     "INFO-GRUPO     "//" 柜  组  统  计 "
#define ReportType2     "INFO-DEPT      "//" 部  类  统  计 "
#define ReportType3     "INFO-ARTICULO "//" 单  品  统  计 "
#define ReportType4     "INFO-PAGO   "// " 付  款  统  计 "
#define ReportType5     "INFO-CAJA +/-  " //" 出 入 金 统 计 "
#define ReportType6     "INFO-CAJA     " //" 钱  箱  统  计 "
#define ReportType7     "INFO-ALTERACION " // " 更  改  统  计 "
#define ReportType8     "INFO-DESCUENTO  " // " 折  扣  统  计 "
#define ReportType9     "INFO-DIVISA    " //" 外  币  统  计 "
#define ReportType10    "INFO-IMPUESTO   " //" 计  税  统  计 "
#define ReportType11    "INFO-CUENTA-MESA" // " 桌  帐  统  计 "
#define ReportType12    "INFO-ALIMENTACION" // " 餐  饮  统  计 "
#define ReportType13    "  INFO-MESA     "//" 桌  台  统  计 "
#define ReportType14    "  ALMACEN       "//" 商  品  库  存 "
#define ReportType15    " FACTURA        "
#define ReportType16    "BUFFER VENDEDOR "

#define ReportType17    "INFO-CAJERO   "//" 收 款 员 统 计 "
#define ReportType18    "INFO-VENDEDOR  "// " 营 业 员 统 计 "

#define Trailer1        "     GRACIAS POR LA VISITA  "//"     ~多  ~谢  ~光  ~临   "
#define Trailer2        "     HASTA LA PROXIMA VISITA"//"           ~再  ~见"
//----------Length<=16------------------
#define ReportList1     "DIA CAJERO"//" 收 款 员 日 报 "
#define ReportList2     "SEMANA CAJERO"//" 收 款 员 周 报 "
#define ReportList3     "DOC/DIA -VENTA"//" 销  售  日  报 "
#define ReportList4     "DOC/SEMANA-VENTA"//" 销  售  周  报 "
#define ReportList5     "DOC-ARTICULO"//" 商  品  报  表 "
#define ReportList6     "DOC-CUENTA-MESA "//  " 桌  帐  报  表 "
#define ReportList7     "DOC-TIEMPO"//" 时  段  报  表 "
#define ReportList8     "D-TODOCAJERO"// "  全收款员日报  "
#define ReportList9     "S-TODOCAJERO"//"  全收款员月报  "
#define ReportList10    "DOC/DIA-VENDEDOR"//" 营 业 员 日 报 "
#define ReportList11    "SEMANA VEND."//" 营 业 员 周 报 "
#define ReportList12    "DIA-TODOVEND."// "  全营业员日报  "
#define ReportList13    "SEMANATODOVEN"// "  全营业员月报  "

#define Promotion1      " USTED TOCA PREMIO  "// "  ~恭  ~喜  ~中  ~奖"

//----------Length<=12------------------
#define Correc1         "CANCELAR"//"~取~消  "
#define Correc2         "ALTERAR "//  "~更~改  "
#define Correc3         "DEVOLUCION"// "~退~货  "
#define Correc4         "CANCEL PEDID"//"~取~消~交~易"
#define Correc5         "APARCA �"//   "~挂~帐"

#define CURRENCY1         "DOLAR   "//"美元    "
#define CURRENCY2         "YEN     "//"日元    "
#define CURRENCY3         "HK $    "//"港币    "
#define CURRENCY4         "DIVISA 1"// "外币 1  "
#define CURRENCY5         "DIVISA 2"//"外币 2  "


#define DISCOUNT1          "COMIS.%"//"%加成"
#define DISCOUNT2          "DTO-%"//  "%折扣"
#define DISCOUNT3          "COMIS.�"// "金额加成"
#define DISCOUNT4          "DTO-�"//  "金额折扣"
#define DISCOUNT5          "COMISION-#"//"单项加成"

#define DISCOUNT6          "S.TOTAL %+"//"小计%加成"
#define DISCOUNT7          "S.TOTAL %-"// "小计%折扣"
#define DISCOUNT8          "+ S.TOTAL"//"小计直加"
#define DISCOUNT9          "- S.TOTAL"//"小计直减"


#define DRAWER1            "CASH"// "现金    "
#define DRAWER2            "CHEQUE  "// "支票    "
#define DRAWER3            "T.CREDITO "//"信用卡  "
#define DRAWER4            "CUPON   "//"购物券  "
#define DRAWER5            "CREDIT  "//"易通卡  "
#define DRAWER6            "IC "// "IC卡    "
#define DRAWER7            "PROPINA "// "小费    "
#define DRAWER8            "DRAW-8  "//打印/显示

#define PBFunction0         "ABRRMESA"// "开台挂帐"
#define PBFunction1         " + MESA  "//"开台增加"
#define PBFunction2         "PAGO TEMPORA"// "暂结    "
#define PBFunction3         "CONFIRMAPAGO"// "确认结帐"
#define PBFunction4         "PRINT MESA"//"打印开台"
#define PBFunction5         "PRINT CUENTA"//"打印帐单"
#define PBFunction6         "CANCELAR"//"取消确认"
#define PBFunction7         "DIVIDECUENTA"// "分帐    "
#define PBFunction8         "TRANS-CUENTA"// "转帐    "
#define PBFunction9         "TRANSFCUENTA"//"转桌帐  "
#define PBFunction10        "GENTE NO  "// "人数    "

#define PORAType1            "P.O"//ccr2016-08-09"CAJA + �"//"出金    "
#define PORAType2            "R.A"//ccr2016-08-09"CAJA - �"//"入金    "
#define PORAType3            "DEVOLVE�IC"//"IC卡退款"
#define PORAType4            "RECARGA"//  "IC卡充值"
#define PORAType5            "-FINANZA"//"退IC押金"
#define PORAType6            "+FINANZA"//"收IC押金"

#define TendType1             "CASH"//"现金    "
#define TendType2             "CHEQUE  "//"支票    "
#define TendType3             "T.CREDITO"//   "信用卡  "
#define TendType4             "CUPON   "//"购物券  "
#define TendType5             "CREDIT  "//"易通卡  "
#define TendType6             "IC "//  "IC卡    "
#define TendType7             "DEBIT CARD"        //"DEBIT CARD"
#define TendType8             "CASH-8    "        //"CASH-8"


#define Modifier1             "INSTRUCCION"// "说明    "
#define ClerkRec1             "CAJERO  "//"收款员  "
#define GroupRec1             "GRUPO   "//"柜组    "
#define DeptRec1              "DEPT"//"部类    "
#define PLURec1               "ARTICULO "//"商品    "
#define SalesPersonRec1       "VENDEDOR"//"营业员  "
#define OffRec1               "OFERTA  "//  "特殊出价  "
//----------Length<=14------------------
#define XZReportList1         "CIERRE DIARIO"// "销售日报表"
#define XZReportList2         "SEMANA-VENTA"//  "销售月报表"
#define XZReportList3         "DOC-ARTICULO  "//"商品报表"
#define XZReportList4         "MESA CUENTA"// "桌帐报表"
#define XZReportList5         "DOC-TIEMPO"//  "时段报表"
#define XZReportList6         "DIA CAJERO"// "收款员日报表"
#define XZReportList7         "SEMANA CAJERO"//"收款员月报表"
#define XZReportList8         "D-TODOCAJERO"// "全收款员日报表"
#define XZReportList9         "S-TODOCAJERO"//  "全收款员月报表"
#if (salNumber)
#define XZReportList10        "DIA-VENDEDOR"//"营业员日报表"
#define XZReportList11        "SEMANA VEND."// "营业员月报表"
#define XZReportList12        "DIA-TODOVEND."// "全营业员日报表"
#define XZReportList13        "SEM-TODOVENDEDO" //"全营业员月报表"
#endif
#if (BARCUSTOMER==1)
#define XZReportList14        "SOCIO/BARCODE"//  "条码会员报表"
#endif
#define XZReportList15        "GRABA EN U" //ccr090715 "报表存U盘"

//----------Length<=6------------------
#define PortType1               "  PC"//"  主机"
#define PortType2               "SCANER"//"条码枪"
#define PortType3               "BALANZ"// "电子秤"
#define PortType4               "COCINA"//"厨  打"
#define PortType5               "TICKET"//"票 打"
#define PortEXTPRN              "EP-PORT"//ccr2016-12-14 ExtPrinter????
#define PortType6               "MR IC"//"射频卡"
#define PortType7               "E CARD"//"E通卡"

#define KClose                  "CERRAR"//  "   关 "

//----------Length<=10------------------
#define SysFlags0		        "LOCAL NO:    "//"店铺编号:"
#define SysFlags1               "CAJA NO:     "// "收款机号:"
#define SysFlags2               "ROUNDING::"//"舍入方式:"
#define SysFlags3               "CAMBIA PVP:"//"商品变价:"
#define SysFlags4               "SILBATO:"//  "蜂鸣器:"
#define SysFlags5               "DESIGNA C"//"强制收款员"
#define SysFlags6               "CAMBIAR C"// "改变收款员"
#define CLERKPASS               "CLAVE C."//"收款员密码"
#define SysFlags8               "PRINT.VEND."// "打印营业员"
#define SysFlags9               "PRINT C."//  "打印收款员"
#define SysFlags10              "PRINT DOC#"// "打印收据号"
#define SysFlags11              "OSCURO PRINT"// "高浓度打印"
#define SysFlags12              "DESIGNA V."//"强制营业员"
#define SysFlags13              "IMPRIME#"// "打印编码:"
#define SysFlags14              "PULSES.TOTAL:"// "按小计键:"
#define SysFlags15              "IMPR.VENTA"// "打印销售量"
#define SysFlags16              "IMPRIME DOC:"// "打印小票:"
#define SysFlags17              "CABEZA-TIC:"//"小票票头:"
#define SysFlags18              "BLOQ SHIFT:"//"锁定SHIFT:"
#define SysFlags19              "MUESTRAHORA:"//"显示时间:"
#define SysFlags20              "COPIA DOC:"// "复制收据:"
#define SysFlags21              "ESPECIAL #:"// "特殊条码:"
#define SysFlags22              "MONTA DIA:"//"操作日志:"
#define SysFlags23              "PRINT.MESA:"//"开台打印:"
#define SysFlags24              "PRINT.STCOK:"//"打印库存:"
#define SysFlags25              "INTERRUPTOR:"// "打印开关:"
#define SysFlags26              "OBL.MESA:"// "必须开台:"
#define SysFlags27              "CONSERVA#"// "保持收据号"
#define SysFlags28              "COPYCOCINA:"// "复制厨打:"
#define SysFlags29              "RECUPERA#"// "收据号复位"
#define SysFlags30              "ALTA IMPRIME:"//"倍高打印:"
#define SysFlags31              "PARIDAD:"//  "对比度:"
#define SysFlags32              "CBZ.CENTRO:"//"票头居中:"
#define SysFlags33              "PIE CENTRO:"//  "票尾居中:"
#define SysFlags34              "ALTA CABEZA:"//票头倍高:
#define SysFlags35              "ALTA PIE:"//票尾倍高
//ccr2016-03-21#define SysFlags36              "EXPORTAR CSV"//".CSV导出"
#define SysFlags37              "PRINT FECHA"// "打印日期"
#define SysFlags38              "PRINT HORA"// "打印时间"
#define SysFlags39              "PRINT INDEX"    //"打印取菜号"
#define SysFlags40              "HEART BEAT"    //ccr2015-08-03"向服务器发送Heart beat"
#define SysFlags41              "PreFix1:"      //ccr2016-08-02
#define SysFlags42              "PreFix2:"      //ccr2016-08-02

//----------Length<=9------------------
#define GrapType1               "MARCA"// " 商店图标"
#define GrapType2               "PREMIO"//" 中奖图标"
#define GrapType3               "LOGO-PIE"//" 票尾图像"
#define GrapType4               "IMPRIME ?"// "打印文字?"

#define GrapSet1                "NO#:"//"序号:"
#define GrapSet2                "FOTO REF."//"图像编号:"
#define GrapSet3                "INICIO"//"开始日期:"
#define GrapSet4                "FECHA-FIN:"//"结束日期:"

  /////======================CHN msg========================//////

#define CurrentMode1            "ABIERTO"//"营业"
#define CurrentMode2            "LEE CUENTA"//"读帐"
#define CurrentMode3            "QUITA CUENTA"//"清帐"
#define CurrentMode4            "CONFIG."//"设置"
#define CurrentMode5            "GERENTE"//"经理"

//----------Length<=14------------------
#define SetupMode1              "FUNCION GRAL."//"综合功能"
#define SetupMode2              "GESTION-ART."//"商品管理"
#define SetupKeyboard           "KEYBOARD"      //DEFKEYBOARD  //
#define msgCOMMHOST             "COMM-HOST"      // 这项不需要???ZWQ
#define SetupMode3              "DEVICE SET"// "通讯与外设"
#define SetupMode4              "CONFIG.VENTA"//"销售功能设置"
#define SetupMode5              "ACCOUNT"//"IC卡管理"
#define SetupMode6              "CABEZA-PIE"//"票头票尾"
#define SetupMode7              "GESTION-DOC"//"报表管理"
#define msgDISCOUNT             "DISCOUNT"      //MENUDISC //
#define SetupMode8              "CONFIGURA"//"餐饮设置"
#define SetupMode9              "CAJERO-CLAVE"//"收银员和密码"
#define SetupMode10             "PROBAR CAJA"//"收款机测试"

#define SysSettings1            "CONFIG.SISTEMA"//"系统参数设置"
#define SysSettings2            "CONFIG.FECHA"// "设置日期"
#define SysSettings3            "CONFIG.HORA"//"设置时间"
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define msgSETEJOURNAL          "CONFIG.EJ"      //SETEJOURNAL //EJOURNAL
#endif

#define SysSettings4            "CONFIG.TECLADO"//  "设置键盘"
#define MESSKEYMACRO            "KEYB. MACRO" // "键盘宏"
#define MESSKEYTYPE             "KEYBOARD TYPE"             // "键盘模式"
#define SysSettings5            "TECLADOCOMIDA"// "启用餐饮键盘"
#define SysSettings6            "TECLADOSUPERM"//"启用超市键盘"
#define SysSettings7            "EXP/IMP DATOS"  //SETEXPORT//  "数据导出/导入"
#define SysSettings8            "GPRS FUNCTION"          //"GPRS通信"  //SETGPRS //ccr2014-11-11 NEWSETUP

#define SetupCaption1           "GRUPO"//"柜组"
#define SetupCaption2           "DEPT"//"部类"
#define SetupCaption3           "ARTICULO"//"单品"
#define SetupCaption4           "IMPRIME ART.#"//"打印商品清单"
#define msgSETTEST              "TEST ECR"//"系统测试"            //SETTEST
#define msgSETCLEARPLU          "CLEAR PLU"//"清除单品"        //SETCLEARPLU
#define msgSETPLULIST           "PRINT PLU"//"打印商品清单"      //SETPLULIST
#define msgSETTSTVER	        "PRINT VERSION"//"打印版本"      //SETTSTVER
#define msgSETCONFIGPLU         "FORM PLU"//"配置单品"          //SETCONFIGPLU
#define msgSETCONFIGDEPT        "FORM DEPT" //配置部类         //SETCONFIGDEPT
#define msgSETINSTALL           "INSTAL/BACKUP"//"安装/备份"        //SETINSTALL
#define msgSETPRINTFAT          "PRINT MAT"//"打印文件分配表"   //SETPRINTFAT

#define SetupCaption5           "COMPUTER"//"通讯口1"
#define SetupCaption6           "BARCODE"//"通讯口2"
#define SetupCaption7           "E-BALANCE"//"通讯口3"
//#define SetupCaption8           "INTERFAZ 4"//"通讯口4"
#define SetupCaption9           "NETWORK"// "网络设置"

#if defined(CASE_QRCODE)
#define InputQRCode           "QRCODE"  //MsgQRCODE
#endif

#define GPRSServerIP			"SERVIDOR IP"//"服务器IP"
#define GPRSServerPort		    "INTERFAZ"//"端口号"

#if (defined(CASE_GPRS))
#define GPRSAPNName        "APN"           //GPRSAPNNAME
#define GPRSUserName        "User Name"           //GPRSUSERNAME
#define GPRSPassword        "Password"           //GPRSPASSWORD

#define GPRSSendMESS            "Check GPRS" //ccr2014-11-11
#define GPRSSetMode        "Set Sent Mode"    //gprsSETMODE //ccr2014-11-11 NEWSETUP-step2
#define GPRSSendECRLogAll   "Send All LOG"    //gprsSendECRLogAll //ccr2014-11-11 NEWSETUP-step2

#define GPRSDownloadPLU     "Update PLU"    //gprsDownloadPLU //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadDEPT    "Update DEPT"    //gprsDownloadDEPT //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadCLERK   "Update CLERK"    //gprsDownloadCLERK //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadHEAD    "Update HEAD"       //gprsDownloadHEAD //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadTRAIL   "Update TRAIL"      //gprsDownloadTRAIL //ccr2014-11-11 NEWSETUP-step2
#define GPRSDownloadALL     "Update ALL"      //gprsDownloadALL //ccr2016-08-18
#define GPRSRestart         "RESET GPRS UNIT"      //gprsRestart //ccr2016-08-26
#endif

#define SetupCaption10          "PRINT DOC"//"票据打印"
#define SetupCaption11          "PRINT.COCINA"//"厨房打印"
#define SetupEXTPrinter         "EXTERNAL PRINT"    //SETEXTPRINTER

#define SetupCaption12          "OFERTA"//"优惠价"
#define SetupCaption13          "CONFIG.PREMIO"//"中奖设置"
#define SetupCaption14          "DESCUENTO"//"折扣"
#define SetupCaption15          "CAJA +/- �"//"出金入金"
#define SetupCaption16          "REVISAR"//"更正功能"
#define SetupCaption17          "PAGO FORMA "//"付款方式"
#define SetupCaption18          "DIVISA"//"外币"
#define SetupCaption19          "CAJA"// "钱箱"
#define SetupCaption20          "IMPUESTO%"//"税率"

#define SetupCaption21          "SET.ACCOUNT"//"IC卡设置"
#define SetAddNewECRVIP         "ADD ACCOUNT"
#define SetPrintECRVIP          "PRINT ACCOUNT"
#define SetCleanALLVIP          "CLEAN ACCOUNT"

#define SetupCaption22          "PERDER.IC"//"IC卡挂失"
#define SetupCaption23          "PRINT IC"//"打印卡信息"

#define SetupCaption24          "CABEZA-TIC"//"收据票头"
#define SetupCaption25          "PIE-TIC"//"收据票尾"
#define SetupCaption26          "CABEZA-DOC"// "票据票头"
#define SetupCaption27          "PIE FOTO  "//"票尾图片"
#define SetupCaption28          "IMPRIME FOTO"//"打印图片"

#define SetupCaption29          "CONFIG.DOC"// "设置报表内容"
#define SetupCaption30          "DOC MODULO"//"报表单元"
#define SetupCaption31          "CONFIG.HORA"// "报表时段设置"
#define SetupCaption32          "QUITA TODODOC"//"总清报表"
#define SetupCaption33          "IMPRIME C/C"//"打印流水帐"

#define SetupCaption34          " MESA"//"桌台"
#define SetupCaption35          "ALIMENTACION"//"餐饮功能"
#define SetupCaption36          "MENU"//"菜单说明"
#define SetupCaption37          "FIRMAR CUENTA"//"签单"
#define SetupCaption38          "CAJERO"//"收款员"
#define SetupCaption39          "VENDEDOR"//"营业员"
#define SetupCaption40          "CONFIG.CLAVE"//"设置档密码"
#define SetupCaption41          "CLAVE-GERENTE"//"经理档密码"
#define SetupCaption42          "CLAVE-LEER"//"只读报表密码"
#define SetupCaption43          "CLAVE-QUITA"//"清帐报表密码"

#define SetupCaption44          "MEMORIA TEST"//"内存测试"
#define SetupCaption45          "DISPLAY TEST"//"显示测试"
#define SetupCaption451         "CUSTOMER"      // 客显测试
#define SetupCaption46          "IMPRESORATEST"//"打印测试"
#define SetupTestRAM           "CHECK RAM"        //SETTSTMEM	       //RAM测试
#define SetupCaption47          "TIEMPO TEST"//"时钟测试"
#define SetupCaption48          "SILBATO TEST"//"蜂鸣器测试"
#define SetupCaption49          "CAJA TEST"// "钱箱测试"
#define SetupCaption50          "TECLADO TEST"//"键盘测试"
#define SetupCaption51          "SERIE TEST"// "串口测试"
#define SetupCaption52          "AUTO-TEST"// "自动测试"
#define SetupCaption53          "REPARTIR DOC"//"文件分配表"

//----------Length<=8------------------
#define msgSETPRNMENU           "PRINT MENU"      //SETPRNMENU   //
#define OperationN1             "GRABAR#"//"记录#"
#define OperationN2             "GRABAR?"//"记录?"
#define OperationN3             "TOTAL"// "总计"
#define OperationN4             "VUELTO"//ccr2016-08-08"EN CAMBIO"//"转换"
#define OperationN5             "SUBTOTAL"//"小计"
#define OperationN6             "CAJA + �"// "出金"
#define OperationN7             "CAJA - �"//"入金"
#define OperationN8             "RESUMEN"//  "报表"
#define OperationN9             "ANADIR"// "增加"
#define OperationN10            "PEDI.ABIERTO"// "挂单"  too long for display
#define OperationN11            "CANCELAR"// "取消"

#define OperationN12            "CONTRASENA"//"密码"
#define OperationN13            "CONFIR.CLA"// "密码确认"
#define OperationN14            "PVP:"// "单价:"
#define OperationN15            "IMPORTE:"//"金额:"
#define OperationN16            "SUPERMERCADO"//"超市版"
#define OperationN17            "RESTRAURANTE"//"餐饮版"
#define OperationN18            "TEST"//  "测试"


#define CaptionN1               "TOTAL"//ccr2016-08-08"DEBIDO"// "应付"
#define CaptionN2               "IMPORTE BRUTO"//"净额"
#define CaptionN3               "PVP"//"价格"
#define CaptionN4               "CANT-PEDIDO"//"交易数"
#define CaptionN5               "CANTIDAD"//"数量"
#define CaptionN6               "IMPORTE"//"金额"
#define CaptionN7               "ENTREGADO"//ccr2016-08-08"COBRAR"// "收取"
#define CaptionN8               "VUELTO"//ccr2016-08-08"EN CAMBIO"//"找零"
#define CaptionN9               "DIA"//"日"
#define CaptionN10              "SEMANA"// "周"
#define CaptionN11              "MES"// "月"
#define CaptionN12              "ANO"// "年"
#define CaptionN13              "STANDARD"//"标准"
#define CaptionN14              "VENDEDOR"//"收款员"
#define CaptionN15              " HORA "//"   时"
#define CaptionN16              " DIA "// "   日"
#define CaptionN17              " MES "//"   月"
#define CaptionN18              "DEVOLUCION"// "退货"
#define CaptionN19              "COSTE"// "成本"
#define CaptionN20              "MESA NO"//"桌号#"
#define CaptionN21              "CHEQUE NO"//"支票号#"
#define CaptionN22              "    #:    "//   "项数:"
#define CaptionN23              "VENTA+I.G.V"// "含税销售"
#define CaptionN24              "I.G.V"//"税额"
#define CaptionN25              "PAGINA"// "页号"
#define CaptionN26              "NO#:"//"号码:"
#define CaptionN27              "APARCA �"//"挂帐"
#define CaptionN28              "ABRIR MESA"//"开台"
#define CaptionN29              "Z"//ccr2016-08-09"CANT-QUITACUENTA"//"清帐计数"
#define CaptionN30              "BORRAR"// "清除"
#define CaptionN31              "S-RESUMEN"//"S-报表"
#if HOR_REPORT==1
#define CaptionN32              "BRUTO"//"BENEFICIO BRUTO"// "毛利"
#else
#define CaptionN32              "BENEFICIO BRUTO"// "毛利"
#endif
#define CaptionN33              "ALMACON +/-"//"库存增减"
#define CaptionN331             "STOCK DECREASED"  // "库存增减",
#define CaptionN34              "CUENTA-MESA ANTERIOR"// "原桌帐:"
#define CaptionN35              "T.CREDITO"// "信用卡#"
#define CaptionN36              "CUENTA-MESA NUEVA:"// "新桌帐:"
#define CaptionN37              "CLIENTE NO"// "人数"
#define CaptionN38              "CAMBIA ARTICULO"// "换货"


#define CaptionN39              "ANADIR?"//"增加?"
#define CaptionN40              "BORRAR?"// "删除?"
#define CaptionN41              "DESCUENT1"//"折扣项 1"
#define CaptionN42              "DESCUENT2"//"折扣项 2"
#define CaptionN43              "VENTA �"//  "销售额"
#define CaptionN44              "DTO MASIVO"//  "大包优惠"
#define CaptionN45              "STOCK CANT"//"库存数量"
#define CaptionN46              "OFERTA"//"特价优惠"
#define CaptionN47              "OPCION:"//    "选项:"
#define CaptionN48              "FIJO"// "固定"
#define CaptionN49              "FLEXI."//"浮动"
#define CaptionN50              "FIJO/FLEX."// "固/浮"

#define CaptionN51              "MESA TEMPORALFIN "//"开台暂结"
#define CaptionN52              " +MESA"//"开台增加"
#define CaptionN53              "CERRA TEMPORAL"// "暂结"
#define CaptionN54              "CONFIRMAPAGO"//"确认结帐"
#define CaptionN55              "IMPRIME MESA"//"打印开台"
#define CaptionN56              "IMPRIMCUENTA"// "打印帐单"
#define CaptionN57              "CANCELA CONFIRMA"//"取消确认"
#define CaptionN58              "DIVIDECUENTA"//"分帐"
#define CaptionN59              "TRANS-CUENTA"// "转帐"
#define CaptionN60              "ALTERAR OPER"//"更换桌台"
#define CaptionN61              "CLIENTE NO"// "顾客人数"

#define CaptionN62              "CONFIG.PREMIO:"// "中奖设定:"
#define CaptionN63              "PUNTOS:"//"消费计点:"

#define CaptionN64              "ALTERA PVP"// "改变单品价格"
#define CaptionN65              "COCINA#"//"厨房打印机组"
#define CaptionN66              " +STOCK"//  "增加库存"
#define CaptionN67              "-STOCK"//  "减少库存"
#define CaptionN68              "TECLAR"// "输入"
#define CaptionN69              "TOTAL"//ccr2016-08-08"DEBIDO"// "待付"


#define CaptionE1               "ERROR RS232"//  "RS232错"
#define CaptionE2               "CORRECTO RS232"//"RS232正确"
#define CaptionE3               "RTC ERROR!"//"RTC 错!"
#define CaptionE4               "CTC ERROR!"//  "CTC 错!"
#define CaptionE5               "FPROM ERROR!"//"FPROM错!"
#define CaptionE6               "INICIAR..."// "初始化..."
#define CaptionE7               "YA INICIADO."// "初始化结束."
#define CaptionE8               "ERROR-"//"出错-"

#define CaptionE9               "CLAVE ERROR!"// "密码错 ！"
#define CaptionE10              "GRABADO CLAVE"//"密码已保存"
#define CaptionE11              "CAJERO:"//"收款员:"
#define CaptionE12              "VENDEDOR:"// "营业员:"
#define CaptionE13              "CAJA NO:"//"收款机号:"
#define CaptionE14              "LOCAL:"// "店铺:"
#define CaptionE15              "CONFIRMAR?"//"确认?"
#define CaptionE16              "TERMINADO"//"完成"

#define CaptionE17              "VERSION ACTUAL:"//"发行版本:"
#define CaptionE18              "PUNTOS ACTUAL"//"本次消费点"
#define CaptionE19              "ARTICULO NUEVO:"//"新增单品:"
#define CaptionE20              "BORRA ARTICULO:"// "删除单品:"
#define CaptionE21              "COMENZAR TEST>>" //"开始检测>>"
#define CaptionE22              "<<YA TEST"//  "<<检测结束"
#define CaptionE23              "STOCK"//"库存"
#define CaptionE24              "ARREGLADO RESUMEN!"// "报表已全清!"
#define CaptionE25              "NO APAQUE EN ACTUALIZAR"//"更新程序切勿断电"
#define CaptionE26              "FILE OVERFLOW!"// "文件空间越界"
#define CaptionE27              "DESBORDADO MEMORIA！"// "内存溢出！"
#define CaptionE28              "MEMORIA LLENA"//  "流水帐存储满"
#define CaptionE29              "CAMBIA ROLLO"//"请更换打印纸"
#define CaptionE30              "MEMORIA EXTERNAL ERR！"// "扩展内存出错！"

#define CaptionE31              "NOTA:"//"说明文字:"

//----------Length<=10------------------
#define CaptionE32              "INVALIDO!"//"无效输入!"
#define CaptionE33              "FECHA INVALIDO!"//"无效日期!"
#define CaptionE34              "HORA INVALIDO"//"无效时间!"
#define CaptionE35              "NO TECLAR!"//"禁止输入!"
#define CaptionE36              "PLU ES VACIO!"// "PLU代码为空!"
#define CaptionE37              "PLU DOC LLENO!"//   "PLU文件溢出!"
#define CaptionE38              "MESA OCUPADA!"// "桌台被占用!"
#define CaptionE39              "MESA ABIERTA!"// "桌台已打开!"
#define CaptionE40              "MESA NO ERROR!"//"桌台号不对!"
#define CaptionE41              "TELCA MESA# "// "请输入桌台号"
#define CaptionE42              "MESA LIBRE"// "没有开台"
#define CaptionE43              "CUENTA-MESA LLENA"//"桌台桌帐溢出"
#define CaptionE44              "NO MODIF.HORA"// "禁止修改时间"
#define CaptionE45              "EN VENTA!"//"仍在销售中!"
#define CaptionE46              "RAM-VENTA LLENA"//"销售缓冲区满"
#define CaptionE47              "STOCK ARTICULO"//"商品没有售出"
#define CaptionE48              "DURANTE COBRO!"// "正在结算中!"
#define CaptionE49              "DATOS LARGO"//"输入数据超长"
#define CaptionE50              "DURANTE PAGO!"//"正在付帐!"
#define CaptionE51              "CLIENTE LLENO!"//  "人数溢出!"
#define CaptionE52              "FALTA PAGO"//  "没有确认结帐"
#define CaptionE53              "NO CANCELE PAGO"// "禁止取消付款"
#define CaptionE54              "SIN ESTE COMIDA!"// "无菜名!"
#define CaptionE55              "SIN ESTE C!"//"无此收款员!"
#define CaptionE56              "CAMBIADO C"// "收款员改变"
#define CaptionE57              "SIN ESTE DOC!"//"无此类报表!"
#define CaptionE58              "FALLO PRINT DOC"// "报表打印中断"
#define CaptionE59              "OBL.GERENTE"//"必须在经理档"
#define CaptionE60              "NO REPARTICION-CUENTA"//  "不能分帐时用"
#define CaptionE61              "OBL.NO#!"// "必须输入号码!"
#define CaptionE62              "KEY POS ERROR"//"钥匙档位错"
#define CaptionE63              "SIN AUTORIZACION!"// "未授权!"
#define CaptionE64              "RESIGNA C"//"请指定收款员"
#define CaptionE65              "RESIGNA VEND"//"请指定营业员"
#define CaptionE66              "NOCAMIBE PLU-PVP！"//"禁止PLU变价！"
#define CaptionE67              "CLAVE ERROR!"// "密码不对!"
#define CaptionE68              "COCINA ERR"//"厨房打印机错"
#define CaptionE69              "PRINT ERROR"//"票据打印机错" //ccr2017-08-31
#define CaptionE70              "LEVANTA EJE"// "打印压杆抬起"
#define CaptionE71              "SIN ROLLO!"//"打印纸用完!"
#define CaptionE72              "TEMPERATURA ALTA"//"打印温度太高"
#define CaptionE73              "SIN DEFINIR BOTON!"//"按键未定义!"
#define CaptionE74              "OBLIGATORIO�"// "必须输入金额"
#define CaptionE75              "NO-FORMA-PAGO"//"禁止多种付款"
#define CaptionE76              "SIN ESTE FUNCION"//"记帐无此功能"
#define CaptionE77              "FALTA PULSES.TOTAL!"//"未按小计键!"
#define CaptionE78              "EN GESTION-STOCK"//"正在管理库存"
#define CaptionE79              "GRABA C/C ERR"//"保存流水帐错"
#define CaptionE80              "COM-MODEM ERR"//"MODEM通讯错"


#define CaptionE81              "MONTA-T ERR!"//"卡操作出错!"
#define CaptionE82              "POS BARCODE ERR!"// "POS代码错!"
#define CaptionE83              "T.DATOS ERR!"//"卡数据错!"
#define CaptionE84              "T.CADUCADA!"//"为过期卡!"
#define CaptionE85              "T.PERDIDA!"// "为挂失卡!"
#define CaptionE86              "CLIENTE TIPO ERR!"//"客户类别错!"
#define CaptionE87              "T.NUEVA!"//   "为新卡!"
#define CaptionE88              "T.INVALIDA!"//"不是购物卡!"
#define CaptionE89              "LEE T.ERR!"//"写卡出错!"
#define CaptionE90              "T.NO ERR!"// "卡号不对!"
#define CaptionE91              "NO T.DTO!"//"禁用折扣卡!"
#define CaptionE92              "NO T.CASH!"//"禁用现金卡!"
#define CaptionE93              "NO T.CREDITO!"//"禁用赊帐卡!"
#define CaptionE94              "NO USE IC!"//"禁用IC卡!"
#define CaptionE95              "QUITA T.ERR!"// "清卡出错!"
#define CaptionE96              "DATOS LLENOS!"//"数据溢出!"
#define CaptionE97              "RECARGA IC ERR!"//"IC充值出错!"
#define CaptionE98              "INICIA IC ERR"//"IC初始化出错"
#define CaptionE99              "NO INICIE IC!"//  "禁止初始化!"
#define CaptionE100             "FALTA SALDO!"//"IC金额不足!"
#define CaptionE101             "PAGO ERR CONIC!"//"IC付款出错!"
#define CaptionE102             "IP DIRECCION ERR!"// "IP地址错!"
#define CaptionE103             "NO CHIP CARD!"
#define CaptionE105		        "HAY PEDIDO ABIERTO"//"已有挂单"

#define CaptionERR105             "BATERIA BAJA!"  // "电池电量低",
#define CaptionERR106             "NO ABRIR STOCK!"	//20070806"没激活库存"//未开启库存!
//ccr090715>>>>>>>>>>>>>>>>>>>>>>
#define CaptionERR76			  "INSERTA PEN DRIVE!"	//  "请插入U盘!"
#define CaptionERR77			  "ABRE DOC ERR!"		//  "打开文件错!"
#define CaptionERR78			  "LEE PENDRIVE ERR!"		//"读U盘出错!"
#define CaptionERR79			  "ESCRIBE DRIVE ERR!"		// "写U盘出错!"
#define CaptionERR80			  "CREA DOC ERR!"		//  "创建文件错!"
#define CaptionERR81			  "CERRA DOC ERR!"		//  "关闭文件错!"
#define CaptionERR82			  "Send LOG 1st"		//
#define CaptionERR83              "GPRS ERROR!"
#define CaptionERR84              "ERROR-SEND!"
#define CaptionERR85              "ERR-RECEIVE"
#define CaptionERR86              "No SERVER!"
#define CaptionERR87              "IP/PORT:NULL"
#define CaptionERR88              "NO SIM CARD"
#define CaptionERR89              "GPRS NOT OK"
#define CaptionERR90              "OUT OF RANGE" //CWXXI90  //
#define CaptionERR91              "NULL ACCOUNT"  //CWXXI91
//ccr2016-10-24 EJ >>>>>>>>>>>>>>>>>>>
#define EJ_CWXXI92  "ERROR DATOS EJ"   	//CWXXI92
#define EJ_CWXXI93  "NO SD TARJETA"        	//CWXXI93
#define EJ_CWXXI94  "ERROR EJ E/W"   	//CWXXI94
#define EJ_CWXXI95  "SIN REGISTR EJ"   	//CWXXI95
#define EJ_CWXXI96  "EJ CASI LLENO"   	//CWXXI96
#define EJ_CWXXI97  "EJ est? lleno"   	//CWXXI97
#define EJ_CWXXI98  "NUEVO EJ"        	//CWXXI98
#define EJ_CWXXI99  "REINICIAR EJ"   	//CWXXI99
#define EJ_CWXXI100 "EJ NO COINCIDE"     	//CWXXI100

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#define CaptionE104             "ABRE CAJA"//   "打开钱箱"
#define CaptionE106             "HAY PEDIDO ABIERTO"//"有挂单"
#define CaptionE107             "CANT.ARTI.PVP"//   "  输入商品数量\n  和商品价格"
#define CaptionE108             "PASA A"    //ZHUANRUDAO

#define MsgPUTOUT               "PO"   	                //PUTOUT
#define MsgRECIEVIN             "RA" 		            //RECIEVIN
#define DownLoad1               ">> LETRA"//  "正在下载字库"
#define DownLoad2               ">> FOTO"//   "正在下载图形"

#define OptionN1                " SI"// "是"
#define OptionN2                " NO"//"否"
#define OptionN3                "NOMBRE:"//"名称:"
#define OptionN4                "DEPT:"//   "部类:"
#define OptionN5                "GRUPO:"//  "柜组:"
#define OptionN6                "IMPRIMIR:"// "打印:"
#define OptionN7                "OPCION:"// "选项:"
#define OptionN8                "CONEXION:"//"链接:"
#define OptionN9                "IMPUESTO%:"// "税率:"
#define OptionN10               "LOCACION NO:"//  "位置编号:"

#define OptionN11               "PRECIO 1:"//"价格1:"
#define OptionN12               "PRECIO 2:"// "价格2:"
#define OptionN13               "PRECIO 3:"// "价格3:"
#define OptionN14               "PRECIO 4:"//  "价格4:"

#define OptionN15               "COSTE:"// "进价:"
#define OptionN16               "DESCUENTO(%):"// "折扣率(%):"
#define OptionN17               "MAXIMO:"// "最大值:"
#define OptionN18               "DESCUENTO:"//"折扣率:"
#define OptionN19               "MAXIMO:"//"最大值:"
#define OptionN20               "IMPUESTO%:"// "税率:"
#define OptionN21               "COSTE:"//"买入价:"
#define OptionN22               "PRECIO:"//"卖出价:"
#define OptionN23               "ESTADO:"//"状态:"
#define OptionN24               "CAJA:"// "钱箱:"
#define MSGOTD                  "OTD"                    //OTDMSG
#define OptionN25               "IMPRIME COCINA:"// "厨房打印:"
#define MSGPERIOD               "PERIOD"                      //PERMSG
#define MSGPOINTER              "POINTER"                     //POIMSG

#define OptionN26               "PERIODO:"  // "周期:",
#define OptionN27               "CONEXION:"  // "连接:",
#define OptionN28               "FUNCION "  // "输入功能码:",
#define OptionN29               "PULSE TO:"  // "按功能键:",
#define OptionN30               "TIPO:"  // "类型:",
#define OptionN31               "INICIO:"  // "开始日期:",
#define OptionN32               "FECHA-FIN:"  // "结束日期:",
#define OptionN33               "HORA-INICIO:"  // "开始时间:",
#define OptionN34               "HORA-FIN:"  // "结束时间:",
#define OptionN35               "SEMANA:"  // "星期:",
#define OptionN36               "DESCUENTO :"  // "折扣:",
#define OptionN37               "MASIVA:"  // "大包数量:",
#define OptionN38               "PVP:"  // "单价:",
#define OptionN39               "OFERTA:"  // "优惠价:",
#define OptionN40               "PROTOCOL:"  // "通讯协议:",
#define OptionN41               "TELIFONO NO#:"  // "电话号码:",
#define OptionN42               "CONTRASENA:"  // "密码:",
#define OptionN43               "FRECUENCIA:"  // "频率:",
#define OptionN44               "MIN.�:"  // "最小金额:",
#define OptionN45               "INTERFAZ :"  // "通讯口:",
#define OptionN46               "PUNTOS �:"  // "计点金额:",
#define OptionN47               "FOTO NO:"  // "图象序号:",
#define OptionN48               "HORA"  // "时间:",
#define OptionN49               "FECHA"  // "日期:",
#define OptionN50               "PRINTER"
#define OptionN51               "TYPE:"      //DZCHXHAO//"秤型号",

//#define WelcomeN1                "创造优秀优创造就\n优创MACRO收款机!"
#define WelcomeN1                "CAJA REGISTRADORA!"//"优创MACRO收款机!"
#define WelcomeN2                " DO THE BEST! \nMade by EUTRON"

#define MessagesN1                "CAJA BLOQUEADA"  // "  收款机已锁定",
#define MessagesN2                "ENCIENDE MODO"  // "按【方式锁】开机",
#define MessagesN3                "FACUTRA             "  // "形式发票",	//XSHFPIAO
#define MessagesN4                "COCINA#"  // "厨房打印机#",	//CHFDYJHAO
#define MessagesN5                "*** IMPRESORA DE COCINA ***"  // "*** 培训状态 ***",
#define MessagesN6                "***** CARGAR BATERIA *****"  // "***** 加电 *****",
#define MessagesN7                "***PAGAR EN PARTE ***"  // "*** 分开付帐 ***",
#define MessagesN8                "*** COPIAR DOCUMENTO ***"  // "*** 复制收据 ***",
#define MessagesN9                "*** ALTERAR PRECIO***"  // "*** 商品变价 ***",
#define MessagesN10               "*** MOVIMIENTO DE STOCK ***"  // "*** 增减库存 ***",
#define MessagesN101              "**INVENTORY- **"  // "*** 减少库存 ***",
#define MessagesN11               "***** CANCELAR*****"  // "***** 取消 *****",
#define MessagesN12               "**  INICIAR PROGRAMA **"  // "** 程序初始化 **",
#define MessagesN13               "<>"
#define MessagesN14               "TOTAL"  ////KPTOTAL
//                                 01234567890123456789012345678901
#define TitleN1	                  "NOMBRE  CANTIDAD.   PVP. IMPORTE"  //--品名   数量   单价   金额
//                                 01234567890123456789012345678901345
#define TitleN3	                  "NOMBRE      CANTIDAD.  IMPORTE"  //--品名   数量   单价   金额
#define TitleN4	                  "NAME                      AMOUNT "  //

#define CaptionM1                 "ENERO"  // "一月",
#define CaptionM2                 "FEBRERO"  // "二月",
#define CaptionM3                 "MARZO"  //  "三月",
#define CaptionM4                 "ABRIL"  //  "四月",
#define CaptionM5                 "MAYO"  // "五月",
#define CaptionM6                 "JUNIO"  // "六月",
#define CaptionM7                 "JULIO"  // "七月",
#define CaptionM8                 "AGOSTO"  // "八月",
#define CaptionM9                 "SEPTIEMBRE"  // "九月",
#define CaptionM10                "OCTUBRE"  // "十月",
#define CaptionM11                "NOVIEMBRE"  // "十一月",
#define CaptionM12                "DICIEMBRE"  // "十二月",

#define CaptionW1                 "LUNES "  // "星期一",			//LUNES
#define CaptionW2                 "MARTES"  // "星期二",			//MARTES
#define CaptionW3                 "MI."  // "星期三",			//MIERCOLES
#define CaptionW4                 "JUEVES"  // "星期四",			//JUEVES
#define CaptionW5                 "VIERNES"  // "星期五",			//VIERNES
#define CaptionW6                 "SABADO"  // "星期六",			//SABADO
#define CaptionW7                 "DOMINGO"  // "星期日",			//DOMINGO

#define ClueN1                    "TECLAR CONFIRMA"  // "按确认键...",

  //Option for KP
#define OptionS1                  "PRINT SUM:"  // "打印总额:",
#define OptionS2                  "I.M.PLATO "  // "菜相同打印",
#define OptionS3                  "I.D.PLATO "  // "菜不同打印",
  //Option for dept
#define OptionS4                  "PVP 0"  // "禁止零价格",
#define OptionS5                  "PRINT.SEP."  // "打印分隔线",
#define OptionS6                  "ITEM VENTA:"  // "单项销售:",
#define OptionS7                  "DTO ITEM 1:"  // "折扣项 1:",
#define OptionS8                  "DTO ITEM 2:"  // "折扣项 2:",
  //Options for tend
#define OptionS9                  "ABRE CAJA:"  // "打开钱箱:",
#define OptionS10                 "TECLA VAL:"  // "输入金额:",
#define OptionS11                 "NO PULSE:"  // "禁止输入:",
#define OptionS12                 "TECLA NO#:"  // "输入号码:",
#define OptionS13                 "NO CAMBIO"  // "找零为小费",
  //Options for PoRa
#define OptionS14                 "ABRE CAJA:"  // "打开钱箱:",
#define OptionS15                 "PAGO FORMA"  // "付款方式键",
#define OptionS16                 "INGRESO:"  // "存入金额:",
#define OptionS17                 "ENTRADA/SALIDA"  // "入金/出金",
  //Option for Disc
#define OptionS18                 "PRINT.DTO."  // "打印折扣额",
#define OptionS19                 "APLICA DTO1:"  // "使用折扣1:",
#define OptionS20                 "APLICA DTO2:"  // "使用折扣2:",
  //Options for currency
#define OptionS21                 "CAMBIO"  // "卖出价找零",
  //Options for Draw
#define OptionS22                 "SIN CENTIMO:"  // "无小数点:",
#define OptionS23                 "ABRE CAJA:"  // "打开钱箱:",
  //Options for Tax
#define OptionS24                 "+ VENTA:"  // "营业/增值:",
#define OptionS25                 "PRINT.TASA"  // "打印税率项",
#define OptionS26                 "ES GST "  // "打印0税项",
#define OptionS27                 "IMPUESTO-GST"  // "为GST类税",
#define OptionS28                 "PRINT.IGV:"  // "打印税额:",
  //Options for clerk
#define OptionS29                 "NO LIMITE:"  // "取消限制:",
#define OptionS30                 "ALTERA LMT:"  // "更改限制:",
#define OptionS31                 "NO PEDIDO:"  // "取消交易:",
#define OptionS32                 "LIMITE DEV.:"  // "退货限制:",
#define OptionS33                 "LIMITE-DTO:"  // "%折扣限制:",
#define OptionS34                 "LMT.COMIS.:"  // "%加成限制:",
#define OptionS35                 "LIMITE-�"  // "+-金额限制",
#define OptionS36                 "FORMACION:"  // "培训模式:",
#define OptionS37                 "INDEX:"    //mPRNINDEX//Ccr "取菜号:",

#define MESS_ECR_IP         "ECR IP ADDRESS:"
#define MESS_ECR_ID         "ECR ID(0-9,A-F):"

#define EditN1                    "<BARCODE>TECLAR GRABAR\n<CONFIRMA>  ACTUAL GRABAR\n"  //ccr "<记录号>  新纪录\n<确认>  当前纪录",
#define EditN2                    "<BARCODE> +/- ARTICULO.\n<CONFIRMA> ARTI.\n"  //ccr "<记录号>增删商品\n<确认>  当前商品\n",
#define EditN3                    "<SALIR>   SALIR"  // "<退出>      退出",
#define EditN4                    "<CONFIRMA>  CONTINUAR\n"  // "<确认>      继续\n",
#define EditN5                    "ELIGE DOC"  // "按<选择>报表",
#define EditN6                    "� COSUMO"  // "消费金额",
#define EditN7                    "TECLADO  ->TECLADO    "  //"键码     ->键   ",
#define EditN8                    "SOLO GTE."  // "只能在经理档用",
#define EditN9                    "PARA VENTA"  // "可在营业档用",
#define EditN10                   "IMPRIME LETRAS"  // "打印文字",
#define EditN11                   "(RESERVAR)"  // "保留",
#define EditN12                   "TEST DOC CLAVE"  // "文件密码测试",
#define EditN13                   "DISCAR NUMERO"  // "拨号",
#define EditN14                   "CONEXON MODEM!"  // "MODEM 连接!",
#define EditN15                   "CODE:"  // "代码:",
#define EditN16                   "DESCENTO%"  // "%折扣",
#define EditN17                   "COMIS.%"  // "%加成",
#define EditN18                   "DOC NO#.:"  // "收据号:",
#define EditN19                   "DOC CANTIDAD "  // "文件数："，0
#define EditN20                   "DOC ESPACIO  "  // "文件空间："?
#define EditN21                   "CALCULA DOC"  // "统计文件"2
#define EditN22                   "CALCULAR IMPUESTO"  // “计税”8
#define EditN23                   "MOVIMIENTO CUENTAE SPACIO"  // "流水空间"
#define EditN24                   "DOC NOMBRE CANTIDAD ESPACIO"  // "文件名      记录数      空间大小"
#define EditN25                   "NOMBRE SERIE TIPO COMUNICACION"  // "串口名称    通信类别   通信协议"
#define EditN26                   "SUPERIOR L.:"  // '顶部空行数',
#define EditN27                   "LIN/PAG:"  // '每页行数',
#define EditN28                   "PRINT INFO"  // '打印信息',
#define EditN29                   "EL SEGUINDA DOC:"  // '第二张票据',
#define EditN30                   "ZONA IZQA:"  // '左边距',
#define EditN31                   "TICKET TIPO   :"  // '票打机型',
#define EditN32                   "IMPRESORA OFF"  // "打印机关",
#define EditN33                   "IMPRESORA ON"  // "打印机开",

  //ChipCard>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN1                "CONTRASENA:"  //I "密码:",
#define ChipCardN2                "CONFIRMA CLAVE:"  // "密码确认:",
#define ChipCardN3                "POS CODE:"  // "POS代码:",
#define ChipCardN4                "CONFIRMA CODE:"  // "代码确认:",
#define ChipCardN5                "GRABADO CODE"  // "代码已保存",
#define ChipCardN6                "CODE ERROR ！"  // "代码错 ！",
#define ChipCardN7                "USE IC:"  // "启用IC卡:",
#define ChipCardN8                "T.DTO:"  // "折扣卡:",
#define ChipCardN9                "T.CASH:"  // "现金卡:",
#define ChipCardN10               "T.CREDITO:"  // "赊帐卡:",
#define ChipCardN11               "PRINT IC"  // "打印IC卡",
#define ChipCardN12               "AUTO-DTO :"  // "自动折扣:",
#define ChipCardN13               "PUNTOS:"  // "消费计点:",
#define ChipCardN14               "USA � :"  // "用现金键:",
#define ChipCardN15               "USA PLAZO"  // "启用有效期", //
#define ChipCardN16               "PERDIDA TARJETA"  // "挂失卡号",
#define ChipCardN17               "T.NO:"  // "卡号:",
#define ChipCardN18               "T.TIPO:"  // "卡类型:",
#define ChipCardN19               "SALDO:"  // "余额"
#define ChipCardN20               "T.FIANZA:"  // "卡押金:",
#define ChipCardN21               "CONSUMO:"  // "消费:",
#define ChipCardN22               "TOTAL RECARGA:"  // "充值总额:",
#define ChipCardN23               "CANT DE UTILIZA:"  // "使用次数:",
#define ChipCardN24               "PVP CATEGORIA:"  // "价格级别:",
#define ChipCardN25               "PIN CODE:"  // "PIN码:",
#define ChipCardN26               "PROTECCION NIVEL:"  // "保护级别:",
#define ChipCardN27               "AUTO-PAGO:"  // "自动结帐:",
#define ChipCardN28               "HORA/CREA.T:"  // "制卡日期:",
#define ChipCardN29               "T.PLAZO:"  // "卡有效期:",
#define ChipCardN30               "CLIENTE:"  // "客户:",
#define ChipCardN31               "INICIAR IC"  // "初始化IC",
#define ChipCardN32               "RECARGA"  // "IC卡充值",
#define ChipCardN33               "BORRAR IC "  // "清除IC卡",
#define ChipCardN34               "+/- �?"  // "充值/退款?",
#define ChipCardN35               " -/+ PUNTOS?"  // "减点/送点?",
#define ChipCardN36               "CONFIRMA+ �?"  // "确认充值?",
#define ChipCardN37               "CONFIRMA- �?"  // "确认退款?",
#define ChipCardN38               "CONFIRMA + PUNTOS?"  // "确认送点?",
#define ChipCardN39               "PAGA PREMIO?"  // "兑奖确认?",
#define ChipCardN40               "PREMIO POR PUNTO"  // "计点兑奖",
#define ChipCardN41               "DEVOLVE�IC"  // "IC卡退款",
#define ChipCardN42               "PUNTOS-PREMIO IC"  // "IC卡兑奖点数",
#define ChipCardN43               "PUNTO-REGALO IC"  // "IC卡送点数",
#define ChipCardN44               "ESPERA POR FAVOR......"  // "请等待......",
#define ChipCardN45               "HAY SALDO, DEVULVE � PRIMERO...."  // "卡内有金额,请先退款....",
#define ChipCardN46               "SOCIO NO"  // "会员号:",
#define ChipCardN47               "SOCIO CONSUMO"  // "会 员 消 费"
#define ChipCardN48               "PARA  NUEVO IP, ENCIENDE"  // "要启用新的IP,请重新开机.",
#define ChipCardN49               "QUITA DOC?"  //  "确认清报表?",
#define ChipCardN50               "ESPERA IC...."  // "等待IC卡....",
#define ChipCardN51               "DBLHIGH PRINT"  // "倍高打印",
//ccr2015-06-15>>>>>>>>>>>>>>>>>>>>>
#define ChipCardN52             "CLEAR"
#define ChipCardN53             "REFUND"
#define ChipCardN54             "PAYMEND"
#define ChipCardN55             "CHARGE"
#define ChipCardN56             "INITIAL"
#define ChipCardN57             "TAKE CASH"
#define ChipCardN58            "ADD POINTS"   //ADDVIPPOINTS
#define ChipCardN59            "DEC POINTS"   //DECVIPPOINTS
#define ChipCardN60            "SUM POINTS"  //VIPPOINTS
//ccr2015-06-15<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MESSEJ_X_STORE      "SALVAR EJ X"//MsgEJ_X_STORE
#define MESSEJ_Z_STORE      "SALVAR EJ Z"//MsgEJ_Z_STORE
#define MESSEJ_TR_STORE     "SALVAR EJ TR"//MsgEJ_TR_STORE
#endif
//ccr2016-02-22>>>>>>>>>>>>>>>>>>>>>
#define MsgMENUXREPORT     "X-REPORT"
#define MsgMENUXEXPORT     "EXPORT(X)"
#define MsgMENUQUICKVIEW   "GRAND TOTAL"
#define MsgMENUXLOGFUNC    "RECEIPT LOG"
#define MsgMENUSENDNEWLOG  "SEND NEW LOG"      //MENUSENDNEWLOG  //发送新流水数据
#define MsgMENUXPASSWORD   "X-PASSWORD"
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUXEJOURNAL    "EJ"     //MENUXEJOURNAL   "E Journal"
#endif
//.................................
#define MsgMENUZREPORT     "Z-REPORT"
#define MsgMENUZEXPORT     "EXPORT(Z)"
#define MsgMENUCLEARLOG    "CLEAR LOG"
#define MsgMENUZGRAND      "CLEAR TOTAL"
#define MsgMENUZPASSWORD   "Z-PASSWORD"
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgMENUZRESETEJ   "REINICIAR EJ"     //MENUZRESETEJ   "Clear EJ"
#endif
//...................................


#define EPosN1                    "EPOS CLAVE:"  // "ePOS认证码:",
#define EPosN2                    "WAIT ACCOUNTS"  // "等待易通卡"
#define EPosN3                    "ACCOUNTS::"  // "易通卡号:",
#define EPosN4                    "EPOS BALANCE:"  // "易通卡余额:",


#define ErrInfoN1                 "DATOS LARGO"  // "通讯数据过长"
#define ErrInfoN2                 " ***   FALLO PRINT, OTRAVEZ *** "  // " *** 打印被中断、重新打印 *** "
#define ErrInfoN3                 "Check error!"  // "数据校验错"

#define PLULIST                  " INFO- ARTICULO"  // "         单 品 信 息 表"
                                 //1234567890123456789012345678901234567890
#define PLUTITLE1                "NOBRE-ARTICULO          REF"// "单品名称                    编码"
#define PLUTITLE2                "DEPT EN STOCK PVP "

#define DEPTLIST                  "INFO-DEPT  "  // "         部 类 信 息 表"

//                                "INFO- ARTICULO"  // "         单 品 信 息 表"
#define DEPTTITLE                 "NO-DEPT  GRUPO  PVP"// 部类名称 归属柜组  价格

#define FisTextN1                  "CONTRIBUYENTE NO#:"  //"纳税人号:"
#define FisTextN2                  "CREA IMPUESTODOC "  //"开出税票      "
#define FisTextN3                  "LICENCIA NO#"  //"授权号码        "
#define FisTextN4                  "ESCRIBIR MAQUINA NO#"  //"写机器号        "
#define FisTextN5                  "CONFIRMA ESCRITO"  //"写入确认        "
#define FisTextN6                  "IMPRIMIR DATOS  "  //"数据印出        "
#define FisTextN7                  "USUARIO NO#"  //"用户号码        "
#define FisTextN8                  "SIN IMPUESTO"  //"免税状态        "
#define FisTextN9                  "Z-NUMERAR:        "  //"Z-计数:         "
#define FisTextN10                 "CANT VENTA "  //"累计销售        "
#define FisTextN11                 "TOTAL IMPUESTO"  //"累计税          "
#define FisTextN12                 "SUM SIN-I.G.V"  //"总免税额        "
#define FisTextN13                 "VENTA TOTAL  "  //"总销售额        "
#define FisTextN14                 "TOTAL I.G.V"  //"总税额          "
#define FisTextN15                 "CANT IMPUESTO/DIA"  //"日税票数        "
#define FisTextN16                 "CANT DOC"  //"收据总数        "
#define FisTextN17                 "NUMERO EXPORT:      "  //"余报表数 :      "
#define FisTextN18                 "IMPRIME IMPUESTO NO"  //"    _计 _税 _印 _出     "
#define FisTextN19                 "TERMINADO PROBAR MUESTRA     "  //"    _试 _装 _结 _束     "
#define FisTextN20                 "   _NOT   _FISCAL     "  //"    _非   _计   _税     "


#define PRTStyleN1                 "NORMAL IMPRIME"  //Normale
#define PRTStyleN2                 " DblHigh"  //Doppia altezza//      "倍高 DblHigh"
#define PRTStyleN3                 "~D~b~l~W~t~h"  //Raddoppio caratteri//"~倍~宽~D~b~l~W~t~h"
#define PRTStyleN4                 "D~b~l~H~i~g~h"  //Dopia altezza + Raddoppio caratteri//  "~倍~高~D~b~l~H~i~g~h"


#define ComTestN1                  "SERIE 1 X"  //"串口 1 测试出错"
#define ComTestN2                  "SERIE 1 v"  //"串口 1 测试正确"
#define ComTestN3                  "SERIE 2 X"  //"串口 2 测试出错"
#define ComTestN4                  "SERIE 2 v"  //"串口 2 测试正确"

#define ComTestN3ERR                "SERIE 3 X"  //"串口 3 测试出错"
#define ComTestN3OK                "SERIE 4 v"  //"串口 3 测试正确"

#define CurrentTimeN1              "HORA ACTUAL:"  // "当前时间"


#define InputMode1				"A:" // "A："
#define InputMode2				"ZONA:" // "区："
#define InputMode3				"ESCRIBIR:" // "拼："
#define InputMode4				"WUBI:" // "笔："

#define SpecialSymbol1			"->"			//DOWNLOAD//Ccr "★",
#define SpecialSymbol2			"<-"			//UPLOAD//Ccr "☆",
#define SpecialSymbol3			"^   v"		// "↑  ↓",
#define DEPTREPORTTITLE         "DEPT      CANTIDAD      IMPORTE"
                            //  "部类名称     交易数量     金额"
#define PLUREPORTTITLE          "NOMBRE      CANTIDAD   IMPORTE"// "商品名称     交易数量       金额"
#define MESS_PRESSKEY   "PRESS ANY KEY..."

#if defined(CASE_QRCODE)
#define InputQRCode           "QRCODE"  //MsgQRCODE
#endif

#define GPRSServerIP			"SERVER IP"//"服务器IP"
#define GPRSServerPort		    "PORT"//"端口号"

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

#define GPRSstr1 "Data error!"
#define GPRSstr2 "GPRS OK"
#define GPRSstr3 "GPRS NOT OK"
#define GPRSstr4 "CONFIRM?"
#define GPRSstr5 "IP/PORT:NULL"
#define GPRSstr6 "ERROR:IP/PORT"
#define GPRSstr7 "Waiting GPRS" // "收货"
#define GPRSstr8 "SI"   // "是"
#define GPRSstr9 "NO"    // "fou "
#define GPRSstr10 "ENVIANDO DATOS.."  // "数据发送中.. "
#define GPRSstr11 "ENVIADO DATOS.."  // "数据已发出.. "
#define GPRSstr28 "ENVIAR CON EXITO.."						// "发送成功.."
#define GPRSstr31 "CONECTAR ERR"	//						// "连接失败"
#define GPRSstr32 "ACCEPTA FALLO"                                           // "接受失败"
#define GPRSstr33 "CONFIRMA ERR"			// "发送确认失败"
#define GPRSstr34 "ENVIA ERR"							// "发送失败"
#define GPRSstr58 "ESTABLECIENDO CONEXION.."		// "正在建立连接.."
#define GPRSstr59 "PREPARA A CONEXION.."		// " 准备建立连接.."
#define GPRSstr60 "RESTAURA MODULO.."	// "正在复位模块.."
#define GPRSstr61 "YA,SALE PORFAVOR.." // "发送完毕,请退出"
#define GPRSxACK  "WAIT ACK ERROR"
#endif //end defined(CASE_GPRS)
///////////////////////////////////////////////////////////////////
#define InsertUSB		"<<Insert USB"//CHRUSB  插入U盘 //

//以下信息不存入Msg[]中
#define usbMAIN1		"COPY TO USB"			//usbDAOCHU	导出收款机文件 //
#define usbMAIN2		"COPY FROM USB"			//usbDAORU 导入收款机文件 //
#define usbMAIN3		"LOG TO USB"		//usbFLOW 导出销售流水 //


#define usbFLOWNEWMESS	    "EXPORT UPDATE"      //1-导出新数据   //
#define usbFLOWALLMESS	    "EXPORT ALL"      //2-导出全部数据
#define usbFLOWDELMESS	    "EXPORT/RESET"    //3-导出全部数据后清空

#define usbALLFile		"ALL FILES"		//usbALL 全部文件 //
#define MESSPRNMENU     "<00>PRINT MENU"//"<00>打印菜单"
#define MESSSETMENU     " MANUALS LIST"//"         设置功能菜单表"
#define MESSCLEANLOG    "QUITA MEMORIA?  "//"  清除流水帐?"
#define MESSCLEANLOGA   "All of LOG\nwill be cleared!"
#define MESSRAMOUT      "NO MAS ESPACIO"//"  存储空间不足!"
#define MESSCLEANFILES  "QUITA DOC DATOS?"// "  清除文件数据?"
#define MESSCONFIGERR   "CAMBIADO DATOS CONFIGURADO\nDATOS CAUSAN ERR!"//"配置数据已改变,\n保留文件数据可能\n引起销售错误!"
#define MESSDELPLU      "SISTEMA CAMBIADO \n      BORRADO ARTICULO!"// "系统变量有变化,\n     单品被删除!"
#define MESSPLUPRINTEND "PLU Printed O.K.\nPress <ENTER>!"

#define REPRN_PWON      "IMPRIME SIN LUZ"//"掉电继续打印........."
#define REPRN_PAPERON   "IMPRIME SIN ROLLO"//"缺纸继续打印........."
#define RECALLFLOW      "RESTAURA MEMORIA"//"恢复销售流水帐"

/////////////////////////////////////////////////////////////

#define InsertUSB		"<<INSERTA PEN DRIVE>>" //"<<插入U盘>>"
//ccr090715>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define usbMAIN1		"EXPORTA DOC" //"导出文件"
#define usbMAIN2		"IMPORTA DOC" //"导入文件"
#define usbMAIN3		"EXP ORIGINAL DOC" //"导出电子存根"

#define usbFLOWNEWMESS	"EXP DATOS NUEVOS"//"导出新数据"
#define usbFLOWALLMESS	"EXP TODOS DATOS"//"导出全部数据"
#define usbFLOWDELMESS	"EXP TODO+QUITA"//"导出全部+清空"

#define usbALLFile		"TODO DOC" //"全部文件"
#define RECALLFLOW      "Recall LOG"

#define USBREPNAME      "PROYECTO NOBRE"// "项目名称"

////////  CSV导出文件标题  /////////////////////////////////////////////
#define DEPTCAP       "REFERENCIA,NOMBRE,GRUPO,OPCION,VENTA\r\n"//  "编码,名称,归属柜组,对应选项,销售选项\r\n"
#define PLUCAP        "REFERENCIA,NOMBRE,GRUPO,PRECIO 1,PRECIO 2,PRECIO 3,PRECIO 4, COSTE,COMBINADA VENTA,STOCK\r\n"// "编码,名称,特殊优惠,归属部类,价格1,价格2,价格3,价格4,进价,捆绑销售,库存\r\n"

#define TAXCAP        "NOMBRE,TASA IMPUESTO,OPCION\r\n"//"名称,税率,选项\r\n"
#define HEADERCAP     "LETRA-CABEZA-DOC"//  "票头文字\r\n"
#define DISCCAP       "NOMBRE,PRINT OPCION ,OPCION,FIJO DESCUENTO,TASA IMPUESTO,MAXIMO VALOR\r\n"//  "名称,打印选项,选项,固定折扣,税率,最大值\r\n"

#define SALPERCAP     "NOMBRE, OPCION\r\n"//"名称,选项\r\n"
#define SYSTEMCAP     "SISTEMA CONTROL SENAL\r\n"// "系统控制标志\r\n"
#define COMMCAP       "NOMBRE,TIPO,COMPANERO CONVENIO,TELEFONO NO#\r\n"//"名称,类型,同学协议,电话号码\r\n"
#define GRAPPRNCAP    "FOTO REF.,INICIO,FECHA  FIN\r\n"// "图像编号,开始日期,结束日期\r\n"
#define GROUPCAP      "NOMBRE, MAXIMO VALOR\r\n"// "名称,最大销售金额\r\n"
#define ICCARDCAP     "CONFIG.IC SOCIO TARJETA\r\n"//"IC会员卡设置\r\n"
#define PROMCAP       "CANT PREMIO,MIN.�,PUNTOS,MIN PUNTO,FOTO NO,NOTA\r\n"// "中奖频率,最小金额,计点,最小点数,图像序号,说明文字\r\n"
#define PBFUNCCAP     "NOMBRE,PRINT CONTROL,,OPCION\r\n"// "名称,打印控制,选项\r\n"
#define CLERKCAP      "NOMBRE,OPCION, CONTRASENA\r\n"//"名称,选项,密码\r\n"
#define MODICAP       "SABOR INSTRUCCION \r\n"//   "口味说明\r\n"
#define PBTOTALCAP    "REFERENCIA,NOMBRE"//  "编码,名称\r\n"
#define ZONECAP       "TIEMPO 1,TIEMPO 2,TIEMPO 3,TIEMPO 4,TIEMPO 5,TIEMPO 6,TIEMPO 7,TIEMPO 8,TIEMPO 9,TIEMPO 10,TIEMPO 11,TIEMPO 12\r\n"// "时段1,时段2,时段3,时段4,时段5,时段6,时段7,时段8,时段9,时段10,时段11,时段12\r\n"
#define KEYBOARDCAP   "DEFINE TECLADO\r\n"// "键盘定义\r\n"
#define OFFCAP        "NOMBRE,TIPO,INICIO,FECHA FIN,FROM,TO ,DISCOUNT,NUMITES,PVP,PRICEP\r\n"//  "名称,类型,开始日期,结束日期,开始时间,结束时间,折扣,整包数,单价,整包价给\r\n"
#define PORACAP       "NOMBRE,PRINT CONTROL,OPCION, CAJA, MAXIMO VALOR\r\n"//"名称,打印控制,选项,钱箱,最大金额\r\n"

#define TRAILERCAP    "LETRA-PIE-DOC\r\n"//"票尾文字\r\n"
#define SLIPHEADCAP   "CABEZA-TIC\r\n"// "收据打印机票头\r\n"
#define ICBLOCKCAP    "IC TARJETA  LISTA NEGRA  PARA SOCIO\r\n"// "IC会员卡黑名单\r\n"

#define EXPORTCSV    "1-CSV,2-BIN:"//"1-CSV,2-BIN:"

#define CSVFLOETITL "PROYECTO NOMBRE, CANTIDAD, IMPORTE,ALBARIN NO,CAJERO,VENDEDOR, FECHA, CAJA, LOCAL,DOC TIP .\r\n"//"项目名称,数量,金额,收据号,收款员,营业员,日期,收款机,店铺,数据类型\r\n"
#define CSVREPTITL  "FECHA,CAJA,LOCAL\r\n"//",日期,收款机,店铺\r\n"

//new messages>>>>>>>>>>>>>>>>>
#define BUTTONBATLOW "BAJA BATERIA!"
#define CHARGEBATNOW "CARGA BATERIA!"

#define SRAMNOTFOUND "SRAM not found!\nSystem HALT!"

#define INPUTPLUCODE "PLU CODE:"

#define MessageE18   "!!!! FAT(PLU) ERROR !!!!"

#define MESS_TABLEHOLD  "Table    Suspend"

#define MESS_LOGREPET   "Re-Send all LOG!"

#define MESS_PAYALSO   "Confirm Payment?"
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
//...............................................
#define MGADDINV        "INVENTORY+"  //1+SUBTOTAL
#define MGDECINV        "INVENTORY-"  //2+SUBTOTAL
#define MGSETPRICE      "PRICE PLU"  //3+SUBTOTAL

#define MGCLEARIC       "Refund IC"
#define MGINITIC        "Init IC"
#define MGCHARGE        "Charge IC"
#define MGADDPOINT      "Add Point"
#define MGPRINTSTOCK   "LIST STOCK"

#define MGPASSWORD      "SET PASSWORD"
#define MGCLOSEPRN      "CLOSE PRINT"
#define MGTRAINING      "TRAINING MODE"
#define MGGOTOMG        "MANAGEMENT"

//..............................................
#define PLUCODE     "PLU CODE"
#define INVQTYMUST  "INPUT QTY/PRICE"
#define ININVENTORY "Press <SUBT> Key\n Exit Inventory!"
//..............................................
#define DEVICENULL		" NULL"
#define DEVICEPORT1     "Port#1"
#define DEVICEPORT2     "Port#2"
#define DEVICEPORT3     "Port#3"
#define DEVICEPORT4     "Port#4"

//................................................
#define CLOSEPRINTER    "Close PRINTER"
#define CLEARGRANDTOTAL "Cls G.T After Z"
#define CLEANALLECRVIP  "CLEAN ACCOUNT?"//"清除会员数据?"
#define ECRVIPTITLE     "ACCOUNT          AMOUNT   POINTS"
#define CREATEDATE      "CREATE DATE"
//ccr2016-06-06>>>>>>>>>>>INDIA>>>>>>>>>>>>>>>>
#define MsgEDITPLU      "EDIT PLU"
#define MsgEDITDEPT     "EDIT DEPART"
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//.......功能码对应的名称.........................................
#define MsgKEYCORR      "CORRECT"
#define MsgKEYCURR      "CURRENCY"
#define MsgKEYDISC      "DISCOUNT"
#define MsgKEYPBFUN     "PB-FUNC"
#define MsgKEYPORA      "PORA"
#define MsgKEYPO_RA      "PO/RA"
#define MsgKEYTEND      "TENDER"
#define MsgKEYCLEAR     "CLEAR"
#define MsgKEYFEED      "FEED"
#define MsgKEYMULT      "X"
#define MsgKEYSUBTOT    "SUBTOTAL"
#define MsgKEYPRICE     "PRICE"
#define MsgKEYPRICEN    "PRICE#"
#define MsgKEYPRICE1    "PRICE1"
#define MsgKEYPRICE2    "PRICE2"
#define MsgKEYPRICE3    "PRICE3"
#define MsgKEYPRICE4    "PRICE4"
#define MsgKEYSHIFT     "SHIFT"
#define MsgKEYDATE      "DATE"
#define MsgKEYWEIGHT    "WEIGHT"
#define MsgKEYCLERKN    "CLERK#"
#define MsgKEYDEPT      "DEPT~"
#define MsgKEYDEPTNo    "DEPT#"
#define MsgKEYPLUNo     "PLU#"
#define MsgKEYDRAW      "OPEN DRAWER"
#define MsgKEYMEMO      "MODIFIER"
#define MsgKEYCLERK     "CLERK*"
#define MsgKEYPLUDIR    "PLU~"
#define MsgKEYLOCK      "MODE KEY"
#define MsgKEYDRAWFUN   "DRAW FUNC*"
#define MsgKEYSALPN     "SALESMAN#"
#define MsgKEYSALP      "SALESMAN"
#define MsgKEYDOT       "'.'"
#define MsgKEYZERO2     "'00'"
#define MsgKEYNUMBER    "0~9"
#define MsgKEYSUSPEND   "HOLD"
#define MsgFUNCLOOK1    "FUNC LOOK1"
#define MsgFUNCLOOK2    "FUNC LOOK2"
#define MsgMODELOCK     "MODE KEY"
#define MsgVIPLOGIN     "ACCOUNT LOGIN"
#define MsgINPUTNUM     "INPUT CODE"
#define MsgCUSTOMER     "CUSTOMER"
#define MsgKEYNULL      "(OTHERS)"

#define MsgRHEADER      "Header*"
#define MsgRTRAILER     "Trail*"
#define MsgKEYBOARD     "Keyboard*"
#define MsgSYSFLAGS     "System*"
#define MsgSETDATE      "Datetime*"

//......................................
#define MsgCONFIRM      "CONFIRM"
#define MsgNET          "NET AMOUNT"  // "净额",
#define VIPLOGOUT       "ACCOUNT LOGOUT"
#define VIPTOTAL        "AMOUNT"
#define VIPTOTALA       "SUM-AMOUNT"
#define DECVIPPOINT     "DEC."//长度必须为4
#define ADDVIPPOINT     "ADD."//长度必须为4
#define INPUTVIPNAME    "NAME:"
#define PRINTKEYBOARD   "PRINT KEYBOARD"
//...........................................
#define MESSALLLOG         "LOG BY REC.NO"
#define MESSDATERANGE     "LOG BY DATE"
#define MESSVIPLOG        "ACCOUNT LOG"
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
#define It_MGITEMS      "MG FUNCTION"
#define It_SELEFUNC     "KEY FUNCTION"
#define It_SELEITEM     "SELECT ITEM"
#define It_SELEKEYTYPE  "K.BOARD TYPE"
#define It_SELETEST     "SELECT ITEM"
#define It_SELELOG      "SELECT LOG"
#define It_SELETEND     "SELECT TYPE"
#define It_SELEDEPT     "SELECT DEPT"
#define It_SELEPLU      "SELECT PLU"
#define It_SELEMODI     "SELECT MODI."
#define It_SELEREPORT   "PRINT REPORT"
#define It_SELEPOINT    "SELECT POINT"// 小数位数"
#define It_SELEEJ       " EJ      "
//.........................................................
#define MESSRAMVAR      "VARIABLE"
//.........................................................
#define PLUNUMBER           "RECORDE COUNT:"
#define PLURANDOMSIZE     	"SIZE OF CODE:"
#define PLUNAMESIZE			"SIZE OF NAME:"
#define PLUPRICESIZE        "SIZE OF PRICE:"
#define PLUPRICELEVEL       "ACCOUNT PRICE:"
#define PLUCOSTSIZE         "SIZE OF COST:"
#define PLUINVENTSIZE       "SIZE OF STOCK:"
#define PLUREPQTYSIZE       "SIZE OF REP-QTY:"
#define PLUREPAMTSIZE       "SIZE OF REP-AMT:"
#define PLUREPDISCSIZE      "SIZE OF REP-DISC:"
#define PLUREPCOSTSIZE      "SIZE OF REP-COST:"
//.........................................................
#define POINTTYPE           "POINTS"
#define POINTTYPE1          "1.000,00"  //两位小数
#define POINTTYPE2          "1,000.00"  //两位小数
#define POINTTYPE3          "   1.000"
#define POINTTYPE4          "   1,000"
#define POINTTYPE5          "1.000,000"  //三位小数
#define POINTTYPE6          "1,000.000"  //三位小数
//#define POINTTYPE7          "1.000,0"  //一位小数
//#define POINTTYPE8          "1,000.0"  //一位小数
//.......................................................
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define MESSEJMESSAGES      "ACERCA DE EJ"    //MsgEJMESSAGES
#define MESSEJALLEJ         "TODO"
#define MESSEJDATERANGE     "EJ POR FECHA"
#define MESSEJRECEIPT       "EJ DE RECIBO"
#define MESSEJXREPORT       "EJ DE X INFORM"
#define MESSEJZREPORT       "EJ DE Z INFORM"
#define MESSEJCLERK         "EJ DE CAJERO"
#endif
//.......................................................
#define DATETYPE        "FORMAT "
#define DDMMYYYY        "DDMMYYYY"
#define MMDDYYYY        "MMDDYYYY"
#define YYYYMMDD        "YYYYMMDD"
//.......................................................
#define XUPDATEBIOS     "Can't Update \nfirmware by USB"
//ccr2016-06-22..........................................
#if defined(CASE_QRCODE)
#define MESSWXPAY       "OK WX-PAY"
#endif

#define SENDLOGAUTO     "SEND AUTO:"
#define DAYSFORLOG      "DAYS FOR SEND:"
#define ACKMUSTNEED     "ACK NEEDED:"

#define CSV_TITLE       "NOMBRE,CANTIDAD,IMPORTE\r\n"

//ccr2017-01-10>>>>>>>>>>>>>>>>>>>>>>>>>
#define Msg_NOSTOCK     "NO STOCK FIELD"
#define Msg_DISPSTOCK   "DISPLAY/PRINT?"
//ccr2017-01-10<<<<<<<<<<<<<<<<<<<<<<<<<<

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
#define Msg_RECNUMFR	"#EJ DE"                  //开始收据号	//RECNUMFR
#define Msg_RECNUMTO	"#EJ PARA"                    //结束收据号	//RECNUMTO
#define Msg_EJBBIAO		"~E~J ~I~N~F~O~R~M~E"		//EJ 报表			//EJBBIAO
#define Msg_EJXINXI		"MENSAJES ACTUALES DE EJ"            //EJ信息			//EJXINXI
#define Msg_NEWEJXINXI	"EJ DESPU?S INICIALIZADO"            //EJ信息			//NEWEJXINXI
#define Msg_EJBHAO		"EJ ID"						//EJ编号			//EJBHAO
#define Msg_EJLEFT      "EJ IZQUIERDO"        //剩余空间
#define Msg_SIZEEJ		"EJ TAMA?O"				//SIZEEJ
#define Msg_EJCSHUA		"EJ INICIO"		//EJ初始化		//EJCSHUA
#define Msg_FORMAT      "BORRAR EJ"
#define Msg_FORMATED    "BORRADO"
#define Msg_RECALLEJ    "RECUERDA EJ"
#define Msg_EJNOTEMPTY  "EJ NO VACIO"

#define Msg_PRNEJURNAL  ">>>IMPRESION DE TESTIGO DIARIO<<"
#define Msg_ENTERSETUP  ">>>>IMPRIMIR EN CONFIGURACION<<<"
#define Msg_EJDATEEND   "FECHA ULTIMA EJ"
#define Msg_EJRECORDS   "ULTIMO #EJ"
#define Msg_EJINITIAL   "INICIALIZACION EJ"

#define Msg_ECRDATEltEJ "FECHA EJ ECR"
#endif
#define Msg_MANAGEMENT "MANAGEMENT LIST "
#endif


