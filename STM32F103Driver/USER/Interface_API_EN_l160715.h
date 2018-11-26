/***************************************************************************
   The file is API intoduction file for developing interface. It can be used by including it in your application.
   You can fulfill the function you need by calling the related function API.
  1.System function1 API definition
  2.Printing API  definition
  3.Serial port API definition
  4.System function2 API definition
  5.SD card API definition
  6.Dispay API definition
  7.Fiscal memory API
  8.Flash memory API
  9.Keyboard layout reading  API
  10.USB conmunication API
****************************************************************************/

/**
 *data type definition
 */
#define BYTE 	unsigned char
#define WORD 	unsigned short
#define ULONG 	unsigned long
#define DWORD 	unsigned long


//---- same as stm32f10x.h ------------
#define uint8_t    unsigned char
#define uint16_t   unsigned short
#define uint32_t   unsigned long

//
#define BIT0	0x01
#define BIT1	0x02
#define BIT2	0x04
#define BIT3	0x08
#define BIT4	0x10
#define BIT5	0x20
#define BIT6	0x40
#define BIT7	0x80

#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000

#define BIT16	0x010000l
#define BIT17	0x020000l
#define BIT18	0x040000l
#define BIT19	0x080000l
#define BIT20	0x100000l
#define BIT21	0x200000l
#define BIT22	0x400000l
#define BIT23	0x800000l

#define BIT24	0x01000000l
#define BIT25	0x02000000l
#define BIT26	0x04000000l
#define BIT27	0x08000000l
#define BIT28	0x10000000l
#define BIT29	0x20000000l
#define BIT30	0x40000000l
#define BIT31	0x80000000l

//type define for date (BCD, 4 bytes) :
typedef struct{
	 	BYTE dd;	//Day
	 	BYTE mm;	//Month
	 	BYTE yy;	//Year
	 	BYTE w;		//week day
} DATE_BCD;


//type define for time (BCD, 4 bytes) :
typedef struct{
	 	BYTE hh;	//Hour
	 	BYTE mm;	//minute
	 	BYTE ss;	//second
	 	BYTE w;   	//dummy : used to have even align
} TIME_BCD;

//-----------------------

extern DATE_BCD rtc_date;          //buffer for get/set date current
extern TIME_BCD rtc_time;          //buffer for get/set time current

//-------------------------------------------------------------------
// the defined command list related to API Bios

#define BiosCmd_ClrFip0		    15	//Close LED
#define BiosCmd_AutoClickOff    23	//Disable AutoClick
#define BiosCmd_AutoClickOn	    24	//Enable AutoClick
#define BiosCmd_BuzzS		    25	//Buzzer short
#define BiosCmd_BuzzM		    26	//Buzzer medium
#define BiosCmd_BuzzL		    27	//Buzzer long
#define BiosCmd_CheckKeyborad	29	//check keyboard


#define	BiosCmd_PrintCheck	    61	//Check the state of printer
#define BiosCmd_ResetCutter	    63	//Reset the paper cutter
#define BiosCmd_PrintGraph	    68	//Print graphic
#define BiosCmd_PrintX		    70	//Print string


#define BiosCmd_GetDate		    80	//RTC: read date
#define BiosCmd_GetTime		    81	//RTC: read time
#define BiosCmd_SetDate		    82	//RTC: set date
#define BiosCmd_SetTime		    83	//RTC: set time

//-----------------------------------------------------------------------------------

//for SD card>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define	BiosCmd_SD_Init			92	//Initializ MMC
#define	BiosCmd_SD_ReadBlock	93	//Raead block
#define	BiosCmd_SD_ReadInfo		94	//Read CID/CSD/PROT/STATUS
#define	BiosCmd_SD_WriteBlock	95	//Write block
#define	BiosCmd_SD_Erase		97	//Erase groups
#define BiosCmd_SD_CheckPresence 101 //Check the presence of SD card

//--------------------1.system function1 API definition------------------------------ 

