/*********************************************************************************************
* 文件：hw.c
* 作者：fuyou 
* 说明：plus节点自动识别无线类型驱动
* 修改：
* 注释：
*********************************************************************************************/
#include "hw.h"
#include "rfUart.h"

extern void do_event();


process_event_t RF_InitEvent;//无线初始化事件，广播
//无线类型标识、名称
char hwType=0;
char hwName[10];


/*---------------------------------------------------------------------------*/
/*
 * Send broadcast events and dispatch events
 * author: fuyou
 * explain: add 2018/12/28
 */
/*---------------------------------------------------------------------------*/
void Process_BroadcastPostSync(process_event_t ev,process_data_t data)
{
    process_post(NULL,ev,data);
    do_event();
}

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
    else if(memcmp(hwName, "NB-IoT", strlen("NB-IoT")) == 0)
    {
        RF_PRINT("Wireless type: NB-IoT\r\n\r\n");
        hwType=5;
        process_start(&NB_process,NULL);
    }
    else
    {
        RF_PRINT("New wireless type\r\n\r\n");
        hwType=0xfe;
    }
    
    Process_BroadcastPostSync(RF_InitEvent,NULL);
}


uint8_t HW_SendReadCommand(uint8_t index)
{
    static const char command_list[][30] = {
        //"AT\r\n",
        "ATE0\r\n",
        "AT+HW?\r\n",
    };
    if(index<(sizeof(command_list)/sizeof(command_list[0])))
    {
        rfUartSendString((char*)command_list[index], strlen(command_list[index]));
        return 1;
    }
    return 0;
}


PROCESS(getHwType_process, "get name driver");

PROCESS_THREAD(getHwType_process, ev, data)
{
    static struct etimer get_name_timer;
    static uint8_t commandIndex=0;
    static char* pbuf;
    
    PROCESS_BEGIN();

    hwType = 0;
    commandIndex = 0;
    etimer_set(&get_name_timer, 100);//等待100ms再发
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev==PROCESS_EVENT_TIMER)
        {
            etimer_set(&get_name_timer, 500);
            
            HW_SendReadCommand(commandIndex);
        }
        if(ev == uart_command_event)
        {
            pbuf = (char*)data;
            if (memcmp(pbuf, "OK", strlen("OK")) == 0)
            {
                commandIndex++;
            }
            if (memcmp(pbuf, "+HW:", strlen("+HW:")) == 0)
            {
                if(commandIndex>0)
                {
                    etimer_stop(&get_name_timer);
                    memcpy(hwName, &pbuf[4], strlen(pbuf)-4);
                    selectType();
                    process_exit(&getHwType_process);
                }
            }
        }
    }
    PROCESS_END();
}
