#include "eth_tcp.h"


TCP_Control_t tcpCB = {
    .RemotePort = 8080,//远端端口号
};


char TcpDataBuf[TCP_BUF_SIZE] = {0};

char TcpSocket_OutBuf[TCP_SOCKET_OUTBUF_SIZE] = {0};
char TcpSocket_InBuf[TCP_SOCKET_INBUF_SIZE] = {0};
static struct tcp_socket TcpSocket = {NULL};


void TcpModeSet(TCP_Mode_t mode)
{
    tcpCB.Mode = mode;
}

uint8_t TcpModeGet()
{
    return tcpCB.Mode;
}

void TcpStatusSet(TCP_Status_t status)
{
    tcpCB.Status = status;
}

uint8_t TcpStatusGet()
{
    return tcpCB.Status;
}


int TcpSocket_DataCallback(struct tcp_socket *s,
                           void *ptr,
                           const uint8_t *input_data_ptr,
                           int input_data_len)
{
    const uint8_t* pbuf = input_data_ptr;
    
    for(uint8_t i=0;
        i<(input_data_len>TCP_BUF_SIZE ? TCP_BUF_SIZE : input_data_len);
        i++)
    {
        TcpDataBuf[i] = *pbuf;
        pbuf++;
    }
    TcpDataBuf[input_data_len] = 0;
    
    tcp_socket_send(&TcpSocket,input_data_ptr,input_data_len);
    
    return 0;
}

void TcpSocket_EventCallback(struct tcp_socket *s,
                             void *ptr,
                             tcp_socket_event_t event)
{
    static char pbuf[20] = {0};
    
    if(event == TCP_SOCKET_CONNECTED)
    {
        tcpCB.LocalPort = uip_conn->lport;
        TcpStatusSet(STA_CONNECT);
        
        sprintf(pbuf,"TCP Connect OK!");
        tcp_socket_send(&TcpSocket,(uint8_t*)pbuf,strlen(pbuf));
    }
    if((event == TCP_SOCKET_CLOSED) || (event == TCP_SOCKET_ABORTED))
    {
        tcpCB.LocalPort = 0;
        TcpStatusSet(STA_UNUNITED);
    }
    if(event == TCP_SOCKET_DATA_SENT)
    {
    }
}


void TpcKeyHandle(uint8_t value)
{
    switch(value)
    {
        case 0x01:
            break;
            
        case 0x02:
            break;
            
        case 0x04:
            if(TcpModeGet() == MODE_SERVER)
            {
                tcp_socket_close(&TcpSocket);
                if(tcp_socket_unlisten(&TcpSocket))
                    TcpModeSet(MODE_NULL);
            }
            else
            {
                if(tcp_socket_listen(&TcpSocket,tcpCB.RemotePort))//创建tcp server
                    TcpModeSet(MODE_SERVER);
            }
            break;
            
        case 0x08:
            if(tcp_socket_connect(&TcpSocket,&tcpCB.RemoteIP,tcpCB.RemotePort))//连接tcp server
                TcpModeSet(MODE_CLIENT);
            break;
    }
}


PROCESS(eth_tcp, "eth_tcp");

PROCESS_THREAD(eth_tcp, ev, data)
{
    static struct etimer etimer_udp;
    static char pbuf[] = {"Contiki OS TCP通信实验"};
    
    PROCESS_BEGIN();
    
    uip_ipaddr(&tcpCB.RemoteIP, 192,168,1,100);    //IP address
    
    tcp_socket_register(&TcpSocket,
                        NULL,
                        (uint8_t*)TcpSocket_OutBuf,
                        TCP_SOCKET_OUTBUF_SIZE,
                        (uint8_t*)TcpSocket_InBuf,
                        TCP_SOCKET_INBUF_SIZE,
                        TcpSocket_DataCallback,
                        TcpSocket_EventCallback);
    
//    if(tcp_socket_listen(&TcpSocket,tcpCB.RemotePort))//创建tcp server
//        TcpModeSet(MODE_SERVER);
    
    process_post(&eth_tcp,PROCESS_EVENT_TIMER,NULL);
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev == PROCESS_EVENT_TIMER)
        {
            etimer_set(&etimer_udp,5000);
            
            if(tcpCB.Status == STA_CONNECT)
                tcp_socket_send(&TcpSocket,(uint8_t*)pbuf,strlen(pbuf));
        }
        if(ev == key_event)
        {
            TpcKeyHandle(*((uint8_t *)data));
        }
    }
    
    PROCESS_END();
}