/** STM32_Initial
 *  Initial the system      
 *  1.The tick timer SysTick interupts every 1ms.
 *  2.Initial all serial ports with 115200/8/1/n
 *  3.Initial port of hardware with HW_GPIOInit();
 *  4.Initial memory
 *  5.Initial display
 *  6.Initial system clock
 *  7.Initial Timer5/Timer6
 *  8.Initial fiscal memory 
 *  9.Initial SD card.
 *  10.Initial USB function
 */
void STM32_Initial(void);

/** Bios**************************************************************
 *
 * Call system function with multi-parameters
 * @param cmd: command
 * @param ptr: assign parameter of string
 * @param par1: assign parameter1
 * @param par2: assign parameter2
 *
 * @return WORD: true-function successed    false-function failed
 ****************************************************************** */
WORD Bios(WORD cmd, void  *ptr, WORD par1, WORD par2);
/* Command and parameter needed are like below
    BiosCmd_SD_Erase: Erase the data of group which is assigned by GROUP_H*65536+GROUP_L in SD card, the partition of group in SD card refers to related manual.
        Example: Bios(BiosCmd_SD_Erase, (far char*)SourceBuff, (WORD)GROUP_L, (WORD)GROUP_H )
    BiosCmd_SD_ReadInfo: Read CID or CSD in SD card and store it in sBuf.
        Example: Bios(BiosCmd_SD_ReadInfo, (char*)sBuf, MMC_SEND_CID,16)
    BiosCmd_SD_Init: Initial SD card, the initialization of SD card refers to related manual.
        Example: Bios(BiosCmd_SD_Init, (far char*)SourceBuff, 0, 0 )
    BiosCmd_SD_ReadBlock: Read a block of data from the address of addr_H*65536+addr_L in SD card, and store it in buf. A block is 1024 bytes generally.
        Example: Bios(BiosCmd_SD_ReadBlock, buf,addr_L,  addr_H)
    BiosCmd_SD_WriteBlock: Write the block of data in buf down at the adsress of addr_H*65536+addr_L in SD card.A block is 1024 bytes generally.
        Example: Bios(BiosCmd_SD_WriteBlock, buf,addr_L,  addr_H)
    BiosCmd_SD_CheckPresence: Check whether SD card exists.
        Example: Bios(BiosCmd_SD_CheckPresence, 0, 0, 0)
    BiosCmd_PrintGraph: Print the graphic assigned by idx.
        Example: Bios(BiosCmd_PrintGraph, (char*)idx, option, position);
        //idx:      the index of graphic(1-24)
        //option:   bit0  =1, add the words in GrafTexts on the graphic
        //          bit1  =1, add the word attached to graphic on graphic
        //position:  assign the position at which the graphic is printed
    BiosCmd_PrintX: send data to printer,Line is the data to be printed;Cmd is the command for printing;DotLinesBlank reserved parameter 
        Example: Bios(BiosCmd_PrintX, char *Line, WORD Cmd , WORD DotLinesBlank);
        //The description of Cmd is like below:*/     //

// ======================================================================
//   The command for printing
// ======================================================================
/////////// bit Command of printrn_Command ///////////////////////////////////////

#define	B_PRI_DBLH			1	// double with print mode for text   PRN_DBLH_Mode
#define	B_PRI_RON			2	// double high print mode for text   PRN_RECEIPT_Mode
#define	B_PRI_JON			3	// print on stub printer   PRN_STUB_Mode
#define	B_PRI_GRAF			4	// print a graphic   PRN_GRAPHIC_Mode
#define	B_PRI_FCUT			5	// Cut paper full   PRN_CUTF_Mode
#define	B_PRI_PCUT			6	// Cut paper half   PRN_CUTH_Mode

// command for BiosCmd_PrintX:
#define	CMDP_RJ		((1 << B_PRI_RON) | (1 << B_PRI_JON))// Print both on receipt printer and stub printer(Reserved)
#define	CMDP_R		((1 << B_PRI_RON))// Print only on receipt printer
#define	CMDP_J		((1 << B_PRI_JON))// Printer only on stub printer 
#define	CMDP_DRJ	((1 << B_PRI_RON) | (1 << B_PRI_JON) | (1 << B_PRI_DBLH))// Print in double-height way both on receipt printer and stub printer(Reserved)
#define	CMDP_DR		((1 << B_PRI_RON) | (1 << B_PRI_DBLH))// Print in double-height way on receipt printer
#define	CMDP_DJ		((1 << B_PRI_JON) | (1 << B_PRI_DBLH))// Print in double-height way on stub printer (Reserved)
#define	CMDP_PRGRA	((1 << B_PRI_RON) | (1 << B_PRI_GRAF))// Print graphic on receipt printer

