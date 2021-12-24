#ifndef _wifi_net_h_
#define _wifi_net_h_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"

unsigned char wifiGat_link(void);
unsigned char wifiGat_keyType(void);
char* wifiGat_mac(void);
char* wifiGat_ssid(void);
char* wifiGat_key(void);
char* wifiGat_ip(void);
char* wifiGat_sip(void);

#endif