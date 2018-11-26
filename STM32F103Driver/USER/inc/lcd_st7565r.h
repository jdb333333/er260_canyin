
#ifndef __LCD_ST7565R_H
#define __LCD_ST7565R_H

#include "TypeDef.h"

// ST7565R Command defines

#define 	ST7565R_CMD_DISPLAY_ON   				0xAF
#define 	ST7565R_CMD_DISPLAY_OFF   				0xAE
#define 	ST7565R_CMD_START_LINE_SET(line)   			(0x40 | (line))
#define 	ST7565R_CMD_PAGE_ADDRESS_SET(page)   		(0xB0 | (page))
#define 	ST7565R_CMD_COLUMN_ADDRESS_SET_MSB(column)	(0x10 | (column))
#define 	ST7565R_CMD_COLUMN_ADDRESS_SET_LSB(column)	(0x00 | (column))
#define 	ST7565R_CMD_ADC_NORMAL   				0xA0
#define 	ST7565R_CMD_ADC_REVERSE   				0xA1
#define 	ST7565R_CMD_DISPLAY_NORMAL   			0xA6
#define 	ST7565R_CMD_DISPLAY_REVERSE   			0xA7
#define 	ST7565R_CMD_DISPLAY_ALL_POINTS_OFF  	0xA4
#define 	ST7565R_CMD_DISPLAY_ALL_POINTS_ON   	0xA5
#define 	ST7565R_CMD_LCD_BIAS_9   				0xA2
#define 	ST7565R_CMD_LCD_BIAS_7   				0xA3
#define 	ST7565R_CMD_NORMAL_SCAN_DIRECTION   	0xC0
#define 	ST7565R_CMD_REVERSE_SCAN_DIRECTION   	0xC8
#define 	ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_0   	0x20
#define 	ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_1   	0x21
#define 	ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_2   	0x22
#define 	ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_3   	0x23
#define 	ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_4   	0x24
#define 	ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_5   	0x25
#define 	ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_6   	0x26
#define 	ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_7   	0x27
#define 	ST7565R_CMD_POWER_CTRL_ALL_ON   		0x2F
#define 	ST7565R_CMD_SLEEP_MODE   				0xAC
#define 	ST7565R_CMD_NORMAL_MODE   				0xAD
#define 	ST7565R_CMD_RESET   					0xE2
#define 	ST7565R_CMD_NOP   						0xE3
#define 	ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET	0x81
#define 	ST7565R_CMD_ELECTRONIC_VOLUME(volume) 	(0x3F & (~volume))
#define 	ST7565R_CMD_BOOSTER_RATIO_SET   		0xF8
#define 	ST7565R_CMD_BOOSTER_RATIO_2X_3X_4X   	0x00
#define 	ST7565R_CMD_BOOSTER_RATIO_5X   			0x01
#define 	ST7565R_CMD_BOOSTER_RATIO_6X   			0x03
#define 	ST7565R_CMD_STATUS_READ   				0x00
#define 	ST7565R_CMD_END   						0xEE
#define 	ST7565R_CMD_READ_MODIFY_WRITE   		0xE0


/* Select Chip Select pin low */
#define ST7565R_CS_LOW()       GPIOF->BRR =	GPIO_Pin_7		// 清位
#define ST7565R_CS_HIGH()      GPIOF->BSRR =	GPIO_Pin_7	// 置位

#define ST7565R_A0_CMD()       GPIOF->BRR =	GPIO_Pin_6		// 清位
#define ST7565R_A0_DATA()      GPIOF->BSRR =	GPIO_Pin_6	// 置位

#define ST7565R_CLK_LOW()      GPIOG->BRR =	GPIO_Pin_15		// 清位
#define ST7565R_CLK_HIGH()     GPIOG->BSRR =	GPIO_Pin_15	// 置位

#define ST7565R_SID_LOW()      GPIOF->BRR =	GPIO_Pin_8		// 清位
#define ST7565R_SID_HIGH()     GPIOF->BSRR =	GPIO_Pin_8	// 置位

#define ST7565R_BL_ON()			GPIOB->BSRR = GPIO_Pin_2
#define ST7565R_BL_OFF()		GPIOB->BRR = GPIO_Pin_2


void mInitialLCD(void);
void mClearScreen( void );
void mSetContrast(uint8_t contrast);

void mLightLCDOn(void);
void mLightLCDOff(void);
//void mSetInvAttr(void);
//void mClearInvAttr(void);
void mSetState(uint8_t NewState);
void mEnCursor(uint8_t NewState);
void mDispCursor(void);
void mMoveCursor(uint8_t x, uint8_t y);
void mRefreshLine(uint8_t LinNum);
void mDrawChar(uint8_t ColNum, uint8_t LinNum,uint8_t hzFlag);
void mDispCharXY(WORD ch, BYTE x, BYTE y);
void mDispStringXY(BYTE* str, BYTE x, BYTE y);

void  mDispString(BYTE y);

#endif //__LCD_ST7565R_H


