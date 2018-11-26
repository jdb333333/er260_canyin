/*********************************************************************************
*  程序编写:陈初荣                                                               *
*  编写日期:2004年2月至7月                                                       *
*  发布日期:2004年10月                                                           *
*  修改日期:                                                                     *
*********************************************************************************/
#ifndef		FLASH_C
#define 	FLASH_C

#if (!defined(DEBUGBYPC))
#include "stm32f10x.h"
#include "fsmc_sram.h"
#include "TypeDef.h"
#endif

/* Target Flash Device: MX29XXXX */

#define MX29LV160DBTI	1


#define COUNTS_PER_MICROSECOND (200)
#define MANUFACTURER_AMD (0x01) /* Manufacturer code for AMD*/
#define BASE_ADDRB ((__IO uint8_t*)0x60000000)
#define BASE_ADDRW ((__IO uint16_t*)0x60000000)

#define FLASH_BASEADDR  0x60000000

/* BASE_ADDRB is the base address of the flash, see the functions FlashRead()
and FlashWrite(). Some applications which require a more complicated
FlashRead() or FlashWrite() may not use BASE_ADDRB */
#define bANY_ADDR (0x00000L)

#define FLASH_SIZE (0x200000L) /* 2048K */

#define FLASH_BLOCKS 35//(sizeof(BlockOffset)/sizeof(BlockOffset[0]))

#define	NOR_FLASH_MX29LV160DB	1
#if NOR_FLASH_MX29LV160DB
#define  ManufacturerID	0xC2
#define  DeviceID 		0x2249	//MX29LV160DT--0x22C4,MX29LV160DB--0x2249
#else
#define	 ManufacturerID	0xBF
#define	 DeviceID		0x234F	//SST39VF1601C--0x234F,SST39VF1602C--0x234E
#endif
//ccr2016-05-13>>>>>>>>>>>>>>>>>>>>>>>>
//ECRVIP会员数据存储区
#define FLASH_ECRVIPVAR1_ADDR   0x004000L
#define FLASH_ECRVIPVAR1_SIZE	0x02000		//16Kx8bit
#define FLASH_ECRVIPVAR1_BLOCKFr 1
#define FLASH_ECRVIPVAR1_BLOCKS	 1

#define FLASH_ECRVIPVAR2_ADDR   0x006000L
#define FLASH_ECRVIPVAR2_SIZE	0x02000		//16Kx8bit
#define FLASH_ECRVIPVAR2_BLOCKFr 2
#define FLASH_ECRVIPVAR2_BLOCKS	 1

#define FLASH_ECRVIPFIX_ADDR    0x008000L
#define FLASH_ECRVIPFIX_SIZE	0x08000		//32Kx8bit
#define FLASH_ECRVIPFIX_BLOCKFr	3
#define FLASH_ECRVIPFIX_BLOCKS	1

#if defined(CASE_FORHANZI)
// BYTE模式下字库地址 和 数据空间地址
#define FLASH_HZK16_ADDR			0x010000L
#define FLASH_HZK16_SIZE			0x40000		//128Kx16bit  实际占用:0---0x3FC60/2
#define FLASH_HZK16_BLOCKFr			4
#define FLASH_HZK16_BLOCKS			4		    //128Kx16bit  实际占用:0---0x3FC60/2


#define FLASH_HZK24_ADDR 			0x050000L
#define FLASH_HZK24_SIZE			0x80000		//256Kx16bit  实际占用:0x40000/2---0xBF140/2
#define FLASH_HZK24_BLOCKFr			8
#define FLASH_HZK24_BLOCKS			8		//256Kx16bit  实际占用:0x40000/2---0xBF140/2

#define	FLASH_LOG_BLOCKFr	        16
#define	FLASH_LOG_BLOCKS            19		//640Kx16bit=1280KB	 用于存储数据
#define	FLASH_LOG_ADDR	            0x0d0000L //BlockOffset[15]
#define	FLASH_LOG_SIZE	            (FLASH_LOG_BLOCKS*0x10000L)		//用于存储数据
#elif defined(CASE_EJFLASH)//FLASH 用于保存EJ时,无LOG
#define	FLASH_LOG_BLOCKFr	        4
#define	FLASH_LOG_BLOCKS            0		//640Kx16bit=1280KB	 用于存储数据
#define	FLASH_LOG_ADDR	            0x010000L //BlockOffset[15]
#define	FLASH_LOG_SIZE	            (FLASH_LOG_BLOCKS*0x10000L)		//用于存储数据
#else//英文版本下,无汉字库空间,因此在同一台机器上测试中英文版本时,会破坏汉字库
#define	FLASH_LOG_BLOCKFr	        16
#define	FLASH_LOG_BLOCKS            19		//640Kx16bit=1280KB	 用于存储数据
#define	FLASH_LOG_ADDR	            0x0d0000L //BlockOffset[15]
#define	FLASH_LOG_SIZE	            (FLASH_LOG_BLOCKS*0x10000L)		//用于存储数据
#endif
//ccr2016-05-13<<<<<<<<<<<<<<<<<<

