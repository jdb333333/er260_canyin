#include "King.h"
#include "Exth.h"
#include "Exthead.h"
#include "string.h"

#if(defined(CASE_GPRS))

#define WAITFOROK   1   //ccr2017-09-29从收到的数据中滤除与"OK"和"ERROR"无关的数据

#include "Gprs.h"

///////////////////////////////////////////////////////////////////////////////
#if (!defined(DEBUGBYPC) && defined(FOR_DEBUG))
#define DEBUGGPRS    //ccr2015-08-21测试数据发送正确性
#endif

#include "message.h"

#if defined(DEBUGGPRS)
#define GPRSECH_SEND(a) UARTSend(LOGIC_COM1,a)//将发送给GPRS的通讯数据从串口1回显
#define GPRSECH_READ(a) UARTSend(LOGIC_COM1,a)//将从GPRS收到的通讯数据从串口1回显
#else
#define GPRSECH_SEND(a) {}//将发送给GPRS的通讯数据从串口1回显
#define GPRSECH_READ(a) {}//将从GPRS收到的通讯数据从串口1回显
#endif

#define MAXLOGPACKAGE      //ccr2015-08-25将流水数据以组包的形式发送
#define GPRSWAITACK     //ccr2015-08-25当发送区满时,等待数据发送完毕
///////////////////////////////////////////////////////////////////////////////

unsigned long GPRS_Wait=0;//ccr2015-03-11下一次尝试GPRS通讯的时间

int PackageReceived=0;

BYTE GPRS_Ready=0;//用来标示GPRS模块的状态

#if defined(ACK_MUST)
BYTE GPRS_WaitACK=0;
#endif

CONST char ATE0[]="ATE0\r";//关闭回显
CONST char GPRSReady[]="AT-Command Interpreter ready\r\n";
//GPRSReady不要超过40,超过40时需要修改GPRSWaitForReady
///////////////////////////////////////////////////////////////////////////////

/**
 * 开机后根据GPRS模块是否返回GPRSReady[]中的信息判断GPRS是否准备好
 *
 * @author EutronSoftware (2014-12-11)
 * @param init:true-进行初始化
 */
void GPRSWaitForReady(BYTE init)
{
    static int rdyPOS=0;
    short j;

    //ccr2015-03-11>>>>>>>>>>>>>>
    if (init)
    {
        rdyPOS = 0;
        GPRS_Wait = GetSystemTimer()+SECONDS(ACTIVE*60);//等待5分钟
        GPRS_Ready = 0;
        GPRSReset(0);
        return;
    }//ccr2015-03-11<<<<<<<<<<<<<<<
    if (!TESTBIT(GPRS_Ready,GPRS_OK_BIT+GPRS_NO_BIT+GPRS_NO_SIM) && GetSystemTimer()>GPRS_Wait)//ccr2015-03-11
    {
        mDrawGPRS(SCREENWD-1,0,0);//在右上角闪烁显示标示
        if (rdyPOS>40)//strlen(GPRSReady))
        {
            if (!TESTBIT(GPRS_Ready,GPRS_SKIP_SIM) && (RTCTimer-rdyPOS)>SECONDS(5))
            {//没有检测过SIM卡
                SETBIT(GPRS_Ready,GPRS_SKIP_SIM);
                if (!GPRSSIM_Exist())
                {
                    SETBIT(GPRS_Ready,GPRS_NO_BIT+GPRS_NO_SIM);
                    ApplVar.ErrorNumber =  ERROR_ID(CWXXI88);
//ccr2016-07-27                    DispSt1(GPRSMess[G_WAITFOR].str);
//ccr2016-07-27 避免重复检测                    GPRSWaitForReady(true);//ccr2015-03-11
                    return;
                }
            }

            if ((RTCTimer-rdyPOS)>SECONDS(20))
            {//GPRS模块需要再等待约20秒才能够进入TCP/IP模式
                Bell(1);
                SETBIT(GPRS_Ready,GPRS_OK_BIT);
                RESETBIT(GPRS_Ready,GPRS_NO_BIT);
                mSetInvAttr();
                DispSt1(GPRSMess[G_GPRSOK].str);
                mClearInvAttr();
                EmptyComm(GPRSPORT);
                rdyPOS = 0;
#if defined(CASE_INDONESIA)
                GPRSSendECR_HeartBeat(0xff);//ccr2015-08-04
#endif
            }
            return;
        }
        //等待GPRS模块应答"AT-Command Interpreter ready\r\n"
        while (!TESTBIT(GPRS_Ready,GPRS_OK_BIT) && CheckComm(GPRSPORT))
        {
            j = ReadComm(GPRSPORT);
            if (j==-1)
                break;
            else
            {
                j &= 0xff;
                GPRSECH_READ(j);//testonly
                if (j==GPRSReady[rdyPOS])
                {
                    rdyPOS++;
                    if (rdyPOS>=strlen(GPRSReady))
                    {
    //                    Bell(1);
    //                    SETBIT(GPRS_Ready,GPRS_OK_BIT);
    //                    DispSt1(GPRSMess[G_GPRSOK].str);
                        rdyPOS = RTCTimer;
                    }
                }
            }
        }
    }
}
/**
 * 向GPRS模块发送一个字节数据
 *
 * @author EutronSoftware (2014-12-15)
 *
 * @param byte
 */
void GPRSSendByte(BYTE byte)
{
    Bios_PortWrite(GPRSPORT+1, &byte, 1, 1);
    GPRSECH_SEND(byte);//testonly
}

//CONST char AtIpdr[]="AT%IPDR\r\n";
//CONST char AtIpdd[]="AT%IPDD\r\n";
//CONST char AtIpdq[]="AT%IPDQ\r\n";

/**
 *
 * 发送字符串数据
 * @author EutronSoftware (2014-12-15)
 *
 * @param str :需要发送的字符串数据
 * @param len :字符串的长度.=-1时,函数自行计算长度,此时的字符串必须以'\n'结束
 */
void GPRSSendAString(CONST char *str,short len)
{
    short i;
    GPRSECH_SEND('>');
    if (len==-1)
        len=strlen(str);
    for (i=0;i<len;i++)
        GPRSSendByte(str[i]);
}
//将str中的数据以HEX的方式发送
void GPRSSendHEXString(char *str,short len)
{
    short i;
    for (i=0;i<len;i++)
        GPRSSendHEX(str[i]);
}


//---------------------------------------------------------------------------
#define WAITFORTYPE 0//等待数据类型:'F'-为流水,'U'为请求
#define WAITFORSIZE 1
#define WAITFORDATA 2


/**
 *  @author EutronSoftware (2014-12-06)
 * 从收到的数据中解析一条ECR文件数据,解析结果存入EcrRecord指定的存储区
 *  data记录格式:[Type]+[size]+[data],  size为data的字节数
 *
 * @param command:需要解析的命令('U'...);=0时,初始化
 *        data:收到的数据,data[0]为命令,data[1]为size,data[2..3]为数据类型(功能码)
 *        size:返回收到的数据长度
 *
 * @return
 *         WORD:=0xffff时,无完整数据;=0时,数据结束;其它值:为收到的数据对应的功能码
 *         EcrRecord[0]+EcrRecord[1]:存放功能码
 *         EcrRecord+2:为对应功能码的数据
 */
#define EcrRecord PCBuffer

WORD GPRSDecodeECRRecord(BYTE command,BYTE data,BYTE *size)
{
    //Variable for GPRSGetECRARecord
    static BYTE GPRS_Command;//收到的数据类型;'F','U','D'....
    static BYTE GPRS_SizeMust;//必须收取的数据数目
    static int GPRS_BytesReceived;//记录已经收到的数据数目
    static BYTE GPRS_WaitFor=WAITFORTYPE;//=0,表示等待数据类型;=1标示等待长度字节;=2表示等待数据

    int i;

    if (command==0)
    {//initial
        GPRS_WaitFor=WAITFORTYPE;
        GPRS_BytesReceived=0;
    }
    else
    {
        switch (GPRS_WaitFor)
        {
        case WAITFORTYPE://等待数据类型
           if (data=='U')//为传送流水和请求文件命令
           {
               GPRS_Command = 'U';
               GPRS_WaitFor = WAITFORSIZE;
               GPRS_BytesReceived = 0;
           }
           break;
       case WAITFORSIZE://等待数据长度,最长256字节
           //if (data<=sizeof(union ECRFILEREC))
           {//为文件数据
                GPRS_SizeMust = data;
                GPRS_BytesReceived = 0;
                GPRS_WaitFor = WAITFORDATA;
           }
           //else//非文件数据
           //     GPRS_WaitFor = WAITFORTYPE;
           break;
       case WAITFORDATA://等待收取数据
           EcrRecord[GPRS_BytesReceived++]=data;
           if (GPRS_BytesReceived==GPRS_SizeMust)
           {//收取到一个完整的数据包
               PackageReceived++;
               GPRS_WaitFor = WAITFORTYPE;

               if (GPRS_Command=='U')
               {//数据收取完毕
                   if (CWORD(EcrRecord[0])==0)
                       return 0;
                   else// if (CWORD(EcrRecord[0])>CLERK && CWORD(EcrRecord[0])<PLU3 || CWORD(EcrRecord[0])>=RHEAD1 && CWORD(EcrRecord[0])<=RTRAIL2)
                   {
                       if (size)
                           *size = GPRS_SizeMust;
                       return CWORD(EcrRecord[0]);
                   }
               }
               else
                   return 0xffff;//解析了一部分数据
           }
           break;
       }
       return 0xffff;//解析了一部分数据
    }
}

/*****************判断缓存中是否含有指定的字符串函数******************
函数原型:BYTE GPRSHand(unsigned char *a)
函数功能:判断缓存中是否含有指定的字符串
输入参数:unsigned char *a 指定的字符串
输出参数: true---含有    false---不含有
调用模块:无
**********************************************************************/
BYTE GPRSHand(char *rec_buf,char *ret)
{
    int i;
    int len=strlen(ret);

    for (i=0;i<len;i++)
    {
        if (rec_buf[i]!=ret[i])
            return false;
    }
    return true;
}

BYTE ASC_2_HEX(BYTE pASC)
{
    if (pASC >= '0' && pASC <= '9')
    {
        return(pASC & 0x0f);
    }
    else if ((pASC >= 'A' && pASC <= 'F') || (pASC >= 'a' && pASC <= 'f'))  //A....F
    {
        return(pASC & 0x0f)+9;
    }
    return 0;
}

void GPRSSendHEX(BYTE byte)  //send data one byte, such as  SOH and so on..
{
    BYTE sCh;

    sCh = byte>>4;
    if (sCh>9)
        sCh += 'A'-10;
    else
        sCh += '0';
    GPRSSendByte(sCh);

    sCh = byte & 0x0f;
    if (sCh>9)
        sCh += 'A'-10;
    else
        sCh += '0';
    GPRSSendByte(sCh);
}
// 忽略GPRS模块返回的数据,当向GPRS模块发送一条命令时,GPRS模块会立即把此命令回送,必须忽略此回送 //
// mWaitFor:等待时间=mWaitFor 秒
// return=0--出错,  =1 正常 //
BYTE GPRSSkipReceived(ULONG mWaitFor)
{
    short j;
    BYTE sEnd=0;

    SetTimerDelay0(mWaitFor);
    while (GetTimerDelay0())
    {
        j = ReadComm(GPRSPORT);
        if (j!=-1)
        {
            GPRSECH_READ(j);//testonly

            if (j=='\r')
                sEnd = '\r';
            else if (j=='\n' &&  sEnd == '\r')
                return true;
            SetTimerDelay0(mWaitFor);
        }
    }
    return false;
}
// empty the comm..skip the string
void GPRSSkipQuick()
{
    short retry,j;
    BYTE sEnd=0;

    while (1)/*for (retry=0; retry<RETRYTIMES; )*/
    {
        j = ReadComm(GPRSPORT);
        if (j!=-1)
        {
            GPRSECH_READ(j);//testonly

            if (j=='\r')
                sEnd = '\r';
            else if (j=='\n' &&  sEnd == '\r')
                return;
        }
        else return;
    }
}
/**
 *
 *
 * @author EutronSoftware (2014-12-15)
 *
 * @param str :读取数据的存放区
 * @param size :需要读取的数据长度
 * @param sWaitFor :等待时长(秒)
 *
 * @return short:=-1,超时,>=0,收到数据
 */
short GPRSReadAString(BYTE *str,short size,ULONG sWaitFor)
{
    short i,j;
    BYTE sEnd=0;

    i =  0;
    SetTimerDelay0(sWaitFor);
    while (i<size && GetTimerDelay0()>0)
    {
        j = ReadComm(GPRSPORT);
        if (j!=-1)
        {
            GPRSECH_READ(j);//testonly

            SetTimerDelay0(sWaitFor);
            if (j=='\r')
            {
                sEnd = '\r';
                str[i]=0;
            }
            else if (j=='\n')
            {
                if (sEnd == '\r')
                    return i;
                else
                {
                    continue;
                    //return 0;
                }
            }
            else
                str[i++]=j & 0xff;
        }
    }
    return -1;
}
// connect to APN,Which is a bridge between GPRS Gateway and INTERNET Gateway..
// return !0, success, 0, failed;
/*CONST char AtSetBaudrate[]="AT+IPR=115200\r\n";
BYTE GPRSSetComm()
{
    GPRSSendAString(AtSetBaudrate,strlen(AtSetBaudrate));
    GPRSSkipReceived();
    if(GPRSReadAString(PCBuffer,sizeof(PCBuffer))>0)
    {
        return (PCBuffer[0]=='O'&&PCBuffer[1]=='K');
    }
    else
        return FALSE;
}*/

