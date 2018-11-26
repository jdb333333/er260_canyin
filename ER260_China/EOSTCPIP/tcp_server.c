/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Christiaan Simons rewrote this file to get a more stable echo example.
 *
 **/

/* This file was modified by ST */


#include "king.h"
#include "exthead.h"
#include "exth.h"
#include "flowbill.h"  //lyq2003


#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "tcp_server.h"


#if LWIP_TCP

static struct tcp_pcb *tcp_server_pcb;

/* ECHO protocol states */
enum tcp_server_states
{
    ES_NONE = 0,
    ES_ACCEPTED,
    ES_RECEIVED,
    ES_CLOSING
};

/* structure for maintaing connection infos to be passed as argument
   to LwIP callbacks*/
struct tcp_server_struct
{
    u8_t state;             /* current connection state */
    struct tcp_pcb *pcb;    /* pointer on the current tcp_pcb */
    struct pbuf *p;         /* pointer on the received/to be transmitted pbuf */
};


static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_server_error(void *arg, err_t err);
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
static err_t tcp_ProcessRecord(struct tcp_pcb *tpcb,char cmd,u8_t *hostString,u8_t psize);
static err_t TCPDecodeRecord(struct tcp_pcb *tpcb,u8_t *data,u8_t size);
void SaveGraphic(int sAddr,char *grphic,int length);//ccr2014-03-25


/**
  * @brief  Initializes the tcp echo server
  * @param  None
  * @retval None
  */
void tcp_server_init(uint16_t port)
{
    /* create new tcp pcb */
    tcp_server_pcb = tcp_new();

    if (tcp_server_pcb != NULL)
    {
        err_t err;

        /* bind echo_pcb to port 7 (ECHO protocol) */
        err = tcp_bind(tcp_server_pcb, IP_ADDR_ANY, port);

        if (err == ERR_OK)
        {
            /* start tcp listening for echo_pcb */
            tcp_server_pcb = tcp_listen(tcp_server_pcb);

            /* initialize LwIP tcp_accept callback function */
            tcp_accept(tcp_server_pcb, tcp_server_accept);
        } else
        {
            LWIP_PLATFORM_ASSERT("Can not bind pcb\n");
        }
    } else
    {
        LWIP_PLATFORM_ASSERT("Can not create new pcb\n");
    }
}

/**
  * @brief  This function is the implementation of tcp_accept LwIP callback
  * @param  arg: not used
  * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
  * @param  err: not used
  * @retval err_t: error status
  */
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    err_t ret_err;
    struct tcp_server_struct *es;

    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);

    /* set priority for the newly accepted tcp connection newpcb */
    tcp_setprio(newpcb, TCP_PRIO_MIN);

    /* allocate structure es to maintain tcp connection informations */
    es = (struct tcp_server_struct *)mem_malloc(sizeof(struct tcp_server_struct));
    if (es != NULL)
    {
        es->state = ES_ACCEPTED;
        es->pcb = newpcb;
        es->p = NULL;

        /* pass newly allocated es structure as argument to newpcb */
        tcp_arg(newpcb, es);

        /* initialize lwip tcp_recv callback function for newpcb  */
        tcp_recv(newpcb, tcp_server_recv);

        /* initialize lwip tcp_err callback function for newpcb  */
        tcp_err(newpcb, tcp_server_error);

        /* initialize lwip tcp_poll callback function for newpcb */
        tcp_poll(newpcb, tcp_server_poll, 1);

        ret_err = ERR_OK;
    } else
    {
        /* return memory error */
        ret_err = ERR_MEM;
    }
    return ret_err;
}


