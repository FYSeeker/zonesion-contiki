#ifndef _heartrate_h_
#define _heartrate_h_

#include "stm32f4xx.h"
#include "tim.h"
#include "delay.h"
#include <contiki.h>

extern short HeartrateValue;

void  heartrate_io_Init(void);
static void LINE1_IRQ(void);
short heartrate_get(void);

#endif