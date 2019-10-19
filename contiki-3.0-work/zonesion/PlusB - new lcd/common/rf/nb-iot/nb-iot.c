#include "nb-iot.h"

PROCESS_NAME(sensor_process);

#if 1
#define NB_DEBUG(...)    printf(__VA_ARGS__)
#else
#define NB_DEBUG(...)
#endif

NB_Info_t* NB_Info = NULL;

uint8_t NB_GetMode()
{
    if(NB_Info == NULL)
        return 0;
    if(memcmp(NB_Info->mode,"COAP",4)==0)
        return 1;
    else if(memcmp(NB_Info->mode,"UDP",3)==0)
        return 2;
    return 0;
}

char* NB_GetID()
{
    if(NB_Info == NULL)
        return NULL;
    return NB_Info->id;
}

char* NB_GetKEY()
{
    if(NB_Info == NULL)
        return NULL;
    return NB_Info->key;
}

char* NB_GetSIP()
{
    if(NB_Info == NULL)
        return NULL;
    return NB_Info->sip;
}

char* NB_GetNodeID()
{
    if(NB_Info == NULL)
        return NULL;
    return NB_Info->nodeID;
}

void NB_ReadConfigHandle(char* pdata)
{
    if (memcmp(pdata, "+MODE:", 6) == 0)
    {
        memcpy(NB_Info->mode, &pdata[6], strlen(pdata)-6);
        NB_Info->mode[strlen(pdata)-8] = 0;
    }
    else if (memcmp(pdata, "+AID:", 5) == 0)
    {
        memcpy(NB_Info->id, &pdata[5], strlen(pdata)-5);
        NB_Info->id[strlen(pdata)-7] = 0;
    }
    else if (memcmp(pdata, "+AKEY:", 6) == 0)
    {
        uint8_t len=0;
        char* p_temp=&pdata[7];
        while(*p_temp != '"')
        {
            p_temp++;
            len++;
        }
        memcpy(NB_Info->key, &pdata[7], len);
        NB_Info->key[len] = 0;
    }
    else if (memcmp(pdata, "+SIP:", 5) == 0)
    {
        memcpy(NB_Info->sip, &pdata[5], strlen(pdata)-5);
        NB_Info->sip[strlen(pdata)-7] = 0;
    }
    else if (memcmp(pdata, "+NODEID:", 8) == 0)
    {
        memcpy(NB_Info->nodeID, &pdata[8], strlen(pdata)-8);
        NB_Info->nodeID[strlen(pdata)-10] = 0;
    }
}

uint8_t NB_SendReadCommand(uint8_t index)
{
    static const char command_list[][30] = {
        "AT+MODE?\r\n",
        "AT+AID?\r\n",
        "AT+AKEY?\r\n",
        "AT+SIP?\r\n",
        "AT+NODEID?\r\n"
    };
    if(index<(sizeof(command_list)/sizeof(command_list[0])))
    {
        rfUartSendString((char*)command_list[index], strlen(command_list[index]));
        return 1;
    }
    return 0;
}

uint8_t NB_WriteConfig()
{
/*
#if !XLABTOOLS_ON    
#endif
*/
    return 0;
}

void NB_UserProgram()
{
    Process_BroadcastPostSync(RF_InitEvent,NULL);
    process_start(&sensor_process,NULL);
}

PROCESS(NB_process, "NB_process");

PROCESS_THREAD(NB_process, ev, data)
{
    PROCESS_BEGIN();
    
    NB_Info = (NB_Info_t*)malloc(sizeof(NB_Info_t*));
    if(NB_Info == NULL)
    {
        NB_DEBUG("\"NB_Info\" Memory allocation failed!");
        PROCESS_EXIT();
    }
    
    static process_event_t NBConfig_event;
    static unsigned char readIndex=0,writeIndex=0;
    static uint8_t configFlag=0;
    
    readIndex=0;
    configFlag=0x01;
    NBConfig_event=process_alloc_event();
    process_post(&NB_process,NBConfig_event,NULL);

    while (1)
    {
        PROCESS_YIELD();
        if (ev==NBConfig_event)
        {
            if((configFlag&0x80))
            {  
                configFlag=0;
                NB_UserProgram();
            }
            else if(configFlag&0x01)//read
            {
                if(NB_SendReadCommand(readIndex)==0)
                {
                    configFlag=0x02; 
                    writeIndex=0;
                    process_post(&NB_process,NBConfig_event,NULL);
                }
            }
            else if(configFlag&0x02)//write
            {
                if(NB_WriteConfig()==0)
                {
                    if(writeIndex>0)
                    {
                        process_start(&getHwType_process,NULL);
                        PROCESS_EXIT();
                    }
                    else
                    {
                        configFlag=(0x80);
                        readIndex=0;
                        process_post(&NB_process,NBConfig_event,NULL);
                    }
                }
                else
                    writeIndex++;
            }
        }
        if (ev == uart_command_event)
        {
            char* pdata = (char *)data;
            if (memcmp(pdata, "+RECV:", 6) == 0)
            {
                short dataLen = atoi(&pdata[6]);
                while((*(pdata++))!='\n');
                if (dataLen > 0)
                {
                    _zxbee_onrecv_fun(pdata, dataLen);
                }
            }
            else if(memcmp(pdata, "OK", 2) == 0)
            {
                if(configFlag&0x01)
                {
                    readIndex++;
                    process_post(&NB_process,NBConfig_event,NULL);
                }
                else if(configFlag&0x02)
                {
                    process_post(&NB_process,NBConfig_event,NULL);
                }
            }
            else if(memcmp(pdata, "ERR:", 4) == 0)
            {
                process_post(&NB_process,NBConfig_event,NULL);
            }
            else
            {
                NB_ReadConfigHandle(pdata);
            }
        }
    }
    PROCESS_END();
}