//---------------------2. Printing API ---------------------- 

//command definitions for RSPrint
#define PRN_DBLW_Mode        BIT0    //double width print mode for text
#define PRN_DBLH_Mode        BIT1    //double height print mode for text
#define PRN_RECEIPT_Mode     BIT2    //print on receipt printer
#define PRN_STUB_Mode        BIT3    //print on stub printer
#define PRN_GRAPHIC_Mode     BIT4    //print a graphic
#define PRN_CUTF_Mode        BIT5    //Cut paper full
#define PRN_CUTH_Mode        BIT6    //Cut paper half
#define PRN_CUTRESET_Mode    BIT7    //reset cutter

/** RSPrint******************************************************************
*
*   Store the string to be printed in printing queue,and start printer task to print.
*   You can call RSPrint directly in application to fulfill printing rather than BiosCmd_PrintX and BiosCmd_PrintGraph of Bios.
*
*  INPUT:
*       *buf:  Text data for print,cautions:           
*              The string will be printed in double height way when buf[0]is '@';
*              buf[n+1] will be printed in double width way when buf[n] is '~';
*
*       count: Bytes in *buf must be print out
*       prnCMD: Command for TEXT ,must be one of:
*               -PRN_DBLW_Mode        //double width print mode for text
*               -PRN_DBLH_Mode        //double height print mode for text
*               -PRN_RECEIPT_Mode     //print on receipt printer
*               -PRN_STUB_Mode        //print on stub printer
*               -PRN_GRAPHIC_Mode     //print a graphic
*               -PRN_CUTF_Mode        //Cut paper full
*               -PRN_CUTH_Mode        //Cut paper half
*               -PRN_CUTRESET_Mode    //Cutter reset
*
*  return:<0;POOL is full & the data don't push in
*         >=0;data has pushed into the pool and the value is the
*         length of the data
*
* REMARK:
*     if prnCMD is the GRAPHIC command,PRN_GRAPHIC_Mode:
*         buf[0] must be the number of the graphic.(1,2,3....)
*                buf[0]=0,stop attach the text;
*         buf[1] must be '0'(don't attache text)
*               or '1' (8 lines of Text must attache for the graphic)
*               or '2' (copy text from graphic)
*        buf[2] Postion by character of the graphic on paper(0,1,2,..,32), It's not the must,
*               =0,Print at the default position defind by the graphic
*     if prnCMD is the CUT PAPER command:
*         buf[0] must be the FULL(1) cut or HALF cut(0).
*
********************************************************************************/
int RSPrint(BYTE *buf, int count, BYTE prnCMD);

/** Bios_1************************************************************
 *
 * Call system function according to cmd
 *
 * @param cmd : command
 *
 * @return WORD: return the result of function calling,
 *              true- function successed; false- function failed
 ****************************************************************** */
WORD  Bios_1(WORD cmd);
/* Commands are like below:      
     BiosCmd_PrintCheck: read the state of printer: paper out/ bar pulled up/ overheat and so on 
        Example: Bios_1(BiosCmd_PrintCheck);
     BiosCmd_ResetCutter: reset the cutter of printer
        Example: Bios_1(BiosCmd_ResetCutter)
     BiosCmd_RestDisp0: redisplay the content of display, it is usually used to display the content of display stored before shuting down when you restart the machine.
        Example: Bios_1(BiosCmd_RestDisp0);
     BiosCmd_ClrFip0: close the display of LED
     BiosCmd_AutoClickOff: close key sound
     BiosCmd_AutoClickOn: open key sound
     BiosCmd_BuzzL: long beep of buzzer
     BiosCmd_BuzzM: middle beep of buzzer
     BiosCmd_BuzzS: short beep of buzzer
     BiosCmd_GetDate: read the date and store it in rtc_date
     BiosCmd_GetTime: read the time and store it in rtc_time
     BiosCmd_SetDate: set the date according to rtc_date
     BiosCmd_SetTime: set the time according to rtc_time 
     BiosCmd_CheckKeyborad: check whether any key is pressed,but don't read key value. 1- key pressed, 0-no key pressed
*/


