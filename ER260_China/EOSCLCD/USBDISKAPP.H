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

extern BYTE Backup_Restore;// 0-���룬1������ //

extern void usbExportNew(void);                    //usbFLOWNEW    1-����������
extern void usbExportALL(void);                    //usbFLOWALL    2-����ȫ������
extern void usbExportDel(void);                    //usbFLOWALLDEL 3-����ȫ�����ݺ����
extern void usbALLTheFile(void);    //Ccrȫ���ļ�  //

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

/////////////////�տ�������ļ�����,�̶�8���ַ�/////////////////////////////////////////////////////

#define DEPTREC       "D_DEPART"                        //DEPTREC      �����ļ�//
#define PLUREC        "D_PLUDEF"                         //PLUREC       ��Ʒ�ļ�//
#define TAXREC		  "D_TAXDEF"				                  //TAXREC			  ˰�� //
#define HEADERREC     "D_HEADER"                      //HEADERREC    Ʊͷ��������//
#define DISCREC       "D_DISDEF"                        //DISCREC      �ۿ��ļ�//
#define SALPERREC     "D_SALPER"                      //SALPERREC    ����Ա�ļ�//
#define SYSTEMREC     "D_SYSTEM"                      //SYSTEMREC    ϵͳ�����ļ�//
#define COMMREC		  "D_PORTDE"			                  //COMMREC			   ͨѶ�� //
#define GRAPPRNREC    "D_GRAPRN"                     //GRAPPRNREC   ƱͷƱβ��ӡͼƬ˵��//
#define GROUPREC      "D_GROUPS"                       //GROUPREC     �����ļ�//
#define ICCARDREC     "D_ICCARD"                      //ICCARDREC    IC��ʹ���ļ�//
#define PROMREC       "D_PROMOT"                        //PROMREC      �н�˵��//
#define PBFUNCREC     "D_PBFUNC"                      //PBFUNCREC    ��������˵���ļ�//
#define CLERKREC      "D_CLERKS"                       //CLERKREC     �տ�Ա�ļ�//
#define MODIREC       "D_MODIFY"                        //MODIREC      ��ζ˵���ļ�//
#define PBTOTALREC    "D_TABLES"                     //PBTOTALREC   ��̨�ļ�//
#define ZONEREC       "D_ZONEDE"                        //ZONEREC      ��ʱͳ�Ƶ�ʱ��ֶ�˵��//
#define KEYBOARDREC	  "D_KEYTBL"	                  //KEYBOARDREC	 ���� //
#define OFFREC        "D_OFFDEF"                         //OFFREC       ��������ļ�//
#define PORAREC       "D_PO_RAS"                        //PORAREC      �������//
#define TRAILERREC    "D_TRAILS"                     //TRAILERREC   Ʊβ��������//
#define SLIPPRNREC	  "D_SLIPPR"		                  //SLIPPRNREC	  Ʊ�ݴ�ӡ //
#define SLIPHEADREC	  "D_SLIPHE"	                  //SLIPHEADREC	 Ʊ�ݴ�ӡͷ //
#define ICBLOCKREC    "D_ICBLOK"                     //ICBLOCKREC   IC���������ļ�//
#define DRAWERREC     "D_DRAWER"                      //DRAWERREC    Ǯ�䶨���ļ�//
#define CORRECTREC    "D_CORREC"                     //CORRECTREC   �����ļ�//
#define TENDREC       "D_PAYDEF"                      //TENDREC      ���ʽ//

/////////////////�տ�����������ļ�����,�涨Ϊ7���ַ�////////////////////////////////////////////////

#define TOTALREP      "R_TOTALS"                        //TOTALREP     �����۱��� //
#define DEPTREP       "R_DEPART"                        //DEPTREP      �����ļ�//
#define PLUREP        "R_PLUREP"                         //PLUREP       ��Ʒ�ļ�//
#define DISCREP       "R_DISCNT"                        //DISCREP      �ۿ��ļ�//
#define SALPERREP     "R_SALERS"                      //SALPERREP    ����Ա�ļ�//
#define GROUPREP      "R_GROUPS"                       //GROUPREP     �����ļ�//
#define CLERKREP      "R_CLERKS"                       //CLERKREP     �տ�Ա�ļ�//
#define PBTOTALREP    "R_PBTOTA"                     //PBTOTALREP   ��̨�ļ�//
#define CORREPTREP    "R_CORREC"                     //CORREPTREP   �����ļ�//
#define TENDREP       "R_TENDER"                      //TENDREP      ���ʽ//

/////////////////�տ����ˮ�����ļ�����,�̶�Ϊ8���ַ�////////////////////
#define BILLREC       "ECR_STUB"                        //BILLREC      U�̵���ˮ����//


