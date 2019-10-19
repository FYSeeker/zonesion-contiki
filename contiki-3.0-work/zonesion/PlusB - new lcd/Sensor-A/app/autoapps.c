#include <contiki.h>

PROCESS_NAME(helloworld); 
PROCESS_NAME(led);
PROCESS_NAME(KeyProcess);
PROCESS_NAME(onboard_sensors);
PROCESS_NAME(lcd);
PROCESS_NAME(rfUartProcess);

struct process * const autostart_processes[] = {
    &led,
    &KeyProcess,
    &lcd,
    &onboard_sensors,
    &rfUartProcess,
    NULL
};

void StartProcesslist(void)
{
    autostart_start(autostart_processes);
}
