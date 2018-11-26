/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__SYSTICK_H
#define	__SYSTICK_H

/* Includes ------------------------------------------------------------------*/

#define uSECOND 1      //uSECOND = SystemCoreClock / 2000000;

/* Exported functions ------------------------------------------------------- */
#define PRNPAUSEMAX  30000           //打印机过热时,暂停30秒
extern  volatile unsigned long msTimer_Delay0,msTimer_Delay1,msTimer_Delay2;
extern  volatile unsigned long RTCTimer;
extern unsigned short Prn_Delay;          //打印机暂停时长

#define GetSystemTimer()  RTCTimer
extern void SetTimerDelay(int timerID,unsigned long msDelay);
extern unsigned long GetTimerDelay(int timerID);

#define SetTimerDelay0(msDelay)   msTimer_Delay0=msDelay
#define SetTimerDelay1(msDelay)   msTimer_Delay1=msDelay
#define SetTimerDelay2(msDelay)   msTimer_Delay2=msDelay

#define GetTimerDelay0()   msTimer_Delay0
#define GetTimerDelay1()   msTimer_Delay1
#define GetTimerDelay2()   msTimer_Delay2

#endif /* __SYSTICK_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
