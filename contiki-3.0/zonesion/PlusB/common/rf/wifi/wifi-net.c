#include "wifi-net.h"
#include "rfUart.h"
#include "zxbee.h"

extern char lcdEvent_InitFlag;
extern process_event_t lcd_event;
PROCESS_NAME(sensor_process);
PROCESS_NAME(lcd);

static unsigned char wifi_link=0;
unsigned char wifi_keyType=-1;
char wifi_mac[18]={0};
char wifi_ssid[50]={0};
char wifi_key[50]={0};
char wifi_ip[20]={0};
char wifi_sip[20]={0};

unsigned char wifiGat_keyType()
{
    return wifi_keyType;
}

char* wifiGat_mac()
{
    return wifi_mac;
}

char* wifiGat_ssid()
{
    return wifi_ssid;
}

char* wifiGat_key()
{
    return wifi_key;
}

char* wifiGat_ip()
{
    return wifi_ip;
}

char* wifiGat_sip()
{
    return wifi_sip;
}

unsigned char wifiGat_link(void)
{
    static unsigned short tick=0;
    tick++;
    if(tick>9)
    {
        tick=0;
        rfUartSendString("AT+LINK?\r\n",strlen("AT+LINK?\r\n"));    //周期性问询
    }
    return wifi_link;
}


PROCESS(wifi_process, "wifi_process");

PROCESS_THREAD(wifi_process, ev, data)
{ 
    char len=0;
    char* temp=NULL;
    static struct etimer wifil_etimer;
    static unsigned char commandSelect=0,commandFlag=0;
    static const char command_list[][50] = {"AT+MAC?\r\n",
                                            "AT+SSID?\r\n",
                                            "AT+KEYTYPE?\r\n",
                                            "AT+KEY?\r\n",
                                            "AT+IP?\r\n"
                                            "AT+SIP?\r\n",
                                            "AT+LINK?\r\n"};

    PROCESS_BEGIN();
    
    commandSelect=0;
    commandFlag=0;
    etimer_set(&wifil_etimer,100);
    
    while (1)
    {
        PROCESS_YIELD();
        if (ev==PROCESS_EVENT_TIMER)
        {
            etimer_set(&wifil_etimer, 200);
            if(commandSelect < sizeof command_list /sizeof command_list[0])
            {
                rfUartSendString((char*)command_list[commandSelect], strlen(command_list[commandSelect]));
                commandFlag=1;
            }
            else
            {
                etimer_stop(&wifil_etimer);
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
            else if (memcmp(pdata, "+LINK:", 6) == 0)
            {
                wifi_link = atoi(&pdata[6]);
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
            else if (memcmp(pdata, "+MAC:", 5) == 0)
            {
                memcpy(wifi_mac,&pdata[5],17);
                wifi_mac[17]=0;
            }
            else if (memcmp(pdata, "+SSID:", 6) == 0)
            {
                len=0;
                temp=&pdata[7];
                while(*temp!='"')
                {
                    temp++;
                    len++;
                }
                memcpy(wifi_ssid,&pdata[6],len+2);
                wifi_ssid[len+3]=0;
            }
            else if (memcmp(pdata, "+KEYTYPE:", 9) == 0)
            {
                wifi_keyType=atoi(&pdata[9]);
            }
            else if (memcmp(pdata, "+KEY:", 5) == 0)
            {
                len=0;
                temp=&pdata[6];
                while(*temp!='"')
                {
                    temp++;
                    len++;
                }
                memcpy(wifi_key,&pdata[5],len+2);
                wifi_key[len+3]=0;
            }
            else if (memcmp(pdata, "+IP:", 4) == 0)
            {
                len=0;
                temp=&pdata[4];
                while(*temp!='\r')
                {
                    temp++;
                    len++;
                }
                memcpy(wifi_ip,&pdata[4],len);
                wifi_ip[len+1]=0;
            }
            else if (memcmp(pdata, "+SIP:", 5) == 0)
            {
                len=0;
                temp=&pdata[5];
                while(*temp!='\r')
                {
                    temp++;
                    len++;
                }
                memcpy(wifi_sip,&pdata[5],len);
                wifi_sip[len+1]=0;
            }
        }
    }
    PROCESS_END();
}