//  检测是否插有SIM卡
// return 	!0-success,	0-failed;
CONST char AT_SIM[]="AT%TSIM\r\n";
BYTE GPRSSIM_Exist()
{
    char readGPRS[32];
    BYTE simOK=false;

    short i,j;

    if (GPRSSend_ATE0())
    {
        GPRSSendAString(AT_SIM,strlen(AT_SIM));

        for (i=0;i<6;i++)
        {
            if ((j=GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(10)))>=0)
            {
                if (CWORD(readGPRS[0])==RET_OK)
                    return simOK;
                if (GPRSHand(readGPRS,"%TSIM 0"))
                    simOK = false;
                else if (GPRSHand(readGPRS,"%TSIM 1"))
                    simOK = true;
            }
            else
                break;
        }
    }
    return simOK;
}

/**
 * 关闭GPRS模块的回显功能
 *
 * @author EutronSoftware (2014-12-10)
 *
 * @return BYTE
 */
BYTE GPRSSend_ATE0()
{
    short i;
    char readGPRS[16];

    GPRSSendAString(ATE0,strlen(ATE0));
#if (WAITFOROK)
    return GPRSWaitForOK(SECONDS(10));
#else
    if (!GPRSSkipReceived(SECONDS(10)))//SKIP "ATE0" or '\r\n'
        return FALSE;
    if (GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(5))>0)
    {
        return(CWORD(readGPRS[0])==RET_OK);
    }
    else
        return FALSE;
#endif
}

CONST char ATCONNECTAPN[]="AT+CGDCONT=1,\"ip\",\"cmnet\"\r\n";
BYTE GPRSConnectAPN()
{
    char readGPRS[32];

    if (ApplVar.IP.APN[0])
    {//ccr2015-03-10>>>>> 发送用户名称和登录密码
        GPRSSendAString(ATCONNECTAPN,19);
        GPRSSendAString(ApplVar.IP.APN,strlen(ApplVar.IP.APN));
        GPRSSendAString("\"\r\n",3);
    }
    else//<<<<<无须APN名
        GPRSSendAString(ATCONNECTAPN,strlen(ATCONNECTAPN));

    if (!GPRSSkipReceived(SECONDS(5)))//*/!GPRSReadAString(PCBuffer, sizeof(PCBuffer))>=0)
        return FALSE;
    if (GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(5))>0)
        return(readGPRS[0]=='O' && readGPRS[1]=='K');
    else
        return FALSE;
}


//  get a virtual ApplVar.IP from GPRS Server..
// AT%ETCPIP="username","password"
// return 	!0-success,	0-failed;
CONST char AtEtcpip[]="AT%ETCPIP=\"\",\"\"\r\n";//AT%ETCPIP="",""\r\n

BYTE GPRSRegisterTCPIP()
{
    char readGPRS[32];

    short i=strlen(AtEtcpip);

    if (ApplVar.IP.UserName[0])
    {//ccr2015-03-10>>>>> 发送用户名称和登录密码
        GPRSSendAString(AtEtcpip,11);
        GPRSSendAString(ApplVar.IP.UserName,strlen(ApplVar.IP.UserName));
        if (ApplVar.IP.Password[0])
        {//有密码
            GPRSSendAString(AtEtcpip+11,3);
            GPRSSendAString(ApplVar.IP.Password,strlen(ApplVar.IP.Password));
            GPRSSendAString(AtEtcpip+14,i-14);
        }
        else//无密码
            GPRSSendAString(AtEtcpip+11,i-11);
    }
    else//<<<<<无须用户名和密码
        GPRSSendAString(AtEtcpip,i);

#if (WAITFOROK)
    return GPRSWaitForOK(SECONDS(10));
#else
    if (!GPRSSkipReceived(SECONDS(10)))//!GPRSReadAString(readGPRS, sizeof(readGPRS)))
        return false;

    if (GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(5))>0)
    {
        if (CWORD(readGPRS[0])==RET_OK)
            return TRUE;
        if (GPRSHand(PCBuffer,"ERROR"))
            return FALSE;
    }
    return FALSE;
#endif
}

CONST char ATIPOPEN[]="AT%IPOPEN=\"TCP\",\"";   //211.142.221.197\",10085\r\n";

// return =false,连接失败; =true连接成功 /
// GPRSConnect_TCPIP("192.168.0.1","10085");
BYTE GPRSConnect_TCPIP(/*CONSTCHAR *pIP,CONSTCHAR *pPort*/)
{
    short len,retry=0;
    short i;


    strcpy(PCBuffer,ATIPOPEN);len=strlen(ATIPOPEN);//get the TCP/IP command
    NumToIP(PCBuffer+len,ApplVar.IP.ServerIP);  //get the ServerIP..
    len = strlen(PCBuffer);
    PCBuffer[len++]='"';
    PCBuffer[len++]=',';
    len += WORDtoASCL(PCBuffer+len,ApplVar.IP.ServerPort);//get the serverPort..
    PCBuffer[len++]='\r';
    PCBuffer[len++]='\n';
    GPRSSendAString(PCBuffer,len);

    if (!GPRSSkipReceived(SECONDS(60)))//!GPRSReadAString(PCBuffer, sizeof(PCBuffer))>=0)
        return false;

    memset(PCBuffer,0,sizeof(PCBuffer));
    while (retry<RETRYTIMES)
    {
        if (GPRSReadAString(PCBuffer,sizeof(PCBuffer),SECONDS(60))>=0)
        {
            if (GPRSHand(PCBuffer,"CONNECT"))
            {
                GPRSSkipReceived(SECONDS(30));
                EmptyComm(GPRSPORT);
                return TRUE;
            }
            else  if (GPRSHand(PCBuffer,"ERROR"))
                    return FALSE;
                else
                    retry++;
        }
        else
            return FALSE;
    }
    return FALSE;
}

//关闭GPRS连接
BYTE CloseGPRS()
{
    if (GPRSCloseTCPIP(2)) // AT%IPCLOSE
        return TRUE;
    else
        EmptyComm(GPRSPORT);
    return FALSE;
}

//CONST char ServerIP[]="211.142.221.197";	//should be seted;
//CONST char ServerPORT[]="10085";			// the same as above;
BYTE GPRSStartGprs()
{
   BYTE flag=0;

   if (ApplVar.CentralLock==SET)
   {//设置模式下,全屏幕显示信息
        mClearScreen();
        {
            DispStrXY(GPRSMess[G_TOCONNECTING].str,0,0);
            EmptyComm(GPRSPORT);
            if (GPRSSend_ATE0()) //Make sure that when connect to Server the gprs are unconnect.
            {
                if ((ApplVar.IP.ServerIP[0] | ApplVar.IP.ServerIP[1] | ApplVar.IP.ServerIP[2] | ApplVar.IP.ServerIP[3])==0
                    || ApplVar.IP.ServerPort==0)
                {//没有设置服务器IP和端口
                    DispStrXY(GPRSMess[G_SERVERIP_NULL].str,0,1);
                    return false;
                }
                else if (GPRSConnectAPN() && GPRSRegisterTCPIP())
                {
                    if (GPRSConnect_TCPIP())
                    {//成功连接到服务器
                        EmptyComm(GPRSPORT);
                        return TRUE;
                    }
                    else
                    {//无法连接服务器
                        CloseGPRS();
                        DispStrXY(GPRSMess[G_X_IPPORT].str,0,2);
                    }
                }
                GPRSReset(SET);//ccr2015-03-11 复位一次
                GPRS_Wait = GetSystemTimer()+SECONDS(50);//ccr2015-03-11
            }
            else
                GPRSWaitForReady(true);//ccr2015-03-11

        }
        DispStrXY(GPRSMess[G_LIANJIESERVERSHIBAI].str,0,1);

        return false;
    }
   else
   {//销售模式下,在状态行显示信息
#if (SCRSTATDBL)
        if (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG)
            DispOnStatDBL(GPRSMess[G_TOCONNECTING].str,0);
        else
#else
            DispSt1(GPRSMess[G_TOCONNECTING].str);//RG-DISPLAY
#endif
        EmptyComm(GPRSPORT);
        if (GPRSSend_ATE0()) //Make sure that when connect to Server the gprs are unconnect.
        {
            if ((ApplVar.IP.ServerIP[0] | ApplVar.IP.ServerIP[1] | ApplVar.IP.ServerIP[2] | ApplVar.IP.ServerIP[3])==0
                || ApplVar.IP.ServerPort==0)
            {//没有设置服务器IP和端口
                ApplVar.ErrorNumber = ERROR_ID(CWXXI87);
                return false;
            }
            else if (GPRSConnectAPN() && GPRSRegisterTCPIP())
            {//成功连接到服务器
                if (GPRSConnect_TCPIP())
                {//成功连接到服务器
                    EmptyComm(GPRSPORT);
                    return TRUE;
                }
                else
                {//无法连接服务器,等待5分钟后再试
                    CloseGPRS();
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI86);
                    GPRS_Wait = GetSystemTimer()+SECONDS(ACTIVE*60);//ccr2015-03-11
                    ClsArea(PROMDISPX,SCREENLN,SCREENWD-PROMDISPX);
                    return false;
                }
            }
            //无法建立通讯,等待5分钟后再试
            GPRS_Wait = GetSystemTimer()+SECONDS(ACTIVE*60);//ccr2015-03-11
#if (0)//ccr2015-03-11>>>>>
            if (!TESTBIT(GPRS_Ready,GPRS_TRY_BIT))
            {
                GPRSReset(0);
                RESETBIT(GPRS_Ready,GPRS_OK_BIT);//重复检测一次
                SETBIT(GPRS_Ready,GPRS_TRY_BIT);
                return false;
            }
            else
                SETBIT(GPRS_Ready,GPRS_NO_BIT);//标示无GPRS模块
#endif//ccr2015-03-11<<<<<<<
        }
        else//GPRS模块无应答,GPRS模块异常
        {//复位5分钟后再试
            GPRSWaitForReady(true);//ccr2015-03-11
//ccr2015-03-11            RESETBIT(GPRS_Ready,GPRS_OK_BIT);
//ccr2015-03-11            SETBIT(GPRS_Ready,GPRS_NO_BIT);//标示无GPRS模块
        }
        ApplVar.ErrorNumber=ERROR_ID(CWXXI83);
        ClsArea(PROMDISPX,SCREENLN,SCREENWD-PROMDISPX);
        return FALSE;
   }
}
//------------------------------------------------------------
#define WAITFOR_IPDATA  0
#define WAITFOR_LENGTH  1
#define WAITFOR_QUOTES  2
#define WAITFOR_DATA    3
#define ENDOF_READ      4
/**
 * 从串口读取GPRS返回的数据 %IPDATA,xx,"12345678",[0d][0a]
 *
 * @author EutronSoftware (2014-12-08)
 *
 * @param pBuff :读取的数据存放区;=0时,初始化GPRSRead_IPDATA
 * @param size :试图读取的字节数目
 *
 * @return short :读取的字节数目;=0时,超时;=-1时,接收数据错
 */
short GPRSRead_IPDATA(char *pBuff,short size,ULONG sWaitFor)
{

    short dataLength,ch;
    char getIPDATA[16];

    short Read_MAX;//GPRS模块收到的数据数目

    BYTE sByte,Read_CR;
    BYTE sBit=0;
    BYTE Read_WaitFor=0xff;// =0 分析数据头 "%IPDATA", =1 分析数据长度; =2,3 分析数据 //

    if (pBuff!=0 && size!=0)
    {
        dataLength= 0;
        SetTimerDelay0(sWaitFor);
        while (GetTimerDelay0())
        {
            ch = ReadComm(GPRSPORT);
            if (ch!=-1)
            {
                GPRSECH_READ(ch);//testonly
                switch (Read_WaitFor)
                {
                case WAITFOR_IPDATA:
                    getIPDATA[dataLength++]=ch;
                    if (ch==':')
                    {
                        getIPDATA[dataLength]=0;
                        if (strcmp(getIPDATA,"%IPDATA:")==0)//	change by lqw %IPDATA->%IPDATA:
                        {
                            sBit = 0;
                            Read_WaitFor = WAITFOR_LENGTH;
                            dataLength= Read_MAX = 0;
                        }
                        else
                            return -1;// 返回的数据出错 //
                    }
                    else if (dataLength>8)
                        return -1;// 返回的数据出错 //
                    break;
                case WAITFOR_LENGTH:
                    if (ch==',')
                        Read_WaitFor = WAITFOR_QUOTES;
                    else if (ch>='0' && ch<='9')
                        Read_MAX = Read_MAX*10 + (ch & 0x0f);// 获取数据长度 //
                    break;
                case WAITFOR_QUOTES:
                    if (ch=='"')
                    {
                        dataLength = 0;
                        Read_WaitFor = WAITFOR_DATA;
                        sBit = 0;
                    }
                    break;
                case WAITFOR_DATA:
                    if (ch=='"')
                        Read_WaitFor = ENDOF_READ;
                    else
                    {
                        if (sBit)
                        {
                            if (dataLength<size)
                                pBuff[dataLength++] = (sByte<<4)+ASC_2_HEX(ch);
                            //if (dataLength>=Read_MAX) return dataLength;// 数据有错 //
                        }
                        else
                            sByte = ASC_2_HEX(ch);

                        sBit ^= 1;
                    }
                    break;
                case ENDOF_READ://ccr2016-10-25>>>>>>>>>>
                    //等待回车+换行
                    if (ch=='\r')
                        Read_CR = '\r';
                    else //if (ch=='\n')
                        return dataLength;
                    break;
                default:
                    if (ch=='%')
                    {//已经读取%IPDATA后的全部数据
                        Read_WaitFor=WAITFOR_IPDATA;// =0 分析数据头 "%IPDATA", =1 分析数据长度; =2,3 分析数据 //
                        dataLength= 0;
                        getIPDATA[dataLength++]=ch;
                    }
                    break;
                }
                SetTimerDelay0(sWaitFor);
            }
        }
        return dataLength;
    }
    else
        return 0;
}

CONST char ATRST[]="AT%RST\r\n";
/**
 * 复位GPRS模块,执行复位操作后,必须等待约20秒,让GPRS进入正常状态
 *
 * @author EutronSoftware (2014-12-10)
 *
 * @return BYTE
 */
