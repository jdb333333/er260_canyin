/*********************************************************************************
*  程序编写:陈初荣                                                               *
*  编写日期:2004年2月至7月                                                       *
*  发布日期:2004年10月                                                           *
*  修改日期:ccr2016-05-13                                                      *
*  流水空间不分中英版本,其空间起始位置和空间大小固定,当为英文版本时,仍然保留字库空间                                                                                      *
*********************************************************************************/
/****c1191_08.c*4Mb Flash Memory************************************************
This source file provides library C code for using the M29x040B devices.
The following devices are supported in the code:
	M29F040B
	M29W040B
The following functions are available in this library:
FlashReadReset() to reset the flash for normal memory access
FlashAutoSelect() to get information about the device
FlashBlockErase() to erase one or more blocks
FlashChipErase() to erase the whole chip
FlashProgram() to program a byte or an array
FlashErrorStr() to return the error string of an error

The hardware specific functions which need to be modified by the user are:
FlashWrite() for writing a byte to the flash
FlashRead() for reading a byte from the flash
FlashPause() for timing short pauses (in micro seconds)
*******************************************************************************/

#include "Flashb.h"


/*******************************************************************************
Constants
*******************************************************************************/

/* Any address offset within the Flash Memory will do */
#define EXPECTED_DEVICE (0xD8) /* Device code for the AM29F160DB */

#if NOR_FLASH_MX29LV160DB
const uint32_t BlockOffset[] = // Offset from BASE_ADDRB of blocks
{
    //保留区
    0x000000L, 		//SA0 16 Kbytes
    //会员数据存储器
	0x004000L,      //SA1 8 Kbytes
	0x006000L,      //SA2 8 Kbytes
	0x008000L,      //SA3 32 Kbytes
    //16点阵显示字库存放区//ccr2016-05-13
    0x010000L,      //SA4 64 Kbytes
	0x020000L,      //SA5 64 Kbytes
	0x030000L,      //SA6 64 Kbytes
	0x040000L,      //SA7 64 Kbytes
    //24点阵打印字库存放区//ccr2016-05-13
	0x050000L,      //SA8 64 Kbytes
	0x060000L,      //SA9 64 Kbytes
	0x070000L,      //SA10 64 Kbytes
	0x080000L,      //SA11 64 Kbytes
	0x090000L,      //SA12 64 Kbytes
	0x0a0000L,      //SA13 64 Kbytes
	0x0b0000L,      //SA14 64 Kbytes
	0x0c0000L,      //SA15 64 Kbytes
    //流水数据存放区
	0x0d0000L,      //SA16 64 Kbytes
	0x0e0000L,      //SA17 64 Kbytes
	0x0f0000L,      //SA18 64 Kbytes
	0x100000L,      //SA19 64 Kbytes
	0x110000L,      //SA20 64 Kbytes
	0x120000L,      //SA21 64 Kbytes
	0x130000L,      //SA22 64 Kbytes
	0x140000L,      //SA23 64 Kbytes
	0x150000L,      //SA24 64 Kbytes
	0x160000L,      //SA25 64 Kbytes
	0x170000L,      //SA26 64 Kbytes
	0x180000L,      //SA27 64 Kbytes
	0x190000L,      //SA28 64 Kbytes
	0x1a0000L,      //SA29 64 Kbytes
	0x1b0000L,      //SA30 64 Kbytes
	0x1c0000L,      //SA31 64 Kbytes
	0x1d0000L,      //SA32 64 Kbytes
	0x1e0000L,      //SA33 64 Kbytes
	0x1f0000L       //SA34 64 Kbytes
};

#endif

/*******************************************************************************
Static Prototypes
The following functions are only needed in this module.
*******************************************************************************/
void bFlashWrite( uint32_t ulOff, uint8_t ucVal );
void wFlashWrite( uint32_t ulOff, uint16_t ucVal );
void bFlashPause( uint32_t uMicroSeconds );
int bFlashDataToggle( void );

