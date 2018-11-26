/************************************************************************
*																		*
*	file name	: bios.h :definition of constants for BIOS				*
*																		*
*																		*
************************************************************************/

#ifndef BIOS_H
#define BIOS_H

#include "bios_dir.h"	//direttive di compilazione del BIOS

//Mode of ECR

#if defined(DD_LOCK)

#define     XX     0x89		//8--c9
#define     ZZ	    0x8a		//9--ca
#define     OFF    0x83		//0--c3
#define     RG	    0x84		//1--c4
#define     MG	    0x86		//3--c6
#define     SET		0x87		//4--c7
//#define     TR		0x88		//5--c8
#define     NOCLERK 128
//锁的钥匙最后位置的键值
extern BYTE GetchLockVal(void);
#else

#define     SET     71
#define     ZZ	    70  //Z
#define     OFF     66
#define     RG	    67
#define     XX	    68
#define     MG	    69
#define     NOCLERK 72

#endif

#ifndef frmmainH //ccr2016-11-18>>>>>>>>>>>>>
#define X   XX
#define Z   ZZ
#endif //frmmainH ccr2016-11-18<<<<<


// Definizione di alcune costanti che indicano "limiti"

#define	MAX_GRAF_TEXTS	20				//Max records testi programmabili per grafica con testi sovrapposti

#ifndef  CONSTCHAR
#define CONSTCHAR  char
#endif

#ifndef  CONSTBYTE
#define CONSTBYTE  BYTE
#endif

#ifndef  CONSTWORD
#define CONSTWORD  WORD
#endif

#ifndef  BOOL
#define BOOL     bool
#endif


// Definzioni di costanti varie
#ifndef  TRUE
#define TRUE	1
#endif

#ifndef  FALSE
#define FALSE   0
#endif

#ifndef  true
#define true	1
#endif

#ifndef  false
#define false   0
#endif

//==========================================================================
//Devices constants
//==========================================================================


#if defined(LCD10_DISP)//为10字符带状态标签显示屏
#define     DISLEN 	10	 /*ccr exclusive decimal point */
#else
#define     DISLEN 	12	 /*ccr exclusive decimal point */
#endif


#if defined(CASE_INNERDOTPRN)
#define     PRTLEN  42		//ccr
#define pSHIFTx     6

#if(defined(DEBUGBYPC))
#define DOTPRNPORT COMPUTER_1
#else
#define DOTPRNPORT 2   //2内部串口
#endif
#define TestPrintGoingOn() 0

#else

#if	(DD_S1245 || DD_Z245 || DD_FTP628)
#define	    PRTLEN  32		//ccr
#elif	(DD_S2342)
#define     PRTLEN  20		//ccr
#else
#define     PRTLEN  36		//ccr
#endif
#define pSHIFTx     0

#endif
//--------------------------------------------------------
//Constants for KEYBOARD
//--------------------------------------------------------

#define KEY_MAXROW	8	//Numero max di righe gestite dallo slave
#define KEY_MAXCOL	8	//Numero max di colonne gestite dallo slave
#define KEY_MAXKEYS	64	//Max numero di tasti possibili (incroci riga/colonna)
//---------------------------------------------------------------------------


// =========================================================================
// Definizione dei comandi peri il BIOS
// =========================================================================

#define BiosCmd_PopLM0A 	0	//Slave-0, Output to LCDM Up row, PopUp mode
#define BiosCmd_OutLM0A		1	//Slave-0, Output to LCDM Up row
#define BiosCmd_RefrLM0A	2	//Slave-0, Re-display LCDM Up row buffer
#define BiosCmd_ClrLM0A		3	//Slave-0, Clear and Re-display LCDM Up row buffer
#define BiosCmd_PopLM0B		4	//Slave-0, Output to LCDM Down row, PopUp mode
#define BiosCmd_OutLM0B		5	//Slave-0, Output to LCDM Down row
#define BiosCmd_RefrLM0B	6	//Slave-0, Re-display LCDM Down row buffer
#define BiosCmd_ClrLM0B		7	//Slave-0, Clear and Re-display LCDM down row buffer
#define BiosCmd_ZeroLM0B	8	//Slave-0, Output "0" to LCDM down row

