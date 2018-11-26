#ifndef EXTH_H
#define EXTH_H

//------------------------For Debug--------------------------
#ifndef ForDebug
#include "debug.h"
#endif
//------------------------For Debug--------------------------
#if defined(DEBUGBYPC)
extern void SRAM_Move(int toAddr, int fromAddr, int BytesToMove);
void OutPrintExForDebug(WORD Cmd, char *Line, WORD DotLinesBlank);

extern "C" __declspec(dllexport)
int  SetupCom232(int BaudRate, int ByteSize, int StopBits, int Parity,DWORD TimesOut);

#else

extern BYTE PowerFlags;		//检测到的电源状态
/*
    Bit0:=0,为适配器供电;=1,为电池供电
    Bit1:=1,为电池供电时,出现报警PWR_WARNING1(电压次低)
    Bit2:=1,为电池供电时,出现报警PWR_WARNING2(电压过低)

    Bit4:=1,纽扣电池出现报警PWR_WARNING1(电压次低)
    Bit5:=1,纽扣电池出现报警PWR_WARNING2(电压过低)

*/
extern BYTE pwrGetStatus(BYTE GetStatus);

#endif

extern void CutRPaper(short lines);

extern void PrintStr_Center(char *str,BYTE alCenter);
extern void PrintStr_Right(char *str);

extern BYTE InputYesNo(const char *pMessage,BYTE x,BYTE y,BYTE pDefault);

extern void LoadReceipLog(void);
extern void ReadPort(void);
extern void WritePort(void);
extern void WriteAgree(void);
extern void PrintVersion(void);
extern void ReadAgree(void);
extern short Read_Flow(BYTE *Area);

extern short GetStrFrKBD(BYTE type,BYTE x,BYTE y,BYTE maxLen,BYTE port);
extern short GetStringCHN(char *buf,short width,BYTE hzInput);
void PrintKeyboard(void);
extern void DeptPluKeysCount(void);
extern void SetRTS(BYTE port);
extern void ClrRTS(BYTE port);
extern void DispSubTotal(void);

extern void ClearCom(void);
extern BYTE PowerCheck(void);
extern BYTE GetInit(void);
extern void InitRtc(void);
extern long TestRam(void);
extern WORD CheckSum(void) ;
extern void DrawerOpen(void);
extern BYTE GetFontT(void);
extern BYTE GetFontB(void);
extern BYTE GetDotFont(void);
extern void PrintDotEpson(void);
extern void InitComm(void);

extern BYTE TestRtc(void);

extern void ReadRam(BYTE *to,WORD bytes);	    /* () read bytes from ram banks */
extern void WriteRam(BYTE *from,WORD bytes);     /* () write bytes to ram banks */

extern void Save_ApplRam(void);
extern BYTE Recall_ApplRam(void);


extern BYTE CheckPWD(const char* PWDType,char *pass);
extern BYTE SetPassword(const char* PWDType,char *pass);
extern int DateTimeToStr(char *sBuf,BYTE pTime);
extern void SetComm(BYTE Com);

extern BYTE TestCom123(BYTE port);

extern void SendComm(BYTE,BYTE);
extern short ReadComm(BYTE);
extern WORD CheckComm(BYTE);
extern void EmptyComm(BYTE);

extern WORD EncordDate(WORD sYear,BYTE sMonth,BYTE sDay);
extern void DecordDate(WORD sDate,WORD *sYear,BYTE *sMonth,BYTE *sDay);
extern BYTE DaysAMonth(WORD y,BYTE m);  //算出某年某月有多少天

extern BYTE ChangePoint(void);
extern BYTE ChangePoint2(void);//ccr2015-08-14
extern void DisplayHex(char *pHex, short pWidth);
void DisplayDecXY(WORD pDec,int x,int y,int pWdith);
void DisplayULongXY(ULONG pULong,int x,int y);
extern short WORDtoASCL(char *to, WORD num);
extern BYTE  WORDtoASC(char *to, WORD num);        /* (void *to, WORD) make ascii string from WORD */
extern void  WORDtoASCZero(char *to, WORD num);     /* (void *to, WORD) make asci string from WORD with */
extern BYTE  ULongtoASC(char *to, ULONG num);        /* (void *to, ULONG) make ascii string from ULONG */
extern int ULongtoASCL(char *to, ULONG num);
extern BYTE Display_RGBuf(short UpDown);
extern BYTE ULong64toASC(char *to, UINT64 num);