/*******************************************************************************
Function: unsigned char FlashWrite( unsigned long ulOff, unsigned char ucVal)
Arguments: ulOff is the byte offset in the flash to write to
			ucVal is the value to be written
Returns: ucVal
Description: This function is used to write a byte to the flash. On many
			microprocessor systems a macro can be used instead, increasing the speed of
			the flash routines. For example:
			#define FlashWrite( ulOff, ucVal ) ( BASE_ADDRB[ulOff] = (unsigned char) ucVal )
			A function is used here instead to allow the user to expand it if necessary.
			The function is made to return ucVal so that it is compatible with the macro.
Pseudo Code:
Step 1: Write ucVal to the byte offset in the flash
Step 2: return ucVal
*******************************************************************************/
void bFlashWrite( uint32_t ulOff, uint8_t ucVal )
{
/* Step1, 2: Write ucVal to the byte offset in the flash and return it */
	BASE_ADDRB[ulOff] = ucVal;
}
void wFlashWrite( uint32_t ulOff, uint16_t ucVal )
{
/* Step1, 2: Write ucVal to the byte offset in the flash and return it */
	CWORD(BASE_ADDRB[ulOff]) = ucVal;
}
/*******************************************************************************
Function: 	unsigned char FlashRead( unsigned long ulOff )
Arguments: 	ulOff is the byte offset into the flash to read from
Returns: 	The unsigned char at the byte offset
Description: This function is used to read a byte from the flash. On many
			microprocessor systems a macro can be used instead, increasing the speed of
			the flash routines. For example:
			#define FlashRead( ulOff ) ( BASE_ADDRB[ulOff] )
			A function is used here instead to allow the user to expand it if necessary.
Pseudo Code:
Step 1: Return the value at byte offset ulOff
*******************************************************************************/
uint8_t bFlashRead( uint32_t ulOff )
{
/* Step 1 Return the value at byte offset ulOff */
	return BASE_ADDRB[ulOff];
}
uint16_t wFlashRead( uint32_t ulOff )
{
/* Step 1 Return the value at byte offset ulOff */
	return CWORD(BASE_ADDRB[ulOff]);
}

/*******************************************************************************
 Read block data from flash in byte mode;
 It's a good idea for use bFlashReadBuffer.
*******************************************************************************/
void bFlashBlockRead( uint32_t ulOff, BYTE uBuff[],WORD rLen)
{
    WORD i;

	for (i=0;i<rLen;i++)
	{
		uBuff[i] = BASE_ADDRB[ulOff];
		ulOff++;
	}
}

