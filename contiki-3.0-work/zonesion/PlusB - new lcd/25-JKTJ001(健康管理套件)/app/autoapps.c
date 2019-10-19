#include <contiki.h>

PROCESS_NAME(helloworld); 
PROCESS_NAME(LedProcess);
PROCESS_NAME(KeyProcess);
PROCESS_NAME(onboard_sensors);
PROCESS_NAME(LcdProcess);
PROCESS_NAME(rfUartProcess);

struct process * const autostart_processes[] = {
    &LedProcess,
    &KeyProcess,
    &onboard_sensors,
    &LcdProcess,
    &rfUartProcess,
    NULL
};

void StartProcesslist(void)
{
    autostart_start(autostart_processes);
}