/*******************************************************************************
Commands for the various functions
*******************************************************************************/
#define FLASH_READ_MANUFACTURER (-2)
#define FLASH_READ_DEVICE_CODE (-1)
/*******************************************************************************
Error Conditions and return values.
See end of C file for explanations and help
*******************************************************************************/
#define FLASH_BLOCK_PROTECTED (0x01)
#define FLASH_BLOCK_UNPROTECTED (0x00)
#define FLASH_BLOCK_NOT_ERASED (0xFF)
#define FLASH_SUCCESS (-1)
/*Description: This value indicates that the flash command has executed	correctly.*/
#define FLASH_POLL_FAIL (-2)
/*Description: The Program/Erase Controller algorithm has not managed to complete
the command operation successfully. This may be because the device is damaged
Solution: Try the command again. If it fails a second time then it is
likely that the device will need to be replaced.
*/
#define FLASH_TOO_MANY_BLOCKS (-3)
/*Description: The user has chosen to erase more blocks than the device has.
This may be because the array of blocks to erase contains the same block
more than once.
Solutions: Check that the program is trying to erase valid blocks. The device
will only have FLASH_BLOCKS blocks (defined at the top of the file). Also check
that the same block has not been added twice or more to the array.
*/
#define FLASH_MPU_TOO_SLOW (-4)
/*Description: The MPU has not managed to write all of the selected blocks to the
		device before the timeout period expired. See BLOCK ERASE COMMAND
		section of the Data Sheet for details.
Solutions: If this occurs occasionally then it may be because an interrupt is
		occuring between writing the blocks to be erased. Search for "DSI!" in
		the code and disable interrupts during the time critical sections.
		If this error condition always occurs then it may be time for a faster
		microprocessor, a better optimising C compiler or, worse still, learn
		assembly. The immediate solution is to only erase one block at a time.
		Disable the test (by #define’ing out the code) and always call the function
		with one block at a time.
*/
#define FLASH_BLOCK_INVALID (-5)
/*Description: A request for an invalid block has been made. Valid blocks number
from 0 to FLASH_BLOCKS-1.
Solution: Check that the block is in the valid range.
*/
#define FLASH_PROGRAM_FAIL (-6)
/*Description: The programmed value has not been programmed correctly.
Solutions: Make sure that the block containing the value was erased before
programming. Try erasing the block and re-programming the value. If it fails
again then the device may need to be changed.
*/
#define FLASH_OFFSET_OUT_OF_RANGE (-7)
/*Description: The address offset given is out of the range of the device.
Solution: Check that the address offset is in the valid range.
*/
#define FLASH_WRONG_TYPE (-8)
/*Description: The source code has been used to access the wrong type of flash.
Solutions: Use a different flash chip with the target hardware or contact
		STMicroelectronics for a different source code library.
*/
#define FLASH_ERASE_FAIL (-14)
/*Description: This indicates that the previous erasure of one block, many blocks
		or of the whole device has failed.
Solution: Investigate this failure further by attempting to erase each block
		individually. If erasing a single block still causes failure, then the Flash
		sadly needs replacing.
*/
#define FLASH_TOGGLE_FAIL (-15)
/*Description: The Program/Erase Controller algorithm has not managed to complete
			the command operation successfully. This may be because the device is damaged.
Solution: Try the command again. If it fails a second time then it is
			likely that the device will need to be replaced.
*/


extern const uint32_t BlockOffset[]; // Offset from BASE_ADDR of blocks (S29GL032M-10-T-A-I-R4-0)

#define bEraseABlock(a) bFlashBlockErase((char*)a,1)

extern int bFlashMBlockErase(int ucBlock, int ucNumBlocks);
extern void bFlashWrite( uint32_t ulOff, uint8_t ucVal );
extern int bFlashProgram( uint32_t ulOff, uint32_t NumBytes, uint8_t *Array );

extern uint8_t wFlashReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);
extern uint8_t bFlashReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumBytesToRead);

#define FlashReadWord(Addr)     *(__IO uint16_t*)(Addr)
#define FlashReadByte(Addr)     *(__IO uint8_t*)(Addr)

#if defined(CASE_INNERDOTPRN)
#define TestPrintGoingOn() 0
#else
extern uint8_t TestPrintGoingOn(void);
#endif
#endif
