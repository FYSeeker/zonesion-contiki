#include "ble-net.h"
#include "rfUart.h"
#include "zxbee.h"

extern char lcdEvent_InitFlag;
extern process_event_t lcd_event;
PROCESS_NAME(sensor_process);
PROCESS_NAME(lcd);

char ble_mac[18]={0};
unsigned char ble_link=0;

char* bleGat_mac()
{
    return ble_mac;
}

char bleGat_link()
{
    static unsigned short tick=0;
    tick++;
    if(tick>9)
    {
        tick=0;
        rfUartSendString("AT+LINK?\r\n",strlen("AT+LINK?\r\n"));    //周期性问询
    }
    return ble_link;
}

PROCESS(ble_process, "ble_process");

PROCESS_THREAD(ble_process, ev, data)
{
    static struct etimer bel_etimer;
    static unsigned char commandSelect=0,commandFlag=0;
    static const char command_list[][50] = {"AT+MAC?\r\n","AT+LINK?\r\n"};

    PROCESS_BEGIN();
    
    commandSelect=0;
    commandFlag=0;
    etimer_set(&bel_etimer,100);
    
    while (1)
    {
        PROCESS_YIELD();
        if (ev==PROCESS_EVENT_TIMER)
        {
            etimer_set(&bel_etimer, 500);
            if(commandSelect < sizeof command_list /sizeof command_list[0])
            {
                rfUartSendString((char*)command_list[commandSelect], strlen(command_list[commandSelect]));
                commandFlag=1;
            }
            else
            {
                etimer_stop(&bel_etimer);
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
                memcpy(ble_mac, &pdata[5], 17);
                ble_mac[17] = 0;
            }
            else if (memcmp(pdata, "+LINK:", 6) == 0)
            {
                ble_link = atoi(&pdata[6]);
            }
        }
    }
    PROCESS_END();
}