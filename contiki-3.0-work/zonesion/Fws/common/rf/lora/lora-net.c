#include "lora-net.h"
#include "rfUart.h"
#include "zxbee.h"

extern char lcdEvent_InitFlag;
extern process_event_t lcd_event;
PROCESS_NAME(sensor_process);
PROCESS_NAME(lcd);

unsigned short lora_fp=0;    //基频
unsigned char lora_pv=0;     //发射功率:1-20
unsigned char lora_sf=0;     //扩频因子:6-12
unsigned char lora_cr=0;     //编码率:1-4
unsigned char lora_ps=0;     //前导码长度
unsigned char lora_bw=0;     //带宽:0-9
unsigned char lora_hop=0;    //跳频开关:0-1
unsigned char lora_hopTab[40]={0};//跳频表
unsigned short lora_netID=0; //网络ID
unsigned short lora_nodeID=0;//地址:0x01-0xFE

unsigned short loraGet_fp()
{
    return lora_fp;
}

unsigned char loraGet_pv()
{
    return lora_pv;
}

unsigned char loraGet_sf()
{
    return lora_sf;
}

unsigned char loraGet_cr()
{
    return lora_cr;
}

unsigned char loraGet_ps()
{
    return lora_ps;
}

unsigned char loraGet_bw()
{
    return lora_bw;
}

unsigned char loraGet_hop()
{
    return lora_hop;
}

unsigned short loraGet_netID()
{
    return lora_netID;
}

unsigned short loraGet_nodeID()
{
    return lora_nodeID;
}

unsigned char* loraGet_hopTab()
{
    return lora_hopTab;
}

PROCESS(lora_process, "lora_process");

PROCESS_THREAD(lora_process, ev, data)
{
    static struct etimer lora_etimer;
    static unsigned char commandSelect=0,commandFlag=0;
    static const char command_list[][50] = {"AT+FP?\r\n",       //基频
                                            "AT+PV?\r\n",       //发射功率
                                            "AT+SF?\r\n",       //扩频因子
                                            "AT+CR?\r\n",       //编码率
                                            "AT+PS?\r\n",       //前导码长度
                                            "AT+BW?\r\n",       //带宽
                                            "AT+HOP?\r\n",      //跳频开关
                                            "AT+HOPTAB?\r\n",   //调频表
                                            "AT+NETID?\r\n",    //网络ID
                                            "AT+NODEID?\r\n"    //地址
                                            };

    PROCESS_BEGIN();
    
    commandSelect=0;
    commandFlag=0;
    etimer_set(&lora_etimer,100);
    
    while (1)
    {
        PROCESS_YIELD();
        if (ev==PROCESS_EVENT_TIMER)
        {
            etimer_set(&lora_etimer, 200);
            if(commandSelect < sizeof command_list /sizeof command_list[0])
            {
                rfUartSendString((char*)command_list[commandSelect], strlen(command_list[commandSelect]));
                commandFlag=1;
            }
            else
            {
                etimer_stop(&lora_etimer);
                lcdEvent_InitFlag=3;
                process_post_synch(&lcd, lcd_event, &lcdEvent_InitFlag);
                process_start(&sensor_process,NULL);
            }
        }
        if (ev == uart_command_event)
        {
            char* pdata = (char *)data;
            if (memcmp(pdata, "OK", 2) == 0)
            {
                if(commandFlag>0)
                    commandSelect++;
            }
            else if (memcmp(pdata, "+RECV:", 6) == 0)
            {
                short dataLen = atoi(&pdata[6]);
                while((*(pdata++))!='\n');
                if (dataLen > 0)
                {
                    _zxbee_onrecv_fun(pdata, dataLen);
                }
            }
            else if (memcmp(pdata, "+FP:", 4) == 0)
            {
                lora_fp=atoi(&pdata[4]);
            }
            else if (memcmp(pdata, "+PV:", 4) == 0)
            {
                lora_pv=atoi(&pdata[4]);
            }
            else if (memcmp(pdata, "+SF:", 4) == 0)
            {
                lora_sf=atoi(&pdata[4]);
            }
            else if (memcmp(pdata, "+CR:", 4) == 0)
            {
                lora_cr=atoi(&pdata[4]);
            }
            else if (memcmp(pdata, "+PS:", 4) == 0)
            {
                lora_ps=atoi(&pdata[4]);
            }
            else if (memcmp(pdata, "+BW:", 4) == 0)
            {
                lora_bw=atoi(&pdata[4]);
            }
            else if (memcmp(pdata, "+HOP:", 5) == 0)
            {
                lora_hop=atoi(&pdata[5]);
            }
            else if (memcmp(pdata, "+HOPTAB:", 8) == 0)
            {
                memcpy(lora_hopTab,&pdata[8],39);
                lora_hopTab[39]=0;
            }
            else if (memcmp(pdata, "+NETID:", 7) == 0)
            {
                lora_netID=atoi(&pdata[7]);
            }
            else if (memcmp(pdata, "+NODEID:", 8) == 0)
            {
                lora_nodeID=atoi(&pdata[8]);
            }
        }
    }
    PROCESS_END();
}