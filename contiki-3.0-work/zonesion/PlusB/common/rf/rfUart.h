/*********************************************************************************************
* 文件：rfUart.h
* 作者：fuyou 
* 说明：plus节点与无线的串口通信驱动
* 修改：
* 注释：
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
//串口接收BUF大小
#define RF_BUF_SIZE 100

//串口调试信息宏
#define RF_UART_DEBUG   0

//xlabTools调试时串口打印不可用
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

//向无线发送字符串
void rfUartSendString(char *buf, int len);
//向无线发送透传数据
uint8_t rfUartSendData(char *str);

#endif
