#ifndef __LCD_HT1621_H
#define __LCD_HT1621_H

#define DISP_WIDTH		10

#define	FLAG_TOTAL		0x01
#define	FLAG_COLLECT	0x02
#define	FLAG_CHANGE		0x04
#define	FLAG_PRICE		0x08



void HT1621_Init(void);
void HT1621_Refresh(void);
void PutsO_Saved(void);
void LED_Off(void);
void LED_On(void);
void PutsO( const char *str);
void LCD10_SetFlag(uint8_t flag);

#endif



