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

/*********************************************************************************************
rfConfigMacro
*********************************************************************************************/
//���ڽ���BUF��С
#define RF_BUF_SIZE 100
//���ڵ�����Ϣ����
#define UartInfoPrint   1
//xlabTools���Կ���
#define xlabTools       0

//��ֹxlabTools����ʱʹ�ô��ڵ���
#if xlabTools
#define UartInfoPrint   0   
#endif

extern process_event_t uart_command_event;
PROCESS_NAME(rfUart_process);

//�����߷����ַ���
void rfUartSendString(char *buf, int len);
//�����߷���͸������
void rfUartSendData(char *str);

#endif
