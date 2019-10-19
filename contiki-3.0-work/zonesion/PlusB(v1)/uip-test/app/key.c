/*********************************************************************************************
* 文件：key.c
* 作者：fuyou
* 说明：
* 功能：按键处理进程
* 修改：
*********************************************************************************************/
/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "key.h"
#include "drive_key.h"

PROCESS_NAME(eth_ui);

uint8_t keyStatus=0;
process_event_t key_event;
PROCESS(key, "key");

/*********************************************************************************************
* 名称：PROCESS_THREAD(key, ev, data)
* 功能：按键处理进程
* 参数：key, ev, data
* 返回：无
* 修改：无
*********************************************************************************************/
PROCESS_THREAD(key, ev, data)
{
    
    PROCESS_BEGIN();
    
    static struct etimer key_etimer;
    static uint8_t keyCount=0,keyFlag=0;
    
    key_init();
    key_event = process_alloc_event();
    
    while (1)
    {
        keyStatus = get_key_status();
        if(keyStatus!=0)
        {
            etimer_set(&key_etimer,10);
            PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
            if(keyStatus!=0)
            {
                if(!(keyFlag&0x01) || (keyFlag&0x02))
                {
                    keyFlag|=0x01;
                    process_post(&eth_ui, key_event, &keyStatus);
                }
                else
                {
                    keyCount++;
                    if(keyCount>19)
                    {
                        keyCount=0;
                        keyFlag|=0x02;
                    }
                }
            }
        }
        else
        {
            etimer_set(&key_etimer,10);
            PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
            if(keyStatus==0)
            {
                keyFlag=0;
            }
        }
        
        etimer_set(&key_etimer,40);
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
    }
    
    PROCESS_END();
}