extern BYTE GetWeekDay(WORD year,BYTE month,BYTE day);
extern WORD GetDaysFrom1900(WORD year,BYTE month,BYTE day);
extern void DeDateToStr(char *strDate,WORD sDate);
extern void BCDDateTimeToStr(char *strDate,BYTE FYear,BYTE FMonth,BYTE FDay,BYTE FHour,BYTE FMinute,BYTE FSecond);
extern BYTE CheckBCDDate(BYTE *sDate);
extern ULONG PackageDate(BYTE *Value);

BYTE TestChangePointMust(void);
void BcdDiv100(BCD *A0);
/***************************************************/

#ifndef MAIN_C
extern  void OutPrint(WORD Cmd, char *Line);	//Lancia stampa (standard)

extern  void OutPrintEx(WORD Cmd, char *Line, WORD DotLinesBlank);
#endif
extern  void PowerUp(void);
extern  short CheckFunctionEntry(void);
extern  BYTE AmtInputMask(void);
extern  void ProcessKey(void);
extern void   ProcessFunctions(void);
extern  WORD KeyInput(WORD  keyno);

extern  void GetEntry(void);

/***************************************************/
#ifndef BUFFER_C
extern  void ProcessDeptRecord(void);
extern  void CheckModifier(void);
extern  void ProBuf(void);
extern  void ProcessBuffer(void);
extern  void StoreInBuffer(void);
#endif
/***************************************************/
#ifndef CLERK_C
extern  void GetClerkOffSet(void);
extern  void WriteClerk(void);
extern  void ReadClerk(void);
extern  void SelectClerk(BYTE lock);
#endif
/***************************************************/
#ifndef COMPUTER_C
extern char InitModem(void);
extern void HandDownModem(void);
extern void DialOverModem(char *pTeleNo);
extern	short ReadString(char *sStr,short size);
extern  void PrnBuffer(char *buf,short len);
extern  void SendString(char *sStr,short len);
extern  void Computer(void);
extern  void ProcessRecord(void);
extern  BYTE SendComp(BYTE type,BCD *tot);
//extern  void SendRecord(BYTE *p,BYTE length);
extern BYTE SendRecord(BYTE *p,BYTE length);
#endif
/***************************************************/
#ifndef CORREC_C
extern  void GetCorrecOffSet(void);
extern  void AddCorrecTotal(void);
extern  void WriteCorrec(void);
extern  void ReadCorrec(void);
extern  void Correction(void);
#if FISCAL == 0 || MALTA == 1
extern  void TransactionVoid(WORD corrType);
#endif
#endif
/***************************************************/
#ifndef CURRENCY_C
extern  void GetCurrOffSet(void);
extern  void AddCurrTotal(void);
extern  void WriteCurr(void);
extern  void ReadCurr(void);
extern  void GetCurrency(BYTE sell, BCD *amt);
extern  void Currency(void);
#endif
/***************************************************/
#ifndef DEPART_C
extern  void GetDeptOffSet(void);
extern  void AddDeptTotal(void);
extern  void WriteDept(void);
extern  void ReadDept(void);
extern  void PrintModifiers(void);
extern  char *GetPluPrint(char *temp,BYTE  cmd);
extern  BYTE CheckRefund(void);
extern  void ProcessDept(void);
#endif
/***************************************************/
#ifndef DISCOUNT_C
extern  void AddDiscTotal(void);
extern  void WriteDisc(void);
extern  void ReadDisc(void);
extern  void PrintPercentage(BCD *perc);
extern  void Discount(void);
extern  void GetDiscOffSet(void);
//cc 2006-10-10 for SubDisc>>>>>>>>>>>>
extern  void CalculateSubDisc(int sType);
//cc 2006-10-10 for SubDisc<<<<<<<<<<<<