BYTE GPRSReset(WORD pSET)
{
    char readGPRS[32];

    GPRSSendAString(ATRST,strlen(ATRST));

    if (ApplVar.CentralLock==pSET)
    {
        mClearScreen();
        DispStrXY(GPRSMess[G_FUWEIMOKUAI].str,0,0);
        GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(10));//AT-Command Interpreter ready
        EmptyComm(GPRSPORT);
//ccr2015-03-11        Delay(SECONDS(20));
    }
    else
#if (SCRSTATDBL)
        if (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG)
            DispOnStatDBL(GPRSMess[G_FUWEIMOKUAI].str,0);
        else
#else
            DispSt1(GPRSMess[G_FUWEIMOKUAI].str);//RG-DISPLAY
#endif

    return TRUE;
}

CONST char ATIPCLOSE[]="AT%IPCLOSE\r\n";
CONST char ATIPCLOSE1[]="AT%IPCLOSE=1\r\n";
CONST char ATIPCLOSE5[]="AT%IPCLOSE=5\r\n";
BYTE GPRSCloseTCPIP(BYTE sMode)
{

    short i,retry;
    if (sMode==1)
    {
        GPRSSendAString(ATIPCLOSE1,strlen(ATIPCLOSE1));
    }
    else  if (sMode==5)
    {
        GPRSSendAString(ATIPCLOSE5,strlen(ATIPCLOSE5));
    }
    else
    {
        GPRSSendAString(ATIPCLOSE,strlen(ATIPCLOSE));
    }

#if (WAITFOROK)
    return GPRSWaitForOK(SECONDS(10));
#else
    if (!GPRSSkipReceived(SECONDS(10)))//!GPRSReadAString(PCBuffer, sizeof(PCBuffer))>=0)
        return false;

    memset(PCBuffer,0,sizeof(PCBuffer));
    for (retry=0;retry<4;retry++)
    {
        if (GPRSReadAString(PCBuffer,sizeof(PCBuffer),SECONDS(10))>0)
        {
            if (GPRSHand(PCBuffer,"%IPCLOSE") || GPRSHand(PCBuffer,"AT%IPCLOSE"))
                continue;
            else  if (GPRSHand(PCBuffer,"ERROR"))
                return FALSE;
            else if (CWORD(PCBuffer[0])==RET_OK) // read "OK"
                return true;// 收到 "OK"
        }
    }
    return FALSE;
#endif
}


///////////ccr2014-11-10>>>>>>>>>>>>>>>>////////////////////////////////////////

BYTE SendATCommand(char *command)
{
    char readGPRS[16];

    GPRSSendAString(command,-1);
#if (WAITFOROK)
    return GPRSWaitForOK(SECONDS(10));
#else
    GPRSSkipReceived(SECONDS(15));//skip 0d 0a
    if (GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(10))>0)
    {
        if (CWORD(readGPRS[0])==RET_OK) // read "OK"
            return true;// 收到 "OK"
    }
    return FALSE;
#endif
}

/**ccr2014-11-26
 * 测试发送一条短消息
 * ATE0[0d]
 * AT+CMGF=1[0d]
 * AT+CMGS=13627423241[0d]
 * Welcome to RUTRON[1a][0d]
 *
 * @author EutronSoftware (2014-11-10)
 */
void TestSendMess()
{
#if (0)//ccr2017-09-30!defined(DEBUGGPRS)

    short l;
	  //向手机发送一条短信息,用于检测GPRS模块的通信是否正常
    EmptyComm(GPRSPORT);
    mClearScreen();
    if (TESTBIT(GPRS_Ready,GPRS_OK_BIT))
    {
        DispStrXY("Mobile #:",0,0);
        l = GetStrFrKBD('9',0,1,20,0);

        if (l>0 && GPRSSend_ATE0())
        {
            SendATCommand("AT+CMGF=1\r");
            GPRSSendAString("AT+CMGS=",-1);
            EntryBuffer[ENTRYSIZE - 1]=0;
            GPRSSendAString(&EntryBuffer[ENTRYSIZE - l - 1],-1);
            GPRSSendByte('\r');
            GPRSSkipReceived(SECONDS(5));//skip '>"

            ScrollScreen(1);
            DispStrXY("Message:",0,1);
            l = GetStrFrKBD('A',0,2,70,0);
            if (l>0)
                memcpy(PCBuffer,&EntryBuffer[ENTRYSIZE - l - 1],l);
            else
                l=0;
            PCBuffer[l]=0x1a;
            PCBuffer[l+1]=0;
            if (!SendATCommand(PCBuffer))
            {
//ccr2016-06-27                DispStrXY("Error send......",0,0);
            }
        }
    }
    else
    {
        DispStrXY(GPRSMess[G_WAITFOR].str,0,0);
    }
    PRESS_ANY_KEY(SCREENLN-2);

#else//ccr2015-08-24>>>>测试GPRS批量数据发送功能,检测是否有数据丢失
    short i,size;
    ULONG sendCount=0;
    BYTE OK_Send;
    ULONG tstID=1;

    mClearScreen();

    if (TESTBIT(GPRS_Ready,GPRS_OK_BIT))//ccr2015-03-11
    {
        EmptyComm(GPRSPORT);

        if (GPRSStartGprs())
        {
            DispStrXY(GPRSMess[G_SHUJUFASONGZHONG].str,0,0);
            //循环发送"H?<<<<<    1"编号数据
            do
            {
                memset(PCBuffer,0,40);
                size = 30;
                PCBuffer[0]='H';
                PCBuffer[1]=size & 0xff;
                PCBuffer[2]=NETIDLOG;
                sprintf(&PCBuffer[3],"<<<<<%15u",tstID++);

                if (!GPRSSendECRData(PCBuffer,size+2))//发送流水数据
                {
                    OK_Send = false;
                    break;
                }

                //size = GPRSRead_IPDATA(PCBuffer,10,SECONDS(5));//ccr2015-08-25>>>>>>等待应答

                sendCount++;

                memset(SysBuf,' ',SCREENWD);
                ULongtoASC(SysBuf+SCREENWD-1,sendCount);
                SysBuf[SCREENWD]=0;
                DispStrXY(SysBuf,0,1);//显示已发送数据数目

                OK_Send = true;
            } while (sendCount<20);//测试发送20条

            GPRSCheckACK(0);//等待数据发送完毕

            if (OK_Send)//发送失败
                DispStrXY(GPRSMess[G_FASONGCHENGGONG].str,0,2);
            else
                DispStrXY(GPRSMess[G_FASONGSHIBAI].str,0,2);

            CloseGPRS();
        }
    }
    else
    {
        DispStrXY(GPRSMess[G_WAITFOR].str,0,0);
    }
    PRESS_ANY_KEY(SCREENLN-2);
#endif
}

///////////////////////////////////////////////////////////////
CONST char ATTXSTATE[]="AT%TXSTATE?\r\n";
/** ccr2015-08-25
 * 查询数据发送状态,等待服务器收到数据的回复ACK
 *
 * @author EutronSoftware (2015-08-25)
 * @param ACKmust:等待服务器返回ACK
*  @return BYTE
*  此命令将返回以下数据
*[0D][0A]
*[25][54][58][53][54][41][54][45][3A][20][0D][0A]
*[31][2C][35][2C][35][0D][0A]
*[32][2C][30][2C][30][0D][0A]
*[33][2C][30][2C][30][0D][0A]
*[0D][0A]
*[4F][4B][0D][0A]
 */

BYTE GPRSCheckACK(BYTE ACKmust)
{
    char readGPRS[32];
    int readInt[3];
    int i,l,reTry,mul;

#if defined(ACK_MUST)
    if (GPRS_ACK_MUST && ACKmust)
    {
        SetTimerDelay2(SECONDS(60));
        do {
            if ((GPRSRead_IPDATA(readGPRS,sizeof(readGPRS)-5,SECONDS(10))>0 && readGPRS[0]==ACKmust))
                return true;
        } while (GetTimerDelay2());
        //无ACK应答,结束发送
        DisplayPrompt(GPRSxACK);//ccr2016-08-26
        return false;
    }
    else
#endif
    {
        SetTimerDelay2(SECONDS(60));
        do {
            GPRSSendAString(ATTXSTATE,strlen(ATTXSTATE));
            while (!GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(30)))//等待%TXSTATE:
            {
            }
    //        if (GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(5))>0)
            {
                if (GPRSHand(readGPRS,"%TXSTATE:"))
                {
                    if ((i=GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(30)))>0)//1,1,0
                    {
                        memset(readInt,0,sizeof(readInt));

                        for (l=mul=0;l<i && mul<3;l++)
                        {//获取GPRS模块已发送并得到服务器确认的数据数目
                            if (readGPRS[l]>='0' && readGPRS[l]<='9')
                            {
                                readInt[mul] = readInt[mul]*10+ (readGPRS[l] & 0x0f);
                            }
                            else
                                if (readGPRS[l]==',')
                                    mul++;
                        }
                    }
                    //skip:'2,0,0','3,0,0','NULL'
                    for (i=0;i<4;i++)
                    {
                        if (GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(30))>0)
                        {
                            if (readGPRS[0]=='O' && readGPRS[1]=='K')
                            {
                                if (readInt[1]==readInt[2])
                                    return true;//服务器收到GPRS的全部数据
                            }
                        }
                    }
                }
            }
        } while (GetTimerDelay2());
        return false;
    }
}

//////////////////////////////////////////////////////////////////
CONST char ATIPSEND[]="AT%IPSEND=";

/**
 * 通过GPRS的AT%IPSEND命令以TCP/IP方式将buffer中的数据发送出去
 * 在调用GPRSSendECRData之前,要用StartGprs建立TCP/IP
 * 调用GPRSSendECRData之后,用EndGprs关闭TCP/IP
 *
 * @author EutronSoftware (2014-11-28)
 *
 * @param buffer
 * @param len :Bytes in buffer must be send
 *
 * @return int :0=false,else return length
 */
int GPRSSendECRData(BYTE *buffer,int len)
{
#define WAITSEND    RETRYTIMES*20

    char readGPRS[32];
    int i,l,reTry,mul;

    if (len)
    {//有数据时才发送
        EmptyComm(GPRSPORT);
        GPRSSendAString(ATIPSEND,strlen(ATIPSEND));//"AT%IPSEND=",数据发送命令
        GPRSSendByte('"');//数据块开始

        for (i=0;i<len;i++)
            GPRSSendHEX(buffer[i]);//发送数据

        GPRSSendByte('"');//数据块结束
        GPRSSendByte('\r');//命令结束

#if !defined(GPRSWAITACK)
        reTry=0;
        do {
#endif
            if (!GPRSSkipReceived(SECONDS(120))) //skip '\r\n'
                return false;

            if ((i=GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(180)))<=0) // read "%IPSEND 1,13"
                return false;
            readGPRS[7]=0;

            if (!GPRSHand(readGPRS,"%IPSEND"))
                return false;// 没有收到 "%IPSEND"

            l=0;mul=1;
            i--;
            for (;i>0 && readGPRS[i]!=',';i--)
            {//获取GPRS模块发送区的剩余空间数据
                if (readGPRS[i]>='0' && readGPRS[i]<='9')
                {
                    l += ((readGPRS[i] & 0x0f)*mul);
                    mul *= 10;
                }
            }
#if !defined(GPRSWAITACK)
            if (reTry>0)//表示执行的是"AT%IPSEND?"查询
            {
                if(!GPRSSkipReceived(SECONDS(120)))	// SKIP one blank line.. add by lqw..090825
                    return FALSE;	// FOR GTM900B ,it will be opened..
            }
#endif
#if (WAITFOROK)
            if (!GPRSWaitForOK(SECONDS(10)))
                return false;//没有收到 "OK"
#else

            if (GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(180))<=0// read "OK"
                 || (CWORD(readGPRS[0])!=RET_OK))
                    return false;//没有收到 "OK"
#endif
#if defined(GPRSWAITACK)
            if (l<=1 && !GPRSCheckACK(0))//ccr2015-08-25
                return false;
#else
            if (l<=1)//缓冲区满
            {//查询缓冲区
                Delay(300);
                strcpy(readGPRS,ATIPSEND);
                readGPRS[9]='?';
                readGPRS[10]='\r';
                GPRSSendAString(readGPRS,11);//"AT%IPSEND?"查询
                mDrawGPRS(SCREENWD-1,0,0);//ccr2015-08-24在右上角闪烁显示标示
            }
            reTry++;
        } while (l<=1 && reTry<WAITSEND);
        if (reTry>=WAITSEND)
        {
            return false;
        }
        else
#endif
            return len;
    }
    else
    {
        return true;
    }
}

//ccr2014-11-26>>>>>>>>>>>>>>>>>>>>>>>>
/*TCP/IP>>>>>>>>>>
ATE0[0d]
AT+CGDCONT=1,"ip","cmnet"[0d];GPRSConnectAPN()
AT%ETCPIP="",""[0d][0a];GPRSRegisterTCPIP()
AT%IPOPEN="TCP","211.147.90.197",55391[0d];GPRSConnect_TCPIP()
AT%IPSEND="12121212121212121212121212"[0d][0a]
AT%IPSEND?[0d][0a]
AT%IPCLOSE[0d] */
/**
 * 通过AT%IPSEND 发送收款机最新销售流水数据
 * 当在SET下时，将发送信息显示在屏幕上
 * 当在销售下时，将发送信息显示在状态行
 * @author EutronSoftware (2014-12-01)
 */

