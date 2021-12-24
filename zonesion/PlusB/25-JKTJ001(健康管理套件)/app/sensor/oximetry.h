#ifndef _oximetry_h_
#define _oximetry_h_

#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include "stm32f4xx.h"
#include <contiki.h>

void oximetry_init(void);
void oximetry_start(void);
void oximetry_stop(void);

u16 Oxygen_get(void);

#endif
