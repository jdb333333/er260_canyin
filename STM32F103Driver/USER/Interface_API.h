/***************************************************************************
  本文件为开发平台的接口说明文件,在用户应用中可直接包含使用此文件
  调用各个功能API实现所要的功能.
  1.系统功能1 API 定义
  2.打印 API
  3.串口 API 定义
  4.系统功能2 API 定义
  5.SD卡 API 定义
  6.显示 API 定义
  7.Fiscal memory API
  8.Flash memory API
  9.键盘读取 API
  10.USB 通信 API
****************************************************************************/

/**
 * 数据类型定义
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
// 对应API Bios的命令定义表

#define BiosCmd_ClrFip0		    15	//close LED
#define BiosCmd_AutoClickOff    23	//Disable AutoClick
#define BiosCmd_AutoClickOn	    24	//Enable AutoClick
#define BiosCmd_BuzzS		    25	//Buzzer short
#define BiosCmd_BuzzM		    26	//Buzzer medium
#define BiosCmd_BuzzL		    27	//Buzzer long
#define BiosCmd_CheckKeyborad	29	//check keyboard


#define	BiosCmd_PrintCheck	    61	//检测打印机状态
#define BiosCmd_ResetCutter	    63	//复位切刀
#define BiosCmd_PrintGraph	    68	//打印图片
#define BiosCmd_PrintX		    70	//打印字符串


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
#define BiosCmd_SD_CheckPresence 101 //检测SD卡是否插入

//--------------------1.系统功能 API 定义------------------------------

/** STM32_Initial
 *  对系统进行初始化
 *  1.滴答定时器SysTick每1ms中断一次
 *  2.对所有串口初始化为115200/8/1/n
 *  3.对硬件端口初始化:HW_GPIOInit();
 *  4.对存储器进行初始化
 *  5.对显示进行初始化
 *  6.系统时钟初始化
 *  7.Timer5/Timer6初始化
 *  8.税控存储器初始化
 *  9.SD卡初始化
 *  10.USB功能初始化
 */
void STM32_Initial(void);

/** Bios**************************************************************
 *
 * 使用多参数调用系统功能
 * @param cmd:命令
 * @param ptr:指定字符串参数
 * @param par1:指定参数1
 * @param par2:指定参数2
 *
 * @return WORD:true-功能执行成功;false-功能执行失败
 ****************************************************************** */
WORD Bios(WORD cmd, void  *ptr, WORD par1, WORD par2);
/* 命令和需要的参数如下
    BiosCmd_SD_Erase:擦除SD卡中由GROUP_H*65536+GROUP_L所指定的组的数据, SD中组的划分需要参见相关手册
        Example: Bios(BiosCmd_SD_Erase, (far char*)SourceBuff, (WORD)GROUP_L, (WORD)GROUP_H )
    BiosCmd_SD_ReadInfo:读取SD卡的CID或CSD,存入sBuf中
        Example: Bios(BiosCmd_SD_ReadInfo, (char*)sBuf, MMC_SEND_CID,16)
    BiosCmd_SD_Init:初始化SD卡, SD的初始化见相关手册
        Example: Bios(BiosCmd_SD_Init, (far char*)SourceBuff, 0, 0 )
    BiosCmd_SD_ReadBlock:从SD卡中的addr_H*65536+addr_L地址中读取一块数据,存入buf中,一块为1024字节
        Example: Bios(BiosCmd_SD_ReadBlock, buf,addr_L,  addr_H)
    BiosCmd_SD_WriteBlock:把buf中的一块数据写入从SD卡中的addr_h*65536+addr_L地址中,通常一块为1024字节
        Example: Bios(BiosCmd_SD_WriteBlock, buf,addr_L,  addr_H)
    BiosCmd_SD_CheckPresence:检查SD卡是否存在
        Example: Bios(BiosCmd_SD_CheckPresence, 0, 0, 0)
    BiosCmd_PrintGraph:打印由idx所指定的图片
        Example: Bios(BiosCmd_PrintGraph, (char*)idx, option, position);
        //idx:      图片序号(1-24)
        //option:   bit0  =1,将GrafTexts中的文字叠加到图片上
        //          bit1  =1,使用图片自带的文字叠加到图片上
        //position:指定图片打印的字符位置
    BiosCmd_PrintX:向打印机发送打印数据,Line为要打印的数据;Cmd为打印命令;DotLinesBlank reserved parameter
        Example: Bios(BiosCmd_PrintX, char *Line, WORD Cmd , WORD DotLinesBlank);
        //Cmd描述如下:*/

