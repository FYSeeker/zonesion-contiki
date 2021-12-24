#ifndef _udp_app_h_
#define _udp_app_h_

#include <contiki.h>
#include "stdio.h"
#include "uip.h"
#include "tcpip.h"

PROCESS_NAME(udp_test);

void udp_establish(void);

#endif