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
            if (uip_newdata())					            //接收新的TCP数据包
            {
                printf("tcp_client uip_newdata!\r\n");      //打印信息
                uip_send(uip_appdata, uip_datalen());       //发送TCP数据包
            }
            if(uip_aborted())
            {
                printf("tcp_client uip_aborted!\r\n"); 
            }
            if(uip_closed())                                //连接关闭了,服务器关闭的时候，会两次进入这个函数
            {
                printf("tcp_client uip_closed!\r\n"); 
            }
            if(uip_timedout())
            {
                printf("tcp_client uip_timedout!\r\n");
            }
            if(uip_connected())                             //连接成功
            {
                printf("tcp_client uip_connected!\r\n");
                uip_send(" connect succeed! ", strlen(" connect succeed! "));//发送TCP数据包
            }
            if(uip_acked())                                 //表示上次发送的数据成功送达
            {
                printf("tcp_client uip_acked!\r\n"); 
            }
            
            if(uip_poll())                                  //当前连接空闲轮训
            {
                printf("tcp_client uip_poll!\r\n"); 
            }
            if(uip_rexmit())                                //重发
            {
                printf("tcp_client uip_rexmit!\r\n"); 
            }
        }
    }
    PROCESS_END();
}