/**
  * @brief  This function is the implementation for tcp_recv LwIP callback
  * @param  arg: pointer on a argument for the tcp_pcb connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  pbuf: pointer on the received pbuf
  * @param  err: error information regarding the reveived pbuf
  * @retval err_t: error code
  */
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    struct tcp_server_struct *es;
    err_t ret_err;
    static int Packages=0;//ccr2015-01-23 testonly


    LWIP_ASSERT("arg != NULL",arg != NULL);

    es = (struct tcp_server_struct *)arg;

    /* if we receive an empty tcp frame from client => close connection */
    if (p == NULL)
    {
        /* remote host closed connection */
        es->state = ES_CLOSING;
        if (es->p == NULL)
        {
            /* we're done sending, close connection */
            tcp_server_connection_close(tpcb, es);
        } else
        {
            /* we're not done yet */
            /* acknowledge received packet */
            tcp_sent(tpcb, tcp_server_sent);

            /* send remaining data*/
            tcp_server_send(tpcb, es);
        }
        ret_err = ERR_OK;
    }
    /* else : a non empty frame was received from client but for some reason err != ERR_OK */
    else if (err != ERR_OK)
    {
        /* free received pbuf*/
        if (p != NULL)
        {
            es->p = NULL;
            pbuf_free(p);
        }
        ret_err = err;
    } else if (es->state == ES_ACCEPTED)
    {
        Packages++;//ccr2015-01-23 testonly
        /* first data chunk in p->payload */
        es->state = ES_RECEIVED;

        /* store reference to incoming pbuf (chain) */
        es->p = p;

        /* initialize LwIP tcp_sent callback function */
        tcp_sent(tpcb, tcp_server_sent);

        /* send back the received data (echo) */
        tcp_server_send(tpcb, es);

        ret_err = ERR_OK;
    } else if (es->state == ES_RECEIVED)
    {
        /* more data received from client and previous data has been already sent*/
        if (es->p == NULL)
        {
            Packages++;//ccr2015-01-23 testonly
            es->p = p;

            /* send back received data */
            tcp_server_send(tpcb, es);
        } else
        {
            struct pbuf *ptr;

            Packages++;//ccr2015-01-23 testonly
            /* chain pbufs to the end of what we recv'ed previously  */
            ptr = es->p;
            pbuf_chain(ptr,p);

        }
        ret_err = ERR_OK;
    }

    /* data received when connection already closed */
    else
    {
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);

        /* free pbuf and do nothing */
        es->p = NULL;
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}

/**
  * @brief  This function implements the tcp_err callback function (called
  *         when a fatal tcp_connection error occurs.
  * @param  arg: pointer on argument parameter
  * @param  err: not used
  * @retval None
  */
static void tcp_server_error(void *arg, err_t err)
{
    struct tcp_server_struct *es;

    LWIP_UNUSED_ARG(err);

    es = (struct tcp_server_struct *)arg;
    if (es != NULL)
    {
        /*  free es structure */
        mem_free(es);
    }
}

/**
  * @brief  This function implements the tcp_poll LwIP callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
  * @retval err_t: error code
  */
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_server_struct *es;

    es = (struct tcp_server_struct *)arg;
    if (es != NULL)
    {
        if (es->p != NULL)
        {
            /* there is a remaining pbuf (chain) , try to send data */
            tcp_server_send(tpcb, es);
        } else
        {
            /* no remaining pbuf (chain)  */
            if (es->state == ES_CLOSING)
            {
                /*  close tcp connection */
                tcp_server_connection_close(tpcb, es);
            }
        }
        ret_err = ERR_OK;
    } else
    {
        /* nothing to be done */
        tcp_abort(tpcb);
        ret_err = ERR_ABRT;
    }
    return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data)
  * @param  None
  * @retval None
  */
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    struct tcp_server_struct *es;

    LWIP_UNUSED_ARG(len);

    es = (struct tcp_server_struct *)arg;

    if (es->p != NULL)
    {
        /* still got pbufs to send */
        tcp_server_send(tpcb, es);
    } else
    {
        /* if no more data to send and client closed connection*/
        if (es->state == ES_CLOSING)
            tcp_server_connection_close(tpcb, es);
    }
    return ERR_OK;
}


/**
  * @brief  This function is used to send data for tcp connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) &&
           (es->p != NULL) &&
           (es->p->len <= tcp_sndbuf(tpcb)))
    {

        /* get pointer on pbuf from es structure */
        ptr = es->p;

        /* enqueue data for transmission */
        if (es->state == ES_RECEIVED)
            wr_err = TCPDecodeRecord(tpcb, ptr->payload, ptr->len);
        else
            wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
        if (wr_err == ERR_OK)
        {
            u16_t plen;

            plen = ptr->len;

            /* continue with next pbuf in chain (if any) */
            es->p = ptr->next;

            if (es->p != NULL)
            {
                /* increment reference count for es->p */
                pbuf_ref(es->p);
            }

            /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
            pbuf_free(ptr);

            /* Update tcp window size to be advertized : should be called when received
            data (with the amount plen) has been processed by the application layer */
            tcp_recved(tpcb, plen);
        } else if (wr_err == ERR_MEM)
        {
            /* we are low on memory, try later / harder, defer to poll */
            es->p = ptr;
            DispStrXY("ERR_MEM---",0,2);//ccr2015-01-23 testonly
        } else
        {
            /* other problem ?? */
            DispStrXY("ERROR=  ",8,2);//ccr2015-01-23 testonly
            DispCharXY(0x30-wr_err,SCREENWD-2,2);//ccr2015-01-23 testonly
        }
    }
}

