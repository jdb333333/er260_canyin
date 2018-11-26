/**
  ******************************************************************************
  * @file    udp_server.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   UDP echo server
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
//#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#include "udp_server.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void udp_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void udp_server_init(u16_t port)
{
   struct udp_pcb *upcb;
   err_t err;

   /* Create a new UDP control block  */
   upcb = udp_new();

   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */

      err = udp_bind(upcb, IP_ADDR_ANY, port);//ccr2015-02-09 UDP_SERVER_PORT

      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, udp_server_receive_callback, NULL);
      }
      else
      {
        LWIP_PLATFORM_ASSERT("can not bind pcb");
      }
   }
   else
   {
     LWIP_PLATFORM_ASSERT("can not create pcb");
   }
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
  int i;
  char ch;
	char *pch;

  /* Connect to the remote client */
  udp_connect(upcb, addr, port);//ccr2015-02-09 UDP_SERVER_PORT

  /* Tell the client that we have accepted it */
  //ccr2015-02-07>>>>>>>>>>>>>
  if (p && p->payload)
  {
		pch = p->payload;
    for (i=0;i<p->len;i++)
    {
      ch = pch[i] | 0x20;
      if (ch>='a' && ch<='z')
      {
        pch[i] ^= 0x20;//将收到的字符大小写转换
      }
    }
  }//<<<<<<<<<<<<<<<<<<<<<<<<
  udp_send(upcb, p);
  /* free the UDP connection, so we can accept new clients */
  udp_disconnect(upcb);

  /* Free the p buffer */
  pbuf_free(p);

}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