void GPRSSendECR_LOG()
{
    short i,size;
    ULONG sendCount=0;//发送计数器
    ULONG logReadCopyNext=0;//下一条即将要读取的流水数据
    ULONG logReadCopyLast=0;//当前读取的流水数据
    BYTE OK_Send;
#if defined(MAXLOGPACKAGE)
    ULONG logSize;//用来指定打包发送的日志数据包大小
    int  sendFlag;//发送计数器
#endif

    if (ApplVar.FlowHeader.ReadP==ApplVar.FlowHeader.NextNP)
        return;
    if (TESTBIT(GPRS_Ready,GPRS_OK_BIT) && GetSystemTimer()>GPRS_Wait)//ccr2015-03-11
    {
        //mClearScreen();
        //DispStrXY(GPRSMess[G_CONNECTING].str,0,0);
        if (GPRSStartGprs())
        {
            if (ApplVar.CentralLock==SET)
                DispStrXY(GPRSMess[G_SHUJUFASONGZHONG].str,0,1);
            else
#if (SCRSTATDBL)
                DispOnStatDBL(GPRSMess[G_SHUJUFASONGZHONG].str,0);
#else
                DispSt1(GPRSMess[G_SHUJUFASONGZHONG].str);//RG-DISPLAY
#endif
            //首先发送收款机编号数据
#if defined(CASE_INDONESIA)
            GPRSSendECR_HeartBeat(0xff);//ccr2015-08-03 ECR空闲时发送heartbeat数据
            size = sizeof(ApplVar.IP.ECR_ID)+1;
            PCBuffer[0]='F';
            PCBuffer[1]=size & 0xff;
            PCBuffer[2]=NETIDLOG;
            memcpy(&PCBuffer[3],ApplVar.IP.ECR_ID,sizeof(ApplVar.IP.ECR_ID));
#if defined(MAXLOGPACKAGE)
            logSize = sizeof(ApplVar.IP.ECR_ID)+3;//ccr2015-08-26
#endif
#else
            size = 3;
            PCBuffer[0]='F';
            PCBuffer[1]=size & 0xff;
            PCBuffer[2]=ECRNOLOG;
            PCBuffer[3]=REGISTER;
            PCBuffer[4]=LOCATION;
#if defined(MAXLOGPACKAGE)
            logSize = 5;//ccr2015-08-26
#endif
#endif
#if defined(MAXLOGPACKAGE)
#if (PCBUFMAX<1024)
#error "PCBUFMAX<1024"
#endif
            //利用PCBuffer,一次发送不超过1024字节的流水数据
            do  {//在此循环里,完成全部流水数据的发送
                sendFlag = 0;//用来标示每次组包中的流水条数
                if (logReadCopyNext!=0)
                    ApplVar.FlowHeader.ReadP = logReadCopyNext;//ccr2015-08-24指向下一条要读取和发送的流水
                logReadCopyLast = ApplVar.FlowHeader.ReadP;//ccr2015-08-24保存当前读取的流水数据地址
                logReadCopyNext = logReadCopyLast;//ccr2016-04-18
                do {//在此循环里,完成一次组包和发送
                    size = Read_Flow((BYTE *)&ApplVar.FlowBuff);//读取一条流水
                    if (size>0 && (size+logSize)<=1020)
                    {
                        //将数据存入PCBuffer,组成大包
                        PCBuffer[logSize++]='F';
                        PCBuffer[logSize++]=size;
                        memcpy(PCBuffer+logSize,(BYTE *)&ApplVar.FlowBuff,size);
                        logSize+=size;
                        sendFlag++;
                    }
                    else if (size>=0)// || (size+logSize)>1020)
                    {
                        if (size)
                            SUB_SCANFP(size);
                        if (sendFlag)
                        {//有要发送的数据,将数据发送出去
                            memset(SysBuf,' ',SCREENWD);
                            OK_Send = ULongtoASC(SysBuf+SCREENWD-1,sendFlag);
                            SysBuf[SCREENWD]=0;
                            if (ApplVar.CentralLock==SET)
                                DispStrXY(SysBuf,0,2);//显示已发送数据数目
                            else
                            {
                                SysBuf[0] = OK_Send;
                                DispStrXY(SysBuf+SCREENWD-SysBuf[0],SCREENWD-SysBuf[0],STATELINE);
                            }

                            logReadCopyLast = logReadCopyNext;
                            logReadCopyNext = ApplVar.FlowHeader.ReadP;
                            ApplVar.FlowHeader.ReadP = logReadCopyLast;//发送出错时,下次从此重新发送
                            if (
                                 !GPRSSendECRData(PCBuffer,logSize)//发送流水数据
#if defined(GPRSWAITACK)
                                 || !GPRSCheckACK(ACK)//等待服务器应答
#endif
                                )
                            {//发送失败时
                                OK_Send = false;
                                size = 0;//结束发送.
                            }
                            else
                            {//发送成功
                                ApplVar.FlowHeader.ReadP = logReadCopyNext;//发送成功,下次从此发送
                                //ccr2016-04-18 logReadCopyNext = 0;
                                if (sendCount==0) sendCount=1;//加收款机NETID的流水
                                sendCount+=sendFlag;
                                OK_Send = true;
                            }
                            logSize = 0;
                        }
                        break;
                    }
                } while (size>0);
            } while (size>0);

            memset(SysBuf,' ',SCREENWD);
            SysBuf[0] = ULongtoASC(SysBuf+SCREENWD-1,sendCount);
            SysBuf[SCREENWD]=0;
            if (OK_Send)
                i = G_FASONGCHENGGONG;
            else
                i = G_FASONGSHIBAI;
            if (ApplVar.CentralLock==SET)
            {
                SysBuf[0]=' ';
                DispStrXY(SysBuf,0,2);//显示已发送数据数目
                DispStrXY(GPRSMess[i].str,0,SCREENLN-2);
            }
            else
            {
#if (SCRSTATDBL)
                DispOnStatDBL(GPRSMess[i].str,0);
#else
                DispSt1(GPRSMess[i].str);//RG-DISPLAY
                DispStrXY(SysBuf+SCREENWD-SysBuf[0],SCREENWD-SysBuf[0],STATELINE);
#endif
            }
#else//逐条发送流水数据方式>>>>>>>>>>>>
            do
            {
                //逐条发送流水数据
                if (!GPRSSendECRData(PCBuffer,size+2))//发送流水数据
                {
                    OK_Send = false;
                    break;
                }
                sendCount++;

                if (logReadCopyNext!=0)
                    ApplVar.FlowHeader.ReadP = logReadCopyNext;//ccr2015-08-24指向下一条要读取和发送的流水

                logReadCopyLast = ApplVar.FlowHeader.ReadP;//ccr2015-08-24保存当前读取的流水数据地址
                size=Read_Flow(PCBuffer+2);//读取流水数据
                logReadCopyNext = ApplVar.FlowHeader.ReadP;//ccr2015-08-24保存下一条将要读取的流水数据
                if (size>0)
                {
                    memset(SysBuf,' ',SCREENWD);
                    OK_Send = ULongtoASC(SysBuf+SCREENWD-1,sendCount);
                    SysBuf[SCREENWD]=0;
                    if (ApplVar.CentralLock==SET)
                        DispStrXY(SysBuf,0,2);//显示已发送数据数目
                    else
                    {
                        SysBuf[0] = OK_Send;
                        DispStrXY(SysBuf+SCREENWD-SysBuf[0],SCREENWD-SysBuf[0],STATELINE);
                    }

                    ApplVar.FlowHeader.ReadP = logReadCopyLast;//ccr2015-08-24恢复最后读取的流水数据地址,以防发送死机等出错
                    PCBuffer[0]='F';
                    PCBuffer[1]=size;
                }
                OK_Send = true;

            } while (size>0);
#if defined(GPRSWAITACK)
            if (sendCount>0)//表示在发送了流水
                 GPRSCheckACK(ACK);//等待服务器应答
#endif
#endif//defined(MAXLOGPACKAGE)

            if (!OK_Send)//发送失败
            {
                if (ApplVar.CentralLock==SET || ApplVar.CentralLock==X)
                    DispStrXY(GPRSMess[G_FASONGSHIBAI].str,0,SCREENLN-2);
                else
                {
                    CheckError(ERROR_ID(CWXXI84));
                    //DispSt1(GPRSMess[G_FASONGSHIBAI].str);
                }
            }
            else if (size==0)//流水数据发送完毕
            {
#if defined(CASE_INDONESIA)
                ApplVar.LastDaysLog = GetDaysFrom1900(Now.year,Now.month,Now.day);
#endif
#if !defined(MAXLOGPACKAGE)
                if (ApplVar.CentralLock==SET)
                    DispStrXY(GPRSMess[G_FASONGCHENGGONG].str,0,SCREENLN-2);
                else
                {
#if (SCRSTATDBL)
                    DispOnStatDBL(GPRSMess[G_FASONGCHENGGONG].str,0);
#else
                    DispSt1(GPRSMess[G_FASONGCHENGGONG].str);//RG-DISPLAY
                    DispStrXY(SysBuf+SCREENWD-SysBuf[0],SCREENWD-SysBuf[0],STATELINE);
#endif
                }
#endif
            }
            else if (size==-1)//流水数据功能号出错
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI48);
            }
            CloseGPRS();
        }
        if (ApplVar.CentralLock==SET)
            ScrollScreen(1);
    }
    else
    {
        if (ApplVar.CentralLock==SET)
        {
            mClearScreen();
            DispStrXY(GPRSMess[G_WAITFOR].str,0,0);
        }
        else
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI89);
#if (SCRSTATDBL)
            DispOnStatDBL(GPRSMess[G_WAITFOR].str,0);
#else
            DispSt1(GPRSMess[G_WAITFOR].str);//RG-DISPLAY
#endif
        }
    }
    if (ApplVar.CentralLock==SET || ApplVar.CentralLock==X)
        PRESS_ANY_KEY(SCREENLN-2);
}

/**
 * 发送全部销售流水数据
 *
 * @author EutronSoftware (2014-12-01)
 */
void GPRSSendECR_LOG_All()
{
    mClearScreen();
    DispStrXY(MESS_LOGREPET,0,0);
    if (InputYesNo(Msg[QUERENF].str,0,1,0)=='Y')
    {
        if (TESTBIT(GPRS_Ready,GPRS_OK_BIT))
            ResetECRFlow();
        GPRSSendECR_LOG();
    }
}

/**
 * 设置流水发送方式
 *
 * @author EutronSoftware (2014-12-01)
 */
void GPRS_SetSendMode()
{
    BYTE y;
    short si;
    WORD saveNumber;

    // 设置发送销售收据的发送参数
    mClearScreen();
    y = InputYesNo(SENDLOGAUTO,0,0,TESTBIT(ApplVar.DaysMust,BIT7));
    if (y!=0)
    {
        if (y=='Y')
            SETBIT(ApplVar.DaysMust,BIT7);//为自动发送流水，即结帐后立即发送流水
        else
            RESETBIT(ApplVar.DaysMust,BIT7);//非自动发送流水，需要手工操作发送流水数据
        y = 1;
        if (!TESTBIT(ApplVar.DaysMust,BIT7))
        {//非自动发送流水状态下，在指定的间隔天数内，必须发送流水数据
            DispStrXY(DAYSFORLOG,0,1);
    #if (SCREENWD>16)
            y=1;
    #else
            y=2;
    #endif
            DisplayDecXY(ApplVar.DaysMust,SCREENWD-2,y,2);
            si = GetStrFrKBD('9',0,y,2,0);
            if (si<0)
            {
                ClearEntry(false);
                return;
            }
            else if (si>0)
            {
                ApplVar.DaysMust = ((EntryBuffer[ENTRYSIZE-3] & 0x0f)*10 + (EntryBuffer[ENTRYSIZE-2] & 0x0f)) & 0x7f;
            }
            y++;
        }
#if defined(ACK_MUST)
        if (y>=SCREENLN-1)
        {
            ScrollScreen(1);
            y=SCREENLN-2;
        }
        y = InputYesNo(ACKMUSTNEED,0,y,GPRS_ACK_MUST);
        if (y=='Y')
            SETBIT(APPLCTRL,BIT2);      //GPRS与服务器通信时,需要进行ACK应答
        else if (y=='N')
            RESETBIT(APPLCTRL,BIT2);    //GPRS与服务器通信时,需要进行ACK应答
        SETBIT(ApplVar.MyFlags,CONFIGECR);
#endif
    }
    ClearEntry(false);
}

/**
 * 从服务器下载由codePLU1指定的收款机文件数据
 * countPLU为需要下载的记录数
 * @author EutronSoftware (2014-12-06)
 *
 * @param typeName :数据类型名称
 * @param codePLU1 :数据数据类型代码(PLU1,DEPT,CLERK...),
 * @param countPLU :记录数,实际可下载的数目可能<countPLU
 *
 * 参数codePLU1=0xfff,countPLU=0:下载全部数据
 *
 */
