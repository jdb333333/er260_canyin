
#ifndef __PWR_CTRL_H
#define __PWR_CTRL_H

#include "stm32f10x.h"

#if (defined(CASE_GIADA3000) || defined(CASE_ER520U))
#define POWERCTRL       0   //=1,允许进行电池检测
#else
#define POWERCTRL       1   //=1,允许进行电池检测
#endif
#define	VIN_Pin		   	GPIO_Pin_4
#define	VRTC_Pin		GPIO_Pin_5

#if !defined(CASE_ER260) && !defined(CASE_ER220)
#define DC_DET_Pin		GPIO_Pin_6
#define DC_DET_PORT		GPIOB
#else
#define DC_DET_Pin		GPIO_Pin_10
#define DC_DET_PORT		GPIOA
#endif

#define BAT_CHG_Pin			GPIO_Pin_11
#define BAT_DONE_Pin		GPIO_Pin_13
#define BAT_STAT_PORT		GPIOG

#define STAT_NOBATERRY	0x00
#define STAT_CHARGING		0x01
#define STAT_DONE				0x02

//检测系统供电方式
//#define DC_DET_GET()	((DC_DET_PORT->IDR & DC_DET_Pin) != 0)    /* TRUE--适配器供电, FALSE--电池供电*/
#if (defined(CASE_GIADA3000) || defined(CASE_ER520U))
#define DC_DET_GET()	(1)    /* TRUE--适配器供电, FALSE--电池供电*/
#else
#if !defined(CASE_ER260) && !defined(CASE_ER220)
#define DC_DET_GET()	((DC_DET_PORT->IDR & DC_DET_Pin) == 0)    /* TRUE--适配器供电, FALSE--电池供电*/
#else
#define DC_DET_GET()	((DC_DET_PORT->IDR & DC_DET_Pin) != 0)    /* TRUE--适配器供电, FALSE--电池供电*/
#endif
#endif

//#define ADC2_DR_ADDR   ((uint32_t)0x4001214C)	//(APB2PERIPH_BASE + 0x2100) + 0x4C
#define ADC1_DR_Address    ((uint32_t)0x4001244C)

//因外部输入阻抗过大导致实际采样值小于理论计算值：
// ER-260: Vbat=Vad+5.625  掉电阀值Vpfi=5.65V


//充电电池检测标准值.理论值:(DR/0xfff)*3300=DR*0.8059
#define VIN_8V7 0xEC8 //8.7V---3075mV---0xEE8
#define VIN_8V6 0xE4C //8.6V---2975mV---0xE6C
#define VIN_8V5 0xDCF //8.5V---2875mV---0xDEF
#define VIN_8V4 0xD53 //8.4V---2775mV---0xD73
#define VIN_8V3 0xCD7 //8.3V---2675mV---0xCF7
#define VIN_8V2 0xC5B //8.2V---2575mV---0xC7B
#define VIN_8V1 0xBDF //8.1V---2475mV---0xBFF
#define VIN_8V0 0xB63 //8.0V---2375mV---0xB83
#define VIN_7V9 0xAE7 //7.9V---2275mV---0xB07
#define VIN_7V8 0xA6B //7.8V---2175mV---0xA8B
#define VIN_7V7 0x9EF //7.7V---2075mV---0xA0F
#define VIN_7V6 0x973 //7.6V---1975mV---0x993
#define VIN_7V5 0x8F7 //7.5V---1875mV---0x917
#define VIN_7V4 0x87B //7.4V---1775mV---0x89B
#define VIN_7V3 0x7FE //7.3V---1675mV---0x81E
#define VIN_7V2 0x782 //7.2V---1575mV---0x7A2
#define VIN_7V1 0x706 //7.1V---1475mV---0x726
#define VIN_7V0 0x68A //7.0V---1375mV---0x6AA

//by ouhs ////理论值:(DR/0xfff)*3329=DR*0.813mV  //8690-5625=3065mV---0xEBA-0xE70=0x4A
#define VIN_6V8 0x59A   //ccr2016-06-22
#define VIN_6V6 0x49E   //ccr2016-06-22

#define	VIN_6V5		0x3E4	//6.5V---875mV---0x434-0x50
#define	VIN_6V2		0x273	//6.2V---575mV---0x2C3
#define	VIN_6V0		0x17D	//6.0V---375mV---0x1CD

#define VRTC_3V0	0xC2F	//3.0V---2769mV---0xD4E, 3075mV---2839mV---0xDA4-0xC85=0x11F
#define	VRTC_2V5	0x9F8	//2.5V---2308mV---0xB17
#define	VRTC_2V2	0x8A3	//2.2V---2031mV---0x9C2



#define VIN_OK  VIN_6V8
#define VIN_LOW VIN_6V6


#define	GET_VIN_STATUS	1
#define	GET_VRTC_STATUS	2

#define PWR_BY_BAT      0x01    //为电池供电

#define PWR_STAT_OK		1

#define PWR_WARNING1	0x02        //供电电池电压报警点1
#define PWR_WARNING2	0x04        //供电电池电压报警点2

#define PWR_BUT_WARNING1     0x10   //纽扣电池电压报警点1
#define PWR_BUT_WARNING2     0x20   //纽扣电池电压报警点2

#if (!defined(DEBUGBYPC))
extern __IO uint16_t Vin_Vrtc_Val[3];
extern uint8_t PowerFlags;		//检测到的电源状态
/*
    Bit0:=0,为适配器供电;=1,为电池供电
    Bit1:=1,为电池供电时,出现报警PWR_WARNING1(电压次低)
    Bit2:=1,为电池供电时,出现报警PWR_WARNING2(电压过低)

    Bit4:=1,纽扣电池出现报警PWR_WARNING1(电压次低)
    Bit5:=1,纽扣电池出现报警PWR_WARNING2(电压过低)
*/

#define BATTERYTooLow(v)  ((PowerFlags & (v))==(v))
#define BatteryVoltage()   (Vin_Vrtc_Val[0])
#define BatteryExist()   (Vin_Vrtc_Val[0]<0xfff)//检测是否有电池(4095)

void pwr_Init(void);
uint8_t pwrGetStatus(uint8_t GetStatus);
uint8_t pwrBATStatus(void);
#endif


/*---------------------------------------------------------------------------------
----------------------------------------------------------------------------------*/
void BackupReg_Init(void);
void WriteToBackupReg(uint8_t bkp_addr, uint16_t bkp_data);
uint16_t ReadFromBackupReg(uint8_t bkp_addr);



#endif	//__PWR_CTRL_H
