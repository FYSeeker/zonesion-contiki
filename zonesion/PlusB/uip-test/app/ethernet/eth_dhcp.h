#ifndef _eth_dhcp_h_
#define _eth_dhcp_h_

#include <contiki.h>
#include "uip.h"
#include "uipopt.h"
#include "tcpip.h"
#include "dhcpc.h"
#include "uip-nameserver.h"
#include "eth_ui.h"

PROCESS_NAME(dhcp_process);
extern uint8_t dhcp_flag;
extern process_event_t dhcp_event;

#endif