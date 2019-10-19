/*********************************************************************************************
* 文件：hw.c
* 作者：fuyou 
* 说明：plus节点自动识别无线类型驱动
* 修改：
* 注释：
*********************************************************************************************/
#include "hw.h"
#include "rfUart.h"

extern process_event_t uart_command_event;
PROCESS_NAME(zigbee_process);
PROCESS_NAME(ble_process);
PROCESS_NAME(wifi_process);
PROCESS_NAME(lora_process);
PROCESS_NAME(sensor_process);

//无线类型标识、名字
char hwType=0;
char hwName[8];

char hwType_Get(void)
{
    return hwType;
}

void hwType_Set(char type)
{
    hwType = type;
}

void selectType(void)
{
    if(memcmp(hwName, "CC2530", strlen("CC2530")) == 0)
    {
        RF_PRINT("Wireless type: CC2530\r\n\r\n");
        hwType=1;
        process_start(&zigbee_process,NULL);
    }
    else if(memcmp(hwName, "CC2540", strlen("CC2540")) == 0)
    {
        RF_PRINT("Wireless type: CC2540\r\n\r\n");
        hwType=2;
        process_start(&ble_process,NULL);
    }
    else if(memcmp(hwName, "CC3200", strlen("CC3200")) == 0)
    {
        RF_PRINT("Wireless type: CC3200\r\n\r\n");
        hwType=3;
        process_start(&wifi_process,NULL);
    }
    else if(memcmp(hwName, "LoRa", strlen("LoRa")) == 0)
    {
        RF_PRINT("Wireless type: LoRa\r\n\r\n");
        hwType=4;
        process_start(&lora_process,NULL);
    }
    else
    {
        RF_PRINT("New wireless type\r\n\r\n");
        hwType=0xfe;
    }
}

PROCESS(getHwType_process, "get name driver");

PROCESS_THREAD(getHwType_process, ev, data)
{
    static struct etimer get_name_timer;
    static uint8_t commandIndex=0;
    static char* pbuf;
    
    PROCESS_BEGIN();

    hwType=0;
    commandIndex=0;
    process_post(&getHwType_process,PROCESS_EVENT_TIMER,NULL);
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev == uart_command_event)
        {
            pbuf = (char*)data;
            if (memcmp(pbuf, "OK", strlen("OK")) == 0)
            {
                commandIndex=1;
            }
            if (memcmp(pbuf, "+HW:", strlen("+HW:")) == 0)
            {
                etimer_stop(&get_name_timer);
                memcpy(hwName, &pbuf[4], 6);
                selectType();
                process_exit(&getHwType_process);
            }
        }
        if(ev==PROCESS_EVENT_TIMER)
        {
            etimer_set(&get_name_timer, 500);
            
            if(commandIndex==1)
            {
                rfUartSendString("AT+HW?\r\n",strlen("AT+HW?\r\n"));
            }
            else
            {
                rfUartSendString("AT\r\n",strlen("AT\r\n"));
            }
        }
    }
    PROCESS_END();
}
