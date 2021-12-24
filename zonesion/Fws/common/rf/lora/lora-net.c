#include "lora-net.h"
#include "rfUart.h"
#include "zxbee.h"

extern char lcdEvent_InitFlag;
extern process_event_t lcd_event;
PROCESS_NAME(sensor_process);
PROCESS_NAME(lcd);

unsigned short lora_fp=0;    //��Ƶ
unsigned char lora_pv=0;     //���书��:1-20
unsigned char lora_sf=0;     //��Ƶ����:6-12
unsigned char lora_cr=0;     //������:1-4
unsigned char lora_ps=0;     //ǰ���볤��
unsigned char lora_bw=0;     //����:0-9
unsigned char lora_hop=0;    //��Ƶ����:0-1
unsigned char lora_hopTab[40]={0};//��Ƶ��
unsigned short lora_netID=0; //����ID
unsigned short lora_nodeID=0;//��ַ:0x01-0xFE

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
    static const char command_list[][50] = {"AT+FP?\r\n",       //��Ƶ
                                            "AT+PV?\r\n",       //���书��
                                            "AT+SF?\r\n",       //��Ƶ����
                                            "AT+CR?\r\n",       //������
                                            "AT+PS?\r\n",       //ǰ���볤��
                                            "AT+BW?\r\n",       //����
                                            "AT+HOP?\r\n",      //��Ƶ����
                                            "AT+HOPTAB?\r\n",   //��Ƶ��
                                            "AT+NETID?\r\n",    //����ID
                                            "AT+NODEID?\r\n"    //��ַ
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