#define BiosCmd_FlagFip0	9	//Slave-0, Attiva/Disattiva  Blink/Punto/Caption sui digits del fip
#define BiosCmd_WrFlafFip0	10	//Slave-0, Trasmette allo slave il buffer flags_fip
#define BiosCmd_RdFlafFip0	11	//Slave-0, Legge dallo slave il buffer flags_fip
#define BiosCmd_Out1Fip0	12	//Slave-0, Carica solo un determinato digit sul fip
#define BiosCmd_OutFip0		13	//Slave 0, Output to FIP
#define BiosCmd_RefrFip0	14	//Slave 0, Refresh FIP Buffer
#define BiosCmd_ClrFip0		15	//Slave 0, Blank FIP
#define BiosCmd_ZeroFip0	16	//Slave 0, Output "0" to FIP
#define BiosCmd_PopFip0		17	//Slave 0, Output to FIP, PopUp mode

#define BiosCmd_SaveDisp0	18	//Slave 0, Salva display per PopUp
#define BiosCmd_RestDisp0	19	//Slave 0, Restore display dopo PopUp

#define BiosCmd_KeyReset	20	//Slave-0, Reset buffer tastiera, annulanndo eventuali tasti esistenti
#define BiosCmd_KeyOff		21	//Slave-0, Disable Keyboard
#define BiosCmd_KeyOn		22	//Slave-0, Enable Keyboard
#define BiosCmd_AutoClickOff 23	//Slave-0, Disable AutoClick
#define BiosCmd_AutoClickOn	24	//Slave-0, Enable AutoClick
#define BiosCmd_BuzzS		25	//Slave-0, Buzzer short
#define BiosCmd_BuzzM		26	//Slave-0, Buzzer medium
#define BiosCmd_BuzzL		27	//Slave-0, Buzzer long
#define BiosCmd_BuzzOff		28	//Slave-0, Buzzer off
#define BiosCmd_CheckKeyborad		29	//Slave-0, Read keyboard

#define BiosCmd_PopLM1A		30	//Slave-1, Output to LCDM Up row, PopUp mode
#define BiosCmd_OutLM1A		31	//Slave-1, Output to LCDM Up row
#define BiosCmd_RefrLM1A	32	//Slave-1, Re-display LCDM Up row buffer
#define BiosCmd_ClrLM1A		33	//Slave-1, Clear and Re-display LCDM Up row buffer
#define BiosCmd_PopLM1B		34	//Slave-1, Output to LCDM Down row, PopUp mode
#define BiosCmd_OutLM1B		35	//Slave-1, Output to LCDM Down row
#define BiosCmd_RefrLM1B	36	//Slave-1, Re-display LCDM Down row buffer
#define BiosCmd_ClrLM1B		37	//Slave-1, Clear and Re-display LCDM down row buffer
#define BiosCmd_ZeroLM1B	38	//Slave-1, Output "0" to LCDM down row

#define BiosCmd_FlagFip1	39	//Slave-1, Attiva/Disattiva  Blink/Punto/Caption sui digits del fip
#define BiosCmd_WrFlafFip1	40	//Slave-1, Trasmette allo slave il buffer flags_fip
#define BiosCmd_RdFlafFip1	41	//Slave-1, Legge dallo slave il buffer flags_fip
#define BiosCmd_Out1Fip1	42	//Slave-1, Carica solo un determinato digit sul fip
#define BiosCmd_OutFip1		43	//Slave 1, Output to FIP
#define BiosCmd_RefrFip1	44	//Slave 1, Refresh FIP Buffer
#define BiosCmd_ClrFip1		45	//Slave 1, Blank FIP
#define BiosCmd_ZeroFip1	46	//Slave 1, Output "0" to FIP
#define BiosCmd_PopFip1		47	//Slave 1, Output to FIP, PopUp mode