//--------------------3. serial port API definition------------------------------ 

/** SetRTS*************************************************************
 *
 * :set the RTS of serial port
 * @param port : 1- serial port 1, 2-serial port 2....
 ****************************************************************** */
void SetRTS(BYTE port);

/** ClrRTS*************************************************************
 *
 * :clear the RTS of serial port
 * @param port:   1- serial port 1, 2-serial port 2.....
 ****************************************************************** */
void ClrRTS(BYTE port);

/** GetCTS*************************************************************
 *
 *  read the state of CTS
 * @param port:  1- serial port 1, 2-serial port 2.....
 * @return BYTE: 0 indicates that the peripheral is in normal state
 ****************************************************************** */
BYTE GetCTS(BYTE port);

/** Bios_PortWrite******************************************************
 *
 * send the data(TxData) which is NumBytes bytes to serial port NumCom.
 *
 * @param NumCom:  1- serial port 1, 2-serial port 2.....
 * @param TxData : the data of characters to be sent.
 * @param NumBytes : the number of characters to be sent
 * @param Flags :reserved
 *
 * @return unsigned short : =true, sending successed; =false, sending failed
 ****************************************************************** */
unsigned short Bios_PortWrite(BYTE NumCom, void  *TxData, unsigned short NumBytes, unsigned char Flags);

/** Bios_PortRead*******************************************************
 *
 *  Read the data which is NumBytes bytes from serialport and store it in Dest. 
 *
 * @param NumCom:   1- serial port 1, 2-serial port 2.....
 * @param Dest : the position stored data read;
 *               when it is 0, check whether there is data in serial port,no waiting if there is no data.
 * @param NumBytes: the number of characters need to be read
 *                 when it is 0,wait for serial port until data is readable; when it is -1, clear the data receive.
 * @param TimeOut : the time delay counted with ms.
 * @param Status :reserved
 *
 * @return short : return the number of character receive actually.
 ****************************************************************** */
short Bios_PortRead(BYTE NumCom, void *Dest, short NumBytes,ULONG TimeOut,unsigned  short *Status);


/**UARTInit***********************************************************
 *
 * @param PortNum :  1- serial port 1, 2-serial port 2.....
 * @param baudrate :1200,2400,4800,9600,19200,38400,57600,115200
 * @param Databits : 8 = 8 bit per carattere;9-9 bit per carattere
 * @param Parity : 0-NONE,1 = ODD,2- EVEN
 * @param Stopbits : 1= 1 stop bit;2-2 stop bit
 *
 * @return uint32_t :true---config success; false---	failure
 ****************************************************************** */
uint32_t UARTInit( uint32_t PortNum, uint32_t baudrate,uint8_t Databits,uint8_t Parity,uint8_t Stopbits );

//--------------------4.system function2 API definition------------------------------

/**Bios_TestMAC******************************************************
 *
 * Check the state of MAC jumper to decide whether to do power-on initialization on machine
 *
 * @return WORD :true-MAC on; false-MAC off
 ****************************************************************** */

WORD Bios_TestMAC(void);


/** Bell,Beep*********************************************************
 *
 *  Open the buzzer according to assigned parameters
 *
 * @param len : set the beep time long/middle/short according to len(1/2/3).     
 * @return short :0
 ****************************************************************** */
short Bell(short len);
void Beep(BYTE len);

/** CutRPaper*********************************************************
 *
 * Cut paper, only work on printers that have cutter.
 *
 * @param lines : the number of lines before cutting
 ****************************************************************** */
void CutRPaper(short lines);

/** DrawerOpen*********************************************************
 *
 * Open drawer
 * 
 ****************************************************************** */
