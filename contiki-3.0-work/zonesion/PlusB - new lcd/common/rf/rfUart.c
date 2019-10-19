/*********************************************************************************************
* 文件：rfUart.c
* 作者：fuyou 
* 说明：plus节点与无线的串口通信驱动
* 修改：
* 注释：
*********************************************************************************************/
#include "rfUart.h"

PROCESS_NAME(getHwType_process);
PROCESS_NAME(zigbee_process);
PROCESS_NAME(ble_process);
PROCESS_NAME(wifi_process);
PROCESS_NAME(lora_process);

process_event_t uart_command_event;
uint8_t RF_SendDataACK=0;


void (*rfSendByte_call)(char)=NULL;

void rfSendByteSetCall(void (*func)(char))
{
    rfSendByte_call=func;
}

void rfUartSendByte(char ch)
{
    if (rfSendByte_call != NULL) 
    {
        rfSendByte_call(ch);
    }
}

//向无线发送字符串
void rfUartSendString(char *buf, int len)
{
    RF_PRINT("<-- %s\r\n",buf);
    for (int i=0; i<len; i++)
    {
        rfUartSendByte(*buf++);
    }
}

//向无线发送透传数据
uint8_t rfUartSendData(char *dat)
{
    uint8_t t=0, len=0;
    char pbuf[20]={0};
   
    RF_SendDataACK=0;
    len = sprintf(pbuf, "AT+SEND=%u\r\n", strlen(dat));
    rfUartSendString(pbuf, len);
    
    while(!RF_SendDataACK)            //等待收到'>'
    {
        delay_ms(1);
        t++;
        if(t>49) return 0;                 //超时
    }
    rfUartSendString(dat, strlen(dat));
    return 1;
}


static int rfUartRecvByte(char ch)
{
    static unsigned char data_len=0;
    static unsigned char buf_offset=0;
    static char RF_BUF_RECV[2][RF_BUF_SIZE]={0};
    static char* pbuf=RF_BUF_RECV[0];
    
#if XLABTOOLS_ON
    uart1_putc(ch);
#endif
    
    if(ch=='>')
    {
        RF_PRINT("--> >\r\n");
        RF_SendDataACK=1;
    }
    else
    {
        if(buf_offset<RF_BUF_SIZE)
        {
            if(data_len>0)
            {
                pbuf[buf_offset++]=ch;
                data_len--;
                if(data_len==0)
                {
                    pbuf[buf_offset]='\0';
                    process_post(&rfUartProcess, uart_command_event, pbuf);
                    if(pbuf==RF_BUF_RECV[0])
                        pbuf=RF_BUF_RECV[1];
                    else
                        pbuf=RF_BUF_RECV[0];
                    buf_offset = 0;
                }
            }
            else
            {
                pbuf[buf_offset++]=ch;
                if(memcmp(&pbuf[buf_offset-2],"\r\n",2)==0)
                {
                    if((buf_offset>6)&&(memcmp(pbuf,"+RECV:",6)==0))
                    {
                        data_len=atoi(&pbuf[6]);
                    }
                    else if(buf_offset>2)
                    {
                        pbuf[buf_offset]='\0';
                        char* ptr=pbuf;
                        if(pbuf==RF_BUF_RECV[0])
                            pbuf=RF_BUF_RECV[1];
                        else
                            pbuf=RF_BUF_RECV[0];
                        buf_offset = 0;
                        process_post_synch(&rfUartProcess, uart_command_event, ptr);
                    }
                    else
                    {
                        buf_offset = 0;
                    }
                }
            }
        }
        else
        {
            buf_offset = 0;
            return -1;
        }
    }
    return 0;
}

/*
//MCU串口接收处理
int mcuRecvByte(char ch)
{
    static char recvBuf[10]={0};
    
    rfUartSendByte(ch);     //转发给rf
    
    for(char i=0;i<9;i++)
    {
        recvBuf[i] = recvBuf[i+1];
    }
    recvBuf[9] = ch;
    
    if(memcmp(recvBuf,"AT+RESET\r\n",10)==0)
    {
       NVIC_SystemReset();
    }
    
    return 0;
}
*/
//MCU串口接收处理
int mcuRecvByte(char ch)
{
    static short offset=0;
    static char recvBuf[128]={0};
    static char buf[2]={0};
    
    if(offset<128)
    {
      recvBuf[offset++]=ch;
      buf[0] = buf[1];
      buf[1] = ch;
      if(memcmp(buf,"\r\n",2)==0)
      {
        rfUartSendString(recvBuf,offset);
        offset=0;
        if(memcmp(recvBuf,"AT+RESET\r\n",10)==0)
        {
           NVIC_SystemReset();
        }
      }
    }
    else
      offset=0;
    
    return 0;
}


PROCESS(rfUartProcess, "rfUartProcess");

PROCESS_THREAD(rfUartProcess, ev, data)
{
    PROCESS_BEGIN();
    
    uart2_init(38400);
    uart2_set_input(rfUartRecvByte);
    rfSendByteSetCall(uart2_putc);
    
#if XLABTOOLS_ON    
    uart1_set_input(mcuRecvByte);
#endif
    
    uart_command_event = process_alloc_event();
    process_start(&getHwType_process,NULL);
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev==uart_command_event)      //uart command handle
        {
            RF_PRINT("--> %s\r\n", (char*)data);
            if(hwType_Get()!=0)
            {
                switch(hwType_Get())
                {
                    case 1:
                        process_post_synch(&zigbee_process, uart_command_event, data);
                        break;
                    case 2:
                        process_post_synch(&ble_process, uart_command_event, data);
                        break;
                    case 3:
                        process_post_synch(&wifi_process, uart_command_event, data);
                        break;
                    case 4:
                        process_post_synch(&lora_process, uart_command_event, data);
                        break;
                    case 5:
                        process_post_synch(&NB_process, uart_command_event, data);
                        break;
                }
            }
            else
            {
                process_post(&getHwType_process, uart_command_event, data);
            }
        }
    }
    
    PROCESS_END();
}