#define BiosCmd_SaveDisp1	48	//Slave 1, Salva display per PopUp
#define BiosCmd_RestDisp1	49	//Slave 1, Restore display dopo PopUp

//ccr 040526 for chipcard
#define BiosCmd_CC_Open      50
#define BiosCmd_CC_Close     51
#define BiosCmd_CC_Read      52
#define BiosCmd_CC_Compare   53
#define BiosCmd_CC_Write     54
#define BiosCmd_CC_VerifyPsw 55
#define BiosCmd_CC_WritePsw  56


#define BiosCmd_PrintRestart 60	//Riattiva/Abort processo di stampa
#define	BiosCmd_PrintCheck	61	//Torna stato corrente sensori stampanti
#define BiosCmd_PrintWaitEnd 62	//Attende stampanti RJ a riposo
#define BiosCmd_ResetCutter	63	//Sblocca la taglierina
#define BiosCmd_PrintGraph	68	//Comando di stampa di un disegno
#define BiosCmd_PrintDirect	69	//Comando di stampa diretti, con attesa fine
#define BiosCmd_PrintX		70	//Comando di stampa R2=opz.stampa  R3=#dot-line blank post extra

#define BiosCmd_SepFip0		71	//Gestione separatori migliaia su fip-0 (DD_NEWPUNTI)
#define BiosCmd_SepFip1		72	//Gestione separatori migliaia su fip-1 (DD_NEWPUNTI)
#define BiosCmd_KeySetMask	73	//Slave-0 : set key-mask table
#define BiosCmd_CursLM0		75 	//74	//Slave-0 : set/reset cursor su LCDM
#define BiosCmd_CursLM1		75	//Slave-1 : set/reset cursor su LCDM
#define BiosCmd_Save2Disp0	76	//Slave 0, Salva display per gest.chiave off
#define BiosCmd_Rest2Disp0	77	//Slave 0, Restore display per gestione chiave off
#define BiosCmd_SlavesInit	78	//Slaves 0 and 1 : Restart and set-up


#define BiosCmd_GetDate		80	//RTC: read date
#define BiosCmd_GetTime		81	//RTC: read time
#define BiosCmd_SetDate		82	//RTC: set date
#define BiosCmd_SetTime		83	//RTC: set time
#define BiosCmd_RtcInit		84	//RTC: init

#define BiosCmd_Refresh		85	//Refresh
#define	BiosCmd_ScannerOn	86	//Abilita lo scanner, se questo esiste su una porta
#define	BiosCmd_ScannerOff	87	//Disabilita lo scanner, se questo esiste su una porta
//-----------------------------------------------------------------------------------



//cc 2006-08-29 for MMC>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// define Comandi per l'MMC :
// --------------------------

#define	BiosCmd_SD_Init			92	//Inizializza MMC
#define	BiosCmd_SD_ReadBlock		93	//Raead block
#define	BiosCmd_SD_ReadInfo		94	//Read CID/CSD/PROT/STATUS
#define	BiosCmd_SD_WriteBlock	    95	//Write block
#define	BiosCmd_SD_WriteInfo		96	//Wite CID/CSD/ ecc