#endif
/***************************************************/
#ifndef DRAWER_C
extern  void GetDrawerOffSet(void);
extern  void AddDrawerTotal(void);
extern  void WriteDrawer(void);
extern  void ReadDrawer(void);
extern  void OpenDrawer(void);
extern  void Drawer(void);
extern void DispDrawerTotal(void);
#endif
/***************************************************/
#ifndef FIRMKEY_C
/**** 获取功能键在ApplVar.AP.FirmKey中的序号 ************
  返回0xff:不是功能键
   ******************************************************/
extern BYTE GetFirmkeyID(BYTE keyno);

extern void PrintSetupMenu(void);
extern  short DisplayOption(void);
extern  WORD CheckFirmKey(void);
extern 	BOOL CheckPassword(void);
extern 	BYTE CheckMode(void);
extern  void (*SetupList)(BYTE keyno);
extern  BYTE GetInputForMenu(BYTE numkey);	//hf 20060105
#endif
/***************************************************/
#ifndef FIXED_C
extern  void CheckMultiply(void);
extern  void NewReceipt(void);
extern  void PrintNumber(BCD *num);
extern  void SuspendReceipt(void);
extern  void Fixed(void);
#endif
/*****************************************************/
#ifndef FISCAL_C
extern  void AddReceiptNumber(void) ;
extern  void GetReceiptNumber(char *to) ;
#if FISCAL == 1
extern  char CheckBattery(void);
extern  short BlankCheckProm(void) ;
extern  BYTE CheckExento(void) ;
extern  short CheckFiscal(void);
extern  void PrintRfc(void) ;
extern  short CheckProm(void);
extern  void FiscalTrailer(void);
extern  void FiscalHeader(void);
extern  void GetFiscalTotal(void);
extern  void PrintFiscalTotal(void);
extern  void PrintFiscalTax(void) ;
extern  BYTE ExentoStatus(void) ;
extern  BYTE CheckExento(void) ;
extern  void DumpFiscal(void) ;
extern  void InitPrinter(void) ;
#endif
#endif
/***************************************************/
#ifndef FORMAT_C
extern  void FormatAmt(char *to,BCD *amt);
extern  void FormatQty(char *to ,BCD *qty );
extern  char *FormatStrQtyPriAmt(CONSTCHAR *str,BCD *qty,BCD *pri,BCD *amt,short width);
extern  char *FormatAmtStr(CONSTCHAR *str ,BCD *amt,short width);
extern  char *FormatQtyStr(CONSTCHAR *str,BCD *qty,short width);
extern  void CheckDisPoint(void);
extern  char *DispQtyStr(CONSTCHAR *str ,BCD *qty,short width);
extern  char *DispAmtStr(CONSTCHAR *str,BCD *amt,short width);
#endif
/***************************************************/
#ifndef GENERAL_C
extern  void PromtionCheck(void);
extern  void GetWordEntry(void);
extern  void AddTotal(void);
extern  void AddPointerTotal(void);
extern  void ClearEntry(BYTE disp);
#if FRONT == 1
extern  void DisplayMessage(void);
#endif
extern  short CheckMaxEntry(BCD *bcd,BYTE a);
extern  void CheckError(BYTE  a);
extern  void CheckBreakKey(void);
extern  void PrintMessage(WORD) ;
extern  WORD RegiInit(void);
extern  WORD RegiStart(void);
extern  void RegiEnd(void);
extern  void PrintTotal(void);
extern  void ReadTotal(void);
extern  void WriteTotal(void);
extern  void PrintPbTrailer(void);
extern void DiaplayTotal(void);
extern void DisplayPointerTotal(char *sFile);
#endif
/***************************************************/
#ifndef HARDTEST_C
extern void PrintLine2(char c);          /* print line of characters */
#if CHINA != 1
extern  void PrintFont(void);
#endif
#if CHINA != 0
extern  void DumpChina(short progNumber);
#endif
extern  void HardTest(WORD idx);
extern void PrintConfInf(void);
extern  void CheckPluFile(WORD type) ;
#if JAN == 1
extern  void MoveXRam(void);
extern  void MergePlu(void);

#endif
#endif
/***************************************************/
void Initial_DateTime(BYTE idx);

