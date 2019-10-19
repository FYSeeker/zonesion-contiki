/*********************************************************************************************
* �ļ���sensor.h
* ���ߣ�
* ˵����ͨ�ô��������ƽӿڳ���ͷ�ļ�
* �޸ģ�
* ע�ͣ�
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

#define SENSOR_TYPE  "851"                                      // ����������

#define SteppingMotor_DIR           GPIO_Pin_8   		        //�����������(DIR)����
#define SteppingMotor_STEP          GPIO_Pin_9   		        //���������λ(CP)����
#define SteppingMotor_EN            GPIO_Pin_10   		        //�������ʹ��(EN)����

void sensor_init(void);
void sensor_poll(unsigned int t);
unsigned short sensor_check(void);

#endif