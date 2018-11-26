
#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"
#include "TypeDef.h"

/*ccr2014-10-14
struct rtc_time {
	int sec;
	int min;
	int hour;
	int mday;
	int mon;
	int year;
	int wday;
};
*/

void rtc_init(void);
void rtc_gettime (RTCTime *p_time);
void rtc_settime (RTCTime *p_time);

DWORD get_fattime ();

/*******************************************************************************************/
/*******************************************************************************************/
/*******************************************************************************************/

void GregorianDay(RTCTime * tm);
uint32_t mktimev(RTCTime *tm);
void to_tm(uint32_t tim, RTCTime * tm);


#endif //__RTC_H


