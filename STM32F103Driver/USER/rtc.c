
#include "rtc.h"

/**********************************************************************************************/

/* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
//#define RTC_CLOCK_SOURCE_LSI  /* LSI used as RTC source clock. The RTC Clock */
                                /* may varies due to LSI frequency dispersion. */

void NVIC_RTCConfiguration(void);
void rtc_Config(void);


/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */

void rtc_init(void)
{
	//NVIC_RTCConfiguration();

	/* 初始化RTC */
    if (BKP_ReadBackupRegister(BKP_DR1) != 0x32F1)
    {
		/* RTC configuration  */
		rtc_Config();

		/* Adjust time by values entered by the user on the hyperterminal */
		//RTC_Time_Adjust();

		/* Indicator for the RTC configuration */
		BKP_WriteBackupRegister(BKP_DR1, 0x32F1);
    }
    else
    {
		/* Check if the Power On Reset flag is set */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			//xprintf("Reset occurred.\n");
		}
		/* Check if the Pin Reset flag is set */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			//xprintf("ExReset occurred\n");
		}

		//xprintf("No need to config RTC.\n");//No need to config

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}

	/* Clear reset flags */
	RCC_ClearFlag();
}


/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void rtc_Config(void)
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
/* The RTC Clock may varies due to LSI frequency dispersion. */
    /* Enable the LSI OSC */
    RCC_LSICmd(ENABLE);

    /* Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

#endif /* RTC_CLOCK_SOURCE_LSI */

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

#ifdef RTC_CLOCK_SOURCE_LSI
	RTC_SetPrescaler(31999);            /* RTC period = RTCCLK/RTC_PR = (32.000 KHz)/(31999+1) */
#elif defined	RTC_CLOCK_SOURCE_LSE
	RTC_SetPrescaler(32767);            /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
#endif

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_RTCConfiguration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/* Get RTC time */
void rtc_gettime (RTCTime *p_time)
{
	uint32_t tmp;
	tmp = RTC_GetCounter();
	to_tm(tmp, p_time);
}

/* Set RTC time */
void rtc_settime (RTCTime *p_time)
{

	/* Wait until last write operation on RTC registers has finished */
	//RTC_WaitForLastTask();
    rtc_Config();//ccr2014-10-29 !!!!! 避免重新设置时间时死机

	/* Get wday */
	GregorianDay(p_time);

	/* Change the current time */
	RTC_SetCounter(mktimev(p_time));

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

}

/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */
DWORD get_fattime ()
{

    RTCTime rtc;

    /* Get local time */
    rtc_gettime(&rtc);

    /* Pack date and time into a DWORD variable */
    return((DWORD)(rtc.year - 1980) << 25)
    | ((DWORD)rtc.mon << 21)
    | ((DWORD)rtc.mday << 16)
    | ((DWORD)rtc.hour << 11)
    | ((DWORD)rtc.min << 5)
    | ((DWORD)rtc.sec >> 1);

}


/*******************************************************************************************/
/*******************************************************************************************/
/*******************************************************************************************/

#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L
#define SECYR			(SECDAY * 365)
//#define	leapyear(year)  ((year%4==0) &&  ((year%100!=0) || (year%400==0))) //ccr2014-10-29 2001到2099之间不可能有此情况
#define	leapyear(year)		((year) % 4 == 0)

#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])

static int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const  int MonthOffset[] = {0,31,59,90,120,151,181,212,243,273,304,334 };

/*
 * This only works for the Gregorian calendar - i.e. after 1752 (in the UK)
 */
void GregorianDay(RTCTime * tm)
{
	int leapsToDate;
	int lastYear;
	int day;

	lastYear=tm->year-1;

	/*
	 * Number of leap corrections to apply up to end of last year
	 */
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;

	/*
	 * This year is a leap year if it is divisible by 4 except when it is
	 * divisible by 100 unless it is divisible by 400
	 *
	 * e.g. 1904 was a leap year, 1900 was not, 1996 is, and 2000 will be
	 */
	if(leapyear(tm->year) && (tm->mon>2)) {
		/*
		 * We are past Feb. 29 in a leap year
		 */
		day=1;
	} else {
		day=0;
	}

	day += lastYear*365 + leapsToDate + MonthOffset[tm->mon-1] + tm->mday;

	tm->wday=day%7;
}

/* Converts Gregorian date to seconds since 1970-01-01 00:00:00.
 * Assumes input in normal date format, i.e. 1980-12-31 23:59:59
 * => year=1980, mon=12, day=31, hour=23, min=59, sec=59.
 *
 * [For the Julian calendar (which was used in Russia before 1917,
 * Britain & colonies before 1752, anywhere else before 1582,
 * and is still in use by some communities) leave out the
 * -year/100+year/400 terms, and add 10.]
 *
 * This algorithm was first published by Gauss (I think).
 *
 * WARNING: this function will overflow on 2106-02-07 06:28:16 on
 * machines were long is 32-bit! (However, as time_t is signed, we
 * will already get problems at other places on 2038-01-19 03:14:08)
 */
u32 mktimev(RTCTime *tm)
{
	if (0 >= (int) (tm->mon -= 2)) {	/* 1..12 -> 11,12,1..10 */
		tm->mon += 12;		/* Puts Feb last since it has leap day */
		tm->year -= 1;
	}

	return (((
		(u32) (tm->year/4 - tm->year/100 + tm->year/400 + 367*tm->mon/12 + tm->mday) +
			tm->year*365 - 719499
	    )*24 + tm->hour /* now have hours */
	  )*60 + tm->min /* now have minutes */
	)*60 + tm->sec; /* finally seconds */
}
//1-星期一,...,6-星期六,7-星期日 //ccr2014-10-14
void to_tm(u32 tim, RTCTime * tm)
{
	register u32    i;
	register long   hms, day;

	day = tim / SECDAY;
	hms = tim % SECDAY;

	/* Hours, minutes, seconds are easy */
	tm->hour = hms / 3600;
	hms %= 3600;
	tm->min = hms / 60;
	tm->sec = hms % 60;

	/* Number of years in days */
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->year = i;

	/* Number of months in days left */
	if (leapyear(tm->year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->mon = i;

	/* Days are what is left over (+1) from all that. */
	tm->mday = day + 1;

	/*
	 * Determine the day of week
	 */
	GregorianDay(tm);
    if (tm->wday==0) tm->wday=7;//ccr2014-10-14
}