/*******************************************************************************
Function: 	void FlashPause( unsigned int uMicroSeconds )
Arguments: uMicroSeconds is the length of the pause in microseconds
Returns: 	none
Description: This routine returns after uMicroSeconds have elapsed. It is used
			in several parts of the code to generate a pause required for correct
			operation of the flash part.
			The routine here works by counting. The user may already have a more suitable
			routine for timing which can be used.
Pseudo Code:
Step 1: Compute count size for pause.
Step 2: Count to the required size.
*****************************************************************/
void bFlashPause( unsigned int uMicroSeconds )
{
	volatile uint32_t ulCountSize;
	/* Step 1: Compute the count size */
	ulCountSize = (uint32_t)uMicroSeconds * COUNTS_PER_MICROSECOND;
	/* Step 2: Count to the required size */
	while( ulCountSize > 0 ) /* Test to see if finished */
		ulCountSize--; /* and count down */
}
/*******************************************************************************
Function: 	void FlashReadReset( void )
Arguments: 	none
Return Value: none
Description: This function places the flash in the Read mode described
			in the Data Sheet. In this mode the flash can be read as normal memory.
			All of the other functions leave the flash in the Read mode so this is
			not strictly necessary. It is provided for completeness.
			Note: A wait of 10us is required if the command is called during a program or
			erase instruction. This is included here to guarantee correct operation. The
			functions in this library call this function if they suspect an error
			during programming or erasing so that the 10us pause is included. Otherwise
			they use the single instruction technique for increased speed.
Pseudo Code:
Step 1: write command sequence (see Commands Table of the Data Sheet)
Step 2: wait 10us
*******************************************************************************/
void bFlashReadReset( void )
{
/* Step 1: write command sequence */
	bFlashWrite( bANY_ADDR, 0xF0 ); /* 1rd Cycle: write 0xF0 to ANY address */
/* Step 2: wait 10us */
	bFlashPause( 10 );
}
/*******************************************************************************
Function: 	int FlashAutoSelect( int iFunc )
Arguments: 	iFunc should be set to either the Read Signature values or to the
			block number. The header file defines the values for reading the Signature.
Note: 		the first block is Block 0
Return Value: When iFunc is >= 0 the function returns FLASH_BLOCK_PROTECTED
			(01h) if the block is protected and FLASH_BLOCK_UNPROTECTED (00h) if it is
			unprotected. See the Auto Select command in the Data Sheet for further
			information.
			When iFunc is FLASH_READ_MANUFACTURER (-2) the function returns the
			manufacturer’s code. The Manufacturer code for ST is 20h.
			When iFunc is FLASH_READ_DEVICE_CODE (-1) the function returns the Device
			Code. The device codes for the parts are:
			M29F040B E2h
			M29W040B E3h
			When iFunc is invalid the function returns FLASH_BLOCK_INVALID (-5)
			Description: This function can be used to read the electronic signature of the
			device, the manufacturer code or the protection level of a block.
Pseudo Code:
Step 1: Send the Auto Select command to the device
Step 2: Read the required function from the device.
Step 3: Return the device to Read mode.
*******************************************************************************/
int bFlashAutoSelect( int iFunc )
{
	int iRetVal; /* Holds the return value */
	/* Step 1: Send the Auto Select command */
	bFlashWrite( 0xAAAL, 0xAA ); /* 1st Cycle */
	bFlashWrite( 0x555L, 0x55 ); /* 2nd Cycle */
	bFlashWrite( 0xAAAL, 0x90 ); /* 3rd Cycle */
	/* Step 2: Read the required function */
	if( iFunc == FLASH_READ_MANUFACTURER )
		iRetVal = MANUFACTURER_AMD;//(int) FlashRead( 0x0000L );
	else if( iFunc == FLASH_READ_DEVICE_CODE )
	{
		iRetVal = (int) bFlashRead( 0x0002L );
		iRetVal = (int) bFlashRead( 0x001cL ) + (int) bFlashRead( 0x001eL ) * 256;
	}
	else if( (iFunc >= 0) && (iFunc < FLASH_BLOCKS) )
		iRetVal = bFlashRead( BlockOffset[iFunc] + 0x0004L );
	else
		iRetVal = FLASH_BLOCK_INVALID;
	/* Step 3: Return to Read mode */
	bFlashWrite( bANY_ADDR, 0xF0 ); /* Use single instruction cycle method */
	return iRetVal;
}
/*******************************************************************************
Function: 	int FlashBlockErase( unsigned char ucNumBlocks,unsigned	char ucBlock[] )
Arguments: 	ucNumBlocks holds the number of blocks in the array ucBlock
			ucBlock is an array containing the blocks to be erased.
Return Value: The function returns the following conditions:
			FLASH_SUCCESS (-1)
			FLASH_TOO_MANY_BLOCKS (-3)
			FLASH_MPU_TOO_SLOW (-4)
			FLASH_WRONG_TYPE (-8)
			FLASH_ERASE_FAIL (-14)
Number of the first protected or invalid block
The user’s array, ucBlock[] is used to report errors on the specified
blocks. If a time-out occurs because the MPU is too slow then the blocks
in ucBlocks which are not erased are overwritten with FLASH_BLOCK_NOT_ERASED
(FFh) and the function returns FLASH_MPU_TOO_SLOW.
If an error occurs during the erasing of the blocks the function returns
FLASH_ERASE_FAIL.
If both errors occur then the function will set the ucBlock array to
FLASH_BLOCK_NOT_ERASED for the unerased blocks. It will return
FLASH_ERASE_FAIL even though the FLASH_MPU_TOO_SLOW has also occurred.
Description: This function erases up to ucNumBlocks in the flash. The blocks
can be listed in any order. The function does not return until the blocks are
erased. If any blocks are protected or invalid none of the blocks are erased.
During the Erase Cycle the Data Toggle Flow Chart of the Data Sheet is
followed. The polling bit, DQ7, is not used.
Pseudo Code:
Step 1: Check for correct flash type
Step 2: Check for protected or invalid blocks
Step 3: Write Block Erase command
Step 4: Check for time-out blocks
Step 5: Wait for the timer bit to be set.
Step 6: Follow Data Toggle Flow Chart until Program/Erase Controller has
completed
Step 7: Return to Read mode (if an error occurred)
*******************************************************************************/
int bFlashBlockErase(uint8_t ucBlock[],  int ucNumBlocks)
{
	int ucCurBlock; /* Range Variable to track current block */
	int iRetVal = FLASH_SUCCESS; /* Holds return value: optimistic initially! */
#if defined(CASE_FORHANZI)
	while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash
#endif
/* Step 1: Check for correct flash type */
//	if( (FlashAutoSelect( FLASH_READ_MANUFACTURER ) != MANUFACTURER_AMD))
//		|| (FlashAutoSelect( FLASH_READ_DEVICE_CODE ) != EXPECTED_DEVICE ) )
//		return FLASH_WRONG_TYPE;
/* Step 2: Check for protected or invalid blocks. */
	if( ucNumBlocks > FLASH_BLOCKS ) /* Check specified blocks <= FLASH_BLOCKS */
		return FLASH_TOO_MANY_BLOCKS;
	for( ucCurBlock = 0; ucCurBlock < ucNumBlocks; ucCurBlock++ )
	{
/* Use FlashAutoSelect to find protected or invalid blocks*/
		if( bFlashAutoSelect((int)ucBlock[ucCurBlock]) != FLASH_BLOCK_UNPROTECTED )
			return (int)ucBlock[ucCurBlock]; /* Return protected/invalid blocks */
	}
/* Step 3: Write Block Erase command */
	bFlashWrite( 0xAAAL, 0xAA );
	bFlashWrite( 0x555L, 0x55 );
	bFlashWrite( 0xAAAL, 0x80 );
	bFlashWrite( 0xAAAL, 0xAA );
	bFlashWrite( 0x555L, 0x55 );
/* DSI!: Time critical section. Additional blocks must be added every 50us */
	for( ucCurBlock = 0; ucCurBlock < ucNumBlocks; ucCurBlock++ )
	{
		bFlashWrite( BlockOffset[ucBlock[ucCurBlock]], 0x30 );
/* Check for Erase Timeout Period (is bit DQ3 set?) */
		if( (bFlashRead( BlockOffset[ucBlock[0]] ) & 0x08) == 0x08 )
			break; /* Cannot set any more blocks due to timeout */
	}
/* ENI! */
/* Step 4: Check for time-out blocks */
	if( ucCurBlock < ucNumBlocks )
	{
/* Indicate that some blocks have been timed out of the erase list */
		iRetVal = FLASH_MPU_TOO_SLOW;
/* Now specify all other blocks as not being erased */
/* Note that we cannot tell if the first (potentially timed out) block is erasing or not */
		while( ucCurBlock < ucNumBlocks )
		{
			ucBlock[ucCurBlock++] = FLASH_BLOCK_NOT_ERASED;
		}
	}
/* Step 5: Wait for the Erase Timer Bit (DQ3) to be set */
	while( 1 ) /* TimeOut!: If, for some reason, the hardware fails then this
					loop may not exit. Use a timer function to implement a timeout
					from the loop. */
	{
		if( ( bFlashRead( BlockOffset[ucBlock[0]] ) & 0x08 ) == 0x08 )
			break; /* Break when device starts the erase cycle */
	}
/* Step 6: Follow Data Toggle Flow Chart until Program/Erase Controller completes */
	if( bFlashDataToggle() != FLASH_SUCCESS )
	{
		iRetVal = FLASH_ERASE_FAIL;
/* Step 7: Return to Read mode (if an error occurred) */
		bFlashReadReset();
	}
	return iRetVal;
}
//从ucBlock号块开始连续擦除ucNumBlocks块
int bFlashMBlockErase(int ucBlock, int ucNumBlocks)
{
	int ucCurBlock; /* Range Variable to track current block */
	int iRetVal = FLASH_SUCCESS; /* Holds return value: optimistic initially! */

#if defined(CASE_FORHANZI)
	while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash
#endif
/* Step 1: Check for correct flash type */
//	if( (FlashAutoSelect( FLASH_READ_MANUFACTURER ) != MANUFACTURER_AMD))
//		|| (FlashAutoSelect( FLASH_READ_DEVICE_CODE ) != EXPECTED_DEVICE ) )
//		return FLASH_WRONG_TYPE;
/* Step 2: Check for protected or invalid blocks. */
	if( ucNumBlocks+ucBlock > FLASH_BLOCKS ) /* Check specified blocks <= FLASH_BLOCKS */
		return FLASH_TOO_MANY_BLOCKS;
	for( ucCurBlock = 0; ucCurBlock < ucNumBlocks; ucCurBlock++ )
	{
/* Use FlashAutoSelect to find protected or invalid blocks*/
		if( bFlashAutoSelect(ucBlock+ucCurBlock) != FLASH_BLOCK_UNPROTECTED )
			return (int)ucBlock+ucCurBlock; /* Return protected/invalid blocks */
	}
/* Step 3: Write Block Erase command */
	bFlashWrite( 0xAAAL, 0xAA );
	bFlashWrite( 0x555L, 0x55 );
	bFlashWrite( 0xAAAL, 0x80 );
	bFlashWrite( 0xAAAL, 0xAA );
	bFlashWrite( 0x555L, 0x55 );
/* DSI!: Time critical section. Additional blocks must be added every 50us */
	for( ucCurBlock = 0; ucCurBlock < ucNumBlocks; ucCurBlock++ )
	{
		bFlashWrite( BlockOffset[ucBlock+ucCurBlock], 0x30 );
/* Check for Erase Timeout Period (is bit DQ3 set?) */
		if( (bFlashRead( BlockOffset[ucBlock] ) & 0x08) == 0x08 )
			break; /* Cannot set any more blocks due to timeout */
	}
/* ENI! */
/* Step 4: Check for time-out blocks */
	if( ucCurBlock < ucNumBlocks )
	{
/* Indicate that some blocks have been timed out of the erase list */
		iRetVal = FLASH_MPU_TOO_SLOW;
/* Now specify all other blocks as not being erased */
/* Note that we cannot tell if the first (potentially timed out) block is erasing or not */
	}
/* Step 5: Wait for the Erase Timer Bit (DQ3) to be set */
	while( 1 ) /* TimeOut!: If, for some reason, the hardware fails then this
					loop may not exit. Use a timer function to implement a timeout
					from the loop. */
	{
		if( ( bFlashRead( BlockOffset[ucBlock] ) & 0x08 ) == 0x08 )
			break; /* Break when device starts the erase cycle */
	}
/* Step 6: Follow Data Toggle Flow Chart until Program/Erase Controller completes */
	if( bFlashDataToggle() != FLASH_SUCCESS )
	{
		iRetVal = FLASH_ERASE_FAIL;
/* Step 7: Return to Read mode (if an error occurred) */
		bFlashReadReset();
	}
	return iRetVal;
}
/*******************************************************************************
Function: int FlashChipErase( void )
Arguments: none
Return Value: On success the function returns FLASH_SUCCESS (-1)
If a block is protected then the function returns the number of the block and
no blocks are erased.
If the erase algorithms fails then the function returns FLASH_ERASE_FAIL (-2)
If the wrong type of flash is detected then FLASH_WRONG_TYPE (-8) is
returned.
Description: The function can be used to erase the whole flash chip so long as
no blocks are protected. If any blocks are protected then nothing is
erased.
Pseudo Code:
Step 1: Check for correct flash type
Step 2: Check that all blocks are unprotected
Step 3: Send Chip Erase Command
Step 4: Follow Data Toggle Flow Chart until Program/Erase Controller has
completed.
Step 5: Return to Read mode (if an error occurred)
*******************************************************************************/
int bFlashChipErase( void )
{
	int ucCurBlock; /* Used to track the current block in a range */

#if defined(CASE_FORHANZI)
	while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash
#endif
	/* Step 1: Check for correct flash type */
//	if( (FlashAutoSelect( FLASH_READ_MANUFACTURER ) != MANUFACTURER_AMD))
//		|| (FlashAutoSelect( FLASH_READ_DEVICE_CODE ) != EXPECTED_DEVICE ) )
//		return FLASH_WRONG_TYPE;
	/* Step 2: Check that all blocks are unprotected */
	for( ucCurBlock = 0; ucCurBlock < FLASH_BLOCKS; ucCurBlock++ )
	{
		if( bFlashAutoSelect( (int)ucCurBlock ) != FLASH_BLOCK_UNPROTECTED )
			return (int)ucCurBlock; /* Return the first protected block */
	}
	/* Step 3: Send Chip Erase Command */
	bFlashWrite( 0xAAAL, 0xAA );
	bFlashWrite( 0x555L, 0x55 );
	bFlashWrite( 0xAAAL, 0x80 );
	bFlashWrite( 0xAAAL, 0xAA );
	bFlashWrite( 0x555L, 0x55 );
	bFlashWrite( 0xAAAL, 0x10 );
	/* Step 4: Follow Data Toggle Flow Chart until Program/Erase Controller has 	completed */
	if( bFlashDataToggle() != FLASH_SUCCESS )
	{
	/* Step 5: Return to Read mode (if an error occurred) */
		bFlashReadReset();
		return FLASH_ERASE_FAIL;
	}
	else
		return FLASH_SUCCESS;
}
/*******************************************************************************
Function: int FlashProgram( unsigned long ulOff, size_t NumBytes,void *Array )
Arguments: ulOff is the word offset into the flash to be programmed
		NumBytes holds the number of bytes in the array.
		Array is a pointer to the array to be programmed.
Return Value: The function returns the following conditions:
			FLASH_SUCCESS (-1)
			FLASH_PROGRAM_FAIL (-6)
			FLASH_OFFSET_OUT_OF_RANGE (-7)
			FLASH_WRONG_TYPE (-8)
Number of the first protected or invalid block
On success the function returns FLASH_SUCCESS (-1).
The function returns FLASH_PROGRAM_FAIL (-6) if a programming failure occurs.
If the address range to be programmed exceeds the address range of the Flash
Device the function returns FLASH_OFFSET_OUT_OF_RANGE (-7) and nothing is
programmed.
If the wrong type of flash is detected then FLASH_WRONG_TYPE (-8) is
returned and nothing is programmed.
If part of the address range to be programmed falls within a protected block,
the function returns the number of the first protected block encountered and
nothing is programmed.
Description: This function is used to program an array into the flash. It does
not erase the flash first and may fail if the block(s) are not erased first.
Pseudo Code:
Step 1: Check for correct flash type
Step 2: Check the offset range is valid
Step 3: Check that the block(s) to be programmed are not protected
Step 4: While there is more to be programmed
Step 5: Check for changes from ‘0’ to ‘1’
Step 6: Program the next byte
Step 7: Follow Data Toggle Flow Chart until Program/Erase Controller has
completed
Step 8: Return to Read mode (if an error occurred)
Step 9: Update pointers
*******************************************************************************/
int wFlashProgramAWord(uint32_t ulOff,uint16_t sWord)
{//program A WORD in ulOff
    uint16_t rWord = FlashReadWord(ulOff+NOR_FLASH_ADDR);
	/* Check for changes from ‘0’ to ‘1’ */
    if( (~rWord) & sWord)
    {
#if defined(FOR_DEBUG)
        xprintf("Err%06LX-W%04x-R%04x\n",ulOff,sWord,rWord);//testonly
#endif
        /* Indicate failure as it is not possible to change a ‘0’ to a ‘1’using a Program command. This must be done using an Erase command */
        return FLASH_ERASE_FAIL;//FLASH_PROGRAM_FAIL;
    }
    else if (rWord==sWord)
    {
        return FLASH_SUCCESS;
    }

	/* Program the WORD */
	bFlashWrite( 0xAAAL, 0xAA ); /* 1st cycle */
	bFlashWrite( 0x555L, 0x55 ); /* 2nd cycle */
	bFlashWrite( 0xAAAL, 0xA0 ); /* Program command */
	wFlashWrite( ulOff, sWord ); /* Program value in WORD mode*/

	/* Follow Data Toggle Flow Chart until Program/Erase Controller	has completed */
	/* See Data Toggle Flow Chart of the Data Sheet */
	if( bFlashDataToggle() == FLASH_TOGGLE_FAIL )
	{
	/* Return to Read mode (if an error occurred) */
			bFlashReadReset();
#if defined(FOR_DEBUG)
            xprintf("ERR%06LX-W%04x-R%04x\n",ulOff,sWord,rWord);//testonly
#endif
			return FLASH_PROGRAM_FAIL;
	}
	return FLASH_SUCCESS;
}

