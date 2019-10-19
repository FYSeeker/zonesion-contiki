#include "eth_dhcp.h"

uint8_t dhcp_flag=0;

void dhcpc_configured(const struct dhcpc_state *s)
{
    uip_sethostaddr(&s->ipaddr);
    uip_setnetmask(&s->netmask);
    uip_setdraddr(&s->default_router);
    uip_nameserver_update(&s->dnsaddr, UIP_NAMESERVER_INFINITE_LIFETIME);
    dhcp_flag |= (1<<7);
    process_post(&dhcp_process,dhcp_event,&dhcp_flag);
}

void dhcpc_unconfigured(const struct dhcpc_state *s)
{
}


process_event_t dhcp_event;
PROCESS(dhcp_process, "dhcp_process");

PROCESS_THREAD(dhcp_process, ev, data)
{
    PROCESS_BEGIN();
    
    dhcpc_init(uip_lladdr.addr, sizeof(uip_lladdr.addr));
    dhcpc_request();
    
    while(1)
    {
        PROCESS_WAIT_EVENT();

        if(ev == tcpip_event || ev == PROCESS_EVENT_TIMER)
        {
            dhcpc_appcall(ev, data);
        }
    }
    
    PROCESS_END();
}