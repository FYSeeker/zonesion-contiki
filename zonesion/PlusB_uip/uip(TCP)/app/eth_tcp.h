#ifndef _eth_tcp_h_
#define _eth_tcp_h_

#include "ethernet.h"
#include "tcp-socket.h"


#define TCP_BUF_SIZE 38
#define TCP_SOCKET_OUTBUF_SIZE 128
#define TCP_SOCKET_INBUF_SIZE 128

typedef enum
{
    MODE_NULL = 0,
    MODE_SERVER = 1,
    MODE_CLIENT = 2,
}TCP_Mode_t;


typedef enum
{
    STA_CONNECT = 1,
    STA_UNUNITED = 2,
}TCP_Status_t;


typedef struct
{
    uip_ipaddr_t RemoteIP;//Ô¶¶ËIP
    uint16_t RemotePort;//Ô¶¶Ë¶Ë¿ÚºÅ
    uint16_t LocalPort;//±¾µØ¶Ë¿ÚºÅ
    TCP_Mode_t Mode;
    TCP_Status_t Status;
}TCP_Control_t;


extern TCP_Control_t tcpCB;
extern char TcpDataBuf[TCP_BUF_SIZE];


void TcpModeSet(TCP_Mode_t mode);
uint8_t TcpModeGet(void);

void TcpStatusSet(TCP_Status_t status);
uint8_t TcpStatusGet(void);


#endif