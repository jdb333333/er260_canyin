/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2010     */
/*------------------------------------------------------------------------*/

#ifndef MONITOR_H
#define MONITOR_H

#define MONITOR_CANCEL  0
#define MONITOR_SCREEN  1
#define MONITOR_RS232   2

#define  MAX_BUFFER_SIZE             (4096)
#define  INQUIRY_LENGTH                64


#define _USE_XFUNC_OUT	1	/* 1: Use output functions To UART0 */
#define	_CR_CRLF		0	/* 1: Convert \n ==> \r\n in the output char */

#define _USE_XFUNC_IN	1	/* 1: Use input function From UART0*/
#define	_LINE_ECHO		0	/* 1: Echo back input chars in get_line function */

/* 1: use the simple string functions in monitor.c
        It results in reduced code size but low efficiency
   0: use the string functions in library, should add <string.h>
        It results in high efficiency but expanded code size
*/
#define _USE_XSTRFUNC	0

extern void (*xfunc_out)(unsigned char);//ccr20110901

#if _USE_XFUNC_OUT

void xputc (char c);
void xputs (const char* str);
void xprintf (const char* fmt, ...);
void xsprintf (char* buff, const char* fmt, ...);
void put_dump (const unsigned char * buff, unsigned long addr, int len);

#else//ccr20110901

#define  xputc(c) {}
#define  xputs(str) {}
#define  xprintf(...) {}
#define  xsprintf(...) {}
#define  put_dump(buff,addr,len) {}

#endif

extern unsigned char (*xfunc_in)(void);//ccr20110901
int xatoi (char** str, long* res);//ccr20110901

#if _USE_XFUNC_IN

int get_line (char* buff, int len);

#else//ccr20110901

#define  get_line(buff,len) {}

#endif

#if _USE_XSTRFUNC

int xstrlen (const char*);
char *xstrcpy (char*, const char*);
void *xmemset (void*, int, int);
char *xstrchr (const char*, int);

#else

#include <string.h>
#define xstrlen(x)      strlen(x)
#define xstrcpy(x,y)    strcpy(x,y)
#define xmemset(x,y,z)  memset(x,y,z)
#define xstrchr(x,y)    strchr(x,y)

#endif
//////// define in main for public /////////////////////////////////

extern char ConsoleLine[128];      /* for concel input buffer*/

extern  unsigned char   UserBuffer[MAX_BUFFER_SIZE];                /*ccr20110829 Buffer used by application                             */

//extern unsigned long TimerDelay ;		/* Used for Delay timer (1ms increment) */
//extern unsigned long RTCTimer ;        /* Timer for RTC (1ms increment)*/
#define sDEBUG(msg) UART_PutString(msg)
#define cDEBUG(c)   UART_PutChar(c)

void DisplayChar(unsigned char ch);

#endif
