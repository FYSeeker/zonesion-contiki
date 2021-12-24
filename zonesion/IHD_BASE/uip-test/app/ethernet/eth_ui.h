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

#define ethMenuOpeionLen 18     //�ֽ�
#define ethMenuIndexMAX  8

typedef enum
{
    UNSELECT=0,
    SELECT=1,
}optionStatus_t;

typedef struct
{
    unsigned char index;                                    //�˵�����
    optionStatus_t optionState[ethMenuIndexMAX];               //ѡ��״̬
    char* optionName[ethMenuIndexMAX];                         //ѡ������
    void (*optionHandle[ethMenuIndexMAX])(optionStatus_t);     //ѡ�����
}_Menu_t;

PROCESS_NAME(eth_ui);
extern uint8_t dhcp_flag;

char ethOptionCallFunc_set(unsigned char optionIndex,void (*func)(optionStatus_t));

#endif