//#define	BiosCmd_SD_WriteCSD		97	//Write CSD
#define	BiosCmd_SD_Erase			97	//Erase groups
#define	BiosCmd_SD_Protect		    98	//Protect group
#define	BiosCmd_SD_UnProtect		99	//UnProtect group
#define	BiosCmd_SD_ReadProtMask    100	//Legge mask protezione gruppi
#define BiosCmd_SD_CheckPresence   101 //cc 20060426????????????????
//cc 2006-08-29 for MMC>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// ======================================================================
// EQU comandi di stampa :
// ======================================================================
/////////// bit Command of print:ApplVar.Prn_Command ///////////////////////////////////////
#define PRN_DBLW_Mode        BIT0    //double with print mode for text
#define PRN_DBLH_Mode        BIT1    //double high print mode for text
#define PRN_RECEIPT_Mode     BIT2    //print on receipt printer
#define PRN_STUB_Mode        BIT3    //print on stub printer
#define PRN_GRAPHIC_Mode     BIT4    //print a graphic
#define PRN_CUTF_Mode        BIT5    //Cut paper full
#define PRN_CUTH_Mode        BIT6    //Cut paper half
#define PRN_CUTRESET_Mode    BIT7    //reset cutter

//20130709>>>>>>
//     Definizione flags di stampa contenute in PRN_FlagI
//     --------------------------------------------------
//

#define	B_PRI_DBLH			1	//    PRN_DBLH_Mode
#define	B_PRI_RON			2	//    PRN_RECEIPT_Mode
#define	B_PRI_JON			3	//    PRN_STUB_Mode
#define	B_PRI_GRAF			4	//    PRN_GRAPHIC_Mode
#define	B_PRI_FCUT			5	//    PRN_CUTF_Mode
#define	B_PRI_PCUT			6	//    PRN_CUTH_Mode

/////////////////////////////////////////////////////////////
#define	B_PRI_GRAFV  		8	//bit 6 : stampa grafica vettorizzata
#define	B_PRI_NOSPC			9	//bit 8 : non fa spaziatura tra le righe
#define	B_PRI_NOPF			10	//bit 9 : ignora errori e abort pwfl
#define	B_PRI_DIRECT		11	//bit 10: speciale per PrintDirect
#define	B_PRI_BURN			12	//bit 2 : il comando prevede stampa

//======================================================================
// EQU comandi di stampa :
//======================================================================

//#define	CMDP_RJ		((1 << B_PRI_RON) | (1 << B_PRI_JON))//     Stampa normale R+J
//#define	CMDP_J		((1 << B_PRI_JON))// Stampa normale solo J :
//#define	CMDP_DRJ	((1 << B_PRI_RON) | (1 << B_PRI_JON) | (1 << B_PRI_DBLH))// Stampa doppia altezza R+J :
//#define	CMDP_DJ		((1 << B_PRI_JON) | (1 << B_PRI_DBLH))// Stampa doppia altezza solo J :
//#define	CMDP_LFRJ	((1 << B_PRI_RON) | (1 << B_PRI_JON))// Stampa line feed R+J :
//#define	CMDP_LFJ	((1 << B_PRI_JON))// Stampa line feed solo J :
//#define	CMDP_JFEED	((1 << B_PRI_JON))// Speciale per avanzamento carta  J :

#define	CMDP_R		((1 << B_PRI_RON))// Stampa normale solo R :
#define	CMDP_DR		((1 << B_PRI_RON) | (1 << B_PRI_DBLH))// Stampa doppia altezza solo R :
#define	CMDP_LFR	((1 << B_PRI_RON))// Stampa line feed solo R :
#define	CMDP_PRGRA	((1 << B_PRI_RON) | (1 << B_PRI_GRAF))// Stampa dot-line grafica solo R :
//#define	CMDP_RFEED	((1 << B_PRI_RON))// Speciale per avanzamento carta  R :
//<<<<<<<<<<
// =============================================================================
// Definizioni relative al font di caratteri
// =============================================================================
//
// =============================================================================
// Funzioni assembler del bios e definizione di tipi:
// =============================================================================


#define BIOS_TICMS	5		//Valore in millisecondi del TIC base (timer A0)