//#define ONLY1   1
#if (0)//defined(ONLY1)
//采用一次只下载一条数据的方式从服务器下载数据,速度慢,可靠
void GPRSDownloadEcrPLUData(const char *typeName,WORD codePLU1,WORD countPLU,ULONG sWaitFor)
{
    short sLen;
    WORD received=0;//统计收到记录数目
    WORD ECRFunc;
    BYTE decodeLen;

    short chReceived;//统计已经收到的字节数据
    short ch;
    char getIPDATA[16];

    WORD Read_MAX;//GPRS模块收到的数据数目

    if (TESTBIT(GPRS_Ready,GPRS_OK_BIT) && GetSystemTimer()>GPRS_Wait)
    {
        if (countPLU>0)
        {
            if (GPRSStartGprs())
            {
                DispStrXY(typeName,0,1);
                //首先发送收款机编号数据
                for (Read_MAX=codePLU1+1;Read_MAX<=codePLU1+countPLU;Read_MAX++)
                {
                    getIPDATA[0]='D';
                    getIPDATA[1]=6;//size
                    CWORD(getIPDATA[2])=Read_MAX;
                    CWORD(getIPDATA[4])=Read_MAX;//默认下载收款机可容纳的单品数量
                    getIPDATA[6]=REGISTER;//收款机编号
                    getIPDATA[7]=LOCATION;//收款机商店编号
                    if (GPRSSendECRData(getIPDATA,6+2))//发送下载请求.
                    {

                        GPRSDecodeECRRecord(0,0,0);//initial GPRSGetECRARecord

                        if (Read_MAX==PLU1+1)
                            ApplVar.AP.Plu.RNumber = 0;

                        chReceived = GPRSRead_IPDATA(PCBuffer,sizeof(PCBuffer),sWaitFor);
                        if (chReceived)
                        {
                            for (sLen=0;sLen<chReceived;sLen++)
                            {
                                ECRFunc = GPRSDecodeECRRecord('U',PCBuffer[sLen],&decodeLen);
                                //存储下载的数据>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                                if (ECRFunc!=0xffff && ECRFunc!=0)//if (ECRFunc==0) //收据收取完毕
                                {
                                    if (ECRFunc>codePLU1 && ECRFunc<=codePLU1+countPLU)
                                    {
                                        ECRFunc -= codePLU1;
                                        switch (codePLU1)
                                        {
                                        case PLU1://收到的为单品数据,数据中包含单品功能码和单品数据
                                            if (ECRFunc<=ApplVar.AP.Plu.Number && decodeLen==sizeof(ApplVar.Plu)+2)
                                            {
                                                if (ApplVar.AP.Plu.RNumber<ECRFunc)
                                                    ApplVar.AP.Plu.RNumber = ECRFunc;

                                                ApplVar.PluNumber = ECRFunc-1;
                                                memcpy(&ApplVar.Plu,EcrRecord+2,sizeof(ApplVar.Plu));
                                                WritePlu();
                                                received++;
                                            }
                                            break;
                                        case DEPT:
                                            if (ECRFunc<=ApplVar.AP.Dept.Number && decodeLen==sizeof(ApplVar.Dept)+2)
                                            {
                                                ApplVar.DeptNumber = ECRFunc-1;
                                                memcpy(&ApplVar.Dept,EcrRecord+2,sizeof(ApplVar.Dept));
                                                WriteDept();
                                                received++;
                                            }
                                            break;
                                        }
                                        //received++;
                                        memset(SysBuf,' ',SCREENWD);
                                        WORDtoASC(SysBuf+SCREENWD-1,received);
                                        SysBuf[SCREENWD]=0;
                                        DispStrXY(SysBuf,0,2);//显示已发送数据数目
                                    }//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                    else
                                    {
                                        DispStrXY(GPRSMess[G_xDATATYPE].str,0,0);//数据错
                                    }
                                }

                            }
                        }
                        else
                            DispStrXY(GPRSMess[G_JIESHOUSHIBAI].str,0,2);//超时错
                    }
                    else
                    {
                        DispStrXY(GPRSMess[G_FASONGSHIBAI].str,0,2);
                    }
                }
                CloseGPRS();
            }
        }
        ScrollScreen(1);
        PRESS_ANY_KEY(SCREENLN-2);//显示按任意键退出界面
    }
}
#else

/**
 * 采用一次下载多条数据的方式从服务器下载数据,速度快,可能数据
 * 从服务器下载由codePLU1指定的收款机文件数据
 * countPLU为需要下载的记录数
 * @author EutronSoftware (2014-12-06)
 *
 * @param typeName :数据类型名称
 * @param codePLU1 :数据数据类型代码(PLU1,DEPT,CLERK...),
 * @param countPLU :记录数,实际可下载的数目可能<countPLU
 *
 * 参数codePLU1=0xffff,countPLU=0:下载全部数据
 *
 */

