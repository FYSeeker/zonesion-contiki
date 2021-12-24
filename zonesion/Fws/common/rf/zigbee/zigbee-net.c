#include "zigbee-net.h"
#include "zxbee.h"
#include "rfUart.h"

extern char lcdEvent_InitFlag;
extern process_event_t lcd_event;
PROCESS_NAME(sensor_process);
PROCESS_NAME(lcd);

static char zigbee_mac[24];
static char zigbee_type = -1;
static unsigned short zigbee_panid;
static unsigned short zigbee_channel;
static unsigned char zigbee_link=0;

PROCESS(zigbee_process, "zigbee_process");

char* zigbeeGat_mac()
{
    return zigbee_mac;
}
unsigned short zigbeeGat_panid()
{
    return zigbee_panid;
}
unsigned short zigbeeGat_channel()
{
    return zigbee_channel;
}
unsigned short zigbeeGat_type()
{
    return zigbee_type;
}

unsigned char zigbeeGat_link()
{
    static unsigned short tick=0;
    tick++;
    if(tick>9)
    {
        tick=0;
        rfUartSendString("AT+LINK?\r\n",strlen("AT+LINK?\r\n"));    //周期性问询
    }
    return zigbee_link;
}

PROCESS_THREAD(zigbee_process, ev, data)
{
    static struct etimer zigbeeUart_etimer;
    static unsigned char commandSelect=0,commandFlag=0;
    static const char command_list[][50] = {"AT+LOGICALTYPE?\r\n","AT+PANID?\r\n","AT+CHANNEL?\r\n","AT+MAC?\r\n","AT+LINK?\r\n"};

    PROCESS_BEGIN();
    
    commandSelect=0;
    commandFlag=0;
    etimer_set(&zigbeeUart_etimer,100);

    while (1)
    {
        PROCESS_YIELD();
        if (ev==PROCESS_EVENT_TIMER)
        {
            etimer_set(&zigbeeUart_etimer, 200);
            if(commandSelect < sizeof command_list /sizeof command_list[0])
            {
                rfUartSendString((char*)command_list[commandSelect], strlen(command_list[commandSelect]));
                commandFlag=1;
            }
            else
            {
                etimer_stop(&zigbeeUart_etimer);
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
            else if (memcmp(pdata, "+MAC:", 5) == 0)
            {
                memcpy(zigbee_mac, &pdata[5], 23);
                zigbee_mac[23] = 0;
            }
            else if (memcmp(pdata, "+LOGICALTYPE:", 13) == 0)
            {
                zigbee_type = pdata[13] - '0';
            }
            else if (memcmp(pdata, "+PANID:", 7) == 0)
            {
                zigbee_panid = atoi(&pdata[7]);
            }
            else if (memcmp(pdata, "+CHANNEL:", 9) == 0)
            {
                zigbee_channel = atoi(&pdata[9]);
            }
            else if (memcmp(pdata, "+LINK:", 6) == 0)
            {
                zigbee_link = atoi(&pdata[6]);
            }
        }
    }
    PROCESS_END();
}