// ======================================================================
// 打印机命令描述数据
// ======================================================================
/////////// bit Command of printrn_Command ///////////////////////////////////////

#define	B_PRI_DBLH			1	// double with print mode for text   PRN_DBLH_Mode
#define	B_PRI_RON			2	// double high print mode for text   PRN_RECEIPT_Mode
#define	B_PRI_JON			3	// print on stub printer   PRN_STUB_Mode
#define	B_PRI_GRAF			4	// print a graphic   PRN_GRAPHIC_Mode
#define	B_PRI_FCUT			5	// Cut paper full   PRN_CUTF_Mode
#define	B_PRI_PCUT			6	// Cut paper half   PRN_CUTH_Mode

// command for BiosCmd_PrintX:
#define	CMDP_RJ		((1 << B_PRI_RON) | (1 << B_PRI_JON))// 同时在收据打印机和存根打印机上打印(Reserved)
#define	CMDP_R		((1 << B_PRI_RON))// 只在收据打印机上打印
#define	CMDP_J		((1 << B_PRI_JON))// 只存根打印机上打印(Reserved)
#define	CMDP_DRJ	((1 << B_PRI_RON) | (1 << B_PRI_JON) | (1 << B_PRI_DBLH))// 同时在收据打印机和存根打印机上倍高打印(Reserved)
#define	CMDP_DR		((1 << B_PRI_RON) | (1 << B_PRI_DBLH))// 收据打印机倍高打印
#define	CMDP_DJ		((1 << B_PRI_JON) | (1 << B_PRI_DBLH))// 存根打印机倍高打印(Reserved)
#define	CMDP_PRGRA	((1 << B_PRI_RON) | (1 << B_PRI_GRAF))// 收据打印机上打印图片

//---------------------2. 打印 API ----------------------

//command definitions for RSPrint
#define PRN_DBLW_Mode        BIT0    //double with print mode for text
#define PRN_DBLH_Mode        BIT1    //double high print mode for text
#define PRN_RECEIPT_Mode     BIT2    //print on receipt printer
#define PRN_STUB_Mode        BIT3    //print on stub printer
#define PRN_GRAPHIC_Mode     BIT4    //print a graphic
#define PRN_CUTF_Mode        BIT5    //Cut paper full
#define PRN_CUTH_Mode        BIT6    //Cut paper half
#define PRN_CUTRESET_Mode    BIT7    //reset cutter

/** RSPrint******************************************************************
*
*   将要打印的字符串存入打印队列并启动打印机任务进行打印.
*   在应用中可以直接调用RSPrint来取代Bios的BiosCmd_PrintX和BiosCmd_PrintGraph实现打印
*
*  INPUT:
*       *buf:  Text data for print,注意:
*              当buf[0]为字符'@'时,字符串将打印成倍高;
*              当buf[n]为字符'~'时,buf[n+1]将打印成倍宽;
*
*       count: Bytes in *buf must be print out
*       prnCMD: Command for TEXT ,must be one of:
*               -PRN_DBLW_Mode        //double with print mode for text
*               -PRN_DBLH_Mode        //double high print mode for text
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
 * 根据cmd调用系统功能
 *
 * @param cmd :命令
 *
 * @return WORD:返回功能调用结果,
 *              true-功能执行成功;false-功能执行失败
 ****************************************************************** */
WORD  Bios_1(WORD cmd);
/* cmd命令如下:
     BiosCmd_PrintCheck:读取打印机的状态:缺纸/压杆抬起/过热等
        Example: Bios_1(BiosCmd_PrintCheck);
     BiosCmd_ResetCutter:复位打印机切刀
        Example: Bios_1(BiosCmd_ResetCutter)
     BiosCmd_RestDisp0:重新显示显示屏幕的内容,通常用于关机后再开机时显示关机前保存的屏幕内容
        Example: Bios_1(BiosCmd_RestDisp0);
     BiosCmd_ClrFip0:关闭LED显示
     BiosCmd_AutoClickOff:关闭按键自动蜂鸣功能
     BiosCmd_AutoClickOn:打开按键自动蜂鸣功能
     BiosCmd_BuzzL:蜂鸣器长鸣一声
     BiosCmd_BuzzM:蜂鸣器中鸣一声
     BiosCmd_BuzzS:蜂鸣器短鸣一声
     BiosCmd_GetDate:读取日期到rtc_date中
     BiosCmd_GetTime:读取时间到rtc_time中
     BiosCmd_SetDate:按照rtc_date中的日期设置日期
     BiosCmd_SetTime:按照rtc_time中的时间设置时间
     BiosCmd_CheckKeyborad:判断是否有按键按下,但是不读取键值。1－有键按下，0－ 无键按下
*/


