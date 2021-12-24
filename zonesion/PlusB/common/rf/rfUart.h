/*********************************************************************************************
* �ļ���rfUart.h
* ���ߣ�fuyou 
* ˵����plus�ڵ������ߵĴ���ͨ������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#ifndef __rfUart_h_
#define __rfUart_h_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include "contiki.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
#include "hw.h"
#include "rf_config.h"

/*********************************************************************************************
rfConfigMacro
*********************************************************************************************/
//���ڽ���BUF��С
#define RF_BUF_SIZE 100

//���ڵ�����Ϣ��
#define RF_UART_DEBUG   0

//xlabTools����ʱ���ڴ�ӡ������
#if XLABTOOLS_ON
#define UartInfoDebug   0
#else
#define UartInfoDebug   RF_UART_DEBUG
#endif

#if UartInfoDebug
#define RF_PRINT(...)    printf(__VA_ARGS__)
#else
#define RF_PRINT(...)
#endif

extern process_event_t uart_command_event;
PROCESS_NAME(rfUartProcess);
extern char RF_BUF_RECV[2][RF_BUF_SIZE];

//�����߷����ַ���
void rfUartSendString(char *buf, int len);
//�����߷���͸������
uint8_t rfUartSendData(char *str);

#endif
