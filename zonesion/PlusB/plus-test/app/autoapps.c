#include <contiki.h>

PROCESS_NAME(helloworld); 
PROCESS_NAME(led);
PROCESS_NAME(key);
PROCESS_NAME(onboard_sensors);
PROCESS_NAME(lcd);
PROCESS_NAME(rfUart_process);
PROCESS_NAME(getHwType_process);
PROCESS_NAME(test_plus);

struct process * const autostart_processes[] = {
    &led,
    &lcd,
    &onboard_sensors,
    &rfUart_process,
    &getHwType_process,
    NULL
};

void StartProcesslist(void)
{
    autostart_start(autostart_processes);
}