//--------------------3.串口API定义------------------------------

/** SetRTS*************************************************************
 *
 * :设置串口port的RTS
 * @param port :1-串口1,2-串口2.....
 ****************************************************************** */
void SetRTS(BYTE port);

/** ClrRTS*************************************************************
 *
 * :清除串口的RTS
 * @param port: 1-串口1,2-串口2.....
 ****************************************************************** */
void ClrRTS(BYTE port);

/** GetCTS*************************************************************
 *
 * 读取串口CTS状态
 * @param port: 1-串口1,2-串口2.....
 * @return BYTE:0表示外设处于正常状态
 ****************************************************************** */
BYTE GetCTS(BYTE port);

/** Bios_PortWrite******************************************************
 *
 * 向串口NumCom发送NumBytes字节的数据(TxData)
 *
 * @param NumCom: 1-串口1,2-串口2.....
 * @param TxData :要发送的字符数据
 * @param NumBytes :要发送的字符数
 * @param Flags :reserved
 *
 * @return unsigned short :=true,发送成功;=false,发送失败
 ****************************************************************** */
unsigned short Bios_PortWrite(BYTE NumCom, void  *TxData, unsigned short NumBytes, unsigned char Flags);

/** Bios_PortRead*******************************************************
 *
 * 从串口读取NumBytes字节的数据并存入Dest中;
 *
 * @param NumCom: 1-串口1,2-串口2.....
 * @param Dest :读取数据存放区;
 *              =0时,检测串口是否有数据,无数据时不等待.
 * @param NumBytes:需要读取的字符数目,
 *                =0时,等待串口数据可读; =-1,清空收到的数 据
 * @param TimeOut :等待延迟,以毫秒为计时单位
 * @param Status :reserved
 *
 * @return short :返回实际所读取的字节数
 ****************************************************************** */
short Bios_PortRead(BYTE NumCom, void *Dest, short NumBytes,ULONG TimeOut,unsigned  short *Status);


/**UARTInit***********************************************************
 *
 * @param PortNum :1-串口1,2-串口2.....
 * @param baudrate :1200,2400,4800,9600,19200,38400,57600,115200
 * @param Databits : 8 = 8 bit per carattere;9-9 bit per carattere
 * @param Parity : 0-NONE,1 = ODD,2- EVEN
 * @param Stopbits : 1= 1 stop bit;2-2 stop bit
 *
 * @return uint32_t :true---config success; false---	failure
 ****************************************************************** */
uint32_t UARTInit( uint32_t PortNum, uint32_t baudrate,uint8_t Databits,uint8_t Parity,uint8_t Stopbits );

//--------------------4.系统功能API定义------------------------------

/**Bios_TestMAC******************************************************
 *
 * 测试MAC跳线器状态,用来检测是否要对收款机进行加电初始化
 *
 * @return WORD :true-MAC on; false-MAC off
 ****************************************************************** */

WORD Bios_TestMAC(void);


/** Bell,Beep*********************************************************
 *
 *  根据指定参数打开蜂鸣器
 *
 * @param len :根据len(1/2/3)分别响铃长/中/短时长
 * @return short :0
 ****************************************************************** */
short Bell(short len);
void Beep(BYTE len);

/** CutRPaper*********************************************************
 *
 * 打印机切纸,只对配置了切刀的打印机有效
 *
 * @param lines :切纸前走纸行数
 ****************************************************************** */
void CutRPaper(short lines);

/** DrawerOpen*********************************************************
 *
 * 打开钱箱
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

//--------------------5.SD卡API定义------------------------------

/***************************************************************
  关于SD卡:SD卡的块大小为1024，每次读写SD卡必须是1024字节
*****************************************************************/

/** MMC_ReadBlock********************************************************
 *
 * 从SD卡上读取一块数据,当SD卡的块大小为1024时，每次读写SD卡必须是1024字节
 *
 * @param buf :数据存储区,字节数必须大于等于1024
 * @param addr :读取地址
 *
 * @return BYTE :true-读取成功;false-读取失败
 ****************************************************************** */
BYTE MMC_ReadBlock(char *buf,ULONG addr);

/** MMC_WriteBlock**********************************************************
 *
 * 向SD卡中写入一块数据，当SD卡的块大小为1024时，每次读写SD卡必须是1024字节
 *
 * @param buf :要写入的数据,字节数必须等于1024
 * @param addr :数据写入地址
 *
 * @return BYTE true-写入成功;false-写入失败
 ****************************************************************** */
BYTE MMC_WriteBlock(char *buf,ULONG addr);