int bFlashProgram( uint32_t ulOff, uint32_t NumBytes,uint8_t *ucArrayPointer ) //ccr2017-11-03
{
    int flasherr;
	uint16_t sWord;
    uint8_t sByte;
	uint32_t ulLastOff; /* Holds the last offset to be programmed */
	int ucCurBlock; /* Range Variable to track current block */

#if defined(CASE_FORHANZI)
	while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash
#endif
/* Step 1: Check for correct flash type */
//	if( (FlashAutoSelect( FLASH_READ_MANUFACTURER ) != MANUFACTURER_AMD))
////		|| (FlashAutoSelect( FLASH_READ_DEVICE_CODE ) != EXPECTED_DEVICE ) )
//		return FLASH_WRONG_TYPE;
/* Step 2: Check the offset and range are valid */
	ulLastOff = ulOff+NumBytes-1;
	if( ulLastOff >= FLASH_SIZE)
		return FLASH_OFFSET_OUT_OF_RANGE;
	if( !NumBytes )
		return FLASH_SUCCESS;
/* Step 3: Check that the block(s) to be programmed are not protected */
	for( ucCurBlock = 0; ucCurBlock < FLASH_BLOCKS; ucCurBlock++ )
	{
/* If the address range to be programmed ends before this block */
		if( BlockOffset[ucCurBlock] > ulLastOff )
			break; /* then we are done */
/* Else if the address range starts beyond this block */
		else if( (ucCurBlock < (FLASH_BLOCKS-1)) &&
			(ulOff >= BlockOffset[ucCurBlock+1]) )
			continue; /* then skip this block */
/* Otherwise if this block is not unprotected */
		else if( bFlashAutoSelect((int)ucCurBlock) != FLASH_BLOCK_UNPROTECTED )
			return (int)ucCurBlock; /* Return first protected block */
	}

	if( ulOff & 0x01 )
	{//Program a BYTE in odd-address
        ulOff--;
		sWord = *ucArrayPointer;
        sByte = FlashReadByte(ulOff+NOR_FLASH_ADDR);
        sWord <<= 8;
        sWord |= sByte;
        flasherr = wFlashProgramAWord( ulOff, sWord );
		if (flasherr!= FLASH_SUCCESS)
			return flasherr;
		ulOff+=2;
		ucArrayPointer++;
		NumBytes--;
	}

	if( !NumBytes )
		return FLASH_SUCCESS;


	while(NumBytes>1 && ulOff <= ulLastOff )
	{//Program a wrod in even-address
		sWord = (*(uint16_t*)ucArrayPointer);
        flasherr = wFlashProgramAWord( ulOff, sWord );
		if (flasherr!= FLASH_SUCCESS)
			return flasherr;
		ucArrayPointer+=2;
		ulOff+=2;
		NumBytes-=2;
	}
	if( NumBytes==1 )
	{//Program the last byte in even-address
		sByte = *ucArrayPointer;
        sWord = FlashReadWord(ulOff+NOR_FLASH_ADDR);
		sWord &= 0xff00;
        sWord |= sByte;
        flasherr = wFlashProgramAWord( ulOff, sWord );
		if (flasherr!= FLASH_SUCCESS)
			return flasherr;
	}
	return FLASH_SUCCESS;
}
/**
 * 写入失败时,追加写入一次
 *
 * @author EutronSoftware (2017-11-03)
 *
 * @param ulOff
 * @param NumBytes
 * @param ucArrayPointer
 *
 * @return int
 */
