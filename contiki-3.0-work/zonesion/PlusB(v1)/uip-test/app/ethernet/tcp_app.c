#include "tcp_app.h"


void tcp_client_connect()
{
    uip_ipaddr_t tcpConnAddr;
    
    uip_ipaddr(&tcpConnAddr, 192,168,1,50);    //IP address
    tcp_connect(&tcpConnAddr,uip_htons(1200),NULL);
}

PROCESS(tcp_test, "tcp_test");

PROCESS_THREAD(tcp_test, ev, data)
{
    PROCESS_BEGIN();
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev == tcpip_event)
        {
            if (uip_newdata())					            //�����µ�TCP���ݰ�
            {
                printf("tcp_client uip_newdata!\r\n");      //��ӡ��Ϣ
                uip_send(uip_appdata, uip_datalen());       //����TCP���ݰ�
            }
            if(uip_aborted())
            {
                printf("tcp_client uip_aborted!\r\n"); 
            }
            if(uip_closed())                                //���ӹر���,�������رյ�ʱ�򣬻����ν����������
            {
                printf("tcp_client uip_closed!\r\n"); 
            }
            if(uip_timedout())
            {
                printf("tcp_client uip_timedout!\r\n");
            }
            if(uip_connected())                             //���ӳɹ�
            {
                printf("tcp_client uip_connected!\r\n");
                uip_send(" connect succeed! ", strlen(" connect succeed! "));//����TCP���ݰ�
            }
            if(uip_acked())                                 //��ʾ�ϴη��͵����ݳɹ��ʹ�
            {
                printf("tcp_client uip_acked!\r\n"); 
            }
            
            if(uip_poll())                                  //��ǰ���ӿ�����ѵ
            {
                printf("tcp_client uip_poll!\r\n"); 
            }
            if(uip_rexmit())                                //�ط�
            {
                printf("tcp_client uip_rexmit!\r\n"); 
            }
        }
    }
    PROCESS_END();
}