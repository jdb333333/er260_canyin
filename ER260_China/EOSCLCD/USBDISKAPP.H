#ifndef USBDISKAPP_H
#define  USBDISKAPP_H

#if (defined(DEBUGBYPC))
/* File access control and file status flags (FIL.flag) */

#define	FA_READ				0x01//Specifies read access to the object.
                                //Data can be read from the file. Combine with FA_WRITE for read-write access.
#define	FA_OPEN_EXISTING	0x00//Opens the file. The function fails if the file is not existing. (Default)
#if _FS_READONLY == 0           //
#define	FA_WRITE			0x02//Specifies write access to the object. Data can be written to the file.
                                //Combine with FA_READ for read-write access.
#define	FA_CREATE_NEW		0x04//Creates a new file. The function fails with FR_EXIST if the file is existing.
#define	FA_CREATE_ALWAYS	0x08//Creates a new file. If the file is existing, it is truncated and overwritten.
#define	FA_OPEN_ALWAYS		0x10//Opens the file if it is existing. If not, a new file is created.
                                //To append data to the file, use f_lseek() function after file open in this method.
#define FA__WRITTEN			0x20//
#define FA__DIRTY			0x40//
#endif                          //
#define FA__ERROR			0x80//
#endif


#define FILE_NAME_LENGTH    64

extern BYTE Backup_Restore;// 0-导入，1－导出 //

extern void usbExportNew(void);                    //usbFLOWNEW    1-导出新数据
extern void usbExportALL(void);                    //usbFLOWALL    2-导出全部数据
extern void usbExportDel(void);                    //usbFLOWALLDEL 3-导出全部数据后清空
extern void usbALLTheFile(void);    //Ccr全部文件  //

extern int FormatToCSV(void *data,BYTE dType,int len,BYTE flag);

extern void ReportToUSB(void);

BYTE CSV_ReceipLog(BYTE init);


extern int Inport_DEPT(void);
extern int Inport_PLU(void);
extern int Inport_TAX(void);
extern int Inport_HEADER(void);
extern int Inport_DISC(void);
extern int Inport_SALPER(void);
extern int Inport_SYSTEM(void);
extern int Inport_COMM(void);
extern int Inport_GRAPPRN(void);
extern int Inport_GROUP(void);
extern int Inport_ICCARD(void);
extern int Inport_PROM(void);
extern int Inport_PBFUNC(void);
extern int Inport_CLERK(void);
extern int Inport_MODI(void);
extern int Inport_PBTOTAL(void);
extern int Inport_ZONE(void);
extern int Inport_KEYBOARD(void);
extern int Inport_OFF(void);
extern int Inport_PORA(void);
extern int Inport_TRAILER(void);
extern int Inport_SLIPHEAD(void);
extern int Inport_ICBLOCK(void);

extern int Export_DEPT(void);
extern int Export_PLU(void);
extern int Export_TAX(void);
extern int Export_HEADER(void);
extern int Export_DISC(void);
extern int Export_SALPER(void);
extern int Export_SYSTEM(void);
extern int Export_COMM(void);
extern int Export_GRAPPRN(void);
extern int Export_GROUP(void);
extern int Export_ICCARD(void);
extern int Export_PROM(void);
extern int Export_PBFUNC(void);
extern int Export_CLERK(void);
extern int Export_MODI(void);
extern int Export_PBTOTAL(void);
extern int Export_ZONE(void);
extern int Export_KEYBOARD(void);
extern int Export_OFF(void);
extern int Export_PORA(void);
extern int Export_TRAILER(void);
extern int Export_SLIPHEAD(void);
extern int Export_ICBLOCK(void);

/////////////////收款机数据文件名称,固定8个字符/////////////////////////////////////////////////////

