#ifndef _lora_net_h_
#define _lora_net_h_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"

unsigned short loraGet_fp(void);
unsigned char loraGet_pv(void);
unsigned char loraGet_sf(void);
unsigned char loraGet_cr(void);
unsigned char loraGet_ps(void);
unsigned char loraGet_bw(void);
unsigned char loraGet_hop(void);
unsigned char* loraGet_hopTab(void);
unsigned short loraGet_netID(void);
unsigned short loraGet_nodeID(void);

#endif