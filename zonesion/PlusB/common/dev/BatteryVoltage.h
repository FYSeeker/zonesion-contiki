#ifndef _batteryVoltage_h_
#define _batteryVoltage_h_

#include <contiki.h>
#include "adc.h"

float BatteryVotage_Get(void);
double CpuTemperature_Get(void);
PROCESS_NAME(PowerProcess);

#endif