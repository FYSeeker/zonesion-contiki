/*********************************************************************************************
* �ļ���hw.h
* ���ߣ�fuyou 2018.12.20
* ˵����ԭ����Զ�ʶ��������������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#ifndef __HW_H
#define __HW_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "rfUart.h"
#include "zigbee-net.h"
#include "ble-net.h"
#include "wifi-net.h"
#include "lora-net.h"
#include "lte_config.h"
#include "lte-ec20.h"


PROCESS_NAME(RF1_GetHwTypeProcess);
PROCESS_NAME(RF2_GetHwTypeProcess);
PROCESS_NAME(RF3_GetHwTypeProcess);
PROCESS_NAME(RF4_GetHwTypeProcess);
PROCESS_NAME(getHwType_process);

uint8_t RF1_hwTypeGet(void);
uint8_t RF2_hwTypeGet(void);
uint8_t RF3_hwTypeGet(void);
uint8_t RF4_hwTypeGet(void);


#endif