/**
  * @brief  This functions closes the tcp connection
  * @param  tcp_pcb: pointer on the tcp connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{

    /* remove all callbacks */
    tcp_arg(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    /* delete es structure */
    if (es != NULL)
    {
        mem_free(es);
    }

    /* close tcp connection */
    tcp_close(tpcb);
}


//---------------------------------------------------------------------------
    #define WAITFORTYPE 0//等待数据类型:'F'-为流水,'U'为请求
    #define WAITFORSIZE 1
    #define WAITFORDATA 2
/**
 *  @author EutronSoftware (2014-12-06)
 * 从收到的数据中解析一条ECR文件数据
 * 解析结果存入EcrRecord指定的存储区
 * EcrRecord[0] :为数据长度
 * EcrRecord[1]+EcrRecord[2]:存放功能码
 * EcrRecord+3:为对应功能码的数据
 *
 * @param
 *        data:收到的数据
 *        size:收到的数据长度
 *
 * @return
 *         WORD:=0xffff时,无完整数据;=0时,数据结束;其它值:为收到的数据对应的功能码
 */
    #define EcrRecord PCBuffer

static err_t TCPDecodeRecord(struct tcp_pcb *tpcb,u8_t *data,u8_t size)
{
    static int PackageReceived=0;
    //Variable for TCPGetECRARecord
    //u8_t EcrRecord[128];
    u8_t TCP_Command;//收到的数据类型;'F','U','D'....
    u8_t TCP_SizeMust;//必须收取的数据数目
    int TCP_BytesReceived=0;//记录已经收到的数据数目
    u8_t TCP_WaitFor=WAITFORTYPE;//=0,表示等待数据类型;=1标示等待长度字节;=2表示等待数据
    int i;

    for (i=0;i<size && TCP_BytesReceived<sizeof(EcrRecord)-2;i++)
    {
        switch (TCP_WaitFor)
        {
        case WAITFORTYPE://等待数据类型
            if (*data==cmdANSWER
                || *data==cmdCLEAR
                || *data==cmdUPDATE
                || *data==cmdDOWNLOAD
                || *data==cmdFLOWDATA
                || *data==cmdGRAPHIC
                || *data==cmdINIT
                || *data==cmdPASSWORD
                || *data==cmdSERVER)//为传送流水和请求文件命令
            {
                TCP_Command = *data;
                TCP_WaitFor = WAITFORSIZE;
                TCP_BytesReceived = 0;
            }
            break;
        case WAITFORSIZE://等待数据长度,最长256字节
            //if (*data<=sizeof(union ECRFILEREC))
            {//为文件数据
                TCP_SizeMust = *data;
                TCP_WaitFor = WAITFORDATA;
                EcrRecord[TCP_BytesReceived++]=TCP_SizeMust;
                TCP_SizeMust++;
            }
            //else//非文件数据
            //     TCP_WaitFor = WAITFORTYPE;
            break;
        case WAITFORDATA://等待收取数据
            EcrRecord[TCP_BytesReceived++]=*data;
            if (TCP_BytesReceived==TCP_SizeMust)
            {//收取到一个完整的数据包
                PackageReceived++;
                TCP_WaitFor = WAITFORTYPE;
                return tcp_ProcessRecord(tpcb, TCP_Command, EcrRecord, TCP_BytesReceived);
                //return tcp_write(tpcb, EcrRecord, TCP_BytesReceived, 1);
            }
            break;
        }
        data++;
    }
    return ERR_OK;//解析了一部分数据
}

/**
 * 将命令cmd对应的数据返回给主机
 *
 * @author EutronSoftware (2015-01-27)
 *
 * @param tpcb
 * @param cmd
 * @param data
 *
 * @return err_t
 */

static err_t tcp_AnswerToHost(struct tcp_pcb *tpcb,char cmd,u16_t data)
{
    char sbuf[6];

    sbuf[0]=cmd;
    sbuf[1]=2;
    *((WORD *)(sbuf+2)) = data;
    return  tcp_write(tpcb, sbuf, 4, 1);/* send end record */
}
/**
 * 将收款机数据data发送给主机,将使用PCBuffer作为发送区
 *
 * @author EutronSoftware (2015-01-27)
 *
 * @param tpcb
 * @param cmd :应答命令
 * @param type :数据类型
 * @param data :数据
 * @param size :数据长度,必须<230;
 *
 * @return err_t
 */
