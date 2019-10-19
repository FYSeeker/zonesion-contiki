#include "heartrate.h"

short HeartrateValue = 0;
clock_time_t currentTime=0,lastTime=0;

static void (*_call)(void);
void EXTI1_Register_Call(unsigned int line, void (*fun)(void))
{
    if (line & EXTI_Line1) _call = fun;
}

void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1)) 
    {
        if (_call != NULL) _call();
    }
    EXTI_ClearITPendingBit(EXTI_Line1);
}

//��ʼ�����ʴ�����
void  heartrate_io_Init()
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);       //ʹ��GPIOCʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);      //ʹ��SYSCFGʱ��

    //�ȳ�ʼ�� ���ʵ�ƽ����IO��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                //����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;             //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;          //100M
    GPIO_Init(GPIOC, &GPIO_InitStructure);                      //��ʼ��

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);//����GPIO���ж���

    //�����ж���
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;                   //�����ж���Ϊ���ⲿ�ж�1
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //����Ϊ�ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //����Ϊ�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //ʹ���ⲿ�ж�
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ⲿ�ж�1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����

    EXTI1_Register_Call(EXTI_Line1, LINE1_IRQ);
}

short heartrate_get()
{
    currentTime = clock_time();
    if((currentTime-lastTime)>5000)
    {
        HeartrateValue=0;
    }
    return HeartrateValue;
}

char arrangement_size_min(u32* a, u32* b)
{
    u32 temp;
    if(*a > *b)
    {
        temp = *a;
        *a = *b;
        *b = temp;
        return 1;
    }
    return 0;
}

#define REMTIMES                10                              //��¼��������        
#define INVALIDS                2                               //��Ч��������        
#define FAULT_TOLERANCE         4                               //����ݴ����

static void LINE1_IRQ(void)
{
    static unsigned int lst, flag = 0, t1 = 0, t2 = 0;
    static uint32_t rate[REMTIMES], ratetimes = 0;                //�����������ݻ���
    unsigned int ct, ratetemp;
    char i,compare_check;

    ct =  clock_time();
    t1 = ct - lst;
    lst = ct;
    
    if (t1 > 600 && t1 < 1200)
    {
        if(t2 == 0) t2 = t1;
        if((t2 > t1 ? t2 - t1 : t1 - t2) < 100)
        {
            rate[ratetimes] = 60000 / t1;				//��ȡ��ǰ�ɼ�����ֵ
            ratetimes ++;
            flag = 0;
            if(ratetimes >= REMTIMES)
            {
                do
                {
                    compare_check = 0;
                    for(i = 0; i < REMTIMES - 1; i++)
                    {
                        compare_check += arrangement_size_min(&rate[i],&rate[i+1]);
                    }
                }
                while(compare_check != 0);
                for(i = INVALIDS / 2; i < REMTIMES - INVALIDS / 2; i ++)
                {
                    ratetemp += rate[i];
                }
                HeartrateValue = ratetemp / (REMTIMES - INVALIDS);
                ratetimes = 0;
            }
            t2 = t1;
        }
        else flag ++;
        if(flag >= FAULT_TOLERANCE)
        {
            t2 = 0;
            flag = 0;
        }
    }
    else flag ++;
    if(flag >= FAULT_TOLERANCE)
    {
        t2 = 0;
        flag = 0;
    }
    lastTime = clock_time();
}

