#ifndef _ethernet_h_
#define _ethernet_h_

#include <contiki.h>
#include "uip_arp.h"
#include "uip.h"
#include "uipopt.h"
#include "tcpip.h"
#include "dm9051_stm.h"
#include "key.h"
#include "drive_led.h"
#include "plus_test.h"

#define BUF             ((struct uip_eth_hdr *)&uip_buf[0])
#define UIP_IP_BUF      ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_ICMP_BUF    ((struct uip_icmp_hdr *)&uip_buf[UIP_LLH_LEN + UIP_IPH_LEN])

#define PING_DATALEN        16
#define ICMP_ECHO_REPLY     0
#define ICMP_ECHO           8


extern uip_lladdr_t EthAddr;//Íø¿¨µØÖ·
extern uip_ipaddr_t HostIpAddr;
extern uip_ipaddr_t GatewayIpAddr;
extern uip_ipaddr_t NetworkMask;

PROCESS_NAME(ethernet_process);
void uipSendPing(uip_ipaddr_t *dest_addr);

#endif