static err_t tcp_RecordToHost(struct tcp_pcb *tpcb,char cmd,u16_t type,u8_t *data,u8_t size)
{
    PCBuffer[0]=cmd;
    PCBuffer[1]=size+2;
    *((WORD *)(PCBuffer+2)) = type;
    memcpy(PCBuffer+4,data,size);
    return  tcp_write(tpcb, PCBuffer,size+4, 1);/* send end record */
}

/**
 * 处理收到记录数据,
 *
 * @author EutronSoftware (2015-01-27)
 *
 * @param tpcb :soket for tcpip
 * @param cmd :'U','D','F'
 * @param data :[size]+[data]
 *     data[0] :为数据长度
 *     data[1]+data[2]:存放功能码
 *     data+3:为对应功能码的数据
 * @param psize :size of data(=data[0]+1)
 *
 * @return err_t
 */

static err_t tcp_ProcessRecord(struct tcp_pcb *tpcb,char cmd,u8_t *hostString,u8_t psize)
{
    static UnLong logFirst=0;

    char sBuffer[sizeof(union FLOWREC)+5];

    WORD start, end, sMax, id, save, type;
    short size;
    UnLong sAddr;
    char *record;
    BYTE sPC,sBAR,sBAL,sEcrNo,sLocate,sEXTMem;
    err_t err_state=ERR_OK;
    char cmdReturn=0;//当其值不为0时,需要向主机进行应答.


#if !defined(CASE_ASCIIDISPLAY)
      mDrawGPRS(SCREENWD-1,SCREENLN-1,1);//ccr2015-01-29 testonly
#endif

    type = *((WORD *)(hostString + 1));   // type: get the DataType..

    id = 0;
    if (type > PLU1 || (type >= UPDPLU && type <= REPPLU))
        save = ApplVar.PluNumber;
    else if (type > DEPT)
        save = ApplVar.DeptNumber;
    else if (type > CLERK)
        save = ApplVar.ClerkNumber;
    else if (type > SALPER)
        save = ApplVar.SalPerNumber;
    switch (cmd)       /* determine record type */
    {
    case cmdGRAPHIC://type包含图片数据的存储地址
        {
            if (ApplVar.CentralLock != SET || psize<3)
            {
                type = ('C'+('A'<<8));
                return tcp_AnswerToHost(tpcb,cmdANSWER,type);
            }

            PutsO("Update GRAP");
            DispSt1C(Msg[ZZXZTXING].str);
            SaveGraphic(type,hostString+3,psize-3);//ccr2014-03-25
            type = ('O'+('K'<<8));
            return tcp_AnswerToHost(tpcb,cmdANSWER,type);
        }
    case cmdUPDATE://Update the ECR file
        if (type > MERGEPLU-1 && type < REPPLU+1)
        {
            CheckPluFile(type);
            ApplVar.ErrorNumber=0;
            cmdReturn = cmdANSWER;
            break;
        }
    case cmdDOWNLOAD: //Send ECR file to host
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
            case RREP4 :        /* user report 7,8 & 9 */
                size = 3 * sizeof(struct FIXEDREPORT);
                record = (char *)&ApplVar.AP.ReportList[9];
                break;
            case RREP5 :        /* user report 7,8 & 9 */
                size = 1 * sizeof(struct FIXEDREPORT);
                record = (char *)&ApplVar.AP.ReportList[12];
                break;

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
            case RHEAD1 :       /* receipt header 0 - 2 */
                size = 3 * (PRTLEN+1);
                record = (char *)ApplVar.TXT.Header;
                break;
            case RHEAD2 :       /* receipt header 3 - 5 */
                size = 3 * (PRTLEN+1);
                record = (char *)ApplVar.TXT.Header[3];
                break;
            case RHEAD3 :       /* receipt header 6 - 7 */
                size = 2 * (PRTLEN+1);
                record = (char *)ApplVar.TXT.Header[6];
                break;
            case RTRAIL1 :       /* receipt trailer 0 - 2 */
                size = 3 * (PRTLEN+1);
                record = (char *)ApplVar.TXT.Trailer;
                break;
            case RTRAIL2 :       /* receipt trailer 3 - 5 */
                size = 3 * (PRTLEN+1);
                record = (char *)ApplVar.TXT.Trailer[3];
                break;
//            case RTRAIL3 :       /* receipt trailer 5 - 5 */
//                size = 1 * (PRTLEN+1);
//                record = (char *)ApplVar.TXT.Trailer[5];
//                break;
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
                    memcpy(&EntryBuffer[ENTRYSIZE - 9],&hostString[3],8);
                    Appl_EntryCounter = 8;
                    NewTimeDate(1);
                    memcpy(&EntryBuffer[ENTRYSIZE - 7],&hostString[11],6);
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
                return tcp_AnswerToHost(tpcb,cmdANSWER,type);
            default :
                break;
            }//switch(type)
            if (size)
            {//更新收款机的配置数据
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
                    memcpy(record, &hostString[3], size);//将数据存入收款机配置文件中
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
                    return tcp_AnswerToHost(tpcb,cmdANSWER,type);//应答host,处理完毕
                }
                //为'D'命令,把配置数据发送给主机
                if (type == START)//转换为相对地址发送给主机
                    ResetStartAddress(false);
                err_state = tcp_RecordToHost(tpcb,cmdUPDATE,type,record,size);/* send record */
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
                        cmdReturn = cmdANSWER;
                        break;
                    }
                    if (ApplVar.AP.Plu.RandomSize && ApplVar.AP.Plu.Number)
                    {
                        ApplVar.Key.Code = 0;   /* suppress repeat */
                        memcpy(&ApplVar.Plu, &hostString[3], sizeof(ApplVar.Plu));
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
                    }
                    err_state = tcp_AnswerToHost(tpcb,cmdANSWER,type);//应答主机,下载数据处理完毕,请求下载下一条
                    break;
                } else if (type > PBTOT)
                {
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        PbTotal(type - PBTOT, 0);
                        if (!(PBINFO & 0x04) && !(PBPRINT & 0x80))       /* open numbers ? */
                            memcpy(ApplVar.PB.Random, &hostString[3], 7);
                        memcpy(ApplVar.PB.Text, &hostString[10], sizeof(ApplVar.PB.Text));
                        PbTotal(type - PBTOT, 3);
                        err_state = tcp_AnswerToHost(tpcb,cmdDOWNLOAD,type+1);//应答主机,下载数据处理完毕,请求下载下一条
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
                            memcpy(&ApplVar.ICBlock, &hostString[3], sizeof(ApplVar.ICBlock));
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
                            memcpy(&ApplVar.Plu, &hostString[3], sizeof(ApplVar.Plu));
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
                            memcpy(&ApplVar.Dept, &hostString[3], sizeof(ApplVar.Dept));
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
                            memcpy(&ApplVar.Group, &hostString[3], sizeof(ApplVar.Group));
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
                            memcpy(&ApplVar.OFFPrice, &hostString[3], sizeof(ApplVar.OFFPrice));
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
                            memcpy(&ApplVar.Clerk, &hostString[3], sizeof(ApplVar.Clerk));
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
                            memcpy(&ApplVar.Tax, &hostString[3], sizeof(ApplVar.Tax));
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
                            memcpy(&ApplVar.SalPer, &hostString[3], sizeof(ApplVar.SalPer));
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
                            memcpy(&ApplVar.Modi, &hostString[3], sizeof(ApplVar.Modi));
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
                                memcpy(&ApplVar.Tend, &hostString[3], sizeof(ApplVar.Tend));
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
                                memcpy(&ApplVar.PoRa, &hostString[3], sizeof(ApplVar.PoRa));
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
                                memcpy(&ApplVar.PbF, &hostString[3], sizeof(ApplVar.PbF));
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
                                memcpy(&ApplVar.Draw, &hostString[3], sizeof(ApplVar.Draw));
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
                                memcpy(&ApplVar.Disc, &hostString[3], sizeof(ApplVar.Disc));
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
                                memcpy(&ApplVar.Curr, &hostString[3], sizeof(ApplVar.Curr));
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
                                memcpy(&ApplVar.Correc, &hostString[3], sizeof(ApplVar.Correc));
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
            }
        }
        break;
    case cmdINIT:           /* init */
        if (type == 0x544e) /* command "INT" */
        {
            InitApplication(true);//强制进行初始化
        }
        cmdReturn = cmdANSWER;//应答主机,数据处理完毕
        ARROWS = 0x01;
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
            memcpy(&ApplVar.AP.ModePwd.PwdX,&hostString[3],MAXPWD);//MAXPWD-1
            break;
        case 1:
            memcpy(&ApplVar.AP.ModePwd.PwdZ,&hostString[3],MAXPWD);
            break;
        case 2:
            memcpy(&ApplVar.AP.ModePwd.PwdSET,&hostString[3],MAXPWD);
            break;
        case 3:
            memcpy(&ApplVar.AP.ModePwd.PwdMG,&hostString[3],MAXPWD);
            break;
        }
        cmdReturn = cmdANSWER;
        break;
    case cmdSERVER://ccr2015-02-04 Catesir for Set IP>>>>>>>>>>>>>>>>
        ApplVar.IP.ServerIP[0]= hostString[1];
        ApplVar.IP.ServerIP[1]= hostString[2];
        ApplVar.IP.ServerIP[2]= hostString[3];
        ApplVar.IP.ServerIP[3]= hostString[4];
        cmdReturn = cmdANSWER;
        break;
    default :
        cmdReturn = cmdANSWER;
        break;
    }

    if (cmdReturn)
    {//必须对host进行应答
        if (cmdReturn==cmdDOWNLOAD)
            err_state = tcp_AnswerToHost(tpcb,cmdDOWNLOAD,type+1);//应答主机,下载数据处理完毕,请求下载下一条
        else
            err_state = tcp_AnswerToHost(tpcb,cmdReturn,0);//应答主机,数据处理完毕
    }

    if (type > PLU1  && type<AGREE || (type >= UPDPLU && type <= REPPLU))
    {
        ApplVar.RepeatCount = 0 ;
        ApplVar.PluNumber = save;
        ReadPlu();
    } else if (type > DEPT  && type<PLU1)
    {
        ApplVar.DeptNumber = save;
        ReadDept();
    } else if (type > CLERK && type<OFFER)
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
    if (!TESTBIT(ApplVar.Clerk.Options, BIT7))
    {
        ApplVar.FTrain = 1 ;
        SETBIT(ARROWS, BIT2) ;
    } else
    {
        ApplVar.FTrain = 0 ;
        RESETBIT(ARROWS, BIT2) ;
    }
    return err_state;
}