#ifndef INITAPPL_C
extern void Save_Config(BYTE enSave);
extern void Save_ApplRam(void);
extern BOOL Recall_Config(void);
extern void InitPlu(BYTE clTotal);
extern void InitDept(BYTE clTotal);
extern void InitGroup(void);
extern void InitTender(void);
extern void InitDrawer(void);
extern void InitCorrec(void);
extern void InitCurr(void);
extern void InitDisc(void);
extern void InitPoRa(void);
extern void InitTax(void);
extern void InitPbF(void);
extern void InitClerk(void);
extern void InitModifier(void);
extern void ClearApplMemory(void);
//根据sizeof(APPLICATION_SAVE),来修正StartAddress
//toActual:=true,转换为实际地址,StartAddress[AddrEndP]=sizeof(APPLICATION_SAVE);
//        =false,转换为相对地址,StartAddress[AddrEndP]=0
//return:=true,地址未越界;=false,地址越界
extern BYTE ResetStartAddress(BYTE toActual);
extern void ClsXRam(void);
extern void InitApplication(BYTE initMust);
#endif

/***************************************************/
#ifndef PLU_C
extern void ReadPluStock(void);
extern void LookPlu(BYTE pType);//pType=0,look price,pType>0 look inv
extern void GetPluOffSet(void);
extern void AddPluTotal(void);
extern void WritePluInventory(void);
extern void WritePlu(void);
extern void WritePlu_At(void);
extern void ReadPlu(void);
extern void Add_New_PLU(WORD pPluNumber);

extern WORD GetPluNumber(BYTE,BYTE*);

extern WORD  GetLinkNumber(void) ;

extern short CheckRandomPlu(BYTE fdelete,BYTE frHost);
extern void ProcessPlu(void);
extern void CheckMixMatch(short bufchk);
extern void PrintDept_PLU(void);

#endif
/***************************************************/
#ifndef PORA_C
extern void GetPoRaOffSet(void);
extern void AddPoRaTotal(void);
extern void WritePoRa(void);
extern void ReadPoRa(void);
extern void PaidOutRa(void);
#endif
/***************************************************/
#ifndef PRINT_C
extern BYTE CheckChar(BYTE);
extern void DisplayStr(char *);
extern void ReleaseSlip(void);
extern BYTE CheckSlip(void);
extern void FeedSlip(BYTE);
extern BYTE PrintAmt(CONSTCHAR *, BCD *amt);
extern BYTE PrintQty(CONSTCHAR *,BCD *);
extern BYTE PrintStr(CONSTCHAR*);
extern BYTE PrintLine(char);
extern BYTE PrintQtyAmt(BCD *,CONSTCHAR *,BCD *,BYTE);
extern void SendSlip(void);
extern void CmdSlip(char);
extern void PrintSlipPage(short);
extern void SlipMargin(void);
extern void PrintSlip(CONSTCHAR*);
extern BYTE PrintLogo(void);
extern void PrintHead1(short);
extern void PrePrintHead(short);
extern short CheckNewChinese(void);
extern short InitChinese(void);
extern short CheckChinese(void);
extern void PrintRJ(CONSTCHAR *);
extern void PrintMultiply(void);
extern void PrintSaleQty(void);
extern BYTE PrintPicture(short ,BYTE);
extern void PrintRegiInfo(BYTE prnGrp);
extern void ReceiptIssue(short);
extern void PrintMessage(WORD);
extern void PrintGprsMessage(WORD);  // add by lqw ,difine in General.c and cpp.
extern void PrintAllons(void);
#endif
/***************************************************/
#ifndef PROGDUMP_C
extern void ProgramDump(void);
#endif

#ifndef PROGOPT_C
#if (SKIP & S_PROG)
extern BYTE GetOpt(BYTE,void *,BYTE);
#else
extern void GetByte(BYTE *, BYTE, BYTE);
extern void GetHexBytes(BYTE *,BYTE);
//extern void InvertBit(BYTE,void *);
extern void InvertBit(WORD,void *);
extern void Btoa(BYTE,void *);
extern BYTE GetOpt(BYTE,void *,BYTE);
#endif
#endif

/***************************************************/
#ifndef PROGRAM_C
extern void SetEditPosition(BYTE x,BYTE y,BYTE len,char *source);

