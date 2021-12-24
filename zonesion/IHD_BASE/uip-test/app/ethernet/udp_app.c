#include "udp_app.h"


void udp_establish()
{
    uip_ipaddr_t udpTestAddr;
    struct uip_udp_conn *udpconn;
    
    uip_ipaddr(&udpTestAddr, 192,168,1,50);    //IP address
    udpconn = udp_new(&udpTestAddr, UIP_HTONS(8090),NULL);
    if(udpconn != NULL) 
    {
        uip_udp_bind(udpconn, UIP_HTONS(8091));
    } 
}

PROCESS(udp_test, "udp_test");

PROCESS_THREAD(udp_test, ev, data)
{
    PROCESS_BEGIN();
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev == tcpip_event)
        {
            if (uip_newdata())					        //�����µ����ݰ�
            {
                printf("udp uip_newdata!\r\n");         //��ӡ��Ϣ
                uip_send(uip_appdata, uip_datalen());   //�������ݰ�
            }
        }
    }
    
    PROCESS_END();
}