#if (0)
int bFlashProgram( uint32_t ulOff, uint32_t NumBytes,uint8_t *ucArrayPointer )
{
    int retVal;

#if defined(CASE_FORHANZI)
	while( TestPrintGoingOn() )	{};//打印时,禁止操作Flash
#endif
    if ((retVal=_bFlashProgram_( ulOff, NumBytes,ucArrayPointer ))!=FLASH_SUCCESS ||
        memcmp(&BASE_ADDRB[ulOff],ucArrayPointer,NumBytes))
        retVal=_bFlashProgram_( ulOff, NumBytes,ucArrayPointer );
	return retVal;
}
#endif
/*******************************************************************************
Function: static int FlashDataToggle( void )
Arguments: none
Return Value: The function returns FLASH_SUCCESS if the Program/Erase Controller
is successful or FLASH_TOGGLE_FAIL if there is a problem.
Description: The function is used to monitor the Program/Erase Controller during
erase or program operations. It returns when the Program/Erase Controller has
completed. In the M29F040B Data Sheet or the M29W040B Data Sheet the Data
Toggle Flow Chart shows the operation of the function.
Pseudo Code:
Step 1: Read DQ6 (into a byte)
Step 2: Read DQ5 and DQ6 (into another byte)
Step 3: If DQ6 did not toggle between the two reads then return FLASH_SUCCESS
Step 4: Else if DQ5 is zero then operation is not yet complete, goto 1
Step 5: Else (DQ5 != 0), read DQ6 again
Step 6: If DQ6 did not toggle between the last two reads then return
FLASH_SUCCESS
Step 7: Else return FLASH_TOGGLE_FAIL
*******************************************************************************/
int bFlashDataToggle( void )
{
	uint8_t uc1, uc2; /* hold values read from any address offset within the Flash Memory */
	while( 1 ) /* TimeOut!: If, for some reason, the hardware fails then this
					loop may not exit. Use a timer function to implement a timeout
					from the loop. */
	{
/* Step 1: Read DQ6 (into a byte) */
		uc1 = bFlashRead( bANY_ADDR ); /* Read DQ6 from the Flash (any address) */
/* Step 2: Read DQ5 and DQ6 (into another byte) */
		uc2 = bFlashRead( bANY_ADDR ); /* Read DQ5 and DQ6 from the Flash (any address) */
/* Step 3: If DQ6 did not toggle between the two reads then return FLASH_SUCCESS */
		if( (uc1&0x40) == (uc2&0x40) ) /* DQ6 == NO Toggle */
			return FLASH_SUCCESS;
/* Step 4: Else if DQ5 is zero then operation is not yet complete */
		if( (uc2&0x20) == 0x00 )
			continue;
/* Step 5: Else (DQ5 == 1), read DQ6 again */
		uc1 = bFlashRead( bANY_ADDR ); /* Read DQ6 from the Flash (any address) */
/* Step 6: If DQ6 did not toggle between the last two reads then return FLASH_SUCCESS */
		if( (uc2&0x40) == (uc1&0x40) ) /* DQ6 == NO Toggle */
			return FLASH_SUCCESS;
/* Step 7: Else return FLASH_TOGGLE_FAIL */
		else /* DQ6 == Toggle here means fail */
			return FLASH_TOGGLE_FAIL;
	} /* end of while loop */
}

