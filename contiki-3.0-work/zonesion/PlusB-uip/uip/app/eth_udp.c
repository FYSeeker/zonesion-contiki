#include "eth_udp.h"


UDP_Control_t udpCB = {
    .RemotePort = 8090,//远端端口号
    .LocalPort = 8091,//本地端口号
};

char UdpDataBuf[UDP_BUF_SIZE] = {0};
static struct udp_socket UdpSocket = {NULL};



void UdpSocketCallback(struct udp_socket *c,
                       void *ptr,
                       const uip_ipaddr_t *source_addr,
                       uint16_t source_port,
                       const uip_ipaddr_t *dest_addr,
                       uint16_t dest_port,
                       const uint8_t *data,
                       uint16_t datalen)
{
    for(uint8_t i=0;
        i<(datalen>UDP_BUF_SIZE ? UDP_BUF_SIZE : datalen);
        i++)
    {
        UdpDataBuf[i] = *data;
        data++;
    }
    UdpDataBuf[datalen] = 0;

    udp_socket_send(&UdpSocket,uip_appdata,uip_datalen());

    printf("RecvData: %s\r\n",UdpDataBuf);         //打印信息
}



PROCESS(eth_udp, "eth_udp");

PROCESS_THREAD(eth_udp, ev, data)
{
    static struct etimer etimer_udp;
    static char pbuf[] = {"Contiki OS UDP通信实验"};
    
    PROCESS_BEGIN();
    
    uip_ipaddr(&udpCB.RemoteIP, 192,168,1,100);//IP address
    
    udp_socket_register(&UdpSocket,NULL,UdpSocketCallback);
    udp_socket_bind(&UdpSocket,udpCB.LocalPort);
    udp_socket_connect(&UdpSocket,&udpCB.RemoteIP,udpCB.RemotePort);
    
    process_post(&eth_udp,PROCESS_EVENT_TIMER,NULL);
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev == PROCESS_EVENT_TIMER)
        {
            etimer_set(&etimer_udp,5000);
            
            udp_socket_send(&UdpSocket,pbuf,strlen(pbuf));
        }
    }
    
    PROCESS_END();
}