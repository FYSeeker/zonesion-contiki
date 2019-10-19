#include "BatteryVoltage.h"

#define CPU_TEMPERATURE_EN 0

float BatteryVoltage=0;
double CpuTemperature=0;

/*********************************************************************************************
* 名称: BatteryVotage_Get
* 功能: 获取电池电压
* 参数: 无
* 返回: 电池电压值，单位伏特（V）
* 修改:
* 注释:
*********************************************************************************************/
float BatteryVotage_Get()
{
  return BatteryVoltage;
}

double CpuTemperature_Get()
{
  return CpuTemperature;
}

/*********************************************************************************************
申明进程名
*********************************************************************************************/
PROCESS(PowerProcess, "PowerProcess");

/*********************************************************************************************
* 名称: PROCESS_THREAD()
* 功能: 板载传感器处理进程
* 参数: onboard_sensors, ev, data
* 返回: 无
* 修改:
* 注释:
*********************************************************************************************/
PROCESS_THREAD(PowerProcess, ev, data)
{
  static struct etimer VBATT_timer;
  static char tick=0;
  static uint32_t AdcValue=0;
  
  PROCESS_BEGIN();
  
  AdcInit();
#if CPU_TEMPERATURE_EN
  CpuTemperature = Get_CpuTemprateX(10);
#endif
  AdcValue=AdcGetX(1,10);
  BatteryVoltage = (float)AdcValue*(3.3/4095.0)*13.6/3.6;
  AdcValue=0;
  
  while(1)
  {
#if CPU_TEMPERATURE_EN
    CpuTemperature = Get_CpuTemprateX(10);
#endif
    AdcValue+=AdcGetX(1,10);
    if(tick>9)
    {
      AdcValue=(uint16_t)(AdcValue/tick);
      BatteryVoltage = (float)AdcValue*(3.3/4095.0)*13.6/3.6;
      tick=0;
      AdcValue=0;
    }
    tick++;
    etimer_set(&VBATT_timer,50);
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);  
  }
  
  PROCESS_END();
}