extern BYTE GetString(short MsgIdx,BYTE *val,WORD size,WORD num);
extern void GetBCDValue(short MsgIdx,char *val,short BCDBytes,char IsQty);
extern void GetByteValue(short MsgIdx,BYTE *val,BYTE max);
extern void CheckBitValue(short MsgIdx,char *Opt,BYTE BitNo,BYTE sInv);

extern BYTE NumToIP(char *IPstr,BYTE *Num);
extern BYTE IPToNum(BYTE *Num,char *IPstr,short Counter);
extern void GetIP(short MsgIdx,BYTE *val);
extern void ProgIP(void);
//ccr2016-01-21>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
extern void ProgHost(void);
extern void ProgBarcodeReader(void);
extern void ProgBalance(void);
extern void  ProgKeyMacro(void);
//ccr2016-01-21<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//============= hf added end ================================
extern void InitSysFlag(void);
extern void ProgPlu(void);
extern void ProgDept(void);
extern void ProgGroup(void);
extern void ProgTend(void);
extern void ProgPoRa(void);
extern void ProgCorrec(void);
extern void ProgDisc(void);
extern void ProgCurr(void);
extern void ProgDraw(void);
extern void ProgPbF(void);
extern void ProgPbInfo(void);
extern void ProgTax(void);
extern void ProgClerk(void);
extern void ProgZone(void);
extern void ProgModi(void);
extern void ProgIC(void);  //ccr chipcard 2004-07-01
extern void ProgICBlock(void);  //ccr chipcard 2004-07-01
extern void ProgHeader(void);
extern void ProgTrailer(void);
extern void ProgSlipHead(void);
extern void ProgSysFlag(void);
extern void ProgSysMes(void);
extern void ProgReport(void);
extern void ProgPasswd(void);
extern void PrintUp(void) ;
extern void ProgFiscal(void) ;
extern void ProgSalPer(void) ;
extern void ProgDate(void) ;
extern void ProgTime(void) ;
extern void ProgOFF(void) ;
extern void ProgPort(BYTE port);


//cc 2006-08-29 for MMC>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
extern void ProgPrnGraph(void);
extern void ProgPromotion(void);
extern void ProgAgree(void);
extern void ProgKPrn(void);
extern void ProgSlip(void);
extern void ProgExtPrinter(void);
//extern void ProgPrnGraph(void);

#endif


#ifndef REPORT_C
extern short CheckTotal(void);

extern void GetReport(BYTE zxNum);
extern void GetSystemReport(BYTE);
extern void PrintReportType(void);
extern void PrintDept(WORD);
extern void PrintPlu(WORD);
extern void PrintTender(WORD);
extern void PrintPoRa(WORD);
extern void PrintCurrency(WORD);
extern void PrintDrawer(WORD);
extern void PrintCorrec(WORD);
extern void PrintDisc(WORD);
extern void PrintTax(WORD);
extern WORD  PeriodSkip(struct REPSIZE *,BYTE);
extern short CheckBreak(void);
extern void PrintRange(void);
extern void ResetReport(void);
extern void PrintReport(short RepFor,struct REPSIZE *);
extern void PrintPluReport(short RepFor);

#if (PLUCODE_X)
BYTE Plu_XReport(BYTE keyno);
#endif

extern void PrintDeptReport(void);
extern void PrintGroupReport(void);
extern void PrintTendReport(void);
extern void PrintPoRaReport(void);
extern void PrintCurrReport(void);
extern void PrintDrawReport(void);
extern void PrintCorrecReport(void);
extern void PrintDiscReport(void);
extern void PrintTaxReport(void);
extern void PrintPbFReport(void);
extern void PrintPointReport(void);
extern void GetSystemReport(BYTE);
extern void PrintBARCustomer(BYTE xz);