void GPRSDownloadEcrPLUData(const char *typeName,WORD codePLU1,WORD countPLU,ULONG sWaitFor)
{

    WORD received=0;//统计收到记录数目
    WORD ECRFunc,FuncNum;
    WORD cECRFunc0=0,cECRFunc1=0xffff;//用来控制显示
    BYTE decodeLen;

    short chReceived;//统计已经收到的字节数据
    short ch;
    char getIPDATA[18];

    WORD Read_MAX;//GPRS模块收到的数据数目
    BYTE sByte;
    BYTE sBit=0;
    BYTE Read_WaitFor=0xff;// =0 分析数据头 "%IPDATA", =1 分析数据长度; =2,3 分析数据 //


    if (TESTBIT(GPRS_Ready,GPRS_OK_BIT) && GetSystemTimer()>GPRS_Wait)
    {
        if (countPLU>0 || codePLU1==0xffff)//ccr2016-08-23
        {
            if (GPRSStartGprs())
            {
                memset(SysBuf,' ',SCREENWD);
                CopyFrStr(SysBuf,typeName);
                if (codePLU1!=0xffff)
                    WORDtoASC(SysBuf+SCREENWD-1,countPLU);
                SysBuf[SCREENWD]=0;
                DispStrXY(SysBuf,0,1);
                GotoXY(0,2);
                {
                    getIPDATA[0]='D';//发送下载请求
                    getIPDATA[1]=6;//size
                    CWORD(getIPDATA[2])=codePLU1+1;//ccr2016-08-18codePLU1=0xffff时,相加结果=0,下载全部数据
                    CWORD(getIPDATA[4])=codePLU1+countPLU;//默认下载收款机可容纳的单品数量
                    getIPDATA[6]=REGISTER;//收款机编号
                    getIPDATA[7]=LOCATION;//收款机商店编号
                    memcpy(&getIPDATA[8],ApplVar.IP.ECR_ID,sizeof(ApplVar.IP.ECR_ID));
                    if (GPRSSendECRData(getIPDATA,6+2+sizeof(ApplVar.IP.ECR_ID)))//发送下载请求.
                    {

                        GPRSDecodeECRRecord(0,0,0);//initial GPRSGetECRARecord

                        ECRFunc= 0xffff;
                        chReceived=0;
                        SetTimerDelay0(sWaitFor);
                        while (GetTimerDelay0() && ECRFunc)
                        {
                            if(KbHit() && (Getch() == ApplVar.AP.FirmKeys[ID_CANCEL]))//ccr2016-08-26
                                    break;
                            ch = ReadComm(GPRSPORT);//读取从服务器返回的数据
                            if (ch!=-1)//有数据
                            {
                                GPRSECH_READ(ch);//testonly
                                switch (Read_WaitFor)
                                {
                                case WAITFOR_IPDATA:
                                    getIPDATA[chReceived++]=ch;
                                    if (ch==':')
                                    {
                                        getIPDATA[chReceived]=0;
                                        if (strcmp(getIPDATA,"%IPDATA:")==0)//	change by lqw %IPDATA->%IPDATA:
                                        {
                                            sBit = 0;
                                            Read_WaitFor = WAITFOR_LENGTH;
                                            chReceived= Read_MAX = 0;
                                        }
                                        else
                                        {
                                            DispStrXY(GPRSMess[G_xDATATYPE].str,0,0xff);//数据错
                                            ECRFunc = 0;// 返回的数据出错 //
                                        }
                                    }
                                    else if (chReceived>8)
                                    {
                                        DispStrXY(GPRSMess[G_xDATATYPE].str,0,0xff);//数据错
                                        ECRFunc = 0;// 返回的数据出错 //
                                    }
                                    break;
                                case WAITFOR_LENGTH:
                                    if (ch==',')
                                        Read_WaitFor = WAITFOR_QUOTES;
                                    else if (ch>='0' && ch<='9')
                                        Read_MAX = Read_MAX*10 + (ch & 0x0f);// 获取数据长度 //
                                    break;
                                case WAITFOR_QUOTES:
                                    if (ch=='"')
                                    {//数据开始
                                        Read_WaitFor = WAITFOR_DATA;
                                        sBit = 0;
#if defined(ACK_MUST)
                                        GPRS_WaitACK=0;
#endif
                                    }
                                    break;
                                case WAITFOR_DATA:
                                    if (ch=='"')//数据结束
                                        Read_WaitFor = ENDOF_READ;
                                    else
                                    {
                                        if (sBit)
                                        {//收到一个完整的字节
                                            chReceived++;
                                            //if (chReceived>=Read_MAX)  return ;// 数据有错 //

                                            ECRFunc = GPRSDecodeECRRecord('U',(sByte<<4)+ASC_2_HEX(ch),&decodeLen);
                                            //存储下载的数据>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                                            if (ECRFunc!=0xffff && ECRFunc!=0)//if (ECRFunc==0) //收据收取完毕
                                            {//将收到的数据写入收款机文件中

                                                if (GPRS_ProcessRecord('U',EcrRecord,decodeLen))
                                                {
                                                    received++;
                                                    memset(SysBuf,' ',SCREENWD);
                                                    if (ECRFunc==ADDPLU)
                                                        SysBuf[0]='+';
                                                    else if(ECRFunc==DELPLU)
                                                        SysBuf[0]='-';
                                                    if (SysBuf[0]!=' ')
                                                    {
                                                        SysBuf[1]='P';SysBuf[2]='L';SysBuf[3]='U';
                                                        if (cECRFunc0 && cECRFunc0!=ECRFunc)
                                                        {
                                                            DispCharXY('\n',0xff,0xff);
                                                            cECRFunc0=ECRFunc;
                                                        }
                                                    }
                                                    else if (codePLU1==0xffff)
                                                    {
                                                        cECRFunc0=ECRFuncCodeToTypeName(ECRFunc,SysBuf);
                                                        if (cECRFunc0!=cECRFunc1)
                                                        {//为新的功能名称
                                                            if (cECRFunc1!=0xffff)
                                                                DispCharXY('\n',0xff,0xff);
                                                            received=1;
                                                            cECRFunc1=cECRFunc0;
                                                        }
                                                        if (cECRFunc0)//有功能名称
                                                            SysBuf[strlen(SysBuf)]=' ';
                                                        else
                                                            WORDtoASCL(SysBuf,ECRFunc);
                                                    }
                                                    WORDtoASC(SysBuf+SCREENWD-1,received);
                                                    SysBuf[SCREENWD]=0;
                                                    DispStrXY(SysBuf,0,0xff);//显示已收到的数据数目
                                                }
                                                else
                                                {
                                                    ScreenFeed(1);
                                                    DispStrXY(GPRSMess[G_xDATATYPE].str,0,0xff);//数据错
                                                }
                                            }//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                                        }
                                        else
                                            sByte = ASC_2_HEX(ch);

                                        sBit ^= 1;
                                    }
                                    break;
                                case ENDOF_READ://ccr2016-10-25>>>>>>>>>>
                                    //等待回车+换行
                                    //DispStrXY("END.",0,5);//testonly
#if defined(ACK_MUST)
                                    if (GPRS_ACK_MUST && GPRS_WaitACK)
                                    {
                                        //发送ACK应答
                                        sByte=ACK;
                                        if ( (!GPRSSendECRData(&sByte,1)//需要对下载进行应答
#if defined(GPRSWAITACK)
                            //ccr2016-08-11    || !GPRSCheckACK(0)//等待服务器应答
#endif
                                            ))
                                        {//发送失败时
                                            //DispStrXY("Send ACK error",0,0xff);//testonly
                                        }
                                        GPRS_WaitACK=false;
                                    }
#endif
                                    if (ch=='\r')
                                        break;
                                    else// if (ch=='\n')
                                    {//已经读取%IPDATA后的全部数据
                                        Read_WaitFor=0xff;
                                    }
                                    //ccr2016-10-25<<<<<<<<
                                default:
                                    if (ch=='%')//ccr2016-10-25 || ch=='\r' || ch=='\n')
                                    {//已经读取%IPDATA后的全部数据
                                        Read_WaitFor=WAITFOR_IPDATA;// =0 分析数据头 "%IPDATA", =1 分析数据长度; =2,3 分析数据 //
                                        chReceived= Read_MAX = 0;
                                        //ccr2016-10-25if (ch=='%')
                                            getIPDATA[chReceived++]=ch;
                                    }
                                    break;
                                }
                                SetTimerDelay0(sWaitFor);

                            }
                        }//While
                        if (!GetTimerDelay0())
                        {
                            ScreenFeed(1);
                            DispStrXY(GPRSMess[G_JIESHOUSHIBAI].str,0,0xff);//超时错
                        }
                    }
                    else
                    {
                        ScreenFeed(1);
                        DispStrXY(GPRSMess[G_FASONGSHIBAI].str,0,0xff);
                    }
                }
                CloseGPRS();
            }
            ScrollScreen(1);
        }
	}
	else
	{
		mClearScreen();
#if (SCRSTATDBL)
        DispOnStatDBL(GPRSMess[G_WAITFOR].str,0);
#else
        DispSt1(GPRSMess[G_WAITFOR].str);//RG-DISPLAY
#endif
	}
	PRESS_ANY_KEY(SCREENLN-2);

}
#endif
/**
 * //"下载单品文件"
 *
 * @author EutronSoftware (2014-12-06)
 */
void GPRS_DownloadPLU()
{
    GPRSDownloadEcrPLUData(Msg[gprsDownloadPLU].str,PLU1,ApplVar.AP.Plu.Number,SECONDS(30));
}
/**
 * //"下载部类文件"
 *
 * @author EutronSoftware (2014-12-06)
 */
void GPRS_DownloadDEPT()
{
    GPRSDownloadEcrPLUData(Msg[gprsDownloadDEPT].str,DEPT,ApplVar.AP.Dept.Number,SECONDS(30));
}
/**
 * //"下载收款员"
 *
 * @author EutronSoftware (2014-12-06)
 */
void GPRS_DownloadCLERK()
{
    GPRSDownloadEcrPLUData(Msg[gprsDownloadCLERK].str,CLERK,ApplVar.AP.Clerk.Number,SECONDS(30));
}
/**
 * //"下载票头"
 *
 * @author EutronSoftware (2014-12-06)
 */
void GPRS_DownloadHEAD()
{
    //为迎合GPRSDownloadEcrPLUData中的CWORD(getIPDATA[2])=codePLU1+1
    //必须使用RHEAD1-1作为下载票头参数
    GPRSDownloadEcrPLUData(Msg[gprsDownloadHEAD].str,RHEAD1-1,2,SECONDS(30));
}
/**
 * //"下载票尾"
 *
 * @author EutronSoftware (2014-12-06)
 */
void GPRS_DownloadPTRAIL()
{
    //为迎合GPRSDownloadEcrPLUData中的CWORD(getIPDATA[2])=codePLU1+1
    //必须使用RTRAIL1-1作为下载票尾参数
    GPRSDownloadEcrPLUData(Msg[gprsDownloadTRAIL].str,RTRAIL1-1,2,SECONDS(30));
}

/**
 * //"下载部数据"
 *
 * @author EutronSoftware (2014-12-06)
 */
void GPRS_DownloadALL()
{
    //为迎合GPRSDownloadEcrPLUData中的CWORD(getIPDATA[2])=codePLU1+1
    //必须使用RTRAIL1-1作为下载票尾参数
    //参数0xfff,0:下载全部数据
    GPRSDownloadEcrPLUData(Msg[gprsDownloadALL].str,0xffff,0,SECONDS(30));
}

/** ccr2015-08-03>>>>>>>>>>>>>>>>>>>>>
 * 通过AT%IPSEND 发送收款机heartbeat消息
 * @author EutronSoftware (2015-08-02)
 *  @param force: =0x00在收款机空闲时发送Heart-Beat;
 *                =0x01立即发送Heart-Beat;
 *                =0xff设置发送Heart-Beat的延迟时间
 */
void GPRSSendECR_HeartBeat(BYTE force)
{
    BYTE HeartBuffer[15];
    static ULONG IDLE_HB;//=GetSystemTimer()+SECONDS(ACTIVE*60);//等待5分钟;

    if (force==0xff)
    {//=0xff设置发送Heart-Beat的延迟时间
        IDLE_HB=GetSystemTimer()+SECONDS(ACTIVE*60);//等待5分钟;
    }
    else if (TESTBIT(GPRS_Ready,GPRS_OK_BIT) //GPRS模块运行正常
             && TESTBIT(DBLHIPRN,BIT5) //允许发送Heart-Beat
             && (force || (ACTIVE && GetSystemTimer()>IDLE_HB)))
    {
        if (GPRSStartGprs())
        {
            //发送收款机编号数据作为HeartBeat信息
            HeartBuffer[0]='H';
            HeartBuffer[1]=sizeof(ApplVar.IP.ECR_ID)+1;
            HeartBuffer[2]=NETIDLOG;
            memcpy(&HeartBuffer[3],ApplVar.IP.ECR_ID,sizeof(ApplVar.IP.ECR_ID));

            GPRSSendECRData(HeartBuffer,sizeof(ApplVar.IP.ECR_ID)+3);//发送HeartBeat数据
            CloseGPRS();

            IDLE_HB = GetSystemTimer()+SECONDS(ACTIVE*60);//下一次发送Heart-Beat的时间间隔;
        }
    }
}//<<<<<<<<<<<<<<<<<<<<<<

/**
 * GPRS_ProcessRecord ccr2016-08-19
 * 处理收到记录数据,
 *
 * @author EutronSoftware (2015-01-27)
 *
 * @param cmd :'U','D','F'
 * @param data :[func]+[data]
 *     data[0]+data[1]:存放功能码
 *     data+2:为对应功能码的数据
 * @param psize :size of data(=data[0]+1)
 *
 * @return BYTE :=true,收到一条数据;=false,收到一条错误的数据.
 */

BYTE GPRS_ProcessRecord(char cmd,BYTE *srvData,BYTE psize)
{
//ccr2016-08-19    static UnLong logFirst=0;

//ccr2016-08-19    char sBuffer[sizeof(union FLOWREC)+5];

    WORD start, end, sMax, id, save, type;
    short size;
    UnLong sAddr;
    char *record;
    BYTE sPC,sBAR,sBAL,sEcrNo,sLocate,sEXTMem;
    BYTE err_state=true;
    char cmdReturn=0;//当其值不为0时,需要向主机进行应答.


#if !defined(CASE_ASCIIDISPLAY)
      mDrawGPRS(SCREENWD-1,SCREENLN-1,1);//ccr2015-01-29 testonly
#endif

    type = *((WORD *)(srvData));   // type: get the DataType..

    id = 0;
    if (type > CLERK)
        save = ApplVar.ClerkNumber;
    else if (type > SALPER)
        save = ApplVar.SalPerNumber;
    switch (cmd)       /* determine record type */
    {
    case cmdUPDATE://Update the ECR file
//ccr2016-08-19    case cmdDOWNLOAD: //Send ECR file to host
        {
            size = 0;
            record = 0;
            switch (type)
            {
            case PROMO:
                size = sizeof(ApplVar.AP.Promotion);
                record = (char *)&ApplVar.AP.Promotion;
                break;
            case SALPER:
                size = sizeof(ApplVar.AP.SalPer);
                record = (char *)&ApplVar.AP.SalPer;
                break;
            case OPTIONS:
                size = sizeof(ApplVar.AP.Options);
                record = (char *)&ApplVar.AP.Options;
                break;
            case PROGID :  /* program ID data */
                size = sizeof(Release);
                record = (char *)Release;
                break;
            case SYSFLG :  /* system Flags */
                size = sizeof(ApplVar.AP.Flag);
                record = (char *)ApplVar.AP.Flag;
                break;
            case START :  /* start addresses */
                size = sizeof(ApplVar.AP.StartAddress) ;
                record = (char *)ApplVar.AP.StartAddress;
                break;
            case RCONFIG : /* config */
                size = sizeof(ApplVar.AP.Config);
                record = (char *)&ApplVar.AP.Config;
                break;
            case RDAY : /* DAY table */
                size = sizeof(ApplVar.AP.Day);
                record = (char *)&ApplVar.AP.Day;
                break;
            case RMONTH : /* ApplVar.Month table */
                size = sizeof(ApplVar.AP.Month);
                record = (char *)&ApplVar.AP.Month;
                break;
            case RZONE : /* zone table */
                size = sizeof(ApplVar.AP.Zone);
                record = (char *)&ApplVar.AP.Zone;
                break;
            case RTOTAL : /* ApplVar.Total sales */
                size = sizeof(ApplVar.AP.Sales);
                record = (char *)&ApplVar.AP.Sales;
                break;
            case KEY0 :
                size = 64;     /* 64 bytes is 32 keys */
                record = (char *)ApplVar.AP.KeyTable;
                break;
            case KEY32 :
                size = 64;     /* 64 bytes is 32 keys */
                record = (char *)&ApplVar.AP.KeyTable[32];
                break;
    //		case KEY64 :
    //		    size = 64;     /* 64 bytes is 32 keys */
    //			record = (char *)&ApplVar.AP.KeyTable[64];
    //		    break;
    //		case KEY96 :
    //		    size = 64;     /* 64 bytes is 32 keys */
    //			record = (char *)&ApplVar.AP.KeyTable[96];
    //		    break;
            case RREP1 :        /* user report 1,2 & 3 */
                size =  3 * sizeof(struct FIXEDREPORT);
                record = (char *)&ApplVar.AP.ReportList[0];
                break;
            case RREP2 :        /* user report 4,5 & 6 */
                size =  3 * sizeof(struct FIXEDREPORT);
                record = (char *)&ApplVar.AP.ReportList[3];
                break;
            case RREP3 :        /* user report 7,8 & 9 */
                size = 3 * sizeof(struct FIXEDREPORT);
                record = (char *)&ApplVar.AP.ReportList[6];
                break;
#if (salNumber)
			case RREP4 :        /* user report 7,8 & 9 */
				size = 3 * sizeof(struct FIXEDREPORT);
				record = (char *)&ApplVar.AP.ReportList[9];
				break;
			case RREP5 :        /* user report 7,8 & 9 */
				size = 1 * sizeof(struct FIXEDREPORT);
				record = (char *)&ApplVar.AP.ReportList[12];
				break;
#else
            case RREP4 :        /* user report 7,8 & 9 */
                size = 1 * sizeof(struct FIXEDREPORT);
                record = (char *)&ApplVar.AP.ReportList[9];
                break;
#endif

            case REPCAP1 :        /* report types 0 - 6 */
                size = 7 * REPWIDTH;
                record = (char *)ApplVar.TXT.ReportType;
                break;
            case REPCAP2 :        /* report types 7 - 13 */
                size = 7 * REPWIDTH;
                record = (char *)ApplVar.TXT.ReportType[7];
                break;
            case REPCAP3 :        /* messages 14 - 15 */
                size = 2 * REPWIDTH;
                record = (char *)ApplVar.TXT.ReportType[14];
                break;
            case RHEAD1 :       /* receipt header 0 - 4 */
                size = 5 * (PRTLEN+1);
                record = (char *)ApplVar.TXT.Header;
                break;
            case RHEAD2 :       /* receipt header 5 - 7 */
                size = 3 * (PRTLEN+1);
                record = (char *)ApplVar.TXT.Header[5];
                break;
            case RTRAIL1 :       /* receipt trailer 0 - 4 */
                size = 5 * (PRTLEN+1);
                record = (char *)ApplVar.TXT.Trailer;
                break;
            case RTRAIL2 :       /* receipt trailer 5 - 5 */
                size = 1 * (PRTLEN+1);
                record = (char *)ApplVar.TXT.Trailer[5];
                break;
            case RSLIPH1 :       /* slip header 0 - 2 */
                size = 3 * SHEADWIDTH;
                record = (char *)ApplVar.TXT.SlipHeader;
                break;
            case RSLIPH2 :       /* receipt header 3 - 5 */
                size = 3 * SHEADWIDTH;
                record = (char *)ApplVar.TXT.SlipHeader[3];
                break;

            case FIRM :
                size = sizeof(ApplVar.AP.FirmKeys);
                record = (char *)ApplVar.AP.FirmKeys;
                break;
            case MNGR :
                size = sizeof(ApplVar.AP.Manager);
                record = (char *)ApplVar.AP.Manager;
                break;
            case CORREC :
                size = sizeof(ApplVar.AP.Correc);
                record = (char *)&ApplVar.AP.Correc;
                break;
            case CURR :
                size = sizeof(ApplVar.AP.Curr);
                record = (char *)&ApplVar.AP.Curr;
                break;
            case DISC :
                size = sizeof(ApplVar.AP.Disc);
                record = (char *)&ApplVar.AP.Disc;
                break;
            case DRAW :
                size = sizeof(ApplVar.AP.Draw);
                record = (char *)&ApplVar.AP.Draw;
                break;
            case PBF :
                size = sizeof(ApplVar.AP.Pb);
                record = (char *)&ApplVar.AP.Pb;
                break;
            case PORA :
                size = sizeof(ApplVar.AP.PoRa);
                record = (char *)&ApplVar.AP.PoRa;
                break;
            case TEND :
                size = sizeof(ApplVar.AP.Tend);
                record = (char *)&ApplVar.AP.Tend;
                break;
            case MODI :
                size = sizeof(ApplVar.AP.Modi);
                record = (char *)&ApplVar.AP.Modi;
                break;
            case TAX :
                size = sizeof(ApplVar.AP.Tax);
                record = (char *)&ApplVar.AP.Tax;
                break;
            case CLERK :
                size = sizeof(ApplVar.AP.Clerk);
                record = (char *)&ApplVar.AP.Clerk;
                break;
            case GROUP :
                size = sizeof(ApplVar.AP.Group);
                record = (char *)&ApplVar.AP.Group;
                break;
            case DEPT :
                size = sizeof(ApplVar.AP.Dept);
                record = (char *)&ApplVar.AP.Dept;
                break;
            case PLU1 :
                size = sizeof(ApplVar.AP.Plu);
                record = (char *)&ApplVar.AP.Plu;
                break;
            case OFFER:
                size = sizeof(ApplVar.AP.OFFPrice);
                record = (char *)&ApplVar.AP.OFFPrice;
                break;
            case AGREE :
                size = sizeof(ApplVar.AP.Agree);
                record = (char *)&ApplVar.AP.Agree;
                break;
            case BLOCKIC:       //ccr chipcard
                size = sizeof(ApplVar.AP.ICBlock);
                record = (char *)&ApplVar.AP.ICBlock;
                break;
            case CHIPCARDSET:   //ccr chipcard
                size = sizeof(ApplVar.ICCardSet);
                record = (char *)&ApplVar.ICCardSet;
                break;
            case REMOTESETTIME :
                if (TESTBIT(ApplVar.MyFlags,ZREPORT))   /* z report taken ? */
                {
                    CheckError(ERROR_ID(CWXXI13));
                    type = 0xffff;
                } else
                {
                    ApplVar.ErrorNumber=0;
                    memcpy(&EntryBuffer[ENTRYSIZE - 9],&srvData[-1+3],8);
                    Appl_EntryCounter = 8;
                    NewTimeDate(1);
                    memcpy(&EntryBuffer[ENTRYSIZE - 7],&srvData[-1+11],6);
                    Appl_EntryCounter = 6;
                    NewTimeDate(2);
                    if (ApplVar.ErrorNumber)
                    {
                        CheckError(0);
                        type = 0xffff;
                    }
                    ClearEntry(true);
                    InActive |= ACTIVE+1;     /* Inactive longer then allowed */
                    CheckTime(false);
                }
                cmdReturn = cmdANSWER;//ccr2016-08-19return tcp_AnswerToHost(tpcb,cmdANSWER,type);
#if defined(ACK_MUST)
                GPRS_WaitACK = true;
#endif
                return (cmdReturn);//ccr2016-10-27
            default :
                break;
            }//switch(type)
            if (size)
            {//更新收款机的配置数据,含票头/票尾/键盘/系统参数等
#if defined(ACK_MUST)
                GPRS_WaitACK = true;
#endif
                if (cmd == cmdUPDATE)   /* update */
                {
                    sEcrNo = REGISTER;
                    sLocate = LOCATION;
                    sPC = COMPUTER;
                    sBAR = BARCODE;
                    sBAL = BALANCE;
                    sEXTMem = EXTRAMN;
                    if (type == CHIPCARDSET)
                        sAddr = ApplVar.ICCardSet.PosCode;
                    memcpy(record, &srvData[-1+3], size);//将数据存入收款机配置文件中
                    if (type == CHIPCARDSET)
                        ApplVar.ICCardSet.PosCode = sAddr;
                    if (type == SYSFLG)
                    {
                        InitSysFlag();
                        COMPUTER = sPC;
                        BARCODE = sBAR;
                        BALANCE = sBAL;
                        REGISTER = sEcrNo;
                        LOCATION = sLocate;
                        EXTRAMN =  sEXTMem;
                    } else if (type == START)
                    {//更新地址分配,收到的地址分配为相对地址!!!!
                        ClsXRam();
                        if (ResetStartAddress(true)==false)
                        {
                            Bell(2);
                            DispSt1C(Msg[WJKJYJIE].str);
                            RJPrint(0,Msg[WJKJYJIE].str);


                            //重新计算内存中可容纳的单品数目
                            sAddr = ApplVar.AP.StartAddress[AddrEndP] - ApplVar.SIZE_EXTRAM;

                            sAddr /= pluRecordSize;
                            sAddr++;//为需要减少的单品数目
                            sMax = sAddr;
                            sAddr *= pluRecordSize;//所减少的单品所占用的空间
                            if (sMax<ApplVar.AP.Plu.Number)
                            {
                                ApplVar.AP.Plu.Number -= sMax;
                                if (ApplVar.AP.Plu.RNumber>ApplVar.AP.Plu.Number)
                                    ApplVar.AP.Plu.RNumber = ApplVar.AP.Plu.Number;
                            } else
                            {
                                RJPrint(0,(char*)cMessageE18);//"!!!! FAT(PLU) ERROR !!!!"
                                ApplVar.AP.Plu.Number = ApplVar.AP.Plu.RNumber = 0;
                            }

                            for (sMax = AddrPLU+1;sMax<AddrMAX;sMax++)
                                ApplVar.AP.StartAddress[sMax] -= sAddr;


                        }
                        ApplVar.MyFlags = 0;
                        //Save_ApplRam();
                    } else if (type == KEY0 || type == KEY32)
                    {
                        DeptPluKeysCount();

                        ApplVar.AP.FirmKeys[ID_xDEPT] = 0xff;
                        ApplVar.AP.FirmKeys[ID_DATE] = 0xff;
                        ApplVar.AP.FirmKeys[ID_RJFEED] = 0xff;
                        ApplVar.AP.FirmKeys[ID_CLEAR] = 0xff;
                        ApplVar.AP.FirmKeys[ID_LOCK] = 0xff;

                        for (id=0;id<MAXKEYB;id++)
                        {
                            switch (ApplVar.AP.KeyTable[id])
                            {
                            case SHIFT1:
                                ApplVar.AP.FirmKeys[ID_xDEPT] = id;
                                break;
                            case DATETIME:
                                ApplVar.AP.FirmKeys[ID_DATE] = id;
                                break;
                            case JPF:
                            case RPF:
                                ApplVar.AP.FirmKeys[ID_RJFEED] = id;
                                break;
                            case CLEAR:
                                ApplVar.AP.FirmKeys[ID_CLEAR] = id;
                                break;
                            case MODELOCK:
                                ApplVar.AP.FirmKeys[ID_LOCK] = id;
                                break;
                            }
                        }
                    }
                    cmdReturn = cmdANSWER;//ccr2016-08-19return tcp_AnswerToHost(tpcb,cmdANSWER,type);//应答host,处理完毕
                }
                //为'D'命令,把配置数据发送给主机
                if (type == START)//转换为相对地址发送给主机
                    ResetStartAddress(false);
                cmdReturn = cmdANSWER;//ccr2016-08-19err_state = tcp_RecordToHost(tpcb,cmdUPDATE,type,record,size);/* send record */
                if (type == START)//恢复为绝对地址
                {
                    ResetStartAddress(true);
                    Init_Flow();
                }
                Save_Config(true);//ccr2014-10-30
            } else
            {
                if (type == ADDPLU || type == DELPLU || type == UPDPLU)
                {
                    if (cmd != cmdUPDATE)
                    {
//ccr2016-08-23                        cmdReturn = cmdANSWER;
                        break;
                    }
                    if (ApplVar.AP.Plu.RandomSize && ApplVar.AP.Plu.Number)
                    {
                        ApplVar.Key.Code = 0;   /* suppress repeat */
                        memcpy(&ApplVar.Plu, &srvData[-1+3], sizeof(ApplVar.Plu));
                        ApplVar.PluNumber = GetPluNumber(1, ApplVar.Plu.Random);        /* fill ApplVar.PluNumber with number */
                        switch (type)
                        {
                        case DELPLU:
                            if (ApplVar.PluNumber)
                            {
                                CheckRandomPlu(1, 1);       /* delete plu */
                                SETBIT(ApplVar.MyFlags,CONFIGECR);
                            }
                            break;
                        case ADDPLU:
                            if (!ApplVar.PluNumber)  /* Not Found then move and Add */
                            {
                                CheckRandomPlu(0, 1);//ccr2014-12-10
                                //ccr2014-12-10if (CheckRandomPlu(0, 1))
                                //ccr2014-12-10	ApplVar.PluNumber = GetPluNumber(0, ApplVar.Plu.Random);        /* fill ApplVar.PluNumber with number */
                                SETBIT(ApplVar.MyFlags,CONFIGECR);
                                break;//ccr2014-12-10
                            }
                        case UPDPLU:
                            if (ApplVar.PluNumber)
                            {
                                ApplVar.PluNumber--;
                                WritePlu();
                                SETBIT(ApplVar.MyFlags,CONFIGECR);
                            }
                        default:
                            break;
                        }
                        cmdReturn = cmdDOWNLOAD;//ccr2016-08-19err_state = tcp_AnswerToHost(tpcb,cmdANSWER,type);//应答主机,下载数据处理完毕,请求下载下一条
                    }
                    break;
                } else if (type > PBTOT)
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        PbTotal(type - PBTOT, 0);
                        if (!(PBINFO & 0x04) && !(PBPRINT & 0x80))       /* open numbers ? */
                            memcpy(ApplVar.PB.Random, &srvData[-1+3], 7);
                        memcpy(ApplVar.PB.Text, &srvData[-1+10], sizeof(ApplVar.PB.Text));
                        PbTotal(type - PBTOT, 3);
                        cmdReturn = cmdDOWNLOAD;//ccr2016-08-19err_state = tcp_AnswerToHost(tpcb,cmdDOWNLOAD,type+1);//应答主机,下载数据处理完毕,请求下载下一条
                        break;
                    }
                    id = PBTOT + 1;
                    if ((ApplVar.AP.Pb.Random & 0x0f) || ApplVar.AP.Pb.Text)
                        sMax = ApplVar.AP.Pb.NumberOfPb;
                    else
                        sMax = 0;                /* Not active */
                    size = sizeof(ApplVar.PB.Random) + sizeof(ApplVar.PB.Text);
                    record = (char *)ApplVar.PB.Random;
                } else if (type > BLOCKIC)    /* Chipcard for block ? ccr chipcard */
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        ApplVar.ICBlockNumber = type - BLOCKIC - 1;
                        if (ApplVar.ICBlockNumber < ApplVar.AP.ICBlock.Number)
                        {
                            memcpy(&ApplVar.ICBlock, &srvData[-1+3], sizeof(ApplVar.ICBlock));
                            WriteICBlock();
                            cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                        } else
                            cmdReturn = cmdANSWER;
                        break;
                    }
                    id = BLOCKIC + 1;
                    sMax = ApplVar.AP.ICBlock.Number;
                    size = sizeof(ApplVar.ICBlock);
                    record = (char *)&ApplVar.ICBlock;

                } else if (type > PLU1)   /* plu */
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        ApplVar.PluNumber = type - PLU1 - 1;
                        if (ApplVar.PluNumber < ApplVar.AP.Plu.Number)
                        {
                            memcpy(&ApplVar.Plu, &srvData[-1+3], sizeof(ApplVar.Plu));
                            WritePlu();
                            if (ApplVar.PluNumber==0)//ccr2015-02-26
                                ApplVar.AP.Plu.RNumber=0;//ccr2015-02-26
                            if (ApplVar.AP.Plu.RandomSize &&    /* set PLU's used */
                                (ApplVar.PluNumber >= ApplVar.AP.Plu.RNumber))
                            {
                                ApplVar.AP.Plu.RNumber = (ApplVar.PluNumber + 1);
                            }
                            cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                        } else
                            cmdReturn = cmdANSWER;
                        break;
                    }
                    id = PLU1 + 1;
                    if (ApplVar.AP.Plu.RandomSize)    /* set PLU's used */
                        sMax = ApplVar.AP.Plu.RNumber;
                    else
                        sMax = ApplVar.AP.Plu.Number;
                    size = sizeof(ApplVar.Plu);
                    record = (char *)&ApplVar.Plu;
                } else if (type > DEPT)   /* dept */
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        ApplVar.DeptNumber = type - DEPT - 1;
                        if (ApplVar.DeptNumber<ApplVar.AP.Dept.Number)
                        {
                            memcpy(&ApplVar.Dept, &srvData[-1+3], sizeof(ApplVar.Dept));
                            WriteDept();
                            cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                        } else
                            cmdReturn = cmdANSWER;
                        break;
                    }
                    id = DEPT + 1;
                    sMax = ApplVar.AP.Dept.Number;
                    size = sizeof(ApplVar.Dept);
                    record = (char *)&ApplVar.Dept;
                } else if (type > GROUP)   /* group */
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        ApplVar.GroupNumber = type - GROUP - 1;
                        if (ApplVar.GroupNumber<ApplVar.AP.Group.Number)
                        {
                            memcpy(&ApplVar.Group, &srvData[-1+3], sizeof(ApplVar.Group));
                            WriteGroup();
                            cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                        } else
                            cmdReturn = cmdANSWER;
                        break;
                    }
                    id = GROUP + 1;
                    sMax = ApplVar.AP.Group.Number;
                    size = sizeof(ApplVar.Group);
                    record = (char *)&ApplVar.Group;
                } else if (type > OFFER)   /* OFFER */
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        ApplVar.OFFNumber = type - OFFER -1;
                        if (ApplVar.OFFNumber<ApplVar.AP.OFFPrice.Number)
                        {
                            memcpy(&ApplVar.OFFPrice, &srvData[-1+3], sizeof(ApplVar.OFFPrice));
                            WriteOFFPrice();
                            cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                        } else
                            cmdReturn = cmdANSWER;
                        break;
                    }
                    id = OFFER+1;
                    sMax = ApplVar.AP.OFFPrice.Number;
                    size = sizeof(ApplVar.OFFPrice);
                    record = (char *)&ApplVar.OFFPrice;
                } else if (type > CLERK)   /* clerk */
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        if (ApplVar.ClerkNumber<=ApplVar.AP.Clerk.Number)
                        {
                            ApplVar.ClerkNumber = type - CLERK;
                            memcpy(&ApplVar.Clerk, &srvData[-1+3], sizeof(ApplVar.Clerk));
                            WriteClerk();
                            cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                        } else
                            cmdReturn = cmdANSWER;
                        break;
                    }
                    id = CLERK;
                    sMax = ApplVar.AP.Clerk.Number + 1;
                    size = sizeof(ApplVar.Clerk);
                    record = (char *)&ApplVar.Clerk;
                } else if (type > TAX)    /* modifier */
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        ApplVar.TaxNumber = type - TAX - 1;
                        if (ApplVar.TaxNumber<ApplVar.AP.Tax.Number)
                        {
                            memcpy(&ApplVar.Tax, &srvData[-1+3], sizeof(ApplVar.Tax));
                            WriteTax();
                            cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                        } else
                            cmdReturn = cmdANSWER;
                        break;
                    }
                    id = TAX + 1;
                    sMax = ApplVar.AP.Tax.Number;
                    size = sizeof(ApplVar.Tax);
                    record = (char *)&ApplVar.Tax;
                } else if (type > SALPER)    /* sales person */
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        ApplVar.SalPerNumber = type - SALPER;
                        if (ApplVar.SalPerNumber<=ApplVar.AP.SalPer.Number)
                        {
                            memcpy(&ApplVar.SalPer, &srvData[-1+3], sizeof(ApplVar.SalPer));
                            WriteSalPer();
                            cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                        } else
                            cmdReturn = cmdANSWER;
                        break;
                    }
                    id = SALPER;
                    sMax = ApplVar.AP.SalPer.Number+1;
                    size = sizeof(ApplVar.SalPer);
                    record = (char *)&ApplVar.SalPer;
                } else if (type > MODI)    /* modifier */
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        ApplVar.ModiNumber = type - MODI - 1;
                        if (ApplVar.ModiNumber<ApplVar.AP.Modi.Number)
                        {
                            memcpy(&ApplVar.Modi, &srvData[-1+3], sizeof(ApplVar.Modi));
                            WriteModi();
                            cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                        } else
                            cmdReturn = cmdANSWER;
                        break;
                    }
                    id = MODI + 1;
                    sMax = ApplVar.AP.Modi.Number;
                    size = sizeof(ApplVar.Modi);
                    record = (char *)&ApplVar.Modi;
                } else
                {
                    id = (type / 100) * 100 + 1;
                    if (type > TEND)
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.TendNumber = type - TEND - 1;
                            if (ApplVar.TendNumber<ApplVar.AP.Tend.Number)
                            {
                                memcpy(&ApplVar.Tend, &srvData[-1+3], sizeof(ApplVar.Tend));
                                WriteTender();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        sMax = ApplVar.AP.Tend.Number;
                        size = sizeof(ApplVar.Tend);
                        record = (char *)&ApplVar.Tend;
                    } else if (type > PORA)
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.PoRaNumber = type - PORA - 1;
                            if (ApplVar.PoRaNumber<ApplVar.AP.PoRa.Number)
                            {
                                memcpy(&ApplVar.PoRa, &srvData[-1+3], sizeof(ApplVar.PoRa));
                                WritePoRa();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        sMax = ApplVar.AP.PoRa.Number;
                        size = sizeof(ApplVar.PoRa);
                        record = (char *)&ApplVar.PoRa;
                    } else if (type > PBF)
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.PbFNumber = type - PBF - 1;
                            if (ApplVar.PbFNumber<ApplVar.AP.Pb.Number)
                            {
                                memcpy(&ApplVar.PbF, &srvData[-1+3], sizeof(ApplVar.PbF));
                                WritePbF();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        sMax = ApplVar.AP.Pb.Number;
                        size = sizeof(ApplVar.PbF);
                        record = (char *)&ApplVar.PbF;
                    } else if (type > DRAW)
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.DrawNumber = type - DRAW - 1;
                            if (ApplVar.DrawNumber<ApplVar.AP.Draw.Number)
                            {
                                memcpy(&ApplVar.Draw, &srvData[-1+3], sizeof(ApplVar.Draw));
                                WriteDrawer();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        sMax = ApplVar.AP.Draw.Number;
                        size = sizeof(ApplVar.Draw);
                        record = (char *)&ApplVar.Draw;
                    } else if (type > DISC)
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.DiscNumber = type - DISC - 1;
                            if (ApplVar.DiscNumber<ApplVar.AP.Disc.Number)
                            {
                                memcpy(&ApplVar.Disc, &srvData[-1+3], sizeof(ApplVar.Disc));
                                WriteDisc();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        sMax = ApplVar.AP.Disc.Number;
                        size = sizeof(ApplVar.Disc);
                        record = (char *)&ApplVar.Disc;
                    } else if (type > CURR)
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.CurrNumber = type - CURR - 1;
                            if (ApplVar.CurrNumber<ApplVar.AP.Curr.Number)
                            {
                                memcpy(&ApplVar.Curr, &srvData[-1+3], sizeof(ApplVar.Curr));
                                WriteCurr();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        sMax = ApplVar.AP.Curr.Number;
                        size = sizeof(ApplVar.Curr);
                        record = (char *)&ApplVar.Curr;
                    } else if (type > CORREC)
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.CorrecNumber = type - CORREC - 1;
                            if (ApplVar.CorrecNumber<ApplVar.AP.Correc.Number)
                            {
                                memcpy(&ApplVar.Correc, &srvData[-1+3], sizeof(ApplVar.Correc));
                                WriteCorrec();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        sMax = ApplVar.AP.Correc.Number;
                        size = sizeof(ApplVar.Correc);
                        record = (char *)&ApplVar.Correc;
                    } else
                    {
                        cmdReturn = cmdANSWER;
                        break;
                    }
                }
