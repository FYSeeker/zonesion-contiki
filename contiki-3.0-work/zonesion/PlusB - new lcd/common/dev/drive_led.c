#include "drive_led.h"

void led_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;                        //����һ��GPIO_InitTypeDef���͵Ľṹ��


    RCC_AHB1PeriphClockCmd(LED1_RCC, ENABLE);                   //ʹ��ʱ��
    GPIO_InitStructure.GPIO_Pin = LED1_PIN;                     //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;               //���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //�������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;            //��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //������������Ϊ2MHz
    GPIO_Init(LED1_GPIO, &GPIO_InitStructure);                  //��ʼ��GPIO����

    RCC_AHB1PeriphClockCmd(LED2_RCC, ENABLE);                   //ʹ��ʱ��
    GPIO_InitStructure.GPIO_Pin = LED2_PIN;                     //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_Init(LED2_GPIO, &GPIO_InitStructure);                  //��ʼ��GPIO����

    RCC_AHB1PeriphClockCmd(LED3_RCC, ENABLE);                   //ʹ��ʱ��
    GPIO_InitStructure.GPIO_Pin = LED3_PIN;                     //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_Init(LED3_GPIO, &GPIO_InitStructure);                  //��ʼ��GPIO����

    RCC_AHB1PeriphClockCmd(LED4_RCC, ENABLE);                   //ʹ��ʱ��
    GPIO_InitStructure.GPIO_Pin = LED4_PIN;                     //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_Init(LED4_GPIO, &GPIO_InitStructure);                  //��ʼ��GPIO����

    RCC_AHB1PeriphClockCmd(LEDR_RCC, ENABLE);                   //ʹ��ʱ��
    GPIO_InitStructure.GPIO_Pin = LEDR_PIN;                     //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_Init(LEDR_GPIO, &GPIO_InitStructure);                  //��ʼ��GPIO����

    RCC_AHB1PeriphClockCmd(LEDG_RCC, ENABLE);                   //ʹ��ʱ��
    GPIO_InitStructure.GPIO_Pin = LEDG_PIN;                     //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_Init(LEDG_GPIO, &GPIO_InitStructure);                  //��ʼ��GPIO����

    RCC_AHB1PeriphClockCmd(LEDB_RCC, ENABLE);                   //ʹ��ʱ��
    GPIO_InitStructure.GPIO_Pin = LEDB_PIN;                     //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_Init(LEDB_GPIO, &GPIO_InitStructure);                  //��ʼ��GPIO����

    turn_off(0xFF);
}