#endif
/***************************************************/
#ifndef SEARCH_C
extern WORD  BinarySearch(	void *,WORD,BYTE,short );
#endif
/***************************************************/
#ifndef TAX_C
extern void GetTaxOffSet(void);
extern void AddTaxTotal(void);
extern void WriteTax(void);
extern void ReadTax(void);
extern void AddTaxItem(BYTE);
extern void AmtRound(BYTE,BCD *);
extern void CalculateTax(BYTE);
extern void CalculateTax2(void);		/* cc 2006-10-28 for SubDisc */
#endif
/***************************************************/
#ifndef TENDER_C
extern short SelectCard(void);
extern void GetTenderOffSet(void);
extern void AddTenderTotal(void);
extern void WriteTender(void);
extern void ReadTender(void);
extern void Tender(void);
#endif
/***************************************************/
#ifndef TIMER_C

extern BYTE day,min,sec;


#if FISCAL == 1
#if MALTA == 1
extern  BYTE rtime;
#endif
#endif
extern void MakeTime(char *,WORD);
extern void CheckTime(BYTE pAll);
extern void GetTimeZones(void);
extern void NewTimeDate(BYTE);
extern void LCDLight_Ctrl(void);    //test
#endif
/***************************************************/

#ifndef REPORT2_C
extern WORD  SetUpReport(void);
extern void MakeTime(char *,WORD);
extern void PrintPointType(void);
extern void ReportEnd(BYTE);
extern void ClearAllReport(void);
extern void PrintReportHeader(void);
void DisplayXZMenu(BYTE xzType);//ccr2016-02-22
void ProcessXFunc(WORD xFunc);
void ProcessZFunc(WORD zFunc);
#endif
/***************************************************/

#ifndef PB_C
extern short StorePbInvoice(short);
extern void PrintPbHeader(void);
extern void PrintNewBal(void) ;
extern void AddPbFTotal(void) ;
extern void ReadPbF(void) ;
extern void WritePbF(void) ;
extern void GetPbFOffSet(void);
extern short GetPbNumber(void) ;
extern void ClearPb(void) ;
extern void PbTotal(WORD pbnum, BYTE cmd) ;
extern void PbFunction(void) ;
extern void GetPbtOffSet(short pbnum);
extern void AddPbtTotal(void);
extern void PrintPbtReport(void);
extern void PrintPb(WORD number);
#endif

/***************************************************/
#ifndef KP_C
extern void CmdKp(BYTE) ;
extern void PrintKp(BYTE cmd, CONSTCHAR *str) ;
extern void PrintPbKp(BYTE) ;
extern void KpHeader(void) ;
extern void IssueKp(void) ;
extern void KpEnd(BYTE) ;
extern void PrintArticle(BYTE cmd, BYTE kp);
extern void SendExtPrinter(BYTE cmd,BYTE *str,BYTE selePRN);//ccr2017-08-31
extern void CutExtPrinter(void);
void CmdExtPrinter(BYTE port,BYTE type,BYTE cmd);
#endif
/***************************************************/

#ifndef PBBUFFER_C
extern BYTE UpdatePbRecord(BYTE *,BYTE) ;
extern BYTE PbBuffer(WORD);
extern void PrintPbItem(BYTE *, BYTE *, BYTE *) ;
extern BYTE StorePbBuffer(void) ;
extern void ProcessPbBuffer(WORD) ;
#endif
/***************************************************/

#ifndef GROUP_C
extern void GetGroupOffSet(void) ;
extern void AddGroupTotal(void) ;
extern void WriteGroup(void) ;
extern void ReadGroup(void) ;
#endif
/***************************************************/

#ifndef SALPER_C
extern void GetSalPerOffSet(void) ;
extern void WriteSalPer(void) ;
extern void ReadSalPer(void) ;
extern void SelectSalPer(void) ;
#endif

/***************************************************/

#ifndef BARCODE_C

/***************************************************************/

#if !(SKIP & S_BAR)
extern void CheckInStore(void) ;
#endif
extern void PrintBarCode(char , char *) ;
extern WORD BarCode(void) ;
#endif
/***************************************************************/

#if BONUS == 1
#ifndef BANUS_C
extern void MakeWide(char *,short) ;
extern void CalcBonusPoints(void) ;
extern char *GetBonusLine(void) ;
#endif
#endif

#ifndef MODIFIER_C
extern void GetModiOffSet(void) ;
extern void WriteModi(void) ;
extern void ReadModi(void) ;
extern void GetModifier(void) ;
#endif

