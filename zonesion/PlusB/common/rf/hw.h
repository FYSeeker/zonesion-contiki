/*********************************************************************************************
* �ļ���hw.h
* ���ߣ�fuyou 
* ˵����plus�ڵ��Զ�ʶ��������������
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
#include "zigbee-net.h"
#include "ble-net.h"
#include "lora-net.h"
#include "wifi-net.h"
#include "nb-iot.h"


extern process_event_t RF_InitEvent;//���߳�ʼ���¼����㲥
PROCESS_NAME(getHwType_process);

char hwType_Get(void);
void hwType_Set(char type);

/*---------------------------------------------------------------------------*/
/*
 * Send broadcast events and dispatch events
 * author: fuyou
 * explain: add 2018/12/28
 */
/*---------------------------------------------------------------------------*/
void Process_BroadcastPostSync(process_event_t ev,process_data_t data);

#endif