#if (0)//ccr2016-08-19 不提供上传文件数据的功能
                start = type - id;
                if (start < sMax)
                {//将指定文件的一条记录数据发送给主机
                    MemSet(record, size, 0);    /* clear memory */
                    memset(sBuffer,' ',INPUTWIDTH);
                    if (type > PBTOT)
                    {
                        CopyFrStr(sBuffer,Msg[SETPBINF].str);
                        PbTotal(start + 1, 0);  /* read */
                    } else if (type > BLOCKIC)//ccr chipcard
                    {
                        CopyFrStr(sBuffer,Msg[GUASHIIC].str);
                        ApplVar.ICBlockNumber = start;
                        ReadICBlock();
                    } else if (type > PLU1)
                    {
                        CopyFrStr(sBuffer,Msg[SETPLU].str);
                        ApplVar.PluNumber= start;
                        ReadPlu();
                    } else if (type > DEPT)
                    {
                        CopyFrStr(sBuffer,Msg[SETDEPT].str);
                        ApplVar.DeptNumber = start;
                        ReadDept();
                    } else if (type > GROUP)
                    {
                        CopyFrStr(sBuffer,Msg[SETGROUP].str);
                        ApplVar.GroupNumber = start;
                        ReadGroup();
                    } else if (type > OFFER)
                    {
                        CopyFrStr(sBuffer,Msg[SETOFF].str);
                        ApplVar.OFFNumber = start;
                        ReadOFFPrice();
                    } else if (type > CLERK)
                    {
                        CopyFrStr(sBuffer,Msg[SETCLERK].str);
                        ApplVar.ClerkNumber = start;
                        ReadClerk();
                    } else if (type > TAX)    /* tax */
                    {
                        CopyFrStr(sBuffer,Msg[SETTAX].str);
                        ApplVar.TaxNumber = start;
                        ReadTax();
                    } else if (type > SALPER)    /* salesperson */
                    {
                        CopyFrStr(sBuffer,Msg[SETSALER].str);
                        ApplVar.SalPerNumber = start;
                        ReadSalPer();
                    } else if (type > MODI)    /* modifier */
                    {
                        CopyFrStr(sBuffer,Msg[SETMODIF].str);
                        ApplVar.ModiNumber = start;
                        ReadModi();
                    } else if (type > TEND)    /* tender */
                    {
                        CopyFrStr(sBuffer,Msg[SETTEND].str);
                        ApplVar.TendNumber = start;
                        ReadTender();
                    } else if (type > PORA)    /* pora */
                    {
                        CopyFrStr(sBuffer,Msg[SETPORA].str);
                        ApplVar.PoRaNumber = start;
                        ReadPoRa();
                    } else if (type > PBF)    /* pb */
                    {
                        CopyFrStr(sBuffer,Msg[SETPBF].str);
                        ApplVar.PbFNumber = start;
                        ReadPbF();
                    } else if (type > DRAW)    /* drawer */
                    {
                        CopyFrStr(sBuffer,Msg[SETDRAWER].str);
                        ApplVar.DrawNumber = start;
                        ReadDrawer();
                    } else if (type > DISC)    /* discount */
                    {
                        CopyFrStr(sBuffer,Msg[SETDISC].str);
                        ApplVar.DiscNumber = start;
                        ReadDisc();
                    } else if (type > CURR)    /* currency */
                    {
                        CopyFrStr(sBuffer,Msg[SETCURR].str);
                        ApplVar.CurrNumber = start;
                        ReadCurr();
                    } else                  /* correction */
                    {
                        CopyFrStr(sBuffer,Msg[SETCORR].str);
                        ApplVar.CorrecNumber = start;
                        ReadCorrec();
                    }
                    WORDtoASC(sBuffer+INPUTWIDTH-1,start);
                    DispSt2(sBuffer);

                    err_state = tcp_RecordToHost(tpcb,cmdUPDATE,type,record, size);//发送记录数据
                } else
                    cmdReturn = cmdANSWER;//无所需要的数据