/**按字读取数据
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */

uint8_t wFlashReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
	ReadAddr+=NOR_FLASH_ADDR;
	for (; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
	{
		/* Read a half-word from the memory */
		*pBuffer++ = FlashReadWord(ReadAddr);

		ReadAddr+=2;
	}
	return SUCCESS;
}

/**按字节读取数据,符合字地址时,则按字读取,提高读取效率
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
uint8_t bFlashReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumBytesToRead)
{
	uint32_t sBytesToRead;

	ReadAddr+=NOR_FLASH_ADDR;

	if( NumBytesToRead )
	{
		if( ReadAddr & 1 )
		{//read a byte in odd-address
			*pBuffer++ = FlashReadByte(ReadAddr);
			ReadAddr++;
			NumBytesToRead--;
		}
		sBytesToRead = NumBytesToRead & 1;
		NumBytesToRead >>=1;
		for (; NumBytesToRead != 0; NumBytesToRead--) /* while there is data to read */
		{//read word
			/* Read a half-word from the memory */
			CWORD(pBuffer[0]) = FlashReadWord(ReadAddr);
			pBuffer+=2;
			ReadAddr+=2;
		}
		if( sBytesToRead )
		{//read the last byte
			*pBuffer++ = FlashReadByte(ReadAddr);
		}
	}
	return SUCCESS;
}

/********************************************************************************
//  根据Flash类型生成Flash大小(M)
*********************************************************************************/
int bFlashAutoSize( int iTypeId )
{
	switch (iTypeId){
	case 0x001A:
		return 0x04;
	case 0x0010:
		return 0x08;
	case 0x0121:
		return 0x16;
	default:
		return 0;
	}
}