/**
 *
 * @author EutronSoftware (2015-02-05)
 * 下载用户自定义图片数据,主要下载的图片中应该包含文字
 *  图片数据最多为4096字节
 *
 * @param sAddr :图片数据的存放地址
 * @param grphic :图片数据
 * @param length :图片数据长度
 *        length=0时,图片下载结束
 */
void SaveGraphic(int sAddr,char *grphic,int length)//ccr2014-03-25
{
#if !defined(CASE_INNERDOTPRN)
#if (!defined(DEBUGBYPC) && DOWN_GR_SIZE)
    BYTE bcc,sBuf;
    int i,j;
#if (CASE_GIADA3000 || CASE_ER520U)
    char *GRBAddrFrom=(char *)DOWN_GR_ADDR;
#else
    char *GRBAddrFrom=(char *)BKPSRAM_BASE;
#endif

    if (!length)
    {
        InfoSelf_Graph.cGrapCols = 0;
        InfoSelf_Graph.cGrapSize = 0;
        //memset(&InfoSelf_Graph,0,sizeof(InfoSelf_Graph));
        if (sAddr>20)//下载的图片数据为符合要求的数据
        {
            ChangeGRBToMyself();
        } else
        {
            GRB_Define.cGraSize=0;
            GRB_Define.cTextFlag=0;
        }

        DispSt2C(Msg[WANCHENG].str);

    }
    if (sAddr==0 && grphic[0]!=0x80)
    {//无字符区,强制增加字符区,为GRB_Define.cGraSize保留空间
        grphic[0]=0x80;
        for (i=0;i<5;i++)
            GRBAddrFrom[sAddr++]=grphic[i];
        for (i=0;i<5;i++)
            GRBAddrFrom[sAddr++]=0;
        for (i=5;i<length;i++)
            GRBAddrFrom[sAddr++]=grphic[i];
    } else
    {
        for (i=0;i<length && sAddr<DOWN_GR_SIZE;i++) //最多4K数据
            GRBAddrFrom[sAddr++]=grphic[i];
    }
    memset(PCBuffer,' ',INPUTWIDTH);
    //CopyFrStr(PCBuffer,Msg[ZZXZTXING].str);
    GRB_Define.cGraSize = sAddr;
    WORDtoASC(PCBuffer+5,GRB_Define.cGraSize);
    PCBuffer[6]=0;
    DispStrXY(PCBuffer+1,INPUTDISPX,SCREENLN);
#endif
#endif
}
#endif /* LWIP_TCP */
