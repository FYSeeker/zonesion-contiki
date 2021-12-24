/*********************************************************************************************
* 文件：rfUart.c
* 作者：fuyou 
* 说明：plus节点与无线的串口通信驱动
* 修改：
* 注释：
*********************************************************************************************/
#include "rfUart.h"

extern char hwType_Get();

extern void uart2_putc(unsigned char x);
extern void uart2_init(unsigned int bpr);
extern void uart2_set_input(int(*fun)(unsigned char));
extern void uart1_set_input(int(*fun)(unsigned char));

PROCESS_NAME(getHwType_process);
PROCESS_NAME(zigbee_process);
PROCESS_NAME(ble_process);
PROCESS_NAME(wifi_process);
PROCESS_NAME(lora_process);

process_event_t uart_command_event;

PROCESS(rfUart_process, "rfUart_process");

void (*rfSendByte_call)(unsigned char)=NULL;
void rfSendByteSetCall(void (*func)(unsigned char))
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
#if UartInfoPrint
    printf("<-- %s\r\n",buf);
#endif
    for (int i=0; i<len; i++)
    {
        rfUartSendByte(*buf++);
    }
}

//向无线发送透传数据
void rfUartSendData(char *dat)
{
    unsigned short dataLen=0,len=0;
    char buf[32]={0};
    
    dataLen = strlen(dat);
    len = sprintf(buf, "AT+SEND=%u\r\n", dataLen);
    rfUartSendString(buf, len);
    delay_ms(50);
    rfUartSendString(dat, dataLen);
    delay_ms(50);
}


char RF_BUF_RECV[2][RF_BUF_SIZE]={0};
unsigned char buf_recv_len=0;

static int rfUartRecvByte(unsigned char ch)
{
    static unsigned char data_len=0;
    static char* pbuf=RF_BUF_RECV[0];
    
#if xlabTools
    printf("%c",ch);
#endif
    
    if(buf_recv_len<RF_BUF_SIZE)
    {
        if(data_len>0)
        {
            pbuf[buf_recv_len++]=ch;
            data_len--;
            if(data_len==0)
            {
                pbuf[buf_recv_len]='\0';
                process_post(&rfUart_process, uart_command_event, pbuf);
                if(pbuf==RF_BUF_RECV[0])
                    pbuf=RF_BUF_RECV[1];
                else
                    pbuf=RF_BUF_RECV[0];
                buf_recv_len = 0;
            }
        }
        else
        {
            pbuf[buf_recv_len++]=ch;
            if(memcmp(&pbuf[buf_recv_len-2],"\r\n",2)==0)
            {
                if((buf_recv_len>6)&&(memcmp(pbuf,"+RECV:",6)==0))
                {
                    data_len=atoi(&pbuf[6]);
                }
                else if(buf_recv_len>2)
                {
                    pbuf[buf_recv_len]='\0';
                    process_post(&rfUart_process, uart_command_event, pbuf);
                    if(pbuf==RF_BUF_RECV[0])
                        pbuf=RF_BUF_RECV[1];
                    else
                        pbuf=RF_BUF_RECV[0];
                    buf_recv_len = 0;
                }
                else
                {
                    buf_recv_len = 0;
                }
            }
        }
    }
    else
    {
        buf_recv_len = 0;
        return -1;
    }
    return 0;
}

//MCU串口接收处理
int mcuRecvByte(unsigned char ch)
{
    static char recvBuf[10]={0};
    
    rfUartSendByte(ch);     //转发给
    
    for(char i=0;i<7;i++)
    {
        recvBuf[i] = recvBuf[i+1];
    }
    recvBuf[7] = ch;
    
    if(memcmp(recvBuf,"AT+RESET",8)==0)
    {
       NVIC_SystemReset();
    }
    
    
    return 0;
}

PROCESS_THREAD(rfUart_process, ev, data)
{
    PROCESS_BEGIN();
    
    uart2_init(38400);
    uart2_set_input(rfUartRecvByte);
    rfSendByteSetCall(uart2_putc);
    
#if xlabTools    
    uart1_set_input(mcuRecvByte);
#endif
    
    uart_command_event = process_alloc_event();
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(ev==uart_command_event)
        {
#if UartInfoPrint
            printf("--> %s\r\n", (char*)data);
#endif
            if(hwType_Get()!=0)
            {
                switch(hwType_Get())
                {
                    case 1:
                        process_post(&zigbee_process, uart_command_event, data);
                        break;
                    case 2:
                        process_post(&ble_process, uart_command_event, data);
                        break;
                    case 3:
                        process_post(&wifi_process, uart_command_event, data);
                        break;
                    case 4:
                        process_post(&lora_process, uart_command_event, data);
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

