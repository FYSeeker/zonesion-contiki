#include "sensor_process.h"

extern void sensor_init(void);
extern void sensor_poll(unsigned int t);

PROCESS(sensor_process, "sensor_process");

PROCESS_THREAD(sensor_process, ev, data)
{
    PROCESS_BEGIN();
    
    static struct etimer sensor_timer;
    static unsigned short tick = 0;
    
    sensor_init();
    
    while(1)
    {
        sensor_poll(++tick);
        if(tick>59999)
            tick=0;
        
        etimer_set(&sensor_timer,1000);
        PROCESS_WAIT_EVENT_UNTIL(ev==PROCESS_EVENT_TIMER);
    }
    
    PROCESS_END();
}