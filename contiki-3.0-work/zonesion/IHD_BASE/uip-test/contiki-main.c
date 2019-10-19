#include <stdio.h>
#include <contiki.h>
#include <contiki-net.h>
#include <dev/leds.h>
#include "usart.h"

unsigned int idle_count = 0;

void uart1_init(unsigned int bpr);
void debug_init(void (*f)(char));
int debug_input(char c);

void main(void)
{
    clock_init();
    uart1_init(38400);
    debug_init(uart1_putc);
    uart1_set_input(debug_input);

    printf("\r\nStarting ");
    printf(CONTIKI_VERSION_STRING);
    printf(" on STM32F40x\r\n");

    process_start(&etimer_process, NULL);
    ctimer_init();
    //serial_shell_init();

#if AUTOSTART_ENABLE
    autostart_start(autostart_processes);
#endif

    while(1)
    {
        do
        {
        }
        while(process_run() > 0);
        idle_count++;
        /* Idle! */
        /* Stop processor clock */
        /* asm("wfi"::); */
    }
}