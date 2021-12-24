/*********************************************************************************************
* �ļ���key.c
* ���ߣ�fuyou
* ˵����
* ���ܣ������������
* �޸ģ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "key.h"
#include "drive_key.h"

PROCESS_NAME(lcd);

process_event_t key_event;

PROCESS(key, "key");

/*********************************************************************************************
* ���ƣ�PROCESS_THREAD(key, ev, data)
* ���ܣ������������
* ������key, ev, data
* ���أ���
* �޸ģ���
*********************************************************************************************/
PROCESS_THREAD(key, ev, data)
{
    PROCESS_BEGIN();
    
    static struct etimer key_timer;
    static char keyStatus=0,keyFlag=1;
    
    key_init();
    key_event = process_alloc_event();
    
    while (1)
    {
        keyStatus = get_key_status();
        if(keyStatus==0)
        {
            keyFlag=1;
        }
        else if(keyFlag)
        {
            keyFlag=0;
            process_post(NULL, key_event, &keyStatus);
        }
        etimer_set(&key_timer,50);
        PROCESS_WAIT_EVENT_UNTIL(ev==PROCESS_EVENT_TIMER);  
    }
    
    PROCESS_END();
}