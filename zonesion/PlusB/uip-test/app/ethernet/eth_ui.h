#ifndef _eth_ui_h_
#define _eth_ui_h_

#include <stdio.h>
#include <string.h>
#include <contiki.h>
#include "ili93xx.h"
#include "uip.h"
#include "uipopt.h"
#include "tcpip.h"
#include "dhcpc.h"
#include "key.h"
#include "udp_app.h"
#include "tcp_app.h"

#define ethMenuOpeionLen 18     //字节
#define ethMenuIndexMAX  8

typedef enum
{
    UNSELECT=0,
    SELECT=1,
}optionStatus_t;

typedef struct
{
    unsigned char index;                                    //菜单索引
    optionStatus_t optionState[ethMenuIndexMAX];               //选项状态
    char* optionName[ethMenuIndexMAX];                         //选项名称
    void (*optionHandle[ethMenuIndexMAX])(optionStatus_t);     //选项处理函数
}_Menu_t;

PROCESS_NAME(eth_ui);
extern uint8_t dhcp_flag;

char ethOptionCallFunc_set(unsigned char optionIndex,void (*func)(optionStatus_t));

#endif