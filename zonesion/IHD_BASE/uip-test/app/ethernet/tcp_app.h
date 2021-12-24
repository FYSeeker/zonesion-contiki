#ifndef _tcp_app_h_
#define _tcp_app_h_

#include <contiki.h>
#include "stdio.h"
#include "uip.h"
#include "tcpip.h"

PROCESS_NAME(tcp_test);

void tcp_client_connect(void);

#endif