/*********************************************************************************************
* ���ƣ�Turn_off
* ���ܣ������Ÿߵ�ƽ���ر�LED
* ������led
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void turn_off(unsigned char led)
{
    if(led & LED1_NUM)                                          //�ж�LEDѡ��
        GPIO_SetBits(LED1_GPIO, LED1_PIN);                      //PE0�����Ÿߵ�ƽ���ر�D1
    if(led & LED2_NUM)
        GPIO_SetBits(LED2_GPIO, LED2_PIN);                      //PE1�����Ÿߵ�ƽ���ر�D2
    if(led & LED3_NUM)
        GPIO_SetBits(LED3_GPIO, LED3_PIN);                      //PE2�����Ÿߵ�ƽ���ر�D3
    if(led & LED4_NUM)
        GPIO_SetBits(LED4_GPIO, LED4_PIN);                      //PE3�����Ÿߵ�ƽ���ر�D4
    if(led & LEDR_NUM)
        GPIO_SetBits(LEDR_GPIO, LEDR_PIN);                      //PB0�����Ÿߵ�ƽ���ر�RGB�Ƶĺ��
    if(led & LEDG_NUM)
        GPIO_SetBits(LEDG_GPIO, LEDG_PIN);                      //PB1�����Ÿߵ�ƽ���ر�RGB�Ƶ��̵�
    if(led & LEDB_NUM)
        GPIO_SetBits(LEDB_GPIO, LEDB_PIN);                      //PB2�����Ÿߵ�ƽ���ر�RGB�Ƶ�����
}

/*********************************************************************************************
* ���ƣ�turn_on
* ���ܣ������ŵ͵�ƽ����LED
* ������led
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void turn_on(unsigned char led)
{
    if(led & LED1_NUM)                                                  //�ж�LEDѡ��
        GPIO_ResetBits(LED1_GPIO, LED1_PIN);                          //PE0�����ŵ͵�ƽ����D1
    if(led & LED2_NUM)
        GPIO_ResetBits(LED2_GPIO, LED2_PIN);                          //PE1�����ŵ͵�ƽ����D2
    if(led & LED3_NUM)
        GPIO_ResetBits(LED3_GPIO, LED3_PIN);                          //PE2�����ŵ͵�ƽ����D3
    if(led & LED4_NUM)
        GPIO_ResetBits(LED4_GPIO, LED4_PIN);                          //PE3�����ŵ͵�ƽ����D4
    if(led & LEDR_NUM)
        GPIO_ResetBits(LEDR_GPIO, LEDR_PIN);                          //PB0�����ŵ͵�ƽ����RGB�Ƶĺ��
    if(led & LEDG_NUM)
        GPIO_ResetBits(LEDG_GPIO, LEDG_PIN);                          //PB1�����ŵ͵�ƽ����RGB�Ƶ��̵�
    if(led & LEDB_NUM)
        GPIO_ResetBits(LEDB_GPIO, LEDB_PIN);                          //PB2�����ŵ͵�ƽ����RGB�Ƶ�����
}
/*********************************************************************************************
* ���ƣ�get_led_status
* ���ܣ���ȡLED״̬
* ������
* ���أ�led_status--bit0-bit3�ֱ��ʾ4·LED�Ƶ�״̬��bit4-bit6�ֱ��ʾRGB�Ƶ�״̬
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
unsigned char get_led_status(void)
{
    unsigned char led_status = 0;
    if(GPIO_ReadOutputDataBit(LED1_GPIO, LED1_PIN))                 //�ж�PE0���ŵ�ƽ
        led_status |= LED1_NUM;                                           //�ߵ�ƽ��led_status bit0��1
    else
        led_status &= ~LED1_NUM;                                          //�͵�ƽ��led_status bit0��0

    if(GPIO_ReadOutputDataBit(LED2_GPIO, LED2_PIN))                 //�ж�PE1���ŵ�ƽ
        led_status |= LED2_NUM;                                           //�ߵ�ƽ��led_status bit1��1
    else
        led_status &= ~LED2_NUM;                                          //�͵�ƽ��led_status bit1��0

    if(GPIO_ReadOutputDataBit(LED3_GPIO, LED3_PIN))                 //�ж�PE2���ŵ�ƽ
        led_status |= LED3_NUM;                                           //�ߵ�ƽ��led_status bit2��1
    else
        led_status &= ~LED3_NUM;                                          //�͵�ƽ��led_status bit2��0

    if(GPIO_ReadOutputDataBit(LED4_GPIO, LED4_PIN))                 //�ж�PE3���ŵ�ƽ
        led_status |= LED4_NUM;                                           //�ߵ�ƽ��led_status bit3��1
    else
        led_status &= ~LED4_NUM;                                          //�͵�ƽ��led_status bit3��0

    if(GPIO_ReadOutputDataBit(LEDR_GPIO, LEDR_PIN))                 //�ж�PB0���ŵ�ƽ
        led_status |= LEDR_NUM;                                         //�ߵ�ƽ��led_status bit4��1
    else
        led_status &= ~LEDR_NUM;                                        //�͵�ƽ��led_status bit4��0

    if(GPIO_ReadOutputDataBit(LEDG_GPIO, LEDG_PIN))                 //�ж�PB1���ŵ�ƽ
        led_status |= LEDG_NUM;                                         //�ߵ�ƽ��led_status bit5��1
    else
        led_status &= ~LEDG_NUM;                                        //�͵�ƽ��led_status bit5��0

    if(GPIO_ReadOutputDataBit(LEDB_GPIO, LEDB_PIN))                 //�ж�PB2���ŵ�ƽ
        led_status |= LEDB_NUM;                                         //�ߵ�ƽ��led_status bit6��1
    else
        led_status &= ~LEDB_NUM;                                        //�͵�ƽ��led_status bit6��0
    return led_status;                                            //����led_status
}