//--------------------6.显示API定义------------------------------

/**PutsO****************************************************************
 *
 * 在操作员和顾客显示屏幕上显示字符串
 *
 * @param str :要显示的字符串
 ****************************************************************** */
void PutsO(const char *str);


/**mDispCharXY***********************************************************
 *
 * 在64x128点阵屏幕第y行,第x列字符位置显示字符ch
 *
 * @param ch: 可以是ASCII或者汉字
 * @param x:列位置 (0~15)或(0~20)
 * @param y:行位置 (0~3)或(0~7)
 ****************************************************************** */
void mDispCharXY(WORD ch, BYTE x, BYTE y);


/**mDispStringXY***********************************************************
 *
 * 在64x128点阵屏幕第y行,第x列字符位置显示字符串
 *
 * @param str: 字符串
 * @param x:列位置 (0~15)或(0~20)
 * @param y:行位置 (0~3)或(0~7)
 ****************************************************************** */
void mDispStringXY(BYTE* str, BYTE x, BYTE y);


/**mMoveCursor**************************************************************
 *
 * 移动光标	在第y(0~3)行,第x(0~15)列位置显示光标，光标靠右对齐
 *
 * @param x
 * @param y
 ***************************************************************************/
void mMoveCursor(uint8_t x, uint8_t y);


/**mDispCursor**************************************************************
 *
 *  在mMoveCursor设定的光标位置显示光标
 *
 *  ******************************************************************** */
void mDispCursor(void);

/**mClearScreen***********************************************************
 *
 * 清除屏幕内容
 *
 * ******************************************************************* */
void mClearScreen(void);

//--------------------7.Fiscal memory API------------------------------

/** Bios_FM_CheckPresence,FMDetect**************************************
 *
 * 检查税控存储器是否存在
 *
 * @return BYTE :true-存在;false-不存在
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
  * 按字节读取外部flash中的数据
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
 * 向flash存储器中写入数据
 *
 * @param ulOff :数据写入地址
 * @param NumBytes :需要写入的字节数目
 * @param ucArrayPointer :需要写入的数据
 *
 * @return int: FLASH_SUCCESS-写入成功,
 *              FLASH_ERASE_FAIL或其它值-写入失败
 ****************************************************************** */
int bFlashProgram( uint32_t ulOff, uint32_t NumBytes,uint8_t *ucArrayPointer );

//
/**bFlashMBlockErase******************************************************
 *
 * 擦除Flash存储块,
 *
 * @param ucBlock :需要擦除的块编号
 * @param ucNumBlocks :从ucBlock号块开始连续擦除ucNumBlocks块
 *
 * @return int :FLASH_SUCCESS-擦除成功,
 *              FLASH_ERASE_FAIL或其它值-擦除失败
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

//--------------------9.键盘读取 API------------------------------

/** Getch*************************************************************
  *
  * 读取键盘输入的按键
  * 在调用Getch之前应该先调用Bios_1(BiosCmd_CheckKeyborad)判断是否有按键
  *
  * @param  None
  * @retval :key pressed; 0,1,2,.....
  ****************************************************************** */
uint8_t Getch(void);


//--------------------10.USB 通信 API---------------------------------

/**USB_COMM_Ready*****************************************************
 *
 * 测试USB状态,当要使用USB仿真串口进行通信时.必须定期调用USB_COMM_Ready
 * 以侦听USB是否有数据
 *
 * @return uint8_t :true-表示USB收到了通信数据
 **********************************************************************/
uint8_t USB_COMM_Ready(void);

/**ReadUSB***********************************************************
 *
 * 读取USB的通信数据
 *
 * @param rCH :用于存放读取到的通信数据
 *
 * @return BYTE :=true,表示有数据,=false表示无数据
 *
 ***************************************************************** */
BYTE ReadUSB(BYTE *rCH);

/**CheckUSB***********************************************************
 *
 * 测试USB是否收到通信数据
 *
 * @return BYTE :=true,表示有数据,=false表示无数据
 *
 ***************************************************************** */
BYTE CheckUSB(void);

/**EmptyUSB***********************************************************
 *
 * 清空USB收到的通信数据
 *
 ***************************************************************** */
void EmptyUSB(void);

/**SendUSB***********************************************************
 *
 * 通过USB发送一个字节数据
 *
 * @param ch :为要发送的字节数据
 *
 ***************************************************************** */
void SendUSB(BYTE ch);

/**SendUSBString******************************************************
 *
 * 通过USB发送一个字符串
 *
 * @param sStr :要发送的字符串
 * @param len :字符串的长度
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

