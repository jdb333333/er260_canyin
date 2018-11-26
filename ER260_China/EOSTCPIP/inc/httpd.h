#ifndef __HTTPD_H
#define __HTTPD_H

#include "stm32f2xx.h"
#include "tcp.h"

#include "web_html.h"

#include <string.h>
#include <stdio.h>

#define HTTP_PORT   80

static err_t http_accept(void *arg,struct tcp_pcb *pcb,err_t err);
static err_t http_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err);

void httpd_init(void);


#endif