#define DEPTREC       "D_DEPART"                        //DEPTREC      部类文件//
#define PLUREC        "D_PLUDEF"                         //PLUREC       单品文件//
#define TAXREC		  "D_TAXDEF"				                  //TAXREC			  税率 //
#define HEADERREC     "D_HEADER"                      //HEADERREC    票头文字数据//
#define DISCREC       "D_DISDEF"                        //DISCREC      折扣文件//
#define SALPERREC     "D_SALPER"                      //SALPERREC    销售员文件//
#define SYSTEMREC     "D_SYSTEM"                      //SYSTEMREC    系统设置文件//
#define COMMREC		  "D_PORTDE"			                  //COMMREC			   通讯口 //
#define GRAPPRNREC    "D_GRAPRN"                     //GRAPPRNREC   票头票尾打印图片说明//
#define GROUPREC      "D_GROUPS"                       //GROUPREC     柜组文件//
#define ICCARDREC     "D_ICCARD"                      //ICCARDREC    IC卡使用文件//
#define PROMREC       "D_PROMOT"                        //PROMREC      中奖说明//
#define PBFUNCREC     "D_PBFUNC"                      //PBFUNCREC    餐饮功能说明文件//
#define CLERKREC      "D_CLERKS"                       //CLERKREC     收款员文件//
#define MODIREC       "D_MODIFY"                        //MODIREC      口味说明文件//
#define PBTOTALREC    "D_TABLES"                     //PBTOTALREC   桌台文件//
#define ZONEREC       "D_ZONEDE"                        //ZONEREC      分时统计的时间分段说明//
#define KEYBOARDREC	  "D_KEYTBL"	                  //KEYBOARDREC	 键盘 //
#define OFFREC        "D_OFFDEF"                         //OFFREC       特殊出价文件//
#define PORAREC       "D_PO_RAS"                        //PORAREC      出金入进//
#define TRAILERREC    "D_TRAILS"                     //TRAILERREC   票尾文字数据//
#define SLIPPRNREC	  "D_SLIPPR"		                  //SLIPPRNREC	  票据打印 //
#define SLIPHEADREC	  "D_SLIPHE"	                  //SLIPHEADREC	 票据打印头 //
#define ICBLOCKREC    "D_ICBLOK"                     //ICBLOCKREC   IC卡黑名单文件//
#define DRAWERREC     "D_DRAWER"                      //DRAWERREC    钱箱定义文件//
#define CORRECTREC    "D_CORREC"                     //CORRECTREC   更正文件//
#define TENDREC       "D_PAYDEF"                      //TENDREC      付款方式//

/////////////////收款机报表数据文件名称,规定为7个字符////////////////////////////////////////////////

#define TOTALREP      "R_TOTALS"                        //TOTALREP     总销售报表 //
#define DEPTREP       "R_DEPART"                        //DEPTREP      部类文件//
#define PLUREP        "R_PLUREP"                         //PLUREP       单品文件//
#define DISCREP       "R_DISCNT"                        //DISCREP      折扣文件//
#define SALPERREP     "R_SALERS"                      //SALPERREP    销售员文件//
#define GROUPREP      "R_GROUPS"                       //GROUPREP     柜组文件//
#define CLERKREP      "R_CLERKS"                       //CLERKREP     收款员文件//
#define PBTOTALREP    "R_PBTOTA"                     //PBTOTALREP   桌台文件//
#define CORREPTREP    "R_CORREC"                     //CORREPTREP   更正文件//
#define TENDREP       "R_TENDER"                      //TENDREP      付款方式//

/////////////////收款机流水数据文件名称,固定为8个字符////////////////////
#define BILLREC       "ECR_STUB"                        //BILLREC      U盘的流水数据//


#define ECR_REC		".CSV"						// 记录文件后缀 //
#define ECR_REP		".CSV"						// 报表文件后缀 //
#define ECR_LOG		".CSV"						// 流水文件后缀 //

//////////////////////////////////////////////////////////////////////////////////