#ifndef frmmainH  //ccr2016-11-18>>>>>>>>>
extern WORD Bios(WORD cmd, void *ptr, WORD par1, WORD par2);	//Accesso al Bios
extern WORD Bios_1(WORD cmd);										//Accesso al Bios
extern WORD Bios_2(WORD cmd, void *ptr);						//Accesso al Bios

//ccr 040526 for chipcard
#define CC_Open(BuffATR) Bios_2(BiosCmd_CC_Open, BuffATR)
#define CC_Close() Bios_1(BiosCmd_CC_Close)
#define CC_Read(Dest, ChipAdr, NumBytes) Bios(BiosCmd_CC_Read,Dest, ChipAdr, NumBytes)
#define CC_Compare(Source, ChipAdr,NumBytes)  Bios(BiosCmd_CC_Compare, Source, ChipAdr,NumBytes)
#define CC_Write(Source, ChipAdr,NumBytes)  Bios(BiosCmd_CC_Write,Source, ChipAdr,NumBytes)
#define CC_VerifyPWD(Password) Bios_2(BiosCmd_CC_VerifyPsw, Password)
#define CC_WritePWD(Password) Bios_2(BiosCmd_CC_WritePsw,Password)

//Legge Num bytes da FM :
//
// Torna : 1 = OK
//         0 = NON-OK (errore in Bios_LastError)
//
extern WORD Bios_FM_Read(void *Dest, ULONG SrcAdr, WORD Num);

//Scrive Num bytes in FM: torna #bytes scritti correttamente
//
// Torna : Numero di bytes scritti correttamente
//
extern WORD Bios_FM_Write(ULONG DestAdr, void *Src, WORD Num);

// Bios_InitSerialPort() : Inizializza porta seriale
//
// Torna : 1 = OK
//         0 = NON-OK (errore in Bios_LastError)
//
extern WORD Bios_PortInit(BYTE NumCom);

// Bios_PortWrite() :
// trasmette NumBytes bytes a partire da *TxData sulla porta NumCom
// Flags :  bit 0 : 0=non attende completamento   1 = attende completamento
//
// Torna : 1 = OK
//         0 = NON-OK (errore in Bios_LastError)
//
extern WORD Bios_PortWrite(BYTE NumCom, void *TxData, WORD NumBytes, BYTE Flags);

// Bios_PortRead() :
//  Ricezione dalla porta NumCom
//  NumBytes : 0 = torna bytes pendenti/Status senza leggere
//            >0 = leggi max NumBytes caratteri con time-out TimeOut(unit\xE0 5 millisecondi)
//            <0 = leggi tutti i caratteri pendenti
//
// Torna : Numero di bytes letti (o pendenti se NumBytes=0)
//         *Status  =  bit-flags per gestione errori :
//                     bit 15 : 1 = uno o pi\xF9 errori presenti
//                     bit 14 : 1 = parity error
//                     bit 13 : 1 = frame error
//                     bit 12 : 1 = overrun error
//                     bit 11 : 1 = overflow su buffer interno di ricezione
//                     bit 10 : 1 = superato time-out
//                     bit 9  : 1 = avvenito power-fail durante la ricezione

extern short Bios_PortRead(BYTE NumCom, void *Dest, short NumBytes, ULONG TimeOut, WORD *Status);

// Bios_Delay()
//
extern void Bios_Delay(WORD MilliSec);

// Funzioni BIOS per SET/GET di propieta varie:
// --------------------------------------------
//
// Bios_SetKeyboard : Setta la tastiera in uso
extern WORD Bios_SetKeyboard(BYTE Code, WORD NumKeys, CONSTBYTE *TabPnt, BYTE TabRecLen);
// Bios_SetCutter : Ailita/disabilita la taglierina
extern WORD Bios_SetCutter(BYTE Stato);


// Bios_SetPort : Configura una porta seriale (ATTENZIONE : NON APRE LA PORTA)
extern WORD Bios_SetPort(BYTE NumCom,  char *SettingString, BYTE Stato);

