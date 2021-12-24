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

/*********************************************************************************************
rfConfigMacro
*********************************************************************************************/
//串口接收BUF大小
#define RF_BUF_SIZE 100
//串口调试信息开关
#define UartInfoPrint   1
//xlabTools调试开关
#define xlabTools       0

//防止xlabTools调试时使用串口调试
#if xlabTools
#define UartInfoPrint   0   
#endif

extern process_event_t uart_command_event;
PROCESS_NAME(rfUart_process);

//向无线发送字符串
void rfUartSendString(char *buf, int len);
//向无线发送透传数据
void rfUartSendData(char *str);

#endif