#define usbReportType0    0							 //"   总  销  售   ",
#define usbReportType1    (usbReportType0 +1)         //" 柜  组  销  售 ",//1
#define usbReportType2    (usbReportType1 +1)         //" 部  类  销  售 ",//2
#define usbReportType3    (usbReportType2 +1)         //" 商  品  销  售 ",//3
#define usbReportType4    (usbReportType3 +1)         //" 付  款  信  息 ",//4
#define usbReportType5    (usbReportType4 +1)         //" 出 入 金 信 息 ",//5
#define usbReportType6    (usbReportType5 +1)         //" 钱  箱  信  息 ",//6
#define usbReportType7    (usbReportType6 +1)         //" 更  改  信  息 ",//7
#define usbReportType8    (usbReportType7 +1)         //" 折  扣  信  息 ",//8
#define usbReportType9    (usbReportType8 +1)         //" 外  币  信  息 ",//9
#define usbReportType10   (usbReportType9 +1)         //" 计  税  信  息 ",//10
#define usbReportType11   (usbReportType10+1)         //" 桌  帐  信  息 ",//11
#define usbReportType12   (usbReportType11+1)         //" 开  台  信  息 ",//12
#define usbReportType13   (usbReportType12+1)         //" 桌  台  信  息 ",//13
#define usbReportType14   (usbReportType13+1)         //" 商  品  库  存 ",//14
#define usbReportType15   (usbReportType14+1)
#define usbReportType16   (usbReportType15+1)
#define usbReportType17   (usbReportType16+1)        	//"    收 款 员 统 计   ",
#define usbReportType18   (usbReportType17+1)   		//"    营 业 员 统 计   ",
#define usbReportType19   (usbReportType18+1)        	//"    收 款 员 折扣 统 计   ",
#define usbReportType20   (usbReportType19+1)   		//"    营 业 员 折扣统 计   ",
#define usbReportType21   (usbReportType20+1)        	//"    收 款 员 桌台统 计   ",
#define usbReportType22   (usbReportType21+1)   		//"    营 业 员 桌台统 计   ",

//////////////////////////////////////////////////////////////////////////////////

#define BACKUPECR	0
#define RESTOREECR	1

//ccr Functions list for menus
typedef struct{
	const char*	FileName;				// 文件名称 //
    const char* CSVTitle;               //导出的CSV文件的title
    int    (*InportECRFile)();    //导入文本形式的收款机文件的功能函数 //
    int    (*ExportECRFile)();    //以文本形式导出收款机文件的功能函数 //
	char*   RECBuffer;				// 文件数据存储区 //
	WORD	RECSize;				// 一条记录的长度  //
	WORD    *RECMax;					// 记录数 //
	WORD    *RECIndex;				// 记录指针 //
    void    (*ReadECRFile)();    //ccr 收款机文件数据读取函数 //
    void    (*WriteECRFile)();    //ccr 收款机文件数据写入函数 //
} ECRFILEClass;

// CSV数据类型描述转换
#define BYTETYPE    1       //字节类型
#define WORDTYPE    2       //字类型
#define LONGTYPE    3       //双字类型
#define CHARTYPE    4       //字符类型
#define STRTYPE     5       //字符串类型
#define QTYTYPE     6       //BCD数量数据,固定三位小数
#define AMTTYPE     7       //BCD金额数据,固定两位小数
#define HQTYTYPE    8       //Hex.BCD数量数据,固定三位小数,即此BCD无Sign
#define HAMTTYPE    9       //Hex.BCD金额数据,固定两位小数,即此BCD无Sign
#define HEXTYPE     10       //HEX数据,即将指定的数据转换为HEX,清除高位0
#define OPTTYPE     11       //选项类型(将数据按位变换 为245,表示Bit2,Bit4,Bit5为1

#define ADD0D0A     12      //ccr2016-09-06追加回车换行

#define PERTYPE     0x80    //追加%

#endif

