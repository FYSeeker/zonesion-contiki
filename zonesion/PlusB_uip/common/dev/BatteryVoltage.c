#include "BatteryVoltage.h"

float BatteryVoltage=0;

extern void AdcInit(void);
extern unsigned short AdcGet(unsigned char ch);

/*********************************************************************************************
* ����: BatteryVotage_Get
* ����: ��ȡ��ص�ѹ
* ����: ��
* ����: ��ص�ѹֵ����λ���أ�V��
* �޸�:
* ע��:
*********************************************************************************************/
float BatteryVotage_Get()
{
    return BatteryVoltage;
}

/*********************************************************************************************
����������
*********************************************************************************************/
PROCESS(BatteryVoltageUpdate, "BatteryVoltageUpdate");

/*********************************************************************************************
* ����: PROCESS_THREAD()
* ����: ���ش������������
* ����: onboard_sensors, ev, data
* ����: ��
* �޸�:
* ע��:
*********************************************************************************************/
PROCESS_THREAD(BatteryVoltageUpdate, ev, data)
{
    static unsigned short adcValue=0;
    static struct etimer VBATT_timer;
    
    PROCESS_BEGIN();
    
    AdcInit();
    
    while(1)
    {
        adcValue = AdcGet(1);
        BatteryVoltage = (float)adcValue*(3.3/4095.0)*13.6/3.6;
        etimer_set(&VBATT_timer,100);
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);  
    }
    
    PROCESS_END();
}