// Bios_SetTic : Abilita la call-back BiosCallBack_TIC() che verra' chiamata ogno Interval millisecondi
extern WORD Bios_SetTic(WORD Interval);


// Bios_SetPortBusy() : Attiva/Disattiva speciale modalit\xE0 "busy"
//Ccr // Bios_SetPortBusy() : Attiva/Disattiva speciale modalit?"busy"
//   Stato : 0 = disattiva modo busy    1=attiva modo busy
//   ChrRx : codice ascii del carattere da filtrare (ENQ)
//   ChrTx : codice ascii del carattere da trasmettere in risposta
//
// Torna : 1 = OK
//         0 = NON-OK (errore in Bios_LastError)
//
extern WORD Bios_SetPortBusy(BYTE NumCom, BYTE Satato, BYTE ChrRx, BYTE ChrTx);


// Bios_TestMAC() : Torna TRUE se MAC-Switch presente
//
extern WORD Bios_TestMAC(void);


//------------------------------------------------------------------------------

// =============================================================================
// Riferimenti alle variabili del bios
// =============================================================================


extern DATE_BCD rtc_date;			//buffer per get/set data corrente
extern TIME_BCD rtc_time;			//buffer per get/set orario corrente

extern BYTE Bios_Key0_Key;			//key number: 00 = No key



// =============================================================================
// Codici di errori del BIOS (Passati nella variabile Bios_LastError)
// =============================================================================
//
//
#define BIOSERR_NO_COM		 	0x0001			//la porta seriale specificata non esiste
#define BIOSERR_COM_NOTSET	0x0002			//la porta seriale non e' configurata
#define BIOSERR_COM_CONF	 	0x0003			//parametri configurazione porta seriale errati
#define BIOSERR_COM_NOTON	 	0x0004			//porta non aperta / disabilitata

#define BIOSERR_COM_SIZE		0x0005

//ccr040526 for chipcard
#define BIOSERR_CC_NOCARD	    0x0006
#define BIOSERR_CC_UNSUPP     0x0007
#define BIOSERR_CC_PSW        0x0008
#define BIOSERR_CC_TMOUT      0x0009
#define BIOSERR_CC_COMPARE    0x000a


//
// ----------------------------------------------------------------------------

// Port1Write() :
// trasmette NumBytes bytes a partire da *TxData sulla porta NumCom
// Flags :  bit 0 : 0=non attende completamento   1 = attende completamento
//
// Torna : 1 = OK
//         0 = NON-OK (errore in Bios_LastError)
//
void Port1Write(void *TxData, WORD NumBytes, BYTE Flags);


// Port2Read() :
//  Ricezione dalla porta NumCom
//  NumBytes : 0 = torna bytes pendenti/Status senza leggere
//            >0 = leggi max NumBytes caratteri con time-out TimeOut(unit\xE0 5 millisecondi)
//            <0 = leggi tutti i caratteri pendenti
//
// Torna : Numero di bytes letti (o pendenti se NumBytes=0)
//         *Status  =  bit-flags per gestione errori :
//                     bit 15 : 1 = uno o pi\xF9 errori presenti
//                     bit 14 : 1 = parity error
//                     bit 13 : 1 = frame error
//                     bit 12 : 1 = overrun error
//                     bit 11 : 1 = overflow su buffer interno di ricezione
//                     bit 10 : 1 = superato time-out
//                     bit 9  : 1 = avvenito power-fail durante la ricezione

void Port1Read(void *Dest, short NumBytes, WORD TimeOut, WORD *Status);


//void BiosUpdate(WORD TimeOut);
//#pragma INTCALL 34 BiosUpdate(R0)
void Write_128Clib(char *frR2R0,unsigned long toA1A0);

void Write64KFlash(char *Dest,unsigned long From);

void Write_128bytes(char *frR2R0,unsigned long toA1A0,WORD Bytes);