void DrawerOpen(void);

/** msDelay************************************************************
  *
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 ms.
  * @retval None
  *
  ****************************************************************** */
void msDelay(ULONG nTime);

/** usDelay************************************************************
  *
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 us.
  * @retval None
  *
  ****************************************************************** */
void usDelay(ULONG nTime);

//--------------------5.SD card API definition------------------------------ 

/*************************************************************** 
   SD card about: the size of block in SD card is 1024 bytes so the size of data must be 1024 bytes every time writing or reading SD card.
*****************************************************************/

/** MMC_ReadBlock********************************************************
 *
 * Read a block of data from SD card, the size of data must be 1024 bytes every time writing or reading SD card if the block of SD card is 1024 bytes.
 *
 * @param buf : the place data stored, it should be larger than 1024 bytes.
 * @param addr :the data address for reading
 *
 * @return BYTE :true-reading successed ;false-reading failed 
 ****************************************************************** */
BYTE MMC_ReadBlock(char *buf,ULONG addr);

/** MMC_WriteBlock**********************************************************
 * 
 *  Write a block of data in SD card, the size of data must be 1024 bytes every time writing or reading SD card if the block of SD card is 1024 bytes.
 *
 * @param buf : the data to be written,it should be larger than 1024 bytes
 * @param addr : the address data written in
 *
 * @return BYTE: true-writing successed;false-writing failed 
 ****************************************************************** */
BYTE MMC_WriteBlock(char *buf,ULONG addr);

//--------------------6.display API definition------------------------------ 

/**PutsO****************************************************************
 *
 * Display strings on the clerk diaplay and customer display.
 *
 * @param str : the string to be displayed 
 ****************************************************************** */
void PutsO(const char *str);


/**mDispCharXY***********************************************************
 *
 * Diapay the character ch at row y and line x on the screen which is 64x128 lattice
 *
 * @param ch: it can be ASCII or Chinese character
 * @param x:  line position (0~15) or (0~20)
 * @param y:  row position (0~3) or (0~7)
 ****************************************************************** */
void mDispCharXY(WORD ch, BYTE x, BYTE y);


/**mDispStringXY***********************************************************
 *
 * Diapay the character ch at row y and line x on the screen which is 64x128 lattice
 *
 * @param str:  string
 * @param x: line position (0~15) or (0~20)
 * @param y: row position (0~3) or (0~7)
 ****************************************************************** */
void mDispStringXY(BYTE* str, BYTE x, BYTE y);


/**mMoveCursor**************************************************************
 *
 * move cursor         display cursor at row y(0~3) and line x(0~15), cursor align right.
 *
 * @param x
 * @param y
 ***************************************************************************/
void mMoveCursor(uint8_t x, uint8_t y);


/**mDispCursor**************************************************************
 *
 *  Display cursor at the position mMoveCursor assigned    
 *
 *  ******************************************************************** */
void mDispCursor(void);

/**mClearScreen***********************************************************
 *
 * Clear the display
 *
 * ******************************************************************* */
void mClearScreen(void);

//--------------------7.Fiscal memory API------------------------------

/** Bios_FM_CheckPresence,FMDetect**************************************
 *
 * Check whether fiscal memory exists
 *
 * @return BYTE : true-exist;false-does not exist
 ****************************************************************** */
BYTE Bios_FM_CheckPresence(void);
uint8_t FMDetect(void);

/**FiscalMem_ReadBuffer*************************************************
  *
  * @brief  Reads a block of data from the FM.
  * @param  pBuffer: pointer to the buffer that receives the
  *                data read from the FM.
  * @param  ReadAddr: FM's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FM.
  * @retval None
  ****************************************************************** */
uint8_t FiscalMem_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

/**FiscalMem_WriteBuffer*************************************************
  *
  * @brief  Writes block of data to the FM.
  * @param  pBuffer: pointer to the buffer containing the data
  *                to be written to the FM.
  * @param  WriteAddr: FM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FM.
  * @retval None
  ****************************************************************** */
uint8_t FiscalMem_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);


//--------------------8.Flash memory API------------------------------

