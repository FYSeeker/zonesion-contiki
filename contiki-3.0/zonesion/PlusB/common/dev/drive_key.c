/*********************************************************************************************
* �ļ���key.c
* ���ߣ�chennian 2017.11.02
* ˵����K1 ��Ӧ���� PE12
*       K2 ��Ӧ�ܽ� PE13
*       K3 ��Ӧ���� PE14
*       K4 ��Ӧ�ܽ� PE15
* ���ܣ�������������
* �޸ģ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "drive_key.h"

/*********************************************************************************************
* ���ƣ�key_init
* ���ܣ������ܽų�ʼ��
* ��������
* ���أ���
* �޸ģ���
*********************************************************************************************/
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                          //����һ��GPIO_InitTypeDef���͵Ľṹ��
    RCC_AHB1PeriphClockCmd( K1_CLK  | K2_CLK|K3_CLK | K4_CLK, ENABLE);             //����KEY��ص�GPIO����ʱ��
    
    GPIO_InitStructure.GPIO_Pin = K1_PIN | K2_PIN |K3_PIN | K4_PIN;                //ѡ��Ҫ���Ƶ�GPIO����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                //�������ŵ��������Ϊ�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                  //��������ģʽΪ���ģʽ
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //��������Ϊ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              //������������Ϊ2MHz
    
    GPIO_Init(K1_PORT, &GPIO_InitStructure);                      //��ʼ��GPIO����
    GPIO_Init(K2_PORT, &GPIO_InitStructure);                      //��ʼ��GPIO����
    GPIO_Init(K3_PORT, &GPIO_InitStructure);                      //��ʼ��GPIO����
    GPIO_Init(K4_PORT, &GPIO_InitStructure);                      //��ʼ��GPIO����
}
void KeyExitxConfig(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
    
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);//PB12 ���ӵ��ж���12
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource13);//PB13 ���ӵ��ж���13
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14);//PB14 ���ӵ��ж���14
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);//PB15 ���ӵ��ж���15
    /* ����EXTI_Line0 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line12 | EXTI_Line13|EXTI_Line14 | EXTI_Line15;//LINE/3
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش��� 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE2/3
    EXTI_Init(&EXTI_InitStructure);//����
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����
    //NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ⲿ�ж�1
    //NVIC_Init(&NVIC_InitStructure);//����
}
/*********************************************************************************************
* ���ƣ�get_key_status
* ���ܣ������ܽų�ʼ��
* ��������
* ���أ�key_status
* �޸ģ�
*********************************************************************************************/
char get_key_status(void)
{
    char key_status = 0;
    if(GPIO_ReadInputDataBit(K1_PORT,K1_PIN) == 0)                //�ж�PB12���ŵ�ƽ״̬
        key_status |= K1_PREESED;                                   //�͵�ƽkey_status bit0λ��1
    if(GPIO_ReadInputDataBit(K2_PORT,K2_PIN) == 0)                //�ж�PB13���ŵ�ƽ״̬
        key_status |= K2_PREESED;                                   //�͵�ƽkey_status bit1λ��1
    if(GPIO_ReadInputDataBit(K3_PORT,K3_PIN) == 0)                //�ж�PB14���ŵ�ƽ״̬
        key_status |= K3_PREESED;                                   //�͵�ƽkey_status bit2λ��1
    if(GPIO_ReadInputDataBit(K4_PORT,K4_PIN) == 0)                //�ж�PB15���ŵ�ƽ״̬
        key_status |= K4_PREESED;                                   //�͵�ƽkey_status bit3λ��1
    return key_status;
}

static void (*call)(int) = NULL;

void KeySetCallback(void (*f)(int k))
{
    call = f;
}
void EXTI15_10_IRQHandler(void)
{
    if (call != NULL) call(1);
    if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
        EXTI_ClearITPendingBit(EXTI_Line12);                        //���LINE14�ϵ��жϱ�־λ
    if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
        EXTI_ClearITPendingBit(EXTI_Line13);                        //���LINE15�ϵ��жϱ�־λ
    if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
        EXTI_ClearITPendingBit(EXTI_Line14);                        //���LINE14�ϵ��жϱ�־λ
    if(EXTI_GetITStatus(EXTI_Line15)!=RESET)
        EXTI_ClearITPendingBit(EXTI_Line15);                        //���LINE15�ϵ��жϱ�־λ
}