#endif
            }
        }
        break;
    case cmdCLEAR:           /* clear memory ? */
        if (type == 0x524c) /* command "C  LR" */
        {
            id = ApplVar.CentralLock;
            start = ApplVar.ClerkNumber;
            end = ApplVar.ClerkLock;
            sMax = ApplVar.SalPerNumber;
//				ClearApplMemory();
            ClearAllReport();
            ApplVar.CentralLock = id;
            ApplVar.ClerkNumber = start;
            ApplVar.ClerkLock = end;
            ApplVar.SalPerNumber = sMax;
        }
        cmdReturn = cmdANSWER;
        ARROWS = 0x01;
        break;
#if (0)//ccr2016-08-19 不提供提取流水的功能
        case cmdINIT:           /* init */
            if (type == 0x544e) /* command "INT" */
            {
                InitApplication(true);//强制进行初始化
            }
            cmdReturn = cmdANSWER;//应答主机,数据处理完毕
            ARROWS = 0x01;
            break;
    case cmdFLOWDATA:
        if(ApplVar.FRegi || ApplVar.FlowHeader.ReadP==ApplVar.FlowHeader.NextNP)
        {//在销售状态下,禁止提取流水数据
            cmdReturn = cmdANSWER;
        }
        else
        {//发送收款机销售流水,数据发送格式:'F'+[size]+[type1]+[type2]+[data]; [type1]+[type2]为流水顺序号
            if (logFirst!=0)
                ApplVar.FlowHeader.ReadP = logFirst;

            if (type==0)
            {
                logFirst = ApplVar.FlowHeader.ReadP;
                sBuffer[0] = ECRNOLOG;
                sBuffer[1] = REGISTER;
                sBuffer[2] = LOCATION;
                return tcp_RecordToHost(tpcb,cmdFLOWDATA,type,sBuffer, 3);//发送记录数据
            }
            sAddr = ApplVar.FlowHeader.ReadP;
            size = Read_Flow(sBuffer);
            logFirst = ApplVar.FlowHeader.ReadP;
            if (size <= 0)
            {
                if (size==-1)
                {
                    ApplVar.ErrorNumber = (ERROR_ID(CWXXI48));
                }

                cmdReturn = cmdANSWER;
            } else
            {
                ApplVar.FlowHeader.ReadP = sAddr;//必须在收到host的应答后,确认host收到了数据,才修正FlowHeader.ReadP
                err_state = tcp_RecordToHost(tpcb,cmdFLOWDATA,type,sBuffer, size);//发送记录数据
                if (err_state!=ERR_OK)
                {//发送失败,恢复流水指针
                    ApplVar.ErrorNumber = (ERROR_ID(CWXXI84));
                    cmdReturn = cmdANSWER;
                } else
                    return err_state;
            }
        }
        break;
    case cmdPASSWORD://ccr2015-02-04 Catesir for PWD>>>>>>>>>>>>>>>>
        switch (type)
        {
        case 0:
            memcpy(&ApplVar.AP.ModePwd.PwdX,&srvData[-1+3],MAXPWD);//MAXPWD-1
            break;
        case 1:
            memcpy(&ApplVar.AP.ModePwd.PwdZ,&srvData[-1+3],MAXPWD);
            break;
        case 2:
            memcpy(&ApplVar.AP.ModePwd.PwdSET,&srvData[-1+3],MAXPWD);
            break;
        case 3:
            memcpy(&ApplVar.AP.ModePwd.PwdMG,&srvData[-1+3],MAXPWD);
            break;
        }
        cmdReturn = cmdANSWER;
        break;
#endif
    default :
//ccr2016-08-23        cmdReturn = cmdANSWER;
        break;
    }

    if (cmdReturn)
    {//必须对host进行应答

#if (0)//ccr2016-10-25>>>>>
#if defined(ACK_MUST)
        if (GPRS_ACK_MUST)
        {
            //发送ACK应答
            sEcrNo=ACK;
            if ( cmdReturn==cmdDOWNLOAD && //需要对下载进行应答
                 (!GPRSSendECRData(&sEcrNo,1)
#if defined(GPRSWAITACK)
//ccr2016-08-11    || !GPRSCheckACK(0)//等待服务器应答
#endif
                ))
            {//发送失败时
            }
        }
#endif
#else
#if defined(ACK_MUST)
            GPRS_WaitACK = GPRS_ACK_MUST;
#endif
#endif//ccr2016-10-25<<<<<

    }

    if (type > CLERK && type<OFFER)
    {
        ApplVar.ClerkNumber = save;

        if (save)
        {
            ReadClerk();
        }

    } else if (type > SALPER  && type<TAX)
    {
        ApplVar.SalPerNumber = save;
        ReadSalPer();
    }
    if (TRAINING)
    {
        ApplVar.FTrain = 1 ;
    } else
    {
        ApplVar.FTrain = 0 ;
    }
    return (cmdReturn);
}

//ccr2016-08-26
void GPRS_Restart()
{
    //DispSt1(GPRSMess[G_WAITFOR].str);
    GPRSWaitForReady(true);
    GPRS_Wait = 0;
    KeyFrHost = ApplVar.AP.FirmKeys[ID_CANCEL];//
}

/********************************************************
 * 等待GPRS模块返回应答"OK"
 *
 * @author EutronSoftware (2017-09-29)
 *
 * @param sWaitFor :等待时长
 *
 * @return BYTE :=true,收到应答"OK",否则没有收到
 *******************************************************/
BYTE GPRSWaitForOK(ULONG sWaitFor)
{
    char strOK[32];
    short i,j;
    BYTE sEnd=0;

    i =  0;
    strOK[0]=0;
    SetTimerDelay0(sWaitFor);
    while (GetTimerDelay0()>0)
    {
        j = ReadComm(GPRSPORT);
        if (j!=-1)
        {
            GPRSECH_READ(j);//testonly

            SetTimerDelay0(sWaitFor);
            switch (j)
            {
            case '\r':
                if (sEnd!='\r')
                {
                    strOK[i]=0;
                    break;
                }
            case '\n':
                //if (sEnd == '\r')
                {
                    if (i>=2 && CWORD(strOK[0]==RET_OK))
                        return true;
                    else if (i>=5 && CLONG(strOK[0])==RET_ERROR)
                        return false;
                }
                i=0;
                break;
            default:
                if (i<sizeof(strOK)-1)
                    strOK[i++]=j & 0xff;
            }
            sEnd = j & 0xff;
        }
    }
    return (CWORD(strOK[0]==RET_OK));//false;
}

/************************************************
 * 画信号强度
 *
 * @author EutronSoftware (2017-10-11)
 ************************************************/
CONST char AT_CSQ[]="AT+CSQ\r\n";
void GPRS_DrawSignal()
{
#if !defined(CASE_ASCIIDISPLAY)
    char readGPRS[32];

    short i,j;
    BYTE  s;
#if defined(FOR_DEBUG)
    if (KbHit())
#endif
    if (TESTBIT(GPRS_Ready,GPRS_OK_BIT))
    {
        GPRSSendAString(AT_CSQ,strlen(AT_SIM));

        for (i=0;i<3;i++)
        {
            if ((j=GPRSReadAString(readGPRS,sizeof(readGPRS),SECONDS(10)))>=0)
            {
                if (CWORD(readGPRS[0])==RET_OK)
                    return;
                if (GPRSHand(readGPRS,"+CSQ:"))
                {
                    s=0;
                    for (j=5;j<10 && readGPRS[j]!=',';j++)
                        s=s*10+(readGPRS[j] & 0x0f);
                    if (s>31) s=0;
                    mDrawGPRSSignal(s,SIGNAL_X,SCREENLN-1);
                }
            }
            else
                break;
        }
    }
    //else
    //    mDrawGPRSSignal(0,SIGNAL_X,SCREENLN-1);
#endif
}

#endif
