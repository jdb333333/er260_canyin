
/* Includes ------------------------------------------------------------------*/
#include "SysTick.h"

/* Private variables ---------------------------------------------------------*/
/**************************************************************
 * 设置延迟计数器
 *
 * @author EutronSoftware (2014-12-17)
 *
 * @param timerID:延时计时器序号(0,1,2)
 * @param msDelay:以毫秒文单位的延迟时长
 ***************************************************************/
void SetTimerDelay(int timerID,unsigned long msDelay)
{
    if (timerID==0)
        msTimer_Delay0 =msDelay;
    else if (timerID==1)
        msTimer_Delay1 =msDelay;
    else if (timerID==2)
        msTimer_Delay2 =msDelay;
}
/**
 * 读取延迟计时器的值
 *
 * @author EutronSoftware (2014-12-17)
 *
 * @param timerID:延时计时器序号(0,1,2)
 *
 * @return int延迟计时器的值
 */
unsigned long GetTimerDelay(int timerID)
{
    if (timerID==0)
        return msTimer_Delay0;
    else if (timerID==1)
        return msTimer_Delay1;
    else if (timerID==2)
        return msTimer_Delay2;
}


