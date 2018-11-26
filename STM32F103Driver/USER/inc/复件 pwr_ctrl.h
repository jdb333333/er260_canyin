
#ifndef __PWR_CTRL_H
#define __PWR_CTRL_H

#define BAT_CTRL		GPIO_Pin_6
#define	BAT_CTRL_ON()	GPIOC->BSRRL = BAT_CTRL
#define	BAT_CTRL_OFF()	GPIOC->BSRRH = BAT_CTRL

#define CHG_CTRL		GPIO_Pin_8
#define	CHG_CTRL_ON()	GPIOG->BSRRH = CHG_CTRL		 // «ÂŒª
#define	CHG_CTRL_OFF()	GPIOG->BSRRL = CHG_CTRL		 // ÷√Œª


extern __IO uint16_t Vin_Backup_ADCValue[2];

void pwr_Init(void);


#endif	//__PWR_CTRL_H