//definitions for Flash--------
#define FLASH_SUCCESS (-1)
#define FLASH_WRONG_TYPE (-8)
#define FLASH_ERASE_FAIL (-14)

/** bFlashReadBuffer*****************************************************
  *
  * Read the data in flash outside byte by byte.
  *
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  ****************************************************************** */
uint8_t bFlashReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumBytesToRead);

/**bFlashProgram*********************************************************
 *
 * Write data down in flash memory
 *
 * @param ulOff : the address data write to 
 * @param NumBytes : the number of bytes to write to
 * @param ucArrayPointer : the data to write to
 *
 * @return int: FLASH_SUCCESS-writing successed,
 *              FLASH_ERASE_FAIL or other value-writing failed
 ****************************************************************** */
int bFlashProgram( uint32_t ulOff, uint32_t NumBytes,uint8_t *ucArrayPointer );

//
/**bFlashMBlockErase******************************************************
 *
 * Erase Flash memory block
 *
 * @param ucBlock : the index of block to be erased
 * @param ucNumBlocks :  erase ucNumBlocks blocks from block ucBlock continuously
 *
 * @return int : FLASH_SUCCESS-erasing successed,  
 *               FLASH_ERASE_FAIL or other value-erasing failed
 ****************************************************************** */
int bFlashMBlockErase(int ucBlock, int ucNumBlocks);


/**bFlashChipErase*****************************************************
 *
 * The function can be used to erase the whole flash chip so long as
 *  no blocks are protected. If any blocks are protected then
 *  nothing is erased.
 *
 * @return int :On success the function returns FLASH_SUCCESS (-1)
 * If a block is protected then the function returns the number of the block and
 * no blocks are erased.
 * If the erase algorithms fails then the function returns FLASH_ERASE_FAIL (-2)
 * If the wrong type of flash is detected then FLASH_WRONG_TYPE (-8) is
 * returned.
 ****************************************************************** */
int bFlashChipErase( void );

//--------------------9. keyboard layout reading API------------------------------    

/** Getch*************************************************************
  *
  * Read the value of key pressed
  * You should call Bios_1(BiosCmd_CheckKeyborad) to check whether any key is pressed before calling Getch.
  *
  * @param  None
  * @retval :key pressed; 0,1,2,.....
  ****************************************************************** */
uint8_t Getch(void);

//--------------------10.USB communication API---------------------------------     

/**USB_COMM_Ready*****************************************************
 *
 * Check the state of USB when you use USB that simulates serial port to communicate. 
 * you must call USB_COMM_Ready regularly to intercept whether there is data from USB.
 * 
 *
 * @return uint8_t : true-indicates that USB gets the data
 **********************************************************************/
uint8_t USB_COMM_Ready(void);

/**ReadUSB***********************************************************
 *
 * Read the data from USB   
 *
 * @param rCH : it is used to store the data read
 *
 * @return BYTE : =true,indicates there is data ,=false, indicate there is no data
 *
 ***************************************************************** */
BYTE ReadUSB(BYTE *rCH);

/**CheckUSB***********************************************************
 *
 * Check whether USB gets the data
 *
 * @return BYTE : =true,indicates there is data ,=false, indicate there is no data
 *
 ***************************************************************** */
BYTE CheckUSB(void);

/**EmptyUSB***********************************************************
 *
 * Clear the data USB gets
 *
 ***************************************************************** */
void EmptyUSB(void);

/**SendUSB***********************************************************
 *
 * Send a byte of data through USB
 *
 * @param ch : the data to be sent
 *
 ***************************************************************** */
void SendUSB(BYTE ch);

/**SendUSBString******************************************************
 *
 * Send a string through USB
 *
 * @param sStr : the string to be sent
 * @param len :  the length of string
 */
void SendUSBString(BYTE *sStr,short len);
/*Example for use of USB commnunication:
     while (USB_COMM_Ready())//ccr2016-06-24
    {
	    	while (CheckUSB())
	    	{
                if (ReadUSB(&bcc))
                    SendUSB(bcc);
            }
    }
 */

//--------------------END--------------------------------------------
