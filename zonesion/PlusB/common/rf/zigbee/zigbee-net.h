#ifndef __ZIGBEE_NET_H__
#define __ZIGBEE_NET_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "rfUart.h"

#if 0
#define ZIGBEE_CONFIG_PANID     0x2100
#define ZIGBEE_CONFIG_CHANNEL   11
#endif


PROCESS_NAME(zigbee_process);


char* zigbeeGat_mac(void);
unsigned short zigbeeGat_panid(void);
unsigned short zigbeeGat_channel(void);
unsigned short zigbeeGat_type(void);
unsigned char zigbeeGat_link(void);

#endif