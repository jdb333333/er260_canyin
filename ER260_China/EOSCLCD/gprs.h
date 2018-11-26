
#ifndef GPRS_H
#define GPRS_H

#if (defined(CASE_GPRS) && defined(CASE_QRCODE))
#error "CASE_GPRS & CASE_QRCODE"
#endif

//-------------------------------------------------------------
#define RET_OK   ('O'+('K'<<8))
#define RET_ERROR   ('E'+('R'<<8)+('R'<<16)+('O'<<24))

//----Flags for GPRS_Ready----------
#define GPRS_OK_BIT     BIT0       //用来标示GPRS是否准备好（是否存在）
#define GPRS_TRY_BIT    BIT1       //标示重复检测一次GPRS
#define GPRS_NO_BIT     BIT2       //标示检测多次，GPRS仍然没有准备好，不再进行检测
#define GPRS_NO_SIM     BIT3       //标示无SIM卡
#define GPRS_SKIP_SIM   BIT4        //不再检测SIM卡
#define GPRS_LINK_ERR    BIT5        //与服务器通讯异常
//-------------------------------------------------------------
#define RETRYTIMES  5

#if defined(CASE_INDIA)
#define ACK_MUST     1   //ccr2016-08-11数据通信时服务器和收款机之间需要进行ACK应答(印度需求,会影响效率_
#endif

#if(defined(DEBUGBYPC))
#define GPRSPORT COMPUTER_1
#else
#define GPRSPORT 2   //GPRSPORT,0,1,2
#endif

extern void mDrawGPRS(BYTE ColNum, BYTE LinNum,BYTE force);

extern void TestSendMess(void);
extern void GPRSSendECR_LOG(void);


extern void ProcessGprs(void);

BYTE GPRSStartGprs(void);

BYTE ASC_2_HEX(BYTE pASC);
void GPRSSendHEX(BYTE byte);  //send data one byte, such as  SOH and so on..
BYTE GPRSSkipReceived(ULONG mWaitFor);

short GPRSReadAString(BYTE *str,short size,ULONG sWaitFor);
// "直接从串口读取GPRS返回的数据 %IPDATA......"//
short GPRSRead_IPDATA(char *pBuff,short size,ULONG sWaitFor);
// "通过AT%IPSEND 发送数据" //

BYTE GPRSConnectAPN(void);
BYTE GPRSRegisterTCPIP(void);
BYTE GPRSConnect_TCPIP(void);
BYTE GPRSSIM_Exist(void);
void GPRS_Restart(void);
BYTE GPRSCloseTCPIP(BYTE sMode);
BYTE GPRSReset(WORD pSET);
void GPRSWaitForReady(BYTE init);
BYTE GPRSSend_ATE0(void);
BYTE GPRSCheckACK(BYTE ACKmust);
void GPRSSendECR_HeartBeat(BYTE force);//ccr2015-08-03
BYTE GPRS_ProcessRecord(char cmd,BYTE *srvData,BYTE psize);//ccr2016-08-19
BYTE GPRSWaitForOK(ULONG sWaitFor);
void GPRS_DrawSignal(void);

#define cmdUPDATE       'U'     //为更新数据请求
#define cmdDOWNLOAD     'D'     //为下载数据请求
#define cmdFLOWDATA     'F'     //为请求收款机流水数据
#define cmdANSWER       'A'     //为对host的应答命令
#define cmdGRAPHIC      'G'     //为下载图片
#define cmdCLEAR        'C'     //清除报表
#define cmdINIT         'I'     //收款机初始化
#define cmdSERVER       'S'     //设置收款机的服务器地址
#define cmdPASSWORD     'W'     //设置收款员密码

#endif