#define ECR_REC		".CSV"						// ��¼�ļ���׺ //
#define ECR_REP		".CSV"						// �����ļ���׺ //
#define ECR_LOG		".CSV"						// ��ˮ�ļ���׺ //

//////////////////////////////////////////////////////////////////////////////////

#define usbReportType0    0							 //"   ��  ��  ��   ",
#define usbReportType1    (usbReportType0 +1)         //" ��  ��  ��  �� ",//1
#define usbReportType2    (usbReportType1 +1)         //" ��  ��  ��  �� ",//2
#define usbReportType3    (usbReportType2 +1)         //" ��  Ʒ  ��  �� ",//3
#define usbReportType4    (usbReportType3 +1)         //" ��  ��  ��  Ϣ ",//4
#define usbReportType5    (usbReportType4 +1)         //" �� �� �� �� Ϣ ",//5
#define usbReportType6    (usbReportType5 +1)         //" Ǯ  ��  ��  Ϣ ",//6
#define usbReportType7    (usbReportType6 +1)         //" ��  ��  ��  Ϣ ",//7
#define usbReportType8    (usbReportType7 +1)         //" ��  ��  ��  Ϣ ",//8
#define usbReportType9    (usbReportType8 +1)         //" ��  ��  ��  Ϣ ",//9
#define usbReportType10   (usbReportType9 +1)         //" ��  ˰  ��  Ϣ ",//10
#define usbReportType11   (usbReportType10+1)         //" ��  ��  ��  Ϣ ",//11
#define usbReportType12   (usbReportType11+1)         //" ��  ̨  ��  Ϣ ",//12
#define usbReportType13   (usbReportType12+1)         //" ��  ̨  ��  Ϣ ",//13
#define usbReportType14   (usbReportType13+1)         //" ��  Ʒ  ��  �� ",//14
#define usbReportType15   (usbReportType14+1)
#define usbReportType16   (usbReportType15+1)
#define usbReportType17   (usbReportType16+1)        	//"    �� �� Ա ͳ ��   ",
#define usbReportType18   (usbReportType17+1)   		//"    Ӫ ҵ Ա ͳ ��   ",
#define usbReportType19   (usbReportType18+1)        	//"    �� �� Ա �ۿ� ͳ ��   ",
#define usbReportType20   (usbReportType19+1)   		//"    Ӫ ҵ Ա �ۿ�ͳ ��   ",
#define usbReportType21   (usbReportType20+1)        	//"    �� �� Ա ��̨ͳ ��   ",
#define usbReportType22   (usbReportType21+1)   		//"    Ӫ ҵ Ա ��̨ͳ ��   ",

//////////////////////////////////////////////////////////////////////////////////

#define BACKUPECR	0
#define RESTOREECR	1

//ccr Functions list for menus
typedef struct{
	const char*	FileName;				// �ļ����� //
    const char* CSVTitle;               //������CSV�ļ���title
    int    (*InportECRFile)();    //�����ı���ʽ���տ���ļ��Ĺ��ܺ��� //
    int    (*ExportECRFile)();    //���ı���ʽ�����տ���ļ��Ĺ��ܺ��� //
	char*   RECBuffer;				// �ļ����ݴ洢�� //
	WORD	RECSize;				// һ����¼�ĳ���  //
	WORD    *RECMax;					// ��¼�� //
	WORD    *RECIndex;				// ��¼ָ�� //
    void    (*ReadECRFile)();    //ccr �տ���ļ����ݶ�ȡ���� //
    void    (*WriteECRFile)();    //ccr �տ���ļ�����д�뺯�� //
} ECRFILEClass;

// CSV������������ת��
#define BYTETYPE    1       //�ֽ�����
#define WORDTYPE    2       //������
#define LONGTYPE    3       //˫������
#define CHARTYPE    4       //�ַ�����
#define STRTYPE     5       //�ַ�������
#define QTYTYPE     6       //BCD��������,�̶���λС��
#define AMTTYPE     7       //BCD�������,�̶���λС��
#define HQTYTYPE    8       //Hex.BCD��������,�̶���λС��,����BCD��Sign
#define HAMTTYPE    9       //Hex.BCD�������,�̶���λС��,����BCD��Sign
#define HEXTYPE     10       //HEX����,����ָ��������ת��ΪHEX,�����λ0
#define OPTTYPE     11       //ѡ������(�����ݰ�λ�任 Ϊ245,��ʾBit2,Bit4,Bit5Ϊ1

#define ADD0D0A     12      //ccr2016-09-06׷�ӻس�����

#define PERTYPE     0x80    //׷��%

#endif