#ifndef ICBLOCK_C  //ccr chipcard 2004-07-01
extern void GetICBlockOffSet(void) ;
extern void WriteICBlock(void) ;
extern void ReadICBlock(void) ;
extern void PrintICBlock(void);
#endif

#ifndef OFF_C
extern void GetOFFPriceOffSet(void);
extern void ReadOFFPrice(void);
extern void WriteOFFPrice(void);
#endif

#ifndef PORT_C
extern void GetPortOffSet(void);
extern void ReadPortPrice(void);
extern void WritePortPrice(void);
extern void SetupPort(BYTE port);
extern BYTE EnCodeProto(char * pro);
extern void DeCodeProto(BYTE In, char * out);

#endif

#ifndef BALANCE_C
extern short 	BalCounter;
extern short BalanceWeight(void);
extern BYTE BalanceCMD(void);
#endif

//#ifndef ICCARD_H	//ccr chipcard
extern void PointsByChipCard(BCD *pValue);
extern void PayByChipCard(BCD *pValue,BCD *pIC_AMT,BYTE pay);
extern BYTE ChipCard(void);
extern short ReadFactory(void);         //OK
extern char RD_ChipCard(void);
extern short Clear_CHIP(void);
extern signed char Initial_CHIP(void);
extern short Charge_CHIP(void);

extern void ProgClearChip(void);
extern void ProgChargeChip(void);
extern void ProgInitialChip(void);
extern void ProgPoints(void);

extern void PrintChipCard(BYTE pSel);
extern BYTE QueryICBlock(unsigned long pICCardNO);
extern void icBCD2EcrBCD(BCD *ecrBCDTo,char *icBCDFr,BYTE pLenFr);

#if(CASE_MFRIC==1)
extern BYTE MFRToIC(int pSel);
extern void ICToMFR(int pSel);
#endif

//#endif



/*******************************************************************************
Function Prototypes
*******************************************************************************/
extern WORD FlashRead( unsigned int ulOff );

extern BYTE Sector_Erase_OP( unsigned int Addr, WORD SC );
#define FlashBlockErase(sectAddr,blocks) Sector_Erase_OP(sectAddr, blocks)

extern BYTE NorFlash_WriteBuffer(WORD* pBuffer, unsigned int WriteAddr, unsigned int NumHalfwordToWrite);
#define FlashProgram(ulOff, numWords, Array ) NorFlash_WriteBuffer(Array,ulOff,numWords)

extern void PROGRAMEND(void);
#if(defined(CASE_GPRS))
extern void ProgGprsUpdateECR(void);
extern BYTE EndGprs(void);
extern BYTE StartGprs(void);
extern BYTE ASC_2_HEX(BYTE pASC);
extern void GPRSSendHEX(BYTE byte);  //send data one byte, such as  SOH and so on..
extern BYTE GPRSSkipReceived(ULONG mWaitFor);
extern short GPRSReadAString(BYTE *str,short size,ULONG sWaitFor);
// 直接从串口读取GPRS返回的数据 %IPDATA......//
extern short GPRSRead_IPDATA(char *pBuff,short size,ULONG sWaitFor);
// 通过AT%IPSEND 发送数据 //

extern BYTE GPRSConnectAPN(void);
extern BYTE GPRSRegisterTCPIP(void);
extern BYTE GPRSConnect_TCPIP(/*CONSTCHAR *pIP,CONSTCHAR *pPort*/);
extern BYTE GPRSSIM_Exist(void);
extern BYTE GPRSCloseTCPIP(BYTE sMode);
extern void GPRSSendByte(BYTE byte);
extern void GPRSSendAString(CONST char * str, short len);
#endif

#if (CASE_USBDISK)
extern short U_ResetInit(void);
extern short U_DiskReady(void);
extern short U_OpenFile(char *filename,BYTE opMode);
extern short U_CreateFile(char *filename);
extern int U_ReadFile(char *buf, unsigned int nbyte);
extern short U_CloseFile(void);
extern short U_LocateFile(ULONG locate_addr);
extern short U_DeleteFile(char *filename);
extern int U_WriteFile(const char *buf, unsigned int nbyte);
extern BYTE U_Eof(void);
extern DWORD U_FileSize(void);
#endif

