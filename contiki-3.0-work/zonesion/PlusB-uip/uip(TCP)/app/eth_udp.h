#ifndef _eth_udp_h_
#define _eth_udp_h_

#include "ethernet.h"
#include "udp-socket.h"

#define UDP_BUF_SIZE 38

typedef struct
{
    uip_ipaddr_t RemoteIP;//Զ��IP
    uint16_t RemotePort;//Զ�˶˿ں�
    uint16_t LocalPort;//���ض˿ں�
}UDP_Control_t;

extern UDP_Control_t udpCB;
extern char UdpDataBuf[UDP_BUF_SIZE];

#endif