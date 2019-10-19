#ifndef __ZXBEE_H__
#define __ZXBEE_H__

#include "contiki.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stimer.h>

//PLUS_NODE Defind Device types definitions
#ifdef PLUS_ZIGBEE_NODE

#define ZG_DEVICETYPE_ROUTER        0x01                        
#define ZG_DEVICETYPE_ENDDEVICE     0x02    
//Device Types NODE_ROUTER
#define NODE_ROUTER                 ZG_DEVICETYPE_ROUTER
//Device Types NODE_ENDDEVICE
#define NODE_ENDDEVICE              ZG_DEVICETYPE_ENDDEVICE

#endif

void zxbeeBegin(void);
int zxbeeAdd(char* tag, char* val);
char* zxbeeEnd(void);
void _zxbee_onrecv_fun(char *pkg, int len);

#endif