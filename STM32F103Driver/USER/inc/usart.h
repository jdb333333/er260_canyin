#ifndef __USART_H
#define	__USART_H

#include <stdio.h>

#define USART_EXTEND_EN   1   //1：启用内部串口UART2(即USART2); 0:不启用

#define UART0	USART1	//外部串口RS232-I
#define UART1	USART3  //外部串口RS232-II
#define UART2 USART2  //内部串口

#define LOGIC_COM1   1  //UART0,外部RS232-I
#define LOGIC_COM2   2  //UART1,外部RS232-II
#define LOGIC_COM3   3  //UART2,内部串口(MFRICard)

#define NUMPORT     (2+USART_EXTEND_EN)

//  1ms?????
#define PROG1ms     15000

//


/** UART time-out definitions in case of using Read() and Write function
 * with Blocking Flag mode
 */
#define UART_BLOCKING_TIMEOUT			(0xFFFFFFFFUL)


/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PORT_DEBUG      1
#define DELAYHOST       2000
#if defined(CASE_GPRS)//ccr2016-10-27>>>>>>
#define rxBUFSIZE	    0x820
#else
#define rxBUFSIZE	    0x400
#endif//ccr2016-10-27<<<<<<<
#define XOFF_AT         (rxBUFSIZE*3/4)  //Send XOFF to host when the buffer is full.
#define XON_AT          (rxBUFSIZE/2)    //Send XON to host when the bufer is empty
/* flags for Status */
#define XOFF_FLOWCTRL   BIT0    /*使用XON/XOFF握手协议*/
#define RTS_FLOWCTRL    BIT1    /*使用RTS握手协议*/
#define XOFF_Flag       BIT6    /*已发送XOFF*/
#define RTS_Flag        BIT7    /*已发送XOFF*/
//........................................
typedef struct UARTRECORD {       /* ----------- HostController EndPoint Descriptor ------------- */
    uint16_t  Status;             /* flags for rs232 */
    uint32_t  TimesOut;           /* Times out for read */
    uint32_t  PutIn;              /* address for save in RxBuffer    */
    uint32_t  GetOut;             /* address for get data from RxBuffer     */
    uint8_t RxBuffer[rxBUFSIZE];  /* Buffer for store data fron UART           */
} TUART;

extern uint32_t TimerDelay;

extern volatile uint8_t UART0TxEmpty;
extern volatile uint8_t UART1TxEmpty;
extern volatile uint8_t UART2TxEmpty;
extern volatile TUART RxUART[NUMPORT];
extern volatile TUART RxUSB;//ccr2016-04-14

/* external functions */
void    UART_PutChar (uint8_t);
uint8_t UART_GetChar (void);
uint8_t UART_GetChar_nb (uint8_t *ch);

void  	USART_printf (const uint8_t *format, ...);


uint32_t UARTInit( uint32_t PortNum, uint32_t baudrate,uint8_t Databits,uint8_t Parity,uint8_t Stopbits );

void UARTSend( uint8_t portNum, uint8_t ch );
void UARTSendStr( uint8_t portNum, uint8_t *BufferPtr, uint32_t Length );
uint8_t UARTGet(uint8_t portNum, uint8_t *ch, uint32_t sdelay);

//int fputc(int ch, FILE *f);
//void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
