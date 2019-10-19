#include "eth_udp.h"


UDP_Control_t udpCB = {
    .RemotePort = 8090,//Զ�˶˿ں�
    .LocalPort = 8091,//���ض˿ں�
};

char UdpDataBuf[UDP_BUF_SIZE] = {0};


/*����udp*/
uint8_t udp_establish()
{
    uip_ipaddr(&udpCB.RemoteIP, 192,168,1,100);//IP address
    uip_udp_conn = udp_new(&udpCB.RemoteIP, UIP_HTONS(udpCB.RemotePort),NULL);
    if(uip_udp_conn != NULL) 
    {
        uip_udp_bind(uip_udp_conn, UIP_HTONS(udpCB.LocalPort));
        return 0;
    } 
    return 1;
}

PROCESS(eth_udp, "eth_udp");

PROCESS_THREAD(eth_udp, ev, data)
{
    
    PROCESS_BEGIN();
    
    udp_establish();
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev == tcpip_event)
        {
            if (uip_newdata())					        //�����µ����ݰ�
            {
                char* pbuf = (char*)uip_appdata;
                
                
                for(uint8_t i=0;
                    i<(uip_datalen()>UDP_BUF_SIZE ? UDP_BUF_SIZE : uip_datalen());
                    i++)
                {
                    UdpDataBuf[i] = *pbuf;
                    pbuf++;
                }
                UdpDataBuf[uip_datalen()] = 0;
                
                uip_send(uip_appdata, uip_datalen());   //�������ݰ�
                
                printf("udp uip_newdata!\r\n");         //��ӡ��Ϣ
            }
        }
    }
    
    PROCESS_END();
}