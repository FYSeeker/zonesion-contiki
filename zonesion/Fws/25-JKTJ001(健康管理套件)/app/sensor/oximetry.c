
#include "oximetry.h"

void uart3_putc(unsigned char x);
void oximetry_uart3_init(unsigned int bpr);
void uart3_set_input();

u16 blood0xygenValue=0;

u16 Oxygen_get()
{
    return blood0xygenValue;
}

static int _on_recv(char ch)
{
    static char rxbuf[16];
    static int offset = 0;
    static int n = 0;
    if(offset == 4)
    {
        rxbuf[n] = ch;
        n++;
        if(n >=3)
        {
            offset = 0;
            n = 0;
            blood0xygenValue = rxbuf[1];
        }
    }
    if (ch == 0xFF)
        offset = 0;
    else if (offset == 0 && ch == 0xC7)
    {
        offset = 1;
    }
    else if (offset == 1 && ch == 0x06)
    {
        offset = 2;
    }
    else if (offset == 2 && ch == 0xA6)
    {
        offset = 3;
    }
    else if (offset == 3 && ch == 0xA0)
    {
        offset = 4;
    }

    return 0;
}

/*********************************************************************************************
* 名称：oximetry_init()
* 功能：血氧传感器初始化
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void oximetry_init(void)
{
    oximetry_uart3_init(115200);
    uart3_set_input(_on_recv);
}

void oximetry_start(void)
{
    char start_cmd[]= {0xFF,0xC7,0x03,0xA3,0xA0};      //开始测量命令
    blood0xygenValue = 0;
    for (int i=0; i<5; i++)
    {
        uart3_putc(start_cmd[i]);
    }
}

void oximetry_stop(void)
{
    char stop_cmd[] = {0xFF,0xC7,0x03,0xA4,0xA1};

    for (int i=0; i<5; i++)
    {
        uart3_putc(stop_cmd[i]);
    }
    blood0xygenValue = 0;
}

void oximetry_uart3_init(unsigned int bpr)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA10复用为USART1

    /* Configure USARTx_Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度10MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = bpr;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);

    /* Enable the USARTz Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable USARTy Receive and Transmit interrupts */
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    /* Enable the USARTy */
    USART_Cmd(USART3, ENABLE);

}
