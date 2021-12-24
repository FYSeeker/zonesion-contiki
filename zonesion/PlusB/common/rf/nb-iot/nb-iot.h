#ifndef _nb_iot_h_
#define _nb_iot_h_


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "zxbee.h"
#include "rfUart.h"

#define NB_MODE_SIZE 10
#define NB_ID_KEY_SIZE 100
#define NB_SIP_SIZE 20
#define NB_NODEID_SIZE 20

typedef struct
{
    char mode[NB_MODE_SIZE];
    char id[NB_ID_KEY_SIZE];
    char key[NB_ID_KEY_SIZE];
    char sip[NB_SIP_SIZE];
    char nodeID[NB_NODEID_SIZE];
}NB_Info_t;

PROCESS_NAME(NB_process);


uint8_t NB_GetMode(void);
char* NB_GetID(void);
char* NB_GetKEY(void);
char* NB_GetSIP(void);
char* NB_GetNodeID(void);

#endif