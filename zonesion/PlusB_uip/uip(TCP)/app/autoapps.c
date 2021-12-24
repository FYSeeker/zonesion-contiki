#include <contiki.h>

PROCESS_NAME(helloworld); 
PROCESS_NAME(led);
PROCESS_NAME(key);
PROCESS_NAME(lcd);
PROCESS_NAME(ethernet_process);
PROCESS_NAME(eth_tcp);

struct process * const autostart_processes[] = {
    &led,
    &key,
    &lcd,
    &ethernet_process,
    &eth_tcp,
    NULL
};