//void EraseChip(unsigned long toA1A0);
//#pragma INTCALL 37 EraseChip(A1A0)

void EraseSector(char *SectorA1A0);

extern WORD CC_Insert(void);	//ccr040526 chipcard
//#pragma PARAMETER CC_Insert(void)

// define Comandi per l'MMC :
// --------------------------

#define	MMC_GO_IDLE_STATE				0x40	//CMD00 = 0x00
#define	MMC_SEND_OP_COND				0x41	//CMD01 = 0x01
#define	MMC_ALL_SEND_CID     			0x42	//CMD02
#define	MMC_SET_RELATIVE_ADDR		0x43    //CMD03
#define	MMC_SET_DSR          			0x44    //CMD04
#define	MMC_SELECT_DESELECT_CARD		0x47    //CMD07
#define	MMC_SEND_CSD					0x49    //CMD09
#define	MMC_SEND_CID					0x4a    //CMD10
#define	MMC_READ_DAT_UNTIL_STOP		0x4b    //CMD11
#define	MMC_STOP_TRANSMISSION		0x4c    //CMD12
#define	MMC_SEND_STATUS        		0x4d    //CMD13
#define	MMC_SET_BUS_WIDTH_REGISTER  0x4e    //CMD14
#define	MMC_GO_INACTIVE_STATE    	0x4f    //CMD15
#define	MMC_SET_BLOCKLEN          	0x50    //CMD16
#define	MMC_READ_BLOCK            	0x51    //CMD17
#define	MMC_READ_MULTIPLE_BLOCK  	0x52    //CMD18
#define	MMC_WRITE_DAT_UNTIL_STOP 	0x54    //CMD20
#define	MMC_WRITE_BLOCK           	0x58    //CMD24
#define	MMC_WRITE_MULTIPLE_BLOCK 	0x59    //CMD25
#define	MMC_PROGRAM_CID           	0x5a    //CMD26
#define	MMC_PROGRAM_CSD           	0x5b    //CMD27
#define	MMC_SET_WRITE_PROT        	0x5c    //CMD28
#define	MMC_CLR_WRITE_PROT        	0x5d    //CMD29
#define	MMC_SEND_WRITE_PROT       	0x5e    //CMD30
#define	MMC_TAG_SECTOR_START      	0x60    //CMD32
#define	MMC_TAG_SECTOR_END        	0x61    //CMD33
#define	MMC_UNTAG_SECTOR           	0x62    //CMD34
#define	MMC_TAG_ERASE_GROUP_START 	0x63    //CMD35
#define	MMC_TAG_ERASE_GROUP_END   	0x64    //CMD36
#define	MMC_UNTAG_ERASE_GROUP     	0x65    //CMD37
#define	MMC_ERASE_SECTORS         	0x66    //CMD38
#define	MMC_READ_OCR					0x7A	//CMD58
#define	MMC_CRC_ON_OFF				0x7B	//CMD59

#if !defined(LCD10_DISP)
#define LCD10_SetFlag(a)  {}
#endif

/** @addtogroup USBD_USER
  * @{
  */
#define USB_NONE        0   //ccr USB上无设备
#define USB_DISK        1   //ccr USB上为盘
#define USB_KEYBOARD    2   //ccr USB上为键盘,条码枪


#endif //frmmainH//ccr2016-11-18<<<<<<<<<

#if((defined(CASE_EJSD) || defined(CASE_EJFLASH)) && defined(DEBUGBYPC))
#define MMC_ReadBlock(buf,addr) 	Bios(BiosCmd_SD_ReadBlock, buf,(WORD)addr,  (addr)>>16)
#define MMC_WriteBlock(buf,addr) Bios(BiosCmd_SD_WriteBlock, buf,(WORD)addr,  (addr)>>16)
//ccr070718 #define MMC_CheckPresence() Bios(BiosCmd_SD_CheckPresence, 0, 0, 0)
#endif


#endif
