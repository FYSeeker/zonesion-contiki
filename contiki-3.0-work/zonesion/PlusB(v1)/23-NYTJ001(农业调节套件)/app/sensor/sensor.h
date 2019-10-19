/*********************************************************************************************
* 文件：sensor.h
* 作者：
* 说明：通用传感器控制接口程序头文件
* 修改：
* 注释：
*********************************************************************************************/
#ifndef _sensor_h_
#define _sensor_h_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "zxbee.h"
#include "Relay.h"
#include "htu21d.h"
#include "BatteryVoltage.h"
#include "rfUart.h"

#define SENSOR_TYPE  "851"                                      // 传感器编码

#define SteppingMotor_DIR           GPIO_Pin_8   		        //步进电机方向(DIR)引脚
#define SteppingMotor_STEP          GPIO_Pin_9   		        //步进电机相位(CP)引脚
#define SteppingMotor_EN            GPIO_Pin_10   		        //步进电机使能(EN)引脚

void sensor_init(void);
void sensor_poll(unsigned int t);
unsigned short sensor_check(void);

#endif