void Beep(BYTE blen);

void CleanKeyboard(void);
//--------------------------------------------------------------------------------------

#if defined(DEBUGBYPC)

#define PRESS_ANY_KEY(yy) do {\
                            LoopInput=1;\
                            DispStrXY(Msg[PRESSANYKEY].str,0,yy);\
                            while (!KbHit())  {};\
                            Getch(); \
                            LoopInput=0;\
                          } while (0)

#define WAIT_INPUT(key)   do {\
                            LoopInput=1;\
                            while (!KbHit()){};\
                            key=Getch();\
                            LoopInput=0;\
                          } while (0)

#define SECONDS(s)      (1000/5*s)
#else

#define PRESS_ANY_KEY(yy) do {\
                            DispStrXY(Msg[PRESSANYKEY].str,0,yy);\
                            while (!KbHit())  {};\
                            Getch();\
                          } while (0)

#define WAIT_INPUT(key)   do {\
                            while (!KbHit()){};\
                            key=Getch();\
                          } while (0)

#define SECONDS(s)      (1000L*s)
#endif
//ccr2016-01-18>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
extern BYTE PosOfFuncKey(WORD funcKey);
extern BYTE AppendEntry(char chKey);

//从组合协议中提取串口速率序号(1.2.3....)
#define DeCodeRate(In) (((In) & 0x07)+1)		//bit0..bit2:Speed
//根据速率序号(1.2.3....),生成组合协议,且固定为8Bit,1Stop,无校验
#define EnCodeRate(rateID) (((rateID-1) & 0x07) | ((0)<<3) | ((1)<<5) |   ((1)<<6))

extern void GetPortRate(void *val);
extern void GetDevicePort(BYTE pType,void *val);
extern BYTE SelectGraphic(BYTE grpIdx);
extern void ReadKeyDesList(void);
extern void SetKeyboard(void);
extern BYTE WaitForYesNo(const char *pMessage,BYTE pDefault);
extern void MGFunctions(void);
void PrintGraphics(void);
void mDrawABox(BYTE LinFrom,BYTE LinTo);
void ECRFuncCodeToName(WORD fCode,char *fName);
WORD ECRFuncCodeToTypeName(WORD fCode,char *fName);
void ReadNumric(void);
BYTE InputWord(BYTE x,BYTE y,WORD vMax);

#if defined(LCD10_DISP)
void LCD10_SetFlag(BYTE flag);
#endif

BYTE CenterForDisplay(BYTE width,BYTE sLen);
void DispGrandTotal(void);
void DisplayPrompt(char *msg);
void ReadKeyMacro(void);
void ReadFuncLook(void);
WORD SelectKeyCode(char *keyName);
void  ProcessKeyMacro(WORD keyCode);
void  ProcessFuncLook(WORD keyCode);
void GetRecordSize(int type);
void ProgPLUFields(void);
void ProgDEPTFields(void);
BYTE WidthofWORD(WORD pDec);
void ProgPointType(void);
void ProgNewVIP(void);
void ProgEJournal(void);
//ccr2016-04-21>>>>>>>>>>>>>>>>>>>
#if defined(STM32F10X_HD_USBCOMM)
short ReadUSBRecord(void);
BYTE SendUSBRecord(BYTE *p ,BYTE length );

#define SendToHost(c) {\
                        if (FromUSB)\
                            SendUSB(c);\
                        else\
                            SendComm(COMPUTER_1,c);\
                       }
#else

#define SendToHost(c)  SendComm(COMPUTER_1,c)

#endif
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#if defined(CASE_INDIA)
extern void SavePlu_DeptEditLOG(void);
#endif

#if defined(CASE_PERU)//ccr2016-10-24>>>>>>>>>>>>>>>
void PrintCustomerMessage(void);
#endif//ccr2016-10-24<<<<<<<<<<<<<<<<<

WORD ReadRecIndex(WORD *idx,BYTE b_w);
void IncRecIndex(WORD *idx,BYTE b_w);
void SetRecIndex(WORD *idx,WORD val,BYTE b_w);

void PrintXZMenu(BYTE xz);
void PrintMGMenu(void);

#endif//#ifndef EXTH_H

